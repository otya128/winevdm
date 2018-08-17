/*
 * USER private definitions
 *
 * Copyright 1993, 2009 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_USER_PRIVATE_H
#define __WINE_USER_PRIVATE_H

#include <stdarg.h>
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winreg.h"
#include "winternl.h"
#include "wine/windef16.h"
#include "wine/library.h"

/* Wow handlers */

/* the structures must match the corresponding ones in user32 */
struct wow_handlers16
{
    LRESULT (*button_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*combo_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*edit_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*listbox_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*mdiclient_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*scrollbar_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*static_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    DWORD   (*wait_message)(DWORD,const HANDLE*,DWORD,DWORD,DWORD);
    HWND    (*create_window)(CREATESTRUCTW*,LPCWSTR,HINSTANCE,BOOL);
    LRESULT (*call_window_proc)(HWND,UINT,WPARAM,LPARAM,LRESULT*,void*);
    LRESULT (*call_dialog_proc)(HWND,UINT,WPARAM,LPARAM,LRESULT*,void*);
    void    (*free_icon_param)(ULONG_PTR);
};

struct wow_handlers32
{
    LRESULT (*button_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*combo_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*edit_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*listbox_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*mdiclient_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*scrollbar_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    LRESULT (*static_proc)(HWND,UINT,WPARAM,LPARAM,BOOL);
    DWORD   (*wait_message)(DWORD,const HANDLE*,DWORD,DWORD,DWORD);
    HWND    (*create_window)(CREATESTRUCTW*,LPCWSTR,HINSTANCE,BOOL);
    HWND    (*get_win_handle)(HWND);
    WNDPROC (*alloc_winproc)(WNDPROC,BOOL);
    struct tagDIALOGINFO *(*get_dialog_info)(HWND,BOOL);
    INT     (*dialog_box_loop)(HWND,HWND);
    ULONG_PTR (*get_icon_param)(HICON);
    ULONG_PTR (*set_icon_param)(HICON,ULONG_PTR);
};

extern struct wow_handlers32 wow_handlers32 DECLSPEC_HIDDEN;

extern HWND create_window16(CREATESTRUCTW*,LPCWSTR,HINSTANCE,BOOL) DECLSPEC_HIDDEN;
extern void free_module_classes(HINSTANCE16) DECLSPEC_HIDDEN;
extern void register_wow_handlers(void) DECLSPEC_HIDDEN;
//extern void WINAPI UserRegisterWowHandlers( const struct wow_handlers16 *new,
//                                            struct wow_handlers32 *orig );

static inline HWND WIN_Handle32( HWND16 hwnd16 )
{
    return wow_handlers32.get_win_handle( (HWND)(ULONG_PTR)hwnd16 );
}

typedef LRESULT (*winproc_callback_t)( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                       LRESULT *result, void *arg );
typedef LRESULT (*winproc_callback16_t)( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
                                         LRESULT *result, void *arg );

extern WNDPROC16 WINPROC_GetProc16( WNDPROC proc, BOOL unicode ) DECLSPEC_HIDDEN;
extern WNDPROC WINPROC_AllocProc16( WNDPROC16 func ) DECLSPEC_HIDDEN;
extern LRESULT WINPROC_CallProc16To32A( winproc_callback_t callback, HWND16 hwnd, UINT16 msg,
                                        WPARAM16 wParam, LPARAM lParam, LRESULT *result, void *arg ) DECLSPEC_HIDDEN;
extern LRESULT WINPROC_CallProc32ATo16( winproc_callback16_t callback, HWND hwnd, UINT msg,
                                        WPARAM wParam, LPARAM lParam, LRESULT *result, void *arg ) DECLSPEC_HIDDEN;

extern void call_WH_CALLWNDPROC_hook( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp ) DECLSPEC_HIDDEN;

#define GET_WORD(ptr)  (*(const WORD *)(ptr))
#define GET_DWORD(ptr) (*(const DWORD *)(ptr))

#define WM_SYSTIMER 0x0118

/* Dialog info structure (must match the user32 one) */
typedef struct tagDIALOGINFO
{
    HWND      hwndFocus;   /* Current control with focus */
    HFONT     hUserFont;   /* Dialog font */
    HMENU     hMenu;       /* Dialog menu */
    UINT      xBaseUnit;   /* Dialog units (depends on the font) */
    UINT      yBaseUnit;
    INT       idResult;    /* EndDialog() result / default pushbutton ID */
    UINT      flags;       /* EndDialog() called for this dialog */
} DIALOGINFO;

#define DF_END  0x0001
#define DF_OWNERENABLED 0x0002

/* HANDLE16 <-> HANDLE conversions */
#define HINSTANCE_16(h32)  (LOWORD(h32))
#define HINSTANCE_32(h16)  ((HINSTANCE)(ULONG_PTR)(h16))

extern HICON16 get_icon_16( HICON icon ) DECLSPEC_HIDDEN;
extern HICON get_icon_32( HICON16 icon16 ) DECLSPEC_HIDDEN;

extern DWORD USER16_AlertableWait DECLSPEC_HIDDEN;
extern WORD USER_HeapSel DECLSPEC_HIDDEN;
//add
/* those defines are (some of the) regular LDR_MODULE.Flags values */
#define LDR_IMAGE_IS_DLL                0x00000004
#define LDR_LOAD_IN_PROGRESS            0x00001000
#define LDR_UNLOAD_IN_PROGRESS          0x00002000
#define LDR_NO_DLL_CALLS                0x00040000
#define LDR_PROCESS_ATTACHED            0x00080000
#define LDR_MODULE_REBASED              0x00200000

/* these ones is Wine specific */
#define LDR_DONT_RESOLVE_REFS           0x40000000
#define LDR_WINE_INTERNAL               0x80000000

/* flag for LdrAddRefDll */
#define LDR_ADDREF_DLL_PIN              0x00000001

/* Wine extensions: call register function, context ptr is passed in the return value LPDWORD */
#define WCB16_REGS       2
#define WCB16_REGS_LONG  4  /* function uses 32-bit lret */
/* Windows Exit Procedure flag values */
#define	WEP_FREE_DLL        0
#define	WEP_SYSTEM_EXIT     1
//wine winbase.h
#define OF_SEARCH             0x0400   /* Used without OF_REOPEN */
/* GetTempFileName() Flags */
#define TF_FORCEDRIVE	        0x80
//winnt.h
/* This flag allows it to create heaps shared by all processes under win95,
FIXME: correct name */
#define HEAP_SHARED                     0x04000000


#define WH_HARDWARE	    8
#define WM_CTLCOLOR            0x0019 /* Added from windowsx.h */
//winuser
#define LB_CARETON             0x01a3
#define LB_CARETOFF            0x01a4
#include <windowsx.h>
//#include "winuser.rh"
#define WM_SIZEWAIT            0x0004 /* DDK / Win16 */
#define WM_SETVISIBLE          0x0009 /* DDK / Win16 */
#define WM_SYSTEMERROR         0x0017 /* DDK / Win16 */
#define WM_ALTTABACTIVE        0x0029 /* DDK / Win16 */
#define WM_FILESYSCHANGE       0x0034 /* DDK / Win16 */
#define WM_ISACTIVEICON        0x0035 /* DDK / Win16 */
#define WM_TESTING             0x003a /* DDK / Win16 */
#define WM_KEYF1               0x004d /* DDK / Win16 */
#define WM_SYNCTASK            0x0089 /* DDK / Win16 */
#define WM_LBTRACKPOINT        0x0131 /* DDK / Win16 */
/* D&D messages */
#define WM_DROPOBJECT          0x022A /* DDK / Win16 */
#define WM_QUERYDROPOBJECT     0x022B /* DDK / Win16 */
#define WM_BEGINDRAG           0x022C /* DDK / Win16 */
#define WM_DRAGLOOP            0x022D /* DDK / Win16 */
#define WM_DRAGSELECT          0x022E /* DDK / Win16 */
#define WM_DRAGMOVE            0x022F /* DDK / Win16 */
/* DDK / Win16 defines */
#define MFS_MASK               __MSABI_LONG(0x0000108B)
#define MFS_HOTTRACKDRAWN      __MSABI_LONG(0x10000000)
#define MFS_CACHEDBMP          __MSABI_LONG(0x20000000)
#define MFS_BOTTOMGAPDROP      __MSABI_LONG(0x40000000)
#define MFS_TOPGAPDROP         __MSABI_LONG(0x80000000)
#define MFS_GAPDROP            (MFS_BOTTOMGAPDROP | MFS_TOPGAPDROP)

#define OCR_HELP               32651 /* DDK / Win16 */
#define OCR_RDRVERT            32652 /* DDK / Win16 */
#define OCR_RDRHORZ            32653 /* DDK / Win16 */
#define OCR_RDR2DIM            32654 /* DDK / Win16 */
#define OCR_RDRNORTH           32655 /* DDK / Win16 */
#define OCR_RDRSOUTH           32656 /* DDK / Win16 */
#define OCR_RDRWEST            32657 /* DDK / Win16 */
#define OCR_RDREAST            32658 /* DDK / Win16 */
#define OCR_RDRNORTHWEST       32659 /* DDK / Win16 */
#define OCR_RDRNORTHEAST       32660 /* DDK / Win16 */
#define OCR_RDRSOUTHWEST       32661 /* DDK / Win16 */
#define OCR_RDRSOUTHEAST       32662 /* DDK / Win16 */
#define EM_SETFONT             0x00c3 /* DDK / Win16 */
#define EM_SETWORDBREAK        0x00ca /* DDK / Win16 */
//winuser
#define DCX_USESTYLE         0x00010000
__declspec(dllimport) PVOID getWOW32Reserved();
__declspec(dllimport) PVOID setWOW32Reserved(PVOID w);
typedef LONG(WINAPI *PGdiGetCharDimensions)
(
HDC           hdc,
LPTEXTMETRICW lptm,
LONG*         height
);
inline LONG WINAPI GdiGetCharDimensions
(
HDC           hdc,
LPTEXTMETRICW lptm,
LONG*         height
)
{
	static PGdiGetCharDimensions func;
	if (!func)
		(PGdiGetCharDimensions)GetProcAddress(GetModuleHandleA("GDI32.DLL"), "GdiGetCharDimensions");
	return func(hdc, lptm, height);
}
typedef BOOL(WINAPI *PDrawCaptionTempA)
(
HWND hwnd,
HDC hdc,
const RECT *rect,
HFONT hFont,
HICON hIcon,
LPCSTR str,
UINT32 uFlags
);
extern PDrawCaptionTempA DrawCaptionTempA;
typedef VOID(WINAPI *PScrollChildren)(HWND, UINT, WPARAM, LPARAM);
extern PScrollChildren ScrollChildren;
typedef void        (WINAPI *PCalcChildScroll)(HWND, INT);
extern PCalcChildScroll CalcChildScroll;
typedef void        (WINAPI *PSetInternalWindowPos)(HWND, UINT, LPRECT, LPPOINT);
extern PSetInternalWindowPos SetInternalWindowPos;
typedef BOOL        (WINAPI *PSetSystemMenu)(HWND, HMENU);
extern PSetSystemMenu SetSystemMenu;
typedef BOOL        (WINAPI *PKillSystemTimer)(HWND, UINT_PTR);
extern PKillSystemTimer KillSystemTimer;
typedef UINT_PTR    (WINAPI *PSetSystemTimer)(HWND, UINT_PTR, UINT, TIMERPROC);
extern PSetSystemTimer SetSystemTimer;
typedef BOOL        (WINAPI *PSetDeskWallPaper)(LPCSTR);
extern PSetDeskWallPaper SetDeskWallPaper;
typedef UINT        (WINAPI *PUserRealizePalette)(HDC);
extern PUserRealizePalette UserRealizePalette;
typedef void (WINAPI *PUserRegisterWowHandlers)(const struct wow_handlers16 *new, struct wow_handlers32 *orig);
extern PUserRegisterWowHandlers UserRegisterWowHandlers;
#define strncasecmp _strnicmp
__declspec(dllimport) void SetWndProc16(WORD hWnd16, DWORD WndProc);
__declspec(dllimport) DWORD GetWndProc16(WORD hWnd16);
#define STR_ATOM_MIN MAXINTATOM
#define STR_ATOM_MAX 0xFFFF
#define STR_ATOM_SIZE STR_ATOM_MAX - STR_ATOM_MIN
struct WNDCLASS16Info
{
    BOOL allocated;
    DWORD wndproc;
    WORD cbWndExtra;
};
extern struct WNDCLASS16Info WNDCLASS16Info[65536];
extern struct WNDCLASS16Info *WNDCLASS16InfoStringAtom[65536];
#include "wine/list.h"

struct class_entry
{
    struct list entry;
    ATOM        atom;
    HINSTANCE16 inst;
    DWORD wndproc16;


    WNDCLASSEXA classInfo;
    LPCSTR win32_classname;
};
struct class_entry *find_win16_class(HINSTANCE16 hInst16, LPCSTR name);
struct class_entry *find_win32_class(LPCSTR name);
LPCSTR win16classname(LPCSTR name);
LPCSTR win32classname(HINSTANCE16 hInst16, LPCSTR name);
#define LOCAL_CLASS_PREFIX "WIN16"

#endif /* __WINE_USER_PRIVATE_H */
