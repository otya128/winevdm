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
#include "wownt32.h"
#include "wine/winuser16.h"

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
typedef struct _tagIMEPRO16 {
	HWND16      hWnd;
	DATETIME    InstDate;
	UINT        wVersion;
	BYTE        szDescription[50];
	BYTE        szName[80];
	BYTE        szOptions[30];
} IMEPRO16, *PIMEPRO16, NEAR *NPIMEPRO16, FAR *LPIMEPRO16;
void IMEPRO16ToIMEPRO(IMEPRO16 *imepro, IMEPROA *imepro32)
{
	imepro32->hWnd = HWND_32(imepro->hWnd);
	memcpy(((HWND*)imepro32) + 1, ((HWND16*)imepro) + 1, sizeof(IMEPROA) - sizeof(HWND));
}
void IMEPROToIMEPRO16(IMEPROA *imepro32, IMEPRO16 *imepro)
{
	imepro->hWnd = HWND_16(imepro32->hWnd);
	memcpy(((HWND16*)imepro) + 1, ((HWND*)imepro32) + 1, sizeof(IMEPRO16) - sizeof(HWND16));
}
BOOL16  WINAPI IMPQueryIME16(IN OUT LPIMEPRO16 lpIMEPro)
{
	IMEPROA imepro32;
	IMEPRO16ToIMEPRO(lpIMEPro, &imepro32);
	BOOL ret = IMPQueryIMEA(&imepro32);
	TRACE("IMPQueryIME16(0x%p) ret{0x%p, \"%s\", \"%s\", \"%s\", %d}\n", lpIMEPro, imepro32.hWnd, imepro32.szDescription, imepro32.szName, imepro32.szOptions, imepro32.wVersion);
	IMEPROToIMEPRO16(&imepro32, lpIMEPro);
	return ret;
}
BOOL WINAPI IMPGetIME16(IN HWND16 hWnd, OUT LPIMEPRO lpIMEPro)
{
	IMEPROA imepro32;
	BOOL ret = IMPGetIME16(HWND_32(hWnd), &imepro32);
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
