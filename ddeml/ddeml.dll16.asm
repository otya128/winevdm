/* File generated automatically from ddeml.dll16.spec; do not edit! */
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
	.string "ddeml.dll16.spec"

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
	.byte 0x05,0x44,0x44,0x45,0x4d,0x4c /* DDEML */
	.short 0
	.byte 0x0d,0x44,0x44,0x45,0x49,0x4e,0x49,0x54,0x49,0x41,0x4c,0x49,0x5a,0x45 /* DdeInitialize */
	.short 2
	.byte 0x0f,0x44,0x44,0x45,0x55,0x4e,0x49,0x4e,0x49,0x54,0x49,0x41,0x4c,0x49,0x5a,0x45 /* DdeUninitialize */
	.short 3
	.byte 0x0e,0x44,0x44,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x4c,0x49,0x53,0x54 /* DdeConnectList */
	.short 4
	.byte 0x12,0x44,0x44,0x45,0x51,0x55,0x45,0x52,0x59,0x4e,0x45,0x58,0x54,0x53,0x45,0x52,0x56,0x45,0x52 /* DdeQueryNextServer */
	.short 5
	.byte 0x11,0x44,0x44,0x45,0x44,0x49,0x53,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x4c,0x49,0x53,0x54 /* DdeDisconnectList */
	.short 6
	.byte 0x0a,0x44,0x44,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* DdeConnect */
	.short 7
	.byte 0x0d,0x44,0x44,0x45,0x44,0x49,0x53,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* DdeDisconnect */
	.short 8
	.byte 0x10,0x44,0x44,0x45,0x51,0x55,0x45,0x52,0x59,0x43,0x4f,0x4e,0x56,0x49,0x4e,0x46,0x4f /* DdeQueryConvInfo */
	.short 9
	.byte 0x10,0x44,0x44,0x45,0x53,0x45,0x54,0x55,0x53,0x45,0x52,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeSetUserHandle */
	.short 10
	.byte 0x14,0x44,0x44,0x45,0x43,0x4c,0x49,0x45,0x4e,0x54,0x54,0x52,0x41,0x4e,0x53,0x41,0x43,0x54,0x49,0x4f,0x4e /* DdeClientTransaction */
	.short 11
	.byte 0x15,0x44,0x44,0x45,0x41,0x42,0x41,0x4e,0x44,0x4f,0x4e,0x54,0x52,0x41,0x4e,0x53,0x41,0x43,0x54,0x49,0x4f,0x4e /* DdeAbandonTransaction */
	.short 12
	.byte 0x0d,0x44,0x44,0x45,0x50,0x4f,0x53,0x54,0x41,0x44,0x56,0x49,0x53,0x45 /* DdePostAdvise */
	.short 13
	.byte 0x13,0x44,0x44,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x41,0x54,0x41,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeCreateDataHandle */
	.short 14
	.byte 0x0a,0x44,0x44,0x45,0x41,0x44,0x44,0x44,0x41,0x54,0x41 /* DdeAddData */
	.short 15
	.byte 0x0a,0x44,0x44,0x45,0x47,0x45,0x54,0x44,0x41,0x54,0x41 /* DdeGetData */
	.short 16
	.byte 0x0d,0x44,0x44,0x45,0x41,0x43,0x43,0x45,0x53,0x53,0x44,0x41,0x54,0x41 /* DdeAccessData */
	.short 17
	.byte 0x0f,0x44,0x44,0x45,0x55,0x4e,0x41,0x43,0x43,0x45,0x53,0x53,0x44,0x41,0x54,0x41 /* DdeUnaccessData */
	.short 18
	.byte 0x11,0x44,0x44,0x45,0x46,0x52,0x45,0x45,0x44,0x41,0x54,0x41,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeFreeDataHandle */
	.short 19
	.byte 0x0f,0x44,0x44,0x45,0x47,0x45,0x54,0x4c,0x41,0x53,0x54,0x45,0x52,0x52,0x4f,0x52 /* DdeGetLastError */
	.short 20
	.byte 0x15,0x44,0x44,0x45,0x43,0x52,0x45,0x41,0x54,0x45,0x53,0x54,0x52,0x49,0x4e,0x47,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeCreateStringHandle */
	.short 21
	.byte 0x13,0x44,0x44,0x45,0x46,0x52,0x45,0x45,0x53,0x54,0x52,0x49,0x4e,0x47,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeFreeStringHandle */
	.short 22
	.byte 0x0e,0x44,0x44,0x45,0x51,0x55,0x45,0x52,0x59,0x53,0x54,0x52,0x49,0x4e,0x47 /* DdeQueryString */
	.short 23
	.byte 0x13,0x44,0x44,0x45,0x4b,0x45,0x45,0x50,0x53,0x54,0x52,0x49,0x4e,0x47,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DdeKeepStringHandle */
	.short 24
	.byte 0x11,0x44,0x44,0x45,0x45,0x4e,0x41,0x42,0x4c,0x45,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b /* DdeEnableCallback */
	.short 26
	.byte 0x0e,0x44,0x44,0x45,0x4e,0x41,0x4d,0x45,0x53,0x45,0x52,0x56,0x49,0x43,0x45 /* DdeNameService */
	.short 27
	.byte 0x13,0x44,0x44,0x45,0x43,0x4d,0x50,0x53,0x54,0x52,0x49,0x4e,0x47,0x48,0x41,0x4e,0x44,0x4c,0x45,0x53 /* DdeCmpStringHandles */
	.short 36
	.byte 0x0c,0x44,0x44,0x45,0x52,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54 /* DdeReconnect */
	.short 37
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x01,0x00
	/* DDEML.2 - DDEML.24 */
	.byte 0x17,0x01
	.byte 0x03
	.short .L__wine_DDEML_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_13-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_24-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* DDEML.26 - DDEML.27 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_DDEML_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_27-.L__wine_spec_code_segment
	.byte 0x08,0x00
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_DDEML_36-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_DDEML_37-.L__wine_spec_code_segment
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
.L__wine_spec_callfrom16_p_long_ll:
	pushl $.L__wine_spec_call16_p_ll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x0000001b,0x00000000
.L__wine_spec_callfrom16_p_long_llllp:
	pushl $.L__wine_spec_call16_p_llllp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $20
	.short 0x86c7
	.long 0x000046db,0x00000000
.L__wine_spec_callfrom16_p_long_llllwwlp:
	pushl $.L__wine_spec_call16_p_llllwwlp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $28
	.short 0x86c7
	.long 0x008c96db,0x00000000
.L__wine_spec_callfrom16_p_long_lllp:
	pushl $.L__wine_spec_call16_p_lllp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000008db,0x00000000
.L__wine_spec_callfrom16_p_long_lllw:
	pushl $.L__wine_spec_call16_p_lllw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x000002db,0x00000000
.L__wine_spec_callfrom16_p_long_llpls:
	pushl $.L__wine_spec_call16_p_llpls
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $18
	.short 0x86c7
	.long 0x0000271b,0x00000000
.L__wine_spec_callfrom16_p_long_lp:
	pushl $.L__wine_spec_call16_p_lp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000023,0x00000000
.L__wine_spec_callfrom16_p_long_lpll:
	pushl $.L__wine_spec_call16_p_lpll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000006e3,0x00000000
.L__wine_spec_callfrom16_p_long_lplllww:
	pushl $.L__wine_spec_call16_p_lplllww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $24
	.short 0x86c7
	.long 0x0004b6e3,0x00000000
.L__wine_spec_callfrom16_p_long_lts:
	pushl $.L__wine_spec_call16_p_lps
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x000000ab,0x00000000
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
.L__wine_spec_callfrom16_p_word_lll:
	pushl $.L__wine_spec_call16_p_lll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000db,0x00000000
.L__wine_spec_callfrom16_p_word_llp:
	pushl $.L__wine_spec_call16_p_llp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000011b,0x00000000
.L__wine_spec_callfrom16_p_word_llw:
	pushl $.L__wine_spec_call16_p_llw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000005b,0x00000000
.L__wine_spec_callfrom16_p_word_plll:
	pushl $.L__wine_spec_call16_p_plll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000006dc,0x00000000
.L__wine_DDEML_2:
	pushw %bp
	pushl $_DdeInitialize16@16
	callw .L__wine_spec_callfrom16_p_word_plll
.L__wine_DDEML_3:
	pushw %bp
	pushl $_DdeUninitialize16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_4:
	pushw %bp
	pushl $_DdeConnectList16@20
	callw .L__wine_spec_callfrom16_p_long_llllp
.L__wine_DDEML_5:
	pushw %bp
	pushl $_DdeQueryNextServer16@8
	callw .L__wine_spec_callfrom16_p_long_ll
.L__wine_DDEML_6:
	pushw %bp
	pushl $_DdeDisconnectList16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_7:
	pushw %bp
	pushl $_DdeConnect16@16
	callw .L__wine_spec_callfrom16_p_long_lllp
.L__wine_DDEML_8:
	pushw %bp
	pushl $_DdeDisconnect16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_9:
	pushw %bp
	pushl $_DdeQueryConvInfo16@12
	callw .L__wine_spec_callfrom16_p_word_llp
.L__wine_DDEML_10:
	pushw %bp
	pushl $_DdeSetUserHandle16@12
	callw .L__wine_spec_callfrom16_p_word_lll
.L__wine_DDEML_11:
	pushw %bp
	pushl $_DdeClientTransaction16@32
	callw .L__wine_spec_callfrom16_p_long_llllwwlp
.L__wine_DDEML_12:
	pushw %bp
	pushl $_DdeAbandonTransaction16@12
	callw .L__wine_spec_callfrom16_p_word_lll
.L__wine_DDEML_13:
	pushw %bp
	pushl $_DdePostAdvise16@12
	callw .L__wine_spec_callfrom16_p_word_lll
.L__wine_DDEML_14:
	pushw %bp
	pushl $_DdeCreateDataHandle16@28
	callw .L__wine_spec_callfrom16_p_long_lplllww
.L__wine_DDEML_15:
	pushw %bp
	pushl $_DdeAddData16@16
	callw .L__wine_spec_callfrom16_p_long_lpll
.L__wine_DDEML_16:
	pushw %bp
	pushl $_DdeGetData16@16
	callw .L__wine_spec_callfrom16_p_long_lpll
.L__wine_DDEML_17:
	pushw %bp
	pushl $_DdeAccessData16@8
	callw .L__wine_spec_callfrom16_p_long_lp
.L__wine_DDEML_18:
	pushw %bp
	pushl $_DdeUnaccessData16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_19:
	pushw %bp
	pushl $_DdeFreeDataHandle16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_20:
	pushw %bp
	pushl $_DdeGetLastError16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_DDEML_21:
	pushw %bp
	pushl $_DdeCreateStringHandle16@12
	callw .L__wine_spec_callfrom16_p_long_lts
.L__wine_DDEML_22:
	pushw %bp
	pushl $_DdeFreeStringHandle16@8
	callw .L__wine_spec_callfrom16_p_word_ll
.L__wine_DDEML_23:
	pushw %bp
	pushl $_DdeQueryString16@20
	callw .L__wine_spec_callfrom16_p_long_llpls
.L__wine_DDEML_24:
	pushw %bp
	pushl $_DdeKeepStringHandle16@8
	callw .L__wine_spec_callfrom16_p_word_ll
.L__wine_DDEML_26:
	pushw %bp
	pushl $_DdeEnableCallback16@12
	callw .L__wine_spec_callfrom16_p_word_llw
.L__wine_DDEML_27:
	pushw %bp
	pushl $_DdeNameService16@16
	callw .L__wine_spec_callfrom16_p_long_lllw
.L__wine_DDEML_36:
	pushw %bp
	pushl $_DdeCmpStringHandles16@8
	callw .L__wine_spec_callfrom16_p_word_ll
.L__wine_DDEML_37:
	pushw %bp
	pushl $_DdeReconnect@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_spec_code_segment_end:

.L__wine_spec_data_segment:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.L__wine_spec_data_segment_end:
	.byte 0

/* relay functions */

	.text
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
	.def .L__wine_spec_call16_p_lll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lll:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_llllp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llllp:
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
	pushl 8(%ecx)
	pushl 12(%ecx)
	pushl 16(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_llllwwlp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llllwwlp:
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	pushl 12(%ecx)
	pushl 16(%ecx)
	pushl 20(%ecx)
	pushl 24(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lllp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lllp:
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
	pushl 8(%ecx)
	pushl 12(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
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
	.def .L__wine_spec_call16_p_llp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llp:
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
	pushl 8(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_llpls; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llpls:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 10(%ecx)
	pushl 14(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_llw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llw:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	pushl 6(%ecx)
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
	.def .L__wine_spec_call16_p_lpll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lpll:
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
	pushl 12(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lplllww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lplllww:
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
	pushl 8(%ecx)
	pushl 12(%ecx)
	movzwl 18(%ecx),%edx
	shr $3,%edx
	movzwl 16(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 20(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lps:
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
	pushl 6(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_plll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_plll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

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
	.long .L__wine_spec_exp_names+17-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+40-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "ddeml.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "DDEML"
__end:/*?*/
