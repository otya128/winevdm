#include "haxmvm.h"

#include "../krnl386/kernel16_private.h"

BOOL initflag;
UINT8 *mem;
#define KRNL386 "krnl386.exe16"
BOOL is_single_step = FALSE;
DWORD WINAPI panic_msgbox(LPCVOID data)
{
    MessageBoxA(NULL, (LPCSTR)data, "Hypervisor error", MB_OK | MB_ICONERROR);
    HeapFree(GetProcessHeap(), 0, data);
    return 0;
}
void haxmvm_panic(const char *fmt, ...)
{
    LPSTR buffer = HeapAlloc(GetProcessHeap(), 0, 512);
    DWORD threadId;
    va_list arg;

    va_start(arg, fmt);
    vsnprintf(buffer, 512, fmt, arg);
    va_end(arg);
    buffer[512 - 1] = '\0';
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)panic_msgbox, buffer, 0, &threadId);
    WaitForSingleObject(hThread, INFINITE);
    ExitThread(1);
}
PVOID dynamic_setWOW32Reserved(PVOID w)
{
    static PVOID(*setWOW32Reserved)(PVOID);
    if (!setWOW32Reserved)
    {
        HMODULE krnl386 = LoadLibraryA(KRNL386);
        setWOW32Reserved = (PVOID(*)(PVOID))GetProcAddress(krnl386, "setWOW32Reserved");
    }
    return setWOW32Reserved(w);
}
PVOID dynamic_getWOW32Reserved()
{
    static PVOID(*getWOW32Reserved)();
    if (!getWOW32Reserved)
    {
        HMODULE krnl386 = LoadLibraryA(KRNL386);
        getWOW32Reserved = (PVOID(*)())GetProcAddress(krnl386, "getWOW32Reserved");
    }
    return getWOW32Reserved();
}
WINE_VM86_TEB_INFO *dynamic_getGdiTebBatch()
{
    static WINE_VM86_TEB_INFO*(*getGdiTebBatch)();
    if (!getGdiTebBatch)
    {
        HMODULE krnl386 = LoadLibraryA(KRNL386);
        getGdiTebBatch = (WINE_VM86_TEB_INFO*(*)())GetProcAddress(krnl386, "getGdiTebBatch");
    }
    return getGdiTebBatch();
}
void dynamic__wine_call_int_handler(CONTEXT *context, BYTE intnum)
{
    static void(*__wine_call_int_handler)(CONTEXT *context, BYTE intnum);
    if (!__wine_call_int_handler)
    {
        HMODULE krnl386 = LoadLibraryA(KRNL386);
        __wine_call_int_handler = (void(*)(CONTEXT *context, BYTE intnum))GetProcAddress(krnl386, "__wine_call_int_handler");
    }
    __wine_call_int_handler(context, intnum);
}
/***********************************************************************
*           SELECTOR_SetEntries
*
* Set the LDT entries for an array of selectors.
*/
static BOOL SELECTOR_SetEntries(WORD sel, const void *base, DWORD size, unsigned char flags)
{
    LDT_ENTRY entry;
    WORD i, count;

    wine_ldt_set_base(&entry, base);
    wine_ldt_set_limit(&entry, size - 1);
    wine_ldt_set_flags(&entry, flags);
    count = (size + 0xffff) / 0x10000;
    for (i = 0; i < count; i++)
    {
        if (wine_ldt_set_entry(sel + (i << 3), &entry) < 0) return FALSE;
        wine_ldt_set_base(&entry, (char*)wine_ldt_get_base(&entry) + 0x10000);
        /* yep, Windows sets limit like that, not 64K sel units */
        wine_ldt_set_limit(&entry, wine_ldt_get_limit(&entry) - 0x10000);
    }
    return TRUE;
}
void wine_ldt_free_entries(unsigned short sel, int count);
/***********************************************************************
*           SELECTOR_AllocBlock
*
* Allocate selectors for a block of linear memory.
*/
WORD SELECTOR_AllocBlock(const void *base, DWORD size, unsigned char flags)
{
    WORD sel, count;

    if (!size) return 0;
    count = (size + 0xffff) / 0x10000;
    if ((sel = wine_ldt_alloc_entries(count)))
    {
        if (SELECTOR_SetEntries(sel, base, size, flags)) return sel;
        wine_ldt_free_entries(sel, count);
        sel = 0;
    }
    return sel;
}
_declspec(dllimport) LDT_ENTRY wine_ldt[8192];

HANDLE hSystem;
HANDLE hVM;
HANDLE hVCPU;
struct hax_tunnel *tunnel;
 // 2MB is enough for 0x00000000-0x7fffffff plus 1 page for the pagedir 
#ifdef _MSC_VER
__declspec(align(4096))
#endif
DWORD guestpt[0x80400] = {0}
#ifdef __GNUC__
__attribute__ ((aligned(4096)))
#endif
;

void load_seg(segment_desc_t *segment, WORD sel)
{
    segment->selector = sel;
    segment->base = (uint64)wine_ldt_get_base(wine_ldt + (sel >> 3));
    segment->limit = 0xffffffff;// (uint64)wine_ldt_get_limit(wine_ldt + (sel >> 3));
    segment->type = wine_ldt[sel >> 3].HighWord.Bits.Type;
    segment->present = wine_ldt[sel >> 3].HighWord.Bits.Pres;
    segment->operand_size = wine_ldt[sel >> 3].HighWord.Bits.Default_Big;
    segment->dpl = wine_ldt[sel >> 3].HighWord.Bits.Dpl;
    segment->granularity = wine_ldt[sel >> 3].HighWord.Bits.Granularity;
    if (!sel || !segment->type || !segment->present)
        segment->ar = 0;
}
 
void set_eflags(struct vcpu_state_t *state, DWORD eflags)
{
    state->_eflags = eflags | 2 | 0x3000 | (is_single_step ? 0x100 : 0);
}
void load_context_to_state(CONTEXT *context, struct vcpu_state_t *state)
{
    DWORD bytes;
    load_seg(&state->_gs, (WORD)context->SegGs);
    load_seg(&state->_fs, (WORD)context->SegFs);
    load_seg(&state->_es, (WORD)context->SegEs);
    load_seg(&state->_ds, (WORD)context->SegDs);
    load_seg(&state->_cs, (WORD)context->SegCs);
    load_seg(&state->_ss, (WORD)context->SegSs);

    state->_edi = context->Edi;
    state->_esi = context->Esi;
    state->_ebx = context->Ebx;
    state->_edx = context->Edx;
    state->_ecx = context->Ecx;
    state->_eax = context->Eax;

    state->_ebp = context->Ebp;
    state->_eip = context->Eip;
    set_eflags(state, context->EFlags);
    state->_esp = context->Esp;
}

void load_context(CONTEXT *context)
{
    DWORD bytes;
    struct vcpu_state_t state;
    if (!DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL))
        return;
    load_seg(&state._gs, (WORD)context->SegGs);
    load_seg(&state._fs, (WORD)context->SegFs);
    load_seg(&state._es, (WORD)context->SegEs);
    load_seg(&state._ds, (WORD)context->SegDs);
    load_seg(&state._cs, (WORD)context->SegCs);
    load_seg(&state._ss, (WORD)context->SegSs);

    state._edi = context->Edi;
    state._esi = context->Esi;
    state._ebx = context->Ebx;
    state._edx = context->Edx;
    state._ecx = context->Ecx;
    state._eax = context->Eax;

    state._ebp = context->Ebp;
    state._eip = context->Eip;
    set_eflags(&state, context->EFlags);
    state._esp = context->Esp;

    if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state, sizeof(state), NULL, 0, &bytes, NULL))
        return;
}

void save_context_from_state(CONTEXT *context, struct vcpu_state_t *state)
{
    context->SegGs = state->_gs.selector;
    context->SegFs = state->_fs.selector;
    context->SegEs = state->_es.selector;
    context->SegDs = state->_ds.selector;
    context->SegCs = state->_cs.selector;
    context->SegSs = state->_ss.selector;

    context->Edi = state->_edi;
    context->Esi = state->_esi;
    context->Ebx = state->_ebx;
    context->Edx = state->_edx;
    context->Ecx = state->_ecx;
    context->Eax = state->_eax;

    context->Ebp = state->_ebp;
    context->Eip = state->_eip;
    context->EFlags = state->_eflags & ~2;
    context->Esp = state->_esp;
    dynamic_setWOW32Reserved((PVOID)(state->_ss.selector << 16 | state->_sp));
}
void save_context(CONTEXT *context)
{
    DWORD bytes;
    struct vcpu_state_t state;
    if (!DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL))
        return;
    save_context_from_state(context, &state);
    context->EFlags &= ~0x200;
}
#include <pshpack1.h>
typedef enum
{
    INT_GATE_TASK = 5,
    INT_GATE_INT16 = 6,
    INT_GATE_TRAP16 = 7,
    INT_GATE_INT32 = 0xE,
    INT_GATE_TRAP32 = 0xF,
} interrupt_gate_type;
typedef struct
{
    WORD offset_low;
    WORD selector;
    BYTE reserved;
    union
    {
        struct
        {
            BYTE type : 4 /* INT_GATE_TASK */, S : 1, DPL : 2, P : 1;
        };
        BYTE data;
    };
    WORD offset_high;
} interrupt_gate;
_STATIC_ASSERT(sizeof(interrupt_gate) == 8);
#include <poppack.h>

#define HAXMVM_STR2(s) #s
#define HAXMVM_STR(s) HAXMVM_STR2(s)
#define HAXMVM_ERR fprintf(stderr, __FUNCTION__ "("  HAXMVM_STR(__LINE__)  ") HAXM err.\n");
#define HAXMVM_ERRF(fmt, ...) fprintf(stderr, __FUNCTION__ "("  HAXMVM_STR(__LINE__)  ") " fmt "\n", __VA_ARGS__);
LPVOID trap_int;

#ifdef _MSC_VER
__declspec(align(4096))
#endif
interrupt_gate idt[256];
#ifdef __GNUC__
__attribute__ ((aligned(4096)))
#endif
;
#ifdef _MSC_VER
__declspec(align(4096))
#endif
LDT_ENTRY gdt[512] = { 0 };
#ifdef __GNUC__
__attribute__ ((aligned(4096)))
#endif
;
WORD seg_cs;
WORD seg_ds;

static BOOL set_ram(struct hax_set_ram_info *ram)
{
    DWORD bytes;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, ram, sizeof(struct hax_set_ram_info), NULL, 0, &bytes, NULL))
        return FALSE;
    DWORD physaddr = ram->pa_start / 4096;
    for (DWORD i = 0; i < (ram->size / 4096); i++)
    {
        DWORD pte = physaddr + i;
        guestpt[pte] = (pte << 12) | 7;
    }
    return TRUE;
}


BOOL init_vm86(BOOL vm86)
{
    ((void(*)())GetProcAddress(GetModuleHandleA("libwine"), "set_intel_vt_x_workaround"))();
    __asm
    {
        mov seg_cs, cs
        mov seg_ds, ds
    }
    hSystem = CreateFileW(L"\\\\.\\HAX", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSystem == INVALID_HANDLE_VALUE)
    {
        HAXMVM_ERRF("HAXM is not installed.\n");
        return FALSE;
    }
    struct hax_module_version ver;
    DWORD bytes;
    if (!DeviceIoControl(hSystem, HAX_IOCTL_VERSION, NULL, NULL, &ver, sizeof(ver), &bytes, NULL))
    {
        HAXMVM_ERRF("VERSION");
        return FALSE;
    }
    uint32_t vm_id;
    if (!DeviceIoControl(hSystem, HAX_IOCTL_CREATE_VM, NULL, NULL, &vm_id, sizeof(vm_id), &bytes, NULL))
    {
        HAXMVM_ERRF("CREATE_VM");
        return FALSE;
    }
    WCHAR buf[1000];
    swprintf_s(buf, RTL_NUMBER_OF(buf), L"\\\\.\\hax_vm%02d", vm_id);
    hVM = CreateFileW(buf, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hVM == INVALID_HANDLE_VALUE)
    {
        HAXMVM_ERRF("Could not create vm.");
        return FALSE;
    }
    uint32_t vcpu_id;
    struct hax_qemu_version verq;
    /* 3~ enable fast mmio */
    verq.cur_version = 1;
    verq.least_version = 0;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_NOTIFY_QEMU_VERSION, &verq, sizeof(verq), NULL, 0, &bytes, NULL))
    {
    }
    vcpu_id = 1;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_VCPU_CREATE, &vcpu_id, sizeof(vcpu_id), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("could not create vcpu.");
        return FALSE;
    }
    swprintf_s(buf, RTL_NUMBER_OF(buf), L"\\\\.\\hax_vm%02d_vcpu%02d", vm_id, vcpu_id);
    hVCPU = CreateFileW(buf, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    struct hax_tunnel_info tunnel_info;
    if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_SETUP_TUNNEL, NULL, 0, &tunnel_info, sizeof(tunnel_info), &bytes, NULL))
    {
        HAXMVM_ERRF("SETUP_TUNNEL");
        return FALSE;
    }
    /* memory mapping */
    struct hax_alloc_ram_info alloc_ram = { 0 };
    struct hax_set_ram_info ram = { 0 };
    MEMORY_BASIC_INFORMATION mbi = { 0 };
    trap_int = VirtualAlloc(NULL, 0x10000, MEM_COMMIT, PAGE_READWRITE);
    alloc_ram.size = 0x201000;
    alloc_ram.va = (uint64_t)guestpt;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("ALLOC_RAM");
        return FALSE;
    }
    ram.pa_start = (uint64_t)guestpt;
    ram.size = 0x201000;
    ram.va = (uint64_t)guestpt;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, &ram, sizeof(ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("SET_RAM");
        return FALSE;
    }
    // fill in the pagedir
    for (int i = 0; i < 512; i++)
        guestpt[0x80000 + i] = ((DWORD)guestpt + 4096 * i) | 7;
    while (!TRUE)
    {
        alloc_ram.size = mbi.RegionSize;
        VirtualQuery((PVOID)((SIZE_T)mbi.BaseAddress + mbi.RegionSize), &mbi, sizeof(mbi));
        if (!mbi.RegionSize)
            break;
        alloc_ram.va = (SIZE_T)mbi.BaseAddress;
        alloc_ram.size = (SIZE_T)mbi.RegionSize;
        if (!alloc_ram.va)
            continue;
        if (0&&mbi.State != MEM_COMMIT)
        {
            if ((SIZE_T)mbi.BaseAddress + mbi.RegionSize < (SIZE_T)mbi.BaseAddress)
            {
                break;
            }
            continue;
        }
        //MmProbeAndLockPages(xx, xx , IoReadAccess|IoWriteAccess) fails
        if (0 && mbi.State == MEM_COMMIT)
        {
            DWORD old;
            if (mbi.Protect & PAGE_READONLY)
            {
                VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &old);
            }
            if ((mbi.Protect & PAGE_EXECUTE_READ) || (mbi.Protect & PAGE_EXECUTE))
            {
                VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &old);
            }
        }
        if (mbi.State == MEM_COMMIT)
        {
            DWORD old;
            if (mbi.Protect & PAGE_READONLY)
            {
                continue;
            }
            if ((mbi.Protect & PAGE_EXECUTE_READ) || (mbi.Protect & PAGE_EXECUTE))
            {
                continue;
            }
        }
        if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
        {
            HAXMVM_ERR;
        }
        if ((SIZE_T)mbi.BaseAddress + mbi.RegionSize < (SIZE_T)mbi.BaseAddress)
        {
            break;
        }
        ram.pa_start = (SIZE_T)mbi.BaseAddress;
        ram.size = (SIZE_T)mbi.RegionSize;
        ram.va = (SIZE_T)mbi.BaseAddress;
        if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, &ram, sizeof(ram), NULL, 0, &bytes, NULL))
        {
            HAXMVM_ERR;
            return FALSE;
        }
    }
    tunnel = (struct hax_tunnel*)tunnel_info.va;
    struct vcpu_state_t state;
    if (!DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL))
    {
        HAXMVM_ERRF("GET_REGS");
        return FALSE;
    }
    /* setup initial states */
    state._gdt.base = (uint64)gdt;
    state._gdt.limit = 0x7ff;
    DWORD trap_addr = (DWORD)trap_int;
    gdt[1].BaseLow = trap_addr & 0xffff;
    gdt[1].HighWord.Bytes.BaseMid = (trap_addr >> 16) & 0xff;
    gdt[1].HighWord.Bytes.BaseHi = (trap_addr >> 24) & 0xff;
    gdt[1].LimitLow = 0xffff;
    gdt[1].HighWord.Bytes.Flags1 = 0x9b;
    gdt[1].HighWord.Bytes.Flags2 = 0x40;
    gdt[2].BaseLow = trap_addr & 0xffff;
    gdt[2].HighWord.Bytes.BaseMid = (trap_addr >> 16) & 0xff;
    gdt[2].HighWord.Bytes.BaseHi = (trap_addr >> 24) & 0xff;
    gdt[2].LimitLow = 0xffff;
    gdt[2].HighWord.Bytes.Flags1 = 0x93;
    gdt[2].HighWord.Bytes.Flags2 = 0x40;
    gdt[4].BaseLow = 0;
    gdt[4].HighWord.Bytes.BaseMid = 0;
    gdt[4].HighWord.Bytes.BaseHi = 0;
    gdt[4].LimitLow = 0xffff;
    gdt[4].HighWord.Bytes.Flags1 = 0xfb;
    gdt[4].HighWord.Bytes.Flags2 = 0xcf;

    state._ldt.selector = 0x18;
    state._ldt.base = (uint64)&wine_ldt[0];
    state._ldt.limit = 65535;
    
    UINT32 *tss = (UINT32 *)gdt + 0x200;
    state._tr.selector = 0x18;
    state._tr.base = (uint64)tss;
    state._tr.limit = 0x64;
    state._tr.ar = 0x8b;
    tss[1] = 0x10000; // SP0
    tss[2] = 0x10; // SS0

    state._idt.limit = 0x8 * 256 - 1;
    state._idt.base = (SIZE_T)&idt[0];
    state._cr3 = guestpt + 0x80000;
    state._cr0 |= 0x80000001;
    state._eflags |= 0x200;
    for (int i = 0; i < 256; i++)
    {
        idt[i].DPL = 3;
        idt[i].type = INT_GATE_INT32;
        idt[i].selector = 0x0b;
        idt[i].P = 1;
        idt[i].offset_low = i;
        idt[i].offset_high = 0;
    }
    memset(trap_int, 0xF4, 256); /* hlt */
    ((char *)trap_int)[256] = 0xcf; /* iret */
    alloc_ram.size = 4096;
    alloc_ram.va = (uint64_t)&idt;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("ALLOC_RAM");
        return FALSE;
    }
    ram.pa_start =  (uint64_t)&idt;
    ram.size = 4096;
    ram.va =  (uint64_t)&idt;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM\n");
        return FALSE;
    }
    alloc_ram.size = 65536 + 4096;
    alloc_ram.va = (uint64_t)&wine_ldt & ~0xfff;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("ALLOC_RAM");
        return FALSE;
    }
    ram.pa_start =  (uint64_t)&wine_ldt & ~0xfff;
    ram.size = 65536 + 4096;
    ram.va =  (uint64_t)&wine_ldt & ~0xfff;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM\n");
        return FALSE;
    }
    alloc_ram.size = 0x10000;
    alloc_ram.va = (uint64_t)trap_int;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("ALLOC_RAM");
        return FALSE;
    }
    ram.pa_start = (uint64_t)trap_int;
    ram.size = 0x10000;
    ram.va = (uint64_t)trap_int;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM");
        return FALSE;
    }
    alloc_ram.size = 0x1000;
    alloc_ram.va = (uint64_t)&gdt;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("ALLOC_RAM");
        return FALSE;
    }
    ram.pa_start = (uint64_t)&gdt;
    ram.size = 0x1000;
    ram.va = (uint64_t)&gdt;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM");
        return FALSE;
    }
    if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state, sizeof(state), NULL, 0, &bytes, NULL))
    {
        HAXMVM_ERRF("SET_REGS");
        return FALSE;
    }
    return TRUE;
}

BOOL vm_exit()
{
    CloseHandle(hVCPU);
    CloseHandle(hVM);
    CloseHandle(hSystem);
    return TRUE;
}
void PUSH16(struct vcpu_state_t *state, WORD val)
{
    state->_esp -= 2;
    unsigned char *stack = (unsigned char*)(state->_ss.base + state->_esp);
    *(LPWORD)stack = val;
}
void PUSH32(struct vcpu_state_t *state, DWORD val)
{
    state->_esp -= 4;
    unsigned char *stack = (unsigned char*)(state->_ss.base + state->_esp);
    *(LPDWORD)stack = val;
}
WORD POP16(struct vcpu_state_t *state)
{
    LPWORD stack = (LPWORD)(state->_ss.base + state->_esp);
    state->_esp += 2;
    return *stack;
}
DWORD POP32(struct vcpu_state_t *state)
{
    LPDWORD stack = (LPDWORD)(state->_ss.base + state->_esp);
    state->_esp += 4;
    return *stack;
}
WORD PEEK16(struct vcpu_state_t *state, int i)
{
    LPWORD stack = (LPWORD)(state->_ss.base + state->_esp);
    return stack[i];
}
DWORD PEEK32(struct vcpu_state_t *state, int i)
{
    LPDWORD stack = (LPDWORD)(state->_ss.base + state->_esp);
    return stack[i];
}
void relay(LPVOID relay_func, BOOL reg, struct vcpu_state_t *state)
{
    unsigned char *stack1 = (unsigned char*)(state->_ss.base + state->_esp);
    unsigned char *stack = stack1;
    /*
    * (sp+24) word   first 16-bit arg
    * (sp+22) word   cs
    * (sp+20) word   ip
    * (sp+18) word   bp
    * (sp+14) long   32-bit entry point (reused for Win16 mutex recursion count)
    * (sp+12) word   ip of actual entry point (necessary for relay debugging)
    * (sp+8)  long   relay (argument conversion) function entry point
    * (sp+4)  long   cs of 16-bit entry point
    * (sp)    long   ip of 16-bit entry point
    */
    DWORD ip = *(DWORD*)stack;
    stack += sizeof(DWORD);
    DWORD cs = *(DWORD*)stack;
    stack += sizeof(DWORD);
    DWORD relay = *(DWORD*)stack;
    stack += sizeof(DWORD);
    WORD ip2 = *(WORD*)stack;
    stack += sizeof(WORD);
    DWORD entry = *(DWORD*)stack;
    //for debug
    void *entryf = (void*)entry;
    stack += sizeof(DWORD);
    WORD bp = *(WORD*)stack;
    stack += sizeof(WORD);
    WORD ip19 = *(WORD*)stack;
    stack += sizeof(WORD);
    WORD cs16 = *(WORD*)stack;
    stack += sizeof(WORD);
    WORD *args = (WORD*)stack;
    state->_eip = ip;
    load_seg(&state->_cs, (WORD)cs);
#include <pshpack1.h>
    /* 16-bit stack layout after __wine_call_from_16() */
    typedef struct _STACK16FRAME
    {
        struct STACK32FRAME *frame32;        /* 00 32-bit frame from last CallTo16() */
        DWORD         edx;            /* 04 saved registers */
        DWORD         ecx;            /* 08 */
        DWORD         ebp;            /* 0c */
        WORD          ds;             /* 10 */
        WORD          es;             /* 12 */
        WORD          fs;             /* 14 */
        WORD          gs;             /* 16 */
        DWORD         callfrom_ip;    /* 18 callfrom tail IP */
        DWORD         module_cs;      /* 1c module code segment */
        DWORD         relay;          /* 20 relay function address */
        WORD          entry_ip;       /* 22 entry point IP */
        DWORD         entry_point;    /* 26 API entry point to call, reused as mutex count */
        WORD          bp;             /* 2a 16-bit stack frame chain */
        WORD          ip;             /* 2c return address */
        WORD          cs;             /* 2e */
    } STACK16FRAME;
#include <poppack.h>
    CONTEXT context;
    DWORD osp = state->_esp;
    PUSH16(state, state->_gs.selector);
    PUSH16(state, state->_fs.selector);
    PUSH16(state, state->_es.selector);
    PUSH16(state, state->_ds.selector);
    PUSH32(state, state->_ebp);
    PUSH32(state, state->_ecx);
    PUSH32(state, state->_edx);
    PUSH32(state, osp);
    save_context_from_state(&context, state);
    STACK16FRAME *oa = (STACK16FRAME*)wine_ldt_get_ptr((WORD)context.SegSs, context.Esp);
    DWORD ooo = (WORD)context.Esp;
    int fret;
    DWORD bytes;
    /*if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, state, sizeof(*state), NULL, 0, &bytes, NULL))
        HAXMVM_ERRF("SET_REGS");*/
    /*
    typedef void(*vm_debug_get_entry_point_t)(char *module, char *func, WORD *ordinal);
    static vm_debug_get_entry_point_t vm_debug_get_entry_point;
    if (!vm_debug_get_entry_point)
    {
        vm_debug_get_entry_point = (vm_debug_get_entry_point_t)GetProcAddress(LoadLibraryA(KRNL386), "vm_debug_get_entry_point");
    }
    char module[100], func[100];
    WORD ordinal = 0;
    vm_debug_get_entry_point(module, func, &ordinal);
    fprintf(stderr, "call built-in func %s.%d: %s ESP %04X\n", module, ordinal, func, context.Esp);
    */
    if ((DWORD)relay_func != relay)
    {
        fret = ((int(*)(void *entry_point, unsigned char *args16, CONTEXT *context))relay_func)((void*)entry, (unsigned char*)args, &context);
    }
    else
        fret = ((int(*)(void *, unsigned char *, CONTEXT *))relay)((void*)entry, (unsigned char*)args, &context);
    if (!reg)
    {
        state->_eax = fret;
    }
    oa = (STACK16FRAME*)wine_ldt_get_ptr((WORD)context.SegSs, context.Esp);
    if (reg)
        state->_eax = (DWORD)context.Eax;
    state->_ecx = reg ? (DWORD)context.Ecx : (DWORD)oa->ecx;
    if (reg)
        state->_edx = (DWORD)context.Edx;
    else
        state->_edx = (DWORD)oa->edx;
    state->_ebx = (DWORD)context.Ebx;
    state->_esp = (DWORD)context.Esp;
    state->_ebp = (DWORD)context.Ebp;
    state->_esi = (DWORD)context.Esi;
    state->_edi = (DWORD)context.Edi;
    state->_esp = osp + 18 + 2;
    state->_esp -= (ooo - context.Esp);
    state->_bp = bp;
    set_eflags(state, context.EFlags);
    load_seg(&state->_es, reg ? (WORD)context.SegEs : (WORD)oa->es);
    load_seg(&state->_ss, (WORD)context.SegSs);
    load_seg(&state->_ds, reg ? (WORD)context.SegDs : (WORD)oa->ds);
    load_seg(&state->_fs, (WORD)context.SegFs);
    load_seg(&state->_gs, (WORD)context.SegGs);
    state->_eip = context.Eip;
    load_seg(&state->_cs, (WORD)context.SegCs);
}

BOOL syscall_init = FALSE;
LPBYTE syscall_trap = FALSE;
#define dprintf(...)// printf(__VA_ARGS__)
void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    pm_interrupt_handler pih
)
{
    if (!initflag)
    {
        haxmvm_panic("Could not initialize the hypervisor.\nHAXM may not be installed.\n");
    }
    if (tunnel->_exit_status == HAX_EXIT_STATECHANGE)
    {
        haxmvm_panic("hypervisor is panicked!!!");
    }
    if (!syscall_init)
    {
        SIZE_T page1 = (SIZE_T)from16_reg / 4096 * 4096;
        SIZE_T page2 = (SIZE_T)__wine_call_from_16 / 4096 * 4096;
        LPBYTE trap = syscall_trap = (LPBYTE)VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
        memset(trap, 0xEE, 4095); /* out forces a vmexit from user mode without modifing any registers */
        struct hax_alloc_ram_info alloc_ram = { 0 };
        struct hax_set_ram_info ram = { 0 };
        alloc_ram.size = 4096;
        alloc_ram.va = trap;
        DWORD bytes;
        if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
        {
            HAXMVM_ERRF("ALLOC_RAM");
        }
        ram.pa_start = (SIZE_T)trap;
        ram.size = (SIZE_T)4096;
        ram.va = (SIZE_T)trap;
        if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, &ram, sizeof(ram), NULL, 0, &bytes, NULL))
        {
            HAXMVM_ERRF("SET_RAM\n");
        }
        guestpt[page1 >> 12] = (DWORD)trap | 7;
        guestpt[page2 >> 12] = (DWORD)trap | 7;
        syscall_init = TRUE;
    }
    //is_single_step = TRUE;
    MEMORY_BASIC_INFORMATION mbi;
    DWORD bytes;
    DWORD ret_addr;
    {

        DWORD bytes;
        struct vcpu_state_t state;
        if (!DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL))
            HAXMVM_ERRF("GET_REGS");
        load_seg(&state._gs, (WORD)context->SegGs);
        load_seg(&state._fs, (WORD)context->SegFs);
        load_seg(&state._es, (WORD)context->SegEs);
        load_seg(&state._ds, (WORD)context->SegDs);
        load_seg(&state._cs, (WORD)context->SegCs);
        load_seg(&state._ss, (WORD)context->SegSs);

        state._edi = context->Edi;
        state._esi = context->Esi;
        state._ebx = context->Ebx;
        state._edx = context->Edx;
        state._ecx = context->Ecx;
        state._eax = context->Eax;

        state._ebp = context->Ebp;
        state._eip = context->Eip;
        set_eflags(&state, context->EFlags);
        state._esp = context->Esp - cbArgs;

        if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state, sizeof(state), NULL, 0, &bytes, NULL))
            HAXMVM_ERRF("SET_REGS");
        unsigned char *stack = (unsigned char*)state._ss.base + state._esp;
        ret_addr = (*(LPDWORD)stack) + 1;
    }

    struct hax_alloc_ram_info alloc_ram = { 0 };
    struct hax_set_ram_info ram = { 0 };
    struct vcpu_state_t state2;
    DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state2, sizeof(state2), &bytes, NULL);
    while (TRUE)
    {
        //DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state2, sizeof(state2), &bytes, NULL);
        dprintf("%04X:%04X(base:%04llX) ESP:%08X F:%08X\n", state2._cs.selector, state2._eip, state2._cs.base, state2._esp, state2._eflags);
        if (state2._cs.selector == (ret_addr >> 16) && state2._eip == (ret_addr & 0xFFFF))
        {
            if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
            {
                HAXMVM_ERRF("SET_REGS");
            }
            break;
        }
        if (is_single_step)
        {
            /* Debug exception */
            fprintf(stderr, "%04x:%04x EAX:%04x EDX:%04x EF:%04x %p\n", state2._cs.selector, state2._eip,
                state2._eax, state2._edx, state2._eflags, (LPBYTE)state2._cs.base + state2._eip);
        }
        if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_RUN, NULL, 0, NULL, 0, &bytes, NULL))
            return;
        DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state2, sizeof(state2), &bytes, NULL);
        dprintf("%04X:%04X(base:%04llX) ESP:%08X F:%08X\n", state2._cs.selector, state2._eip, state2._cs.base, state2._esp, state2._eflags);
        if (state2._cs.selector == (ret_addr >> 16) && state2._eip == (ret_addr & 0xFFFF))
        {
            if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
            {
                HAXMVM_ERRF("SET_REGS");
            }
            break;
        }
        LPVOID ptr = (LPBYTE)state2._cs.base + state2._eip;
        switch(tunnel->_exit_status)
        {
            case HAX_EXIT_IO:
                if (tunnel->io._direction == HAX_IO_OUT)
                {
                    LPBYTE ptr2 = (LPBYTE)ptr - 1;
                    BOOL is_reg = ptr2 == from16_reg;
                    if (is_reg || ptr2 == __wine_call_from_16)
                    {
                        relay(relay_call_from_16, is_reg, &state2);
                        if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
                        {
                            HAXMVM_ERRF("SET_REGS");
                        }
                        break;
                    }
	            }
                break;
            case HAX_EXIT_HLT:
                if (((DWORD)ptr >= (DWORD)trap_int) && ((DWORD)ptr <= ((DWORD)trap_int + 256)))
                {
                    int intvec = ((DWORD)ptr & 0xff) - 1;
                    state2._eip = 256;
                    if (intvec == 0x0e)
                    {
                        alloc_ram.size = 4096;
                        alloc_ram.va = state2._cr2 & ~0xfff;
                        if (!DeviceIoControl(hVM, HAX_VM_IOCTL_ALLOC_RAM, &alloc_ram, sizeof(alloc_ram), NULL, 0, &bytes, NULL))
                        {
                            HAXMVM_ERRF("ALLOC_RAM");
                        }
                        ram.pa_start = state2._cr2 & ~0xfff;
                        ram.size = 4096;
                        ram.va = state2._cr2 & ~0xfff;
                        if (!set_ram(&ram))
                        {
                            HAXMVM_ERRF("SET_RAM");
                        }
                        state2._esp += 4;
                    }
                    else if(intvec < 0x10)
                        pih(intvec, MAKESEGPTR(state2._cs.selector, state2._eip & 0xffff));
                    else
                    {
                            DWORD eip = POP32(&state2);
                            DWORD cs = POP32(&state2);
                            DWORD eflags = POP32(&state2);
                            DWORD esp = POP32(&state2);
                            DWORD ss = POP32(&state2);
                            load_seg(&state2._cs, (WORD)cs);
                            state2._eip = eip;
                            load_seg(&state2._ss, (WORD)ss);
                            state2._esp = esp;
                            PUSH16(&state2, (WORD)eflags);
                            PUSH16(&state2, (WORD)cs);
                            PUSH16(&state2, (WORD)eip);
                            CONTEXT ctx;
                            save_context_from_state(&ctx, &state2);
                            if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
                            {
                                    HAXMVM_ERRF("SET_REGS");
                            }
                            dynamic__wine_call_int_handler(&ctx, intvec);
                            load_context_to_state(&ctx, &state2);
                            POP16(&state2);
                            POP16(&state2);
                            POP16(&state2);
                     }
                }
                break;
            case HAX_EXIT_INTERRUPT: //TODO: vm_inject
                break;
            case HAX_EXIT_STATECHANGE:
            default:
                HAXMVM_ERRF("%04X:%04X(base:%04llX) ESP:%08X", state2._cs.selector, state2._eip, state2._cs.base, state2._esp);
                HAXMVM_ERRF("hypervisor is panicked!!!");
                haxmvm_panic("hypervisor is panicked!!!");
        }
        if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
        {
                HAXMVM_ERRF("SET_REGS");
        }
        save_context(context);
    }
}

__declspec(dllexport) DWORD wine_call_to_16_vm86(DWORD target, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    BOOL vm86,
    void *memory_base,
    pm_interrupt_handler pih)
{

    mem = vm86 ? (UINT8*)memory_base : NULL;
    if (!initflag)
        initflag = init_vm86(vm86);
    CONTEXT context;
    PVOID oldstack = dynamic_getWOW32Reserved();
    save_context(&context);
    //why??
    dynamic_setWOW32Reserved(oldstack);
    context.SegSs = ((size_t)dynamic_getWOW32Reserved() >> 16) & 0xFFFF;
    context.Esp = ((size_t)dynamic_getWOW32Reserved()) & 0xFFFF;
    context.SegCs = target >> 16;
    context.Eip = target & 0xFFFF;//i386_jmp_far(target >> 16, target & 0xFFFF);
    vm86main(&context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih);
    return context.Eax | context.Edx << 16;
}
__declspec(dllexport) void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    BOOL vm86,
    void *memory_base,
    pm_interrupt_handler pih
)
{
    mem = vm86 ? (UINT8*)memory_base : NULL;
    if (!initflag)
        initflag = init_vm86(vm86);
    vm86main(context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih);


}

SIZE_T base = 0;
SIZE_T x87func = 0x200 - 0x10;
void callx87(SIZE_T addr)
{
    DWORD bytes;
    struct vcpu_state_t state;
    DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL);
    state._rip = addr;
    state._cs.selector = seg_cs;
    state._cs.base = 0;
    DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state, sizeof(state), NULL, 0, &bytes, NULL);
    if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_RUN, NULL, 0, NULL, 0, &bytes, NULL))
        return;
    //DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL);

    if (tunnel->_exit_status == HAX_EXIT_STATECHANGE)
    {
        HAXMVM_ERRF("hypervisor is panicked!!!");
        haxmvm_panic("win87em: hypervisor is panicked!!!");
    }
}
/* x87 service functions */
void fldcw(WORD a)
{
    SIZE_T location = base + x87func + 1 * 0x10;
    callx87(location);
}
void wait()
{
    SIZE_T location = base + x87func + 2 * 0x10;
    callx87(location);
}
void fninit()
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
void fstcw(WORD* a)
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
void frndint()
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
void fclex()
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
void fsave(char* a)
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
void frstor(const char* a)
{
    SIZE_T location = base + x87func + 3 * 0x10;
    callx87(location);
}
typedef void(*fldcw_t)(WORD);
typedef void(*wait_t)();
typedef void(*fninit_t)();
typedef void(*fstcw_t)(WORD*);
typedef void(*frndint_t)();
typedef void(*fclex_t)();
typedef void(*fsave_t)(char*);
typedef void(*frstor_t)(const char*);
typedef struct
{
    fldcw_t fldcw;
    wait_t wait;
    fninit_t fninit;
    fstcw_t fstcw;
    frndint_t frndint;
    fclex_t fclex;
    fsave_t fsave;
    frstor_t frstor;
} x87function;
__declspec(dllexport) void load_x87function(x87function *func)
{
    func->fclex = fclex;
    func->fldcw = fldcw;
    func->fninit = fninit;
    func->frndint = frndint;
    func->frstor = frstor;
    func->fsave = fsave;
    func->fstcw = fstcw;
    func->wait = wait;
}
