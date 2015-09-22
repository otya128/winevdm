/* File generated automatically from gdi.exe16.spec; do not edit! */
/* This file can be copied, modified and distributed without restriction. */


	.section .init,"ax"
	jmp 1f
__wine_spec_pe_header:
	.skip 69632
1:

	.data
	.align 4
	.globl ___wine_spec_nt_header
___wine_spec_nt_header:
.L__wine_spec_rva_base:
	.long 0x4550
	.short 0x014c
	.short 0
	.long 0
	.long 0
	.long 0
	.short 224
	.short 0x2000
	.short 0x010b
	.byte 0
	.byte 0
	.long 0
	.long 0
	.long 0
	.long ___wine_spec_dll_entry
	.long 0
	.long 0
	.long __wine_spec_pe_header
	.long 4096
	.long 4096
	.short 1,0
	.short 0,0
	.short 4,0
	.long 0
	.long __end-.L__wine_spec_rva_base
	.long 4096
	.long 0
	.short 0x0000
	.short 0x0100
	.long 1048576,4096
	.long 1048576,4096
	.long 0
	.long 16
	.long .L__wine_spec_exports-.L__wine_spec_rva_base,.L__wine_spec_exports_end-.L__wine_spec_exports
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0
	.long 0,0

	.section .rodata
	.globl ___wine_spec_file_name
___wine_spec_file_name:
.L__wine_spec_file_name:
	.string "gdi.exe16.spec"

	.section .init,"ax"
	call ___wine_spec_init_ctor

/* module data */

	.data
	.align 4
	.globl __wine_spec_dos_header
__wine_spec_dos_header:
	.globl _wine_spec_dos_header
_wine_spec_dos_header:
.L__wine_spec_dos_header:
	.short 0x5a4d
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0
	.short 0,0,0,0
	.short 0
	.short 0
	.short 0,0,0,0,0,0,0,0,0,0
	.long .L__wine_spec_ne_header-.L__wine_spec_dos_header
.L__wine_spec_ne_header:
	.short 0x454e
	.byte 0
	.byte 0
	.short .L__wine_spec_ne_enttab-.L__wine_spec_ne_header
	.short .L__wine_spec_ne_enttab_end-.L__wine_spec_ne_enttab
	.long 0
	.short 0x8001
	.short 2
	.short 0
	.short 0
	.long 0
	.short 0,2
	.short 2
	.short 0
	.short 0
	.short .L__wine_spec_ne_segtab-.L__wine_spec_ne_header
	.short .L__wine_spec_ne_rsrctab-.L__wine_spec_ne_header
	.short .L__wine_spec_ne_restab-.L__wine_spec_ne_header
	.short .L__wine_spec_ne_modtab-.L__wine_spec_ne_header
	.short .L__wine_spec_ne_imptab-.L__wine_spec_ne_header
	.long 0
	.short 0
	.short 0
	.short 0
	.byte 0x02
	.byte 0x08
	.short 0
	.short 0
	.short 0
	.short 0

.L__wine_spec_ne_segtab:
	.short .L__wine_spec_code_segment-.L__wine_spec_dos_header
	.short .L__wine_spec_code_segment_end-.L__wine_spec_code_segment
	.short 0x2000
	.short .L__wine_spec_code_segment_end-.L__wine_spec_code_segment
	.short .L__wine_spec_data_segment-.L__wine_spec_dos_header
	.short .L__wine_spec_data_segment_end-.L__wine_spec_data_segment
	.short 0x0001
	.short .L__wine_spec_data_segment_end-.L__wine_spec_data_segment

.L__wine_spec_ne_rsrctab:
	.short 0
	.short 0
	.byte 0

	.align 2
.L__wine_spec_ne_restab:
	.byte 0x03,0x47,0x44,0x49 /* GDI */
	.short 0
	.byte 0x0a,0x53,0x45,0x54,0x42,0x4b,0x43,0x4f,0x4c,0x4f,0x52 /* SetBkColor */
	.short 1
	.byte 0x09,0x53,0x45,0x54,0x42,0x4b,0x4d,0x4f,0x44,0x45 /* SetBkMode */
	.short 2
	.byte 0x0a,0x53,0x45,0x54,0x4d,0x41,0x50,0x4d,0x4f,0x44,0x45 /* SetMapMode */
	.short 3
	.byte 0x07,0x53,0x45,0x54,0x52,0x4f,0x50,0x32 /* SetROP2 */
	.short 4
	.byte 0x09,0x53,0x45,0x54,0x52,0x45,0x4c,0x41,0x42,0x53 /* SetRelAbs */
	.short 5
	.byte 0x0f,0x53,0x45,0x54,0x50,0x4f,0x4c,0x59,0x46,0x49,0x4c,0x4c,0x4d,0x4f,0x44,0x45 /* SetPolyFillMode */
	.short 6
	.byte 0x11,0x53,0x45,0x54,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x42,0x4c,0x54,0x4d,0x4f,0x44,0x45 /* SetStretchBltMode */
	.short 7
	.byte 0x15,0x53,0x45,0x54,0x54,0x45,0x58,0x54,0x43,0x48,0x41,0x52,0x41,0x43,0x54,0x45,0x52,0x45,0x58,0x54,0x52,0x41 /* SetTextCharacterExtra */
	.short 8
	.byte 0x0c,0x53,0x45,0x54,0x54,0x45,0x58,0x54,0x43,0x4f,0x4c,0x4f,0x52 /* SetTextColor */
	.short 9
	.byte 0x14,0x53,0x45,0x54,0x54,0x45,0x58,0x54,0x4a,0x55,0x53,0x54,0x49,0x46,0x49,0x43,0x41,0x54,0x49,0x4f,0x4e /* SetTextJustification */
	.short 10
	.byte 0x0c,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47 /* SetWindowOrg */
	.short 11
	.byte 0x0c,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54 /* SetWindowExt */
	.short 12
	.byte 0x0e,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47 /* SetViewportOrg */
	.short 13
	.byte 0x0e,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54 /* SetViewportExt */
	.short 14
	.byte 0x0f,0x4f,0x46,0x46,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47 /* OffsetWindowOrg */
	.short 15
	.byte 0x0e,0x53,0x43,0x41,0x4c,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54 /* ScaleWindowExt */
	.short 16
	.byte 0x11,0x4f,0x46,0x46,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47 /* OffsetViewportOrg */
	.short 17
	.byte 0x10,0x53,0x43,0x41,0x4c,0x45,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54 /* ScaleViewportExt */
	.short 18
	.byte 0x06,0x4c,0x49,0x4e,0x45,0x54,0x4f /* LineTo */
	.short 19
	.byte 0x06,0x4d,0x4f,0x56,0x45,0x54,0x4f /* MoveTo */
	.short 20
	.byte 0x0f,0x45,0x58,0x43,0x4c,0x55,0x44,0x45,0x43,0x4c,0x49,0x50,0x52,0x45,0x43,0x54 /* ExcludeClipRect */
	.short 21
	.byte 0x11,0x49,0x4e,0x54,0x45,0x52,0x53,0x45,0x43,0x54,0x43,0x4c,0x49,0x50,0x52,0x45,0x43,0x54 /* IntersectClipRect */
	.short 22
	.byte 0x03,0x41,0x52,0x43 /* Arc */
	.short 23
	.byte 0x07,0x45,0x4c,0x4c,0x49,0x50,0x53,0x45 /* Ellipse */
	.short 24
	.byte 0x09,0x46,0x4c,0x4f,0x4f,0x44,0x46,0x49,0x4c,0x4c /* FloodFill */
	.short 25
	.byte 0x03,0x50,0x49,0x45 /* Pie */
	.short 26
	.byte 0x09,0x52,0x45,0x43,0x54,0x41,0x4e,0x47,0x4c,0x45 /* Rectangle */
	.short 27
	.byte 0x09,0x52,0x4f,0x55,0x4e,0x44,0x52,0x45,0x43,0x54 /* RoundRect */
	.short 28
	.byte 0x06,0x50,0x41,0x54,0x42,0x4c,0x54 /* PatBlt */
	.short 29
	.byte 0x06,0x53,0x41,0x56,0x45,0x44,0x43 /* SaveDC */
	.short 30
	.byte 0x08,0x53,0x45,0x54,0x50,0x49,0x58,0x45,0x4c /* SetPixel */
	.short 31
	.byte 0x0d,0x4f,0x46,0x46,0x53,0x45,0x54,0x43,0x4c,0x49,0x50,0x52,0x47,0x4e /* OffsetClipRgn */
	.short 32
	.byte 0x07,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* TextOut */
	.short 33
	.byte 0x06,0x42,0x49,0x54,0x42,0x4c,0x54 /* BitBlt */
	.short 34
	.byte 0x0a,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x42,0x4c,0x54 /* StretchBlt */
	.short 35
	.byte 0x07,0x50,0x4f,0x4c,0x59,0x47,0x4f,0x4e /* Polygon */
	.short 36
	.byte 0x08,0x50,0x4f,0x4c,0x59,0x4c,0x49,0x4e,0x45 /* Polyline */
	.short 37
	.byte 0x06,0x45,0x53,0x43,0x41,0x50,0x45 /* Escape */
	.short 38
	.byte 0x09,0x52,0x45,0x53,0x54,0x4f,0x52,0x45,0x44,0x43 /* RestoreDC */
	.short 39
	.byte 0x07,0x46,0x49,0x4c,0x4c,0x52,0x47,0x4e /* FillRgn */
	.short 40
	.byte 0x08,0x46,0x52,0x41,0x4d,0x45,0x52,0x47,0x4e /* FrameRgn */
	.short 41
	.byte 0x09,0x49,0x4e,0x56,0x45,0x52,0x54,0x52,0x47,0x4e /* InvertRgn */
	.short 42
	.byte 0x08,0x50,0x41,0x49,0x4e,0x54,0x52,0x47,0x4e /* PaintRgn */
	.short 43
	.byte 0x0d,0x53,0x45,0x4c,0x45,0x43,0x54,0x43,0x4c,0x49,0x50,0x52,0x47,0x4e /* SelectClipRgn */
	.short 44
	.byte 0x0c,0x53,0x45,0x4c,0x45,0x43,0x54,0x4f,0x42,0x4a,0x45,0x43,0x54 /* SelectObject */
	.short 45
	.byte 0x0a,0x42,0x49,0x54,0x4d,0x41,0x50,0x42,0x49,0x54,0x53 /* BITMAPBITS */
	.short 46
	.byte 0x0a,0x43,0x4f,0x4d,0x42,0x49,0x4e,0x45,0x52,0x47,0x4e /* CombineRgn */
	.short 47
	.byte 0x0c,0x43,0x52,0x45,0x41,0x54,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateBitmap */
	.short 48
	.byte 0x14,0x43,0x52,0x45,0x41,0x54,0x45,0x42,0x49,0x54,0x4d,0x41,0x50,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateBitmapIndirect */
	.short 49
	.byte 0x13,0x43,0x52,0x45,0x41,0x54,0x45,0x42,0x52,0x55,0x53,0x48,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateBrushIndirect */
	.short 50
	.byte 0x16,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x4f,0x4d,0x50,0x41,0x54,0x49,0x42,0x4c,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateCompatibleBitmap */
	.short 51
	.byte 0x12,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x4f,0x4d,0x50,0x41,0x54,0x49,0x42,0x4c,0x45,0x44,0x43 /* CreateCompatibleDC */
	.short 52
	.byte 0x08,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x43 /* CreateDC */
	.short 53
	.byte 0x11,0x43,0x52,0x45,0x41,0x54,0x45,0x45,0x4c,0x4c,0x49,0x50,0x54,0x49,0x43,0x52,0x47,0x4e /* CreateEllipticRgn */
	.short 54
	.byte 0x19,0x43,0x52,0x45,0x41,0x54,0x45,0x45,0x4c,0x4c,0x49,0x50,0x54,0x49,0x43,0x52,0x47,0x4e,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateEllipticRgnIndirect */
	.short 55
	.byte 0x0a,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x4f,0x4e,0x54 /* CreateFont */
	.short 56
	.byte 0x12,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x4f,0x4e,0x54,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateFontIndirect */
	.short 57
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x48,0x41,0x54,0x43,0x48,0x42,0x52,0x55,0x53,0x48 /* CreateHatchBrush */
	.short 58
	.byte 0x12,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x41,0x54,0x54,0x45,0x52,0x4e,0x42,0x52,0x55,0x53,0x48 /* CreatePatternBrush */
	.short 60
	.byte 0x09,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x45,0x4e /* CreatePen */
	.short 61
	.byte 0x11,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x45,0x4e,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreatePenIndirect */
	.short 62
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x4f,0x4c,0x59,0x47,0x4f,0x4e,0x52,0x47,0x4e /* CreatePolygonRgn */
	.short 63
	.byte 0x0d,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x45,0x43,0x54,0x52,0x47,0x4e /* CreateRectRgn */
	.short 64
	.byte 0x15,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x45,0x43,0x54,0x52,0x47,0x4e,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateRectRgnIndirect */
	.short 65
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x53,0x4f,0x4c,0x49,0x44,0x42,0x52,0x55,0x53,0x48 /* CreateSolidBrush */
	.short 66
	.byte 0x06,0x44,0x50,0x54,0x4f,0x4c,0x50 /* DPtoLP */
	.short 67
	.byte 0x08,0x44,0x45,0x4c,0x45,0x54,0x45,0x44,0x43 /* DeleteDC */
	.short 68
	.byte 0x0c,0x44,0x45,0x4c,0x45,0x54,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54 /* DeleteObject */
	.short 69
	.byte 0x09,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x53 /* EnumFonts */
	.short 70
	.byte 0x0b,0x45,0x4e,0x55,0x4d,0x4f,0x42,0x4a,0x45,0x43,0x54,0x53 /* EnumObjects */
	.short 71
	.byte 0x08,0x45,0x51,0x55,0x41,0x4c,0x52,0x47,0x4e /* EqualRgn */
	.short 72
	.byte 0x0e,0x45,0x58,0x43,0x4c,0x55,0x44,0x45,0x56,0x49,0x53,0x52,0x45,0x43,0x54 /* ExcludeVisRect */
	.short 73
	.byte 0x0d,0x47,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x42,0x49,0x54,0x53 /* GetBitmapBits */
	.short 74
	.byte 0x0a,0x47,0x45,0x54,0x42,0x4b,0x43,0x4f,0x4c,0x4f,0x52 /* GetBkColor */
	.short 75
	.byte 0x09,0x47,0x45,0x54,0x42,0x4b,0x4d,0x4f,0x44,0x45 /* GetBkMode */
	.short 76
	.byte 0x0a,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x58 /* GetClipBox */
	.short 77
	.byte 0x12,0x47,0x45,0x54,0x43,0x55,0x52,0x52,0x45,0x4e,0x54,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e /* GetCurrentPosition */
	.short 78
	.byte 0x08,0x47,0x45,0x54,0x44,0x43,0x4f,0x52,0x47 /* GetDCOrg */
	.short 79
	.byte 0x0d,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45,0x43,0x41,0x50,0x53 /* GetDeviceCaps */
	.short 80
	.byte 0x0a,0x47,0x45,0x54,0x4d,0x41,0x50,0x4d,0x4f,0x44,0x45 /* GetMapMode */
	.short 81
	.byte 0x09,0x47,0x45,0x54,0x4f,0x42,0x4a,0x45,0x43,0x54 /* GetObject */
	.short 82
	.byte 0x08,0x47,0x45,0x54,0x50,0x49,0x58,0x45,0x4c /* GetPixel */
	.short 83
	.byte 0x0f,0x47,0x45,0x54,0x50,0x4f,0x4c,0x59,0x46,0x49,0x4c,0x4c,0x4d,0x4f,0x44,0x45 /* GetPolyFillMode */
	.short 84
	.byte 0x07,0x47,0x45,0x54,0x52,0x4f,0x50,0x32 /* GetROP2 */
	.short 85
	.byte 0x09,0x47,0x45,0x54,0x52,0x45,0x4c,0x41,0x42,0x53 /* GetRelAbs */
	.short 86
	.byte 0x0e,0x47,0x45,0x54,0x53,0x54,0x4f,0x43,0x4b,0x4f,0x42,0x4a,0x45,0x43,0x54 /* GetStockObject */
	.short 87
	.byte 0x11,0x47,0x45,0x54,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x42,0x4c,0x54,0x4d,0x4f,0x44,0x45 /* GetStretchBltMode */
	.short 88
	.byte 0x15,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x43,0x48,0x41,0x52,0x41,0x43,0x54,0x45,0x52,0x45,0x58,0x54,0x52,0x41 /* GetTextCharacterExtra */
	.short 89
	.byte 0x0c,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x43,0x4f,0x4c,0x4f,0x52 /* GetTextColor */
	.short 90
	.byte 0x0d,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54 /* GetTextExtent */
	.short 91
	.byte 0x0b,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x46,0x41,0x43,0x45 /* GetTextFace */
	.short 92
	.byte 0x0e,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x4d,0x45,0x54,0x52,0x49,0x43,0x53 /* GetTextMetrics */
	.short 93
	.byte 0x0e,0x47,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54 /* GetViewportExt */
	.short 94
	.byte 0x0e,0x47,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47 /* GetViewportOrg */
	.short 95
	.byte 0x0c,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54 /* GetWindowExt */
	.short 96
	.byte 0x0c,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47 /* GetWindowOrg */
	.short 97
	.byte 0x10,0x49,0x4e,0x54,0x45,0x52,0x53,0x45,0x43,0x54,0x56,0x49,0x53,0x52,0x45,0x43,0x54 /* IntersectVisRect */
	.short 98
	.byte 0x06,0x4c,0x50,0x54,0x4f,0x44,0x50 /* LPtoDP */
	.short 99
	.byte 0x07,0x4c,0x49,0x4e,0x45,0x44,0x44,0x41 /* LineDDA */
	.short 100
	.byte 0x09,0x4f,0x46,0x46,0x53,0x45,0x54,0x52,0x47,0x4e /* OffsetRgn */
	.short 101
	.byte 0x0c,0x4f,0x46,0x46,0x53,0x45,0x54,0x56,0x49,0x53,0x52,0x47,0x4e /* OffsetVisRgn */
	.short 102
	.byte 0x09,0x50,0x54,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* PtVisible */
	.short 103
	.byte 0x0e,0x52,0x45,0x43,0x54,0x56,0x49,0x53,0x49,0x42,0x4c,0x45,0x4f,0x4c,0x44 /* RectVisibleOld */
	.short 104
	.byte 0x0c,0x53,0x45,0x4c,0x45,0x43,0x54,0x56,0x49,0x53,0x52,0x47,0x4e /* SelectVisRgn */
	.short 105
	.byte 0x0d,0x53,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x42,0x49,0x54,0x53 /* SetBitmapBits */
	.short 106
	.byte 0x08,0x53,0x45,0x54,0x44,0x43,0x4f,0x52,0x47 /* SetDCOrg */
	.short 117
	.byte 0x10,0x49,0x4e,0x54,0x45,0x52,0x4e,0x41,0x4c,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x43 /* InternalCreateDC */
	.short 118
	.byte 0x0f,0x41,0x44,0x44,0x46,0x4f,0x4e,0x54,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* AddFontResource */
	.short 119
	.byte 0x17,0x47,0x45,0x54,0x43,0x4f,0x4e,0x54,0x49,0x4e,0x55,0x49,0x4e,0x47,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54 /* GetContinuingTextExtent */
	.short 120
	.byte 0x05,0x44,0x45,0x41,0x54,0x48 /* Death */
	.short 121
	.byte 0x0c,0x52,0x45,0x53,0x55,0x52,0x52,0x45,0x43,0x54,0x49,0x4f,0x4e /* Resurrection */
	.short 122
	.byte 0x0c,0x50,0x4c,0x41,0x59,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* PlayMetaFile */
	.short 123
	.byte 0x0b,0x47,0x45,0x54,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* GetMetaFile */
	.short 124
	.byte 0x0e,0x43,0x52,0x45,0x41,0x54,0x45,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CreateMetaFile */
	.short 125
	.byte 0x0d,0x43,0x4c,0x4f,0x53,0x45,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CloseMetaFile */
	.short 126
	.byte 0x0e,0x44,0x45,0x4c,0x45,0x54,0x45,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* DeleteMetaFile */
	.short 127
	.byte 0x06,0x4d,0x55,0x4c,0x44,0x49,0x56 /* MulDiv */
	.short 128
	.byte 0x0a,0x53,0x41,0x56,0x45,0x56,0x49,0x53,0x52,0x47,0x4e /* SaveVisRgn */
	.short 129
	.byte 0x0d,0x52,0x45,0x53,0x54,0x4f,0x52,0x45,0x56,0x49,0x53,0x52,0x47,0x4e /* RestoreVisRgn */
	.short 130
	.byte 0x0d,0x49,0x4e,0x51,0x55,0x49,0x52,0x45,0x56,0x49,0x53,0x52,0x47,0x4e /* InquireVisRgn */
	.short 131
	.byte 0x0e,0x53,0x45,0x54,0x45,0x4e,0x56,0x49,0x52,0x4f,0x4e,0x4d,0x45,0x4e,0x54 /* SetEnvironment */
	.short 132
	.byte 0x0e,0x47,0x45,0x54,0x45,0x4e,0x56,0x49,0x52,0x4f,0x4e,0x4d,0x45,0x4e,0x54 /* GetEnvironment */
	.short 133
	.byte 0x09,0x47,0x45,0x54,0x52,0x47,0x4e,0x42,0x4f,0x58 /* GetRgnBox */
	.short 134
	.byte 0x12,0x52,0x45,0x4d,0x4f,0x56,0x45,0x46,0x4f,0x4e,0x54,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* RemoveFontResource */
	.short 136
	.byte 0x03,0x47,0x53,0x56 /* GSV */
	.short 137
	.byte 0x07,0x44,0x50,0x58,0x4c,0x41,0x54,0x45 /* DPXlate */
	.short 138
	.byte 0x0d,0x53,0x45,0x54,0x57,0x49,0x4e,0x56,0x49,0x45,0x57,0x45,0x58,0x54 /* SetWinViewExt */
	.short 139
	.byte 0x08,0x53,0x43,0x41,0x4c,0x45,0x45,0x58,0x54 /* ScaleExt */
	.short 140
	.byte 0x07,0x57,0x4f,0x52,0x44,0x53,0x45,0x54 /* WordSet */
	.short 141
	.byte 0x09,0x52,0x45,0x43,0x54,0x53,0x54,0x55,0x46,0x46 /* RectStuff */
	.short 142
	.byte 0x09,0x4f,0x46,0x46,0x53,0x45,0x54,0x4f,0x52,0x47 /* OffsetOrg */
	.short 143
	.byte 0x06,0x4c,0x4f,0x43,0x4b,0x44,0x43 /* LockDC */
	.short 144
	.byte 0x08,0x55,0x4e,0x4c,0x4f,0x43,0x4b,0x44,0x43 /* UnlockDC */
	.short 145
	.byte 0x0a,0x4c,0x4f,0x43,0x4b,0x55,0x4e,0x4c,0x4f,0x43,0x4b /* LockUnlock */
	.short 146
	.byte 0x0c,0x47,0x44,0x49,0x5f,0x46,0x41,0x52,0x46,0x52,0x41,0x4d,0x45 /* GDI_FarFrame */
	.short 147
	.byte 0x0b,0x53,0x45,0x54,0x42,0x52,0x55,0x53,0x48,0x4f,0x52,0x47 /* SetBrushOrg */
	.short 148
	.byte 0x0b,0x47,0x45,0x54,0x42,0x52,0x55,0x53,0x48,0x4f,0x52,0x47 /* GetBrushOrg */
	.short 149
	.byte 0x0f,0x55,0x4e,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54 /* UnrealizeObject */
	.short 150
	.byte 0x0c,0x43,0x4f,0x50,0x59,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CopyMetaFile */
	.short 151
	.byte 0x0a,0x47,0x44,0x49,0x49,0x4e,0x49,0x54,0x41,0x50,0x50 /* GDIInitApp */
	.short 152
	.byte 0x08,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x43 /* CreateIC */
	.short 153
	.byte 0x0f,0x47,0x45,0x54,0x4e,0x45,0x41,0x52,0x45,0x53,0x54,0x43,0x4f,0x4c,0x4f,0x52 /* GetNearestColor */
	.short 154
	.byte 0x0a,0x51,0x55,0x45,0x52,0x59,0x41,0x42,0x4f,0x52,0x54 /* QueryAbort */
	.short 155
	.byte 0x17,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x53,0x43,0x41,0x52,0x44,0x41,0x42,0x4c,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateDiscardableBitmap */
	.short 156
	.byte 0x10,0x43,0x4f,0x4d,0x50,0x41,0x54,0x49,0x42,0x4c,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* CompatibleBitmap */
	.short 157
	.byte 0x0c,0x45,0x4e,0x55,0x4d,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b /* EnumCallback */
	.short 158
	.byte 0x0f,0x47,0x45,0x54,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x42,0x49,0x54,0x53 /* GetMetaFileBits */
	.short 159
	.byte 0x0f,0x53,0x45,0x54,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x42,0x49,0x54,0x53 /* SetMetaFileBits */
	.short 160
	.byte 0x0a,0x50,0x54,0x49,0x4e,0x52,0x45,0x47,0x49,0x4f,0x4e /* PtInRegion */
	.short 161
	.byte 0x12,0x47,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x44,0x49,0x4d,0x45,0x4e,0x53,0x49,0x4f,0x4e /* GetBitmapDimension */
	.short 162
	.byte 0x12,0x53,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x44,0x49,0x4d,0x45,0x4e,0x53,0x49,0x4f,0x4e /* SetBitmapDimension */
	.short 163
	.byte 0x09,0x50,0x49,0x58,0x54,0x4f,0x4c,0x49,0x4e,0x45 /* PixToLine */
	.short 164
	.byte 0x09,0x49,0x53,0x44,0x43,0x44,0x49,0x52,0x54,0x59 /* IsDCDirty */
	.short 169
	.byte 0x0b,0x53,0x45,0x54,0x44,0x43,0x53,0x54,0x41,0x54,0x55,0x53 /* SetDCStatus */
	.short 170
	.byte 0x08,0x4c,0x56,0x42,0x55,0x4e,0x49,0x4f,0x4e /* LVBUNION */
	.short 171
	.byte 0x0a,0x53,0x45,0x54,0x52,0x45,0x43,0x54,0x52,0x47,0x4e /* SetRectRgn */
	.short 172
	.byte 0x0a,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x52,0x47,0x4e /* GetClipRgn */
	.short 173
	.byte 0x05,0x42,0x4c,0x4f,0x41,0x54 /* BLOAT */
	.short 174
	.byte 0x0c,0x45,0x4e,0x55,0x4d,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* EnumMetaFile */
	.short 175
	.byte 0x12,0x50,0x4c,0x41,0x59,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x52,0x45,0x43,0x4f,0x52,0x44 /* PlayMetaFileRecord */
	.short 176
	.byte 0x04,0x52,0x43,0x4f,0x53 /* RCOS */
	.short 177
	.byte 0x04,0x52,0x53,0x49,0x4e /* RSIN */
	.short 178
	.byte 0x0a,0x47,0x45,0x54,0x44,0x43,0x53,0x54,0x41,0x54,0x45 /* GetDCState */
	.short 179
	.byte 0x0a,0x53,0x45,0x54,0x44,0x43,0x53,0x54,0x41,0x54,0x45 /* SetDCState */
	.short 180
	.byte 0x0f,0x52,0x45,0x43,0x54,0x49,0x4e,0x52,0x45,0x47,0x49,0x4f,0x4e,0x4f,0x4c,0x44 /* RectInRegionOld */
	.short 181
	.byte 0x0a,0x52,0x45,0x51,0x55,0x45,0x53,0x54,0x53,0x45,0x4d /* REQUESTSEM */
	.short 182
	.byte 0x08,0x43,0x4c,0x45,0x41,0x52,0x53,0x45,0x4d /* CLEARSEM */
	.short 183
	.byte 0x0c,0x53,0x54,0x55,0x46,0x46,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* STUFFVISIBLE */
	.short 184
	.byte 0x0d,0x53,0x54,0x55,0x46,0x46,0x49,0x4e,0x52,0x45,0x47,0x49,0x4f,0x4e /* STUFFINREGION */
	.short 185
	.byte 0x14,0x44,0x45,0x4c,0x45,0x54,0x45,0x41,0x42,0x4f,0x56,0x45,0x4c,0x49,0x4e,0x45,0x46,0x4f,0x4e,0x54,0x53 /* DELETEABOVELINEFONTS */
	.short 186
	.byte 0x0f,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54,0x45,0x58 /* GetTextExtentEx */
	.short 188
	.byte 0x09,0x53,0x45,0x54,0x44,0x43,0x48,0x4f,0x4f,0x4b /* SetDCHook */
	.short 190
	.byte 0x09,0x47,0x45,0x54,0x44,0x43,0x48,0x4f,0x4f,0x4b /* GetDCHook */
	.short 191
	.byte 0x0c,0x53,0x45,0x54,0x48,0x4f,0x4f,0x4b,0x46,0x4c,0x41,0x47,0x53 /* SetHookFlags */
	.short 192
	.byte 0x0d,0x53,0x45,0x54,0x42,0x4f,0x55,0x4e,0x44,0x53,0x52,0x45,0x43,0x54 /* SetBoundsRect */
	.short 193
	.byte 0x0d,0x47,0x45,0x54,0x42,0x4f,0x55,0x4e,0x44,0x53,0x52,0x45,0x43,0x54 /* GetBoundsRect */
	.short 194
	.byte 0x0c,0x53,0x45,0x4c,0x45,0x43,0x54,0x42,0x49,0x54,0x4d,0x41,0x50 /* SelectBitmap */
	.short 195
	.byte 0x15,0x53,0x45,0x54,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x42,0x49,0x54,0x53,0x42,0x45,0x54,0x54,0x45,0x52 /* SetMetaFileBitsBetter */
	.short 196
	.byte 0x08,0x44,0x4d,0x42,0x49,0x54,0x42,0x4c,0x54 /* DMBITBLT */
	.short 201
	.byte 0x0b,0x44,0x4d,0x43,0x4f,0x4c,0x4f,0x52,0x49,0x4e,0x46,0x4f /* DMCOLORINFO */
	.short 202
	.byte 0x0c,0x44,0x4d,0x45,0x4e,0x55,0x4d,0x44,0x46,0x4f,0x4e,0x54,0x53 /* dmEnumDFonts */
	.short 206
	.byte 0x09,0x44,0x4d,0x45,0x4e,0x55,0x4d,0x4f,0x42,0x4a /* DMENUMOBJ */
	.short 207
	.byte 0x08,0x44,0x4d,0x4f,0x55,0x54,0x50,0x55,0x54 /* DMOUTPUT */
	.short 208
	.byte 0x07,0x44,0x4d,0x50,0x49,0x58,0x45,0x4c /* DMPIXEL */
	.short 209
	.byte 0x0f,0x44,0x4d,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54 /* dmRealizeObject */
	.short 210
	.byte 0x08,0x44,0x4d,0x53,0x54,0x52,0x42,0x4c,0x54 /* DMSTRBLT */
	.short 211
	.byte 0x08,0x44,0x4d,0x53,0x43,0x41,0x4e,0x4c,0x52 /* DMSCANLR */
	.short 212
	.byte 0x05,0x42,0x52,0x55,0x54,0x45 /* BRUTE */
	.short 213
	.byte 0x0c,0x44,0x4d,0x45,0x58,0x54,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* DMEXTTEXTOUT */
	.short 214
	.byte 0x0e,0x44,0x4d,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48 /* DMGETCHARWIDTH */
	.short 215
	.byte 0x0c,0x44,0x4d,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x42,0x4c,0x54 /* DMSTRETCHBLT */
	.short 216
	.byte 0x09,0x44,0x4d,0x44,0x49,0x42,0x42,0x49,0x54,0x53 /* DMDIBBITS */
	.short 217
	.byte 0x0f,0x44,0x4d,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x44,0x49,0x42,0x49,0x54,0x53 /* DMSTRETCHDIBITS */
	.short 218
	.byte 0x0d,0x44,0x4d,0x53,0x45,0x54,0x44,0x49,0x42,0x54,0x4f,0x44,0x45,0x56 /* DMSETDIBTODEV */
	.short 219
	.byte 0x0b,0x44,0x4d,0x54,0x52,0x41,0x4e,0x53,0x50,0x4f,0x53,0x45 /* DMTRANSPOSE */
	.short 220
	.byte 0x08,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x51 /* CreatePQ */
	.short 230
	.byte 0x05,0x4d,0x49,0x4e,0x50,0x51 /* MinPQ */
	.short 231
	.byte 0x09,0x45,0x58,0x54,0x52,0x41,0x43,0x54,0x50,0x51 /* ExtractPQ */
	.short 232
	.byte 0x08,0x49,0x4e,0x53,0x45,0x52,0x54,0x50,0x51 /* InsertPQ */
	.short 233
	.byte 0x06,0x53,0x49,0x5a,0x45,0x50,0x51 /* SizePQ */
	.short 234
	.byte 0x08,0x44,0x45,0x4c,0x45,0x54,0x45,0x50,0x51 /* DeletePQ */
	.short 235
	.byte 0x07,0x4f,0x50,0x45,0x4e,0x4a,0x4f,0x42 /* OpenJob */
	.short 240
	.byte 0x0a,0x57,0x52,0x49,0x54,0x45,0x53,0x50,0x4f,0x4f,0x4c /* WriteSpool */
	.short 241
	.byte 0x0b,0x57,0x52,0x49,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WriteDialog */
	.short 242
	.byte 0x08,0x43,0x4c,0x4f,0x53,0x45,0x4a,0x4f,0x42 /* CloseJob */
	.short 243
	.byte 0x09,0x44,0x45,0x4c,0x45,0x54,0x45,0x4a,0x4f,0x42 /* DeleteJob */
	.short 244
	.byte 0x0b,0x47,0x45,0x54,0x53,0x50,0x4f,0x4f,0x4c,0x4a,0x4f,0x42 /* GetSpoolJob */
	.short 245
	.byte 0x0e,0x53,0x54,0x41,0x52,0x54,0x53,0x50,0x4f,0x4f,0x4c,0x50,0x41,0x47,0x45 /* StartSpoolPage */
	.short 246
	.byte 0x0c,0x45,0x4e,0x44,0x53,0x50,0x4f,0x4f,0x4c,0x50,0x41,0x47,0x45 /* EndSpoolPage */
	.short 247
	.byte 0x08,0x51,0x55,0x45,0x52,0x59,0x4a,0x4f,0x42 /* QueryJob */
	.short 248
	.byte 0x04,0x43,0x4f,0x50,0x59 /* Copy */
	.short 250
	.byte 0x0f,0x44,0x45,0x4c,0x45,0x54,0x45,0x53,0x50,0x4f,0x4f,0x4c,0x50,0x41,0x47,0x45 /* DeleteSpoolPage */
	.short 253
	.byte 0x09,0x53,0x50,0x4f,0x4f,0x4c,0x46,0x49,0x4c,0x45 /* SpoolFile */
	.short 254
	.byte 0x10,0x53,0x54,0x41,0x52,0x54,0x44,0x4f,0x43,0x50,0x52,0x49,0x4e,0x54,0x45,0x52,0x41 /* StartDocPrintEra */
	.short 267
	.byte 0x10,0x53,0x54,0x41,0x52,0x54,0x50,0x41,0x47,0x45,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* StartPagePrinter */
	.short 268
	.byte 0x0c,0x57,0x52,0x49,0x54,0x45,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* WritePrinter */
	.short 269
	.byte 0x0e,0x45,0x4e,0x44,0x50,0x41,0x47,0x45,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* EndPagePrinter */
	.short 270
	.byte 0x0c,0x41,0x42,0x4f,0x52,0x54,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* AbortPrinter */
	.short 271
	.byte 0x0d,0x45,0x4e,0x44,0x44,0x4f,0x43,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* EndDocPrinter */
	.short 272
	.byte 0x0c,0x43,0x4c,0x4f,0x53,0x45,0x50,0x52,0x49,0x4e,0x54,0x45,0x52 /* ClosePrinter */
	.short 274
	.byte 0x11,0x47,0x45,0x54,0x52,0x45,0x41,0x4c,0x44,0x52,0x49,0x56,0x45,0x52,0x49,0x4e,0x46,0x4f /* GetRealDriverInfo */
	.short 280
	.byte 0x11,0x44,0x52,0x56,0x53,0x45,0x54,0x50,0x52,0x49,0x4e,0x54,0x45,0x52,0x44,0x41,0x54,0x41 /* DrvSetPrinterData */
	.short 281
	.byte 0x11,0x44,0x52,0x56,0x47,0x45,0x54,0x50,0x52,0x49,0x4e,0x54,0x45,0x52,0x44,0x41,0x54,0x41 /* DrvGetPrinterData */
	.short 282
	.byte 0x14,0x45,0x4e,0x47,0x49,0x4e,0x45,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48,0x45,0x58 /* ENGINEGETCHARWIDTHEX */
	.short 299
	.byte 0x13,0x45,0x4e,0x47,0x49,0x4e,0x45,0x45,0x4e,0x55,0x4d,0x45,0x52,0x41,0x54,0x45,0x46,0x4f,0x4e,0x54 /* EngineEnumerateFont */
	.short 300
	.byte 0x10,0x45,0x4e,0x47,0x49,0x4e,0x45,0x44,0x45,0x4c,0x45,0x54,0x45,0x46,0x4f,0x4e,0x54 /* EngineDeleteFont */
	.short 301
	.byte 0x11,0x45,0x4e,0x47,0x49,0x4e,0x45,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x46,0x4f,0x4e,0x54 /* EngineRealizeFont */
	.short 302
	.byte 0x12,0x45,0x4e,0x47,0x49,0x4e,0x45,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48 /* EngineGetCharWidth */
	.short 303
	.byte 0x14,0x45,0x4e,0x47,0x49,0x4e,0x45,0x53,0x45,0x54,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* EngineSetFontContext */
	.short 304
	.byte 0x11,0x45,0x4e,0x47,0x49,0x4e,0x45,0x47,0x45,0x54,0x47,0x4c,0x59,0x50,0x48,0x42,0x4d,0x50 /* EngineGetGlyphBMP */
	.short 305
	.byte 0x11,0x45,0x4e,0x47,0x49,0x4e,0x45,0x4d,0x41,0x4b,0x45,0x46,0x4f,0x4e,0x54,0x44,0x49,0x52 /* EngineMakeFontDir */
	.short 306
	.byte 0x10,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x41,0x42,0x43,0x57,0x49,0x44,0x54,0x48,0x53 /* GetCharABCWidths */
	.short 307
	.byte 0x15,0x47,0x45,0x54,0x4f,0x55,0x54,0x4c,0x49,0x4e,0x45,0x54,0x45,0x58,0x54,0x4d,0x45,0x54,0x52,0x49,0x43,0x53 /* GetOutlineTextMetrics */
	.short 308
	.byte 0x0f,0x47,0x45,0x54,0x47,0x4c,0x59,0x50,0x48,0x4f,0x55,0x54,0x4c,0x49,0x4e,0x45 /* GetGlyphOutline */
	.short 309
	.byte 0x1a,0x43,0x52,0x45,0x41,0x54,0x45,0x53,0x43,0x41,0x4c,0x41,0x42,0x4c,0x45,0x46,0x4f,0x4e,0x54,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* CreateScalableFontResource */
	.short 310
	.byte 0x0b,0x47,0x45,0x54,0x46,0x4f,0x4e,0x54,0x44,0x41,0x54,0x41 /* GetFontData */
	.short 311
	.byte 0x16,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54,0x4f,0x55,0x54,0x4c,0x49,0x4e,0x45,0x46,0x4f,0x4e,0x54,0x46,0x49,0x4c,0x45 /* ConvertOutLineFontFile */
	.short 312
	.byte 0x11,0x47,0x45,0x54,0x52,0x41,0x53,0x54,0x45,0x52,0x49,0x5a,0x45,0x52,0x43,0x41,0x50,0x53 /* GetRasterizerCaps */
	.short 313
	.byte 0x10,0x45,0x4e,0x47,0x49,0x4e,0x45,0x45,0x58,0x54,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* EngineExtTextOut */
	.short 314
	.byte 0x14,0x45,0x4e,0x47,0x49,0x4e,0x45,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x46,0x4f,0x4e,0x54,0x45,0x58,0x54 /* EngineRealizeFontExt */
	.short 315
	.byte 0x15,0x45,0x4e,0x47,0x49,0x4e,0x45,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48,0x53,0x54,0x52 /* EngineGetCharWidthStr */
	.short 316
	.byte 0x14,0x45,0x4e,0x47,0x49,0x4e,0x45,0x47,0x45,0x54,0x47,0x4c,0x59,0x50,0x48,0x42,0x4d,0x50,0x45,0x58,0x54 /* EngineGetGlyphBmpExt */
	.short 317
	.byte 0x10,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x46,0x41,0x4d,0x49,0x4c,0x49,0x45,0x53 /* EnumFontFamilies */
	.short 330
	.byte 0x0f,0x47,0x45,0x54,0x4b,0x45,0x52,0x4e,0x49,0x4e,0x47,0x50,0x41,0x49,0x52,0x53 /* GetKerningPairs */
	.short 332
	.byte 0x0c,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x41,0x4c,0x49,0x47,0x4e /* GetTextAlign */
	.short 345
	.byte 0x0c,0x53,0x45,0x54,0x54,0x45,0x58,0x54,0x41,0x4c,0x49,0x47,0x4e /* SetTextAlign */
	.short 346
	.byte 0x0a,0x4d,0x46,0x44,0x52,0x41,0x57,0x54,0x45,0x58,0x54 /* MFDRAWTEXT */
	.short 347
	.byte 0x05,0x43,0x48,0x4f,0x52,0x44 /* Chord */
	.short 348
	.byte 0x0e,0x53,0x45,0x54,0x4d,0x41,0x50,0x50,0x45,0x52,0x46,0x4c,0x41,0x47,0x53 /* SetMapperFlags */
	.short 349
	.byte 0x0c,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48 /* GetCharWidth */
	.short 350
	.byte 0x0a,0x45,0x58,0x54,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* ExtTextOut */
	.short 351
	.byte 0x15,0x47,0x45,0x54,0x50,0x48,0x59,0x53,0x49,0x43,0x41,0x4c,0x46,0x4f,0x4e,0x54,0x48,0x41,0x4e,0x44,0x4c,0x45 /* GetPhysicalFontHandle */
	.short 352
	.byte 0x14,0x47,0x45,0x54,0x41,0x53,0x50,0x45,0x43,0x54,0x52,0x41,0x54,0x49,0x4f,0x46,0x49,0x4c,0x54,0x45,0x52 /* GetAspectRatioFilter */
	.short 353
	.byte 0x0d,0x53,0x48,0x52,0x49,0x4e,0x4b,0x47,0x44,0x49,0x48,0x45,0x41,0x50 /* ShrinkGDIHeap */
	.short 354
	.byte 0x0a,0x46,0x54,0x52,0x41,0x50,0x50,0x49,0x4e,0x47,0x30 /* FTrapping0 */
	.short 355
	.byte 0x0d,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* CreatePalette */
	.short 360
	.byte 0x10,0x47,0x44,0x49,0x53,0x45,0x4c,0x45,0x43,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* GDISelectPalette */
	.short 361
	.byte 0x11,0x47,0x44,0x49,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* GDIRealizePalette */
	.short 362
	.byte 0x11,0x47,0x45,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x45,0x4e,0x54,0x52,0x49,0x45,0x53 /* GetPaletteEntries */
	.short 363
	.byte 0x11,0x53,0x45,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x45,0x4e,0x54,0x52,0x49,0x45,0x53 /* SetPaletteEntries */
	.short 364
	.byte 0x15,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* RealizeDefaultPalette */
	.short 365
	.byte 0x0c,0x55,0x50,0x44,0x41,0x54,0x45,0x43,0x4f,0x4c,0x4f,0x52,0x53 /* UpdateColors */
	.short 366
	.byte 0x0e,0x41,0x4e,0x49,0x4d,0x41,0x54,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* AnimatePalette */
	.short 367
	.byte 0x0d,0x52,0x45,0x53,0x49,0x5a,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* ResizePalette */
	.short 368
	.byte 0x16,0x47,0x45,0x54,0x4e,0x45,0x41,0x52,0x45,0x53,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x49,0x4e,0x44,0x45,0x58 /* GetNearestPaletteIndex */
	.short 370
	.byte 0x0c,0x45,0x58,0x54,0x46,0x4c,0x4f,0x4f,0x44,0x46,0x49,0x4c,0x4c /* ExtFloodFill */
	.short 372
	.byte 0x13,0x53,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x55,0x53,0x45 /* SetSystemPaletteUse */
	.short 373
	.byte 0x13,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x55,0x53,0x45 /* GetSystemPaletteUse */
	.short 374
	.byte 0x17,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x45,0x4e,0x54,0x52,0x49,0x45,0x53 /* GetSystemPaletteEntries */
	.short 375
	.byte 0x07,0x52,0x45,0x53,0x45,0x54,0x44,0x43 /* ResetDC */
	.short 376
	.byte 0x08,0x53,0x54,0x41,0x52,0x54,0x44,0x4f,0x43 /* StartDoc */
	.short 377
	.byte 0x06,0x45,0x4e,0x44,0x44,0x4f,0x43 /* EndDoc */
	.short 378
	.byte 0x09,0x53,0x54,0x41,0x52,0x54,0x50,0x41,0x47,0x45 /* StartPage */
	.short 379
	.byte 0x07,0x45,0x4e,0x44,0x50,0x41,0x47,0x45 /* EndPage */
	.short 380
	.byte 0x0c,0x53,0x45,0x54,0x41,0x42,0x4f,0x52,0x54,0x50,0x52,0x4f,0x43 /* SetAbortProc */
	.short 381
	.byte 0x08,0x41,0x42,0x4f,0x52,0x54,0x44,0x4f,0x43 /* AbortDoc */
	.short 382
	.byte 0x0f,0x46,0x41,0x53,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x46,0x52,0x41,0x4d,0x45 /* FastWindowFrame */
	.short 400
	.byte 0x0d,0x47,0x44,0x49,0x4d,0x4f,0x56,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* GDIMOVEBITMAP */
	.short 401
	.byte 0x10,0x47,0x44,0x49,0x47,0x45,0x54,0x42,0x49,0x54,0x53,0x47,0x4c,0x4f,0x42,0x41,0x4c /* GDIGETBITSGLOBAL */
	.short 402
	.byte 0x08,0x47,0x44,0x49,0x49,0x4e,0x49,0x54,0x32 /* GdiInit2 */
	.short 403
	.byte 0x12,0x47,0x45,0x54,0x54,0x54,0x47,0x4c,0x59,0x50,0x48,0x49,0x4e,0x44,0x45,0x58,0x4d,0x41,0x50 /* GetTTGlyphIndexMap */
	.short 404
	.byte 0x0c,0x46,0x49,0x4e,0x41,0x4c,0x47,0x44,0x49,0x49,0x4e,0x49,0x54 /* FinalGdiInit */
	.short 405
	.byte 0x18,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x45,0x41,0x4c,0x42,0x49,0x54,0x4d,0x41,0x50,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CREATEREALBITMAPINDIRECT */
	.short 406
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x55,0x53,0x45,0x52,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateUserBitmap */
	.short 407
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x45,0x41,0x4c,0x42,0x49,0x54,0x4d,0x41,0x50 /* CREATEREALBITMAP */
	.short 408
	.byte 0x1b,0x43,0x52,0x45,0x41,0x54,0x45,0x55,0x53,0x45,0x52,0x44,0x49,0x53,0x43,0x41,0x52,0x44,0x41,0x42,0x4c,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateUserDiscardableBitmap */
	.short 409
	.byte 0x0f,0x49,0x53,0x56,0x41,0x4c,0x49,0x44,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* IsValidMetaFile */
	.short 410
	.byte 0x0d,0x47,0x45,0x54,0x43,0x55,0x52,0x4c,0x4f,0x47,0x46,0x4f,0x4e,0x54 /* GetCurLogFont */
	.short 411
	.byte 0x12,0x49,0x53,0x44,0x43,0x43,0x55,0x52,0x52,0x45,0x4e,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* IsDCCurrentPalette */
	.short 412
	.byte 0x0d,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x44,0x49,0x42,0x49,0x54,0x53 /* StretchDIBits */
	.short 439
	.byte 0x09,0x53,0x45,0x54,0x44,0x49,0x42,0x49,0x54,0x53 /* SetDIBits */
	.short 440
	.byte 0x09,0x47,0x45,0x54,0x44,0x49,0x42,0x49,0x54,0x53 /* GetDIBits */
	.short 441
	.byte 0x0e,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x42,0x49,0x54,0x4d,0x41,0x50 /* CreateDIBitmap */
	.short 442
	.byte 0x11,0x53,0x45,0x54,0x44,0x49,0x42,0x49,0x54,0x53,0x54,0x4f,0x44,0x45,0x56,0x49,0x43,0x45 /* SetDIBitsToDevice */
	.short 443
	.byte 0x12,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x4f,0x55,0x4e,0x44,0x52,0x45,0x43,0x54,0x52,0x47,0x4e /* CreateRoundRectRgn */
	.short 444
	.byte 0x15,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x42,0x50,0x41,0x54,0x54,0x45,0x52,0x4e,0x42,0x52,0x55,0x53,0x48 /* CreateDIBPatternBrush */
	.short 445
	.byte 0x10,0x44,0x45,0x56,0x49,0x43,0x45,0x43,0x4f,0x4c,0x4f,0x52,0x4d,0x41,0x54,0x43,0x48 /* DEVICECOLORMATCH */
	.short 449
	.byte 0x0b,0x50,0x4f,0x4c,0x59,0x50,0x4f,0x4c,0x59,0x47,0x4f,0x4e /* PolyPolygon */
	.short 450
	.byte 0x14,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x4f,0x4c,0x59,0x50,0x4f,0x4c,0x59,0x47,0x4f,0x4e,0x52,0x47,0x4e /* CreatePolyPolygonRgn */
	.short 451
	.byte 0x0b,0x47,0x44,0x49,0x53,0x45,0x45,0x47,0x44,0x49,0x44,0x4f /* GdiSeeGdiDo */
	.short 452
	.byte 0x12,0x47,0x44,0x49,0x54,0x41,0x53,0x4b,0x54,0x45,0x52,0x4d,0x49,0x4e,0x41,0x54,0x49,0x4f,0x4e /* GDITASKTERMINATION */
	.short 460
	.byte 0x0e,0x53,0x45,0x54,0x4f,0x42,0x4a,0x45,0x43,0x54,0x4f,0x57,0x4e,0x45,0x52 /* SetObjectOwner */
	.short 461
	.byte 0x0b,0x49,0x53,0x47,0x44,0x49,0x4f,0x42,0x4a,0x45,0x43,0x54 /* IsGDIObject */
	.short 462
	.byte 0x11,0x4d,0x41,0x4b,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54,0x50,0x52,0x49,0x56,0x41,0x54,0x45 /* MakeObjectPrivate */
	.short 463
	.byte 0x1b,0x46,0x49,0x58,0x55,0x50,0x42,0x4f,0x47,0x55,0x53,0x50,0x55,0x42,0x4c,0x49,0x53,0x48,0x45,0x52,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* FIXUPBOGUSPUBLISHERMETAFILE */
	.short 464
	.byte 0x0b,0x52,0x45,0x43,0x54,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* RectVisible */
	.short 465
	.byte 0x0c,0x52,0x45,0x43,0x54,0x49,0x4e,0x52,0x45,0x47,0x49,0x4f,0x4e /* RectInRegion */
	.short 466
	.byte 0x0d,0x55,0x4e,0x49,0x43,0x4f,0x44,0x45,0x54,0x4f,0x41,0x4e,0x53,0x49 /* UNICODETOANSI */
	.short 467
	.byte 0x14,0x47,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x44,0x49,0x4d,0x45,0x4e,0x53,0x49,0x4f,0x4e,0x45,0x58 /* GetBitmapDimensionEx */
	.short 468
	.byte 0x0d,0x47,0x45,0x54,0x42,0x52,0x55,0x53,0x48,0x4f,0x52,0x47,0x45,0x58 /* GetBrushOrgEx */
	.short 469
	.byte 0x14,0x47,0x45,0x54,0x43,0x55,0x52,0x52,0x45,0x4e,0x54,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e,0x45,0x58 /* GetCurrentPositionEx */
	.short 470
	.byte 0x12,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54,0x50,0x4f,0x49,0x4e,0x54 /* GetTextExtentPoint */
	.short 471
	.byte 0x10,0x47,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54,0x45,0x58 /* GetViewportExtEx */
	.short 472
	.byte 0x10,0x47,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47,0x45,0x58 /* GetViewportOrgEx */
	.short 473
	.byte 0x0e,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54,0x45,0x58 /* GetWindowExtEx */
	.short 474
	.byte 0x0e,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47,0x45,0x58 /* GetWindowOrgEx */
	.short 475
	.byte 0x13,0x4f,0x46,0x46,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47,0x45,0x58 /* OffsetViewportOrgEx */
	.short 476
	.byte 0x11,0x4f,0x46,0x46,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47,0x45,0x58 /* OffsetWindowOrgEx */
	.short 477
	.byte 0x14,0x53,0x45,0x54,0x42,0x49,0x54,0x4d,0x41,0x50,0x44,0x49,0x4d,0x45,0x4e,0x53,0x49,0x4f,0x4e,0x45,0x58 /* SetBitmapDimensionEx */
	.short 478
	.byte 0x10,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54,0x45,0x58 /* SetViewportExtEx */
	.short 479
	.byte 0x10,0x53,0x45,0x54,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x4f,0x52,0x47,0x45,0x58 /* SetViewportOrgEx */
	.short 480
	.byte 0x0e,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54,0x45,0x58 /* SetWindowExtEx */
	.short 481
	.byte 0x0e,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4f,0x52,0x47,0x45,0x58 /* SetWindowOrgEx */
	.short 482
	.byte 0x08,0x4d,0x4f,0x56,0x45,0x54,0x4f,0x45,0x58 /* MoveToEx */
	.short 483
	.byte 0x12,0x53,0x43,0x41,0x4c,0x45,0x56,0x49,0x45,0x57,0x50,0x4f,0x52,0x54,0x45,0x58,0x54,0x45,0x58 /* ScaleViewportExtEx */
	.short 484
	.byte 0x10,0x53,0x43,0x41,0x4c,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58,0x54,0x45,0x58 /* ScaleWindowExtEx */
	.short 485
	.byte 0x16,0x47,0x45,0x54,0x41,0x53,0x50,0x45,0x43,0x54,0x52,0x41,0x54,0x49,0x4f,0x46,0x49,0x4c,0x54,0x45,0x52,0x45,0x58 /* GetAspectRatioFilterEx */
	.short 486
	.byte 0x10,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x42,0x53,0x45,0x43,0x54,0x49,0x4f,0x4e /* CreateDIBSection */
	.short 489
	.byte 0x10,0x43,0x4c,0x4f,0x53,0x45,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CloseEnhMetafile */
	.short 490
	.byte 0x0f,0x43,0x4f,0x50,0x59,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CopyEnhMetafile */
	.short 491
	.byte 0x11,0x43,0x52,0x45,0x41,0x54,0x45,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* CreateEnhMetafile */
	.short 492
	.byte 0x11,0x44,0x45,0x4c,0x45,0x54,0x45,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* DeleteEnhMetafile */
	.short 493
	.byte 0x0a,0x47,0x44,0x49,0x43,0x4f,0x4d,0x4d,0x45,0x4e,0x54 /* GDIComment */
	.short 495
	.byte 0x0e,0x47,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45 /* GetEnhMetafile */
	.short 496
	.byte 0x12,0x47,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x42,0x49,0x54,0x53 /* GetEnhMetafileBits */
	.short 497
	.byte 0x19,0x47,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x44,0x45,0x53,0x43,0x52,0x49,0x50,0x54,0x49,0x4f,0x4e /* GetEnhMetafileDescription */
	.short 498
	.byte 0x14,0x47,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* GetEnhMetafileHeader */
	.short 499
	.byte 0x1c,0x47,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45,0x45,0x4e,0x54,0x52,0x49,0x45,0x53 /* GetEnhMetafilePaletteEntries */
	.short 501
	.byte 0x0a,0x50,0x4f,0x4c,0x59,0x42,0x45,0x5a,0x49,0x45,0x52 /* PolyBezier */
	.short 502
	.byte 0x0c,0x50,0x4f,0x4c,0x59,0x42,0x45,0x5a,0x49,0x45,0x52,0x54,0x4f /* PolyBezierTo */
	.short 503
	.byte 0x15,0x50,0x4c,0x41,0x59,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x52,0x45,0x43,0x4f,0x52,0x44 /* PlayEnhMetafileRecord */
	.short 504
	.byte 0x12,0x53,0x45,0x54,0x45,0x4e,0x48,0x4d,0x45,0x54,0x41,0x46,0x49,0x4c,0x45,0x42,0x49,0x54,0x53 /* SetEnhMetafileBits */
	.short 505
	.byte 0x0a,0x53,0x45,0x54,0x4d,0x45,0x54,0x41,0x52,0x47,0x4e /* SetMetaRgn */
	.short 506
	.byte 0x10,0x45,0x58,0x54,0x53,0x45,0x4c,0x45,0x43,0x54,0x43,0x4c,0x49,0x50,0x52,0x47,0x4e /* ExtSelectClipRgn */
	.short 508
	.byte 0x09,0x41,0x42,0x4f,0x52,0x54,0x50,0x41,0x54,0x48 /* AbortPath */
	.short 511
	.byte 0x09,0x42,0x45,0x47,0x49,0x4e,0x50,0x41,0x54,0x48 /* BeginPath */
	.short 512
	.byte 0x0b,0x43,0x4c,0x4f,0x53,0x45,0x46,0x49,0x47,0x55,0x52,0x45 /* CloseFigure */
	.short 513
	.byte 0x07,0x45,0x4e,0x44,0x50,0x41,0x54,0x48 /* EndPath */
	.short 514
	.byte 0x08,0x46,0x49,0x4c,0x4c,0x50,0x41,0x54,0x48 /* FillPath */
	.short 515
	.byte 0x0b,0x46,0x4c,0x41,0x54,0x54,0x45,0x4e,0x50,0x41,0x54,0x48 /* FlattenPath */
	.short 516
	.byte 0x07,0x47,0x45,0x54,0x50,0x41,0x54,0x48 /* GetPath */
	.short 517
	.byte 0x0c,0x50,0x41,0x54,0x48,0x54,0x4f,0x52,0x45,0x47,0x49,0x4f,0x4e /* PathToRegion */
	.short 518
	.byte 0x0e,0x53,0x45,0x4c,0x45,0x43,0x54,0x43,0x4c,0x49,0x50,0x50,0x41,0x54,0x48 /* SelectClipPath */
	.short 519
	.byte 0x11,0x53,0x54,0x52,0x4f,0x4b,0x45,0x41,0x4e,0x44,0x46,0x49,0x4c,0x4c,0x50,0x41,0x54,0x48 /* StrokeAndFillPath */
	.short 520
	.byte 0x0a,0x53,0x54,0x52,0x4f,0x4b,0x45,0x50,0x41,0x54,0x48 /* StrokePath */
	.short 521
	.byte 0x09,0x57,0x49,0x44,0x45,0x4e,0x50,0x41,0x54,0x48 /* WidenPath */
	.short 522
	.byte 0x0c,0x45,0x58,0x54,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x45,0x4e /* ExtCreatePen */
	.short 523
	.byte 0x0f,0x47,0x45,0x54,0x41,0x52,0x43,0x44,0x49,0x52,0x45,0x43,0x54,0x49,0x4f,0x4e /* GetArcDirection */
	.short 524
	.byte 0x0f,0x53,0x45,0x54,0x41,0x52,0x43,0x44,0x49,0x52,0x45,0x43,0x54,0x49,0x4f,0x4e /* SetArcDirection */
	.short 525
	.byte 0x0d,0x47,0x45,0x54,0x4d,0x49,0x54,0x45,0x52,0x4c,0x49,0x4d,0x49,0x54 /* GetMiterLimit */
	.short 526
	.byte 0x0d,0x53,0x45,0x54,0x4d,0x49,0x54,0x45,0x52,0x4c,0x49,0x4d,0x49,0x54 /* SetMiterLimit */
	.short 527
	.byte 0x11,0x47,0x44,0x49,0x50,0x41,0x52,0x41,0x4d,0x45,0x54,0x45,0x52,0x53,0x49,0x4e,0x46,0x4f /* GDIParametersInfo */
	.short 528
	.byte 0x15,0x43,0x52,0x45,0x41,0x54,0x45,0x48,0x41,0x4c,0x46,0x54,0x4f,0x4e,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* CreateHalftonePalette */
	.short 529
	.byte 0x0a,0x52,0x41,0x57,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* RawTextOut */
	.short 530
	.byte 0x0d,0x52,0x41,0x57,0x45,0x58,0x54,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* RawExtTextOut */
	.short 531
	.byte 0x10,0x52,0x41,0x57,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54 /* RawGetTextExtent */
	.short 532
	.byte 0x0a,0x42,0x49,0x44,0x49,0x4c,0x41,0x59,0x4f,0x55,0x54 /* BiDiLayout */
	.short 536
	.byte 0x13,0x42,0x49,0x44,0x49,0x43,0x52,0x45,0x41,0x54,0x45,0x54,0x41,0x42,0x53,0x54,0x52,0x49,0x4e,0x47 /* BiDiCreateTabString */
	.short 538
	.byte 0x0c,0x42,0x49,0x44,0x49,0x47,0x4c,0x59,0x50,0x48,0x4f,0x55,0x54 /* BiDiGlyphOut */
	.short 540
	.byte 0x13,0x42,0x49,0x44,0x49,0x47,0x45,0x54,0x53,0x54,0x52,0x49,0x4e,0x47,0x45,0x58,0x54,0x45,0x4e,0x54 /* BiDiGetStringExtent */
	.short 543
	.byte 0x10,0x42,0x49,0x44,0x49,0x44,0x45,0x4c,0x45,0x54,0x45,0x53,0x54,0x52,0x49,0x4e,0x47 /* BiDiDeleteString */
	.short 555
	.byte 0x0f,0x42,0x49,0x44,0x49,0x53,0x45,0x54,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x53 /* BiDiSetDefaults */
	.short 556
	.byte 0x0f,0x42,0x49,0x44,0x49,0x47,0x45,0x54,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x53 /* BiDiGetDefaults */
	.short 558
	.byte 0x09,0x42,0x49,0x44,0x49,0x53,0x48,0x41,0x50,0x45 /* BiDiShape */
	.short 560
	.byte 0x12,0x42,0x49,0x44,0x49,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4d,0x50,0x4c,0x45,0x4d,0x45,0x4e,0x54 /* BiDiFontComplement */
	.short 561
	.byte 0x0e,0x42,0x49,0x44,0x49,0x53,0x45,0x54,0x4b,0x41,0x53,0x48,0x49,0x44,0x41 /* BiDiSetKashida */
	.short 564
	.byte 0x0f,0x42,0x49,0x44,0x49,0x4b,0x45,0x58,0x54,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* BiDiKExtTextOut */
	.short 565
	.byte 0x0b,0x42,0x49,0x44,0x49,0x53,0x48,0x41,0x50,0x45,0x45,0x58 /* BiDiShapeEx */
	.short 566
	.byte 0x12,0x42,0x49,0x44,0x49,0x43,0x52,0x45,0x41,0x54,0x45,0x53,0x54,0x52,0x49,0x4e,0x47,0x45,0x58 /* BiDiCreateStringEx */
	.short 569
	.byte 0x11,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54,0x52,0x54,0x4f,0x4c /* GetTextExtentRtoL */
	.short 571
	.byte 0x0d,0x47,0x45,0x54,0x48,0x44,0x43,0x43,0x48,0x41,0x52,0x53,0x45,0x54 /* GetHDCCharSet */
	.short 572
	.byte 0x0c,0x42,0x49,0x44,0x49,0x4c,0x41,0x59,0x4f,0x55,0x54,0x45,0x58 /* BiDiLayoutEx */
	.short 573
	.byte 0x10,0x53,0x45,0x54,0x44,0x49,0x42,0x43,0x4f,0x4c,0x4f,0x52,0x54,0x41,0x42,0x4c,0x45 /* SetDIBColorTable */
	.short 602
	.byte 0x10,0x47,0x45,0x54,0x44,0x49,0x42,0x43,0x4f,0x4c,0x4f,0x52,0x54,0x41,0x42,0x4c,0x45 /* GetDIBColorTable */
	.short 603
	.byte 0x0d,0x53,0x45,0x54,0x53,0x4f,0x4c,0x49,0x44,0x42,0x52,0x55,0x53,0x48 /* SetSolidBrush */
	.short 604
	.byte 0x0f,0x53,0x59,0x53,0x44,0x45,0x4c,0x45,0x54,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54 /* SysDeleteObject */
	.short 605
	.byte 0x0e,0x53,0x45,0x54,0x4d,0x41,0x47,0x49,0x43,0x43,0x4f,0x4c,0x4f,0x52,0x53 /* SetMagicColors */
	.short 606
	.byte 0x0d,0x47,0x45,0x54,0x52,0x45,0x47,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41 /* GetRegionData */
	.short 607
	.byte 0x0f,0x45,0x58,0x54,0x43,0x52,0x45,0x41,0x54,0x45,0x52,0x45,0x47,0x49,0x4f,0x4e /* ExtCreateRegion */
	.short 608
	.byte 0x10,0x47,0x44,0x49,0x46,0x52,0x45,0x45,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45,0x53 /* GdiFreeResources */
	.short 609
	.byte 0x0f,0x47,0x44,0x49,0x53,0x49,0x47,0x4e,0x41,0x4c,0x50,0x52,0x4f,0x43,0x33,0x32 /* GdiSignalProc32 */
	.short 610
	.byte 0x0c,0x47,0x45,0x54,0x52,0x41,0x4e,0x44,0x4f,0x4d,0x52,0x47,0x4e /* GetRandomRgn */
	.short 611
	.byte 0x0e,0x47,0x45,0x54,0x54,0x45,0x58,0x54,0x43,0x48,0x41,0x52,0x53,0x45,0x54 /* GetTextCharset */
	.short 612
	.byte 0x12,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x46,0x41,0x4d,0x49,0x4c,0x49,0x45,0x53,0x45,0x58 /* EnumFontFamiliesEx */
	.short 613
	.byte 0x0b,0x41,0x44,0x44,0x4c,0x50,0x4b,0x54,0x4f,0x47,0x44,0x49 /* AddLpkToGDI */
	.short 614
	.byte 0x15,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x41,0x43,0x54,0x45,0x52,0x50,0x4c,0x41,0x43,0x45,0x4d,0x45,0x4e,0x54 /* GetCharacterPlacement */
	.short 615
	.byte 0x13,0x47,0x45,0x54,0x46,0x4f,0x4e,0x54,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x49,0x4e,0x46,0x4f /* GetFontLanguageInfo */
	.short 616
	.byte 0x14,0x42,0x55,0x49,0x4c,0x44,0x49,0x4e,0x56,0x45,0x52,0x53,0x45,0x54,0x41,0x42,0x4c,0x45,0x44,0x49,0x42 /* BuildInverseTableDIB */
	.short 650
	.byte 0x16,0x47,0x44,0x49,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41,0x4c,0x53 /* GDITHKCONNECTIONDATALS */
	.short 701
	.byte 0x1b,0x46,0x54,0x5f,0x47,0x44,0x49,0x46,0x54,0x48,0x4b,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41 /* FT_GDIFTHKTHKCONNECTIONDATA */
	.short 702
	.byte 0x15,0x46,0x44,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41,0x53,0x4c /* FDTHKCONNECTIONDATASL */
	.short 703
	.byte 0x16,0x49,0x43,0x4d,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41,0x53,0x4c /* ICMTHKCONNECTIONDATASL */
	.short 704
	.byte 0x12,0x49,0x43,0x4d,0x43,0x52,0x45,0x41,0x54,0x45,0x54,0x52,0x41,0x4e,0x53,0x46,0x4f,0x52,0x4d /* ICMCreateTransform */
	.short 820
	.byte 0x12,0x49,0x43,0x4d,0x44,0x45,0x4c,0x45,0x54,0x45,0x54,0x52,0x41,0x4e,0x53,0x46,0x4f,0x52,0x4d /* ICMDeleteTransform */
	.short 821
	.byte 0x0f,0x49,0x43,0x4d,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x52,0x47,0x42 /* ICMTranslateRGB */
	.short 822
	.byte 0x10,0x49,0x43,0x4d,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x52,0x47,0x42,0x53 /* ICMTranslateRGBs */
	.short 823
	.byte 0x15,0x49,0x43,0x4d,0x43,0x48,0x45,0x43,0x4b,0x43,0x4f,0x4c,0x4f,0x52,0x53,0x49,0x4e,0x47,0x41,0x4d,0x55,0x54 /* ICMCheckColorsInGamut */
	.short 824
	.byte 0x09,0x53,0x45,0x54,0x4c,0x41,0x59,0x4f,0x55,0x54 /* SetLayout */
	.short 1000
	.byte 0x09,0x47,0x45,0x54,0x4c,0x41,0x59,0x4f,0x55,0x54 /* GetLayout */
	.short 1001
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* GDI.1 - GDI.58 */
	.byte 0x3a,0x01
	.byte 0x03
	.short .L__wine_GDI_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_13-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_27-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_28-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_29-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_31-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_32-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_33-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_34-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_35-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_36-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_37-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_38-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_39-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_40-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_41-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_42-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_43-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_44-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_45-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_46-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_47-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_48-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_49-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_50-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_51-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_52-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_53-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_54-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_55-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_56-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_57-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_58-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.60 - GDI.106 */
	.byte 0x2f,0x01
	.byte 0x03
	.short .L__wine_GDI_60-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_61-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_62-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_63-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_64-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_65-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_66-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_67-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_68-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_69-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_70-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_71-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_72-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_73-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_74-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_75-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_76-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_77-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_78-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_79-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_80-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_81-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_82-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_83-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_84-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_85-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_86-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_87-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_88-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_89-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_90-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_91-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_92-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_93-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_94-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_95-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_96-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_97-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_98-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_99-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_100-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_101-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_102-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_103-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_104-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_105-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_106-.L__wine_spec_code_segment
	.byte 0x0a,0x00
	/* GDI.117 - GDI.134 */
	.byte 0x12,0x01
	.byte 0x03
	.short .L__wine_GDI_117-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_118-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_119-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_120-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_121-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_122-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_123-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_124-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_125-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_126-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_127-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_128-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_129-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_130-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_131-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_132-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_133-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_134-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.136 - GDI.164 */
	.byte 0x1d,0x01
	.byte 0x03
	.short .L__wine_GDI_136-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_137-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_138-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_139-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_140-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_141-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_142-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_143-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_144-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_145-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_146-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_147-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_148-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_149-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_150-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_151-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_152-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_153-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_154-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_155-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_156-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_157-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_158-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_159-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_160-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_161-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_162-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_163-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_164-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* GDI.169 - GDI.186 */
	.byte 0x12,0x01
	.byte 0x03
	.short .L__wine_GDI_169-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_170-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_171-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_172-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_173-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_174-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_175-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_176-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_177-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_178-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_179-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_180-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_181-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_182-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_183-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_184-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_185-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_186-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.188 - GDI.188 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_188-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.190 - GDI.196 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_GDI_190-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_191-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_192-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_193-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_194-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_195-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_196-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* GDI.201 - GDI.202 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_GDI_201-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_202-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* GDI.206 - GDI.220 */
	.byte 0x0f,0x01
	.byte 0x03
	.short .L__wine_GDI_206-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_207-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_208-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_209-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_210-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_211-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_212-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_213-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_214-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_215-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_216-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_217-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_218-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_219-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_220-.L__wine_spec_code_segment
	.byte 0x09,0x00
	/* GDI.230 - GDI.235 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_GDI_230-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_231-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_232-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_233-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_234-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_235-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* GDI.240 - GDI.248 */
	.byte 0x09,0x01
	.byte 0x03
	.short .L__wine_GDI_240-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_241-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_242-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_243-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_244-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_245-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_246-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_247-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_248-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.250 - GDI.250 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_250-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.253 - GDI.254 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_GDI_253-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_254-.L__wine_spec_code_segment
	.byte 0x0c,0x00
	/* GDI.267 - GDI.272 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_GDI_267-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_268-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_269-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_270-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_271-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_272-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.274 - GDI.274 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_274-.L__wine_spec_code_segment
	.byte 0x05,0x00
	/* GDI.280 - GDI.282 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_GDI_280-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_281-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_282-.L__wine_spec_code_segment
	.byte 0x10,0x00
	/* GDI.299 - GDI.317 */
	.byte 0x13,0x01
	.byte 0x03
	.short .L__wine_GDI_299-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_300-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_301-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_302-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_303-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_304-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_305-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_306-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_307-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_308-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_309-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_310-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_311-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_312-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_313-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_314-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_315-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_316-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_317-.L__wine_spec_code_segment
	.byte 0x0c,0x00
	/* GDI.330 - GDI.330 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_330-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.332 - GDI.332 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_332-.L__wine_spec_code_segment
	.byte 0x0c,0x00
	/* GDI.345 - GDI.355 */
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_GDI_345-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_346-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_347-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_348-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_349-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_350-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_351-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_352-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_353-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_354-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_355-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* GDI.360 - GDI.368 */
	.byte 0x09,0x01
	.byte 0x03
	.short .L__wine_GDI_360-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_361-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_362-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_363-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_364-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_365-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_366-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_367-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_368-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.370 - GDI.370 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_370-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.372 - GDI.382 */
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_GDI_372-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_373-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_374-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_375-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_376-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_377-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_378-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_379-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_380-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_381-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_382-.L__wine_spec_code_segment
	.byte 0x11,0x00
	/* GDI.400 - GDI.412 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_GDI_400-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_401-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_402-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_403-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_404-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_405-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_406-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_407-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_408-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_409-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_410-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_411-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_412-.L__wine_spec_code_segment
	.byte 0x1a,0x00
	/* GDI.439 - GDI.445 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_GDI_439-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_440-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_441-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_442-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_443-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_444-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_445-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* GDI.449 - GDI.452 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_GDI_449-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_450-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_451-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_452-.L__wine_spec_code_segment
	.byte 0x07,0x00
	/* GDI.460 - GDI.486 */
	.byte 0x1b,0x01
	.byte 0x03
	.short .L__wine_GDI_460-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_461-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_462-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_463-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_464-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_465-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_466-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_467-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_468-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_469-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_470-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_471-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_472-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_473-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_474-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_475-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_476-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_477-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_478-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_479-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_480-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_481-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_482-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_483-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_484-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_485-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_486-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.489 - GDI.493 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_GDI_489-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_490-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_491-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_492-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_493-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.495 - GDI.499 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_GDI_495-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_496-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_497-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_498-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_499-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.501 - GDI.506 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_GDI_501-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_502-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_503-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_504-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_505-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_506-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.508 - GDI.508 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_508-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.511 - GDI.532 */
	.byte 0x16,0x01
	.byte 0x03
	.short .L__wine_GDI_511-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_512-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_513-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_514-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_515-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_516-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_517-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_518-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_519-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_520-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_521-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_522-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_523-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_524-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_525-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_526-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_527-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_528-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_529-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_530-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_531-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_532-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* GDI.536 - GDI.536 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_536-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.538 - GDI.538 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_538-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.540 - GDI.540 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_540-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.543 - GDI.543 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_543-.L__wine_spec_code_segment
	.byte 0x0b,0x00
	/* GDI.555 - GDI.556 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_GDI_555-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_556-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.558 - GDI.558 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_558-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.560 - GDI.561 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_GDI_560-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_561-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.564 - GDI.566 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_GDI_564-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_565-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_566-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* GDI.569 - GDI.569 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_569-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* GDI.571 - GDI.573 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_GDI_571-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_572-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_573-.L__wine_spec_code_segment
	.byte 0x1c,0x00
	/* GDI.602 - GDI.616 */
	.byte 0x0f,0x01
	.byte 0x03
	.short .L__wine_GDI_602-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_603-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_604-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_605-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_606-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_607-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_608-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_609-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_610-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_611-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_612-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_613-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_614-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_615-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_616-.L__wine_spec_code_segment
	.byte 0x21,0x00
	/* GDI.650 - GDI.650 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_GDI_650-.L__wine_spec_code_segment
	.byte 0x32,0x00
	/* GDI.701 - GDI.704 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_GDI_701-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_702-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_703-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_704-.L__wine_spec_code_segment
	.byte 0x73,0x00
	/* GDI.820 - GDI.824 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_GDI_820-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_821-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_822-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_823-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_824-.L__wine_spec_code_segment
	.byte 0xaf,0x00
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_GDI_1000-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_GDI_1001-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_llll:
	pushl $.L__wine_spec_call16_p_llll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000006db,0x00000000
.L__wine_spec_callfrom16_p_long_ppp:
	pushl $.L__wine_spec_call16_p_ppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00000124,0x00000000
.L__wine_spec_callfrom16_p_long_tll:
	pushl $.L__wine_spec_call16_p_pll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000000dd,0x00000000
.L__wine_spec_callfrom16_p_long_ttlpl:
	pushl $.L__wine_spec_call16_p_pplpl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $20
	.short 0x86c7
	.long 0x000038ed,0x00000000
.L__wine_spec_callfrom16_p_long_ttpplp:
	pushl $.L__wine_spec_call16_p_pppplp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $24
	.short 0x86c7
	.long 0x0002392d,0x00000000
.L__wine_spec_callfrom16_p_long_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $2
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_long_wl:
	pushl $.L__wine_spec_call16_p_wl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000019,0x00000000
.L__wine_spec_callfrom16_p_long_wllpl:
	pushl $.L__wine_spec_call16_p_wllpl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $18
	.short 0x86c7
	.long 0x000038d9,0x00000000
.L__wine_spec_callfrom16_p_long_wlp:
	pushl $.L__wine_spec_call16_p_wlp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000119,0x00000000
.L__wine_spec_callfrom16_p_long_wp:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000021,0x00000000
.L__wine_spec_callfrom16_p_long_wpp:
	pushl $.L__wine_spec_call16_p_wpp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000121,0x00000000
.L__wine_spec_callfrom16_p_long_wps:
	pushl $.L__wine_spec_call16_p_wps
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x000000a1,0x00000000
.L__wine_spec_callfrom16_p_long_wss:
	pushl $.L__wine_spec_call16_p_wss
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000091,0x00000000
.L__wine_spec_callfrom16_p_long_wssl:
	pushl $.L__wine_spec_call16_p_wssl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000691,0x00000000
.L__wine_spec_callfrom16_p_long_wssss:
	pushl $.L__wine_spec_call16_p_wssss
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00002491,0x00000000
.L__wine_spec_callfrom16_p_long_wwwlp:
	pushl $.L__wine_spec_call16_p_wwwlp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00004649,0x00000000
.L__wine_spec_callfrom16_p_long_wwwplpp:
	pushl $.L__wine_spec_call16_p_wwwplpp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $22
	.short 0x86c7
	.long 0x00123849,0x00000000
.L__wine_spec_callfrom16_p_long_wwww:
	pushl $.L__wine_spec_call16_p_wwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000249,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_p_word_l:
	pushl $.L__wine_spec_call16_p_l
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000003,0x00000000
.L__wine_spec_callfrom16_p_word_lllw:
	pushl $.L__wine_spec_call16_p_lllw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000002db,0x00000000
.L__wine_spec_callfrom16_p_word_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_word_ppw:
	pushl $.L__wine_spec_call16_p_ppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000064,0x00000000
.L__wine_spec_callfrom16_p_word_ppww:
	pushl $.L__wine_spec_call16_p_ppww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000264,0x00000000
.L__wine_spec_callfrom16_p_word_pw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000000c,0x00000000
.L__wine_spec_callfrom16_p_word_pww:
	pushl $.L__wine_spec_call16_p_pww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000004c,0x00000000
.L__wine_spec_callfrom16_p_word_pwwp:
	pushl $.L__wine_spec_call16_p_pwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000084c,0x00000000
.L__wine_spec_callfrom16_p_word_ssl:
	pushl $.L__wine_spec_call16_p_ssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000d2,0x00000000
.L__wine_spec_callfrom16_p_word_sss:
	pushl $.L__wine_spec_call16_p_sss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000092,0x00000000
.L__wine_spec_callfrom16_p_word_ssss:
	pushl $.L__wine_spec_call16_p_ssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000492,0x00000000
.L__wine_spec_callfrom16_p_word_ssssll:
	pushl $.L__wine_spec_call16_p_ssssll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0001b492,0x00000000
.L__wine_spec_callfrom16_p_word_ssssss:
	pushl $.L__wine_spec_call16_p_ssssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00012492,0x00000000
.L__wine_spec_callfrom16_p_word_ssssswwwwwwwwt:
	pushl $.L__wine_spec_call16_p_ssssswwwwwwwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $30
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0924a492,0x00000a49
.L__wine_spec_callfrom16_p_word_t:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000005,0x00000000
.L__wine_spec_callfrom16_p_word_tttp:
	pushl $.L__wine_spec_call16_p_pppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000096d,0x00000000
.L__wine_spec_callfrom16_p_word_ttw:
	pushl $.L__wine_spec_call16_p_ppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000006d,0x00000000
.L__wine_spec_callfrom16_p_word_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $2
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_word_wl:
	pushl $.L__wine_spec_call16_p_wl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000019,0x00000000
.L__wine_spec_callfrom16_p_word_wll:
	pushl $.L__wine_spec_call16_p_wll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000d9,0x00000000
.L__wine_spec_callfrom16_p_word_wlw:
	pushl $.L__wine_spec_call16_p_wlw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000059,0x00000000
.L__wine_spec_callfrom16_p_word_wp:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000021,0x00000000
.L__wine_spec_callfrom16_p_word_wplll:
	pushl $.L__wine_spec_call16_p_wplll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000036e1,0x00000000
.L__wine_spec_callfrom16_p_word_wplppw:
	pushl $.L__wine_spec_call16_p_wplppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000c8e1,0x00000000
.L__wine_spec_callfrom16_p_word_wppw:
	pushl $.L__wine_spec_call16_p_wppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000321,0x00000000
.L__wine_spec_callfrom16_p_word_wps:
	pushl $.L__wine_spec_call16_p_wps
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000a1,0x00000000
.L__wine_spec_callfrom16_p_word_wpsp:
	pushl $.L__wine_spec_call16_p_wpsp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000008a1,0x00000000
.L__wine_spec_callfrom16_p_word_wpssl:
	pushl $.L__wine_spec_call16_p_wpssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000034a1,0x00000000
.L__wine_spec_callfrom16_p_word_wpw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000061,0x00000000
.L__wine_spec_callfrom16_p_word_wpwpll:
	pushl $.L__wine_spec_call16_p_wpwpll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0001b861,0x00000000
.L__wine_spec_callfrom16_p_word_wpwwplp:
	pushl $.L__wine_spec_call16_p_wpwwplp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $22
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0011c261,0x00000000
.L__wine_spec_callfrom16_p_word_ws:
	pushl $.L__wine_spec_call16_p_ws
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000011,0x00000000
.L__wine_spec_callfrom16_p_word_wsp:
	pushl $.L__wine_spec_call16_p_wsp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000111,0x00000000
.L__wine_spec_callfrom16_p_word_wss:
	pushl $.L__wine_spec_call16_p_wss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000091,0x00000000
.L__wine_spec_callfrom16_p_word_wssl:
	pushl $.L__wine_spec_call16_p_wssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000691,0x00000000
.L__wine_spec_callfrom16_p_word_wsslw:
	pushl $.L__wine_spec_call16_p_wsslw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001691,0x00000000
.L__wine_spec_callfrom16_p_word_wssp:
	pushl $.L__wine_spec_call16_p_wssp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000891,0x00000000
.L__wine_spec_callfrom16_p_word_wssss:
	pushl $.L__wine_spec_call16_p_wssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00002491,0x00000000
.L__wine_spec_callfrom16_p_word_wssssl:
	pushl $.L__wine_spec_call16_p_wssssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0001a491,0x00000000
.L__wine_spec_callfrom16_p_word_wssssp:
	pushl $.L__wine_spec_call16_p_wssssp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00022491,0x00000000
.L__wine_spec_callfrom16_p_word_wssssss:
	pushl $.L__wine_spec_call16_p_wssssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00092491,0x00000000
.L__wine_spec_callfrom16_p_word_wssssssss:
	pushl $.L__wine_spec_call16_p_wssssssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x02492491,0x00000000
.L__wine_spec_callfrom16_p_word_wssssssssppwl:
	pushl $.L__wine_spec_call16_p_wssssssssppwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $32
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x22492491,0x000000cc
.L__wine_spec_callfrom16_p_word_wsssssswwppw:
	pushl $.L__wine_spec_call16_p_wsssssswwppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $28
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x21292491,0x0000000c
.L__wine_spec_callfrom16_p_word_wsssswssl:
	pushl $.L__wine_spec_call16_p_wsssswssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0348a491,0x00000000
.L__wine_spec_callfrom16_p_word_wsssswssssl:
	pushl $.L__wine_spec_call16_p_wsssswssssl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $24
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x1248a491,0x00000003
.L__wine_spec_callfrom16_p_word_wsstw:
	pushl $.L__wine_spec_call16_p_wsspw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001a91,0x00000000
.L__wine_spec_callfrom16_p_word_wsswptwp:
	pushl $.L__wine_spec_call16_p_wsswppwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $22
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0086c291,0x00000000
.L__wine_spec_callfrom16_p_word_wt:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000029,0x00000000
.L__wine_spec_callfrom16_p_word_wtll:
	pushl $.L__wine_spec_call16_p_wpll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000006e9,0x00000000
.L__wine_spec_callfrom16_p_word_wttt:
	pushl $.L__wine_spec_call16_p_wppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000b69,0x00000000
.L__wine_spec_callfrom16_p_word_wtw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000069,0x00000000
.L__wine_spec_callfrom16_p_word_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_word_wwll:
	pushl $.L__wine_spec_call16_p_wwll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000006c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwp:
	pushl $.L__wine_spec_call16_p_wwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000109,0x00000000
.L__wine_spec_callfrom16_p_word_www:
	pushl $.L__wine_spec_call16_p_www
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000049,0x00000000
.L__wine_spec_callfrom16_p_word_wwwp:
	pushl $.L__wine_spec_call16_p_wwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000849,0x00000000
.L__wine_spec_callfrom16_p_word_wwws:
	pushl $.L__wine_spec_call16_p_wwws
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000449,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwp:
	pushl $.L__wine_spec_call16_p_wwwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00004249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwppw:
	pushl $.L__wine_spec_call16_p_wwwwppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00064249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwww:
	pushl $.L__wine_spec_call16_p_wwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwwww:
	pushl $.L__wine_spec_call16_p_wwwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00049249,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_GDI_1:
	pushw %bp
	pushl $_SetBkColor16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_GDI_2:
	pushw %bp
	pushl $_SetBkMode16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_3:
	pushw %bp
	pushl $_SetMapMode16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_4:
	pushw %bp
	pushl $_SetROP216@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_5:
	pushw %bp
	pushl $_SetRelAbs16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_6:
	pushw %bp
	pushl $_SetPolyFillMode16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_7:
	pushw %bp
	pushl $_SetStretchBltMode16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_8:
	pushw %bp
	pushl $_SetTextCharacterExtra16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_GDI_9:
	pushw %bp
	pushl $_SetTextColor16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_GDI_10:
	pushw %bp
	pushl $_SetTextJustification16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_11:
	pushw %bp
	pushl $_SetWindowOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_12:
	pushw %bp
	pushl $_SetWindowExt16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_13:
	pushw %bp
	pushl $_SetViewportOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_14:
	pushw %bp
	pushl $_SetViewportExt16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_15:
	pushw %bp
	pushl $_OffsetWindowOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_16:
	pushw %bp
	pushl $_ScaleWindowExt16@20
	callw .L__wine_spec_callfrom16_p_long_wssss
.L__wine_GDI_17:
	pushw %bp
	pushl $_OffsetViewportOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_18:
	pushw %bp
	pushl $_ScaleViewportExt16@20
	callw .L__wine_spec_callfrom16_p_long_wssss
.L__wine_GDI_19:
	pushw %bp
	pushl $_LineTo16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_20:
	pushw %bp
	pushl $_MoveTo16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_21:
	pushw %bp
	pushl $_ExcludeClipRect16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_22:
	pushw %bp
	pushl $_IntersectClipRect16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_23:
	pushw %bp
	pushl $_Arc16@36
	callw .L__wine_spec_callfrom16_p_word_wssssssss
.L__wine_GDI_24:
	pushw %bp
	pushl $_Ellipse16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_25:
	pushw %bp
	pushl $_FloodFill16@16
	callw .L__wine_spec_callfrom16_p_word_wssl
.L__wine_GDI_26:
	pushw %bp
	pushl $_Pie16@36
	callw .L__wine_spec_callfrom16_p_word_wssssssss
.L__wine_GDI_27:
	pushw %bp
	pushl $_Rectangle16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_28:
	pushw %bp
	pushl $_RoundRect16@28
	callw .L__wine_spec_callfrom16_p_word_wssssss
.L__wine_GDI_29:
	pushw %bp
	pushl $_PatBlt16@24
	callw .L__wine_spec_callfrom16_p_word_wssssl
.L__wine_GDI_30:
	pushw %bp
	pushl $_SaveDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_31:
	pushw %bp
	pushl $_SetPixel16@16
	callw .L__wine_spec_callfrom16_p_long_wssl
.L__wine_GDI_32:
	pushw %bp
	pushl $_OffsetClipRgn16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_33:
	pushw %bp
	pushl $_TextOut16@20
	callw .L__wine_spec_callfrom16_p_word_wsstw
.L__wine_GDI_34:
	pushw %bp
	pushl $_BitBlt16@36
	callw .L__wine_spec_callfrom16_p_word_wsssswssl
.L__wine_GDI_35:
	pushw %bp
	pushl $_StretchBlt16@44
	callw .L__wine_spec_callfrom16_p_word_wsssswssssl
.L__wine_GDI_36:
	pushw %bp
	pushl $_Polygon16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_37:
	pushw %bp
	pushl $_Polyline16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_38:
	pushw %bp
	pushl $_Escape16@20
	callw .L__wine_spec_callfrom16_p_long_wwwlp
.L__wine_GDI_39:
	pushw %bp
	pushl $_RestoreDC16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_GDI_40:
	pushw %bp
	pushl $_FillRgn16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_41:
	pushw %bp
	pushl $_FrameRgn16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_GDI_42:
	pushw %bp
	pushl $_InvertRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_43:
	pushw %bp
	pushl $_PaintRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_44:
	pushw %bp
	pushl $_SelectClipRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_45:
	pushw %bp
	pushl $_SelectObject16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_46:
	pushw %bp
	pushl $___wine_stub_BITMAPBITS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_47:
	pushw %bp
	pushl $_CombineRgn16@16
	callw .L__wine_spec_callfrom16_p_word_wwws
.L__wine_GDI_48:
	pushw %bp
	pushl $_CreateBitmap16@20
	callw .L__wine_spec_callfrom16_p_word_wwwwp
.L__wine_GDI_49:
	pushw %bp
	pushl $_CreateBitmapIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_50:
	pushw %bp
	pushl $_CreateBrushIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_51:
	pushw %bp
	pushl $_CreateCompatibleBitmap16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_52:
	pushw %bp
	pushl $_CreateCompatibleDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_53:
	pushw %bp
	pushl $_CreateDC16@16
	callw .L__wine_spec_callfrom16_p_word_tttp
.L__wine_GDI_54:
	pushw %bp
	pushl $_CreateEllipticRgn16@16
	callw .L__wine_spec_callfrom16_p_word_ssss
.L__wine_GDI_55:
	pushw %bp
	pushl $_CreateEllipticRgnIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_56:
	pushw %bp
	pushl $_CreateFont16@56
	callw .L__wine_spec_callfrom16_p_word_ssssswwwwwwwwt
.L__wine_GDI_57:
	pushw %bp
	pushl $_CreateFontIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_58:
	pushw %bp
	pushl $_CreateHatchBrush16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_GDI_60:
	pushw %bp
	pushl $_CreatePatternBrush16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_61:
	pushw %bp
	pushl $_CreatePen16@12
	callw .L__wine_spec_callfrom16_p_word_ssl
.L__wine_GDI_62:
	pushw %bp
	pushl $_CreatePenIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_63:
	pushw %bp
	pushl $_CreatePolygonRgn16@12
	callw .L__wine_spec_callfrom16_p_word_pww
.L__wine_GDI_64:
	pushw %bp
	pushl $_CreateRectRgn16@16
	callw .L__wine_spec_callfrom16_p_word_ssss
.L__wine_GDI_65:
	pushw %bp
	pushl $_CreateRectRgnIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_66:
	pushw %bp
	pushl $_CreateSolidBrush16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_GDI_67:
	pushw %bp
	pushl $_DPtoLP16@12
	callw .L__wine_spec_callfrom16_p_word_wps
.L__wine_GDI_68:
	pushw %bp
	pushl $_DeleteDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_69:
	pushw %bp
	pushl $_DeleteObject16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_70:
	pushw %bp
	pushl $_EnumFonts16@16
	callw .L__wine_spec_callfrom16_p_word_wtll
.L__wine_GDI_71:
	pushw %bp
	pushl $_EnumObjects16@16
	callw .L__wine_spec_callfrom16_p_word_wwll
.L__wine_GDI_72:
	pushw %bp
	pushl $_EqualRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_73:
	pushw %bp
	pushl $_ExcludeVisRect16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_74:
	pushw %bp
	pushl $_GetBitmapBits16@12
	callw .L__wine_spec_callfrom16_p_long_wlp
.L__wine_GDI_75:
	pushw %bp
	pushl $_GetBkColor16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_76:
	pushw %bp
	pushl $_GetBkMode16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_77:
	pushw %bp
	pushl $_GetClipBox16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_78:
	pushw %bp
	pushl $_GetCurrentPosition16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_79:
	pushw %bp
	pushl $_GetDCOrg16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_80:
	pushw %bp
	pushl $_GetDeviceCaps16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_GDI_81:
	pushw %bp
	pushl $_GetMapMode16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_82:
	pushw %bp
	pushl $_GetObject16@12
	callw .L__wine_spec_callfrom16_p_word_wsp
.L__wine_GDI_83:
	pushw %bp
	pushl $_GetPixel16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_84:
	pushw %bp
	pushl $_GetPolyFillMode16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_85:
	pushw %bp
	pushl $_GetROP216@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_86:
	pushw %bp
	pushl $_GetRelAbs16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_87:
	pushw %bp
	pushl $_GetStockObject16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_88:
	pushw %bp
	pushl $_GetStretchBltMode16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_89:
	pushw %bp
	pushl $_GetTextCharacterExtra16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_90:
	pushw %bp
	pushl $_GetTextColor16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_91:
	pushw %bp
	pushl $_GetTextExtent16@12
	callw .L__wine_spec_callfrom16_p_long_wps
.L__wine_GDI_92:
	pushw %bp
	pushl $_GetTextFace16@12
	callw .L__wine_spec_callfrom16_p_word_wsp
.L__wine_GDI_93:
	pushw %bp
	pushl $_GetTextMetrics16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_94:
	pushw %bp
	pushl $_GetViewportExt16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_95:
	pushw %bp
	pushl $_GetViewportOrg16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_96:
	pushw %bp
	pushl $_GetWindowExt16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_97:
	pushw %bp
	pushl $_GetWindowOrg16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_98:
	pushw %bp
	pushl $_IntersectVisRect16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_99:
	pushw %bp
	pushl $_LPtoDP16@12
	callw .L__wine_spec_callfrom16_p_word_wps
.L__wine_GDI_100:
	pushw %bp
	pushl $_LineDDA16@24
	callw .L__wine_spec_callfrom16_p_word_ssssll
.L__wine_GDI_101:
	pushw %bp
	pushl $_OffsetRgn16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_102:
	pushw %bp
	pushl $_OffsetVisRgn16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_103:
	pushw %bp
	pushl $_PtVisible16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_104:
	pushw %bp
	pushl $_RectVisible16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_105:
	pushw %bp
	pushl $_SelectVisRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_106:
	pushw %bp
	pushl $_SetBitmapBits16@12
	callw .L__wine_spec_callfrom16_p_long_wlp
.L__wine_GDI_117:
	pushw %bp
	pushl $_SetDCOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_118:
	pushw %bp
	pushl $___wine_stub_InternalCreateDC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_119:
	pushw %bp
	pushl $_AddFontResource16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_GDI_120:
	pushw %bp
	pushl $___wine_stub_GetContinuingTextExtent
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_121:
	pushw %bp
	pushl $_Death16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_122:
	pushw %bp
	pushl $_Resurrection16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwwww
.L__wine_GDI_123:
	pushw %bp
	pushl $_PlayMetaFile16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_124:
	pushw %bp
	pushl $_GetMetaFile16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_GDI_125:
	pushw %bp
	pushl $_CreateMetaFile16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_GDI_126:
	pushw %bp
	pushl $_CloseMetaFile16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_127:
	pushw %bp
	pushl $_DeleteMetaFile16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_128:
	pushw %bp
	pushl $_MulDiv16@12
	callw .L__wine_spec_callfrom16_p_word_sss
.L__wine_GDI_129:
	pushw %bp
	pushl $_SaveVisRgn16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_130:
	pushw %bp
	pushl $_RestoreVisRgn16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_131:
	pushw %bp
	pushl $_InquireVisRgn16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_132:
	pushw %bp
	pushl $_SetEnvironment16@12
	callw .L__wine_spec_callfrom16_p_word_ttw
.L__wine_GDI_133:
	pushw %bp
	pushl $_GetEnvironment16@12
	callw .L__wine_spec_callfrom16_p_word_ttw
.L__wine_GDI_134:
	pushw %bp
	pushl $_GetRgnBox16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_136:
	pushw %bp
	pushl $_RemoveFontResource16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_GDI_137:
	pushw %bp
	pushl $___wine_stub_GSV
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_138:
	pushw %bp
	pushl $___wine_stub_DPXlate
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_139:
	pushw %bp
	pushl $___wine_stub_SetWinViewExt
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_140:
	pushw %bp
	pushl $___wine_stub_ScaleExt
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_141:
	pushw %bp
	pushl $___wine_stub_WordSet
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_142:
	pushw %bp
	pushl $___wine_stub_RectStuff
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_143:
	pushw %bp
	pushl $___wine_stub_OffsetOrg
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_144:
	pushw %bp
	pushl $___wine_stub_LockDC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_145:
	pushw %bp
	pushl $___wine_stub_UnlockDC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_146:
	pushw %bp
	pushl $___wine_stub_LockUnlock
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_147:
	pushw %bp
	pushl $___wine_stub_GDI_FarFrame
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_148:
	pushw %bp
	pushl $_SetBrushOrg16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_149:
	pushw %bp
	pushl $_GetBrushOrg16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_150:
	pushw %bp
	pushl $_UnrealizeObject16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_151:
	pushw %bp
	pushl $_CopyMetaFile16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_GDI_152:
	pushw %bp
	pushl $___wine_stub_GDIInitApp
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_153:
	pushw %bp
	pushl $_CreateIC16@16
	callw .L__wine_spec_callfrom16_p_word_tttp
.L__wine_GDI_154:
	pushw %bp
	pushl $_GetNearestColor16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_GDI_155:
	pushw %bp
	pushl $_QueryAbort16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_156:
	pushw %bp
	pushl $_CreateDiscardableBitmap16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_157:
	pushw %bp
	pushl $___wine_stub_CompatibleBitmap
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_158:
	pushw %bp
	pushl $___wine_stub_EnumCallback
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_159:
	pushw %bp
	pushl $_GetMetaFileBits16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_160:
	pushw %bp
	pushl $_SetMetaFileBits16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_161:
	pushw %bp
	pushl $_PtInRegion16@12
	callw .L__wine_spec_callfrom16_p_word_wss
.L__wine_GDI_162:
	pushw %bp
	pushl $_GetBitmapDimension16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_163:
	pushw %bp
	pushl $_SetBitmapDimension16@12
	callw .L__wine_spec_callfrom16_p_long_wss
.L__wine_GDI_164:
	pushw %bp
	pushl $___wine_stub_PixToLine
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_169:
	pushw %bp
	pushl $___wine_stub_IsDCDirty
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_170:
	pushw %bp
	pushl $___wine_stub_SetDCStatus
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_171:
	pushw %bp
	pushl $___wine_stub_LVBUNION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_172:
	pushw %bp
	pushl $_SetRectRgn16@20
	callw .L__wine_spec_callfrom16_p_word_wssss
.L__wine_GDI_173:
	pushw %bp
	pushl $_GetClipRgn16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_174:
	pushw %bp
	pushl $___wine_stub_BLOAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_175:
	pushw %bp
	pushl $_EnumMetaFile16@16
	callw .L__wine_spec_callfrom16_p_word_wwll
.L__wine_GDI_176:
	pushw %bp
	pushl $_PlayMetaFileRecord16@16
	callw .L__wine_spec_callfrom16_p_word_wppw
.L__wine_GDI_177:
	pushw %bp
	pushl $___wine_stub_RCOS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_178:
	pushw %bp
	pushl $___wine_stub_RSIN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_179:
	pushw %bp
	pushl $_GetDCState16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_180:
	pushw %bp
	pushl $_SetDCState16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_181:
	pushw %bp
	pushl $_RectInRegion16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_182:
	pushw %bp
	pushl $___wine_stub_REQUESTSEM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_183:
	pushw %bp
	pushl $___wine_stub_CLEARSEM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_184:
	pushw %bp
	pushl $___wine_stub_STUFFVISIBLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_185:
	pushw %bp
	pushl $___wine_stub_STUFFINREGION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_186:
	pushw %bp
	pushl $___wine_stub_DELETEABOVELINEFONTS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_188:
	pushw %bp
	pushl $___wine_stub_GetTextExtentEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_190:
	pushw %bp
	pushl $_SetDCHook16@12
	callw .L__wine_spec_callfrom16_p_word_wll
.L__wine_GDI_191:
	pushw %bp
	pushl $_GetDCHook16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_GDI_192:
	pushw %bp
	pushl $_SetHookFlags16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_193:
	pushw %bp
	pushl $_SetBoundsRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_194:
	pushw %bp
	pushl $_GetBoundsRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_195:
	pushw %bp
	pushl $___wine_stub_SelectBitmap
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_196:
	pushw %bp
	pushl $_SetMetaFileBitsBetter16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_201:
	pushw %bp
	pushl $___wine_stub_DMBITBLT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_202:
	pushw %bp
	pushl $___wine_stub_DMCOLORINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_206:
	pushw %bp
	pushl $___wine_stub_dmEnumDFonts
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_207:
	pushw %bp
	pushl $___wine_stub_DMENUMOBJ
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_208:
	pushw %bp
	pushl $___wine_stub_DMOUTPUT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_209:
	pushw %bp
	pushl $___wine_stub_DMPIXEL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_210:
	pushw %bp
	pushl $___wine_stub_dmRealizeObject
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_211:
	pushw %bp
	pushl $___wine_stub_DMSTRBLT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_212:
	pushw %bp
	pushl $___wine_stub_DMSCANLR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_213:
	pushw %bp
	pushl $___wine_stub_BRUTE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_214:
	pushw %bp
	pushl $___wine_stub_DMEXTTEXTOUT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_215:
	pushw %bp
	pushl $___wine_stub_DMGETCHARWIDTH
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_216:
	pushw %bp
	pushl $___wine_stub_DMSTRETCHBLT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_217:
	pushw %bp
	pushl $___wine_stub_DMDIBBITS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_218:
	pushw %bp
	pushl $___wine_stub_DMSTRETCHDIBITS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_219:
	pushw %bp
	pushl $___wine_stub_DMSETDIBTODEV
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_220:
	pushw %bp
	pushl $___wine_stub_DMTRANSPOSE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_230:
	pushw %bp
	pushl $_CreatePQ16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_231:
	pushw %bp
	pushl $_MinPQ16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_232:
	pushw %bp
	pushl $_ExtractPQ16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_233:
	pushw %bp
	pushl $_InsertPQ16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_234:
	pushw %bp
	pushl $_SizePQ16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_235:
	pushw %bp
	pushl $_DeletePQ16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_240:
	pushw %bp
	pushl $_OpenJob16@12
	callw .L__wine_spec_callfrom16_p_word_ttw
.L__wine_GDI_241:
	pushw %bp
	pushl $_WriteSpool16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_242:
	pushw %bp
	pushl $_WriteDialog16@12
	callw .L__wine_spec_callfrom16_p_word_wtw
.L__wine_GDI_243:
	pushw %bp
	pushl $_CloseJob16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_244:
	pushw %bp
	pushl $_DeleteJob16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_245:
	pushw %bp
	pushl $_GetSpoolJob16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_GDI_246:
	pushw %bp
	pushl $_StartSpoolPage16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_247:
	pushw %bp
	pushl $_EndSpoolPage16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_248:
	pushw %bp
	pushl $___wine_stub_QueryJob
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_250:
	pushw %bp
	pushl $_Copy16@12
	callw .L__wine_spec_callfrom16_p_word_ppw
.L__wine_GDI_253:
	pushw %bp
	pushl $___wine_stub_DeleteSpoolPage
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_254:
	pushw %bp
	pushl $___wine_stub_SpoolFile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_267:
	pushw %bp
	pushl $___wine_stub_StartDocPrintEra
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_268:
	pushw %bp
	pushl $___wine_stub_StartPagePrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_269:
	pushw %bp
	pushl $___wine_stub_WritePrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_270:
	pushw %bp
	pushl $___wine_stub_EndPagePrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_271:
	pushw %bp
	pushl $___wine_stub_AbortPrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_272:
	pushw %bp
	pushl $___wine_stub_EndDocPrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_274:
	pushw %bp
	pushl $___wine_stub_ClosePrinter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_280:
	pushw %bp
	pushl $___wine_stub_GetRealDriverInfo
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_281:
	pushw %bp
	pushl $_DrvSetPrinterData16@20
	callw .L__wine_spec_callfrom16_p_long_ttlpl
.L__wine_GDI_282:
	pushw %bp
	pushl $_DrvGetPrinterData16@24
	callw .L__wine_spec_callfrom16_p_long_ttpplp
.L__wine_GDI_299:
	pushw %bp
	pushl $___wine_stub_ENGINEGETCHARWIDTHEX
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_300:
	pushw %bp
	pushl $_EngineEnumerateFont16@12
	callw .L__wine_spec_callfrom16_p_long_tll
.L__wine_GDI_301:
	pushw %bp
	pushl $_EngineDeleteFont16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_302:
	pushw %bp
	pushl $_EngineRealizeFont16@12
	callw .L__wine_spec_callfrom16_p_long_ppp
.L__wine_GDI_303:
	pushw %bp
	pushl $_EngineGetCharWidth16@16
	callw .L__wine_spec_callfrom16_p_word_pwwp
.L__wine_GDI_304:
	pushw %bp
	pushl $_EngineSetFontContext16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_GDI_305:
	pushw %bp
	pushl $_EngineGetGlyphBMP16@28
	callw .L__wine_spec_callfrom16_p_word_wpwwplp
.L__wine_GDI_306:
	pushw %bp
	pushl $_EngineMakeFontDir16@12
	callw .L__wine_spec_callfrom16_p_long_wpp
.L__wine_GDI_307:
	pushw %bp
	pushl $_GetCharABCWidths16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_308:
	pushw %bp
	pushl $_GetOutlineTextMetrics16@12
	callw .L__wine_spec_callfrom16_p_word_wwp
.L__wine_GDI_309:
	pushw %bp
	pushl $_GetGlyphOutline16@28
	callw .L__wine_spec_callfrom16_p_long_wwwplpp
.L__wine_GDI_310:
	pushw %bp
	pushl $_CreateScalableFontResource16@16
	callw .L__wine_spec_callfrom16_p_word_wttt
.L__wine_GDI_311:
	pushw %bp
	pushl $_GetFontData16@20
	callw .L__wine_spec_callfrom16_p_long_wllpl
.L__wine_GDI_312:
	pushw %bp
	pushl $___wine_stub_ConvertOutLineFontFile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_313:
	pushw %bp
	pushl $_GetRasterizerCaps16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_GDI_314:
	pushw %bp
	pushl $___wine_stub_EngineExtTextOut
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_315:
	pushw %bp
	pushl $_EngineRealizeFontExt16@16
	callw .L__wine_spec_callfrom16_p_long_llll
.L__wine_GDI_316:
	pushw %bp
	pushl $___wine_stub_EngineGetCharWidthStr
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_317:
	pushw %bp
	pushl $___wine_stub_EngineGetGlyphBmpExt
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_330:
	pushw %bp
	pushl $_EnumFontFamilies16@16
	callw .L__wine_spec_callfrom16_p_word_wtll
.L__wine_GDI_332:
	pushw %bp
	pushl $_GetKerningPairs16@12
	callw .L__wine_spec_callfrom16_p_word_wwp
.L__wine_GDI_345:
	pushw %bp
	pushl $_GetTextAlign16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_346:
	pushw %bp
	pushl $_SetTextAlign16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_347:
	pushw %bp
	pushl $___wine_stub_MFDRAWTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_348:
	pushw %bp
	pushl $_Chord16@36
	callw .L__wine_spec_callfrom16_p_word_wssssssss
.L__wine_GDI_349:
	pushw %bp
	pushl $_SetMapperFlags16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_GDI_350:
	pushw %bp
	pushl $_GetCharWidth16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_351:
	pushw %bp
	pushl $_ExtTextOut16@32
	callw .L__wine_spec_callfrom16_p_word_wsswptwp
.L__wine_GDI_352:
	pushw %bp
	pushl $___wine_stub_GetPhysicalFontHandle
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_353:
	pushw %bp
	pushl $___wine_stub_GetAspectRatioFilter
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_354:
	pushw %bp
	pushl $___wine_stub_ShrinkGDIHeap
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_355:
	pushw %bp
	pushl $___wine_stub_FTrapping0
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_360:
	pushw %bp
	pushl $_CreatePalette16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_GDI_361:
	pushw %bp
	pushl $_GDISelectPalette16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_362:
	pushw %bp
	pushl $_GDIRealizePalette16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_363:
	pushw %bp
	pushl $_GetPaletteEntries16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_364:
	pushw %bp
	pushl $_SetPaletteEntries16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_365:
	pushw %bp
	pushl $_RealizeDefaultPalette16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_366:
	pushw %bp
	pushl $_UpdateColors16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_367:
	pushw %bp
	pushl $_AnimatePalette16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_368:
	pushw %bp
	pushl $_ResizePalette16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_370:
	pushw %bp
	pushl $_GetNearestPaletteIndex16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_GDI_372:
	pushw %bp
	pushl $_ExtFloodFill16@20
	callw .L__wine_spec_callfrom16_p_word_wsslw
.L__wine_GDI_373:
	pushw %bp
	pushl $_SetSystemPaletteUse16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_374:
	pushw %bp
	pushl $_GetSystemPaletteUse16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_375:
	pushw %bp
	pushl $_GetSystemPaletteEntries16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_376:
	pushw %bp
	pushl $_ResetDC16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_377:
	pushw %bp
	pushl $_StartDoc16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_378:
	pushw %bp
	pushl $_EndDoc16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_379:
	pushw %bp
	pushl $_StartPage16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_380:
	pushw %bp
	pushl $_EndPage16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_381:
	pushw %bp
	pushl $_SetAbortProc16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_GDI_382:
	pushw %bp
	pushl $_AbortDoc16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_400:
	pushw %bp
	pushl $_FastWindowFrame16@20
	callw .L__wine_spec_callfrom16_p_word_wpssl
.L__wine_GDI_401:
	pushw %bp
	pushl $___wine_stub_GDIMOVEBITMAP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_402:
	pushw %bp
	pushl $___wine_stub_GDIGETBITSGLOBAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_403:
	pushw %bp
	pushl $_GdiInit216@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_404:
	pushw %bp
	pushl $___wine_stub_GetTTGlyphIndexMap
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_405:
	pushw %bp
	pushl $_FinalGdiInit16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_406:
	pushw %bp
	pushl $___wine_stub_CREATEREALBITMAPINDIRECT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_407:
	pushw %bp
	pushl $_CreateUserBitmap16@20
	callw .L__wine_spec_callfrom16_p_word_wwwwp
.L__wine_GDI_408:
	pushw %bp
	pushl $___wine_stub_CREATEREALBITMAP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_409:
	pushw %bp
	pushl $_CreateUserDiscardableBitmap16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_410:
	pushw %bp
	pushl $_IsValidMetaFile16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_411:
	pushw %bp
	pushl $_GetCurLogFont16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_412:
	pushw %bp
	pushl $_IsDCCurrentPalette16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_439:
	pushw %bp
	pushl $_StretchDIBits16@52
	callw .L__wine_spec_callfrom16_p_word_wssssssssppwl
.L__wine_GDI_440:
	pushw %bp
	pushl $_SetDIBits16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwppw
.L__wine_GDI_441:
	pushw %bp
	pushl $_GetDIBits16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwppw
.L__wine_GDI_442:
	pushw %bp
	pushl $_CreateDIBitmap16@24
	callw .L__wine_spec_callfrom16_p_word_wplppw
.L__wine_GDI_443:
	pushw %bp
	pushl $_SetDIBitsToDevice16@48
	callw .L__wine_spec_callfrom16_p_word_wsssssswwppw
.L__wine_GDI_444:
	pushw %bp
	pushl $_CreateRoundRectRgn16@24
	callw .L__wine_spec_callfrom16_p_word_ssssss
.L__wine_GDI_445:
	pushw %bp
	pushl $_CreateDIBPatternBrush16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_449:
	pushw %bp
	pushl $___wine_stub_DEVICECOLORMATCH
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_450:
	pushw %bp
	pushl $_PolyPolygon16@16
	callw .L__wine_spec_callfrom16_p_word_wppw
.L__wine_GDI_451:
	pushw %bp
	pushl $_CreatePolyPolygonRgn16@16
	callw .L__wine_spec_callfrom16_p_word_ppww
.L__wine_GDI_452:
	pushw %bp
	pushl $_GdiSeeGdiDo16@16
	callw .L__wine_spec_callfrom16_p_long_wwww
.L__wine_GDI_460:
	pushw %bp
	pushl $___wine_stub_GDITASKTERMINATION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_461:
	pushw %bp
	pushl $_SetObjectOwner16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_462:
	pushw %bp
	pushl $_IsGDIObject16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_463:
	pushw %bp
	pushl $_MakeObjectPrivate16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_464:
	pushw %bp
	pushl $___wine_stub_FIXUPBOGUSPUBLISHERMETAFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_465:
	pushw %bp
	pushl $_RectVisible16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_466:
	pushw %bp
	pushl $_RectInRegion16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_467:
	pushw %bp
	pushl $___wine_stub_UNICODETOANSI
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_468:
	pushw %bp
	pushl $_GetBitmapDimensionEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_469:
	pushw %bp
	pushl $_GetBrushOrgEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_470:
	pushw %bp
	pushl $_GetCurrentPositionEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_471:
	pushw %bp
	pushl $_GetTextExtentPoint16@16
	callw .L__wine_spec_callfrom16_p_word_wpsp
.L__wine_GDI_472:
	pushw %bp
	pushl $_GetViewportExtEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_473:
	pushw %bp
	pushl $_GetViewportOrgEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_474:
	pushw %bp
	pushl $_GetWindowExtEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_475:
	pushw %bp
	pushl $_GetWindowOrgEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_476:
	pushw %bp
	pushl $_OffsetViewportOrgEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_477:
	pushw %bp
	pushl $_OffsetWindowOrgEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_478:
	pushw %bp
	pushl $_SetBitmapDimensionEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_479:
	pushw %bp
	pushl $_SetViewportExtEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_480:
	pushw %bp
	pushl $_SetViewportOrgEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_481:
	pushw %bp
	pushl $_SetWindowExtEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_482:
	pushw %bp
	pushl $_SetWindowOrgEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_483:
	pushw %bp
	pushl $_MoveToEx16@16
	callw .L__wine_spec_callfrom16_p_word_wssp
.L__wine_GDI_484:
	pushw %bp
	pushl $_ScaleViewportExtEx16@24
	callw .L__wine_spec_callfrom16_p_word_wssssp
.L__wine_GDI_485:
	pushw %bp
	pushl $_ScaleWindowExtEx16@24
	callw .L__wine_spec_callfrom16_p_word_wssssp
.L__wine_GDI_486:
	pushw %bp
	pushl $_GetAspectRatioFilterEx16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_GDI_489:
	pushw %bp
	pushl $_CreateDIBSection16@24
	callw .L__wine_spec_callfrom16_p_word_wpwpll
.L__wine_GDI_490:
	pushw %bp
	pushl $___wine_stub_CloseEnhMetafile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_491:
	pushw %bp
	pushl $___wine_stub_CopyEnhMetafile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_492:
	pushw %bp
	pushl $___wine_stub_CreateEnhMetafile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_493:
	pushw %bp
	pushl $___wine_stub_DeleteEnhMetafile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_495:
	pushw %bp
	pushl $___wine_stub_GDIComment
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_496:
	pushw %bp
	pushl $___wine_stub_GetEnhMetafile
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_497:
	pushw %bp
	pushl $___wine_stub_GetEnhMetafileBits
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_498:
	pushw %bp
	pushl $___wine_stub_GetEnhMetafileDescription
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_499:
	pushw %bp
	pushl $___wine_stub_GetEnhMetafileHeader
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_501:
	pushw %bp
	pushl $___wine_stub_GetEnhMetafilePaletteEntries
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_502:
	pushw %bp
	pushl $_PolyBezier16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_503:
	pushw %bp
	pushl $_PolyBezierTo16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_GDI_504:
	pushw %bp
	pushl $___wine_stub_PlayEnhMetafileRecord
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_505:
	pushw %bp
	pushl $___wine_stub_SetEnhMetafileBits
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_506:
	pushw %bp
	pushl $___wine_stub_SetMetaRgn
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_508:
	pushw %bp
	pushl $_ExtSelectClipRgn16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_GDI_511:
	pushw %bp
	pushl $_AbortPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_512:
	pushw %bp
	pushl $_BeginPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_513:
	pushw %bp
	pushl $_CloseFigure16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_514:
	pushw %bp
	pushl $_EndPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_515:
	pushw %bp
	pushl $_FillPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_516:
	pushw %bp
	pushl $_FlattenPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_517:
	pushw %bp
	pushl $_GetPath16@16
	callw .L__wine_spec_callfrom16_p_word_wppw
.L__wine_GDI_518:
	pushw %bp
	pushl $_PathToRegion16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_519:
	pushw %bp
	pushl $_SelectClipPath16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_520:
	pushw %bp
	pushl $_StrokeAndFillPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_521:
	pushw %bp
	pushl $_StrokePath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_522:
	pushw %bp
	pushl $_WidenPath16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_523:
	pushw %bp
	pushl $___wine_stub_ExtCreatePen
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_524:
	pushw %bp
	pushl $_GetArcDirection16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_525:
	pushw %bp
	pushl $_SetArcDirection16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_GDI_526:
	pushw %bp
	pushl $___wine_stub_GetMiterLimit
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_527:
	pushw %bp
	pushl $___wine_stub_SetMiterLimit
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_528:
	pushw %bp
	pushl $___wine_stub_GDIParametersInfo
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_529:
	pushw %bp
	pushl $_CreateHalftonePalette16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_530:
	pushw %bp
	pushl $_RawTextOut16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_531:
	pushw %bp
	pushl $_RawExtTextOut16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_532:
	pushw %bp
	pushl $_RawGetTextExtent16@12
	callw .L__wine_spec_callfrom16_p_word_wtw
.L__wine_GDI_536:
	pushw %bp
	pushl $_BiDiLayout16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_538:
	pushw %bp
	pushl $_BiDiCreateTabString16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_540:
	pushw %bp
	pushl $_BiDiGlyphOut16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_543:
	pushw %bp
	pushl $_BiDiGetStringExtent16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_555:
	pushw %bp
	pushl $_BiDiDeleteString16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_556:
	pushw %bp
	pushl $_BiDiSetDefaults16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_558:
	pushw %bp
	pushl $_BiDiGetDefaults16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_560:
	pushw %bp
	pushl $_BiDiShape16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_561:
	pushw %bp
	pushl $_BiDiFontComplement16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_564:
	pushw %bp
	pushl $_BiDiSetKashida16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_565:
	pushw %bp
	pushl $_BiDiKExtTextOut16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_566:
	pushw %bp
	pushl $_BiDiShapeEx16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_569:
	pushw %bp
	pushl $_BiDiCreateStringEx16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_571:
	pushw %bp
	pushl $_GetTextExtentRtoL16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_572:
	pushw %bp
	pushl $_GetHDCCharSet16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_573:
	pushw %bp
	pushl $_BiDiLayoutEx16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_GDI_602:
	pushw %bp
	pushl $_SetDIBColorTable16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_603:
	pushw %bp
	pushl $_GetDIBColorTable16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_GDI_604:
	pushw %bp
	pushl $_SetSolidBrush16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_GDI_605:
	pushw %bp
	pushl $_DeleteObject16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_606:
	pushw %bp
	pushl $_SetMagicColors16@12
	callw .L__wine_spec_callfrom16_p_word_wlw
.L__wine_GDI_607:
	pushw %bp
	pushl $_GetRegionData16@12
	callw .L__wine_spec_callfrom16_p_long_wlp
.L__wine_GDI_608:
	pushw %bp
	pushl $___wine_stub_ExtCreateRegion
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_609:
	pushw %bp
	pushl $_GdiFreeResources16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_GDI_610:
	pushw %bp
	pushl $_GdiSignalProc@16
	callw .L__wine_spec_callfrom16_p_word_lllw
.L__wine_GDI_611:
	pushw %bp
	pushl $___wine_stub_GetRandomRgn
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_612:
	pushw %bp
	pushl $_GetTextCharset16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_GDI_613:
	pushw %bp
	pushl $_EnumFontFamiliesEx16@20
	callw .L__wine_spec_callfrom16_p_word_wplll
.L__wine_GDI_614:
	pushw %bp
	pushl $___wine_stub_AddLpkToGDI
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_615:
	pushw %bp
	pushl $___wine_stub_GetCharacterPlacement
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_616:
	pushw %bp
	pushl $_GetFontLanguageInfo16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_GDI_650:
	pushw %bp
	pushl $___wine_stub_BuildInverseTableDIB
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_701:
	pushw %bp
	pushl $___wine_stub_GDITHKCONNECTIONDATALS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_702:
	pushw %bp
	pushl $___wine_stub_FT_GDIFTHKTHKCONNECTIONDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_703:
	pushw %bp
	pushl $___wine_stub_FDTHKCONNECTIONDATASL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_704:
	pushw %bp
	pushl $___wine_stub_ICMTHKCONNECTIONDATASL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_820:
	pushw %bp
	pushl $___wine_stub_ICMCreateTransform
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_821:
	pushw %bp
	pushl $___wine_stub_ICMDeleteTransform
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_822:
	pushw %bp
	pushl $___wine_stub_ICMTranslateRGB
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_823:
	pushw %bp
	pushl $___wine_stub_ICMTranslateRGBs
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_824:
	pushw %bp
	pushl $___wine_stub_ICMCheckColorsInGamut
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_GDI_1000:
	pushw %bp
	pushl $_SetLayout16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_GDI_1001:
	pushw %bp
	pushl $___wine_stub_GetLayout
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_spec_code_segment_end:

.L__wine_spec_data_segment:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.L__wine_spec_data_segment_end:
	.byte 0

/* relay functions */

	.text
	.align 4
	.def .L__wine_spec_call16_c_; .scl 2; .type 32; .endef
.L__wine_spec_call16_c_:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_l; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_l:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_llll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llll:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	pushl 12(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lllw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lllw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	pushl 6(%ecx)
	pushl 10(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_p; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_p:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pplpl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pplpl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%edx
	shr $3,%edx
	movzwl 16(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pppp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pppplp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pppplp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%edx
	shr $3,%edx
	movzwl 16(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 22(%ecx),%edx
	shr $3,%edx
	movzwl 20(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ppww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ssl:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_sss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_sss:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ssss:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ssssll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ssssll:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movswl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ssssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ssssss:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ssssswwwwwwwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ssssswwwwwwwwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	movswl 20(%ecx),%eax
	pushl %eax
	movswl 22(%ecx),%eax
	pushl %eax
	movswl 24(%ecx),%eax
	pushl %eax
	movswl 26(%ecx),%eax
	pushl %eax
	movswl 28(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_w; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_w:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wl:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wll:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wllpl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wllpl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 8(%ecx)
	pushl 12(%ecx)
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wlp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wlp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wlw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wlw:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wplll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wplll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wplppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wplppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 10(%ecx)
	movzwl 16(%ecx),%edx
	shr $3,%edx
	movzwl 14(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wps:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpsp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpsp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpssl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwpll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwpll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%edx
	shr $3,%edx
	movzwl 14(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwwplp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwwplp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%edx
	shr $3,%edx
	movzwl 16(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ws; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ws:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wss:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssl:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsslw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsslw:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsspw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsspw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssss:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssssl:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssssp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssssp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssssss:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssssssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssssssss:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movswl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssssssssppwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssssssssppwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movswl 18(%ecx),%eax
	pushl %eax
	movswl 20(%ecx),%eax
	pushl %eax
	movswl 22(%ecx),%eax
	pushl %eax
	movswl 24(%ecx),%eax
	pushl %eax
	movswl 26(%ecx),%eax
	pushl %eax
	movswl 28(%ecx),%eax
	pushl %eax
	movzwl 30(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsssssswwppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsssssswwppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movswl 18(%ecx),%eax
	pushl %eax
	movswl 20(%ecx),%eax
	pushl %eax
	movswl 22(%ecx),%eax
	pushl %eax
	movswl 24(%ecx),%eax
	pushl %eax
	movzwl 26(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsssswssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsssswssl:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movswl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsssswssssl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsssswssssl:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movswl 18(%ecx),%eax
	pushl %eax
	movswl 20(%ecx),%eax
	pushl %eax
	movzwl 22(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsswppwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsswppwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movswl 18(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ww:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwll:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_www; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_www:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwlp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwlp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwplpp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwplpp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwws; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwws:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwww:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwww:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwww:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_BITMAPBITS; .scl 2; .type 32; .endef
___wine_stub_BITMAPBITS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_BITMAPBITS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_InternalCreateDC; .scl 2; .type 32; .endef
___wine_stub_InternalCreateDC:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_InternalCreateDC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetContinuingTextExtent; .scl 2; .type 32; .endef
___wine_stub_GetContinuingTextExtent:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetContinuingTextExtent_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GSV; .scl 2; .type 32; .endef
___wine_stub_GSV:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GSV_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DPXlate; .scl 2; .type 32; .endef
___wine_stub_DPXlate:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DPXlate_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetWinViewExt; .scl 2; .type 32; .endef
___wine_stub_SetWinViewExt:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SetWinViewExt_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ScaleExt; .scl 2; .type 32; .endef
___wine_stub_ScaleExt:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ScaleExt_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WordSet; .scl 2; .type 32; .endef
___wine_stub_WordSet:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_WordSet_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_RectStuff; .scl 2; .type 32; .endef
___wine_stub_RectStuff:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_RectStuff_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OffsetOrg; .scl 2; .type 32; .endef
___wine_stub_OffsetOrg:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_OffsetOrg_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LockDC; .scl 2; .type 32; .endef
___wine_stub_LockDC:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_LockDC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UnlockDC; .scl 2; .type 32; .endef
___wine_stub_UnlockDC:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_UnlockDC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LockUnlock; .scl 2; .type 32; .endef
___wine_stub_LockUnlock:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_LockUnlock_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDI_FarFrame; .scl 2; .type 32; .endef
___wine_stub_GDI_FarFrame:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDI_FarFrame_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDIInitApp; .scl 2; .type 32; .endef
___wine_stub_GDIInitApp:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDIInitApp_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CompatibleBitmap; .scl 2; .type 32; .endef
___wine_stub_CompatibleBitmap:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CompatibleBitmap_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EnumCallback; .scl 2; .type 32; .endef
___wine_stub_EnumCallback:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EnumCallback_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PixToLine; .scl 2; .type 32; .endef
___wine_stub_PixToLine:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_PixToLine_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_IsDCDirty; .scl 2; .type 32; .endef
___wine_stub_IsDCDirty:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_IsDCDirty_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetDCStatus; .scl 2; .type 32; .endef
___wine_stub_SetDCStatus:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SetDCStatus_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LVBUNION; .scl 2; .type 32; .endef
___wine_stub_LVBUNION:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_LVBUNION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BLOAT; .scl 2; .type 32; .endef
___wine_stub_BLOAT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_BLOAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_RCOS; .scl 2; .type 32; .endef
___wine_stub_RCOS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_RCOS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_RSIN; .scl 2; .type 32; .endef
___wine_stub_RSIN:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_RSIN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_REQUESTSEM; .scl 2; .type 32; .endef
___wine_stub_REQUESTSEM:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_REQUESTSEM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CLEARSEM; .scl 2; .type 32; .endef
___wine_stub_CLEARSEM:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CLEARSEM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_STUFFVISIBLE; .scl 2; .type 32; .endef
___wine_stub_STUFFVISIBLE:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_STUFFVISIBLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_STUFFINREGION; .scl 2; .type 32; .endef
___wine_stub_STUFFINREGION:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_STUFFINREGION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DELETEABOVELINEFONTS; .scl 2; .type 32; .endef
___wine_stub_DELETEABOVELINEFONTS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DELETEABOVELINEFONTS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetTextExtentEx; .scl 2; .type 32; .endef
___wine_stub_GetTextExtentEx:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetTextExtentEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SelectBitmap; .scl 2; .type 32; .endef
___wine_stub_SelectBitmap:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SelectBitmap_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMBITBLT; .scl 2; .type 32; .endef
___wine_stub_DMBITBLT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMBITBLT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMCOLORINFO; .scl 2; .type 32; .endef
___wine_stub_DMCOLORINFO:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMCOLORINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_dmEnumDFonts; .scl 2; .type 32; .endef
___wine_stub_dmEnumDFonts:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_dmEnumDFonts_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMENUMOBJ; .scl 2; .type 32; .endef
___wine_stub_DMENUMOBJ:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMENUMOBJ_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMOUTPUT; .scl 2; .type 32; .endef
___wine_stub_DMOUTPUT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMOUTPUT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMPIXEL; .scl 2; .type 32; .endef
___wine_stub_DMPIXEL:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMPIXEL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_dmRealizeObject; .scl 2; .type 32; .endef
___wine_stub_dmRealizeObject:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_dmRealizeObject_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMSTRBLT; .scl 2; .type 32; .endef
___wine_stub_DMSTRBLT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMSTRBLT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMSCANLR; .scl 2; .type 32; .endef
___wine_stub_DMSCANLR:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMSCANLR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BRUTE; .scl 2; .type 32; .endef
___wine_stub_BRUTE:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_BRUTE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMEXTTEXTOUT; .scl 2; .type 32; .endef
___wine_stub_DMEXTTEXTOUT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMEXTTEXTOUT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMGETCHARWIDTH; .scl 2; .type 32; .endef
___wine_stub_DMGETCHARWIDTH:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMGETCHARWIDTH_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMSTRETCHBLT; .scl 2; .type 32; .endef
___wine_stub_DMSTRETCHBLT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMSTRETCHBLT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMDIBBITS; .scl 2; .type 32; .endef
___wine_stub_DMDIBBITS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMDIBBITS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMSTRETCHDIBITS; .scl 2; .type 32; .endef
___wine_stub_DMSTRETCHDIBITS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMSTRETCHDIBITS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMSETDIBTODEV; .scl 2; .type 32; .endef
___wine_stub_DMSETDIBTODEV:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMSETDIBTODEV_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DMTRANSPOSE; .scl 2; .type 32; .endef
___wine_stub_DMTRANSPOSE:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DMTRANSPOSE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_QueryJob; .scl 2; .type 32; .endef
___wine_stub_QueryJob:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_QueryJob_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DeleteSpoolPage; .scl 2; .type 32; .endef
___wine_stub_DeleteSpoolPage:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DeleteSpoolPage_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SpoolFile; .scl 2; .type 32; .endef
___wine_stub_SpoolFile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SpoolFile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_StartDocPrintEra; .scl 2; .type 32; .endef
___wine_stub_StartDocPrintEra:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_StartDocPrintEra_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_StartPagePrinter; .scl 2; .type 32; .endef
___wine_stub_StartPagePrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_StartPagePrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WritePrinter; .scl 2; .type 32; .endef
___wine_stub_WritePrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_WritePrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EndPagePrinter; .scl 2; .type 32; .endef
___wine_stub_EndPagePrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EndPagePrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_AbortPrinter; .scl 2; .type 32; .endef
___wine_stub_AbortPrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_AbortPrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EndDocPrinter; .scl 2; .type 32; .endef
___wine_stub_EndDocPrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EndDocPrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ClosePrinter; .scl 2; .type 32; .endef
___wine_stub_ClosePrinter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ClosePrinter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetRealDriverInfo; .scl 2; .type 32; .endef
___wine_stub_GetRealDriverInfo:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetRealDriverInfo_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ENGINEGETCHARWIDTHEX; .scl 2; .type 32; .endef
___wine_stub_ENGINEGETCHARWIDTHEX:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ENGINEGETCHARWIDTHEX_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ConvertOutLineFontFile; .scl 2; .type 32; .endef
___wine_stub_ConvertOutLineFontFile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ConvertOutLineFontFile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EngineExtTextOut; .scl 2; .type 32; .endef
___wine_stub_EngineExtTextOut:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EngineExtTextOut_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EngineGetCharWidthStr; .scl 2; .type 32; .endef
___wine_stub_EngineGetCharWidthStr:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EngineGetCharWidthStr_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EngineGetGlyphBmpExt; .scl 2; .type 32; .endef
___wine_stub_EngineGetGlyphBmpExt:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_EngineGetGlyphBmpExt_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFDRAWTEXT; .scl 2; .type 32; .endef
___wine_stub_MFDRAWTEXT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_MFDRAWTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetPhysicalFontHandle; .scl 2; .type 32; .endef
___wine_stub_GetPhysicalFontHandle:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetPhysicalFontHandle_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetAspectRatioFilter; .scl 2; .type 32; .endef
___wine_stub_GetAspectRatioFilter:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetAspectRatioFilter_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ShrinkGDIHeap; .scl 2; .type 32; .endef
___wine_stub_ShrinkGDIHeap:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ShrinkGDIHeap_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FTrapping0; .scl 2; .type 32; .endef
___wine_stub_FTrapping0:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_FTrapping0_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDIMOVEBITMAP; .scl 2; .type 32; .endef
___wine_stub_GDIMOVEBITMAP:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDIMOVEBITMAP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDIGETBITSGLOBAL; .scl 2; .type 32; .endef
___wine_stub_GDIGETBITSGLOBAL:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDIGETBITSGLOBAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetTTGlyphIndexMap; .scl 2; .type 32; .endef
___wine_stub_GetTTGlyphIndexMap:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetTTGlyphIndexMap_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CREATEREALBITMAPINDIRECT; .scl 2; .type 32; .endef
___wine_stub_CREATEREALBITMAPINDIRECT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CREATEREALBITMAPINDIRECT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CREATEREALBITMAP; .scl 2; .type 32; .endef
___wine_stub_CREATEREALBITMAP:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CREATEREALBITMAP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEVICECOLORMATCH; .scl 2; .type 32; .endef
___wine_stub_DEVICECOLORMATCH:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DEVICECOLORMATCH_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDITASKTERMINATION; .scl 2; .type 32; .endef
___wine_stub_GDITASKTERMINATION:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDITASKTERMINATION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FIXUPBOGUSPUBLISHERMETAFILE; .scl 2; .type 32; .endef
___wine_stub_FIXUPBOGUSPUBLISHERMETAFILE:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_FIXUPBOGUSPUBLISHERMETAFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UNICODETOANSI; .scl 2; .type 32; .endef
___wine_stub_UNICODETOANSI:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_UNICODETOANSI_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CloseEnhMetafile; .scl 2; .type 32; .endef
___wine_stub_CloseEnhMetafile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CloseEnhMetafile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CopyEnhMetafile; .scl 2; .type 32; .endef
___wine_stub_CopyEnhMetafile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CopyEnhMetafile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CreateEnhMetafile; .scl 2; .type 32; .endef
___wine_stub_CreateEnhMetafile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_CreateEnhMetafile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DeleteEnhMetafile; .scl 2; .type 32; .endef
___wine_stub_DeleteEnhMetafile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DeleteEnhMetafile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDIComment; .scl 2; .type 32; .endef
___wine_stub_GDIComment:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDIComment_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetEnhMetafile; .scl 2; .type 32; .endef
___wine_stub_GetEnhMetafile:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetEnhMetafile_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetEnhMetafileBits; .scl 2; .type 32; .endef
___wine_stub_GetEnhMetafileBits:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetEnhMetafileBits_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetEnhMetafileDescription; .scl 2; .type 32; .endef
___wine_stub_GetEnhMetafileDescription:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetEnhMetafileDescription_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetEnhMetafileHeader; .scl 2; .type 32; .endef
___wine_stub_GetEnhMetafileHeader:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetEnhMetafileHeader_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetEnhMetafilePaletteEntries; .scl 2; .type 32; .endef
___wine_stub_GetEnhMetafilePaletteEntries:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetEnhMetafilePaletteEntries_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PlayEnhMetafileRecord; .scl 2; .type 32; .endef
___wine_stub_PlayEnhMetafileRecord:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_PlayEnhMetafileRecord_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetEnhMetafileBits; .scl 2; .type 32; .endef
___wine_stub_SetEnhMetafileBits:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SetEnhMetafileBits_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetMetaRgn; .scl 2; .type 32; .endef
___wine_stub_SetMetaRgn:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SetMetaRgn_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ExtCreatePen; .scl 2; .type 32; .endef
___wine_stub_ExtCreatePen:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ExtCreatePen_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetMiterLimit; .scl 2; .type 32; .endef
___wine_stub_GetMiterLimit:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetMiterLimit_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetMiterLimit; .scl 2; .type 32; .endef
___wine_stub_SetMiterLimit:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SetMiterLimit_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDIParametersInfo; .scl 2; .type 32; .endef
___wine_stub_GDIParametersInfo:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDIParametersInfo_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ExtCreateRegion; .scl 2; .type 32; .endef
___wine_stub_ExtCreateRegion:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ExtCreateRegion_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetRandomRgn; .scl 2; .type 32; .endef
___wine_stub_GetRandomRgn:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetRandomRgn_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_AddLpkToGDI; .scl 2; .type 32; .endef
___wine_stub_AddLpkToGDI:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_AddLpkToGDI_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetCharacterPlacement; .scl 2; .type 32; .endef
___wine_stub_GetCharacterPlacement:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetCharacterPlacement_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BuildInverseTableDIB; .scl 2; .type 32; .endef
___wine_stub_BuildInverseTableDIB:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_BuildInverseTableDIB_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GDITHKCONNECTIONDATALS; .scl 2; .type 32; .endef
___wine_stub_GDITHKCONNECTIONDATALS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GDITHKCONNECTIONDATALS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FT_GDIFTHKTHKCONNECTIONDATA; .scl 2; .type 32; .endef
___wine_stub_FT_GDIFTHKTHKCONNECTIONDATA:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_FT_GDIFTHKTHKCONNECTIONDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDTHKCONNECTIONDATASL; .scl 2; .type 32; .endef
___wine_stub_FDTHKCONNECTIONDATASL:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_FDTHKCONNECTIONDATASL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMTHKCONNECTIONDATASL; .scl 2; .type 32; .endef
___wine_stub_ICMTHKCONNECTIONDATASL:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMTHKCONNECTIONDATASL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMCreateTransform; .scl 2; .type 32; .endef
___wine_stub_ICMCreateTransform:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMCreateTransform_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMDeleteTransform; .scl 2; .type 32; .endef
___wine_stub_ICMDeleteTransform:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMDeleteTransform_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMTranslateRGB; .scl 2; .type 32; .endef
___wine_stub_ICMTranslateRGB:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMTranslateRGB_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMTranslateRGBs; .scl 2; .type 32; .endef
___wine_stub_ICMTranslateRGBs:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMTranslateRGBs_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ICMCheckColorsInGamut; .scl 2; .type 32; .endef
___wine_stub_ICMCheckColorsInGamut:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ICMCheckColorsInGamut_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetLayout; .scl 2; .type 32; .endef
___wine_stub_GetLayout:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GetLayout_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_BITMAPBITS_string:
	.string "BITMAPBITS"
.L__wine_stub_InternalCreateDC_string:
	.string "InternalCreateDC"
.L__wine_stub_GetContinuingTextExtent_string:
	.string "GetContinuingTextExtent"
.L__wine_stub_GSV_string:
	.string "GSV"
.L__wine_stub_DPXlate_string:
	.string "DPXlate"
.L__wine_stub_SetWinViewExt_string:
	.string "SetWinViewExt"
.L__wine_stub_ScaleExt_string:
	.string "ScaleExt"
.L__wine_stub_WordSet_string:
	.string "WordSet"
.L__wine_stub_RectStuff_string:
	.string "RectStuff"
.L__wine_stub_OffsetOrg_string:
	.string "OffsetOrg"
.L__wine_stub_LockDC_string:
	.string "LockDC"
.L__wine_stub_UnlockDC_string:
	.string "UnlockDC"
.L__wine_stub_LockUnlock_string:
	.string "LockUnlock"
.L__wine_stub_GDI_FarFrame_string:
	.string "GDI_FarFrame"
.L__wine_stub_GDIInitApp_string:
	.string "GDIInitApp"
.L__wine_stub_CompatibleBitmap_string:
	.string "CompatibleBitmap"
.L__wine_stub_EnumCallback_string:
	.string "EnumCallback"
.L__wine_stub_PixToLine_string:
	.string "PixToLine"
.L__wine_stub_IsDCDirty_string:
	.string "IsDCDirty"
.L__wine_stub_SetDCStatus_string:
	.string "SetDCStatus"
.L__wine_stub_LVBUNION_string:
	.string "LVBUNION"
.L__wine_stub_BLOAT_string:
	.string "BLOAT"
.L__wine_stub_RCOS_string:
	.string "RCOS"
.L__wine_stub_RSIN_string:
	.string "RSIN"
.L__wine_stub_REQUESTSEM_string:
	.string "REQUESTSEM"
.L__wine_stub_CLEARSEM_string:
	.string "CLEARSEM"
.L__wine_stub_STUFFVISIBLE_string:
	.string "STUFFVISIBLE"
.L__wine_stub_STUFFINREGION_string:
	.string "STUFFINREGION"
.L__wine_stub_DELETEABOVELINEFONTS_string:
	.string "DELETEABOVELINEFONTS"
.L__wine_stub_GetTextExtentEx_string:
	.string "GetTextExtentEx"
.L__wine_stub_SelectBitmap_string:
	.string "SelectBitmap"
.L__wine_stub_DMBITBLT_string:
	.string "DMBITBLT"
.L__wine_stub_DMCOLORINFO_string:
	.string "DMCOLORINFO"
.L__wine_stub_dmEnumDFonts_string:
	.string "dmEnumDFonts"
.L__wine_stub_DMENUMOBJ_string:
	.string "DMENUMOBJ"
.L__wine_stub_DMOUTPUT_string:
	.string "DMOUTPUT"
.L__wine_stub_DMPIXEL_string:
	.string "DMPIXEL"
.L__wine_stub_dmRealizeObject_string:
	.string "dmRealizeObject"
.L__wine_stub_DMSTRBLT_string:
	.string "DMSTRBLT"
.L__wine_stub_DMSCANLR_string:
	.string "DMSCANLR"
.L__wine_stub_BRUTE_string:
	.string "BRUTE"
.L__wine_stub_DMEXTTEXTOUT_string:
	.string "DMEXTTEXTOUT"
.L__wine_stub_DMGETCHARWIDTH_string:
	.string "DMGETCHARWIDTH"
.L__wine_stub_DMSTRETCHBLT_string:
	.string "DMSTRETCHBLT"
.L__wine_stub_DMDIBBITS_string:
	.string "DMDIBBITS"
.L__wine_stub_DMSTRETCHDIBITS_string:
	.string "DMSTRETCHDIBITS"
.L__wine_stub_DMSETDIBTODEV_string:
	.string "DMSETDIBTODEV"
.L__wine_stub_DMTRANSPOSE_string:
	.string "DMTRANSPOSE"
.L__wine_stub_QueryJob_string:
	.string "QueryJob"
.L__wine_stub_DeleteSpoolPage_string:
	.string "DeleteSpoolPage"
.L__wine_stub_SpoolFile_string:
	.string "SpoolFile"
.L__wine_stub_StartDocPrintEra_string:
	.string "StartDocPrintEra"
.L__wine_stub_StartPagePrinter_string:
	.string "StartPagePrinter"
.L__wine_stub_WritePrinter_string:
	.string "WritePrinter"
.L__wine_stub_EndPagePrinter_string:
	.string "EndPagePrinter"
.L__wine_stub_AbortPrinter_string:
	.string "AbortPrinter"
.L__wine_stub_EndDocPrinter_string:
	.string "EndDocPrinter"
.L__wine_stub_ClosePrinter_string:
	.string "ClosePrinter"
.L__wine_stub_GetRealDriverInfo_string:
	.string "GetRealDriverInfo"
.L__wine_stub_ENGINEGETCHARWIDTHEX_string:
	.string "ENGINEGETCHARWIDTHEX"
.L__wine_stub_ConvertOutLineFontFile_string:
	.string "ConvertOutLineFontFile"
.L__wine_stub_EngineExtTextOut_string:
	.string "EngineExtTextOut"
.L__wine_stub_EngineGetCharWidthStr_string:
	.string "EngineGetCharWidthStr"
.L__wine_stub_EngineGetGlyphBmpExt_string:
	.string "EngineGetGlyphBmpExt"
.L__wine_stub_MFDRAWTEXT_string:
	.string "MFDRAWTEXT"
.L__wine_stub_GetPhysicalFontHandle_string:
	.string "GetPhysicalFontHandle"
.L__wine_stub_GetAspectRatioFilter_string:
	.string "GetAspectRatioFilter"
.L__wine_stub_ShrinkGDIHeap_string:
	.string "ShrinkGDIHeap"
.L__wine_stub_FTrapping0_string:
	.string "FTrapping0"
.L__wine_stub_GDIMOVEBITMAP_string:
	.string "GDIMOVEBITMAP"
.L__wine_stub_GDIGETBITSGLOBAL_string:
	.string "GDIGETBITSGLOBAL"
.L__wine_stub_GetTTGlyphIndexMap_string:
	.string "GetTTGlyphIndexMap"
.L__wine_stub_CREATEREALBITMAPINDIRECT_string:
	.string "CREATEREALBITMAPINDIRECT"
.L__wine_stub_CREATEREALBITMAP_string:
	.string "CREATEREALBITMAP"
.L__wine_stub_DEVICECOLORMATCH_string:
	.string "DEVICECOLORMATCH"
.L__wine_stub_GDITASKTERMINATION_string:
	.string "GDITASKTERMINATION"
.L__wine_stub_FIXUPBOGUSPUBLISHERMETAFILE_string:
	.string "FIXUPBOGUSPUBLISHERMETAFILE"
.L__wine_stub_UNICODETOANSI_string:
	.string "UNICODETOANSI"
.L__wine_stub_CloseEnhMetafile_string:
	.string "CloseEnhMetafile"
.L__wine_stub_CopyEnhMetafile_string:
	.string "CopyEnhMetafile"
.L__wine_stub_CreateEnhMetafile_string:
	.string "CreateEnhMetafile"
.L__wine_stub_DeleteEnhMetafile_string:
	.string "DeleteEnhMetafile"
.L__wine_stub_GDIComment_string:
	.string "GDIComment"
.L__wine_stub_GetEnhMetafile_string:
	.string "GetEnhMetafile"
.L__wine_stub_GetEnhMetafileBits_string:
	.string "GetEnhMetafileBits"
.L__wine_stub_GetEnhMetafileDescription_string:
	.string "GetEnhMetafileDescription"
.L__wine_stub_GetEnhMetafileHeader_string:
	.string "GetEnhMetafileHeader"
.L__wine_stub_GetEnhMetafilePaletteEntries_string:
	.string "GetEnhMetafilePaletteEntries"
.L__wine_stub_PlayEnhMetafileRecord_string:
	.string "PlayEnhMetafileRecord"
.L__wine_stub_SetEnhMetafileBits_string:
	.string "SetEnhMetafileBits"
.L__wine_stub_SetMetaRgn_string:
	.string "SetMetaRgn"
.L__wine_stub_ExtCreatePen_string:
	.string "ExtCreatePen"
.L__wine_stub_GetMiterLimit_string:
	.string "GetMiterLimit"
.L__wine_stub_SetMiterLimit_string:
	.string "SetMiterLimit"
.L__wine_stub_GDIParametersInfo_string:
	.string "GDIParametersInfo"
.L__wine_stub_ExtCreateRegion_string:
	.string "ExtCreateRegion"
.L__wine_stub_GetRandomRgn_string:
	.string "GetRandomRgn"
.L__wine_stub_AddLpkToGDI_string:
	.string "AddLpkToGDI"
.L__wine_stub_GetCharacterPlacement_string:
	.string "GetCharacterPlacement"
.L__wine_stub_BuildInverseTableDIB_string:
	.string "BuildInverseTableDIB"
.L__wine_stub_GDITHKCONNECTIONDATALS_string:
	.string "GDITHKCONNECTIONDATALS"
.L__wine_stub_FT_GDIFTHKTHKCONNECTIONDATA_string:
	.string "FT_GDIFTHKTHKCONNECTIONDATA"
.L__wine_stub_FDTHKCONNECTIONDATASL_string:
	.string "FDTHKCONNECTIONDATASL"
.L__wine_stub_ICMTHKCONNECTIONDATASL_string:
	.string "ICMTHKCONNECTIONDATASL"
.L__wine_stub_ICMCreateTransform_string:
	.string "ICMCreateTransform"
.L__wine_stub_ICMDeleteTransform_string:
	.string "ICMDeleteTransform"
.L__wine_stub_ICMTranslateRGB_string:
	.string "ICMTranslateRGB"
.L__wine_stub_ICMTranslateRGBs_string:
	.string "ICMTranslateRGBs"
.L__wine_stub_ICMCheckColorsInGamut_string:
	.string "ICMCheckColorsInGamut"
.L__wine_stub_GetLayout_string:
	.string "GetLayout"

/* export table */

	.data
	.align 4
.L__wine_spec_exports:
	.long 0
	.long 0
	.long 0
	.long .L__wine_spec_exp_names-.L__wine_spec_rva_base
	.long 1
	.long 2
	.long 2
	.long .L__wine_spec_exports_funcs-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_name_ptrs-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_ordinals-.L__wine_spec_rva_base

.L__wine_spec_exports_funcs:
	.long .L__wine_spec_dos_header
	.long .L__wine_spec_main_module

.L__wine_spec_exp_name_ptrs:
	.long .L__wine_spec_exp_names+15-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+38-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "gdi.exe16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "GDI"
__end:/*?*/
