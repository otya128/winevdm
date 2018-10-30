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


static LRESULT CALLBACK call_WH_MSGFILTER( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_KEYBOARD( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_GETMESSAGE( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_CALLWNDPROC( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_CBT( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_MOUSE( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK call_WH_SHELL( INT code, WPARAM wp, LPARAM lp );

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


struct hook_entry
{
    struct list entry;
    SEGPTR proc16;
    HTASK16 htask16;
    HINSTANCE16 hinst16;
    INT type;
};

struct hook16_queue_info
{
    HHOOK       hhook[NB_HOOKS16];
    struct hook_entry *hook_entry[NB_HOOKS16];
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

static HHOOK entry_to_hhook(struct hook_entry *entry)
{
    return (HHOOK)entry;
}
static struct hook_entry *hhook_to_entry(HHOOK hhook)
{
    return (struct hook_entry *)hhook;
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


static LRESULT call_hook_entry_16(struct hook16_queue_info *info, struct hook_entry *hook_entry, INT id, INT code, WPARAM wp, LPARAM lp);
/***********************************************************************
 *           call_hook_16
 */
static LRESULT call_hook_16( INT id, INT code, WPARAM wp, LPARAM lp )
{
    struct hook16_queue_info *info = get_hook_info(FALSE);
    struct hook_entry *hook_entry = list_head(info->hook_entry[id - WH_MIN]);
    if (!hook_entry)
        return 0;
    LRESULT result = call_hook_entry_16(info, hook_entry, id, code, wp, lp);
    return result;
}
static LRESULT call_hook_entry_16( struct hook16_queue_info *info, struct hook_entry *hook_entry, INT id, INT code, WPARAM wp, LPARAM lp )
{
    WORD args[4];
    DWORD ret;
    CONTEXT context;

    args[3] = code;
    args[2] = wp;
    args[1] = HIWORD(lp);
    args[0] = LOWORD(lp);
    memset(&context, 0, sizeof(context));
    context.SegDs = context.SegEs = SELECTOROF(getWOW32Reserved());
    context.SegFs = wine_get_fs();
    context.SegGs = wine_get_gs();
    context.Eax = hook_entry->hinst16 | 1; /* Handle To Sel */
    if (!context.Eax) context.Eax = context.SegDs;
    context.SegCs = SELECTOROF((DWORD)hook_entry->proc16);
    context.Eip = OFFSETOF((DWORD)hook_entry->proc16);
    context.Ebp = OFFSETOF(getWOW32Reserved()) + FIELD_OFFSET(STACK16FRAME, bp);
    WOWCallback16Ex( (DWORD)hook_entry->proc16, WCB16_REGS | WCB16_PASCAL, sizeof(args), args, (LPDWORD)&context );

    ret = MAKELONG(LOWORD(context.Eax), LOWORD(context.Edx));

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
    ret = call_hook_16( WH_CALLWNDPROC, params->code, params->wparam, lp );
    UnMapLS( lp );

    *result = 0;
    return ret;
}

/* helper for SendMessage16 */
void call_WH_CALLWNDPROC_hook( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp )
{
    CWPSTRUCT16 cwp;
    struct hook16_queue_info *info = get_hook_info( FALSE );
    struct hook_entry *hook_entry, *next;

    if (!info || !info->hook_entry[WH_CALLWNDPROC - WH_MINHOOK]) return;

    cwp.hwnd    = hwnd;
    cwp.message = msg;
    cwp.wParam  = wp;
    cwp.lParam  = lp;

    lp = MapLS( &cwp );

    call_hook_16(WH_CALLWNDPROC, HC_ACTION, 1, lp);
    UnMapLS( lp );
}

/***********************************************************************
 *		call_WH_MSGFILTER
 */
static LRESULT CALLBACK call_WH_MSGFILTER( INT code, WPARAM wp, LPARAM lp )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    LRESULT ret;
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_MSGFILTER - WH_MIN], code, wp, lp);

    map_msg_32_to_16( msg32, &msg16 );
    lp = MapLS( &msg16 );
    ret = call_hook_16( WH_MSGFILTER, code, wp, lp );
    UnMapLS( lp );
    return ret;
}


/***********************************************************************
 *		call_WH_KEYBOARD
 */
static LRESULT CALLBACK call_WH_KEYBOARD( INT code, WPARAM wp, LPARAM lp )
{
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_KEYBOARD - WH_MIN], code, wp, lp);
    return call_hook_16( WH_KEYBOARD, code, wp, lp );
}


/***********************************************************************
 *		call_WH_GETMESSAGE
 */
static LRESULT CALLBACK call_WH_GETMESSAGE( INT code, WPARAM wp, LPARAM lp )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    LRESULT ret;
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_GETMESSAGE - WH_MIN], code, wp, lp);

    map_msg_32_to_16( msg32, &msg16 );

    lp = MapLS( &msg16 );
    ret = call_hook_16( WH_GETMESSAGE, code, wp, lp );
    UnMapLS( lp );

    map_msg_16_to_32( &msg16, msg32 );
    return ret;
}


/***********************************************************************
 *		call_WH_CALLWNDPROC
 */
static LRESULT CALLBACK call_WH_CALLWNDPROC( INT code, WPARAM wp, LPARAM lp )
{
    struct wndproc_hook_params params;
    CWPSTRUCT *cwp32 = (CWPSTRUCT *)lp;
    LRESULT result;
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_CALLWNDPROC - WH_MIN], code, wp, lp);

    params.code   = code;
    params.wparam = wp;
    return WINPROC_CallProc32ATo16( wndproc_hook_callback16, cwp32->hwnd, cwp32->message,
                                    cwp32->wParam, cwp32->lParam, &result, &params );
}


/***********************************************************************
 *		call_WH_CBT
 */
static LRESULT CALLBACK call_WH_CBT( INT code, WPARAM wp, LPARAM lp )
{
    LRESULT ret = 0;
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_CBT - WH_MIN], code, wp, lp);

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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp );
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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp );
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
            ret = call_hook_16( WH_CBT, code, wp, lp );
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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp );
            UnMapLS( lp );
            break;
    }
    case HCBT_MINMAX:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), lp);
        break;
    }
    case HCBT_QS:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_DESTROYWND:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), lp);
        break;
    }
    case HCBT_KEYSKIPPED:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_SYSCOMMAND:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp);
        break;
    }
    case HCBT_SETFOCUS:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), HWND_16(lp));
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
static LRESULT CALLBACK call_WH_MOUSE( INT code, WPARAM wp, LPARAM lp )
{
    MOUSEHOOKSTRUCT *ms32 = (MOUSEHOOKSTRUCT *)lp;
    MOUSEHOOKSTRUCT16 ms16;
    LRESULT ret;
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_MOUSE - WH_MIN], code, wp, lp);

    ms16.pt.x         = ms32->pt.x;
    ms16.pt.y         = ms32->pt.y;
    ms16.hwnd         = HWND_16( ms32->hwnd );
    ms16.wHitTestCode = ms32->wHitTestCode;
    ms16.dwExtraInfo  = ms32->dwExtraInfo;

    lp = MapLS( &ms16 );
    ret = call_hook_16( WH_MOUSE, code, wp, lp );
    UnMapLS( lp );
    return ret;
}


/***********************************************************************
 *		call_WH_SHELL
 */
static LRESULT CALLBACK call_WH_SHELL( INT code, WPARAM wp, LPARAM lp )
{
    CallNextHookEx(get_hook_info(FALSE)->hhook[WH_MOUSE - WH_SHELL], code, wp, lp);
    return call_hook_16( WH_SHELL, code, wp, lp );
}


// TODO: key delay and mouse messages
static void WINAPI journal_playback_cb( HWND hwnd, UINT msg, int id, DWORD sysTime )
{
    EVENTMSG16 emsg;
    LPARAM lp;
    INPUT input;
    lp = MapLS( &emsg );
    call_hook_16( WH_JOURNALPLAYBACK, HC_GETNEXT, 0, lp );
    call_hook_16( WH_JOURNALPLAYBACK, HC_SKIP, 0, 0 );
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
    struct hook_entry *entry;

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

    info = get_hook_info(TRUE);
    if (!info->hhook[index])
    {
        if (id == WH_JOURNALPLAYBACK)
        {
            info->hhook[index] = SetTimer(NULL, 0, 100, journal_playback_cb);
        }
        else
        {
            info->hhook[index] = SetWindowsHookExA(id, hook_procs[index], 0, thread);
        }
    }
    if (!info->hhook[index])
        return 0;
    if (info)
    {
        struct hook_entry *head = info->hook_entry[index];
        entry = (struct hook_entry*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct hook_entry));
        if (!head)
        {
            struct list* first = (struct list*)HeapAlloc(GetProcessHeap(), 0, sizeof(struct list));
            list_init(first);
            info->hook_entry[index] = first;
        }
        list_add_tail(&info->hook_entry[index]->entry, entry);
        entry->proc16 = proc;
        entry->hinst16 = hInst;
        entry->htask16 = hTask;
        entry->type = id;
    }

    return hhook_to_entry(entry);
}


/***********************************************************************
 *		UnhookWindowsHook (USER.234)
 */
BOOL16 WINAPI UnhookWindowsHook16( INT16 id, HOOKPROC16 proc )
{
    int index;
    struct hook16_queue_info *info;
    struct hook_entry *hook_entry, *next;

    if (id < WH_MINHOOK || id > WH_MAXHOOK16) return FALSE;
    info = get_hook_info(FALSE);
    LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, &info->hook_entry[id - WH_MINHOOK]->entry, struct hook_entry, entry)
    {
        if (hook_entry->proc16 == proc)
        {
            return UnhookWindowsHookEx16(entry_to_hhook(hook_entry));
        }
    }
    return FALSE;
}


/***********************************************************************
 *		UnhookWindowsHookEx (USER.292)
 */
BOOL16 WINAPI UnhookWindowsHookEx16(HHOOK hhook)
{
    BOOL result = FALSE;
    struct hook16_queue_info *info = get_hook_info(FALSE);
    struct hook_entry *unhook = hhook_to_entry(hhook);
    INT type = unhook->type;
    int index = type - WH_MINHOOK;
    if (info)
    {
        struct hook_entry *hook_entry, *next;

        LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, &info->hook_entry[index]->entry, struct hook_entry, entry)
        {
            if (hook_entry == unhook)
            {
                list_remove(&hook_entry->entry);
                HeapFree(GetProcessHeap(), 0, hook_entry);
                result = TRUE;
            }
        }
    }
    if (list_empty(&info->hook_entry[index]->entry))
    {
        if (type == WH_JOURNALPLAYBACK)
        {
            KillTimer(NULL, (UINT_PTR)info->hhook[index]);
        }
        else
        {
            UnhookWindowsHookEx(info->hhook[index]);
        }
    }
    return result;
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
    struct hook16_queue_info *info = get_hook_info(FALSE);
    struct hook_entry *entry = hhook_to_entry(hhook);
    LRESULT ret = 0;
    struct hook_entry *next_hook;
    if (!entry)
        return 0;
    next_hook = list_next(&info->hook_entry[entry->type - WH_MIN]->entry, entry);
    if (!next_hook)
        return 0;
    return call_hook_entry_16(info, next_hook, next_hook->type, code, wparam, lparam);
}


/***********************************************************************
 *		DefHookProc (USER.235)
 */
LRESULT WINAPI DefHookProc16( INT16 code, WPARAM16 wparam, LPARAM lparam, HHOOK *hhook )
{
    return CallNextHookEx16( *hhook, code, wparam, lparam );
}
