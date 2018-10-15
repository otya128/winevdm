#include <Windows.h>
#define new _new
extern "C"
{
#include "config.h"
#undef __oaidl_h__

#include <stdarg.h>
#include "wine/debug.h"
#include "user_private.h"
	WINE_DEFAULT_DEBUG_CHANNEL(stub);
	PDrawCaptionTempA DrawCaptionTempA = 0;
	PScrollChildren ScrollChildren;
	void WINAPI CalcChildScrollImpl(HWND hWnd, INT n)
	{
		FIXME("CalcChildScrollImpl(%p, %d)\n", hWnd, n);
	}
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
#define LOAD_FUNC_FUNC(dll, name, defaultfunc) LOAD_FUNC(dll, name); if(!name) name = defaultfunc;
	DrawCaptionTempA = (PDrawCaptionTempA)GetProcAddress(GetModuleHandleA("USER32.DLL"), "DrawCaptionTempA");
	 
	LOAD_FUNC("USER32.DLL", ScrollChildren);
	LOAD_FUNC_FUNC("USER32.DLL", CalcChildScroll, CalcChildScrollImpl);
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

    /* wine adb926d5a5032130d55e4d39316efdb11cc1fe1d/dlls/gdi32/font.c */
    /***********************************************************************
     *           GdiGetCharDimensions    (GDI32.@)
     *
     * Gets the average width of the characters in the English alphabet.
     *
     * PARAMS
     *  hdc    [I] Handle to the device context to measure on.
     *  lptm   [O] Pointer to memory to store the text metrics into.
     *  height [O] On exit, the maximum height of characters in the English alphabet.
     *
     * RETURNS
     *  The average width of characters in the English alphabet.
     *
     * NOTES
     *  This function is used by the dialog manager to get the size of a dialog
     *  unit. It should also be used by other pieces of code that need to know
     *  the size of a dialog unit in logical units without having access to the
     *  window handle of the dialog.
     *  Windows caches the font metrics from this function, but we don't and
     *  there doesn't appear to be an immediate advantage to do so.
     *
     * SEE ALSO
     *  GetTextExtentPointW, GetTextMetricsW, MapDialogRect.
     */
    LONG WINAPI GdiGetCharDimensions(HDC hdc, LPTEXTMETRICW lptm, LONG *height)
    {
        SIZE sz;
        static const WCHAR alphabet[] = {
            'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q',
            'r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H',
            'I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',0};

        if(lptm && !GetTextMetricsW(hdc, lptm)) return 0;

        if(!GetTextExtentPointW(hdc, alphabet, 52, &sz)) return 0;

        if (height) *height = sz.cy;
        return (sz.cx / 26 + 1) / 2;
    }
}
