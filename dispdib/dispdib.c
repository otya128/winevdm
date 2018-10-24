/*
 * DISPDIB.dll
 *
 * Copyright 1998 Ove Kåven (with some help from Marcus Meissner)
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
#include <string.h>
#include <stdlib.h>
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "mmsystem.h"
#include "wine/wingdi16.h"
#include "windows/dispdib.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ddraw);

static HTASK owner = 0;
static HDC dddc;
static int height;
static HWND ddhwnd;

LRESULT CALLBACK ddwndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND parhwnd = GetWindowLongA(hwnd, GWL_HWNDPARENT);
    switch (uMsg)
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_DEADCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSCHAR:
        case WM_SYSDEADCHAR:
        case WM_UNICHAR:
        case WM_TIMER:
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            return CallWindowProcA(GetWindowLongA(parhwnd, GWL_WNDPROC), parhwnd, uMsg, wParam, lParam);
    }
    if (uMsg >= MM_JOY1MOVE)
        return CallWindowProcA(GetWindowLongA(parhwnd, GWL_WNDPROC), parhwnd, uMsg, wParam, lParam);
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

/*********************************************************************
 *	DisplayDib	(DISPDIB.1)
 *
 *  Disables GDI and takes over the VGA screen to show DIBs in full screen.
 *
 * FLAGS
 *
 *  DISPLAYDIB_NOPALETTE: don't change palette
 *  DISPLAYDIB_NOCENTER: don't center bitmap
 *  DISPLAYDIB_NOWAIT: don't wait (for keypress) before returning
 *  DISPLAYDIB_BEGIN: start of multiple calls (does not restore the screen)
 *  DISPLAYDIB_END: end of multiple calls (restores the screen)
 *  DISPLAYDIB_MODE_DEFAULT: default display mode
 *  DISPLAYDIB_MODE_320x200x8: Standard VGA 320x200 256 colors
 *  DISPLAYDIB_MODE_320x240x8: Tweaked VGA 320x240 256 colors
 *
 * RETURNS
 *
 *  DISPLAYDIB_NOERROR: success
 *  DISPLAYDIB_NOTSUPPORTED: function not supported
 *  DISPLAYDIB_INVALIDDIB: null or invalid DIB header
 *  DISPLAYDIB_INVALIDFORMAT: invalid DIB format
 *  DISPLAYDIB_INVALIDTASK: not called from current task
 *
 * BUGS
 *
 *  Waiting for keypresses is not implemented.
 */
WORD WINAPI DisplayDib(
		LPBITMAPINFO lpbi, /* [in] DIB header with resolution and palette */
		LPSTR lpBits,      /* [in] Bitmap bits to show */
		WORD wFlags        /* [in] */
	)
{
    HTASK task = GetCurrentTask();
    if ((wFlags & DISPLAYDIB_BEGIN) && !owner)
    {
        switch (wFlags & DISPLAYDIB_MODE)
        {
                default:
                    return DISPLAYDIB_NOTSUPPORTED;
                case DISPLAYDIB_MODE_DEFAULT:
                case DISPLAYDIB_MODE_320x200x8:
                    height = 200;
                    break;
                case DISPLAYDIB_MODE_320x240x8:
                    height = 240;
                    break;
        }
        WNDCLASSA wc;
        if (!GetClassInfoA(GetModuleHandleA(NULL), "DispDibClass", &wc))
        {
            WNDCLASSA ddwc = {0};
            ddwc.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
            ddwc.lpfnWndProc = ddwndproc;
            ddwc.hInstance = GetModuleHandleA(NULL);
            ddwc.lpszClassName = "DispDibClass";
            if (!RegisterClassA(&ddwc))
                return DISPLAYDIB_NOTSUPPORTED;
        }

        dddc = CreateCompatibleDC(0);
        BITMAPINFO *bmap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256*4 + sizeof(BITMAPINFOHEADER));
        VOID *section;
        bmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmap->bmiHeader.biWidth = 320;
        bmap->bmiHeader.biHeight = height;
        bmap->bmiHeader.biPlanes = 1;
        bmap->bmiHeader.biBitCount = 8;
        HBITMAP ddbmap = CreateDIBSection(dddc, bmap, DIB_RGB_COLORS, &section, NULL, 0);
        HeapFree(GetProcessHeap(), 0, bmap);
        SelectObject(dddc, ddbmap);

        char title[32] = "DispDib";
        HWND parhwnd = GetActiveWindow();
        GetWindowTextA(parhwnd, title, 32);
        ddhwnd = CreateWindowExA(0, "DispDibClass", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parhwnd, NULL, GetModuleHandleA(NULL), NULL);
        if (!ddhwnd)
            return DISPLAYDIB_NOTSUPPORTED;
        owner = task;
        return DISPLAYDIB_NOERROR;
    }
    else if ((owner != task) || !owner)
        return DISPLAYDIB_INVALIDTASK;
    else if (wFlags & DISPLAYDIB_END)
    {
        DestroyWindow(ddhwnd);
        DeleteDC(dddc);
        owner = 0;
        return DISPLAYDIB_NOERROR;
    }
    if (!(wFlags & DISPLAYDIB_NOPALETTE))
    {
        if (!lpbi)
            return DISPLAYDIB_INVALIDDIB;
        SetDIBColorTable(dddc, 0, lpbi->bmiHeader.biClrUsed, &lpbi->bmiColors); 
    }
    if(!(wFlags & /*DISPLAYDIB_NOIMAGE*/ 0x80))
    {
        if (!lpBits)
            return DISPLAYDIB_INVALIDDIB;
        RECT ddrect;
        GetClientRect(ddhwnd, &ddrect);
        HDC dc = GetDC(ddhwnd);
        SetDIBitsToDevice(dddc, 0, 0, 320, height, 0, 0, 0, 320, lpBits, lpbi, DIB_RGB_COLORS);
        StretchBlt(dc, 0, 0, ddrect.right, ddrect.bottom, dddc, 0, 0, 320, height, SRCCOPY);
        ReleaseDC(ddhwnd, dc);
    }
    return DISPLAYDIB_NOERROR;
}
