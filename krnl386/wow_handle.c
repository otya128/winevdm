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
} HANDLE_DATA;
typedef struct
{
    HANDLE_DATA handles[65536];
    int align;
} HANDLE_STORAGE;
#define HANDLE_TYPE_HANDLE 0
#define HANDLE_TYPE_HGDI 1
#define HANDLE_TYPE_MAX 2
static HANDLE_STORAGE *handle_list;
static BOOL handle_trace;
static BOOL handle_init_flag;
/* this function called by DllMain(kernel.c) */
void init_wow_handle()
{
    if (handle_init_flag)
        return;
    handle_trace = TRACE_ON(thunk);
    handle_list = (HANDLE_STORAGE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, HANDLE_TYPE_MAX * sizeof(HANDLE_STORAGE));
    handle_init_flag = TRUE;
    handle_list[HANDLE_TYPE_HANDLE].align = 1;
    /*
    hdc1 = CreateCompatibleDC(0);
    hdc2 = CreateCompatibleDC(0);
    hdc3 = CreateCompatibleDC(0);
    hdc4 = CreateCompatibleDC(0);

    hdc1: 0x6c, hdc2: 0x70, hdc3: 0x74, hdc4: 0x78...
    PBRUSH expects HDC to be a multiple of 4.
    */
    handle_list[HANDLE_TYPE_HGDI].align = 4;
}
WORD get_handle16_data(HANDLE h, HANDLE_STORAGE *hs, HANDLE_DATA **o);
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
WORD get_handle16(HANDLE h, HANDLE_STORAGE *hs)
{
	if (is_reserved_handle32(h))
	{
		return h;
	}
	HANDLE_DATA *hd;
	int hnd16 = get_handle16_data(h, hs, &hd);
	hd->handle32 = h;
	return hnd16;
}
WORD get_handle16_data(HANDLE h, HANDLE_STORAGE *hs, HANDLE_DATA **o)
{
	//?
	if (is_reserved_handle32(h))
	{
		*o = &hs->handles[(size_t)h];
		return h;
	}
	WORD fhandle = 0;
	for (WORD i = HANDLE_RESERVED; i; i += hs->align)
	{
		if (!hs->handles[i].handle32 && !fhandle)
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
		ERR("Could not allocate a handle.\n");
	}
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
    memset(hs->handles + h, 0, sizeof(*hs->handles));
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
		return (UINT16)h;
	}
	return hs->handles[h].handle32 ? hs->handles[h].handle32 : h;
}
//handle16 -> wow64 handle32
HANDLE WINAPI K32WOWHandle32HWND(WORD handle)
{
	HANDLE h32 = get_handle32(handle, &handle_list[HANDLE_TYPE_HANDLE]);
    if (handle_trace)
        DPRINTF("HANDLE1632 %04X %p\n", handle, h32);
	return h32;
}
//handle16 <- wow64 handle32
HANDLE16 WINAPI K32WOWHandle16HWND(HANDLE handle)
{
    HANDLE16 h16 = get_handle16(handle, &handle_list[HANDLE_TYPE_HANDLE]);
    if (handle_trace)
        DPRINTF("HANDLE3216 %p %04X\n", handle, h16);
	return h16;
}

//handle16 -> wow64 handle32
HANDLE WINAPI K32WOWHandle32HGDI(WORD handle)
{
    HANDLE h32 = get_handle32(handle, &handle_list[HANDLE_TYPE_HGDI]);
    if (handle_trace)
        DPRINTF("HGDI1632 %04X %p\n", handle, h32);
    return h32;
}
//handle16 <- wow64 handle32
HANDLE16 WINAPI K32WOWHandle16HGDI(HANDLE handle)
{
    HANDLE16 h16 = get_handle16(handle, &handle_list[HANDLE_TYPE_HGDI]);
    if (handle_trace)
        DPRINTF("HGDI3216 %p %04X\n", handle, h16);
    return h16;
}
static BOOL is_gdiobj(WOW_HANDLE_TYPE type)
{
    return type == WOW_TYPE_HDC || type == WOW_TYPE_HBITMAP || type == WOW_TYPE_HBRUSH ||
        type == WOW_TYPE_HFONT || type == WOW_TYPE_HMETAFILE || type == WOW_TYPE_HPALETTE ||
        type == WOW_TYPE_HPEN || type == WOW_TYPE_HRGN;
}
__declspec(dllexport) void WINAPI K32WOWHandle16Destroy(HANDLE handle, WOW_HANDLE_TYPE type)
{
    if (is_gdiobj(type))
    {
        destroy_handle16(&handle_list[HANDLE_TYPE_HGDI], K32WOWHandle16HGDI(handle));
        return;
    }
    destroy_handle16(&handle_list[HANDLE_TYPE_HANDLE], K32WOWHandle16HWND(handle));
}
__declspec(dllexport) void WINAPI K32WOWHandle16DestroyHint(HANDLE handle, WOW_HANDLE_TYPE type)
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
