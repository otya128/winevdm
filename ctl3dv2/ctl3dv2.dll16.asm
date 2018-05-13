/* File generated automatically from ctl3dv2.dll16.spec; do not edit! */
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
	.string "ctl3dv2.dll16.spec"

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
	.byte 0x07,0x43,0x54,0x4c,0x33,0x44,0x56,0x32 /* CTL3DV2 */
	.short 0
	.byte 0x0b,0x43,0x54,0x4c,0x33,0x44,0x47,0x45,0x54,0x56,0x45,0x52 /* Ctl3dGetVer */
	.short 1
	.byte 0x10,0x43,0x54,0x4c,0x33,0x44,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x44,0x4c,0x47 /* Ctl3dSubclassDlg */
	.short 2
	.byte 0x10,0x43,0x54,0x4c,0x33,0x44,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x43,0x54,0x4c /* Ctl3dSubclassCtl */
	.short 3
	.byte 0x0d,0x43,0x54,0x4c,0x33,0x44,0x43,0x54,0x4c,0x43,0x4f,0x4c,0x4f,0x52 /* Ctl3dCtlColor */
	.short 4
	.byte 0x0c,0x43,0x54,0x4c,0x33,0x44,0x45,0x4e,0x41,0x42,0x4c,0x45,0x44 /* Ctl3dEnabled */
	.short 5
	.byte 0x10,0x43,0x54,0x4c,0x33,0x44,0x43,0x4f,0x4c,0x4f,0x52,0x43,0x48,0x41,0x4e,0x47,0x45 /* Ctl3dColorChange */
	.short 6
	.byte 0x0c,0x42,0x54,0x4e,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43,0x33,0x44 /* BtnWndProc3d */
	.short 7
	.byte 0x0d,0x45,0x44,0x49,0x54,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43,0x33,0x44 /* EditWndProc3d */
	.short 8
	.byte 0x0d,0x4c,0x49,0x53,0x54,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43,0x33,0x44 /* ListWndProc3d */
	.short 9
	.byte 0x0e,0x43,0x4f,0x4d,0x42,0x4f,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43,0x33,0x44 /* ComboWndProc3d */
	.short 10
	.byte 0x0f,0x53,0x54,0x41,0x54,0x49,0x43,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43,0x33,0x44 /* StaticWndProc3d */
	.short 11
	.byte 0x0d,0x43,0x54,0x4c,0x33,0x44,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* Ctl3dRegister */
	.short 12
	.byte 0x0f,0x43,0x54,0x4c,0x33,0x44,0x55,0x4e,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* Ctl3dUnregister */
	.short 13
	.byte 0x11,0x43,0x54,0x4c,0x33,0x44,0x41,0x55,0x54,0x4f,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53 /* Ctl3dAutoSubclass */
	.short 16
	.byte 0x0c,0x43,0x54,0x4c,0x33,0x44,0x44,0x4c,0x47,0x50,0x52,0x4f,0x43 /* Ctl3dDlgProc */
	.short 17
	.byte 0x0f,0x43,0x54,0x4c,0x33,0x44,0x43,0x54,0x4c,0x43,0x4f,0x4c,0x4f,0x52,0x45,0x58 /* Ctl3dCtlColorEx */
	.short 18
	.byte 0x0d,0x43,0x54,0x4c,0x33,0x44,0x53,0x45,0x54,0x53,0x54,0x59,0x4c,0x45 /* Ctl3dSetStyle */
	.short 19
	.byte 0x12,0x43,0x54,0x4c,0x33,0x44,0x44,0x4c,0x47,0x46,0x52,0x41,0x4d,0x45,0x50,0x41,0x49,0x4e,0x54 /* Ctl3dDlgFramePaint */
	.short 20
	.byte 0x12,0x43,0x54,0x4c,0x33,0x44,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x44,0x4c,0x47,0x45,0x58 /* Ctl3dSubclassDlgEx */
	.short 21
	.byte 0x11,0x43,0x54,0x4c,0x33,0x44,0x57,0x49,0x4e,0x49,0x4e,0x49,0x43,0x48,0x41,0x4e,0x47,0x45 /* Ctl3dWinIniChange */
	.short 22
	.byte 0x13,0x43,0x54,0x4c,0x33,0x44,0x49,0x53,0x41,0x55,0x54,0x4f,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53 /* Ctl3dIsAutoSubclass */
	.short 23
	.byte 0x13,0x43,0x54,0x4c,0x33,0x44,0x55,0x4e,0x41,0x55,0x54,0x4f,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53 /* Ctl3dUnAutoSubclass */
	.short 24
	.byte 0x12,0x43,0x54,0x4c,0x33,0x44,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x43,0x54,0x4c,0x45,0x58 /* Ctl3dSubclassCtlEx */
	.short 25
	.byte 0x12,0x43,0x54,0x4c,0x33,0x44,0x55,0x4e,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x43,0x54,0x4c /* Ctl3dUnsubclassCtl */
	.short 26
	.byte 0x13,0x43,0x54,0x4c,0x33,0x44,0x41,0x55,0x54,0x4f,0x53,0x55,0x42,0x43,0x4c,0x41,0x53,0x53,0x45,0x58 /* Ctl3dAutoSubclassEx */
	.short 27
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* CTL3DV2.1 - CTL3DV2.13 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_CTL3DV2_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_13-.L__wine_spec_code_segment
	.byte 0x02,0x00
	.byte 0x0c,0x01
	.byte 0x03
	.short .L__wine_CTL3DV2_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_CTL3DV2_27-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_wwwl:
	pushl $.L__wine_spec_call16_p_wwwl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000649,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
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
.L__wine_spec_callfrom16_p_word_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_word_wwl:
	pushl $.L__wine_spec_call16_p_wwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000c9,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_CTL3DV2_1:
	pushw %bp
	pushl $_Ctl3dGetVer16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_2:
	pushw %bp
	pushl $_Ctl3dSubclassDlg16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_CTL3DV2_3:
	pushw %bp
	pushl $_Ctl3dSubclassCtl16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_CTL3DV2_4:
	pushw %bp
	pushl $_Ctl3dCtlColor16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_CTL3DV2_5:
	pushw %bp
	pushl $_Ctl3dEnabled16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_6:
	pushw %bp
	pushl $_Ctl3dColorChange16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_7:
	pushw %bp
	pushl $_BtnWndProc3d16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_8:
	pushw %bp
	pushl $_EditWndProc3d16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_9:
	pushw %bp
	pushl $_ListWndProc3d16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_10:
	pushw %bp
	pushl $_ComboWndProc3d16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_11:
	pushw %bp
	pushl $_StaticWndProc3d16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_12:
	pushw %bp
	pushl $_Ctl3dRegister16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_CTL3DV2_13:
	pushw %bp
	pushl $_Ctl3dUnregister16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_CTL3DV2_16:
	pushw %bp
	pushl $_Ctl3dAutoSubclass16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_CTL3DV2_17:
	pushw %bp
	pushl $_Ctl3dDlgProc16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_18:
	pushw %bp
	pushl $_Ctl3dCtlColorEx16@12
	callw .L__wine_spec_callfrom16_p_word_wwl
.L__wine_CTL3DV2_19:
	pushw %bp
	pushl $___wine_stub_Ctl3dSetStyle
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_CTL3DV2_20:
	pushw %bp
	pushl $_Ctl3dDlgFramePaint16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_CTL3DV2_21:
	pushw %bp
	pushl $_Ctl3dSubclassDlgEx16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_CTL3DV2_22:
	pushw %bp
	pushl $_Ctl3dWinIniChange16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_23:
	pushw %bp
	pushl $_Ctl3dIsAutoSubclass16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_24:
	pushw %bp
	pushl $_Ctl3dUnAutoSubclass16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_CTL3DV2_25:
	pushw %bp
	pushl $_Ctl3dSubclassCtlEx16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_CTL3DV2_26:
	pushw %bp
	pushl $_Ctl3dUnsubclassCtl16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_CTL3DV2_27:
	pushw %bp
	pushl $_Ctl3dAutoSubclassEx16@8
	callw .L__wine_spec_callfrom16_p_word_wl
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
	.align 4
	.def .L__wine_spec_call16_p_wwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwl:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwl:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
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
	.def ___wine_stub_Ctl3dSetStyle; .scl 2; .type 32; .endef
___wine_stub_Ctl3dSetStyle:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_Ctl3dSetStyle_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_Ctl3dSetStyle_string:
	.string "Ctl3dSetStyle"

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
	.long .L__wine_spec_exp_names+19-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+42-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "ctl3dv2.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "CTL3DV2"
__end:/*?*/
