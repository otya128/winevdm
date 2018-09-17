/*
 * 16-bit CTL3D and CTL3DV2 API stubs.
 *
 * Copyright (c) 2003 Dmitry Timoshkov
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

#include "wine/winbase16.h"
#include "wine/winuser16.h"
#include <WowNT32.h>

/* Ctl3dSubclassDlg */
#define CTL3D_BUTTONS 0x0001
#define CTL3D_LISTBOXES 0x0002
#define CTL3D_EDITS 0x0004
#define CTL3D_COMBOS 0x0008
#define CTL3D_STATICTEXTS 0x0010
#define CTL3D_STATICFRAMES 0x0020
#define CTL3D_ALL 0xffff

/* resource */
#define CTL3D_3DCHECK 26567

/* Ctl3dSubclassDlgEx */
#define CTL3D_NODLGWINDOW 0x00010000
/* WM_DLGBORDER result */
#define WM_DLGBORDER (WM_USER + 3567)
#define CTL3D_NOBORDER 0
#define CTL3D_BORDER 1

/* WM_DLGSUBCLASS result */
#define WM_DLGSUBCLASS (WM_USER + 3568)
#define CTL3D_NOSUBCLASS 0
#define CTL3D_SUBCLASS 1

#define CTLMSGOFFSET 3569
#define CTL3D_CTLCOLOR (WM_USER + CTLMSGOFFSET)

static BOOL16 CTL3D16_is_auto_subclass = FALSE;

static WNDPROC listbox_proc;
static WNDPROC button_proc;
static WNDPROC static_proc;
static WNDPROC combo_proc;
static WNDPROC edit_proc;
static WNDPROC dialog_proc;
LRESULT WINAPI __wine_call_wndproc(HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam, WNDPROC proc);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD reason, LPVOID lpvReserved)
{
    WNDCLASSA cls;
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        if (GetClassInfoA(hinstDLL, "LISTBOX", &cls))
            listbox_proc = cls.lpfnWndProc;
        if (GetClassInfoA(hinstDLL, "BUTTON", &cls))
            button_proc = cls.lpfnWndProc;
        if (GetClassInfoA(hinstDLL, "STATIC", &cls))
            static_proc = cls.lpfnWndProc;
        if (GetClassInfoA(hinstDLL, "COMBOBOX", &cls))
            combo_proc = cls.lpfnWndProc;
        if (GetClassInfoA(hinstDLL, "EDIT", &cls))
            edit_proc = cls.lpfnWndProc;
        if (GetClassInfoA(hinstDLL, "#32770", &cls))
            dialog_proc = cls.lpfnWndProc;
        break;
    }
    return TRUE;
}
/***********************************************************************
 *		Ctl3dAutoSubclass (CTL3DV2.16)
 */
BOOL16 WINAPI Ctl3dAutoSubclass16(HINSTANCE16 hInst)
{
    CTL3D16_is_auto_subclass = TRUE;
    return TRUE;
}

/***********************************************************************
 *		Ctl3dAutoSubclassEx (CTL3DV2.27)
 */
BOOL16 WINAPI Ctl3dAutoSubclassEx16(HINSTANCE16 hInst, DWORD type)
{
    CTL3D16_is_auto_subclass = TRUE;
    return TRUE;
}

/***********************************************************************
 *		Ctl3dColorChange (CTL3DV2.6)
 */
BOOL16 WINAPI Ctl3dColorChange16(void)
{
    return TRUE;
}

/***********************************************************************
 *		Ctl3dCtlColor (CTL3DV2.4)
 */
HBRUSH WINAPI Ctl3dCtlColor16(HDC16 hdc, LONG hwnd)
{
    return 0;
}

/***********************************************************************
 *		Ctl3dCtlColorEx (CTL3DV2.18)
 */
HBRUSH WINAPI Ctl3dCtlColorEx16(UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    return 0;
}

/***********************************************************************
 *		Ctl3dDlgFramePaint (CTL3DV2.20)
 */
LRESULT WINAPI Ctl3dDlgFramePaint16(HWND16 hwnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    return __wine_call_wndproc(hwnd, msg, wParam, lParam, DefWindowProcA);
}

/***********************************************************************
 *		Ctl3dEnabled (CTL3DV2.5)
 */
BOOL16 WINAPI Ctl3dEnabled16(void)
{
    return FALSE;
}

/***********************************************************************
 *		Ctl3dGetVer (CTL3DV2.1)
 */
WORD WINAPI Ctl3dGetVer16(void)
{
    return MAKEWORD(31,2);
}

/***********************************************************************
 *		Ctl3dIsAutoSubclass (CTL3DV2.23)
 */
BOOL16 WINAPI Ctl3dIsAutoSubclass16(void)
{
    return CTL3D16_is_auto_subclass;
}

/***********************************************************************
 *		Ctl3dRegister (CTL3DV2.12)
 */
BOOL16 WINAPI Ctl3dRegister16(HINSTANCE16 hInst)
{
    return TRUE;
}


/***********************************************************************
*		Ctl3dSubclassCtlEx (CTL3DV2.25)
*/
BOOL16 WINAPI Ctl3dSubclassCtlEx16(HWND16 hwnd, INT16 type)
{
    HWND hwnd32 = HWND_32(hwnd);
    char buf[200] = { 0 };
    GetClassNameA(hwnd32, buf, sizeof(buf));
    if ((type & CTL3D_EDITS) && (!strcmpi(buf, "EDIT")) || ((type & CTL3D_LISTBOXES) && !strcmpi(buf, "LISTBOX")))
    {
        SetWindowLongA(hwnd32, GWL_EXSTYLE, GetWindowLongA(HWND_32(hwnd), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetWindowPos(hwnd32, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
    return TRUE;
}

/***********************************************************************
 *		Ctl3dSubclassCtl (CTL3DV2.3)
 */
BOOL16 WINAPI Ctl3dSubclassCtl16(HWND16 hwnd)
{
    return Ctl3dSubclassCtlEx16(hwnd, CTL3D_ALL);
}

struct ctl3d_enumproc
{
    HWND parent;
    DWORD types;
};
BOOL CALLBACK ctl3d_enumproc(HWND hwnd, LPARAM lparam)
{
    struct ctl3d_enumproc *data = (struct ctl3d_enumproc*)lparam;
    if (GetParent(hwnd) != data->parent)
        return TRUE;
    Ctl3dSubclassCtlEx16(hwnd, data->types);
}
/***********************************************************************
 *		Ctl3dSubclassDlgEx (CTL3DV2.21)
 */
BOOL16 WINAPI Ctl3dSubclassDlgEx16(HWND16 hwnd, DWORD types)
{
    HWND hwnd32 = HWND_32(hwnd);
    struct ctl3d_enumproc data = { 0 };
    data.types = types;
    EnumChildWindows(hwnd32, ctl3d_enumproc, (LPARAM)&data);
    return FALSE;
}

/***********************************************************************
*		Ctl3dSubclassDlg (CTL3DV2.2)
*/
BOOL16 WINAPI Ctl3dSubclassDlg16(HWND16 hwnd, WORD types)
{
    return Ctl3dSubclassDlgEx16(hwnd, types);
}

/***********************************************************************
 *		Ctl3dUnAutoSubclass (CTL3DV2.24)
 */
BOOL16 WINAPI Ctl3dUnAutoSubclass16(void)
{
    CTL3D16_is_auto_subclass = FALSE;
    return FALSE;
}

/***********************************************************************
 *		Ctl3dUnregister (CTL3DV2.13)
 */
BOOL16 WINAPI Ctl3dUnregister16(HINSTANCE16 hInst)
{
    CTL3D16_is_auto_subclass = FALSE;
    return TRUE;
}

/***********************************************************************
 *		Ctl3dUnsubclassCtl (CTL3DV2.26)
 */
BOOL16 WINAPI Ctl3dUnsubclassCtl16(HWND16 hwnd)
{
    return FALSE;
}

/***********************************************************************
 *		Ctl3dWinIniChange (CTL3DV2.22)
 */
void WINAPI Ctl3dWinIniChange16(void)
{
}

/***********************************************************************
 *		ComboWndProc3d (CTL3DV2.10)
 */
LRESULT WINAPI ComboWndProc3d16(HWND16 hwnd, UINT16 msg,WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, combo_proc);
}

/***********************************************************************
 *		BtnWndProc3d (CTL3DV2.7)
 */
LRESULT WINAPI BtnWndProc3d16(HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, button_proc);
}

/***********************************************************************
 *		StaticWndProc3d (CTL3DV2.11)
 */
LRESULT WINAPI StaticWndProc3d16(HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, static_proc);
}

/***********************************************************************
 *		EditWndProc3d (CTL3DV2.8)
 */
LRESULT WINAPI EditWndProc3d16(HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, edit_proc);
}

/***********************************************************************
 *		ListWndProc3d (CTL3DV2.9)
 */
LRESULT WINAPI ListWndProc3d16(HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, listbox_proc);
}

/***********************************************************************
 *		Ctl3dDlgProc (CTL3DV2.17)
 */
LRESULT WINAPI Ctl3dDlgProc16(HWND16 hwnd, UINT16 msg, WPARAM16 wparam, LPARAM lparam)
{
    return __wine_call_wndproc(hwnd, msg, wparam, lparam, dialog_proc);
}

WORD WINAPI Ctl3dSetStyle16(WORD a1, SEGPTR a2, WORD a3)
{
    return 0;
}
