/* File generated automatically from ..\wifeman\wifeman.dll16.spec; do not edit! */
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
	.string "..\wifeman\wifeman.dll16.spec"

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
	.byte 0x07,0x57,0x49,0x46,0x45,0x4d,0x41,0x4e /* WIFEMAN */
	.short 0
	.byte 0x0f,0x46,0x44,0x49,0x4c,0x4f,0x41,0x44,0x46,0x4f,0x4e,0x54,0x46,0x49,0x4c,0x45 /* FDILOADFONTFILE */
	.short 1
	.byte 0x11,0x46,0x44,0x49,0x55,0x4e,0x4c,0x4f,0x41,0x44,0x46,0x4f,0x4e,0x54,0x46,0x49,0x4c,0x45 /* FDIUNLOADFONTFILE */
	.short 2
	.byte 0x0d,0x46,0x44,0x49,0x51,0x55,0x45,0x52,0x59,0x46,0x41,0x43,0x45,0x53 /* FDIQUERYFACES */
	.short 3
	.byte 0x12,0x46,0x44,0x49,0x4f,0x50,0x45,0x4e,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* FDIOPENFONTCONTEXT */
	.short 4
	.byte 0x11,0x46,0x44,0x49,0x53,0x45,0x54,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* FDISETFONTCONTEXT */
	.short 5
	.byte 0x13,0x46,0x44,0x49,0x43,0x4c,0x4f,0x53,0x45,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* FDICLOSEFONTCONTEXT */
	.short 6
	.byte 0x10,0x46,0x44,0x49,0x51,0x55,0x45,0x52,0x59,0x46,0x41,0x43,0x45,0x41,0x54,0x54,0x52 /* FDIQUERYFACEATTR */
	.short 7
	.byte 0x10,0x46,0x44,0x49,0x51,0x55,0x45,0x52,0x59,0x43,0x48,0x41,0x52,0x41,0x54,0x54,0x52 /* FDIQUERYCHARATTR */
	.short 8
	.byte 0x0a,0x46,0x44,0x49,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c /* FDICONTROL */
	.short 9
	.byte 0x10,0x46,0x44,0x49,0x43,0x4c,0x41,0x49,0x4d,0x46,0x4f,0x4e,0x54,0x46,0x49,0x4c,0x45 /* FDICLAIMFONTFILE */
	.short 10
	.byte 0x12,0x46,0x44,0x49,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54,0x46,0x4f,0x4e,0x54,0x46,0x49,0x4c,0x45 /* FDICONVERTFONTFILE */
	.short 11
	.byte 0x10,0x4d,0x49,0x4d,0x41,0x44,0x44,0x46,0x4f,0x4e,0x54,0x44,0x52,0x49,0x56,0x45,0x52 /* MIMADDFONTDRIVER */
	.short 14
	.byte 0x0e,0x4d,0x49,0x4d,0x41,0x44,0x44,0x46,0x4f,0x4e,0x54,0x50,0x41,0x43,0x4b /* MIMADDFONTPACK */
	.short 15
	.byte 0x13,0x4d,0x49,0x4d,0x52,0x45,0x4d,0x4f,0x56,0x45,0x46,0x4f,0x4e,0x54,0x44,0x52,0x49,0x56,0x45,0x52 /* MIMREMOVEFONTDRIVER */
	.short 16
	.byte 0x11,0x4d,0x49,0x4d,0x52,0x45,0x4d,0x4f,0x56,0x45,0x46,0x4f,0x4e,0x54,0x50,0x41,0x43,0x4b /* MIMREMOVEFONTPACK */
	.short 17
	.byte 0x12,0x4d,0x49,0x4d,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x44,0x52,0x49,0x56,0x45,0x52,0x53 /* MIMENUMFONTDRIVERS */
	.short 18
	.byte 0x10,0x4d,0x49,0x4d,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x50,0x41,0x43,0x4b,0x53 /* MIMENUMFONTPACKS */
	.short 19
	.byte 0x12,0x4d,0x49,0x4d,0x4f,0x50,0x45,0x4e,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* MIMOPENFONTCONTEXT */
	.short 20
	.byte 0x13,0x4d,0x49,0x4d,0x43,0x4c,0x4f,0x53,0x45,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* MIMCLOSEFONTCONTEXT */
	.short 21
	.byte 0x12,0x4d,0x49,0x4d,0x47,0x45,0x54,0x49,0x4e,0x46,0x4f,0x52,0x4d,0x41,0x54,0x49,0x4f,0x4e,0x53 /* MIMGETINFORMATIONS */
	.short 22
	.byte 0x12,0x4d,0x49,0x53,0x43,0x49,0x53,0x44,0x42,0x43,0x53,0x4c,0x45,0x41,0x44,0x42,0x59,0x54,0x45 /* MISCISDBCSLEADBYTE */
	.short 23
	.byte 0x0f,0x4d,0x49,0x53,0x43,0x53,0x45,0x54,0x45,0x52,0x52,0x4f,0x49,0x4e,0x46,0x4f /* MISCSETERROINFO */
	.short 24
	.byte 0x0f,0x4d,0x49,0x53,0x43,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x49,0x4e,0x46,0x4f /* MISCGETERROINFO */
	.short 25
	.byte 0x08,0x55,0x42,0x53,0x54,0x52,0x4c,0x45,0x4e /* UBSTRLEN */
	.short 26
	.byte 0x08,0x55,0x42,0x53,0x54,0x52,0x43,0x50,0x59 /* UBSTRCPY */
	.short 27
	.byte 0x09,0x55,0x42,0x53,0x54,0x52,0x4e,0x43,0x50,0x59 /* UBSTRNCPY */
	.short 28
	.byte 0x08,0x55,0x42,0x53,0x54,0x52,0x43,0x41,0x54 /* UBSTRCAT */
	.short 29
	.byte 0x08,0x55,0x42,0x53,0x54,0x52,0x43,0x4d,0x50 /* UBSTRCMP */
	.short 30
	.byte 0x09,0x41,0x53,0x43,0x49,0x5a,0x54,0x4f,0x55,0x5a /* ASCIZTOUZ */
	.short 31
	.byte 0x09,0x55,0x5a,0x54,0x4f,0x41,0x53,0x43,0x49,0x5a /* UZTOASCIZ */
	.short 32
	.byte 0x12,0x4d,0x49,0x53,0x43,0x57,0x41,0x52,0x4e,0x49,0x4e,0x47,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* MISCWARNINGMESSAGE */
	.short 33
	.byte 0x1e,0x4d,0x49,0x53,0x43,0x57,0x41,0x52,0x4e,0x49,0x4e,0x47,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x57,0x49,0x54,0x48,0x41,0x52,0x47,0x55,0x4d,0x45,0x4e,0x54 /* MISCWARNINGMESSAGEWITHARGUMENT */
	.short 34
	.byte 0x14,0x46,0x43,0x4d,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x41,0x43,0x48,0x45,0x42,0x55,0x46,0x46,0x45,0x52 /* FCMCREATECACHEBUFFER */
	.short 35
	.byte 0x14,0x46,0x43,0x4d,0x44,0x45,0x4c,0x45,0x54,0x45,0x43,0x41,0x43,0x48,0x45,0x42,0x55,0x46,0x46,0x45,0x52 /* FCMDELETECACHEBUFFER */
	.short 36
	.byte 0x10,0x46,0x43,0x4d,0x52,0x45,0x51,0x55,0x45,0x53,0x54,0x49,0x4d,0x41,0x47,0x45,0x53 /* FCMREQUESTIMAGES */
	.short 37
	.byte 0x10,0x46,0x43,0x4d,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x49,0x4d,0x41,0x47,0x45,0x53 /* FCMRELEASEIMAGES */
	.short 38
	.byte 0x13,0x46,0x43,0x4d,0x52,0x45,0x51,0x55,0x45,0x53,0x54,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x46,0x42 /* FCMREQUESTDEFAULTFB */
	.short 39
	.byte 0x13,0x46,0x43,0x4d,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x46,0x42 /* FCMRELEASEDEFAULTFB */
	.short 40
	.byte 0x16,0x46,0x43,0x4d,0x43,0x41,0x4c,0x43,0x55,0x4c,0x41,0x54,0x45,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54 /* FCMCALCULATETEXTEXTENT */
	.short 41
	.byte 0x1b,0x46,0x43,0x4d,0x43,0x41,0x4c,0x43,0x55,0x4c,0x41,0x54,0x45,0x4f,0x55,0x54,0x50,0x55,0x54,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e,0x53 /* FCMCALCULATEOUTPUTPOSITIONS */
	.short 42
	.byte 0x0a,0x46,0x43,0x4d,0x43,0x4c,0x45,0x41,0x4e,0x55,0x50 /* FCMCLEANUP */
	.short 43
	.byte 0x12,0x46,0x43,0x4d,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x4e,0x54,0x42,0x55,0x46,0x46,0x45,0x52,0x53 /* FCMENUMFONTBUFFERS */
	.short 44
	.byte 0x0e,0x4d,0x49,0x53,0x43,0x47,0x45,0x54,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e /* MISCGETVERSION */
	.short 45
	.byte 0x15,0x4d,0x49,0x53,0x43,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54,0x46,0x4f,0x4e,0x54,0x46,0x41,0x4d,0x49,0x4c,0x59 /* MISCCONVERTFONTFAMILY */
	.short 46
	.byte 0x12,0x4d,0x49,0x53,0x43,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54,0x43,0x48,0x41,0x52,0x53,0x45,0x54 /* MISCCONVERTCHARSET */
	.short 47
	.byte 0x17,0x4d,0x49,0x53,0x43,0x49,0x46,0x49,0x4d,0x45,0x54,0x52,0x49,0x43,0x53,0x54,0x4f,0x4c,0x4f,0x47,0x46,0x4f,0x4e,0x54 /* MISCIFIMETRICSTOLOGFONT */
	.short 48
	.byte 0x1b,0x4d,0x49,0x53,0x43,0x49,0x46,0x49,0x4d,0x45,0x54,0x52,0x49,0x43,0x53,0x54,0x4f,0x54,0x45,0x58,0x54,0x4d,0x45,0x54,0x52,0x49,0x43,0x53 /* MISCIFIMETRICSTOTEXTMETRICS */
	.short 49
	.byte 0x11,0x4d,0x49,0x53,0x43,0x4d,0x41,0x4b,0x45,0x54,0x45,0x58,0x54,0x58,0x46,0x4f,0x52,0x4d /* MISCMAKETEXTXFORM */
	.short 50
	.byte 0x10,0x46,0x43,0x4d,0x43,0x41,0x4c,0x43,0x42,0x59,0x54,0x45,0x57,0x49,0x44,0x54,0x48 /* FCMCALCBYTEWIDTH */
	.short 51
	.byte 0x0f,0x46,0x43,0x4d,0x46,0x4f,0x52,0x43,0x45,0x43,0x41,0x43,0x48,0x45,0x49,0x4e /* FCMFORCECACHEIN */
	.short 52
	.byte 0x0d,0x46,0x43,0x4d,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x46,0x43 /* FCMVALIDATEFC */
	.short 53
	.byte 0x0f,0x46,0x43,0x4d,0x55,0x4e,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x46,0x43 /* FCMUNVALIDATEFC */
	.short 54
	.byte 0x10,0x46,0x43,0x4d,0x51,0x55,0x45,0x52,0x59,0x46,0x41,0x43,0x45,0x41,0x54,0x54,0x52 /* FCMQUERYFACEATTR */
	.short 55
	.byte 0x17,0x46,0x43,0x4d,0x50,0x52,0x4f,0x43,0x45,0x53,0x53,0x44,0x45,0x56,0x49,0x43,0x45,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c /* FCMPROCESSDEVICECONTROL */
	.short 56
	.byte 0x12,0x4d,0x49,0x53,0x43,0x54,0x52,0x41,0x4e,0x43,0x41,0x54,0x45,0x53,0x54,0x52,0x49,0x4e,0x47 /* MISCTRANCATESTRING */
	.short 57
	.byte 0x0e,0x4d,0x49,0x53,0x43,0x41,0x44,0x44,0x48,0x55,0x47,0x45,0x50,0x54,0x52 /* MISCADDHUGEPTR */
	.short 58
	.byte 0x17,0x4d,0x49,0x53,0x43,0x47,0x45,0x54,0x53,0x45,0x47,0x4d,0x45,0x4e,0x54,0x49,0x4e,0x43,0x52,0x45,0x4d,0x45,0x4e,0x54 /* MISCGETSEGMENTINCREMENT */
	.short 59
	.byte 0x11,0x4d,0x49,0x53,0x43,0x49,0x53,0x57,0x49,0x46,0x45,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c /* MISCISWIFECONTROL */
	.short 60
	.byte 0x12,0x4d,0x49,0x53,0x43,0x49,0x53,0x47,0x41,0x49,0x4a,0x49,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c /* MISCISGAIJICONTROL */
	.short 61
	.byte 0x17,0x46,0x43,0x4d,0x47,0x45,0x54,0x45,0x55,0x44,0x43,0x4c,0x45,0x41,0x44,0x42,0x59,0x54,0x45,0x52,0x41,0x4e,0x47,0x45 /* FCMGETEUDCLEADBYTERANGE */
	.short 62
	.byte 0x0c,0x46,0x43,0x4d,0x47,0x45,0x54,0x45,0x55,0x44,0x43,0x46,0x42 /* FCMGETEUDCFB */
	.short 63
	.byte 0x0f,0x46,0x43,0x4d,0x47,0x45,0x54,0x43,0x48,0x41,0x52,0x57,0x49,0x44,0x54,0x48 /* FCMGETCHARWIDTH */
	.short 64
	.byte 0x18,0x4d,0x49,0x53,0x43,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x4d,0x4f,0x4e,0x4f,0x46,0x4f,0x4e,0x54,0x49,0x4d,0x41,0x47,0x45 /* MISCSTRETCHMONOFONTIMAGE */
	.short 65
	.byte 0x18,0x4d,0x49,0x53,0x43,0x47,0x45,0x54,0x45,0x55,0x44,0x43,0x4c,0x45,0x41,0x44,0x42,0x59,0x54,0x45,0x52,0x41,0x4e,0x47,0x45 /* MISCGETEUDCLEADBYTERANGE */
	.short 66
	.byte 0x1c,0x4d,0x49,0x53,0x43,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x51,0x55,0x49,0x43,0x4b,0x53,0x45,0x41,0x52,0x43,0x48,0x54,0x41,0x42,0x4c,0x45 /* MISCVALIDATEQUICKSEARCHTABLE */
	.short 67
	.byte 0x1e,0x4d,0x49,0x53,0x43,0x55,0x4e,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x51,0x55,0x49,0x43,0x4b,0x53,0x45,0x41,0x52,0x43,0x48,0x54,0x41,0x42,0x4c,0x45 /* MISCUNVALIDATEQUICKSEARCHTABLE */
	.short 68
	.byte 0x1a,0x4d,0x49,0x53,0x43,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x4e,0x4f,0x54,0x49,0x46,0x59,0x46,0x55,0x4e,0x43,0x54,0x49,0x4f,0x4e /* MISCREGISTERNOTIFYFUNCTION */
	.short 69
	.byte 0x1c,0x4d,0x49,0x53,0x43,0x55,0x4e,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x4e,0x4f,0x54,0x49,0x46,0x59,0x46,0x55,0x4e,0x43,0x54,0x49,0x4f,0x4e /* MISCUNREGISTERNOTIFYFUNCTION */
	.short 70
	.byte 0x15,0x46,0x43,0x4d,0x52,0x45,0x51,0x55,0x45,0x53,0x54,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x46,0x42,0x45,0x58 /* FCMREQUESTDEFAULTFBEX */
	.short 71
	.byte 0x13,0x4d,0x49,0x53,0x43,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x57,0x49,0x46,0x45,0x46,0x4f,0x4e,0x54 /* MISCREALIZEWIFEFONT */
	.short 72
	.byte 0x12,0x4d,0x49,0x53,0x43,0x44,0x45,0x4c,0x45,0x54,0x45,0x57,0x49,0x46,0x45,0x46,0x4f,0x4e,0x54 /* MISCDELETEWIFEFONT */
	.short 73
	.byte 0x0b,0x4d,0x41,0x50,0x57,0x49,0x46,0x45,0x46,0x4f,0x4e,0x54 /* MAPWIFEFONT */
	.short 74
	.byte 0x14,0x53,0x45,0x54,0x43,0x4c,0x4f,0x53,0x45,0x4f,0x4c,0x44,0x45,0x53,0x54,0x46,0x43,0x46,0x55,0x4e,0x43 /* SETCLOSEOLDESTFCFUNC */
	.short 75
	.byte 0x15,0x43,0x41,0x43,0x48,0x45,0x44,0x4f,0x50,0x45,0x4e,0x46,0x4f,0x4e,0x54,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54 /* CACHEDOPENFONTCONTEXT */
	.short 76
	.byte 0x0c,0x46,0x49,0x4c,0x4c,0x46,0x4f,0x4e,0x54,0x49,0x4e,0x46,0x4f /* FILLFONTINFO */
	.short 77
	.byte 0x0f,0x46,0x49,0x4c,0x4c,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54,0x49,0x4e,0x46,0x4f /* FILLCONTEXTINFO */
	.short 78
	.byte 0x09,0x47,0x45,0x54,0x45,0x55,0x44,0x43,0x46,0x44 /* GETEUDCFD */
	.short 79
	.byte 0x12,0x47,0x45,0x54,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x46,0x4f,0x4e,0x54,0x49,0x4e,0x46,0x4f /* GETDEFAULTFONTINFO */
	.short 80
	.byte 0x15,0x44,0x49,0x41,0x47,0x4c,0x4f,0x43,0x41,0x4c,0x4c,0x4f,0x43,0x4b,0x41,0x4e,0x59,0x54,0x48,0x49,0x4e,0x47 /* DIAGLOCALLOCKANYTHING */
	.short 128
	.byte 0x17,0x44,0x49,0x41,0x47,0x4c,0x4f,0x43,0x41,0x4c,0x55,0x4e,0x4c,0x4f,0x43,0x4b,0x41,0x4e,0x59,0x54,0x48,0x49,0x4e,0x47 /* DIAGLOCALUNLOCKANYTHING */
	.short 129
	.byte 0x14,0x44,0x49,0x41,0x47,0x47,0x45,0x54,0x43,0x4f,0x4e,0x46,0x49,0x52,0x4d,0x53,0x54,0x52,0x49,0x4e,0x47 /* DIAGGETCONFIRMSTRING */
	.short 130
	.byte 0x0d,0x44,0x49,0x41,0x47,0x53,0x45,0x4c,0x46,0x43,0x48,0x45,0x43,0x4b /* DIAGSELFCHECK */
	.short 131
	.byte 0x17,0x44,0x49,0x41,0x47,0x53,0x45,0x4c,0x46,0x43,0x48,0x45,0x43,0x4b,0x41,0x4e,0x44,0x57,0x41,0x52,0x4e,0x49,0x4e,0x47 /* DIAGSELFCHECKANDWARNING */
	.short 132
	.byte 0x0e,0x44,0x45,0x42,0x55,0x47,0x41,0x53,0x53,0x45,0x52,0x54,0x49,0x4f,0x4e /* DEBUGASSERTION */
	.short 150
	.byte 0x0b,0x44,0x45,0x4c,0x41,0x59,0x45,0x44,0x49,0x4e,0x49,0x54 /* DELAYEDINIT */
	.short 151
	.byte 0x09,0x46,0x49,0x52,0x53,0x54,0x49,0x4e,0x49,0x54 /* FIRSTINIT */
	.short 152
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* WIFEMAN.1 - WIFEMAN.11 */
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_WIFEMAN_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_11-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* WIFEMAN.14 - WIFEMAN.80 */
	.byte 0x43,0x01
	.byte 0x03
	.short .L__wine_WIFEMAN_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_27-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_28-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_29-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_31-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_32-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_33-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_34-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_35-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_36-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_37-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_38-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_39-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_40-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_41-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_42-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_43-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_44-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_45-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_46-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_47-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_48-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_49-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_50-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_51-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_52-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_53-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_54-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_55-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_56-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_57-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_58-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_59-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_60-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_61-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_62-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_63-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_64-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_65-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_66-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_67-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_68-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_69-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_70-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_71-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_72-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_73-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_74-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_75-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_76-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_77-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_78-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_79-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_80-.L__wine_spec_code_segment
	.byte 0x2f,0x00
	/* WIFEMAN.128 - WIFEMAN.132 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_WIFEMAN_128-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_129-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_130-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_131-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_132-.L__wine_spec_code_segment
	.byte 0x11,0x00
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_WIFEMAN_150-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_151-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WIFEMAN_152-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_WIFEMAN_1:
	pushw %bp
	pushl $___wine_stub_FDILOADFONTFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_2:
	pushw %bp
	pushl $___wine_stub_FDIUNLOADFONTFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_3:
	pushw %bp
	pushl $___wine_stub_FDIQUERYFACES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_4:
	pushw %bp
	pushl $___wine_stub_FDIOPENFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_5:
	pushw %bp
	pushl $___wine_stub_FDISETFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_6:
	pushw %bp
	pushl $___wine_stub_FDICLOSEFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_7:
	pushw %bp
	pushl $___wine_stub_FDIQUERYFACEATTR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_8:
	pushw %bp
	pushl $___wine_stub_FDIQUERYCHARATTR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_9:
	pushw %bp
	pushl $___wine_stub_FDICONTROL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_10:
	pushw %bp
	pushl $___wine_stub_FDICLAIMFONTFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_11:
	pushw %bp
	pushl $___wine_stub_FDICONVERTFONTFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_14:
	pushw %bp
	pushl $___wine_stub_MIMADDFONTDRIVER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_15:
	pushw %bp
	pushl $___wine_stub_MIMADDFONTPACK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_16:
	pushw %bp
	pushl $___wine_stub_MIMREMOVEFONTDRIVER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_17:
	pushw %bp
	pushl $___wine_stub_MIMREMOVEFONTPACK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_18:
	pushw %bp
	pushl $___wine_stub_MIMENUMFONTDRIVERS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_19:
	pushw %bp
	pushl $___wine_stub_MIMENUMFONTPACKS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_20:
	pushw %bp
	pushl $___wine_stub_MIMOPENFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_21:
	pushw %bp
	pushl $___wine_stub_MIMCLOSEFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_22:
	pushw %bp
	pushl $___wine_stub_MIMGETINFORMATIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_23:
	pushw %bp
	pushl $___wine_stub_MISCISDBCSLEADBYTE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_24:
	pushw %bp
	pushl $___wine_stub_MISCSETERROINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_25:
	pushw %bp
	pushl $___wine_stub_MISCGETERROINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_26:
	pushw %bp
	pushl $___wine_stub_UBSTRLEN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_27:
	pushw %bp
	pushl $___wine_stub_UBSTRCPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_28:
	pushw %bp
	pushl $___wine_stub_UBSTRNCPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_29:
	pushw %bp
	pushl $___wine_stub_UBSTRCAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_30:
	pushw %bp
	pushl $___wine_stub_UBSTRCMP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_31:
	pushw %bp
	pushl $___wine_stub_ASCIZTOUZ
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_32:
	pushw %bp
	pushl $___wine_stub_UZTOASCIZ
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_33:
	pushw %bp
	pushl $___wine_stub_MISCWARNINGMESSAGE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_34:
	pushw %bp
	pushl $___wine_stub_MISCWARNINGMESSAGEWITHARGUMENT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_35:
	pushw %bp
	pushl $___wine_stub_FCMCREATECACHEBUFFER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_36:
	pushw %bp
	pushl $___wine_stub_FCMDELETECACHEBUFFER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_37:
	pushw %bp
	pushl $___wine_stub_FCMREQUESTIMAGES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_38:
	pushw %bp
	pushl $___wine_stub_FCMRELEASEIMAGES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_39:
	pushw %bp
	pushl $___wine_stub_FCMREQUESTDEFAULTFB
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_40:
	pushw %bp
	pushl $___wine_stub_FCMRELEASEDEFAULTFB
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_41:
	pushw %bp
	pushl $___wine_stub_FCMCALCULATETEXTEXTENT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_42:
	pushw %bp
	pushl $___wine_stub_FCMCALCULATEOUTPUTPOSITIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_43:
	pushw %bp
	pushl $___wine_stub_FCMCLEANUP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_44:
	pushw %bp
	pushl $___wine_stub_FCMENUMFONTBUFFERS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_45:
	pushw %bp
	pushl $___wine_stub_MISCGETVERSION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_46:
	pushw %bp
	pushl $___wine_stub_MISCCONVERTFONTFAMILY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_47:
	pushw %bp
	pushl $___wine_stub_MISCCONVERTCHARSET
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_48:
	pushw %bp
	pushl $___wine_stub_MISCIFIMETRICSTOLOGFONT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_49:
	pushw %bp
	pushl $___wine_stub_MISCIFIMETRICSTOTEXTMETRICS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_50:
	pushw %bp
	pushl $___wine_stub_MISCMAKETEXTXFORM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_51:
	pushw %bp
	pushl $___wine_stub_FCMCALCBYTEWIDTH
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_52:
	pushw %bp
	pushl $___wine_stub_FCMFORCECACHEIN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_53:
	pushw %bp
	pushl $___wine_stub_FCMVALIDATEFC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_54:
	pushw %bp
	pushl $___wine_stub_FCMUNVALIDATEFC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_55:
	pushw %bp
	pushl $___wine_stub_FCMQUERYFACEATTR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_56:
	pushw %bp
	pushl $___wine_stub_FCMPROCESSDEVICECONTROL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_57:
	pushw %bp
	pushl $___wine_stub_MISCTRANCATESTRING
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_58:
	pushw %bp
	pushl $___wine_stub_MISCADDHUGEPTR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_59:
	pushw %bp
	pushl $___wine_stub_MISCGETSEGMENTINCREMENT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_60:
	pushw %bp
	pushl $___wine_stub_MISCISWIFECONTROL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_61:
	pushw %bp
	pushl $___wine_stub_MISCISGAIJICONTROL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_62:
	pushw %bp
	pushl $___wine_stub_FCMGETEUDCLEADBYTERANGE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_63:
	pushw %bp
	pushl $___wine_stub_FCMGETEUDCFB
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_64:
	pushw %bp
	pushl $___wine_stub_FCMGETCHARWIDTH
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_65:
	pushw %bp
	pushl $___wine_stub_MISCSTRETCHMONOFONTIMAGE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_66:
	pushw %bp
	pushl $___wine_stub_MISCGETEUDCLEADBYTERANGE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_67:
	pushw %bp
	pushl $___wine_stub_MISCVALIDATEQUICKSEARCHTABLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_68:
	pushw %bp
	pushl $___wine_stub_MISCUNVALIDATEQUICKSEARCHTABLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_69:
	pushw %bp
	pushl $___wine_stub_MISCREGISTERNOTIFYFUNCTION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_70:
	pushw %bp
	pushl $___wine_stub_MISCUNREGISTERNOTIFYFUNCTION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_71:
	pushw %bp
	pushl $___wine_stub_FCMREQUESTDEFAULTFBEX
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_72:
	pushw %bp
	pushl $___wine_stub_MISCREALIZEWIFEFONT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_73:
	pushw %bp
	pushl $___wine_stub_MISCDELETEWIFEFONT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_74:
	pushw %bp
	pushl $___wine_stub_MAPWIFEFONT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_75:
	pushw %bp
	pushl $___wine_stub_SETCLOSEOLDESTFCFUNC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_76:
	pushw %bp
	pushl $___wine_stub_CACHEDOPENFONTCONTEXT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_77:
	pushw %bp
	pushl $___wine_stub_FILLFONTINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_78:
	pushw %bp
	pushl $___wine_stub_FILLCONTEXTINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_79:
	pushw %bp
	pushl $___wine_stub_GETEUDCFD
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_80:
	pushw %bp
	pushl $___wine_stub_GETDEFAULTFONTINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_128:
	pushw %bp
	pushl $___wine_stub_DIAGLOCALLOCKANYTHING
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_129:
	pushw %bp
	pushl $___wine_stub_DIAGLOCALUNLOCKANYTHING
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_130:
	pushw %bp
	pushl $___wine_stub_DIAGGETCONFIRMSTRING
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_131:
	pushw %bp
	pushl $___wine_stub_DIAGSELFCHECK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_132:
	pushw %bp
	pushl $___wine_stub_DIAGSELFCHECKANDWARNING
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_150:
	pushw %bp
	pushl $___wine_stub_DEBUGASSERTION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_151:
	pushw %bp
	pushl $___wine_stub_DELAYEDINIT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_WIFEMAN_152:
	pushw %bp
	pushl $___wine_stub_FIRSTINIT
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
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_FDILOADFONTFILE; .scl 2; .type 32; .endef
___wine_stub_FDILOADFONTFILE:
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
	movl $.L__wine_stub_FDILOADFONTFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDIUNLOADFONTFILE; .scl 2; .type 32; .endef
___wine_stub_FDIUNLOADFONTFILE:
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
	movl $.L__wine_stub_FDIUNLOADFONTFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDIQUERYFACES; .scl 2; .type 32; .endef
___wine_stub_FDIQUERYFACES:
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
	movl $.L__wine_stub_FDIQUERYFACES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDIOPENFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_FDIOPENFONTCONTEXT:
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
	movl $.L__wine_stub_FDIOPENFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDISETFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_FDISETFONTCONTEXT:
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
	movl $.L__wine_stub_FDISETFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDICLOSEFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_FDICLOSEFONTCONTEXT:
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
	movl $.L__wine_stub_FDICLOSEFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDIQUERYFACEATTR; .scl 2; .type 32; .endef
___wine_stub_FDIQUERYFACEATTR:
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
	movl $.L__wine_stub_FDIQUERYFACEATTR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDIQUERYCHARATTR; .scl 2; .type 32; .endef
___wine_stub_FDIQUERYCHARATTR:
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
	movl $.L__wine_stub_FDIQUERYCHARATTR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDICONTROL; .scl 2; .type 32; .endef
___wine_stub_FDICONTROL:
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
	movl $.L__wine_stub_FDICONTROL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDICLAIMFONTFILE; .scl 2; .type 32; .endef
___wine_stub_FDICLAIMFONTFILE:
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
	movl $.L__wine_stub_FDICLAIMFONTFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FDICONVERTFONTFILE; .scl 2; .type 32; .endef
___wine_stub_FDICONVERTFONTFILE:
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
	movl $.L__wine_stub_FDICONVERTFONTFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMADDFONTDRIVER; .scl 2; .type 32; .endef
___wine_stub_MIMADDFONTDRIVER:
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
	movl $.L__wine_stub_MIMADDFONTDRIVER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMADDFONTPACK; .scl 2; .type 32; .endef
___wine_stub_MIMADDFONTPACK:
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
	movl $.L__wine_stub_MIMADDFONTPACK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMREMOVEFONTDRIVER; .scl 2; .type 32; .endef
___wine_stub_MIMREMOVEFONTDRIVER:
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
	movl $.L__wine_stub_MIMREMOVEFONTDRIVER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMREMOVEFONTPACK; .scl 2; .type 32; .endef
___wine_stub_MIMREMOVEFONTPACK:
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
	movl $.L__wine_stub_MIMREMOVEFONTPACK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMENUMFONTDRIVERS; .scl 2; .type 32; .endef
___wine_stub_MIMENUMFONTDRIVERS:
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
	movl $.L__wine_stub_MIMENUMFONTDRIVERS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMENUMFONTPACKS; .scl 2; .type 32; .endef
___wine_stub_MIMENUMFONTPACKS:
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
	movl $.L__wine_stub_MIMENUMFONTPACKS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMOPENFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_MIMOPENFONTCONTEXT:
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
	movl $.L__wine_stub_MIMOPENFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMCLOSEFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_MIMCLOSEFONTCONTEXT:
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
	movl $.L__wine_stub_MIMCLOSEFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MIMGETINFORMATIONS; .scl 2; .type 32; .endef
___wine_stub_MIMGETINFORMATIONS:
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
	movl $.L__wine_stub_MIMGETINFORMATIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCISDBCSLEADBYTE; .scl 2; .type 32; .endef
___wine_stub_MISCISDBCSLEADBYTE:
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
	movl $.L__wine_stub_MISCISDBCSLEADBYTE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCSETERROINFO; .scl 2; .type 32; .endef
___wine_stub_MISCSETERROINFO:
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
	movl $.L__wine_stub_MISCSETERROINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCGETERROINFO; .scl 2; .type 32; .endef
___wine_stub_MISCGETERROINFO:
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
	movl $.L__wine_stub_MISCGETERROINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UBSTRLEN; .scl 2; .type 32; .endef
___wine_stub_UBSTRLEN:
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
	movl $.L__wine_stub_UBSTRLEN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UBSTRCPY; .scl 2; .type 32; .endef
___wine_stub_UBSTRCPY:
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
	movl $.L__wine_stub_UBSTRCPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UBSTRNCPY; .scl 2; .type 32; .endef
___wine_stub_UBSTRNCPY:
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
	movl $.L__wine_stub_UBSTRNCPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UBSTRCAT; .scl 2; .type 32; .endef
___wine_stub_UBSTRCAT:
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
	movl $.L__wine_stub_UBSTRCAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UBSTRCMP; .scl 2; .type 32; .endef
___wine_stub_UBSTRCMP:
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
	movl $.L__wine_stub_UBSTRCMP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ASCIZTOUZ; .scl 2; .type 32; .endef
___wine_stub_ASCIZTOUZ:
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
	movl $.L__wine_stub_ASCIZTOUZ_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UZTOASCIZ; .scl 2; .type 32; .endef
___wine_stub_UZTOASCIZ:
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
	movl $.L__wine_stub_UZTOASCIZ_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCWARNINGMESSAGE; .scl 2; .type 32; .endef
___wine_stub_MISCWARNINGMESSAGE:
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
	movl $.L__wine_stub_MISCWARNINGMESSAGE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCWARNINGMESSAGEWITHARGUMENT; .scl 2; .type 32; .endef
___wine_stub_MISCWARNINGMESSAGEWITHARGUMENT:
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
	movl $.L__wine_stub_MISCWARNINGMESSAGEWITHARGUMENT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMCREATECACHEBUFFER; .scl 2; .type 32; .endef
___wine_stub_FCMCREATECACHEBUFFER:
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
	movl $.L__wine_stub_FCMCREATECACHEBUFFER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMDELETECACHEBUFFER; .scl 2; .type 32; .endef
___wine_stub_FCMDELETECACHEBUFFER:
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
	movl $.L__wine_stub_FCMDELETECACHEBUFFER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMREQUESTIMAGES; .scl 2; .type 32; .endef
___wine_stub_FCMREQUESTIMAGES:
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
	movl $.L__wine_stub_FCMREQUESTIMAGES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMRELEASEIMAGES; .scl 2; .type 32; .endef
___wine_stub_FCMRELEASEIMAGES:
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
	movl $.L__wine_stub_FCMRELEASEIMAGES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMREQUESTDEFAULTFB; .scl 2; .type 32; .endef
___wine_stub_FCMREQUESTDEFAULTFB:
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
	movl $.L__wine_stub_FCMREQUESTDEFAULTFB_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMRELEASEDEFAULTFB; .scl 2; .type 32; .endef
___wine_stub_FCMRELEASEDEFAULTFB:
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
	movl $.L__wine_stub_FCMRELEASEDEFAULTFB_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMCALCULATETEXTEXTENT; .scl 2; .type 32; .endef
___wine_stub_FCMCALCULATETEXTEXTENT:
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
	movl $.L__wine_stub_FCMCALCULATETEXTEXTENT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMCALCULATEOUTPUTPOSITIONS; .scl 2; .type 32; .endef
___wine_stub_FCMCALCULATEOUTPUTPOSITIONS:
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
	movl $.L__wine_stub_FCMCALCULATEOUTPUTPOSITIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMCLEANUP; .scl 2; .type 32; .endef
___wine_stub_FCMCLEANUP:
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
	movl $.L__wine_stub_FCMCLEANUP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMENUMFONTBUFFERS; .scl 2; .type 32; .endef
___wine_stub_FCMENUMFONTBUFFERS:
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
	movl $.L__wine_stub_FCMENUMFONTBUFFERS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCGETVERSION; .scl 2; .type 32; .endef
___wine_stub_MISCGETVERSION:
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
	movl $.L__wine_stub_MISCGETVERSION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCCONVERTFONTFAMILY; .scl 2; .type 32; .endef
___wine_stub_MISCCONVERTFONTFAMILY:
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
	movl $.L__wine_stub_MISCCONVERTFONTFAMILY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCCONVERTCHARSET; .scl 2; .type 32; .endef
___wine_stub_MISCCONVERTCHARSET:
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
	movl $.L__wine_stub_MISCCONVERTCHARSET_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCIFIMETRICSTOLOGFONT; .scl 2; .type 32; .endef
___wine_stub_MISCIFIMETRICSTOLOGFONT:
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
	movl $.L__wine_stub_MISCIFIMETRICSTOLOGFONT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCIFIMETRICSTOTEXTMETRICS; .scl 2; .type 32; .endef
___wine_stub_MISCIFIMETRICSTOTEXTMETRICS:
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
	movl $.L__wine_stub_MISCIFIMETRICSTOTEXTMETRICS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCMAKETEXTXFORM; .scl 2; .type 32; .endef
___wine_stub_MISCMAKETEXTXFORM:
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
	movl $.L__wine_stub_MISCMAKETEXTXFORM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMCALCBYTEWIDTH; .scl 2; .type 32; .endef
___wine_stub_FCMCALCBYTEWIDTH:
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
	movl $.L__wine_stub_FCMCALCBYTEWIDTH_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMFORCECACHEIN; .scl 2; .type 32; .endef
___wine_stub_FCMFORCECACHEIN:
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
	movl $.L__wine_stub_FCMFORCECACHEIN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMVALIDATEFC; .scl 2; .type 32; .endef
___wine_stub_FCMVALIDATEFC:
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
	movl $.L__wine_stub_FCMVALIDATEFC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMUNVALIDATEFC; .scl 2; .type 32; .endef
___wine_stub_FCMUNVALIDATEFC:
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
	movl $.L__wine_stub_FCMUNVALIDATEFC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMQUERYFACEATTR; .scl 2; .type 32; .endef
___wine_stub_FCMQUERYFACEATTR:
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
	movl $.L__wine_stub_FCMQUERYFACEATTR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMPROCESSDEVICECONTROL; .scl 2; .type 32; .endef
___wine_stub_FCMPROCESSDEVICECONTROL:
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
	movl $.L__wine_stub_FCMPROCESSDEVICECONTROL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCTRANCATESTRING; .scl 2; .type 32; .endef
___wine_stub_MISCTRANCATESTRING:
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
	movl $.L__wine_stub_MISCTRANCATESTRING_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCADDHUGEPTR; .scl 2; .type 32; .endef
___wine_stub_MISCADDHUGEPTR:
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
	movl $.L__wine_stub_MISCADDHUGEPTR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCGETSEGMENTINCREMENT; .scl 2; .type 32; .endef
___wine_stub_MISCGETSEGMENTINCREMENT:
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
	movl $.L__wine_stub_MISCGETSEGMENTINCREMENT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCISWIFECONTROL; .scl 2; .type 32; .endef
___wine_stub_MISCISWIFECONTROL:
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
	movl $.L__wine_stub_MISCISWIFECONTROL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCISGAIJICONTROL; .scl 2; .type 32; .endef
___wine_stub_MISCISGAIJICONTROL:
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
	movl $.L__wine_stub_MISCISGAIJICONTROL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMGETEUDCLEADBYTERANGE; .scl 2; .type 32; .endef
___wine_stub_FCMGETEUDCLEADBYTERANGE:
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
	movl $.L__wine_stub_FCMGETEUDCLEADBYTERANGE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMGETEUDCFB; .scl 2; .type 32; .endef
___wine_stub_FCMGETEUDCFB:
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
	movl $.L__wine_stub_FCMGETEUDCFB_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMGETCHARWIDTH; .scl 2; .type 32; .endef
___wine_stub_FCMGETCHARWIDTH:
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
	movl $.L__wine_stub_FCMGETCHARWIDTH_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCSTRETCHMONOFONTIMAGE; .scl 2; .type 32; .endef
___wine_stub_MISCSTRETCHMONOFONTIMAGE:
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
	movl $.L__wine_stub_MISCSTRETCHMONOFONTIMAGE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCGETEUDCLEADBYTERANGE; .scl 2; .type 32; .endef
___wine_stub_MISCGETEUDCLEADBYTERANGE:
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
	movl $.L__wine_stub_MISCGETEUDCLEADBYTERANGE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCVALIDATEQUICKSEARCHTABLE; .scl 2; .type 32; .endef
___wine_stub_MISCVALIDATEQUICKSEARCHTABLE:
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
	movl $.L__wine_stub_MISCVALIDATEQUICKSEARCHTABLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCUNVALIDATEQUICKSEARCHTABLE; .scl 2; .type 32; .endef
___wine_stub_MISCUNVALIDATEQUICKSEARCHTABLE:
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
	movl $.L__wine_stub_MISCUNVALIDATEQUICKSEARCHTABLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCREGISTERNOTIFYFUNCTION; .scl 2; .type 32; .endef
___wine_stub_MISCREGISTERNOTIFYFUNCTION:
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
	movl $.L__wine_stub_MISCREGISTERNOTIFYFUNCTION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCUNREGISTERNOTIFYFUNCTION; .scl 2; .type 32; .endef
___wine_stub_MISCUNREGISTERNOTIFYFUNCTION:
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
	movl $.L__wine_stub_MISCUNREGISTERNOTIFYFUNCTION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FCMREQUESTDEFAULTFBEX; .scl 2; .type 32; .endef
___wine_stub_FCMREQUESTDEFAULTFBEX:
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
	movl $.L__wine_stub_FCMREQUESTDEFAULTFBEX_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCREALIZEWIFEFONT; .scl 2; .type 32; .endef
___wine_stub_MISCREALIZEWIFEFONT:
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
	movl $.L__wine_stub_MISCREALIZEWIFEFONT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MISCDELETEWIFEFONT; .scl 2; .type 32; .endef
___wine_stub_MISCDELETEWIFEFONT:
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
	movl $.L__wine_stub_MISCDELETEWIFEFONT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MAPWIFEFONT; .scl 2; .type 32; .endef
___wine_stub_MAPWIFEFONT:
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
	movl $.L__wine_stub_MAPWIFEFONT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SETCLOSEOLDESTFCFUNC; .scl 2; .type 32; .endef
___wine_stub_SETCLOSEOLDESTFCFUNC:
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
	movl $.L__wine_stub_SETCLOSEOLDESTFCFUNC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CACHEDOPENFONTCONTEXT; .scl 2; .type 32; .endef
___wine_stub_CACHEDOPENFONTCONTEXT:
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
	movl $.L__wine_stub_CACHEDOPENFONTCONTEXT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FILLFONTINFO; .scl 2; .type 32; .endef
___wine_stub_FILLFONTINFO:
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
	movl $.L__wine_stub_FILLFONTINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FILLCONTEXTINFO; .scl 2; .type 32; .endef
___wine_stub_FILLCONTEXTINFO:
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
	movl $.L__wine_stub_FILLCONTEXTINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GETEUDCFD; .scl 2; .type 32; .endef
___wine_stub_GETEUDCFD:
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
	movl $.L__wine_stub_GETEUDCFD_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GETDEFAULTFONTINFO; .scl 2; .type 32; .endef
___wine_stub_GETDEFAULTFONTINFO:
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
	movl $.L__wine_stub_GETDEFAULTFONTINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIAGLOCALLOCKANYTHING; .scl 2; .type 32; .endef
___wine_stub_DIAGLOCALLOCKANYTHING:
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
	movl $.L__wine_stub_DIAGLOCALLOCKANYTHING_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIAGLOCALUNLOCKANYTHING; .scl 2; .type 32; .endef
___wine_stub_DIAGLOCALUNLOCKANYTHING:
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
	movl $.L__wine_stub_DIAGLOCALUNLOCKANYTHING_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIAGGETCONFIRMSTRING; .scl 2; .type 32; .endef
___wine_stub_DIAGGETCONFIRMSTRING:
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
	movl $.L__wine_stub_DIAGGETCONFIRMSTRING_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIAGSELFCHECK; .scl 2; .type 32; .endef
___wine_stub_DIAGSELFCHECK:
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
	movl $.L__wine_stub_DIAGSELFCHECK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIAGSELFCHECKANDWARNING; .scl 2; .type 32; .endef
___wine_stub_DIAGSELFCHECKANDWARNING:
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
	movl $.L__wine_stub_DIAGSELFCHECKANDWARNING_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEBUGASSERTION; .scl 2; .type 32; .endef
___wine_stub_DEBUGASSERTION:
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
	movl $.L__wine_stub_DEBUGASSERTION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DELAYEDINIT; .scl 2; .type 32; .endef
___wine_stub_DELAYEDINIT:
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
	movl $.L__wine_stub_DELAYEDINIT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FIRSTINIT; .scl 2; .type 32; .endef
___wine_stub_FIRSTINIT:
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
	movl $.L__wine_stub_FIRSTINIT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_FDILOADFONTFILE_string:
	.string "FDILOADFONTFILE"
.L__wine_stub_FDIUNLOADFONTFILE_string:
	.string "FDIUNLOADFONTFILE"
.L__wine_stub_FDIQUERYFACES_string:
	.string "FDIQUERYFACES"
.L__wine_stub_FDIOPENFONTCONTEXT_string:
	.string "FDIOPENFONTCONTEXT"
.L__wine_stub_FDISETFONTCONTEXT_string:
	.string "FDISETFONTCONTEXT"
.L__wine_stub_FDICLOSEFONTCONTEXT_string:
	.string "FDICLOSEFONTCONTEXT"
.L__wine_stub_FDIQUERYFACEATTR_string:
	.string "FDIQUERYFACEATTR"
.L__wine_stub_FDIQUERYCHARATTR_string:
	.string "FDIQUERYCHARATTR"
.L__wine_stub_FDICONTROL_string:
	.string "FDICONTROL"
.L__wine_stub_FDICLAIMFONTFILE_string:
	.string "FDICLAIMFONTFILE"
.L__wine_stub_FDICONVERTFONTFILE_string:
	.string "FDICONVERTFONTFILE"
.L__wine_stub_MIMADDFONTDRIVER_string:
	.string "MIMADDFONTDRIVER"
.L__wine_stub_MIMADDFONTPACK_string:
	.string "MIMADDFONTPACK"
.L__wine_stub_MIMREMOVEFONTDRIVER_string:
	.string "MIMREMOVEFONTDRIVER"
.L__wine_stub_MIMREMOVEFONTPACK_string:
	.string "MIMREMOVEFONTPACK"
.L__wine_stub_MIMENUMFONTDRIVERS_string:
	.string "MIMENUMFONTDRIVERS"
.L__wine_stub_MIMENUMFONTPACKS_string:
	.string "MIMENUMFONTPACKS"
.L__wine_stub_MIMOPENFONTCONTEXT_string:
	.string "MIMOPENFONTCONTEXT"
.L__wine_stub_MIMCLOSEFONTCONTEXT_string:
	.string "MIMCLOSEFONTCONTEXT"
.L__wine_stub_MIMGETINFORMATIONS_string:
	.string "MIMGETINFORMATIONS"
.L__wine_stub_MISCISDBCSLEADBYTE_string:
	.string "MISCISDBCSLEADBYTE"
.L__wine_stub_MISCSETERROINFO_string:
	.string "MISCSETERROINFO"
.L__wine_stub_MISCGETERROINFO_string:
	.string "MISCGETERROINFO"
.L__wine_stub_UBSTRLEN_string:
	.string "UBSTRLEN"
.L__wine_stub_UBSTRCPY_string:
	.string "UBSTRCPY"
.L__wine_stub_UBSTRNCPY_string:
	.string "UBSTRNCPY"
.L__wine_stub_UBSTRCAT_string:
	.string "UBSTRCAT"
.L__wine_stub_UBSTRCMP_string:
	.string "UBSTRCMP"
.L__wine_stub_ASCIZTOUZ_string:
	.string "ASCIZTOUZ"
.L__wine_stub_UZTOASCIZ_string:
	.string "UZTOASCIZ"
.L__wine_stub_MISCWARNINGMESSAGE_string:
	.string "MISCWARNINGMESSAGE"
.L__wine_stub_MISCWARNINGMESSAGEWITHARGUMENT_string:
	.string "MISCWARNINGMESSAGEWITHARGUMENT"
.L__wine_stub_FCMCREATECACHEBUFFER_string:
	.string "FCMCREATECACHEBUFFER"
.L__wine_stub_FCMDELETECACHEBUFFER_string:
	.string "FCMDELETECACHEBUFFER"
.L__wine_stub_FCMREQUESTIMAGES_string:
	.string "FCMREQUESTIMAGES"
.L__wine_stub_FCMRELEASEIMAGES_string:
	.string "FCMRELEASEIMAGES"
.L__wine_stub_FCMREQUESTDEFAULTFB_string:
	.string "FCMREQUESTDEFAULTFB"
.L__wine_stub_FCMRELEASEDEFAULTFB_string:
	.string "FCMRELEASEDEFAULTFB"
.L__wine_stub_FCMCALCULATETEXTEXTENT_string:
	.string "FCMCALCULATETEXTEXTENT"
.L__wine_stub_FCMCALCULATEOUTPUTPOSITIONS_string:
	.string "FCMCALCULATEOUTPUTPOSITIONS"
.L__wine_stub_FCMCLEANUP_string:
	.string "FCMCLEANUP"
.L__wine_stub_FCMENUMFONTBUFFERS_string:
	.string "FCMENUMFONTBUFFERS"
.L__wine_stub_MISCGETVERSION_string:
	.string "MISCGETVERSION"
.L__wine_stub_MISCCONVERTFONTFAMILY_string:
	.string "MISCCONVERTFONTFAMILY"
.L__wine_stub_MISCCONVERTCHARSET_string:
	.string "MISCCONVERTCHARSET"
.L__wine_stub_MISCIFIMETRICSTOLOGFONT_string:
	.string "MISCIFIMETRICSTOLOGFONT"
.L__wine_stub_MISCIFIMETRICSTOTEXTMETRICS_string:
	.string "MISCIFIMETRICSTOTEXTMETRICS"
.L__wine_stub_MISCMAKETEXTXFORM_string:
	.string "MISCMAKETEXTXFORM"
.L__wine_stub_FCMCALCBYTEWIDTH_string:
	.string "FCMCALCBYTEWIDTH"
.L__wine_stub_FCMFORCECACHEIN_string:
	.string "FCMFORCECACHEIN"
.L__wine_stub_FCMVALIDATEFC_string:
	.string "FCMVALIDATEFC"
.L__wine_stub_FCMUNVALIDATEFC_string:
	.string "FCMUNVALIDATEFC"
.L__wine_stub_FCMQUERYFACEATTR_string:
	.string "FCMQUERYFACEATTR"
.L__wine_stub_FCMPROCESSDEVICECONTROL_string:
	.string "FCMPROCESSDEVICECONTROL"
.L__wine_stub_MISCTRANCATESTRING_string:
	.string "MISCTRANCATESTRING"
.L__wine_stub_MISCADDHUGEPTR_string:
	.string "MISCADDHUGEPTR"
.L__wine_stub_MISCGETSEGMENTINCREMENT_string:
	.string "MISCGETSEGMENTINCREMENT"
.L__wine_stub_MISCISWIFECONTROL_string:
	.string "MISCISWIFECONTROL"
.L__wine_stub_MISCISGAIJICONTROL_string:
	.string "MISCISGAIJICONTROL"
.L__wine_stub_FCMGETEUDCLEADBYTERANGE_string:
	.string "FCMGETEUDCLEADBYTERANGE"
.L__wine_stub_FCMGETEUDCFB_string:
	.string "FCMGETEUDCFB"
.L__wine_stub_FCMGETCHARWIDTH_string:
	.string "FCMGETCHARWIDTH"
.L__wine_stub_MISCSTRETCHMONOFONTIMAGE_string:
	.string "MISCSTRETCHMONOFONTIMAGE"
.L__wine_stub_MISCGETEUDCLEADBYTERANGE_string:
	.string "MISCGETEUDCLEADBYTERANGE"
.L__wine_stub_MISCVALIDATEQUICKSEARCHTABLE_string:
	.string "MISCVALIDATEQUICKSEARCHTABLE"
.L__wine_stub_MISCUNVALIDATEQUICKSEARCHTABLE_string:
	.string "MISCUNVALIDATEQUICKSEARCHTABLE"
.L__wine_stub_MISCREGISTERNOTIFYFUNCTION_string:
	.string "MISCREGISTERNOTIFYFUNCTION"
.L__wine_stub_MISCUNREGISTERNOTIFYFUNCTION_string:
	.string "MISCUNREGISTERNOTIFYFUNCTION"
.L__wine_stub_FCMREQUESTDEFAULTFBEX_string:
	.string "FCMREQUESTDEFAULTFBEX"
.L__wine_stub_MISCREALIZEWIFEFONT_string:
	.string "MISCREALIZEWIFEFONT"
.L__wine_stub_MISCDELETEWIFEFONT_string:
	.string "MISCDELETEWIFEFONT"
.L__wine_stub_MAPWIFEFONT_string:
	.string "MAPWIFEFONT"
.L__wine_stub_SETCLOSEOLDESTFCFUNC_string:
	.string "SETCLOSEOLDESTFCFUNC"
.L__wine_stub_CACHEDOPENFONTCONTEXT_string:
	.string "CACHEDOPENFONTCONTEXT"
.L__wine_stub_FILLFONTINFO_string:
	.string "FILLFONTINFO"
.L__wine_stub_FILLCONTEXTINFO_string:
	.string "FILLCONTEXTINFO"
.L__wine_stub_GETEUDCFD_string:
	.string "GETEUDCFD"
.L__wine_stub_GETDEFAULTFONTINFO_string:
	.string "GETDEFAULTFONTINFO"
.L__wine_stub_DIAGLOCALLOCKANYTHING_string:
	.string "DIAGLOCALLOCKANYTHING"
.L__wine_stub_DIAGLOCALUNLOCKANYTHING_string:
	.string "DIAGLOCALUNLOCKANYTHING"
.L__wine_stub_DIAGGETCONFIRMSTRING_string:
	.string "DIAGGETCONFIRMSTRING"
.L__wine_stub_DIAGSELFCHECK_string:
	.string "DIAGSELFCHECK"
.L__wine_stub_DIAGSELFCHECKANDWARNING_string:
	.string "DIAGSELFCHECKANDWARNING"
.L__wine_stub_DEBUGASSERTION_string:
	.string "DEBUGASSERTION"
.L__wine_stub_DELAYEDINIT_string:
	.string "DELAYEDINIT"
.L__wine_stub_FIRSTINIT_string:
	.string "FIRSTINIT"

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
	.long .L__wine_spec_exp_names+30-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+53-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "..\wifeman\wifeman.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "WIFEMAN"
__end:/*?*/
