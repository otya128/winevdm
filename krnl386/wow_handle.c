/*
handle16 <-> wow64 handle32
*/

#include "config.h"
#include "wine/port.h"

#include <assert.h>
#include <stdarg.h>
#include <errno.h>

#include "windows.h"
#include "wine/winbase16.h"
#include "windef.h"
#include "winbase.h"
#include "winerror.h"
#include "windows/wownt32.h"
#include "excpt.h"
#include "winternl.h"
#include "kernel16_private.h"
#include "wine/exception.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(thunk);
#define HANDLE_RESERVED 32
typedef struct
{
	HANDLE handle32;
    DWORD wndproc;
    DWORD dlgproc;
    HINSTANCE16 hInst16;
    HMENU16 hMenu16;
    void *ptr;
    WOW_HANDLE_TYPE type;
} HANDLE_DATA;
typedef struct tagHANDLE_STORAGE *LPHANDLE_STORAGE;
typedef void(*clean_up_t)(LPHANDLE_STORAGE);
typedef struct
{
    HANDLE_DATA *handles;
    LPCSTR name;
    int align;
    int align2;
    clean_up_t clean_up;
} HANDLE_STORAGE;
#define HANDLE_TYPE_HANDLE 0
#define HANDLE_TYPE_HGDI 1
#define HANDLE_TYPE_MAX 2
static HANDLE_STORAGE handle_list[HANDLE_TYPE_MAX];
static BOOL handle_trace;
static BOOL handle_init_flag;
static void hgdi_clean_up(HANDLE_STORAGE* hs);
static void user_handle_clean_up(HANDLE_STORAGE* hs);
static BOOL map_low_word_user_handle;
/* this function called by DllMain(kernel.c) */
void init_wow_handle()
{
    if (handle_init_flag)
        return;
    handle_trace = TRACE_ON(thunk);
    handle_init_flag = TRUE;
    handle_list[HANDLE_TYPE_HANDLE].handles = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 65536 * sizeof(HANDLE_DATA));
    handle_list[HANDLE_TYPE_HANDLE].align = 1;
    /* IsWindow(hdc) should return FALSE */
    handle_list[HANDLE_TYPE_HANDLE].align2 = 4;
    handle_list[HANDLE_TYPE_HANDLE].name = "HANDLE";
    handle_list[HANDLE_TYPE_HANDLE].clean_up = user_handle_clean_up;
    /*
    hdc1 = CreateCompatibleDC(0);
    hdc2 = CreateCompatibleDC(0);
    hdc3 = CreateCompatibleDC(0);
    hdc4 = CreateCompatibleDC(0);

    hdc1: 0x6c, hdc2: 0x70, hdc3: 0x74, hdc4: 0x78...
    PBRUSH expects HDC to be a multiple of 4.
    */
    handle_list[HANDLE_TYPE_HGDI].align = 4;
    handle_list[HANDLE_TYPE_HGDI].name = "HGDI";
    handle_list[HANDLE_TYPE_HGDI].align2 = 0;
    handle_list[HANDLE_TYPE_HGDI].handles = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 65536 * sizeof(HANDLE_DATA));
    handle_list[HANDLE_TYPE_HGDI].clean_up = hgdi_clean_up;
    map_low_word_user_handle = krnl386_get_config_int("otvdm", "MapLowWordUserHandle", FALSE);
}
WORD get_handle16_data(HANDLE h, HANDLE_STORAGE *hs, HANDLE_DATA **o);

static DWORD get_handle_type(HANDLE h, HANDLE_STORAGE *hs)
{
    if (hs != &handle_list[HANDLE_TYPE_HGDI])
        return 0;
    return GetObjectType(h) << 16;
}

BOOL is_reserved_handle32(HANDLE h)
{
    SSIZE_T signedh = (SSIZE_T)h;
    if (signedh < HANDLE_RESERVED && signedh > -HANDLE_RESERVED)
    {
        return TRUE;
    }
    return FALSE;
}
BOOL is_reserved_handle16(HANDLE16 h)
{
    INT16 signedh = (INT16)h;
    if (signedh < HANDLE_RESERVED && signedh > -HANDLE_RESERVED)
    {
        return TRUE;
    }
    return FALSE;
}
WORD get_handle16(HANDLE h, HANDLE_STORAGE *hs, WOW_HANDLE_TYPE type)
{
	if (is_reserved_handle32(h))
	{
		return h;
	}
	HANDLE_DATA *hd;
	int hnd16 = get_handle16_data(h, hs, &hd);
    if (!hd)
        return 0;
	hd->handle32 = h;
    hd->type = type;
	return hnd16;
}
WORD get_handle16_data(HANDLE h, HANDLE_STORAGE *hs, HANDLE_DATA **o)
{
    int retry_count = 0;
	//?
	if (is_reserved_handle32(h))
	{
		*o = &hs->handles[(size_t)h];
		return h;
	}
	WORD fhandle = 0;
    DWORD type = get_handle_type(h, hs);

    WORD s = HANDLE_RESERVED;
retry:
	for (WORD i = s; i; i += hs->align)
	{
        if (i >= (WORD)(-HANDLE_RESERVED))
            break;
        if (hs->align2 && (i & -hs->align2) == i)
            continue;
		if ((!hs->handles[i].handle32 || (hs->handles[i].handle32 == type)) && !fhandle)
		{
			fhandle = i;
		}
		if (hs->handles[i].handle32 == h)
		{
			*o = &hs->handles[i];
			return i;
		}
	}
	if (!fhandle)
	{
        *o = NULL;
		ERR("Could not allocate a handle.\n");
        retry_count++;
        if (retry_count == 1 && hs->clean_up)
        {
            hs->clean_up(hs);
            goto retry;
        }
        return 0;
	}
    if (handle_trace)
        DPRINTF("allocate %s %p=>%04x\n", hs->name, h, fhandle);
	*o = &hs->handles[fhandle];
    memset(*o, 0, sizeof(HANDLE_DATA));
	return fhandle;
}
void destroy_handle16(HANDLE_STORAGE *hs, WORD h)
{
    if (is_reserved_handle32(h))
    {
        return;
    }
    DWORD type = get_handle_type(hs->handles[h].handle32, hs);
    memset(hs->handles + h, 0, sizeof(*hs->handles));
    hs->handles[h].handle32 = type;
}
BOOL get_handle32_data(WORD h, HANDLE_STORAGE *hs, HANDLE_DATA **o)
{
    if (!h)
    {
        *o = NULL;
        return FALSE;
    }
    if (is_reserved_handle16(h))
	{
		*o = &hs->handles[(size_t)h];
		(*o)->handle32 = h;
		return TRUE;
	}
	*o = &hs->handles[h];
    return TRUE;
}
HANDLE get_handle32(WORD h, HANDLE_STORAGE *hs)
{
	if (is_reserved_handle16(h))
	{
        hs->handles[h].handle32 = h;
		return (UINT16)h;
	}
    if (hs->handles[h].handle32)
    {
        return hs->handles[h].handle32;
    }
    hs->handles[h].handle32 = h;
    return (HANDLE)h;
}

static CRITICAL_SECTION *handle_lock = 0;

static void enter_handle_lock()
{
    if (!handle_lock)
    {
        handle_lock = (CRITICAL_SECTION *)HeapAlloc(GetProcessHeap(), 0, sizeof(CRITICAL_SECTION));
        InitializeCriticalSection(handle_lock);
    }
    EnterCriticalSection(handle_lock);
}

static void leave_handle_lock()
{
    LeaveCriticalSection(handle_lock);
}

//handle16 -> wow64 handle32
HANDLE WINAPI K32WOWHandle32User(WORD handle)
{
    HANDLE h32;
    if (map_low_word_user_handle)
    {
        return (HANDLE)handle;
    }
    enter_handle_lock();
    h32 = get_handle32(handle, &handle_list[HANDLE_TYPE_HANDLE]);
    if (handle_trace)
        DPRINTF("HANDLE1632 %04X %p\n", handle, h32);
    leave_handle_lock();
    return h32;
}

HANDLE WINAPI K32WOWHandle32Other(WORD handle)
{
    enter_handle_lock();
    HANDLE h32 = get_handle32(handle, &handle_list[HANDLE_TYPE_HANDLE]);
    if (handle_trace)
        DPRINTF("HANDLE1632 %04X %p\n", handle, h32);
    leave_handle_lock();
    return h32;
}

//handle16 <- wow64 handle32
HANDLE16 WINAPI K32WOWHandle16User(HANDLE handle, WOW_HANDLE_TYPE type)
{
    HANDLE16 h16;
    if (map_low_word_user_handle)
    {
        return (HANDLE16)LOWORD(handle);
    }
    enter_handle_lock();
    h16 = get_handle16(handle, &handle_list[HANDLE_TYPE_HANDLE], type);
    if (handle_trace)
        DPRINTF("HANDLE3216 %p %04X\n", handle, h16);
    leave_handle_lock();
    return h16;
}

HANDLE16 WINAPI K32WOWHandle16Other(HANDLE handle, WOW_HANDLE_TYPE type)
{
    enter_handle_lock();
    HANDLE16 h16 = get_handle16(handle, &handle_list[HANDLE_TYPE_HANDLE], type);
    if (handle_trace)
        DPRINTF("HANDLE3216 %p %04X\n", handle, h16);
    leave_handle_lock();
    return h16;
}

//handle16 -> wow64 handle32
HANDLE WINAPI K32WOWHandle32HGDI(WORD handle)
{
    enter_handle_lock();
    HANDLE h32 = get_handle32(handle, &handle_list[HANDLE_TYPE_HGDI]);
    if (handle_trace)
        DPRINTF("HGDI1632 %04X %p\n", handle, h32);
    leave_handle_lock();
    return h32;
}
//handle16 <- wow64 handle32
HANDLE16 WINAPI K32WOWHandle16HGDI(HANDLE handle, WOW_HANDLE_TYPE type)
{
    enter_handle_lock();
    HANDLE16 h16 = get_handle16(handle, &handle_list[HANDLE_TYPE_HGDI], type);
    if (handle_trace)
        DPRINTF("HGDI3216 %p %04X\n", handle, h16);
    leave_handle_lock();
    return h16;
}
static BOOL is_gdiobj(WOW_HANDLE_TYPE type)
{
    return type == WOW_TYPE_HDC || type == WOW_TYPE_HBITMAP || type == WOW_TYPE_HBRUSH ||
        type == WOW_TYPE_HFONT || type == WOW_TYPE_HMETAFILE || type == WOW_TYPE_HPALETTE ||
        type == WOW_TYPE_HPEN || type == WOW_TYPE_HRGN;
}
void WINAPI K32WOWHandle16Destroy(HANDLE handle, WOW_HANDLE_TYPE type)
{
    if (is_gdiobj(type))
    {
        HGDIOBJ16 h16 = K32WOWHandle16HGDI(handle, type);
        if (handle_trace)
            DPRINTF("destroy HGDI %p %04X\n", handle, h16);
        destroy_handle16(&handle_list[HANDLE_TYPE_HGDI], h16);
        return;
    }
    else if (type == WOW_TYPE_HWND || type == WOW_TYPE_HMENU || type == WOW_TYPE_HDWP || type == WOW_TYPE_HDROP || type == WOW_TYPE_HACCEL)
    {
        if (!map_low_word_user_handle)
        {
            HANDLE16 h16 = K32WOWHandle16User(handle, type);
            if (handle_trace)
                DPRINTF("destroy User HANDLE %p %04X\n", handle, h16);
            destroy_handle16(&handle_list[HANDLE_TYPE_HANDLE], h16);
        }
    }
    else
    {
        HANDLE16 h16 = K32WOWHandle16Other(handle, type);
        if (handle_trace)
            DPRINTF("destroy HANDLE %p %04X\n", handle, h16);
        destroy_handle16(&handle_list[HANDLE_TYPE_HANDLE], h16);
    }
}
void WINAPI K32WOWHandle16DestroyHint(HANDLE handle, WOW_HANDLE_TYPE type)
{
    if (type == WOW_TYPE_HWND)
    {
        if (IsWindow(handle))
            return;
        K32WOWHandle16Destroy(handle, type);
    }
    if (type == WOW_TYPE_HMENU)
    {
        if (IsMenu(handle))
            return;
        K32WOWHandle16Destroy(handle, type);
    }
    if (is_gdiobj(type))
    {
        if (GetObjectType(handle))
            return;
        K32WOWHandle16Destroy(handle, type);
    }
}

static void hgdi_clean_up(HANDLE_STORAGE* hs)
{
    for (int i = HANDLE_RESERVED; i < (WORD)(-HANDLE_RESERVED); i += hs->align)
    {
        if (handle_trace)
        {
            static const char *tbl[] =
            {
                "(deleted)",
                "OBJ_PEN",
                "OBJ_BRUSH",
                "OBJ_DC",
                "OBJ_METADC",
                "OBJ_PAL",
                "OBJ_FONT",
                "OBJ_BITMAP",
                "OBJ_REGION",
                "OBJ_METAFILE",
                "OBJ_MEMDC",
                "OBJ_EXTPEN",
                "OBJ_ENHMETADC",
                "OBJ_ENHMETAFILE",
                "OBJ_COLORSPACE"
            };
            DPRINTF("%p: %s\n", hs->handles[i].handle32, tbl[GetObjectType(hs->handles[i].handle32)]);
        }
        K32WOWHandle16DestroyHint(hs->handles[i].handle32, WOW_TYPE_HDC);
    }
}

static void user_handle_clean_up(HANDLE_STORAGE* hs)
{
    for (int i = HANDLE_RESERVED; i < (WORD)(-HANDLE_RESERVED); i += hs->align)
    {
        K32WOWHandle16DestroyHint(hs->handles[i].handle32, hs->handles[i].type);
    }
}

__declspec(dllexport) void SetWindowHInst16(WORD hWnd16, HINSTANCE16 hinst16)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
    {
        ERR("Invalid Window Handle SetWindowHInst16(%04X,%04X)\n", hWnd16, hinst16);
        return;
    }
    dat->hInst16 = hinst16;
}
__declspec(dllexport) HINSTANCE16 GetWindowHInst16(WORD hWnd16)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
    {
        ERR("Invalid Window Handle GetWindowHInst16(%04X)\n", hWnd16);
        return 0;
    }
    return dat->hInst16;
}

__declspec(dllexport) void SetWindowHMenu16(WORD hWnd16, HMENU16 h)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
    {
        ERR("Invalid Window Handle SetWindowHMenu16(%04X,%04X)\n", hWnd16, h);
        return;
    }
    dat->hMenu16 = h;
}
__declspec(dllexport) HMENU16 GetWindowHMenu16(WORD hWnd16)
{
	HANDLE_DATA *dat;
	if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
	{
		ERR("Invalid Window Handle GetWindowHMenu16(%04X)\n", hWnd16);
		return 0;
	}
	if (dat->hMenu16)
	{
		return dat->hMenu16;
	}
	return WOWHandle16(GetMenu(dat->handle32), WOW_TYPE_HMENU);
}

__declspec(dllexport) void SetWndProc16(WORD hWnd16, DWORD WndProc)
{
	HANDLE_DATA *dat;
	if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
	{
		ERR("Invalid Window Handle SetWndProc16(%04X,%04X)\n", hWnd16, WndProc);
		return;
	}
	dat->wndproc = WndProc;
}
__declspec(dllexport) DWORD GetWndProc16(WORD hWnd16)
{
	HANDLE_DATA *dat;
	if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
	{
		ERR("Invalid Window Handle GetWndProc16(%04X)\n", hWnd16);
		return 0;
	}
	return dat->wndproc;
}


__declspec(dllexport) void SetDlgProc16(WORD hWnd16, DWORD WndProc)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
    {
        ERR("Invalid Window Handle SetDlgProc16(%04X,%04X)\n", hWnd16, WndProc);
        return;
    }
    dat->dlgproc = WndProc;
}
__declspec(dllexport) DWORD GetDlgProc16(WORD hWnd16)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hWnd16, &handle_list[HANDLE_TYPE_HANDLE], &dat))
    {
        ERR("Invalid Window Handle GetDlgProc16(%04X)\n", hWnd16);
        return 0;
    }
    return dat->dlgproc;
}

__declspec(dllexport) void SetPtr16(WORD hdl16, void *ptr, int type)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hdl16, &handle_list[type], &dat))
    {
        ERR("Invalid Handle SetPtr16(%04X,%04X)\n", hdl16, (DWORD)ptr);
        return;
    }
    dat->ptr = ptr;
}

__declspec(dllexport) void *GetPtr16(WORD hdl16, int type)
{
    HANDLE_DATA *dat;
    if (!get_handle32_data(hdl16, &handle_list[type], &dat))
    {
        ERR("Invalid Handle GetPtr16(%04X)\n", hdl16);
        return NULL;
    }
    return dat->ptr;
}
