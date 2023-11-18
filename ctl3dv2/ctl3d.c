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

struct autosubclass
{
    HHOOK hook;
    DWORD type;
};

static DWORD autosubclass_index = NULL;

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
        autosubclass_index = TlsAlloc();
        break;
    case DLL_PROCESS_DETACH:
        TlsFree(autosubclass_index);
        break;
    case DLL_THREAD_DETACH:
    {
        struct autosubclass *asc = TlsGetValue(autosubclass_index);
        if (asc)
        {
            UnhookWindowsHookEx(asc->hook);
            HeapFree(GetProcessHeap(), 0, asc);
        }
        break;
    }
    }
    return TRUE;
}

static void ctl3d_static(DWORD type, DWORD *style, DWORD *exstyle)
{
    switch (*style & 0xf)
    {
        case SS_BLACKFRAME:
            if (type & CTL3D_STATICFRAMES)
            {
                *style &= ~SS_BLACKFRAME;
                *exstyle |= WS_EX_CLIENTEDGE;
            }
            break;
        case SS_GRAYFRAME:
            if (type & CTL3D_STATICFRAMES)
            {
                *style &= ~SS_GRAYFRAME;
                *style |= SS_ETCHEDFRAME;
            }
            break;
        case SS_WHITEFRAME:
            if (type & CTL3D_STATICFRAMES)
            {
                *style &= ~SS_WHITEFRAME;
                *exstyle |= WS_EX_DLGMODALFRAME;
            }
            break;
        case SS_BLACKRECT:
            if (type & CTL3D_STATICTEXTS)
            {
                *style &= ~SS_BLACKRECT;
                *exstyle |= WS_EX_CLIENTEDGE;
            }
            break;
        case SS_GRAYRECT:
            if (type & CTL3D_STATICTEXTS)
            {
                *style &= ~SS_GRAYRECT;
                *style |= SS_ETCHEDFRAME;
            }
            break;
        case SS_WHITERECT:
            if (type & CTL3D_STATICTEXTS)
            {
                *style &= ~SS_WHITERECT;
                *exstyle |= WS_EX_DLGMODALFRAME;
            }
            break;
    }
}                              

static LRESULT CALLBACK subclassproc(INT code, WPARAM wp, LPARAM lp)
{
    struct autosubclass *asc = TlsGetValue(autosubclass_index);
    if (code == HCBT_CREATEWND)
    {
        CBT_CREATEWNDA *cbt_cw = (CBT_CREATEWNDA *)lp;
        char *cls = cbt_cw->lpcs->lpszClass;
        if ((DWORD)cls & 0xffff0000)
        {
            if ((asc->type & CTL3D_EDITS) && (!strcmpi(cls, "EDIT")) || ((asc->type & CTL3D_LISTBOXES) && !strcmpi(cls, "LISTBOX")))
            {
                DWORD exstyle = cbt_cw->lpcs->dwExStyle | WS_EX_CLIENTEDGE;
                SetWindowLongA(wp, GWL_EXSTYLE, exstyle);
            }
            if (!strcmpi(cls, "STATIC"))
            {
                DWORD style = cbt_cw->lpcs->style;
                DWORD exstyle = cbt_cw->lpcs->dwExStyle;
                ctl3d_static(asc->type, &style, &exstyle);
                cbt_cw->lpcs->style = style;
                cbt_cw->lpcs->dwExStyle = exstyle;
                SetWindowLongA(wp, GWL_STYLE, style);
                SetWindowLongA(wp, GWL_EXSTYLE, exstyle);
            }
        }
    }
    return CallNextHookEx(asc->hook, code, wp, lp);
}

/***********************************************************************
 *		Ctl3dAutoSubclassEx (CTL3DV2.27)
 */
BOOL16 WINAPI Ctl3dAutoSubclassEx16(HINSTANCE16 hInst, DWORD type)
{
    struct autosubclass *asc = TlsGetValue(autosubclass_index);
    if (!asc)
    {
        asc = (struct autosubclass *)HeapAlloc(GetProcessHeap(), 0, sizeof(struct autosubclass));
        asc->hook = SetWindowsHookExA(WH_CBT, subclassproc, NULL, GetCurrentThreadId());
        TlsSetValue(autosubclass_index, asc);
    }
    asc->type = type;
    return TRUE;
}

/***********************************************************************
 *		Ctl3dAutoSubclass (CTL3DV2.16)
 */
BOOL16 WINAPI Ctl3dAutoSubclass16(HINSTANCE16 hInst)
{
    Ctl3dAutoSubclassEx16(hInst, CTL3D_ALL);
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
    return TlsGetValue(autosubclass_index) ? TRUE : FALSE;
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
        SetWindowLongA(hwnd32, GWL_EXSTYLE, GetWindowLongA(hwnd32, GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
        SetWindowPos(hwnd32, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
    else if (!strcmpi(buf, "STATIC"))
    {
        DWORD style = GetWindowLongA(hwnd32, GWL_STYLE);
        DWORD exstyle = GetWindowLongA(hwnd32, GWL_EXSTYLE);
        ctl3d_static(type, &style, &exstyle);
        SetWindowLongA(hwnd32, GWL_STYLE, style);
        SetWindowLongA(hwnd32, GWL_EXSTYLE, exstyle);
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
    Ctl3dSubclassCtlEx16(HWND_16(hwnd), data->types);
}
/***********************************************************************
 *		Ctl3dSubclassDlgEx (CTL3DV2.21)
 */
BOOL16 WINAPI Ctl3dSubclassDlgEx16(HWND16 hwnd, DWORD types)
{
    struct ctl3d_enumproc data = { 0 };
    data.parent = HWND_32(hwnd);
    data.types = types;
    EnumChildWindows(data.parent, ctl3d_enumproc, (LPARAM)&data);
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
    struct autosubclass *asc = TlsGetValue(autosubclass_index);
    if (asc)
    {
        UnhookWindowsHookEx(asc->hook);
        HeapFree(GetProcessHeap(), 0, asc);
        TlsSetValue(autosubclass_index, 0);
    }
    return FALSE;
}

/***********************************************************************
 *		Ctl3dUnregister (CTL3DV2.13)
 */
BOOL16 WINAPI Ctl3dUnregister16(HINSTANCE16 hInst)
{
    Ctl3dUnAutoSubclass16();
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
