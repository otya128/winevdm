/* File generated automatically from netapi.dll16.spec; do not edit! */
/* This file can be copied, modified and distributed without restriction. */


/* module data */

	.data
	.align 4
__wine_spec_dos_header:
	.globl __wine_spec_dos_header
.L__wine_spec_dos_header:
	.globl ___wine_spec_dos_header
.L___wine_spec_dos_header:
___wine_spec_dos_header:
	.short 0x5a4d
	.short 0x0040
	.short 0x0001
	.short (.L__wine_spec_ne_header-.L__wine_spec_dos_header)/16
	.short 0
	.short 0xffff
	.short 0
	.short 0x00b8
	.short 0
	.short 0
	.short 0
	.short 0
	.short .L__wine_spec_ne_header-.L__wine_spec_dos_header
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
	.short 0x0000
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
	.byte 0x06,0x4e,0x45,0x54,0x41,0x50,0x49 /* netapi */
	.short 0
	.byte 0x0f,0x4e,0x45,0x54,0x57,0x4b,0x53,0x54,0x41,0x47,0x45,0x54,0x49,0x4e,0x46,0x4f /* NetWkstaGetInfo */
	.short 249
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0xf8,0x00
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_netapi_249-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.code16
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_word_twpwp_249:
	pushl $.L__wine_spec_call16_p_pwpwp_249
	lcalll $0,$0
	orw %ax,%ax
	lretw $16
	nop
	.byte 0x8d,0xb6,0x00,0x00,0x00,0x00
	.short 0x86c7
	.long 0x0000430d,0x00000000
.L__wine_netapi_249:
	pushw %bp
	pushl $_NetWkstaGetInfo16@20
	callw .L__wine_spec_callfrom16_p_word_twpwp_249
.L__wine_spec_code_segment_end:

.L__wine_spec_data_segment:
	.byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
.L__wine_spec_data_segment_end:
	.byte 0

/* relay functions */

	.text
	.align 4
	.def .L__wine_spec_call16_p_pwpwp_249; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwpwp_249:
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
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	calll *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy
.code32

	.section .rodata
	.globl ___wine_spec_file_name
___wine_spec_file_name:
.L__wine_spec_file_name:
	.string "netapi.dll16"
	.text
___wine_spec_thunk_text_16:

	.globl ___wine_call16_start
___wine_call16_start:

	.align 4
	.def ___wine_call_from_16; .scl 2; .type 32; .endef
	.globl ___wine_call_from_16
___wine_call_from_16:
	pushw %gs
	pushw %fs
	pushw %es
	pushw %ds
	pushl %ebp
	pushl %ecx
	pushl %edx
	.byte 0x2e
	movl _CallTo16_DataSelector,%edx
	movw %dx, %ds
	movw %dx, %es
	movw _CallTo16_TebSelector, %fs
	.byte 0x64
	mov (472),%gs
	movw %ss, %dx
	andl $0xfff8, %edx
	shrl $1, %edx
	movl __imp__wine_ldt_copy, %ecx
	movl 0(%ecx,%edx), %edx
	movzwl %sp, %ebp
	leal -4(%ebp,%edx), %edx
	.byte 0x64
	movl (3728), %ebp
	pushl %ebp
	.byte 0x64
	movw %ss, (3730)
	.byte 0x64
	movw %sp, (3728)
	pushl %ds
	popl %ss
	movl %ebp, %esp
	addl $0x20,%ebp
	subl $8,%esp
	andl $~15,%esp
	addl $8,%esp
	leal 0x30(%edx),%eax
	pushl %eax
	pushl 0x26(%edx)
	call *0x20(%edx)
	.byte 0x64
	movw (3730), %ss
	.byte 0x64
	movzwl (3728), %esp
	.byte 0x64
	popl (3728)
	popl %edx
	popl %ecx
	popl %ebp
	popw %ds
	popw %es
	popw %fs
	popw %gs
	lret $12

	.align 4
	.def ___wine_call_from_16_regs; .scl 2; .type 32; .endef
	.globl ___wine_call_from_16_regs
___wine_call_from_16_regs:
	pushw %gs
	pushw %fs
	pushw %es
	pushw %ds
	pushl %ebp
	pushl %ecx
	pushl %edx
	pushfl
	.byte 0x2e
	movl _CallTo16_DataSelector,%edx
	movw %dx, %ds
	movw %dx, %es
	movw _CallTo16_TebSelector, %fs
	.byte 0x64
	mov (472),%gs
	movw %ss, %dx
	andl $0xfff8, %edx
	shrl $1, %edx
	movl __imp__wine_ldt_copy, %ecx
	movl 0(%ecx,%edx), %edx
	movzwl %sp, %ebp
	leal 0(%ebp,%edx), %edx
	popl %ecx
	.byte 0x64
	movl (3728), %ebp
	pushl %ebp
	.byte 0x64
	movw %ss, (3730)
	.byte 0x64
	movw %sp, (3728)
	pushl %ds
	popl %ss
	movl %ebp, %esp
	addl $0x20,%ebp
	subl $0x2cc,%esp
	movl %ecx,0xc0(%esp)
	movl %eax,0xb0(%esp)
	movl %ebx,0xa4(%esp)
	movl %esi,0xa0(%esp)
	movl %edi,0x9c(%esp)
	movl 0x0c(%edx),%eax
	movl %eax,0xb4(%esp)
	movl 0x08(%edx),%eax
	movl %eax,0xac(%esp)
	movl 0x04(%edx),%eax
	movl %eax,0xa8(%esp)
	movzwl 0x10(%edx),%eax
	movl %eax,0x98(%esp)
	movzwl 0x12(%edx),%eax
	movl %eax,0x94(%esp)
	movzwl 0x14(%edx),%eax
	movl %eax,0x90(%esp)
	movzwl 0x16(%edx),%eax
	movl %eax,0x8c(%esp)
	movzwl 0x2e(%edx),%eax
	movl %eax,0xbc(%esp)
	movzwl 0x2c(%edx),%eax
	movl %eax,0xb8(%esp)
	.byte 0x64
	movzwl (3730), %eax
	movl %eax,0xc8(%esp)
	.byte 0x64
	movzwl (3728), %eax
	addl $0x2c,%eax
	movl %eax,0xc4(%esp)
	movl %esp,%eax
	andl $~15,%esp
	subl $4,%esp
	pushl %eax
	leal 0x30(%edx),%eax
	pushl %eax
	pushl 0x26(%edx)
	call *0x20(%edx)
	leal -748(%ebp),%ebx
	.byte 0x64
	movw (3730), %ss
	.byte 0x64
	movzwl (3728), %esp
	.byte 0x64
	popl (3728)
	addw $0x14,%sp
	popl %eax
	popl %edx
	movw 0xc8(%ebx),%ss
	movl 0xc4(%ebx),%esp
	addl $4, %esp
	pushw 0xbc(%ebx)
	pushw 0xb8(%ebx)
	pushl %edx
	pushl %eax
	pushl 0xc0(%ebx)
	pushl 0x98(%ebx)
	pushl 0x94(%ebx)
	popl %es
	pushl 0x90(%ebx)
	popl %fs
	pushl 0x8c(%ebx)
	popl %gs
	movl 0xb4(%ebx),%ebp
	movl 0xa0(%ebx),%esi
	movl 0x9c(%ebx),%edi
	movl 0xb0(%ebx),%eax
	movl 0xa8(%ebx),%edx
	movl 0xac(%ebx),%ecx
	movl 0xa4(%ebx),%ebx
	popl %ds
	popfl
	lret

	.align 4
	.def ___wine_call_from_16_thunk; .scl 2; .type 32; .endef
	.globl ___wine_call_from_16_thunk
___wine_call_from_16_thunk:
	pushw %gs
	pushw %fs
	pushw %es
	pushw %ds
	pushl %ebp
	pushl %ecx
	pushl %edx
	.byte 0x2e
	movl _CallTo16_DataSelector,%edx
	movw %dx, %ds
	movw %dx, %es
	movw _CallTo16_TebSelector, %fs
	.byte 0x64
	mov (472),%gs
	movw %ss, %dx
	andl $0xfff8, %edx
	shrl $1, %edx
	movl __imp__wine_ldt_copy, %ecx
	movl 0(%ecx,%edx), %edx
	movzwl %sp, %ebp
	leal -4(%ebp,%edx), %edx
	.byte 0x64
	movl (3728), %ebp
	pushl %ebp
	.byte 0x64
	movw %ss, (3730)
	.byte 0x64
	movw %sp, (3728)
	pushl %ds
	popl %ss
	movl %ebp, %esp
	addl $0x20,%ebp
	leal 0x1a(%edx),%ebx
	leal -4(%esp), %ebp
	call *0x26(%edx)
	.byte 0x64
	movw (3730), %ss
	.byte 0x64
	movzwl (3728), %esp
	.byte 0x64
	popl (3728)
	addl $8, %esp
	popl %ebp
	popw %ds
	popw %es
	popw %fs
	popw %gs
	addl $20, %esp
	xorb %ch, %ch
	popl %ebx
	addw %cx, %sp
	push %ebx
	.byte 0x66
	lret

	.align 4
	.def ___wine_call_to_16; .scl 2; .type 32; .endef
	.globl ___wine_call_to_16
___wine_call_to_16:
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %esi
	pushl %edi
	.byte 0x64
	mov %gs,(472)
	.byte 0x64
	pushl (3728)
	pushl 16(%ebp)
	.byte 0x64
	pushl (0)
	.byte 0x64
	movl %esp,(0)
	pushl %cs
	call .L__wine_call_to_16
	.byte 0x64
	popl (0)
	addl $4, %esp
	.byte 0x64
	popl (3728)
	andl $0xffff,%eax
	shll $16,%edx
	orl %edx,%eax
	popl %edi
	popl %esi
	popl %ebx
	popl %ebp
	ret $12
.L__wine_call_to_16:
	movl %esp,%edx
	.byte 0x64
	movw (3730),%ss
	.byte 0x64
	movw (3728),%sp
	.byte 0x64
	movl %edx,(3728)
	movzwl %sp,%ebp
	leal 0x2a(%ebp),%ebp
	subw 0x2c(%edx), %sp
	pushl 0x28(%edx)
	pushw -22(%ebp)
	popw %fs
	pushw -20(%ebp)
	popw %gs
	movw %ss,%ax
	movw %ax,%ds
	movw %ax,%es
	.byte 0x66
	lret

	.align 4
	.def ___wine_call_to_16_regs; .scl 2; .type 32; .endef
	.globl ___wine_call_to_16_regs
___wine_call_to_16_regs:
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %esi
	pushl %edi
	.byte 0x64
	mov %gs,(472)
	.byte 0x64
	pushl (3728)
	pushl 16(%ebp)
	.byte 0x64
	pushl (0)
	.byte 0x64
	movl %esp,(0)
	pushl %cs
	call .L__wine_call_to_16_regs
	.byte 0x64
	popl (0)
	addl $4, %esp
	.byte 0x64
	popl (3728)
	movl 0x14(%esp),%edi
	movl %eax,0xb0(%edi)
	movl %ebx,0xa4(%edi)
	movl %ecx,0xac(%edi)
	movl %edx,0xa8(%edi)
	movl %ebp,0xb4(%edi)
	movl %esi,0xc4(%edi)
	popl %edi
	popl %esi
	popl %ebx
	popl %ebp
	ret $12
.L__wine_call_to_16_regs:
	movl %esp,%edx
	.byte 0x64
	movw (3730),%ss
	.byte 0x64
	movw (3728),%sp
	.byte 0x64
	movl %edx,(3728)
	movzwl %sp,%ebp
	leal 0x2a(%ebp),%ebp
	subw 0x2c(%edx), %sp
	movl 0x28(%edx),%edx
	pushw 0xbc(%edx)
	pushw 0xb8(%edx)
	pushw 0x98(%edx)
	pushl 0x94(%edx)
	popl %es
	pushl 0x90(%edx)
	popl %fs
	pushl 0x8c(%edx)
	popl %gs
	movl 0xb4(%edx),%ebp
	movl 0xa0(%edx),%esi
	movl 0x9c(%edx),%edi
	movl 0xb0(%edx),%eax
	movl 0xa4(%edx),%ebx
	movl 0xac(%edx),%ecx
	movl 0xa8(%edx),%edx
	popw %ds
	.byte 0x66
	lret

	.align 4
	.def ___wine_call_to_16_ret; .scl 2; .type 32; .endef
	.globl ___wine_call_to_16_ret
___wine_call_to_16_ret:
	movl %esp,%esi
	.byte 0x2e
	movl _CallTo16_DataSelector-___wine_call16_start,%edi
	movw %di,%ds
	movw %di,%es
	.byte 0x2e
	mov _CallTo16_TebSelector-___wine_call16_start,%fs
	.byte 0x64
	mov (472),%gs
	movw %di,%ss
	.byte 0x64
	movl (3728),%esp
	lret

	.align 4
	.def _CALL32_CBClient; .scl 2; .type 32; .endef
	.globl _CALL32_CBClient
_CALL32_CBClient:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 16(%ebp), %ebx
	leal -8(%esp), %eax
	movl %eax, -8(%ebx)
	movl 20(%ebp), %esi
	movl (%esi), %esi
	movl 8(%ebp), %eax
	movl 12(%ebp), %ebp
	pushl %cs
	call *%eax
	movl 32(%esp), %edi
	movl %esi, (%edi)
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret

	.align 4
	.def _CALL32_CBClient_Ret; .scl 2; .type 32; .endef
	.globl _CALL32_CBClient_Ret
_CALL32_CBClient_Ret:
	movzwl %sp, %ebx
	lssl %ss:-16(%ebx), %esp
	lret

	.align 4
	.def _CALL32_CBClientEx; .scl 2; .type 32; .endef
	.globl _CALL32_CBClientEx
_CALL32_CBClientEx:
	pushl %ebp
	movl %esp,%ebp
	pushl %edi
	pushl %esi
	pushl %ebx
	movl 16(%ebp), %ebx
	leal -8(%esp), %eax
	movl %eax, 12(%ebx)
	movl 20(%ebp), %esi
	movl (%esi), %esi
	movl 8(%ebp), %eax
	movl 12(%ebp), %ebp
	pushl %cs
	call *%eax
	movl 32(%esp), %edi
	movl %esi, (%edi)
	movl 36(%esp), %ebx
	movl %ebp, (%ebx)
	popl %ebx
	popl %esi
	popl %edi
	popl %ebp
	ret

	.align 4
	.def _CALL32_CBClientEx_Ret; .scl 2; .type 32; .endef
	.globl _CALL32_CBClientEx_Ret
_CALL32_CBClientEx_Ret:
	movzwl %bp, %ebx
	subw %bp, %sp
	movzwl %sp, %ebp
	lssl %ss:-12(%ebx), %esp
	lret
	.globl ___wine_call16_end
___wine_call16_end:

	.data
	.align 4
	.globl _CallTo16_DataSelector
_CallTo16_DataSelector:
	.long 0
	.globl _CallTo16_TebSelector
_CallTo16_TebSelector:
	.long 0
	.globl ___wine_call16_start_p
___wine_call16_start_p:
	.long ___wine_call16_start
	.globl ___wine_call16_end_p
___wine_call16_end_p:
	.long ___wine_call16_end
	.globl ___wine_call_to_16_ret_p
___wine_call_to_16_ret_p:
	.long ___wine_call_to_16_ret

	.section .rodata
.L__wine_spec_main_module:
	.string "netapi"
__end:/*?*/
