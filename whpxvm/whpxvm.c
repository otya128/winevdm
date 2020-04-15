#include "whpxvm.h"

#include "../krnl386/kernel16_private.h"
#include "wownt32.h"

BOOL initflag;
UINT8 *mem;
#define KRNL386 "krnl386.exe16"
BOOL is_single_step = FALSE;
HRESULT(WINAPI *pWHvCreatePartition)(WHV_PARTITION_HANDLE *Partition);
HRESULT(WINAPI *pWHvSetupPartition)(WHV_PARTITION_HANDLE Partition);
HRESULT(WINAPI *pWHvMapGpaRange)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ VOID* SourceAddress,
    _In_ WHV_GUEST_PHYSICAL_ADDRESS GuestAddress,
    _In_ UINT64 SizeInBytes,
    _In_ WHV_MAP_GPA_RANGE_FLAGS Flags
);
HRESULT(WINAPI *pWHvUnmapGpaRange)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ WHV_GUEST_PHYSICAL_ADDRESS GuestAddress,
    _In_ UINT64 SizeInBytes
    );
HRESULT(WINAPI*pWHvCreateVirtualProcessor)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ UINT32 VpIndex,
    _In_ UINT32 Flags
    );
HRESULT(WINAPI *pWHvGetVirtualProcessorRegisters)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ UINT32 VpIndex,
    _In_reads_(RegisterCount) const WHV_REGISTER_NAME* RegisterNames,
    _In_ UINT32 RegisterCount,
    _Out_writes_(RegisterCount) WHV_REGISTER_VALUE* RegisterValues
    );
HRESULT(WINAPI *pWHvSetVirtualProcessorRegisters)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ UINT32 VpIndex,
    _In_reads_(RegisterCount) const WHV_REGISTER_NAME* RegisterNames,
    _In_ UINT32 RegisterCount,
    _In_reads_(RegisterCount) const WHV_REGISTER_VALUE* RegisterValues
    );
HRESULT(WINAPI *pWHvSetPartitionProperty)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ WHV_PARTITION_PROPERTY_CODE PropertyCode,
    _In_reads_bytes_(PropertyBufferSizeInBytes) const VOID* PropertyBuffer,
    _In_ UINT32 PropertyBufferSizeInBytes
    );
HRESULT(WINAPI *pWHvRunVirtualProcessor)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ UINT32 VpIndex,
    _Out_writes_bytes_(ExitContextSizeInBytes) VOID* ExitContext,
    _In_ UINT32 ExitContextSizeInBytes
    );
HRESULT(WINAPI *pWHvCancelRunVirtualProcessor)(
    _In_ WHV_PARTITION_HANDLE Partition,
    _In_ UINT32 VpIndex,
    _In_ UINT32 Flags
    );


DWORD WINAPI panic_msgbox(LPVOID data)
{
    MessageBoxA(NULL, (LPCSTR)data, "Hypervisor error", MB_OK | MB_ICONERROR);
    HeapFree(GetProcessHeap(), 0, data);
    return 0;
}
#define PANIC_HRESULT(msg, hresult) panic_hresult(msg, hresult, __FUNCTION__, __LINE__)
#define PANIC(msg, ...) panic("%s:%d\n" msg, __FUNCTION__, __LINE__, __VA_ARGS__)

/* _Noreturn */
void panic(const char *msg, ...)
{
    LPSTR buffer = HeapAlloc(GetProcessHeap(), 0, 512);
    DWORD threadId;
    va_list arg;

    va_start(arg, msg);
    vsnprintf(buffer, 512, msg, arg);
    va_end(arg);
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)panic_msgbox, buffer, 0, &threadId);
    WaitForSingleObject(hThread, INFINITE);
    ExitThread(1);
    return;
}

/* _Noreturn */
void panic_hresult(const char *msg, HRESULT result, const char *func, int line)
{
    LPSTR buffer = HeapAlloc(GetProcessHeap(), 0, 512);
    DWORD threadId;
    snprintf(buffer, 512, "%s:%d\n%s HRESULT=%08x", func, line, msg, result);
    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)panic_msgbox, buffer, 0, &threadId);
    WaitForSingleObject(hThread, INFINITE);
    ExitThread(1);
    return;
}

static HMODULE krnl386 = 0;


PVOID dynamic_setWOW32Reserved(PVOID w)
{
    static PVOID(*setWOW32Reserved)(PVOID);
    if (!setWOW32Reserved)
    {
        if (!krnl386)
            krnl386 = LoadLibraryA(KRNL386);
        setWOW32Reserved = (PVOID(*)(PVOID))GetProcAddress(krnl386, "setWOW32Reserved");
    }
    return setWOW32Reserved(w);
}
PVOID dynamic_getWOW32Reserved()
{
    static PVOID(*getWOW32Reserved)();
    if (!getWOW32Reserved)
    {
        if (!krnl386)
            krnl386 = LoadLibraryA(KRNL386);
        getWOW32Reserved = (PVOID(*)())GetProcAddress(krnl386, "getWOW32Reserved");
    }
    return getWOW32Reserved();
}
WINE_VM86_TEB_INFO *dynamic_getGdiTebBatch()
{
    static WINE_VM86_TEB_INFO*(*getGdiTebBatch)();
    if (!getGdiTebBatch)
    {
        if (!krnl386)
            krnl386 = LoadLibraryA(KRNL386);
        getGdiTebBatch = (WINE_VM86_TEB_INFO*(*)())GetProcAddress(krnl386, "getGdiTebBatch");
    }
    return getGdiTebBatch();
}
void dynamic__wine_call_int_handler(CONTEXT *context, BYTE intnum)
{
    static void(*__wine_call_int_handler)(CONTEXT *context, BYTE intnum);
    if (!__wine_call_int_handler)
    {
        if (!krnl386)
            krnl386 = LoadLibraryA(KRNL386);
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

LDT_ENTRY gdt[];
void load_seg(WHV_X64_SEGMENT_REGISTER *segment, WORD sel)
{
    const LDT_ENTRY *entry = (sel & 0x4 || (sel >> 3) >= 512) ? wine_ldt : gdt;
    segment->Selector = sel;
    segment->Base = (UINT64)wine_ldt_get_base(entry + (sel >> 3));
    segment->Limit =  (UINT64)wine_ldt_get_limit(entry + (sel >> 3));
    segment->Attributes = 0;
    segment->SegmentType = entry[sel >> 3].HighWord.Bits.Type;
    segment->Present = entry[sel >> 3].HighWord.Bits.Pres;
    segment->Default = entry[sel >> 3].HighWord.Bits.Default_Big;
    segment->DescriptorPrivilegeLevel = entry[sel >> 3].HighWord.Bits.Dpl;
    segment->Granularity = entry[sel >> 3].HighWord.Bits.Granularity;
    segment->NonSystemSegment = entry[sel >> 3].HighWord.Bits.Type >> 4;
    if (!sel || !segment->SegmentType || !segment->Present)
        segment->Attributes = 0;
}

void set_eflags(struct whpx_vcpu_state *state, DWORD eflags)
{
    state->rflags.Reg32 = eflags | 2 | 0x3000 | (is_single_step ? 0x100 : 0) | 0x200;
}

void load_context_to_state(CONTEXT *context, struct whpx_vcpu_state *state)
{
    DWORD bytes;
    load_seg(get_gs(state), (WORD)context->SegGs);
    load_seg(get_fs(state), (WORD)context->SegFs);
    load_seg(get_es(state), (WORD)context->SegEs);
    load_seg(get_ds(state), (WORD)context->SegDs);
    load_seg(get_cs(state), (WORD)context->SegCs);
    load_seg(get_ss(state), (WORD)context->SegSs);

    set_edi(state, context->Edi);
    set_esi(state, context->Esi);
    set_ebx(state, context->Ebx);
    set_edx(state, context->Edx);
    set_ecx(state, context->Ecx);
    set_eax(state, context->Eax);

    set_ebp(state, context->Ebp);
    set_eip(state, context->Eip);
    set_eflags(state, context->EFlags);
    set_esp(state, context->Esp);
}

void save_context_from_state(CONTEXT *context, struct whpx_vcpu_state *state)
{
    context->SegGs = get_seg_selector(get_gs(state));
    context->SegFs = get_seg_selector(get_fs(state));
    context->SegEs = get_seg_selector(get_es(state));
    context->SegDs = get_seg_selector(get_ds(state));
    context->SegCs = get_seg_selector(get_cs(state));
    context->SegSs = get_seg_selector(get_ss(state));

    context->Edi = get_edi(state);
    context->Esi = get_esi(state);
    context->Ebx = get_ebx(state);
    context->Edx = get_edx(state);
    context->Ecx = get_ecx(state);
    context->Eax = get_eax(state);

    context->Ebp = get_ebp(state);
    context->Eip = get_eip(state);
    context->EFlags = get_eflags(state) & ~2;
    context->Esp = get_esp(state);
    dynamic_setWOW32Reserved((PVOID)(get_seg_selector(get_ss(state)) << 16 | get_sp(state)));
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

typedef DWORD (*DOSVM_inport_t)(int port, int size);
typedef void (*DOSVM_outport_t)(int port, int size, DWORD value);
DOSVM_inport_t DOSVM_inport;
DOSVM_outport_t DOSVM_outport;

HANDLE inject_event;
CRITICAL_SECTION inject_crit_section;
typedef VOID (WINAPI *GetpWin16Lock_t)(SYSLEVEL **lock);
GetpWin16Lock_t pGetpWin16Lock;
SYSLEVEL *win16_syslevel;
typedef BOOL(WINAPI *WOWCallback16Ex_t)(DWORD vpfn16, DWORD dwFlags,
    DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
WOWCallback16Ex_t pWOWCallback16Ex;
typedef BOOL (WINAPI *vm_inject_t)(DWORD vpfn16, DWORD dwFlags,
    DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
BOOL WINAPI vm_inject(DWORD vpfn16, DWORD dwFlags,
    DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
__declspec(dllexport) DWORD wine_call_to_16_vm86(DWORD target, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    BOOL vm86,
    void *memory_base,
    pm_interrupt_handler pih);

static CRITICAL_SECTION running_critical_section;

static WHV_PARTITION_HANDLE partition;
static BOOL load_func(HMODULE hmod, LPCSTR fname, LPVOID *dest)
{
    *dest = GetProcAddress(hmod, fname);
    return *dest != NULL;
}
static BOOL load_funcs(void)
{
    HMODULE hmod = LoadLibraryW(L"WinHvPlatform.dll");
    BOOL result = TRUE;
    result = result && load_func(hmod, "WHvCreatePartition", (LPVOID*)&pWHvCreatePartition);
    result = result && load_func(hmod, "WHvSetupPartition", (LPVOID*)&pWHvSetupPartition);
    result = result && load_func(hmod, "WHvMapGpaRange", (LPVOID*)&pWHvMapGpaRange);
    result = result && load_func(hmod, "WHvUnmapGpaRange", (LPVOID*)&pWHvUnmapGpaRange);
    result = result && load_func(hmod, "WHvCreateVirtualProcessor", (LPVOID*)&pWHvCreateVirtualProcessor);
    result = result && load_func(hmod, "WHvGetVirtualProcessorRegisters", (LPVOID*)&pWHvGetVirtualProcessorRegisters);
    result = result && load_func(hmod, "WHvSetVirtualProcessorRegisters", (LPVOID*)&pWHvSetVirtualProcessorRegisters);
    result = result && load_func(hmod, "WHvSetPartitionProperty", (LPVOID*)&pWHvSetPartitionProperty);
    result = result && load_func(hmod, "WHvRunVirtualProcessor", (LPVOID*)&pWHvRunVirtualProcessor);
    result = result && load_func(hmod, "WHvCancelRunVirtualProcessor", (LPVOID*)&pWHvCancelRunVirtualProcessor);
    return result;
}
BOOL init_vm86(BOOL vm86)
{
    if (!load_funcs())
    {
        PANIC("Failed to load WinHvPlatform.dll");
        return FALSE;
    }
    HRESULT result;
    WHV_PARTITION_PROPERTY prop;
    InitializeCriticalSection(&running_critical_section);
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
    if (FAILED(result = pWHvCreatePartition(&partition)))
    {
        PANIC_HRESULT("WHvCreatePartition", result);
        return FALSE;
    }
    prop.ProcessorCount = 1;
    if (FAILED(result = pWHvSetPartitionProperty(partition, WHvPartitionPropertyCodeProcessorCount, &prop, sizeof(prop))))
    {
        PANIC_HRESULT("WHvSetPartitionProperty", result);
        return FALSE;
    }
    if (FAILED(result = pWHvSetupPartition(partition)))
    {
        PANIC_HRESULT("WHvSetupPartition", result);
        return FALSE;
    }
    if (FAILED(result = pWHvMapGpaRange(partition, (LPVOID)0x1000, 0x1000, 0x80000000 - 0x1000, WHvMapGpaRangeFlagRead | WHvMapGpaRangeFlagWrite | WHvMapGpaRangeFlagExecute)))
    {
        PANIC_HRESULT("WHvMapGpaRange", result);
        return FALSE;
    }
    if (FAILED(result = pWHvCreateVirtualProcessor(partition, 0, 0)))
    {
        PANIC_HRESULT("WHvCreateVirtualProcessor", result);
        return FALSE;
    }
    struct whpx_vcpu_state state;
    if (FAILED(result = pWHvGetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state.values)))
    {
        PANIC_HRESULT("WHvCreateVirtualProcessor", result);
        return FALSE;
    }
    /* setup initial states */
    state.gdtr.Table.Base = (UINT64)gdt;
    state.gdtr.Table.Limit = 0x7ff;
    trap_int = VirtualAlloc(NULL, 0x10000, MEM_COMMIT, PAGE_READWRITE);
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
    GetThreadSelectorEntry(GetCurrentThread(), seg_cs, gdt + (seg_cs >> 3));
    GetThreadSelectorEntry(GetCurrentThread(), seg_ds, gdt + (seg_ds >> 3));

    state.ldtr.Segment.Selector = 0x18;
    state.ldtr.Segment.Base = (UINT64)&wine_ldt[0];
    state.ldtr.Segment.Limit = 65535;

    UINT32 *tss = (UINT32 *)gdt + 0x200;
    state.tr.Segment.Selector = 0x18;
    state.tr.Segment.Base = (UINT64)tss;
    state.tr.Segment.Limit = 0x64;
    state.tr.Segment.Attributes = 0x8b;
    tss[1] = 0x10000; // SP0
    tss[2] = 0x10; // SS0

    state.idtr.Table.Limit = 0x8 * 256 - 1;
    state.idtr.Table.Base = (SIZE_T)&idt[0];
    state.cr0.Reg32 |= 1;
    state.rflags.Reg32 |= 0x200;
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
    if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state.values)))
    {
        PANIC_HRESULT("WHvSetVirtualProcessorRegisters", result);
        return FALSE;
    }

    if (!krnl386)
        krnl386 = LoadLibraryA(KRNL386);
    pWOWCallback16Ex = (WOWCallback16Ex_t)GetProcAddress(krnl386, "K32WOWCallback16Ex");
    void(WINAPI *set_vm_inject_cb)(vm_inject_t) = (void(WINAPI *)(vm_inject_t))GetProcAddress(krnl386, "set_vm_inject_cb");
    set_vm_inject_cb(vm_inject);
    inject_event = CreateEventW(NULL, TRUE, FALSE, NULL);
    InitializeCriticalSection(&inject_crit_section);
    pGetpWin16Lock = (GetpWin16Lock_t)GetProcAddress(krnl386, "GetpWin16Lock");
    pGetpWin16Lock(&win16_syslevel);
    DOSVM_inport = (DOSVM_inport_t)GetProcAddress(krnl386, "DOSVM_inport");
    DOSVM_outport = (DOSVM_outport_t)GetProcAddress(krnl386, "DOSVM_outport");
    return TRUE;
}

void PUSH16(struct whpx_vcpu_state *state, WORD val)
{
    if (state->ss.Segment.Default)
    {
        state->rsp.Reg32 -= 2;
        unsigned char *stack = (unsigned char*)(state->ss.Segment.Base + state->rsp.Reg32);
        *(LPWORD)stack = val;
    }
    else
    {
        state->rsp.Reg16 -= 2;
        unsigned char *stack = (unsigned char*)(state->ss.Segment.Base + state->rsp.Reg16);
        *(LPWORD)stack = val;
    }
}
void PUSH32(struct whpx_vcpu_state *state, DWORD val)
{
    if (state->ss.Segment.Default)
    {
        state->rsp.Reg32 -= 4;
        unsigned char *stack = (unsigned char*)(state->ss.Segment.Base + state->rsp.Reg32);
        *(LPDWORD)stack = val;
    }
    else
    {
        state->rsp.Reg16 -= 4;
        unsigned char *stack = (unsigned char*)(state->ss.Segment.Base + state->rsp.Reg16);
        *(LPDWORD)stack = val;
    }
}
WORD POP16(struct whpx_vcpu_state *state)
{
    if (state->ss.Segment.Default)
    {
        LPWORD stack = (LPWORD)(state->ss.Segment.Base + state->rsp.Reg32);
        state->rsp.Reg32 += 2;
        return *stack;
    }
    else
    {
        LPWORD stack = (LPWORD)(state->ss.Segment.Base + state->rsp.Reg16);
        state->rsp.Reg16 += 2;
        return *stack;
    }
}
DWORD POP32(struct whpx_vcpu_state *state)
{
    if (state->ss.Segment.Default)
    {
        LPDWORD stack = (LPDWORD)(state->ss.Segment.Base + state->rsp.Reg32);
        state->rsp.Reg32 += 4;
        return *stack;
    }
    else
    {
        LPDWORD stack = (LPDWORD)(state->ss.Segment.Base + state->rsp.Reg16);
        state->rsp.Reg16 += 4;
        return *stack;
    }
}
WORD PEEK16(struct whpx_vcpu_state *state, int i)
{
    if (state->ss.Segment.Default)
    {
        LPWORD stack = (LPWORD)(state->ss.Segment.Base + state->rsp.Reg32);
        return stack[i];
    }
    else
    {
        LPWORD stack = (LPWORD)(state->ss.Segment.Base + state->rsp.Reg16);
        return stack[i];
    }
}
DWORD PEEK32(struct whpx_vcpu_state *state, int i)
{
    if (state->ss.Segment.Default)
    {
        LPDWORD stack = (LPDWORD)(state->ss.Segment.Base + state->rsp.Reg32);
        return stack[i];
    }
    else
    {
        LPDWORD stack = (LPDWORD)(state->ss.Segment.Base + state->rsp.Reg16);
        return stack[i];
    }
}

void relay(LPVOID relay_func, BOOL reg, struct whpx_vcpu_state *state, DWORD ret_addr, DWORD cbArgs, PEXCEPTION_HANDLER handler, DWORD old_frame16)
{
    unsigned char *stack1 = (unsigned char*)(get_seg_base(get_ss(state))+ get_sp(state));
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
    set_eip(state, ip);
    load_seg(get_cs(state), (WORD)cs);
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
    DWORD osp = get_esp(state);
    PUSH16(state, get_seg_selector(get_gs(state)));
    PUSH16(state, get_seg_selector(get_fs(state)));
    PUSH16(state, get_seg_selector(get_es(state)));
    PUSH16(state, get_seg_selector(get_ds(state)));
    PUSH32(state, get_ebp(state));
    PUSH32(state, get_ecx(state));
    PUSH32(state, get_edx(state));
    PUSH32(state, osp);
    save_context_from_state(&context, state);
    STACK16FRAME *oa = (STACK16FRAME*)wine_ldt_get_ptr((WORD)context.SegSs, context.Esp);
    DWORD ooo = (WORD)context.Esp;
    int off = 0;
    if (reg)
    {
        context.Esp = osp + (SIZE_T)stack - (SIZE_T)stack1 - 4;
        off = ooo - context.Esp;
        context.Ebp = (context.Ebp & ~0xffff) | bp;
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
        set_eax(state, fret);
    }
    if (pctx)
    {
        /* Throw16 */
        context = *pctx;
        reg = TRUE;
    }
    if (!reg)
    {
        set_eax(state, fret);
        context.SegSs = ((size_t)dynamic_getWOW32Reserved() >> 16) & 0xFFFF;
        context.Esp = ((size_t)dynamic_getWOW32Reserved()) & 0xFFFF;
        oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp);
    }
    else
    {
        oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp + off);
    }
    if (reg)
        set_eax(state, (DWORD)context.Eax);
    set_ecx(state, reg ? (DWORD)context.Ecx : (DWORD)oa->ecx);
    if (reg)
        set_edx(state, (DWORD)context.Edx);
    else
        set_edx(state, (DWORD)oa->edx);
    set_ebx(state, (DWORD)context.Ebx);
    set_esp(state, (DWORD)context.Esp);
    set_ebp(state, (DWORD)context.Ebp);
    set_esi(state, (DWORD)context.Esi);
    set_edi(state, (DWORD)context.Edi);
    load_seg(get_es(state), reg ? (WORD)context.SegEs : (WORD)oa->es);
    load_seg(get_ss(state), (WORD)context.SegSs);
    load_seg(get_ds(state), reg ? (WORD)context.SegDs : (WORD)oa->ds);
    //ES, CS, SS, DS, FS, GS
    /* Some programs expect that gs is not a valid selector! */
    /* Some programs expect that fs is not a valid selector! */
    /* win16 sets 0? */
    load_seg(get_fs(state), 0);
    load_seg(get_gs(state), 0);
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
        set_esp(state, context.Esp);
        set_ebp(state, context.Ebp);
    }
    else
    {
        set_sp(state, context.Esp + 0x2c);
        set_bp(state, bp);
    }
    set_eflags(state, context.EFlags);
    set_eip(state, context.Eip);
    load_seg(get_cs(state), (WORD)context.SegCs);
}

LPBYTE get_base_addr(WORD sel)
{
    return wine_ldt_get_base(wine_ldt + (sel >> 3));
}
static uint64 dr7;
void set_break_point(struct whpx_vcpu_state *state, WORD sel, DWORD addr, int breakpoint)
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
        state->dr0.Reg32 = linear;
        break;
    case 1:
        state->dr1.Reg32 = linear;
        break;
    case 2:
        state->dr2.Reg32 = linear;
        break;
    case 3:
        state->dr3.Reg32 = linear;
        break;
    default:
        return;
    }
    state->dr7.Reg32 |= (1 << (2 * breakpoint + 1)) | (0 << (18 + 4 * breakpoint));
    state->dr6.Reg32 = 0;
}
typedef int disassemble_debug_t(char *buffer, UINT8 *oprom, BOOL op_size, UINT64 eip);
static disassemble_debug_t *disassemble_debug;
static void trace(struct whpx_vcpu_state *state, uint16 cs, uint32 eip, uint16 ss, uint32 esp, uint32 eflags)
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
    if (state->fs.Segment.Selector || state->gs.Segment.Selector)
    {
        fprintf(stderr,
            "EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X,"
            "ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X,"
            "ES:%04X,CS:%04X,SS:%04X,DS:%04X,FS:%04x,GS:%04x,"
            "IP:%04X,stack:%08X,"
            "EFLAGS:%08X"
            "\n",
            get_eax(state), get_ecx(state), get_edx(state), get_ebx(state),
            esp, get_ebp(state), get_esi(state), get_edi(state),
            get_seg_selector(get_es(state)), cs, ss, get_seg_selector(get_ds(state)), get_seg_selector(get_fs(state)), get_seg_selector(get_gs(state)),
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
            get_eax(state), get_ecx(state), get_edx(state), get_ebx(state),
            esp, get_ebp(state), get_esi(state), get_edi(state),
            get_seg_selector(get_es(state)), cs, ss, get_seg_selector(get_ds(state)),
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

static void set_int()
{
    const WHV_REGISTER_NAME notreg = WHvX64RegisterDeliverabilityNotifications;
    WHV_REGISTER_VALUE delnot = {0};
    delnot.DeliverabilityNotifications.InterruptNotification = 1;
    pWHvSetVirtualProcessorRegisters(partition, 0, &notreg, 1, &delnot);
}

volatile struct
{
    BOOL inject;
    DWORD vpfn16;
    DWORD dwFlags;
    DWORD cbArgs;
    LPVOID pArgs;
    LPDWORD pdwRetCode;
} vm_inject_state;
BOOL WINAPI vm_inject(DWORD vpfn16, DWORD dwFlags,
    DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode)
{
    if(dwFlags != WCB16_PASCAL)
        PANIC("vm_inject call not pascal");
    if (TryEnterCriticalSection(&win16_syslevel->crst))
    {
        /* There are no threads running VM. (e.g. call GetMessage) */
        EnterCriticalSection(&inject_crit_section);
        /* 16-bit stack is allocated by thread_attach(krnl386/kernel.c) */
        BOOL result = pWOWCallback16Ex(vpfn16, dwFlags, cbArgs, pArgs, pdwRetCode);
        LeaveCriticalSection(&inject_crit_section);
        LeaveCriticalSection(&win16_syslevel->crst);
        return result;
    }
    EnterCriticalSection(&inject_crit_section);
    {
        if (vm_inject_state.inject)
        {
            /* FIXME: multiple interrupts */
            LeaveCriticalSection(&inject_crit_section);
            return FALSE;
        }
        vm_inject_state.vpfn16 = vpfn16;
        vm_inject_state.dwFlags = dwFlags;
        vm_inject_state.cbArgs = cbArgs;
        vm_inject_state.pArgs = pArgs;
        vm_inject_state.pdwRetCode = pdwRetCode;
        vm_inject_state.inject = TRUE;
        ResetEvent(inject_event);
    }
    LeaveCriticalSection(&inject_crit_section);
    if (pWHvCancelRunVirtualProcessor(partition, 0, 0) != ERROR_SUCCESS)
        set_int();
    WaitForSingleObject(inject_event, INFINITE);
    return TRUE;
}
    
void vm_inject_call(SEGPTR ret_addr, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    pm_interrupt_handler pih, CONTEXT *context)
{
    DWORD ret;
    EnterCriticalSection(&inject_crit_section);
    static char intstack[4096];
    static WORD intstksel = 0;
    if (!intstksel)
        intstksel = SELECTOR_AllocBlock(intstack, 4096, WINE_LDT_FLAGS_DATA);
                    
    WORD sp = context->Esp;
    DWORD ss_base = get_base_addr(context->SegSs);
    if (wine_ldt_get_flags(wine_ldt + (context->SegSs >> 3)) & WINE_LDT_FLAGS_32BIT) // don't call int handler on a large stack
    {
        ss_base = (DWORD)intstack;
        sp = 4096;
        dynamic_setWOW32Reserved((PVOID)MAKESEGPTR(intstksel, 4096));
    }
    {
        char *stack = ss_base + sp - vm_inject_state.cbArgs;
        vm_inject_state.inject = FALSE;
        memcpy(stack, vm_inject_state.pArgs, vm_inject_state.cbArgs);
        /* push return address */
        stack -= sizeof(SEGPTR);
        *((SEGPTR *)stack) = ret_addr;
        vm_inject_state.cbArgs += sizeof(SEGPTR);
    }
    LeaveCriticalSection(&inject_crit_section);
    ret = wine_call_to_16_vm86(vm_inject_state.vpfn16, vm_inject_state.cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, FALSE, NULL, pih);
    if (vm_inject_state.pdwRetCode)
        *vm_inject_state.pdwRetCode = ret;
    SetEvent(inject_event);
}

BOOL syscall_init = FALSE;
void vm86main(CONTEXT *context, DWORD csip, DWORD sssp, DWORD cbArgs, PEXCEPTION_HANDLER handler,
    void(*from16_reg)(void),
    LONG(*__wine_call_from_16)(void),
    int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
    void(*__wine_call_to_16_ret)(void),
    int dasm,
    pm_interrupt_handler pih
)
{
    DWORD old_frame16 = PtrToUlong(dynamic_getWOW32Reserved());
    HRESULT result;
    EnterCriticalSection(&running_critical_section);
    if (!syscall_init)
    {
        SIZE_T page1 = (SIZE_T)from16_reg & ~0xfff;
        SIZE_T page2 = (SIZE_T)__wine_call_from_16 & ~0xfff;
        SIZE_T page3 = (SIZE_T)__wine_call_to_16_ret & ~0xfff;
        pWHvUnmapGpaRange(partition, page1, 0x1000);
        pWHvUnmapGpaRange(partition, page2, 0x1000);
        pWHvUnmapGpaRange(partition, page3, 0x1000);
        syscall_init = TRUE;
    }
    is_single_step = dasm;
    DWORD ret_addr;
    struct whpx_vcpu_state state;
    {
        if (FAILED(result = pWHvGetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state.values)))
        {
            PANIC_HRESULT("WHvGetVirtualProcessorRegisters", result);
        }
        load_seg(&state.gs, (WORD)0);
        load_seg(&state.fs, (WORD)0);
        if (!csip)
        {
            load_seg(&state.es, (WORD)context->SegEs);
            load_seg(&state.ds, (WORD)context->SegDs);
            load_seg(&state.cs, (WORD)context->SegCs);
            load_seg(&state.ss, (WORD)context->SegSs);

            state.rdi.Reg32 = context->Edi;
            state.rsi.Reg32 = context->Esi;
            state.rbx.Reg32 = context->Ebx;
            state.rdx.Reg32 = context->Edx;
            state.rcx.Reg32 = context->Ecx;
            state.rax.Reg32 = context->Eax;

            state.rbp.Reg32 = context->Ebp;
            state.rip.Reg32 = context->Eip;
            set_eflags(&state, context->EFlags);
            state.rsp.Reg32 = context->Esp - cbArgs;
        }
        else
        {
            load_seg(&state.cs, (WORD)SELECTOROF(csip));
            load_seg(&state.ss, (WORD)SELECTOROF(sssp));
            state.rip.Reg32 = OFFSETOF(csip);
            state.rsp.Reg32 = OFFSETOF(sssp) - cbArgs;
        }
        if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state.values)))
        {
            PANIC_HRESULT("WHvSetVirtualProcessorRegisters", result);
        }
        unsigned char *stack = (unsigned char*)state.ss.Segment.Base + state.rsp.Reg32;
        ret_addr = (*(LPDWORD)stack);
    }
    struct whpx_vcpu_state state2 = state;
    if (is_single_step)
    {
        trace(&state2, get_seg_selector(get_cs(&state2)), get_eip(&state2), get_seg_selector(get_ss(&state2)), get_esp(&state2), get_eflags(&state2));
    }
    while (TRUE)
    {
        WHV_RUN_VP_EXIT_CONTEXT exit;
        if (state2.cs.Segment.Selector == (ret_addr >> 16) && state2.rip.Reg32 == (ret_addr & 0xFFFF))
        {
            if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
            {
                PANIC_HRESULT("WHvSetVirtualProcessorRegisters", result);
            }
            break;
        }
        if (FAILED(result = pWHvRunVirtualProcessor(partition, 0, &exit, sizeof(exit))))
        {
            LeaveCriticalSection(&running_critical_section);
            PANIC_HRESULT("WHvRunVirtualProcessor", result);
            return;
        }
        if (FAILED(result = pWHvGetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
        {
            PANIC_HRESULT("WHvGetVirtualProcessorRegisters", result);
        }
        UINT32 ptr = (UINT32)exit.VpContext.Cs.Base + (UINT32)exit.VpContext.Rip;
        switch (exit.ExitReason)
        {
        case WHvRunVpExitReasonMemoryAccess:
            if (exit.MemoryAccess.AccessInfo.GpaUnmapped)
            {
                if (exit.MemoryAccess.Gva == exit.VpContext.Rip && exit.VpContext.Cs.Selector == seg_cs)
                {
                    BOOL is_reg = ptr == from16_reg;
                    if (is_reg || ptr == __wine_call_from_16)
                    {
                        LeaveCriticalSection(&running_critical_section);
                        relay(relay_call_from_16, is_reg, &state2, ret_addr, cbArgs, handler, old_frame16);
                        EnterCriticalSection(&running_critical_section);
                    }
                }
                if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
                {
                    PANIC_HRESULT("WHvSetVirtualProcessorRegisters", result);
                }
            }
            break;
        case WHvRunVpExitReasonX64Halt:
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
                set_eip(&state2, 256);
                if (name)
                {
                    PANIC("exception");
                }
                else
                {
                    DWORD eip = POP32(&state2);
                    DWORD cs = POP32(&state2);
                    DWORD eflags = POP32(&state2);
                    DWORD esp = POP32(&state2);
                    DWORD ss = POP32(&state2);
                    load_seg(get_cs(&state2), (WORD)cs);
                    set_eip(&state2, eip);
                    load_seg(get_ss(&state2), (WORD)ss);
                    set_esp(&state2, esp);
                    CONTEXT ctx;
                    save_context_from_state(&ctx, &state2);
                    if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
                    {

                    }
                    LeaveCriticalSection(&running_critical_section);
                    dynamic__wine_call_int_handler(&ctx, intvec);
                    EnterCriticalSection(&running_critical_section);
                    load_context_to_state(&ctx, &state2);
                    if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
                    {

                    }
                }
            }
            else
            {
                PANIC("HALT");
            }
            break;
        case WHvRunVpExitReasonCanceled:
        {
            if (!vm_inject_state.inject)
                PANIC("unexpected vm cancellation");
            const WHV_REGISTER_NAME reg = WHvX64RegisterRflags;
            WHV_REGISTER_VALUE flags = {0};
            pWHvGetVirtualProcessorRegisters(partition, 0, &reg, 1, &flags);
            if (!(flags.Reg32 & 0x200))
            {
                set_int();
                break;
            }
        }
        case WHvRunVpExitReasonX64InterruptWindow:
        {
            if (!vm_inject_state.inject)
                PANIC("unexpected vm interrupt");
            CONTEXT ictx;
            save_context_from_state(&ictx, &state2);
            vm_inject_call(ret_addr, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih, &ictx);
            load_context_to_state(&ictx, &state2);
            if (FAILED(result = pWHvSetVirtualProcessorRegisters(partition, 0, whpx_vcpu_reg_names, ARRAYSIZE(whpx_vcpu_reg_names), state2.values)))
            {

            }
            break;
        }
        case WHvRunVpExitReasonX64IoPortAccess:
        {
            WHV_X64_IO_PORT_ACCESS_CONTEXT *io = &exit.IoPortAccess;
            WHV_REGISTER_VALUE vals[] = {{0}, {0}};
            const WHV_REGISTER_NAME regs[] = {WHvX64RegisterRip, WHvX64RegisterRax, WHvX64RegisterRflags};
            vals[0].Reg32 = exit.VpContext.Rip + exit.VpContext.InstructionLength;
            vals[1].Reg64 = io->Rax;
            if(!(io->AccessInfo.StringOp))
            {
                switch(io->AccessInfo.AccessSize)
                {
                    case 1:
                        if(!io->AccessInfo.IsWrite)
                            vals[1].Reg64 = (io->Rax & ~0xff) | DOSVM_inport(io->PortNumber, 1);
                        else
                            DOSVM_outport(io->PortNumber, io->Rax, 1);
                        break;
                    case 2:
                        if(!io->AccessInfo.IsWrite)
                        {
                            vals[1].Reg64 = (io->Rax & ~0xffff);
                            vals[1].Reg64 |= DOSVM_inport(io->PortNumber, 1);
                            vals[1].Reg64 |= DOSVM_inport(io->PortNumber + 1, 1) << 8;
                        }
                        else
                        {
                            DOSVM_outport(io->PortNumber, io->Rax, 1);
                            DOSVM_outport(io->PortNumber + 1, io->Rax >> 8, 1);
                        }
                        break;
                    case 4:
                        if(!io->AccessInfo.IsWrite)
                        {
                            vals[1].Reg64 = DOSVM_inport(io->PortNumber, 1);
                            vals[1].Reg64 |= DOSVM_inport(io->PortNumber + 1, 1) << 8;
                            vals[1].Reg64 |= DOSVM_inport(io->PortNumber + 2, 1) << 16;
                            vals[1].Reg64 |= DOSVM_inport(io->PortNumber + 3, 1) << 24;
                        }
                        else
                        {
                            DOSVM_outport(io->PortNumber, io->Rax, 1);
                            DOSVM_outport(io->PortNumber + 1, io->Rax >> 8, 1);
                            DOSVM_outport(io->PortNumber + 2, io->Rax >> 16, 1);
                            DOSVM_outport(io->PortNumber + 3, io->Rax >> 24, 1);
                        }
                        break;
                }
            }
            else
            {
                UINT32 count = io->AccessInfo.RepPrefix ? io->Rcx : 1;
                UINT8 *addr;
                if(!io->AccessInfo.IsWrite)
                    addr = mem + io->Ds.Base + io->Rsi;
                else
                    addr = mem + io->Es.Base + io->Rdi;
                for(int i = 0; i < count; i++)
                {
                    addr = exit.VpContext.Rflags & 0x400 ? addr - io->AccessInfo.AccessSize : addr + io->AccessInfo.AccessSize;
                    switch(io->AccessInfo.AccessSize)
                    {
                        case 1:
                            if(!io->AccessInfo.IsWrite)
                                DOSVM_outport(io->PortNumber, *addr, 1);
                            else
                                *addr = DOSVM_inport(io->PortNumber, 1);
                            break;
                        case 2:
                            if(!io->AccessInfo.IsWrite)
                            {
                                DOSVM_outport(io->PortNumber, *addr, 1);
                                DOSVM_outport(io->PortNumber + 1, *(addr + 1), 1);
                            }
                            else
                            {
                                *addr = DOSVM_inport(io->PortNumber, 1);
                                *(addr + 1) = DOSVM_inport(io->PortNumber + 1, 1);
                            }
                            break;
                        case 4:
                            if(!io->AccessInfo.IsWrite)
                            {
                                DOSVM_outport(io->PortNumber, *addr, 1);
                                DOSVM_outport(io->PortNumber + 1, *(addr + 1), 1);
                                DOSVM_outport(io->PortNumber + 2, *(addr + 2), 1);
                                DOSVM_outport(io->PortNumber + 3, *(addr + 3), 1);
                            }
                            else
                            {
                                *addr = DOSVM_inport(io->PortNumber, 1);
                                *(addr + 1) = DOSVM_inport(io->PortNumber + 1, 1);
                                *(addr + 2) = DOSVM_inport(io->PortNumber + 2, 1);
                                *(addr + 3) = DOSVM_inport(io->PortNumber + 3, 1);
                            }
                            break;
                    }
                }
            }
            pWHvSetVirtualProcessorRegisters(partition, 0, regs, 2, vals);
            break;
        }
        default:
            PANIC("unexpected exit reason %d", exit.ExitReason);
            break;
        }
    }
    save_context_from_state(context, &state2);
    LeaveCriticalSection(&running_critical_section);
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
    CONTEXT context = { 0 };
    vm86main(&context, target, (DWORD)dynamic_getWOW32Reserved(), cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih);
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
    vm86main(context, 0, 0, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih);


}
