#include "kernel16_private.h"

#include "config.h"
#include "wine/port.h"

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winternl.h"
#include "wine/winbase16.h"
#include "wownt32.h"
#include "kernel16_private.h"
#include "dosexe.h"

#include "excpt.h"
#include "wine/debug.h"
#include "wine/exception.h"

WINE_DEFAULT_DEBUG_CHANNEL(stub);
void DPMI_PendingEventCheck(void)
{
    FIXME("DPMI_PendingEventCheck\n");
}
void DPMI_PendingEventCheck_Cleanup(void)
{
    FIXME("DPMI_PendingEventCheck_Cleanup\n");
}
void DPMI_PendingEventCheck_Return(void)
{
    FIXME("DPMI_PendingEventCheck_Return\n");
}
//int31.c
typedef struct tagRMCB {
    DWORD address;
    DWORD proc_ofs, proc_sel;
    DWORD regs_ofs, regs_sel;
    struct tagRMCB *next;
} RMCB;
#ifdef _MSC_VER
void DPMI_CallRMCB32(RMCB *rmcb, UINT16 ss, DWORD esp, UINT16*es, DWORD*edi)
{
    ERR("NOTIMPL:DPMI_CallRMCB32(%p, %u, %u, %p, %p)\n", rmcb, ss, esp, es, edi);
}
#endif
/* Process flags */
#define PDB32_DEBUGGED      0x0001  /* Process is being debugged */
#define PDB32_WIN16_PROC    0x0008  /* Win16 process */
#define PDB32_DOS_PROC      0x0010  /* Dos process */
#define PDB32_CONSOLE_PROC  0x0020  /* Console process */
#define PDB32_FILE_APIS_OEM 0x0040  /* File APIs are OEM */
#define PDB32_WIN32S_PROC   0x8000  /* Win32s process */
/***********************************************************************
*           GetProcessFlags    (KERNEL32.@)
*/
DWORD WINAPI GetProcessFlags(DWORD processid)
{
    IMAGE_NT_HEADERS *nt;
    DWORD flags = 0;

    if (processid && processid != GetCurrentProcessId()) return 0;

    if ((nt = RtlImageNtHeader(NtCurrentTeb()->Peb->ImageBaseAddress)))
    {
        if (nt->OptionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
            flags |= PDB32_CONSOLE_PROC;
    }
    if (!AreFileApisANSI()) flags |= PDB32_FILE_APIS_OEM;
    if (IsDebuggerPresent()) flags |= PDB32_DEBUGGED;
    return flags;
}
typedef BOOL(WINAPI *PVerifyConsoleIoHandle)(HANDLE);
BOOL WINAPI VerifyConsoleIoHandle(HANDLE h)
{
    static PVerifyConsoleIoHandle func = 0;
    if (!func)
        func = GetProcAddress(GetModuleHandleA("kernel32.dll"), "VerifyConsoleIoHandle");
    if (!func)
    {
        ERR("can't load kernel32!VerifyConsoleIoHandle\n");
    }
    return func(h);
}
typedef HANDLE(WINAPI *PConvertToGlobalHandle)(HANDLE);
HANDLE WINAPI ConvertToGlobalHandle(HANDLE hSrc)
{
    static PConvertToGlobalHandle func = 0;
    if (!func)
        func = GetProcAddress(GetModuleHandleA("kernel32.dll"), "ConvertToGlobalHandle");
    if (!func)
    {
        ERR("can't load kernel32!ConvertToGlobalHandle\n");
    }
    return func(hSrc);
}
DWORD CallTo16_DataSelector DECLSPEC_HIDDEN;
DWORD CallTo16_TebSelector DECLSPEC_HIDDEN;
SEGPTR CALL32_CBClient_RetAddr DECLSPEC_HIDDEN;
SEGPTR CALL32_CBClientEx_RetAddr DECLSPEC_HIDDEN;
WINE_DECLARE_DEBUG_CHANNEL(disasm);
char *DOSMEM_dosmem;
static int get_debug_mode()
{
    return TRACE_ON(disasm);
}
DWORD wine_pm_interrupt_handler(WORD num, DWORD addr)
{
    HTASK16 hTask = GetCurrentTask();
    TDB *pTask = GlobalLock16(hTask);
    if (!pTask)
        return 0;
    DWORD handler = 0;
    switch (num)
    {
    case 0:
        handler = pTask->int0;
        break;
    case 2:
        handler = pTask->int2;
        break;
    case 0x0d:
        /* FIXME: push addr? */
        /* handler = HasGPHandler16(addr); */
        break;
    case 4:
        handler = pTask->int4;
        break;
    case 6:
        handler = pTask->int6;
        break;
    case 7:
        handler = pTask->int7;
        break;
    case 0x3e:
        handler = pTask->int3e;
        break;
    case 0x75:
        handler = pTask->int75;
        break;
    }
    GlobalUnlock16(hTask);
    return handler;
}

extern LPVOID *__wine_call_to_16_ret_p;
wine_call_to_16_vm86_t func_wine_call_to_16_vm86;
wine_call_to_16_regs_vm86_t func_wine_call_to_16_regs_vm86;
DWORD WINAPI wine_call_to_16(FARPROC16 target, DWORD cbArgs, PEXCEPTION_HANDLER handler)
{
    return func_wine_call_to_16_vm86(target, cbArgs, handler, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret_p, get_debug_mode(), FALSE, DOSMEM_dosmem, wine_pm_interrupt_handler);
}
void WINAPI wine_call_to_16_regs(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler)
{
    //why??
    context->SegSs = SELECTOROF(getWOW32Reserved());
    context->Esp = OFFSETOF(getWOW32Reserved());
    func_wine_call_to_16_regs_vm86(context, cbArgs, handler, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret_p, get_debug_mode(), FALSE, DOSMEM_dosmem, wine_pm_interrupt_handler);
}
void __wine_enter_vm86(CONTEXT *context)
{
    func_wine_call_to_16_regs_vm86(context, NULL, NULL, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret_p, get_debug_mode(), TRUE, DOSMEM_dosmem, wine_pm_interrupt_handler);
}
BOOL16 WINAPI IsDBCSLeadByte16(BYTE TestChar)
{
    TRACE("IsDBCSLeadByte16(%c)\n", TestChar);
    return IsDBCSLeadByte(TestChar);
}
extern DWORD WOW32ReservedTls;
__declspec(dllexport) PVOID getWOW32Reserved()
{
    /* TlsGetValue clears win32 last error! */
    /* GetLastError() is called by INT21_GetExtendedError(FIXME?) */
    int err = GetLastError();
    PVOID v = TlsGetValue(WOW32ReservedTls);
    SetLastError(err);
    return v;
}
__declspec(dllexport) PVOID setWOW32Reserved(PVOID w)
{
    int err = GetLastError();
    TlsSetValue(WOW32ReservedTls, w);
    SetLastError(err);
    return w;
}

__declspec(thread) WINE_VM86_TEB_INFO GdiTebBatch;
__declspec(dllexport) WINE_VM86_TEB_INFO *getGdiTebBatch()
{
    return &GdiTebBatch;
}

extern DWORD kernel_thread_data_tls;
__declspec(dllexport) struct kernel_thread_data *tls_get_kernel_thread_data()
{
    return (struct kernel_thread_data*)TlsGetValue(kernel_thread_data_tls);
}
