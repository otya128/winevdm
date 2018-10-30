/*
 * Windows 16-bit hook functions
 *
 * Copyright 1994, 1995, 2002 Alexandre Julliard
 * Copyright 1996 Andrew Lewycky
 *
 * Based on investigations by Alex Korobka
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

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winuser.h"
#include "wownt32.h"
#include "wine/winuser16.h"
#include "user_private.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(hook);


#include <pshpack1.h>
typedef struct
{
    BYTE pop_eax; /* 58 1 */
    BYTE push_imm; /* 68 2 */
    LPVOID param; /* 6 */
    BYTE push_eax; /* 50 7*/
    BYTE jmp; /* 0xEA 8 */
    LPVOID proc; /* 12 */
    WORD cs; /* 14 */
    WORD used; /* 16 */
} thunk_header;
#include <poppack.h>
typedef struct
{
    thunk_header thunk;
    ULONG_PTR extra_data[12]; /* 64 */
} thunk_info;
typedef struct
{
    thunk_header thunk;
    HHOOK hook;
    INT id;
    SEGPTR proc16;
    HTASK16 htask16;
    HINSTANCE16 hinst16;
    INT type;
} hook_thunk_info;


static LRESULT CALLBACK call_WH_MSGFILTER(hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_KEYBOARD( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_GETMESSAGE( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_CALLWNDPROC( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_CBT( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_MOUSE( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_SHELL( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp );

#define WH_MAXHOOK16 WH_SHELL  /* Win16 only supports up to WH_SHELL */
#define NB_HOOKS16 (WH_MAXHOOK16 - WH_MINHOOK + 1)

static const HOOKPROC hook_procs[NB_HOOKS16] =
{
    call_WH_MSGFILTER,   /* WH_MSGFILTER	*/
    NULL,                /* WH_JOURNALRECORD */
    (HOOKPROC)-1,        /* WH_JOURNALPLAYBACK */
    call_WH_KEYBOARD,    /* WH_KEYBOARD */
    call_WH_GETMESSAGE,  /* WH_GETMESSAGE */
    call_WH_CALLWNDPROC, /* WH_CALLWNDPROC */
    call_WH_CBT,         /* WH_CBT */
    NULL,                /* WH_SYSMSGFILTER */
    call_WH_MOUSE,       /* WH_MOUSE */
    NULL,                /* WH_HARDWARE */
    NULL,                /* WH_DEBUG */
    call_WH_SHELL        /* WH_SHELL */
};


static thunk_info *thunk_array;
static int max_thunk;
static void init_proc_thunk()
{
    if (thunk_array)
        return;
    max_thunk = 4096 / sizeof(thunk_info);
    thunk_array = VirtualAlloc(NULL, max_thunk * sizeof(thunk_info), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
}

static thunk_info *init_thunk_data(int i, LPVOID func)
{
    thunk_array[i].thunk.pop_eax = 0x58;
    thunk_array[i].thunk.push_imm = 0x68;
    thunk_array[i].thunk.param = thunk_array + i;
    thunk_array[i].thunk.push_eax = 0x50;
    thunk_array[i].thunk.jmp = 0xEA;
    thunk_array[i].thunk.proc = func;
    thunk_array[i].thunk.cs = wine_get_cs();
    thunk_array[i].thunk.used = TRUE;
    return thunk_array + i;
}
static thunk_header *allocate_proc_thunk(LPVOID func)
{
    init_proc_thunk();
    for (int i = 0; i < max_thunk; i++)
    {
        if (!thunk_array[i].thunk.used)
        {
            return (thunk_header*)init_thunk_data(i, func);
        }
    }
    ERR("could not allocate thunk!\n");
    return NULL;
}
static void delete_proc_thunk(thunk_header *thunk)
{
    thunk->used = FALSE;
}

struct hook16_queue_info
{
    INT         id;                /* id of current hook */
    hook_thunk_info *thunk[NB_HOOKS16];
};

static struct hook16_queue_info *get_hook_info( BOOL create )
{
    static DWORD hook_tls = TLS_OUT_OF_INDEXES;
    struct hook16_queue_info *info = TlsGetValue( hook_tls );

    if (!info && create)
    {
        if (hook_tls == TLS_OUT_OF_INDEXES) hook_tls = TlsAlloc();
        info = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*info) );
        TlsSetValue( hook_tls, info );
    }
    return info;
}


/***********************************************************************
 *           map_msg_16_to_32
 */
static inline void map_msg_16_to_32( const MSG16 *msg16, MSG *msg32 )
{
    msg32->hwnd    = WIN_Handle32(msg16->hwnd);
    msg32->message = msg16->message;
    msg32->wParam  = msg16->wParam;
    msg32->lParam  = msg16->lParam;
    msg32->time    = msg16->time;
    msg32->pt.x    = msg16->pt.x;
    msg32->pt.y    = msg16->pt.y;
}


/***********************************************************************
 *           map_msg_32_to_16
 */
static inline void map_msg_32_to_16( const MSG *msg32, MSG16 *msg16 )
{
    msg16->hwnd    = HWND_16(msg32->hwnd);
    msg16->message = msg32->message;
    msg16->wParam  = msg32->wParam;
    msg16->lParam  = msg32->lParam;
    msg16->time    = msg32->time;
    msg16->pt.x    = msg32->pt.x;
    msg16->pt.y    = msg32->pt.y;
}


/***********************************************************************
 *           call_hook_16
 */
static LRESULT call_hook_16( hook_thunk_info *thunk, INT id, INT code, WPARAM wp, LPARAM lp )
{
    WORD args[4];
    DWORD ret;
    INT prev_id = thunk->id;
    CONTEXT context;
    thunk->id = id;

    args[3] = code;
    args[2] = wp;
    args[1] = HIWORD(lp);
    args[0] = LOWORD(lp);
    memset(&context, 0, sizeof(context));
    context.SegDs = context.SegEs = SELECTOROF(getWOW32Reserved());
    context.SegFs = wine_get_fs();
    context.SegGs = wine_get_gs();
    context.Eax = thunk->hinst16 | 1; /* Handle To Sel */
    if (!context.Eax) context.Eax = context.SegDs;
    context.SegCs = SELECTOROF((DWORD)thunk->proc16);
    context.Eip = OFFSETOF((DWORD)thunk->proc16);
    context.Ebp = OFFSETOF(getWOW32Reserved()) + FIELD_OFFSET(STACK16FRAME, bp);
    WOWCallback16Ex( (DWORD)thunk->proc16, WCB16_REGS | WCB16_PASCAL, sizeof(args), args, (LPDWORD)&context );

    ret = MAKELONG(LOWORD(context.Eax), LOWORD(context.Edx));
    thunk->id = prev_id;

    /* Grrr. While the hook procedure is supposed to have an LRESULT return
       value even in Win16, it seems that for those hook types where the
       return value is interpreted as BOOL, Windows doesn't actually check
       the HIWORD ...  Some buggy Win16 programs, notably WINFILE, rely on
       that, because they neglect to clear DX ... */
    if (id != WH_JOURNALPLAYBACK) ret = LOWORD( ret );
    return ret;
}


struct wndproc_hook_params
{
    HHOOK  hhook;
    INT    code;
    WPARAM wparam;
    hook_thunk_info *thunk;
};

/* callback for WINPROC_Call16To32A */
static LRESULT wndproc_hook_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                      LRESULT *result, void *arg )
{
    struct wndproc_hook_params *params = arg;
    CWPSTRUCT cwp;

    cwp.hwnd    = hwnd;
    cwp.message = msg;
    cwp.wParam  = wp;
    cwp.lParam  = lp;
    *result = 0;
    return CallNextHookEx( params->hhook, params->code, params->wparam, (LPARAM)&cwp );
}

/* callback for WINPROC_Call32ATo16 */
static LRESULT wndproc_hook_callback16( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
                                        LRESULT *result, void *arg )
{
    struct wndproc_hook_params *params = (struct wndproc_hook_params*)arg;
    CWPSTRUCT16 cwp;
    LRESULT ret;

    cwp.hwnd    = hwnd;
    cwp.message = msg;
    cwp.wParam  = wp;
    cwp.lParam  = lp;

    lp = MapLS( &cwp );
    ret = call_hook_16( params->thunk, WH_CALLWNDPROC, params->code, params->wparam, lp );
    UnMapLS( lp );

    *result = 0;
    return ret;
}

/* helper for SendMessage16 */
void call_WH_CALLWNDPROC_hook( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp )
{
    CWPSTRUCT16 cwp;
    struct hook16_queue_info *info = get_hook_info( FALSE );

    if (!info || !info->thunk[WH_CALLWNDPROC - WH_MINHOOK]) return;

    cwp.hwnd    = hwnd;
    cwp.message = msg;
    cwp.wParam  = wp;
    cwp.lParam  = lp;

    lp = MapLS( &cwp );
    /* FIXME: It does not work when there are multiple hooks. */
    call_hook_16( info->thunk[WH_CALLWNDPROC - WH_MINHOOK], WH_CALLWNDPROC, HC_ACTION, 1, lp );
    UnMapLS( lp );
}

/***********************************************************************
 *		call_WH_MSGFILTER
 */
static LRESULT CALLBACK call_WH_MSGFILTER( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    LRESULT ret;

    map_msg_32_to_16( msg32, &msg16 );
    lp = MapLS( &msg16 );
    ret = call_hook_16( thunk, WH_MSGFILTER, code, wp, lp );
    UnMapLS( lp );
    return ret;
}


/***********************************************************************
 *		call_WH_KEYBOARD
 */
static LRESULT CALLBACK call_WH_KEYBOARD( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    return call_hook_16( thunk, WH_KEYBOARD, code, wp, lp );
}


/***********************************************************************
 *		call_WH_GETMESSAGE
 */
static LRESULT CALLBACK call_WH_GETMESSAGE( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    LRESULT ret;

    map_msg_32_to_16( msg32, &msg16 );

    lp = MapLS( &msg16 );
    ret = call_hook_16( thunk, WH_GETMESSAGE, code, wp, lp );
    UnMapLS( lp );

    map_msg_16_to_32( &msg16, msg32 );
    return ret;
}


/***********************************************************************
 *		call_WH_CALLWNDPROC
 */
static LRESULT CALLBACK call_WH_CALLWNDPROC( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    struct wndproc_hook_params params;
    CWPSTRUCT *cwp32 = (CWPSTRUCT *)lp;
    LRESULT result;

    params.code   = code;
    params.wparam = wp;
    params.thunk = thunk;
    return WINPROC_CallProc32ATo16( wndproc_hook_callback16, cwp32->hwnd, cwp32->message,
                                    cwp32->wParam, cwp32->lParam, &result, &params );
}


/***********************************************************************
 *		call_WH_CBT
 */
static LRESULT CALLBACK call_WH_CBT( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    LRESULT ret = 0;

    switch (code)
    {
    case HCBT_CREATEWND:
        {
            CBT_CREATEWNDA *cbtcw32 = (CBT_CREATEWNDA *)lp;
            CBT_CREATEWND16 cbtcw16;
            CREATESTRUCT16 cs16;

            cs16.lpCreateParams = (SEGPTR)cbtcw32->lpcs->lpCreateParams;
            cs16.hInstance      = HINSTANCE_16(cbtcw32->lpcs->hInstance);
            cs16.hMenu          = HMENU_16(cbtcw32->lpcs->hMenu);
            cs16.hwndParent     = HWND_16(cbtcw32->lpcs->hwndParent);
            cs16.cy             = cbtcw32->lpcs->cy;
            cs16.cx             = cbtcw32->lpcs->cx;
            cs16.y              = cbtcw32->lpcs->y;
            cs16.x              = cbtcw32->lpcs->x;
            cs16.style          = cbtcw32->lpcs->style;
            cs16.lpszName       = MapLS( cbtcw32->lpcs->lpszName );
            cs16.lpszClass      = MapLS( win16classname(cbtcw32->lpcs->lpszClass) );
            cs16.dwExStyle      = cbtcw32->lpcs->dwExStyle;

            cbtcw16.lpcs = (CREATESTRUCT16 *)MapLS( &cs16 );
            cbtcw16.hwndInsertAfter = HWND_16( cbtcw32->hwndInsertAfter );

            lp = MapLS( &cbtcw16 );
            ret = call_hook_16( thunk, WH_CBT, code, HWND_16(wp), lp );
            UnMapLS( cs16.lpszName );
            UnMapLS( cs16.lpszClass );

            cbtcw32->hwndInsertAfter = WIN_Handle32( cbtcw16.hwndInsertAfter );
            UnMapLS( (SEGPTR)cbtcw16.lpcs );
            UnMapLS( lp );
            break;
        }

    case HCBT_ACTIVATE:
        {
            CBTACTIVATESTRUCT *cas32 = (CBTACTIVATESTRUCT *)lp;
            CBTACTIVATESTRUCT16 cas16;

            cas16.fMouse     = cas32->fMouse;
            cas16.hWndActive = HWND_16( cas32->hWndActive );

            lp = MapLS( &cas16 );
            ret = call_hook_16( thunk, WH_CBT, code, HWND_16(wp), lp );
            UnMapLS( lp );
            break;
        }
    case HCBT_CLICKSKIPPED:
        {
            MOUSEHOOKSTRUCT *ms32 = (MOUSEHOOKSTRUCT *)lp;
            MOUSEHOOKSTRUCT16 ms16;

            ms16.pt.x         = ms32->pt.x;
            ms16.pt.y         = ms32->pt.y;
            ms16.hwnd         = HWND_16( ms32->hwnd );
            ms16.wHitTestCode = ms32->wHitTestCode;
            ms16.dwExtraInfo  = ms32->dwExtraInfo;

            lp = MapLS( &ms16 );
            ret = call_hook_16( thunk, WH_CBT, code, wp, lp );
            UnMapLS( lp );
            break;
        }
    case HCBT_MOVESIZE:
        {
            RECT *rect32 = (RECT *)lp;
            RECT16 rect16;

            rect16.left   = rect32->left;
            rect16.top    = rect32->top;
            rect16.right  = rect32->right;
            rect16.bottom = rect32->bottom;
            lp = MapLS( &rect16 );
            ret = call_hook_16( thunk, WH_CBT, code, HWND_16(wp), lp );
            UnMapLS( lp );
            break;
    }
    case HCBT_MINMAX:
    {
        ret = call_hook_16(thunk, WH_CBT, code, HWND_16(wp), lp);
        break;
    }
    case HCBT_QS:
    {
        ret = call_hook_16(thunk, WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_DESTROYWND:
    {
        ret = call_hook_16(thunk, WH_CBT, code, HWND_16(wp), lp);
        break;
    }
    case HCBT_KEYSKIPPED:
    {
        ret = call_hook_16(thunk, WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_SYSCOMMAND:
    {
        ret = call_hook_16(thunk, WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_SETFOCUS:
    {
        ret = call_hook_16(thunk, WH_CBT, code, HWND_16(wp), HWND_16(lp));
        break;
    }
    default:
        ERR("unknown wndproc hook code:%04X, wParam=%p, lParam=%p\n", code, wp, lp);
    }
    return ret;
}


/***********************************************************************
 *		call_WH_MOUSE
 */
static LRESULT CALLBACK call_WH_MOUSE( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    MOUSEHOOKSTRUCT *ms32 = (MOUSEHOOKSTRUCT *)lp;
    MOUSEHOOKSTRUCT16 ms16;
    LRESULT ret;

    ms16.pt.x         = ms32->pt.x;
    ms16.pt.y         = ms32->pt.y;
    ms16.hwnd         = HWND_16( ms32->hwnd );
    ms16.wHitTestCode = ms32->wHitTestCode;
    ms16.dwExtraInfo  = ms32->dwExtraInfo;

    lp = MapLS( &ms16 );
    ret = call_hook_16( thunk, WH_MOUSE, code, wp, lp );
    UnMapLS( lp );
    return ret;
}


/***********************************************************************
 *		call_WH_SHELL
 */
static LRESULT CALLBACK call_WH_SHELL( hook_thunk_info *thunk, INT code, WPARAM wp, LPARAM lp )
{
    return call_hook_16( thunk, WH_SHELL, code, wp, lp );
}


// TODO: key delay and mouse messages
static void WINAPI journal_playback_cb( hook_thunk_info *thunk, HWND hwnd, UINT msg, int id, DWORD sysTime )
{
    EVENTMSG16 emsg;
    LPARAM lp;
    INPUT input;
    lp = MapLS( &emsg );
    call_hook_16( thunk, WH_JOURNALPLAYBACK, HC_GETNEXT, 0, lp );
    call_hook_16( thunk, WH_JOURNALPLAYBACK, HC_SKIP, 0, 0 );
    UnMapLS( &msg );
    TRACE("WH_JOURNALPLAYBACK message: %x paramL: %x paramH: %x\n", emsg.message, emsg.paramL, emsg.paramH);
    switch( emsg.message )
       {
            case WM_KEYDOWN:
                input.type = 1;
                input.ki.wVk = emsg.paramL;
                input.ki.wScan = emsg.paramH;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                SendInput( 1, &input, sizeof(input) );
                break;
            case WM_KEYUP:
                input.type = 1;
                input.ki.wVk = emsg.paramL;
                input.ki.wScan = emsg.paramH;
                input.ki.dwFlags = 2;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                SendInput( 1, &input, sizeof(input) );
                break;
        }
}


/***********************************************************************
 *		SetWindowsHook (USER.121)
 */
FARPROC16 WINAPI SetWindowsHook16( INT16 id, HOOKPROC16 proc )
{
    HINSTANCE16 hInst = FarGetOwner16( HIWORD(proc) );

    /* WH_MSGFILTER is the only task-specific hook for SetWindowsHook() */
    HTASK16 hTask = (id == WH_MSGFILTER) ? GetCurrentTask() : 0;

    return (FARPROC16)SetWindowsHookEx16( id, proc, hInst, hTask );
}


/***********************************************************************
 *		SetWindowsHookEx (USER.291)
 */
HHOOK WINAPI SetWindowsHookEx16(INT16 id, HOOKPROC16 proc, HINSTANCE16 hInst, HTASK16 hTask)
{
    struct hook16_queue_info *info;
    HHOOK hook;
    int index = id - WH_MINHOOK;
    DWORD thread = GetCurrentThreadId();
    hook_thunk_info *thunk;

    if (id < WH_MINHOOK || id > WH_MAXHOOK16) return 0;
    if (!hook_procs[index])
    {
        FIXME("hook type %d broken in Win16\n", id);
        return 0;
    }
    if (!hTask) FIXME("System-global hooks (%d) broken in Win16\n", id);
    else if (hTask != GetCurrentTask())
    {
        thread = GetThreadId((HANDLE)HTASK_32(hTask));
        if (!thread)
        {
            ERR("invalid task? %04x\n", hTask);
            return 0;
        }
    }

    thunk = allocate_proc_thunk(hook_procs[index]);
    thunk->type = id;
    if (id == WH_JOURNALPLAYBACK)
    {
        if (!(hook = (HHOOK)SetTimer(NULL, 0, 100, journal_playback_cb)))
        {
            delete_proc_thunk(thunk);
            return 0;
        }
    }
    else if (!(hook = SetWindowsHookExA(id, (HOOKPROC)thunk, 0, thread)))
    {
        delete_proc_thunk(thunk);
        return 0;
    }
    if (info)
        info = get_hook_info(TRUE);
    info->thunk[index] = thunk;
    thunk->hook = hook;
    thunk->proc16 = proc;
    thunk->hinst16 = hInst;
    thunk->htask16 = hTask;
    return hook;
}


hook_thunk_info *find_hook_thunk_info(HHOOK hhook)
{
    int index;
    if (!thunk_array)
        return NULL;
    for (index = 0; index < max_thunk; index++)
    {
        hook_thunk_info *info = (hook_thunk_info*)(thunk_array + index);
        if (info->thunk.used && info->hook == hhook)
        {
            return info;
        }
    }
    return NULL;
}
/***********************************************************************
 *		UnhookWindowsHook (USER.234)
 */
BOOL16 WINAPI UnhookWindowsHook16( INT16 id, HOOKPROC16 proc )
{
    int index;

    if (!thunk_array)
        return FALSE;
    if (id < WH_MINHOOK || id > WH_MAXHOOK16) return FALSE;
    for (index = 0; index < max_thunk; index++)
    {
        hook_thunk_info *info = (hook_thunk_info*)(thunk_array + index);
        if (info->thunk.used && info->proc16 == proc)
        {
            return UnhookWindowsHookEx16(info->hook);
        }
    }
    return TRUE;
}


/***********************************************************************
 *		UnhookWindowsHookEx (USER.292)
 */
BOOL16 WINAPI UnhookWindowsHookEx16(HHOOK hhook)
{
    int index;
    BOOL result;
    struct hook16_queue_info *info;
    hook_thunk_info *thunk = find_hook_thunk_info(hhook);
    if (!thunk)
        return FALSE;
    if (thunk->type == WH_JOURNALPLAYBACK)
        result = KillTimer(NULL, (UINT_PTR)hhook);
    else
        result = UnhookWindowsHookEx(hhook);
    if (!result)
        return FALSE;
    info = get_hook_info(FALSE);
    if (info)
    {
        if (info->thunk[thunk->type] == thunk)
        {
            info->thunk[thunk->type] = NULL;
        }
    }
    thunk->hook = 0;
    thunk->proc16 = 0;
    delete_proc_thunk((thunk_header*)thunk);
    return (BOOL16)result;
}


/***********************************************************************
 *		CallMsgFilter32 (USER.823)
 */
BOOL16 WINAPI CallMsgFilter32_16( MSG32_16 *lpmsg16_32, INT16 code, BOOL16 wHaveParamHigh )
{
    MSG msg32;
    BOOL16 ret;

    if (GetSysModalWindow16()) return FALSE;
    msg32.hwnd      = WIN_Handle32( lpmsg16_32->msg.hwnd );
    msg32.message   = lpmsg16_32->msg.message;
    msg32.lParam    = lpmsg16_32->msg.lParam;
    msg32.time      = lpmsg16_32->msg.time;
    msg32.pt.x      = lpmsg16_32->msg.pt.x;
    msg32.pt.y      = lpmsg16_32->msg.pt.y;
    if (wHaveParamHigh) msg32.wParam = MAKELONG(lpmsg16_32->msg.wParam, lpmsg16_32->wParamHigh);
    else msg32.wParam = lpmsg16_32->msg.wParam;

    ret = (BOOL16)CallMsgFilterA(&msg32, code);

    lpmsg16_32->msg.hwnd    = HWND_16( msg32.hwnd );
    lpmsg16_32->msg.message = msg32.message;
    lpmsg16_32->msg.wParam  = LOWORD(msg32.wParam);
    lpmsg16_32->msg.lParam  = msg32.lParam;
    lpmsg16_32->msg.time    = msg32.time;
    lpmsg16_32->msg.pt.x    = msg32.pt.x;
    lpmsg16_32->msg.pt.y    = msg32.pt.y;
    if (wHaveParamHigh) lpmsg16_32->wParamHigh = HIWORD(msg32.wParam);
    return ret;
}


/***********************************************************************
 *		CallMsgFilter (USER.123)
 */
BOOL16 WINAPI CallMsgFilter16( MSG16 *msg, INT16 code )
{
    return CallMsgFilter32_16( (MSG32_16 *)msg, code, FALSE );
}


/***********************************************************************
 *		CallNextHookEx (USER.293)
 */
LRESULT WINAPI CallNextHookEx16( HHOOK hhook, INT16 code, WPARAM16 wparam, LPARAM lparam )
{
    hook_thunk_info *info;
    LRESULT ret = 0;
    /* FIXME: Use current hook? */
    if (!(info = find_hook_thunk_info(hhook)))
        return 0;

    switch (info->id)
    {
    case WH_MSGFILTER:
    {
        MSG16 *msg16 = MapSL(lparam);
        MSG msg32;

        map_msg_16_to_32( msg16, &msg32 );
        ret = CallNextHookEx( hhook, code, wparam, (LPARAM)&msg32 );
        break;
    }

    case WH_GETMESSAGE:
    {
        MSG16 *msg16 = MapSL(lparam);
        MSG msg32;

        map_msg_16_to_32( msg16, &msg32 );
        ret = CallNextHookEx( hhook, code, wparam, (LPARAM)&msg32 );
        map_msg_32_to_16( &msg32, msg16 );
        break;
    }

    case WH_CALLWNDPROC:
    {
        CWPSTRUCT16 *cwp16 = MapSL(lparam);
        LRESULT result;
        struct wndproc_hook_params params;

        params.hhook  = hhook;
        params.code   = code;
        params.wparam = wparam;
        ret = WINPROC_CallProc16To32A( wndproc_hook_callback, cwp16->hwnd, cwp16->message,
                                       cwp16->wParam, cwp16->lParam, &result, &params );
        break;
    }

    case WH_CBT:
        switch (code)
        {
        case HCBT_CREATEWND:
            {
                CBT_CREATEWNDA cbtcw32;
                CREATESTRUCTA cs32;
                CBT_CREATEWND16 *cbtcw16 = MapSL(lparam);
                CREATESTRUCT16 *cs16 = MapSL( (SEGPTR)cbtcw16->lpcs );

                cbtcw32.lpcs = &cs32;
                cbtcw32.hwndInsertAfter = WIN_Handle32( cbtcw16->hwndInsertAfter );

                cs32.lpCreateParams = (LPVOID)cs16->lpCreateParams;
                cs32.hInstance      = HINSTANCE_32(cs16->hInstance);
                cs32.hMenu          = HMENU_32(cs16->hMenu);
                cs32.hwndParent     = WIN_Handle32(cs16->hwndParent);
                cs32.cy             = cs16->cy;
                cs32.cx             = cs16->cx;
                cs32.y              = cs16->y;
                cs32.x              = cs16->x;
                cs32.style          = cs16->style;
                cs32.lpszName       = MapSL( cs16->lpszName );
                cs32.lpszClass      = win32classname(cs16->hInstance, MapSL( cs16->lpszClass) );
                cs32.dwExStyle      = cs16->dwExStyle;

                ret = CallNextHookEx( hhook, code, HWND_32(wparam), (LPARAM)&cbtcw32 );
                cbtcw16->hwndInsertAfter = HWND_16( cbtcw32.hwndInsertAfter );
                break;
            }
        case HCBT_ACTIVATE:
            {
                CBTACTIVATESTRUCT16 *cas16 = MapSL(lparam);
                CBTACTIVATESTRUCT cas32;
                cas32.fMouse = cas16->fMouse;
                cas32.hWndActive = WIN_Handle32(cas16->hWndActive);
                ret = CallNextHookEx( hhook, code, HWND_32(wparam), (LPARAM)&cas32 );
                break;
            }
        case HCBT_CLICKSKIPPED:
            {
                MOUSEHOOKSTRUCT16 *ms16 = MapSL(lparam);
                MOUSEHOOKSTRUCT ms32;

                ms32.pt.x = ms16->pt.x;
                ms32.pt.y = ms16->pt.y;
                /* wHitTestCode may be negative, so convince compiler to do
                   correct sign extension. Yay. :| */
                ms32.wHitTestCode = (INT)(INT16)ms16->wHitTestCode;
                ms32.dwExtraInfo = ms16->dwExtraInfo;
                ms32.hwnd = WIN_Handle32( ms16->hwnd );
                ret = CallNextHookEx( hhook, code, wparam, (LPARAM)&ms32 );
                break;
            }
        case HCBT_MOVESIZE:
            {
                RECT16 *rect16 = MapSL(lparam);
                RECT rect32;

                rect32.left   = rect16->left;
                rect32.top    = rect16->top;
                rect32.right  = rect16->right;
                rect32.bottom = rect16->bottom;
                ret = CallNextHookEx( hhook, code, HWND_32(wparam), (LPARAM)&rect32 );
                break;
            }
        }
        break;

    case WH_MOUSE:
    {
        MOUSEHOOKSTRUCT16 *ms16 = MapSL(lparam);
        MOUSEHOOKSTRUCT ms32;

        ms32.pt.x = ms16->pt.x;
        ms32.pt.y = ms16->pt.y;
        /* wHitTestCode may be negative, so convince compiler to do
           correct sign extension. Yay. :| */
        ms32.wHitTestCode = (INT)((INT16)ms16->wHitTestCode);
        ms32.dwExtraInfo = ms16->dwExtraInfo;
        ms32.hwnd = WIN_Handle32(ms16->hwnd);
        ret = CallNextHookEx( hhook, code, wparam, (LPARAM)&ms32 );
        break;
    }

    case WH_SHELL:
    case WH_KEYBOARD:
        ret = CallNextHookEx( hhook, code, wparam, lparam );
        break;

    case WH_HARDWARE:
    case WH_FOREGROUNDIDLE:
    case WH_CALLWNDPROCRET:
    case WH_SYSMSGFILTER:
    case WH_JOURNALRECORD:
    case WH_JOURNALPLAYBACK:
    default:
        FIXME("\t[%i] 16to32 translation unimplemented\n", info->id);
        ret = CallNextHookEx( hhook, code, wparam, lparam );
        break;
    }
    return ret;
}


/***********************************************************************
 *		DefHookProc (USER.235)
 */
LRESULT WINAPI DefHookProc16( INT16 code, WPARAM16 wparam, LPARAM lparam, HHOOK *hhook )
{
    return CallNextHookEx16( *hhook, code, wparam, lparam );
}
