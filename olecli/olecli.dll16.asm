/* File generated automatically from olecli.dll16.spec; do not edit! */
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
	.string "olecli.dll16.spec"

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
	.byte 0x06,0x4f,0x4c,0x45,0x43,0x4c,0x49 /* OLECLI */
	.short 0
	.byte 0x09,0x4f,0x4c,0x45,0x44,0x45,0x4c,0x45,0x54,0x45 /* OLEDELETE */
	.short 2
	.byte 0x0f,0x4f,0x4c,0x45,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* OLESAVETOSTREAM */
	.short 3
	.byte 0x11,0x4f,0x4c,0x45,0x4c,0x4f,0x41,0x44,0x46,0x52,0x4f,0x4d,0x53,0x54,0x52,0x45,0x41,0x4d /* OLELOADFROMSTREAM */
	.short 4
	.byte 0x08,0x4f,0x4c,0x45,0x43,0x4c,0x4f,0x4e,0x45 /* OLECLONE */
	.short 6
	.byte 0x0f,0x4f,0x4c,0x45,0x43,0x4f,0x50,0x59,0x46,0x52,0x4f,0x4d,0x4c,0x49,0x4e,0x4b /* OLECOPYFROMLINK */
	.short 7
	.byte 0x08,0x4f,0x4c,0x45,0x45,0x51,0x55,0x41,0x4c /* OLEEQUAL */
	.short 8
	.byte 0x14,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* OleQueryLinkFromClip */
	.short 9
	.byte 0x16,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* OleQueryCreateFromClip */
	.short 10
	.byte 0x15,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* OleCreateLinkFromClip */
	.short 11
	.byte 0x11,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* OleCreateFromClip */
	.short 12
	.byte 0x12,0x4f,0x4c,0x45,0x43,0x4f,0x50,0x59,0x54,0x4f,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44 /* OLECOPYTOCLIPBOARD */
	.short 13
	.byte 0x0c,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x54,0x59,0x50,0x45 /* OleQueryType */
	.short 14
	.byte 0x0f,0x4f,0x4c,0x45,0x53,0x45,0x54,0x48,0x4f,0x53,0x54,0x4e,0x41,0x4d,0x45,0x53 /* OLESETHOSTNAMES */
	.short 15
	.byte 0x12,0x4f,0x4c,0x45,0x53,0x45,0x54,0x54,0x41,0x52,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45 /* OLESETTARGETDEVICE */
	.short 16
	.byte 0x0c,0x4f,0x4c,0x45,0x53,0x45,0x54,0x42,0x4f,0x55,0x4e,0x44,0x53 /* OLESETBOUNDS */
	.short 17
	.byte 0x0e,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* OLEQUERYBOUNDS */
	.short 18
	.byte 0x07,0x4f,0x4c,0x45,0x44,0x52,0x41,0x57 /* OLEDRAW */
	.short 19
	.byte 0x0c,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4f,0x50,0x45,0x4e /* OLEQUERYOPEN */
	.short 20
	.byte 0x0b,0x4f,0x4c,0x45,0x41,0x43,0x54,0x49,0x56,0x41,0x54,0x45 /* OLEACTIVATE */
	.short 21
	.byte 0x09,0x4f,0x4c,0x45,0x55,0x50,0x44,0x41,0x54,0x45 /* OLEUPDATE */
	.short 22
	.byte 0x0c,0x4f,0x4c,0x45,0x52,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* OLERECONNECT */
	.short 23
	.byte 0x17,0x4f,0x4c,0x45,0x47,0x45,0x54,0x4c,0x49,0x4e,0x4b,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* OLEGETLINKUPDATEOPTIONS */
	.short 24
	.byte 0x17,0x4f,0x4c,0x45,0x53,0x45,0x54,0x4c,0x49,0x4e,0x4b,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* OLESETLINKUPDATEOPTIONS */
	.short 25
	.byte 0x0e,0x4f,0x4c,0x45,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54,0x53 /* OLEENUMFORMATS */
	.short 26
	.byte 0x08,0x4f,0x4c,0x45,0x43,0x4c,0x4f,0x53,0x45 /* OLECLOSE */
	.short 27
	.byte 0x0a,0x4f,0x4c,0x45,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* OLEGETDATA */
	.short 28
	.byte 0x0a,0x4f,0x4c,0x45,0x53,0x45,0x54,0x44,0x41,0x54,0x41 /* OLESETDATA */
	.short 29
	.byte 0x10,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x50,0x52,0x4f,0x54,0x4f,0x43,0x4f,0x4c /* OLEQUERYPROTOCOL */
	.short 30
	.byte 0x11,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4f,0x55,0x54,0x4f,0x46,0x44,0x41,0x54,0x45 /* OLEQUERYOUTOFDATE */
	.short 31
	.byte 0x10,0x4f,0x4c,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54 /* OLEOBJECTCONVERT */
	.short 32
	.byte 0x15,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x54,0x45,0x4d,0x50,0x4c,0x41,0x54,0x45 /* OLECREATEFROMTEMPLATE */
	.short 33
	.byte 0x09,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45 /* OLECREATE */
	.short 34
	.byte 0x15,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x53,0x54,0x41,0x54,0x55,0x53 /* OLEQUERYRELEASESTATUS */
	.short 35
	.byte 0x14,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x45,0x52,0x52,0x4f,0x52 /* OLEQUERYRELEASEERROR */
	.short 36
	.byte 0x15,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x4d,0x45,0x54,0x48,0x4f,0x44 /* OLEQUERYRELEASEMETHOD */
	.short 37
	.byte 0x11,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* OLECREATEFROMFILE */
	.short 38
	.byte 0x15,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* OLECREATELINKFROMFILE */
	.short 39
	.byte 0x0a,0x4f,0x4c,0x45,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* OLERELEASE */
	.short 40
	.byte 0x14,0x4f,0x4c,0x45,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x43,0x4c,0x49,0x45,0x4e,0x54,0x44,0x4f,0x43 /* OleRegisterClientDoc */
	.short 41
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x56,0x4f,0x4b,0x45,0x43,0x4c,0x49,0x45,0x4e,0x54,0x44,0x4f,0x43 /* OleRevokeClientDoc */
	.short 42
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x4e,0x41,0x4d,0x45,0x43,0x4c,0x49,0x45,0x4e,0x54,0x44,0x4f,0x43 /* OleRenameClientDoc */
	.short 43
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x56,0x45,0x52,0x54,0x43,0x4c,0x49,0x45,0x4e,0x54,0x44,0x4f,0x43 /* OleRevertClientDoc */
	.short 44
	.byte 0x11,0x4f,0x4c,0x45,0x53,0x41,0x56,0x45,0x44,0x43,0x4c,0x49,0x45,0x4e,0x54,0x44,0x4f,0x43 /* OleSavedClientDoc */
	.short 45
	.byte 0x09,0x4f,0x4c,0x45,0x52,0x45,0x4e,0x41,0x4d,0x45 /* OLERENAME */
	.short 46
	.byte 0x0e,0x4f,0x4c,0x45,0x45,0x4e,0x55,0x4d,0x4f,0x42,0x4a,0x45,0x43,0x54,0x53 /* OleEnumObjects */
	.short 47
	.byte 0x0c,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4e,0x41,0x4d,0x45 /* OLEQUERYNAME */
	.short 48
	.byte 0x11,0x4f,0x4c,0x45,0x53,0x45,0x54,0x43,0x4f,0x4c,0x4f,0x52,0x53,0x43,0x48,0x45,0x4d,0x45 /* OLESETCOLORSCHEME */
	.short 49
	.byte 0x0e,0x4f,0x4c,0x45,0x52,0x45,0x51,0x55,0x45,0x53,0x54,0x44,0x41,0x54,0x41 /* OLEREQUESTDATA */
	.short 50
	.byte 0x0d,0x4f,0x4c,0x45,0x4c,0x4f,0x43,0x4b,0x53,0x45,0x52,0x56,0x45,0x52 /* OLELOCKSERVER */
	.short 54
	.byte 0x0f,0x4f,0x4c,0x45,0x55,0x4e,0x4c,0x4f,0x43,0x4b,0x53,0x45,0x52,0x56,0x45,0x52 /* OLEUNLOCKSERVER */
	.short 55
	.byte 0x0c,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x53,0x49,0x5a,0x45 /* OLEQUERYSIZE */
	.short 56
	.byte 0x0a,0x4f,0x4c,0x45,0x45,0x58,0x45,0x43,0x55,0x54,0x45 /* OLEEXECUTE */
	.short 57
	.byte 0x12,0x4f,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x4e,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* OLECREATEINVISIBLE */
	.short 58
	.byte 0x15,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x43,0x4c,0x49,0x45,0x4e,0x54,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e /* OLEQUERYCLIENTVERSION */
	.short 59
	.byte 0x0b,0x4f,0x4c,0x45,0x49,0x53,0x44,0x43,0x4d,0x45,0x54,0x41 /* OleIsDcMeta */
	.short 60
	.byte 0x0a,0x44,0x4f,0x43,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* DOCWNDPROC */
	.short 100
	.byte 0x0b,0x53,0x52,0x56,0x52,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* SRVRWNDPROC */
	.short 101
	.byte 0x0e,0x4d,0x46,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x46,0x55,0x4e,0x43 /* MFCALLBACKFUNC */
	.short 102
	.byte 0x11,0x44,0x45,0x46,0x4c,0x4f,0x41,0x44,0x46,0x52,0x4f,0x4d,0x53,0x54,0x52,0x45,0x41,0x4d /* DEFLOADFROMSTREAM */
	.short 110
	.byte 0x11,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* DEFCREATEFROMCLIP */
	.short 111
	.byte 0x15,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* DEFCREATELINKFROMCLIP */
	.short 112
	.byte 0x15,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x54,0x45,0x4d,0x50,0x4c,0x41,0x54,0x45 /* DEFCREATEFROMTEMPLATE */
	.short 113
	.byte 0x09,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45 /* DEFCREATE */
	.short 114
	.byte 0x11,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* DEFCREATEFROMFILE */
	.short 115
	.byte 0x15,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* DEFCREATELINKFROMFILE */
	.short 116
	.byte 0x12,0x44,0x45,0x46,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x4e,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* DEFCREATEINVISIBLE */
	.short 117
	.byte 0x09,0x4c,0x45,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* LERELEASE */
	.short 200
	.byte 0x06,0x4c,0x45,0x53,0x48,0x4f,0x57 /* LESHOW */
	.short 201
	.byte 0x09,0x4c,0x45,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* LEGETDATA */
	.short 202
	.byte 0x09,0x4c,0x45,0x53,0x45,0x54,0x44,0x41,0x54,0x41 /* LESETDATA */
	.short 203
	.byte 0x0e,0x4c,0x45,0x53,0x45,0x54,0x48,0x4f,0x53,0x54,0x4e,0x41,0x4d,0x45,0x53 /* LESETHOSTNAMES */
	.short 204
	.byte 0x11,0x4c,0x45,0x53,0x45,0x54,0x54,0x41,0x52,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45 /* LESETTARGETDEVICE */
	.short 205
	.byte 0x0b,0x4c,0x45,0x53,0x45,0x54,0x42,0x4f,0x55,0x4e,0x44,0x53 /* LESETBOUNDS */
	.short 206
	.byte 0x0e,0x4c,0x45,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* LESAVETOSTREAM */
	.short 207
	.byte 0x07,0x4c,0x45,0x43,0x4c,0x4f,0x4e,0x45 /* LECLONE */
	.short 208
	.byte 0x0e,0x4c,0x45,0x43,0x4f,0x50,0x59,0x46,0x52,0x4f,0x4d,0x4c,0x49,0x4e,0x4b /* LECOPYFROMLINK */
	.short 209
	.byte 0x07,0x4c,0x45,0x45,0x51,0x55,0x41,0x4c /* LEEQUAL */
	.short 210
	.byte 0x06,0x4c,0x45,0x43,0x4f,0x50,0x59 /* LECOPY */
	.short 211
	.byte 0x0b,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x54,0x59,0x50,0x45 /* LEQUERYTYPE */
	.short 212
	.byte 0x0d,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* LEQUERYBOUNDS */
	.short 213
	.byte 0x06,0x4c,0x45,0x44,0x52,0x41,0x57 /* LEDRAW */
	.short 214
	.byte 0x0b,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4f,0x50,0x45,0x4e /* LEQUERYOPEN */
	.short 215
	.byte 0x0a,0x4c,0x45,0x41,0x43,0x54,0x49,0x56,0x41,0x54,0x45 /* LEACTIVATE */
	.short 216
	.byte 0x08,0x4c,0x45,0x55,0x50,0x44,0x41,0x54,0x45 /* LEUPDATE */
	.short 218
	.byte 0x0b,0x4c,0x45,0x52,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* LERECONNECT */
	.short 219
	.byte 0x0c,0x4c,0x45,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54 /* LEENUMFORMAT */
	.short 220
	.byte 0x0f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x50,0x52,0x4f,0x54,0x4f,0x43,0x4f,0x4c /* LEQUERYPROTOCOL */
	.short 221
	.byte 0x10,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x4f,0x55,0x54,0x4f,0x46,0x44,0x41,0x54,0x45 /* LEQUERYOUTOFDATE */
	.short 222
	.byte 0x0f,0x4c,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54 /* LEOBJECTCONVERT */
	.short 223
	.byte 0x0c,0x4c,0x45,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x41,0x54,0x41 /* LECHANGEDATA */
	.short 224
	.byte 0x07,0x4c,0x45,0x43,0x4c,0x4f,0x53,0x45 /* LECLOSE */
	.short 225
	.byte 0x12,0x4c,0x45,0x47,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* LEGETUPDATEOPTIONS */
	.short 226
	.byte 0x12,0x4c,0x45,0x53,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* LESETUPDATEOPTIONS */
	.short 227
	.byte 0x09,0x4c,0x45,0x45,0x58,0x45,0x43,0x55,0x54,0x45 /* LEEXECUTE */
	.short 228
	.byte 0x0c,0x4c,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54,0x4c,0x4f,0x4e,0x47 /* LEOBJECTLONG */
	.short 229
	.byte 0x11,0x4c,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x4e,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* LECREATEINVISIBLE */
	.short 230
	.byte 0x09,0x4d,0x46,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* MFRELEASE */
	.short 300
	.byte 0x09,0x4d,0x46,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* MFGETDATA */
	.short 301
	.byte 0x0e,0x4d,0x46,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* MFSAVETOSTREAM */
	.short 302
	.byte 0x07,0x4d,0x46,0x43,0x4c,0x4f,0x4e,0x45 /* MFCLONE */
	.short 303
	.byte 0x07,0x4d,0x46,0x45,0x51,0x55,0x41,0x4c /* MFEQUAL */
	.short 304
	.byte 0x06,0x4d,0x46,0x43,0x4f,0x50,0x59 /* MFCOPY */
	.short 305
	.byte 0x0d,0x4d,0x46,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* MFQUERYBOUNDS */
	.short 307
	.byte 0x06,0x4d,0x46,0x44,0x52,0x41,0x57 /* MFDRAW */
	.short 308
	.byte 0x0c,0x4d,0x46,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54 /* MFENUMFORMAT */
	.short 309
	.byte 0x0c,0x4d,0x46,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x41,0x54,0x41 /* MFCHANGEDATA */
	.short 310
	.byte 0x09,0x42,0x4d,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* BMRELEASE */
	.short 400
	.byte 0x09,0x42,0x4d,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* BMGETDATA */
	.short 401
	.byte 0x0e,0x42,0x4d,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* BMSAVETOSTREAM */
	.short 402
	.byte 0x07,0x42,0x4d,0x43,0x4c,0x4f,0x4e,0x45 /* BMCLONE */
	.short 403
	.byte 0x07,0x42,0x4d,0x45,0x51,0x55,0x41,0x4c /* BMEQUAL */
	.short 404
	.byte 0x06,0x42,0x4d,0x43,0x4f,0x50,0x59 /* BMCOPY */
	.short 405
	.byte 0x0d,0x42,0x4d,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* BMQUERYBOUNDS */
	.short 407
	.byte 0x06,0x42,0x4d,0x44,0x52,0x41,0x57 /* BMDRAW */
	.short 408
	.byte 0x0c,0x42,0x4d,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54 /* BMENUMFORMAT */
	.short 409
	.byte 0x0c,0x42,0x4d,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x41,0x54,0x41 /* BMCHANGEDATA */
	.short 410
	.byte 0x0a,0x44,0x49,0x42,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* DIBRELEASE */
	.short 500
	.byte 0x0a,0x44,0x49,0x42,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* DIBGETDATA */
	.short 501
	.byte 0x0f,0x44,0x49,0x42,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* DIBSAVETOSTREAM */
	.short 502
	.byte 0x08,0x44,0x49,0x42,0x43,0x4c,0x4f,0x4e,0x45 /* DIBCLONE */
	.short 503
	.byte 0x08,0x44,0x49,0x42,0x45,0x51,0x55,0x41,0x4c /* DIBEQUAL */
	.short 504
	.byte 0x07,0x44,0x49,0x42,0x43,0x4f,0x50,0x59 /* DIBCOPY */
	.short 505
	.byte 0x0e,0x44,0x49,0x42,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* DIBQUERYBOUNDS */
	.short 507
	.byte 0x07,0x44,0x49,0x42,0x44,0x52,0x41,0x57 /* DIBDRAW */
	.short 508
	.byte 0x0d,0x44,0x49,0x42,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54 /* DIBENUMFORMAT */
	.short 509
	.byte 0x0d,0x44,0x49,0x42,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x41,0x54,0x41 /* DIBCHANGEDATA */
	.short 510
	.byte 0x0a,0x47,0x45,0x4e,0x52,0x45,0x4c,0x45,0x41,0x53,0x45 /* GENRELEASE */
	.short 600
	.byte 0x0a,0x47,0x45,0x4e,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* GENGETDATA */
	.short 601
	.byte 0x0a,0x47,0x45,0x4e,0x53,0x45,0x54,0x44,0x41,0x54,0x41 /* GENSETDATA */
	.short 602
	.byte 0x0f,0x47,0x45,0x4e,0x53,0x41,0x56,0x45,0x54,0x4f,0x53,0x54,0x52,0x45,0x41,0x4d /* GENSAVETOSTREAM */
	.short 603
	.byte 0x08,0x47,0x45,0x4e,0x43,0x4c,0x4f,0x4e,0x45 /* GENCLONE */
	.short 604
	.byte 0x08,0x47,0x45,0x4e,0x45,0x51,0x55,0x41,0x4c /* GENEQUAL */
	.short 605
	.byte 0x07,0x47,0x45,0x4e,0x43,0x4f,0x50,0x59 /* GENCOPY */
	.short 606
	.byte 0x0e,0x47,0x45,0x4e,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* GENQUERYBOUNDS */
	.short 608
	.byte 0x07,0x47,0x45,0x4e,0x44,0x52,0x41,0x57 /* GENDRAW */
	.short 609
	.byte 0x0d,0x47,0x45,0x4e,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54 /* GENENUMFORMAT */
	.short 610
	.byte 0x0d,0x47,0x45,0x4e,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x41,0x54,0x41 /* GENCHANGEDATA */
	.short 611
	.byte 0x07,0x45,0x52,0x52,0x53,0x48,0x4f,0x57 /* ERRSHOW */
	.short 701
	.byte 0x0a,0x45,0x52,0x52,0x53,0x45,0x54,0x44,0x41,0x54,0x41 /* ERRSETDATA */
	.short 702
	.byte 0x0f,0x45,0x52,0x52,0x53,0x45,0x54,0x48,0x4f,0x53,0x54,0x4e,0x41,0x4d,0x45,0x53 /* ERRSETHOSTNAMES */
	.short 703
	.byte 0x12,0x45,0x52,0x52,0x53,0x45,0x54,0x54,0x41,0x52,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45 /* ERRSETTARGETDEVICE */
	.short 704
	.byte 0x0c,0x45,0x52,0x52,0x53,0x45,0x54,0x42,0x4f,0x55,0x4e,0x44,0x53 /* ERRSETBOUNDS */
	.short 705
	.byte 0x0f,0x45,0x52,0x52,0x43,0x4f,0x50,0x59,0x46,0x52,0x4f,0x4d,0x4c,0x49,0x4e,0x4b /* ERRCOPYFROMLINK */
	.short 706
	.byte 0x0c,0x45,0x52,0x52,0x51,0x55,0x45,0x52,0x59,0x4f,0x50,0x45,0x4e /* ERRQUERYOPEN */
	.short 707
	.byte 0x0b,0x45,0x52,0x52,0x41,0x43,0x54,0x49,0x56,0x41,0x54,0x45 /* ERRACTIVATE */
	.short 708
	.byte 0x08,0x45,0x52,0x52,0x43,0x4c,0x4f,0x53,0x45 /* ERRCLOSE */
	.short 709
	.byte 0x09,0x45,0x52,0x52,0x55,0x50,0x44,0x41,0x54,0x45 /* ERRUPDATE */
	.short 710
	.byte 0x0c,0x45,0x52,0x52,0x52,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* ERRRECONNECT */
	.short 711
	.byte 0x10,0x45,0x52,0x52,0x51,0x55,0x45,0x52,0x59,0x50,0x52,0x4f,0x54,0x4f,0x43,0x4f,0x4c /* ERRQUERYPROTOCOL */
	.short 712
	.byte 0x11,0x45,0x52,0x52,0x51,0x55,0x45,0x52,0x59,0x4f,0x55,0x54,0x4f,0x46,0x44,0x41,0x54,0x45 /* ERRQUERYOUTOFDATE */
	.short 713
	.byte 0x10,0x45,0x52,0x52,0x4f,0x42,0x4a,0x45,0x43,0x54,0x43,0x4f,0x4e,0x56,0x45,0x52,0x54 /* ERROBJECTCONVERT */
	.short 714
	.byte 0x13,0x45,0x52,0x52,0x47,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* ERRGETUPDATEOPTIONS */
	.short 715
	.byte 0x13,0x45,0x52,0x52,0x53,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x4f,0x50,0x54,0x49,0x4f,0x4e,0x53 /* ERRSETUPDATEOPTIONS */
	.short 716
	.byte 0x0a,0x45,0x52,0x52,0x45,0x58,0x45,0x43,0x55,0x54,0x45 /* ERREXECUTE */
	.short 717
	.byte 0x0d,0x45,0x52,0x52,0x4f,0x42,0x4a,0x45,0x43,0x54,0x4c,0x4f,0x4e,0x47 /* ERROBJECTLONG */
	.short 718
	.byte 0x10,0x50,0x42,0x4c,0x4f,0x41,0x44,0x46,0x52,0x4f,0x4d,0x53,0x54,0x52,0x45,0x41,0x4d /* PBLOADFROMSTREAM */
	.short 800
	.byte 0x10,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* PBCREATEFROMCLIP */
	.short 801
	.byte 0x14,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x43,0x4c,0x49,0x50 /* PBCREATELINKFROMCLIP */
	.short 802
	.byte 0x14,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x54,0x45,0x4d,0x50,0x4c,0x41,0x54,0x45 /* PBCREATEFROMTEMPLATE */
	.short 803
	.byte 0x08,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45 /* PBCREATE */
	.short 804
	.byte 0x06,0x50,0x42,0x44,0x52,0x41,0x57 /* PBDRAW */
	.short 805
	.byte 0x0d,0x50,0x42,0x51,0x55,0x45,0x52,0x59,0x42,0x4f,0x55,0x4e,0x44,0x53 /* PBQUERYBOUNDS */
	.short 806
	.byte 0x11,0x50,0x42,0x43,0x4f,0x50,0x59,0x54,0x4f,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44 /* PBCOPYTOCLIPBOARD */
	.short 807
	.byte 0x10,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* PBCREATEFROMFILE */
	.short 808
	.byte 0x14,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x4c,0x49,0x4e,0x4b,0x46,0x52,0x4f,0x4d,0x46,0x49,0x4c,0x45 /* PBCREATELINKFROMFILE */
	.short 809
	.byte 0x0d,0x50,0x42,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x4d,0x41,0x54,0x53 /* PBENUMFORMATS */
	.short 810
	.byte 0x09,0x50,0x42,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* PBGETDATA */
	.short 811
	.byte 0x11,0x50,0x42,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x4e,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* PBCREATEINVISIBLE */
	.short 812
	.byte 0x0c,0x4f,0x42,0x4a,0x51,0x55,0x45,0x52,0x59,0x4e,0x41,0x4d,0x45 /* OBJQUERYNAME */
	.short 910
	.byte 0x09,0x4f,0x42,0x4a,0x52,0x45,0x4e,0x41,0x4d,0x45 /* OBJRENAME */
	.short 911
	.byte 0x0c,0x4f,0x42,0x4a,0x51,0x55,0x45,0x52,0x59,0x54,0x59,0x50,0x45 /* OBJQUERYTYPE */
	.short 912
	.byte 0x0c,0x4f,0x42,0x4a,0x51,0x55,0x45,0x52,0x59,0x53,0x49,0x5a,0x45 /* OBJQUERYSIZE */
	.short 913
	.byte 0x0e,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x44,0x4c,0x47,0x50,0x52,0x4f,0x43 /* CONNECTDLGPROC */
	.short 950
	.byte 0x0a,0x53,0x45,0x54,0x4e,0x45,0x54,0x4e,0x41,0x4d,0x45 /* SETNETNAME */
	.short 951
	.byte 0x0d,0x43,0x48,0x45,0x43,0x4b,0x4e,0x45,0x54,0x44,0x52,0x49,0x56,0x45 /* CHECKNETDRIVE */
	.short 952
	.byte 0x0f,0x53,0x45,0x54,0x4e,0x45,0x58,0x54,0x4e,0x45,0x54,0x44,0x52,0x49,0x56,0x45 /* SETNEXTNETDRIVE */
	.short 953
	.byte 0x14,0x47,0x45,0x54,0x54,0x41,0x53,0x4b,0x56,0x49,0x53,0x49,0x42,0x4c,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GETTASKVISIBLEWINDOW */
	.short 954
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x01,0x00
	/* OLECLI.2 - OLECLI.4 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_OLECLI_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_4-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.6 - OLECLI.50 */
	.byte 0x2d,0x01
	.byte 0x03
	.short .L__wine_OLECLI_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_13-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_27-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_28-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_29-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_31-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_32-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_33-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_34-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_35-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_36-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_37-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_38-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_39-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_40-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_41-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_42-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_43-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_44-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_45-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_46-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_47-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_48-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_49-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_50-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* OLECLI.54 - OLECLI.60 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_OLECLI_54-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_55-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_56-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_57-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_58-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_59-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_60-.L__wine_spec_code_segment
	.byte 0x27,0x00
	/* OLECLI.100 - OLECLI.102 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_OLECLI_100-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_101-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_102-.L__wine_spec_code_segment
	.byte 0x07,0x00
	/* OLECLI.110 - OLECLI.117 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_OLECLI_110-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_111-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_112-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_113-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_114-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_115-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_116-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_117-.L__wine_spec_code_segment
	.byte 0x52,0x00
	/* OLECLI.200 - OLECLI.216 */
	.byte 0x11,0x01
	.byte 0x03
	.short .L__wine_OLECLI_200-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_201-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_202-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_203-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_204-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_205-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_206-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_207-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_208-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_209-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_210-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_211-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_212-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_213-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_214-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_215-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_216-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.218 - OLECLI.230 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_OLECLI_218-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_219-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_220-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_221-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_222-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_223-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_224-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_225-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_226-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_227-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_228-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_229-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_230-.L__wine_spec_code_segment
	.byte 0x45,0x00
	/* OLECLI.300 - OLECLI.305 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_OLECLI_300-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_301-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_302-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_303-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_304-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_305-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.307 - OLECLI.310 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_OLECLI_307-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_308-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_309-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_310-.L__wine_spec_code_segment
	.byte 0x59,0x00
	/* OLECLI.400 - OLECLI.405 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_OLECLI_400-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_401-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_402-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_403-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_404-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_405-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.407 - OLECLI.410 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_OLECLI_407-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_408-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_409-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_410-.L__wine_spec_code_segment
	.byte 0x59,0x00
	/* OLECLI.500 - OLECLI.505 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_OLECLI_500-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_501-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_502-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_503-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_504-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_505-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.507 - OLECLI.510 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_OLECLI_507-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_508-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_509-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_510-.L__wine_spec_code_segment
	.byte 0x59,0x00
	/* OLECLI.600 - OLECLI.606 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_OLECLI_600-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_601-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_602-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_603-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_604-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_605-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_606-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* OLECLI.608 - OLECLI.611 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_OLECLI_608-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_609-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_610-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_611-.L__wine_spec_code_segment
	.byte 0x59,0x00
	/* OLECLI.701 - OLECLI.718 */
	.byte 0x12,0x01
	.byte 0x03
	.short .L__wine_OLECLI_701-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_702-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_703-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_704-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_705-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_706-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_707-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_708-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_709-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_710-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_711-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_712-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_713-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_714-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_715-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_716-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_717-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_718-.L__wine_spec_code_segment
	.byte 0x51,0x00
	/* OLECLI.800 - OLECLI.812 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_OLECLI_800-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_801-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_802-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_803-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_804-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_805-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_806-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_807-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_808-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_809-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_810-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_811-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_812-.L__wine_spec_code_segment
	.byte 0x61,0x00
	/* OLECLI.910 - OLECLI.913 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_OLECLI_910-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_911-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_912-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_913-.L__wine_spec_code_segment
	.byte 0x24,0x00
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_OLECLI_950-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_951-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_952-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_953-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLECLI_954-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_l:
	pushl $.L__wine_spec_call16_p_l
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000003,0x00000000
.L__wine_spec_callfrom16_p_long_lp:
	pushl $.L__wine_spec_call16_p_lp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000023,0x00000000
.L__wine_spec_callfrom16_p_long_pplp:
	pushl $.L__wine_spec_call16_p_pplp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000008e4,0x00000000
.L__wine_spec_callfrom16_p_word_l:
	pushl $.L__wine_spec_call16_p_l
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000003,0x00000000
.L__wine_spec_callfrom16_p_word_ll:
	pushl $.L__wine_spec_call16_p_ll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000001b,0x00000000
.L__wine_spec_callfrom16_p_word_pl:
	pushl $.L__wine_spec_call16_p_pl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000001c,0x00000000
.L__wine_spec_callfrom16_p_word_tlltlww:
	pushl $.L__wine_spec_call16_p_pllplww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $24
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0004badd,0x00000000
.L__wine_spec_callfrom16_p_word_tww:
	pushl $.L__wine_spec_call16_p_pww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000004d,0x00000000
.L__wine_spec_callfrom16_p_word_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $2
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_OLECLI_2:
	pushw %bp
	pushl $___wine_stub_OLEDELETE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_3:
	pushw %bp
	pushl $___wine_stub_OLESAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_4:
	pushw %bp
	pushl $___wine_stub_OLELOADFROMSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_6:
	pushw %bp
	pushl $___wine_stub_OLECLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_7:
	pushw %bp
	pushl $___wine_stub_OLECOPYFROMLINK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_8:
	pushw %bp
	pushl $___wine_stub_OLEEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_9:
	pushw %bp
	pushl $_OleQueryLinkFromClip16@12
	callw .L__wine_spec_callfrom16_p_word_tww
.L__wine_OLECLI_10:
	pushw %bp
	pushl $_OleQueryCreateFromClip16@12
	callw .L__wine_spec_callfrom16_p_word_tww
.L__wine_OLECLI_11:
	pushw %bp
	pushl $_OleCreateLinkFromClip16@28
	callw .L__wine_spec_callfrom16_p_word_tlltlww
.L__wine_OLECLI_12:
	pushw %bp
	pushl $_OleCreateFromClip16@28
	callw .L__wine_spec_callfrom16_p_word_tlltlww
.L__wine_OLECLI_13:
	pushw %bp
	pushl $___wine_stub_OLECOPYTOCLIPBOARD
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_14:
	pushw %bp
	pushl $_OleQueryType16@8
	callw .L__wine_spec_callfrom16_p_word_pl
.L__wine_OLECLI_15:
	pushw %bp
	pushl $___wine_stub_OLESETHOSTNAMES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_16:
	pushw %bp
	pushl $___wine_stub_OLESETTARGETDEVICE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_17:
	pushw %bp
	pushl $___wine_stub_OLESETBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_18:
	pushw %bp
	pushl $___wine_stub_OLEQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_19:
	pushw %bp
	pushl $___wine_stub_OLEDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_20:
	pushw %bp
	pushl $___wine_stub_OLEQUERYOPEN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_21:
	pushw %bp
	pushl $___wine_stub_OLEACTIVATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_22:
	pushw %bp
	pushl $___wine_stub_OLEUPDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_23:
	pushw %bp
	pushl $___wine_stub_OLERECONNECT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_24:
	pushw %bp
	pushl $___wine_stub_OLEGETLINKUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_25:
	pushw %bp
	pushl $___wine_stub_OLESETLINKUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_26:
	pushw %bp
	pushl $___wine_stub_OLEENUMFORMATS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_27:
	pushw %bp
	pushl $___wine_stub_OLECLOSE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_28:
	pushw %bp
	pushl $___wine_stub_OLEGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_29:
	pushw %bp
	pushl $___wine_stub_OLESETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_30:
	pushw %bp
	pushl $___wine_stub_OLEQUERYPROTOCOL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_31:
	pushw %bp
	pushl $___wine_stub_OLEQUERYOUTOFDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_32:
	pushw %bp
	pushl $___wine_stub_OLEOBJECTCONVERT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_33:
	pushw %bp
	pushl $___wine_stub_OLECREATEFROMTEMPLATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_34:
	pushw %bp
	pushl $___wine_stub_OLECREATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_35:
	pushw %bp
	pushl $___wine_stub_OLEQUERYRELEASESTATUS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_36:
	pushw %bp
	pushl $___wine_stub_OLEQUERYRELEASEERROR
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_37:
	pushw %bp
	pushl $___wine_stub_OLEQUERYRELEASEMETHOD
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_38:
	pushw %bp
	pushl $___wine_stub_OLECREATEFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_39:
	pushw %bp
	pushl $___wine_stub_OLECREATELINKFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_40:
	pushw %bp
	pushl $___wine_stub_OLERELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_41:
	pushw %bp
	pushl $_OleRegisterClientDoc16@16
	callw .L__wine_spec_callfrom16_p_long_pplp
.L__wine_OLECLI_42:
	pushw %bp
	pushl $_OleRevokeClientDoc16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLECLI_43:
	pushw %bp
	pushl $_OleRenameClientDoc16@8
	callw .L__wine_spec_callfrom16_p_long_lp
.L__wine_OLECLI_44:
	pushw %bp
	pushl $_OleRevertClientDoc16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_OLECLI_45:
	pushw %bp
	pushl $_OleSavedClientDoc16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLECLI_46:
	pushw %bp
	pushl $___wine_stub_OLERENAME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_47:
	pushw %bp
	pushl $_OleEnumObjects16@8
	callw .L__wine_spec_callfrom16_p_word_ll
.L__wine_OLECLI_48:
	pushw %bp
	pushl $___wine_stub_OLEQUERYNAME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_49:
	pushw %bp
	pushl $___wine_stub_OLESETCOLORSCHEME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_50:
	pushw %bp
	pushl $___wine_stub_OLEREQUESTDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_54:
	pushw %bp
	pushl $___wine_stub_OLELOCKSERVER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_55:
	pushw %bp
	pushl $___wine_stub_OLEUNLOCKSERVER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_56:
	pushw %bp
	pushl $___wine_stub_OLEQUERYSIZE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_57:
	pushw %bp
	pushl $___wine_stub_OLEEXECUTE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_58:
	pushw %bp
	pushl $___wine_stub_OLECREATEINVISIBLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_59:
	pushw %bp
	pushl $___wine_stub_OLEQUERYCLIENTVERSION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_60:
	pushw %bp
	pushl $_OleIsDcMeta16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_OLECLI_100:
	pushw %bp
	pushl $___wine_stub_DOCWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_101:
	pushw %bp
	pushl $___wine_stub_SRVRWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_102:
	pushw %bp
	pushl $___wine_stub_MFCALLBACKFUNC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_110:
	pushw %bp
	pushl $___wine_stub_DEFLOADFROMSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_111:
	pushw %bp
	pushl $___wine_stub_DEFCREATEFROMCLIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_112:
	pushw %bp
	pushl $___wine_stub_DEFCREATELINKFROMCLIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_113:
	pushw %bp
	pushl $___wine_stub_DEFCREATEFROMTEMPLATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_114:
	pushw %bp
	pushl $___wine_stub_DEFCREATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_115:
	pushw %bp
	pushl $___wine_stub_DEFCREATEFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_116:
	pushw %bp
	pushl $___wine_stub_DEFCREATELINKFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_117:
	pushw %bp
	pushl $___wine_stub_DEFCREATEINVISIBLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_200:
	pushw %bp
	pushl $___wine_stub_LERELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_201:
	pushw %bp
	pushl $___wine_stub_LESHOW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_202:
	pushw %bp
	pushl $___wine_stub_LEGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_203:
	pushw %bp
	pushl $___wine_stub_LESETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_204:
	pushw %bp
	pushl $___wine_stub_LESETHOSTNAMES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_205:
	pushw %bp
	pushl $___wine_stub_LESETTARGETDEVICE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_206:
	pushw %bp
	pushl $___wine_stub_LESETBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_207:
	pushw %bp
	pushl $___wine_stub_LESAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_208:
	pushw %bp
	pushl $___wine_stub_LECLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_209:
	pushw %bp
	pushl $___wine_stub_LECOPYFROMLINK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_210:
	pushw %bp
	pushl $___wine_stub_LEEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_211:
	pushw %bp
	pushl $___wine_stub_LECOPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_212:
	pushw %bp
	pushl $___wine_stub_LEQUERYTYPE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_213:
	pushw %bp
	pushl $___wine_stub_LEQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_214:
	pushw %bp
	pushl $___wine_stub_LEDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_215:
	pushw %bp
	pushl $___wine_stub_LEQUERYOPEN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_216:
	pushw %bp
	pushl $___wine_stub_LEACTIVATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_218:
	pushw %bp
	pushl $___wine_stub_LEUPDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_219:
	pushw %bp
	pushl $___wine_stub_LERECONNECT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_220:
	pushw %bp
	pushl $___wine_stub_LEENUMFORMAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_221:
	pushw %bp
	pushl $___wine_stub_LEQUERYPROTOCOL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_222:
	pushw %bp
	pushl $___wine_stub_LEQUERYOUTOFDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_223:
	pushw %bp
	pushl $___wine_stub_LEOBJECTCONVERT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_224:
	pushw %bp
	pushl $___wine_stub_LECHANGEDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_225:
	pushw %bp
	pushl $___wine_stub_LECLOSE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_226:
	pushw %bp
	pushl $___wine_stub_LEGETUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_227:
	pushw %bp
	pushl $___wine_stub_LESETUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_228:
	pushw %bp
	pushl $___wine_stub_LEEXECUTE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_229:
	pushw %bp
	pushl $___wine_stub_LEOBJECTLONG
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_230:
	pushw %bp
	pushl $___wine_stub_LECREATEINVISIBLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_300:
	pushw %bp
	pushl $___wine_stub_MFRELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_301:
	pushw %bp
	pushl $___wine_stub_MFGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_302:
	pushw %bp
	pushl $___wine_stub_MFSAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_303:
	pushw %bp
	pushl $___wine_stub_MFCLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_304:
	pushw %bp
	pushl $___wine_stub_MFEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_305:
	pushw %bp
	pushl $___wine_stub_MFCOPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_307:
	pushw %bp
	pushl $___wine_stub_MFQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_308:
	pushw %bp
	pushl $___wine_stub_MFDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_309:
	pushw %bp
	pushl $___wine_stub_MFENUMFORMAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_310:
	pushw %bp
	pushl $___wine_stub_MFCHANGEDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_400:
	pushw %bp
	pushl $___wine_stub_BMRELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_401:
	pushw %bp
	pushl $___wine_stub_BMGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_402:
	pushw %bp
	pushl $___wine_stub_BMSAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_403:
	pushw %bp
	pushl $___wine_stub_BMCLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_404:
	pushw %bp
	pushl $___wine_stub_BMEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_405:
	pushw %bp
	pushl $___wine_stub_BMCOPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_407:
	pushw %bp
	pushl $___wine_stub_BMQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_408:
	pushw %bp
	pushl $___wine_stub_BMDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_409:
	pushw %bp
	pushl $___wine_stub_BMENUMFORMAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_410:
	pushw %bp
	pushl $___wine_stub_BMCHANGEDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_500:
	pushw %bp
	pushl $___wine_stub_DIBRELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_501:
	pushw %bp
	pushl $___wine_stub_DIBGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_502:
	pushw %bp
	pushl $___wine_stub_DIBSAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_503:
	pushw %bp
	pushl $___wine_stub_DIBCLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_504:
	pushw %bp
	pushl $___wine_stub_DIBEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_505:
	pushw %bp
	pushl $___wine_stub_DIBCOPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_507:
	pushw %bp
	pushl $___wine_stub_DIBQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_508:
	pushw %bp
	pushl $___wine_stub_DIBDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_509:
	pushw %bp
	pushl $___wine_stub_DIBENUMFORMAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_510:
	pushw %bp
	pushl $___wine_stub_DIBCHANGEDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_600:
	pushw %bp
	pushl $___wine_stub_GENRELEASE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_601:
	pushw %bp
	pushl $___wine_stub_GENGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_602:
	pushw %bp
	pushl $___wine_stub_GENSETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_603:
	pushw %bp
	pushl $___wine_stub_GENSAVETOSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_604:
	pushw %bp
	pushl $___wine_stub_GENCLONE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_605:
	pushw %bp
	pushl $___wine_stub_GENEQUAL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_606:
	pushw %bp
	pushl $___wine_stub_GENCOPY
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_608:
	pushw %bp
	pushl $___wine_stub_GENQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_609:
	pushw %bp
	pushl $___wine_stub_GENDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_610:
	pushw %bp
	pushl $___wine_stub_GENENUMFORMAT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_611:
	pushw %bp
	pushl $___wine_stub_GENCHANGEDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_701:
	pushw %bp
	pushl $___wine_stub_ERRSHOW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_702:
	pushw %bp
	pushl $___wine_stub_ERRSETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_703:
	pushw %bp
	pushl $___wine_stub_ERRSETHOSTNAMES
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_704:
	pushw %bp
	pushl $___wine_stub_ERRSETTARGETDEVICE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_705:
	pushw %bp
	pushl $___wine_stub_ERRSETBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_706:
	pushw %bp
	pushl $___wine_stub_ERRCOPYFROMLINK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_707:
	pushw %bp
	pushl $___wine_stub_ERRQUERYOPEN
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_708:
	pushw %bp
	pushl $___wine_stub_ERRACTIVATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_709:
	pushw %bp
	pushl $___wine_stub_ERRCLOSE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_710:
	pushw %bp
	pushl $___wine_stub_ERRUPDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_711:
	pushw %bp
	pushl $___wine_stub_ERRRECONNECT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_712:
	pushw %bp
	pushl $___wine_stub_ERRQUERYPROTOCOL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_713:
	pushw %bp
	pushl $___wine_stub_ERRQUERYOUTOFDATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_714:
	pushw %bp
	pushl $___wine_stub_ERROBJECTCONVERT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_715:
	pushw %bp
	pushl $___wine_stub_ERRGETUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_716:
	pushw %bp
	pushl $___wine_stub_ERRSETUPDATEOPTIONS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_717:
	pushw %bp
	pushl $___wine_stub_ERREXECUTE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_718:
	pushw %bp
	pushl $___wine_stub_ERROBJECTLONG
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_800:
	pushw %bp
	pushl $___wine_stub_PBLOADFROMSTREAM
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_801:
	pushw %bp
	pushl $___wine_stub_PBCREATEFROMCLIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_802:
	pushw %bp
	pushl $___wine_stub_PBCREATELINKFROMCLIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_803:
	pushw %bp
	pushl $___wine_stub_PBCREATEFROMTEMPLATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_804:
	pushw %bp
	pushl $___wine_stub_PBCREATE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_805:
	pushw %bp
	pushl $___wine_stub_PBDRAW
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_806:
	pushw %bp
	pushl $___wine_stub_PBQUERYBOUNDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_807:
	pushw %bp
	pushl $___wine_stub_PBCOPYTOCLIPBOARD
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_808:
	pushw %bp
	pushl $___wine_stub_PBCREATEFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_809:
	pushw %bp
	pushl $___wine_stub_PBCREATELINKFROMFILE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_810:
	pushw %bp
	pushl $___wine_stub_PBENUMFORMATS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_811:
	pushw %bp
	pushl $___wine_stub_PBGETDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_812:
	pushw %bp
	pushl $___wine_stub_PBCREATEINVISIBLE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_910:
	pushw %bp
	pushl $___wine_stub_OBJQUERYNAME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_911:
	pushw %bp
	pushl $___wine_stub_OBJRENAME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_912:
	pushw %bp
	pushl $___wine_stub_OBJQUERYTYPE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_913:
	pushw %bp
	pushl $___wine_stub_OBJQUERYSIZE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_950:
	pushw %bp
	pushl $___wine_stub_CONNECTDLGPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_951:
	pushw %bp
	pushl $___wine_stub_SETNETNAME
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_952:
	pushw %bp
	pushl $___wine_stub_CHECKNETDRIVE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_953:
	pushw %bp
	pushl $___wine_stub_SETNEXTNETDRIVE
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLECLI_954:
	pushw %bp
	pushl $___wine_stub_GETTASKVISIBLEWINDOW
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
	.def .L__wine_spec_call16_p_ll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ll:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lp:
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
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
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
	.def .L__wine_spec_call16_p_pllplww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pllplww:
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
	pushl 4(%ecx)
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 12(%ecx)
	pushl 16(%ecx)
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
	.def .L__wine_spec_call16_p_pplp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pplp:
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
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_OLEDELETE; .scl 2; .type 32; .endef
___wine_stub_OLEDELETE:
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
	movl $.L__wine_stub_OLEDELETE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_OLESAVETOSTREAM:
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
	movl $.L__wine_stub_OLESAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLELOADFROMSTREAM; .scl 2; .type 32; .endef
___wine_stub_OLELOADFROMSTREAM:
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
	movl $.L__wine_stub_OLELOADFROMSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECLONE; .scl 2; .type 32; .endef
___wine_stub_OLECLONE:
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
	movl $.L__wine_stub_OLECLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECOPYFROMLINK; .scl 2; .type 32; .endef
___wine_stub_OLECOPYFROMLINK:
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
	movl $.L__wine_stub_OLECOPYFROMLINK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEEQUAL; .scl 2; .type 32; .endef
___wine_stub_OLEEQUAL:
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
	movl $.L__wine_stub_OLEEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECOPYTOCLIPBOARD; .scl 2; .type 32; .endef
___wine_stub_OLECOPYTOCLIPBOARD:
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
	movl $.L__wine_stub_OLECOPYTOCLIPBOARD_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETHOSTNAMES; .scl 2; .type 32; .endef
___wine_stub_OLESETHOSTNAMES:
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
	movl $.L__wine_stub_OLESETHOSTNAMES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETTARGETDEVICE; .scl 2; .type 32; .endef
___wine_stub_OLESETTARGETDEVICE:
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
	movl $.L__wine_stub_OLESETTARGETDEVICE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETBOUNDS; .scl 2; .type 32; .endef
___wine_stub_OLESETBOUNDS:
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
	movl $.L__wine_stub_OLESETBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYBOUNDS:
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
	movl $.L__wine_stub_OLEQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEDRAW; .scl 2; .type 32; .endef
___wine_stub_OLEDRAW:
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
	movl $.L__wine_stub_OLEDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYOPEN; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYOPEN:
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
	movl $.L__wine_stub_OLEQUERYOPEN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEACTIVATE; .scl 2; .type 32; .endef
___wine_stub_OLEACTIVATE:
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
	movl $.L__wine_stub_OLEACTIVATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEUPDATE; .scl 2; .type 32; .endef
___wine_stub_OLEUPDATE:
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
	movl $.L__wine_stub_OLEUPDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLERECONNECT; .scl 2; .type 32; .endef
___wine_stub_OLERECONNECT:
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
	movl $.L__wine_stub_OLERECONNECT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEGETLINKUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_OLEGETLINKUPDATEOPTIONS:
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
	movl $.L__wine_stub_OLEGETLINKUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETLINKUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_OLESETLINKUPDATEOPTIONS:
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
	movl $.L__wine_stub_OLESETLINKUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEENUMFORMATS; .scl 2; .type 32; .endef
___wine_stub_OLEENUMFORMATS:
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
	movl $.L__wine_stub_OLEENUMFORMATS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECLOSE; .scl 2; .type 32; .endef
___wine_stub_OLECLOSE:
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
	movl $.L__wine_stub_OLECLOSE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEGETDATA; .scl 2; .type 32; .endef
___wine_stub_OLEGETDATA:
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
	movl $.L__wine_stub_OLEGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETDATA; .scl 2; .type 32; .endef
___wine_stub_OLESETDATA:
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
	movl $.L__wine_stub_OLESETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYPROTOCOL; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYPROTOCOL:
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
	movl $.L__wine_stub_OLEQUERYPROTOCOL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYOUTOFDATE; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYOUTOFDATE:
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
	movl $.L__wine_stub_OLEQUERYOUTOFDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEOBJECTCONVERT; .scl 2; .type 32; .endef
___wine_stub_OLEOBJECTCONVERT:
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
	movl $.L__wine_stub_OLEOBJECTCONVERT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECREATEFROMTEMPLATE; .scl 2; .type 32; .endef
___wine_stub_OLECREATEFROMTEMPLATE:
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
	movl $.L__wine_stub_OLECREATEFROMTEMPLATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECREATE; .scl 2; .type 32; .endef
___wine_stub_OLECREATE:
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
	movl $.L__wine_stub_OLECREATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYRELEASESTATUS; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYRELEASESTATUS:
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
	movl $.L__wine_stub_OLEQUERYRELEASESTATUS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYRELEASEERROR; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYRELEASEERROR:
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
	movl $.L__wine_stub_OLEQUERYRELEASEERROR_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYRELEASEMETHOD; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYRELEASEMETHOD:
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
	movl $.L__wine_stub_OLEQUERYRELEASEMETHOD_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECREATEFROMFILE; .scl 2; .type 32; .endef
___wine_stub_OLECREATEFROMFILE:
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
	movl $.L__wine_stub_OLECREATEFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECREATELINKFROMFILE; .scl 2; .type 32; .endef
___wine_stub_OLECREATELINKFROMFILE:
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
	movl $.L__wine_stub_OLECREATELINKFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLERELEASE; .scl 2; .type 32; .endef
___wine_stub_OLERELEASE:
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
	movl $.L__wine_stub_OLERELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLERENAME; .scl 2; .type 32; .endef
___wine_stub_OLERENAME:
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
	movl $.L__wine_stub_OLERENAME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYNAME; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYNAME:
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
	movl $.L__wine_stub_OLEQUERYNAME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLESETCOLORSCHEME; .scl 2; .type 32; .endef
___wine_stub_OLESETCOLORSCHEME:
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
	movl $.L__wine_stub_OLESETCOLORSCHEME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEREQUESTDATA; .scl 2; .type 32; .endef
___wine_stub_OLEREQUESTDATA:
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
	movl $.L__wine_stub_OLEREQUESTDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLELOCKSERVER; .scl 2; .type 32; .endef
___wine_stub_OLELOCKSERVER:
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
	movl $.L__wine_stub_OLELOCKSERVER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEUNLOCKSERVER; .scl 2; .type 32; .endef
___wine_stub_OLEUNLOCKSERVER:
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
	movl $.L__wine_stub_OLEUNLOCKSERVER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYSIZE; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYSIZE:
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
	movl $.L__wine_stub_OLEQUERYSIZE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEEXECUTE; .scl 2; .type 32; .endef
___wine_stub_OLEEXECUTE:
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
	movl $.L__wine_stub_OLEEXECUTE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLECREATEINVISIBLE; .scl 2; .type 32; .endef
___wine_stub_OLECREATEINVISIBLE:
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
	movl $.L__wine_stub_OLECREATEINVISIBLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYCLIENTVERSION; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYCLIENTVERSION:
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
	movl $.L__wine_stub_OLEQUERYCLIENTVERSION_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DOCWNDPROC; .scl 2; .type 32; .endef
___wine_stub_DOCWNDPROC:
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
	movl $.L__wine_stub_DOCWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SRVRWNDPROC; .scl 2; .type 32; .endef
___wine_stub_SRVRWNDPROC:
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
	movl $.L__wine_stub_SRVRWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFCALLBACKFUNC; .scl 2; .type 32; .endef
___wine_stub_MFCALLBACKFUNC:
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
	movl $.L__wine_stub_MFCALLBACKFUNC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFLOADFROMSTREAM; .scl 2; .type 32; .endef
___wine_stub_DEFLOADFROMSTREAM:
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
	movl $.L__wine_stub_DEFLOADFROMSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATEFROMCLIP; .scl 2; .type 32; .endef
___wine_stub_DEFCREATEFROMCLIP:
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
	movl $.L__wine_stub_DEFCREATEFROMCLIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATELINKFROMCLIP; .scl 2; .type 32; .endef
___wine_stub_DEFCREATELINKFROMCLIP:
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
	movl $.L__wine_stub_DEFCREATELINKFROMCLIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATEFROMTEMPLATE; .scl 2; .type 32; .endef
___wine_stub_DEFCREATEFROMTEMPLATE:
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
	movl $.L__wine_stub_DEFCREATEFROMTEMPLATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATE; .scl 2; .type 32; .endef
___wine_stub_DEFCREATE:
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
	movl $.L__wine_stub_DEFCREATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATEFROMFILE; .scl 2; .type 32; .endef
___wine_stub_DEFCREATEFROMFILE:
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
	movl $.L__wine_stub_DEFCREATEFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATELINKFROMFILE; .scl 2; .type 32; .endef
___wine_stub_DEFCREATELINKFROMFILE:
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
	movl $.L__wine_stub_DEFCREATELINKFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DEFCREATEINVISIBLE; .scl 2; .type 32; .endef
___wine_stub_DEFCREATEINVISIBLE:
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
	movl $.L__wine_stub_DEFCREATEINVISIBLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LERELEASE; .scl 2; .type 32; .endef
___wine_stub_LERELEASE:
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
	movl $.L__wine_stub_LERELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESHOW; .scl 2; .type 32; .endef
___wine_stub_LESHOW:
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
	movl $.L__wine_stub_LESHOW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEGETDATA; .scl 2; .type 32; .endef
___wine_stub_LEGETDATA:
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
	movl $.L__wine_stub_LEGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESETDATA; .scl 2; .type 32; .endef
___wine_stub_LESETDATA:
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
	movl $.L__wine_stub_LESETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESETHOSTNAMES; .scl 2; .type 32; .endef
___wine_stub_LESETHOSTNAMES:
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
	movl $.L__wine_stub_LESETHOSTNAMES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESETTARGETDEVICE; .scl 2; .type 32; .endef
___wine_stub_LESETTARGETDEVICE:
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
	movl $.L__wine_stub_LESETTARGETDEVICE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESETBOUNDS; .scl 2; .type 32; .endef
___wine_stub_LESETBOUNDS:
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
	movl $.L__wine_stub_LESETBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_LESAVETOSTREAM:
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
	movl $.L__wine_stub_LESAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECLONE; .scl 2; .type 32; .endef
___wine_stub_LECLONE:
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
	movl $.L__wine_stub_LECLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECOPYFROMLINK; .scl 2; .type 32; .endef
___wine_stub_LECOPYFROMLINK:
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
	movl $.L__wine_stub_LECOPYFROMLINK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEEQUAL; .scl 2; .type 32; .endef
___wine_stub_LEEQUAL:
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
	movl $.L__wine_stub_LEEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECOPY; .scl 2; .type 32; .endef
___wine_stub_LECOPY:
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
	movl $.L__wine_stub_LECOPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEQUERYTYPE; .scl 2; .type 32; .endef
___wine_stub_LEQUERYTYPE:
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
	movl $.L__wine_stub_LEQUERYTYPE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_LEQUERYBOUNDS:
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
	movl $.L__wine_stub_LEQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEDRAW; .scl 2; .type 32; .endef
___wine_stub_LEDRAW:
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
	movl $.L__wine_stub_LEDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEQUERYOPEN; .scl 2; .type 32; .endef
___wine_stub_LEQUERYOPEN:
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
	movl $.L__wine_stub_LEQUERYOPEN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEACTIVATE; .scl 2; .type 32; .endef
___wine_stub_LEACTIVATE:
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
	movl $.L__wine_stub_LEACTIVATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEUPDATE; .scl 2; .type 32; .endef
___wine_stub_LEUPDATE:
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
	movl $.L__wine_stub_LEUPDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LERECONNECT; .scl 2; .type 32; .endef
___wine_stub_LERECONNECT:
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
	movl $.L__wine_stub_LERECONNECT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEENUMFORMAT; .scl 2; .type 32; .endef
___wine_stub_LEENUMFORMAT:
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
	movl $.L__wine_stub_LEENUMFORMAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEQUERYPROTOCOL; .scl 2; .type 32; .endef
___wine_stub_LEQUERYPROTOCOL:
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
	movl $.L__wine_stub_LEQUERYPROTOCOL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEQUERYOUTOFDATE; .scl 2; .type 32; .endef
___wine_stub_LEQUERYOUTOFDATE:
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
	movl $.L__wine_stub_LEQUERYOUTOFDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEOBJECTCONVERT; .scl 2; .type 32; .endef
___wine_stub_LEOBJECTCONVERT:
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
	movl $.L__wine_stub_LEOBJECTCONVERT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECHANGEDATA; .scl 2; .type 32; .endef
___wine_stub_LECHANGEDATA:
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
	movl $.L__wine_stub_LECHANGEDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECLOSE; .scl 2; .type 32; .endef
___wine_stub_LECLOSE:
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
	movl $.L__wine_stub_LECLOSE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEGETUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_LEGETUPDATEOPTIONS:
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
	movl $.L__wine_stub_LEGETUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LESETUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_LESETUPDATEOPTIONS:
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
	movl $.L__wine_stub_LESETUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEEXECUTE; .scl 2; .type 32; .endef
___wine_stub_LEEXECUTE:
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
	movl $.L__wine_stub_LEEXECUTE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LEOBJECTLONG; .scl 2; .type 32; .endef
___wine_stub_LEOBJECTLONG:
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
	movl $.L__wine_stub_LEOBJECTLONG_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LECREATEINVISIBLE; .scl 2; .type 32; .endef
___wine_stub_LECREATEINVISIBLE:
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
	movl $.L__wine_stub_LECREATEINVISIBLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFRELEASE; .scl 2; .type 32; .endef
___wine_stub_MFRELEASE:
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
	movl $.L__wine_stub_MFRELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFGETDATA; .scl 2; .type 32; .endef
___wine_stub_MFGETDATA:
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
	movl $.L__wine_stub_MFGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFSAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_MFSAVETOSTREAM:
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
	movl $.L__wine_stub_MFSAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFCLONE; .scl 2; .type 32; .endef
___wine_stub_MFCLONE:
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
	movl $.L__wine_stub_MFCLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFEQUAL; .scl 2; .type 32; .endef
___wine_stub_MFEQUAL:
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
	movl $.L__wine_stub_MFEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFCOPY; .scl 2; .type 32; .endef
___wine_stub_MFCOPY:
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
	movl $.L__wine_stub_MFCOPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_MFQUERYBOUNDS:
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
	movl $.L__wine_stub_MFQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFDRAW; .scl 2; .type 32; .endef
___wine_stub_MFDRAW:
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
	movl $.L__wine_stub_MFDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFENUMFORMAT; .scl 2; .type 32; .endef
___wine_stub_MFENUMFORMAT:
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
	movl $.L__wine_stub_MFENUMFORMAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MFCHANGEDATA; .scl 2; .type 32; .endef
___wine_stub_MFCHANGEDATA:
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
	movl $.L__wine_stub_MFCHANGEDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMRELEASE; .scl 2; .type 32; .endef
___wine_stub_BMRELEASE:
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
	movl $.L__wine_stub_BMRELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMGETDATA; .scl 2; .type 32; .endef
___wine_stub_BMGETDATA:
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
	movl $.L__wine_stub_BMGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMSAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_BMSAVETOSTREAM:
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
	movl $.L__wine_stub_BMSAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMCLONE; .scl 2; .type 32; .endef
___wine_stub_BMCLONE:
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
	movl $.L__wine_stub_BMCLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMEQUAL; .scl 2; .type 32; .endef
___wine_stub_BMEQUAL:
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
	movl $.L__wine_stub_BMEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMCOPY; .scl 2; .type 32; .endef
___wine_stub_BMCOPY:
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
	movl $.L__wine_stub_BMCOPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_BMQUERYBOUNDS:
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
	movl $.L__wine_stub_BMQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMDRAW; .scl 2; .type 32; .endef
___wine_stub_BMDRAW:
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
	movl $.L__wine_stub_BMDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMENUMFORMAT; .scl 2; .type 32; .endef
___wine_stub_BMENUMFORMAT:
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
	movl $.L__wine_stub_BMENUMFORMAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BMCHANGEDATA; .scl 2; .type 32; .endef
___wine_stub_BMCHANGEDATA:
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
	movl $.L__wine_stub_BMCHANGEDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBRELEASE; .scl 2; .type 32; .endef
___wine_stub_DIBRELEASE:
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
	movl $.L__wine_stub_DIBRELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBGETDATA; .scl 2; .type 32; .endef
___wine_stub_DIBGETDATA:
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
	movl $.L__wine_stub_DIBGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBSAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_DIBSAVETOSTREAM:
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
	movl $.L__wine_stub_DIBSAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBCLONE; .scl 2; .type 32; .endef
___wine_stub_DIBCLONE:
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
	movl $.L__wine_stub_DIBCLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBEQUAL; .scl 2; .type 32; .endef
___wine_stub_DIBEQUAL:
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
	movl $.L__wine_stub_DIBEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBCOPY; .scl 2; .type 32; .endef
___wine_stub_DIBCOPY:
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
	movl $.L__wine_stub_DIBCOPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_DIBQUERYBOUNDS:
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
	movl $.L__wine_stub_DIBQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBDRAW; .scl 2; .type 32; .endef
___wine_stub_DIBDRAW:
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
	movl $.L__wine_stub_DIBDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBENUMFORMAT; .scl 2; .type 32; .endef
___wine_stub_DIBENUMFORMAT:
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
	movl $.L__wine_stub_DIBENUMFORMAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DIBCHANGEDATA; .scl 2; .type 32; .endef
___wine_stub_DIBCHANGEDATA:
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
	movl $.L__wine_stub_DIBCHANGEDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENRELEASE; .scl 2; .type 32; .endef
___wine_stub_GENRELEASE:
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
	movl $.L__wine_stub_GENRELEASE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENGETDATA; .scl 2; .type 32; .endef
___wine_stub_GENGETDATA:
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
	movl $.L__wine_stub_GENGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENSETDATA; .scl 2; .type 32; .endef
___wine_stub_GENSETDATA:
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
	movl $.L__wine_stub_GENSETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENSAVETOSTREAM; .scl 2; .type 32; .endef
___wine_stub_GENSAVETOSTREAM:
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
	movl $.L__wine_stub_GENSAVETOSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENCLONE; .scl 2; .type 32; .endef
___wine_stub_GENCLONE:
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
	movl $.L__wine_stub_GENCLONE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENEQUAL; .scl 2; .type 32; .endef
___wine_stub_GENEQUAL:
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
	movl $.L__wine_stub_GENEQUAL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENCOPY; .scl 2; .type 32; .endef
___wine_stub_GENCOPY:
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
	movl $.L__wine_stub_GENCOPY_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_GENQUERYBOUNDS:
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
	movl $.L__wine_stub_GENQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENDRAW; .scl 2; .type 32; .endef
___wine_stub_GENDRAW:
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
	movl $.L__wine_stub_GENDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENENUMFORMAT; .scl 2; .type 32; .endef
___wine_stub_GENENUMFORMAT:
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
	movl $.L__wine_stub_GENENUMFORMAT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GENCHANGEDATA; .scl 2; .type 32; .endef
___wine_stub_GENCHANGEDATA:
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
	movl $.L__wine_stub_GENCHANGEDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSHOW; .scl 2; .type 32; .endef
___wine_stub_ERRSHOW:
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
	movl $.L__wine_stub_ERRSHOW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSETDATA; .scl 2; .type 32; .endef
___wine_stub_ERRSETDATA:
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
	movl $.L__wine_stub_ERRSETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSETHOSTNAMES; .scl 2; .type 32; .endef
___wine_stub_ERRSETHOSTNAMES:
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
	movl $.L__wine_stub_ERRSETHOSTNAMES_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSETTARGETDEVICE; .scl 2; .type 32; .endef
___wine_stub_ERRSETTARGETDEVICE:
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
	movl $.L__wine_stub_ERRSETTARGETDEVICE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSETBOUNDS; .scl 2; .type 32; .endef
___wine_stub_ERRSETBOUNDS:
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
	movl $.L__wine_stub_ERRSETBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRCOPYFROMLINK; .scl 2; .type 32; .endef
___wine_stub_ERRCOPYFROMLINK:
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
	movl $.L__wine_stub_ERRCOPYFROMLINK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRQUERYOPEN; .scl 2; .type 32; .endef
___wine_stub_ERRQUERYOPEN:
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
	movl $.L__wine_stub_ERRQUERYOPEN_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRACTIVATE; .scl 2; .type 32; .endef
___wine_stub_ERRACTIVATE:
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
	movl $.L__wine_stub_ERRACTIVATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRCLOSE; .scl 2; .type 32; .endef
___wine_stub_ERRCLOSE:
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
	movl $.L__wine_stub_ERRCLOSE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRUPDATE; .scl 2; .type 32; .endef
___wine_stub_ERRUPDATE:
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
	movl $.L__wine_stub_ERRUPDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRRECONNECT; .scl 2; .type 32; .endef
___wine_stub_ERRRECONNECT:
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
	movl $.L__wine_stub_ERRRECONNECT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRQUERYPROTOCOL; .scl 2; .type 32; .endef
___wine_stub_ERRQUERYPROTOCOL:
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
	movl $.L__wine_stub_ERRQUERYPROTOCOL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRQUERYOUTOFDATE; .scl 2; .type 32; .endef
___wine_stub_ERRQUERYOUTOFDATE:
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
	movl $.L__wine_stub_ERRQUERYOUTOFDATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERROBJECTCONVERT; .scl 2; .type 32; .endef
___wine_stub_ERROBJECTCONVERT:
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
	movl $.L__wine_stub_ERROBJECTCONVERT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRGETUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_ERRGETUPDATEOPTIONS:
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
	movl $.L__wine_stub_ERRGETUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERRSETUPDATEOPTIONS; .scl 2; .type 32; .endef
___wine_stub_ERRSETUPDATEOPTIONS:
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
	movl $.L__wine_stub_ERRSETUPDATEOPTIONS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERREXECUTE; .scl 2; .type 32; .endef
___wine_stub_ERREXECUTE:
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
	movl $.L__wine_stub_ERREXECUTE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ERROBJECTLONG; .scl 2; .type 32; .endef
___wine_stub_ERROBJECTLONG:
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
	movl $.L__wine_stub_ERROBJECTLONG_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBLOADFROMSTREAM; .scl 2; .type 32; .endef
___wine_stub_PBLOADFROMSTREAM:
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
	movl $.L__wine_stub_PBLOADFROMSTREAM_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATEFROMCLIP; .scl 2; .type 32; .endef
___wine_stub_PBCREATEFROMCLIP:
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
	movl $.L__wine_stub_PBCREATEFROMCLIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATELINKFROMCLIP; .scl 2; .type 32; .endef
___wine_stub_PBCREATELINKFROMCLIP:
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
	movl $.L__wine_stub_PBCREATELINKFROMCLIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATEFROMTEMPLATE; .scl 2; .type 32; .endef
___wine_stub_PBCREATEFROMTEMPLATE:
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
	movl $.L__wine_stub_PBCREATEFROMTEMPLATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATE; .scl 2; .type 32; .endef
___wine_stub_PBCREATE:
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
	movl $.L__wine_stub_PBCREATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBDRAW; .scl 2; .type 32; .endef
___wine_stub_PBDRAW:
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
	movl $.L__wine_stub_PBDRAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBQUERYBOUNDS; .scl 2; .type 32; .endef
___wine_stub_PBQUERYBOUNDS:
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
	movl $.L__wine_stub_PBQUERYBOUNDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCOPYTOCLIPBOARD; .scl 2; .type 32; .endef
___wine_stub_PBCOPYTOCLIPBOARD:
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
	movl $.L__wine_stub_PBCOPYTOCLIPBOARD_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATEFROMFILE; .scl 2; .type 32; .endef
___wine_stub_PBCREATEFROMFILE:
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
	movl $.L__wine_stub_PBCREATEFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATELINKFROMFILE; .scl 2; .type 32; .endef
___wine_stub_PBCREATELINKFROMFILE:
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
	movl $.L__wine_stub_PBCREATELINKFROMFILE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBENUMFORMATS; .scl 2; .type 32; .endef
___wine_stub_PBENUMFORMATS:
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
	movl $.L__wine_stub_PBENUMFORMATS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBGETDATA; .scl 2; .type 32; .endef
___wine_stub_PBGETDATA:
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
	movl $.L__wine_stub_PBGETDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PBCREATEINVISIBLE; .scl 2; .type 32; .endef
___wine_stub_PBCREATEINVISIBLE:
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
	movl $.L__wine_stub_PBCREATEINVISIBLE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OBJQUERYNAME; .scl 2; .type 32; .endef
___wine_stub_OBJQUERYNAME:
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
	movl $.L__wine_stub_OBJQUERYNAME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OBJRENAME; .scl 2; .type 32; .endef
___wine_stub_OBJRENAME:
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
	movl $.L__wine_stub_OBJRENAME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OBJQUERYTYPE; .scl 2; .type 32; .endef
___wine_stub_OBJQUERYTYPE:
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
	movl $.L__wine_stub_OBJQUERYTYPE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OBJQUERYSIZE; .scl 2; .type 32; .endef
___wine_stub_OBJQUERYSIZE:
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
	movl $.L__wine_stub_OBJQUERYSIZE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CONNECTDLGPROC; .scl 2; .type 32; .endef
___wine_stub_CONNECTDLGPROC:
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
	movl $.L__wine_stub_CONNECTDLGPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SETNETNAME; .scl 2; .type 32; .endef
___wine_stub_SETNETNAME:
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
	movl $.L__wine_stub_SETNETNAME_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CHECKNETDRIVE; .scl 2; .type 32; .endef
___wine_stub_CHECKNETDRIVE:
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
	movl $.L__wine_stub_CHECKNETDRIVE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SETNEXTNETDRIVE; .scl 2; .type 32; .endef
___wine_stub_SETNEXTNETDRIVE:
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
	movl $.L__wine_stub_SETNEXTNETDRIVE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GETTASKVISIBLEWINDOW; .scl 2; .type 32; .endef
___wine_stub_GETTASKVISIBLEWINDOW:
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
	movl $.L__wine_stub_GETTASKVISIBLEWINDOW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_OLEDELETE_string:
	.string "OLEDELETE"
.L__wine_stub_OLESAVETOSTREAM_string:
	.string "OLESAVETOSTREAM"
.L__wine_stub_OLELOADFROMSTREAM_string:
	.string "OLELOADFROMSTREAM"
.L__wine_stub_OLECLONE_string:
	.string "OLECLONE"
.L__wine_stub_OLECOPYFROMLINK_string:
	.string "OLECOPYFROMLINK"
.L__wine_stub_OLEEQUAL_string:
	.string "OLEEQUAL"
.L__wine_stub_OLECOPYTOCLIPBOARD_string:
	.string "OLECOPYTOCLIPBOARD"
.L__wine_stub_OLESETHOSTNAMES_string:
	.string "OLESETHOSTNAMES"
.L__wine_stub_OLESETTARGETDEVICE_string:
	.string "OLESETTARGETDEVICE"
.L__wine_stub_OLESETBOUNDS_string:
	.string "OLESETBOUNDS"
.L__wine_stub_OLEQUERYBOUNDS_string:
	.string "OLEQUERYBOUNDS"
.L__wine_stub_OLEDRAW_string:
	.string "OLEDRAW"
.L__wine_stub_OLEQUERYOPEN_string:
	.string "OLEQUERYOPEN"
.L__wine_stub_OLEACTIVATE_string:
	.string "OLEACTIVATE"
.L__wine_stub_OLEUPDATE_string:
	.string "OLEUPDATE"
.L__wine_stub_OLERECONNECT_string:
	.string "OLERECONNECT"
.L__wine_stub_OLEGETLINKUPDATEOPTIONS_string:
	.string "OLEGETLINKUPDATEOPTIONS"
.L__wine_stub_OLESETLINKUPDATEOPTIONS_string:
	.string "OLESETLINKUPDATEOPTIONS"
.L__wine_stub_OLEENUMFORMATS_string:
	.string "OLEENUMFORMATS"
.L__wine_stub_OLECLOSE_string:
	.string "OLECLOSE"
.L__wine_stub_OLEGETDATA_string:
	.string "OLEGETDATA"
.L__wine_stub_OLESETDATA_string:
	.string "OLESETDATA"
.L__wine_stub_OLEQUERYPROTOCOL_string:
	.string "OLEQUERYPROTOCOL"
.L__wine_stub_OLEQUERYOUTOFDATE_string:
	.string "OLEQUERYOUTOFDATE"
.L__wine_stub_OLEOBJECTCONVERT_string:
	.string "OLEOBJECTCONVERT"
.L__wine_stub_OLECREATEFROMTEMPLATE_string:
	.string "OLECREATEFROMTEMPLATE"
.L__wine_stub_OLECREATE_string:
	.string "OLECREATE"
.L__wine_stub_OLEQUERYRELEASESTATUS_string:
	.string "OLEQUERYRELEASESTATUS"
.L__wine_stub_OLEQUERYRELEASEERROR_string:
	.string "OLEQUERYRELEASEERROR"
.L__wine_stub_OLEQUERYRELEASEMETHOD_string:
	.string "OLEQUERYRELEASEMETHOD"
.L__wine_stub_OLECREATEFROMFILE_string:
	.string "OLECREATEFROMFILE"
.L__wine_stub_OLECREATELINKFROMFILE_string:
	.string "OLECREATELINKFROMFILE"
.L__wine_stub_OLERELEASE_string:
	.string "OLERELEASE"
.L__wine_stub_OLERENAME_string:
	.string "OLERENAME"
.L__wine_stub_OLEQUERYNAME_string:
	.string "OLEQUERYNAME"
.L__wine_stub_OLESETCOLORSCHEME_string:
	.string "OLESETCOLORSCHEME"
.L__wine_stub_OLEREQUESTDATA_string:
	.string "OLEREQUESTDATA"
.L__wine_stub_OLELOCKSERVER_string:
	.string "OLELOCKSERVER"
.L__wine_stub_OLEUNLOCKSERVER_string:
	.string "OLEUNLOCKSERVER"
.L__wine_stub_OLEQUERYSIZE_string:
	.string "OLEQUERYSIZE"
.L__wine_stub_OLEEXECUTE_string:
	.string "OLEEXECUTE"
.L__wine_stub_OLECREATEINVISIBLE_string:
	.string "OLECREATEINVISIBLE"
.L__wine_stub_OLEQUERYCLIENTVERSION_string:
	.string "OLEQUERYCLIENTVERSION"
.L__wine_stub_DOCWNDPROC_string:
	.string "DOCWNDPROC"
.L__wine_stub_SRVRWNDPROC_string:
	.string "SRVRWNDPROC"
.L__wine_stub_MFCALLBACKFUNC_string:
	.string "MFCALLBACKFUNC"
.L__wine_stub_DEFLOADFROMSTREAM_string:
	.string "DEFLOADFROMSTREAM"
.L__wine_stub_DEFCREATEFROMCLIP_string:
	.string "DEFCREATEFROMCLIP"
.L__wine_stub_DEFCREATELINKFROMCLIP_string:
	.string "DEFCREATELINKFROMCLIP"
.L__wine_stub_DEFCREATEFROMTEMPLATE_string:
	.string "DEFCREATEFROMTEMPLATE"
.L__wine_stub_DEFCREATE_string:
	.string "DEFCREATE"
.L__wine_stub_DEFCREATEFROMFILE_string:
	.string "DEFCREATEFROMFILE"
.L__wine_stub_DEFCREATELINKFROMFILE_string:
	.string "DEFCREATELINKFROMFILE"
.L__wine_stub_DEFCREATEINVISIBLE_string:
	.string "DEFCREATEINVISIBLE"
.L__wine_stub_LERELEASE_string:
	.string "LERELEASE"
.L__wine_stub_LESHOW_string:
	.string "LESHOW"
.L__wine_stub_LEGETDATA_string:
	.string "LEGETDATA"
.L__wine_stub_LESETDATA_string:
	.string "LESETDATA"
.L__wine_stub_LESETHOSTNAMES_string:
	.string "LESETHOSTNAMES"
.L__wine_stub_LESETTARGETDEVICE_string:
	.string "LESETTARGETDEVICE"
.L__wine_stub_LESETBOUNDS_string:
	.string "LESETBOUNDS"
.L__wine_stub_LESAVETOSTREAM_string:
	.string "LESAVETOSTREAM"
.L__wine_stub_LECLONE_string:
	.string "LECLONE"
.L__wine_stub_LECOPYFROMLINK_string:
	.string "LECOPYFROMLINK"
.L__wine_stub_LEEQUAL_string:
	.string "LEEQUAL"
.L__wine_stub_LECOPY_string:
	.string "LECOPY"
.L__wine_stub_LEQUERYTYPE_string:
	.string "LEQUERYTYPE"
.L__wine_stub_LEQUERYBOUNDS_string:
	.string "LEQUERYBOUNDS"
.L__wine_stub_LEDRAW_string:
	.string "LEDRAW"
.L__wine_stub_LEQUERYOPEN_string:
	.string "LEQUERYOPEN"
.L__wine_stub_LEACTIVATE_string:
	.string "LEACTIVATE"
.L__wine_stub_LEUPDATE_string:
	.string "LEUPDATE"
.L__wine_stub_LERECONNECT_string:
	.string "LERECONNECT"
.L__wine_stub_LEENUMFORMAT_string:
	.string "LEENUMFORMAT"
.L__wine_stub_LEQUERYPROTOCOL_string:
	.string "LEQUERYPROTOCOL"
.L__wine_stub_LEQUERYOUTOFDATE_string:
	.string "LEQUERYOUTOFDATE"
.L__wine_stub_LEOBJECTCONVERT_string:
	.string "LEOBJECTCONVERT"
.L__wine_stub_LECHANGEDATA_string:
	.string "LECHANGEDATA"
.L__wine_stub_LECLOSE_string:
	.string "LECLOSE"
.L__wine_stub_LEGETUPDATEOPTIONS_string:
	.string "LEGETUPDATEOPTIONS"
.L__wine_stub_LESETUPDATEOPTIONS_string:
	.string "LESETUPDATEOPTIONS"
.L__wine_stub_LEEXECUTE_string:
	.string "LEEXECUTE"
.L__wine_stub_LEOBJECTLONG_string:
	.string "LEOBJECTLONG"
.L__wine_stub_LECREATEINVISIBLE_string:
	.string "LECREATEINVISIBLE"
.L__wine_stub_MFRELEASE_string:
	.string "MFRELEASE"
.L__wine_stub_MFGETDATA_string:
	.string "MFGETDATA"
.L__wine_stub_MFSAVETOSTREAM_string:
	.string "MFSAVETOSTREAM"
.L__wine_stub_MFCLONE_string:
	.string "MFCLONE"
.L__wine_stub_MFEQUAL_string:
	.string "MFEQUAL"
.L__wine_stub_MFCOPY_string:
	.string "MFCOPY"
.L__wine_stub_MFQUERYBOUNDS_string:
	.string "MFQUERYBOUNDS"
.L__wine_stub_MFDRAW_string:
	.string "MFDRAW"
.L__wine_stub_MFENUMFORMAT_string:
	.string "MFENUMFORMAT"
.L__wine_stub_MFCHANGEDATA_string:
	.string "MFCHANGEDATA"
.L__wine_stub_BMRELEASE_string:
	.string "BMRELEASE"
.L__wine_stub_BMGETDATA_string:
	.string "BMGETDATA"
.L__wine_stub_BMSAVETOSTREAM_string:
	.string "BMSAVETOSTREAM"
.L__wine_stub_BMCLONE_string:
	.string "BMCLONE"
.L__wine_stub_BMEQUAL_string:
	.string "BMEQUAL"
.L__wine_stub_BMCOPY_string:
	.string "BMCOPY"
.L__wine_stub_BMQUERYBOUNDS_string:
	.string "BMQUERYBOUNDS"
.L__wine_stub_BMDRAW_string:
	.string "BMDRAW"
.L__wine_stub_BMENUMFORMAT_string:
	.string "BMENUMFORMAT"
.L__wine_stub_BMCHANGEDATA_string:
	.string "BMCHANGEDATA"
.L__wine_stub_DIBRELEASE_string:
	.string "DIBRELEASE"
.L__wine_stub_DIBGETDATA_string:
	.string "DIBGETDATA"
.L__wine_stub_DIBSAVETOSTREAM_string:
	.string "DIBSAVETOSTREAM"
.L__wine_stub_DIBCLONE_string:
	.string "DIBCLONE"
.L__wine_stub_DIBEQUAL_string:
	.string "DIBEQUAL"
.L__wine_stub_DIBCOPY_string:
	.string "DIBCOPY"
.L__wine_stub_DIBQUERYBOUNDS_string:
	.string "DIBQUERYBOUNDS"
.L__wine_stub_DIBDRAW_string:
	.string "DIBDRAW"
.L__wine_stub_DIBENUMFORMAT_string:
	.string "DIBENUMFORMAT"
.L__wine_stub_DIBCHANGEDATA_string:
	.string "DIBCHANGEDATA"
.L__wine_stub_GENRELEASE_string:
	.string "GENRELEASE"
.L__wine_stub_GENGETDATA_string:
	.string "GENGETDATA"
.L__wine_stub_GENSETDATA_string:
	.string "GENSETDATA"
.L__wine_stub_GENSAVETOSTREAM_string:
	.string "GENSAVETOSTREAM"
.L__wine_stub_GENCLONE_string:
	.string "GENCLONE"
.L__wine_stub_GENEQUAL_string:
	.string "GENEQUAL"
.L__wine_stub_GENCOPY_string:
	.string "GENCOPY"
.L__wine_stub_GENQUERYBOUNDS_string:
	.string "GENQUERYBOUNDS"
.L__wine_stub_GENDRAW_string:
	.string "GENDRAW"
.L__wine_stub_GENENUMFORMAT_string:
	.string "GENENUMFORMAT"
.L__wine_stub_GENCHANGEDATA_string:
	.string "GENCHANGEDATA"
.L__wine_stub_ERRSHOW_string:
	.string "ERRSHOW"
.L__wine_stub_ERRSETDATA_string:
	.string "ERRSETDATA"
.L__wine_stub_ERRSETHOSTNAMES_string:
	.string "ERRSETHOSTNAMES"
.L__wine_stub_ERRSETTARGETDEVICE_string:
	.string "ERRSETTARGETDEVICE"
.L__wine_stub_ERRSETBOUNDS_string:
	.string "ERRSETBOUNDS"
.L__wine_stub_ERRCOPYFROMLINK_string:
	.string "ERRCOPYFROMLINK"
.L__wine_stub_ERRQUERYOPEN_string:
	.string "ERRQUERYOPEN"
.L__wine_stub_ERRACTIVATE_string:
	.string "ERRACTIVATE"
.L__wine_stub_ERRCLOSE_string:
	.string "ERRCLOSE"
.L__wine_stub_ERRUPDATE_string:
	.string "ERRUPDATE"
.L__wine_stub_ERRRECONNECT_string:
	.string "ERRRECONNECT"
.L__wine_stub_ERRQUERYPROTOCOL_string:
	.string "ERRQUERYPROTOCOL"
.L__wine_stub_ERRQUERYOUTOFDATE_string:
	.string "ERRQUERYOUTOFDATE"
.L__wine_stub_ERROBJECTCONVERT_string:
	.string "ERROBJECTCONVERT"
.L__wine_stub_ERRGETUPDATEOPTIONS_string:
	.string "ERRGETUPDATEOPTIONS"
.L__wine_stub_ERRSETUPDATEOPTIONS_string:
	.string "ERRSETUPDATEOPTIONS"
.L__wine_stub_ERREXECUTE_string:
	.string "ERREXECUTE"
.L__wine_stub_ERROBJECTLONG_string:
	.string "ERROBJECTLONG"
.L__wine_stub_PBLOADFROMSTREAM_string:
	.string "PBLOADFROMSTREAM"
.L__wine_stub_PBCREATEFROMCLIP_string:
	.string "PBCREATEFROMCLIP"
.L__wine_stub_PBCREATELINKFROMCLIP_string:
	.string "PBCREATELINKFROMCLIP"
.L__wine_stub_PBCREATEFROMTEMPLATE_string:
	.string "PBCREATEFROMTEMPLATE"
.L__wine_stub_PBCREATE_string:
	.string "PBCREATE"
.L__wine_stub_PBDRAW_string:
	.string "PBDRAW"
.L__wine_stub_PBQUERYBOUNDS_string:
	.string "PBQUERYBOUNDS"
.L__wine_stub_PBCOPYTOCLIPBOARD_string:
	.string "PBCOPYTOCLIPBOARD"
.L__wine_stub_PBCREATEFROMFILE_string:
	.string "PBCREATEFROMFILE"
.L__wine_stub_PBCREATELINKFROMFILE_string:
	.string "PBCREATELINKFROMFILE"
.L__wine_stub_PBENUMFORMATS_string:
	.string "PBENUMFORMATS"
.L__wine_stub_PBGETDATA_string:
	.string "PBGETDATA"
.L__wine_stub_PBCREATEINVISIBLE_string:
	.string "PBCREATEINVISIBLE"
.L__wine_stub_OBJQUERYNAME_string:
	.string "OBJQUERYNAME"
.L__wine_stub_OBJRENAME_string:
	.string "OBJRENAME"
.L__wine_stub_OBJQUERYTYPE_string:
	.string "OBJQUERYTYPE"
.L__wine_stub_OBJQUERYSIZE_string:
	.string "OBJQUERYSIZE"
.L__wine_stub_CONNECTDLGPROC_string:
	.string "CONNECTDLGPROC"
.L__wine_stub_SETNETNAME_string:
	.string "SETNETNAME"
.L__wine_stub_CHECKNETDRIVE_string:
	.string "CHECKNETDRIVE"
.L__wine_stub_SETNEXTNETDRIVE_string:
	.string "SETNEXTNETDRIVE"
.L__wine_stub_GETTASKVISIBLEWINDOW_string:
	.string "GETTASKVISIBLEWINDOW"

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
	.long .L__wine_spec_exp_names+18-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+41-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "olecli.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "OLECLI"
__end:/*?*/
