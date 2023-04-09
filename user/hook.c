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
#undef _WINTERNL_
#include "winternl.h"

WINE_DEFAULT_DEBUG_CHANNEL(hook);


static BOOL WINAPI TebTlsSetValue(TEB *teb, DWORD index, LPVOID value)
{
    if (index < TLS_MINIMUM_AVAILABLE)
    {
        teb->TlsSlots[index] = value;
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * 32 /* 8 * sizeof(teb->Peb->TlsExpansionBitmapBits) */)
        {
            return FALSE;
        }
        ((PVOID*)teb->TlsExpansionSlots)[index] = value;
    }
    return TRUE;
}

static LPVOID WINAPI TebTlsGetValue(TEB *teb, DWORD index)
{
    LPVOID ret;

    if (index < TLS_MINIMUM_AVAILABLE)
    {
        ret = teb->TlsSlots[index];
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * 32 /* 8 * sizeof(teb->Peb->TlsExpansionBitmapBits) */)
        {
            return NULL;
        }
        if (!teb->TlsExpansionSlots) ret = NULL;
        else ret = ((PVOID*)teb->TlsExpansionSlots)[index];
    }
    return ret;
}

static LRESULT CALLBACK task_call_WH_MSGFILTER( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_KEYBOARD( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_GETMESSAGE( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_CALLWNDPROC( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_CBT( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_SYSMSGFILTER( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_MOUSE( INT code, WPARAM wp, LPARAM lp );
static LRESULT CALLBACK task_call_WH_SHELL( INT code, WPARAM wp, LPARAM lp );

static LRESULT CALLBACK global_call_WH_MSGFILTER(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_KEYBOARD(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_GETMESSAGE(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_CALLWNDPROC(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_CBT(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_SYSMSGFILTER(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_MOUSE(INT code, WPARAM wp, LPARAM lp);
static LRESULT CALLBACK global_call_WH_SHELL(INT code, WPARAM wp, LPARAM lp);

#define WH_MAXHOOK16 WH_SHELL  /* Win16 only supports up to WH_SHELL */
#define NB_HOOKS16 (WH_MAXHOOK16 - WH_MINHOOK + 1)

static const HOOKPROC hook_procs[NB_HOOKS16] =
{
    task_call_WH_MSGFILTER,   /* WH_MSGFILTER	*/
    NULL,                /* WH_JOURNALRECORD */
    (HOOKPROC)-1,        /* WH_JOURNALPLAYBACK */
    task_call_WH_KEYBOARD,    /* WH_KEYBOARD */
    task_call_WH_GETMESSAGE,  /* WH_GETMESSAGE */
    task_call_WH_CALLWNDPROC, /* WH_CALLWNDPROC */
    task_call_WH_CBT,         /* WH_CBT */
    task_call_WH_SYSMSGFILTER, /* WH_SYSMSGFILTER */
    task_call_WH_MOUSE,       /* WH_MOUSE */
    NULL,                /* WH_HARDWARE */
    NULL,                /* WH_DEBUG */
    task_call_WH_SHELL        /* WH_SHELL */
};


static const HOOKPROC global_hook_procs[NB_HOOKS16] =
{
    global_call_WH_MSGFILTER,   /* WH_MSGFILTER	*/
    NULL,                /* WH_JOURNALRECORD */
    (HOOKPROC) - 1,        /* WH_JOURNALPLAYBACK */
    global_call_WH_KEYBOARD,    /* WH_KEYBOARD */
    global_call_WH_GETMESSAGE,  /* WH_GETMESSAGE */
    global_call_WH_CALLWNDPROC, /* WH_CALLWNDPROC */
    global_call_WH_CBT,         /* WH_CBT */
    global_call_WH_SYSMSGFILTER, /* WH_SYSMSGFILTER */
    global_call_WH_MOUSE,       /* WH_MOUSE */
    NULL,                /* WH_HARDWARE */
    NULL,                /* WH_DEBUG */
    global_call_WH_SHELL        /* WH_SHELL */
};

struct hook_entry
{
    struct list entry;
    SEGPTR proc16;
    HTASK16 htask16;
    HINSTANCE16 hinst16;
    INT type;
    BOOL global;
    BOOL deleted;
};

extern HQUEUE16 hqFirst;
static CRITICAL_SECTION global_hook_section;
static CRITICAL_SECTION_DEBUG global_hook_section_debug =
{
    0, 0, &global_hook_section,
    { &global_hook_section_debug.ProcessLocksList, &global_hook_section_debug.ProcessLocksList },
      0, 0, { (DWORD_PTR)(__FILE__ ": global hook") }
};
static CRITICAL_SECTION global_hook_section = { &global_hook_section_debug, -1, 0, 0, 0, 0 };
struct list global_hook_entry[NB_HOOKS16] =
{
    LIST_INIT(global_hook_entry[0]),
    LIST_INIT(global_hook_entry[1]),
    LIST_INIT(global_hook_entry[2]),
    LIST_INIT(global_hook_entry[3]),
    LIST_INIT(global_hook_entry[4]),
    LIST_INIT(global_hook_entry[5]),
    LIST_INIT(global_hook_entry[6]),
    LIST_INIT(global_hook_entry[7]),
    LIST_INIT(global_hook_entry[8]),
    LIST_INIT(global_hook_entry[9]),
    LIST_INIT(global_hook_entry[10]),
    LIST_INIT(global_hook_entry[11]),
};
struct hook16_queue_info
{
    HHOOK       hhook[NB_HOOKS16];
    struct list hook_entry[NB_HOOKS16];
    HHOOK       global_hhook[NB_HOOKS16];
};

static struct hook16_queue_info *get_hook_info( BOOL create, HTASK16 hTask )
{
    static DWORD hook_tls = TLS_OUT_OF_INDEXES;
    struct hook16_queue_info *info;
    TDB *tdb;
    if (!hTask /* current task */)
    {
        info = (struct hook16_queue_info*)TlsGetValue( hook_tls );
    }
    else
    {
        tdb = GlobalLock16(hTask);
        info = (struct hook16_queue_info*)TebTlsGetValue( tdb->teb, hook_tls );
    }
    if (!info && create)
    {
        if (hook_tls == TLS_OUT_OF_INDEXES) hook_tls = TlsAlloc();
        info = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*info) );
        if (!hTask /* current task */)
        {
            TlsSetValue( hook_tls, info );
        }
        else
        {
            TebTlsSetValue( tdb->teb, hook_tls, info );
        }
    }
    return info;
}


#define HHOOK_HIGHWORD MAKEWORD('H', 'K')
#define HOOK_MAX 200
/* free list */
struct hook_entry *hook_free;
struct list hook_free_list = LIST_INIT(hook_free_list);

static void init_hook()
{
    if (hook_free)
        return;
    hook_free = HeapAlloc(GetProcessHeap(), 0, HOOK_MAX * sizeof(struct hook_entry));
    for (int i = 1; i < HOOK_MAX; i++)
    {
        list_add_tail(&hook_free_list, &hook_free[i].entry);
        hook_free[i].deleted = TRUE;
    }
}

static struct hook_entry *allocate_hook()
{
    struct list *alloc;
    struct hook_entry *entry;
    init_hook();
    alloc = list_head(&hook_free_list);
    if (!alloc)
    {
        ERR("could not allocate HHOOK\n");
        return NULL;
    }
    list_remove(alloc);
    entry = LIST_ENTRY(alloc, struct hook_entry, entry);
    entry->deleted = FALSE;
    return entry;
}

static void free_hook(struct hook_entry *entry)
{
    entry->deleted = TRUE;
    list_add_tail(&hook_free_list, &entry->entry);
}

/*
 * HHOOK (~Windows 3.0)
 * https://devblogs.microsoft.com/oldnewthing/20060809-18/?p=30183
 * https://devblogs.microsoft.com/oldnewthing/20060810-06/?p=30173
 * HHOOK (Windows 3.1~)
 * HIWORD: 'HK'
 * LOWORD: near pointer?
 * LOWORD: index (NTVDM)
 */
static HHOOK entry_to_hhook(struct hook_entry *entry)
{
    if (entry->deleted)
        return NULL;
    if (hook_free <= entry && hook_free + HOOK_MAX >= entry)
        return (HHOOK)MAKELONG((WORD)(entry - hook_free), HHOOK_HIGHWORD);
    return NULL;
}
static struct hook_entry *hhook_to_entry(HHOOK hhook)
{
    struct hook_entry *entry = (struct hook_entry*)(LOWORD(hhook) + hook_free);
    if (HIWORD(hhook) != HHOOK_HIGHWORD)
        return NULL;
    if (entry->deleted)
        return NULL;
    return entry;
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
static LRESULT call_hook_16( INT id, INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    struct hook16_queue_info *info = get_hook_info(FALSE, 0);
    struct list *head = list_head(&(global ? global_hook_entry : info->hook_entry)[id - WH_MIN]);
    if (!head)
        return 0;
    struct hook_entry *hook_entry = LIST_ENTRY(head, struct hook_entry, entry);
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
    context.Eax = context.SegDs;
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
    BOOL global;
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
    ret = call_hook_16( WH_CALLWNDPROC, params->code, params->wparam, lp, params->global );
    UnMapLS( lp );

    *result = 0;
    return ret;
}

struct msgfilter_hook_params
{
    HHOOK  hhook;
    INT    code;
    DWORD  time;
    POINT  pt;
    WPARAM wparam;
    BOOL   global;
    INT    type;
};

static LRESULT msgfilter_hook_callback16( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
                                        LRESULT *result, void *arg )
{
    struct msgfilter_hook_params *params = (struct msgfilter_hook_params*)arg;
    MSG16 msg16;
    LRESULT ret;

    msg16.hwnd    = hwnd;
    msg16.message = msg;
    msg16.wParam  = wp;
    msg16.lParam  = lp;
    msg16.time    = params->time;
    msg16.pt.x    = params->pt.x;
    msg16.pt.y    = params->pt.y;

    lp = MapLS( &msg16 );
    ret = call_hook_16( params->type, params->code, params->wparam, lp, params->global );
    UnMapLS( lp );

    *result = 0;
    return ret;
}

/* helper for SendMessage16 */
void call_WH_CALLWNDPROC_hook( HWND16 hwnd, UINT16 *msg, WPARAM16 *wp, LPARAM *lp )
{
    CWPSTRUCT16 cwp;
    SEGPTR lpcwp = 0;
    struct hook16_queue_info *info = get_hook_info( FALSE, 0 );

    if (!info || (!info->hhook[WH_CALLWNDPROC - WH_MINHOOK] && !info->global_hhook[WH_CALLWNDPROC - WH_MINHOOK])) return;

    cwp.hwnd    = hwnd;
    cwp.message = *msg;
    cwp.wParam  = *wp;
    cwp.lParam  = *lp;

    lpcwp = MapLS( &cwp );

    if (info->global_hhook[WH_CALLWNDPROC - WH_MINHOOK])
        call_hook_16(WH_CALLWNDPROC, HC_ACTION, 1, lpcwp, TRUE);
    if (info->hhook[WH_CALLWNDPROC - WH_MINHOOK])
        call_hook_16(WH_CALLWNDPROC, HC_ACTION, 1, lpcwp, FALSE);
    UnMapLS( lpcwp );
    *msg = cwp.message;
    *wp = cwp.wParam;
    *lp = cwp.lParam;
}

HHOOK get_hhook(INT code, BOOL global)
{
    if (global)
    {
        return get_hook_info(FALSE, 0)->global_hhook[code - WH_MIN];
    }
    return get_hook_info(FALSE, 0)->hhook[code - WH_MIN];
}

/***********************************************************************
 *		call_WH_MSGFILTER
 */
static LRESULT CALLBACK call_WH_MSGFILTER( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    MSG16 *thunk;
    LRESULT ret;
    CallNextHookEx(get_hhook(WH_MSGFILTER, global), code, wp, lp);

    if (thunk = (MSG16 *)GetPropA(msg32->hwnd, "dialogmsgthunk"))
        ret = call_hook_16( WH_MSGFILTER, code, wp, thunk, global);
    else
    {
        struct msgfilter_hook_params params;
        MSG *msg = (MSG *)lp;
        LRESULT result;

        params.time   = msg->time;
        params.pt.x   = msg->pt.x;
        params.pt.y   = msg->pt.y;
        params.code   = code;
        params.wparam = wp;
        params.global = global;
        params.type   = WH_MSGFILTER; 
        return WINPROC_CallProc32ATo16( msgfilter_hook_callback16, msg->hwnd, msg->message,
                                    msg->wParam, msg->lParam, &result, &params );
    }
    return ret;
}

static LRESULT CALLBACK task_call_WH_MSGFILTER(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_MSGFILTER(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_MSGFILTER(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_MSGFILTER(code, wp, lp, TRUE);
}


/***********************************************************************
 *		call_WH_KEYBOARD
 */
static LRESULT CALLBACK call_WH_KEYBOARD( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    CallNextHookEx(get_hhook(WH_KEYBOARD, global), code, wp, lp);
    return call_hook_16( WH_KEYBOARD, code, wp, lp, global );
}


static LRESULT CALLBACK task_call_WH_KEYBOARD(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_KEYBOARD(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_KEYBOARD(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_KEYBOARD(code, wp, lp, TRUE);
}

/***********************************************************************
 *		call_WH_GETMESSAGE
 */
static LRESULT CALLBACK call_WH_GETMESSAGE( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    struct msgfilter_hook_params params;
    MSG *msg = (MSG *)lp;
    LRESULT result;
    CallNextHookEx(get_hhook(WH_GETMESSAGE, global), code, wp, lp);

    if (global && !msg->hwnd) return 1; // XXX: a thread message can cause the hook to be called in the wrong context

    params.time   = msg->time;
    params.pt.x   = msg->pt.x;
    params.pt.y   = msg->pt.y;
    params.code   = code;
    params.wparam = wp;
    params.global = global;
    params.type   = WH_GETMESSAGE; 
    return WINPROC_CallProc32ATo16( msgfilter_hook_callback16, msg->hwnd, msg->message,
                    msg->wParam, msg->lParam, &result, &params );
}


static LRESULT CALLBACK task_call_WH_GETMESSAGE(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_GETMESSAGE(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_GETMESSAGE(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_GETMESSAGE(code, wp, lp, TRUE);
}

/***********************************************************************
 *		call_WH_CALLWNDPROC
 */
static LRESULT CALLBACK call_WH_CALLWNDPROC( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    struct wndproc_hook_params params;
    CWPSTRUCT *cwp32 = (CWPSTRUCT *)lp;
    LRESULT result;
    CallNextHookEx(get_hhook(WH_CALLWNDPROC, global), code, wp, lp);

    params.code   = code;
    params.wparam = wp;
    params.global = global;
    return WINPROC_CallProc32ATo16( wndproc_hook_callback16, cwp32->hwnd, cwp32->message,
                                    cwp32->wParam, cwp32->lParam, &result, &params );
}


static LRESULT CALLBACK task_call_WH_CALLWNDPROC(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_CALLWNDPROC(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_CALLWNDPROC(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_CALLWNDPROC(code, wp, lp, TRUE);
}

/***********************************************************************
 *		call_WH_CBT
 */
static LRESULT CALLBACK call_WH_CBT( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    LRESULT ret = 0;
    CallNextHookEx(get_hhook(WH_CBT, global), code, wp, lp);

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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp, global );
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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp, global );
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
            ret = call_hook_16( WH_CBT, code, wp, lp, global );
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
            ret = call_hook_16( WH_CBT, code, HWND_16(wp), lp, global );
            UnMapLS( lp );
            break;
    }
    case HCBT_MINMAX:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), lp, global);
        break;
    }
    case HCBT_QS:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp, global);
        break;
    }
    case HCBT_DESTROYWND:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), lp, global);
        break;
    }
    case HCBT_KEYSKIPPED:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp, global);
        break;
    }
    case HCBT_SYSCOMMAND:
    {
        ret = call_hook_16(WH_CBT, code, wp, lp, global);
        break;
    }
    case HCBT_SETFOCUS:
    {
        ret = call_hook_16(WH_CBT, code, HWND_16(wp), HWND_16(lp), global);
        break;
    }
    default:
        ERR("unknown wndproc hook code:%04X, wParam=%p, lParam=%p\n", code, wp, lp);
    }
    return ret;
}


static LRESULT CALLBACK task_call_WH_CBT(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_CBT(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_CBT(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_CBT(code, wp, lp, TRUE);
}

/***********************************************************************
 *		call_WH_SYSMSGFILTER
 */
static LRESULT CALLBACK call_WH_SYSMSGFILTER( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    MSG *msg32 = (MSG *)lp;
    MSG16 msg16;
    LRESULT ret;
    CallNextHookEx(get_hhook(WH_SYSMSGFILTER, global), code, wp, lp);

    map_msg_32_to_16( msg32, &msg16 );
    lp = MapLS( &msg16 );
    ret = call_hook_16( WH_SYSMSGFILTER, code, wp, lp, global);
    UnMapLS( lp );
    return ret;
}

static LRESULT CALLBACK task_call_WH_SYSMSGFILTER(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_SYSMSGFILTER(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_SYSMSGFILTER(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_SYSMSGFILTER(code, wp, lp, TRUE);
}


/***********************************************************************
 *		call_WH_MOUSE
 */
static LRESULT CALLBACK call_WH_MOUSE( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    MOUSEHOOKSTRUCT *ms32 = (MOUSEHOOKSTRUCT *)lp;
    MOUSEHOOKSTRUCT16 ms16;
    LRESULT ret;
    CallNextHookEx(get_hhook(WH_MOUSE, global), code, wp, lp);

    ms16.pt.x         = ms32->pt.x;
    ms16.pt.y         = ms32->pt.y;
    ms16.hwnd         = HWND_16( ms32->hwnd );
    ms16.wHitTestCode = ms32->wHitTestCode;
    ms16.dwExtraInfo  = ms32->dwExtraInfo;

    lp = MapLS( &ms16 );
    ret = call_hook_16( WH_MOUSE, code, wp, lp, global );
    UnMapLS( lp );
    return ret;
}


static LRESULT CALLBACK task_call_WH_MOUSE(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_MOUSE(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_MOUSE(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_MOUSE(code, wp, lp, TRUE);
}

/***********************************************************************
 *		call_WH_SHELL
 */
static LRESULT CALLBACK call_WH_SHELL( INT code, WPARAM wp, LPARAM lp, BOOL global )
{
    CallNextHookEx(get_hhook(WH_SHELL, global), code, wp, lp);
    return call_hook_16( WH_SHELL, code, wp, lp, global );
}

static LRESULT CALLBACK task_call_WH_SHELL(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_SHELL(code, wp, lp, FALSE);
}

static LRESULT CALLBACK global_call_WH_SHELL(INT code, WPARAM wp, LPARAM lp)
{
    return call_WH_SHELL(code, wp, lp, TRUE);
}


// TODO: key delay and mouse messages
static void WINAPI journal_playback_cb( HWND hwnd, UINT msg, UINT_PTR id, DWORD sysTime )
{
    EVENTMSG16 emsg;
    LPARAM lp;
    INPUT input;
    lp = MapLS( &emsg );
    call_hook_16( WH_JOURNALPLAYBACK, HC_GETNEXT, 0, lp, TRUE );
    UnMapLS( lp );
    TRACE("WH_JOURNALPLAYBACK message: %x paramL: %x paramH: %x\n", emsg.message, emsg.paramL, emsg.paramH);
    switch( emsg.message )
       {
            case WM_QUEUESYNC:
                if (GetActiveWindow16())
                    PostMessage16(GetActiveWindow16(), WM_QUEUESYNC, 0, NULL);
                else
                    PostThreadMessage(GetCurrentThreadId(), WM_QUEUESYNC, 0, NULL);
                break;
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                input.type = 1;
                input.ki.wVk = emsg.paramL;
                input.ki.wScan = emsg.paramH;
                input.ki.dwFlags = 0;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                SendInput( 1, &input, sizeof(input) );
                break;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                input.type = 1;
                input.ki.wVk = emsg.paramL;
                input.ki.wScan = emsg.paramH;
                input.ki.dwFlags = 2;
                input.ki.time = 0;
                input.ki.dwExtraInfo = 0;
                SendInput( 1, &input, sizeof(input) );
                break;
        }
    call_hook_16( WH_JOURNALPLAYBACK, HC_SKIP, 0, 0, TRUE );
}


/***********************************************************************
 *		SetWindowsHook (USER.121)
 */
FARPROC16 WINAPI SetWindowsHook16( INT16 id, HOOKPROC16 proc )
{
    HINSTANCE16 hInst = FarGetOwner16( HIWORD(proc) );

    /* WH_MSGFILTER is the only task-specific hook for SetWindowsHook() */
    HTASK16 hTask = (id == WH_MSGFILTER) ? GetCurrentTask() : 0;

    if (SELECTOROF(proc) == 0x4b48) // Don't try to call an hhook, this a valid GDT selector we only use the LDT
        return 0;	

    return (FARPROC16)SetWindowsHookEx16( id, proc, hInst, hTask );
}


void install_global_hook()
{
    QUEUE16 *q = (QUEUE16*)MapSL(MAKESEGPTR(hqFirst, 0));
    while (q)
    {
        if (IsTask16(q->hTask))
        {
            struct hook16_queue_info *info = get_hook_info(TRUE, q->hTask);
            TDB *tdb;
            int id;
            tdb = GlobalLock16(q->hTask);
            for (id = WH_MINHOOK; id < WH_MAXHOOK16; id++)
            {
                int index = id - WH_MINHOOK;
                if (!list_empty(&global_hook_entry[index]))
                {
                    if (!info->global_hhook[index])
                    {
                        if (id == WH_JOURNALPLAYBACK)
                        {
                            BlockInput(TRUE);
                            info->hhook[index] = SetTimer(NULL, 0, 100, journal_playback_cb);
                            return;
                        }
                        else
                            info->global_hhook[index] = SetWindowsHookExA(id, global_hook_procs[index], 0, *(LPDWORD)((LPBYTE)tdb->teb + 0x24));
                    }
                }
            }
        }
        q = (QUEUE16*)MapSL(MAKESEGPTR(q->next, 0));
    }
}

/***********************************************************************
 *		SetWindowsHookEx (USER.291)
 */
HHOOK WINAPI SetWindowsHookEx16(INT16 id, HOOKPROC16 proc, HINSTANCE16 hInst, HTASK16 hTask)
{
    struct hook16_queue_info *info;
    int index = id - WH_MINHOOK;
    DWORD thread = GetCurrentThreadId();
    struct hook_entry *entry;

    TRACE("(%d,%04x:%04x,%04x,%04x)\n", id, SELECTOROF(proc), OFFSETOF(proc), hInst, hTask);
    if (id < WH_MINHOOK || id > WH_MAXHOOK16) return 0;
    if (!hook_procs[index])
    {
        FIXME("hook type %d broken in Win16\n", id);
        return 0;
    }
    if (!hTask)
        WARN("System-global hooks (%d) broken in Win16\n", id);
    else if (hTask != GetCurrentTask())
    {
        thread = (DWORD)HTASK_32(hTask);
        if (!thread)
        {
            ERR("invalid task? %04x\n", hTask);
            return 0;
        }
    }
    if (hTask)
    {
        info = get_hook_info(TRUE, hTask);

        if (!info->hhook[index])
        {
            if (id == WH_JOURNALPLAYBACK)
            {
                ERR("WH_JOURNALPLAYBACK is always global\n");
                return 0;
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
            struct list *head = &info->hook_entry[index];
            entry = (struct hook_entry*)allocate_hook();
            if (!entry)
            {
                ERR("failed to allocate hook\n");
                return 0;
            }
            if (!head->next)
            {
                list_init(head);
            }
            list_add_head(head, &entry->entry);
            entry->proc16 = proc;
            entry->hinst16 = GetExePtr(hInst);
            entry->htask16 = hTask;
            entry->type = id;
            entry->global = FALSE;
        }
    }
    else
    {
        entry = (struct hook_entry*)allocate_hook();
        if (!entry)
        {
            ERR("failed to allocate hook\n");
            return 0;
        }
        entry->proc16 = proc;
        entry->hinst16 = GetExePtr(hInst);
        entry->htask16 = hTask;
        entry->type = id;
        entry->global = TRUE;
        list_add_head(&global_hook_entry[index], &entry->entry);
        install_global_hook();
    }

    return entry_to_hhook(entry);
}


static BOOL16 unhook_global_hook(INT16 id, HOOKPROC16 proc)
{
    int index = id - WH_MINHOOK;
    EnterCriticalSection(&global_hook_section);
    struct hook_entry *hook_entry, *next;
    LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, &global_hook_entry[index], struct hook_entry, entry)
    {
        if (hook_entry->proc16 == proc)
        {
            LeaveCriticalSection(&global_hook_section);
            return UnhookWindowsHookEx16(entry_to_hhook(hook_entry));
        }
    }
    LeaveCriticalSection(&global_hook_section);
}
/***********************************************************************
 *		UnhookWindowsHook (USER.234)
 */
BOOL16 WINAPI UnhookWindowsHook16( INT16 id, HOOKPROC16 proc )
{
    int index = id - WH_MINHOOK;
    struct hook16_queue_info *info;
    struct hook_entry *hook_entry, *next;

    TRACE("(%d,%04x:%04x)\n", id, SELECTOROF(proc), OFFSETOF(proc));
    if (id < WH_MINHOOK || id > WH_MAXHOOK16) return FALSE;
    info = get_hook_info(FALSE, 0);
    if (!info || info->hook_entry[index].next == NULL)
    {
        return unhook_global_hook(id, proc);
    }
    LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, &info->hook_entry[index], struct hook_entry, entry)
    {
        if (hook_entry->proc16 == proc)
        {
            return UnhookWindowsHookEx16(entry_to_hhook(hook_entry));
        }
    }
    return unhook_global_hook(id, proc);
}


/***********************************************************************
 *		UnhookWindowsHookEx (USER.292)
 */
BOOL16 WINAPI UnhookWindowsHookEx16(HHOOK hhook)
{
    BOOL result = FALSE;
    struct hook16_queue_info *info;
    struct hook_entry *unhook = hhook_to_entry(hhook);
    INT type;
    struct list *entry;
    TRACE("(%08x)\n", hhook);
    if (!unhook)
        return FALSE;
    info = get_hook_info(FALSE, unhook->htask16);
    type = unhook->type;
    int index = type - WH_MINHOOK;
    if (unhook->global)
    {
        entry = &global_hook_entry[index];
    }
    else
    {
        entry = &info->hook_entry[index];
    }
    if (info)
    {
        struct hook_entry *hook_entry, *next;

        LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, entry, struct hook_entry, entry)
        {
            if (hook_entry == unhook)
            {
                list_remove(&hook_entry->entry);
                free_hook(hook_entry);
                result = TRUE;
            }
        }
        if (list_empty(entry))
        {
            if (type == WH_JOURNALPLAYBACK)
            {
                INPUT input = {0};
                input.type = 1;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                // check the modifers and make sure they are released if not pressed
                if (!(GetAsyncKeyState(VK_SHIFT) & 0x8000))
                {
                    input.ki.wVk = VK_SHIFT;
                    SendInput(1, &input, sizeof(INPUT));
                }
                if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000))
                {
                    input.ki.wVk = VK_CONTROL;
                    SendInput(1, &input, sizeof(INPUT));
                }
                if (!(GetAsyncKeyState(VK_MENU) & 0x8000))
                {
                    input.ki.wVk = VK_MENU;
                    SendInput(1, &input, sizeof(INPUT));
                }
                BlockInput(FALSE);
                KillTimer(NULL, (UINT_PTR)info->hhook[index]);
            }
            else
            {
                UnhookWindowsHookEx(info->hhook[index]);
            }
            info->hhook[index] = NULL;
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
    struct hook16_queue_info *info = get_hook_info(FALSE, 0);
    struct hook_entry *entry = hhook_to_entry(hhook);
    LRESULT ret = 0;
    struct list *next;
    struct hook_entry *next_hook;
    if (!entry)
        return 0;
    next = list_next(&(entry->global ? global_hook_entry : info->hook_entry)[entry->type - WH_MIN], &entry->entry);
    if (!next)
        return 0;
    next_hook = LIST_ENTRY(next, struct hook_entry, entry);
    if (!next_hook)
        return 0;
    return call_hook_entry_16(info, next_hook, next_hook->type, code, wparam, lparam);
}


/***********************************************************************
 *		DefHookProc (USER.235)
 */
LRESULT WINAPI DefHookProc16( INT16 code, WPARAM16 wparam, LPARAM lparam, HHOOK *lphhook )
{
    if (!((LONG_PTR)lphhook >> 16))
    {
        return 0;
    }
    return CallNextHookEx16( *lphhook, code, wparam, lparam );
}


void free_module_hooks(HINSTANCE16 hinst)
{
    QUEUE16 *q = (QUEUE16*)MapSL(MAKESEGPTR(hqFirst, 0));
    while (q)
    {
        if (IsTask16(q->hTask))
        {
            struct hook16_queue_info *info = get_hook_info(FALSE, q->hTask);
            int i;
            if (info)
            {
                for (i = 0; i < NB_HOOKS16; i++)
                {
                    struct list *list = &info->hook_entry[i];
                    struct hook_entry *hook_entry, *next;
                    if (list->next == NULL)
                        continue;
                    LIST_FOR_EACH_ENTRY_SAFE(hook_entry, next, list, struct hook_entry, entry)
                    {
                        if (hook_entry->hinst16 == hinst)
                        {
                            UnhookWindowsHookEx16(entry_to_hhook(hook_entry));
                        }
                    }
                }
            }
        }
        q = (QUEUE16*)MapSL(MAKESEGPTR(q->next, 0));
    }
}
