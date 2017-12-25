/*
 * Copyright 2001 Andreas Mohr
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
#include "winnls32.h"
#include <imm.h>
#include "wownt32.h"
#include "wine/winuser16.h"

WINE_DEFAULT_DEBUG_CHANNEL(winnls);
WINE_DECLARE_DEBUG_CHANNEL(winnls);
//BOOL WINAPI WINNLS32EnableIME(HWND hWnd, BOOL fEnable);
//BOOL WINAPI WINNLS32GetEnableStatus(HWND hWnd);

/***********************************************************************
 *		WINNLSEnableIME (WINNLS.16)
 */
BOOL WINAPI WINNLSEnableIME16( HWND16 hwnd, BOOL enable )
{
    return WINNLSEnableIME( HWND_32(hwnd), enable );
}

/***********************************************************************
 *		WINNLSGetEnableStatus (WINNLS.18)
 */
BOOL WINAPI WINNLSGetEnableStatus16( HWND16 hwnd )
{
    return WINNLSGetEnableStatus( HWND_32(hwnd) );
}
#include "ime.h"
#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(nls);
#include <pshpack1.h>
typedef struct _tagIMEPRO16 {
	HWND16      hWnd;
	DATETIME    InstDate;
	UINT16      wVersion;
	BYTE        szDescription[50];
	BYTE        szName[80];
	BYTE        szOptions[30];
} IMEPRO16, *PIMEPRO16, NEAR *NPIMEPRO16, FAR *LPIMEPRO16;
#include <poppack.h>
void IMEPRO16ToIMEPRO(IMEPRO16 *imepro, IMEPROA *imepro32)
{
	imepro32->hWnd = HWND_32(imepro->hWnd);
    imepro32->InstDate = imepro->InstDate;
    imepro32->wVersion = imepro->wVersion;
    memcpy(imepro32->szDescription, imepro->szDescription, sizeof(imepro32->szDescription));
    memcpy(imepro32->szName, imepro->szName, sizeof(imepro32->szName));
    memcpy(imepro32->szOptions, imepro->szOptions, sizeof(imepro32->szOptions));
}
void IMEPROToIMEPRO16(IMEPROA *imepro32, IMEPRO16 *imepro)
{
	imepro->hWnd = HWND_16(imepro32->hWnd);
    imepro->InstDate = imepro32->InstDate;
    //lost data
    imepro->wVersion = (UINT16)imepro32->wVersion;
    memcpy(imepro->szDescription, imepro32->szDescription, sizeof(imepro32->szDescription));
    memcpy(imepro->szName, imepro32->szName, sizeof(imepro32->szName));
    memcpy(imepro->szOptions, imepro32->szOptions, sizeof(imepro32->szOptions));
}
BOOL16  WINAPI IMPQueryIME16(IN OUT LPIMEPRO16 lpIMEPro)
{
	IMEPROA imepro32;
    //WHY???
    if (!strcmp(lpIMEPro->szName, "msctf.dll"))
        return FALSE;
	IMEPRO16ToIMEPRO(lpIMEPro, &imepro32);
	BOOL ret = IMPQueryIMEA(&imepro32);
	TRACE("IMPQueryIME16(0x%p) ret{0x%p, \"%s\", \"%s\", \"%s\", %d}\n", lpIMEPro, imepro32.hWnd, imepro32.szDescription, imepro32.szName, imepro32.szOptions, imepro32.wVersion);
	IMEPROToIMEPRO16(&imepro32, lpIMEPro);
	return ret;
}
BOOL WINAPI IMPGetIME16(IN HWND16 hWnd, OUT LPIMEPRO lpIMEPro)
{
	IMEPROA imepro32;
	BOOL ret = IMPGetIMEA(HWND_32(hWnd), &imepro32);
	IMEPROToIMEPRO16(&imepro32, lpIMEPro);
	return ret;
}
BOOL  WINAPI IMPSetIME16(IN HWND16 hWnd, IN LPIMEPROA lpIMEPro)
{
	IMEPROA imepro32;
	IMEPRO16ToIMEPRO(lpIMEPro, &imepro32);
	return IMPSetIME16(HWND_32(hWnd), &imepro32);
}
BOOL WINAPI IMPModifyIME16(LPSTR lpStr, IN LPIMEPRO lpIMEPro)
{
	ERR("NOTIMPL:IMPModifyIME16(\"%s\", 0x%p)\n", lpStr, lpIMEPro);
}
WORD WINAPI IMPGetDefaultIME16(LPIMEPRO lpIMEPro)
{
	ERR("NOTIMPL:IMPGetDefaultIME16(0x%p)\n", lpIMEPro);
	return 0;
}
WORD WINAPI IMPSetDefaultIME16(LPIMEPRO lpIMEPro)
{
	ERR("NOTIMPL:IMPSetDefaultIME16(0x%p)\n", lpIMEPro);
	return 0;
}
BOOL WINAPI WINNLSSendString16(HWND16 hWnd, WORD word, LPVOID lpVoid)
{
	ERR("NOTIMPL:WINNLSSendString16(0x%p, %d, %p)\n", HWND_32(hWnd), word, lpVoid);
	return 0;
}
BOOL WINAPI WINNLSPostAppMessage16(HWND16 hWnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
	ERR("NOTIMPL:WINNLSPostAppMessage16(0x%p, %d, %d, %d)\n", HWND_32(hWnd), msg, wParam, lParam);
	return 0;
}
LRESULT WINAPI WINNLSSendAppMessage16(HWND16 hWnd, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
	ERR("NOTIMPL:WINNLSSendAppMessage16(0x%p, %d, %d, %d)\n", HWND_32(hWnd), msg, wParam, lParam);
	return 0;
}
BOOL WINAPI WINNLSSendControl16(WORD arg1, WORD arg2)
{
	ERR("NOTIMPL:WINNLSSendControl16(%d, %d)\n", arg1, arg2);
	return 0;
}
typedef struct tagIMEMESSAGE16 {
    WORD unknown1;
    WORD unknown2;
    WORD unknown3;
    WORD unknown4;
    WORD unknown5;
    WORD unknown6;
    WORD unknown7;
    WORD unknown8;
} IMEMESSAGE16, FAR *LPIMEMESSAGE16;
LRESULT WINAPI SendIMEMessageEx16(
    _In_ HWND16   hwnd,
    _In_ LPARAM lParam
);
LRESULT WINAPI SendIMEMessage16(HWND16 a1, WPARAM16 a2, WPARAM16 a3)
{
    ERR("NOTIMPL:SendIMEMessage16(%04X, %04X, %04X)\n", a1, a2, a3);
    return SendIMEMessageEx16(a1, a2 << 16 | a3);
}
typedef struct tagIMESTRUCT16 {
    UINT16   fnc;        // function code
    WPARAM16 wParam;     // word parameter
    UINT16   wCount;     // word counter
    UINT16   dchSource;  // offset to Source from top of memory object
    UINT16   dchDest;    // offset to Desrination from top of memory object
    LPARAM   lParam1;
    LPARAM   lParam2;
    LPARAM   lParam3;
} IMESTRUCT16, *PIMESTRUCT16, NEAR *NPIMESTRUCT16, FAR *LPIMESTRUCT16;
LRESULT WINAPI SendIMEMessageEx16(
    _In_ HWND16   hwnd,
    _In_ LPARAM lParam
)
{
    HWND hwnd32 = HWND_32(hwnd);
    HGLOBAL16 hglobal = lParam;
    LPIMESTRUCT16 lpime = (LPIMESTRUCT16)(GlobalLock16(hglobal));
    HGLOBAL hglobal32 = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(IMESTRUCT));
    LPIMESTRUCT lpime32 = (LPIMESTRUCT)(GlobalLock(hglobal32));
    lpime32->fnc = lpime->fnc;
    lpime32->wParam = lpime->wParam;
    lpime32->wCount = lpime->wCount;
    lpime32->dchSource = lpime->dchSource;
    lpime32->dchDest = lpime->dchDest;
    lpime32->lParam1 = lpime->lParam1;
    lpime32->lParam2 = lpime->lParam2;
    lpime32->lParam3 = lpime->lParam3;
    const char *a = GlobalLock16(lpime->lParam1);
    const char *b = GlobalLock16(lpime->lParam2);
    GlobalUnlock16(lpime->lParam1);
    GlobalUnlock16(lpime->lParam2);
    switch (lpime->fnc)
    {
    case IME_SETCONVERSIONWINDOW:
    {
        switch (lpime->wParam)
        {
        case MCW_WINDOW:
        {
            TRACE("MCW_WINDOW(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            COMPOSITIONFORM c = { 0 };
            c.dwStyle = CFS_FORCE_POSITION;
            c.ptCurrentPos.x = (lpime->lParam1 >> 16) & ~0x800;
            c.ptCurrentPos.y = (short)lpime->lParam1;
            HIMC hi = ImmGetContext(hwnd32);
            BOOL a = ImmSetCompositionWindow(hi, &c);
            ImmReleaseContext(hwnd32, hi);
            return;
        }
            break;
        case MCW_DEFAULT:
            TRACE("MCW_DEFAULT(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_RECT:
            TRACE("MCW_RECT(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_SCREEN:
            TRACE("MCW_SCREEN(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_VERTICAL:
            TRACE("MCW_VERTICAL(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_HIDDEN:
            TRACE("MCW_HIDDEN(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_WINDOW | MCW_RECT:
            TRACE("MCW_WINDOW | MCW_RECT(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        case MCW_SCREEN | MCW_RECT:
            TRACE("MCW_SCREEN | MCW_RECT(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
    LRESULT ret = SendIMEMessageExA(hwnd32, (LPARAM)hglobal32);
    lpime->fnc = lpime32->fnc;
    lpime->wParam = lpime32->wParam;
    lpime->wCount = lpime32->wCount;
    lpime->dchSource = lpime32->dchSource;
    lpime->dchDest = lpime32->dchDest;
    lpime->lParam1 = lpime32->lParam1;
    lpime->lParam2 = lpime32->lParam2;
    lpime->lParam3 = lpime32->lParam3;
    GlobalUnlock(hglobal32);
    GlobalUnlock16(hglobal);
    GlobalFree(hglobal32);
    return ret;
}

///////////////
#include <stdio.h>
void __wine_spec_init_ctor()
{
	ERR("NOTIMPL:__wine_spec_init_ctor()\n");
}
void __wine_spec_unimplemented_stub(const char *module, const char *function)
{
	ERR("NOTIMPL:__wine_spec_unimplemented_stub(%s, %s)\n", module, function);
}
void __wine_spec_dll_entry()
{
	ERR("NOTIMPL:__wine_spec_dll_entry(?)\n");
}
