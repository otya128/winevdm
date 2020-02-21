#include "haxmvm.h"

#include "../krnl386/kernel16_private.h"

BOOL initflag;
UINT8 *mem;
#define KRNL386 "krnl386.exe16"
BOOL is_single_step = FALSE;
DWORD WINAPI panic_msgbox(LPVOID data)
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
__declspec(dllimport) LDT_ENTRY wine_ldt[8192];

HANDLE hSystem;
HANDLE hVM;
HANDLE hVCPU;
struct hax_tunnel *tunnel;
 // 2MB is enough for 0x00000000-0x7fffffff plus 1 page for the pagedir 
#ifdef _MSC_VER
__declspec(align(4096))
#endif
DWORD
#ifdef __GNUC__
__attribute__ ((aligned(4096)))
#endif
guestpt[0x80400] = {0}

;

LDT_ENTRY gdt[];
void load_seg(segment_desc_t *segment, WORD sel)
{
    const LDT_ENTRY *entry = (sel & 0x4 || (sel >> 3) >= 512) ? wine_ldt : gdt;
    segment->selector = sel;
    segment->base = (uint64)wine_ldt_get_base(entry + (sel >> 3));
    segment->limit =  (uint64)wine_ldt_get_limit(entry + (sel >> 3));
    segment->type = entry[sel >> 3].HighWord.Bits.Type;
    segment->present = entry[sel >> 3].HighWord.Bits.Pres;
    segment->operand_size = entry[sel >> 3].HighWord.Bits.Default_Big;
    segment->dpl = entry[sel >> 3].HighWord.Bits.Dpl;
    segment->granularity = entry[sel >> 3].HighWord.Bits.Granularity;
    segment->null = 0;
    segment->desc = entry[sel >> 3].HighWord.Bits.Type >> 4;
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
#ifdef _MSC_VER
#define HAXMVM_ERR fprintf(stderr, __FUNCTION__ "("  HAXMVM_STR(__LINE__)  ") HAXM err.\n");
#define HAXMVM_ERRF(fmt, ...) fprintf(stderr, __FUNCTION__ "("  HAXMVM_STR(__LINE__)  ") " fmt "\n", __VA_ARGS__);
#else
#define HAXMVM_ERR fprintf(stderr, "%s("  HAXMVM_STR(__LINE__)  ") HAXM err.\n", __FUNCTION__);
#define HAXMVM_ERRF(fmt, ...) fprintf(stderr, "%s("  HAXMVM_STR(__LINE__)  ") " fmt "\n", __FUNCTION__, ##__VA_ARGS__);
#endif
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

static BOOL set_ram(struct hax_set_ram_info *pram)
{
    DWORD bytes;
    struct hax_set_ram_info ram = *pram;
    ram.flags = HAX_RAM_INFO_STANDALONE;
    if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, &ram, sizeof(struct hax_set_ram_info), NULL, 0, &bytes, NULL))
        return FALSE;
    DWORD physaddr = ram.pa_start / 4096;
    for (DWORD i = 0; i < (ram.size / 4096); i++)
    {
        DWORD pte = physaddr + i;
        guestpt[pte] = (pte << 12) | 7;
    }
    return TRUE;
}

#define dprintf(...) //fprintf(stderr, __VA_ARGS__)

static CRITICAL_SECTION crst;
static CRITICAL_SECTION_DEBUG critsect_debug =
{
    0, 0, &crst,
    { &critsect_debug.ProcessLocksList, &critsect_debug.ProcessLocksList },
      0, 0, { (DWORD_PTR)(__FILE__ ": crst") }
};
static CRITICAL_SECTION crst = { &critsect_debug, -1, 0, 0, 0, 0 };
void alloc_ram(uint32_t addr, uint32_t size)
{
    MEMORY_BASIC_INFORMATION mbi;
    struct hax_set_ram_info ram = { 0 };
    BOOL protect = 0;
    DWORD old = 0;
    ram.pa_start = addr;
    ram.size = size;
    ram.va = addr;
    if (VirtualQuery(ram.va, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
    {
        if (!(mbi.Protect & PAGE_EXECUTE_READWRITE) && !(mbi.Protect & PAGE_READWRITE))
        {
            protect = VirtualProtect(ram.va, 4096, PAGE_EXECUTE_READWRITE, &old);
        }
    }
    dprintf("alloc = %08x\n", ram.va);
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM");
    }
    if (protect && 0)
    {
        VirtualProtect(ram.va, 4096, old, &old);
    }
}

static void intel_vt_x_workaround_update_entry(int sel, const LDT_ENTRY *entry)
{
    DWORD bytes;
    uint32 start = ((uint32)wine_ldt_get_base(entry) & ~0xfff);
    uint32 size = 0;
    EnterCriticalSection(&crst);
    for (uint32 i = start; i <= ((uint32)wine_ldt_get_base(entry) + wine_ldt_get_limit(entry)); i += 4096)
    {
        if (!i || guestpt[i / 4096])
        {
            continue;
        }
        alloc_ram(i, 4096);
    }
    LeaveCriticalSection(&crst);
}

/*
 * WOW64 syscall entry:
 * B8 XX XX XX XX mov eax,XXXXX
 * BA XX XX XX XX mov edx,_Wow64SystemServiceCall@0
 * FF D2          call edx
 * C2 XX XX       ret xx
 * 90             nop
 */
#include <pshpack1.h>
struct wow64_syscall
{
    BYTE mov_eax;
    DWORD num;
    BYTE mov_edx;
    DWORD Wow64SystemServiceCall;
    WORD call_edx;
    BYTE ret;
    WORD ret_n;
    BYTE nop;
};
struct hooked_syscall
{
    BYTE jmp_rel;
    DWORD address;
};
#include <poppack.h>
BOOL hook_nt_syscall(struct wow64_syscall **old_syscall, LPVOID hook_func, LPCSTR hook_dll, LPCSTR hook_func_name)
{
    HMODULE dll = GetModuleHandleA(hook_dll);
    LPVOID func = (LPVOID)GetProcAddress(dll, hook_func_name);
    struct wow64_syscall *syscall = (struct wow64_syscall*)func;
    MEMORY_BASIC_INFORMATION mbi;
    DWORD old;
    if (!func)
    {
        return FALSE;
    }
    if (syscall->mov_eax != 0xb8 || syscall->mov_edx != 0xba || syscall->call_edx != 0xd2ff || syscall->ret != 0xc2 || syscall->nop != 0x90)
    {
        return FALSE;
    }
    if (!VirtualQuery(syscall, &mbi, sizeof(mbi)))
    {
        return FALSE;
    }
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &old);
    *old_syscall = VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    **old_syscall = *syscall;
    ((struct hooked_syscall*)syscall)->jmp_rel = 0xe9;
    ((struct hooked_syscall*)syscall)->address = (char*)hook_func - (char*)(&((struct hooked_syscall*)syscall)->address + 1);
    VirtualProtect(mbi.BaseAddress, mbi.RegionSize, old, &old);
    return TRUE;
}
struct wow64_syscall *old_NtFreeVirtualMemory;
NTSTATUS NTAPI hook_NtFreeVirtualMemory(HANDLE ProcessHandle, PVOID *BaseAddress, PULONG RegionSize, ULONG FreeType)
{
    NTSTATUS result = ((NTSTATUS(NTAPI*)(HANDLE, PVOID*, PULONG, ULONG))old_NtFreeVirtualMemory)(ProcessHandle, BaseAddress, RegionSize, FreeType);
    if (NT_SUCCESS(result))
    {
        DWORD bytes;
        DWORD physaddr = (DWORD)*BaseAddress / 4096;
        for (DWORD i = 0; i < (*RegionSize / 4096); i++)
        {
            DWORD pte = physaddr + i;
            if (sizeof(guestpt) / sizeof(guestpt[0]) <= pte)
                *(int*)0 = 0;
            if (guestpt[pte])
            {
                struct hax_set_ram_info ram = { pte * 4096, 4096, HAX_RAM_INFO_INVALID };

                if (!DeviceIoControl(hVM, HAX_VM_IOCTL_SET_RAM, &ram, sizeof(ram), NULL, 0, &bytes, NULL))
                {
                    HAXMVM_ERRF("Failed to discard memory %p", pte);
                }
                guestpt[pte] = 0;
            }
        }
    }
    return result;
}


BOOL init_vm86(BOOL vm86)
{
    if (!hook_nt_syscall(&old_NtFreeVirtualMemory, hook_NtFreeVirtualMemory, "ntdll", "NtFreeVirtualMemory"))
    {
        HAXMVM_ERRF("Failed to hook NtFreeVirtualMemory.");
    }
#ifdef _MSC_VER
    __asm
    {
        mov seg_cs, cs
        mov seg_ds, ds
    }
#else
    seg_cs = wine_get_cs();
    seg_ds = wine_get_ds();
#endif
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
    ram.pa_start =  (uint64_t)&idt;
    ram.size = 4096;
    ram.va =  (uint64_t)&idt;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM\n");
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
    ram.pa_start = (uint64_t)trap_int;
    ram.size = 0x10000;
    ram.va = (uint64_t)trap_int;
    if (!set_ram(&ram))
    {
        HAXMVM_ERRF("SET_RAM");
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
    ((void(*)(void(*intel_vt_x_workaround_update_entry)(const LDT_ENTRY *entry)))GetProcAddress(GetModuleHandleA("libwine"), "set_intel_vt_x_workaround"))(intel_vt_x_workaround_update_entry);
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
    if (state->_ss.operand_size)
    {
        state->_esp -= 2;
        unsigned char *stack = (unsigned char*)(state->_ss.base + state->_esp);
        *(LPWORD)stack = val;
    }
    else
    {
        state->_sp -= 2;
        unsigned char *stack = (unsigned char*)(state->_ss.base + state->_sp);
        *(LPWORD)stack = val;
    }
}
void PUSH32(struct vcpu_state_t *state, DWORD val)
{
    if (state->_ss.operand_size)
    {
        state->_esp -= 4;
        unsigned char *stack = (unsigned char*)(state->_ss.base + state->_esp);
        *(LPDWORD)stack = val;
    }
    else
    {
        state->_sp -= 4;
        unsigned char *stack = (unsigned char*)(state->_ss.base + state->_sp);
        *(LPDWORD)stack = val;
    }
}
WORD POP16(struct vcpu_state_t *state)
{
    if (state->_ss.operand_size)
    {
        LPWORD stack = (LPWORD)(state->_ss.base + state->_esp);
        state->_esp += 2;
        return *stack;
    }
    else
    {
        LPWORD stack = (LPWORD)(state->_ss.base + state->_sp);
        state->_sp += 2;
        return *stack;
    }
}
DWORD POP32(struct vcpu_state_t *state)
{
    if (state->_ss.operand_size)
    {
        LPDWORD stack = (LPDWORD)(state->_ss.base + state->_esp);
        state->_esp += 4;
        return *stack;
    }
    else
    {
        LPDWORD stack = (LPDWORD)(state->_ss.base + state->_sp);
        state->_sp += 4;
        return *stack;
    }
}
WORD PEEK16(struct vcpu_state_t *state, int i)
{
    if (state->_ss.operand_size)
    {
        LPWORD stack = (LPWORD)(state->_ss.base + state->_esp);
        return stack[i];
    }
    else
    {
        LPWORD stack = (LPWORD)(state->_ss.base + state->_sp);
        return stack[i];
    }
}
DWORD PEEK32(struct vcpu_state_t *state, int i)
{
    if (state->_ss.operand_size)
    {
        LPDWORD stack = (LPDWORD)(state->_ss.base + state->_esp);
        return stack[i];
    }
    else
    {
        LPDWORD stack = (LPDWORD)(state->_ss.base + state->_sp);
        return stack[i];
    }
}
void relay(LPVOID relay_func, BOOL reg, struct vcpu_state_t *state, DWORD ret_addr, DWORD cbArgs, PEXCEPTION_HANDLER handler, DWORD old_frame16)
{
    unsigned char *stack1 = (unsigned char*)(state->_ss.base + state->_sp);
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
    STACK32FRAME dummy_stack32 = { 0 };
    dummy_stack32.retaddr = ret_addr;
    dummy_stack32.nb_args = cbArgs;
    dummy_stack32.frame.Handler = handler;
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
    int off = 0;
    if (reg)
    {
        context.Esp = osp + (SIZE_T)stack - (SIZE_T)stack1 - 4;
        off = ooo - context.Esp;
        context.Ebp = bp;
        context.Eip = ip19;
        context.SegCs = cs16;
    }
    int fret;
    LPVOID old;
    PCONTEXT pctx = NULL;
#if _MSC_VER
    __asm
    {
        mov old, esp
        push cbArgs
        push old /* target(esp) */
        push retaddr /* retaddr */
        push ebp
        push ebx
        push esi
        push edi
        push old_frame16 /* frame16 */
        /* set up exception handler */
        push handler
        mov  eax, fs:[0]
        push eax
        mov dummy_stack32.frame.Next, eax
        mov  fs : [0], esp
        push cs
        push 0
        mov eax, [oa]
        mov[eax], esp
        lea eax, [context]
        push eax
        push args
        push entry
        call relay_func
        add esp, 12 + 8
        mov fret, eax
        pop dword ptr fs : [0]
        pop eax
        jmp skip
        retaddr :
        mov pctx, ecx
        skip:
        mov esp, old
    }
#else
    fret = ((int(*)(void *entry_point, unsigned char *args16, CONTEXT *context))relay_func)((void*)entry, (unsigned char*)args, &context);
#endif
    if (!reg)
    {
        state->_eax = fret;
    }
    if (pctx)
    {
        /* Throw16 */
        context = *pctx;
        reg = TRUE;
    }
    if (!reg)
    {
        state->_eax = fret;
        context.SegSs = ((size_t)dynamic_getWOW32Reserved() >> 16) & 0xFFFF;
        context.Esp = ((size_t)dynamic_getWOW32Reserved()) & 0xFFFF;
        oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp);
    }
    else
    {
        oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp + off);
    }
    if (reg) state->_eax = (DWORD)context.Eax;
    state->_ecx = reg ? (DWORD)context.Ecx : (DWORD)oa->ecx;
    if (reg) state->_edx = (DWORD)context.Edx;
    else
        state->_edx = (DWORD)oa->edx;
    state->_ebx = (DWORD)context.Ebx;
    state->_esp = (DWORD)context.Esp;
    state->_ebp = (DWORD)context.Ebp;
    state->_esi = (DWORD)context.Esi;
    state->_edi = (DWORD)context.Edi;
    load_seg(&state->_es, reg ? (WORD)context.SegEs : (WORD)oa->es);
    load_seg(&state->_ss, (WORD)context.SegSs);
    load_seg(&state->_ds, reg ? (WORD)context.SegDs : (WORD)oa->ds);
    //ES, CS, SS, DS, FS, GS
    /* Some programs expect that gs is not a valid selector! */
    /* Some programs expect that fs is not a valid selector! */
    /* win16 sets 0? */
    load_seg(&state->_fs, 0);//(WORD)context.SegFs == reg_fs ? 0 : context.SegFs;
    load_seg(&state->_gs, 0);//(WORD)context.SegGs == reg_gs ? 0 : context.SegGs;
    if (reg)
    {
        if (!(ip19 != context.Eip || cs16 != context.SegCs))
        {
            context.Eip = oa->callfrom_ip;
            context.SegCs = oa->module_cs;
        }
        else
        {
            /* CS:IP changed! */
            context.Eip = context.Eip;
        }
        state->_esp = context.Esp;
        state->_ebp = context.Ebp;
    }
    else
    {
        state->_esp = osp + 18 + 2;
        state->_esp -= (ooo - context.Esp);
        WORD bpp = state->_esp;
        state->_esp = context.Esp + 0x2c;
        state->_ebp = bp;
    }
    set_eflags(state, context.EFlags);
    state->_eip = context.Eip;
    load_seg(&state->_cs, (WORD)context.SegCs);
}

LPBYTE get_base_addr(WORD sel)
{
    return wine_ldt_get_base(wine_ldt + (sel >> 3));
}
static uint64 dr7;
void set_break_point(struct vcpu_state_t *state, WORD sel, DWORD addr, int breakpoint)
{
    LPBYTE base = get_base_addr(sel);
    if (!base)
        return;
    if (wine_ldt_get_limit(wine_ldt + (sel >> 3)) < addr)
        return;
    uint64 linear = (uint64)base + addr;
    switch (breakpoint)
    {
    case 0:
        state->_dr0 = linear;
        break;
    case 1:
        state->_dr1 = linear;
        break;
    case 2:
        state->_dr2 = linear;
        break;
    case 3:
        state->_dr3 = linear;
        break;
    default:
        return;
    }
    state->_dr7 |= (1 << (2 * breakpoint + 1)) | (0 << (18 + 4 * breakpoint));
    state->_dr6 = 0;
}
typedef int disassemble_debug_t(char *buffer, UINT8 *oprom, BOOL op_size, UINT64 eip);
static disassemble_debug_t *disassemble_debug;
static void trace(struct vcpu_state_t *state, uint16 cs, uint32 eip, uint16 ss, uint32 esp, uint32 eflags)
{
    char buf[512];
    UINT8 *d = get_base_addr(cs) + eip;

    if (!disassemble_debug)
    {
        disassemble_debug = (disassemble_debug_t*)GetProcAddress(LoadLibraryA("vm86.dll"), "disassemble_debug");
    }
    int len = disassemble_debug(buf, d, (wine_ldt_get_flags(wine_ldt + (cs >> 3)) & WINE_LDT_FLAGS_32BIT) == WINE_LDT_FLAGS_32BIT, eip);
    int i;
    fprintf(stderr, "%04x:%04x\t", cs, eip);
    for (i = 0; i < len; i++)
    {
        fprintf(stderr, "%02x", d[i]);
    }
    fprintf(stderr, "\t%s\n", buf);
    eflags &= ~2;
    eflags &= ~0x100;
    eflags |= 0x200;
    if (state->_fs.selector || state->_gs.selector)
    {
        fprintf(stderr,
            "EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X,"
            "ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X,"
            "ES:%04X,CS:%04X,SS:%04X,DS:%04X,FS:%04x,GS:%04x,"
            "IP:%04X,stack:%08X,"
            "EFLAGS:%08X"
            "\n",
            state->_eax, state->_ecx, state->_edx, state->_ebx,
            esp, state->_ebp, state->_esi, state->_edi,
            state->_es.selector, cs, ss, state->_ds.selector, state->_fs.selector, state->_gs.selector,
            eip, *(LPDWORD)(get_base_addr(ss) + esp),
            eflags
        );
    }
    else
    {
        fprintf(stderr,
            "EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X,"
            "ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X,"
            "ES:%04X,CS:%04X,SS:%04X,DS:%04X,"
            "IP:%04X,stack:%08X,"
            "EFLAGS:%08X"
            "\n",
            state->_eax, state->_ecx, state->_edx, state->_ebx,
            esp, state->_ebp, state->_esi, state->_edi,
            state->_es.selector, cs, ss, state->_ds.selector,
            eip, *(LPDWORD)(get_base_addr(ss) + esp),
            eflags
        );
    }
}
BOOL has_x86_exception_err(WORD num)
{
    switch (num)
    {
    case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:
        return FALSE;
    case 8:
        return TRUE;
    case 9:
        return FALSE;
    case 10:case 11:case 12:case 13:case 14:
        return TRUE;
    case 15:case 16:
        return FALSE;
    case 17:
        return TRUE;
    case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
        return FALSE;
    case 30:
        return TRUE;
    case 31:
        return FALSE;
    }
    return FALSE;
}
BOOL syscall_init = FALSE;
LPBYTE syscall_trap = FALSE;
void fstsw(WORD* a);
void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    pm_interrupt_handler pih
)
{
    DWORD old_frame16 = PtrToUlong(dynamic_getWOW32Reserved());
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
        SIZE_T page3 = (SIZE_T)__wine_call_to_16_ret / 4096 * 4096;
        LPBYTE trap = syscall_trap = (LPBYTE)VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);
        memset(trap, 0xEE, 4096); /* out forces a vmexit from user mode without modifing any registers */
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
        guestpt[page3 >> 12] = (DWORD)trap | 7;
        syscall_init = TRUE;
    }
    is_single_step = dasm;
    MEMORY_BASIC_INFORMATION mbi;
    DWORD bytes;
    DWORD ret_addr;
    struct vcpu_state_t state_ini;
    {

        DWORD bytes;
        struct vcpu_state_t state;
        if (!DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL))
            HAXMVM_ERRF("GET_REGS");
        load_seg(&state._gs, (WORD)0);
        load_seg(&state._fs, (WORD)0);
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

    struct vcpu_state_t state2;
    DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state2, sizeof(state2), &bytes, NULL);
    if (is_single_step)
    {
        trace(&state2, state2._cs.selector, state2._eip, state2._ss.selector, state2._esp, state2._eflags);
    }
    while (TRUE)
    {
        dprintf("run %04X:%04X(base:%04llX) ESP:%08X F:%08X CS:%08X\n", state2._cs.selector, state2._eip, state2._cs.base, state2._esp, state2._eflags, state2._cs.ar);
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
        dprintf("end %04X:%04X(base:%04llX) ESP:%08X F:%08X\n", state2._cs.selector, state2._eip, state2._cs.base, state2._esp, state2._eflags);
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
                        relay(relay_call_from_16, is_reg, &state2, ret_addr, cbArgs, handler, old_frame16);
                        if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
                        {
                            HAXMVM_ERRF("SET_REGS");
                        }
                        break;
                    }
	            }
                else if (tunnel->io._direction == HAX_IO_IN)
                {
                    printf("");
                }
                break;
            case HAX_EXIT_HLT:
                if (((DWORD)ptr >= (DWORD)trap_int) && ((DWORD)ptr <= ((DWORD)trap_int + 256)))
                {
                    int intvec = ((DWORD)ptr & 0xff) - 1;
                    BOOL has_err = has_x86_exception_err(intvec);
                    DWORD err = has_err ? PEEK32(&state2, 0) : 0;
                    DWORD eip = PEEK32(&state2, (has_err ? 1 : 0) + 0);
                    DWORD cs = PEEK32(&state2, (has_err ? 1 : 0) + 1);
                    DWORD flags = PEEK32(&state2, (has_err ? 1 : 0) + 2);
                    DWORD old_esp = PEEK32(&state2, (has_err ? 1 : 0) + 3);
                    DWORD old_ss = PEEK32(&state2, (has_err ? 1 : 0) + 4);
                    const char *name = NULL;
                    switch (intvec)
                    {
                    case 0: name = "#DE"; break;
                    case 2: name = "int 2h"; break;
                    case 4: name = "#OF"; break;
                    case 6: name = "#UD"; break;
                    case 7: name = "#NM"; break;
                    case 8: name = "#DF"; break;
                    case 10: name = "#TS"; break;
                    case 11: name = "#NP"; break;
                    case 12: name = "#SS"; break;
                    case 13: name = "#GP"; break;
                    case 14: name = "#PF"; break;
                    }
                    state2._eip = 256;
                    if (intvec == 1 && (state2._dr6 & 15 || is_single_step))
                    {
                        if (state2._dr6 & 15)
                        {
                            if (state2._dr6 & 1)
                            {
                                fprintf(stderr, "breakpoint 0\n");
                            }
                            if (state2._dr6 & 2)
                            {
                                fprintf(stderr, "breakpoint 1\n");
                            }
                            if (state2._dr6 & 4)
                            {
                                fprintf(stderr, "breakpoint 2\n");
                            }
                            if (state2._dr6 & 8)
                            {
                                fprintf(stderr, "breakpoint 3\n");
                            }
                            state2._dr6 = 0;
                            flags |= 0x100;
                            dr7 = state2._dr7;
                            state2._dr7 = 0;
                            /* breakpoint -> disable -> step -> enable */
                        }
                        else if (!is_single_step)
                        {
                            flags &= ~0x100;
                            state2._dr7 = dr7;
                        }
                        trace(&state2, cs, eip, old_ss, old_esp, flags);
                        state2._eip = 256;
                        break;
                    }
                    dprintf("err:%X flg:%08X %04X:%04X\n", err, flags, cs, eip);
                    if (intvec == 0x0e)
                    {
                        alloc_ram(state2._cr2 & ~0xfff, 4096);
                        state2._esp += 4;
                    }
                    else if (name)
                    {
                        if (intvec == 0x0d)
                        {
                            if (err == 0x40)
                            {
                                /* many startups access the BDA directly */
                                static WORD dosmem_0040H = 0;
                                if (!dosmem_0040H)
                                {
                                    DWORD(WINAPI *GetProcAddress16)(HMODULE16, LPCSTR);
                                    HMODULE16(WINAPI *GetModuleHandle16)(LPCSTR);
                                    static HMODULE krnl386;
                                    if (!krnl386)
                                        krnl386 = LoadLibraryA(KRNL386);
                                    GetProcAddress16 = (DWORD(WINAPI *)(HMODULE16, LPCSTR))GetProcAddress(krnl386, "GetProcAddress16");
                                    GetModuleHandle16 = (HMODULE16(WINAPI *)(LPCSTR))GetProcAddress(krnl386, "GetModuleHandle16");
                                    dosmem_0040H = (WORD)GetProcAddress16(GetModuleHandle16("KERNEL"), (LPCSTR)193);
                                    (void(*)(void))GetProcAddress(krnl386, "DOSVM_start_bios_timer")();
                                }
                                err = POP32(&state2);
                                eip = POP32(&state2);
                                cs = POP32(&state2);
                                flags = POP32(&state2);
                                old_esp = POP32(&state2);
                                old_ss = POP32(&state2);
                                /* allocate segment 40h */
                                LPLDT_ENTRY entry = wine_ldt + (dosmem_0040H >> __AHSHIFT);
                                gdt[0x40 >> __AHSHIFT] = *entry;
                                load_seg(&state2._cs, cs);
                                state2._eip = eip;
                                set_eflags(&state2, flags & ~0x10000);
                                load_seg(&state2._ss, old_ss);
                                state2._esp = old_esp;
                                break;
                            }
                        }
                        trace(&state2, cs, eip, old_ss, old_esp, flags);
                        HAXMVM_ERRF("%s %02x %04x %04x:%04x %04x:%04x", name, intvec, err, cs, eip, old_ss, old_esp);
                        HAXMVM_ERRF("%04X:%04X(base:%04llX) ESP:%08X", state2._cs.selector, state2._eip, state2._cs.base, state2._esp);
                        HAXMVM_ERRF("exception");
                        haxmvm_panic("exception %s", name);
                        pih(intvec, MAKESEGPTR(state2._cs.selector, state2._eip & 0xffff));
                    }
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
                            CONTEXT ctx;
                            save_context_from_state(&ctx, &state2);
                            if (!DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state2, sizeof(state2), NULL, 0, &bytes, NULL))
                            {
                                    HAXMVM_ERRF("SET_REGS");
                            }
                            if (intvec == 0x10) // redirect fpu errors to int 13h
                            {
                                WORD sw;
                                fstsw(&sw);
                                if (sw & 0x80)
                                    intvec = 2;
                            }
                            dynamic__wine_call_int_handler(&ctx, intvec);
                            load_context_to_state(&ctx, &state2);
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
    }
    save_context(context);
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
    return (context.Eax & 0xffff) | context.Edx << 16;
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
void callx87(const char *addr, LPCVOID eax)
{
    DWORD bytes;
    struct vcpu_state_t state;
    DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL);
    state._rip = addr;
    state._eax = eax;
    state._ds.selector = seg_ds;
    state._ds.base = 0;
    state._cs.selector = seg_cs;
    state._cs.base = 0;
    while (TRUE)
    {
        DeviceIoControl(hVCPU, HAX_VCPU_SET_REGS, &state, sizeof(state), NULL, 0, &bytes, NULL);
        if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_RUN, NULL, 0, NULL, 0, &bytes, NULL))
            return;
        DeviceIoControl(hVCPU, HAX_VCPU_GET_REGS, NULL, 0, &state, sizeof(state), &bytes, NULL);
        if (tunnel->_exit_status == HAX_EXIT_HLT)
        {
            struct vcpu_state_t state2 = state;
            LPVOID ptr = (LPBYTE)state2._cs.base + state2._eip;
            if (((DWORD)ptr >= (DWORD)trap_int) && ((DWORD)ptr <= ((DWORD)trap_int + 256)))
            {
                int intvec = ((DWORD)ptr & 0xff) - 1;
                state2._eip = 256;
                if (intvec == 0x0e)
                {
                    alloc_ram(state2._cr2 & ~0xfff, 4096);
                    state2._esp += 4;
                }
                else
                    break;
            }
            else
                break;
            state = state2;
        }
        else
            break;
    }
    if (tunnel->_exit_status == HAX_EXIT_STATECHANGE)
    {
        HAXMVM_ERRF("hypervisor is panicked!!!");
        haxmvm_panic("win87em: hypervisor is panicked!!!");
    }
}
/* x87 service functions */
static BOOL get_fpu_regs(struct fx_layout *fx)
{
    if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_GET_FPU, NULL, 0, fx, sizeof(*fx), NULL, NULL))
    {
        HAXMVM_ERRF("HAX_VCPU_IOCTL_GET_FPU");
        return FALSE;
    }
    return TRUE;
}
static BOOL set_fpu_regs(struct fx_layout *fx)
{
    if (!DeviceIoControl(hVCPU, HAX_VCPU_IOCTL_SET_FPU, fx, sizeof(*fx), NULL, 0, NULL, NULL))
    {
        HAXMVM_ERRF("HAX_VCPU_IOCTL_SET_FPU");
        return FALSE;
    }
    return TRUE;
}
void fldcw(WORD a)
{
    struct fx_layout fx;
    if (!get_fpu_regs(&fx))
    {
        return;
    }
    fx.fcw = a;
    if (!set_fpu_regs(&fx))
    {
        return;
    }
}
void wait()
{
    char instr[] = { 0x9b, 0xee }; /* wait */
    callx87(instr, NULL);
}
void fninit()
{
    char instr[] = { 0xdb, 0xe3, 0xee }; /* fninit */
    callx87(instr, NULL);
}
void fstcw(WORD* a)
{
    struct fx_layout fx;
    if (!get_fpu_regs(&fx))
    {
        return;
    }
    *a = fx.fcw;
}
void fstsw(WORD* a)
{
    struct fx_layout fx;
    if (!get_fpu_regs(&fx))
    {
        return;
    }
    *a = fx.fsw;
}
void frndint()
{
    char instr[] = { 0xd9, 0xfc, 0xee }; /* frndint */
    callx87(instr, NULL);
}
void fclex()
{
    char instr[] = { 0xdb, 0xe2, 0xee }; /* fnclex */
    callx87(instr, NULL);
}
void fsave(char* a)
{
    char instr[] = { 0xdd, 0x30, 0xee }; /* fnsave [eax] */
    callx87(instr, a);
}
void frstor(const char* a)
{
    char instr[] = { 0xdd, 0x20, 0xee }; /* frstor [eax] */
    callx87(instr, a);
}
void fstenv32(char* a)
{
    const char instr[] = { 0xd9, 0x30, 0xee }; /* fnstenv dword ptr [eax] */
    callx87(instr, a);
}
typedef void(*fldcw_t)(WORD);
typedef void(*wait_t)();
typedef void(*fninit_t)();
typedef void(*fstcw_t)(WORD*);
typedef void(*fstsw_t)(WORD*);
typedef void(*frndint_t)();
typedef void(*fclex_t)();
typedef void(*fsave_t)(char*);
typedef void(*fstenv32_t)(char*);
typedef void(*frstor_t)(const char*);
typedef DWORD(*fistp_t)(WORD);
typedef struct
{
    fldcw_t fldcw;
    wait_t wait;
    fninit_t fninit;
    fstcw_t fstcw;
    fstsw_t fstsw;
    frndint_t frndint;
    fclex_t fclex;
    fsave_t fsave;
    frstor_t frstor;
    fstenv32_t fstenv32;
    fistp_t fistp;
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
    func->fstsw = fstsw;
    func->wait = wait;
    func->fstenv32 = fstenv32;
}
