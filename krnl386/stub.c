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
void DPMI_CallRMCB32(RMCB *rmcb, UINT16 ss, DWORD esp, UINT16*es, DWORD*edi)
{
	DPRINTF("NOTIMPL:DPMI_CallRMCB32(%p, %u, %u, %p, %p)", rmcb, ss, esp, es, edi);
	__ASM_GLOBAL_FUNC(DPMI_CallRMCB32,
		"pushl %ebp\n\t"
		__ASM_CFI(".cfi_adjust_cfa_offset 4\n\t")
		__ASM_CFI(".cfi_rel_offset %ebp,0\n\t")
		"movl %esp,%ebp\n\t"
		__ASM_CFI(".cfi_def_cfa_register %ebp\n\t")
		"pushl %edi\n\t"
		__ASM_CFI(".cfi_rel_offset %edi,-4\n\t")
		"pushl %esi\n\t"
		__ASM_CFI(".cfi_rel_offset %esi,-8\n\t")
		"movl 0x8(%ebp),%eax\n\t"
		"movl 0x10(%ebp),%esi\n\t"
		"movl 0xc(%ebp),%edx\n\t"
		"movl 0x10(%eax),%ecx\n\t"
		"movl 0xc(%eax),%edi\n\t"
		"addl $0x4,%eax\n\t"
		"pushl %ebp\n\t"
		"pushl %ebx\n\t"
		"pushl %es\n\t"
		"pushl %ds\n\t"
		"pushfl\n\t"
		"mov %cx,%es\n\t"
		"mov %dx,%ds\n\t"
		".byte 0x36, 0xff, 0x18\n\t" /* lcall *%ss:(%eax) */
		"popl %ds\n\t"
		"mov %es,%dx\n\t"
		"popl %es\n\t"
		"popl %ebx\n\t"
		"popl %ebp\n\t"
		"movl 0x14(%ebp),%eax\n\t"
		"movw %dx,(%eax)\n\t"
		"movl 0x18(%ebp),%edx\n\t"
		"movl %edi,(%edx)\n\t"
		"popl %esi\n\t"
		__ASM_CFI(".cfi_same_value %esi\n\t")
		"popl %edi\n\t"
		__ASM_CFI(".cfi_same_value %edi\n\t")
		"leave\n\t"
		__ASM_CFI(".cfi_def_cfa %esp,4\n\t")
		__ASM_CFI(".cfi_same_value %ebp\n\t")
		"ret")
}
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
BOOL    WINAPI VerifyConsoleIoHandle(HANDLE h)
{
	static PVerifyConsoleIoHandle func = 0;
	if (!func)
		func = GetProcAddress(GetModuleHandleA("kernel32.dll"), "VerifyConsoleIoHandle");
	if (!func)
	{
		DPRINTF("can't load kernel32!VerifyConsoleIoHandle\n");
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
		DPRINTF("can't load kernel32!VerifyConsoleIoHandle\n");
	}
	return func(hSrc);
}
DWORD CallTo16_DataSelector DECLSPEC_HIDDEN;
DWORD CallTo16_TebSelector DECLSPEC_HIDDEN;
SEGPTR CALL32_CBClient_RetAddr DECLSPEC_HIDDEN;
SEGPTR CALL32_CBClientEx_RetAddr DECLSPEC_HIDDEN;

void CALL32_CBClient_Ret(void)
{
	DPRINTF("NOTIMPL:CALL32_CBClient_Ret()");
}
void CALL32_CBClientEx_Ret(void)
{
	DPRINTF("NOTIMPL:CALL32_CBClientEx_Ret()");
}
WINE_DECLARE_DEBUG_CHANNEL(disasm);
void __wine_call_to_16_ret(void);
/* symbols exported from relay16.s */
//zatsu
char *DOSMEM_dosmem;
DWORD WINAPI wine_call_to_16(FARPROC16 target, DWORD cbArgs, PEXCEPTION_HANDLER handler)
{
	//DPRINTF("NOTIMPL:wine_call_to_16(%p, %u, %p)", target, cbArgs, handler);
	return wine_call_to_16_vm86(target, cbArgs, handler, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, TRACE_ON(disasm), FALSE, DOSMEM_dosmem);

	return 0;
}
 void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler);
void WINAPI wine_call_to_16_regs(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler)
{
	//DPRINTF("NOTIMPL:wine_call_to_16_regs(%p, %u, %p)", context, cbArgs, handler);
	//why??
	context->SegSs = SELECTOROF(getWOW32Reserved());
	context->Esp = OFFSETOF(getWOW32Reserved());
	wine_call_to_16_regs_vm86(context, cbArgs, handler, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, TRACE_ON(disasm), FALSE, DOSMEM_dosmem);
}
/*
int call_entry_point(void *func, int nb_args, const DWORD *args)
{
	DPRINTF("NOTIMPL:call_entry_point(%p, %d, %p)", func, nb_args, args);
}*/
void __wine_call_from_16_thunk(void)
{
	DPRINTF("NOTIMPL:__wine_call_from_16_thunk()");
}
/*
void FT_Prolog(void)
{
	DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}
void FT_PrologPrime(void)
{
	DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}
void QT_Thunk(void)
{
	DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}
void QT_ThunkPrime(void)
{
	DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}*/
LONG __wine_call_from_16(void)
{
	//DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
	return 0;
}
void __wine_call_from_16_regs(void)
{
	//DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}
BYTE __wine_call16_start[1];
BYTE __wine_call16_end[1];
void __wine_call_to_16_ret(void)
{
	//DPRINTF("NOTIMPL:" __FUNCTION__ "()\n");
}
DWORD CALL32_CBClient(FARPROC proc, LPWORD args, WORD *stackLin, DWORD *esi)
{
	DPRINTF("NOTIMPL:CALL32_CBClient(%p, %p, %p, %p)\n", proc, args, stackLin, esi);
	return 0;
}
DWORD CALL32_CBClientEx(FARPROC proc, LPWORD args, WORD *stackLin, DWORD *esi, INT *nArgs)
{
	DPRINTF("NOTIMPL:CALL32_CBClientEx(%p, %p, %p, %p, %p)\n", proc, args, stackLin, esi, nArgs);
	return 0;
}
void __wine_enter_vm86(CONTEXT *context)
{
    wine_call_to_16_regs_vm86(context, NULL, NULL, __wine_call_from_16_regs, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, TRACE_ON(disasm), TRUE, DOSMEM_dosmem);
	DPRINTF("NOTIMPL:__wine_enter_vm86(%p)\n", context);
}
void __wine_spec_init_ctor()
{
	DPRINTF("NOTIMPL:__wine_spec_init_ctor()\n");
}
void __wine_spec_unimplemented_stub(const char *module, const char *function)
{
	DPRINTF("NOTIMPL:__wine_spec_unimplemented_stub(%s, %s)\n", module, function);
}
BOOL16 WINAPI IsDBCSLeadByte16(BYTE TestChar)
{
	TRACE("IsDBCSLeadByte16_(%c)", TestChar);
	return IsDBCSLeadByte(TestChar);
}
//TLS
__declspec(thread) PVOID WOW32Reserved;
__declspec(dllexport) PVOID getWOW32Reserved()
{
	return WOW32Reserved;
}
__declspec(dllexport) PVOID setWOW32Reserved(PVOID w)
{
	return WOW32Reserved = w;
}
