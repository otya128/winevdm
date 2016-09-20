/* File generated automatically from wing.dll16.spec; do not edit! */
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
	.string "wing.dll16.spec"

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
	.byte 0x04,0x57,0x49,0x4e,0x47 /* WING */
	.short 0
	.byte 0x0c,0x57,0x49,0x4e,0x47,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x43 /* WinGCreateDC */
	.short 1001
	.byte 0x16,0x57,0x49,0x4e,0x47,0x52,0x45,0x43,0x4f,0x4d,0x4d,0x45,0x4e,0x44,0x44,0x49,0x42,0x46,0x4f,0x52,0x4d,0x41,0x54 /* WinGRecommendDIBFormat */
	.short 1002
	.byte 0x10,0x57,0x49,0x4e,0x47,0x43,0x52,0x45,0x41,0x54,0x45,0x42,0x49,0x54,0x4d,0x41,0x50 /* WinGCreateBitmap */
	.short 1003
	.byte 0x11,0x57,0x49,0x4e,0x47,0x47,0x45,0x54,0x44,0x49,0x42,0x50,0x4f,0x49,0x4e,0x54,0x45,0x52 /* WinGGetDIBPointer */
	.short 1004
	.byte 0x14,0x57,0x49,0x4e,0x47,0x47,0x45,0x54,0x44,0x49,0x42,0x43,0x4f,0x4c,0x4f,0x52,0x54,0x41,0x42,0x4c,0x45 /* WinGGetDIBColorTable */
	.short 1005
	.byte 0x14,0x57,0x49,0x4e,0x47,0x53,0x45,0x54,0x44,0x49,0x42,0x43,0x4f,0x4c,0x4f,0x52,0x54,0x41,0x42,0x4c,0x45 /* WinGSetDIBColorTable */
	.short 1006
	.byte 0x19,0x57,0x49,0x4e,0x47,0x43,0x52,0x45,0x41,0x54,0x45,0x48,0x41,0x4c,0x46,0x54,0x4f,0x4e,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* WinGCreateHalfTonePalette */
	.short 1007
	.byte 0x17,0x57,0x49,0x4e,0x47,0x43,0x52,0x45,0x41,0x54,0x45,0x48,0x41,0x4c,0x46,0x54,0x4f,0x4e,0x45,0x42,0x52,0x55,0x53,0x48 /* WinGCreateHalfToneBrush */
	.short 1008
	.byte 0x0e,0x57,0x49,0x4e,0x47,0x53,0x54,0x52,0x45,0x54,0x43,0x48,0x42,0x4c,0x54 /* WinGStretchBlt */
	.short 1009
	.byte 0x0a,0x57,0x49,0x4e,0x47,0x42,0x49,0x54,0x42,0x4c,0x54 /* WinGBitBlt */
	.short 1010
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0xff,0x00
	.byte 0xff,0x00
	.byte 0xff,0x00
	.byte 0xeb,0x00
	.byte 0x0a,0x01
	.byte 0x03
	.short .L__wine_WING_1001-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1002-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1003-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1004-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1005-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1006-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1007-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1008-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1009-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_WING_1010-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_wp:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000021,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_p_word_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_word_wpp:
	pushl $.L__wine_spec_call16_p_wpp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000121,0x00000000
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
.L__wine_spec_callfrom16_p_word_wwwwwwww:
	pushl $.L__wine_spec_call16_p_wwwwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00249249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwwwwwww:
	pushl $.L__wine_spec_call16_p_wwwwwwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x09249249,0x00000000
.L__wine_WING_1001:
	pushw %bp
	pushl $_WinGCreateDC16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_WING_1002:
	pushw %bp
	pushl $_WinGRecommendDIBFormat16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_WING_1003:
	pushw %bp
	pushl $_WinGCreateBitmap16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_WING_1004:
	pushw %bp
	pushl $_WinGGetDIBPointer16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_WING_1005:
	pushw %bp
	pushl $_WinGGetDIBColorTable16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_WING_1006:
	pushw %bp
	pushl $_WinGSetDIBColorTable16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_WING_1007:
	pushw %bp
	pushl $_WinGCreateHalfTonePalette16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_WING_1008:
	pushw %bp
	pushl $_WinGCreateHalfToneBrush16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_WING_1009:
	pushw %bp
	pushl $_WinGStretchBlt16@40
	callw .L__wine_spec_callfrom16_p_word_wwwwwwwwww
.L__wine_WING_1010:
	pushw %bp
	pushl $_WinGBitBlt16@32
	callw .L__wine_spec_callfrom16_p_word_wwwwwwww
.L__wine_spec_code_segment_end:

.L__wine_spec_data_segment:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.L__wine_spec_data_segment_end:
	.byte 0

/* relay functions */

	.text
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
	.def .L__wine_spec_call16_p_wwwwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwwww:
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwwwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwwwwww:
	pushl %ebp
	movl %esp,%ebp
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
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
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
	.long .L__wine_spec_exp_names+16-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+39-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "wing.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "WING"
__end:/*?*/
