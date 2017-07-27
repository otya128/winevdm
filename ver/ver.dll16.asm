/* File generated automatically from ..\ver\ver.dll16.spec; do not edit! */
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
	.string "..\ver\ver.dll16.spec"

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
	.byte 0x03,0x56,0x45,0x52 /* VER */
	.short 0
	.byte 0x13,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45,0x53,0x49,0x5a,0x45 /* GetFileResourceSize */
	.short 2
	.byte 0x0f,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* GetFileResource */
	.short 3
	.byte 0x16,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e,0x49,0x4e,0x46,0x4f,0x53,0x49,0x5a,0x45 /* GetFileVersionInfoSize */
	.short 6
	.byte 0x12,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e,0x49,0x4e,0x46,0x4f /* GetFileVersionInfo */
	.short 7
	.byte 0x0b,0x56,0x45,0x52,0x46,0x49,0x4e,0x44,0x46,0x49,0x4c,0x45 /* VerFindFile */
	.short 8
	.byte 0x0e,0x56,0x45,0x52,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c,0x46,0x49,0x4c,0x45 /* VerInstallFile */
	.short 9
	.byte 0x0f,0x56,0x45,0x52,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x4e,0x41,0x4d,0x45 /* VerLanguageName */
	.short 10
	.byte 0x0d,0x56,0x45,0x52,0x51,0x55,0x45,0x52,0x59,0x56,0x41,0x4c,0x55,0x45 /* VerQueryValue */
	.short 11
	.byte 0x15,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e,0x49,0x4e,0x46,0x4f,0x52,0x41,0x57 /* GETFILEVERSIONINFORAW */
	.short 20
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x01,0x00
	/* VER.2 - VER.3 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_VER_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_3-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* VER.6 - VER.11 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_VER_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_VER_11-.L__wine_spec_code_segment
	.byte 0x08,0x00
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_VER_20-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_ltpp:
	pushl $.L__wine_spec_call16_p_lppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000092b,0x00000000
.L__wine_spec_callfrom16_p_long_tllp:
	pushl $.L__wine_spec_call16_p_pllp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000008dd,0x00000000
.L__wine_spec_callfrom16_p_long_tp:
	pushl $.L__wine_spec_call16_p_pp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000025,0x00000000
.L__wine_spec_callfrom16_p_long_tttllp:
	pushl $.L__wine_spec_call16_p_pppllp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $24
	.short 0x86c7
	.long 0x0002376d,0x00000000
.L__wine_spec_callfrom16_p_long_tttp:
	pushl $.L__wine_spec_call16_p_pppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000096d,0x00000000
.L__wine_spec_callfrom16_p_long_wpw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000061,0x00000000
.L__wine_spec_callfrom16_p_long_wtttpppp:
	pushl $.L__wine_spec_call16_p_wppppppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $30
	.short 0x86c7
	.long 0x00924b69,0x00000000
.L__wine_spec_callfrom16_p_long_wtttttpp:
	pushl $.L__wine_spec_call16_p_wppppppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $30
	.short 0x86c7
	.long 0x0092db69,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_VER_2:
	pushw %bp
	pushl $_GetFileResourceSize16@16
	callw .L__wine_spec_callfrom16_p_long_tttp
.L__wine_VER_3:
	pushw %bp
	pushl $_GetFileResource16@24
	callw .L__wine_spec_callfrom16_p_long_tttllp
.L__wine_VER_6:
	pushw %bp
	pushl $_GetFileVersionInfoSize16@8
	callw .L__wine_spec_callfrom16_p_long_tp
.L__wine_VER_7:
	pushw %bp
	pushl $_GetFileVersionInfo16@16
	callw .L__wine_spec_callfrom16_p_long_tllp
.L__wine_VER_8:
	pushw %bp
	pushl $_VerFindFile16@32
	callw .L__wine_spec_callfrom16_p_long_wtttpppp
.L__wine_VER_9:
	pushw %bp
	pushl $_VerInstallFile16@32
	callw .L__wine_spec_callfrom16_p_long_wtttttpp
.L__wine_VER_10:
	pushw %bp
	pushl $_VerLanguageName16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_VER_11:
	pushw %bp
	pushl $_VerQueryValue16@16
	callw .L__wine_spec_callfrom16_p_long_ltpp
.L__wine_VER_20:
	pushw %bp
	pushl $___wine_stub_GETFILEVERSIONINFORAW
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
	.def .L__wine_spec_call16_p_lppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lppp:
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
	pushl 12(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pllp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pllp:
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
	.def .L__wine_spec_call16_p_pppllp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pppllp:
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
	.def .L__wine_spec_call16_p_wppppppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppppppp:
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
	movzwl 26(%ecx),%edx
	shr $3,%edx
	movzwl 24(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 28(%ecx),%eax
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
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_GETFILEVERSIONINFORAW; .scl 2; .type 32; .endef
___wine_stub_GETFILEVERSIONINFORAW:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_GETFILEVERSIONINFORAW_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_GETFILEVERSIONINFORAW_string:
	.string "GETFILEVERSIONINFORAW"

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
	.long .L__wine_spec_exp_names+22-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+45-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "..\ver\ver.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "VER"
__end:/*?*/
