#include <Windows.h>
#define new _new
extern "C"
{
#include "config.h"
#undef __oaidl_h__

#include <stdarg.h>
#include "wine/debug.h"
#include "user_private.h"
	PDrawCaptionTempA DrawCaptionTempA = 0;
	PScrollChildren ScrollChildren;
	PCalcChildScroll CalcChildScroll;
	PSetInternalWindowPos SetInternalWindowPos;
	PSetSystemMenu SetSystemMenu;
	PKillSystemTimer KillSystemTimer;
	PSetSystemTimer SetSystemTimer;
	PSetDeskWallPaper SetDeskWallPaper;
	PUserRealizePalette UserRealizePalette;
	PUserRegisterWowHandlers UserRegisterWowHandlers;
	//struct __wine_ldt_copy *wine_ldt_copy_ptr = &wine_ldt_copy;
	//struct __wine_ldt_copy wine_ldt_copy;
}
int ab();
static int a = ab();
int ab()
{
#define LOAD_FUNC(dll, name) name = (P##name)GetProcAddress(GetModuleHandleA(dll), #name);//if(!name) MessageBoxA(NULL, "can't load " dll "!" #name,"", 0)
	DrawCaptionTempA = (PDrawCaptionTempA)GetProcAddress(GetModuleHandleA("USER32.DLL"), "DrawCaptionTempA");
	 
	LOAD_FUNC("USER32.DLL", ScrollChildren);
	LOAD_FUNC("USER32.DLL", CalcChildScroll);
	LOAD_FUNC("USER32.DLL", SetInternalWindowPos);
	LOAD_FUNC("USER32.DLL", SetSystemMenu);
	LOAD_FUNC("USER32.DLL", KillSystemTimer);
	LOAD_FUNC("USER32.DLL", SetSystemTimer);
	LOAD_FUNC("USER32.DLL", SetDeskWallPaper);
	LOAD_FUNC("USER32.DLL", UserRealizePalette);
	LOAD_FUNC("USER32.DLL", UserRegisterWowHandlers);
	return 0;
}
extern "C"
{
	void __wine_spec_init_ctor()
	{
		DPRINTF("NOTIMPL:__wine_spec_init_ctor()\n");
	}
	void __wine_spec_unimplemented_stub(const char *module, const char *function)
	{
		DPRINTF("NOTIMPL:__wine_spec_unimplemented_stub(%s, %s)\n", module, function);
	}
	void __wine_spec_dll_entry()
	{
		DPRINTF("NOTIMPL:__wine_spec_dll_entry(?)\n");
	}
	WINE_DEFAULT_DEBUG_CHANNEL(stub);
	/***********************************************************************
	*		SignalProc32 (USER.391)
	*		UserSignalProc (USER32.@)
	*
	* The exact meaning of the USER signals is undocumented, but this
	* should cover the basic idea:
	*
	* USIG_DLL_UNLOAD_WIN16
	*     This is sent when a 16-bit module is unloaded.
	*
	* USIG_DLL_UNLOAD_WIN32
	*     This is sent when a 32-bit module is unloaded.
	*
	* USIG_DLL_UNLOAD_ORPHANS
	*     This is sent after the last Win3.1 module is unloaded,
	*     to allow removal of orphaned menus.
	*
	* USIG_FAULT_DIALOG_PUSH
	* USIG_FAULT_DIALOG_POP
	*     These are called to allow USER to prepare for displaying a
	*     fault dialog, even though the fault might have happened while
	*     inside a USER critical section.
	*
	* USIG_THREAD_INIT
	*     This is called from the context of a new thread, as soon as it
	*     has started to run.
	*
	* USIG_THREAD_EXIT
	*     This is called, still in its context, just before a thread is
	*     about to terminate.
	*
	* USIG_PROCESS_CREATE
	*     This is called, in the parent process context, after a new process
	*     has been created.
	*
	* USIG_PROCESS_INIT
	*     This is called in the new process context, just after the main thread
	*     has started execution (after the main thread's USIG_THREAD_INIT has
	*     been sent).
	*
	* USIG_PROCESS_LOADED
	*     This is called after the executable file has been loaded into the
	*     new process context.
	*
	* USIG_PROCESS_RUNNING
	*     This is called immediately before the main entry point is called.
	*
	* USIG_PROCESS_EXIT
	*     This is called in the context of a process that is about to
	*     terminate (but before the last thread's USIG_THREAD_EXIT has
	*     been sent).
	*
	* USIG_PROCESS_DESTROY
	*     This is called after a process has terminated.
	*
	*
	* The meaning of the dwFlags bits is as follows:
	*
	* USIG_FLAGS_WIN32
	*     Current process is 32-bit.
	*
	* USIG_FLAGS_GUI
	*     Current process is a (Win32) GUI process.
	*
	* USIG_FLAGS_FEEDBACK
	*     Current process needs 'feedback' (determined from the STARTUPINFO
	*     flags STARTF_FORCEONFEEDBACK / STARTF_FORCEOFFFEEDBACK).
	*
	* USIG_FLAGS_FAULT
	*     The signal is being sent due to a fault.
	*/
	extern void *_wine_spec_dos_header;
	WORD WINAPI UserSignalProc(UINT uCode, DWORD dwThreadOrProcessID,
		DWORD dwFlags, HMODULE16 hModule)
	{
		//_wine_spec_dos_header = 0;
		FIXME("(%04x, %08x, %04x, %04x)\n",
			uCode, dwThreadOrProcessID, dwFlags, hModule);
		/* FIXME: Should chain to GdiSignalProc now. */
		return 0;
	}
}
