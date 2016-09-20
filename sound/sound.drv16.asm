/* File generated automatically from ..\sound\sound.drv16.spec; do not edit! */
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
	.string "..\sound\sound.drv16.spec"

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
	.byte 0x05,0x53,0x4f,0x55,0x4e,0x44 /* SOUND */
	.short 0
	.byte 0x09,0x4f,0x50,0x45,0x4e,0x53,0x4f,0x55,0x4e,0x44 /* OpenSound */
	.short 1
	.byte 0x0a,0x43,0x4c,0x4f,0x53,0x45,0x53,0x4f,0x55,0x4e,0x44 /* CloseSound */
	.short 2
	.byte 0x11,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x51,0x55,0x45,0x55,0x45,0x53,0x49,0x5a,0x45 /* SetVoiceQueueSize */
	.short 3
	.byte 0x0c,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x4e,0x4f,0x54,0x45 /* SetVoiceNote */
	.short 4
	.byte 0x0e,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x41,0x43,0x43,0x45,0x4e,0x54 /* SetVoiceAccent */
	.short 5
	.byte 0x10,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x45,0x4e,0x56,0x45,0x4c,0x4f,0x50,0x45 /* SetVoiceEnvelope */
	.short 6
	.byte 0x0d,0x53,0x45,0x54,0x53,0x4f,0x55,0x4e,0x44,0x4e,0x4f,0x49,0x53,0x45 /* SetSoundNoise */
	.short 7
	.byte 0x0d,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x53,0x4f,0x55,0x4e,0x44 /* SetVoiceSound */
	.short 8
	.byte 0x0a,0x53,0x54,0x41,0x52,0x54,0x53,0x4f,0x55,0x4e,0x44 /* StartSound */
	.short 9
	.byte 0x09,0x53,0x54,0x4f,0x50,0x53,0x4f,0x55,0x4e,0x44 /* StopSound */
	.short 10
	.byte 0x0e,0x57,0x41,0x49,0x54,0x53,0x4f,0x55,0x4e,0x44,0x53,0x54,0x41,0x54,0x45 /* WaitSoundState */
	.short 11
	.byte 0x0d,0x53,0x59,0x4e,0x43,0x41,0x4c,0x4c,0x56,0x4f,0x49,0x43,0x45,0x53 /* SyncAllVoices */
	.short 12
	.byte 0x0f,0x43,0x4f,0x55,0x4e,0x54,0x56,0x4f,0x49,0x43,0x45,0x4e,0x4f,0x54,0x45,0x53 /* CountVoiceNotes */
	.short 13
	.byte 0x11,0x47,0x45,0x54,0x54,0x48,0x52,0x45,0x53,0x48,0x4f,0x4c,0x44,0x45,0x56,0x45,0x4e,0x54 /* GetThresholdEvent */
	.short 14
	.byte 0x12,0x47,0x45,0x54,0x54,0x48,0x52,0x45,0x53,0x48,0x4f,0x4c,0x44,0x53,0x54,0x41,0x54,0x55,0x53 /* GetThresholdStatus */
	.short 15
	.byte 0x11,0x53,0x45,0x54,0x56,0x4f,0x49,0x43,0x45,0x54,0x48,0x52,0x45,0x53,0x48,0x4f,0x4c,0x44 /* SetVoiceThreshold */
	.short 16
	.byte 0x06,0x44,0x4f,0x42,0x45,0x45,0x50 /* DoBeep */
	.short 17
	.byte 0x0b,0x4d,0x59,0x4f,0x50,0x45,0x4e,0x53,0x4f,0x55,0x4e,0x44 /* MYOPENSOUND */
	.short 18
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x12,0x01
	.byte 0x03
	.short .L__wine_SOUND_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_7-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_8-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_9-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_13-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_SOUND_18-.L__wine_spec_code_segment
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
.L__wine_spec_callfrom16_p_word_wlw:
	pushl $.L__wine_spec_call16_p_wlw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000059,0x00000000
.L__wine_spec_callfrom16_p_word_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_word_www:
	pushl $.L__wine_spec_call16_p_www
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000049,0x00000000
.L__wine_spec_callfrom16_p_word_wwww:
	pushl $.L__wine_spec_call16_p_wwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwww:
	pushl $.L__wine_spec_call16_p_wwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001249,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_SOUND_1:
	pushw %bp
	pushl $_OpenSound16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_2:
	pushw %bp
	pushl $_CloseSound16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_3:
	pushw %bp
	pushl $_SetVoiceQueueSize16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_SOUND_4:
	pushw %bp
	pushl $_SetVoiceNote16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_SOUND_5:
	pushw %bp
	pushl $_SetVoiceAccent16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_SOUND_6:
	pushw %bp
	pushl $_SetVoiceEnvelope16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_SOUND_7:
	pushw %bp
	pushl $_SetSoundNoise16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_SOUND_8:
	pushw %bp
	pushl $_SetVoiceSound16@12
	callw .L__wine_spec_callfrom16_p_word_wlw
.L__wine_SOUND_9:
	pushw %bp
	pushl $_StartSound16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_10:
	pushw %bp
	pushl $_StopSound16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_11:
	pushw %bp
	pushl $_WaitSoundState16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_SOUND_12:
	pushw %bp
	pushl $_SyncAllVoices16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_13:
	pushw %bp
	pushl $_CountVoiceNotes16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_SOUND_14:
	pushw %bp
	pushl $_GetThresholdEvent16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_SOUND_15:
	pushw %bp
	pushl $_GetThresholdStatus16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_16:
	pushw %bp
	pushl $_SetVoiceThreshold16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_SOUND_17:
	pushw %bp
	pushl $_DoBeep16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_SOUND_18:
	pushw %bp
	pushl $___wine_stub_MYOPENSOUND
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
	.def .L__wine_spec_call16_p_wwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwww:
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
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwww:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
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
	call *8(%ebp)
	leave
	ret
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_MYOPENSOUND; .scl 2; .type 32; .endef
___wine_stub_MYOPENSOUND:
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
 	nop
	subl $12,%esp
	movl $.L__wine_stub_MYOPENSOUND_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_MYOPENSOUND_string:
	.string "MYOPENSOUND"

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
	.long .L__wine_spec_exp_names+26-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+49-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "..\sound\sound.drv16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "SOUND"
__end:/*?*/
