/*
 * COMMDLG - Print Dialog
 *
 * Copyright 1994 Martin Ayotte
 * Copyright 1996 Albrecht Kleine
 * Copyright 1999 Klaas van Gend
 * Copyright 2000 Huw D M Davies
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

#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "wine/wingdi16.h"
#include "winuser.h"
#include "wine/winuser16.h"
#include "commdlg.h"
#include "dlgs.h"
#include "wine/debug.h"
#include "cderr.h"
#include "winspool.h"
#include "cdlg16.h"

WINE_DEFAULT_DEBUG_CHANNEL(commdlg);
/* Maximum length of a device name */
#define CCHDEVICENAME16 32

/* Maximum length of a paper name */
#define CCHPAPERNAME16  64
#pragma pack(push, 1)
typedef struct {
    char  dmDeviceName[CCHDEVICENAME16];
    UINT16  dmSpecVersion;
    UINT16  dmDriverVersion;
    UINT16  dmSize;
    UINT16  dmDriverExtra;
    DWORD dmFields;
    INT16   dmOrientation;
    INT16   dmPaperSize;
    INT16   dmPaperLength;
    INT16   dmPaperWidth;
    INT16   dmScale;
    INT16   dmCopies;
    INT16   dmDefaultSource;
    INT16   dmPrintQuality;
    INT16   dmColor;
    INT16   dmDuplex;
    INT16   dmYResolution;
    INT16   dmTTOption;
} DEVMODE16, *LPDEVMODE16;
#pragma pack(pop)
//sizeof(DEVMODE16) != sizeof(DEVMODEA)
void DEVMODE16To32(CONST DEVMODE16 *src, LPDEVMODEA dst)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME - 1] = 0;
    dst->dmSpecVersion = 0x30a;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = sizeof(DEVMODE16);
    dst->dmDriverExtra = 0;
    dst->dmFields = src->dmFields;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
}
void DEVMODE32To16(LPDEVMODE16 dst, const LPDEVMODEA src)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME16 - 1] = 0;
    dst->dmSpecVersion = 0x30a;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = sizeof(DEVMODE16);
    dst->dmDriverExtra = 0;
    dst->dmFields = src->dmFields;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
}

static void global_handle_to_16( HGLOBAL16 *h16, HGLOBAL handle )
{
    DWORD size;
    HGLOBAL16 ret;

    if (!handle) return;
    size = GlobalSize( handle );
    if (*h16) ret = GlobalReAlloc16( *h16, size, GMEM_MOVEABLE );
    else ret = GlobalAlloc16( GMEM_MOVEABLE, size );
    if (ret)
    {
        void *src = GlobalLock( handle );
        void *dst = GlobalLock16( ret );
        memcpy( dst, src, size );
        GlobalUnlock( handle );
        GlobalUnlock16( ret );
        *h16 = ret;
    }
}

static HGLOBAL global_handle_from_16( HGLOBAL16 handle )
{
    DWORD size;
    HGLOBAL ret;

    if (!handle) return 0;
    size = GlobalSize16( handle );
    if ((ret = GlobalAlloc( GMEM_MOVEABLE, size )))
    {
        void *src = GlobalLock16( handle );
        void *dst = GlobalLock( ret );
        memcpy( dst, src, size );
        GlobalUnlock16( handle );
        GlobalUnlock( ret );
    }
    return ret;
}

LPDLGTEMPLATEA resource_to_dialog32(HINSTANCE16 hInst, LPCSTR name)
{
    HRSRC16 res = FindResource16(hInst, name, (LPCSTR)RT_DIALOG);
    HGLOBAL16 handle = LoadResource16(hInst, res);
    DWORD size = SizeofResource16(hInst, res);
    SEGPTR ptr16 = WOWGlobalLock16(handle);
    DWORD size2;

    LPDLGTEMPLATEA r = dialog_template16_to_template32(hInst, ptr16, &size2, NULL);
    FreeResource16(handle);
    return r;
}

LPDLGTEMPLATEA handle_to_dialog32(HGLOBAL16 hg)
{
    DWORD size2;
    SEGPTR ptr16 = WOWGlobalLock16(hg);
    LPDLGTEMPLATEA r = dialog_template16_to_template32(NULL, ptr16, &size2, NULL);
    WOWGlobalUnlock16(hg);
    return r;
}

/**********************************************************************
 *
 *      16 bit commdlg
 */

/***********************************************************************
 *           PrintDlg   (COMMDLG.20)
 *
 *  Displays the PRINT dialog box, which enables the user to specify
 *  specific properties of the print job.
 *
 * RETURNS
 *  nonzero if the user pressed the OK button
 *  zero    if the user cancelled the window or an error occurred
 *
 * BUGS
 *  * calls up to the 32-bit versions of the Dialogs, which look different
 *  * Customizing is *not* implemented.
 */

BOOL16 WINAPI PrintDlg16( SEGPTR pd )
{
    LPPRINTDLG16 lppd = MapSL(pd);
    PRINTDLGA pd32;
    BOOL ret;
    HINSTANCE16 hInst;
    LPDLGTEMPLATEA template_setup = NULL;
    LPDLGTEMPLATEA template_print = NULL;
    HGLOBAL *hdma = NULL;
    if (lppd->hDevMode)
    {
        hdma = GlobalAlloc(GMEM_MOVEABLE, sizeof(DEVMODEA));
        DEVMODE16To32(GlobalLock16(lppd->hDevMode), GlobalLock(hdma));
        GlobalUnlock(hdma);
        GlobalUnlock16(lppd->hDevMode);
    }

    if (!lppd) return PrintDlgA(NULL); /* generate failure with CDERR_INITIALIZATION */

    pd32.lStructSize = sizeof(pd32);
    pd32.Flags       = lppd->Flags & ~(PD_ENABLEPRINTTEMPLATE | PD_ENABLEPRINTTEMPLATEHANDLE |
                                       PD_ENABLESETUPTEMPLATE | PD_ENABLESETUPTEMPLATEHANDLE |
                                       PD_ENABLEPRINTHOOK | PD_ENABLESETUPHOOK);
    pd32.hwndOwner   = HWND_32(lppd->hwndOwner);
    pd32.hDevMode    = hdma; 
    pd32.hDevNames   = global_handle_from_16( lppd->hDevNames );
    pd32.nFromPage   = lppd->nFromPage;
    pd32.nToPage     = lppd->nToPage;
    pd32.nMinPage    = lppd->nMinPage;
    pd32.nMaxPage    = lppd->nMaxPage;
    pd32.nCopies     = lppd->nCopies;
    pd32.lpfnPrintHook = NULL;
    pd32.lpfnSetupHook = NULL;
    hInst = lppd->hInstance;

    if ((lppd->Flags & PD_ENABLEPRINTTEMPLATE) || (lppd->Flags & PD_ENABLEPRINTTEMPLATEHANDLE))
    {
        if (lppd->Flags & PD_ENABLEPRINTTEMPLATE)
            template_print = resource_to_dialog32(hInst, MapSL(lppd->lpPrintTemplateName));
        else
            template_print = handle_to_dialog32(lppd->hPrintTemplate);
        pd32.hPrintTemplate = (HGLOBAL)template_print;
        pd32.Flags |= PD_ENABLEPRINTTEMPLATEHANDLE;
    }
    if ((lppd->Flags & PD_ENABLESETUPTEMPLATE) || (lppd->Flags & PD_ENABLESETUPTEMPLATEHANDLE))
    {
        if (lppd->Flags & PD_ENABLESETUPTEMPLATE)
            template_setup = resource_to_dialog32(hInst, MapSL(lppd->lpSetupTemplateName));
        else
            template_setup = handle_to_dialog32(lppd->hSetupTemplate);
        pd32.hSetupTemplate = (HGLOBAL)template_setup;
        pd32.Flags |= PD_ENABLESETUPTEMPLATEHANDLE;
    }
    if (lppd->Flags & PD_ENABLEPRINTHOOK)
    {
        COMMDLGTHUNK *thunk = allocate_thunk(pd, (SEGPTR)lppd->lpfnPrintHook);
        pd32.Flags |= PD_ENABLEPRINTHOOK;
        pd32.lpfnPrintHook = (LPPRINTHOOKPROC)thunk;
    }
    if (lppd->Flags & PD_ENABLESETUPHOOK)
    {
        COMMDLGTHUNK *thunk = allocate_thunk(pd, (SEGPTR)lppd->lpfnSetupHook);
        pd32.Flags |= PD_ENABLEPRINTHOOK;
        pd32.lpfnSetupHook = (LPSETUPHOOKPROC)thunk;
    }

    /* Generate failure with CDERR_STRUCTSIZE, when needed */
    if (lppd->lStructSize != sizeof(PRINTDLG16)) pd32.lStructSize--;

    if ((ret = PrintDlgA( &pd32 )))
    {
        lppd->hDC = HDC_16( pd32.hDC );
        global_handle_to_16( &lppd->hDevNames, pd32.hDevNames );

        lppd->nFromPage   = pd32.nFromPage;
        lppd->nToPage     = pd32.nToPage;
        lppd->nMinPage    = pd32.nMinPage;
        lppd->nMaxPage    = pd32.nMaxPage;
        lppd->nCopies     = pd32.nCopies;
        if (pd32.hDevMode)
        {
            if (!lppd->hDevMode)
                lppd->hDevMode = GlobalAlloc16(GMEM_MOVEABLE, sizeof(DEVMODE16));
            DEVMODE32To16(GlobalLock16(lppd->hDevMode), GlobalLock(pd32.hDevMode));
            GlobalUnlock(pd32.hDevMode);
            GlobalUnlock16(lppd->hDevMode);
        }
    }
    GlobalFree( pd32.hDevNames );
    GlobalFree( pd32.hDevMode );
    delete_thunk(pd32.lpfnPrintHook);
    delete_thunk(pd32.lpfnSetupHook);
    HeapFree(GetProcessHeap(), 0, template_setup);
    HeapFree(GetProcessHeap(), 0, template_print);
    return ret;
}

/***********************************************************************
 *           PrintDlgProc   (COMMDLG.21)
 */
BOOL16 CALLBACK PrintDlgProc16(HWND16 hDlg16, UINT16 uMsg, WPARAM16 wParam, LPARAM lParam)
{
    FIXME( "%04x %04x %04x %08lx: stub\n", hDlg16, uMsg, wParam, lParam );
    return FALSE;
}

/***********************************************************************
 *           PrintSetupDlgProc   (COMMDLG.22)
 */
BOOL16 CALLBACK PrintSetupDlgProc16(HWND16 hWnd16, UINT16 wMsg, WPARAM16 wParam,
				   LPARAM lParam)
{
  HWND hWnd = HWND_32(hWnd16);
  switch (wMsg)
    {
    case WM_INITDIALOG:
      TRACE("WM_INITDIALOG lParam=%08lX\n", lParam);
      ShowWindow(hWnd, SW_SHOWNORMAL);
      return (TRUE);
    case WM_COMMAND:
      switch (wParam) {
      case IDOK:
	EndDialog(hWnd, TRUE);
	return(TRUE);
      case IDCANCEL:
	EndDialog(hWnd, FALSE);
	return(TRUE);
      }
      return(FALSE);
    }
  return FALSE;
}
