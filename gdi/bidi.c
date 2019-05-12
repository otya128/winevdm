/*
 * Win16 BiDi functions
 * Copyright 2000 Erez Volk
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
 *
 * NOTE: Right now, most of these functions do nothing.
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wine/wingdi16.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(gdi);

/***********************************************************************
 *		RawTextOut   (GDI.530)
 */
LONG WINAPI RawTextOut16(HDC16 hdc, INT16 x, INT16 y, LPCSTR str, INT16 count)
{
    FIXME("(%04x,%d,%d,%s,%d)\n", hdc, x, y, debugstr_an(str, count), count);
    return TextOut16(hdc, x, y, str, count);
}

/***********************************************************************
 *		RawExtTextOut   (GDI.531)
 */
LONG WINAPI RawExtTextOut16( HDC16 hdc, INT16 x, INT16 y, UINT16 flags,
                            const RECT16 *lprect, LPCSTR str, UINT16 count,
                            const INT16 *lpDx )
{
    FIXME("(%04x,%d,%d,%04x,%p,%s,%d,%p)\n", hdc, x, y, flags, lprect, 
        debugstr_an(str, count), count, lpDx);
    return ExtTextOut16(hdc, x, y, flags, lprect, str, count, lpDx);
}

/***********************************************************************
 *		RawGetTextExtent   (GDI.532)
 */
LONG WINAPI RawGetTextExtent16(HDC16 hdc, LPCSTR lpszString, INT16 cbString )
{
      FIXME("(%04hx, %p, %hd): stub\n", hdc, lpszString, cbString);
      return GetTextExtent16(hdc, lpszString, cbString);
}

/***********************************************************************
 *		RawGetTextExtentEx   (GDI.533)
 */
LONG WINAPI RawGetTextExtentEx16(void)
{
    FIXME("stub (no prototype)\n");
    return 0;
}

/***********************************************************************
 *		BiDiLayout   (GDI.536)
 */
LONG WINAPI BiDiLayout16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCreateTabString   (GDI.537)
 */
LONG WINAPI BiDiCreateTabString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCreateString   (GDI.538)
 */
LONG WINAPI BiDiCreateString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiStringOut   (GDI.539)
 */
LONG WINAPI BiDiStringOut16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGlyphOut   (GDI.540)
 */
LONG WINAPI BiDiGlyphOut16(WORD a1, WORD a2, WORD a3, WORD a4, LPCSTR a5, WORD a6, WORD a7, WORD a8)
{
    FIXME("stub(%04x,%04x,%04x,%04x,%s,%04x,%04x,%04x)\n", a1, a2, a3, a4, debugstr_a(a5), a6, a7, a8);
    return 0;
}

/***********************************************************************
 *		BiDiJustifyString   (GDI.541)
 */
LONG WINAPI BiDiJustifyString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiSetStringTabs   (GDI.542)
 */
LONG WINAPI BiDiSetStringTabs16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGetStringExtent   (GDI.543)
 */
LONG WINAPI BiDiGetStringExtent16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGetNextGlyph   (GDI.544)
 */
LONG WINAPI BiDiGetNextGlyph16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGetPrevGlyph   (GDI.545)
 */
LONG WINAPI BiDiGetPrevGlyph16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiIsStringRTL   (GDI.546)
 */
LONG WINAPI BiDiIsStringRTL16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGlyphLength   (GDI.547)
 */
LONG WINAPI BiDiGlyphLength16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCaretStringToPel   (GDI.548)
 */
LONG WINAPI BiDiCaretStringTopel16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCaretPelToString   (GDI.549)
 */
LONG WINAPI BiDiCaretPelToString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiStringToGlyph   (GDI.550)
 */
LONG WINAPI BiDiStringToGlyph16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGlyphToString   (GDI.551)
 */
LONG WINAPI BiDiGlyphToString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiPelToGlyph   (GDI.552)
 */
LONG WINAPI BiDiPelToGlyph16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGlyphToPel   (GDI.553)
 */
LONG WINAPI BiDiGlyphToPel16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiBounds   (GDI.554)
 */
LONG WINAPI BiDiBounds16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiDeleteString   (GDI.555)
 */
LONG WINAPI BiDiDeleteString16(WORD a1)
{
    FIXME("stub(%04x)\n", a1);
    return 0;
}

/***********************************************************************
 *		BiDiSetDefaults   (GDI.556)
 */
LONG WINAPI BiDiSetDefaults16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiGetDefaults   (GDI.558)
 */
LONG WINAPI BiDiGetDefaults16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCalcString16   (GDI.559)
 */
LONG WINAPI BiDiCalcString16(WORD a1, WORD a2, LPCSTR a3, WORD a4, WORD a5, WORD a6, WORD a7, WORD a8)
{
    FIXME("stub(%04x,%04x,%p,%04x,%04x,%04x,%04x,%04x)\n", a1, a2, a3, a4, a5, a6, a7, a8);
    return 0;
}

/***********************************************************************
 *		BiDiShape   (GDI.560)
 */
LONG WINAPI BiDiShape16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiFontComplement   (GDI.561)
 */
LONG WINAPI BiDiFontComplement16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCalcTabString   (GDI.563)
 */
LONG WINAPI BiDiCalcTabString16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiSetKashida   (GDI.564)
 */
LONG WINAPI BiDiSetKashida16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiKExtTextOut   (GDI.565)
 */
LONG WINAPI BiDiKExtTextOut16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiShapeEx   (GDI.566)
 */
LONG WINAPI BiDiShapeEx16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCreateStringEx   (GDI.569)
 */
LONG WINAPI BiDiCreateStringEx16(WORD a1, WORD a2, WORD a3, LPVOID a4, WORD a5, WORD a6, WORD a7, WORD a8, WORD a9)
{
    FIXME("stub(%04x,%04x,%04x,%p,%04x,%04x,%04x,%04x,%04x)\n", a1, a2, a3, a4, a5, a6, a7, a8, a9);
    return 0;
}

/***********************************************************************
 *		GetUnicodeMap   (GDI.570)
 */
LONG WINAPI GetUnicodeMap16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		GetTextExtentRtoL   (GDI.571)
 */
LONG WINAPI GetTextExtentRtoL16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		GetHDCCharSet   (GDI.572)
 */
LONG WINAPI GetHDCCharSet16(HDC16 a1)
{
    FIXME("stub(%04x)\n", a1);
    return 0;
}

/***********************************************************************
 *		BiDiLayoutEx   (GDI.573)
 */
LONG WINAPI BiDiLayoutEx16(LPVOID a1, WORD a2, WORD a3, WORD a4, LPVOID a5, WORD a6, WORD a7, WORD a8, WORD a9, LPVOID a10, LPVOID a11, WORD a12, WORD a13, WORD a14)
{
    FIXME("stub(%p,%04x,%04x,%04x,%p,%04x,%04x,%04x,%04x,%p,%p,%04x,%04x,%04x)\n", a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
    return 0;
}

/***********************************************************************
 *		BiDiCreateTabStringEx   (GDI.574)
 */
LONG WINAPI BiDiCreateTabStringEx16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCalcTabStringEx   (GDI.575)
 */
LONG WINAPI BiDiCalcTabStringEx16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		BiDiCalcStringEx   (GDI.576)
 */
LONG WINAPI BiDiCalcStringEx16(void) { FIXME("stub (no prototype)\n"); return 0; }

/***********************************************************************
 *		SetTextCodePage   (GDI.588)
 */
LONG WINAPI SetTextCodePage16(WORD a1, WORD a2)
{
    FIXME("stub(%04x,%04x)\n", a1, a2);
    return 0;
}

/***********************************************************************
 *		GetTextCodePage   (GDI.589)
 */
LONG WINAPI GetTextCodePage16()
{
    FIXME("stub(np)\n");
    return 0;
}
