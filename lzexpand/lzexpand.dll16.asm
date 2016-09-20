/* File generated automatically from lzexpand.dll16.spec; do not edit! */
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
	.string "lzexpand.dll16.spec"

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
	.byte 0x08,0x4c,0x5a,0x45,0x58,0x50,0x41,0x4e,0x44 /* LZEXPAND */
	.short 0
	.byte 0x06,0x4c,0x5a,0x43,0x4f,0x50,0x59 /* LZCopy */
	.short 1
	.byte 0x0a,0x4c,0x5a,0x4f,0x50,0x45,0x4e,0x46,0x49,0x4c,0x45 /* LZOpenFile */
	.short 2
	.byte 0x06,0x4c,0x5a,0x49,0x4e,0x49,0x54 /* LZInit */
	.short 3
	.byte 0x06,0x4c,0x5a,0x53,0x45,0x45,0x4b /* LZSeek */
	.short 4
	.byte 0x06,0x4c,0x5a,0x52,0x45,0x41,0x44 /* LZRead */
	.short 5
	.byte 0x07,0x4c,0x5a,0x43,0x4c,0x4f,0x53,0x45 /* LZClose */
	.short 6
	.byte 0x07,0x4c,0x5a,0x53,0x54,0x41,0x52,0x54 /* LZStart */
	.short 7
	.byte 0x0a,0x43,0x4f,0x50,0x59,0x4c,0x5a,0x46,0x49,0x4c,0x45 /* CopyLZFile */
	.short 8
	.byte 0x06,0x4c,0x5a,0x44,0x4f,0x4e,0x45 /* LZDone */
	.short 9
	.byte 0x0f,0x47,0x45,0x54,0x45,0x58,0x50,0x41,0x4e,0x44,0x45,0x44,0x4e,0x41,0x4d,0x45 /* GetExpandedName */
	.short 10
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x0a,0x01
	.byte 0x03
	.short .L__wine_LZEXPAND_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_LZEXPAND_10-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_wlw:
	pushl $.L__wine_spec_call16_p_wlw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000059,0x00000000
.L__wine_spec_callfrom16_p_long_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_p_word_tp:
	pushl $.L__wine_spec_call16_p_pp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000025,0x00000000
.L__wine_spec_callfrom16_p_word_tpw:
	pushl $.L__wine_spec_call16_p_ppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000065,0x00000000
.L__wine_spec_callfrom16_p_word_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $2
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_word_wpw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000061,0x00000000
.L__wine_LZEXPAND_1:
	pushw %bp
	pushl $_LZCopy16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_LZEXPAND_2:
	pushw %bp
	pushl $_LZOpenFile16@12
	callw .L__wine_spec_callfrom16_p_word_tpw
.L__wine_LZEXPAND_3:
	pushw %bp
	pushl $_LZInit16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_LZEXPAND_4:
	pushw %bp
	pushl $_LZSeek16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_LZEXPAND_5:
	pushw %bp
	pushl $_LZRead16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_LZEXPAND_6:
	pushw %bp
	pushl $_LZClose16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_LZEXPAND_7:
	pushw %bp
	pushl $_LZStart16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_LZEXPAND_8:
	pushw %bp
	pushl $_CopyLZFile16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_LZEXPAND_9:
	pushw %bp
	pushl $_LZDone16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_LZEXPAND_10:
	pushw %bp
	pushl $_GetExpandedName16@8
	callw .L__wine_spec_callfrom16_p_word_tp
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
	.def .L__wine_spec_call16_p_pp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pp:
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
	.long .L__wine_spec_exp_names+20-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+43-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "lzexpand.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "LZEXPAND"
__end:/*?*/
