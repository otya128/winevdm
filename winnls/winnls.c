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
#include <windows.h>
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
    ERR("IMPQueryIME16(0x%p) ret{0x%p, \"%s\", \"%s\", \"%s\", %d}\n", lpIMEPro, imepro32.hWnd, imepro32.szDescription, imepro32.szName, imepro32.szOptions, imepro32.wVersion);
	IMEPROToIMEPRO16(&imepro32, lpIMEPro);
	return ret;
}
BOOL WINAPI IMPGetIME16(IN HWND16 hWnd, OUT LPIMEPRO16 lpIMEPro)
{
	IMEPROA imepro32;
	BOOL ret = IMPGetIMEA(HWND_32(hWnd), &imepro32);
    ERR("\n");
	IMEPROToIMEPRO16(&imepro32, lpIMEPro);
	return ret;
}
BOOL  WINAPI IMPSetIME16(IN HWND16 hWnd, IN LPIMEPRO16 lpIMEPro)
{
	IMEPROA imepro32;
	IMEPRO16ToIMEPRO(lpIMEPro, &imepro32);
    ERR("\n");
	return IMPSetIMEA(HWND_32(hWnd), &imepro32);
}
BOOL WINAPI IMPModifyIME16(LPSTR lpStr, IN LPIMEPRO lpIMEPro)
{
	ERR("NOTIMPL:IMPModifyIME16(\"%s\", 0x%p)\n", lpStr, lpIMEPro);
    return FALSE;
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
#include <pshpack1.h>
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
C_ASSERT(sizeof(IMESTRUCT16) == 0x16);
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
#include <poppack.h>

/* convert a LOGFONT16 to a LOGFONTA */
static void logfont_16_to_A(const LOGFONT16 *font16, LPLOGFONTA font32)
{
    font32->lfHeight = font16->lfHeight;
    font32->lfWidth = font16->lfWidth;
    font32->lfEscapement = font16->lfEscapement;
    font32->lfOrientation = font16->lfOrientation;
    font32->lfWeight = font16->lfWeight;
    font32->lfItalic = font16->lfItalic;
    font32->lfUnderline = font16->lfUnderline;
    font32->lfStrikeOut = font16->lfStrikeOut;
    font32->lfCharSet = font16->lfCharSet;
    font32->lfOutPrecision = font16->lfOutPrecision;
    font32->lfClipPrecision = font16->lfClipPrecision;
    font32->lfQuality = font16;
    font32->lfPitchAndFamily = font16->lfPitchAndFamily;
    memcpy(font32->lfFaceName, font16->lfFaceName, LF_FACESIZE);
    font32->lfFaceName[LF_FACESIZE - 1] = 0;
}

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
    LRESULT ret = 0;
    LOGFONTA lfont;
    lpime32->fnc = lpime->fnc;
    lpime32->wParam = lpime->wParam;
    lpime32->wCount = lpime->wCount;
    lpime32->dchSource = lpime->dchSource;
    lpime32->dchDest = lpime->dchDest;
    lpime32->lParam1 = lpime->lParam1;
    lpime32->lParam2 = lpime->lParam2;
    lpime32->lParam3 = lpime->lParam3;
    switch (lpime32->fnc)
    {
    case IME_SETCONVERSIONWINDOW:
    {
        switch (lpime->wParam)
        {
        case MCW_WINDOW:
            TRACE("MCW_WINDOW(%p, %p, %p)\n", lpime->lParam1, lpime->lParam2, lpime->lParam3);
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
            TRACE("IME_SETCONVERSIONWINDOW unknwon %04x (%p, %p, %p)\n", lpime->wParam, lpime->lParam1, lpime->lParam2, lpime->lParam3);
            break;
        }
    }
        break;
    case IME_ENTERWORDREGISTERMODE:
        TRACE("IME_ENTERWORDREGISTERMODE\n");
        break;
    case IME_GETCONVERSIONMODE:
        TRACE("IME_GETCONVERSIONMODE\n");
        break;
    case 0x12:
    case IME_SETCONVERSIONFONTEX:
    {
        LPLOGFONTA lplogfont = &lfont;
        TRACE("IME_SETCONVERSIONFONTEX %d\n", lpime32->fnc);

        if (lpime32->fnc == 0x12)
        {
            HFONT hFont32 = HFONT_32(lpime->wParam);
            if (!hFont32 || !GetObjectA(hFont32, sizeof(LOGFONTA), lplogfont))
            {
                ret = 0;
                goto done;
            }
        }
        else if (lpime32->fnc == IME_SETCONVERSIONFONTEX)
        {
            if (lpime32->lParam1)
            {
                LPLOGFONT16 l16 = (LPLOGFONT16)GlobalLock16(lpime32->lParam1);
                logfont_16_to_A(l16, &lfont);
            }
            else
            {
                HFONT hFont32 = GetStockObject(SYSTEM_FONT);
                if (!hFont32 || !GetObjectA(hFont32, sizeof(LOGFONTA), lplogfont))
                {
                    ret = 0;
                    goto done;
                }
            }
        }
        lpime32->lParam1 = lplogfont;
        HIMC himc = ImmGetContext(hwnd32);
        //IME_SETCONVERSIONFONTEX doesnt work well
        ImmSetCompositionFontA(himc, lplogfont);
        if (lpime32->fnc == IME_SETCONVERSIONFONTEX)
        {
            GlobalUnlock16(lpime32->lParam1);
        }
    }
    break;
    case IME_SETCONVERSIONMODE:
        TRACE("IME_SETCONVERSIONMODE\n");
        break;
    //case IME_SETLEVEL:
    //    ERR("IME_SETLEVEL\n");
    //    break;
    case IME_GETOPEN:
        TRACE("IME_GETOPEN\n");
        break;
    case IME_SETOPEN:
        TRACE("IME_SETOPEN\n");
        break;
    case IME_GETVERSION:
        TRACE("IME_GETVERSION\n");
        break;
        /*
    case IME_SET_MODE:
        TRACE("IME_SET_MODE\n");
        break;
        */
    case IME_GETIMECAPS:
        TRACE("IME_GETIMECAPS\n");
        break;
    case IME_SENDVKEY:
        TRACE("IME_SENDVKEY\n");
        break;
    //case IME_SET_MODEK:
    //    ERR("IME_SET_MODEK\n");
    //  break;
    default:
        TRACE("unknown %04x\n", lpime->fnc);
        break;
    }
    TRACE("(%04x,%04x,%04x,%04x,%04x,%08x,%08x,%08x)\n", lpime->fnc, lpime->wParam, lpime->wCount, lpime->dchSource, lpime->dchDest, lpime->lParam1, lpime->lParam2, lpime->lParam3);
    ret = SendIMEMessageExA(hwnd32, (LPARAM)hglobal32);
    lpime->fnc = lpime32->fnc;
    lpime->wParam = lpime32->wParam;
    lpime->wCount = lpime32->wCount;
    lpime->dchSource = lpime32->dchSource;
    lpime->dchDest = lpime32->dchDest;
    lpime->lParam1 = lpime32->lParam1;
    lpime->lParam2 = lpime32->lParam2;
    lpime->lParam3 = lpime32->lParam3;
    TRACE("(%04x,%04x,%04x,%04x,%04x,%08x,%08x,%08x) retval=%08x\n", lpime->fnc, lpime->wParam, lpime->wCount, lpime->dchSource, lpime->dchDest, lpime->lParam1, lpime->lParam2, lpime->lParam3, ret);
    lpime->wParam = 0;
done:
    GlobalUnlock(hglobal32);
    GlobalUnlock16(hglobal);
    GlobalFree(hglobal32);
    return ret;
}
BOOL WINAPI WINNLSSetIMEHandle16(SEGPTR a1, HWND16 a2)
{
    LPCSTR str = MapSL(a1); /* MSIME */
    HWND hwnd = HWND_32(a2);/* window class: MSIME window caption: MSIMEServer */
    FIXME("(%s,%p)\n", debugstr_a(str), hwnd);
    return TRUE;
}

BOOL WINAPI WINNLSSetIMEHotkey16(HWND16 a1, WORD a2)
{
    HWND hwnd = HWND_32(a1);
    FIXME("(%p,%02x)\n", hwnd, a2);
    return TRUE;
}

BOOL WINAPI WINNLSSetIMEStatus16(HWND16 a1, WORD a2)
{
    HWND hwnd = HWND_32(a1);
    FIXME("(%p,%02x)\n", hwnd, a2);
    return TRUE;
}

BOOL WINAPI WINNLSSetKeyState16(WORD a1)
{
    FIXME("(%02x)\n", a1);
    return TRUE;
}
