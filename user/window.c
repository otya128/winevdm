/*
 * 16-bit windowing functions
 *
 * Copyright 2001 Alexandre Julliard
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

#include "wine/winuser16.h"
#include "wownt32.h"
#include "user_private.h"
#include "wine/server.h"
#include "wine/debug.h"
#include "wine/exception.h"

WINE_DEFAULT_DEBUG_CHANNEL(win);

void WINAPI K32WOWHandle16DestroyHint(HANDLE handle, WOW_HANDLE_TYPE type);
BOOL16 WINAPI IsOldWindowsTask(HINSTANCE16 hInst);
/* size of buffer needed to store an atom string */
#define ATOM_BUFFER_SIZE 256

/* handle <--> handle16 conversions */
#define HANDLE_16(h32)		(LOWORD(h32))
#define HANDLE_32(h16)		((HANDLE)(ULONG_PTR)(h16))

/* wine(25cc380b8ed41652b135657ef7651beef2f20ae4) dlls/user32/menu.c */
BOOL is_win_menu_disallowed(DWORD style)
{
    return (style & (WS_CHILD | WS_POPUP)) == WS_CHILD;
}
WNDPROC16 WINPROC_AllocNativeProc_2(WNDPROC func);

static HWND16 hwndSysModal;


static struct list class_list = LIST_INIT( class_list );

struct wnd_enum_info
{
    WNDENUMPROC16 proc;
    LPARAM        param;
};

/* callback for 16-bit window enumeration functions */
static BOOL CALLBACK wnd_enum_callback( HWND hwnd, LPARAM param )
{
    const struct wnd_enum_info *info = (struct wnd_enum_info *)param;
    WORD args[3];
    DWORD ret;

    args[2] = HWND_16(hwnd);
    args[1] = HIWORD(info->param);
    args[0] = LOWORD(info->param);
    WOWCallback16Ex( (DWORD)info->proc, WCB16_PASCAL, sizeof(args), args, &ret );
    return LOWORD(ret);
}

/* convert insert after window handle to 32-bit */
static inline HWND full_insert_after_hwnd( HWND16 hwnd )
{
    if (hwnd == (HWND)0xffff)
        return HWND_TOPMOST;
    if (hwnd == (HWND)0xfffe)
        return HWND_NOTOPMOST;
    if (hwnd == (HWND)HWND_TOP)
        return HWND_TOP;
    if (hwnd == (HWND)HWND_BOTTOM)
        return HWND_BOTTOM;
    HWND ret = WIN_Handle32( hwnd );
    return ret;
}

BOOL CALLBACK remove_wndproc(HWND hwnd, LPARAM lparam)
{
    if (GetExePtr(GetWindowWord16(HWND_16(hwnd), GWL_HINSTANCE)) == lparam)
    {
        TRACE("HWND %p WNDPROC removed\n");
        SetWindowLongPtrA(hwnd, GWLP_WNDPROC, DefWindowProcA);
    }
    return TRUE;
}

BOOL CALLBACK enum_thread_window(HWND hwnd, LPARAM lparam)
{
    struct class_entry *clazz = (struct class_entry*)lparam;
    if (GetClassWord(hwnd, GCW_ATOM) == clazz->atom)
    {
        if (GetWindowLongPtrA(hwnd, GWLP_WNDPROC) != DefWindowProcA)
        {
            TRACE("HWND %p WNDPROC removed\n");
            SetWindowLongPtrA(hwnd, GWLP_WNDPROC, DefWindowProcA);
        }
        TRACE("HWND %p destroyed\n");
        DestroyWindow(hwnd);
    }
    return TRUE;
}

void free_module_classes( HINSTANCE16 inst )
{
    struct class_entry *class, *next;

    EnumThreadWindows(GetCurrentThreadId(), remove_wndproc, (LPARAM)inst);
    LIST_FOR_EACH_ENTRY_SAFE( class, next, &class_list, struct class_entry, entry )
    {
        if (class->inst != inst) continue;
        list_remove( &class->entry );
        if (!UnregisterClassA( (LPCSTR)MAKEINTATOM(class->atom), HINSTANCE_32(class->inst) ))
        {
            if (GetLastError() == ERROR_CLASS_HAS_WINDOWS)
            {
                EnumThreadWindows( GetCurrentThreadId(), enum_thread_window, (LPARAM)class );
                UnregisterClassA( (LPCSTR)MAKEINTATOM(class->atom), HINSTANCE_32(class->inst) );
            }
        }
        HeapFree( GetProcessHeap(), 0, class );
	}
}

BOOL is_dialog(HWND hwnd)
{
    struct WW
    {
        union
        {
            DWORD state;
            struct
            {
                int WFMPRESENT : 1;
                int WFVPRESENT : 1;
                int WFHPRESENT : 1;
                int WFCPRESENT : 1;
                int WFSENDSIZEMOVE : 1;
                int WFMSGBOX : 1;
                int WFFRAMEON : 1;
                int WFHASSPB : 1;
                int WFNONCPAINT : 1;
                int WFSENDERASEBKGND : 1;
                int WFERASEBKGND : 1;
                int WFSENDNCPAINT : 1;
                int WFINTERNALPAINT : 1;
                int WFUPDATEDIRTY : 1;
                int WFHIDDENPOPUP : 1;
                int WFMENUDRAW : 1;
                int WFDIALOGWINDOW : 1;
                /*...*/
            } stateFlags;
        };
        DWORD state2;
        DWORD exstyle;
        DWORD style;
        /*...*/
    };
    /* undocumented, GetWindowLongW(hwnd, -1) returns a pointer to the internal window structure */
    struct WW *ww = GetWindowLongW(hwnd, -1);
    if (!ww)
        return FALSE;
    __TRY
    {
        if (ww->stateFlags.WFDIALOGWINDOW)
            return TRUE;
    }
    __EXCEPT_ALL
    {
    }
	__ENDTRY
    return FALSE;
}

struct ThreadWindows
{
    UINT numHandles;
    UINT numAllocs;
    HWND *handles;
};

static BOOL CALLBACK MSGBOX_EnumProc(HWND hwnd, LPARAM lParam)
{
    struct ThreadWindows *threadWindows = (struct ThreadWindows *)lParam;

    if (!EnableWindow(hwnd, FALSE))
    {
        if(threadWindows->numHandles >= threadWindows->numAllocs)
        {
            threadWindows->handles = HeapReAlloc(GetProcessHeap(), 0, threadWindows->handles,
                                                 (threadWindows->numAllocs*2)*sizeof(HWND));
            threadWindows->numAllocs *= 2;
        }
        threadWindows->handles[threadWindows->numHandles++]=hwnd;
    }
    return TRUE;
}

/**************************************************************************
 *              MessageBox   (USER.1)
 */
INT16 WINAPI MessageBox16( HWND16 hwnd, LPCSTR text, LPCSTR title, UINT16 type )
{
    DWORD count;
    struct ThreadWindows threadWindows;
    ReleaseThunkLock(&count);
#ifdef ATTACH_THREAD_INPUT
    //Force to set window to foreground.
    DWORD foregroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    AttachThreadInput(GetCurrentThreadId(), foregroundID, TRUE);
#endif
    if ((type & MB_TASKMODAL) && (hwnd == NULL))
    {
        threadWindows.numHandles = 0;
        threadWindows.numAllocs = 10;
        threadWindows.handles = HeapAlloc(GetProcessHeap(), 0, 10*sizeof(HWND));
        EnumThreadWindows(GetCurrentThreadId(), MSGBOX_EnumProc, (LPARAM)&threadWindows);
    }
    int ret = MessageBoxA( WIN_Handle32(hwnd), text, title, type & ~MB_TASKMODAL );
    if ((type & MB_TASKMODAL) && (hwnd == NULL))
    {
        for (int i = 0; i < threadWindows.numHandles; i++)
            EnableWindow(threadWindows.handles[i], TRUE);
        HeapFree(GetProcessHeap(), 0, threadWindows.handles);
    }
#ifdef ATTACH_THREAD_INPUT
    AttachThreadInput(GetCurrentThreadId(), foregroundID, FALSE);
#endif
    RestoreThunkLock(count);
    return ret;
}

#include <pshpack1.h>
typedef struct
{
    BYTE pop_eax; /* 58 */
    BYTE push_imm; /* 68 */
    LPVOID this_;
    BYTE push_eax; /* 50 */
    BYTE jmp; /* 0xEA */
    LPVOID func;
    WORD cs;
    BOOL used;
    LPVOID param;
} TIMERTHUNK;
#include <poppack.h>
static TIMERTHUNK *timer_thunk_array;
static int MAX_TIMER_THUNK;
static void init_timer_thunk()
{
    if (timer_thunk_array)
        return;
    MAX_TIMER_THUNK = 4096 / sizeof(TIMERTHUNK);
    timer_thunk_array = VirtualAlloc(NULL, MAX_TIMER_THUNK * sizeof(TIMERTHUNK), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}
static TIMERTHUNK *init_timer_thunk_data(LPVOID param, int i, LPVOID func)
{
    timer_thunk_array[i].pop_eax = 0x58;
    timer_thunk_array[i].push_imm = 0x68;
    timer_thunk_array[i].this_ = timer_thunk_array + i;
    timer_thunk_array[i].push_eax = 0x50;
    timer_thunk_array[i].jmp = 0xEA;
    timer_thunk_array[i].func = func;
    timer_thunk_array[i].cs = wine_get_cs();
    timer_thunk_array[i].used = TRUE;
    timer_thunk_array[i].param = param;
    return timer_thunk_array + i;
}
WNDPROC WINPROC_AllocNativeProc(WNDPROC16 func);
static TIMERPROC allocate_timer_thunk(LPVOID param, LPVOID func, BOOL re)
{
    init_timer_thunk();
    if (re)
    {
        for (int i = 0; i < MAX_TIMER_THUNK; i++)
        {
            if (timer_thunk_array[i].used && timer_thunk_array[i].param == param)
            {
                return (TIMERPROC)(timer_thunk_array + i);
            }
        }
    }
    for (int i = 0; i < MAX_TIMER_THUNK; i++)
    {
        if (!timer_thunk_array[i].used)
        {
            return (TIMERPROC)init_timer_thunk_data(param, i, func);
        }
    }
    ERR("could not allocate timer thunk!\n");
    return WINPROC_AllocNativeProc(func);
}
VOID CALLBACK TimerProc_Thunk(TIMERTHUNK *data, HWND hWnd, UINT msg, UINT_PTR wp, DWORD lp)
{
    CallWindowProc16(data->param, HWND_16(hWnd), msg, wp, lp);
    return;
}
/***********************************************************************
 *		SetTimer (USER.10)
 */
UINT16 WINAPI SetTimer16( HWND16 hwnd, UINT16 id, UINT16 timeout, TIMERPROC16 proc )
{
    TIMERPROC proc32;
    UINT ret;
    if (proc == NULL)
    {
        return SetTimer(WIN_Handle32(hwnd), id, timeout, NULL);
    }
    proc32 = allocate_timer_thunk(proc, TimerProc_Thunk, TRUE);
    ret = SetTimer( WIN_Handle32(hwnd), id, timeout, proc32 );
    return ret;
}


/***********************************************************************
 *		SetSystemTimer (USER.11)
 */
UINT16 WINAPI SetSystemTimer16( HWND16 hwnd, UINT16 id, UINT16 timeout, TIMERPROC16 proc )
{
    TIMERPROC proc32 = proc == NULL ? NULL : allocate_timer_thunk(proc, TimerProc_Thunk, TRUE);
    if (!SetSystemTimer)
    {
        ERR("SetSystemTimer NULL\n");
        return 0;
    }
    return SetSystemTimer( WIN_Handle32(hwnd), id, timeout, proc32 );
}


/**************************************************************************
 *              KillTimer   (USER.12)
 */
BOOL16 WINAPI KillTimer16( HWND16 hwnd, UINT16 id )
{
    return KillTimer( WIN_Handle32(hwnd), id );
}


/**************************************************************************
 *              SetCapture   (USER.18)
 */
HWND16 WINAPI SetCapture16( HWND16 hwnd )
{
    return HWND_16( SetCapture( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              ReleaseCapture   (USER.19)
 */
BOOL16 WINAPI ReleaseCapture16(void)
{
    BOOL result = ReleaseCapture();
    static BOOL checked_is_windows_10;
    static BOOL is_windows_10_or_later;
    if (!checked_is_windows_10)
    {
        checked_is_windows_10 = TRUE;
        is_windows_10_or_later = GetProcAddress(GetModuleHandleA("KERNEL32"), "InitializeEnclave") != NULL;
    }
    if (is_windows_10_or_later)
    {
        /* Windows 10 doesn't send WM_MOUSEMOVE */
#if 0
        POINT point;
        if (GetCursorPos(&point))
        {
            HWND hwnd = GetCapture();
            if (!hwnd)
                hwnd = WindowFromPoint(point);
            if (hwnd)
                PostMessageA(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(point.x, point.y));
        }
#endif
        INPUT input;
        input.type = INPUT_MOUSE;
        input.mi.mouseData = 0;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
        input.mi.time = 0;
        input.mi.dwExtraInfo = 0;
        input.mi.dx = 1;
        input.mi.dy = 0;
        SendInput(1, &input, sizeof(input));
        input.mi.dx = -1;
        input.mi.dy = 0;
        SendInput(1, &input, sizeof(input));
    }
    return result;
}


/**************************************************************************
 *              SetFocus   (USER.22)
 */
HWND16 WINAPI SetFocus16( HWND16 hwnd )
{
    DWORD count;
    HWND16 result;
    ReleaseThunkLock(&count);
    result = HWND_16(SetFocus(WIN_Handle32(hwnd)));
    RestoreThunkLock(count);
    return result;
}


/**************************************************************************
 *              GetFocus   (USER.23)
 */
HWND16 WINAPI GetFocus16(void)
{
    return HWND_16( GetFocus() );
}


/**************************************************************************
 *              RemoveProp   (USER.24)
 */
HANDLE16 WINAPI RemoveProp16( HWND16 hwnd, LPCSTR str )
{
    return HANDLE_16(RemovePropA( WIN_Handle32(hwnd), str ));
}


/**************************************************************************
 *              GetProp   (USER.25)
 */
HANDLE16 WINAPI GetProp16( HWND16 hwnd, LPCSTR str )
{
    return HANDLE_16(GetPropA( WIN_Handle32(hwnd), str ));
}


/**************************************************************************
 *              SetProp   (USER.26)
 */
BOOL16 WINAPI SetProp16( HWND16 hwnd, LPCSTR str, HANDLE16 handle )
{
    return SetPropA( WIN_Handle32(hwnd), str, HANDLE_32(handle) );
}


/***********************************************************************
 *              EnumProps   (USER.27)
 */
INT16 WINAPI EnumProps16( HWND16 hwnd, PROPENUMPROC16 func )
{
    ERR("(%04x,%08x)\n", hwnd, func);
	return 0;
#if 0
    int ret = -1, i, count, total = 32;
    property_data_t *list;

    while (total)
    {
        if (!(list = HeapAlloc( GetProcessHeap(), 0, total * sizeof(*list) ))) break;
        count = 0;
        SERVER_START_REQ( get_window_properties )
        {
            req->window = wine_server_user_handle( HWND_32(hwnd) );
            wine_server_set_reply( req, list, total * sizeof(*list) );
            if (!wine_server_call( req )) count = reply->total;
        }
        SERVER_END_REQ;

        if (count && count <= total)
        {
            char string[ATOM_BUFFER_SIZE];
            SEGPTR segptr = MapLS( string );
            WORD args[4];
            DWORD result;

            for (i = 0; i < count; i++)
            {
                if (list[i].string)  /* it was a string originally */
                {
                    if (!GlobalGetAtomNameA( list[i].atom, string, ATOM_BUFFER_SIZE )) continue;
                    args[3] = hwnd;
                    args[2] = SELECTOROF(segptr);
                    args[1] = OFFSETOF(segptr);
                    args[0] = LOWORD(list[i].data);
                }
                else
                {
                    args[3] = hwnd;
                    args[2] = 0;
                    args[1] = list[i].atom;
                    args[0] = LOWORD(list[i].data);
                }
                WOWCallback16Ex( (DWORD)func, WCB16_PASCAL, sizeof(args), args, &result );
                if (!(ret = LOWORD(result))) break;
            }
            UnMapLS( segptr );
            HeapFree( GetProcessHeap(), 0, list );
            break;
        }
        HeapFree( GetProcessHeap(), 0, list );
        total = count;  /* restart with larger buffer */
    }
    return ret;
#endif
}


/**************************************************************************
 *              ClientToScreen   (USER.28)
 */
void WINAPI ClientToScreen16( HWND16 hwnd, LPPOINT16 lppnt )
{
    MapWindowPoints16( hwnd, 0, lppnt, 1 );
}


/**************************************************************************
 *              ScreenToClient   (USER.29)
 */
void WINAPI ScreenToClient16( HWND16 hwnd, LPPOINT16 lppnt )
{
    MapWindowPoints16( 0, hwnd, lppnt, 1 );
}


/**************************************************************************
 *              WindowFromPoint   (USER.30)
 */
HWND16 WINAPI WindowFromPoint16( POINT16 pt )
{
    POINT pt32;

    pt32.x = pt.x;
    pt32.y = pt.y;
    return HWND_16( WindowFromPoint( pt32 ) );
}


/**************************************************************************
 *              IsIconic   (USER.31)
 */
BOOL16 WINAPI IsIconic16(HWND16 hwnd)
{
    return IsIconic( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetWindowRect   (USER.32)
 */
void WINAPI GetWindowRect16( HWND16 hwnd, LPRECT16 rect )
{
    RECT rect32;

    if (GetWindowRect( WIN_Handle32(hwnd), &rect32 ))
    {
        rect->left   = rect32.left;
        rect->top    = rect32.top;
        rect->right  = rect32.right;
        rect->bottom = rect32.bottom;
    }
}


/**************************************************************************
 *              GetClientRect   (USER.33)
 */
void WINAPI GetClientRect16( HWND16 hwnd, LPRECT16 rect )
{
    RECT rect32;

    if (GetClientRect( WIN_Handle32(hwnd), &rect32 ))
    {
        rect->left   = rect32.left;
        rect->top    = rect32.top;
        rect->right  = rect32.right;
        rect->bottom = rect32.bottom;
    }
}


/**************************************************************************
 *              EnableWindow   (USER.34)
 */
BOOL16 WINAPI EnableWindow16( HWND16 hwnd, BOOL16 enable )
{
    DWORD count;
    ReleaseThunkLock(&count);
    BOOL16 result = EnableWindow( WIN_Handle32(hwnd), enable );
    RestoreThunkLock(count);
    return result;
}


/**************************************************************************
 *              IsWindowEnabled   (USER.35)
 */
BOOL16 WINAPI IsWindowEnabled16(HWND16 hwnd)
{
    return IsWindowEnabled( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetWindowText   (USER.36)
 */
INT16 WINAPI GetWindowText16( HWND16 hwnd, SEGPTR lpString, INT16 nMaxCount )
{
    return SendMessage16( hwnd, WM_GETTEXT, nMaxCount, lpString );
}


/**************************************************************************
 *              SetWindowText   (USER.37)
 */
BOOL16 WINAPI SetWindowText16( HWND16 hwnd, SEGPTR lpString )
{
    return SendMessage16( hwnd, WM_SETTEXT, 0, lpString );
}


/**************************************************************************
 *              GetWindowTextLength   (USER.38)
 */
INT16 WINAPI GetWindowTextLength16( HWND16 hwnd )
{
    return SendMessage16( hwnd, WM_GETTEXTLENGTH, 0, 0 );
}


/***********************************************************************
 *		BeginPaint (USER.39)
 */
HDC16 WINAPI BeginPaint16( HWND16 hwnd, LPPAINTSTRUCT16 lps )
{
	PAINTSTRUCT ps = { 0 };

    BeginPaint( WIN_Handle32(hwnd), &ps );
    lps->hdc            = HDC_16(ps.hdc);
    lps->fErase         = ps.fErase;
    lps->rcPaint.top    = ps.rcPaint.top;
    lps->rcPaint.left   = ps.rcPaint.left;
    lps->rcPaint.right  = ps.rcPaint.right;
    lps->rcPaint.bottom = ps.rcPaint.bottom;
    lps->fRestore       = ps.fRestore;
    lps->fIncUpdate     = ps.fIncUpdate;
    return lps->hdc;
}


/***********************************************************************
 *		EndPaint (USER.40)
 */
BOOL16 WINAPI EndPaint16( HWND16 hwnd, const PAINTSTRUCT16* lps )
{
    PAINTSTRUCT ps = { 0 };

    ps.hdc = HDC_32(lps->hdc);
	ps.fErase = lps->fErase;
	ps.rcPaint.top = lps->rcPaint.top;
	ps.rcPaint.left = lps->rcPaint.left;
	ps.rcPaint.right = lps->rcPaint.right;
	ps.rcPaint.bottom = lps->rcPaint.bottom;
	ps.fRestore = lps->fRestore;
	ps.fIncUpdate = lps->fIncUpdate;
    BOOL result = EndPaint( WIN_Handle32(hwnd), &ps );
    if (result)
    {
        K32WOWHandle16DestroyHint(ps.hdc, WOW_TYPE_HDC);
    }
    return result;
}


/***********************************************************************
 *		CreateWindow (USER.41)
 */
HWND16 WINAPI CreateWindow16( LPCSTR className, LPCSTR windowName,
                              DWORD style, INT16 x, INT16 y, INT16 width,
                              INT16 height, HWND16 parent, HMENU16 menu,
                              HINSTANCE16 instance, LPVOID data )
{
    return CreateWindowEx16( 0, className, windowName, style,
                             x, y, width, height, parent, menu, instance, data );
}


/**************************************************************************
 *              ShowWindow   (USER.42)
 */
BOOL16 WINAPI ShowWindow16( HWND16 hwnd, INT16 cmd )
{
    DWORD count;

    ReleaseThunkLock(&count);
#ifdef ATTACH_THREAD_INPUT
    DWORD foregroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    //Attach
    AttachThreadInput(GetCurrentThreadId(), foregroundID, TRUE);
#endif
    BOOL ret = ShowWindow( WIN_Handle32(hwnd), cmd );
#ifdef ATTACH_THREAD_INPUT
    //Dettach
    AttachThreadInput(GetCurrentThreadId(), foregroundID, FALSE);
#endif
    RestoreThunkLock(count);
    return ret;
}


/**************************************************************************
 *              CloseWindow   (USER.43)
 */
BOOL16 WINAPI CloseWindow16( HWND16 hwnd )
{
    DWORD count;
    ReleaseThunkLock(&count);
    BOOL16 result = CloseWindow( WIN_Handle32(hwnd) );
    RestoreThunkLock(count);
    return result;
}


/**************************************************************************
 *              OpenIcon   (USER.44)
 */
BOOL16 WINAPI OpenIcon16( HWND16 hwnd )
{
    return OpenIcon( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              BringWindowToTop   (USER.45)
 */
BOOL16 WINAPI BringWindowToTop16( HWND16 hwnd )
{
    DWORD count;
    BOOL16 result;
    ReleaseThunkLock(&count);
#ifdef ATTACH_THREAD_INPUT
    //Force to set window to foreground.
    DWORD foregroundID = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    AttachThreadInput(GetCurrentThreadId(), foregroundID, TRUE);
#endif
    result = (BOOL16)BringWindowToTop( WIN_Handle32(hwnd) );
#ifdef ATTACH_THREAD_INPUT
    AttachThreadInput(GetCurrentThreadId(), foregroundID, FALSE);
#endif
    RestoreThunkLock(count);
    return result;
}


/**************************************************************************
 *              GetParent   (USER.46)
 */
HWND16 WINAPI GetParent16( HWND16 hwnd )
{
    return HWND_16( GetParent( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              IsWindow   (USER.47)
 */
BOOL16 WINAPI IsWindow16( HWND16 hwnd )
{
    STACK16FRAME *frame = MapSL( (SEGPTR)getWOW32Reserved() );
    frame->es = USER_HeapSel;
    HWND hwnd32 = HWND_32(hwnd);
    if (!HIWORD(hwnd32))
    {
        /* invalid handle */
        return FALSE;
    }
    /* don't use WIN_Handle32 here, we don't care about the full handle */
    return IsWindow( hwnd32 );
}


/**************************************************************************
 *              IsChild   (USER.48)
 */
BOOL16 WINAPI IsChild16( HWND16 parent, HWND16 child )
{
    return IsChild( WIN_Handle32(parent), WIN_Handle32(child) );
}


/**************************************************************************
 *              IsWindowVisible   (USER.49)
 */
BOOL16 WINAPI IsWindowVisible16( HWND16 hwnd )
{
    return IsWindowVisible( WIN_Handle32(hwnd) );
}


HWND16 WINAPI FindWindowEx16(HWND16 parent, HWND16 child, LPCSTR className, LPCSTR title);
/**************************************************************************
 *              FindWindow   (USER.50)
 */
HWND16 WINAPI FindWindow16( LPCSTR className, LPCSTR title )
{
    return FindWindowEx16( (HWND16)NULL, (HWND16)NULL, className, title );
}


/**************************************************************************
 *              DestroyWindow   (USER.53)
 */
BOOL16 WINAPI DestroyWindow16( HWND16 hwnd )
{
    DWORD count;
    BOOL result;
    ReleaseThunkLock(&count);
    result = DestroyWindow(WIN_Handle32(hwnd));
    RestoreThunkLock(count);
}


/*******************************************************************
 *           EnumWindows   (USER.54)
 */
BOOL16 WINAPI EnumWindows16( WNDENUMPROC16 func, LPARAM lParam )
{
    struct wnd_enum_info info;

    info.proc  = func;
    info.param = lParam;
    return EnumWindows( wnd_enum_callback, (LPARAM)&info );
}


/**********************************************************************
 *           EnumChildWindows   (USER.55)
 */
BOOL16 WINAPI EnumChildWindows16( HWND16 parent, WNDENUMPROC16 func, LPARAM lParam )
{
    struct wnd_enum_info info;

    info.proc  = func;
    info.param = lParam;
    return EnumChildWindows( WIN_Handle32(parent), wnd_enum_callback, (LPARAM)&info );
}


/**************************************************************************
 *              MoveWindow   (USER.56)
 */
BOOL16 WINAPI MoveWindow16( HWND16 hwnd, INT16 x, INT16 y, INT16 cx, INT16 cy, BOOL16 repaint )
{
    return SetWindowPos16(hwnd, 0, x, y, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE | (repaint ? 0 : SWP_NOREDRAW));
}


/***********************************************************************
 *		RegisterClass (USER.57)
 */
ATOM WINAPI RegisterClass16( const WNDCLASS16 *wc )
{
    WNDCLASSEX16 wcex;

    wcex.cbSize        = sizeof(wcex);
    wcex.style         = wc->style;
    wcex.lpfnWndProc   = wc->lpfnWndProc;
    wcex.cbClsExtra    = wc->cbClsExtra;
    wcex.cbWndExtra    = wc->cbWndExtra;
    wcex.hInstance     = wc->hInstance;
    wcex.hIcon         = wc->hIcon;
    wcex.hCursor       = wc->hCursor;
    wcex.hbrBackground = wc->hbrBackground;
    wcex.lpszMenuName  = wc->lpszMenuName;
    wcex.lpszClassName = wc->lpszClassName;
    wcex.hIconSm       = 0;
    return RegisterClassEx16( &wcex );
}


/**************************************************************************
 *              GetClassName   (USER.58)
 */
INT16 WINAPI GetClassName16( HWND16 hwnd, LPSTR buffer, INT16 count )
{
    char sclassName[200];
    SIZE_T len = sizeof(sclassName) / sizeof(sclassName[0]);
    LPSTR className = sclassName;
    SIZE_T clen;
    if (count == 0)
        return 0;
    if (count == 1)
    {
        buffer[0] = '\0';
        return 0;
    }
    while (TRUE)
    {
        clen = GetClassNameA(WIN_Handle32(hwnd), className, len);
        //className is truncated
        if (len - 1 == clen)
        {
            LPVOID heap;
            if (className != sclassName)
            {
                heap = HeapReAlloc(GetProcessHeap(), 0, className, len * 2);
            }
            else
            {
                heap = HeapAlloc(GetProcessHeap(), 0, len * 2);
            }
            if (!heap)
                break;
            className = (LPSTR)heap;
            len *= 2;
            continue;
        }
        break;
    }
    if (clen > 32767)
    {
        clen = 32767;
    }
    if (clen != 0)
    {
        struct class_entry *entry = find_win32_class(className);
        if (!entry)
        {
            clen = min(clen, (SIZE_T)(count - 1));
            memcpy(buffer, className, clen);
            buffer[clen] = 0;
        }
        else
        {
            clen = min(strlen(entry->classInfo.lpszClassName), (SIZE_T)(count - 1));
            memcpy(buffer, entry->classInfo.lpszClassName, clen);
            buffer[clen] = 0;
        }
    }
    if (className != sclassName)
    {
        HeapFree(GetProcessHeap(), 0, className);
    }
    return (INT16)clen;
}


/**************************************************************************
 *              SetActiveWindow   (USER.59)
 */
HWND16 WINAPI SetActiveWindow16( HWND16 hwnd )
{
    return HWND_16( SetActiveWindow( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              GetActiveWindow   (USER.60)
 */
HWND16 WINAPI GetActiveWindow16(void)
{
    return HWND_16( GetActiveWindow() );
}


/**************************************************************************
 *              ScrollWindow   (USER.61)
 */
void WINAPI ScrollWindow16( HWND16 hwnd, INT16 dx, INT16 dy, const RECT16 *rect,
                            const RECT16 *clipRect )
{
    RECT rect32, clipRect32;

    if (rect)
    {
        rect32.left   = rect->left;
        rect32.top    = rect->top;
        rect32.right  = rect->right;
        rect32.bottom = rect->bottom;
    }
    if (clipRect)
    {
        clipRect32.left   = clipRect->left;
        clipRect32.top    = clipRect->top;
        clipRect32.right  = clipRect->right;
        clipRect32.bottom = clipRect->bottom;
    }
    ScrollWindow( WIN_Handle32(hwnd), dx, dy, rect ? &rect32 : NULL,
                  clipRect ? &clipRect32 : NULL );
}


/**************************************************************************
 *              SetScrollPos   (USER.62)
 */
INT16 WINAPI SetScrollPos16( HWND16 hwnd, INT16 nBar, INT16 nPos, BOOL16 redraw )
{
    return SetScrollPos( WIN_Handle32(hwnd), nBar, nPos, redraw );
}


/**************************************************************************
 *              GetScrollPos   (USER.63)
 */
INT16 WINAPI GetScrollPos16( HWND16 hwnd, INT16 nBar )
{
    return GetScrollPos( WIN_Handle32(hwnd), nBar );
}


/**************************************************************************
 *              SetScrollRange   (USER.64)
 */
void WINAPI SetScrollRange16( HWND16 hwnd, INT16 nBar, INT16 MinVal, INT16 MaxVal, BOOL16 redraw )
{
    /* Invalid range -> range is set to (0,0) */
    if ((INT)MaxVal - (INT)MinVal > 0x7fff) MinVal = MaxVal = 0;
    SetScrollRange( WIN_Handle32(hwnd), nBar, MinVal, MaxVal, redraw );
}


/**************************************************************************
 *              GetScrollRange   (USER.65)
 */
BOOL16 WINAPI GetScrollRange16( HWND16 hwnd, INT16 nBar, LPINT16 lpMin, LPINT16 lpMax)
{
    INT min, max;
    BOOL ret = GetScrollRange( WIN_Handle32(hwnd), nBar, &min, &max );
    if (lpMin) *lpMin = min;
    if (lpMax) *lpMax = max;
    return ret;
}


/* layered-window based desktop DC emulation for DWM */
LRESULT WINAPI DesktopDCEmulationWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_NCCALCSIZE:
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}
static HWND dcemu_hwnd;
static COLORREF DWMDesktopDCFixBackgroundColor = RGB(254, 254, 254);
void init_dcemu()
{
    WNDCLASSW wc = { 0 };
    RECT wkara = { 0 };
    HWND hwnd;
    COLORREF colorkey = DWMDesktopDCFixBackgroundColor;
    if (dcemu_hwnd)
        return;
    wc.lpfnWndProc = DesktopDCEmulationWindowProc;
    wc.style = CS_OWNDC;
    wc.hbrBackground = CreateSolidBrush(colorkey);
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.lpszClassName = L"DesktopDCEmulation";
    RegisterClassW(&wc);
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &wkara, 0);
    hwnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, L"DesktopDCEmulation", L"", 0, wkara.left, wkara.top, wkara.right - wkara.left, wkara.bottom - wkara.top, NULL, NULL, NULL, NULL);
    if (!hwnd)
        return;
    SetWindowLongW(hwnd, GWL_STYLE, GetWindowLongW(hwnd, GWL_STYLE) & ~WS_BORDER & ~WS_SIZEBOX &~WS_DLGFRAME);
    SetLayeredWindowAttributes(hwnd, colorkey, 0, LWA_COLORKEY);
    ShowWindow(hwnd, SW_NORMAL);
    UpdateWindow(hwnd);
    dcemu_hwnd = hwnd;
}
/**************************************************************************
 *              GetDC   (USER.66)
 */
HDC16 WINAPI GetDC16( HWND16 hwnd )
{
    static BOOL DWMDesktopDCFix_init;
    static BOOL DWMDesktopDCFix;
    if (!DWMDesktopDCFix_init)
    {
        DWMDesktopDCFix_init = TRUE;
        DWMDesktopDCFix = krnl386_get_config_int("otvdm", "DWMDesktopDCFix", FALSE);
        int R = krnl386_get_config_int("otvdm", "DWMDesktopDCFixBackgroundColorR", 254);
        int G = krnl386_get_config_int("otvdm", "DWMDesktopDCFixBackgroundColorG", 254);
        int B = krnl386_get_config_int("otvdm", "DWMDesktopDCFixBackgroundColorB", 254);
        DWMDesktopDCFixBackgroundColor = RGB(R, G, B);
    }
    if (DWMDesktopDCFix && hwnd == 0)
    {
        init_dcemu();
        SetWindowPos(dcemu_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREDRAW);
        HDC dc = GetDC(dcemu_hwnd);
        SetWindowOrgEx(dc, 0, 0, NULL);
        MoveToEx(dc, 0, 0, NULL);
        SelectClipRgn(dc, NULL);
        return HDC_16(dc);
    }
    return HDC_16(GetDC( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              GetWindowDC   (USER.67)
 */
HDC16 WINAPI GetWindowDC16( HWND16 hwnd )
{
    return GetDCEx16( hwnd, 0, DCX_USESTYLE | DCX_WINDOW );
}


/**************************************************************************
 *              ReleaseDC   (USER.68)
 */
INT16 WINAPI ReleaseDC16( HWND16 hwnd, HDC16 hdc )
{
    INT16 result = (INT16)ReleaseDC( WIN_Handle32(hwnd), HDC_32(hdc) );
    if (result)
    {
        K32WOWHandle16DestroyHint(HDC_32(hdc), WOW_TYPE_HDC);
    }
    return result;
}


/**************************************************************************
 *              FlashWindow   (USER.105)
 */
BOOL16 WINAPI FlashWindow16( HWND16 hwnd, BOOL16 bInvert )
{
    return FlashWindow( WIN_Handle32(hwnd), bInvert );
}


/**************************************************************************
 *              WindowFromDC   (USER.117)
 */
HWND16 WINAPI WindowFromDC16( HDC16 hDC )
{
    return HWND_16( WindowFromDC( HDC_32(hDC) ) );
}


/**************************************************************************
 *              UpdateWindow   (USER.124)
 */
void WINAPI UpdateWindow16( HWND16 hwnd )
{
    RedrawWindow16( hwnd, NULL, 0, RDW_UPDATENOW | RDW_ALLCHILDREN );
}


/**************************************************************************
 *              InvalidateRect   (USER.125)
 */
void WINAPI InvalidateRect16( HWND16 hwnd, const RECT16 *rect, BOOL16 erase )
{
    RedrawWindow16( hwnd, rect, 0, RDW_INVALIDATE | (erase ? RDW_ERASE : 0) );
}


/**************************************************************************
 *              InvalidateRgn   (USER.126)
 */
void WINAPI InvalidateRgn16( HWND16 hwnd, HRGN16 hrgn, BOOL16 erase )
{
    RedrawWindow16( hwnd, NULL, hrgn, RDW_INVALIDATE | (erase ? RDW_ERASE : 0) );
}


/**************************************************************************
 *              ValidateRect   (USER.127)
 */
void WINAPI ValidateRect16( HWND16 hwnd, const RECT16 *rect )
{
    RedrawWindow16( hwnd, rect, 0, RDW_VALIDATE | RDW_NOCHILDREN );
}


/**************************************************************************
 *              ValidateRgn   (USER.128)
 */
void WINAPI ValidateRgn16( HWND16 hwnd, HRGN16 hrgn )
{
    RedrawWindow16( hwnd, NULL, hrgn, RDW_VALIDATE | RDW_NOCHILDREN );
}


static WORD get_system_window_class_wndextra(const char *cls, BOOL *f)
{
    char buf[100];
    *f = TRUE;
    if (IS_INTRESOURCE(cls))
    {
        GlobalGetAtomNameA((ATOM)LOWORD(cls), buf, 100);
        cls = buf;
    }
    if (!strncasecmp(cls, "EDIT", sizeof(cls)))
        return 6;
    if (!strncasecmp(cls, "LISTBOX", sizeof(cls)))
        return 2;
    if (!strncasecmp(cls, "SCROLLBAR", sizeof(cls)))
        return 12;
    if (!strncasecmp(cls, "COMBOBOX", sizeof(cls)))
        return 2;
    if (!strncasecmp(cls, "STATIC", sizeof(cls)))
        return 6;
    if (!strncasecmp(cls, "BUTTON", sizeof(cls)))
        return 4;
    *f = FALSE;
    return 0;
}

/**************************************************************************
 *              GetClassWord   (USER.129)
 */
WORD WINAPI GetClassWord16( HWND16 hwnd, INT16 offset )
{
    HICON icon;

    switch (offset)
    {
    case GCL_CBWNDEXTRA:
    {
        char cls[1000];
        GetClassNameA(HWND_32(hwnd), cls, sizeof(cls));
        BOOL f;
        WORD cb = get_system_window_class_wndextra(cls, &f);
        if (f)
            return cb;
        ATOM atom = GetClassWord(HWND_32(hwnd), GCW_ATOM);
        if (WNDCLASS16Info[atom].allocated)
        {
            return WNDCLASS16Info[atom].cbWndExtra;
        }
        else
        {
            return (WORD)GetClassLongA(WIN_Handle32(hwnd), offset);
        }
    }
    case GCLP_HCURSOR:
    case GCLP_HICON:
    case GCLP_HICONSM:
        icon = (HICON)GetClassLongPtrW( WIN_Handle32(hwnd), offset );
        return get_icon_16( icon );
    case GCLP_HBRBACKGROUND:
        return HBRUSH_16((HBRUSH)GetClassLongPtrW(WIN_Handle32(hwnd), offset));
    }
    return GetClassWord( WIN_Handle32(hwnd), offset );
}


/**************************************************************************
 *              SetClassWord   (USER.130)
 */
WORD WINAPI SetClassWord16( HWND16 hwnd, INT16 offset, WORD newval )
{
    HICON icon;

    switch (offset)
    {
    case GCLP_HCURSOR:
    case GCLP_HICON:
    case GCLP_HICONSM:
        icon = (HICON)SetClassLongPtrW( WIN_Handle32(hwnd), offset, (ULONG_PTR)get_icon_32(newval) );
        return get_icon_16( icon );
    case GCLP_HBRBACKGROUND:
        return HBRUSH_16((HBRUSH)SetClassLongPtrW(WIN_Handle32(hwnd), offset, HBRUSH_32(newval)));
    }
    return SetClassWord( WIN_Handle32(hwnd), offset, newval );
}


LRESULT CALLBACK UserAdapterWindowClass(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    ERR("(%p, %d, %d, %d)\n", msg, wparam, lparam);
    return FALSE;
}
/***********************************************************************
 *		GetClassLong (USER.131)
 */
LONG WINAPI GetClassLong16( HWND16 hwnd16, INT16 offset )
{
    LONG_PTR ret = GetClassLongA( WIN_Handle32(hwnd16), offset );

    switch( offset )
    {
    case GCLP_WNDPROC:
    {
        ATOM atom = GetClassWord(WIN_Handle32(hwnd16), GCW_ATOM);
        WNDPROC proc = GetClassLongPtrA(WIN_Handle32(hwnd16), GCLP_WNDPROC);
        return WNDCLASS16Info[atom].wndproc ? WNDCLASS16Info[atom].wndproc : WINPROC_AllocNativeProc_2(proc);//(LONG_PTR)WINPROC_GetProc16((WNDPROC)ret, FALSE);
    }
    case GCLP_MENUNAME:
        return MapLS( (void *)ret );  /* leak */
    case GCLP_HCURSOR:
    case GCLP_HICON:
    case GCLP_HICONSM:
        return get_icon_16( (HICON)ret );
    case GCLP_HBRBACKGROUND:
        return HBRUSH_16(ret);
    default:
        return ret;
    }
}


/***********************************************************************
 *		SetClassLong (USER.132)
 */
LONG WINAPI SetClassLong16( HWND16 hwnd16, INT16 offset, LONG newval )
{
    HICON icon;

    switch( offset )
    {
    case GCLP_HCURSOR:
    case GCLP_HICON:
    case GCLP_HICONSM:
        icon = (HICON)SetClassLongPtrW( WIN_Handle32(hwnd16), offset, (ULONG_PTR)get_icon_32(newval) );
        return get_icon_16( icon );
    case GCLP_WNDPROC:
        {
            WNDPROC new_proc = (WNDPROC16)newval;
            //WNDPROC old_proc = (WNDPROC)SetClassLongA( WIN_Handle32(hwnd16), offset, (LONG_PTR)new_proc );
            ATOM atom = GetClassWord(WIN_Handle32(hwnd16), GCW_ATOM);
            WNDPROC old = WNDCLASS16Info[atom].wndproc;
            WNDCLASS16Info[atom].wndproc = new_proc;
            return old;//(LONG)WINPROC_GetProc16(old_proc, FALSE);
        }
    case GCLP_HBRBACKGROUND:
        return HBRUSH_32(SetClassLongA(WIN_Handle32(hwnd16), offset, HBRUSH_32(newval)));
    case GCLP_MENUNAME:
        newval = (LONG)MapSL( newval );
        /* fall through */
    default:
        return SetClassLongA( WIN_Handle32(hwnd16), offset, newval );
    }
}

BYTE *hwndwordbuf[65536];
//__declspec(dllexport)
void SetWindowHInst16(WORD hWnd16, HINSTANCE16 hinst16);
//__declspec(dllexport) 
HINSTANCE16 GetWindowHInst16(WORD hWnd16);
//__declspec(dllexport)
void SetWindowHMenu16(WORD hWnd16, HMENU16 hinst16);
//__declspec(dllexport) 
HMENU16 GetWindowHMenu16(WORD hWnd16);
static WORD get_actual_cbwndextra(HWND16 hwnd16)
{
    SIZE_T siz = GetClassWord16(hwnd16, GCL_CBWNDEXTRA);
    if (siz >= 65536)
        return 65535;
    WORD w = (WORD)siz;
    if (w < DLGWINDOWEXTRA)
        return DLGWINDOWEXTRA;
    return w;
}
/**************************************************************************
 *              GetWindowWord   (USER.133)
 */
WORD WINAPI GetWindowWord16( HWND16 hwnd, INT16 offset )
{
    if (offset >= 0)
    {
        size_t siz = get_actual_cbwndextra(hwnd);
        if (siz + sizeof(WORD) < offset)
        {
            ERR("(0x%04X, %d) Out of range\n", hwnd, offset);
            return 0;
        }
        if (!hwndwordbuf[hwnd])
        {
            //TODO:leak
            hwndwordbuf[hwnd] = calloc(siz, sizeof(BYTE));
        }
        return *(WORD*)(hwndwordbuf[hwnd] + offset);
    }
    switch (offset)
    {
    case GWL_STYLE:
        return (WORD)GetWindowLong16(hwnd, offset);
    case GWL_HINSTANCE:
    {
        HINSTANCE16 h16 = GetWindowHInst16(hwnd);
        if (h16)
        {
            return h16;
        }
        HINSTANCE h = GetWindowLongPtrW(WIN_Handle32(hwnd), offset);
        /* 32-bit dll -> 16-bit hinst mapping? */
        /* LoadLibrary16 returns hinst GetModuleHandle16 returns hmodule */
        if (h && (h == GetModuleHandleW(L"COMDLG32.DLL") || h == GetModuleHandleW(L"COMMDLG.DLL16")))
        {
            if (GetModuleHandle16("COMMDLG"))
            {
                HINSTANCE16 commdlg = LoadLibrary16("COMMDLG");
                FreeLibrary16(commdlg);
                return commdlg;
            }
        }

        if (h == 0xFFFF0000)
        {
            HINSTANCE16 user = LoadLibrary16("USER");
            FreeLibrary16(user);
            return user;
        }
        if (HIWORD(h))
        {
            HTASK16 task = GetWindowTask16(hwnd);
            if (!task)
            {
                HINSTANCE16 user = LoadLibrary16("USER");
                FreeLibrary16(user);
                return user;
            }
            TDB *tdb = GlobalLock16(task);
            h16 = tdb->hInstance;
            GlobalUnlock16(task);
            return h16;
        }
        return LOWORD(h);
    }
    case GWL_ID:
    {
        LONG ret = GetWindowLongA(WIN_Handle32(hwnd), GWL_ID);
        return ret;
    }
    case GWL_HWNDPARENT:
        return HWND_16(GetWindowLongA(WIN_Handle32(hwnd), offset));
    default:
        break;
    }
    return GetWindowWord( WIN_Handle32(hwnd), offset );
}


/**************************************************************************
 *              SetWindowWord   (USER.134)
 */
WORD WINAPI SetWindowWord16( HWND16 hwnd, INT16 offset, WORD newval )
{
    if (offset >= 0)
    {
        size_t siz = get_actual_cbwndextra(hwnd);
        if (siz + sizeof(WORD) < offset)
        {
            ERR("(0x%04X, %d, 0x%04X) Out of range\n", hwnd, offset, newval);
            return 0;
        }
        if (!hwndwordbuf[hwnd])
        {
            //TODO:leak
            hwndwordbuf[hwnd] = calloc(siz, sizeof(BYTE));
        }
        WORD old = *(WORD*)(hwndwordbuf[hwnd] + offset);
        *(WORD*)(hwndwordbuf[hwnd] + offset) = newval;
        return old;
    }
    switch (offset)
    {
    case GWL_HINSTANCE:
    {
        HINSTANCE16 old = (HINSTANCE16)GetWindowWord16(hwnd, offset);
        SetWindowHInst16(hwnd, newval);
        (SetWindowLongA(WIN_Handle32(hwnd), offset, HINSTANCE_32(newval)));
        return old;
    }
    case GWL_ID:
    {
        LONG ret = SetWindowLongA(WIN_Handle32(hwnd), GWL_ID, newval);
        return ret;
    }
    case GWL_HWNDPARENT:
        return (SetWindowLongA(WIN_Handle32(hwnd), offset, HWND_32(newval)));
    default:
        break;
    }
    return SetWindowWord( WIN_Handle32(hwnd), offset, newval );
}


/**********************************************************************
 *		GetWindowLong (USER.135)
 */
LONG WINAPI GetWindowLong16( HWND16 hwnd16, INT16 offset )
{
    HWND hwnd = WIN_Handle32( hwnd16 );
    LONG_PTR retvalue;
    BOOL is_winproc = (offset == GWLP_WNDPROC);

    if (offset >= 0)
    {
        int cbWndExtra = get_actual_cbwndextra( hwnd16 );

        if (offset > (int)(cbWndExtra - sizeof(LONG)))
        {
            /*
             * Some programs try to access last element from 16 bit
             * code using illegal offset value. Hopefully this is
             * what those programs really expect.
             */
            if (cbWndExtra >= 4 && offset == cbWndExtra - sizeof(WORD))
            {
                offset = cbWndExtra - sizeof(LONG);
            }
            else
            {
                ERR("(0x%04X, %d) Out of range\n", hwnd, offset);
                SetLastError( ERROR_INVALID_INDEX );
                return 0;
            }
        }
        else if (offset == DWLP_DLGPROC)
        {
            is_winproc = is_dialog(hwnd);
        }
    }
    if (is_winproc)
    {
        if (offset == DWLP_DLGPROC)
        {
            retvalue = (LONG_PTR)GetDlgProc16(hwnd16);
        }
        else
        {
            retvalue = (LONG_PTR)GetWndProc16(hwnd16);
        }
        if (retvalue)
            return retvalue;
        retvalue = GetWindowLongA(hwnd, offset);
        retvalue = WINPROC_AllocNativeProc_2(retvalue);
        return retvalue;
    }//krnl386.exe
    //if (is_winproc) retvalue = (LONG_PTR)WINPROC_GetProc16( (WNDPROC)retvalue, FALSE );
    if (offset >= 0)
    {
        size_t siz = get_actual_cbwndextra(hwnd16);
        if (siz + sizeof(LONG) < offset)
        {
            ERR("(0x%04X, %d) Out of range\n", hwnd, offset);
            SetLastError(ERROR_INVALID_INDEX);
            return 0;
        }
        if (!hwndwordbuf[hwnd16])
        {
            //TODO:leak
            hwndwordbuf[hwnd16] = calloc(siz, sizeof(BYTE));
        }
        return *(LONG*)(hwndwordbuf[hwnd16] + offset);
    }
    retvalue = GetWindowLongA(hwnd, offset);
    return retvalue;
}


void InitWndProc16(HWND hWnd, HWND16 hWnd16);
LRESULT get_message_callback(HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
    LRESULT *result, void *arg);
LRESULT call_window_proc16(HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam,
    LRESULT *result, void *arg);
LRESULT CALLBACK DlgProcCall16(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc16(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
WNDPROC get_classinfo_wndproc(const char *class);
/**********************************************************************
 *		SetWindowLong (USER.136)
 */
LONG WINAPI SetWindowLong16( HWND16 hwnd16, INT16 offset, LONG newval )
{
    HWND hwnd = WIN_Handle32( hwnd16 );
    BOOL is_winproc = (offset == GWLP_WNDPROC);

    if (offset == DWLP_DLGPROC)
        is_winproc = is_dialog(hwnd);

	if (is_winproc)
    {
        FARPROC oldproc = GetWindowLongA(hwnd, offset);
        DWORD old = ((LONG_PTR)GetWindowLong16(hwnd16, offset));
        WNDPROC new_proc = (WNDPROC16)newval;
        if (offset == DWLP_DLGPROC)
        {
            SetDlgProc16(hwnd16, new_proc);
        }
        else
        {
            SetWndProc16(hwnd16, new_proc);//krnl386.exe
        }
        if (offset == DWLP_DLGPROC)
        {
            if (oldproc != DlgProc)
            {
                /* FIXME: set DlgProc */
                SetWindowLongA(hwnd, offset, DlgProc);
            }
        }
        else if (oldproc != WindowProc16)
        {
            SetWindowLongA(hwnd, offset, WindowProc16);
        }
		return old;
    }
    if (offset >= 0)
    {
        size_t siz = get_actual_cbwndextra(hwnd16);
        if (siz + sizeof(LONG) < offset)
        {
            ERR("(0x%04X, %d) Out of range\n", hwnd, offset);
            SetLastError(ERROR_INVALID_INDEX);
            return 0;
        }
        if (!hwndwordbuf[hwnd16])
        {
            //TODO:leak
            hwndwordbuf[hwnd16] = calloc(siz, sizeof(BYTE));
        }
        LONG old = *(LONG*)(hwndwordbuf[hwnd16] + offset);
        *(LONG*)(hwndwordbuf[hwnd16] + offset) = newval;
        return old;
    }
    /*else*/ return SetWindowLongA( hwnd, offset, newval );
}


/**************************************************************************
 *              OpenClipboard   (USER.137)
 */
BOOL16 WINAPI OpenClipboard16( HWND16 hwnd )
{
    return OpenClipboard( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetClipboardOwner   (USER.140)
 */
HWND16 WINAPI GetClipboardOwner16(void)
{
    return HWND_16( GetClipboardOwner() );
}


/**************************************************************************
 *              SetClipboardViewer   (USER.147)
 */
HWND16 WINAPI SetClipboardViewer16( HWND16 hwnd )
{
    return HWND_16( SetClipboardViewer( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              GetClipboardViewer   (USER.148)
 */
HWND16 WINAPI GetClipboardViewer16(void)
{
    return HWND_16( GetClipboardViewer() );
}


/**************************************************************************
 *              ChangeClipboardChain   (USER.149)
 */
BOOL16 WINAPI ChangeClipboardChain16(HWND16 hwnd, HWND16 hwndNext)
{
    return ChangeClipboardChain( WIN_Handle32(hwnd), WIN_Handle32(hwndNext) );
}


/**************************************************************************
 *              GetSystemMenu   (USER.156)
 */
HMENU16 WINAPI GetSystemMenu16( HWND16 hwnd, BOOL16 revert )
{
    return HMENU_16(GetSystemMenu( WIN_Handle32(hwnd), revert ));
}


/**************************************************************************
 *              GetMenu   (USER.157)
 */
HMENU16 WINAPI GetMenu16( HWND16 hwnd )
{
    return GetWindowHMenu16(( /*WIN_Handle32*/(hwnd) ));
}


/**************************************************************************
 *              SetMenu   (USER.158)
 */
BOOL16 WINAPI SetMenu16( HWND16 hwnd, HMENU16 hMenu )
{
    BOOL result = SetMenu(WIN_Handle32(hwnd), HMENU_32(hMenu));
    if (result)
    {
        SetWindowHMenu16(hwnd, hMenu);
    }
}


/**************************************************************************
 *              DrawMenuBar   (USER.160)
 */
void WINAPI DrawMenuBar16( HWND16 hwnd )
{
    DrawMenuBar( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              HiliteMenuItem   (USER.162)
 */
BOOL16 WINAPI HiliteMenuItem16( HWND16 hwnd, HMENU16 hMenu, UINT16 id, UINT16 wHilite )
{
    return HiliteMenuItem( WIN_Handle32(hwnd), HMENU_32(hMenu), id, wHilite );
}


/**************************************************************************
 *              CreateCaret   (USER.163)
 */
void WINAPI CreateCaret16( HWND16 hwnd, HBITMAP16 bitmap, INT16 width, INT16 height )
{
    CreateCaret( WIN_Handle32(hwnd), HBITMAP_32(bitmap), width, height );
}


/*****************************************************************
 *		DestroyCaret (USER.164)
 */
void WINAPI DestroyCaret16(void)
{
    DestroyCaret();
}


/*****************************************************************
 *		SetCaretPos (USER.165)
 */
void WINAPI SetCaretPos16( INT16 x, INT16 y )
{
    SetCaretPos( x, y );
}


/**************************************************************************
 *              HideCaret   (USER.166)
 */
void WINAPI HideCaret16( HWND16 hwnd )
{
    HideCaret( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              ShowCaret   (USER.167)
 */
void WINAPI ShowCaret16( HWND16 hwnd )
{
    ShowCaret( WIN_Handle32(hwnd) );
}


/*****************************************************************
 *		SetCaretBlinkTime (USER.168)
 */
void WINAPI SetCaretBlinkTime16( UINT16 msecs )
{
    SetCaretBlinkTime( msecs );
}


/*****************************************************************
 *		GetCaretBlinkTime (USER.169)
 */
UINT16 WINAPI GetCaretBlinkTime16(void)
{
    return GetCaretBlinkTime();
}


/**************************************************************************
 *              ArrangeIconicWindows   (USER.170)
 */
UINT16 WINAPI ArrangeIconicWindows16( HWND16 parent)
{
    return ArrangeIconicWindows( WIN_Handle32(parent) );
}


/**************************************************************************
 *              SwitchToThisWindow   (USER.172)
 */
void WINAPI SwitchToThisWindow16( HWND16 hwnd, BOOL16 restore )
{
    SwitchToThisWindow( WIN_Handle32(hwnd), restore );
}


/**************************************************************************
 *              KillSystemTimer   (USER.182)
 */
BOOL16 WINAPI KillSystemTimer16( HWND16 hwnd, UINT16 id )
{
    if (!KillSystemTimer)
    {
        ERR("KillSystemTimer NULL\n");
        return 0;
    }
    return KillSystemTimer( WIN_Handle32(hwnd), id );
}


/*****************************************************************
 *		GetCaretPos (USER.183)
 */
void WINAPI GetCaretPos16( LPPOINT16 pt16 )
{
    POINT pt;
    if (GetCaretPos( &pt ))
    {
        pt16->x = pt.x;
        pt16->y = pt.y;
    }
}


/**************************************************************************
 *              SetSysModalWindow   (USER.188)
 */
HWND16 WINAPI SetSysModalWindow16( HWND16 hwnd )
{
    HWND16 old = hwndSysModal;
    hwndSysModal = hwnd;
    return old;
}


/**************************************************************************
 *              GetSysModalWindow   (USER.189)
 */
HWND16 WINAPI GetSysModalWindow16(void)
{
    return hwndSysModal;
}


/**************************************************************************
 *              GetUpdateRect   (USER.190)
 */
BOOL16 WINAPI GetUpdateRect16( HWND16 hwnd, LPRECT16 rect, BOOL16 erase )
{
    RECT r;
    BOOL16 ret;
    HWND hwnd32 = WIN_Handle32(hwnd);

    if (!rect) return GetUpdateRect(hwnd32, NULL, erase );
    if (!IsWindow(hwnd32))
        return FALSE;
    ret = GetUpdateRect( hwnd32, &r, erase );
    rect->left   = r.left;
    rect->top    = r.top;
    rect->right  = r.right;
    rect->bottom = r.bottom;
    return ret;
}


/**************************************************************************
 *              ChildWindowFromPoint   (USER.191)
 */
HWND16 WINAPI ChildWindowFromPoint16( HWND16 hwndParent, POINT16 pt )
{
    POINT pt32;

    pt32.x = pt.x;
    pt32.y = pt.y;
    return HWND_16( ChildWindowFromPoint( WIN_Handle32(hwndParent), pt32 ));
}


/***********************************************************************
 *		CascadeChildWindows (USER.198)
 */
void WINAPI CascadeChildWindows16( HWND16 parent, WORD action )
{
    CascadeWindows( WIN_Handle32(parent), action, NULL, 0, NULL );
}


/***********************************************************************
 *		TileChildWindows (USER.199)
 */
void WINAPI TileChildWindows16( HWND16 parent, WORD action )
{
    TileWindows( WIN_Handle32(parent), action, NULL, 0, NULL );
}


/***********************************************************************
 *		GetWindowTask   (USER.224)
 */
HTASK16 WINAPI GetWindowTask16( HWND16 hwnd )
{
    DWORD tid = GetWindowThreadProcessId( HWND_32(hwnd), NULL );
    if (!tid) return 0;
    return HTASK_16(tid);
}

/**********************************************************************
 *		EnumTaskWindows   (USER.225)
 */
BOOL16 WINAPI EnumTaskWindows16( HTASK16 hTask, WNDENUMPROC16 func, LPARAM lParam )
{
    struct wnd_enum_info info;
    DWORD tid = HTASK_32( hTask );

    if (!tid) return FALSE;
    info.proc  = func;
    info.param = lParam;
    return EnumThreadWindows( tid, wnd_enum_callback, (LPARAM)&info );
}


/**************************************************************************
 *              GetTopWindow   (USER.229)
 */
HWND16 WINAPI GetTopWindow16( HWND16 hwnd )
{
    return HWND_16( GetTopWindow( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              GetNextWindow   (USER.230)
 */
HWND16 WINAPI GetNextWindow16( HWND16 hwnd, WORD flag )
{
    if ((flag != GW_HWNDNEXT) && (flag != GW_HWNDPREV)) return 0;
    return GetWindow16( hwnd, flag );
}


/**************************************************************************
 *              SetWindowPos   (USER.232)
 */
BOOL16 WINAPI SetWindowPos16( HWND16 hwnd, HWND16 hwndInsertAfter,
                              INT16 x, INT16 y, INT16 cx, INT16 cy, WORD flags)
{
    HWND hwnd32 = WIN_Handle32(hwnd);
    if (hwnd == (WORD)HWND_TOPMOST)
    {
        hwnd32 = HWND_TOPMOST;
    }
    if (hwnd == (WORD)HWND_NOTOPMOST)
    {
        hwnd32 = HWND_NOTOPMOST;
    }
    if (flags & SWP_NOREDRAW)
    {
        RECT rect;
        GetWindowRect(hwnd32, &rect);
        /* top-level window */
        if (GetAncestor(hwnd32, GA_PARENT) == GetDesktopWindow())
        {
            flags &= ~SWP_NOREDRAW;
        }
        if (rect.left == rect.right || rect.bottom == rect.top)
        {
            flags &= ~SWP_NOREDRAW;
        }
    }
    DWORD count;
    ReleaseThunkLock(&count);
    BOOL16 result = SetWindowPos( hwnd32, full_insert_after_hwnd(hwndInsertAfter),
                         x, y, cx, cy, flags );
    RestoreThunkLock(count);
    return result;
}


/**************************************************************************
 *              SetParent   (USER.233)
 */
HWND16 WINAPI SetParent16( HWND16 hwndChild, HWND16 hwndNewParent )
{
    return HWND_16( SetParent( WIN_Handle32(hwndChild), WIN_Handle32(hwndNewParent) ));
}


/**************************************************************************
 *              GetCapture   (USER.236)
 */
HWND16 WINAPI GetCapture16(void)
{
    return HWND_16( GetCapture() );
}


/**************************************************************************
 *              GetUpdateRgn   (USER.237)
 */
INT16 WINAPI GetUpdateRgn16( HWND16 hwnd, HRGN16 hrgn, BOOL16 erase )
{
    return GetUpdateRgn( WIN_Handle32(hwnd), HRGN_32(hrgn), erase );
}


/**************************************************************************
 *              ExcludeUpdateRgn   (USER.238)
 */
INT16 WINAPI ExcludeUpdateRgn16( HDC16 hdc, HWND16 hwnd )
{
    return ExcludeUpdateRgn( HDC_32(hdc), WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetOpenClipboardWindow   (USER.248)
 */
HWND16 WINAPI GetOpenClipboardWindow16(void)
{
    return HWND_16( GetOpenClipboardWindow() );
}


/*******************************************************************
 *		MapWindowPoints (USER.258)
 */
void WINAPI MapWindowPoints16( HWND16 hwndFrom, HWND16 hwndTo, LPPOINT16 lppt, UINT16 count )
{
    POINT buffer[8], *ppt = buffer;
    UINT i;

    if (count > 8) ppt = HeapAlloc( GetProcessHeap(), 0, count * sizeof(*ppt) );
    for (i = 0; i < count; i++)
    {
        ppt[i].x = lppt[i].x;
        ppt[i].y = lppt[i].y;
    }
    MapWindowPoints( WIN_Handle32(hwndFrom), WIN_Handle32(hwndTo), ppt, count );
    for (i = 0; i < count; i++)
    {
        lppt[i].x = ppt[i].x;
        lppt[i].y = ppt[i].y;
    }
    if (ppt != buffer) HeapFree( GetProcessHeap(), 0, ppt );
}


/**************************************************************************
 *              BeginDeferWindowPos   (USER.259)
 */
HDWP16 WINAPI BeginDeferWindowPos16( INT16 count )
{
    return HDWP_16(BeginDeferWindowPos( count ));
}


/**************************************************************************
 *              DeferWindowPos   (USER.260)
 */
HDWP16 WINAPI DeferWindowPos16( HDWP16 hdwp, HWND16 hwnd, HWND16 hwndAfter,
                                INT16 x, INT16 y, INT16 cx, INT16 cy,
                                UINT16 flags )
{
    return HDWP_16(DeferWindowPos( HDWP_32(hdwp), WIN_Handle32(hwnd),
		   full_insert_after_hwnd(hwndAfter), x, y, cx, cy, flags ));
}


/**************************************************************************
 *              EndDeferWindowPos   (USER.261)
 */
BOOL16 WINAPI EndDeferWindowPos16( HDWP16 hdwp )
{
    return EndDeferWindowPos(HDWP_32(hdwp));
}


/**************************************************************************
 *              GetWindow   (USER.262)
 */
HWND16 WINAPI GetWindow16( HWND16 hwnd, WORD rel )
{
    return HWND_16( GetWindow( WIN_Handle32(hwnd), rel ) );
}


/**************************************************************************
 *              ShowOwnedPopups   (USER.265)
 */
void WINAPI ShowOwnedPopups16( HWND16 owner, BOOL16 fShow )
{
    ShowOwnedPopups( WIN_Handle32(owner), fShow );
}


/**************************************************************************
 *              ShowScrollBar   (USER.267)
 */
void WINAPI ShowScrollBar16( HWND16 hwnd, INT16 nBar, BOOL16 fShow )
{
    ShowScrollBar( WIN_Handle32(hwnd), nBar, fShow );
}


/**************************************************************************
 *              IsZoomed   (USER.272)
 */
BOOL16 WINAPI IsZoomed16(HWND16 hwnd)
{
    return IsZoomed( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetDlgCtrlID   (USER.277)
 */
INT16 WINAPI GetDlgCtrlID16( HWND16 hwnd )
{
    return GetDlgCtrlID( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              GetDesktopHwnd   (USER.278)
 *
 * Exactly the same thing as GetDesktopWindow(), but not documented.
 * Don't ask me why...
 */
HWND16 WINAPI GetDesktopHwnd16(void)
{
    return GetDesktopWindow16();
}


/**************************************************************************
 *              SetSystemMenu   (USER.280)
 */
BOOL16 WINAPI SetSystemMenu16( HWND16 hwnd, HMENU16 hMenu )
{
    return SetSystemMenu( WIN_Handle32(hwnd), HMENU_32(hMenu) );
}


/**************************************************************************
 *              GetDesktopWindow   (USER.286)
 */
HWND16 WINAPI GetDesktopWindow16(void)
{
    return HWND_16( GetDesktopWindow() );
}


/**************************************************************************
 *              GetLastActivePopup   (USER.287)
 */
HWND16 WINAPI GetLastActivePopup16( HWND16 hwnd )
{
    return HWND_16( GetLastActivePopup( WIN_Handle32(hwnd) ));
}


/**************************************************************************
 *              RedrawWindow   (USER.290)
 */
BOOL16 WINAPI RedrawWindow16( HWND16 hwnd, const RECT16 *rectUpdate,
                              HRGN16 hrgnUpdate, UINT16 flags )
{
    DWORD count;
    BOOL result;
    ReleaseThunkLock(&count);
    if (rectUpdate)
    {
        RECT r;
        r.left   = rectUpdate->left;
        r.top    = rectUpdate->top;
        r.right  = rectUpdate->right;
        r.bottom = rectUpdate->bottom;
        result = RedrawWindow(WIN_Handle32(hwnd), &r, HRGN_32(hrgnUpdate), flags);
    }
    else
    {
        result = RedrawWindow(WIN_Handle32(hwnd), NULL, HRGN_32(hrgnUpdate), flags);
    }
    RestoreThunkLock(count);
}


/**************************************************************************
 *              LockWindowUpdate   (USER.294)
 */
BOOL16 WINAPI LockWindowUpdate16( HWND16 hwnd )
{
    return LockWindowUpdate( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              ScrollWindowEx   (USER.319)
 */
INT16 WINAPI ScrollWindowEx16( HWND16 hwnd, INT16 dx, INT16 dy,
                               const RECT16 *rect, const RECT16 *clipRect,
                               HRGN16 hrgnUpdate, LPRECT16 rcUpdate,
                               UINT16 flags )
{
    RECT rect32, clipRect32, rcUpdate32;
    BOOL16 ret;

    if (rect)
    {
        rect32.left   = rect->left;
        rect32.top    = rect->top;
        rect32.right  = rect->right;
        rect32.bottom = rect->bottom;
    }
    if (clipRect)
    {
        clipRect32.left   = clipRect->left;
        clipRect32.top    = clipRect->top;
        clipRect32.right  = clipRect->right;
        clipRect32.bottom = clipRect->bottom;
    }
    ret = ScrollWindowEx( WIN_Handle32(hwnd), dx, dy, rect ? &rect32 : NULL,
                          clipRect ? &clipRect32 : NULL, HRGN_32(hrgnUpdate),
                          (rcUpdate) ? &rcUpdate32 : NULL, flags );
    if (rcUpdate)
    {
        rcUpdate->left   = rcUpdate32.left;
        rcUpdate->top    = rcUpdate32.top;
        rcUpdate->right  = rcUpdate32.right;
        rcUpdate->bottom = rcUpdate32.bottom;
    }
    return ret;
}


/**************************************************************************
 *              FillWindow   (USER.324)
 */
void WINAPI FillWindow16( HWND16 hwndParent, HWND16 hwnd, HDC16 hdc, HBRUSH16 hbrush )
{
    RECT rect;
    RECT16 rc16;
    GetClientRect( WIN_Handle32(hwnd), &rect );
    DPtoLP( HDC_32(hdc), (LPPOINT)&rect, 2 );
    rc16.left   = rect.left;
    rc16.top    = rect.top;
    rc16.right  = rect.right;
    rc16.bottom = rect.bottom;
    PaintRect16( hwndParent, hwnd, hdc, hbrush, &rc16 );
}


/**************************************************************************
 *              PaintRect   (USER.325)
 */
void WINAPI PaintRect16( HWND16 hwndParent, HWND16 hwnd, HDC16 hdc,
                         HBRUSH16 hbrush, const RECT16 *rect)
{
    if (hbrush <= CTLCOLOR_STATIC)
    {
        HWND parent = WIN_Handle32(hwndParent), hwnd32 = WIN_Handle32(hwnd);

        if (!parent) return;
        hbrush = SendMessageW( parent, WM_CTLCOLORMSGBOX + hbrush, hdc, (LPARAM)hwnd32 );
        if (!hbrush) hbrush = DefWindowProcW( parent, WM_CTLCOLORMSGBOX + hbrush,
                                              hdc, (LPARAM)hwnd32 );
    }
    if (hbrush) FillRect16( hdc, rect, hbrush );
}


/**************************************************************************
 *              GetControlBrush   (USER.326)
 */
HBRUSH16 WINAPI GetControlBrush16( HWND16 hwnd, HDC16 hdc, UINT16 ctlType )
{
    HBRUSH16 ret;
    HWND hwnd32 = WIN_Handle32(hwnd);
    HWND parent = GetParent( hwnd32 );

    if (!parent) parent = hwnd32;
    ret = SendMessageW( parent, WM_CTLCOLORMSGBOX + ctlType, hdc, (LPARAM)hwnd32 );
    if (!ret) ret = DefWindowProcW( parent, WM_CTLCOLORMSGBOX + ctlType,
                                    hdc, (LPARAM)hwnd32 );
    return ret;
}


/**************************************************************************
 *              GetDCEx   (USER.359)
 */
HDC16 WINAPI GetDCEx16( HWND16 hwnd, HRGN16 hrgnClip, DWORD flags )
{
    return HDC_16(GetDCEx(WIN_Handle32(hwnd), HRGN_32(hrgnClip), flags));
}


/**************************************************************************
 *              GetWindowPlacement   (USER.370)
 */
BOOL16 WINAPI GetWindowPlacement16( HWND16 hwnd, WINDOWPLACEMENT16 *wp16 )
{
    WINDOWPLACEMENT wpl;

    wpl.length = sizeof(wpl);
    if (!GetWindowPlacement( WIN_Handle32(hwnd), &wpl )) return FALSE;
    wp16->length  = sizeof(*wp16);
    wp16->flags   = wpl.flags;
    wp16->showCmd = wpl.showCmd;
    wp16->ptMinPosition.x = wpl.ptMinPosition.x;
    wp16->ptMinPosition.y = wpl.ptMinPosition.y;
    wp16->ptMaxPosition.x = wpl.ptMaxPosition.x;
    wp16->ptMaxPosition.y = wpl.ptMaxPosition.y;
    wp16->rcNormalPosition.left   = wpl.rcNormalPosition.left;
    wp16->rcNormalPosition.top    = wpl.rcNormalPosition.top;
    wp16->rcNormalPosition.right  = wpl.rcNormalPosition.right;
    wp16->rcNormalPosition.bottom = wpl.rcNormalPosition.bottom;
    return TRUE;
}


/**************************************************************************
 *              SetWindowPlacement   (USER.371)
 */
BOOL16 WINAPI SetWindowPlacement16( HWND16 hwnd, const WINDOWPLACEMENT16 *wp16 )
{
    WINDOWPLACEMENT wpl;

    if (!wp16) return FALSE;
    wpl.length  = sizeof(wpl);
    wpl.flags   = wp16->flags;
    wpl.showCmd = wp16->showCmd;
    wpl.ptMinPosition.x = wp16->ptMinPosition.x;
    wpl.ptMinPosition.y = wp16->ptMinPosition.y;
    wpl.ptMaxPosition.x = wp16->ptMaxPosition.x;
    wpl.ptMaxPosition.y = wp16->ptMaxPosition.y;
    wpl.rcNormalPosition.left   = wp16->rcNormalPosition.left;
    wpl.rcNormalPosition.top    = wp16->rcNormalPosition.top;
    wpl.rcNormalPosition.right  = wp16->rcNormalPosition.right;
    wpl.rcNormalPosition.bottom = wp16->rcNormalPosition.bottom;
    return SetWindowPlacement( WIN_Handle32(hwnd), &wpl );
}

struct WNDCLASS16Info WNDCLASS16Info[65536];
struct WNDCLASS16Info *WNDCLASS16InfoStringAtom[65536];
#include "../toolhelp/toolhelp.h"

__declspec(dllexport) BOOL16 WINAPI USER_ClassFirst16(CLASSENTRY *pClassEntry)
{
    FIXME("\n");
    return FALSE;
}
__declspec(dllexport) BOOL16 WINAPI USER_ClassNext16(CLASSENTRY *pClassEntry)
{
    FIXME("\n");
    return FALSE;
}

NE_TYPEINFO *get_resource_table(HMODULE16 hmod, LPCSTR type, LPBYTE *restab);
/***********************************************************************
 *		RegisterClassEx (USER.397)
 */
ATOM WINAPI RegisterClassEx16( const WNDCLASSEX16 *wc )
{
    struct class_entry *class;
    WNDCLASSEXA wc32;
    HINSTANCE16 inst;
    ATOM atom;
    SIZE_T class_len;
    SIZE_T menu_len;
    SIZE_T buf_len;
    struct A
    {
        LPCSTR local_class_prefix;
        DWORD hInst;
        LPCSTR name;
    } a;
    char atombuf[256];
    va_list arg;
    LPCSTR buf = NULL;
    LPCSTR classname32 = MapSL(wc->lpszClassName);

    inst = GetExePtr( wc->hInstance );
    if (!inst)
    {
        inst = GetExePtr(GetCurrentTask());
    }

    wc32.cbSize        = sizeof(wc32);
    wc32.style         = wc->style | (IsOldWindowsTask(GetCurrentTask()) ? CS_GLOBALCLASS : 0);
    wc32.lpfnWndProc   = WindowProc16;//WINPROC_AllocProc16( wc->lpfnWndProc );
    wc32.cbClsExtra    = wc->cbClsExtra;
    wc32.cbWndExtra    = 100;
    wc32.hInstance     = HINSTANCE_32(inst);
    wc32.hIcon         = get_icon_32(wc->hIcon);
    
    if (!wc32.hIcon)
    {
        LPBYTE restab;
        NE_TYPEINFO *type = get_resource_table(inst, RT_GROUP_ICON, &restab);
        if (!type)
            type = get_resource_table(inst, RT_ICON, &restab);
        if (type)
        {
            LPCSTR id = (LPCSTR)(((NE_NAMEINFO *)((char *)type + sizeof(NE_TYPEINFO)))->id);
            char name[32] = {0};
            if (!((int)id & 0x8000))
            {
                int len = restab[0] > 31 ? 31 : restab[0];
                strncpy(name, restab + (int)id + 1, len);
                id = name;
            }
            wc32.hIcon = get_icon_32(LoadIcon16(inst, id));
         }
    }

    wc32.hCursor       = get_icon_32( wc->hCursor );
    wc32.hbrBackground = HBRUSH_32(wc->hbrBackground);
    wc32.lpszMenuName  = MapSL(wc->lpszMenuName);
    wc32.lpszClassName = classname32;
    wc32.hIconSm       = get_icon_32(wc->hIconSm);
    if (!HIWORD(wc32.lpszClassName))
    {
        GlobalGetAtomNameA(wc32.lpszClassName, atombuf, 256);
        wc32.lpszClassName = atombuf;
    }
    a.hInst = inst;
    a.name = wc32.lpszClassName;
    a.local_class_prefix = LOCAL_CLASS_PREFIX;
    arg = (va_list)&a;
    /*
    WNDCLASS.hInstance behaviour is mysterious
    RegisterClass(hInst=0xcafe,...)
    GetClassInfoEx(hInst=0xcafe,...) success
    CreateWindow(hInst=0x1234,...) success
    GetClassInfoEx(hInst=0xbeef,...) success???
    CreateWindow(hInst=0xbeef,...) fail???

    add hModule prefix to classname
    */
    if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING, "%1!s!%2!04X!%3!s!%0", NULL, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), &buf, 0, &arg))
    {
        wc32.lpszClassName = buf;
    }
    /*
    register same class:
    Win3.1 returns 0
    WOW returns same class atom
    */
    atom = RegisterClassExA( &wc32 );
    TRACE("(%08x,%08x,%04x,%04x,%04x,%04x,%04x,%04x,%s,%s,%04x) Ret:%04x\n", wc->style, wc->lpfnWndProc, wc->cbClsExtra, wc->cbWndExtra, wc->hInstance, wc->hIcon, wc->hCursor, wc->hbrBackground, debugstr_a(wc32.lpszMenuName), debugstr_a(wc32.lpszClassName), wc->hIconSm, atom);
	if (atom)
	{
        WNDCLASS16Info[atom].allocated = TRUE;
        WNDCLASS16Info[atom].wndproc = (DWORD)(wc->lpfnWndProc);
        WNDCLASS16Info[atom].cbWndExtra = wc->cbWndExtra;
	}
    class_len = strlen(a.name) + 1;
    menu_len = !IS_INTRESOURCE(wc->lpszMenuName) ? strlen(wc32.lpszMenuName) + 1 : 0;
    buf_len = strlen(buf) + 1;

	if (atom && (class = HeapAlloc(GetProcessHeap(), 0, sizeof(*class) + class_len + menu_len + buf_len)))
    {
		class->atom = atom;
        class->inst = inst;
		class->wndproc16 = WNDCLASS16Info[atom].wndproc;
        class->classInfo = wc32;
        class->classInfo.lpszClassName = (LPBYTE)class + sizeof(*class);
        memcpy(class->classInfo.lpszClassName, a.name, class_len);
        if (menu_len)
        {
            class->classInfo.lpszMenuName = (LPBYTE)class + sizeof(*class) + class_len;
            memcpy(class->classInfo.lpszMenuName, wc32.lpszMenuName, menu_len);
        }
        class->win32_classname = (LPBYTE)class + sizeof(*class) + class_len + menu_len;
        memcpy(class->win32_classname, buf, buf_len);
        list_add_tail( &class_list, &class->entry );
    }
    LocalFree(buf);
    return atom;
}

struct class_entry *find_win32_class(LPCSTR name)
{
    struct class_entry *clazz;
    ATOM class_atom = LOWORD(name);
    BOOL is_atom = IS_INTRESOURCE(name);
    LIST_FOR_EACH_ENTRY(clazz, &class_list, struct class_entry, entry)
    {
        if (is_atom)
        {
            if (class_atom == clazz->atom)
            {
                return clazz;
            }
            continue;
        }
        if (!strcmpi(name, clazz->win32_classname))
        {
            return clazz;
        }
    }
    return NULL;
}

struct class_entry *find_win16_class(HINSTANCE16 hInst16, LPCSTR name)
{
    struct class_entry *clazz;
    ATOM class_atom = LOWORD(name);
    BOOL is_atom = IS_INTRESOURCE(name);
    hInst16 = GetExePtr(hInst16);
    LIST_FOR_EACH_ENTRY(clazz, &class_list, struct class_entry, entry)
    {
        if ((clazz->inst == hInst16 || (clazz->classInfo.style & CS_GLOBALCLASS)))
        {
            if (is_atom)
            {
                if (class_atom == clazz->atom)
                {
                    return clazz;
                }
                continue;
            }
            if (!strcmpi(name, clazz->classInfo.lpszClassName))
            {
                return clazz;
            }
        }
    }
    return NULL;
}
LPCSTR win16classname(LPCSTR name)
{
    struct class_entry *entry = find_win32_class(name);
    if (entry)
        return entry->classInfo.lpszClassName;
    if (HIWORD(name) && name[0] == '#')
    {
        ATOM atom = FindAtomA(name);
        if (atom != 0)
            return atom;
    }
    return name;
}
LPCSTR win32classname(HINSTANCE16 hInst16, LPCSTR name)
{
    struct class_entry *entry = find_win16_class(hInst16, name);
    if (entry)
        return entry->win32_classname;
    return name;
}

/***********************************************************************
 *		GetClassInfoEx (USER.398)
 *
 * FIXME: this is just a guess, I have no idea if GetClassInfoEx() is the
 * same in Win16 as in Win32. --AJ
 */
BOOL16 WINAPI GetClassInfoEx16( HINSTANCE16 hInst16, SEGPTR name, WNDCLASSEX16 *wc )
{
    static HMODULE user32_module;
    WNDCLASSEXA wc32;
    HINSTANCE hInstance;
    ATOM ret = 0;
    struct class_entry *class;
    LPCSTR name32 = MapSL(name);
    ATOM class_atom = LOWORD(name);
    BOOL is_atom = IS_INTRESOURCE(name);

    if (!user32_module) user32_module = GetModuleHandleA( "user32.dll" );
    if (hInst16 == GetModuleHandle16("user")) hInstance = user32_module;
    else hInstance = HINSTANCE_32(GetExePtr( hInst16 ));

    class = find_win16_class(hInst16, name32);
    if (class)
    {
        wc32 = class->classInfo;
        ret = class->atom;
    }
    /* win32 class */
    if (!ret)
        ret = GetClassInfoExA( hInstance, name32, &wc32 );

    if (ret)
    {
        wc->lpfnWndProc = WNDCLASS16Info[ret].wndproc ? WNDCLASS16Info[ret].wndproc : WINPROC_AllocNativeProc(wc32.lpfnWndProc);//WINPROC_GetProc16( wc32.lpfnWndProc, FALSE );
        wc->style         = wc32.style;
        wc->cbClsExtra    = wc32.cbClsExtra;
        BOOL f;
        WORD cb = get_system_window_class_wndextra(wc32.lpszClassName, &f);
        wc->cbWndExtra    = f ? cb : wc32.cbWndExtra;
        wc->hInstance     = (wc32.hInstance == user32_module) ? GetModuleHandle16("user") : HINSTANCE_16(wc32.hInstance);
        wc->hIcon         = get_icon_16( wc32.hIcon );
        wc->hIconSm       = get_icon_16( wc32.hIconSm );
        wc->hCursor       = get_icon_16( wc32.hCursor );
        wc->hbrBackground = HBRUSH_16(wc32.hbrBackground);
        wc->lpszClassName = MapLS(wc32.lpszClassName);
        wc->lpszMenuName  = MapLS(wc32.lpszMenuName);  /* FIXME: leak */
    }
    return ret;
}


/**************************************************************************
 *              ChildWindowFromPointEx   (USER.399)
 */
HWND16 WINAPI ChildWindowFromPointEx16( HWND16 hwndParent, POINT16 pt, UINT16 uFlags)
{
    POINT pt32;

    pt32.x = pt.x;
    pt32.y = pt.y;
    return HWND_16( ChildWindowFromPointEx( WIN_Handle32(hwndParent), pt32, uFlags ));
}


/**************************************************************************
 *              GetPriorityClipboardFormat   (USER.402)
 */
INT16 WINAPI GetPriorityClipboardFormat16( UINT16 *list, INT16 count )
{
    int i;

    for (i = 0; i < count; i++)
        if (IsClipboardFormatAvailable( list[i] )) return list[i];
    return -1;
}


/***********************************************************************
 *		UnregisterClass (USER.403)
 */
BOOL16 WINAPI UnregisterClass16( LPCSTR className, HINSTANCE16 hInstance )
{
    ATOM atom;

    if (hInstance == GetModuleHandle16("user")) hInstance = 0;
    else hInstance = GetExePtr( hInstance );
    struct class_entry *win32 = find_win16_class(hInstance, className);

    if (win32)
    {
        WNDCLASS16Info[win32->atom].allocated = FALSE;
        className = win32->win32_classname;
        BOOL ret = UnregisterClassA(className, HINSTANCE_32(hInstance));
        list_remove(&win32->entry);
        HeapFree(GetProcessHeap(), 0, win32);
        return ret;
    }
    return UnregisterClassA( className, HINSTANCE_32(hInstance) );
}


/***********************************************************************
 *		GetClassInfo (USER.404)
 */
BOOL16 WINAPI GetClassInfo16( HINSTANCE16 hInst16, SEGPTR name, WNDCLASS16 *wc )
{
    WNDCLASSEX16 wcex;
    UINT16 ret = GetClassInfoEx16( hInst16, name, &wcex );

    if (ret)
    {
        wc->style         = wcex.style;
        wc->lpfnWndProc   = wcex.lpfnWndProc;
        wc->cbClsExtra    = wcex.cbClsExtra;
        wc->cbWndExtra    = wcex.cbWndExtra;
        wc->hInstance     = wcex.hInstance;
        wc->hIcon         = wcex.hIcon;
        wc->hCursor       = wcex.hCursor;
        wc->hbrBackground = wcex.hbrBackground;
        wc->lpszMenuName  = wcex.lpszMenuName;
        wc->lpszClassName = wcex.lpszClassName;
    }
    return ret;
}


/**************************************************************************
 *              TrackPopupMenu   (USER.416)
 */
BOOL16 WINAPI TrackPopupMenu16( HMENU16 hMenu, UINT16 wFlags, INT16 x, INT16 y,
                                INT16 nReserved, HWND16 hwnd, const RECT16 *lpRect )
{
    RECT r;
    if (lpRect)
    {
        r.left   = lpRect->left;
        r.top    = lpRect->top;
        r.right  = lpRect->right;
        r.bottom = lpRect->bottom;
    }
    return TrackPopupMenu( HMENU_32(hMenu), wFlags, x, y, nReserved,
                           WIN_Handle32(hwnd), lpRect ? &r : NULL );
}


/**************************************************************************
 *              FindWindowEx   (USER.427)
 */
HWND16 WINAPI FindWindowEx16( HWND16 parent, HWND16 child, LPCSTR className, LPCSTR title )
{
    char sbuf[200];
    char *buf = sbuf;
    SIZE_T buf_size = sizeof(sbuf) / sizeof(char);
    HWND child32 = WIN_Handle32(child);
    HWND parent32 = WIN_Handle32(parent);
    LPCSTR lcprefix = LOCAL_CLASS_PREFIX;
    SIZE_T lcprefix_len = strlen(lcprefix);
    char atomname[256];

    if (!HIWORD(className) && LOWORD(className))
    {
        GlobalGetAtomNameA(className, atomname, ARRAYSIZE(atomname));
        className = atomname;
    }
    while (TRUE)
    {
        child32 = FindWindowExA(NULL, child32, NULL, title);
        if (!child32)
            break;
        if (!className)
            break;
        int classname_len = 0;
        while (TRUE)
        {
            classname_len = GetClassNameA(child32, buf, buf_size);
            if (!classname_len)
            {
                if (buf_size != 0)
                {
                    sbuf[0] = '\0';
                }
                break;
            }
            //truncated
            if (classname_len == buf_size - 1)
            {
                LPVOID heap;
                if (buf != sbuf)
                {
                    heap = HeapReAlloc(GetProcessHeap(), 0, buf, buf_size * 2);
                }
                else
                {
                    heap = HeapAlloc(GetProcessHeap(), 0, buf_size * 2);
                }
                if (!heap)
                    break;
                buf = (LPCSTR)heap;
                buf_size *= 2;
                continue;
            }
            break;
        }
        if (classname_len <= lcprefix_len)
        {
            if (!strcmpi(sbuf, className))
            {
                break;
            }
            continue;
        }
        //is win16 local class
        if (!memicmp(sbuf, LOCAL_CLASS_PREFIX, lcprefix_len))
        {
            LPCSTR win16cls = win16classname(sbuf);
            if (!strcmpi(win16cls, className))
            {
                break;
            }
        }
        if (!strcmpi(sbuf, className))
        {
            break;
        }
    }
    if (buf != sbuf)
    {
        HeapFree(GetProcessHeap(), 0, buf);
    }
    return HWND_16(child32);
}


LRESULT def_frame_proc_callback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *result, void *arg)
{
    DWORD count;
    ReleaseThunkLock(&count);
    *result = DefFrameProcA(hwnd, (HWND)arg, msg, wp, lp);
    RestoreThunkLock(count);
    return *result;
}
/***********************************************************************
 *		DefFrameProc (USER.445)
 */
LRESULT WINAPI DefFrameProc16( HWND16 hwnd, HWND16 hwndMDIClient,
                               UINT16 message, WPARAM16 wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_SETTEXT:
        lParam = (LPARAM)MapSL(lParam);
        /* fall through */
    case WM_COMMAND:
    case WM_SETFOCUS:
    case WM_SIZE:
        return DefFrameProcA( WIN_Handle32(hwnd), WIN_Handle32(hwndMDIClient),
                              message, wParam, lParam );
	case WM_NCACTIVATE:
		return DefFrameProcA(WIN_Handle32(hwnd), WIN_Handle32(hwndMDIClient),
			message, wParam, HRGN_32(lParam));

    case WM_NEXTMENU:
        {
            MDINEXTMENU next_menu;
            DefFrameProcW( WIN_Handle32(hwnd), WIN_Handle32(hwndMDIClient),
                           message, wParam, (LPARAM)&next_menu );
            return MAKELONG( HMENU_16(next_menu.hmenuNext), HWND_16(next_menu.hwndNext) );
        }
    default:
    {
        LRESULT result;
        WINPROC_CallProc16To32A(def_frame_proc_callback, hwnd, message, wParam, lParam, &result, HWND_32(hwndMDIClient));
        return result;
    }
    }
}


/***********************************************************************
 *		DefMDIChildProc (USER.447)
 */
LRESULT WINAPI DefMDIChildProc16( HWND16 hwnd, UINT16 message,
                                  WPARAM16 wParam, LPARAM lParam )
{
    switch (message)
    {
    case WM_SETTEXT:
        return DefMDIChildProcA( WIN_Handle32(hwnd), message, wParam, (LPARAM)MapSL(lParam) );

    case WM_SETFOCUS:
        return DefMDIChildProcW(WIN_Handle32(hwnd), message, HWND_32(wParam), lParam);

    case WM_MENUCHAR:
    case WM_CLOSE:
    case WM_CHILDACTIVATE:
    case WM_SYSCOMMAND:
    case WM_SETVISIBLE:
    case WM_SIZE:
    case WM_SYSCHAR:
        return DefMDIChildProcW( WIN_Handle32(hwnd), message, wParam, lParam );

    case WM_GETMINMAXINFO:
        {
            MINMAXINFO16 *mmi16 = MapSL(lParam);
            MINMAXINFO mmi;

            mmi.ptReserved.x     = mmi16->ptReserved.x;
            mmi.ptReserved.y     = mmi16->ptReserved.y;
            mmi.ptMaxSize.x      = mmi16->ptMaxSize.x;
            mmi.ptMaxSize.y      = mmi16->ptMaxSize.y;
            mmi.ptMaxPosition.x  = mmi16->ptMaxPosition.x;
            mmi.ptMaxPosition.y  = mmi16->ptMaxPosition.y;
            mmi.ptMinTrackSize.x = mmi16->ptMinTrackSize.x;
            mmi.ptMinTrackSize.y = mmi16->ptMinTrackSize.y;
            mmi.ptMaxTrackSize.x = mmi16->ptMaxTrackSize.x;
            mmi.ptMaxTrackSize.y = mmi16->ptMaxTrackSize.y;

            DefMDIChildProcW( WIN_Handle32(hwnd), message, wParam, (LPARAM)&mmi );

            mmi16->ptReserved.x     = mmi.ptReserved.x;
            mmi16->ptReserved.y     = mmi.ptReserved.y;
            mmi16->ptMaxSize.x      = mmi.ptMaxSize.x;
            mmi16->ptMaxSize.y      = mmi.ptMaxSize.y;
            mmi16->ptMaxPosition.x  = mmi.ptMaxPosition.x;
            mmi16->ptMaxPosition.y  = mmi.ptMaxPosition.y;
            mmi16->ptMinTrackSize.x = mmi.ptMinTrackSize.x;
            mmi16->ptMinTrackSize.y = mmi.ptMinTrackSize.y;
            mmi16->ptMaxTrackSize.x = mmi.ptMaxTrackSize.x;
            mmi16->ptMaxTrackSize.y = mmi.ptMaxTrackSize.y;
            return 0;
        }
    case WM_NEXTMENU:
        {
            MDINEXTMENU next_menu;
            DefMDIChildProcW( WIN_Handle32(hwnd), message, wParam, (LPARAM)&next_menu );
            return MAKELONG( HMENU_16(next_menu.hmenuNext), HWND_16(next_menu.hwndNext) );
        }
    default:
        return DefWindowProc16(hwnd, message, wParam, lParam);
    }
}


/**************************************************************************
 *              DrawAnimatedRects   (USER.448)
 */
BOOL16 WINAPI DrawAnimatedRects16( HWND16 hwnd, INT16 idAni,
                                   const RECT16* lprcFrom, const RECT16* lprcTo )
{
    RECT rcFrom32, rcTo32;
    rcFrom32.left   = lprcFrom->left;
    rcFrom32.top    = lprcFrom->top;
    rcFrom32.right  = lprcFrom->right;
    rcFrom32.bottom = lprcFrom->bottom;
    rcTo32.left     = lprcTo->left;
    rcTo32.top      = lprcTo->top;
    rcTo32.right    = lprcTo->right;
    rcTo32.bottom   = lprcTo->bottom;
    return DrawAnimatedRects( WIN_Handle32(hwnd), idAni, &rcFrom32, &rcTo32 );
}

/***********************************************************************
 *		CreateWindowEx (USER.452)
 */
HWND16 WINAPI CreateWindowEx16( DWORD exStyle, LPCSTR className,
                                LPCSTR windowName, DWORD style, INT16 x,
                                INT16 y, INT16 width, INT16 height,
                                HWND16 parent, HMENU16 menu,
                                HINSTANCE16 instance, LPVOID data )
{
    CREATESTRUCTA cs;
    char buffer[256];
    HWND hwnd;
    BOOL release = FALSE;
    DWORD count;

    if (instance == NULL)
    {
        HTASK16 task = GetCurrentTask();
        TDB *tdb = GlobalLock16(task);
        instance = tdb->hInstance;
        GlobalUnlock16(task);
    }

    /* Fix the coordinates */

    cs.x  = (x == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)x;
    cs.y  = (y == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)y;
    cs.cx = (width == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)width;
    cs.cy = (height == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)height;

    // make windows 1.0 programs appear in a usable window
    if (IsOldWindowsTask(GetCurrentTask()) && !(style & WS_CHILD) && (!cs.cx || !cs.cy))
    {
        cs.x = CW_USEDEFAULT;
        cs.y = CW_USEDEFAULT;
        cs.cx = CW_USEDEFAULT;
        cs.cy = CW_USEDEFAULT;
    }

    /* Create the window */

    cs.lpCreateParams = data;
    cs.hInstance      = HINSTANCE_32(instance);
    cs.hMenu          = menu;
	
    cs.hwndParent     = WIN_Handle32( parent );
    cs.style          = style;
    cs.lpszName       = windowName;
    cs.lpszClass      = className;
    cs.dwExStyle      = exStyle;

    if (cs.hwndParent && IsWindow(cs.hwndParent))
    {
        HTASK16 parenttask = GetWindowTask16(parent);
        if (parenttask != 0 && parenttask != GetCurrentTask())
        {
            TRACE("GetWindowTask16(parent) != GetCurrentTask()\n");
            release = TRUE;
        }
    }

    cs.lpszClass = win32classname(cs.hInstance, cs.lpszClass);
    if (!IS_INTRESOURCE(cs.lpszClass))
    {
    }
    else
    {
        if (!GlobalGetAtomNameA(LOWORD(className), buffer, sizeof(buffer))) return 0;
        cs.lpszClass = buffer;
        cs.lpszClass = win32classname(cs.hInstance, cs.lpszClass);
    }
    /* load the menu */
    if (!menu && !is_win_menu_disallowed(style))
    {
        WNDCLASSA class;
        HINSTANCE16 module = GetExePtr( instance );

        if (GetClassInfoA( HINSTANCE_32(module), cs.lpszClass, &class ))
            cs.hMenu = HMENU_32( LoadMenu16( module, class.lpszMenuName ));
    }
    else if (!is_win_menu_disallowed(style))
    {
        cs.hMenu = HMENU_32(menu);
    }

    WNDCLASSEXA wndclass;
    //reactos win32ss/user/ntuser/window.c
    if (GetClassInfoExA(cs.hInstance, cs.lpszClass, &wndclass) && cs.hwndParent && (wndclass.style & CS_PARENTDC) && !(GetWindowLongA(cs.hwndParent, GWL_STYLE)  & WS_CLIPCHILDREN))
    {
        cs.style &= ~(WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    }
    if (release)
    {
        ReleaseThunkLock(&count);
    }
    hwnd = create_window16((CREATESTRUCTW *)&cs, (LPCWSTR)cs.lpszClass, HINSTANCE_32(instance), FALSE);
    if (release)
    {
        RestoreThunkLock(count);
    }
    if (hwnd == NULL)
    {
        ERR("Could not create window(%08x,\"%s\"(\"%s\"),\"%s\",%08x,%04x,%04x,%04x,%04x,%04x,%04x,%04x,%08x)\n", exStyle, IS_INTRESOURCE(className) ? "(ATOM)" : className, cs.lpszClass, windowName, style, x, y, width, height, parent, menu, instance, data);
        return NULL;
    }
    HWND16 hWnd16 = HWND_16(hwnd);
	InitWndProc16(hwnd, hWnd16);
    SetWindowHInst16(hWnd16, instance);
    SetWindowHMenu16(hWnd16, menu);
	return hWnd16;
}
void InitWndProc16(HWND hWnd, HWND16 hWnd16)
{
	ATOM classatom = GetClassLongA(hWnd, GCW_ATOM);//WNDPROC16
	if (classatom && !GetWndProc16(hWnd16))
	{
		SetWndProc16(hWnd16, WNDCLASS16Info[classatom].wndproc);
	}
}


/***********************************************************************
 *		GetInternalWindowPos (USER.460)
 */
UINT16 WINAPI GetInternalWindowPos16( HWND16 hwnd, LPRECT16 rectWnd, LPPOINT16 ptIcon )
{
    WINDOWPLACEMENT16 wndpl;

    if (!GetWindowPlacement16( hwnd, &wndpl )) return 0;
    if (rectWnd) *rectWnd = wndpl.rcNormalPosition;
    if (ptIcon)  *ptIcon = wndpl.ptMinPosition;
    return wndpl.showCmd;
}


/**************************************************************************
 *              SetInternalWindowPos   (USER.461)
 */
void WINAPI SetInternalWindowPos16( HWND16 hwnd, UINT16 showCmd, LPRECT16 rect, LPPOINT16 pt )
{
    RECT rc32;
    POINT pt32;

    if (rect)
    {
        rc32.left   = rect->left;
        rc32.top    = rect->top;
        rc32.right  = rect->right;
        rc32.bottom = rect->bottom;
    }
    if (pt)
    {
        pt32.x = pt->x;
        pt32.y = pt->y;
    }
    SetInternalWindowPos( WIN_Handle32(hwnd), showCmd,
                          rect ? &rc32 : NULL, pt ? &pt32 : NULL );
}


/**************************************************************************
 *              CalcChildScroll   (USER.462)
 */
void WINAPI CalcChildScroll16( HWND16 hwnd, WORD scroll )
{
    CalcChildScroll( WIN_Handle32(hwnd), scroll );
}


/**************************************************************************
 *              ScrollChildren   (USER.463)
 */
void WINAPI ScrollChildren16(HWND16 hwnd, UINT16 uMsg, WPARAM16 wParam, LPARAM lParam)
{
    ScrollChildren( WIN_Handle32(hwnd), uMsg, wParam, lParam );
}


/**************************************************************************
 *              DragDetect   (USER.465)
 */
BOOL16 WINAPI DragDetect16( HWND16 hwnd, POINT16 pt )
{
    POINT pt32;

    pt32.x = pt.x;
    pt32.y = pt.y;
    return DragDetect( WIN_Handle32(hwnd), pt32 );
}


/**************************************************************************
 *              SetScrollInfo   (USER.475)
 */
INT16 WINAPI SetScrollInfo16( HWND16 hwnd, INT16 nBar, const SCROLLINFO *info, BOOL16 redraw )
{
    return SetScrollInfo( WIN_Handle32(hwnd), nBar, info, redraw );
}


/**************************************************************************
 *              GetScrollInfo   (USER.476)
 */
BOOL16 WINAPI GetScrollInfo16( HWND16 hwnd, INT16 nBar, LPSCROLLINFO info )
{
    return GetScrollInfo( WIN_Handle32(hwnd), nBar, info );
}


/**************************************************************************
 *              EnableScrollBar   (USER.482)
 */
BOOL16 WINAPI EnableScrollBar16( HWND16 hwnd, INT16 nBar, UINT16 flags )
{
    return EnableScrollBar( WIN_Handle32(hwnd), nBar, flags );
}


/**************************************************************************
 *              GetShellWindow   (USER.600)
 */
HWND16 WINAPI GetShellWindow16(void)
{
    return HWND_16( GetShellWindow() );
}


/**************************************************************************
 *              GetForegroundWindow   (USER.608)
 */
HWND16 WINAPI GetForegroundWindow16(void)
{
    return HWND_16( GetForegroundWindow() );
}


/**************************************************************************
 *              SetForegroundWindow   (USER.609)
 */
BOOL16 WINAPI SetForegroundWindow16( HWND16 hwnd )
{
    return SetForegroundWindow( WIN_Handle32(hwnd) );
}


/**************************************************************************
 *              DrawCaptionTemp   (USER.657)
 */
BOOL16 WINAPI DrawCaptionTemp16( HWND16 hwnd, HDC16 hdc, const RECT16 *rect,
                                 HFONT16 hFont, HICON16 hIcon, LPCSTR str, UINT16 uFlags )
{
    RECT rect32;

    if (rect)
    {
        rect32.left   = rect->left;
        rect32.top    = rect->top;
        rect32.right  = rect->right;
        rect32.bottom = rect->bottom;
    }
    return DrawCaptionTempA( WIN_Handle32(hwnd), HDC_32(hdc),
			     rect ? &rect32 : NULL, HFONT_32(hFont),
			     get_icon_32(hIcon), str, uFlags & 0x1f );
}


/**************************************************************************
 *              DrawCaption   (USER.660)
 */
BOOL16 WINAPI DrawCaption16( HWND16 hwnd, HDC16 hdc, const RECT16 *rect, UINT16 flags )
{
    RECT rect32;

    if (rect)
    {
        rect32.left   = rect->left;
        rect32.top    = rect->top;
        rect32.right  = rect->right;
        rect32.bottom = rect->bottom;
    }
    return DrawCaption(WIN_Handle32(hwnd), HDC_32(hdc), rect ? &rect32 : NULL, flags);
}


/**************************************************************************
 *              GetMenuItemRect   (USER.665)
 */
BOOL16 WINAPI GetMenuItemRect16( HWND16 hwnd, HMENU16 hMenu, UINT16 uItem,
                                 LPRECT16 rect)
{
     RECT r32;
     BOOL res;
     if (!rect) return FALSE;
     res = GetMenuItemRect( WIN_Handle32(hwnd), HMENU_32(hMenu), uItem, &r32 );
     rect->left   = r32.left;
     rect->top    = r32.top;
     rect->right  = r32.right;
     rect->bottom = r32.bottom;
     return res;
}


/**************************************************************************
 *              SetWindowRgn   (USER.668)
 */
INT16 WINAPI SetWindowRgn16( HWND16 hwnd, HRGN16 hrgn, BOOL16 redraw )
{
    return SetWindowRgn( WIN_Handle32(hwnd), HRGN_32(hrgn), redraw );
}


/**************************************************************************
 *              MessageBoxIndirect   (USER.827)
 */
INT16 WINAPI MessageBoxIndirect16( LPMSGBOXPARAMS16 msgbox )
{
    char caption[256], text[256];
    MSGBOXPARAMSA msgbox32;

    msgbox32.cbSize             = msgbox->cbSize;
    msgbox32.hwndOwner          = WIN_Handle32( msgbox->hwndOwner );
    msgbox32.hInstance          = 0;
    msgbox32.dwStyle            = msgbox->dwStyle;
    msgbox32.lpszIcon           = NULL;
    msgbox32.dwContextHelpId    = msgbox->dwContextHelpId;
    msgbox32.lpfnMsgBoxCallback = msgbox->lpfnMsgBoxCallback;
    msgbox32.dwLanguageId       = msgbox->dwLanguageId;

    if (!HIWORD(msgbox->lpszCaption))
    {
        LoadString16( msgbox->hInstance, LOWORD(msgbox->lpszCaption), caption, sizeof(caption) );
        msgbox32.lpszCaption = caption;
    }
    else msgbox32.lpszCaption = MapSL(msgbox->lpszCaption);

    if (!HIWORD(msgbox->lpszText))
    {
        LoadString16( msgbox->hInstance, LOWORD(msgbox->lpszText), text, sizeof(text) );
        msgbox32.lpszText = text;
    }
    else msgbox32.lpszText = MapSL(msgbox->lpszText);

    if ((msgbox->dwStyle & MB_ICONMASK) == MB_USERICON)
    {
        FIXME( "user icon %s not supported\n", debugstr_a( MapSL(msgbox->lpszIcon) ));
        msgbox32.dwStyle &= ~MB_USERICON;
    }

    return MessageBoxIndirectA( &msgbox32 );
}
