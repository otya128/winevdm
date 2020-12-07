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
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "mmsystem.h"
#include "wine/wingdi16.h"
#include "windows/dispdib.h"
#include "wine/debug.h"
#include "../krnl386/dosexe.h"

WINE_DEFAULT_DEBUG_CHANNEL(ddraw);

#define width 320

static HTASK16 owner = 0;
static HDC dddc;
static int height;
static HWND ddhwnd;
static INTPROC oldproc;
static OUTPROC oldout[0x20];
static INPROC oldin[0x20];
static HANDLE running = 0;
static BOOL vsync = FALSE;
static BOOL hsync = FALSE;
static LPVOID vram;

static LRESULT CALLBACK ddwndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND parhwnd = (HWND)GetWindowLongA(hwnd, GWL_HWNDPARENT);
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
        case WM_CLOSE:
            return CallWindowProcA((WNDPROC)GetWindowLongA(parhwnd, GWL_WNDPROC), parhwnd, uMsg, wParam, lParam);
    }
    if (uMsg >= MM_JOY1MOVE)
        return CallWindowProcA((WNDPROC)GetWindowLongA(parhwnd, GWL_WNDPROC), parhwnd, uMsg, wParam, lParam);
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

static void CALLBACK retrace_cb(LPVOID arg, DWORD low, DWORD high)
{
    vsync = TRUE;
    if (WaitForSingleObject(running, 0))
    {
        SetEvent(running);
        ExitThread(0);
    }
    RECT ddrect;
    GetClientRect(ddhwnd, &ddrect);
    HDC dc = GetDC(ddhwnd);
    SetBitmapBits(GetCurrentObject(dddc, OBJ_BITMAP), width * height, vram);
    StretchBlt(dc, 0, 0, ddrect.right, ddrect.bottom, dddc, 0, 0, width, height, SRCCOPY);
    ReleaseDC(ddhwnd, dc);
}

static DWORD CALLBACK retrace_th(LPVOID arg)
{
    LARGE_INTEGER when;
    HANDLE timer;

    if (!(timer = CreateWaitableTimerA( NULL, FALSE, NULL ))) return 0;

    when.u.LowPart = when.u.HighPart = 0;
    SetWaitableTimer(timer, &when, 17, retrace_cb, arg, FALSE);
    for (;;) SleepEx(INFINITE, TRUE);
}

static void start_retrace_timer()
{
    if (running) return;
    if (height == 240) FIXME("240 px height doesn't work properly with direct fb access\n");
    running = CreateEventA(NULL, TRUE, TRUE, NULL);
    vram = MapSL((DWORD)GetProcAddress16(GetModuleHandle16("KERNEL"), (LPCSTR)174) << 16);
    CloseHandle(CreateThread(NULL, 0, retrace_th, NULL, 0, NULL));
}

static void WINAPI ddInt10Handler(CONTEXT *context)
{
    if (GetCurrentTask() != owner)
    {
        oldproc(context);
        return;
    }

    switch (AH_reg(context))
    {
        case 0x00:
            start_retrace_timer();
            switch (AL_reg(context))
            {
                case 0x13:
                    height = 200;
                    break;
                default:
                    FIXME("Vid mode %#x not supported\n", AL_reg(context));
                    break;
             }
             break;
        default:
            FIXME("Int 10 func: %#x unimplemented\n", AH_reg(context));
            break;
    }
}

static DWORD WINAPI ddVGAinHandler(int port, int size)
{
    if (GetCurrentTask() != owner)
        return oldin[port - 0x3c0] ? oldin[port - 0x3c0](port, size) : 0;

    DWORD ret = -1;

    switch (port)
    {
        case 0x3da:
        {
            start_retrace_timer();
            ret = vsync ? 9 : 0;
            vsync = FALSE;
            hsync = !hsync;
            if (!ret) ret = hsync ? 1 : 0;
            break;
        }
        default:
            FIXME("vga port %#x unimplemented\n", port);
            break;
    }
    return ret;
}

static void WINAPI ddVGAoutHandler(int port, int size, DWORD value)
{
    if (GetCurrentTask() != owner)
    {
        if (oldout[port - 0x3c0])
            oldout[port - 0x3c0](port, size, value);
        return;
    }

    if ((port & ~3) != 0x3c8) start_retrace_timer();

    static BYTE dacidx;
    static BYTE dacclr = 0;

    switch (port)
    {
        case 0x3c8:
            dacidx = value & 0xff;
            dacclr = 0;
            if (size == 1) break;
            value >>= 8;
        case 0x3c9:
        {
            RGBQUAD color;
            GetDIBColorTable(dddc, dacidx, 1, &color);
            switch (dacclr++)
            {
                case 0:
                    color.rgbRed = (BYTE)value << 2;
                    break;
                case 1:
                    color.rgbGreen = (BYTE)value << 2;
                    break;
                case 2:
                    color.rgbBlue = (BYTE)value << 2;
                    dacclr = 0;
                    break;
            }
            SetDIBColorTable(dddc, dacidx, 1, &color);
            if (!dacclr) dacidx++;
            break;
        }
        default:
            FIXME("vga port %#x unimplemented\n", port);
            break;
    }
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
    HTASK16 task = GetCurrentTask();
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
            ddwc.style = CS_HREDRAW | CS_VREDRAW;
            ddwc.lpfnWndProc = ddwndproc;
            ddwc.hInstance = GetModuleHandleA(NULL);
            ddwc.lpszClassName = "DispDibClass";
            if (!RegisterClassA(&ddwc))
                return DISPLAYDIB_NOTSUPPORTED;
        }

        owner = task;
        dddc = CreateCompatibleDC(0);
        BITMAPINFO *bmap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256*4 + sizeof(BITMAPINFOHEADER));
        VOID *section;
        bmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmap->bmiHeader.biWidth = width;
        bmap->bmiHeader.biHeight = height;
        bmap->bmiHeader.biPlanes = 1;
        bmap->bmiHeader.biBitCount = 8;
        HBITMAP ddbmap = CreateDIBSection(dddc, bmap, DIB_RGB_COLORS, &section, NULL, 0);
        HeapFree(GetProcessHeap(), 0, bmap);
        SelectObject(dddc, ddbmap);

        char title[32] = "\0";
        HWND parhwnd = GetActiveWindow();
        GetWindowTextA(parhwnd, title, 32);
        if (title[0] == '\0') GetModuleName16(GetCurrentTask(), title, 32);
        ddhwnd = CreateWindowExA(0, "DispDibClass", title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parhwnd, NULL, GetModuleHandleA(NULL), NULL);
        if (!ddhwnd)
        {
            owner = 0;
            return DISPLAYDIB_NOTSUPPORTED;
        }
        oldproc = DOSVM_SetBuiltinVector(0x10, ddInt10Handler);
        for (int i = 0; i < 0x20; i++)
            DOSVM_setportcb(ddVGAoutHandler, ddVGAinHandler, i + 0x3c0, &oldout[i], &oldin[i]);
        return DISPLAYDIB_NOERROR;
    }
    else if ((owner != task) || !owner)
        return DISPLAYDIB_INVALIDTASK;
    else if (wFlags & DISPLAYDIB_END)
    {
        DestroyWindow(ddhwnd);
        DeleteDC(dddc);
        DOSVM_SetBuiltinVector(0x10, oldproc);
        for (int i = 0; i < 0x20; i++)
            DOSVM_setportcb(oldout[i], oldin[i], i + 0x3c0, &oldout[i], &oldin[i]);
        if (running)
        {
            ResetEvent(running);
            WaitForSingleObject(running, INFINITE);
            CloseHandle(running);
            running = 0;
        }
        owner = 0;
        return DISPLAYDIB_NOERROR;
    }
    if (!(wFlags & DISPLAYDIB_NOPALETTE))
    {
        if (!lpbi)
            return DISPLAYDIB_INVALIDDIB;
        SetDIBColorTable(dddc, 0, lpbi->bmiHeader.biClrUsed, (RGBQUAD *)&lpbi->bmiColors); 
    }
    if(!(wFlags & /*DISPLAYDIB_NOIMAGE*/ 0x80))
    {
        if (!lpBits)
            return DISPLAYDIB_INVALIDDIB;
        RECT ddrect;
        GetClientRect(ddhwnd, &ddrect);
        HDC dc = GetDC(ddhwnd);
        SetDIBitsToDevice(dddc, 0, 0, width, height, 0, 0, 0, height, lpBits, lpbi, DIB_RGB_COLORS);
        StretchBlt(dc, 0, 0, ddrect.right, ddrect.bottom, dddc, 0, 0, width, height, SRCCOPY);
        ReleaseDC(ddhwnd, dc);
    }
    return DISPLAYDIB_NOERROR;
}
