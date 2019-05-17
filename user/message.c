/*
 * 16-bit messaging support
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

#include "config.h"
#include "wine/port.h"

#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "wine/winuser16.h"
#include "wownt32.h"
#include "winerror.h"
#include "dde.h"
#include "user_private.h"
#include "wine/debug.h"
#include "message_table.h"
#include "../krnl386/kernel16_private.h"
#include "commctrl.h"
 /*
unknwon combobox message
When EDIT received WM_KILLFOCUS, EDIT sent this message to COMBOBOX.
*/
#define CB_UNKNOWN_167 0x167
#define CB_UNKNOWN_167_16 (WM_USER + 27)
/* undocumented messages */
//reactos sdk/include/reactos/undocuser.h
#define WM_QUERYPARKICON    0x00000036
#define WM_CLIENTSHUTDOWN   0x0000003B
#define WM_COPYGLOBALDATA   0x00000049
#define WM_LOGONNOTIFY      0x0000004C
#define WM_KLUDGEMINRECT    0x0000008B
#define WM_UAHDRAWMENU      0x00000091
#define WM_UAHDRAWITEM      0x00000092 // WM_DRAWITEM
#define WM_UAHINITMENU      0x00000093
#define WM_UAHMEASUREITEM   0x00000094 // WM_MEASUREITEM
#define WM_UAHDRAWMENUNC    0x00000095
#define WM_NCUAHDRAWCAPTION 0x000000AE
#define WM_NCUAHDRAWFRAME   0x000000AF
#define WM_CBLOSTTEXTFOCUS  0x00000167
#define MN_SETHMENU         0x000001e0
#define WM_POPUPSYSTEMMENU  0x00000313
#define WM_UAHINIT          0x0000031b


#ifndef EM_ENABLEFEATURE /* WINVER < 0x0604 */
#define EM_ENABLEFEATURE        0x00DA
#endif
WINE_DEFAULT_DEBUG_CHANNEL(msg);
WINE_DECLARE_DEBUG_CHANNEL(message);
BOOL is_win_menu_disallowed(DWORD style);
DWORD USER16_AlertableWait = 0;

struct wow_handlers32 wow_handlers32;
#define TIMER32_LPARAM 0x42137460

struct timer32_wrapper
{
    WPARAM wParam;
    LPARAM lParam;
    BOOL ref;
};
#define TIMER32_WRAP_SIZE 400
static int timer32_count;
static struct timer32_wrapper timer32[TIMER32_WRAP_SIZE];

typedef enum
{
    WINDOW_TYPE_WINDOW,
    WINDOW_TYPE_STATIC,
    WINDOW_TYPE_BUTTON,
    WINDOW_TYPE_LISTBOX,
    WINDOW_TYPE_EDIT,
    WINDOW_TYPE_SCROLLBAR,
    WINDOW_TYPE_COMBOBOX,
    WINDOW_TYPE_MDICLIENT,
} WINDOW_TYPE;
LPBYTE window_type_table;
#include <pshpack1.h>
typedef struct
{
    UINT16 uNotification;
    HWND16 hWnd;
    POINT16 ptCursor;
} DRAGLISTINFO16;
#include <poppack.h>
UINT drag_list_message;
static void dump_hmenu(HMENU menu)
{
    int count = GetMenuItemCount(menu);
    for (int i = 0; i < count; i++)
    {
        MENUITEMINFOA item = { sizeof(MENUITEMINFOA) };
        item.fMask = MIIM_BITMAP | MIIM_ID | MIIM_STRING;
        char buf[1000];
        buf[0] = 0;
        item.dwTypeData = buf;
        item.cch = 1000;
        if (!GetMenuItemInfoA(menu, i, TRUE, &item))
        {
        }
        MESSAGE("%s bmp:%p id:%04x\n", buf, item.hbmpItem, item.wID);
        if (item.hSubMenu)
        {
            MESSAGE("====BEGIN SUB MENU %p/%p ====\n", menu, item.hSubMenu);
            dump_hmenu(item.hSubMenu);
            MESSAGE("====END   SUB MENU %p/%p ====\n", menu, item.hSubMenu);
        }
    }
}
static void dump_menu(HWND hwnd)
{
    HMENU menu = GetMenu(hwnd);
    dump_hmenu(menu);
}
static LRESULT send_message_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                      LRESULT *result, void *arg )
{
    *result = SendMessageA( hwnd, msg, wp, lp );
    return *result;
}

static LRESULT post_message_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                      LRESULT *result, void *arg )
{
    *result = 0;
    return PostMessageA( hwnd, msg, wp, lp );
}

static LRESULT post_thread_message_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                             LRESULT *result, void *arg )
{
    DWORD_PTR tid = (DWORD_PTR)arg;
    *result = 0;
    return PostThreadMessageA( tid, msg, wp, lp );
}

/*static */LRESULT get_message_callback( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
                                     LRESULT *result, void *arg )
{
    MSG16 *msg16 = arg;

    msg16->hwnd    = hwnd;
    msg16->message = msg;
    msg16->wParam  = wp;
    msg16->lParam  = lp;
    *result = 0;
    return 0;
}

static LRESULT defdlg_proc_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                     LRESULT *result, void *arg )
{
    *result = DefDlgProcA( hwnd, msg, wp, lp );
    return *result;
}

static LRESULT call_window_proc_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                          LRESULT *result, void *arg )
{
    WNDPROC proc = arg;
    *result = CallWindowProcA( proc, hwnd, msg, wp, lp );
    return *result;
}


/**********************************************************************
 * Support for window procedure thunks
 */

#include "pshpack1.h"
typedef struct
{
    BYTE        popl_eax;        /* popl  %eax (return address) */
    BYTE        pushl_func;      /* pushl $proc */
    WNDPROC     proc;
    BYTE        pushl_eax;       /* pushl %eax */
    BYTE        ljmp;            /* ljmp relay*/
    DWORD       relay_offset;    /* __wine_call_wndproc */
    WORD        relay_sel;
} WINPROC_THUNK;
#include "poppack.h"

#define WINPROC_HANDLE (~0u >> 16)
#define MAX_WINPROCS32 4096
#define MAX_WINPROCS16 1024

static WNDPROC16 winproc16_array[MAX_WINPROCS16];
static BOOL winproc16_native[MAX_WINPROCS16];
static unsigned int winproc16_used;

static WINPROC_THUNK *thunk_array;
static UINT thunk_selector;

/* return the window proc index for a given handle, or -1 for an invalid handle
 * indices 0 .. MAX_WINPROCS32-1 are for 32-bit procs,
 * indices MAX_WINPROCS32 .. MAX_WINPROCS32+MAX_WINPROCS16-1 for 16-bit procs */
static int winproc_to_index( WNDPROC16 handle )
{
    unsigned int index;

    if (HIWORD(handle) == thunk_selector)
    {
        index = LOWORD(handle) / sizeof(WINPROC_THUNK);
        /* check alignment */
        if (index * sizeof(WINPROC_THUNK) != LOWORD(handle)) return -1;
        /* check array limits */
        if (index >= MAX_WINPROCS32) return -1;
    }
    else
    {
        index = LOWORD(handle);
        if ((ULONG_PTR)handle >> 16 != WINPROC_HANDLE) return -1;
        /* check array limits */
        if (index >= winproc16_used + MAX_WINPROCS32) return -1;
    }
    return index;
}

/* allocate a 16-bit thunk for an existing window proc */
static WNDPROC16 alloc_win16_thunk( WNDPROC handle )
{
    static FARPROC16 relay;
    WINPROC_THUNK *thunk;
    UINT index = LOWORD( handle );

    if (index >= MAX_WINPROCS32)  /* already a 16-bit proc */
        return winproc16_array[index - MAX_WINPROCS32];

    if (!thunk_array)  /* allocate the array and its selector */
    {
        LDT_ENTRY entry;

        assert( MAX_WINPROCS16 * sizeof(WINPROC_THUNK) <= 0x10000 );

        if (!(thunk_selector = wine_ldt_alloc_entries(1))) return NULL;
        if (!(thunk_array = VirtualAlloc( NULL, MAX_WINPROCS16 * sizeof(WINPROC_THUNK), MEM_COMMIT,
                                          PAGE_EXECUTE_READWRITE ))) return NULL;
        wine_ldt_set_base( &entry, thunk_array );
        wine_ldt_set_limit( &entry, MAX_WINPROCS16 * sizeof(WINPROC_THUNK) - 1 );
        wine_ldt_set_flags( &entry, WINE_LDT_FLAGS_CODE | WINE_LDT_FLAGS_32BIT );
        wine_ldt_set_entry( thunk_selector, &entry );
        relay = GetProcAddress16( GetModuleHandle16("user"), "__wine_call_wndproc" );
    }

    thunk = &thunk_array[index];
    thunk->popl_eax     = 0x58;   /* popl  %eax */
    thunk->pushl_func   = 0x68;   /* pushl $proc */
    thunk->proc         = handle;
    thunk->pushl_eax    = 0x50;   /* pushl %eax */
    thunk->ljmp         = 0xea;   /* ljmp   relay*/
    thunk->relay_offset = OFFSETOF(relay);
    thunk->relay_sel    = SELECTOROF(relay);
    return (WNDPROC16)MAKESEGPTR( thunk_selector, index * sizeof(WINPROC_THUNK) );
}

LRESULT CALLBACK WindowProc16(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK edit_wndproc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
/* Some bad behavior programs access native WNDPROC... */
BYTE *dummy_proc;
BOOL dummy_proc_allocated;
static void init_dummy_proc()
{
    if (!dummy_proc_allocated)
    {
        dummy_proc = HeapAlloc(GetProcessHeap(), 0xcc, MAX_WINPROCS32 * 2);
        memset(dummy_proc, 0xc3, MAX_WINPROCS32 * 2);
        dummy_proc_allocated = TRUE;
        LDT_ENTRY dummy;
        wine_ldt_set_base(&dummy, dummy_proc);
        wine_ldt_set_limit(&dummy, MAX_WINPROCS32 * 2);
        wine_ldt_set_flags(&dummy, WINE_LDT_FLAGS_CODE);
        wine_ldt_set_entry(0xffff, &dummy);
    }
}
WNDPROC WINPROC_AllocNativeProc(WNDPROC16 func)
{
    init_dummy_proc();
    if (func == (WNDPROC16)WindowProc16)
        func = (WNDPROC16)DefWindowProcA;
    WNDPROC ret = WINPROC_AllocProc16(func);
    int index = winproc_to_index((WNDPROC16)ret);
    if (index == -1)
        return ret;
    if (index >= MAX_WINPROCS32)
        index -= MAX_WINPROCS32;
    winproc16_native[index] = TRUE;
    return ret;
}
BOOL WINPROC_IsNativeProc(WNDPROC16 func)
{
    int index = winproc_to_index(func);
    if (index == -1)
        return FALSE;
    if (index >= MAX_WINPROCS32)
        index -= MAX_WINPROCS32;
    return winproc16_native[index];
}
/**********************************************************************
 *	     WINPROC_AllocProc16
 */
WNDPROC WINPROC_AllocProc16( WNDPROC16 func )
{
    unsigned int index;
    WNDPROC ret;

    if (!func) return NULL;

    /* check if the function is already a win proc */
    if ((index = winproc_to_index( func )) != -1)
        return (WNDPROC)(ULONG_PTR)(index | (WINPROC_HANDLE << 16));

    /* then check if we already have a winproc for that function */
    for (index = 0; index < winproc16_used; index++)
        if (winproc16_array[index] == func) goto done;

    if (winproc16_used >= MAX_WINPROCS16)
    {
        FIXME( "too many winprocs, cannot allocate one for 16-bit %p\n", func );
        return NULL;
    }
    winproc16_array[winproc16_used++] = func;

done:
    ret = (WNDPROC)(ULONG_PTR)((index + MAX_WINPROCS32) | (WINPROC_HANDLE << 16));
    TRACE( "returning %p for %p/16-bit (%d/%d used)\n",
           ret, func, winproc16_used, MAX_WINPROCS16 );
    return ret;
}

WNDPROC16 WINPROC_AllocProc16_2(WNDPROC func)
{
    unsigned int index;
    WNDPROC ret;

    if (!func) return NULL;
    if (func == WindowProc16)
        func = DefWindowProcA;

    /* then check if we already have a winproc for that function */
    for (index = 0; index < winproc16_used; index++)
        if (winproc16_array[index] == func) goto done;

    if (winproc16_used >= MAX_WINPROCS16)
    {
        FIXME("too many winprocs, cannot allocate one for 16-bit %p\n", func);
        return NULL;
    }
    winproc16_array[winproc16_used++] = func;

done:
    ret = (WNDPROC)(ULONG_PTR)((index + MAX_WINPROCS32) | (WINPROC_HANDLE << 16));
    TRACE("returning %p for %p/16-bit (%d/%d used)\n",
        ret, func, winproc16_used, MAX_WINPROCS16);
    return ret;
}

WNDPROC16 WINPROC_AllocNativeProc_2(WNDPROC func)
{
    init_dummy_proc();
    WNDPROC16 ret = WINPROC_AllocProc16_2(func);
    int index = winproc_to_index(ret);
    if (index == -1)
        return ret;
    if (index >= MAX_WINPROCS32)
        index -= MAX_WINPROCS32;
    winproc16_native[index] = TRUE;
    return ret;
}
/**********************************************************************
 *	     WINPROC_GetProc16
 *
 * Get a window procedure pointer that can be passed to the Windows program.
 */
WNDPROC16 WINPROC_GetProc16( WNDPROC proc, BOOL unicode )
{
    WNDPROC winproc = wow_handlers32.alloc_winproc( proc, unicode );

    if ((ULONG_PTR)winproc >> 16 != WINPROC_HANDLE) return (WNDPROC16)winproc;
    return alloc_win16_thunk( winproc );
}
/* vm functions */
WORD get_native_wndproc_segment()
{
    return 0xffff;
}
static WORD POP16(CONTEXT *context)
{
    LPWORD stack = MapSL(MAKESEGPTR(context->SegSs, context->Esp));
    context->Esp += 2;
    return *stack;
}
static DWORD POP32(CONTEXT *context)
{
    LPDWORD stack = MapSL(MAKESEGPTR(context->SegSs, context->Esp));
    context->Esp += 4;
    return *stack;
}
DWORD call_native_wndproc_context(CONTEXT *context)
{
    LPWORD stack = MapSL(MAKESEGPTR(context->SegSs, context->Esp));
    SEGPTR ret = POP32(context);
    LPARAM lParam = POP32(context);
    WPARAM16 wParam = POP16(context);
    UINT16 msg = POP16(context);
    HWND16 hwnd = POP16(context);
    LRESULT result = CallWindowProc16((WNDPROC16)MAKESEGPTR(context->SegCs, context->Eip), hwnd, msg, wParam, lParam);
    context->Edx = HIWORD(result);
    context->Eax = LOWORD(result);
    context->Eip = LOWORD(ret);
    context->SegCs = HIWORD(ret);
    return 0;
}

/* call a 16-bit window procedure */
/*static*/ LRESULT call_window_proc16( HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam,
                                   LRESULT *result, void *arg )
{
    WNDPROC16 func = arg;
    int index = winproc_to_index( func );
    CONTEXT context;
    size_t size = 0;
#include <pshpack1.h>
    struct
    {
        WORD params[5];
        union
        {
            CREATESTRUCT16 cs16;
            DRAWITEMSTRUCT16 dis16;
            COMPAREITEMSTRUCT16 cis16;
        } u;
    } args;
#include <poppack.h>

    if (index >= MAX_WINPROCS32) func = winproc16_array[index - MAX_WINPROCS32];

    /* Window procedures want ax = hInstance, ds = es = ss */

    memset(&context, 0, sizeof(context));
    context.SegDs = context.SegEs = SELECTOROF(getWOW32Reserved());
    context.SegFs = wine_get_fs();
    context.SegGs = wine_get_gs();
    context.Eax = !hwnd ? 0 : GetWindowWord16(hwnd, GWLP_HINSTANCE) | 1; /* Handle To Sel */
    if (!context.Eax) context.Eax = context.SegDs;
    context.Ebx = 6;
    context.Esi = hwnd;
    context.SegCs = SELECTOROF(func);
    context.Eip   = OFFSETOF(func);
    context.Ebp   = OFFSETOF(getWOW32Reserved()) + FIELD_OFFSET(STACK16FRAME, bp);

    if (lParam)
    {
        /* Some programs (eg. the "Undocumented Windows" examples, JWP) only
           work if structures passed in lParam are placed in the stack/data
           segment. Programmers easily make the mistake of converting lParam
           to a near rather than a far pointer, since Windows apparently
           allows this. We copy the structures to the 16 bit stack; this is
           ugly but makes these programs work. */
        switch (msg)
        {
          case WM_CREATE:
          case WM_NCCREATE:
            size = sizeof(CREATESTRUCT16); break;
          case WM_DRAWITEM:
            size = sizeof(DRAWITEMSTRUCT16); break;
          case WM_COMPAREITEM:
            size = sizeof(COMPAREITEMSTRUCT16); break;
        }
        if (size)
        {
            memcpy( &args.u, MapSL(lParam), size );
            lParam = PtrToUlong(getWOW32Reserved()) - size;
        }
    }
    PVOID old = getWOW32Reserved();

    args.params[4] = hwnd;
    args.params[3] = msg;
    args.params[2] = wParam;
    args.params[1] = HIWORD(lParam);
    args.params[0] = LOWORD(lParam);
    WOWCallback16Ex( 0, WCB16_REGS, sizeof(args.params) + size, &args, (DWORD *)&context );
    //restore stack
    setWOW32Reserved(old);
    *result = MAKELONG( LOWORD(context.Eax), LOWORD(context.Edx) );
    return *result;
}

static LRESULT call_dialog_proc16( HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp,
                                   LRESULT *result, void *arg )
{
    LRESULT ret = call_window_proc16( hwnd, msg, wp, lp, result, arg );
    *result = GetWindowLongPtrW( WIN_Handle32(hwnd), DWLP_MSGRESULT );
    return LOWORD(ret);
}

static LRESULT call_window_proc_Ato16( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                       LRESULT *result, void *arg )
{
    return WINPROC_CallProc32ATo16( call_window_proc16, hwnd, msg, wp, lp, result, arg );
}

static LRESULT call_dialog_proc_Ato16( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                       LRESULT *result, void *arg )
{
    return WINPROC_CallProc32ATo16( call_dialog_proc16, hwnd, msg, wp, lp, result, arg );
}



/**********************************************************************
 * Support for Edit word break proc thunks
 */

#define MAX_THUNKS 32

#include <pshpack1.h>
static struct word_break_thunk
{
    BYTE                popl_eax;       /* popl  %eax (return address) */
    BYTE                pushl_proc16;   /* pushl proc16 */
    EDITWORDBREAKPROC16 proc16;
    BYTE                pushl_eax;      /* pushl %eax */
    BYTE                jmp;            /* ljmp call_word_break_proc16 */
    DWORD               callback;
} *word_break_thunks;
#include <poppack.h>

/**********************************************************************
 *           call_word_break_proc16
 */
static INT16 CALLBACK call_word_break_proc16( SEGPTR proc16, LPSTR text, INT index, INT count, INT action )
{
    SEGPTR segptr;
    WORD args[5];
    DWORD result;

    segptr = MapLS( text );
    args[4] = SELECTOROF(segptr);
    args[3] = OFFSETOF(segptr);
    args[2] = index;
    args[1] = count;
    args[0] = action;
    WOWCallback16Ex( proc16, WCB16_PASCAL, sizeof(args), args, &result );
    UnMapLS( segptr );
    return LOWORD(result);
}

/******************************************************************
 *		add_word_break_thunk
 */
static struct word_break_thunk *add_word_break_thunk( EDITWORDBREAKPROC16 proc16 )
{
    struct word_break_thunk *thunk;

    if (!word_break_thunks)
    {
        word_break_thunks = VirtualAlloc( NULL, MAX_THUNKS * sizeof(*thunk),
                                          MEM_COMMIT, PAGE_EXECUTE_READWRITE );
        if (!word_break_thunks) return NULL;

        for (thunk = word_break_thunks; thunk < &word_break_thunks[MAX_THUNKS]; thunk++)
        {
            thunk->popl_eax     = 0x58;   /* popl  %eax */
            thunk->pushl_proc16 = 0x68;   /* pushl proc16 */
            thunk->pushl_eax    = 0x50;   /* pushl %eax */
            thunk->jmp          = 0xe9;   /* jmp call_word_break_proc16 */
            thunk->callback     = (char *)call_word_break_proc16 - (char *)(&thunk->callback + 1);
        }
    }
    for (thunk = word_break_thunks; thunk < &word_break_thunks[MAX_THUNKS]; thunk++)
        if (thunk->proc16 == proc16) return thunk;

    for (thunk = word_break_thunks; thunk < &word_break_thunks[MAX_THUNKS]; thunk++)
    {
        if (thunk->proc16) continue;
        thunk->proc16 = proc16;
        return thunk;
    }
    FIXME("Out of word break thunks\n");
    return NULL;
}

/******************************************************************
 *		get_word_break_thunk
 */
static EDITWORDBREAKPROC16 get_word_break_thunk( EDITWORDBREAKPROCA proc )
{
    struct word_break_thunk *thunk = (struct word_break_thunk *)proc;
    if (word_break_thunks && thunk >= word_break_thunks && thunk < &word_break_thunks[MAX_THUNKS])
        return thunk->proc16;
    return NULL;
}


/***********************************************************************
 * Support for 16<->32 message mapping
 */

static inline void *get_buffer( void *static_buffer, size_t size, size_t need )
{
    if (size >= need) return static_buffer;
    return HeapAlloc( GetProcessHeap(), 0, need );
}

static inline void free_buffer( void *static_buffer, void *buffer )
{
    if (buffer != static_buffer) HeapFree( GetProcessHeap(), 0, buffer );
}

static void RECT16to32( const RECT16 *from, RECT *to )
{
    to->left   = from->left;
    to->top    = from->top;
    to->right  = from->right;
    to->bottom = from->bottom;
}

static void RECT32to16( const RECT *from, RECT16 *to )
{
    to->left   = from->left;
    to->top    = from->top;
    to->right  = from->right;
    to->bottom = from->bottom;
}

static void MINMAXINFO32to16( const MINMAXINFO *from, MINMAXINFO16 *to )
{
    to->ptReserved.x     = from->ptReserved.x;
    to->ptReserved.y     = from->ptReserved.y;
    to->ptMaxSize.x      = from->ptMaxSize.x;
    to->ptMaxSize.y      = from->ptMaxSize.y;
    to->ptMaxPosition.x  = from->ptMaxPosition.x;
    to->ptMaxPosition.y  = from->ptMaxPosition.y;
    to->ptMinTrackSize.x = from->ptMinTrackSize.x;
    to->ptMinTrackSize.y = from->ptMinTrackSize.y;
    to->ptMaxTrackSize.x = from->ptMaxTrackSize.x;
    to->ptMaxTrackSize.y = from->ptMaxTrackSize.y;
}

static void MINMAXINFO16to32( const MINMAXINFO16 *from, MINMAXINFO *to )
{
    to->ptReserved.x     = from->ptReserved.x;
    to->ptReserved.y     = from->ptReserved.y;
    to->ptMaxSize.x      = from->ptMaxSize.x;
    to->ptMaxSize.y      = from->ptMaxSize.y;
    to->ptMaxPosition.x  = from->ptMaxPosition.x;
    to->ptMaxPosition.y  = from->ptMaxPosition.y;
    to->ptMinTrackSize.x = from->ptMinTrackSize.x;
    to->ptMinTrackSize.y = from->ptMinTrackSize.y;
    to->ptMaxTrackSize.x = from->ptMaxTrackSize.x;
    to->ptMaxTrackSize.y = from->ptMaxTrackSize.y;
}

static void WINDOWPOS32to16( const WINDOWPOS* from, WINDOWPOS16* to )
{
    to->hwnd            = HWND_16(from->hwnd);
    to->hwndInsertAfter = HWND_16(from->hwndInsertAfter);
    to->x               = from->x;
    to->y               = from->y;
    to->cx              = from->cx;
    to->cy              = from->cy;
    to->flags           = from->flags;
}

static void WINDOWPOS16to32( const WINDOWPOS16* from, WINDOWPOS* to )
{
    to->hwnd            = WIN_Handle32(from->hwnd);
    to->hwndInsertAfter = (from->hwndInsertAfter == (HWND16)-1) ?
                           HWND_TOPMOST : WIN_Handle32(from->hwndInsertAfter);
    to->x               = from->x;
    to->y               = from->y;
    to->cx              = from->cx;
    to->cy              = from->cy;
    to->flags           = from->flags;
}

/* The strings are not copied */
static void CREATESTRUCT32Ato16( HWND hwnd32, const CREATESTRUCTA* from, CREATESTRUCT16* to )
{
    to->lpCreateParams = (SEGPTR)from->lpCreateParams;
    to->hInstance      = HINSTANCE_16(from->hInstance);
    if (!is_win_menu_disallowed(GetWindowLongA(hwnd32, GWL_STYLE)))
    {
        to->hMenu      = HMENU_16(from->hMenu);
    }
    else
    {
        to->hMenu      = (HMENU16)from->hMenu;
    }
    to->hwndParent     = HWND_16(from->hwndParent);
    to->cy             = from->cy;
    to->cx             = from->cx;
    to->y              = from->y;
    to->x              = from->x;
    to->style          = from->style;
    to->dwExStyle      = from->dwExStyle;
}

static void CREATESTRUCT16to32A( HWND hwnd32, const CREATESTRUCT16* from, CREATESTRUCTA *to )
{
    to->lpCreateParams = (LPVOID)from->lpCreateParams;
    to->hInstance      = HINSTANCE_32(from->hInstance);
    if (!is_win_menu_disallowed(GetWindowLongA(hwnd32, GWL_STYLE)))
    {
        to->hMenu      = HMENU_32(from->hMenu);
    }
    else
    {
        to->hMenu      = (HMENU)from->hMenu;
    }
    to->hwndParent     = WIN_Handle32(from->hwndParent);
    to->cy             = from->cy;
    to->cx             = from->cx;
    to->y              = from->y;
    to->x              = from->x;
    to->style          = from->style;
    to->dwExStyle      = from->dwExStyle;
    to->lpszName       = MapSL(from->lpszName);
    to->lpszClass      = win32classname(from->hInstance, MapSL(from->lpszClass));
}

/* The strings are not copied */
static void MDICREATESTRUCT32Ato16( const MDICREATESTRUCTA* from, MDICREATESTRUCT16* to )
{
    to->hOwner = HINSTANCE_16(from->hOwner);
    to->x      = from->x;
    to->y      = from->y;
    to->cx     = from->cx;
    to->cy     = from->cy;
    to->style  = from->style;
    to->lParam = from->lParam;
}

static void MDICREATESTRUCT16to32A( const MDICREATESTRUCT16* from, MDICREATESTRUCTA *to )
{
	to->hOwner = HINSTANCE_32(from->hOwner);

	to->x      = (from->x == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)from->x;
	to->y      = (from->y == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)from->y;
	to->cx     = (from->cx == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)from->cx;
	to->cy     = (from->cy == CW_USEDEFAULT16) ? CW_USEDEFAULT : (INT)from->cy;
    to->style  = from->style;
    to->lParam = from->lParam;
    to->szTitle = MapSL(from->szTitle);
    to->szClass = win32classname(from->hOwner, MapSL(from->szClass));
}

static UINT_PTR convert_handle_16_to_32(HANDLE16 src, unsigned int flags)
{
    HANDLE      dst;
    UINT        sz = GlobalSize16(src);
    LPSTR       ptr16, ptr32;

    if (!(dst = GlobalAlloc(flags, sz)))
        return 0;
    ptr16 = GlobalLock16(src);
    ptr32 = GlobalLock(dst);
    if (ptr16 != NULL && ptr32 != NULL) memcpy(ptr32, ptr16, sz);
    GlobalUnlock16(src);
    GlobalUnlock(dst);

    return (UINT_PTR)dst;
}

static HANDLE16 convert_handle_32_to_16(UINT_PTR src, unsigned int flags)
{
    HANDLE16    dst;
    UINT        sz = GlobalSize((HANDLE)src);
    LPSTR       ptr16, ptr32;

    if (!(dst = GlobalAlloc16(flags, sz)))
        return 0;
    ptr32 = GlobalLock((HANDLE)src);
    ptr16 = GlobalLock16(dst);
    if (ptr16 != NULL && ptr32 != NULL) memcpy(ptr16, ptr32, sz);
    GlobalUnlock((HANDLE)src);
    GlobalUnlock16(dst);

    return dst;
}

static BOOL is_old_app( HWND hwnd )
{
    HINSTANCE inst = (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE );
    return inst && !((ULONG_PTR)inst >> 16) && (GetExpWinVer16(LOWORD(inst)) & 0xFF00) == 0x0300;
}

static int find_sub_menu( HMENU *hmenu, HMENU16 target )
{
    int i, pos, count = GetMenuItemCount( *hmenu );

    for (i = 0; i < count; i++)
    {
        HMENU sub = GetSubMenu( *hmenu, i );
        if (!sub) continue;
        if (HMENU_16(sub) == target) return i;
        if ((pos = find_sub_menu( &sub, target )) != -1)
        {
            *hmenu = sub;
            return pos;
        }
    }
    return -1;
}

WNDPROC get_classinfo_wndproc(const char *class)
{
    WNDCLASSA wc;
    GetClassInfoA(NULL, class, &wc);
    return wc.lpfnWndProc;
}
WNDPROC get_window_wndproc(const char *class)
{
    WNDPROC lpfnWndProc2 = NULL;
    HWND hWnd = CreateWindowExA(0, class, "", 0, 0, 0, 1, 1, 0, 0, GetModuleHandleA(NULL), 0);
    if (hWnd)
    {
        lpfnWndProc2 = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
        DestroyWindow(hWnd);
    }
    return lpfnWndProc2;
}
BOOL is_listbox_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("LISTBOX");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("LISTBOX");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isListBox(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_LISTBOX)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_listbox_wndproc(lpfnWndProc);
}

BOOL is_edit_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("EDIT");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("EDIT");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isEdit(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_EDIT)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_edit_wndproc(lpfnWndProc);
}

BOOL is_scrollbar_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("SCROLLBAR");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("SCROLLBAR");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isScrollBar(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_SCROLLBAR)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_scrollbar_wndproc(lpfnWndProc);
}

BOOL is_combobox_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("COMBOBOX");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("COMBOBOX");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isComboBox(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_COMBOBOX)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_combobox_wndproc(lpfnWndProc);
}

BOOL is_static_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("STATIC");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("STATIC");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isStatic(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_STATIC)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_static_wndproc(lpfnWndProc);
}

BOOL is_button_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("BUTTON");
    if (!lpfnWndProc2)
        lpfnWndProc2 = get_window_wndproc("BUTTON");
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL isButton(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_BUTTON)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_button_wndproc(lpfnWndProc);
}

BOOL is_mdiclient_wndproc(WNDPROC lpfnWndProc)
{
    static WNDPROC lpfnWndProc1 = 0;
    static WNDPROC lpfnWndProc2 = 0;
    if (!lpfnWndProc1)
        lpfnWndProc1 = get_classinfo_wndproc("MDICLIENT");
    if (!lpfnWndProc2)
    {
        CLIENTCREATESTRUCT ccs = { 0 };
        HWND hwnd = CreateWindowExA(0, "MDICLIENT", "", 0, 0, 0, 1, 1, 0, 0, GetModuleHandleA(NULL), &ccs);
        lpfnWndProc2 = (WNDPROC)GetWindowLongPtrA(hwnd, GWLP_WNDPROC);
        DestroyWindow(hwnd);
    }
    return lpfnWndProc ? (lpfnWndProc == lpfnWndProc1 || lpfnWndProc == lpfnWndProc2) : FALSE;
}
BOOL is_mdiclient(HWND16 hWnd16, HWND hWnd)
{
    if (window_type_table[hWnd16] == WINDOW_TYPE_MDICLIENT)
        return TRUE;
    WNDPROC lpfnWndProc = (WNDPROC)GetWindowLongPtrA(hWnd, GWLP_WNDPROC);
    return is_mdiclient_wndproc(lpfnWndProc);
}
/***********************************************************************
*           listbox_proc16
*/
static LRESULT listbox_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
	, LRESULT *result, void *arg, BOOL *f)
{
	static const UINT msg16_offset = LB_ADDSTRING16 - LB_ADDSTRING;
	LRESULT ret;

	*f = TRUE;
	switch (msg)
	{
	case WM_SIZE:
		if (is_old_app(hwnd))
		{
			DWORD style = GetWindowLongW(hwnd, GWL_STYLE);
			int height, remaining, item_height;
			RECT rect;

			/* give a margin for error to old 16 bits programs - if we need
			less than the height of the nonclient area, round to the
			*next* number of items */

			if (!(style & LBS_NOINTEGRALHEIGHT) && !(style & LBS_OWNERDRAWVARIABLE))
			{
				GetClientRect(hwnd, &rect);
				height = rect.bottom - rect.top;
				item_height = wow_handlers32.listbox_proc(hwnd, LB_GETITEMHEIGHT, 0, 0, FALSE);
				remaining = item_height ? (height % item_height) : 0;
				if ((height > item_height) && remaining)
				{
					GetWindowRect(hwnd, &rect);
					if ((item_height - remaining) <= rect.bottom - rect.top - height)
						remaining = remaining - item_height;
					TRACE("[%p]: changing height %d -> %d\n", hwnd, height, height - remaining);
					SetWindowPos(hwnd, 0, 0, 0, rect.right - rect.left,
						rect.bottom - rect.top - remaining,
						SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
					*result = 0;
					return 0;
				}
			}
		}
		ret = callback(hwnd, msg, wParam, lParam, result, arg);
		break;
//		return wow_handlers32.listbox_proc(hwnd, msg, wParam, lParam, unicode);

	case LB_RESETCONTENT16:
	case LB_DELETESTRING16:
	case LB_GETITEMDATA16:
	case LB_SETITEMDATA16:
	case LB_GETCOUNT16:
	case LB_GETTEXTLEN16:
	case LB_GETCURSEL16:
	case LB_GETTOPINDEX16:
	case LB_GETITEMHEIGHT16:
	case LB_SETCARETINDEX16:
	case LB_GETCARETINDEX16:
	case LB_SETTOPINDEX16:
	case LB_SETCOLUMNWIDTH16:
	case LB_GETSELCOUNT16:
	case LB_SELITEMRANGE16:
	case LB_SELITEMRANGEEX16:
	case LB_GETHORIZONTALEXTENT16:
	case LB_SETHORIZONTALEXTENT16:
	case LB_GETANCHORINDEX16:
	case LB_CARETON16:
	case LB_CARETOFF16:
		msg -= msg16_offset;
		break;
	case LB_GETSEL16:
	case LB_SETSEL16:
	case LB_SETCURSEL16:
	case LB_SETANCHORINDEX16:
		wParam = (INT)(INT16)wParam;
		msg -= msg16_offset;
		break;
	case LB_INSERTSTRING16:
	case LB_FINDSTRING16:
	case LB_FINDSTRINGEXACT16:
	case LB_SELECTSTRING16:
		wParam = (INT)(INT16)wParam;
		/* fall through */
	case LB_ADDSTRING16:
	case LB_ADDFILE16:
	{
		DWORD style = GetWindowLongW(hwnd, GWL_STYLE);
		if ((style & LBS_HASSTRINGS) || !(style & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
			lParam = (LPARAM)MapSL(lParam);
		msg -= msg16_offset;
		break;
	}
	case LB_GETTEXT16:
		lParam = (LPARAM)MapSL(lParam);
		msg -= msg16_offset;
        ret = callback(hwnd, msg, wParam, lParam, result, arg);
        return *result;
		break;
	case LB_SETITEMHEIGHT16:
		lParam = LOWORD(lParam);
		msg -= msg16_offset;
		break;
	case LB_GETITEMRECT16:
	{
		RECT rect;
		RECT16 *r16 = MapSL(lParam);
		ret = callback(hwnd, LB_GETITEMRECT, (INT16)wParam, (LPARAM)&rect, result, arg);
//		ret = wow_handlers32.listbox_proc(hwnd, LB_GETITEMRECT, (INT16)wParam, (LPARAM)&rect, FALSE);
		r16->left = rect.left;
		r16->top = rect.top;
		r16->right = rect.right;
		r16->bottom = rect.bottom;
		return ret;
	}
	case LB_GETSELITEMS16:
	{
		INT16 *array16 = MapSL(lParam);
		INT i, count = (INT16)wParam, *array;
		if (!(array = HeapAlloc(GetProcessHeap(), 0, wParam * sizeof(*array)))) return LB_ERRSPACE;
		ret = callback(hwnd, LB_GETSELITEMS, count, (LPARAM)array, result, arg);
		//ret = wow_handlers32.listbox_proc(hwnd, LB_GETSELITEMS, count, (LPARAM)array, FALSE);
		for (i = 0; i < ret; i++) array16[i] = array[i];
		HeapFree(GetProcessHeap(), 0, array);
		return ret;
	}
	case LB_DIR16:
		/* according to Win16 docs, DDL_DRIVES should make DDL_EXCLUSIVE
		* be set automatically (this is different in Win32) */
		if (wParam & DDL_DRIVES) wParam |= DDL_EXCLUSIVE;
		lParam = (LPARAM)MapSL(lParam);
		msg -= msg16_offset;
		break;
	case LB_SETTABSTOPS16:
	{
		INT i, count, *tabs = NULL;
		INT16 *tabs16 = MapSL(lParam);

		if ((count = (INT16)wParam) > 0)
		{
			if (!(tabs = HeapAlloc(GetProcessHeap(), 0, wParam * sizeof(*tabs)))) return LB_ERRSPACE;
			for (i = 0; i < count; i++) tabs[i] = tabs16[i];
		}
		ret = callback(hwnd, LB_SETTABSTOPS, count, (LPARAM)tabs, result, arg);
		//ret = wow_handlers32.listbox_proc(hwnd, LB_SETTABSTOPS, count, (LPARAM)tabs, FALSE);
		HeapFree(GetProcessHeap(), 0, tabs);
		return ret;
	}
	default:
		*f = FALSE;
		return 0;
	}
	ret = callback(hwnd, msg, wParam, lParam, result, arg);
	return ret;//wow_handlers32.listbox_proc(hwnd, msg, wParam, lParam, FALSE);
}
static LRESULT combo_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f);
static LRESULT button_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f);
static LRESULT edit_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f);
static LRESULT scrollbar_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f);
static LRESULT static_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f);
void DROPSTRUCT16_32(LPDROPSTRUCT lpds32, LPDROPSTRUCT16 lpds16)
{
    lpds32->hwndSink = HWND_32(lpds16->hwndSink);
    lpds32->hwndSource = HWND_32(lpds16->hwndSource);
    lpds32->dwControlData = lpds16->dwControlData;
    lpds32->dwData = lpds16->dwData;
    lpds32->wFmt = lpds16->wFmt;
    lpds32->ptDrop.x = lpds16->ptDrop.x;
    lpds32->ptDrop.y = lpds16->ptDrop.y;
    TRACE("%p,%p,%04x,%08x,%d,%d,%08x\n", lpds32->hwndSink, lpds32->hwndSource, lpds32->wFmt, lpds32->dwData, lpds32->ptDrop.x, lpds32->ptDrop.y, lpds32->dwControlData);
}
void DROPSTRUCT32_16(LPDROPSTRUCT lpds32, LPDROPSTRUCT16 lpds16)
{
    DWORD pid;
    lpds16->hwndSink = HWND_16(lpds32->hwndSink);
    lpds16->hwndSource = HWND_16(lpds32->hwndSource);
    lpds16->dwControlData = lpds32->dwControlData;
    lpds16->dwData = lpds32->dwData;
    lpds16->wFmt = lpds32->wFmt;
    lpds16->ptDrop.x = lpds32->ptDrop.x;
    lpds16->ptDrop.y = lpds32->ptDrop.y;
    if (GetWindowThreadProcessId(lpds32->hwndSource, &pid))
    {
        if (GetCurrentProcessId() != pid)
        {
            WCHAR buf[0x1000];
            SIZE_T read;
            HANDLE process = OpenProcess(PROCESS_VM_READ, FALSE, pid);
            if (ReadProcessMemory(process, lpds32->dwData, buf, sizeof(buf), &read) && read >= sizeof(*buf))
            {
                buf[(read / sizeof(*buf)) - 1] = 0;
                ERR("D&D file from other processes is broken in win32. %s\n", debugstr_w(buf));
            }
            CloseHandle(process);
        }
    }
    TRACE("%p,%p,%04x,%08x,%d,%d,%08x\n", lpds32->hwndSink, lpds32->hwndSource, lpds32->wFmt, lpds32->dwData, lpds32->ptDrop.x, lpds32->ptDrop.y, lpds32->dwControlData);
}

ATOM atom_progman;
ATOM atom_progman16;
ATOM gatom_progman;
ATOM gatom_progman16;
static void init_atom()
{
    if (!atom_progman)
    {
        atom_progman = AddAtomA("Progman");
    }
    if (!gatom_progman)
    {
        gatom_progman = GlobalAddAtomA("Progman");
    }
    if (!atom_progman16)
    {
        atom_progman16 = AddAtomA("Progman16");
    }
    if (!gatom_progman16)
    {
        gatom_progman16 = GlobalAddAtomA("Progman16");
    }
}
static ATOM service16_32(ATOM atom)
{
    init_atom();
    if (atom_progman == atom)
        return atom_progman16;
    if (gatom_progman == atom)
	return gatom_progman16;
    return atom;
}

static ATOM service32_16(ATOM atom)
{
    init_atom();
    if (atom_progman16 == atom)
        return atom_progman;
    if (gatom_progman16 == atom)
        return gatom_progman;
    return atom;
}

static ATOM topic16_32(ATOM atom)
{
    init_atom();
    if (atom_progman == atom)
        return atom_progman16;
    if (gatom_progman == atom)
        return gatom_progman16;
    return atom;
}

static ATOM topic32_16(ATOM atom)
{
    init_atom();
    if (atom_progman16 == atom)
        return atom_progman;
    if (gatom_progman16 == atom)
        return gatom_progman;
    return atom;
}
/**********************************************************************
 *	     WINPROC_CallProc16To32A
 */
LRESULT WINPROC_CallProc16To32A( winproc_callback_t callback, HWND16 hwnd, UINT16 msg,
                                 WPARAM16 wParam, LPARAM lParam, LRESULT *result, void *arg )
{
    LRESULT ret = 0;
    HWND hwnd32 = WIN_Handle32( hwnd );

    const char *msg_str = message_to_str(msg);
    TRACE("(%p, %04X, %s(%04X), %04X, %08X)\n", callback, hwnd, msg_str, msg, wParam, lParam);
    if (isListBox(hwnd, hwnd32) || (call_window_proc_callback == callback && is_listbox_wndproc(arg)))
	{
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_LISTBOX;
		ret = listbox_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
		if (f)
			return ret;
	}
    if (isComboBox(hwnd, hwnd32) || (call_window_proc_callback == callback && is_combobox_wndproc(arg)))
    {
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_COMBOBOX;
        ret = combo_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
        if (f)
            return ret;
    }
    if (isButton(hwnd, hwnd32) || (call_window_proc_callback == callback && is_button_wndproc(arg)))
    {
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_BUTTON;
        ret = button_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
        if (f)
            return ret;
    }
    if (isEdit(hwnd, hwnd32) || (call_window_proc_callback == callback && is_edit_wndproc(arg)))
    {
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_EDIT;
        ret = edit_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
        if (f)
            return ret;
    }
    if (isScrollBar(hwnd, hwnd32) || (call_window_proc_callback == callback && is_scrollbar_wndproc(arg)))
    {
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_SCROLLBAR;
        ret = scrollbar_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
        if (f)
            return ret;
    }
    if (isStatic(hwnd, hwnd32) || (call_window_proc_callback == callback && is_static_wndproc(arg)))
    {
        BOOL f;
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_STATIC;
        ret = static_proc_CallProc16To32A(callback, hwnd32, msg, wParam, lParam, 0, result, arg, &f);
        if (f)
            return ret;
    }
    switch(msg)
    {
    case WM_NCCREATE:
    case WM_CREATE:
        {
            CREATESTRUCT16 *cs16 = MapSL(lParam);
            CREATESTRUCTA cs;
            MDICREATESTRUCTA mdi_cs;
            BOOL mdiclient = GetWindowLongW(hwnd32, GWL_EXSTYLE) & WS_EX_MDICHILD || is_mdiclient(hwnd, hwnd32) || (call_window_proc_callback == callback && is_mdiclient_wndproc(arg));

            CREATESTRUCT16to32A( hwnd32, cs16, &cs );
            if (mdiclient)
            {
                MDICREATESTRUCT16 *mdi_cs16 = MapSL(cs16->lpCreateParams);
                MDICREATESTRUCT16to32A(mdi_cs16, &mdi_cs);
                cs.lpCreateParams = &mdi_cs;
            }
            ret = callback( hwnd32, msg, wParam, (LPARAM)&cs, result, arg );
            if (mdiclient)
                cs.lpCreateParams = cs16->lpCreateParams;
            CREATESTRUCT32Ato16( hwnd32, &cs, cs16 );
        }
        break;
    case WM_MDICREATE:
        {
            MDICREATESTRUCT16 *cs16 = MapSL(lParam);
            MDICREATESTRUCTA cs;

            MDICREATESTRUCT16to32A( cs16, &cs );
            ret = callback( hwnd32, msg, wParam, (LPARAM)&cs, result, arg );
			*result = HWND_16((HWND)*result);
            MDICREATESTRUCT32Ato16( &cs, cs16 );
        }
        break;
    case WM_MDIACTIVATE:
        if (lParam)
            ret = callback( hwnd32, msg, (WPARAM)WIN_Handle32( HIWORD(lParam) ),
                            (LPARAM)WIN_Handle32( LOWORD(lParam) ), result, arg );
        else /* message sent to MDI client */
            ret = callback( hwnd32, msg, wParam, lParam, result, arg );
        break;
    case WM_MDIGETACTIVE:
        {
            BOOL maximized = FALSE;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&maximized, result, arg );
            *result = MAKELRESULT( LOWORD(HWND_16((HWND)*result)), maximized );
        }
        break;
    case WM_MDISETMENU:
        {
            void SetWindowHMenu16(WORD hWnd16, HMENU16 hinst16);
            ret = callback( hwnd32, wParam ? WM_MDIREFRESHMENU : WM_MDISETMENU,
                            (WPARAM)HMENU_32(LOWORD(lParam)), (LPARAM)HMENU_32(HIWORD(lParam)),
                            result, arg );
            if (ret && LOWORD(lParam))
                SetWindowHMenu16(GetParent16(hwnd), LOWORD(lParam));
            *result = HMENU_16((HMENU)*result);
        }
        break;
    case WM_MDIRESTORE:
        ret = callback( hwnd32, msg, (WPARAM)WIN_Handle32(wParam), lParam, result, arg );
        *result = 0;
        break;
    case WM_MDINEXT:
        ret = callback(hwnd32, msg, (WPARAM)WIN_Handle32(wParam), lParam, result, arg);
        break;
    case WM_GETMINMAXINFO:
        {
            MINMAXINFO16 *mmi16 = MapSL(lParam);
            MINMAXINFO mmi;

            MINMAXINFO16to32( mmi16, &mmi );
            ret = callback( hwnd32, msg, wParam, (LPARAM)&mmi, result, arg );
            MINMAXINFO32to16( &mmi, mmi16 );
        }
        break;
    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS16 *winpos16 = MapSL(lParam);
            WINDOWPOS winpos;

            WINDOWPOS16to32( winpos16, &winpos );
            ret = callback( hwnd32, msg, wParam, (LPARAM)&winpos, result, arg );
            WINDOWPOS32to16( &winpos, winpos16 );
        }
        break;
    case WM_NCCALCSIZE:
        {
            NCCALCSIZE_PARAMS16 *nc16 = MapSL(lParam);
            NCCALCSIZE_PARAMS nc;
            WINDOWPOS winpos;

            RECT16to32( &nc16->rgrc[0], &nc.rgrc[0] );
            if (wParam)
            {
                RECT16to32( &nc16->rgrc[1], &nc.rgrc[1] );
                RECT16to32( &nc16->rgrc[2], &nc.rgrc[2] );
                WINDOWPOS16to32( MapSL(nc16->lppos), &winpos );
                nc.lppos = &winpos;
            }
            ret = callback( hwnd32, msg, wParam, (LPARAM)&nc, result, arg );
            RECT32to16( &nc.rgrc[0], &nc16->rgrc[0] );
            if (wParam)
            {
                RECT32to16( &nc.rgrc[1], &nc16->rgrc[1] );
                RECT32to16( &nc.rgrc[2], &nc16->rgrc[2] );
                WINDOWPOS32to16( &winpos, MapSL(nc16->lppos) );
            }
        }
        break;
    case WM_COMPAREITEM:
        {
            COMPAREITEMSTRUCT16* cis16 = MapSL(lParam);
            COMPAREITEMSTRUCT cis;
            cis.CtlType    = cis16->CtlType;
            cis.CtlID      = cis16->CtlID;
            cis.hwndItem   = WIN_Handle32( cis16->hwndItem );
            cis.itemID1    = cis16->itemID1;
            cis.itemData1  = cis16->itemData1;
            cis.itemID2    = cis16->itemID2;
            cis.itemData2  = cis16->itemData2;
            cis.dwLocaleId = 0;  /* FIXME */
            ret = callback( hwnd32, msg, wParam, (LPARAM)&cis, result, arg );
        }
        break;
    case WM_DELETEITEM:
        {
            DELETEITEMSTRUCT16* dis16 = MapSL(lParam);
            DELETEITEMSTRUCT dis;
            dis.CtlType  = dis16->CtlType;
            dis.CtlID    = dis16->CtlID;
            dis.hwndItem = WIN_Handle32( dis16->hwndItem );
            dis.itemData = dis16->itemData;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&dis, result, arg );
        }
        break;
    case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT16* mis16 = MapSL(lParam);
            MEASUREITEMSTRUCT mis;
            mis.CtlType    = mis16->CtlType;
            mis.CtlID      = mis16->CtlID;
            mis.itemID     = mis16->itemID;
            mis.itemWidth  = mis16->itemWidth;
            mis.itemHeight = mis16->itemHeight;
            mis.itemData   = mis16->itemData;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&mis, result, arg );
            mis16->itemWidth  = (UINT16)mis.itemWidth;
            mis16->itemHeight = (UINT16)mis.itemHeight;
        }
        break;
    case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT16* dis16 = MapSL(lParam);
            DRAWITEMSTRUCT dis;
            dis.CtlType       = dis16->CtlType;
            dis.CtlID         = dis16->CtlID;
            dis.itemID        = dis16->itemID == 0xFFFF ? ~0 : dis16->itemID;
            dis.itemAction    = dis16->itemAction;
            dis.itemState     = dis16->itemState;
            dis.hwndItem      = (dis.CtlType == ODT_MENU) ? (HWND)HMENU_32(dis16->hwndItem)
                                                          : WIN_Handle32( dis16->hwndItem );
            dis.hDC           = HDC_32(dis16->hDC);
            dis.itemData      = dis16->itemData;
            dis.rcItem.left   = dis16->rcItem.left;
            dis.rcItem.top    = dis16->rcItem.top;
            dis.rcItem.right  = dis16->rcItem.right;
            dis.rcItem.bottom = dis16->rcItem.bottom;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&dis, result, arg );
        }
        break;
    case WM_COPYDATA:
        {
            COPYDATASTRUCT16 *cds16 = MapSL(lParam);
            COPYDATASTRUCT cds;
            cds.dwData = cds16->dwData;
            cds.cbData = cds16->cbData;
            cds.lpData = MapSL(cds16->lpData);
            ret = callback( hwnd32, msg, wParam, (LPARAM)&cds, result, arg );
        }
        break;
    case WM_GETDLGCODE:
        if (lParam)
        {
            MSG16 *msg16 = MapSL(lParam);
            MSG msg32;
            msg32.hwnd    = WIN_Handle32( msg16->hwnd );
            msg32.message = msg16->message;
            msg32.wParam  = msg16->wParam;
            msg32.lParam  = msg16->lParam;
            msg32.time    = msg16->time;
            msg32.pt.x    = msg16->pt.x;
            msg32.pt.y    = msg16->pt.y;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&msg32, result, arg );
        }
        else
            ret = callback( hwnd32, msg, wParam, lParam, result, arg );
        break;
    case WM_NEXTMENU:
        {
            MDINEXTMENU next;
            next.hmenuIn   = (HMENU)lParam;
            next.hmenuNext = 0;
            next.hwndNext  = 0;
            ret = callback( hwnd32, msg, wParam, (LPARAM)&next, result, arg );
            *result = MAKELONG( HMENU_16(next.hmenuNext), HWND_16(next.hwndNext) );
        }
        break;
    case WM_ACTIVATE:
    case WM_CHARTOITEM:
    case WM_COMMAND:
    case WM_VKEYTOITEM:
        ret = callback( hwnd32, msg, MAKEWPARAM( wParam, HIWORD(lParam) ),
                        (LPARAM)WIN_Handle32( LOWORD(lParam) ), result, arg );
        break;
    case WM_HSCROLL:
    case WM_VSCROLL:
        ret = callback( hwnd32, msg, MAKEWPARAM( wParam, LOWORD(lParam) ),
                        (LPARAM)WIN_Handle32( HIWORD(lParam) ), result, arg );
        break;
    case WM_CTLCOLOR:
        if (HIWORD(lParam) <= CTLCOLOR_STATIC)
            ret = callback( hwnd32, WM_CTLCOLORMSGBOX + HIWORD(lParam),
                            (WPARAM)HDC_32(wParam), (LPARAM)WIN_Handle32( LOWORD(lParam) ),
                            result, arg );
        *result = HBRUSH_16((HBRUSH)*result);
        break;
    case WM_GETTEXT:
    case WM_SETTEXT:
    case WM_WININICHANGE:
    case WM_DEVMODECHANGE:
    case WM_ASKCBFORMATNAME:
    case WM_NOTIFY:
        ret = callback( hwnd32, msg, wParam, (LPARAM)MapSL(lParam), result, arg );
        break;
    case WM_MENUCHAR:
        ret = callback( hwnd32, msg, MAKEWPARAM( wParam, LOWORD(lParam) ),
                        (LPARAM)HMENU_32(HIWORD(lParam)), result, arg );
        break;
    case WM_MENUSELECT:
        if((LOWORD(lParam) & MF_POPUP) && (LOWORD(lParam) != 0xFFFF))
        {
            HMENU hmenu = HMENU_32(HIWORD(lParam));
            int pos = find_sub_menu( &hmenu, wParam );
            if (pos == -1) pos = 0;
            wParam = pos;
        }
        ret = callback( hwnd32, msg, MAKEWPARAM( wParam, LOWORD(lParam) ),
                        (LPARAM)HMENU_32(HIWORD(lParam)), result, arg );
        break;
    case WM_PARENTNOTIFY:
        if ((wParam == WM_CREATE) || (wParam == WM_DESTROY))
            ret = callback( hwnd32, msg, MAKEWPARAM( wParam, HIWORD(lParam) ),
                            (LPARAM)WIN_Handle32( LOWORD(lParam) ), result, arg );
        else
            ret = callback( hwnd32, msg, wParam, lParam, result, arg );
        break;
    case WM_ACTIVATEAPP:
        /* We need this when SetActiveWindow sends a Sendmessage16() to
         * a 32-bit window. Might be superfluous with 32-bit interprocess
         * message queues. */
        if (lParam) lParam = HTASK_32(lParam);
        ret = callback( hwnd32, msg, wParam, lParam, result, arg );
        break;
    case WM_DDE_INITIATE:
        ret = callback(hwnd32, msg, (WPARAM)WIN_Handle32(wParam), MAKELONG(service16_32(LOWORD(lParam)), topic16_32(HIWORD(lParam))), result, arg);
        break;
    case WM_DDE_TERMINATE:
        ret = callback(hwnd32, msg, (WPARAM)WIN_Handle32(wParam), lParam, result, arg);
        break;
    case WM_DDE_REQUEST:
    case WM_DDE_UNADVISE:
        ret = callback(hwnd32, msg, (WPARAM)WIN_Handle32(wParam), MAKELONG(LOWORD(lParam), topic16_32(HIWORD(lParam))), result, arg);
        break;
    case WM_DDE_ADVISE:
    case WM_DDE_DATA:
    case WM_DDE_POKE:
        {
            HANDLE16 lo16 = LOWORD(lParam);
            UINT_PTR lo32 = 0;
            if (lo16 && !(lo32 = convert_handle_16_to_32(lo16, GMEM_DDESHARE))) break;
            lParam = PackDDElParam( msg, lo32, topic16_32(HIWORD(lParam)) );
            ret = callback( hwnd32, msg, (WPARAM)WIN_Handle32(wParam), lParam, result, arg );
        }
        break; /* FIXME don't know how to free allocated memory (handle)  !! */
    case WM_DDE_ACK:
        {
            UINT_PTR lo = LOWORD(lParam);
            UINT_PTR hi = HIWORD(lParam);
            int flag = 0;
            char buf[256];

            if (hi >= 0xc000 && GlobalGetAtomNameA(hi, buf, 256) > 0) flag |= 1;
            if (GlobalSize16(hi) != 0) flag |= 2;
            lo = lo; /* atom or flag */
            switch (flag)
            {
            case 0:
                if (hi)
                {
                    WARN("DDE_ACK: neither atom nor handle!!!\n");
                    hi = 0;
                }
                break;
            case 1:
                hi = topic16_32(hi);
                break; /* atom, nothing to do */
            case 3:
                WARN("DDE_ACK: %lx both atom and handle... choosing handle\n", hi);
                /* fall through */
            case 2:
                hi = convert_handle_16_to_32(hi, GMEM_DDESHARE);
                break;
            }
            lParam = PackDDElParam( WM_DDE_ACK, lo, hi );
            ret = callback( hwnd32, msg, (WPARAM)WIN_Handle32(wParam), lParam, result, arg );
        }
        break; /* FIXME don't know how to free allocated memory (handle) !! */
    case WM_DDE_EXECUTE:
        lParam = convert_handle_16_to_32( HIWORD(lParam), GMEM_DDESHARE );
        ret = callback( hwnd32, msg, (WPARAM)HWND_32(wParam), lParam, result, arg );
        break; /* FIXME don't know how to free allocated memory (handle) !! */
    case WM_PAINTCLIPBOARD:
    case WM_SIZECLIPBOARD:
        FIXME_(msg)( "message %04x needs translation\n", msg );
        break;
	case WM_NCPAINT:
        ret = callback(hwnd32, msg, (WPARAM)HRGN_32(wParam), lParam, result, arg);
		break;
    case WM_ERASEBKGND:
        ret = callback(hwnd32, msg, (WPARAM)HDC_32(wParam), lParam, result, arg);
        break;
    case WM_SETFONT:
        ret = callback(hwnd32, msg, (WPARAM)HFONT_32(wParam), lParam, result, arg);
        break;
    case WM_MOUSEACTIVATE:
        ret = callback(hwnd32, msg, (WPARAM)HWND_32(wParam), lParam, result, arg);
        break;
    case WM_SETFOCUS:
    case WM_SETCURSOR:
        ret = callback(hwnd32, msg, (WPARAM)HWND_32(wParam), lParam, result, arg);
        break;
    case WM_INITMENU:
    case WM_INITMENUPOPUP:
    case WM_UNINITMENUPOPUP:
        ret = callback(hwnd32, msg, (WPARAM)HMENU_32(wParam), lParam, result, arg);
        break;
    case WM_THEMECHANGED:
        ret = callback(hwnd32, msg, (INT)((INT16)wParam), lParam, result, arg);
        break;
    case WM_NCACTIVATE:
        ret = callback(hwnd32, msg, wParam, (LPARAM)HWND_32((HWND16)lParam), result, arg);
        break;
    case WM_GETICON:
        ret = callback(hwnd32, msg, wParam, lParam, result, arg);
        *result = get_icon_16((HICON)*result);
        break;
    case WM_SETICON:
        ret = callback(hwnd32, msg, wParam, (LPARAM)get_icon_32((HICON16)lParam), result, arg);
        break;
    case WM_QUERYDRAGICON:
        ret = callback(hwnd32, msg, wParam, lParam, result, arg);
        *result = get_icon_16((HICON)*result);
        break;

    case WM_QUERYDROPOBJECT:
    case WM_DROPOBJECT:
    case WM_DRAGLOOP:
    case WM_DRAGSELECT:
    case WM_DRAGMOVE:
    {
        LPDROPSTRUCT16 lpds16 = (LPDROPSTRUCT16)MapSL(lParam);
        DROPSTRUCT ds32;
        HICON16 icon16;
        DROPSTRUCT16_32(&ds32, lpds16);
        ret = callback(hwnd32, msg, wParam, (LPARAM)&ds32, result, arg);
        DROPSTRUCT32_16(&ds32, lpds16);
        icon16 = get_icon_16((HICON)*result);
        if (icon16)
        {
            *result = (LRESULT)icon16;
        }
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_SYSTIMER:
    case WM_TIMER:
        if (lParam != TIMER32_LPARAM)
        {
            ret = callback(hwnd32, msg, wParam, lParam, result, arg);
            break;
        }
        else
        {
            if (wParam < TIMER32_WRAP_SIZE)
            {
                ret = callback(hwnd32, msg, timer32[wParam].wParam, timer32[wParam].lParam, result, arg);
                timer32[wParam].ref = FALSE;
                timer32_count--;
            }
            else
            {
                ret = callback(hwnd32, msg, wParam, lParam, result, arg);
            }
            break;
        }
    case WM_GETFONT:
        ret = callback(hwnd32, msg, wParam, lParam, result, arg);
        *result = HFONT_16((HFONT)*result);
        break;
    case WM_KILLFOCUS:
    case WM_INITDIALOG:
    case WM_MDIDESTROY:
    case WM_MDIMAXIMIZE:
        ret = callback(hwnd32, msg, (WPARAM)HWND_32((HWND16)wParam), lParam, result, arg);
        break;
    case WM_ENTERIDLE:
        ret = callback(hwnd32, msg, wParam, (LPARAM)HWND_32((HWND16)lParam), result, arg);
        break;
    case WM_UPDATEUISTATE:
        ret = callback(hwnd32, msg, lParam, lParam, result, arg);
        break;
    case WM_PRINT:
    case WM_PRINTCLIENT:
    case WM_NCUAHDRAWFRAME: /* undocumented wparam: hdc */
        ret = callback(hwnd32, msg, (WPARAM)HDC_32(wParam), lParam, result, arg);
        break;
    default:
    {
        if (msg != WM_NULL && msg == drag_list_message)
        {
            DRAGLISTINFO16 *di = (DRAGLISTINFO16*)MapSL(lParam);
            DRAGLISTINFO di32;
            di32.hWnd = HWND_32(di->hWnd);
            di32.uNotification = di->uNotification;
            di32.ptCursor.x = di->ptCursor.x;
            di32.ptCursor.y = di->ptCursor.y;
            ret = callback(hwnd32, msg, wParam, (LPARAM)&di32, result, arg);
            break;
        }
        ret = callback(hwnd32, msg, wParam, lParam, result, arg);
        break;
    }
    }
    return ret;
}

#include <uxtheme.h>
#include <vssym32.h>
#include "../mmsystem/winemm16.h"

void InitWndProc16(HWND hWnd, HWND16 hWnd16);
/**********************************************************************
 *	     WINPROC_CallProc32ATo16
 *
 * Call a 16-bit window procedure, translating the 32-bit args.
 */
LRESULT WINPROC_CallProc32ATo16( winproc_callback16_t callback, HWND hwnd, UINT msg,
                                 WPARAM wParam, LPARAM lParam, LRESULT *result, void *arg )
{
    LRESULT ret = 0;

    HWND16 hwnd16 = HWND_16(hwnd);
    const char *msg_str = message_to_str(msg);
    TRACE("(%p, %p, %s(%04X), %08X, %08X)\n", callback, hwnd, msg_str, msg, wParam, lParam);
    if (BM_GETCHECK <= msg && msg <= BM_SETDONTCLICK)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_BUTTON;
    }
    if (LB_ADDSTRING <= msg && msg <= LB_MSGMAX)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_LISTBOX;
    }
    if (STM_SETICON <= msg && msg <= STM_MSGMAX)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_STATIC;
    }
    if (SBM_SETPOS <= msg && msg <= SBM_GETSCROLLBARINFO)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_SCROLLBAR;
    }
    if (EM_GETSEL <= msg && msg <= EM_ENABLEFEATURE)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_EDIT;
    }
    if (CB_GETEDITSEL <= msg && msg <= CB_MSGMAX)
    {
        window_type_table[hwnd16] = (BYTE)WINDOW_TYPE_COMBOBOX;
    }
    switch (msg)
    {
    case WM_NCCREATE:
    case WM_CREATE:
        {
            InitWndProc16(hwnd, HWND_16(hwnd));
            CREATESTRUCTA *cs32 = (CREATESTRUCTA *)lParam;
            CREATESTRUCT16 cs;
            MDICREATESTRUCT16 mdi_cs16;
            BOOL mdi_child = (GetWindowLongW(hwnd, GWL_EXSTYLE) & WS_EX_MDICHILD);

            CREATESTRUCT32Ato16( hwnd, cs32, &cs );
            cs.lpszName  = MapLS( cs32->lpszName );
            cs.lpszClass = MapLS( win16classname(cs32->lpszClass) );
            if (mdi_child)
            {
                MDICREATESTRUCTA *mdi_cs = cs32->lpCreateParams;
                MDICREATESTRUCT32Ato16( mdi_cs, &mdi_cs16 );
                mdi_cs16.szTitle = MapLS( mdi_cs->szTitle );
                mdi_cs16.szClass = MapLS( win16classname(mdi_cs->szClass) );
                cs.lpCreateParams = MapLS( &mdi_cs16 );
            }
            lParam = MapLS( &cs );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            UnMapLS( cs.lpszName );
            UnMapLS( cs.lpszClass );
            if (mdi_child)
            {
                UnMapLS( cs.lpCreateParams );
                UnMapLS( mdi_cs16.szTitle );
                UnMapLS( mdi_cs16.szClass );
            }
        }
        break;
    case WM_MDICREATE:
        {
            MDICREATESTRUCTA *cs32 = (MDICREATESTRUCTA *)lParam;
            MDICREATESTRUCT16 cs;

            MDICREATESTRUCT32Ato16( cs32, &cs );
            cs.szTitle = MapLS( cs32->szTitle );
            cs.szClass = MapLS( win16classname(cs32->szClass) );
            lParam = MapLS( &cs );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            UnMapLS( cs.szTitle );
            UnMapLS( cs.szClass );
        }
        break;
    case WM_MDIACTIVATE:
        if (GetWindowLongW( hwnd, GWL_EXSTYLE ) & WS_EX_MDICHILD)
            ret = callback( HWND_16(hwnd), msg, ((HWND)lParam == hwnd),
                            MAKELPARAM( HWND_16((HWND)lParam), HWND_16((HWND)wParam) ), result, arg );
        else
            ret = callback( HWND_16(hwnd), msg, (WPARAM16)HWND_16( (HWND)wParam ), 0, result, arg );
        break;
    case WM_MDIGETACTIVE:
        ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        if (lParam) *(BOOL *)lParam = (BOOL16)HIWORD(*result);
        *result = (LRESULT)WIN_Handle32( LOWORD(*result) );
        break;
    case WM_MDISETMENU:
        ret = callback( HWND_16(hwnd), msg, (lParam == 0),
                        MAKELPARAM( LOWORD(wParam), LOWORD(lParam) ), result, arg );
        *result = (LRESULT)HMENU_32((HMENU16)*result);
        break;
    case WM_MDINEXT:
        ret = callback(HWND_16(hwnd), msg, HWND_16(wParam), lParam, result, arg);
        break;
    case WM_GETMINMAXINFO:
        {
            MINMAXINFO *mmi32 = (MINMAXINFO *)lParam;
            MINMAXINFO16 mmi;

            MINMAXINFO32to16( mmi32, &mmi );
            lParam = MapLS( &mmi );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            MINMAXINFO16to32( &mmi, mmi32 );
        }
        break;
    case WM_NCCALCSIZE:
        {
            NCCALCSIZE_PARAMS *nc32 = (NCCALCSIZE_PARAMS *)lParam;
            NCCALCSIZE_PARAMS16 nc;
            WINDOWPOS16 winpos;

            RECT32to16( &nc32->rgrc[0], &nc.rgrc[0] );
            if (wParam)
            {
                RECT32to16( &nc32->rgrc[1], &nc.rgrc[1] );
                RECT32to16( &nc32->rgrc[2], &nc.rgrc[2] );
                WINDOWPOS32to16( nc32->lppos, &winpos );
                nc.lppos = MapLS( &winpos );
            }
            lParam = MapLS( &nc );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            RECT16to32( &nc.rgrc[0], &nc32->rgrc[0] );
            if (wParam)
            {
                RECT16to32( &nc.rgrc[1], &nc32->rgrc[1] );
                RECT16to32( &nc.rgrc[2], &nc32->rgrc[2] );
                WINDOWPOS16to32( &winpos, nc32->lppos );
                UnMapLS( nc.lppos );
            }
        }
        break;
    case WM_WINDOWPOSCHANGING:
    case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS *winpos32 = (WINDOWPOS *)lParam;
            WINDOWPOS16 winpos;

            WINDOWPOS32to16( winpos32, &winpos );
            lParam = MapLS( &winpos );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            WINDOWPOS16to32( &winpos, winpos32 );
        }
        break;
    case WM_COMPAREITEM:
        {
            COMPAREITEMSTRUCT *cis32 = (COMPAREITEMSTRUCT *)lParam;
            COMPAREITEMSTRUCT16 cis;
            cis.CtlType    = cis32->CtlType;
            cis.CtlID      = cis32->CtlID;
            cis.hwndItem   = HWND_16( cis32->hwndItem );
            cis.itemID1    = cis32->itemID1;
            cis.itemData1  = cis32->itemData1;
            cis.itemID2    = cis32->itemID2;
            cis.itemData2  = cis32->itemData2;
            lParam = MapLS( &cis );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
        }
        break;
    case WM_DELETEITEM:
        {
            DELETEITEMSTRUCT *dis32 = (DELETEITEMSTRUCT *)lParam;
            DELETEITEMSTRUCT16 dis;
            dis.CtlType  = dis32->CtlType;
            dis.CtlID    = dis32->CtlID;
            dis.itemID   = dis32->itemID;
            dis.hwndItem = (dis.CtlType == ODT_MENU) ? (HWND16)LOWORD(dis32->hwndItem)
                                                     : HWND_16( dis32->hwndItem );
            dis.itemData = dis32->itemData;
            lParam = MapLS( &dis );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
        }
        break;
    case WM_DRAWITEM:
        {
            DRAWITEMSTRUCT *dis32 = (DRAWITEMSTRUCT *)lParam;
            DRAWITEMSTRUCT16 dis;
            dis.CtlType       = dis32->CtlType;
            dis.CtlID         = dis32->CtlID;
            dis.itemID        = dis32->itemID;
            dis.itemAction    = dis32->itemAction;
            dis.itemState     = dis32->itemState;
            dis.hwndItem      = HWND_16( dis32->hwndItem );
            dis.hDC           = HDC_16(dis32->hDC);
            dis.itemData      = dis32->itemData;
            dis.rcItem.left   = dis32->rcItem.left;
            dis.rcItem.top    = dis32->rcItem.top;
            dis.rcItem.right  = dis32->rcItem.right;
            dis.rcItem.bottom = dis32->rcItem.bottom;
            lParam = MapLS( &dis );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
        }
        break;
    case WM_MEASUREITEM:
        {
            MEASUREITEMSTRUCT *mis32 = (MEASUREITEMSTRUCT *)lParam;
            MEASUREITEMSTRUCT16 mis;
            mis.CtlType    = mis32->CtlType;
            mis.CtlID      = mis32->CtlID;
            mis.itemID     = mis32->itemID;
            mis.itemWidth  = mis32->itemWidth;
            mis.itemHeight = mis32->itemHeight;
            mis.itemData   = mis32->itemData;
            lParam = MapLS( &mis );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            mis32->itemWidth  = (INT16)mis.itemWidth;
            mis32->itemHeight = mis.itemHeight;
        }
        break;
    case WM_COPYDATA:
        {
            COPYDATASTRUCT *cds32 = (COPYDATASTRUCT *)lParam;
            COPYDATASTRUCT16 cds;

            cds.dwData = cds32->dwData;
            cds.cbData = cds32->cbData;
            cds.lpData = MapLS( cds32->lpData );
            lParam = MapLS( &cds );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
            UnMapLS( cds.lpData );
        }
        break;
    case WM_GETDLGCODE:
        if (lParam)
        {
            MSG *msg32 = (MSG *)lParam;
            MSG16 msg16;

            msg16.hwnd    = HWND_16( msg32->hwnd );
            msg16.message = msg32->message;
            msg16.wParam  = msg32->wParam;
            msg16.lParam  = msg32->lParam;
            msg16.time    = msg32->time;
            msg16.pt.x    = msg32->pt.x;
            msg16.pt.y    = msg32->pt.y;
            lParam = MapLS( &msg16 );
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
            UnMapLS( lParam );
        }
        else
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        break;
    case WM_NEXTMENU:
        {
            MDINEXTMENU *next = (MDINEXTMENU *)lParam;
            ret = callback( HWND_16(hwnd), msg, wParam, (LPARAM)next->hmenuIn, result, arg );
            next->hmenuNext = HMENU_32( LOWORD(*result) );
            next->hwndNext  = WIN_Handle32( HIWORD(*result) );
            *result = 0;
        }
        break;
    case WM_GETTEXT:
    case WM_ASKCBFORMATNAME:
        wParam = min( wParam, 0xff80 ); /* Must be < 64K */
        /* fall through */
    case WM_NOTIFY:
    case WM_SETTEXT:
    case WM_WININICHANGE:
    case WM_DEVMODECHANGE:
        lParam = MapLS( (void *)lParam );
        ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        UnMapLS( lParam );
        break;
    case WM_ACTIVATE:
    case WM_CHARTOITEM:
    case WM_COMMAND:
    case WM_VKEYTOITEM:
        {
            if (HIWORD(lParam) == 0)
                lParam = MAKELPARAM(LOWORD(lParam), HIWORD(wParam));
            else
                lParam = MAKELPARAM(HWND_16((HWND)lParam), HIWORD(wParam));
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        }
        break;
    case WM_HSCROLL:
    case WM_VSCROLL:
        ret = callback( HWND_16(hwnd), msg, wParam, MAKELPARAM( HIWORD(wParam), HWND_16((HWND)lParam) ),
                        result, arg );
        break;
    case WM_CTLCOLORMSGBOX:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSCROLLBAR:
    case WM_CTLCOLORSTATIC:
        ret = callback( HWND_16(hwnd), WM_CTLCOLOR, HDC_16((HDC)wParam),
                        MAKELPARAM( HWND_16((HWND)lParam), msg - WM_CTLCOLORMSGBOX ), result, arg );
        *result = HBRUSH_32((HBRUSH16)*result);
        break;
    case WM_MENUSELECT:
        if(HIWORD(wParam) & MF_POPUP)
        {
            HMENU hmenu;
            if ((HIWORD(wParam) != 0xffff) || lParam)
            {
                if ((hmenu = GetSubMenu( (HMENU)lParam, LOWORD(wParam) )))
                {
                    ret = callback( HWND_16(hwnd), msg, HMENU_16(hmenu),
                                    MAKELPARAM( HIWORD(wParam), (HMENU16)HMENU_16((HMENU)lParam) ), result, arg );
                    break;
                }
            }
        }
        /* fall through */
    case WM_MENUCHAR:
        ret = callback( HWND_16(hwnd), msg, wParam,
                        MAKELPARAM( HIWORD(wParam), (HMENU16)HMENU_16((HMENU)lParam) ), result, arg );
        break;
    case WM_PARENTNOTIFY:
        if ((LOWORD(wParam) == WM_CREATE) || (LOWORD(wParam) == WM_DESTROY))
            ret = callback( HWND_16(hwnd), msg, wParam,
                            MAKELPARAM( HWND_16((HWND)lParam), HIWORD(wParam) ), result, arg );
        else
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        break;
    case WM_ACTIVATEAPP:
        ret = callback( HWND_16(hwnd), msg, wParam, HTASK_16( lParam ), result, arg );
        break;
    case WM_PAINT:
        if (IsIconic( hwnd ) && GetClassLongPtrW( hwnd, GCLP_HICON ))
            ret = callback( HWND_16(hwnd), WM_PAINTICON, 1, lParam, result, arg );
        else
            ret = callback( HWND_16(hwnd), WM_PAINT, wParam, lParam, result, arg );
        break;
	case WM_NCPAINT:
        ret = callback(HWND_16(hwnd), WM_NCPAINT, HRGN_16((HRGN)wParam), lParam, result, arg);
		break;
	case WM_NCACTIVATE:
		ret = callback(HWND_16(hwnd), WM_NCACTIVATE, wParam, HWND_16((HWND)lParam), result, arg);
		break;
    case WM_ERASEBKGND:
        if (IsIconic( hwnd ) && GetClassLongPtrW( hwnd, GCLP_HICON )) msg = WM_ICONERASEBKGND;
        ret = callback( HWND_16(hwnd), msg, HDC_16((HDC)wParam), lParam, result, arg );
        break;
    case WM_SETFONT:
        ret = callback(HWND_16(hwnd), msg, HFONT_16((HFONT)wParam), lParam, result, arg);
        break;
    case WM_DDE_INITIATE:
        ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam), MAKELONG(service32_16(LOWORD(lParam)), topic32_16(HIWORD(lParam))), result, arg );
        break;
    case WM_DDE_TERMINATE:
        ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam), lParam, result, arg );
        break;
    case WM_DDE_UNADVISE:
    case WM_DDE_REQUEST:
        ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam), MAKELONG(LOWORD(lParam), topic32_16(HIWORD(lParam))), result, arg );
        break;
    case WM_DDE_ADVISE:
    case WM_DDE_DATA:
    case WM_DDE_POKE:
        {
            UINT_PTR lo32, hi;
            HANDLE16 lo16 = 0;

            UnpackDDElParam( msg, lParam, &lo32, &hi );
            if (lo32 && !(lo16 = convert_handle_32_to_16(lo32, GMEM_DDESHARE))) break;
            lo16 = topic32_16(lo16);
            ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam),
                            MAKELPARAM(lo16, hi), result, arg );
        }
        break; /* FIXME don't know how to free allocated memory (handle)  !! */
    case WM_DDE_ACK:
        {
            UINT_PTR lo, hi;
            int flag = 0;
            char buf[256];

            // UnpackDDElParam fails with HEAP_FAILURE_INVALID_ARGUMENT if lParam doesn't have bit 2 set
            if (!(lParam & 4) || !UnpackDDElParam( msg, lParam, &lo, &hi ))
            {
                lo = LOWORD(lParam);
                hi = HIWORD(lParam);
            }

            if (hi >= 0xc000 && GlobalGetAtomNameA((ATOM)hi, buf, sizeof(buf)) > 0) flag |= 1;
            if (HIWORD(hi) && GlobalSize((HANDLE)hi) != 0) flag |= 2;
            lo = lo; /* atom or flag */
            switch (flag)
            {
            case 0:
                if (hi)
                {
                    WARN("DDE_ACK: neither atom nor handle!!!\n");
                    hi = 0;
                }
                break;
            case 1:
                hi = topic32_16(hi);
                break; /* atom, nothing to do */
            case 3:
                WARN("DDE_ACK: %lx both atom and handle... choosing handle\n", hi);
                /* fall through */
            case 2:
                hi = convert_handle_32_to_16(hi, GMEM_DDESHARE);
                break;
            }
            ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam),
                            MAKELPARAM(lo, hi), result, arg );
        }
        break; /* FIXME don't know how to free allocated memory (handle) !! */
    case WM_DDE_EXECUTE:
        lParam = MAKELPARAM( 0, convert_handle_32_to_16( lParam, GMEM_DDESHARE ));
        ret = callback( HWND_16(hwnd), msg, HWND_16((HWND)wParam), lParam, result, arg );
        break; /* FIXME don't know how to free allocated memory (handle) !! */
    case SBM_SETRANGE:
        ret = callback( HWND_16(hwnd), SBM_SETRANGE16, 0, MAKELPARAM(wParam, lParam), result, arg );
        break;
    case SBM_GETRANGE:
        ret = callback( HWND_16(hwnd), SBM_GETRANGE16, wParam, lParam, result, arg );
        *(LPINT)wParam = LOWORD(*result);
        *(LPINT)lParam = HIWORD(*result);
        break;
    case BM_GETCHECK:
    case BM_SETCHECK:
    case BM_GETSTATE:
    case BM_SETSTATE:
    case BM_SETSTYLE:
        ret = callback( HWND_16(hwnd), msg + BM_GETCHECK16 - BM_GETCHECK, wParam, lParam, result, arg );
        break;
    case EM_GETRECT:
    {
        lParam = MapLS(lParam);
        ret = callback(HWND_16(hwnd), msg + EM_GETSEL16 - EM_GETSEL, wParam, lParam, result, arg);
        UnMapLS(lParam);
    }
    break;
    case EM_GETLINE:
        ret = callback(HWND_16(hwnd), msg + EM_GETSEL16 - EM_GETSEL, wParam, (LPARAM)MapLS(lParam), result, arg);
        break;
    case EM_GETSEL:
    case EM_SETRECT:
    case EM_SETRECTNP:
    case EM_SCROLL:
    case EM_LINESCROLL:
    case EM_SCROLLCARET:
    case EM_GETMODIFY:
    case EM_SETMODIFY:
    case EM_GETLINECOUNT:
    case EM_LINEINDEX:
    case EM_SETHANDLE:
    case EM_GETHANDLE:
    case EM_GETTHUMB:
    case EM_LINELENGTH:
    case EM_REPLACESEL:
    case EM_LIMITTEXT:
    case EM_CANUNDO:
    case EM_UNDO:
    case EM_FMTLINES:
    case EM_LINEFROMCHAR:
    case EM_SETTABSTOPS:
    case EM_SETPASSWORDCHAR:
    case EM_EMPTYUNDOBUFFER:
    case EM_GETFIRSTVISIBLELINE:
    case EM_SETREADONLY:
    case EM_SETWORDBREAKPROC:
    case EM_GETWORDBREAKPROC:
    case EM_GETPASSWORDCHAR:
        ret = callback( HWND_16(hwnd), msg + EM_GETSEL16 - EM_GETSEL, wParam, lParam, result, arg );
        break;
    case EM_SETSEL:
        ret = callback( HWND_16(hwnd), EM_SETSEL16, 0, MAKELPARAM( wParam, lParam ), result, arg );
        break;
    case LB_CARETOFF:
    case LB_CARETON:
    case LB_DELETESTRING:
    case LB_GETANCHORINDEX:
    case LB_GETCARETINDEX:
    case LB_GETCOUNT:
    case LB_GETCURSEL:
    case LB_GETHORIZONTALEXTENT:
    case LB_GETITEMDATA:
    case LB_GETITEMHEIGHT:
    case LB_GETSEL:
    case LB_GETSELCOUNT:
    case LB_GETTEXTLEN:
    case LB_GETTOPINDEX:
    case LB_RESETCONTENT:
    case LB_SELITEMRANGE:
    case LB_SELITEMRANGEEX:
    case LB_SETANCHORINDEX:
    case LB_SETCARETINDEX:
    case LB_SETCOLUMNWIDTH:
    case LB_SETCURSEL:
    case LB_SETHORIZONTALEXTENT:
    case LB_SETITEMDATA:
    case LB_SETITEMHEIGHT:
    case LB_SETSEL:
    case LB_SETTOPINDEX:
        ret = callback( HWND_16(hwnd), msg + LB_ADDSTRING16 - LB_ADDSTRING, wParam, lParam, result, arg );
        break;
    case LB_ADDSTRING:
    case LB_FINDSTRING:
    case LB_FINDSTRINGEXACT:
    case LB_INSERTSTRING:
    case LB_SELECTSTRING:
    case LB_GETTEXT:
    case LB_DIR:
    case LB_ADDFILE:
        lParam = MapLS( (LPSTR)lParam );
        ret = callback( HWND_16(hwnd), msg + LB_ADDSTRING16 - LB_ADDSTRING, wParam, lParam, result, arg );
        UnMapLS( lParam );
        break;
    case LB_GETSELITEMS:
        {
            INT *items32 = (INT *)lParam;
            INT16 *items, buffer[512];
            unsigned int i;

            wParam = min( wParam, 0x7f80 ); /* Must be < 64K */
            if (!(items = get_buffer( buffer, sizeof(buffer), wParam * sizeof(INT16) ))) break;
            lParam = MapLS( items );
            ret = callback( HWND_16(hwnd), LB_GETSELITEMS16, wParam, lParam, result, arg );
            UnMapLS( lParam );
            for (i = 0; i < wParam; i++) items32[i] = items[i];
            free_buffer( buffer, items );
        }
        break;
    case LB_SETTABSTOPS:
        if (wParam)
        {
            INT *stops32 = (INT *)lParam;
            INT16 *stops, buffer[512];
            unsigned int i;

            wParam = min( wParam, 0x7f80 ); /* Must be < 64K */
            if (!(stops = get_buffer( buffer, sizeof(buffer), wParam * sizeof(INT16) ))) break;
            for (i = 0; i < wParam; i++) stops[i] = stops32[i];
            lParam = MapLS( stops );
            ret = callback( HWND_16(hwnd), LB_SETTABSTOPS16, wParam, lParam, result, arg );
            UnMapLS( lParam );
            free_buffer( buffer, stops );
        }
        else ret = callback( HWND_16(hwnd), LB_SETTABSTOPS16, wParam, lParam, result, arg );
        break;
    case CB_DELETESTRING:
    case CB_GETCOUNT:
    case CB_GETLBTEXTLEN:
    case CB_LIMITTEXT:
    case CB_RESETCONTENT:
    case CB_SETEDITSEL:
    case CB_GETCURSEL:
    case CB_SETCURSEL:
    case CB_SHOWDROPDOWN:
    case CB_SETITEMDATA:
    case CB_SETITEMHEIGHT:
    case CB_GETITEMHEIGHT:
    case CB_SETEXTENDEDUI:
    case CB_GETEXTENDEDUI:
    case CB_GETDROPPEDSTATE:
        ret = callback( HWND_16(hwnd), msg + CB_GETEDITSEL16 - CB_GETEDITSEL, wParam, lParam, result, arg );
        break;
    /* unknown message */
    case CB_UNKNOWN_167:
        ret = callback(HWND_16(hwnd), CB_UNKNOWN_167_16, wParam, lParam, result, arg);
        break;
    case CB_GETEDITSEL:
        ret = callback( HWND_16(hwnd), CB_GETEDITSEL16, wParam, lParam, result, arg );
        if (wParam) *((PUINT)(wParam)) = LOWORD(*result);
        if (lParam) *((PUINT)(lParam)) = HIWORD(*result);  /* FIXME: subtract 1? */
        break;
    case CB_ADDSTRING:
    case CB_FINDSTRING:
    case CB_FINDSTRINGEXACT:
    case CB_INSERTSTRING:
    case CB_SELECTSTRING:
    case CB_DIR:
    case CB_GETLBTEXT:
        lParam = MapLS( (LPSTR)lParam );
        ret = callback( HWND_16(hwnd), msg + CB_GETEDITSEL16 - CB_GETEDITSEL, wParam, lParam, result, arg );
        UnMapLS( lParam );
        break;
    case LB_GETITEMRECT:
    case CB_GETDROPPEDCONTROLRECT:
        {
            RECT *r32 = (RECT *)lParam;
            RECT16 rect;
            lParam = MapLS( &rect );
            ret = callback( HWND_16(hwnd),
                            (msg == LB_GETITEMRECT) ? LB_GETITEMRECT16 : CB_GETDROPPEDCONTROLRECT16,
                            wParam, lParam, result, arg );
            UnMapLS( lParam );
            RECT16to32( &rect, r32 );
        }
        break;
    case WM_PAINTCLIPBOARD:
    case WM_SIZECLIPBOARD:
        FIXME_(msg)( "message %04x needs translation\n", msg );
        break;
    /* the following messages should not be sent to 16-bit apps */
    case WM_SIZING:
    case WM_MOVING:
    case WM_CAPTURECHANGED:
    case WM_STYLECHANGING:
    case WM_STYLECHANGED:
        break;
    case WM_MOUSEACTIVATE:
        ret = callback(HWND_16(hwnd), msg, HWND_16((HWND)wParam), lParam, result, arg);
        break;
    case WM_SETFOCUS:
    case WM_SETCURSOR:
        ret = callback(HWND_16(hwnd), msg, HWND_16((HWND)wParam), lParam, result, arg);
        break;
    case WM_INITMENU:
    case WM_INITMENUPOPUP:
    case WM_UNINITMENUPOPUP:
        ret = callback(HWND_16(hwnd), msg, HMENU_16((HMENU)wParam), lParam, result, arg);
        break;
    case WM_GETICON:
        ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
        *result = (LRESULT)get_icon_32((HICON16)*result);
        break;
    case WM_SETICON:
        ret = callback(HWND_16(hwnd), msg, wParam, get_icon_16((HICON)lParam), result, arg);
        break;
    case WM_QUERYDRAGICON:
        ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
        *result = (LRESULT)get_icon_32((HICON16)*result);
        break;
    case WM_QUERYDROPOBJECT:
    case WM_DROPOBJECT:
    case WM_DRAGLOOP:
    case WM_DRAGSELECT:
    case WM_DRAGMOVE:
    {
        LPDROPSTRUCT lpds32 = (LPDROPSTRUCT)lParam;
        DROPSTRUCT16 ds16;
        LPDROPSTRUCT16 lpds16 = &ds16;
        SEGPTR sds16 = MapLS(lpds16);
        HICON icon32;
        DROPSTRUCT32_16(lpds32, lpds16);
        ret = callback(HWND_16(hwnd), msg, wParam, sds16, result, arg);
        icon32 = get_icon_32((HICON16)*result);
        if (icon32)
        {
            *result = (LRESULT)icon32;
        }
        DROPSTRUCT16_32(lpds32, lpds16);
        break;
    }
    //some applications (afx?) crash when processing this message
    case WM_THEMECHANGED:
        break;
    case MM_WOM_OPEN:
    case MM_WOM_CLOSE:
        ret = callback(HWND_16(hwnd), msg, HDRVR_16((HDRVR)wParam), lParam, result, arg);
        break;
    case MM_WOM_DONE:
    {
        ret = callback(HWND_16(hwnd), msg, HDRVR_16((HDRVR)wParam), lParam, result, arg);
        break;
    }
    case WM_MOUSEWHEEL:
    case WM_SYSTIMER:
    case WM_TIMER:
        if (!HIWORD(wParam))
        {
            ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
            break;
        }
        else
        {
            int index = -1;
            for (int i = 0; i < TIMER32_WRAP_SIZE; i++)
            {
                if (!timer32[i].ref)
                {
                    index = i;
                    break;
                }
            }
            if (index == -1)
            {
                ERR("could not allocate TIMER32(%s)\n", message_to_str(msg));
                ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
                break;
            }
            timer32_count++;
            timer32[index].ref = TRUE;
            timer32[index].wParam = wParam;
            timer32[index].lParam = lParam;
            ret = callback(HWND_16(hwnd), msg, index, TIMER32_LPARAM, result, arg);
            break;
        }
    case WM_GETFONT:
        ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
        *result = (LRESULT)HFONT_32((HFONT16)*result);
        break;
    case WM_KILLFOCUS:
    case WM_MDIDESTROY:
    case WM_INITDIALOG:
        ret = callback(HWND_16(hwnd), msg, HWND_16((HWND)wParam), lParam, result, arg);
        break;
    case WM_ENTERIDLE:
        ret = callback(HWND_16(hwnd), msg, wParam, HWND_16((HWND)lParam), result, arg);
        break;
    case WM_UPDATEUISTATE:
        ret = callback(HWND_16(hwnd), msg, wParam, wParam, result, arg);
        break;
    case WM_PRINT:
    case WM_PRINTCLIENT:
    case WM_NCUAHDRAWFRAME: /* undocumented wparam: hdc */
        ret = callback(HWND_16(hwnd), msg, HDC_16((HDC)wParam), lParam, result, arg);
        break;
    case 0x4c8: //MCIWNDM_NOTIFYMODE
    {
        char clsname[12];
        BOOL r = GetClassNameA(wParam, &clsname, 12);
        if (r && !stricmp(clsname, "MCIWNDCLASS"))
            ret = callback(HWND_16(hwnd), msg, HWND_16(wParam), lParam, result, arg);
        else
            ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        break;
    }
    default:
    {
        if (msg != WM_NULL && msg == drag_list_message)
        {
            LPDRAGLISTINFO di = (LPDRAGLISTINFO)lParam;
            DRAGLISTINFO16 di16;
            di16.hWnd = HWND_16(di->hWnd);
            di16.uNotification = di->uNotification;
            di16.ptCursor.x = di->ptCursor.x;
            di16.ptCursor.y = di->ptCursor.y;
            lParam = MapLS(&di16);
            ret = callback(HWND_16(hwnd), msg, wParam, lParam, result, arg);
            UnMapLS(lParam);
            break;
        }
    }
        ret = callback( HWND_16(hwnd), msg, wParam, lParam, result, arg );
        break;
    }
    return ret;
}


static LRESULT send_message_timeout_callback( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
                                      LRESULT *result, void *arg )
{
    DWORD count;
    ReleaseThunkLock(&count);
    LRESULT success = SendMessageTimeoutA(hwnd, msg, wp, lp, SMTO_NORMAL, 1000, result);
    RestoreThunkLock(count);
    if (!success)
    {
        if (GetLastError() == ERROR_TIMEOUT)
        {
            ERR("SendMessage(%p,%s,%04x,%08x,uTimeout=1000ms) timed out.\n", hwnd, message_to_str(msg), wp, lp);
        }
    }
    return *result;
}
typedef struct
{
    UINT msg;
    WPARAM wp;
    LPARAM lp;
} send_message_to_same_process_args;
BOOL CALLBACK send_message_to_same_process(HWND hwnd, LPARAM lp)
{
    DWORD pid;
    DWORD_PTR result;
    send_message_to_same_process_args *args = (send_message_to_same_process_args*)lp;
    if (GetWindowThreadProcessId(hwnd, &pid) && pid == GetCurrentProcessId())
    {
        SendMessageTimeoutA(hwnd, args->msg, args->wp, args->lp, SMTO_NORMAL, 1000, &result);
    }
    return TRUE;
}

static LRESULT send_message_to_same_process_callback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
    LRESULT *result, void *arg)
{
    DWORD count;
    send_message_to_same_process_args args;
    args.msg = msg;
    args.wp = wp;
    args.lp = lp;
    ReleaseThunkLock(&count);
    *result = 0;
    EnumWindows(send_message_to_same_process, &args);
    RestoreThunkLock(count);
    return *result;
}
/* InstallShield uses SendMessage(HWND_BROADCAST) for IPC */
LRESULT WINAPI fix_installshield(HWND16 hwnd16, UINT16 msg, WPARAM16 wparam, LPARAM lparam, BOOL *fixed)
{
    char name[256];
    LRESULT result;
    static const char *table[] = 
    {
        "LZW_IS_CLIENT_ALIVE_SE",
        "ISDEL_MSG_DELENABLE",
        "ISDEL_MSG_DELEXISTS",
        "ISDEL_MSG_DELDONE",
        "ISMSG16_TERMINATE",
        "ISMSG32_TERMINATE",
        "LOGO_MSG_LOGOCLOSE_30",
        "LOGO_MSG_LOGOSTATUS_30",
    };
    *fixed = FALSE;
    if (!GetClipboardFormatNameA(msg, name, ARRAYSIZE(name)))
    {
        return 0;
    }
    for (SIZE_T i = 0; i < ARRAYSIZE(table); i++)
    {
        if (!strcmp(name, table[i]))
        {
            *fixed = TRUE;
            WINPROC_CallProc16To32A(send_message_to_same_process_callback, hwnd16, msg, wparam, lparam, &result, NULL);
            return result;
        }
    }
    return 0;
}
/***********************************************************************
 *		SendMessage  (USER.111)
 */
LRESULT WINAPI SendMessage16( HWND16 hwnd16, UINT16 msg, WPARAM16 wparam, LPARAM lparam )
{
    LRESULT result;
    HWND hwnd = WIN_Handle32( hwnd16 );
    SetEvent(kernel_get_thread_data()->idle_event);

    // SendMessageTimeout always fails with this message
    if (msg == WM_DDE_EXECUTE)
       return PostMessage16( hwnd16, msg, wparam, lparam );

    if (hwnd != HWND_BROADCAST &&
        GetWindowThreadProcessId( hwnd, NULL ) == GetCurrentThreadId())
    {
        /* call 16-bit window proc directly */
        WNDPROC16 winproc;

		if (!(winproc = (WNDPROC16)GetWndProc16(hwnd16)))
		{
            DLGPROC16 dlgproc;
            /* workaround for PBRUSH.EXE (color palette) */
            if (msg == WM_COMMAND)
            {
                dlgproc = (DLGPROC16)GetDlgProc16(hwnd16);
                if (dlgproc)
                {
                    /* first the WH_CALLWNDPROC hook */
                    call_WH_CALLWNDPROC_hook(hwnd16, &msg, &wparam, &lparam);
                    CallWindowProc16(dlgproc, hwnd16, msg, wparam, lparam);
                    return GetWindowLong16(hwnd16, DWL_MSGRESULT);
                }
            }
			WINPROC_CallProc16To32A(send_message_callback, hwnd16, msg, wparam, lparam, &result, NULL);
			return result;
		}

        /* first the WH_CALLWNDPROC hook */
        call_WH_CALLWNDPROC_hook(hwnd16, &msg, &wparam, &lparam);
        TRACE_(message)("(0x%04x) [%04x] wp=%04x lp=%08lx\n", hwnd16, msg, wparam, lparam );
        result = CallWindowProc16( winproc, hwnd16, msg, wparam, lparam );
        TRACE_(message)("(0x%04x) [%04x] wp=%04x lp=%08lx returned %08lx\n",
                        hwnd16, msg, wparam, lparam, result );
    }
    else  /* map to 32-bit unicode for inter-thread/process message */
    {
        if (hwnd == HWND_BROADCAST && msg >= 0xC000)
        {
            BOOL fixed = FALSE;
            LRESULT result;
            result = fix_installshield(hwnd16, msg, wparam, lparam, &fixed);
            if (fixed)
                return result;
        }
        TRACE_(message)("(0x%04x) to 32-bit [%04x] wp=%04x lp=%08lx\n", hwnd16, msg, wparam, lparam);
        WINPROC_CallProc16To32A( send_message_timeout_callback, hwnd16, msg, wparam, lparam, &result, NULL );
        TRACE_(message)("(0x%04x) to 32-bit [%04x] wp=%04x lp=%08lx returned %08lx\n",
            hwnd16, msg, wparam, lparam, result);
    }
    return result;
}


/***********************************************************************
 *		PostMessage  (USER.110)
 */
BOOL16 WINAPI PostMessage16( HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam )
{
    LRESULT unused;
    return WINPROC_CallProc16To32A( post_message_callback, hwnd, msg, wparam, lparam, &unused, NULL );
}


/***********************************************************************
 *		PostAppMessage (USER.116)
 */
BOOL16 WINAPI PostAppMessage16( HTASK16 hTask, UINT16 msg, WPARAM16 wparam, LPARAM lparam )
{
    LRESULT unused;
    DWORD_PTR tid = HTASK_32( hTask );

    if (!tid) return FALSE;
    return WINPROC_CallProc16To32A( post_thread_message_callback, 0, msg, wparam, lparam,
                                    &unused, (void *)tid );
}


/**********************************************************************
 *		CallWindowProc (USER.122)
 */
LRESULT WINAPI CallWindowProc16( WNDPROC16 func, HWND16 hwnd, UINT16 msg,
                                 WPARAM16 wParam, LPARAM lParam )
{
    int index = winproc_to_index( func );
    LRESULT result;

    if (!func) return 0;

    if (WINPROC_IsNativeProc(func))
    {
        WINPROC_CallProc16To32A(call_window_proc_callback, hwnd, msg, wParam, lParam, &result, (void*)winproc16_array[index - MAX_WINPROCS32]);
        return result;
    }
    if (index == -1 || index >= MAX_WINPROCS32)
        call_window_proc16( hwnd, msg, wParam, lParam, &result, func );
    else
    {
        WNDPROC proc = (WNDPROC)func;
        if (thunk_array && thunk_array[index].proc) proc = thunk_array[index].proc;
        WINPROC_CallProc16To32A( call_window_proc_callback, hwnd, msg, wParam, lParam, &result, proc );
    }
    return result;
}


/**********************************************************************
 *	     __wine_call_wndproc   (USER.1010)
 */
LRESULT WINAPI __wine_call_wndproc( HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam, WNDPROC proc )
{
    LRESULT result;
    WINPROC_CallProc16To32A( call_window_proc_callback, hwnd, msg, wParam, lParam, &result, proc );
    return result;
}


/***********************************************************************
 *		InSendMessage  (USER.192)
 */
BOOL16 WINAPI InSendMessage16(void)
{
    return InSendMessage();
}


/***********************************************************************
 *		ReplyMessage  (USER.115)
 */
void WINAPI ReplyMessage16( LRESULT result )
{
    ReplyMessage( result );
}


static ATOM atom_UserAdapterWindowClass;
/***********************************************************************
 *		PeekMessage32 (USER.819)
 */
BOOL16 WINAPI PeekMessage32_16( MSG32_16 *msg16, HWND16 hwnd16,
                                UINT16 first, UINT16 last, UINT16 flags,
                                BOOL16 wHaveParamHigh )
{
    MSG msg;
    LRESULT unused;
    HWND hwnd = WIN_Handle32( hwnd16 );

    if(USER16_AlertableWait)
        MsgWaitForMultipleObjectsEx( 0, NULL, 0, 0, MWMO_ALERTABLE );
    if (!PeekMessageA( &msg, hwnd, first, last, flags )) return FALSE;

    if (atom_UserAdapterWindowClass == 0)
    {
        WNDCLASSA c;
        HMODULE hmod = GetModuleHandleW(L"CoreMessaging");
        if (hmod)
        {
            atom_UserAdapterWindowClass = GetClassInfoA(hmod, "UserAdapterWindowClass", &c);
        }
    }
    if ((flags & PM_REMOVE) && atom_UserAdapterWindowClass != 0 && msg.hwnd != NULL && GetClassWord(msg.hwnd, GCW_ATOM) == atom_UserAdapterWindowClass)
    {
        DispatchMessageA(&msg);
        return PeekMessage32_16(msg16, hwnd16, first, last, flags, wHaveParamHigh);
    }
    msg16->msg.time    = msg.time;
    msg16->msg.pt.x    = (INT16)msg.pt.x;
    msg16->msg.pt.y    = (INT16)msg.pt.y;
    if (wHaveParamHigh) msg16->wParamHigh = HIWORD(msg.wParam);
    WINPROC_CallProc32ATo16( get_message_callback, msg.hwnd, msg.message, msg.wParam, msg.lParam,
                             &unused, &msg16->msg );
    return TRUE;
}


typedef struct
{
    BOOL down;
    BOOL found;
    BOOL scrollable;
    HWND hwnd;
} enum_scrollbar_data;
static void check_scrollable(enum_scrollbar_data *d, HWND hwnd)
{
    SCROLLINFO si = { 0 };
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;
    char ctrl[12];
    RealGetWindowClassA(hwnd, ctrl, 12);
    if (!stricmp(ctrl, "SCROLLBAR"))
    {
        if (GetScrollInfo(hwnd, SB_CTL, &si))
        {
            if (GetWindowLongW(hwnd, GWL_STYLE) & SBS_VERT)
            {
                d->found = TRUE;
                if ((d->down && si.nMax > si.nPos) || (!d->down && si.nMin < si.nPos))
                {
                    d->scrollable = TRUE;
                    d->hwnd = hwnd;
                }
            }
            return;
        }
    }
    if (GetScrollInfo(hwnd, SB_VERT, &si))
    {
        d->found = TRUE;
        if ((d->down && si.nMax > si.nPos) || (!d->down && si.nMin < si.nPos))
        {
            d->scrollable = TRUE;
            d->hwnd = NULL;
        }
        return;
    }
}
static BOOL CALLBACK enum_scrollbar_proc(HWND hwnd, LPARAM lp)
{
    enum_scrollbar_data *d = (enum_scrollbar_data*)lp;
    check_scrollable(d, hwnd);
    return TRUE;
}
static LRESULT defwindow_proc_callback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
    LRESULT *result, void *arg)
{
    _LeaveWin16Lock();
    if (msg == WM_MOUSEWHEEL)
    {
        enum_scrollbar_data d = { 0 };
        d.down = (INT16)HIWORD(wp) < 0;
        check_scrollable(&d, hwnd);
        EnumChildWindows(hwnd, (WNDENUMPROC)enum_scrollbar_proc, (LPARAM)&d);
        if (d.scrollable)
        {
            SendMessage16(HWND_16(hwnd), WM_VSCROLL, d.down ? SB_LINEDOWN : SB_LINEUP, MAKELONG(0, (WORD)HWND_16(d.hwnd)));
            SendMessage16(HWND_16(hwnd), WM_VSCROLL, SB_ENDSCROLL, MAKELONG(0, (WORD)HWND_16(d.hwnd)));
            _EnterWin16Lock();
            return 0;
        }
    }
    *result = DefWindowProcA(hwnd, msg, wp, lp);
    _EnterWin16Lock();
    return *result;
}
/***********************************************************************
 *		DefWindowProc (USER.107)
 */
LRESULT WINAPI DefWindowProc16( HWND16 hwnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam )
{
    LRESULT result;
    WINPROC_CallProc16To32A(defwindow_proc_callback, hwnd16, msg, wParam, lParam, &result, 0);
    return result;
}


/***********************************************************************
 *              DefDlgProc (USER.308)
 */
LRESULT WINAPI DefDlgProc16( HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam )
{
    LRESULT result;
    WINPROC_CallProc16To32A( defdlg_proc_callback, hwnd, msg, wParam, lParam, &result, 0 );
    return result;
}


BOOL16 WINAPI IsOldWindowsTask(HINSTANCE16 hInst);
/***********************************************************************
 *		PeekMessage  (USER.109)
 */
BOOL16 WINAPI PeekMessage16( MSG16 *msg, HWND16 hwnd,
                             UINT16 first, UINT16 last, UINT16 flags )
{
    DWORD count;
    if (!(flags & PM_NOYIELD))
    {
        ReleaseThunkLock(&count);
        MsgWaitForMultipleObjects(0, NULL, FALSE, 1, QS_ALLINPUT);
        /* Sleep(1); /* yield thread */
        /* Some programs use PeekMessage instead of GetMessage, so use 100% CPU... */
        /* MsgWaitForMultipleObjects(0, NULL, FALSE, 10, QS_ALLINPUT); /**/
    }
    BOOL ret = PeekMessage32_16((MSG32_16 *)msg, hwnd, first, last, flags, FALSE);
    if (!(flags & PM_NOYIELD))
    {
        RestoreThunkLock(count);
    }
    if (IsOldWindowsTask(GetCurrentTask()) && !msg->hwnd)
        msg->hwnd = HWND_16(GetDesktopWindow());
    return ret;
}


/***********************************************************************
 *		GetMessage32  (USER.820)
 */
BOOL16 WINAPI GetMessage32_16( MSG32_16 *msg16, HWND16 hwnd16, UINT16 first,
                               UINT16 last, BOOL16 wHaveParamHigh )
{
    MSG msg;
    LRESULT unused;
    HWND hwnd = WIN_Handle32( hwnd16 );
    SetEvent(kernel_get_thread_data()->idle_event);

    if(USER16_AlertableWait)
        MsgWaitForMultipleObjectsEx( 0, NULL, INFINITE, 0, MWMO_ALERTABLE );

    DWORD count;

    //Yield
    ReleaseThunkLock(&count);
    GetMessageA(&msg, hwnd, first, last);
    RestoreThunkLock(count);
    msg16->msg.time    = msg.time;
    msg16->msg.pt.x    = (INT16)msg.pt.x;
    msg16->msg.pt.y    = (INT16)msg.pt.y;
    if (wHaveParamHigh) msg16->wParamHigh = HIWORD(msg.wParam);
    WINPROC_CallProc32ATo16( get_message_callback, msg.hwnd, msg.message, msg.wParam, msg.lParam,
                             &unused, &msg16->msg );

    TRACE( "message %04x, hwnd %p, filter(%04x - %04x)\n",
           msg16->msg.message, hwnd, first, last );

    return msg16->msg.message != WM_QUIT;
}


/***********************************************************************
 *		GetMessage  (USER.108)
 */
BOOL16 WINAPI GetMessage16( MSG16 *msg, HWND16 hwnd, UINT16 first, UINT16 last )
{
    return GetMessage32_16( (MSG32_16 *)msg, hwnd, first, last, FALSE );
}


/***********************************************************************
 *		TranslateMessage32 (USER.821)
 */
BOOL16 WINAPI TranslateMessage32_16( const MSG32_16 *msg, BOOL16 wHaveParamHigh )
{
    MSG msg32;

    msg32.hwnd    = WIN_Handle32( msg->msg.hwnd );
    msg32.message = msg->msg.message;
    msg32.wParam  = MAKEWPARAM( msg->msg.wParam, wHaveParamHigh ? msg->wParamHigh : 0 );
    msg32.lParam  = msg->msg.lParam;
    return TranslateMessage( &msg32 );
}


/***********************************************************************
 *		TranslateMessage (USER.113)
 */
BOOL16 WINAPI TranslateMessage16( const MSG16 *msg )
{
    return TranslateMessage32_16( (const MSG32_16 *)msg, FALSE );
}


LRESULT call_native_wndproc(HWND16 hWnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
	LRESULT result;
	WNDPROC wndproc32 = (WNDPROC)GetWindowLongPtrA(HWND_32(hWnd16), GWLP_WNDPROC);
	if (!wndproc32)
	{
		SetLastError(ERROR_INVALID_WINDOW_HANDLE);
		return 0;
	}
	WINPROC_CallProc16To32A(call_window_proc_callback, hWnd16, msg, wParam, lParam, &result, wndproc32);
	return result;
}
/***********************************************************************
 *		DispatchMessage (USER.114)
 */
LONG WINAPI DispatchMessage16( const MSG16* msg )
{
    WNDPROC16 winproc;
    LRESULT retval;
    WNDPROC wndproc32 = (WNDPROC)GetWindowLongPtrA(HWND_32(msg->hwnd), GWLP_WNDPROC);

      /* Process timer messages */
    if ((msg->message == WM_TIMER) || (msg->message == WM_SYSTIMER))
    {
        if (msg->lParam)
        {
            WPARAM wp = (WPARAM)msg->wParam;
            LPARAM lp = msg->lParam;
            if (msg->lParam == TIMER32_LPARAM)
            {
                timer32[msg->wParam].ref = FALSE;
                wp = timer32[msg->wParam].wParam;
                lp = timer32[msg->wParam].lParam;
                timer32_count--;
            }
            if (msg->lParam == TIMER32_LPARAM || (msg->lParam & 0xFFFF0000) != 0xFFFF0000)
            {
                MSG msg32;

                msg32.hwnd = WIN_Handle32(msg->hwnd);
                msg32.wParam = wp;
                msg32.lParam = lp;
                msg32.time = msg->time;
                msg32.pt.x = msg->pt.x;
                msg32.pt.y = msg->pt.y;
                msg32.message = msg->message;
                return DispatchMessageA(&msg32);
            }
            return CallWindowProc16((WNDPROC16)msg->lParam, msg->hwnd,
                msg->message, (WPARAM16)wp, GetTickCount());
        }
	}

    if (!msg->hwnd || !(winproc = (WNDPROC16)GetWndProc16( msg->hwnd )) ||
        wndproc32 != WindowProc16 /* OleSetMenuDescriptor put a window in a subclass */)
	{
		LRESULT result;
		if (!wndproc32)
		{
			SetLastError(ERROR_INVALID_WINDOW_HANDLE);
			return 0;
		}
		WINPROC_CallProc16To32A(call_window_proc_callback, msg->hwnd, msg->message, msg->wParam, msg->lParam, &result,
			wndproc32);
		return result;
    }
	TRACE_(message)("(0x%04x) [%04x] wp=%04x lp=%08lx\n", msg->hwnd, msg->message, msg->wParam, msg->lParam);
    retval = CallWindowProc16( winproc, msg->hwnd, msg->message, msg->wParam, msg->lParam );
    TRACE_(message)("(0x%04x) [%04x] wp=%04x lp=%08lx returned %08lx\n",
		msg->hwnd, msg->message, msg->wParam, msg->lParam, retval);
    return retval;
}


/***********************************************************************
 *		DispatchMessage32 (USER.822)
 */
LONG WINAPI DispatchMessage32_16( const MSG32_16 *msg16, BOOL16 wHaveParamHigh )
{
    if (wHaveParamHigh == FALSE)
        return DispatchMessage16( &msg16->msg );
    else
    {
        MSG msg;

        msg.hwnd    = WIN_Handle32( msg16->msg.hwnd );
        msg.message = msg16->msg.message;
        msg.wParam  = MAKEWPARAM( msg16->msg.wParam, msg16->wParamHigh );
        msg.lParam  = msg16->msg.lParam;
        msg.time    = msg16->msg.time;
        msg.pt.x    = msg16->msg.pt.x;
        msg.pt.y    = msg16->msg.pt.y;
        return DispatchMessageA( &msg );
    }
}



static LRESULT is_dialog_message_callback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
    LRESULT *result, void *arg)
{
    MSG16 *msg16 = (MSG16*)arg;
    MSG mesg;
    if (msg == WM_DWMNCRENDERINGCHANGED)
        return;
    mesg.hwnd = HWND_32(msg16->hwnd);
    mesg.message = msg;
    mesg.wParam = wp;
    mesg.lParam = lp;
    mesg.time = msg16->time;
    mesg.pt.x = msg16->pt.x;
    mesg.pt.y = msg16->pt.y;
    *result = IsDialogMessageA(hwnd, &mesg);
    return *result;
}
/***********************************************************************
 *		IsDialogMessage (USER.90)
 */
BOOL16 WINAPI IsDialogMessage16( HWND16 hwndDlg, MSG16 *msg16 )
{
    MSG msg;
    HWND hwndDlg32;

    msg.hwnd  = WIN_Handle32(msg16->hwnd);
    hwndDlg32 = WIN_Handle32(hwndDlg);

    switch(msg16->message)
    {
    case WM_KEYDOWN:
    case WM_CHAR:
    case WM_SYSCHAR:
        msg.message = msg16->message;
        msg.wParam  = msg16->wParam;
        msg.lParam  = msg16->lParam;
        msg.time    = msg16->time;
        msg.pt.x    = msg16->pt.x;
        msg.pt.y    = msg16->pt.y;
        return IsDialogMessageA( hwndDlg32, &msg );
    default:
    {
        LPARAM result;
        return WINPROC_CallProc16To32A(is_dialog_message_callback, hwndDlg, msg16->message, msg16->wParam, msg16->lParam, &result, msg16);
    }
    }

    if ((hwndDlg32 != msg.hwnd) && !IsChild( hwndDlg32, msg.hwnd )) return FALSE;
    TranslateMessage16( msg16 );
	//????
	if (DispatchMessage16(msg16) == FALSE)
		DefDlgProc16(hwndDlg, msg16->message, msg16->wParam, msg16->lParam);
    return TRUE;
}


/***********************************************************************
 *		MsgWaitForMultipleObjects  (USER.640)
 */
DWORD WINAPI MsgWaitForMultipleObjects16( DWORD count, const HANDLE *handles,
                                          BOOL wait_all, DWORD timeout, DWORD mask )
{
    return MsgWaitForMultipleObjectsEx( count, handles, timeout, mask,
                                        wait_all ? MWMO_WAITALL : 0 );
}


/**********************************************************************
 *		SetDoubleClickTime (USER.20)
 */
void WINAPI SetDoubleClickTime16( UINT16 interval )
{
    SetDoubleClickTime( interval );
}


/**********************************************************************
 *		GetDoubleClickTime (USER.21)
 */
UINT16 WINAPI GetDoubleClickTime16(void)
{
    return GetDoubleClickTime();
}


/***********************************************************************
 *		PostQuitMessage (USER.6)
 */
void WINAPI PostQuitMessage16( INT16 exitCode )
{
    PostQuitMessage( exitCode );
}


/**********************************************************************
 *		GetKeyState (USER.106)
 */
INT16 WINAPI GetKeyState16(INT16 vkey)
{
    return GetKeyState(vkey);
}


/**********************************************************************
 *		GetKeyboardState (USER.222)
 */
BOOL WINAPI GetKeyboardState16( LPBYTE state )
{
    return GetKeyboardState( state );
}


/**********************************************************************
 *		SetKeyboardState (USER.223)
 */
BOOL WINAPI SetKeyboardState16( LPBYTE state )
{
    return SetKeyboardState( state );
}


/***********************************************************************
 *		SetMessageQueue (USER.266)
 */
BOOL16 WINAPI SetMessageQueue16( INT16 size )
{
    return SetMessageQueue( size );
}


/***********************************************************************
 *		UserYield (USER.332)
 */
void WINAPI UserYield16(void)
{
    MSG msg;
    PeekMessageW( &msg, 0, 0, 0, PM_REMOVE | PM_QS_SENDMESSAGE );
}


/***********************************************************************
 *		GetQueueStatus (USER.334)
 */
DWORD WINAPI GetQueueStatus16( UINT16 flags )
{
    return GetQueueStatus( flags );
}


/***********************************************************************
 *		GetInputState (USER.335)
 */
BOOL16 WINAPI GetInputState16(void)
{
    return GetInputState();
}


/**********************************************************************
 *           TranslateAccelerator      (USER.178)
 */
INT16 WINAPI TranslateAccelerator16( HWND16 hwnd, HACCEL16 hAccel, LPMSG16 msg )
{
    MSG msg32;

    if (!msg) return 0;
    msg32.message = msg->message;
    /* msg32.hwnd not used */
    msg32.wParam  = msg->wParam;
    msg32.lParam  = msg->lParam;
    return TranslateAcceleratorW( WIN_Handle32(hwnd), HACCEL_32(hAccel), &msg32 );
}


/**********************************************************************
 *		TranslateMDISysAccel (USER.451)
 */
BOOL16 WINAPI TranslateMDISysAccel16( HWND16 hwndClient, LPMSG16 msg )
{
    if (msg->message == WM_KEYDOWN || msg->message == WM_SYSKEYDOWN)
    {
        MSG msg32;
        msg32.hwnd    = WIN_Handle32(msg->hwnd);
        msg32.message = msg->message;
        msg32.wParam  = msg->wParam;
        msg32.lParam  = msg->lParam;
        /* MDICLIENTINFO is still the same for win32 and win16 ... */
        return TranslateMDISysAccel( WIN_Handle32(hwndClient), &msg32 );
    }
    return 0;
}


/***********************************************************************
*           button_proc16
*/
static LRESULT button_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f)
{
    static const UINT msg16_offset = BM_GETCHECK16 - BM_GETCHECK;
    *f = TRUE;

    switch (msg)
    {
    case BM_GETCHECK16:
    case BM_SETCHECK16:
    case BM_GETSTATE16:
    case BM_SETSTATE16:
    case BM_SETSTYLE16:
        return callback(hwnd, msg - msg16_offset, wParam, lParam, result, arg);
    default:
        *f = FALSE;
        return 0;
    }
}
/***********************************************************************
 *           button_proc16
 */
static LRESULT button_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    static const UINT msg16_offset = BM_GETCHECK16 - BM_GETCHECK;

    switch (msg)
    {
    case BM_GETCHECK16:
    case BM_SETCHECK16:
    case BM_GETSTATE16:
    case BM_SETSTATE16:
    case BM_SETSTYLE16:
        return wow_handlers32.button_proc( hwnd, msg - msg16_offset, wParam, lParam, FALSE );
    default:
        return wow_handlers32.button_proc( hwnd, msg, wParam, lParam, unicode );
    }
}


/***********************************************************************
 *           combo_proc16
 */
static LRESULT combo_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    static const UINT msg16_offset = CB_GETEDITSEL16 - CB_GETEDITSEL;

    switch (msg)
    {
    case CB_INSERTSTRING16:
    case CB_SELECTSTRING16:
    case CB_FINDSTRING16:
    case CB_FINDSTRINGEXACT16:
        wParam = (INT)(INT16)wParam;
        /* fall through */
    case CB_ADDSTRING16:
    {
        DWORD style = GetWindowLongW( hwnd, GWL_STYLE );
        if ((style & CBS_HASSTRINGS) || !(style & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE)))
           lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    }
    case CB_SETITEMHEIGHT16:
    case CB_GETITEMHEIGHT16:
    case CB_SETCURSEL16:
    case CB_GETLBTEXTLEN16:
    case CB_GETITEMDATA16:
    case CB_SETITEMDATA16:
        wParam = (INT)(INT16)wParam;	/* signed integer */
        msg -= msg16_offset;
        break;
    case CB_GETDROPPEDCONTROLRECT16:
        lParam = (LPARAM)MapSL(lParam);
        if (lParam)
        {
            RECT r;
            RECT16 *r16 = (RECT16 *)lParam;
            wow_handlers32.combo_proc( hwnd, CB_GETDROPPEDCONTROLRECT, wParam, (LPARAM)&r, FALSE );
            r16->left   = r.left;
            r16->top    = r.top;
            r16->right  = r.right;
            r16->bottom = r.bottom;
        }
        return CB_OKAY;
    case CB_DIR16:
        if (wParam & DDL_DRIVES) wParam |= DDL_EXCLUSIVE;
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case CB_GETLBTEXT16:
        wParam = (INT)(INT16)wParam;
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case CB_GETEDITSEL16:
        wParam = lParam = 0;   /* just in case */
        msg -= msg16_offset;
        break;
    case CB_LIMITTEXT16:
    case CB_SETEDITSEL16:
    case CB_DELETESTRING16:
    case CB_RESETCONTENT16:
    case CB_GETDROPPEDSTATE16:
    case CB_SHOWDROPDOWN16:
    case CB_GETCOUNT16:
    case CB_GETCURSEL16:
    case CB_SETEXTENDEDUI16:
    case CB_GETEXTENDEDUI16:
        msg -= msg16_offset;
        break;
    default:
        return wow_handlers32.combo_proc( hwnd, msg, wParam, lParam, unicode );
    }
    return wow_handlers32.combo_proc( hwnd, msg, wParam, lParam, FALSE );
}


/***********************************************************************
*           combo_proc16
*/
static LRESULT combo_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f) 
{
    static const UINT msg16_offset = CB_GETEDITSEL16 - CB_GETEDITSEL;
    *f = TRUE;
    switch (msg)
    {
    case CB_INSERTSTRING16:
    case CB_SELECTSTRING16:
    case CB_FINDSTRING16:
    case CB_FINDSTRINGEXACT16:
        wParam = (INT)(INT16)wParam;
        /* fall through */
    case CB_ADDSTRING16:
    {
        DWORD style = GetWindowLongW(hwnd, GWL_STYLE);
        if ((style & CBS_HASSTRINGS) || !(style & (CBS_OWNERDRAWFIXED | CBS_OWNERDRAWVARIABLE)))
            lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    }
    case CB_SETITEMHEIGHT16:
    case CB_GETITEMHEIGHT16:
    case CB_SETCURSEL16:
    case CB_GETLBTEXTLEN16:
    case CB_GETITEMDATA16:
    case CB_SETITEMDATA16:
        wParam = (INT)(INT16)wParam;	/* signed integer */
        msg -= msg16_offset;
        break;
    case CB_GETDROPPEDCONTROLRECT16:
        lParam = (LPARAM)MapSL(lParam);
        if (lParam)
        {
            RECT r;
            RECT16 *r16 = (RECT16 *)lParam;
            r.left = r16->left;
            r.top = r16->top;
            r.right = r16->right;
            r.bottom = r16->bottom;
            LRESULT ret = callback(hwnd, CB_GETDROPPEDCONTROLRECT, wParam, (LPARAM)&r, result, arg);//wow_handlers32.combo_proc(hwnd, CB_GETDROPPEDCONTROLRECT, wParam, (LPARAM)&r, FALSE);
            r16->left = r.left;
            r16->top = r.top;
            r16->right = r.right;
            r16->bottom = r.bottom;
            *result = CB_OKAY;
            return ret;
        }
    case CB_DIR16:
        if (wParam & DDL_DRIVES) wParam |= DDL_EXCLUSIVE;
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case CB_GETLBTEXT16:
        wParam = (INT)(INT16)wParam;
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case CB_GETEDITSEL16:
        wParam = lParam = 0;   /* just in case */
        msg -= msg16_offset;
        break;
    case CB_LIMITTEXT16:
    case CB_SETEDITSEL16:
    case CB_DELETESTRING16:
    case CB_RESETCONTENT16:
    case CB_GETDROPPEDSTATE16:
    case CB_SHOWDROPDOWN16:
    case CB_GETCOUNT16:
    case CB_GETCURSEL16:
    case CB_SETEXTENDEDUI16:
    case CB_GETEXTENDEDUI16:
        msg -= msg16_offset;
        break;
    case CB_UNKNOWN_167_16:
        msg = CB_UNKNOWN_167;
        break;
    default:
        *f = FALSE;
        return 0;
    }
    return callback(hwnd, msg, wParam, lParam, result, arg);
}


/*********************************************************************
 * edit_lock_buffer (internal)
 *
 * A 16 bit application might send an EM_GETHANDLE message and expect a HLOCAL16
 * (16 bit SEG:OFF handler). From that moment on we have to keep using this
 * 16 bit memory handler, because it is supposed to be valid at all times after
 * EM_GETHANDLE.
 * We create a HLOCAL16 buffer in edit_get_handle and copy the text from the
 * HLOCAL buffer, when needed
 *
 */

#define GWW_HANDLE16 sizeof(void*)

static void edit_lock_buffer( HWND hwnd )
{
    STACK16FRAME* stack16 = MapSL(PtrToUlong(getWOW32Reserved()));
    HLOCAL16 hloc16 = GetWindowWord16( HWND_16(hwnd), GWW_HANDLE16 );
    HANDLE16 oldDS;
    HLOCAL hloc32;
    UINT size;

    if (!hloc16) return;
    if (!(hloc32 = (HLOCAL)wow_handlers32.edit_proc( hwnd, EM_GETHANDLE, 0, 0, FALSE ))) return;

    oldDS = stack16->ds;
    stack16->ds = GetWindowWord16( HWND_16(hwnd), GWLP_HINSTANCE );
    size = LocalSize16(hloc16);
    if (LocalReAlloc( hloc32, size, LMEM_MOVEABLE ))
    {
        char *text = MapSL( LocalLock16( hloc16 ));
        char *dest = LocalLock( hloc32 );
        memcpy( dest, text, size );
        LocalUnlock( hloc32 );
        LocalUnlock16( hloc16 );
    }
    stack16->ds = oldDS;

}

static void edit_unlock_buffer( HWND hwnd )
{
    STACK16FRAME* stack16 = MapSL(PtrToUlong(getWOW32Reserved()));
    HLOCAL16 hloc16 = GetWindowWord16( HWND_16(hwnd), GWW_HANDLE16 );
    HANDLE16 oldDS;
    HLOCAL hloc32;
    UINT size;

    if (!hloc16) return;
    if (!(hloc32 = (HLOCAL)wow_handlers32.edit_proc( hwnd, EM_GETHANDLE, 0, 0, FALSE ))) return;
    size = LocalSize( hloc32 );

    oldDS = stack16->ds;
    stack16->ds = GetWindowWord16( HWND_16(hwnd), GWLP_HINSTANCE );
    if (LocalReAlloc16( hloc16, size, LMEM_MOVEABLE ))
    {
        char *text = LocalLock( hloc32 );
        char *dest = MapSL( LocalLock16( hloc16 ));
        memcpy( dest, text, size );
        LocalUnlock( hloc32 );
        LocalUnlock16( hloc16 );
    }
    stack16->ds = oldDS;
}

static HLOCAL16 edit_get_handle( HWND hwnd )
{
    CHAR *textA;
    UINT alloc_size;
    HLOCAL hloc;
    STACK16FRAME* stack16;
    HANDLE16 oldDS;
    HLOCAL16 hloc16 = GetWindowWord16( HWND_16(hwnd), GWW_HANDLE16 );

    if (hloc16) return hloc16;

    if (!(hloc = (HLOCAL)wow_handlers32.edit_proc( hwnd, EM_GETHANDLE, 0, 0, FALSE ))) return 0;
    alloc_size = LocalSize( hloc );

    stack16 = MapSL(PtrToUlong(getWOW32Reserved()));
    oldDS = stack16->ds;
    stack16->ds = GetWindowWord16( HWND_16(hwnd), GWLP_HINSTANCE );

    if (!LocalHeapSize16())
    {
        if (!LocalInit16(stack16->ds, 0, GlobalSize16(stack16->ds)))
        {
            ERR("could not initialize local heap\n");
            goto done;
        }
    }

    if (!(hloc16 = LocalAlloc16(LMEM_MOVEABLE | LMEM_ZEROINIT, alloc_size)))
    {
        ERR("could not allocate new 16 bit buffer\n");
        goto done;
    }

    if (!(textA = MapSL(LocalLock16( hloc16))))
    {
        ERR("could not lock new 16 bit buffer\n");
        LocalFree16(hloc16);
        hloc16 = 0;
        goto done;
    }
    memcpy( textA, LocalLock( hloc ), alloc_size );
    LocalUnlock( hloc );
    LocalUnlock16( hloc16 );
    SetWindowWord16( HWND_16(hwnd), GWW_HANDLE16, hloc16 );

done:
    stack16->ds = oldDS;
    return hloc16;
}

static void edit_set_handle( HWND hwnd, HLOCAL16 hloc16 )
{
    STACK16FRAME* stack16 = MapSL(PtrToUlong(getWOW32Reserved()));
    HINSTANCE16 hInstance = GetWindowWord16(HWND_16(hwnd), GWLP_HINSTANCE );
    HANDLE16 oldDS = stack16->ds;
    HLOCAL hloc32;
    INT count;
    CHAR *text;

    if (!(GetWindowLongW( hwnd, GWL_STYLE ) & ES_MULTILINE)) return;
    if (!hloc16) return;

    stack16->ds = hInstance;
    count = LocalSize16(hloc16);
    text = MapSL(LocalLock16(hloc16));
    if ((hloc32 = LocalAlloc(LMEM_MOVEABLE, count)))
    {
        memcpy( LocalLock(hloc32), text, count );
        LocalUnlock(hloc32);
        LocalUnlock16(hloc16);
        SetWindowWord16( HWND_16(hwnd), GWW_HANDLE16, hloc16 );
    }
    stack16->ds = oldDS;

    if (hloc32) wow_handlers32.edit_proc( hwnd, EM_SETHANDLE, (WPARAM)hloc32, 0, FALSE );
}

static void edit_destroy_handle( HWND hwnd )
{
    HLOCAL16 hloc16 = GetWindowWord16( HWND_16(hwnd), GWW_HANDLE16 );
    if (hloc16)
    {
        STACK16FRAME* stack16 = MapSL(PtrToUlong(getWOW32Reserved()));
        HANDLE16 oldDS = stack16->ds;

        stack16->ds = GetWindowWord16( HWND_16(hwnd), GWLP_HINSTANCE );
        while (LocalUnlock16(hloc16)) ;
        LocalFree16(hloc16);
        stack16->ds = oldDS;
        SetWindowWord16( HWND_16(hwnd), GWW_HANDLE16, 0 );
    }
}

static LRESULT edit_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
, LRESULT *result, void *arg, BOOL *f)
{
    static const UINT msg16_offset = EM_GETSEL16 - EM_GETSEL;
    *f = TRUE;

    //edit_lock_buffer( hwnd );
    switch (msg)
    {
    case EM_SCROLL16:
    case EM_SCROLLCARET16:
    case EM_GETMODIFY16:
    case EM_SETMODIFY16:
    case EM_GETLINECOUNT16:
    case EM_GETTHUMB16:
    case EM_LINELENGTH16:
    case EM_LIMITTEXT16:
    case EM_CANUNDO16:
    case EM_UNDO16:
    case EM_FMTLINES16:
    case EM_LINEFROMCHAR16:
    case EM_SETPASSWORDCHAR16:
    case EM_EMPTYUNDOBUFFER16:
    case EM_SETREADONLY16:
    case EM_GETPASSWORDCHAR16:
        /* these messages missing from specs */
    case WM_USER + 15:
    case WM_USER + 16:
    case WM_USER + 19:
    case WM_USER + 26:
        return callback(hwnd, msg - msg16_offset, wParam, lParam, result, arg);
    case EM_GETSEL16:
        return callback(hwnd, msg - msg16_offset, 0, 0, result, arg);
    case EM_REPLACESEL16:
    case EM_GETLINE16:
        return callback(hwnd, msg - msg16_offset, wParam, (LPARAM)MapSL(lParam), result, arg);
    case EM_LINESCROLL16:
        return callback(hwnd, msg - msg16_offset, (INT)(SHORT)HIWORD(lParam), (INT)(SHORT)LOWORD(lParam), result, arg);
        break;
    case EM_LINEINDEX16:
        if ((INT16)wParam == -1) wParam = -1;
        return callback(hwnd, msg - msg16_offset, wParam, lParam, result, arg);
    case EM_SETSEL16:
        if ((short)LOWORD(lParam) == -1)
        {
            wParam = -1;
            lParam = 0;
        }
        else
        {
            wParam = LOWORD(lParam);
            lParam = HIWORD(lParam);
        }
        return callback(hwnd, msg - msg16_offset, wParam, lParam, result, arg);
    case EM_GETRECT16:
        if (lParam)
        {
            RECT rect;
            RECT16 *r16 = MapSL(lParam);
            if (SELECTOROF(r16) == 0x0000)
            {
                ERR("EM_GETRECT16:Invalid pointer(%02X:%02X)\n", SELECTOROF(lParam), OFFSETOF(lParam));
                return 0;
            }
            LRESULT a = callback(hwnd, msg - msg16_offset, wParam, (LPARAM)&rect, result, arg);
            r16->left = rect.left;
            r16->top = rect.top;
            r16->right = rect.right;
            r16->bottom = rect.bottom;
            return a;
        }
        break;
    case EM_SETRECT16:
    case EM_SETRECTNP16:
        if (lParam)
        {
            RECT rect;
            RECT16 *r16 = MapSL(lParam);
            rect.left = r16->left;
            rect.top = r16->top;
            rect.right = r16->right;
            rect.bottom = r16->bottom;
            return callback(hwnd, msg - msg16_offset, wParam, (LPARAM)&rect, result, arg);
        }
        break;
    case EM_SETHANDLE16:
        edit_set_handle(hwnd, (HLOCAL16)wParam);
        *result = FALSE;
        return *result;
    case EM_GETHANDLE16:
        *result = edit_get_handle(hwnd);
        return *result;
    case EM_SETTABSTOPS16:
    {
        INT16 *tabs16 = MapSL(lParam);
        INT i, count = wParam, *tabs = NULL;
        if (count > 0)
        {
            if (!(tabs = HeapAlloc(GetProcessHeap(), 0, count * sizeof(*tabs)))) return 0;
            for (i = 0; i < count; i++) tabs[i] = tabs16[i];
        }
        LRESULT r = callback(hwnd, msg - msg16_offset, wParam, (LPARAM)tabs, result, arg);
        HeapFree(GetProcessHeap(), 0, tabs);
        return r;
    }
    case EM_GETFIRSTVISIBLELINE16:
        if (!(GetWindowLongW(hwnd, GWL_STYLE) & ES_MULTILINE)) break;
        return callback(hwnd, msg - msg16_offset, wParam, lParam, result, arg);
    case EM_SETWORDBREAKPROC16:
    {
        struct word_break_thunk *thunk = add_word_break_thunk((EDITWORDBREAKPROC16)lParam);
        return callback(hwnd, EM_SETWORDBREAKPROC, wParam, (LPARAM)thunk, result, arg);
    }
    case EM_GETWORDBREAKPROC16:
        callback(hwnd, msg, wParam, lParam, result, arg);
        *result = (LRESULT)get_word_break_thunk((EDITWORDBREAKPROCA)*result);
        return *result;
    case WM_NCDESTROY:
        edit_destroy_handle(hwnd);
        return callback(hwnd, msg, wParam, lParam, result, arg);/* no unlock on destroy */
    case WM_HSCROLL:
    case WM_VSCROLL:
        if (LOWORD(wParam) == EM_GETTHUMB16 || LOWORD(wParam) == EM_LINESCROLL16) wParam -= msg16_offset;
        return callback(hwnd, msg, wParam, lParam, result, arg);
    }
    //edit_unlock_buffer( hwnd );
    *f = FALSE;
    return *result;
}


/*********************************************************************
 *	edit_proc16
 */
static LRESULT edit_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    static const UINT msg16_offset = EM_GETSEL16 - EM_GETSEL;
    LRESULT result = 0;

    //edit_lock_buffer( hwnd );
    switch (msg)
    {
    case EM_SCROLL16:
    case EM_SCROLLCARET16:
    case EM_GETMODIFY16:
    case EM_SETMODIFY16:
    case EM_GETLINECOUNT16:
    case EM_GETTHUMB16:
    case EM_LINELENGTH16:
    case EM_LIMITTEXT16:
    case EM_CANUNDO16:
    case EM_UNDO16:
    case EM_FMTLINES16:
    case EM_LINEFROMCHAR16:
    case EM_SETPASSWORDCHAR16:
    case EM_EMPTYUNDOBUFFER16:
    case EM_SETREADONLY16:
    case EM_GETPASSWORDCHAR16:
	/* these messages missing from specs */
    case WM_USER+15:
    case WM_USER+16:
    case WM_USER+19:
    case WM_USER+26:
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, lParam, FALSE );
        break;
    case EM_GETSEL16:
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, 0, 0, FALSE );
        break;
    case EM_REPLACESEL16:
    case EM_GETLINE16:
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, (LPARAM)MapSL(lParam), FALSE );
        break;
    case EM_LINESCROLL16:
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, (INT)(SHORT)HIWORD(lParam),
                                           (INT)(SHORT)LOWORD(lParam), FALSE );
        break;
    case EM_LINEINDEX16:
        if ((INT16)wParam == -1) wParam = -1;
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, lParam, FALSE );
        break;
    case EM_SETSEL16:
        if ((short)LOWORD(lParam) == -1)
        {
            wParam = -1;
            lParam = 0;
        }
        else
        {
            wParam = LOWORD(lParam);
            lParam = HIWORD(lParam);
        }
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, lParam, FALSE );
        break;
    case EM_GETRECT16:
        if (lParam)
        {
            RECT rect;
            RECT16 *r16 = MapSL(lParam);
            if (SELECTOROF(r16) == 0x0000)
            {
                ERR("EM_GETRECT16:Invalid pointer(%02X:%02X)\n", SELECTOROF(lParam), OFFSETOF(lParam));
                return 0;
            }
            wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, (LPARAM)&rect, FALSE );
            r16->left   = rect.left;
            r16->top    = rect.top;
            r16->right  = rect.right;
            r16->bottom = rect.bottom;
        }
        break;
    case EM_SETRECT16:
    case EM_SETRECTNP16:
        if (lParam)
        {
            RECT rect;
            RECT16 *r16 = MapSL(lParam);
            rect.left   = r16->left;
            rect.top    = r16->top;
            rect.right  = r16->right;
            rect.bottom = r16->bottom;
            wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, (LPARAM)&rect, FALSE );
        }
        break;
    case EM_SETHANDLE16:
        edit_set_handle( hwnd, (HLOCAL16)wParam );
        break;
    case EM_GETHANDLE16:
        result = edit_get_handle( hwnd );
        break;
    case EM_SETTABSTOPS16:
    {
        INT16 *tabs16 = MapSL(lParam);
        INT i, count = wParam, *tabs = NULL;
        if (count > 0)
        {
            if (!(tabs = HeapAlloc( GetProcessHeap(), 0, count * sizeof(*tabs) ))) return 0;
            for (i = 0; i < count; i++) tabs[i] = tabs16[i];
        }
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, count, (LPARAM)tabs, FALSE );
        HeapFree( GetProcessHeap(), 0, tabs );
        break;
    }
    case EM_GETFIRSTVISIBLELINE16:
        if (!(GetWindowLongW( hwnd, GWL_STYLE ) & ES_MULTILINE)) break;
        result = wow_handlers32.edit_proc( hwnd, msg - msg16_offset, wParam, lParam, FALSE );
        break;
    case EM_SETWORDBREAKPROC16:
    {
        struct word_break_thunk *thunk = add_word_break_thunk( (EDITWORDBREAKPROC16)lParam );
        result = wow_handlers32.edit_proc( hwnd, EM_SETWORDBREAKPROC, wParam, (LPARAM)thunk, FALSE );
        break;
    }
    case EM_GETWORDBREAKPROC16:
        result = wow_handlers32.edit_proc( hwnd, EM_GETWORDBREAKPROC, wParam, lParam, FALSE );
        result = (LRESULT)get_word_break_thunk( (EDITWORDBREAKPROCA)result );
        break;
    case WM_NCDESTROY:
        edit_destroy_handle( hwnd );
        return wow_handlers32.edit_proc( hwnd, msg, wParam, lParam, unicode );  /* no unlock on destroy */
    case WM_HSCROLL:
    case WM_VSCROLL:
        if (LOWORD(wParam) == EM_GETTHUMB16 || LOWORD(wParam) == EM_LINESCROLL16) wParam -= msg16_offset;
        result = wow_handlers32.edit_proc( hwnd, msg, wParam, lParam, unicode );
        break;
    default:
        result = wow_handlers32.edit_proc( hwnd, msg, wParam, lParam, unicode );
        break;
    }
    //edit_unlock_buffer( hwnd );
    return result;
}


/***********************************************************************
 *           listbox_proc16
 */
static LRESULT listbox_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    static const UINT msg16_offset = LB_ADDSTRING16 - LB_ADDSTRING;
    LRESULT ret;

    switch (msg)
    {
    case WM_SIZE:
        if (is_old_app( hwnd ))
        {
            DWORD style = GetWindowLongW( hwnd, GWL_STYLE );
            int height, remaining, item_height;
            RECT rect;

            /* give a margin for error to old 16 bits programs - if we need
               less than the height of the nonclient area, round to the
               *next* number of items */

            if (!(style & LBS_NOINTEGRALHEIGHT) && !(style & LBS_OWNERDRAWVARIABLE))
            {
                GetClientRect( hwnd, &rect );
                height = rect.bottom - rect.top;
                item_height = wow_handlers32.listbox_proc( hwnd, LB_GETITEMHEIGHT, 0, 0, FALSE );
                remaining = item_height ? (height % item_height) : 0;
                if ((height > item_height) && remaining)
                {
                    GetWindowRect( hwnd, &rect );
                    if ((item_height - remaining) <= rect.bottom - rect.top - height)
                        remaining = remaining - item_height;
                    TRACE( "[%p]: changing height %d -> %d\n", hwnd, height, height - remaining );
                    SetWindowPos( hwnd, 0, 0, 0, rect.right - rect.left,
                                  rect.bottom - rect.top - remaining,
                                  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE );
                    return 0;
                }
            }
        }
        return wow_handlers32.listbox_proc( hwnd, msg, wParam, lParam, unicode );

    case LB_RESETCONTENT16:
    case LB_DELETESTRING16:
    case LB_GETITEMDATA16:
    case LB_SETITEMDATA16:
    case LB_GETCOUNT16:
    case LB_GETTEXTLEN16:
    case LB_GETCURSEL16:
    case LB_GETTOPINDEX16:
    case LB_GETITEMHEIGHT16:
    case LB_SETCARETINDEX16:
    case LB_GETCARETINDEX16:
    case LB_SETTOPINDEX16:
    case LB_SETCOLUMNWIDTH16:
    case LB_GETSELCOUNT16:
    case LB_SELITEMRANGE16:
    case LB_SELITEMRANGEEX16:
    case LB_GETHORIZONTALEXTENT16:
    case LB_SETHORIZONTALEXTENT16:
    case LB_GETANCHORINDEX16:
    case LB_CARETON16:
    case LB_CARETOFF16:
        msg -= msg16_offset;
        break;
    case LB_GETSEL16:
    case LB_SETSEL16:
    case LB_SETCURSEL16:
    case LB_SETANCHORINDEX16:
        wParam = (INT)(INT16)wParam;
        msg -= msg16_offset;
        break;
    case LB_INSERTSTRING16:
    case LB_FINDSTRING16:
    case LB_FINDSTRINGEXACT16:
    case LB_SELECTSTRING16:
        wParam = (INT)(INT16)wParam;
        /* fall through */
    case LB_ADDSTRING16:
    case LB_ADDFILE16:
    {
        DWORD style = GetWindowLongW( hwnd, GWL_STYLE );
        if ((style & LBS_HASSTRINGS) || !(style & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)))
            lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    }
    case LB_GETTEXT16:
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case LB_SETITEMHEIGHT16:
        lParam = LOWORD(lParam);
        msg -= msg16_offset;
        break;
    case LB_GETITEMRECT16:
        {
            RECT rect;
            RECT16 *r16 = MapSL(lParam);
            ret = wow_handlers32.listbox_proc( hwnd, LB_GETITEMRECT, (INT16)wParam, (LPARAM)&rect, FALSE );
            r16->left   = rect.left;
            r16->top    = rect.top;
            r16->right  = rect.right;
            r16->bottom = rect.bottom;
            return ret;
        }
    case LB_GETSELITEMS16:
    {
        INT16 *array16 = MapSL( lParam );
        INT i, count = (INT16)wParam, *array;
        if (!(array = HeapAlloc( GetProcessHeap(), 0, wParam * sizeof(*array) ))) return LB_ERRSPACE;
        ret = wow_handlers32.listbox_proc( hwnd, LB_GETSELITEMS, count, (LPARAM)array, FALSE );
        for (i = 0; i < ret; i++) array16[i] = array[i];
        HeapFree( GetProcessHeap(), 0, array );
        return ret;
    }
    case LB_DIR16:
        /* according to Win16 docs, DDL_DRIVES should make DDL_EXCLUSIVE
         * be set automatically (this is different in Win32) */
        if (wParam & DDL_DRIVES) wParam |= DDL_EXCLUSIVE;
        lParam = (LPARAM)MapSL(lParam);
        msg -= msg16_offset;
        break;
    case LB_SETTABSTOPS16:
    {
        INT i, count, *tabs = NULL;
        INT16 *tabs16 = MapSL( lParam );

        if ((count = (INT16)wParam) > 0)
        {
            if (!(tabs = HeapAlloc( GetProcessHeap(), 0, wParam * sizeof(*tabs) ))) return LB_ERRSPACE;
            for (i = 0; i < count; i++) tabs[i] = tabs16[i];
        }
        ret = wow_handlers32.listbox_proc( hwnd, LB_SETTABSTOPS, count, (LPARAM)tabs, FALSE );
        HeapFree( GetProcessHeap(), 0, tabs );
        return ret;
    }
    default:
        return wow_handlers32.listbox_proc( hwnd, msg, wParam, lParam, unicode );
    }
    return wow_handlers32.listbox_proc( hwnd, msg, wParam, lParam, FALSE );
}


/***********************************************************************
 *           mdiclient_proc16
 */
static LRESULT mdiclient_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    if (msg == WM_CREATE)
    {
        LPCREATESTRUCTA cs = (LPCREATESTRUCTA)lParam;
        HINSTANCE instance = (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE );
        BOOL is_win32 = !instance || ((ULONG_PTR)instance >> 16);

	/* Translation layer doesn't know what's in the cs->lpCreateParams
	 * so we have to keep track of what environment we're in. */
	if (!is_win32)
	{
            void *orig = cs->lpCreateParams;
            LRESULT ret;
            CLIENTCREATESTRUCT ccs;
            CLIENTCREATESTRUCT16 *ccs16 = MapSL( PtrToUlong( orig ));

            ccs.hWindowMenu  = HMENU_32(ccs16->hWindowMenu);
            ccs.idFirstChild = ccs16->idFirstChild;
            cs->lpCreateParams = &ccs;
            ret = wow_handlers32.mdiclient_proc( hwnd, msg, wParam, lParam, unicode );
            cs->lpCreateParams = orig;
            return ret;
	}
    }
    return wow_handlers32.mdiclient_proc( hwnd, msg, wParam, lParam, unicode );
}

static LRESULT scrollbar_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f)
{
    static const UINT msg16_offset = SBM_SETPOS16 - SBM_SETPOS;
    *f = TRUE;
    switch (msg)
    {
    case SBM_SETPOS16:
    case SBM_GETPOS16:
    case SBM_ENABLE_ARROWS16:
        msg -= msg16_offset;
        return callback(hwnd, msg, wParam, lParam, result, arg);
    case SBM_SETRANGE16:
        msg = wParam ? SBM_SETRANGEREDRAW : SBM_SETRANGE;
        wParam = LOWORD(lParam);
        lParam = HIWORD(lParam);
        return callback(hwnd, msg, wParam, lParam, result, arg);
    case SBM_GETRANGE16:
    {
        INT min, max;
        callback(hwnd, SBM_GETRANGE, (WPARAM)&min, (LPARAM)&max, result, arg);
        return *result = MAKELRESULT(min, max);
    }
    }
    return *f = FALSE;
}
/***********************************************************************
 *           scrollbar_proc16
 */
static LRESULT scrollbar_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    static const UINT msg16_offset = SBM_SETPOS16 - SBM_SETPOS;

    switch (msg)
    {
    case SBM_SETPOS16:
    case SBM_GETPOS16:
    case SBM_ENABLE_ARROWS16:
        msg -= msg16_offset;
        break;
    case SBM_SETRANGE16:
        msg = wParam ? SBM_SETRANGEREDRAW : SBM_SETRANGE;
        wParam = LOWORD(lParam);
        lParam = HIWORD(lParam);
        break;
    case SBM_GETRANGE16:
    {
        INT min, max;
        wow_handlers32.scrollbar_proc( hwnd, SBM_GETRANGE, (WPARAM)&min, (LPARAM)&max, FALSE );
        return MAKELRESULT(min, max);
    }
    default:
        return wow_handlers32.scrollbar_proc( hwnd, msg, wParam, lParam, unicode );
    }
    return wow_handlers32.scrollbar_proc( hwnd, msg, wParam, lParam, FALSE );
}

static LRESULT static_proc_CallProc16To32A(winproc_callback_t callback, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode
    , LRESULT *result, void *arg, BOOL *f)
{
    LRESULT ret;
    switch (msg)
    {
    case STM_SETICON16:
        wParam = (WPARAM)get_icon_32((HICON16)wParam);
        *f = TRUE;
        ret = callback(hwnd, STM_GETICON, wParam, lParam, result, arg);
        return ret;
    case STM_GETICON16:
        ret = callback(hwnd, STM_GETICON, wParam, lParam, result, arg);
        *result = get_icon_16((HICON)*result);
        *f = TRUE;
        return ret;
    default:
        break;
    }
    return *f = FALSE;
}
/***********************************************************************
 *           static_proc16
 */
static LRESULT static_proc16( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL unicode )
{
    switch (msg)
    {
    case WM_CREATE://WM_NCCREATE:
    {
        CREATESTRUCTA *cs = (CREATESTRUCTA *)lParam;
        if (((ULONG_PTR)cs->hInstance >> 16)) break;  /* 32-bit instance, nothing to do */
        switch (cs->style & SS_TYPEMASK)
        {
        case SS_ICON:
            {
                LRESULT ret = wow_handlers32.static_proc(hwnd, msg, wParam, lParam, unicode);
                SetWindowTextA(hwnd, "");
                HICON16 icon = LoadIcon16( HINSTANCE_16(cs->hInstance), cs->lpszName );
                if (!icon) icon = LoadCursor16( HINSTANCE_16(cs->hInstance), cs->lpszName );
                if (icon) wow_handlers32.static_proc( hwnd, STM_SETIMAGE, IMAGE_ICON,
                                                      (LPARAM)get_icon_32(icon), FALSE );
                return ret;
            }
        case SS_BITMAP:
            {
                LRESULT ret = wow_handlers32.static_proc(hwnd, msg, wParam, lParam, unicode);
                SetWindowTextA(hwnd, "");
                HBITMAP16 bitmap = LoadBitmap16( HINSTANCE_16(cs->hInstance), cs->lpszName );
                if (bitmap) wow_handlers32.static_proc( hwnd, STM_SETIMAGE, IMAGE_BITMAP,
                                                        (LPARAM)HBITMAP_32(bitmap), FALSE );
                return ret;
            }
        }
        break;
    }
    case STM_SETICON16:
        wParam = (WPARAM)get_icon_32( (HICON16)wParam );
        return wow_handlers32.static_proc( hwnd, STM_SETICON, wParam, lParam, FALSE );
    case STM_GETICON16:
        return get_icon_16( (HICON)wow_handlers32.static_proc( hwnd, STM_GETICON, wParam, lParam, FALSE ));
    default:
        break;
    }
    return wow_handlers32.static_proc(hwnd, msg, wParam, lParam, unicode);
}
LRESULT CALLBACK static_wndproc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return static_proc16(hwnd, msg, wParam, lParam, FALSE);
}
LRESULT CALLBACK listbox_wndproc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return listbox_proc16(hwnd, msg, wParam, lParam, FALSE);
}
LRESULT CALLBACK button_wndproc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return button_proc16(hwnd, msg, wParam, lParam, FALSE);
}
LRESULT CALLBACK edit_wndproc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return edit_proc16(hwnd, msg, wParam, lParam, FALSE);
}

BOOL aero_diasble;
enum SEPARATE_TASKBAR {
    SEPARATE_TASKBAR_DONT_SEPARATE = 0,
    SEPARATE_TASKBAR_SEPARATE_FOR_EACH_WOW = 1,
    SEPARATE_TASKBAR_SEPARATE = 2,
};
enum SEPARATE_TASKBAR separate_taskbar;

/* set taskbar id */
#include <shlobj.h>
#include <propkey.h>
#include <propvarutil.h>
void set_app_id(HWND hWnd, LPCWSTR name)
{
    DWORD count;
    IPropertyStore *propstore;
    HRESULT hr;
    static HMODULE shell32;
    static HRESULT (STDAPICALLTYPE*pSHGetPropertyStoreForWindow)(HWND hwnd, REFIID riid, void** ppv);
    if (!shell32)
    {
        shell32 = GetModuleHandleA("shell32");
        pSHGetPropertyStoreForWindow = (HRESULT (STDAPICALLTYPE*)(HWND hwnd, REFIID riid, void** ppv))GetProcAddress(shell32, "SHGetPropertyStoreForWindow");
    }
    if (!pSHGetPropertyStoreForWindow)
        return;
    ReleaseThunkLock(&count);
    hr = pSHGetPropertyStoreForWindow(hWnd, &IID_IPropertyStore, &propstore);
    if (SUCCEEDED(hr))
    {
        PROPVARIANT pv;
        SIZE_T len = (wcslen(name) + 1) * sizeof(*name);
        LPWSTR id2 = CoTaskMemAlloc(len);
        if (id2)
        {
            memcpy(id2, name, len);
            V_UNION(&pv, pwszVal) = id2;
            V_VT(&pv) = VT_LPWSTR;
            hr = propstore->lpVtbl->SetValue(propstore, &PKEY_AppUserModel_ID, &pv);
            PropVariantClear(&pv);
        }
        propstore->lpVtbl->Release(propstore);
    }
    RestoreThunkLock(count);
}
void set_window_app_id(HWND hwnd)
{
    /* OTVDM.{PID}.{module path} */
    WCHAR buffer[1024];
    CHAR a[MAX_PATH];
    INT16 len = GetModuleFileName16(GetCurrentTask(), a, MAX_PATH);
    /* tiny canonicalize */
    for (int i = 0; i < len; i++)
    {
        a[i] = (char)toupper(a[i]);
    }
    if (separate_taskbar == SEPARATE_TASKBAR_SEPARATE_FOR_EACH_WOW)
    {
        wsprintfW(buffer, L"OTVDM.%d.%S", GetCurrentProcessId(), a);
    }
    else
    {
        wsprintfW(buffer, L"OTVDM.%S", a);
    }
    set_app_id(hwnd, buffer);
}
void detect_window_type(HWND16 hwnd, HWND hwnd32)
{
    char name[100];
    RealGetWindowClassA(hwnd32, name, 100);
    /* detect window type */
    if (isListBox(hwnd, hwnd32) || !stricmp(name, "LISTBOX") || !stricmp(name, "COMBOLBOX"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_LISTBOX;
    }
    if (isComboBox(hwnd, hwnd32) || !stricmp(name, "COMBOBOX"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_COMBOBOX;
    }
    if (isButton(hwnd, hwnd32) || !stricmp(name, "BUTTON"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_BUTTON;
    }
    if (isEdit(hwnd, hwnd32) || !stricmp(name, "EDIT"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_EDIT;
    }
    if (isScrollBar(hwnd, hwnd32) || !stricmp(name, "SCROLLBAR"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_SCROLLBAR;
    }
    if (isStatic(hwnd, hwnd32) || !stricmp(name, "STATIC"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_STATIC;
    }
    if (is_mdiclient(hwnd, hwnd32) || !stricmp(name, "MDICLIENT"))
    {
        window_type_table[hwnd] = (BYTE)WINDOW_TYPE_MDICLIENT;
    }
}
DWORD hhook_tls_index;
typedef struct
{
    HHOOK wndproc;
    HHOOK wndprocret;
    HHOOK cbt;
} user_hook_data;
LRESULT CALLBACK CBTHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    user_hook_data *hd = (user_hook_data*)TlsGetValue(hhook_tls_index);
    HHOOK hook = hd ? hd->cbt : NULL;
    if (nCode < 0)
    {
        return CallNextHookEx(hook, nCode, wParam, lParam);
    }
    if (nCode == HCBT_CREATEWND)
    {
        HWND hWnd = (HWND)wParam;
        LPCBT_CREATEWNDA create = (LPCBT_CREATEWNDA)lParam;
        if (aero_diasble)
        {
            SetThemeAppProperties(0);
            SetWindowTheme(hWnd, L"", L"");
        }
        if (separate_taskbar)
        {
            set_window_app_id(hWnd);
        }
        detect_window_type(HWND_16(hWnd), hWnd);
    }
    return FALSE;
}
LRESULT CALLBACK WndProcRetHook(int code, WPARAM wParam, LPARAM lParam)
{
    user_hook_data *hd = (user_hook_data*)TlsGetValue(hhook_tls_index);
    HHOOK hook = hd ? hd->wndprocret : NULL;
    if (code < 0)
        return CallNextHookEx(hook, code, wParam, lParam);
    if (code == HC_ACTION)
    {
        CWPRETSTRUCT *pcwp = (CWPRETSTRUCT *)lParam;
        HWND hwnd = pcwp->hwnd;
        if (!IsWindow(hwnd))
        {
            return CallNextHookEx(hook, code, wParam, lParam);
        }
        HWND16 hwnd16 = HWND_16(hwnd);
        detect_window_type(hwnd16, hwnd);
        if (window_type_table[hwnd16] == WINDOW_TYPE_STATIC)
        {
            if (pcwp->message == WM_CREATE)
            {
                do
                {
                    LPARAM lParam = pcwp->lParam;
                    WPARAM wParam = pcwp->wParam;
                    UINT msg = pcwp->message;
                    HWND hwnd = pcwp->hwnd;
                    CREATESTRUCTA *cs = (CREATESTRUCTA *)lParam;
                    if (!cs)
                        break;
                    if (((ULONG_PTR)cs->hInstance >> 16)) break;  /* 32-bit instance, nothing to do */
                    switch (cs->style & SS_TYPEMASK)
                    {
                    case SS_ICON:
                    {
                        SetWindowTextA(hwnd, "");
                        HICON16 icon = LoadIcon16(HINSTANCE_16(cs->hInstance), cs->lpszName);
                        if (!icon) icon = LoadCursor16(HINSTANCE_16(cs->hInstance), cs->lpszName);
                        if (icon) wow_handlers32.static_proc(hwnd, STM_SETIMAGE, IMAGE_ICON,
                            (LPARAM)get_icon_32(icon), FALSE);
                        break;
                    }
                    case SS_BITMAP:
                    {
                        SetWindowTextA(hwnd, "");
                        HBITMAP16 bitmap = LoadBitmap16(HINSTANCE_16(cs->hInstance), cs->lpszName);
                        if (bitmap) wow_handlers32.static_proc(hwnd, STM_SETIMAGE, IMAGE_BITMAP,
                            (LPARAM)HBITMAP_32(bitmap), FALSE);
                        break;
                    }
                    }
                } while (0);
            }
        }
    }

    return CallNextHookEx(hook, code, wParam, lParam);
}
LRESULT CALLBACK WndProcHook(int code, WPARAM wParam, LPARAM lParam)
{
    user_hook_data *hd = (user_hook_data*)TlsGetValue(hhook_tls_index);
    HHOOK hook = hd ? hd->wndproc : NULL;
    if (code < 0)
        return CallNextHookEx(hook, code, wParam, lParam);
    if (code == HC_ACTION) {
        if (wParam == 0) {
            CWPSTRUCT *pcwp = (CWPSTRUCT *)lParam;
            //I don't know.
            if (pcwp->message == WM_SHOWWINDOW)
            {
                //see 42353ecbadd096358f250a9dd931d4cf0981b417 reactos win32ss/user/ntuser/winpos.c:2551
                SendMessageA(pcwp->hwnd, WM_SETVISIBLE, pcwp->wParam, 0);
                if (aero_diasble)
                {
                    SetWindowTheme(pcwp->hwnd, L"", L"");
                }
            }
        }
    }

    return CallNextHookEx(hook, code, wParam, lParam);
}
LRESULT wow_static_proc_wrapper(HWND a, UINT b, WPARAM c, LPARAM d, BOOL e)
{
    static WNDPROC proc = NULL;
    if (!proc)
    {
        proc = get_classinfo_wndproc("STATIC");
    }
    return CallWindowProcA(proc, a, b, c, d);
}
LRESULT wow_listbox_proc_wrapper(HWND a, UINT b, WPARAM c, LPARAM d, BOOL e)
{
    static WNDPROC proc = NULL;
    if (!proc)
    {
        proc = get_classinfo_wndproc("LISTBOX");
    }
    return CallWindowProcA(proc, a, b, c, d);
}
LRESULT wow_button_proc_wrapper(HWND a, UINT b, WPARAM c, LPARAM d, BOOL e)
{
    static WNDPROC proc = NULL;
    if (!proc)
    {
        proc = get_classinfo_wndproc("BUTTON");
    }
    return CallWindowProcA(proc, a, b, c, d);
}
LRESULT wow_edit_proc_wrapper(HWND a, UINT b, WPARAM c, LPARAM d, BOOL e)
{
    static WNDPROC proc = NULL;
    if (!proc)
    {
        proc = get_classinfo_wndproc("EDIT");
    }
    return CallWindowProcA(proc, a, b, c, d);
}
void InitNewThreadHook()
{
    if (TlsGetValue(hhook_tls_index))
        return;
    user_hook_data *hd = (user_hook_data*)HeapAlloc(GetProcessHeap(), 0, sizeof(user_hook_data));
    TlsSetValue(hhook_tls_index, hd);
    hd->wndproc = SetWindowsHookExA(WH_CALLWNDPROC, WndProcHook, GetModuleHandle(NULL), GetCurrentThreadId());
    hd->wndprocret = SetWindowsHookExA(WH_CALLWNDPROCRET, WndProcRetHook, GetModuleHandle(NULL), GetCurrentThreadId());
    hd->cbt = SetWindowsHookExA(WH_CBT, CBTHook, GetModuleHandle(NULL), GetCurrentThreadId());
}
void InitHook()
{
    isStatic(0, NULL);
    isListBox(0, NULL);
    isButton(0, NULL);
    isEdit(0, NULL);
    isComboBox(0, NULL);
    isScrollBar(0, NULL);
    is_mdiclient(0, NULL);
    hhook_tls_index = TlsAlloc();
    InitNewThreadHook();
}

void WINAPI ShellDDEInit(BOOL bInit);
#include "wine/winbase16.h"
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved
)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        window_type_table = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 65536);
        aero_diasble = krnl386_get_config_int("otvdm", "DisableAero", TRUE);
        if (!IsThemeActive())
        {
            aero_diasble = FALSE;
        }
        drag_list_message = RegisterWindowMessage(DRAGLISTMSGSTRING);
        separate_taskbar = krnl386_get_config_int("otvdm", "SeparateTaskbar", SEPARATE_TASKBAR_SEPARATE);
        ShellDDEInit(TRUE);
    }
    if (fdwReason == DLL_PROCESS_DETACH)
    {
        ShellDDEInit(FALSE);
    }
    return TRUE;
}
BOOL16 WINAPI WaitMessage16()
{
    DWORD lock;
    ReleaseThunkLock(&lock);
	BOOL ret = WaitMessage();
    RestoreThunkLock(lock);
    return ret;
}
/***********************************************************************
 *           wait_message16
 */
static DWORD wait_message16( DWORD count, const HANDLE *handles, DWORD timeout, DWORD mask, DWORD flags )
{
    DWORD lock, ret;

    ReleaseThunkLock( &lock );
    ret = wow_handlers32.wait_message( count, handles, timeout, mask, flags );
    RestoreThunkLock( lock );
    return ret;
}


/***********************************************************************
 *           create_window16
 */
HWND create_window16( CREATESTRUCTW *cs, LPCWSTR className, HINSTANCE instance, BOOL unicode )
{
    /* map to module handle */
    /*if (instance && !((ULONG_PTR)instance >> 16))
        instance = HINSTANCE_32( GetExePtr( HINSTANCE_16(instance) ));*/

    return wow_handlers32.create_window( cs, className, instance, unicode );
}


/***********************************************************************
 *           free_icon_param
 */
static void free_icon_param( ULONG_PTR param )
{
    GlobalFree16( LOWORD(param) );
}

typedef struct tagHWND16Data
{
	HWND hwnd;
	struct tagHWND16Data *next;
	struct tagHWND16Data *prev;
} HWND16Data;

HWND get_win_handle(HWND hWnd16)
{
	return HWND_32((HWND16)hWnd16);
}
HWND create_window(CREATESTRUCTW* cs, LPCWSTR className, HINSTANCE instance, BOOL unicode)
{
    CLIENTCREATESTRUCT c32;
    if (!strncasecmp((LPCSTR)className, "MDICLIENT", strlen((LPCSTR)className)))
    {
        CLIENTCREATESTRUCT16 *c16 = MapSL(cs->lpCreateParams);
        c32.idFirstChild = c16->idFirstChild;
        c32.hWindowMenu = HMENU_32(c16->hWindowMenu);
        cs->lpCreateParams = (LPVOID)&c32;
    }
	SetLastError(0);
	HWND hWnd = CreateWindowExA(cs->dwExStyle, (LPCSTR)className, (LPCSTR)cs->lpszName, cs->style, cs->x, cs->y, cs->cx, cs->cy, cs->hwndParent, cs->hMenu, instance, cs->lpCreateParams);
	if (hWnd == 0)
	{
		if (GetLastError() == ERROR_INVALID_MENU_HANDLE)
		{
			cs->hMenu = HMENU_32((HMENU16)cs->hMenu);
			hWnd = CreateWindowExA(cs->dwExStyle, (LPCSTR)className, (LPCSTR)cs->lpszName, cs->style, cs->x, cs->y, cs->cx, cs->cy, cs->hwndParent, cs->hMenu, instance, cs->lpCreateParams);
		}
	}
	return hWnd;
}
__declspec(dllimport) HICON16 K32HICON_16(HICON handle);
__declspec(dllimport) HICON K32HICON_32(HICON16 handle);
ULONG_PTR hIconParams[65536];
//HICON->HICON16??
ULONG_PTR get_icon_param(HICON hIcon)
{
	return hIconParams[K32HICON_16(hIcon)];
}
ULONG_PTR set_icon_param(HICON hIcon, ULONG_PTR param)
{
	return hIconParams[K32HICON_16(hIcon)] = param;
}
struct tagDIALOGINFO unknown;
struct tagDIALOGINFO *get_dialog_info(HWND hWnd, BOOL b)
{
	ERR("should not be called (%p,%d)\n", hWnd, b);
	return &unknown;
}
INT dialog_box_loop(HWND hWnd, HWND owner)
{
    ERR("should not be called (%p,%p)\n");
    return 0;
}
void register_wow_handlers(void)
{
    static const struct wow_handlers16 handlers16 =
    {
        button_proc16,
        combo_proc16,
        edit_proc16,
        listbox_proc16,
        mdiclient_proc16,
        scrollbar_proc16,
        static_proc16,
        wait_message16,
        create_window16,
        call_window_proc_Ato16,
        call_dialog_proc_Ato16,
        free_icon_param
    };

	struct wow_handlers32 wow_handlers322 = { 0 };
    /* UserRegisterWowHandlers( &handlers16, &wow_handlers322 ); */
	wow_handlers32 = wow_handlers322;
	wow_handlers32.get_win_handle = get_win_handle;
	wow_handlers32.create_window = create_window;
	wow_handlers32.get_icon_param = get_icon_param;
	wow_handlers32.set_icon_param = set_icon_param;
	wow_handlers32.get_dialog_info = get_dialog_info;
	wow_handlers32.dialog_box_loop = dialog_box_loop;

    wow_handlers32.static_proc = wow_static_proc_wrapper;
    wow_handlers32.listbox_proc = wow_listbox_proc_wrapper;
    wow_handlers32.button_proc = wow_button_proc_wrapper;
    wow_handlers32.edit_proc = wow_edit_proc_wrapper;
    InitHook();
}

BOOL is_dialog(HWND hwnd);
HMENU get_dialog_hmenu(HWND hWnd);
LRESULT CALLBACK WindowProc16(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    HWND16 hWnd16 = HWND_16(hDlg);
    if (!GetWndProc16(hWnd16))
    {
        InitWndProc16(hDlg, hWnd16);
    }
    /* some programs don't call DlgProc */
    if (Msg == WM_INITDIALOG && is_dialog(hDlg))
    {
        if (!GetMenu(hDlg))
        {
            SetMenu(hDlg, get_dialog_hmenu(hDlg));
        }
    }
    WNDPROC16 wndproc16 = (WNDPROC16)GetWndProc16(hWnd16);
    if (wndproc16)
    {
        MSG msg = { 0 };
        msg.hwnd = hDlg;
        msg.message = Msg;
        msg.wParam = wParam;
        msg.lParam = lParam;
        LRESULT result;
        if (WINPROC_IsNativeProc(wndproc16))
        {
            int index = winproc_to_index((WNDPROC16)wndproc16);
            if (index >= MAX_WINPROCS32)
                index -= MAX_WINPROCS32;
            WNDPROC wndproc32 = winproc16_array[index];
            return CallWindowProcA(wndproc32, hDlg, Msg, wParam, lParam);
        }
        WINPROC_CallProc32ATo16(call_window_proc16, msg.hwnd, msg.message, msg.wParam, msg.lParam,
            &result, wndproc16);
        return result;
    }
    return DefWindowProcA(hDlg, Msg, wParam, lParam);
}

static LRESULT LPMSG16_32_callback(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
    LRESULT *result, void *arg)
{
    MSG *msg32 = (MSG*)arg;
    msg32->hwnd = hwnd;
    msg32->message = msg;
    msg32->lParam = lp;
    msg32->wParam = wp;
    return *result = 0;
}

static LRESULT LPMSG32_16_callback(HWND16 hwnd, UINT16 msg, WPARAM16 wp, LPARAM lp ,
    LRESULT *result, void *arg)
{
    MSG16 *msg16 = (MSG16*)arg;
    msg16->hwnd = hwnd;
    msg16->message = msg;
    msg16->lParam = lp;
    msg16->wParam = wp;
    return *result = 0;
}
void WINAPI window_message32_16(const MSG *msg32, MSG16 *msg16)
{
    LRESULT ret;
    msg16->pt.x = msg32->pt.x;
    msg16->pt.y = msg32->pt.y;
    msg16->time = msg32->time;
    WINPROC_CallProc32ATo16(LPMSG32_16_callback, msg16->hwnd, msg16->message, msg16->wParam, msg16->lParam, &ret, msg16);
}

void WINAPI window_message16_32(const MSG16 *msg16, MSG *msg32)
{
    LRESULT ret;
    msg32->pt.x = msg16->pt.x;
    msg32->pt.y = msg16->pt.y;
    msg32->time = msg16->time;
    WINPROC_CallProc16To32A(LPMSG16_32_callback, msg16->hwnd, msg16->message, msg16->wParam, msg16->lParam, &ret, msg32);
}
