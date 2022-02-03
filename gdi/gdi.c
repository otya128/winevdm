/*
 * GDI 16-bit functions
 *
 * Copyright 2002 Alexandre Julliard
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
#include <stdlib.h>
#include <math.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "wownt32.h"
#define ExtDeviceMode WINAPI ExtDeviceMode
#include "winspool.h"
#undef ExtDeviceMode
#include "wine/wingdi16.h"
#include "wine/list.h"
#if 0
#include "wine/gdi_driver.h"
#endif
#include "wine/debug.h"
#include "wine/exception.h"
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

WINE_DEFAULT_DEBUG_CHANNEL(gdi);

//#define HGDIOBJ_32(handle16)    ((HGDIOBJ)(ULONG_PTR)(handle16))
//#define HGDIOBJ_16(handle32)    ((HGDIOBJ16)(ULONG_PTR)(handle32))
__declspec(dllimport) HGDIOBJ16 K32HGDIOBJ_16(HGDIOBJ handle);
__declspec(dllimport) HGDIOBJ K32HGDIOBJ_32(HGDIOBJ16 handle);
#define HGDIOBJ_32(handle16)    (K32HGDIOBJ_32(handle16))
#define HGDIOBJ_16(handle32)    (K32HGDIOBJ_16(handle32))
void WINAPI K32WOWHandle16Destroy(HANDLE handle, WOW_HANDLE_TYPE type);
static BYTE fix_font_charset(BYTE charset);

struct saved_visrgn
{
    struct list entry;
    HDC         hdc;
    HRGN        hrgn;
};

static struct list saved_regions = LIST_INIT( saved_regions );

static HPALETTE16 hPrimaryPalette;
static HGDIOBJ16 stock[STOCK_LAST + 1] = {0};

void WINAPI DibMapGlobalMemory(WORD sel, void *base, DWORD size);
void WINAPI DibUnmapGlobalMemory(void *base, DWORD size);
void WINAPI GlobalMapInternal(WORD sel, void *base, DWORD size);
struct dib_driver
{
    struct list entry;
    HDC         hdc;
    HANDLE      hSection;
    LPVOID      map;
    HBITMAP     bitmap;
    WORD        selector;
    DWORD       size;
    DWORD       padding;
};
static struct list dib_drivers = LIST_INIT(dib_drivers);

// on windows 3.1 in 8bit color mode:
// RGB() is matched to the 20 system colors
// PALETTEINDEX() is a direct index to the dc palette, 0 if out of range
// PALETTERGB() is matched to the dc palette
// DIBINDEX() only applies to dib.drv dcs
// color is treated as RGB() unless high byte is exactly 1, 2 or high word 0x10ff for DIBINDEX()
//
// on windows 10 in 24bit color mode:
// RGB() is the direct color
// PALETTEINDEX() is a direct index to the dc palette, 0 if out of range
// PALETTERGB() is the direct color
// DIBINDEX() is 0 for non-dib dcs
// color is treated as PALETTEINDEX() if bit 16 is set else DIBINDEX() if bit 24 is set and byte 3 is 0xff
//
// ntvdm in 8bit color mode behaves like win3.1 with DIBINDEX()
static COLORREF check_colorref(COLORREF color)
{
    int type = color >> 16;
    if ((type != 0x10ff) && ((type & 0xff00) != 0x100) && ((type & 0xff00) != 0x200))
        color &= 0xffffff;
    return color;
}

/*
 * ############################################################################
 */

#include <pshpack1.h>
#define GDI_MAX_THUNKS      32

static struct gdi_thunk
{
    BYTE                        popl_eax;       /* popl  %eax (return address) */
    BYTE                        pushl_pfn16;    /* pushl pfn16 */
    DWORD                       pfn16;          /* pfn16 */
    BYTE                        pushl_eax;      /* pushl %eax */
    BYTE                        jmp;            /* ljmp GDI_Callback3216 */
    DWORD                       callback;
    HDC16                       hdc;
} *GDI_Thunks;

#include <poppack.h>

/**********************************************************************
 *           GDI_Callback3216
 */
static BOOL CALLBACK GDI_Callback3216( DWORD pfn16, HDC hdc, INT code )
{
    if (pfn16)
    {
        WORD args[2];
        DWORD ret;

        args[1] = HDC_16(hdc);
        args[0] = code;
        WOWCallback16Ex( pfn16, WCB16_PASCAL, sizeof(args), args, &ret );
        return LOWORD(ret);
    }
    return TRUE;
}


/******************************************************************
 *		GDI_AddThunk
 *
 */
static struct gdi_thunk* GDI_AddThunk(HDC16 dc16, ABORTPROC16 pfn16)
{
    struct gdi_thunk* thunk;

    if (!GDI_Thunks)
    {
        GDI_Thunks = VirtualAlloc(NULL, GDI_MAX_THUNKS * sizeof(*GDI_Thunks),
                                        MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!GDI_Thunks)
        {
            return NULL;
        }
        for (thunk = GDI_Thunks; thunk < &GDI_Thunks[GDI_MAX_THUNKS]; thunk++)
        {
            thunk->popl_eax     = 0x58;   /* popl  %eax */
            thunk->pushl_pfn16  = 0x68;   /* pushl pfn16 */
            thunk->pfn16        = 0;
            thunk->pushl_eax    = 0x50;   /* pushl %eax */
            thunk->jmp          = 0xe9;   /* jmp GDI_Callback3216 */
            thunk->callback     = (char *)GDI_Callback3216 - (char *)(&thunk->callback + 1);
        }
    }
    for (thunk = GDI_Thunks; thunk < &GDI_Thunks[GDI_MAX_THUNKS]; thunk++)
    {
        if (thunk->pfn16 == 0)
        {
            thunk->pfn16 = (DWORD)pfn16;
            thunk->hdc   = dc16;
            return thunk;
        }
    }
    FIXME("Out of mmdrv-thunks. Bump GDI_MAX_THUNKS\n");
    return NULL;
}

/******************************************************************
 *		GDI_DeleteThunk
 */
static void    GDI_DeleteThunk(struct gdi_thunk* thunk)
{
    thunk->pfn16 = 0;
}

/******************************************************************
 *		GDI_FindThunk
 */
static struct gdi_thunk*        GDI_FindThunk(HDC16 hdc)
{
    struct gdi_thunk* thunk;

    if (!GDI_Thunks) return NULL;
    for (thunk = GDI_Thunks; thunk < &GDI_Thunks[GDI_MAX_THUNKS]; thunk++)
    {
        if (thunk->hdc == hdc)  return thunk;
    }
    return NULL;
}

/**********************************************************************
 *           QueryAbort   (GDI.155)
 *
 *  Calls the app's AbortProc function if avail.
 *
 * RETURNS
 * TRUE if no AbortProc avail or AbortProc wants to continue printing.
 * FALSE if AbortProc wants to abort printing.
 */
BOOL16 WINAPI QueryAbort16(HDC16 hdc16, INT16 reserved)
{
    struct gdi_thunk* thunk = GDI_FindThunk(hdc16);

    if (!thunk) {
        ERR("Invalid hdc 0x%x\n", hdc16);
	return FALSE;
    }
    return GDI_Callback3216( thunk->pfn16, HDC_32(hdc16), 0 );
}


/**********************************************************************
 *           SetAbortProc   (GDI.381)
 */
INT16 WINAPI SetAbortProc16(HDC16 hdc16, ABORTPROC16 abrtprc)
{
    struct gdi_thunk*   thunk;

    thunk = GDI_AddThunk(hdc16, abrtprc);
    if (!thunk) return FALSE;
    if (!SetAbortProc(HDC_32( hdc16 ), (ABORTPROC)thunk))
    {
        GDI_DeleteThunk(thunk);
        return FALSE;
    }
    return TRUE;
}

/*
 * ############################################################################
 */

struct callback16_info
{
    FARPROC16 proc;
    LPARAM    param;
    INT       result;
};

/* callback for LineDDA16 */
static void CALLBACK linedda_callback( INT x, INT y, LPARAM param )
{
    const struct callback16_info *info = (struct callback16_info *)param;
    WORD args[4];

    args[3] = x;
    args[2] = y;
    args[1] = HIWORD(info->param);
    args[0] = LOWORD(info->param);
    WOWCallback16Ex( (DWORD)info->proc, WCB16_PASCAL, sizeof(args), args, NULL );
}

/* callback for EnumObjects16 */
static INT CALLBACK enum_pens_callback( void *ptr, LPARAM param )
{
    const struct callback16_info *info = (struct callback16_info *)param;
    LOGPEN *pen = ptr;
    LOGPEN16 pen16;
    SEGPTR segptr;
    DWORD ret;
    WORD args[4];

    pen16.lopnStyle   = pen->lopnStyle;
    pen16.lopnWidth.x = pen->lopnWidth.x;
    pen16.lopnWidth.y = pen->lopnWidth.y;
    pen16.lopnColor   = pen->lopnColor;
    segptr = MapLS( &pen16 );
    args[3] = SELECTOROF(segptr);
    args[2] = OFFSETOF(segptr);
    args[1] = HIWORD(info->param);
    args[0] = LOWORD(info->param);
    WOWCallback16Ex( (DWORD)info->proc, WCB16_PASCAL, sizeof(args), args, &ret );
    UnMapLS( segptr );
    return LOWORD(ret);
}

/* callback for EnumObjects16 */
static INT CALLBACK enum_brushes_callback( void *ptr, LPARAM param )
{
    const struct callback16_info *info = (struct callback16_info *)param;
    LOGBRUSH *brush = ptr;
    LOGBRUSH16 brush16;
    SEGPTR segptr;
    DWORD ret;
    WORD args[4];

    brush16.lbStyle = brush->lbStyle;
    brush16.lbColor = brush->lbColor;
    brush16.lbHatch = brush->lbHatch;
    segptr = MapLS( &brush16 );
    args[3] = SELECTOROF(segptr);
    args[2] = OFFSETOF(segptr);
    args[1] = HIWORD(info->param);
    args[0] = LOWORD(info->param);
    WOWCallback16Ex( (DWORD)info->proc, WCB16_PASCAL, sizeof(args), args, &ret );
    UnMapLS( segptr );
    return ret;
}

static BYTE fix_font_quality(LOGFONT16 *font16)
{
    static BOOL init_enable_antialias;
    static BOOL enable_antialias;
    static BYTE force_font_quality;
    if (!init_enable_antialias)
    {
        init_enable_antialias = TRUE;
        enable_antialias = krnl386_get_config_int("otvdm", "EnableFontAntialias", FALSE);
        force_font_quality = (BYTE)krnl386_get_config_int("otvdm", "ForceFontQuality", -1);
    }

    if (font16->lfWidth && font16->lfHeight)
        return font16->lfQuality;

    if (force_font_quality != (BYTE)-1)
        return force_font_quality;
    if (enable_antialias)
        return font16->lfQuality;
    return NONANTIALIASED_QUALITY;
}

/* convert a LOGFONT16 to a LOGFONTA */
static void logfont_16_to_A( const LOGFONT16 *font16, LPLOGFONTA font32 )
{
    font32->lfHeight = font16->lfHeight;
    font32->lfWidth = font16->lfWidth;
    font32->lfEscapement = font16->lfEscapement;
    font32->lfOrientation = font16->lfOrientation;
    font32->lfWeight = font16->lfWeight;
    font32->lfItalic = font16->lfItalic;
    font32->lfUnderline = font16->lfUnderline;
    font32->lfStrikeOut = font16->lfStrikeOut;
    font32->lfCharSet = fix_font_charset(font16->lfCharSet);
    font32->lfOutPrecision = font16->lfOutPrecision;
    font32->lfClipPrecision = font16->lfClipPrecision;
    font32->lfQuality = fix_font_quality(font16);
    font32->lfPitchAndFamily = font16->lfPitchAndFamily;
    memcpy(font32->lfFaceName, font16->lfFaceName, LF_FACESIZE);
    font32->lfFaceName[LF_FACESIZE-1] = 0;
}

/* convert a LOGFONTA to a LOGFONT16 */
static void logfont_A_to_16( const LOGFONTA* font32, LPLOGFONT16 font16 )
{
    font16->lfHeight = font32->lfHeight;
    font16->lfWidth = font32->lfWidth;
    font16->lfEscapement = font32->lfEscapement;
    font16->lfOrientation = font32->lfOrientation;
    font16->lfWeight = font32->lfWeight;
    font16->lfItalic = font32->lfItalic;
    font16->lfUnderline = font32->lfUnderline;
    font16->lfStrikeOut = font32->lfStrikeOut;
    font16->lfCharSet = font32->lfCharSet;
    font16->lfOutPrecision = font32->lfOutPrecision;
    font16->lfClipPrecision = font32->lfClipPrecision;
    font16->lfQuality = font32->lfQuality;
    font16->lfPitchAndFamily = font32->lfPitchAndFamily;
    memcpy(font16->lfFaceName, font32->lfFaceName, LF_FACESIZE);
    font16->lfFaceName[LF_FACESIZE-1] = 0;
}

/* convert a ENUMLOGFONTEXA to a ENUMLOGFONTEX16 */
static void enumlogfontex_A_to_16( const ENUMLOGFONTEXA *fontA,
                                   LPENUMLOGFONTEX16 font16 )
{
    logfont_A_to_16( (const LOGFONTA *)fontA, (LPLOGFONT16)font16);

    memcpy(font16->elfFullName, fontA->elfFullName, LF_FULLFACESIZE);
    font16->elfFullName[LF_FULLFACESIZE-1] = '\0';
    memcpy(font16->elfStyle, fontA->elfStyle, LF_FACESIZE);
    font16->elfStyle[LF_FACESIZE-1] = '\0';
    memcpy(font16->elfScript, fontA->elfScript, LF_FACESIZE);
    font16->elfScript[LF_FACESIZE-1] = '\0';
}

/* convert a NEWTEXTMETRICEXA to a NEWTEXTMETRICEX16 */
static void newtextmetricex_A_to_16( const NEWTEXTMETRICEXA *ptmA,
                                     LPNEWTEXTMETRICEX16 ptm16 )
{
    ptm16->ntmTm.tmHeight = ptmA->ntmTm.tmHeight;
    ptm16->ntmTm.tmAscent = ptmA->ntmTm.tmAscent;
    ptm16->ntmTm.tmDescent = ptmA->ntmTm.tmDescent;
    ptm16->ntmTm.tmInternalLeading = ptmA->ntmTm.tmInternalLeading;
    ptm16->ntmTm.tmExternalLeading = ptmA->ntmTm.tmExternalLeading;
    ptm16->ntmTm.tmAveCharWidth = ptmA->ntmTm.tmAveCharWidth;
    ptm16->ntmTm.tmMaxCharWidth = ptmA->ntmTm.tmMaxCharWidth;
    ptm16->ntmTm.tmWeight = ptmA->ntmTm.tmWeight;
    ptm16->ntmTm.tmOverhang = ptmA->ntmTm.tmOverhang;
    ptm16->ntmTm.tmDigitizedAspectX = ptmA->ntmTm.tmDigitizedAspectX;
    ptm16->ntmTm.tmDigitizedAspectY = ptmA->ntmTm.tmDigitizedAspectY;
    ptm16->ntmTm.tmFirstChar = ptmA->ntmTm.tmFirstChar;
    ptm16->ntmTm.tmLastChar = ptmA->ntmTm.tmLastChar;
    ptm16->ntmTm.tmDefaultChar = ptmA->ntmTm.tmDefaultChar;
    ptm16->ntmTm.tmBreakChar = ptmA->ntmTm.tmBreakChar;
    ptm16->ntmTm.tmItalic = ptmA->ntmTm.tmItalic;
    ptm16->ntmTm.tmUnderlined = ptmA->ntmTm.tmUnderlined;
    ptm16->ntmTm.tmStruckOut = ptmA->ntmTm.tmStruckOut;
    ptm16->ntmTm.tmPitchAndFamily = ptmA->ntmTm.tmPitchAndFamily;
    ptm16->ntmTm.tmCharSet = ptmA->ntmTm.tmCharSet;
    ptm16->ntmTm.ntmFlags = ptmA->ntmTm.ntmFlags;
    ptm16->ntmTm.ntmSizeEM = ptmA->ntmTm.ntmSizeEM;
    ptm16->ntmTm.ntmCellHeight = ptmA->ntmTm.ntmCellHeight;
    ptm16->ntmTm.ntmAvgWidth = ptmA->ntmTm.ntmAvgWidth;
    ptm16->ntmFontSig = ptmA->ntmFontSig;
}

static const char *font_list[] =
{
    "\x82\x6c\x82\x72 \x83\x53\x83\x56\x83\x62\x83\x4e", /* MS Gothic (Japanese font) */
    "\x82\x6c\x82\x72 \x96\xbe\x92\xa9", /* MS Mincho (Japanese font) */

    "Arial",
    "Courier New",
    "Times New Roman",
    "Wingdings",
    "Symbol",
    "CenturyOldst",
    "Arial Narrow",
    "Book Antiqua",
    "Bookman Old Style",
    "Century Gothic",
    "Century Schoolbook",
    "Monotype Corsiva",
    "Monotype Sorts",
    "Fences",
    "MT Extra",
    "Monotype Sorts",
    "System",
    "FixedSys",
    "Modern",
    "Script",
    "Terminal",
    "Roman",
    "Small Fonts",
    "MS Serif",
    "MS Dialog",
    "MS Sans Serif",
    "MS LineDraw",
    "Century",
    "Algerian",
    "Arial Rounded MT Bold",
    "Braggadocio",
    "Britannic Bold",
    "Brush Script MT",
    "Colonna MT",
    "Desdemona",
    "Footlight MT Light",
    "Impact",
    "Kino MT",
    "Wide Latin",
    "Matura MT Script Capitals",
    "Playbill",
    "Courier",
    "Helvetica",
    "Helv",
    "Times",
    "ITC Avant Garde Gothic",
    "ITC Bookman",
    "New Century Schoolbook",
    "Palatino",
    "ITC Zapf Chancery",
    "ITC Zapf Dingbats",
    "AvantGarde",
    "Bookman",
    "NewCenturySchlbk",
    "Palatino",
    "ZapfChancery",
    "ZapfDingbats",
    "AvantGarde",
    "Bookman",
    "NewCenturySchlbk",
    "Palatino",
    "ZapfChancery",
    "ZapfDingbats",
    NULL
};

static struct list font_allowed_list = LIST_INIT(font_allowed_list);

static struct list font_disallowed_list = LIST_INIT(font_disallowed_list);

struct font_entry
{
    struct list entry;
    const char *font_name;
};

static void construct_allowed_font_list()
{
    int buf_size = 2048;
    LPSTR font = HeapAlloc(GetProcessHeap(), 0, buf_size);
    while (TRUE)
    {
        DWORD size = krnl386_get_config_string("EnumFontLimitation", NULL, "", font, buf_size);
        if (size < buf_size)
        {
            break;
        }
        buf_size *= 2;
        font = HeapReAlloc(GetProcessHeap(), 0, font, buf_size);
    }
    while (TRUE)
    {
        size_t len = strlen(font);
        struct font_entry *elem;
        BOOL allowed;
        if (len == 0)
            break;
        elem = (struct font_entry*)HeapAlloc(GetProcessHeap(), 0, sizeof(*elem));
        allowed = krnl386_get_config_int("EnumFontLimitation", font, 1);
        elem->font_name = font;
        if (allowed)
        {
            list_add_tail(&font_allowed_list, &elem->entry);
        }
        else
        {
            list_add_tail(&font_disallowed_list, &elem->entry);
        }
        font += len + 1;
    }
}

/*
 * callback for EnumFontFamiliesEx16
 * Note: plf is really an ENUMLOGFONTEXA, and ptm is a NEWTEXTMETRICEXA.
 *       We have to use other types because of the FONTENUMPROCA definition.
 */
static INT CALLBACK enum_font_callback( const LOGFONTA *plf,
                                        const TEXTMETRICA *ptm, DWORD fType,
                                        LPARAM param )
{
    struct callback16_info *info = (struct callback16_info *)param;
    ENUMLOGFONTEX16 elfe16;
    NEWTEXTMETRICEX16 ntm16;
    SEGPTR segelfe16;
    SEGPTR segntm16;
    WORD args[7];
    DWORD ret;
    static BOOL enum_font_limitation_init;
    static BOOL enum_font_limitation;

    enumlogfontex_A_to_16((const ENUMLOGFONTEXA *)plf, &elfe16);
    newtextmetricex_A_to_16((const NEWTEXTMETRICEXA *)ptm, &ntm16);
    /* some old programs can not process many fonts(1000+) */
    if (!enum_font_limitation_init)
    {
        enum_font_limitation_init = TRUE;
        enum_font_limitation = krnl386_get_config_int("otvdm", "EnumFontLimitation", FALSE);
        if (enum_font_limitation)
        {
            construct_allowed_font_list();
        }
    }
    if (enum_font_limitation)
    {
        /* TODO: configurable */
        int i;
        BOOL found = FALSE;
        struct font_entry *font;
        BOOL disallow = FALSE;

        LIST_FOR_EACH_ENTRY(font, &font_disallowed_list, struct font_entry, entry)
        {
            if (!stricmp(elfe16.elfLogFont.lfFaceName, font->font_name))
            {
                disallow = TRUE;
                break;
            }
        }
        if (!disallow)
        {
            for (i = 0; font_list[i]; i++)
            {
                if (!stricmp(elfe16.elfLogFont.lfFaceName, font_list[i]))
                {
                    found = TRUE;
                    break;
                }
            }
        }
        if (!found && !disallow)
        {
            LIST_FOR_EACH_ENTRY(font, &font_allowed_list, struct font_entry, entry)
            {
                if (!stricmp(elfe16.elfLogFont.lfFaceName, font->font_name))
                {
                    found = TRUE;
                    break;
                }
            }
        }
        if (!found)
        {
            TRACE("font %s skipped.\n", elfe16.elfLogFont.lfFaceName);
            return info->result;
        }
    }
    segelfe16 = MapLS( &elfe16 );
    segntm16 = MapLS( &ntm16 );
    args[6] = SELECTOROF(segelfe16);
    args[5] = OFFSETOF(segelfe16);
    args[4] = SELECTOROF(segntm16);
    args[3] = OFFSETOF(segntm16);
    args[2] = fType;
    args[1] = HIWORD(info->param);
    args[0] = LOWORD(info->param);

    WOWCallback16Ex( (DWORD)info->proc, WCB16_PASCAL, sizeof(args), args, &ret );
    UnMapLS( segelfe16 );
    UnMapLS( segntm16 );
    return info->result = LOWORD(ret);
}

struct dib_segptr_bits
{
    struct list entry;
    HBITMAP16 bmp;
    WORD      sel;
    WORD      count;
};

static struct list dib_segptr_list = LIST_INIT( dib_segptr_list );

static SEGPTR alloc_segptr_bits( HBITMAP bmp, void *bits32 )
{
    DIBSECTION dib;
    unsigned int i, size;
    struct dib_segptr_bits *bits;

    if (!(bits = HeapAlloc( GetProcessHeap(), 0, sizeof(*bits) ))) return 0;

    GetObjectW( bmp, sizeof(dib), &dib );
    size = dib.dsBm.bmHeight * dib.dsBm.bmWidthBytes;

    /* calculate number of sel's needed for size with 64K steps */
    bits->bmp   = HBITMAP_16( bmp );
    bits->count = (size + 0xffff) / 0x10000;
    bits->sel   = AllocSelectorArray16( bits->count );
    GlobalMapInternal(bits->sel, bits32, size);

    for (i = 0; i < bits->count; i++)
    {
        SetSelectorBase(bits->sel + (i << __AHSHIFT), (DWORD)bits32 + i * 0x10000);
        SetSelectorLimit16(bits->sel + (i << __AHSHIFT), size - 1); /* yep, limit is correct */
        size -= 0x10000;
    }
    list_add_head( &dib_segptr_list, &bits->entry );
    return MAKESEGPTR( bits->sel, 0 );
}

static void free_segptr_bits( HBITMAP16 bmp )
{
    unsigned int i;
    struct dib_segptr_bits *bits;

    LIST_FOR_EACH_ENTRY( bits, &dib_segptr_list, struct dib_segptr_bits, entry )
    {
        if (bits->bmp != bmp) continue;
        for (i = 0; i < bits->count; i++) FreeSelector16( bits->sel + (i << __AHSHIFT) );

        GlobalMapInternal(bits->sel, NULL, 0);
        list_remove( &bits->entry );
        HeapFree( GetProcessHeap(), 0, bits );
        return;
    }
}

#if 0
/* window surface used to implement the DIB.DRV driver */

struct dib_window_surface
{
    struct window_surface header;
    RECT                  bounds;
    void                 *bits;
    UINT                  info_size;
    BITMAPINFO            info;   /* variable size, must be last */
};

static struct dib_window_surface *get_dib_surface( struct window_surface *surface )
{
    return (struct dib_window_surface *)surface;
}

/***********************************************************************
 *           dib_surface_lock
 */
static void dib_surface_lock( struct window_surface *window_surface )
{
    /* nothing to do */
}

/***********************************************************************
 *           dib_surface_unlock
 */
static void dib_surface_unlock( struct window_surface *window_surface )
{
    /* nothing to do */
}

/***********************************************************************
 *           dib_surface_get_bitmap_info
 */
static void *dib_surface_get_bitmap_info( struct window_surface *window_surface, BITMAPINFO *info )
{
    struct dib_window_surface *surface = get_dib_surface( window_surface );

    memcpy( info, &surface->info, surface->info_size );
    return surface->bits;
}

/***********************************************************************
 *           dib_surface_get_bounds
 */
static RECT *dib_surface_get_bounds( struct window_surface *window_surface )
{
    struct dib_window_surface *surface = get_dib_surface( window_surface );

    return &surface->bounds;
}

/***********************************************************************
 *           dib_surface_set_region
 */
static void dib_surface_set_region( struct window_surface *window_surface, HRGN region )
{
    /* nothing to do */
}

/***********************************************************************
 *           dib_surface_flush
 */
static void dib_surface_flush( struct window_surface *window_surface )
{
    /* nothing to do */
}

/***********************************************************************
 *           dib_surface_destroy
 */
static void dib_surface_destroy( struct window_surface *window_surface )
{
    struct dib_window_surface *surface = get_dib_surface( window_surface );

    TRACE( "freeing %p\n", surface );
    HeapFree( GetProcessHeap(), 0, surface );
}

static const struct window_surface_funcs dib_surface_funcs =
{
    dib_surface_lock,
    dib_surface_unlock,
    dib_surface_get_bitmap_info,
    dib_surface_get_bounds,
    dib_surface_set_region,
    dib_surface_flush,
    dib_surface_destroy
};

/***********************************************************************
 *           create_surface
 */
static struct window_surface *create_surface( const BITMAPINFO *info )
{
    struct dib_window_surface *surface;
    int color = 0;

    if (info->bmiHeader.biBitCount <= 8)
        color = info->bmiHeader.biClrUsed ? info->bmiHeader.biClrUsed : (1 << info->bmiHeader.biBitCount);
    else if (info->bmiHeader.biCompression == BI_BITFIELDS)
        color = 3;

    surface = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY,
                         offsetof( struct dib_window_surface, info.bmiColors[color] ));
    if (!surface) return NULL;

    surface->header.funcs       = &dib_surface_funcs;
    surface->header.rect.left   = 0;
    surface->header.rect.top    = 0;
    surface->header.rect.right  = info->bmiHeader.biWidth;
    surface->header.rect.bottom = abs(info->bmiHeader.biHeight);
    surface->header.ref         = 1;
    surface->info_size          = offsetof( BITMAPINFO, bmiColors[color] );
    surface->bits               = (char *)info + surface->info_size;
    memcpy( &surface->info, info, surface->info_size );

    TRACE( "created %p %ux%u for info %p bits %p\n",
           surface, surface->header.rect.right, surface->header.rect.bottom, info, surface->bits );
    return &surface->header;
}
#endif


/***********************************************************************
 *           SetBkColor    (GDI.1)
 */
COLORREF WINAPI SetBkColor16( HDC16 hdc, COLORREF color )
{
    return SetBkColor( HDC_32(hdc), check_colorref(color) );
}


/***********************************************************************
 *		SetBkMode (GDI.2)
 */
INT16 WINAPI SetBkMode16( HDC16 hdc, INT16 mode )
{
    return SetBkMode( HDC_32(hdc), mode );
}


/***********************************************************************
 *           SetMapMode    (GDI.3)
 */
INT16 WINAPI SetMapMode16( HDC16 hdc, INT16 mode )
{
    return SetMapMode( HDC_32(hdc), mode );
}


/***********************************************************************
 *		SetROP2	(GDI.4)
 */
INT16 WINAPI SetROP216( HDC16 hdc, INT16 mode )
{
    return SetROP2( HDC_32(hdc), mode );
}


INT WINAPI SetRelAbs(HDC 	hdc,
	INT 	Mode
	);
/***********************************************************************
 *		SetRelAbs (GDI.5)
 */
INT16 WINAPI SetRelAbs16( HDC16 hdc, INT16 mode )
{
    return SetRelAbs( HDC_32(hdc), mode );
}


/***********************************************************************
 *		SetPolyFillMode	(GDI.6)
 */
INT16 WINAPI SetPolyFillMode16( HDC16 hdc, INT16 mode )
{
    return SetPolyFillMode( HDC_32(hdc), mode );
}


/***********************************************************************
 *		SetStretchBltMode (GDI.7)
 */
INT16 WINAPI SetStretchBltMode16( HDC16 hdc, INT16 mode )
{
    return SetStretchBltMode( HDC_32(hdc), mode );
}


/***********************************************************************
 *           SetTextCharacterExtra    (GDI.8)
 */
INT16 WINAPI SetTextCharacterExtra16( HDC16 hdc, INT16 extra )
{
    return SetTextCharacterExtra( HDC_32(hdc), extra );
}


/***********************************************************************
 *           SetTextColor    (GDI.9)
 */
COLORREF WINAPI SetTextColor16( HDC16 hdc, COLORREF color )
{
    return SetTextColor( HDC_32(hdc), check_colorref(color) );
}


/***********************************************************************
 *           SetTextJustification    (GDI.10)
 */
INT16 WINAPI SetTextJustification16( HDC16 hdc, INT16 extra, INT16 breaks )
{
    return SetTextJustification( HDC_32(hdc), extra, breaks );
}


/***********************************************************************
 *           SetWindowOrg    (GDI.11)
 */
DWORD WINAPI SetWindowOrg16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT pt;
    if (!SetWindowOrgEx( HDC_32(hdc), x, y, &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *           SetWindowExt    (GDI.12)
 */
DWORD WINAPI SetWindowExt16( HDC16 hdc, INT16 x, INT16 y )
{
    SIZE size;
    if (!SetWindowExtEx( HDC_32(hdc), x, y, &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *           SetViewportOrg    (GDI.13)
 */
DWORD WINAPI SetViewportOrg16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT pt;
    if (!SetViewportOrgEx( HDC_32(hdc), x, y, &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *           SetViewportExt    (GDI.14)
 */
DWORD WINAPI SetViewportExt16( HDC16 hdc, INT16 x, INT16 y )
{
    SIZE size;
    if (!SetViewportExtEx( HDC_32(hdc), x, y, &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *           OffsetWindowOrg    (GDI.15)
 */
DWORD WINAPI OffsetWindowOrg16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT pt;
    if (!OffsetWindowOrgEx( HDC_32(hdc), x, y, &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *           ScaleWindowExt    (GDI.16)
 */
DWORD WINAPI ScaleWindowExt16( HDC16 hdc, INT16 xNum, INT16 xDenom,
                             INT16 yNum, INT16 yDenom )
{
    SIZE size;
    if (!ScaleWindowExtEx( HDC_32(hdc), xNum, xDenom, yNum, yDenom, &size ))
        return FALSE;
    return MAKELONG( size.cx,  size.cy );
}


/***********************************************************************
 *           OffsetViewportOrg    (GDI.17)
 */
DWORD WINAPI OffsetViewportOrg16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT pt;
    if (!OffsetViewportOrgEx( HDC_32(hdc), x, y, &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *           ScaleViewportExt    (GDI.18)
 */
DWORD WINAPI ScaleViewportExt16( HDC16 hdc, INT16 xNum, INT16 xDenom,
                               INT16 yNum, INT16 yDenom )
{
    SIZE size;
    if (!ScaleViewportExtEx( HDC_32(hdc), xNum, xDenom, yNum, yDenom, &size ))
        return FALSE;
    return MAKELONG( size.cx,  size.cy );
}


/***********************************************************************
 *           LineTo    (GDI.19)
 */
BOOL16 WINAPI LineTo16( HDC16 hdc, INT16 x, INT16 y )
{
    return LineTo( HDC_32(hdc), x, y );
}


/***********************************************************************
 *           MoveTo    (GDI.20)
 */
DWORD WINAPI MoveTo16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT pt;

    if (!MoveToEx( HDC_32(hdc), x, y, &pt )) return 0;
    return MAKELONG(pt.x,pt.y);
}


/***********************************************************************
 *           ExcludeClipRect    (GDI.21)
 */
INT16 WINAPI ExcludeClipRect16( HDC16 hdc, INT16 left, INT16 top,
                                INT16 right, INT16 bottom )
{
    return ExcludeClipRect( HDC_32(hdc), left, top, right, bottom );
}


/***********************************************************************
 *           IntersectClipRect    (GDI.22)
 */
INT16 WINAPI IntersectClipRect16( HDC16 hdc, INT16 left, INT16 top,
                                  INT16 right, INT16 bottom )
{
    return IntersectClipRect( HDC_32(hdc), left, top, right, bottom );
}


/***********************************************************************
 *           Arc    (GDI.23)
 */
BOOL16 WINAPI Arc16( HDC16 hdc, INT16 left, INT16 top, INT16 right,
                     INT16 bottom, INT16 xstart, INT16 ystart,
                     INT16 xend, INT16 yend )
{
    return Arc( HDC_32(hdc), left, top, right, bottom, xstart, ystart, xend, yend );
}


/***********************************************************************
 *           Ellipse    (GDI.24)
 */
BOOL16 WINAPI Ellipse16( HDC16 hdc, INT16 left, INT16 top,
                         INT16 right, INT16 bottom )
{
    return Ellipse( HDC_32(hdc), left, top, right, bottom );
}


/**********************************************************************
 *          FloodFill   (GDI.25)
 */
BOOL16 WINAPI FloodFill16( HDC16 hdc, INT16 x, INT16 y, COLORREF color )
{
    return ExtFloodFill( HDC_32(hdc), x, y, check_colorref(color), FLOODFILLBORDER );
}


/***********************************************************************
 *           Pie    (GDI.26)
 */
BOOL16 WINAPI Pie16( HDC16 hdc, INT16 left, INT16 top,
                     INT16 right, INT16 bottom, INT16 xstart, INT16 ystart,
                     INT16 xend, INT16 yend )
{
    return Pie( HDC_32(hdc), left, top, right, bottom, xstart, ystart, xend, yend );
}


/***********************************************************************
 *           Rectangle    (GDI.27)
 */
BOOL16 WINAPI Rectangle16( HDC16 hdc, INT16 left, INT16 top,
                           INT16 right, INT16 bottom )
{
    return Rectangle( HDC_32(hdc), left, top, right, bottom );
}


/***********************************************************************
 *           RoundRect    (GDI.28)
 */
BOOL16 WINAPI RoundRect16( HDC16 hdc, INT16 left, INT16 top, INT16 right,
                           INT16 bottom, INT16 ell_width, INT16 ell_height )
{
    return RoundRect( HDC_32(hdc), left, top, right, bottom, ell_width, ell_height );
}


/***********************************************************************
 *           PatBlt    (GDI.29)
 */
BOOL16 WINAPI PatBlt16( HDC16 hdc, INT16 left, INT16 top,
                        INT16 width, INT16 height, DWORD rop)
{
    return PatBlt( HDC_32(hdc), left, top, width, height, rop );
}


/***********************************************************************
 *           SaveDC    (GDI.30)
 */
INT16 WINAPI SaveDC16( HDC16 hdc )
{
    return SaveDC( HDC_32(hdc) );
}


/***********************************************************************
 *           SetPixel    (GDI.31)
 */
COLORREF WINAPI SetPixel16( HDC16 hdc, INT16 x, INT16 y, COLORREF color )
{
    return SetPixel( HDC_32(hdc), x, y, check_colorref(color) );
}


/***********************************************************************
 *           OffsetClipRgn    (GDI.32)
 */
INT16 WINAPI OffsetClipRgn16( HDC16 hdc, INT16 x, INT16 y )
{
    return OffsetClipRgn( HDC_32(hdc), x, y );
}


/***********************************************************************
 *           TextOut    (GDI.33)
 */
BOOL16 WINAPI TextOut16( HDC16 hdc, INT16 x, INT16 y, LPCSTR str, INT16 count )
{
    return TextOutA( HDC_32(hdc), x, y, str, count );
}


/***********************************************************************
 *           BitBlt    (GDI.34)
 */
BOOL16 WINAPI BitBlt16( HDC16 hdcDst, INT16 xDst, INT16 yDst, INT16 width,
                        INT16 height, HDC16 hdcSrc, INT16 xSrc, INT16 ySrc,
                        DWORD rop )
{
    return BitBlt( HDC_32(hdcDst), xDst, yDst, width, height, HDC_32(hdcSrc), xSrc, ySrc, rop );
}


/***********************************************************************
 *           StretchBlt    (GDI.35)
 */
BOOL16 WINAPI StretchBlt16( HDC16 hdcDst, INT16 xDst, INT16 yDst,
                            INT16 widthDst, INT16 heightDst,
                            HDC16 hdcSrc, INT16 xSrc, INT16 ySrc,
                            INT16 widthSrc, INT16 heightSrc, DWORD rop )
{
    return StretchBlt( HDC_32(hdcDst), xDst, yDst, widthDst, heightDst,
                       HDC_32(hdcSrc), xSrc, ySrc, widthSrc, heightSrc, rop );
}


/**********************************************************************
 *          Polygon  (GDI.36)
 */
BOOL16 WINAPI Polygon16( HDC16 hdc, const POINT16* pt, INT16 count )
{
    int i;
    BOOL ret;
    LPPOINT pt32 = HeapAlloc( GetProcessHeap(), 0, count*sizeof(POINT) );

    if (!pt32) return FALSE;
    for (i=count;i--;)
    {
        pt32[i].x = pt[i].x;
        pt32[i].y = pt[i].y;
    }
    ret = Polygon(HDC_32(hdc),pt32,count);
    HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/**********************************************************************
 *          Polyline  (GDI.37)
 */
BOOL16 WINAPI Polyline16( HDC16 hdc, const POINT16* pt, INT16 count )
{
    int i;
    BOOL16 ret;
    LPPOINT pt32 = HeapAlloc( GetProcessHeap(), 0, count*sizeof(POINT) );

    if (!pt32) return FALSE;
    for (i=count;i--;)
    {
        pt32[i].x = pt[i].x;
        pt32[i].y = pt[i].y;
    }
    ret = Polyline(HDC_32(hdc),pt32,count);
    HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/***********************************************************************
 *            Escape   (GDI.38)
 */
INT16 WINAPI Escape16( HDC16 hdc, INT16 escape, INT16 in_count, SEGPTR in_data, LPVOID out_data )
{
    INT ret;

    switch(escape)
    {
    /* Escape(hdc,CLIP_TO_PATH,LPINT16,NULL) */
    /* Escape(hdc,DRAFTMODE,LPINT16,NULL) */
    /* Escape(hdc,ENUMPAPERBINS,LPINT16,LPSTR); */
    /* Escape(hdc,EPSPRINTING,LPINT16,NULL) */
    /* Escape(hdc,EXT_DEVICE_CAPS,LPINT16,LPDWORD) */
    /* Escape(hdc,GETCOLORTABLE,LPINT16,LPDWORD) */
    /* Escape(hdc,MOUSETRAILS,LPINT16,NULL) */
    /* Escape(hdc,POSTSCRIPT_IGNORE,LPINT16,NULL) */
    /* Escape(hdc,QUERYESCSUPPORT,LPINT16,NULL) */
    /* Escape(hdc,SET_ARC_DIRECTION,LPINT16,NULL) */
    /* Escape(hdc,SET_POLY_MODE,LPINT16,NULL) */
    /* Escape(hdc,SET_SCREEN_ANGLE,LPINT16,NULL) */
    /* Escape(hdc,SET_SPREAD,LPINT16,NULL) */
    case CLIP_TO_PATH:
    case DRAFTMODE:
    case ENUMPAPERBINS:
    case EPSPRINTING:
    case EXT_DEVICE_CAPS:
    case GETCOLORTABLE:
    case MOUSETRAILS:
    case POSTSCRIPT_IGNORE:
    case QUERYESCSUPPORT:
    case SET_ARC_DIRECTION:
    case SET_POLY_MODE:
    case SET_SCREEN_ANGLE:
    case SET_SPREAD:
    {
        INT16 *ptr = MapSL(in_data);
        INT data = *ptr;
        return Escape( HDC_32(hdc), escape, sizeof(data), (LPCSTR)&data, out_data );
    }

    /* Escape(hdc,ENABLEDUPLEX,LPUINT16,NULL) */
    case ENABLEDUPLEX:
    {
        UINT16 *ptr = MapSL(in_data);
        UINT data = *ptr;
        return Escape( HDC_32(hdc), escape, sizeof(data), (LPCSTR)&data, NULL );
    }

    /* Escape(hdc,GETPHYSPAGESIZE,NULL,LPPOINT16) */
    /* Escape(hdc,GETPRINTINGOFFSET,NULL,LPPOINT16) */
    /* Escape(hdc,GETSCALINGFACTOR,NULL,LPPOINT16) */
    case GETPHYSPAGESIZE:
    case GETPRINTINGOFFSET:
    case GETSCALINGFACTOR:
    {
        POINT16 *ptr = out_data;
        POINT pt32;
        ret = Escape( HDC_32(hdc), escape, 0, NULL, &pt32 );
        ptr->x = pt32.x;
        ptr->y = pt32.y;
        return ret;
    }

    /* Escape(hdc,ENABLEPAIRKERNING,LPINT16,LPINT16); */
    /* Escape(hdc,ENABLERELATIVEWIDTHS,LPINT16,LPINT16); */
    /* Escape(hdc,SETCOPYCOUNT,LPINT16,LPINT16) */
    /* Escape(hdc,SETKERNTRACK,LPINT16,LPINT16) */
    /* Escape(hdc,SETLINECAP,LPINT16,LPINT16) */
    /* Escape(hdc,SETLINEJOIN,LPINT16,LPINT16) */
    /* Escape(hdc,SETMITERLIMIT,LPINT16,LPINT16) */
    case ENABLEPAIRKERNING:
    case ENABLERELATIVEWIDTHS:
    case SETCOPYCOUNT:
    case SETKERNTRACK:
    case SETLINECAP:
    case SETLINEJOIN:
    case SETMITERLIMIT:
    case SETCHARSET:
    {
        INT16 *new = MapSL(in_data);
        INT16 *old = out_data;
        INT out, in = *new;
        ret = Escape( HDC_32(hdc), escape, sizeof(in), (LPCSTR)&in, &out );
        if (old)
            *old = out;
        return ret;
    }

    /* Escape(hdc,SETABORTPROC,ABORTPROC,NULL); */
    case SETABORTPROC:
        return SetAbortProc16( hdc, (ABORTPROC16)in_data );

    /* Escape(hdc,STARTDOC,LPSTR,LPDOCINFO16);
     * lpvOutData is actually a pointer to the DocInfo structure and used as
     * a second input parameter */
    case STARTDOC:
        if (out_data)
        {
            ret = StartDoc16( hdc, out_data );
            if (ret > 0) ret = StartPage( HDC_32(hdc) );
            return ret;
        }
        return Escape( HDC_32(hdc), escape, in_count, MapSL(in_data), NULL );

    /* Escape(hdc,SET_BOUNDS,LPRECT16,NULL); */
    /* Escape(hdc,SET_CLIP_BOX,LPRECT16,NULL); */
    case SET_BOUNDS:
    case SET_CLIP_BOX:
    {
        RECT16 *rc16 = MapSL(in_data);
        RECT rc;
        rc.left   = rc16->left;
        rc.top    = rc16->top;
        rc.right  = rc16->right;
        rc.bottom = rc16->bottom;
        return Escape( HDC_32(hdc), escape, sizeof(rc), (LPCSTR)&rc, NULL );
    }

    /* Escape(hdc,NEXTBAND,NULL,LPRECT16); */
    case NEXTBAND:
    {
        RECT rc;
        RECT16 *rc16 = out_data;
        ret = Escape( HDC_32(hdc), escape, 0, NULL, &rc );
        rc16->left   = rc.left;
        rc16->top    = rc.top;
        rc16->right  = rc.right;
        rc16->bottom = rc.bottom;
        return ret;
    }
    /* Escape(hdc,DRAWPATTERNRECT,PRECT_STRUCT*,NULL); */
    case DRAWPATTERNRECT:
    {
        DRAWPATRECT pr;
        DRAWPATRECT16 *pr16 = MapSL(in_data);

        pr.ptPosition.x = pr16->ptPosition.x;
        pr.ptPosition.y = pr16->ptPosition.y;
        pr.ptSize.x	= pr16->ptSize.x;
        pr.ptSize.y	= pr16->ptSize.y;
        pr.wStyle	= pr16->wStyle;
        pr.wPattern	= pr16->wPattern;
        return Escape( HDC_32(hdc), escape, sizeof(pr), (LPCSTR)&pr, NULL );
    }

    /* Escape(hdc,ABORTDOC,NULL,NULL); */
    /* Escape(hdc,BANDINFO,BANDINFOSTRUCT*,BANDINFOSTRUCT*); */
    /* Escape(hdc,BEGIN_PATH,NULL,NULL); */
    /* Escape(hdc,ENDDOC,NULL,NULL); */
    /* Escape(hdc,END_PATH,PATHINFO,NULL); */
    /* Escape(hdc,EXTTEXTOUT,EXTTEXT_STRUCT*,NULL); */
    /* Escape(hdc,FLUSHOUTPUT,NULL,NULL); */
    /* Escape(hdc,GETFACENAME,NULL,LPSTR); */
    /* Escape(hdc,GETPAIRKERNTABLE,NULL,KERNPAIR*); */
    /* Escape(hdc,GETSETPAPERBINS,BinInfo*,BinInfo*); */
    /* Escape(hdc,GETSETPRINTORIENT,ORIENT*,NULL); */
    /* Escape(hdc,GETSETSCREENPARAMS,SCREENPARAMS*,SCREENPARAMS*); */
    /* Escape(hdc,GETTECHNOLOGY,NULL,LPSTR); */
    /* Escape(hdc,GETTRACKKERNTABLE,NULL,KERNTRACK*); */
    /* Escape(hdc,MFCOMMENT,LPSTR,NULL); */
    /* Escape(hdc,NEWFRAME,NULL,NULL); */
    /* Escape(hdc,PASSTHROUGH,LPSTR,NULL); */
    /* Escape(hdc,RESTORE_CTM,NULL,NULL); */
    /* Escape(hdc,SAVE_CTM,NULL,NULL); */
    /* Escape(hdc,SETALLJUSTVALUES,EXTTEXTDATA*,NULL); */
    /* Escape(hdc,SETCOLORTABLE,COLORTABLE_STRUCT*,LPDWORD); */
    /* Escape(hdc,SET_BACKGROUND_COLOR,LPDWORD,LPDWORD); */
    /* Escape(hdc,TRANSFORM_CTM,LPSTR,NULL); */
    case ABORTDOC:
    case BANDINFO:
    case BEGIN_PATH:
    case ENDDOC:
    case END_PATH:
    case EXTTEXTOUT:
    case FLUSHOUTPUT:
    case GETFACENAME:
    case GETPAIRKERNTABLE:
    case GETSETPAPERBINS:
    case GETSETPRINTORIENT:
    case GETSETSCREENPARAMS:
    case GETTECHNOLOGY:
    case GETTRACKKERNTABLE:
    case MFCOMMENT:
    case NEWFRAME:
    case PASSTHROUGH:
    case RESTORE_CTM:
    case SAVE_CTM:
    case SETALLJUSTVALUES:
    case SETCOLORTABLE:
    case SET_BACKGROUND_COLOR:
    case TRANSFORM_CTM:
        /* pass it unmodified to the 32-bit function */
        return Escape( HDC_32(hdc), escape, in_count, MapSL(in_data), out_data );

    /* Escape(hdc,ENUMPAPERMETRICS,LPINT16,LPRECT16); */
    /* Escape(hdc,GETEXTENDEDTEXTMETRICS,LPUINT16,EXTTEXTMETRIC*); */
    /* Escape(hdc,GETEXTENTTABLE,LPSTR,LPINT16); */
    /* Escape(hdc,GETSETPAPERMETRICS,LPRECT16,LPRECT16); */
    /* Escape(hdc,GETVECTORBRUSHSIZE,LPLOGBRUSH16,LPPOINT16); */
    /* Escape(hdc,GETVECTORPENSIZE,LPLOGPEN16,LPPOINT16); */
    case ENUMPAPERMETRICS:
    case GETEXTENDEDTEXTMETRICS:
    case GETEXTENTTABLE:
    case GETSETPAPERMETRICS:
    case GETVECTORBRUSHSIZE:
    case GETVECTORPENSIZE:
    default:
        FIXME("unknown/unsupported 16-bit escape %x (%d,%p,%p)\n",
              escape, in_count, MapSL(in_data), out_data );
        return Escape( HDC_32(hdc), escape, in_count, MapSL(in_data), out_data );
    case 0xA1D:
    {
        /* get EUDC font size */
        TEXTMETRICW tm;
        SIZE16 *size = (SIZE16*)out_data;
        if (GetTextMetricsW(HDC_32(hdc), &tm))
        {
            size->cx = size->cy = tm.tmHeight;
            return TRUE;
        }
        return FALSE;
    }
    }
}


/***********************************************************************
 *           RestoreDC    (GDI.39)
 */
BOOL16 WINAPI RestoreDC16( HDC16 hdc, INT16 level )
{
    return RestoreDC( HDC_32(hdc), level );
}


/***********************************************************************
 *           FillRgn    (GDI.40)
 */
BOOL16 WINAPI FillRgn16( HDC16 hdc, HRGN16 hrgn, HBRUSH16 hbrush )
{
    return FillRgn( HDC_32(hdc), HRGN_32(hrgn), HBRUSH_32(hbrush) );
}


/***********************************************************************
 *           FrameRgn     (GDI.41)
 */
BOOL16 WINAPI FrameRgn16( HDC16 hdc, HRGN16 hrgn, HBRUSH16 hbrush,
                          INT16 nWidth, INT16 nHeight )
{
    return FrameRgn( HDC_32(hdc), HRGN_32(hrgn), HBRUSH_32(hbrush), nWidth, nHeight );
}


/***********************************************************************
 *           InvertRgn    (GDI.42)
 */
BOOL16 WINAPI InvertRgn16( HDC16 hdc, HRGN16 hrgn )
{
    return InvertRgn( HDC_32(hdc), HRGN_32(hrgn) );
}


/***********************************************************************
 *           PaintRgn    (GDI.43)
 */
BOOL16 WINAPI PaintRgn16( HDC16 hdc, HRGN16 hrgn )
{
    return PaintRgn( HDC_32(hdc), HRGN_32(hrgn) );
}


/***********************************************************************
 *           SelectClipRgn    (GDI.44)
 */
INT16 WINAPI SelectClipRgn16( HDC16 hdc, HRGN16 hrgn )
{
    return SelectClipRgn( HDC_32(hdc), HRGN_32(hrgn) );
}


/*
 * Prevent the GDI object selected by DC from being deleted.
 * GDI32 fails to delete selected objects as documented, but sometimes it seems
 * to be able to delete it.
 * GetCurrentObject returns the selected object, but it is unreliable
 * because sometimes it returns zero. (metafile DC?)
 */

struct hdc_selected_object
{
    struct list entry;
    HDC hdc;
    HGDIOBJ selected_objects[GDI_OBJ_LAST + 1];
};

static struct list hdc_selected_objects = LIST_INIT(hdc_selected_objects);

void delete_dc_entry(HDC hdc32)
{
    struct hdc_selected_object *dc, *next;
    LIST_FOR_EACH_ENTRY_SAFE(dc, next, &hdc_selected_objects, struct hdc_selected_object, entry)
    {
        if (dc->hdc == hdc32)
        {
            list_remove(&dc->entry);
            HeapFree(GetProcessHeap(), 0, dc);
        }
    }
}

/***********************************************************************
 *           SelectObject    (GDI.45)
 */
HGDIOBJ16 WINAPI SelectObject16( HDC16 hdc, HGDIOBJ16 handle )
{
    HDC hdc32 = HDC_32(hdc);
    HGDIOBJ handle32 = HGDIOBJ_32(handle);
    struct hdc_selected_object *entry, *dc = NULL;
    HGDIOBJ result = SelectObject( hdc32, handle32 );
    DWORD type = GetObjectType(handle32);
    DWORD dc_type = GetObjectType(hdc32);
    if (dc_type)
    {
        LIST_FOR_EACH_ENTRY(entry, &hdc_selected_objects, struct hdc_selected_object, entry)
        {
            if (entry->hdc == hdc32)
            {
                dc = entry;
                break;
            }
        }
        if (!dc)
        {
            dc = (struct hdc_selected_object*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct hdc_selected_object));
            dc->hdc = hdc32;
            list_add_head(&hdc_selected_objects, &dc->entry);
        }
        if (type >= 1 && type <= GDI_OBJ_LAST)
        {
            dc->selected_objects[type] = handle32;
        }
    }
    return HGDIOBJ_16( result );
}


/***********************************************************************
 *           CombineRgn    (GDI.47)
 */
INT16 WINAPI CombineRgn16(HRGN16 hDest, HRGN16 hSrc1, HRGN16 hSrc2, INT16 mode)
{
    return CombineRgn( HRGN_32(hDest), HRGN_32(hSrc1), HRGN_32(hSrc2), mode );
}


/***********************************************************************
 *           CreateBitmap    (GDI.48)
 */
HBITMAP16 WINAPI CreateBitmap16( INT16 width, INT16 height, UINT16 planes,
                                 UINT16 bpp, LPCVOID bits )
{
    if (krnl386_get_compat_mode("256color") && (bpp == 8) && (planes == 1))
    {
        HDC dc = GetDC(NULL);
        HBITMAP ret = CreateCompatibleBitmap(dc, width, height);
        if (bits)
        {
            BITMAPINFO *bmap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256*2 + sizeof(BITMAPINFOHEADER));
            UINT16 *colors = (UINT16 *)bmap->bmiColors;
            VOID *section;
            bmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmap->bmiHeader.biWidth = width;
            bmap->bmiHeader.biHeight = -height;
            bmap->bmiHeader.biPlanes = 1;
            bmap->bmiHeader.biBitCount = 8;
            for (int i = 0; i < 256; i++)
                colors[i] = i;
            SetDIBits(dc, ret, 0, height, bits, bmap, DIB_PAL_COLORS);
            HeapFree(GetProcessHeap(), 0, bmap);
        }
        ReleaseDC(NULL, dc);
        return HBITMAP_16(ret);
    }
    else if ((planes == 1) && ((bpp > 8)))
    {
        HDC dc = GetDC(NULL);
        if (bpp == GetDeviceCaps(dc, BITSPIXEL))
        {
            BITMAPINFO bmpinfo = {0};
            VOID *bmp;
            HDC16 dc16 = HDC_16(dc);
            bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmpinfo.bmiHeader.biWidth = width;
            bmpinfo.bmiHeader.biHeight = -height;
            bmpinfo.bmiHeader.biPlanes = 1;
            bmpinfo.bmiHeader.biBitCount = bpp;
            HBITMAP16 ret = CreateDIBSection16(dc16, &bmpinfo, DIB_RGB_COLORS, &bmp, NULL, NULL);
            if (bits)
                SetDIBits(dc, HBITMAP_32(ret), 0, height, bits, &bmpinfo, DIB_RGB_COLORS);
            ReleaseDC(NULL, dc);
            return ret;
        }
        ReleaseDC(NULL, dc);
    }

    return HBITMAP_16( CreateBitmap( width, height, planes & 0xff, bpp & 0xff, bits ) );
}


/***********************************************************************
 *           CreateBitmapIndirect    (GDI.49)
 */
HBITMAP16 WINAPI CreateBitmapIndirect16( const BITMAP16 * bmp )
{
    return CreateBitmap16( bmp->bmWidth, bmp->bmHeight, bmp->bmPlanes,
                           bmp->bmBitsPixel, MapSL( bmp->bmBits ) );
}


/***********************************************************************
 *           CreateBrushIndirect    (GDI.50)
 */
HBRUSH16 WINAPI CreateBrushIndirect16( const LOGBRUSH16 * brush )
{
    LOGBRUSH brush32;

    if (brush->lbStyle == BS_DIBPATTERN || brush->lbStyle == BS_DIBPATTERN8X8)
        return CreateDIBPatternBrush16( brush->lbHatch, brush->lbColor );

    if (brush->lbStyle == BS_NULL)
        return GetStockObject16(NULL_BRUSH);

    brush32.lbStyle = brush->lbStyle;
    brush32.lbColor = brush->lbColor;
    if (brush->lbStyle == BS_PATTERN)
    {
        brush32.lbHatch = HBITMAP_32(brush->lbHatch);
    }
    else
        brush32.lbHatch = brush->lbHatch;
    return HBRUSH_16( CreateBrushIndirect(&brush32) );
}


/***********************************************************************
 *           CreateCompatibleBitmap    (GDI.51)
 */
HBITMAP16 WINAPI CreateCompatibleBitmap16( HDC16 hdc, INT16 width, INT16 height )
{
    return HBITMAP_16( CreateCompatibleBitmap( HDC_32(hdc), width, height ) );
}

BOOL16 WINAPI IsOldWindowsTask(HINSTANCE16 hInst);
BYTE get_aflags(HMODULE16 hModule);
/***********************************************************************
 *           CreateCompatibleDC    (GDI.52)
 */
HDC16 WINAPI CreateCompatibleDC16( HDC16 hdc )
{
    HDC hdc32 = CreateCompatibleDC( HDC_32(hdc) );
    if (IsOldWindowsTask(GetCurrentTask()) && !(get_aflags(GetExePtr(GetCurrentTask())) & NE_AFLAGS_WIN2_PROTMODE) && (GetCurrentObject(hdc32, OBJ_FONT) == GetStockObject(SYSTEM_FONT)))
        SelectObject(hdc32, GetStockObject(SYSTEM_FIXED_FONT));
    return HDC_16(hdc32);
}


void add_dib_driver_entry(HBITMAP bitmap, HDC dc, HANDLE hSection, LPVOID bits, WORD selector, DWORD size, DWORD padding)
{
    struct dib_driver *drv = (struct dib_driver*)HeapAlloc(GetProcessHeap(), 0, sizeof(struct dib_driver));
    drv->bitmap = bitmap;
    drv->hdc = dc;
    drv->hSection = hSection;
    drv->map = bits;
    drv->selector = selector;
    drv->size = size;
    drv->padding = padding;
    list_add_head(&dib_drivers, &drv->entry);
}
struct dib_driver *find_dib_driver(WORD selector)
{
    struct dib_driver *dib, *next;
    LIST_FOR_EACH_ENTRY_SAFE(dib, next, &dib_drivers, struct dib_driver, entry)
    {
        if (dib->selector == selector)
            return dib;
    }
    return NULL;
}
/***********************************************************************
 *           CreateDC    (GDI.53)
 */
HDC16 WINAPI CreateDC16( LPCSTR driver, LPCSTR device, LPCSTR output,
                         SEGPTR segInitData )
{
    const DEVMODEA *initData = (const DEVMODEA*)MapSL(segInitData);
    HDC16 dc;
    HINSTANCE16 drv;
    char tmp[256];
    if (device && !strcmp(device, "DefaultPrinter"))
    {
        int len = 256;
        if (GetDefaultPrinterA(tmp, &len))
            device = tmp;
    }

#if 0
    if (!lstrcmpiA( driver, "dib" ) || !lstrcmpiA( driver, "dirdib" ))
    {
        struct window_surface *surface;
        HDC hdc;

        if (!(surface = create_surface( (const BITMAPINFO *)initData ))) return 0;

        if ((hdc = CreateDCA( "DISPLAY", NULL, NULL, NULL )))
        {
            __wine_set_visible_region( hdc, CreateRectRgnIndirect( &surface->rect ),
                                       &surface->rect, &surface->rect, surface );
            TRACE( "returning hdc %p surface %p\n", hdc, surface );
        }
        window_surface_release( surface );
        return HDC_16( hdc );
    }
#else
    if (!lstrcmpiA(driver, "dib") || !lstrcmpiA(driver, "dirdib") || !lstrcmpiA(driver, "dib.drv") || !lstrcmpiA(driver, "dirdib.drv"))
    {
        void *pvBits;
        HDC memdc = CreateCompatibleDC(NULL);
        HBITMAP bitmap;
        WORD selector = SELECTOROF(segInitData);
        WORD offset = OFFSETOF(segInitData);
        LPVOID base = MapSL(MAKESEGPTR(selector, 0));
        DWORD limit = wine_ldt_copy.limit[selector >> __AHSHIFT] + 1;
        volatile  BITMAPINFO *bmi = (const BITMAPINFO *)initData;
        HANDLE hSection = INVALID_HANDLE_VALUE;
        LPBYTE bits = NULL;
        DWORD avail_size;
        DWORD offset_align;
        DWORD offset_bits;
        if (segInitData == 0)
        {
            return HDC_16(memdc);
        }
        if (find_dib_driver(selector))
        {
            FIXME("Multiple DIB mappings on the same segment are not supported.\n");
            return HDC_16(memdc);
        }
        offset_bits = offset + bmi->bmiHeader.biSize + bmi->bmiHeader.biClrUsed * sizeof(RGBQUAD);
#define DWORD_PADDING(x) (((x) + 3) & -4)
        offset_align = DWORD_PADDING(offset_bits) - offset_bits;
        avail_size = max(0x10000, limit);
        hSection = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, avail_size + offset_align, NULL);
        bits = (LPBYTE)MapViewOfFile(hSection, FILE_MAP_WRITE, 0, 0, 0);
        memcpy(bits + offset_align, base, limit);
        bitmap = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, &pvBits, hSection, offset_align + offset_bits);
        SelectObject(memdc, bitmap);
        DibMapGlobalMemory(selector, (LPBYTE)bits + offset_align, avail_size);
        if (bmi->bmiHeader.biClrUsed)
            SetDIBColorTable(memdc, 0, bmi->bmiHeader.biClrUsed, &bmi->bmiColors);
        add_dib_driver_entry(bitmap, memdc, hSection, bits, selector, avail_size, offset_align);
        return HDC_16(memdc);
    }
#endif
    if (!driver || !stricmp(driver, "winspool"))
    {
        if (!initData || !IsValidDevmodeA(initData, initData->dmSize + initData->dmDriverExtra))
        {
            LONG size = ExtDeviceMode(NULL, NULL, NULL, device, output, NULL, NULL, 0);
            char *dma = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
            ExtDeviceMode(NULL, NULL, (LPDEVMODEA)dma, device, output, NULL, NULL, DM_COPY);
            HDC16 ret = HDC_16(CreateDCA(driver, device, output, (LPDEVMODEA)dma));
            HeapFree(GetProcessHeap(), 0, (LPVOID)dma);
            return ret;
        }
        else
        {
            DEVMODEA dma = {0};
            memcpy(&dma, initData, initData->dmSize);
            dma.dmSize = sizeof(DEVMODEA);
            dma.dmDriverExtra = 0;
            return HDC_16( CreateDCA( driver, device, output, &dma ) );
        }
    }
    
    dc = HDC_16( CreateDCA( driver, device, output, initData ) );

    if (dc)
        return dc;
    drv = LoadLibrary16(driver);
    if (drv == 2)
    {
        char buf[MAX_PATH];
        *buf = 0;
        StringCchCatA(buf, ARRAYSIZE(buf), driver);
        StringCchCatA(buf, ARRAYSIZE(buf), ".DRV");
        drv = LoadLibrary16(buf);
    }
    if (drv < 32)
    {
        ERR("DC driver %s not found\n", debugstr_a(driver));
        return 0;
    }
    FIXME("DC driver %s not supported\n", debugstr_a(driver));
    return CreateDC16("DIB", NULL, NULL, NULL);
}


/***********************************************************************
 *           CreateEllipticRgn    (GDI.54)
 */
HRGN16 WINAPI CreateEllipticRgn16( INT16 left, INT16 top, INT16 right, INT16 bottom )
{
    return HRGN_16( CreateEllipticRgn( left, top, right, bottom ) );
}


/***********************************************************************
 *           CreateEllipticRgnIndirect    (GDI.55)
 */
HRGN16 WINAPI CreateEllipticRgnIndirect16( const RECT16 *rect )
{
    return HRGN_16( CreateEllipticRgn( rect->left, rect->top, rect->right, rect->bottom ) );
}


static BYTE fix_font_charset(BYTE charset)
{
    //windows 3.1(JP) behaviour
    if (GetACP() == 932/*sjis*/)
    {
        if (charset != ANSI_CHARSET && charset != DEFAULT_CHARSET && charset != SYMBOL_CHARSET && charset != OEM_CHARSET)
        {
            charset = SHIFTJIS_CHARSET;
        }
    }
    return charset;
}

/***********************************************************************
 *           CreateFontIndirect   (GDI.57)
 */
HFONT16 WINAPI CreateFontIndirect16( const LOGFONT16 *plf16 )
{
    HFONT ret;
    if (plf16)
    {
        LOGFONTA lfA;
        logfont_16_to_A( plf16, &lfA );
        if (lfA.lfCharSet == 0xfe)
            lfA.lfCharSet = 0xfd;
        ret = CreateFontIndirectA( &lfA );
        TRACE("(%d, %d, %04X, %04X, %04X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %s) = %04X\n", plf16->lfHeight, plf16->lfWidth, plf16->lfEscapement, plf16->lfOrientation, plf16->lfWeight
            , plf16->lfItalic, plf16->lfUnderline, plf16->lfStrikeOut, plf16->lfCharSet, plf16->lfOutPrecision, plf16->lfClipPrecision
            , plf16->lfPitchAndFamily, debugstr_a(plf16->lfFaceName), (int)HFONT_16(ret));
    }
    else ret = CreateFontIndirectA( NULL );
    return HFONT_16(ret);
}

/***********************************************************************
*           CreateFont    (GDI.56)
*/
HFONT16 WINAPI CreateFont16(INT16 height, INT16 width, INT16 esc, INT16 orient,
    INT16 weight, BYTE italic, BYTE underline,
    BYTE strikeout, BYTE charset, BYTE outpres,
    BYTE clippres, BYTE quality, BYTE pitch,
    LPCSTR name)
{
    LOGFONT16 lf16 = { 0 };
    lf16.lfHeight = height;
    lf16.lfWidth = width;
    lf16.lfEscapement = esc;
    lf16.lfOrientation = orient;
    lf16.lfWeight = weight;
    lf16.lfItalic = italic;
    lf16.lfUnderline = underline;
    lf16.lfStrikeOut = strikeout;
    lf16.lfCharSet = charset;
    lf16.lfOutPrecision = outpres;
    lf16.lfClipPrecision = clippres;
    lf16.lfQuality = quality;
    lf16.lfPitchAndFamily = pitch;
    if (name)
        memcpy(lf16.lfFaceName, name, min(LF_FACESIZE - 1, strlen(name) + 1));
    return CreateFontIndirect16(&lf16);
}

/***********************************************************************
 *           CreateHatchBrush    (GDI.58)
 */
HBRUSH16 WINAPI CreateHatchBrush16( INT16 style, COLORREF color )
{
    return HBRUSH_16( CreateHatchBrush( style, check_colorref(color) ) );
}


/***********************************************************************
 *           CreatePatternBrush    (GDI.60)
 */
HBRUSH16 WINAPI CreatePatternBrush16( HBITMAP16 hbitmap )
{
    return HBRUSH_16( CreatePatternBrush( HBITMAP_32(hbitmap) ));
}


/***********************************************************************
 *           CreatePen    (GDI.61)
 */
HPEN16 WINAPI CreatePen16( INT16 style, INT16 width, COLORREF color )
{
    LOGPEN logpen;

    if (style == PS_NULL)
        return GetStockObject16(NULL_PEN);

    logpen.lopnStyle = style;
    logpen.lopnWidth.x = width;
    logpen.lopnWidth.y = 0;
    logpen.lopnColor = check_colorref(color);
    return HPEN_16( CreatePenIndirect( &logpen ) );
}


/***********************************************************************
 *           CreatePenIndirect    (GDI.62)
 */
HPEN16 WINAPI CreatePenIndirect16( const LOGPEN16 * pen )
{
    LOGPEN logpen;

    if (pen->lopnStyle == PS_NULL)
        return GetStockObject16(NULL_PEN);
    
    if (pen->lopnStyle > PS_INSIDEFRAME) return 0;
    logpen.lopnStyle   = pen->lopnStyle;
    logpen.lopnWidth.x = pen->lopnWidth.x;
    logpen.lopnWidth.y = pen->lopnWidth.y;
    logpen.lopnColor   = pen->lopnColor;
    return HPEN_16( CreatePenIndirect( &logpen ) );
}


/***********************************************************************
 *           CreatePolygonRgn    (GDI.63)
 */
HRGN16 WINAPI CreatePolygonRgn16( const POINT16 * points, INT16 count, INT16 mode )
{
    return CreatePolyPolygonRgn16( points, &count, 1, mode );
}


/***********************************************************************
 *           CreateRectRgn    (GDI.64)
 *
 * NOTE: cf. SetRectRgn16
 */
HRGN16 WINAPI CreateRectRgn16( INT16 left, INT16 top, INT16 right, INT16 bottom )
{
    HRGN hrgn;

    if (left < right) hrgn = CreateRectRgn( left, top, right, bottom );
    else hrgn = CreateRectRgn( 0, 0, 0, 0 );
    return HRGN_16(hrgn);
}


/***********************************************************************
 *           CreateRectRgnIndirect    (GDI.65)
 */
HRGN16 WINAPI CreateRectRgnIndirect16( const RECT16* rect )
{
    return CreateRectRgn16( rect->left, rect->top, rect->right, rect->bottom );
}


/***********************************************************************
 *           CreateSolidBrush    (GDI.66)
 */
HBRUSH16 WINAPI CreateSolidBrush16( COLORREF color )
{
    return HBRUSH_16( CreateSolidBrush( check_colorref(color) ) );
}


void delete_dib_driver(HDC hdc)
{
    struct dib_driver *dib, *next;
    LIST_FOR_EACH_ENTRY_SAFE(dib, next, &dib_drivers, struct dib_driver, entry)
    {
        if (dib->hdc != hdc) continue;
        list_remove(&dib->entry);
        DibUnmapGlobalMemory((LPBYTE)dib->map + dib->padding, dib->size);
        DeleteObject(dib->bitmap);
        UnmapViewOfFile(dib->map);
        CloseHandle(dib->hSection);
        HeapFree(GetProcessHeap(), 0, dib);
    }
}
/***********************************************************************
 *           DeleteDC    (GDI.68)
 */
BOOL16 WINAPI DeleteDC16( HDC16 hdc )
{
    HDC hdc32 = HDC_32(hdc);
    if (DeleteDC( hdc32 ))
    {
        struct saved_visrgn *saved, *next;
        struct gdi_thunk* thunk;

        if ((thunk = GDI_FindThunk(hdc))) GDI_DeleteThunk(thunk);

        LIST_FOR_EACH_ENTRY_SAFE( saved, next, &saved_regions, struct saved_visrgn, entry )
        {
            if (saved->hdc != hdc32) continue;
            list_remove( &saved->entry );
            DeleteObject( saved->hrgn );
            HeapFree( GetProcessHeap(), 0, saved );
        }
        delete_dc_entry(hdc32);
        delete_dib_driver(hdc32);
        K32WOWHandle16Destroy(hdc32, WOW_TYPE_HDC /* GDIOBJ */);
        return TRUE;
    }
    else if (!GetObjectType(hdc32))
        return TRUE; // Assume object was already released
    return FALSE;
}


/***********************************************************************
 *           DeleteObject    (GDI.69)
 *           SysDeleteObject (GDI.605)
 */
BOOL16 WINAPI DeleteObject16( HGDIOBJ16 obj )
{
    HANDLE object = HGDIOBJ_32(obj);
    BOOL result;
    int type = GetObjectType(object);
    static BOOL (*haxmvm_DeleteObject)(HGDIOBJ);
    static BOOL init;
    struct hdc_selected_object *dc, *next;
    if (!init)
    {
        HMODULE haxmvm = GetModuleHandleW(L"haxmvm");
        haxmvm_DeleteObject = haxmvm ? (BOOL(*)(HGDIOBJ))GetProcAddress(haxmvm, "haxmvm_DeleteObject") : NULL;
        init = TRUE;
    }
    LIST_FOR_EACH_ENTRY_SAFE(dc, next, &hdc_selected_objects, struct hdc_selected_object, entry)
    {
        if (type >= 1 && type <= GDI_OBJ_LAST)
        {
            HGDIOBJ selected = GetCurrentObject(dc->hdc, type);
            if ((selected && object == selected) || (!selected && dc->selected_objects[type] == object))
            {
                if (!GetObjectType(dc->hdc))
                {
                    delete_dc_entry(dc);
                    continue;
                }
                TRACE("GDIOBJ %p(%04x) is selected by DC %p\n", object, obj, dc->hdc);
                return TRUE;
            }
        }
    }
    for (int i = 0; i <= STOCK_LAST; i++)
        if (obj == stock[i]) return TRUE;
    if (type == OBJ_BITMAP) free_segptr_bits( obj );
    else if ((type == OBJ_PAL) && GetPtr16(object, 1))
    {
        HeapFree(GetProcessHeap(), 0, GetPtr16(object, 1));
        SetPtr16(object, NULL, 1);
    }
    if (haxmvm_DeleteObject)
    {
        result = haxmvm_DeleteObject(object);
    }
    else
    {
        result = DeleteObject( object );
    }
    if (GetObjectType(object) != 0 && result)
    {
        K32WOWHandle16Destroy(object, WOW_TYPE_HDC /* GDIOBJ */);
    }
    else
    {
        if (type == OBJ_PAL)
           return TRUE;
    }
    return result;
}


/***********************************************************************
 *           EnumFonts      (GDI.70)
 */
INT16 WINAPI EnumFonts16( HDC16 hDC, LPCSTR lpName, FONTENUMPROC16 efproc,
                          LPARAM lpData )
{
    struct callback16_info info;

    info.proc = (FARPROC16)efproc;
    info.param = lpData;
    info.result = 1;
    /* Don't call EnumFontFamilies! */
    return EnumFontsA(HDC_32(hDC), lpName, enum_font_callback, (LPARAM)&info);
}


/***********************************************************************
 *           EnumObjects    (GDI.71)
 */
INT16 WINAPI EnumObjects16( HDC16 hdc, INT16 obj, GOBJENUMPROC16 proc, LPARAM lParam )
{
    struct callback16_info info;

    info.proc  = (FARPROC16)proc;
    info.param = lParam;
    switch(obj)
    {
    case OBJ_PEN:
        return EnumObjects( HDC_32(hdc), OBJ_PEN, enum_pens_callback, (LPARAM)&info );
    case OBJ_BRUSH:
        return EnumObjects( HDC_32(hdc), OBJ_BRUSH, enum_brushes_callback, (LPARAM)&info );
    }
    return 0;
}


/***********************************************************************
 *           EqualRgn    (GDI.72)
 */
BOOL16 WINAPI EqualRgn16( HRGN16 rgn1, HRGN16 rgn2 )
{
    return EqualRgn( HRGN_32(rgn1), HRGN_32(rgn2) );
}


/***********************************************************************
 *           GetBitmapBits    (GDI.74)
 */
LONG WINAPI GetBitmapBits16( HBITMAP16 hbitmap, LONG count, LPVOID buffer )
{
    return GetBitmapBits( HBITMAP_32(hbitmap), count, buffer );
}


/***********************************************************************
 *		GetBkColor (GDI.75)
 */
COLORREF WINAPI GetBkColor16( HDC16 hdc )
{
    return GetBkColor( HDC_32(hdc) );
}


/***********************************************************************
 *		GetBkMode (GDI.76)
 */
INT16 WINAPI GetBkMode16( HDC16 hdc )
{
    return GetBkMode( HDC_32(hdc) );
}


/***********************************************************************
 *           GetClipBox    (GDI.77)
 */
INT16 WINAPI GetClipBox16( HDC16 hdc, LPRECT16 rect )
{
    RECT rect32;
    INT ret = GetClipBox( HDC_32(hdc), &rect32 );

    if (ret != ERROR)
    {
        rect->left   = rect32.left;
        rect->top    = rect32.top;
        rect->right  = rect32.right;
        rect->bottom = rect32.bottom;
    }
    return ret;
}


/***********************************************************************
 *		GetCurrentPosition (GDI.78)
 */
DWORD WINAPI GetCurrentPosition16( HDC16 hdc )
{
    POINT pt32;
    if (!GetCurrentPositionEx( HDC_32(hdc), &pt32 )) return 0;
    return MAKELONG( pt32.x, pt32.y );
}


/***********************************************************************
 *           GetDCOrg    (GDI.79)
 */
DWORD WINAPI GetDCOrg16( HDC16 hdc )
{
    POINT pt;
    if (GetDCOrgEx( HDC_32(hdc), &pt )) return MAKELONG( pt.x, pt.y );
    return 0;
}


/***********************************************************************
 *           GetDeviceCaps    (GDI.80)
 */
INT16 WINAPI GetDeviceCaps16( HDC16 hdc, INT16 cap )
{
    HDC hdc32 = HDC_32(hdc);
    INT16 ret = GetDeviceCaps( hdc32, cap );
    /* some apps don't expect -1 and think it's a B&W screen */
    if (krnl386_get_compat_mode("256color") && (GetDeviceCaps(hdc32, TECHNOLOGY) == DT_RASDISPLAY))
    {
        switch (cap)
        {
            case BITSPIXEL:
                ret = 8;
                break;
            case SIZEPALETTE:
                ret = 256;
                break;
            case RASTERCAPS:
                ret |= RC_PALETTE;
                break;
            case NUMCOLORS:
                ret = 20;
                break;
        }
    }
    else if ((cap == NUMCOLORS) && (ret == -1)) ret = 2048;
    if (krnl386_get_compat_mode("640X480") && (GetDeviceCaps(hdc32, TECHNOLOGY) == DT_RASDISPLAY))
    {
        switch (cap)
        {
            case HORZRES:
                ret = 640;
                break;
            case VERTRES:
                ret = 480;
                break;
        }
    }
    return ret;
}


/***********************************************************************
 *		GetMapMode (GDI.81)
 */
INT16 WINAPI GetMapMode16( HDC16 hdc )
{
    return GetMapMode( HDC_32(hdc) );
}


/***********************************************************************
 *           GetObject    (GDI.82)
 */
INT16 WINAPI GetObject16( HGDIOBJ16 handle16, INT16 count, LPVOID buffer )
{
    HGDIOBJ handle = HGDIOBJ_32( handle16 );
    switch( GetObjectType( handle ))
    {
    case OBJ_PEN:
        if (buffer)
        {
            LOGPEN16 *pen16 = buffer;
            LOGPEN pen;

            if (count < sizeof(LOGPEN16)) return 0;
            if (!GetObjectW( handle, sizeof(pen), &pen )) return 0;

            pen16->lopnStyle   = pen.lopnStyle;
            pen16->lopnColor   = pen.lopnColor;
            pen16->lopnWidth.x = pen.lopnWidth.x;
            pen16->lopnWidth.y = pen.lopnWidth.y;
        }
        return sizeof(LOGPEN16);

    case OBJ_BRUSH:
        if (buffer)
        {
            LOGBRUSH brush;
            LOGBRUSH16 brush16;

            if (!GetObjectW( handle, sizeof(brush), &brush )) return 0;
            brush16.lbStyle = brush.lbStyle;
            brush16.lbColor = brush.lbColor;
            brush16.lbHatch = brush.lbHatch;
            if (count > sizeof(brush16)) count = sizeof(brush16);
            memcpy( buffer, &brush16, count );
            return count;
        }
        return sizeof(LOGBRUSH16);

    case OBJ_PAL:
        return GetObjectW( handle, count, buffer );

    case OBJ_FONT:
        if (buffer)
        {
            LOGFONTA font;
            LOGFONT16 font16;

            if (!GetObjectA( handle, sizeof(font), &font )) return 0;
            logfont_A_to_16( &font, &font16 );
            if (count > sizeof(font16)) count = sizeof(font16);
            memcpy( buffer, &font16, count );
            return count;
        }
        return sizeof(LOGFONT16);

    case OBJ_BITMAP:
        {
            DIBSECTION dib;
            INT size;
            BITMAP16 bmp16;

            if (!(size = GetObjectW( handle, sizeof(dib), &dib ))) return 0;
            if (size == sizeof(DIBSECTION) && count > sizeof(BITMAP16))
            {
                FIXME("not implemented for DIBs: count %d\n", count);
                return 0;
            }
            else
            {
		if (!buffer) return sizeof(BITMAP16);
                bmp16.bmType       = dib.dsBm.bmType;
                bmp16.bmWidth      = dib.dsBm.bmWidth;
                bmp16.bmHeight     = dib.dsBm.bmHeight;
                bmp16.bmWidthBytes = dib.dsBm.bmWidthBytes;
                bmp16.bmPlanes     = dib.dsBm.bmPlanes;
                bmp16.bmBitsPixel  = dib.dsBm.bmBitsPixel;
                bmp16.bmBits       = 0;
            	memcpy(buffer, &bmp16, count);
		return count;
	    }
        }

    default:
        return 0;
    }
}


/***********************************************************************
 *           GetPixel    (GDI.83)
 */
COLORREF WINAPI GetPixel16( HDC16 hdc, INT16 x, INT16 y )
{
    return GetPixel( HDC_32(hdc), x, y );
}


/***********************************************************************
 *		GetPolyFillMode (GDI.84)
 */
INT16 WINAPI GetPolyFillMode16( HDC16 hdc )
{
    return GetPolyFillMode( HDC_32(hdc) );
}


/***********************************************************************
 *		GetROP2 (GDI.85)
 */
INT16 WINAPI GetROP216( HDC16 hdc )
{
    return GetROP2( HDC_32(hdc) );
}

DWORD WINAPI GetRelAbs(_In_ HDC 	hdc,
	_In_ DWORD 	dwIgnore
	);
/***********************************************************************
 *		GetRelAbs (GDI.86)
 */
INT16 WINAPI GetRelAbs16( HDC16 hdc )
{
    return GetRelAbs( HDC_32(hdc), 0 );
}


/***********************************************************************
 *           GetStockObject    (GDI.87)
 */
HGDIOBJ16 WINAPI GetStockObject16( INT16 obj )
{
    if (IsOldWindowsTask(GetCurrentTask()) && !(get_aflags(GetExePtr(GetCurrentTask())) & NE_AFLAGS_WIN2_PROTMODE) && (obj == SYSTEM_FONT))
        obj = SYSTEM_FIXED_FONT;
    HGDIOBJ16 ret = HGDIOBJ_16( GetStockObject( obj ) );
    if (ret && (obj <= STOCK_LAST))
        stock[obj] = ret;
    return ret;
}


/***********************************************************************
 *		GetStretchBltMode (GDI.88)
 */
INT16 WINAPI GetStretchBltMode16( HDC16 hdc )
{
    return GetStretchBltMode( HDC_32(hdc) );
}


/***********************************************************************
 *           GetTextCharacterExtra    (GDI.89)
 */
INT16 WINAPI GetTextCharacterExtra16( HDC16 hdc )
{
    return GetTextCharacterExtra( HDC_32(hdc) );
}


/***********************************************************************
 *		GetTextColor (GDI.90)
 */
COLORREF WINAPI GetTextColor16( HDC16 hdc )
{
    return GetTextColor( HDC_32(hdc) );
}

// for old fon vector fonts
// Windows 10 only returns the extent based on the relative angle between the orientation and escapement
// but ignores the absolute angles
// Windows XP is mostly the same but calculates the full extent when the relative angle is 0
// Windows 3.1 returns the full extent but rounds the relative angle down to 0, 90, 180 or 270 degrees
static void check_font_rotation(HDC hdc, SIZE16 *box)
{
    if (LOWORD(LOBYTE(GetVersion())) >= 0x6)
    {
        TEXTMETRICA tm;
        GetTextMetricsA(hdc, &tm);
        if((tm.tmPitchAndFamily & (TMPF_VECTOR | TMPF_TRUETYPE)) != TMPF_VECTOR)
            return;
        HFONT hfont = GetCurrentObject(hdc, OBJ_FONT);
        LOGFONT lfont;
        GetObject(hfont, sizeof(LOGFONT), &lfont);
        if (lfont.lfEscapement == lfont.lfOrientation)
        {
            int angle = lfont.lfEscapement % 1800;
            const float d2r = 3.14159265358979323846 / 1800;
            if (angle)
            {
                int x = box->cx, y = box->cy;
                box->cx = (y * cosf((900 - angle) * d2r)) + (x * fabsf(cosf(angle * d2r)));
                box->cy = (x * cosf((900 - angle) * d2r)) + (y * fabsf(cosf(angle * d2r)));
            }
        }
    }
}

/***********************************************************************
 *           GetTextExtent    (GDI.91)
 */
DWORD WINAPI GetTextExtent16( HDC16 hdc, LPCSTR str, INT16 count )
{
    SIZE16 size;
    DWORD ret;
    __TRY
    {
        if (!GetTextExtentPoint16( hdc, str, count, &size ))
        {
            ret = 0;
        }
        else
        {
            ret = MAKELONG( size.cx, size.cy );
        }
    }
    __EXCEPT_ALL
    {
        return 0;
    }
    __ENDTRY
    return ret;
}


/***********************************************************************
 *           GetTextFace    (GDI.92)
 */
INT16 WINAPI GetTextFace16( HDC16 hdc, INT16 count, LPSTR name )
{
    return GetTextFaceA( HDC_32(hdc), count, name );
}


/***********************************************************************
 *           GetTextMetrics    (GDI.93)
 */
BOOL16 WINAPI GetTextMetrics16( HDC16 hdc, TEXTMETRIC16 *tm )
{
    TEXTMETRICA tm32;

    if (!GetTextMetricsA( HDC_32(hdc), &tm32 )) return FALSE;

    tm->tmHeight           = tm32.tmHeight;
    tm->tmAscent           = tm32.tmAscent;
    tm->tmDescent          = tm32.tmDescent;
    tm->tmInternalLeading  = tm32.tmInternalLeading;
    tm->tmExternalLeading  = tm32.tmExternalLeading;
    tm->tmAveCharWidth     = tm32.tmAveCharWidth;
    tm->tmMaxCharWidth     = tm32.tmMaxCharWidth;
    tm->tmWeight           = tm32.tmWeight;
    tm->tmOverhang         = tm32.tmOverhang;
    tm->tmDigitizedAspectX = tm32.tmDigitizedAspectX;
    tm->tmDigitizedAspectY = tm32.tmDigitizedAspectY;
    tm->tmFirstChar        = tm32.tmFirstChar;
    tm->tmLastChar         = tm32.tmLastChar;
    tm->tmDefaultChar      = tm32.tmDefaultChar;
    tm->tmBreakChar        = tm32.tmBreakChar;
    tm->tmItalic           = tm32.tmItalic;
    tm->tmUnderlined       = tm32.tmUnderlined;
    tm->tmStruckOut        = tm32.tmStruckOut;
    tm->tmPitchAndFamily   = tm32.tmPitchAndFamily;
    tm->tmCharSet          = tm32.tmCharSet;
    return TRUE;
}


/***********************************************************************
 *		GetViewportExt (GDI.94)
 */
DWORD WINAPI GetViewportExt16( HDC16 hdc )
{
    SIZE size;
    if (!GetViewportExtEx( HDC_32(hdc), &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *		GetViewportOrg (GDI.95)
 */
DWORD WINAPI GetViewportOrg16( HDC16 hdc )
{
    POINT pt;
    if (!GetViewportOrgEx( HDC_32(hdc), &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *		GetWindowExt (GDI.96)
 */
DWORD WINAPI GetWindowExt16( HDC16 hdc )
{
    SIZE size;
    if (!GetWindowExtEx( HDC_32(hdc), &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *		GetWindowOrg (GDI.97)
 */
DWORD WINAPI GetWindowOrg16( HDC16 hdc )
{
    POINT pt;
    if (!GetWindowOrgEx( HDC_32(hdc), &pt )) return 0;
    return MAKELONG( pt.x, pt.y );
}




/**********************************************************************
 *           LineDDA   (GDI.100)
 */
void WINAPI LineDDA16( INT16 nXStart, INT16 nYStart, INT16 nXEnd,
                       INT16 nYEnd, LINEDDAPROC16 proc, LPARAM lParam )
{
    struct callback16_info info;
    info.proc  = (FARPROC16)proc;
    info.param = lParam;
    LineDDA( nXStart, nYStart, nXEnd, nYEnd, linedda_callback, (LPARAM)&info );
}


/***********************************************************************
 *           OffsetRgn    (GDI.101)
 */
INT16 WINAPI OffsetRgn16( HRGN16 hrgn, INT16 x, INT16 y )
{
    return OffsetRgn( HRGN_32(hrgn), x, y );
}


/***********************************************************************
 *           PtVisible    (GDI.103)
 */
BOOL16 WINAPI PtVisible16( HDC16 hdc, INT16 x, INT16 y )
{
    return PtVisible( HDC_32(hdc), x, y );
}


/***********************************************************************
 *           SelectVisRgn   (GDI.105)
 */
INT16 WINAPI SelectVisRgn16( HDC16 hdc, HRGN16 hrgn )
{
    FIXME( "%04x %04x no longer supported\n", hdc, hrgn );
    return ERROR;
}


/***********************************************************************
 *           SetBitmapBits    (GDI.106)
 */
LONG WINAPI SetBitmapBits16( HBITMAP16 hbitmap, LONG count, LPCVOID buffer )
{
    HBITMAP hbmp32 = HBITMAP_32(hbitmap);
    BITMAP bmp;
    if (GetObject(hbmp32, sizeof(BITMAP), &bmp) != sizeof(BITMAP))
        return 0;
    if (krnl386_get_compat_mode("256color") && (bmp.bmBitsPixel > 8))
    {
        HDC dc = CreateCompatibleDC(NULL);
        BITMAPINFO *bmap = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 256*2 + sizeof(BITMAPINFOHEADER));
        UINT16 *colors = (UINT16 *)bmap->bmiColors;
        VOID *section;
        bmap->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmap->bmiHeader.biWidth = bmp.bmWidth;
        bmap->bmiHeader.biHeight = -(count / bmp.bmWidth);
        bmap->bmiHeader.biPlanes = 1;
        bmap->bmiHeader.biBitCount = 8;
        for (int i = 0; i < 256; i++)
            colors[i] = i;
        int ret = SetDIBits(dc, hbmp32, 0, bmap->bmiHeader.biHeight, buffer, bmap, DIB_PAL_COLORS);
        HeapFree(GetProcessHeap(), 0, bmap);
        DeleteDC(dc);
        return ret * bmp.bmWidth;
    }

    DWORD size = bmp.bmHeight * bmp.bmWidthBytes;
    return SetBitmapBits( hbmp32, min(size, count), buffer );
}

#include <pshpack1.h>
typedef struct
{
    SHORT dfVersion;		/* Version */
    LONG dfSize;		/* Total File Size */
    char dfCopyright[60];	/* Copyright notice */
    FONTINFO16 fi;		/* FONTINFO structure */
} WINFNT;
#include <poppack.h>

LPCSTR krnl386_search_executable_file(LPCSTR lpFile, LPSTR buf, SIZE_T size, BOOL search_builtin);
NE_TYPEINFO *get_resource_table(HMODULE16 hmod, LPCSTR type, LPBYTE *restab);
/***********************************************************************
 *           AddFontResource    (GDI.119)
 */
INT16 WINAPI AddFontResource16( LPCSTR filename )
{
    int ret = 0;
    LPCSTR filenameold = filename;
    char filebuf[MAX_PATH];
    filename = krnl386_search_executable_file(filename, filebuf, MAX_PATH, FALSE);
    TRACE("(%s(%s))\n", debugstr_a(filenameold), debugstr_a(filename));
    ret = AddFontResourceExA(filename, FR_PRIVATE, 0);
    if (ret) return ret;

    // try to load 0x100 version bitmap font by converting to a 0x200 font
    // which Windows can load
    HINSTANCE16 mod = 0;
    HANDLE fh, mh;
    NE_NAMEINFO *name;
    int count = 0;
    LPVOID font;
    if ((mod = LoadLibrary16(filename)) >= 32)
    {
        NE_TYPEINFO *type = get_resource_table(mod, RT_FONT, NULL);
        if (!type)
        {
            FreeLibrary16(mod);
            return 0;
        }
        count = type->count;
        name = (NE_NAMEINFO*)(type + 1);
    }
    else
    {
        fh = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(fh == INVALID_HANDLE_VALUE)
            return 0;
        mh = CreateFileMappingA(fh, filename, PAGE_READONLY, 0, 0, NULL);
        font = MapViewOfFile(mh, FILE_MAP_READ, 0, 0, 0);
        count = 1;
        mod = 0;
    }
    char *dst = HeapAlloc(GetProcessHeap(), 0, 65536);
    HGLOBAL16 mem = 0;
    __TRY
    {
        for(int num = 0; num < count; num++)
        {
            WINFNT *fnt;
            int size;
            if(mod)
            {
                if (mem)
                    FreeResource16(mem);
                HRSRC16 res = FindResource16(mod, name->id, 8);
                mem = LoadResource16(mod, res);
                font = LockResource16(mem);
                size = SizeofResource16(mod, res);
                name++;
            }
            else
                size = GetFileSize(fh, NULL);

            fnt = font;

            if (fnt->dfVersion != 0x100)
            {
                int fnum;
                if (AddFontMemResourceEx(font, fnt->dfSize, 0, &fnum))
                {
                    TRACE("Added %d fonts\n", fnum);
                    ret += fnum;
                }
                continue;
            }
            if((size < 117) || (fnt->fi.dfType & 1))
                continue;

            memcpy(dst, font, 118);
            int fontsize = (unsigned char)fnt->fi.dfLastChar - (unsigned char)fnt->fi.dfFirstChar + 2;
            WORD *charoff = (char *)font + 117;
            WORD *ncharoff = dst + 118;
            int hdrsize = 118 + (fontsize * 4);

            if(size < (117 + (fnt->fi.dfPixWidth ? (fontsize * 2) : 0) + (fnt->fi.dfWidthBytes * fnt->fi.dfPixHeight)))
            {
                TRACE("Font corrupt\n");
                continue;
            }

            for(int i = 0; i < (fontsize - 1); i++)
            {
                ncharoff[i * 2] = fnt->fi.dfPixWidth ? fnt->fi.dfPixWidth : charoff[i + 1] - charoff[i];
                ncharoff[(i * 2) + 1] = i ? (((ncharoff[(i * 2) - 2] + 7) / 8) * fnt->fi.dfPixHeight) + ncharoff[(i * 2) - 1] : hdrsize;
            }
            ncharoff[(fontsize - 1) * 2] = ncharoff[((fontsize - 1) * 2) + 1] = 0;
            int rowwid = fnt->fi.dfWidthBytes;
            int nbitsize = ncharoff[(fontsize * 2) - 3] + (((ncharoff[(fontsize * 2) - 4] + 7) / 8) * fnt->fi.dfPixHeight) - hdrsize;
            char *bitdata = (char *)font + fnt->fi.dfBitsOffset;
            char *nbitdata = dst + hdrsize;
            for(int i = 0; i < (fontsize - 1); i++)
            {
                int addr = ncharoff[(i * 2) + 1] - hdrsize;
                int bytewid = (ncharoff[i * 2] + 7) / 8;
                for(int j = 0; j < fnt->fi.dfPixHeight; j++)
                {
                    for(int k = 0; k < bytewid; k++)
                    {
                        WORD bits = *(WORD *)(bitdata + (rowwid * j) + ((fnt->fi.dfPixWidth ? fnt->fi.dfPixWidth : charoff[i]) / 8) + k);
                        bits = (bits >> 8) | (bits << 8);
                        bits <<= charoff[i] % 8;
                        nbitdata[addr + j + (k * fnt->fi.dfPixHeight)] = bits >> 8;
                    }
                }
            }
            strncpy(dst + hdrsize + nbitsize, (char *)font + fnt->fi.dfFace, 256);
            int namelen = strlen(dst + hdrsize + nbitsize);
            if (namelen > 256)
                namelen = 256;
            fnt = dst;
            fnt->dfVersion = 0x200;
            fnt->fi.dfBitsOffset = hdrsize;
            fnt->fi.dfFace = hdrsize + nbitsize;
            fnt->dfSize = fnt->fi.dfFace + namelen + 1;
            if (fnt->fi.dfCharSet == 0xfe) // pagemaker fonts have charset 0xfe which windows 10 treats as utf-8
                fnt->fi.dfCharSet = 0xfd;	
            int fnum;
            if (AddFontMemResourceEx(dst, fnt->dfSize, 0, &fnum))
            {
                TRACE("Added %d fonts\n", fnum);
                ret += fnum;
            }
        }
    }
    __EXCEPT_ALL
    {
        ERR("Failed to load old font\n");
    }
    __ENDTRY
    if(mod)
    {
        if (mem)
            FreeResource16(mem);
        FreeLibrary16(mod);
    }
    else
    {
        UnmapViewOfFile(font);
        CloseHandle(mh);
        CloseHandle(fh);
    }
    HeapFree(GetProcessHeap(), 0, dst);
    return ret;
}


/***********************************************************************
 *           Death    (GDI.121)
 *
 * Disables GDI, switches back to text mode.
 * We don't have to do anything here,
 * just let console support handle everything
 */
void WINAPI Death16(HDC16 hdc)
{
    MESSAGE("Death(%04x) called. Application enters text mode...\n", hdc);
}


/***********************************************************************
 *           Resurrection    (GDI.122)
 *
 * Restores GDI functionality
 */
void WINAPI Resurrection16(HDC16 hdc,
                           WORD w1, WORD w2, WORD w3, WORD w4, WORD w5, WORD w6)
{
    MESSAGE("Resurrection(%04x, %04x, %04x, %04x, %04x, %04x, %04x) called. Application left text mode.\n",
            hdc, w1, w2, w3, w4, w5, w6);
}


/***********************************************************************
 *           MulDiv   (GDI.128)
 */
INT16 WINAPI MulDiv16( INT16 nMultiplicand, INT16 nMultiplier, INT16 nDivisor)
{
    INT ret;
    if (!nDivisor) return -32768;
    /* We want to deal with a positive divisor to simplify the logic. */
    if (nDivisor < 0)
    {
      nMultiplicand = - nMultiplicand;
      nDivisor = -nDivisor;
    }
    /* If the result is positive, we "add" to round. else,
     * we subtract to round. */
    if ( ( (nMultiplicand <  0) && (nMultiplier <  0) ) ||
         ( (nMultiplicand >= 0) && (nMultiplier >= 0) ) )
        ret = (((int)nMultiplicand * nMultiplier) + (nDivisor/2)) / nDivisor;
    else
        ret = (((int)nMultiplicand * nMultiplier) - (nDivisor/2)) / nDivisor;
    if ((ret > 32767) || (ret < -32767)) return -32768;
    return (INT16) ret;
}


/***********************************************************************
 *           GetRgnBox    (GDI.134)
 */
INT16 WINAPI GetRgnBox16( HRGN16 hrgn, LPRECT16 rect )
{
    RECT r;
    INT16 ret = GetRgnBox( HRGN_32(hrgn), &r );
    rect->left   = r.left;
    rect->top    = r.top;
    rect->right  = r.right;
    rect->bottom = r.bottom;
    return ret;
}


/***********************************************************************
 *           RemoveFontResource    (GDI.136)
 */
BOOL16 WINAPI RemoveFontResource16( LPCSTR filename)
{
    char filebuf[MAX_PATH];
    filename = krnl386_search_executable_file(filename, filebuf, MAX_PATH, FALSE);
    return RemoveFontResourceExA(filename, FR_PRIVATE, 0);
}


/***********************************************************************
 *           SetBrushOrg    (GDI.148)
 */
DWORD WINAPI SetBrushOrg16( HDC16 hdc, INT16 x, INT16 y )
{
    POINT dcpt;
    POINT pt;

    if (GetDCOrgEx(HDC_32(hdc), &dcpt))
    {
        x -= dcpt.x;
        y -= dcpt.y;
        if (!SetBrushOrgEx(HDC_32(hdc), x, y, &pt)) return 0;
        pt.x += dcpt.x;
        pt.y += dcpt.y;
    }
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *		GetBrushOrg (GDI.149)
 */
DWORD WINAPI GetBrushOrg16( HDC16 hdc )
{
    POINT dcpt;
    POINT pt;
    if (GetDCOrgEx(HDC_32(hdc), &dcpt))
    {
        if (!GetBrushOrgEx(HDC_32(hdc), &pt)) return 0;
        pt.x += dcpt.x;
        pt.y += dcpt.y;
    }
    return MAKELONG( pt.x, pt.y );
}


/***********************************************************************
 *           UnrealizeObject    (GDI.150)
 */
BOOL16 WINAPI UnrealizeObject16( HGDIOBJ16 obj )
{
    return UnrealizeObject( HGDIOBJ_32(obj) );
}


/***********************************************************************
 *           CreateIC    (GDI.153)
 */
HDC16 WINAPI CreateIC16( LPCSTR driver, LPCSTR device, LPCSTR output,
                         const DEVMODEA* initData )
{
    char tmp[256];
    if (device && !strcmp(device, "DefaultPrinter"))
    {
        int len = 256;
        if (GetDefaultPrinterA(tmp, &len))
            device = tmp;
    }
    if (initData && (!driver || !stricmp(driver, "winspool")))
    {
        DEVMODEA dma = {0};
        if (!IsValidDevmodeA(initData, initData->dmSize + initData->dmDriverExtra))
            initData = NULL;
        else
        {
            memcpy(&dma, initData, initData->dmSize);
            dma.dmSize = sizeof(DEVMODEA);
            dma.dmDriverExtra = 0;
            return HDC_16( CreateICA( driver, device, output, &dma ) );
        }
    }
    else if (!stricmp(driver, "dib"))
        driver = "DISPLAY";
    return HDC_16( CreateICA( driver, device, output, initData ) );
}


/***********************************************************************
 *           GetNearestColor   (GDI.154)
 */
COLORREF WINAPI GetNearestColor16( HDC16 hdc, COLORREF color )
{
    return GetNearestColor( HDC_32(hdc), check_colorref(color) );
}


/***********************************************************************
 *           CreateDiscardableBitmap    (GDI.156)
 */
HBITMAP16 WINAPI CreateDiscardableBitmap16( HDC16 hdc, INT16 width, INT16 height )
{
    return HBITMAP_16( CreateDiscardableBitmap( HDC_32(hdc), width, height ) );
}


/***********************************************************************
 *           PtInRegion    (GDI.161)
 */
BOOL16 WINAPI PtInRegion16( HRGN16 hrgn, INT16 x, INT16 y )
{
    return PtInRegion( HRGN_32(hrgn), x, y );
}


/***********************************************************************
 *           GetBitmapDimension    (GDI.162)
 */
DWORD WINAPI GetBitmapDimension16( HBITMAP16 hbitmap )
{
    SIZE16 size;
    if (!GetBitmapDimensionEx16( hbitmap, &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *           SetBitmapDimension    (GDI.163)
 */
DWORD WINAPI SetBitmapDimension16( HBITMAP16 hbitmap, INT16 x, INT16 y )
{
    SIZE16 size;
    if (!SetBitmapDimensionEx16( hbitmap, x, y, &size )) return 0;
    return MAKELONG( size.cx, size.cy );
}


/***********************************************************************
 *           SetRectRgn    (GDI.172)
 *
 * NOTE: Win 3.1 sets region to empty if left > right
 */
void WINAPI SetRectRgn16( HRGN16 hrgn, INT16 left, INT16 top, INT16 right, INT16 bottom )
{
    if (left < right) SetRectRgn( HRGN_32(hrgn), left, top, right, bottom );
    else SetRectRgn( HRGN_32(hrgn), 0, 0, 0, 0 );
}


/******************************************************************
 *             PlayMetaFileRecord   (GDI.176)
 */
void WINAPI PlayMetaFileRecord16( HDC16 hdc, HANDLETABLE16 *ht, METARECORD *mr, UINT16 handles )
{
    HANDLETABLE *ht32 = HeapAlloc( GetProcessHeap(), 0, FIELD_OFFSET(HANDLETABLE, objectHandle[handles] ));
    unsigned int i;

    for (i = 0; i < handles; i++) ht32->objectHandle[i] = HGDIOBJ_32(ht->objectHandle[i]);
    PlayMetaFileRecord( HDC_32(hdc), ht32, mr, handles );
    for (i = 0; i < handles; i++) ht->objectHandle[i] = HGDIOBJ_16(ht32->objectHandle[i]);
    HeapFree( GetProcessHeap(), 0, ht32 );
}


/***********************************************************************
 *           SetDCHook   (GDI.190)
 */
BOOL16 WINAPI SetDCHook16( HDC16 hdc16, FARPROC16 hookProc, DWORD dwHookData )
{
    FIXME( "%04x %p %x: not supported\n", hdc16, hookProc, dwHookData );
    return FALSE;
}


/***********************************************************************
 *           GetDCHook   (GDI.191)
 */
DWORD WINAPI GetDCHook16( HDC16 hdc16, FARPROC16 *phookProc )
{
    FIXME( "%04x: not supported\n", hdc16 );
    return 0;
}


/***********************************************************************
 *           SetHookFlags   (GDI.192)
 */
WORD WINAPI SetHookFlags16( HDC16 hdc, WORD flags )
{
    FIXME( "%04x %x: not supported\n", hdc, flags );
    return 0;
}


/***********************************************************************
 *           SetBoundsRect    (GDI.193)
 */
UINT16 WINAPI SetBoundsRect16( HDC16 hdc, const RECT16* rect, UINT16 flags )
{
    if (rect)
    {
        RECT rect32;
        rect32.left   = rect->left;
        rect32.top    = rect->top;
        rect32.right  = rect->right;
        rect32.bottom = rect->bottom;
        return SetBoundsRect( HDC_32( hdc ), &rect32, flags );
    }
    else return SetBoundsRect( HDC_32( hdc ), NULL, flags );
}


/***********************************************************************
 *           GetBoundsRect    (GDI.194)
 */
UINT16 WINAPI GetBoundsRect16( HDC16 hdc, LPRECT16 rect, UINT16 flags)
{
    RECT rect32;
    UINT ret = GetBoundsRect( HDC_32( hdc ), &rect32, flags );
    if (rect)
    {
        rect->left   = rect32.left;
        rect->top    = rect32.top;
        rect->right  = rect32.right;
        rect->bottom = rect32.bottom;
    }
    return ret;
}


/***********************************************************************
 *		EngineEnumerateFont (GDI.300)
 */
WORD WINAPI EngineEnumerateFont16(LPSTR fontname, FARPROC16 proc, DWORD data )
{
    FIXME("(%s,%p,%x),stub\n",fontname,proc,data);
    return 0;
}


/***********************************************************************
 *		EngineDeleteFont (GDI.301)
 */
WORD WINAPI EngineDeleteFont16(LPFONTINFO16 lpFontInfo)
{
    WORD handle;

    /*	untested, don't know if it works.
	We seem to access some structure that is located after the
	FONTINFO. The FONTINFO documentation says that there may
	follow some char-width table or font bitmap or vector info.
	I think it is some kind of font bitmap that begins at offset 0x52,
	as FONTINFO goes up to 0x51.
	If this is correct, everything should be implemented correctly.
    */
    if ( ((lpFontInfo->dfType & (RASTER_FONTTYPE|DEVICE_FONTTYPE)) == (RASTER_FONTTYPE|DEVICE_FONTTYPE))
         && (LOWORD(lpFontInfo->dfFace) == LOWORD(lpFontInfo)+0x6e)
         && (handle = *(WORD *)(lpFontInfo+0x54)) )
    {
        *(WORD *)(lpFontInfo+0x54) = 0;
        GlobalFree16(handle);
    }
    return 1;
}


/***********************************************************************
 *		EngineRealizeFont (GDI.302)
 */
WORD WINAPI EngineRealizeFont16(LPLOGFONT16 lplogFont, LPTEXTXFORM16 lptextxform, LPFONTINFO16 lpfontInfo)
{
    FIXME("(%p,%p,%p),stub\n",lplogFont,lptextxform,lpfontInfo);

    return 0;
}


/***********************************************************************
 *		EngineRealizeFontExt (GDI.315)
 */
WORD WINAPI EngineRealizeFontExt16(LONG l1, LONG l2, LONG l3, LONG l4)
{
    FIXME("(%08x,%08x,%08x,%08x),stub\n",l1,l2,l3,l4);

    return 0;
}


/***********************************************************************
 *		EngineGetCharWidth (GDI.303)
 */
WORD WINAPI EngineGetCharWidth16(LPFONTINFO16 lpFontInfo, BYTE firstChar, BYTE lastChar, LPINT16 buffer)
{
    int i;

    for (i = firstChar; i <= lastChar; i++)
       FIXME(" returns font's average width for range %d to %d\n", firstChar, lastChar);
    *buffer++ = lpFontInfo->dfAvgWidth; /* insert some charwidth functionality here; use average width for now */
    return 1;
}


/***********************************************************************
 *		EngineSetFontContext (GDI.304)
 */
WORD WINAPI EngineSetFontContext16(LPFONTINFO16 lpFontInfo, WORD data)
{
   FIXME("stub?\n");
   return 0;
}

/***********************************************************************
 *		EngineGetGlyphBMP (GDI.305)
 */
WORD WINAPI EngineGetGlyphBMP16(WORD word, LPFONTINFO16 lpFontInfo, WORD w1, WORD w2,
                              LPSTR string, DWORD dword, /*LPBITMAPMETRICS16*/ LPVOID metrics)
{
    FIXME("stub?\n");
    return 0;
}


/***********************************************************************
 *		EngineMakeFontDir (GDI.306)
 */
DWORD WINAPI EngineMakeFontDir16(HDC16 hdc, LPFONTDIR16 fontdir, LPCSTR string)
{
    FIXME(" stub! (always fails)\n");
    return ~0UL; /* error */
}


/***********************************************************************
 *           GetCharABCWidths   (GDI.307)
 */
BOOL16 WINAPI GetCharABCWidths16( HDC16 hdc, UINT16 firstChar, UINT16 lastChar, LPABC16 abc )
{
    BOOL ret;
    UINT i;
    LPABC abc32 = HeapAlloc( GetProcessHeap(), 0, sizeof(ABC) * (lastChar - firstChar + 1) );

    if ((ret = GetCharABCWidthsA( HDC_32(hdc), firstChar, lastChar, abc32 )))
    {
        for (i = firstChar; i <= lastChar; i++)
        {
            abc[i-firstChar].abcA = abc32[i-firstChar].abcA;
            abc[i-firstChar].abcB = abc32[i-firstChar].abcB;
            abc[i-firstChar].abcC = abc32[i-firstChar].abcC;
        }
    }
    HeapFree( GetProcessHeap(), 0, abc32 );
    return ret;
}


/***********************************************************************
 *           GetOutlineTextMetrics (GDI.308)
 *
 * Gets metrics for TrueType fonts.
 *
 * PARAMS
 *    hdc    [In]  Handle of device context
 *    cbData [In]  Size of metric data array
 *    lpOTM  [Out] Address of metric data array
 *
 * RETURNS
 *    Success: Non-zero or size of required buffer
 *    Failure: 0
 *
 * NOTES
 *    lpOTM should be LPOUTLINETEXTMETRIC
 */
UINT16 WINAPI GetOutlineTextMetrics16( HDC16 hdc, UINT16 cbData,
                                       LPOUTLINETEXTMETRIC16 lpOTM )
{
    HDC hdc32 = HDC_32(hdc);
    int offset = sizeof(OUTLINETEXTMETRICA) - sizeof(*lpOTM);
    if (!lpOTM)
    {
        UINT buf_size = GetOutlineTextMetricsA(hdc32, NULL, NULL);
        if (buf_size == 0)
            return 0;
        return buf_size - offset;
    }
    if (cbData < sizeof(OUTLINETEXTMETRIC16))
        return 0;
    /* extra data size for strings */
    SIZE_T exsize = cbData - sizeof(*lpOTM);

    LPOUTLINETEXTMETRICA otm32 = HeapAlloc(GetProcessHeap(), 0, sizeof(OUTLINETEXTMETRICA) + exsize);
    otm32->otmSize = sizeof(*otm32) + exsize;
    UINT result = GetOutlineTextMetricsA(hdc32, sizeof(*otm32) + exsize, otm32);
    if (result == 0)
    {
        HeapFree(GetProcessHeap(), 0, otm32);
        return 0;
    }
    lpOTM->otmSize = otm32->otmSize - offset;
    lpOTM->otmTextMetrics.tmHeight = otm32->otmTextMetrics.tmHeight;
    lpOTM->otmTextMetrics.tmAscent = otm32->otmTextMetrics.tmAscent;
    lpOTM->otmTextMetrics.tmDescent = otm32->otmTextMetrics.tmDescent;
    lpOTM->otmTextMetrics.tmInternalLeading = otm32->otmTextMetrics.tmInternalLeading;
    lpOTM->otmTextMetrics.tmExternalLeading = otm32->otmTextMetrics.tmExternalLeading;
    lpOTM->otmTextMetrics.tmAveCharWidth = otm32->otmTextMetrics.tmAveCharWidth;
    lpOTM->otmTextMetrics.tmMaxCharWidth = otm32->otmTextMetrics.tmMaxCharWidth;
    lpOTM->otmTextMetrics.tmWeight = otm32->otmTextMetrics.tmWeight;
    lpOTM->otmTextMetrics.tmOverhang = otm32->otmTextMetrics.tmOverhang;
    lpOTM->otmTextMetrics.tmDigitizedAspectX = otm32->otmTextMetrics.tmDigitizedAspectX;
    lpOTM->otmTextMetrics.tmDigitizedAspectY = otm32->otmTextMetrics.tmDigitizedAspectY;
    lpOTM->otmTextMetrics.tmFirstChar = otm32->otmTextMetrics.tmFirstChar;
    lpOTM->otmTextMetrics.tmLastChar = otm32->otmTextMetrics.tmLastChar;
    lpOTM->otmTextMetrics.tmDefaultChar = otm32->otmTextMetrics.tmDefaultChar;
    lpOTM->otmTextMetrics.tmBreakChar = otm32->otmTextMetrics.tmBreakChar;
    lpOTM->otmTextMetrics.tmItalic = otm32->otmTextMetrics.tmItalic;
    lpOTM->otmTextMetrics.tmUnderlined = otm32->otmTextMetrics.tmUnderlined;
    lpOTM->otmTextMetrics.tmStruckOut = otm32->otmTextMetrics.tmStruckOut;
    lpOTM->otmTextMetrics.tmPitchAndFamily = otm32->otmTextMetrics.tmPitchAndFamily;
    lpOTM->otmTextMetrics.tmCharSet = otm32->otmTextMetrics.tmCharSet;
    lpOTM->otmFiller = otm32->otmFiller;
    lpOTM->otmPanoseNumber = otm32->otmPanoseNumber;
    lpOTM->otmfsSelection = otm32->otmfsSelection;
    lpOTM->otmfsType = otm32->otmfsType;
    lpOTM->otmsCharSlopeRise = otm32->otmsCharSlopeRise;
    lpOTM->otmsCharSlopeRun = otm32->otmsCharSlopeRun;
    lpOTM->otmItalicAngle = otm32->otmItalicAngle;
    lpOTM->otmEMSquare = otm32->otmEMSquare;
    lpOTM->otmAscent = otm32->otmAscent;
    lpOTM->otmDescent = otm32->otmDescent;
    lpOTM->otmLineGap = otm32->otmLineGap;
    lpOTM->otmsCapEmHeight = otm32->otmsCapEmHeight;
    lpOTM->otmsXHeight = otm32->otmsXHeight;
    lpOTM->otmrcFontBox.left = otm32->otmrcFontBox.left;
    lpOTM->otmrcFontBox.top = otm32->otmrcFontBox.top;
    lpOTM->otmrcFontBox.right = otm32->otmrcFontBox.right;
    lpOTM->otmrcFontBox.bottom = otm32->otmrcFontBox.bottom;
    lpOTM->otmMacAscent = otm32->otmMacAscent;
    lpOTM->otmMacDescent = otm32->otmMacDescent;
    lpOTM->otmMacLineGap = otm32->otmMacLineGap;
    lpOTM->otmusMinimumPPEM = otm32->otmusMinimumPPEM;
    lpOTM->otmptSubscriptSize.x = otm32->otmptSubscriptSize.x;
    lpOTM->otmptSubscriptSize.y = otm32->otmptSubscriptSize.y;
    lpOTM->otmptSubscriptOffset.x = otm32->otmptSubscriptOffset.x;
    lpOTM->otmptSubscriptOffset.y = otm32->otmptSubscriptOffset.y;
    lpOTM->otmptSuperscriptSize.x = otm32->otmptSuperscriptSize.x;
    lpOTM->otmptSuperscriptSize.y = otm32->otmptSuperscriptSize.y;
    lpOTM->otmptSuperscriptOffset.x = otm32->otmptSuperscriptOffset.x;
    lpOTM->otmptSuperscriptOffset.y = otm32->otmptSuperscriptOffset.y;
    lpOTM->otmsStrikeoutSize = otm32->otmsStrikeoutSize;
    lpOTM->otmsStrikeoutPosition = otm32->otmsStrikeoutPosition;
    lpOTM->otmsUnderscorePosition = otm32->otmsUnderscorePosition;
    lpOTM->otmsUnderscoreSize = otm32->otmsUnderscoreSize;
    lpOTM->otmpFamilyName = otm32->otmpFamilyName ? (SIZE_T)otm32->otmpFamilyName - offset : 0;
    lpOTM->otmpFaceName = otm32->otmpFaceName ? (SIZE_T)otm32->otmpFaceName - offset : 0;
    lpOTM->otmpStyleName = otm32->otmpStyleName ? (SIZE_T)otm32->otmpStyleName - offset : 0;
    lpOTM->otmpFullName = otm32->otmpFullName ? (SIZE_T)otm32->otmpFullName - offset : 0;
    memcpy((LPBYTE)lpOTM + sizeof(*lpOTM), (LPBYTE)otm32 + sizeof(*otm32), result - sizeof(*otm32));
    HeapFree(GetProcessHeap(), 0, otm32);
    return result - offset;
}


/***********************************************************************
 *           GetGlyphOutline    (GDI.309)
 */
DWORD WINAPI GetGlyphOutline16( HDC16 hdc, UINT16 uChar, UINT16 fuFormat,
                                LPGLYPHMETRICS16 lpgm, DWORD cbBuffer,
                                LPVOID lpBuffer, const MAT2 *lpmat2 )
{
    DWORD ret;
    GLYPHMETRICS gm32;

    ret = GetGlyphOutlineA( HDC_32(hdc), uChar, fuFormat, &gm32, cbBuffer, lpBuffer, lpmat2);
    if (ret && ret != GDI_ERROR)
    {
        lpgm->gmBlackBoxX = gm32.gmBlackBoxX;
        lpgm->gmBlackBoxY = gm32.gmBlackBoxY;
        lpgm->gmptGlyphOrigin.x = gm32.gmptGlyphOrigin.x;
        lpgm->gmptGlyphOrigin.y = gm32.gmptGlyphOrigin.y;
        lpgm->gmCellIncX = gm32.gmCellIncX;
        lpgm->gmCellIncY = gm32.gmCellIncY;
    }
    return ret;
}


/***********************************************************************
 *           CreateScalableFontResource   (GDI.310)
 */
BOOL16 WINAPI CreateScalableFontResource16( UINT16 fHidden, LPCSTR lpszResourceFile,
                                            LPCSTR fontFile, LPCSTR path )
{
    return CreateScalableFontResourceA( fHidden, lpszResourceFile, fontFile, path );
}


/*************************************************************************
 *             GetFontData    (GDI.311)
 *
 */
DWORD WINAPI GetFontData16( HDC16 hdc, DWORD table, DWORD offset, LPVOID buffer, DWORD count )
{
    return GetFontData( HDC_32(hdc), table, offset, buffer, count );
}


/*************************************************************************
 *             GetRasterizerCaps   (GDI.313)
 */
BOOL16 WINAPI GetRasterizerCaps16( LPRASTERIZER_STATUS lprs, UINT16 cbNumBytes )
{
    /* LPRASTERIZER_STATUS must be aligned */
    BYTE caps_buf[100] = { 0 };
    LPRASTERIZER_STATUS cap = caps_buf + (sizeof(SIZE_T) - (SIZE_T)caps_buf % sizeof(SIZE_T));
    if (GetRasterizerCaps(cap, cbNumBytes))
    {
        memcpy(lprs, cap, min(cbNumBytes, sizeof(RASTERIZER_STATUS)));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/***********************************************************************
 *             EnumFontFamilies    (GDI.330)
 */
INT16 WINAPI EnumFontFamilies16( HDC16 hDC, LPCSTR lpFamily,
                                 FONTENUMPROC16 efproc, LPARAM lpData )
{
    LOGFONT16 lf, *plf;

    if (lpFamily)
    {
        if (!*lpFamily) return 1;
        lstrcpynA( lf.lfFaceName, lpFamily, LF_FACESIZE );
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfPitchAndFamily = 0;
        plf = &lf;
    }
    else plf = NULL;

    return EnumFontFamiliesEx16( hDC, plf, efproc, lpData, 0 );
}


/*************************************************************************
 *             GetKerningPairs   (GDI.332)
 *
 */
INT16 WINAPI GetKerningPairs16( HDC16 hdc, INT16 count, LPKERNINGPAIR16 pairs )
{
    KERNINGPAIR *pairs32;
    INT i, ret;

    if (!count) return 0;

    if (!(pairs32 = HeapAlloc( GetProcessHeap(), 0, count * sizeof(*pairs32) ))) return 0;
    if ((ret = GetKerningPairsA( HDC_32(hdc), count, pairs32 )))
    {
        for (i = 0; i < ret; i++)
        {
            pairs->wFirst      = pairs32->wFirst;
            pairs->wSecond     = pairs32->wSecond;
            pairs->iKernAmount = pairs32->iKernAmount;
        }
    }
    HeapFree( GetProcessHeap(), 0, pairs32 );
    return ret;
}



/***********************************************************************
 *		GetTextAlign (GDI.345)
 */
UINT16 WINAPI GetTextAlign16( HDC16 hdc )
{
    return GetTextAlign( HDC_32(hdc) );
}


/***********************************************************************
 *           SetTextAlign    (GDI.346)
 */
UINT16 WINAPI SetTextAlign16( HDC16 hdc, UINT16 align )
{
    return SetTextAlign( HDC_32(hdc), align );
}


/***********************************************************************
 *           Chord    (GDI.348)
 */
BOOL16 WINAPI Chord16( HDC16 hdc, INT16 left, INT16 top,
                       INT16 right, INT16 bottom, INT16 xstart, INT16 ystart,
                       INT16 xend, INT16 yend )
{
    return Chord( HDC_32(hdc), left, top, right, bottom, xstart, ystart, xend, yend );
}


/***********************************************************************
 *           SetMapperFlags    (GDI.349)
 */
DWORD WINAPI SetMapperFlags16( HDC16 hdc, DWORD flags )
{
    return SetMapperFlags( HDC_32(hdc), flags );
}


/***********************************************************************
 *           GetCharWidth    (GDI.350)
 */
BOOL16 WINAPI GetCharWidth16( HDC16 hdc, UINT16 firstChar, UINT16 lastChar, LPINT16 buffer )
{
    BOOL retVal = FALSE;
    TEXTMETRIC tm = {0};
    HDC hdc32 = HDC_32(hdc);
    GetTextMetricsA(hdc32, &tm);

    if( firstChar != lastChar )
    {
        LPINT buf32 = HeapAlloc(GetProcessHeap(), 0, sizeof(INT)*(1 + (lastChar - firstChar)));
        if( buf32 )
        {
            LPINT obuf32 = buf32;
            UINT i;

            retVal = GetCharWidth32A( hdc32, firstChar, lastChar, buf32);
            if (retVal)
            {
                for (i = firstChar; i <= lastChar; i++) *buffer++ = (*buf32++) + tm.tmOverhang;
            }
            HeapFree(GetProcessHeap(), 0, obuf32);
        }
    }
    else /* happens quite often to warrant a special treatment */
    {
        INT chWidth;
        retVal = GetCharWidth32A( hdc32, firstChar, lastChar, &chWidth );
        *buffer = chWidth + tm.tmOverhang;
    }
    return retVal;
}


/***********************************************************************
 *           ExtTextOut   (GDI.351)
 */
BOOL16 WINAPI ExtTextOut16( HDC16 hdc, INT16 x, INT16 y, UINT16 flags,
                            const RECT16 *lprect, LPCSTR str, UINT16 count,
                            const INT16 *lpDx )
{
    BOOL        ret;
    int         i;
    RECT        rect32;
    LPINT       lpdx32 = NULL;

    if (lpDx) {
        lpdx32 = HeapAlloc( GetProcessHeap(),0, sizeof(INT)*count );
        if(lpdx32 == NULL) return FALSE;
        for (i=count;i--;) lpdx32[i]=lpDx[i];
    }
    if (lprect)
    {
        rect32.left   = lprect->left;
        rect32.top    = lprect->top;
        rect32.right  = lprect->right;
        rect32.bottom = lprect->bottom;
    }
    ret = ExtTextOutA(HDC_32(hdc), x, y, flags, lprect ? &rect32 : NULL, str, count, lpdx32);
    HeapFree( GetProcessHeap(), 0, lpdx32 );
    return ret;
}


/***********************************************************************
 *           CreatePalette    (GDI.360)
 */
HPALETTE16 WINAPI CreatePalette16( const LOGPALETTE* palette )
{
    return HPALETTE_16( CreatePalette( palette ) );
}


/***********************************************************************
 *           GDISelectPalette   (GDI.361)
 */
HPALETTE16 WINAPI GDISelectPalette16( HDC16 hdc, HPALETTE16 hpalette, WORD wBkg )
{
    HPALETTE16 ret = HPALETTE_16( SelectPalette( HDC_32(hdc), HPALETTE_32(hpalette), wBkg ));
    if (ret && !wBkg) hPrimaryPalette = hpalette;
    return ret;
}


/***********************************************************************
 *           GDIRealizePalette   (GDI.362)
 */
UINT16 WINAPI GDIRealizePalette16( HDC16 hdc )
{
    return RealizePalette( HDC_32(hdc) );
}


/***********************************************************************
 *           GetPaletteEntries    (GDI.363)
 */
UINT16 WINAPI GetPaletteEntries16( HPALETTE16 hpalette, UINT16 start,
                                   UINT16 count, LPPALETTEENTRY entries )
{
    return GetPaletteEntries( HPALETTE_32(hpalette), start, count, entries );
}


/***********************************************************************
 *           SetPaletteEntries    (GDI.364)
 */
UINT16 WINAPI SetPaletteEntries16( HPALETTE16 hpalette, UINT16 start,
                                   UINT16 count, const PALETTEENTRY *entries )
{
    return SetPaletteEntries( HPALETTE_32(hpalette), start, count, entries );
}


/**********************************************************************
 *            UpdateColors   (GDI.366)
 */
INT16 WINAPI UpdateColors16( HDC16 hdc )
{
    UpdateColors( HDC_32(hdc) );
    return TRUE;
}


/***********************************************************************
 *           AnimatePalette   (GDI.367)
 */
void WINAPI AnimatePalette16( HPALETTE16 hpalette, UINT16 StartIndex,
                              UINT16 NumEntries, const PALETTEENTRY* PaletteColors)
{
    HPALETTE hpal32 = HPALETTE_32(hpalette);
    if (GetObjectType(hpal32) != OBJ_PAL) return;
    AnimatePalette( hpal32, StartIndex, NumEntries, PaletteColors );
    if (GetPtr16(hpalette, 1))
    {
        HWND16 *hwlist = GetPtr16(hpalette, 1);
        for (int i = 0; i < 10; i++)
        {
            if (hwlist[i])
            {
                HWND hwnd = HWND_32(hwlist[i]);
                InvalidateRect(hwnd, NULL, FALSE);
                //UpdateWindow(hwnd);
            }
        }
    }
}


/***********************************************************************
 *           ResizePalette   (GDI.368)
 */
BOOL16 WINAPI ResizePalette16( HPALETTE16 hpalette, UINT16 cEntries )
{
    return ResizePalette( HPALETTE_32(hpalette), cEntries );
}


/***********************************************************************
 *           GetNearestPaletteIndex   (GDI.370)
 */
UINT16 WINAPI GetNearestPaletteIndex16( HPALETTE16 hpalette, COLORREF color )
{
    return GetNearestPaletteIndex( HPALETTE_32(hpalette), check_colorref(color) );
}


/**********************************************************************
 *          ExtFloodFill   (GDI.372)
 */
BOOL16 WINAPI ExtFloodFill16( HDC16 hdc, INT16 x, INT16 y, COLORREF color,
                              UINT16 fillType )
{
    return ExtFloodFill( HDC_32(hdc), x, y, check_colorref(color), fillType );
}


static UINT16 syspaluse = SYSPAL_STATIC;
/***********************************************************************
 *           SetSystemPaletteUse   (GDI.373)
 */
UINT16 WINAPI SetSystemPaletteUse16( HDC16 hdc, UINT16 use )
{
    syspaluse = use;
    return SetSystemPaletteUse( HDC_32(hdc), use );
}


/***********************************************************************
 *           GetSystemPaletteUse   (GDI.374)
 */
UINT16 WINAPI GetSystemPaletteUse16( HDC16 hdc )
{
    UINT16 ret = GetSystemPaletteUse( HDC_32(hdc) );
    if (!ret && krnl386_get_compat_mode("256color"))
         ret = syspaluse;
    return ret;
}


/***********************************************************************
 *           GetSystemPaletteEntries   (GDI.375)
 */
UINT16 WINAPI GetSystemPaletteEntries16( HDC16 hdc, UINT16 start, UINT16 count,
                                         LPPALETTEENTRY entries )
{
    return GetSystemPaletteEntries( HDC_32(hdc), start, count, entries );
}


/***********************************************************************
 *           ResetDC    (GDI.376)
 */
HDC16 WINAPI ResetDC16( HDC16 hdc, const DEVMODEA *devmode )
{
    DEVMODEA dma = {0};
    if (!IsValidDevmodeA(devmode, devmode->dmSize + devmode->dmDriverExtra))
        return NULL;
    memcpy(&dma, devmode, devmode->dmSize);
    dma.dmSize = sizeof(DEVMODEA);
    dma.dmDriverExtra = 0;
    if (!dma.dmDeviceName[0]) dma.dmDeviceName[0] = '.';
    return HDC_16( ResetDCA( HDC_32(hdc), &dma ) );
}


/******************************************************************
 *           StartDoc   (GDI.377)
 */
INT16 WINAPI StartDoc16( HDC16 hdc, const DOCINFO16 *lpdoc )
{
    DOCINFOA docA;

    docA.cbSize = lpdoc->cbSize;
    docA.lpszDocName = MapSL(lpdoc->lpszDocName);
    docA.lpszOutput = MapSL(lpdoc->lpszOutput);
    if(lpdoc->cbSize > offsetof(DOCINFO16,lpszDatatype))
        docA.lpszDatatype = MapSL(lpdoc->lpszDatatype);
    else
        docA.lpszDatatype = NULL;
    if(lpdoc->cbSize > offsetof(DOCINFO16,fwType))
        docA.fwType = lpdoc->fwType;
    else
        docA.fwType = 0;
    return StartDocA( HDC_32(hdc), &docA );
}


/******************************************************************
 *           EndDoc   (GDI.378)
 */
INT16 WINAPI EndDoc16( HDC16 hdc )
{
    return EndDoc( HDC_32(hdc) );
}


/******************************************************************
 *           StartPage   (GDI.379)
 */
INT16 WINAPI StartPage16( HDC16 hdc )
{
    return StartPage( HDC_32(hdc) );
}


/******************************************************************
 *           EndPage   (GDI.380)
 */
INT16 WINAPI EndPage16( HDC16 hdc )
{
    return EndPage( HDC_32(hdc) );
}


/******************************************************************************
 *           AbortDoc   (GDI.382)
 */
INT16 WINAPI AbortDoc16( HDC16 hdc )
{
    return AbortDoc( HDC_32(hdc) );
}


/***********************************************************************
 *           FastWindowFrame    (GDI.400)
 */
BOOL16 WINAPI FastWindowFrame16( HDC16 hdc, const RECT16 *rect,
                               INT16 width, INT16 height, DWORD rop )
{
    HDC hdc32 = HDC_32(hdc);
    HBRUSH hbrush = SelectObject( hdc32, GetStockObject( GRAY_BRUSH ) );
    PatBlt( hdc32, rect->left, rect->top,
            rect->right - rect->left - width, height, rop );
    PatBlt( hdc32, rect->left, rect->top + height, width,
            rect->bottom - rect->top - height, rop );
    PatBlt( hdc32, rect->left + width, rect->bottom - 1,
            rect->right - rect->left - width, -height, rop );
    PatBlt( hdc32, rect->right - 1, rect->top, -width,
            rect->bottom - rect->top - height, rop );
    SelectObject( hdc32, hbrush );
    return TRUE;
}


/***********************************************************************
 *           GdiInit2     (GDI.403)
 *
 * See "Undocumented Windows"
 *
 * PARAMS
 *   h1 [I] GDI object
 *   h2 [I] global data
 */
HANDLE16 WINAPI GdiInit216( HANDLE16 h1, HANDLE16 h2 )
{
    FIXME("(%04x, %04x), stub.\n", h1, h2);
    if (h2 == 0xffff) return 0xffff; /* undefined return value */
    return h1; /* FIXME: should be the memory handle of h1 */
}


/***********************************************************************
 *           FinalGdiInit     (GDI.405)
 */
void WINAPI FinalGdiInit16( HBRUSH16 hPattern /* [in] fill pattern of desktop */ )
{
}


/***********************************************************************
 *           CreateUserBitmap    (GDI.407)
 */
HBITMAP16 WINAPI CreateUserBitmap16( INT16 width, INT16 height, UINT16 planes,
                                     UINT16 bpp, LPCVOID bits )
{
    return CreateBitmap16( width, height, planes, bpp, bits );
}


/***********************************************************************
 *           CreateUserDiscardableBitmap    (GDI.409)
 */
HBITMAP16 WINAPI CreateUserDiscardableBitmap16( WORD dummy, INT16 width, INT16 height )
{
    HDC hdc = CreateDCA( "DISPLAY", NULL, NULL, NULL );
    HBITMAP ret = CreateCompatibleBitmap( hdc, width, height );
    DeleteDC( hdc );
    return HBITMAP_16(ret);
}


/***********************************************************************
 *		GetCurLogFont (GDI.411)
 */
HFONT16 WINAPI GetCurLogFont16( HDC16 hdc )
{
    return HFONT_16( GetCurrentObject( HDC_32(hdc), OBJ_FONT ) );
}

static DWORD rle_size(int type, const VOID *bits)
{
    DWORD ret = 0;
    __TRY
    {
        DWORD offset = 0;
        BYTE *data = (BYTE *)bits;
        BOOL loop = TRUE;
        do
        {
            if (!data[offset++])
            {
                BYTE byte = data[offset++];
                switch (byte)
                {
                    case 0:
                        break;
                    case 1:
                        ret = offset;
                        loop = FALSE;
                        break;
                    default:
                        offset += byte / type;
                        offset = (offset + 1) & ~1;
                        break;
                }
            }
            else
                offset++;
        }
        while (loop);
    }
    __EXCEPT_ALL
    {
        ERR("bad bitmap, type %d", type);
    }
    __ENDTRY
    return ret;
}


/***********************************************************************
 *           StretchDIBits   (GDI.439)
 */
INT16 WINAPI StretchDIBits16( HDC16 hdc, INT16 xDst, INT16 yDst, INT16 widthDst,
                              INT16 heightDst, INT16 xSrc, INT16 ySrc, INT16 widthSrc,
                              INT16 heightSrc, const VOID *bits,
                              const BITMAPINFO *info, UINT16 wUsage, DWORD dwRop )
{
    BITMAPINFO *bmp = NULL;
    HBITMAP16 ret;
    if ((info->bmiHeader.biCompression == BI_RLE4) || (info->bmiHeader.biCompression == BI_RLE8))
    {
        int hdrsize = info->bmiHeader.biSize + ((info->bmiHeader.biClrUsed ? info->bmiHeader.biClrUsed :
                        (info->bmiHeader.biBitCount == 4 ? 16 : 256)) * (wUsage == DIB_PAL_COLORS ? 2 : 4));

        bmp = HeapAlloc(GetProcessHeap(), 0, hdrsize);
        memcpy(bmp, info, hdrsize);
        bmp->bmiHeader.biSizeImage = rle_size(info->bmiHeader.biCompression, bits);
    }
    ret = StretchDIBits( HDC_32(hdc), xDst, yDst, widthDst, heightDst,
                          xSrc, ySrc, widthSrc, heightSrc, bits,
                          bmp ? bmp : info, wUsage, dwRop );
    if (bmp)
        HeapFree(GetProcessHeap(), 0, bmp);
    return ret;
}


/***********************************************************************
 *           SetDIBits    (GDI.440)
 */
INT16 WINAPI SetDIBits16( HDC16 hdc, HBITMAP16 hbitmap, UINT16 startscan,
                          UINT16 lines, LPCVOID bits, const BITMAPINFO *info,
                          UINT16 coloruse )
{
    HBITMAP hbitmap32 = HBITMAP_32(hbitmap);
    if (krnl386_get_compat_mode("256color"))
    {
        // the conversion from 8bpp->1 on winxp+ (even in 256 color mode) is different than win31/95 and wine
        // the problem shows in The Even More Incredible Machine where the sprites are almost completely masked out
        // this does it like wine
        // TODO: other wrong color conversions, top down dibs
        BITMAP bmap;
        int ret = GetObject(hbitmap32, sizeof(BITMAP), &bmap);
        if ((bmap.bmPlanes == 1) && (bmap.bmBitsPixel == 1) && (info->bmiHeader.biBitCount == 8) &&
            (info->bmiHeader.biPlanes == 1) && (info->bmiHeader.biCompression == BI_RGB) && (coloruse == DIB_RGB_COLORS))
        {
            int bytewidth = (info->bmiHeader.biWidth + 3) & ~3;
            HDC hdc32 = CreateCompatibleDC(HDC_32(hdc));
            SelectObject(hdc32, hbitmap32);
            int start = bmap.bmHeight - lines - startscan;
            for (int y = 0; y < lines; y++)
            {
                for (int x = 0; x < info->bmiHeader.biWidth; x++)
                {
                    RGBQUAD *color = &info->bmiColors[((unsigned char *)bits)[((lines - y - 1) * bytewidth) + x]];
                    if (SetPixel(hdc32, x, y + start, RGB(color->rgbRed, color->rgbGreen, color->rgbBlue)) == -1)
                    {
                        DeleteDC(hdc32);
                        return y;
                    }
                }
            }
            DeleteDC(hdc32);
            return lines;
        }
    }
    BITMAPINFO *bmp = NULL;
    HBITMAP16 ret;
    if (((info->bmiHeader.biCompression == BI_RLE4) || (info->bmiHeader.biCompression == BI_RLE8)) && !info->bmiHeader.biSizeImage)
    {
        int hdrsize = info->bmiHeader.biSize + ((info->bmiHeader.biClrUsed ? info->bmiHeader.biClrUsed :
                        (info->bmiHeader.biBitCount == 4 ? 16 : 256)) * (coloruse == DIB_PAL_COLORS ? 2 : 4));

        bmp = HeapAlloc(GetProcessHeap(), 0, hdrsize);
        memcpy(bmp, info, hdrsize);
        bmp->bmiHeader.biSizeImage = rle_size(info->bmiHeader.biCompression, bits);
    }

    ret = SetDIBits( HDC_32(hdc), hbitmap32, startscan, lines, bits, bmp ? bmp : info, coloruse );
    if (bmp)
        HeapFree(GetProcessHeap(), 0, bmp);
    return ret;
}


/***********************************************************************
 *           GetDIBits    (GDI.441)
 */
INT16 WINAPI GetDIBits16( HDC16 hdc, HBITMAP16 hbitmap, UINT16 startscan,
                          UINT16 lines, LPVOID bits, BITMAPINFO * info,
                          UINT16 coloruse )
{
    if (lines > info->bmiHeader.biHeight)
    {
        lines = info->bmiHeader.biHeight;
    }
    return GetDIBits( HDC_32(hdc), HBITMAP_32(hbitmap), startscan, lines, bits, info, coloruse );
}


/***********************************************************************
 *           CreateDIBitmap    (GDI.442)
 */
HBITMAP16 WINAPI CreateDIBitmap16( HDC16 hdc, const BITMAPINFOHEADER * header,
                                   DWORD init, LPCVOID bits, const BITMAPINFO * data,
                                   UINT16 coloruse )
{
    BITMAPINFO *bmp = NULL;
    HBITMAP16 ret;
    if ((data->bmiHeader.biCompression == BI_RLE4) || (data->bmiHeader.biCompression == BI_RLE8))
    {
        int hdrsize = data->bmiHeader.biSize + ((data->bmiHeader.biClrUsed ? data->bmiHeader.biClrUsed :
                        (data->bmiHeader.biBitCount == 4 ? 16 : 256)) * (coloruse == DIB_PAL_COLORS ? 2 : 4));

        bmp = HeapAlloc(GetProcessHeap(), 0, hdrsize);
        memcpy(bmp, data, hdrsize);
        bmp->bmiHeader.biSizeImage = rle_size(data->bmiHeader.biCompression, bits);
    }
    ret = HBITMAP_16( CreateDIBitmap( HDC_32(hdc), header, init, bits, bmp ? bmp : data, coloruse ) );
    if (bmp)
        HeapFree(GetProcessHeap(), 0, bmp);
    return ret;
}


/***********************************************************************
 *           SetDIBitsToDevice    (GDI.443)
 */
INT16 WINAPI SetDIBitsToDevice16( HDC16 hdc, INT16 xDest, INT16 yDest, INT16 cx,
                                  INT16 cy, INT16 xSrc, INT16 ySrc, UINT16 startscan,
                                  UINT16 lines, LPCVOID bits, const BITMAPINFO *info,
                                  UINT16 coloruse )
{
    return SetDIBitsToDevice( HDC_32(hdc), xDest, yDest, cx, cy, xSrc, ySrc,
                              startscan, lines, bits, info, coloruse );
}


/***********************************************************************
 *           CreateRoundRectRgn    (GDI.444)
 *
 * If either ellipse dimension is zero we call CreateRectRgn16 for its
 * `special' behaviour. -ve ellipse dimensions can result in GPFs under win3.1
 * we just let CreateRoundRectRgn convert them to +ve values.
 */

HRGN16 WINAPI CreateRoundRectRgn16( INT16 left, INT16 top, INT16 right, INT16 bottom,
                                    INT16 ellipse_width, INT16 ellipse_height )
{
    if( ellipse_width == 0 || ellipse_height == 0 )
        return CreateRectRgn16( left, top, right, bottom );
    else
        return HRGN_16( CreateRoundRectRgn( left, top, right, bottom,
                                            ellipse_width, ellipse_height ));
}


/***********************************************************************
 *           CreateDIBPatternBrush    (GDI.445)
 */
HBRUSH16 WINAPI CreateDIBPatternBrush16( HGLOBAL16 hbitmap, UINT16 coloruse )
{
    BITMAPINFO *bmi;
    HBRUSH16 ret;

    if (!(bmi = GlobalLock16( hbitmap ))) return 0;
    ret = HBRUSH_16( CreateDIBPatternBrushPt( bmi, coloruse ));
    GlobalUnlock16( hbitmap );
    return ret;
}


/**********************************************************************
 *          PolyPolygon (GDI.450)
 */
BOOL16 WINAPI PolyPolygon16( HDC16 hdc, const POINT16* pt, const INT16* counts,
                             UINT16 polygons )
{
    int         i,nrpts;
    LPPOINT     pt32;
    LPINT       counts32;
    BOOL16      ret;

    nrpts=0;
    for (i=polygons;i--;)
        nrpts+=counts[i];
    pt32 = HeapAlloc( GetProcessHeap(), 0, sizeof(POINT)*nrpts);
    if(pt32 == NULL) return FALSE;
    for (i=nrpts;i--;)
    {
        pt32[i].x = pt[i].x;
        pt32[i].y = pt[i].y;
    }
    counts32 = HeapAlloc( GetProcessHeap(), 0, polygons*sizeof(INT) );
    if(counts32 == NULL) {
        HeapFree( GetProcessHeap(), 0, pt32 );
        return FALSE;
    }
    for (i=polygons;i--;) counts32[i]=counts[i];

    ret = PolyPolygon(HDC_32(hdc),pt32,counts32,polygons);
    HeapFree( GetProcessHeap(), 0, counts32 );
    HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/***********************************************************************
 *           CreatePolyPolygonRgn    (GDI.451)
 */
HRGN16 WINAPI CreatePolyPolygonRgn16( const POINT16 *points,
                                      const INT16 *count, INT16 nbpolygons, INT16 mode )
{
    HRGN hrgn;
    int i, npts = 0;
    INT *count32;
    POINT *points32;

    for (i = 0; i < nbpolygons; i++) npts += count[i];
    points32 = HeapAlloc( GetProcessHeap(), 0, npts * sizeof(POINT) );
    for (i = 0; i < npts; i++)
    {
        points32[i].x = points[i].x;
        points32[i].y = points[i].y;
    }

    count32 = HeapAlloc( GetProcessHeap(), 0, nbpolygons * sizeof(INT) );
    for (i = 0; i < nbpolygons; i++) count32[i] = count[i];
    hrgn = CreatePolyPolygonRgn( points32, count32, nbpolygons, mode );
    HeapFree( GetProcessHeap(), 0, count32 );
    HeapFree( GetProcessHeap(), 0, points32 );
    return HRGN_16(hrgn);
}


/***********************************************************************
 *           GdiSeeGdiDo   (GDI.452)
 */
DWORD WINAPI GdiSeeGdiDo16( WORD wReqType, WORD wParam1, WORD wParam2,
                          WORD wParam3 )
{
    DWORD ret = ~0U;

    switch (wReqType)
    {
    case 0x0001:  /* LocalAlloc */
        WARN("LocalAlloc16(%x, %x): ignoring\n", wParam1, wParam3);
        ret = 0;
        break;
    case 0x0002:  /* LocalFree */
        WARN("LocalFree16(%x): ignoring\n", wParam1);
        ret = 0;
        break;
    case 0x0003:  /* LocalCompact */
        WARN("LocalCompact16(%x): ignoring\n", wParam3);
        ret = 65000; /* lie about the amount of free space */
        break;
    case 0x0103:  /* LocalHeap */
        WARN("LocalHeap16(): ignoring\n");
        break;
    default:
        WARN("(wReqType=%04x): Unknown\n", wReqType);
        break;
    }
    return ret;
}


/***********************************************************************
 *           SetObjectOwner    (GDI.461)
 */
void WINAPI SetObjectOwner16( HGDIOBJ16 handle, HANDLE16 owner )
{
    /* Nothing to do */
}


/***********************************************************************
 *           IsGDIObject    (GDI.462)
 *
 * returns type of object if valid (W95 system programming secrets p. 264-5)
 */
BOOL16 WINAPI IsGDIObject16( HGDIOBJ16 handle16 )
{
    static const BYTE type_map[] =
    {
        0,  /* bad */
        1,  /* OBJ_PEN */
        2,  /* OBJ_BRUSH */
        7,  /* OBJ_DC */
        9,  /* OBJ_METADC */
        4,  /* OBJ_PAL */
        3,  /* OBJ_FONT */
        5,  /* OBJ_BITMAP */
        6,  /* OBJ_REGION */
        10, /* OBJ_METAFILE */
        7,  /* OBJ_MEMDC */
        0,  /* OBJ_EXTPEN */
        9,  /* OBJ_ENHMETADC */
        12, /* OBJ_ENHMETAFILE */
        0   /* OBJ_COLORSPACE */
    };

    UINT type = GetObjectType( HGDIOBJ_32( handle16 ));

    if (type >= ARRAY_SIZE(type_map)) return FALSE;
    return type_map[type];
}


/***********************************************************************
 *           RectVisible    (GDI.465)
 *           RectVisibleOld (GDI.104)
 */
BOOL16 WINAPI RectVisible16( HDC16 hdc, const RECT16* rect16 )
{
    RECT rect;

    rect.left   = rect16->left;
    rect.top    = rect16->top;
    rect.right  = rect16->right;
    rect.bottom = rect16->bottom;
    return RectVisible( HDC_32(hdc), &rect );
}


/***********************************************************************
 *           RectInRegion    (GDI.466)
 *           RectInRegionOld (GDI.181)
 */
BOOL16 WINAPI RectInRegion16( HRGN16 hrgn, const RECT16 *rect )
{
    RECT r32;

    r32.left   = rect->left;
    r32.top    = rect->top;
    r32.right  = rect->right;
    r32.bottom = rect->bottom;
    return RectInRegion( HRGN_32(hrgn), &r32 );
}


/***********************************************************************
 *           GetBitmapDimensionEx    (GDI.468)
 */
BOOL16 WINAPI GetBitmapDimensionEx16( HBITMAP16 hbitmap, LPSIZE16 size )
{
    SIZE size32;
    BOOL ret = GetBitmapDimensionEx( HBITMAP_32(hbitmap), &size32 );

    if (ret)
    {
        size->cx = size32.cx;
        size->cy = size32.cy;
    }
    return ret;
}


/***********************************************************************
 *		GetBrushOrgEx (GDI.469)
 */
BOOL16 WINAPI GetBrushOrgEx16( HDC16 hdc, LPPOINT16 pt )
{
    POINT pt32;
    if (!GetBrushOrgEx( HDC_32(hdc), &pt32 )) return FALSE;
    pt->x = pt32.x;
    pt->y = pt32.y;
    return TRUE;
}


/***********************************************************************
 *		GetCurrentPositionEx (GDI.470)
 */
BOOL16 WINAPI GetCurrentPositionEx16( HDC16 hdc, LPPOINT16 pt )
{
    POINT pt32;
    if (!GetCurrentPositionEx( HDC_32(hdc), &pt32 )) return FALSE;
    pt->x = pt32.x;
    pt->y = pt32.y;
    return TRUE;
}


/***********************************************************************
 *           GetTextExtentPoint    (GDI.471)
 *
 * FIXME: Should this have a bug for compatibility?
 * Original Windows versions of GetTextExtentPoint{A,W} have documented
 * bugs (-> MSDN KB q147647.txt).
 */
BOOL16 WINAPI GetTextExtentPoint16( HDC16 hdc, LPCSTR str, INT16 count, LPSIZE16 size )
{
    SIZE size32;
    HDC hdc32 = HDC_32(hdc);
    TEXTMETRICA tm;
    GetTextMetricsA(hdc32, &tm);
    BOOL ret = GetTextExtentPoint32A( hdc32, str, count, &size32 );

    if (ret)
    {
        size->cx = size32.cx + tm.tmOverhang;
        size->cy = size32.cy;
        check_font_rotation( hdc32, size ); 
    }
    return ret;
}


/***********************************************************************
 *		GetViewportExtEx (GDI.472)
 */
BOOL16 WINAPI GetViewportExtEx16( HDC16 hdc, LPSIZE16 size )
{
    SIZE size32;
    if (!GetViewportExtEx( HDC_32(hdc), &size32 )) return FALSE;
    size->cx = size32.cx;
    size->cy = size32.cy;
    return TRUE;
}


/***********************************************************************
 *		GetViewportOrgEx (GDI.473)
 */
BOOL16 WINAPI GetViewportOrgEx16( HDC16 hdc, LPPOINT16 pt )
{
    POINT pt32;
    if (!GetViewportOrgEx( HDC_32(hdc), &pt32 )) return FALSE;
    pt->x = pt32.x;
    pt->y = pt32.y;
    return TRUE;
}


/***********************************************************************
 *		GetWindowExtEx (GDI.474)
 */
BOOL16 WINAPI GetWindowExtEx16( HDC16 hdc, LPSIZE16 size )
{
    SIZE size32;
    if (!GetWindowExtEx( HDC_32(hdc), &size32 )) return FALSE;
    size->cx = size32.cx;
    size->cy = size32.cy;
    return TRUE;
}


/***********************************************************************
 *		GetWindowOrgEx (GDI.475)
 */
BOOL16 WINAPI GetWindowOrgEx16( HDC16 hdc, LPPOINT16 pt )
{
    POINT pt32;
    if (!GetWindowOrgEx( HDC_32(hdc), &pt32 )) return FALSE;
    pt->x = pt32.x;
    pt->y = pt32.y;
    return TRUE;
}


/***********************************************************************
 *           OffsetViewportOrgEx    (GDI.476)
 */
BOOL16 WINAPI OffsetViewportOrgEx16( HDC16 hdc, INT16 x, INT16 y, LPPOINT16 pt)
{
    POINT pt32;
    BOOL16 ret = OffsetViewportOrgEx( HDC_32(hdc), x, y, &pt32 );
    if (pt)
    {
        pt->x = pt32.x;
        pt->y = pt32.y;
    }
    return ret;
}


/***********************************************************************
 *           OffsetWindowOrgEx    (GDI.477)
 */
BOOL16 WINAPI OffsetWindowOrgEx16( HDC16 hdc, INT16 x, INT16 y, LPPOINT16 pt )
{
    POINT pt32;
    BOOL16 ret = OffsetWindowOrgEx( HDC_32(hdc), x, y, &pt32 );
    if (pt)
    {
        pt->x = pt32.x;
        pt->y = pt32.y;
    }
    return ret;
}


/***********************************************************************
 *           SetBitmapDimensionEx    (GDI.478)
 */
BOOL16 WINAPI SetBitmapDimensionEx16( HBITMAP16 hbitmap, INT16 x, INT16 y, LPSIZE16 prevSize )
{
    SIZE size32;
    BOOL ret = SetBitmapDimensionEx( HBITMAP_32(hbitmap), x, y, &size32 );

    if (ret && prevSize)
    {
        prevSize->cx = size32.cx;
        prevSize->cy = size32.cy;
    }
    return ret;
}


/***********************************************************************
 *           SetViewportExtEx    (GDI.479)
 */
BOOL16 WINAPI SetViewportExtEx16( HDC16 hdc, INT16 x, INT16 y, LPSIZE16 size )
{
    SIZE size32;
    BOOL16 ret = SetViewportExtEx( HDC_32(hdc), x, y, &size32 );
    if (size) { size->cx = size32.cx; size->cy = size32.cy; }
    return ret;
}


/***********************************************************************
 *           SetViewportOrgEx    (GDI.480)
 */
BOOL16 WINAPI SetViewportOrgEx16( HDC16 hdc, INT16 x, INT16 y, LPPOINT16 pt )
{
    POINT pt32;
    BOOL16 ret = SetViewportOrgEx( HDC_32(hdc), x, y, &pt32 );
    if (pt)
    {
        pt->x = pt32.x;
        pt->y = pt32.y;
    }
    return ret;
}


/***********************************************************************
 *           SetWindowExtEx    (GDI.481)
 */
BOOL16 WINAPI SetWindowExtEx16( HDC16 hdc, INT16 x, INT16 y, LPSIZE16 size )
{
    SIZE size32;
    BOOL16 ret = SetWindowExtEx( HDC_32(hdc), x, y, &size32 );
    if (size) { size->cx = size32.cx; size->cy = size32.cy; }
    return ret;
}


/***********************************************************************
 *           SetWindowOrgEx    (GDI.482)
 */
BOOL16 WINAPI SetWindowOrgEx16( HDC16 hdc, INT16 x, INT16 y, LPPOINT16 pt )
{
    POINT pt32;
    BOOL16 ret = SetWindowOrgEx( HDC_32(hdc), x, y, &pt32 );
    if (pt)
    {
        pt->x = pt32.x;
        pt->y = pt32.y;
    }
    return ret;
}


/***********************************************************************
 *           MoveToEx    (GDI.483)
 */
BOOL16 WINAPI MoveToEx16( HDC16 hdc, INT16 x, INT16 y, LPPOINT16 pt )
{
    POINT pt32;

    if (!MoveToEx( HDC_32(hdc), x, y, &pt32 )) return FALSE;
    if (pt)
    {
        pt->x = pt32.x;
        pt->y = pt32.y;
    }
    return TRUE;
}


/***********************************************************************
 *           ScaleViewportExtEx    (GDI.484)
 */
BOOL16 WINAPI ScaleViewportExtEx16( HDC16 hdc, INT16 xNum, INT16 xDenom,
                                    INT16 yNum, INT16 yDenom, LPSIZE16 size )
{
    SIZE size32;
    BOOL16 ret = ScaleViewportExtEx( HDC_32(hdc), xNum, xDenom, yNum, yDenom,
                                       &size32 );
    if (size) { size->cx = size32.cx; size->cy = size32.cy; }
    return ret;
}


/***********************************************************************
 *           ScaleWindowExtEx    (GDI.485)
 */
BOOL16 WINAPI ScaleWindowExtEx16( HDC16 hdc, INT16 xNum, INT16 xDenom,
                                  INT16 yNum, INT16 yDenom, LPSIZE16 size )
{
    SIZE size32;
    BOOL16 ret = ScaleWindowExtEx( HDC_32(hdc), xNum, xDenom, yNum, yDenom,
                                     &size32 );
    if (size) { size->cx = size32.cx; size->cy = size32.cy; }
    return ret;
}


/***********************************************************************
 *           GetAspectRatioFilterEx  (GDI.486)
 */
BOOL16 WINAPI GetAspectRatioFilterEx16( HDC16 hdc, LPSIZE16 pAspectRatio )
{
    FIXME("(%04x, %p): -- Empty Stub !\n", hdc, pAspectRatio);
    return FALSE;
}


/******************************************************************************
 *           PolyBezier  (GDI.502)
 */
BOOL16 WINAPI PolyBezier16( HDC16 hdc, const POINT16* lppt, INT16 cPoints )
{
    int i;
    BOOL16 ret;
    LPPOINT pt32 = HeapAlloc( GetProcessHeap(), 0, cPoints*sizeof(POINT) );
    if(!pt32) return FALSE;
    for (i=cPoints;i--;)
    {
        pt32[i].x = lppt[i].x;
        pt32[i].y = lppt[i].y;
    }
    ret= PolyBezier(HDC_32(hdc), pt32, cPoints);
    HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/******************************************************************************
 *           PolyBezierTo  (GDI.503)
 */
BOOL16 WINAPI PolyBezierTo16( HDC16 hdc, const POINT16* lppt, INT16 cPoints )
{
    int i;
    BOOL16 ret;
    LPPOINT pt32 = HeapAlloc( GetProcessHeap(), 0,
                                           cPoints*sizeof(POINT) );
    if(!pt32) return FALSE;
    for (i=cPoints;i--;)
    {
        pt32[i].x = lppt[i].x;
        pt32[i].y = lppt[i].y;
    }
    ret= PolyBezierTo(HDC_32(hdc), pt32, cPoints);
    HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/******************************************************************************
 *           ExtSelectClipRgn   (GDI.508)
 */
INT16 WINAPI ExtSelectClipRgn16( HDC16 hdc, HRGN16 hrgn, INT16 fnMode )
{
  return ExtSelectClipRgn( HDC_32(hdc), HRGN_32(hrgn), fnMode);
}


/***********************************************************************
 *           AbortPath    (GDI.511)
 */
BOOL16 WINAPI AbortPath16(HDC16 hdc)
{
    return AbortPath( HDC_32(hdc) );
}


/***********************************************************************
 *           BeginPath    (GDI.512)
 */
BOOL16 WINAPI BeginPath16(HDC16 hdc)
{
    return BeginPath( HDC_32(hdc) );
}


/***********************************************************************
 *           CloseFigure    (GDI.513)
 */
BOOL16 WINAPI CloseFigure16(HDC16 hdc)
{
    return CloseFigure( HDC_32(hdc) );
}


/***********************************************************************
 *           EndPath    (GDI.514)
 */
BOOL16 WINAPI EndPath16(HDC16 hdc)
{
    return EndPath( HDC_32(hdc) );
}


/***********************************************************************
 *           FillPath    (GDI.515)
 */
BOOL16 WINAPI FillPath16(HDC16 hdc)
{
    return FillPath( HDC_32(hdc) );
}


/*******************************************************************
 *           FlattenPath    (GDI.516)
 */
BOOL16 WINAPI FlattenPath16(HDC16 hdc)
{
    return FlattenPath( HDC_32(hdc) );
}


/***********************************************************************
 *           GetPath    (GDI.517)
 */
INT16 WINAPI GetPath16(HDC16 hdc, LPPOINT16 pPoints, LPBYTE pTypes, INT16 nSize)
{
    FIXME("(%d,%p,%p): stub\n",hdc,pPoints,pTypes);
    return 0;
}


/***********************************************************************
 *           PathToRegion    (GDI.518)
 */
HRGN16 WINAPI PathToRegion16(HDC16 hdc)
{
    return HRGN_16( PathToRegion( HDC_32(hdc) ));
}


/***********************************************************************
 *           SelectClipPath    (GDI.519)
 */
BOOL16 WINAPI SelectClipPath16(HDC16 hdc, INT16 iMode)
{
    return SelectClipPath( HDC_32(hdc), iMode );
}


/*******************************************************************
 *           StrokeAndFillPath    (GDI.520)
 */
BOOL16 WINAPI StrokeAndFillPath16(HDC16 hdc)
{
    return StrokeAndFillPath( HDC_32(hdc) );
}


/*******************************************************************
 *           StrokePath    (GDI.521)
 */
BOOL16 WINAPI StrokePath16(HDC16 hdc)
{
    return StrokePath( HDC_32(hdc) );
}


/*******************************************************************
 *           WidenPath    (GDI.522)
 */
BOOL16 WINAPI WidenPath16(HDC16 hdc)
{
    return WidenPath( HDC_32(hdc) );
}


/***********************************************************************
 *		GetArcDirection (GDI.524)
 */
INT16 WINAPI GetArcDirection16( HDC16 hdc )
{
    return GetArcDirection( HDC_32(hdc) );
}


/***********************************************************************
 *           SetArcDirection    (GDI.525)
 */
INT16 WINAPI SetArcDirection16( HDC16 hdc, INT16 nDirection )
{
    return SetArcDirection( HDC_32(hdc), (INT)nDirection );
}


/***********************************************************************
 *           CreateHalftonePalette (GDI.529)
 */
HPALETTE16 WINAPI CreateHalftonePalette16( HDC16 hdc )
{
    return HPALETTE_16( CreateHalftonePalette( HDC_32(hdc) ));
}


/***********************************************************************
 *           SetDIBColorTable    (GDI.602)
 */
UINT16 WINAPI SetDIBColorTable16( HDC16 hdc, UINT16 startpos, UINT16 entries, RGBQUAD *colors )
{
    return SetDIBColorTable( HDC_32(hdc), startpos, entries, colors );
}


/***********************************************************************
 *           GetDIBColorTable    (GDI.603)
 */
UINT16 WINAPI GetDIBColorTable16( HDC16 hdc, UINT16 startpos, UINT16 entries, RGBQUAD *colors )
{
    return GetDIBColorTable( HDC_32(hdc), startpos, entries, colors );
}


/***********************************************************************
 *           GetRegionData   (GDI.607)
 *
 * FIXME: is LPRGNDATA the same in Win16 and Win32 ?
 */
DWORD WINAPI GetRegionData16( HRGN16 hrgn, DWORD count, LPRGNDATA rgndata )
{
    return GetRegionData( HRGN_32(hrgn), count, rgndata );
}


/***********************************************************************
 *           GdiFreeResources   (GDI.609)
 */
WORD WINAPI GdiFreeResources16( DWORD reserve )
{
    return 90; /* lie about it, it shouldn't matter */
}


/***********************************************************************
 *           GdiSignalProc32     (GDI.610)
 */
WORD WINAPI GdiSignalProc( UINT uCode, DWORD dwThreadOrProcessID,
                           DWORD dwFlags, HMODULE16 hModule )
{
    return 0;
}


/***********************************************************************
 *           GetTextCharset   (GDI.612)
 */
UINT16 WINAPI GetTextCharset16( HDC16 hdc )
{
    return GetTextCharset( HDC_32(hdc) );
}


/***********************************************************************
 *           EnumFontFamiliesEx (GDI.613)
 */
INT16 WINAPI EnumFontFamiliesEx16( HDC16 hdc, LPLOGFONT16 plf,
                                   FONTENUMPROC16 proc, LPARAM lParam,
                                   DWORD dwFlags)
{
    struct callback16_info info;
    LOGFONTA lfA, *plfA;

    info.proc  = (FARPROC16)proc;
    info.param = lParam;
    info.result = 1;

    if (plf)
    {
        logfont_16_to_A(plf, &lfA);
        plfA = &lfA;
    }
    else plfA = NULL;

    return EnumFontFamiliesExA( HDC_32(hdc), plfA, enum_font_callback,
                                (LPARAM)&info, dwFlags );
}


/*************************************************************************
 *             GetFontLanguageInfo   (GDI.616)
 */
DWORD WINAPI GetFontLanguageInfo16( HDC16 hdc )
{
    return GetFontLanguageInfo( HDC_32(hdc) );
}


/***********************************************************************
 *           SetLayout   (GDI.1000)
 *
 * Sets left->right or right->left text layout flags of a dc.
 */
BOOL16 WINAPI SetLayout16( HDC16 hdc, DWORD layout )
{
    return SetLayout( HDC_32(hdc), layout );
}


/***********************************************************************
 *           SetSolidBrush   (GDI.604)
 *
 * Change the color of a solid brush.
 *
 * PARAMS
 *  hBrush   [I] Brush to change the color of
 *  newColor [I] New color for hBrush
 *
 * RETURNS
 *  Success: TRUE. The color of hBrush is set to newColor.
 *  Failure: FALSE.
 *
 * FIXME
 *  This function is undocumented and untested. The implementation may
 *  not be correct.
 */
BOOL16 WINAPI SetSolidBrush16(HBRUSH16 hBrush, COLORREF newColor )
{
    FIXME( "%04x %08x no longer supported\n", hBrush, newColor );
    return FALSE;
}


/***********************************************************************
 *           Copy   (GDI.250)
 */
void WINAPI Copy16( LPVOID src, LPVOID dst, WORD size )
{
    memcpy( dst, src, size );
}

/***********************************************************************
 *           RealizeDefaultPalette    (GDI.365)
 */
UINT16 WINAPI RealizeDefaultPalette16( HDC16 hdc )
{
    FIXME( "%04x semi-stub\n", hdc );
    return GDIRealizePalette16( hdc );
}

/***********************************************************************
 *           IsDCCurrentPalette   (GDI.412)
 */
BOOL16 WINAPI IsDCCurrentPalette16(HDC16 hDC)
{
    return HPALETTE_16( GetCurrentObject( HDC_32(hDC), OBJ_PAL )) == hPrimaryPalette;
}

/*********************************************************************
 *           SetMagicColors   (GDI.606)
 */
VOID WINAPI SetMagicColors16(HDC16 hDC, COLORREF color, UINT16 index)
{
    FIXME("(hDC %04x, color %04x, index %04x): stub\n", hDC, (int)color, index);

}


/***********************************************************************
 *           DPtoLP    (GDI.67)
 */
BOOL16 WINAPI DPtoLP16( HDC16 hdc, LPPOINT16 points, INT16 count )
{
    POINT points32[8], *pt32 = points32;
    int i;
    BOOL ret;

    if (count > 8)
    {
        if (!(pt32 = HeapAlloc( GetProcessHeap(), 0, count * sizeof(*pt32) ))) return FALSE;
    }
    for (i = 0; i < count; i++)
    {
        pt32[i].x = points[i].x;
        pt32[i].y = points[i].y;
    }
    if ((ret = DPtoLP( HDC_32(hdc), pt32, count )))
    {
        for (i = 0; i < count; i++)
        {
            points[i].x = pt32[i].x;
            points[i].y = pt32[i].y;
        }
    }
    if (pt32 != points32) HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/***********************************************************************
 *           LPtoDP    (GDI.99)
 */
BOOL16 WINAPI LPtoDP16( HDC16 hdc, LPPOINT16 points, INT16 count )
{
    POINT points32[8], *pt32 = points32;
    int i;
    BOOL ret;

    if (count > 8)
    {
        if (!(pt32 = HeapAlloc( GetProcessHeap(), 0, count * sizeof(*pt32) ))) return FALSE;
    }
    for (i = 0; i < count; i++)
    {
        pt32[i].x = points[i].x;
        pt32[i].y = points[i].y;
    }
    if ((ret = LPtoDP( HDC_32(hdc), pt32, count )))
    {
        for (i = 0; i < count; i++)
        {
            points[i].x = pt32[i].x;
            points[i].y = pt32[i].y;
        }
    }
    if (pt32 != points32) HeapFree( GetProcessHeap(), 0, pt32 );
    return ret;
}


/***********************************************************************
 *           GetDCState   (GDI.179)
 */
HDC16 WINAPI GetDCState16( HDC16 hdc )
{
    ERR( "no longer supported\n" );
    return 0;
}


/***********************************************************************
 *           SetDCState   (GDI.180)
 */
void WINAPI SetDCState16( HDC16 hdc, HDC16 hdcs )
{
    ERR( "no longer supported\n" );
}

/***********************************************************************
 *           SetDCOrg   (GDI.117)
 */
DWORD WINAPI SetDCOrg16( HDC16 hdc16, INT16 x, INT16 y )
{
    FIXME( "%04x %d,%d no longer supported\n", hdc16, x, y );
    return 0;
}


/***********************************************************************
 *		InquireVisRgn   (GDI.131)
 */
HRGN16 WINAPI InquireVisRgn16( HDC16 hdc )
{
    static HRGN hrgn;

    if (!hrgn) hrgn = CreateRectRgn( 0, 0, 0, 0 );
    GetRandomRgn( HDC_32(hdc), hrgn, SYSRGN );
    return HRGN_16(hrgn);
}


/***********************************************************************
 *           OffsetVisRgn    (GDI.102)
 */
INT16 WINAPI OffsetVisRgn16( HDC16 hdc16, INT16 x, INT16 y )
{
    FIXME( "%04x %d,%d no longer supported\n", hdc16, x, y );
    return ERROR;
}


/***********************************************************************
 *           ExcludeVisRect   (GDI.73)
 */
INT16 WINAPI ExcludeVisRect16( HDC16 hdc16, INT16 left, INT16 top, INT16 right, INT16 bottom )
{
    FIXME( "%04x %d,%d-%d,%d no longer supported\n", hdc16, left, top, right, bottom );
    return ERROR;
}


/***********************************************************************
 *           IntersectVisRect   (GDI.98)
 */
INT16 WINAPI IntersectVisRect16( HDC16 hdc16, INT16 left, INT16 top, INT16 right, INT16 bottom )
{
    FIXME( "%04x %d,%d-%d,%d no longer supported\n", hdc16, left, top, right, bottom );
    return ERROR;
}


/***********************************************************************
 *           SaveVisRgn   (GDI.129)
 */
HRGN16 WINAPI SaveVisRgn16( HDC16 hdc16 )
{
    FIXME( "%04x no longer supported\n", hdc16 );
    return 0;
}


/***********************************************************************
 *           RestoreVisRgn   (GDI.130)
 */
INT16 WINAPI RestoreVisRgn16( HDC16 hdc16 )
{
    FIXME( "%04x no longer supported\n", hdc16 );
    return ERROR;
}


/***********************************************************************
 *		GetClipRgn (GDI.173)
 */
HRGN16 WINAPI GetClipRgn16( HDC16 hdc )
{
    static HRGN hrgn;

    if (!hrgn) hrgn = CreateRectRgn( 0, 0, 0, 0 );
    GetClipRgn( HDC_32(hdc), hrgn );
    return HRGN_16(hrgn);
}


/***********************************************************************
 *           MakeObjectPrivate    (GDI.463)
 *
 * What does that mean ?
 * Some little docu can be found in "Undocumented Windows",
 * but this is basically useless.
 */
void WINAPI MakeObjectPrivate16( HGDIOBJ16 handle16, BOOL16 private )
{
    FIXME( "stub: %x %u\n", handle16, private );
}

/***********************************************************************
 *           CreateDIBSection    (GDI.489)
 */
HBITMAP16 WINAPI CreateDIBSection16 (HDC16 hdc, const BITMAPINFO *bmi, UINT16 usage,
                                     SEGPTR *bits16, HANDLE section, DWORD offset)
{
    LPVOID bits32;
    HBITMAP hbitmap;

    hbitmap = CreateDIBSection( HDC_32(hdc), bmi, usage, &bits32, section, offset );
    if (hbitmap && bits32 && bits16) *bits16 = alloc_segptr_bits( hbitmap, bits32 );
    return HBITMAP_16(hbitmap);
}

void WINAPI GdiTaskTermination16(WORD arg1)
{
    FIXME("(%04x)\n", arg1);
}
WORD WINAPI GetPhysicalFontHandle16(WORD arg1/* dc? */)
{
    FIXME("(%04x)\n", arg1);
    return 0;
}
WORD WINAPI GdiMoveBitmap16(WORD arg1)
{
    FIXME("(%04x)\n", arg1);
    return 0;
}
BOOL16 WINAPI IsDCDirty16(WORD arg1, SEGPTR arg2)
{
    FIXME("(%04x,%08x)\n", arg1, arg2);
    return 0;
}
void WINAPI UnicodeToAnsi16(SEGPTR arg1, SEGPTR arg2)
{
    FIXME("(%08x,%08x)\n", arg1, arg2);
    return 0;
}
void WINAPI ShrinkGdiHeap16()
{
    FIXME("()\n");
}
DWORD WINAPI GetAspectRatioFilter16(WORD arg1)
{
    FIXME("(%04x)\n", arg1);
    return 0;
}
WORD WINAPI SelectBitmap16(WORD arg1, WORD arg2)
{
    FIXME("(%04x)\n", arg1);
    return SelectObject16(arg1, arg2);
}

WORD WINAPI GetFontAssocStatus16(HDC16 hdc)
{
    static BOOL load;
    static FARPROC GetFontAssocStatus;
    if (!load)
    {
        load = TRUE;
        GetFontAssocStatus = GetProcAddress(GetModuleHandleW(L"GDI32"), "GetFontAssocStatus");
    }
    if (!GetFontAssocStatus)
    {
        ERR("(%04x)\n", hdc);
        return 0;
    }
    return ((ULONG(WINAPI*)(HDC))GetFontAssocStatus)(HDC_32(hdc));
}

// style
// bit 1: scan direction 0 = left to right 1 = right to left
// bit 7: match type 0 = color 1 = not color

WORD WINAPI ScanLR16(HDC16 hdc, WORD x, WORD y, DWORD color, WORD style)
{
    HDC hdc32 = HDC_32(hdc);
    if (!hdc32 || (GetDeviceCaps(hdc32, TECHNOLOGY) != DT_RASDISPLAY))
    {
        ERR("Invalid DC (%04x)\n", hdc);
        return -1;
    }
    COLORREF pixel = GetPixel(hdc32, x, y);
    if (pixel == CLR_INVALID)
        return 0x8000; // out of range
    for (int i = x; (i <= 0xffff) && (i >= 0); (style & 2) ? i-- : i++)
    {
        pixel = GetPixel(hdc32, i, y);
        if (pixel == CLR_INVALID)
            return -1;
        if ((style & 0x80) ? (pixel != color) : (pixel == color))
            return i;
    }
    return -1;
}
        

LPCSTR RedirectSystemDir(LPCSTR path, LPSTR to, size_t max_len);

BOOL WINAPI DllEntryPoint(DWORD fdwReason, HINSTANCE hinstDLL, WORD ds,
                          WORD wHeapSize, DWORD dwReserved1, WORD wReserved2)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        static BOOL init = FALSE;
        if (init == TRUE)
            break;
        init = TRUE;
        WIN32_FIND_DATAA fileinfo = {0};
        char syspath[MAX_PATH];
        char fonfile[MAX_PATH];
        char origsyspath[MAX_PATH];
        strncpy(origsyspath, getenv("windir"), MAX_PATH);
        strcat(origsyspath, "\\system\\");
        RedirectSystemDir(origsyspath, syspath, MAX_PATH);
        strcpy(fonfile, syspath);
        strcat(fonfile, "*.*");
        HANDLE file = FindFirstFileA(fonfile, &fileinfo);
        if (file == INVALID_HANDLE_VALUE)
            break;

        BOOL ret;
        do
        {
            LPCSTR *ext = fileinfo.cFileName + strlen(fileinfo.cFileName) - 4;
            if (!stricmp(ext, ".ttf") || !stricmp(ext, ".fon"))
            {
                strcpy(fonfile, syspath);
                strcat(fonfile, fileinfo.cFileName);
                AddFontResource16(fonfile);
            }
            ret = FindNextFileA(file, &fileinfo);
        } while (ret);
        FindClose(file);
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
