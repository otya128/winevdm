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

///////////////
#include <stdio.h>
void __wine_spec_init_ctor()
{
	fprintf(stderr, "NOTIMPL:__wine_spec_init_ctor()\n");
}
void __wine_spec_unimplemented_stub(const char *module, const char *function)
{
	fprintf(stderr, "NOTIMPL:__wine_spec_unimplemented_stub(%s, %s)\n", module, function);
}
void __wine_spec_dll_entry()
{
	fprintf(stderr, "NOTIMPL:__wine_spec_dll_entry(?)\n");
}
