/*
 *	OLE2NLS library
 *
 *	Copyright 1995	Martin von Loewis
 *      Copyright 1998  David Lee Lambert
 *      Copyright 2000  Julio César Gázquez
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

#include "config.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>

#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "winuser.h"

#include "wine/winbase16.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

/* FIXME: other language flags? */

/* same as win32 */
#define NORM_IGNORECASE16           0x00000001
#define NORM_IGNORENONSPACE16       0x00000002
#define NORM_IGNORESYMBOLS16        0x00000004
/* different from win32 */
#define NORM_IGNOREWIDTH16          0x00000008
#define NORM_IGNOREKANATYPE16       0x00000040

/* deleted? */
#define NORM_IGNOREJAPANACCENT16    0x00000080
#define NORM_IGNOREKASHIDA16        0x00040000

#define NORM_ALL16 (NORM_IGNORECASE16 | NORM_IGNORENONSPACE16 | NORM_IGNORESYMBOLS16 | NORM_IGNOREWIDTH16 | NORM_IGNOREKANATYPE16 | NORM_IGNOREJAPANACCENT16 | NORM_IGNOREKASHIDA16)

/* same as win32 */
#define LCMAP_LOWERCASE16    0x00000100
#define LCMAP_UPPERCASE16    0x00000200
#define LCMAP_SORTKEY16      0x00000400
/* different from win32 */
#define LCMAP_HALFWIDTH16    0x00000800
#define LCMAP_FULLWIDTH16    0x00001000
#define LCMAP_HIRAGANA16     0x00002000
#define LCMAP_KATAKANA16     0x00004000

#define LCMAP_ALL16 (LCMAP_LOWERCASE16 | LCMAP_UPPERCASE16 | LCMAP_SORTKEY16 | LCMAP_HALFWIDTH16 | LCMAP_FULLWIDTH16 | LCMAP_HIRAGANA16 | LCMAP_KATAKANA16)
static LPVOID lpNLSInfo = NULL;

/******************************************************************************
 *		GetLocaleInfoA	[OLE2NLS.5]
 * Is the last parameter really WORD for Win16?
 */
INT16 WINAPI GetLocaleInfo16(LCID lcid,LCTYPE LCType,LPSTR buf,INT16 len)
{
	return GetLocaleInfoA(lcid,LCType,buf,len);
}

/******************************************************************************
 *		GetStringTypeA	[OLE2NLS.7]
 */
BOOL16 WINAPI GetStringType16(LCID locale,DWORD dwInfoType,LPCSTR src,
                              INT16 cchSrc,LPWORD chartype)
{
	return GetStringTypeExA(locale,dwInfoType,src,cchSrc,chartype);
}

/******************************************************************************
 *		GetUserDefaultLCID	[OLE2NLS.1]
 */
LCID WINAPI GetUserDefaultLCID16(void)
{
    return GetUserDefaultLCID();
}

/******************************************************************************
 *		GetSystemDefaultLCID	[OLE2NLS.2]
 */
LCID WINAPI GetSystemDefaultLCID16(void)
{
    return GetSystemDefaultLCID();
}

/******************************************************************************
 *		GetUserDefaultLangID	[OLE2NLS.3]
 */
LANGID WINAPI GetUserDefaultLangID16(void)
{
    return GetUserDefaultLangID();
}

/******************************************************************************
 *		GetSystemDefaultLangID	[OLE2NLS.4]
 */
LANGID WINAPI GetSystemDefaultLangID16(void)
{
    return GetSystemDefaultLangID();
}

static DWORD convert_NORM16_to_NORM(DWORD flags16)
{
    DWORD flags = 0;
    if (flags16 & NORM_IGNORECASE16)
        flags |= NORM_IGNORECASE;
    if (flags16 & NORM_IGNORENONSPACE16)
        flags |= NORM_IGNORENONSPACE;
    if (flags16 & NORM_IGNORESYMBOLS16)
        flags |= NORM_IGNORESYMBOLS;
    if (flags16 & NORM_IGNOREWIDTH16)
        flags |= NORM_IGNOREWIDTH;
    if (flags16 & NORM_IGNOREKANATYPE16)
        flags |= NORM_IGNOREKANATYPE;
    if (flags16 & NORM_IGNOREJAPANACCENT16)
    {
        FIXME("NORM_IGNOREJAPANACCENT\n");
        flags |= 0;
    }
    if (flags16 & NORM_IGNOREKASHIDA16)
    {
        FIXME("NORM_IGNOREKASHIDA\n");
        flags |= 0;
    }
    return flags;
}
static DWORD convert_LCMAP16_to_LCMAP(DWORD mapflags16)
{
    DWORD mapflags = 0;
    if (mapflags16 & LCMAP_LOWERCASE16)
        mapflags |= LCMAP_LOWERCASE;
    if (mapflags16 & LCMAP_UPPERCASE16)
        mapflags |= LCMAP_UPPERCASE;
    if (mapflags16 & LCMAP_SORTKEY16)
        mapflags |= LCMAP_SORTKEY;
    if (mapflags16 & LCMAP_HALFWIDTH16)
        mapflags |= LCMAP_HALFWIDTH;
    if (mapflags16 & LCMAP_FULLWIDTH16)
        mapflags |= LCMAP_FULLWIDTH;
    if (mapflags16 & LCMAP_HIRAGANA16)
        mapflags |= LCMAP_HIRAGANA;
    if (mapflags16 & LCMAP_KATAKANA16)
        mapflags |= LCMAP_KATAKANA;
    return mapflags | convert_NORM16_to_NORM(mapflags16);
}

/******************************************************************************
 *		LCMapStringA	[OLE2NLS.6]
 */
INT16 WINAPI LCMapString16(LCID lcid, DWORD mapflags16, LPCSTR srcstr, INT16 srclen,
			   LPSTR dststr, INT16 dstlen)
{
    DWORD mapflags = 0;
    if ((mapflags16 & ~(LCMAP_ALL16 | NORM_ALL16)) != 0)
    {
        return 0;
    }
    mapflags = convert_LCMAP16_to_LCMAP(mapflags16);
    return (INT16)LCMapStringA(lcid, mapflags, srcstr, srclen, dststr, dstlen);
}

/***********************************************************************
 *           CompareStringA       (OLE2NLS.8)
 */
INT16 WINAPI CompareString16(LCID lcid, DWORD flags16, LPCSTR str1, INT16 len1, LPCSTR str2, INT16 len2)
{
    DWORD flags = 0;
    if ((flags16 & ~NORM_ALL16) != 0)
    {
        return 0;
    }
    flags = convert_NORM16_to_NORM(flags16);
    return CompareStringA(lcid, flags, str1, len1, str2, len2);
}

/******************************************************************************
 *      RegisterNLSInfoChanged  [OLE2NLS.9]
 */
BOOL16 WINAPI RegisterNLSInfoChanged16(LPVOID lpNewNLSInfo) /* [???] FIXME */
{
	FIXME("Fully implemented, but doesn't effect anything.\n");

	if (!lpNewNLSInfo) {
		lpNLSInfo = NULL;
		return TRUE;
	}
	else {
		if (!lpNLSInfo) {
			lpNLSInfo = lpNewNLSInfo;
			return TRUE;
		}
	}

	return FALSE; /* ptr not set */
}
