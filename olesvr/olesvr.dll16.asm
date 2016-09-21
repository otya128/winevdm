/* File generated automatically from ..\olesvr\olesvr.dll16.spec; do not edit! */
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
	.string "..\olesvr\olesvr.dll16.spec"

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
	.byte 0x06,0x4f,0x4c,0x45,0x53,0x56,0x52 /* OLESVR */
	.short 0
	.byte 0x11,0x4f,0x4c,0x45,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x53,0x45,0x52,0x56,0x45,0x52 /* OleRegisterServer */
	.short 2
	.byte 0x0f,0x4f,0x4c,0x45,0x52,0x45,0x56,0x4f,0x4b,0x45,0x53,0x45,0x52,0x56,0x45,0x52 /* OleRevokeServer */
	.short 3
	.byte 0x0e,0x4f,0x4c,0x45,0x42,0x4c,0x4f,0x43,0x4b,0x53,0x45,0x52,0x56,0x45,0x52 /* OleBlockServer */
	.short 4
	.byte 0x10,0x4f,0x4c,0x45,0x55,0x4e,0x42,0x4c,0x4f,0x43,0x4b,0x53,0x45,0x52,0x56,0x45,0x52 /* OleUnblockServer */
	.short 5
	.byte 0x14,0x4f,0x4c,0x45,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x53,0x45,0x52,0x56,0x45,0x52,0x44,0x4f,0x43 /* OleRegisterServerDoc */
	.short 6
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x56,0x4f,0x4b,0x45,0x53,0x45,0x52,0x56,0x45,0x52,0x44,0x4f,0x43 /* OleRevokeServerDoc */
	.short 7
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x4e,0x41,0x4d,0x45,0x53,0x45,0x52,0x56,0x45,0x52,0x44,0x4f,0x43 /* OleRenameServerDoc */
	.short 8
	.byte 0x12,0x4f,0x4c,0x45,0x52,0x45,0x56,0x45,0x52,0x54,0x53,0x45,0x52,0x56,0x45,0x52,0x44,0x4f,0x43 /* OleRevertServerDoc */
	.short 9
	.byte 0x11,0x4f,0x4c,0x45,0x53,0x41,0x56,0x45,0x44,0x53,0x45,0x52,0x56,0x45,0x52,0x44,0x4f,0x43 /* OleSavedServerDoc */
	.short 10
	.byte 0x0f,0x4f,0x4c,0x45,0x52,0x45,0x56,0x4f,0x4b,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54 /* OLEREVOKEOBJECT */
	.short 11
	.byte 0x15,0x4f,0x4c,0x45,0x51,0x55,0x45,0x52,0x59,0x53,0x45,0x52,0x56,0x45,0x52,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e /* OLEQUERYSERVERVERSION */
	.short 12
	.byte 0x0b,0x53,0x52,0x56,0x52,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* SRVRWNDPROC */
	.short 21
	.byte 0x0a,0x44,0x4f,0x43,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* DOCWNDPROC */
	.short 22
	.byte 0x0b,0x49,0x54,0x45,0x4d,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* ITEMWNDPROC */
	.short 23
	.byte 0x0b,0x53,0x45,0x4e,0x44,0x44,0x41,0x54,0x41,0x4d,0x53,0x47 /* SENDDATAMSG */
	.short 24
	.byte 0x0b,0x46,0x49,0x4e,0x44,0x49,0x54,0x45,0x4d,0x57,0x4e,0x44 /* FINDITEMWND */
	.short 25
	.byte 0x0c,0x49,0x54,0x45,0x4d,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b /* ITEMCALLBACK */
	.short 26
	.byte 0x10,0x54,0x45,0x52,0x4d,0x49,0x4e,0x41,0x54,0x45,0x43,0x4c,0x49,0x45,0x4e,0x54,0x53 /* TERMINATECLIENTS */
	.short 27
	.byte 0x13,0x54,0x45,0x52,0x4d,0x49,0x4e,0x41,0x54,0x45,0x44,0x4f,0x43,0x43,0x4c,0x49,0x45,0x4e,0x54,0x53 /* TERMINATEDOCCLIENTS */
	.short 28
	.byte 0x10,0x44,0x45,0x4c,0x45,0x54,0x45,0x43,0x4c,0x49,0x45,0x4e,0x54,0x49,0x4e,0x46,0x4f /* DELETECLIENTINFO */
	.short 29
	.byte 0x0d,0x53,0x45,0x4e,0x44,0x52,0x45,0x4e,0x41,0x4d,0x45,0x4d,0x53,0x47 /* SENDRENAMEMSG */
	.short 30
	.byte 0x10,0x45,0x4e,0x55,0x4d,0x46,0x4f,0x52,0x54,0x45,0x52,0x4d,0x49,0x4e,0x41,0x54,0x45 /* ENUMFORTERMINATE */
	.short 31
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x01,0x00
	/* OLESVR.2 - OLESVR.12 */
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_OLESVR_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_12-.L__wine_spec_code_segment
	.byte 0x08,0x00
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_OLESVR_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_27-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_28-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_29-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_OLESVR_31-.L__wine_spec_code_segment
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
.L__wine_spec_callfrom16_p_long_lt:
	pushl $.L__wine_spec_call16_p_lp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x0000002b,0x00000000
.L__wine_spec_callfrom16_p_long_ltpp:
	pushl $.L__wine_spec_call16_p_lppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000092b,0x00000000
.L__wine_spec_callfrom16_p_long_tppww:
	pushl $.L__wine_spec_call16_p_pppww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x00001325,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_OLESVR_2:
	pushw %bp
	pushl $_OleRegisterServer16@20
	callw .L__wine_spec_callfrom16_p_long_tppww
.L__wine_OLESVR_3:
	pushw %bp
	pushl $_OleRevokeServer16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLESVR_4:
	pushw %bp
	pushl $_OleBlockServer16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLESVR_5:
	pushw %bp
	pushl $_OleUnblockServer16@8
	callw .L__wine_spec_callfrom16_p_long_lp
.L__wine_OLESVR_6:
	pushw %bp
	pushl $_OleRegisterServerDoc16@16
	callw .L__wine_spec_callfrom16_p_long_ltpp
.L__wine_OLESVR_7:
	pushw %bp
	pushl $_OleRevokeServerDoc16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLESVR_8:
	pushw %bp
	pushl $_OleRenameServerDoc16@8
	callw .L__wine_spec_callfrom16_p_long_lt
.L__wine_OLESVR_9:
	pushw %bp
	pushl $_OleRevertServerDoc16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLESVR_10:
	pushw %bp
	pushl $_OleSavedServerDoc16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_OLESVR_11:
	pushw %bp
	pushl $___wine_stub_OLEREVOKEOBJECT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_12:
	pushw %bp
	pushl $___wine_stub_OLEQUERYSERVERVERSION
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_21:
	pushw %bp
	pushl $___wine_stub_SRVRWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_22:
	pushw %bp
	pushl $___wine_stub_DOCWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_23:
	pushw %bp
	pushl $___wine_stub_ITEMWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_24:
	pushw %bp
	pushl $___wine_stub_SENDDATAMSG
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_25:
	pushw %bp
	pushl $___wine_stub_FINDITEMWND
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_26:
	pushw %bp
	pushl $___wine_stub_ITEMCALLBACK
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_27:
	pushw %bp
	pushl $___wine_stub_TERMINATECLIENTS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_28:
	pushw %bp
	pushl $___wine_stub_TERMINATEDOCCLIENTS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_29:
	pushw %bp
	pushl $___wine_stub_DELETECLIENTINFO
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_30:
	pushw %bp
	pushl $___wine_stub_SENDRENAMEMSG
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_OLESVR_31:
	pushw %bp
	pushl $___wine_stub_ENUMFORTERMINATE
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
	.def .L__wine_spec_call16_p_pppww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pppww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
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

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_OLEREVOKEOBJECT; .scl 2; .type 32; .endef
___wine_stub_OLEREVOKEOBJECT:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_OLEREVOKEOBJECT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLEQUERYSERVERVERSION; .scl 2; .type 32; .endef
___wine_stub_OLEQUERYSERVERVERSION:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_OLEQUERYSERVERVERSION_string,4(%esp)
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
	.def ___wine_stub_ITEMWNDPROC; .scl 2; .type 32; .endef
___wine_stub_ITEMWNDPROC:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ITEMWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SENDDATAMSG; .scl 2; .type 32; .endef
___wine_stub_SENDDATAMSG:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SENDDATAMSG_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FINDITEMWND; .scl 2; .type 32; .endef
___wine_stub_FINDITEMWND:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_FINDITEMWND_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ITEMCALLBACK; .scl 2; .type 32; .endef
___wine_stub_ITEMCALLBACK:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ITEMCALLBACK_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TERMINATECLIENTS; .scl 2; .type 32; .endef
___wine_stub_TERMINATECLIENTS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_TERMINATECLIENTS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TERMINATEDOCCLIENTS; .scl 2; .type 32; .endef
___wine_stub_TERMINATEDOCCLIENTS:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_TERMINATEDOCCLIENTS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DELETECLIENTINFO; .scl 2; .type 32; .endef
___wine_stub_DELETECLIENTINFO:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_DELETECLIENTINFO_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SENDRENAMEMSG; .scl 2; .type 32; .endef
___wine_stub_SENDRENAMEMSG:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_SENDRENAMEMSG_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ENUMFORTERMINATE; .scl 2; .type 32; .endef
___wine_stub_ENUMFORTERMINATE:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_ENUMFORTERMINATE_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_OLEREVOKEOBJECT_string:
	.string "OLEREVOKEOBJECT"
.L__wine_stub_OLEQUERYSERVERVERSION_string:
	.string "OLEQUERYSERVERVERSION"
.L__wine_stub_SRVRWNDPROC_string:
	.string "SRVRWNDPROC"
.L__wine_stub_DOCWNDPROC_string:
	.string "DOCWNDPROC"
.L__wine_stub_ITEMWNDPROC_string:
	.string "ITEMWNDPROC"
.L__wine_stub_SENDDATAMSG_string:
	.string "SENDDATAMSG"
.L__wine_stub_FINDITEMWND_string:
	.string "FINDITEMWND"
.L__wine_stub_ITEMCALLBACK_string:
	.string "ITEMCALLBACK"
.L__wine_stub_TERMINATECLIENTS_string:
	.string "TERMINATECLIENTS"
.L__wine_stub_TERMINATEDOCCLIENTS_string:
	.string "TERMINATEDOCCLIENTS"
.L__wine_stub_DELETECLIENTINFO_string:
	.string "DELETECLIENTINFO"
.L__wine_stub_SENDRENAMEMSG_string:
	.string "SENDRENAMEMSG"
.L__wine_stub_ENUMFORTERMINATE_string:
	.string "ENUMFORTERMINATE"

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
	.long .L__wine_spec_exp_names+28-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+51-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "..\olesvr\olesvr.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "OLESVR"
__end:/*?*/
