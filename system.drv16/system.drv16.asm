/* File generated automatically from system.drv16.spec; do not edit! */
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
	.string "system.drv16.spec"

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
	.byte 0x06,0x53,0x59,0x53,0x54,0x45,0x4d /* SYSTEM */
	.short 0
	.byte 0x0d,0x49,0x4e,0x51,0x55,0x49,0x52,0x45,0x53,0x59,0x53,0x54,0x45,0x4d /* InquireSystem */
	.short 1
	.byte 0x11,0x43,0x52,0x45,0x41,0x54,0x45,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52 /* CreateSystemTimer */
	.short 2
	.byte 0x0f,0x4b,0x49,0x4c,0x4c,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52 /* KillSystemTimer */
	.short 3
	.byte 0x12,0x45,0x4e,0x41,0x42,0x4c,0x45,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52,0x53 /* EnableSystemTimers */
	.short 4
	.byte 0x13,0x44,0x49,0x53,0x41,0x42,0x4c,0x45,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52,0x53 /* DisableSystemTimers */
	.short 5
	.byte 0x12,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x4d,0x53,0x45,0x43,0x43,0x4f,0x55,0x4e,0x54 /* GetSystemMSecCount */
	.short 6
	.byte 0x10,0x47,0x45,0x54,0x38,0x30,0x58,0x38,0x37,0x53,0x41,0x56,0x45,0x53,0x49,0x5a,0x45 /* Get80x87SaveSize */
	.short 7
	.byte 0x0e,0x53,0x41,0x56,0x45,0x38,0x30,0x58,0x38,0x37,0x53,0x54,0x41,0x54,0x45 /* Save80x87State */
	.short 8
	.byte 0x11,0x52,0x45,0x53,0x54,0x4f,0x52,0x45,0x38,0x30,0x58,0x38,0x37,0x53,0x54,0x41,0x54,0x45 /* Restore80x87State */
	.short 9
	.byte 0x0f,0x49,0x4e,0x51,0x55,0x49,0x52,0x45,0x4c,0x4f,0x4e,0x47,0x49,0x4e,0x54,0x53 /* INQUIRELONGINTS */
	.short 13
	.byte 0x08,0x41,0x32,0x30,0x5f,0x50,0x52,0x4f,0x43 /* A20_Proc */
	.short 20
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* SYSTEM.1 - SYSTEM.9 */
	.byte 0x09,0x01
	.byte 0x03
	.short .L__wine_SYSTEM_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SYSTEM_9-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* SYSTEM.13 - SYSTEM.13 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_SYSTEM_13-.L__wine_spec_code_segment
	.byte 0x06,0x00
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_SYSTEM_20-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
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
.L__wine_spec_callfrom16_p_word_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000004,0x00000000
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
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_SYSTEM_1:
	pushw %bp
	pushl $_InquireSystem16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_SYSTEM_2:
	pushw %bp
	pushl $_CreateSystemTimer16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_SYSTEM_3:
	pushw %bp
	pushl $_SYSTEM_KillSystemTimer@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_SYSTEM_4:
	pushw %bp
	pushl $_EnableSystemTimers16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SYSTEM_5:
	pushw %bp
	pushl $_DisableSystemTimers16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SYSTEM_6:
	pushw %bp
	pushl $_GetSystemMSecCount16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_SYSTEM_7:
	pushw %bp
	pushl $_Get80x87SaveSize16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SYSTEM_8:
	pushw %bp
	pushl $_Save80x87State16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_SYSTEM_9:
	pushw %bp
	pushl $_Restore80x87State16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_SYSTEM_13:
	pushw %bp
	pushl $___wine_stub_INQUIRELONGINTS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_SYSTEM_20:
	pushw %bp
	pushl $_A20_Proc16@4
	callw .L__wine_spec_callfrom16_p_word_w
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

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_INQUIRELONGINTS; .scl 2; .type 32; .endef
___wine_stub_INQUIRELONGINTS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_INQUIRELONGINTS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_INQUIRELONGINTS_string:
	.string "INQUIRELONGINTS"

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
	.string "system.drv16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "SYSTEM"
__end:/*?*/
