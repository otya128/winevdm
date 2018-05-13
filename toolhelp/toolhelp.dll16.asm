/* File generated automatically from toolhelp.dll16.spec; do not edit! */
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
	.string "toolhelp.dll16.spec"

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
	.byte 0x05,0x4d,0x4f,0x55,0x53,0x45 /* MOUSE */
	.short 0
	.byte 0x11,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x48,0x41,0x4e,0x44,0x4c,0x45,0x54,0x4f,0x53,0x45,0x4c /* GlobalHandleToSel */
	.short 50
	.byte 0x0b,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x46,0x49,0x52,0x53,0x54 /* GlobalFirst */
	.short 51
	.byte 0x0a,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x4e,0x45,0x58,0x54 /* GlobalNext */
	.short 52
	.byte 0x0a,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x49,0x4e,0x46,0x4f /* GlobalInfo */
	.short 53
	.byte 0x11,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x45,0x4e,0x54,0x52,0x59,0x48,0x41,0x4e,0x44,0x4c,0x45 /* GlobalEntryHandle */
	.short 54
	.byte 0x11,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x45,0x4e,0x54,0x52,0x59,0x4d,0x4f,0x44,0x55,0x4c,0x45 /* GlobalEntryModule */
	.short 55
	.byte 0x09,0x4c,0x4f,0x43,0x41,0x4c,0x49,0x4e,0x46,0x4f /* LocalInfo */
	.short 56
	.byte 0x0a,0x4c,0x4f,0x43,0x41,0x4c,0x46,0x49,0x52,0x53,0x54 /* LocalFirst */
	.short 57
	.byte 0x09,0x4c,0x4f,0x43,0x41,0x4c,0x4e,0x45,0x58,0x54 /* LocalNext */
	.short 58
	.byte 0x0b,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x46,0x49,0x52,0x53,0x54 /* ModuleFirst */
	.short 59
	.byte 0x0a,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x4e,0x45,0x58,0x54 /* ModuleNext */
	.short 60
	.byte 0x0e,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x46,0x49,0x4e,0x44,0x4e,0x41,0x4d,0x45 /* ModuleFindName */
	.short 61
	.byte 0x10,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x46,0x49,0x4e,0x44,0x48,0x41,0x4e,0x44,0x4c,0x45 /* ModuleFindHandle */
	.short 62
	.byte 0x09,0x54,0x41,0x53,0x4b,0x46,0x49,0x52,0x53,0x54 /* TaskFirst */
	.short 63
	.byte 0x08,0x54,0x41,0x53,0x4b,0x4e,0x45,0x58,0x54 /* TaskNext */
	.short 64
	.byte 0x0e,0x54,0x41,0x53,0x4b,0x46,0x49,0x4e,0x44,0x48,0x41,0x4e,0x44,0x4c,0x45 /* TaskFindHandle */
	.short 65
	.byte 0x0f,0x53,0x54,0x41,0x43,0x4b,0x54,0x52,0x41,0x43,0x45,0x46,0x49,0x52,0x53,0x54 /* StackTraceFirst */
	.short 66
	.byte 0x13,0x53,0x54,0x41,0x43,0x4b,0x54,0x52,0x41,0x43,0x45,0x43,0x53,0x49,0x50,0x46,0x49,0x52,0x53,0x54 /* StackTraceCSIPFirst */
	.short 67
	.byte 0x0e,0x53,0x54,0x41,0x43,0x4b,0x54,0x52,0x41,0x43,0x45,0x4e,0x45,0x58,0x54 /* StackTraceNext */
	.short 68
	.byte 0x0a,0x43,0x4c,0x41,0x53,0x53,0x46,0x49,0x52,0x53,0x54 /* ClassFirst */
	.short 69
	.byte 0x09,0x43,0x4c,0x41,0x53,0x53,0x4e,0x45,0x58,0x54 /* ClassNext */
	.short 70
	.byte 0x0e,0x53,0x59,0x53,0x54,0x45,0x4d,0x48,0x45,0x41,0x50,0x49,0x4e,0x46,0x4f /* SystemHeapInfo */
	.short 71
	.byte 0x0a,0x4d,0x45,0x4d,0x4d,0x41,0x4e,0x49,0x4e,0x46,0x4f /* MemManInfo */
	.short 72
	.byte 0x0e,0x4e,0x4f,0x54,0x49,0x46,0x59,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* NotifyRegister */
	.short 73
	.byte 0x10,0x4e,0x4f,0x54,0x49,0x46,0x59,0x55,0x4e,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* NotifyUnregister */
	.short 74
	.byte 0x11,0x49,0x4e,0x54,0x45,0x52,0x52,0x55,0x50,0x54,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* InterruptRegister */
	.short 75
	.byte 0x13,0x49,0x4e,0x54,0x45,0x52,0x52,0x55,0x50,0x54,0x55,0x4e,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52 /* InterruptUnRegister */
	.short 76
	.byte 0x0c,0x54,0x45,0x52,0x4d,0x49,0x4e,0x41,0x54,0x45,0x41,0x50,0x50 /* TerminateApp */
	.short 77
	.byte 0x0a,0x4d,0x45,0x4d,0x4f,0x52,0x59,0x52,0x45,0x41,0x44 /* MemoryRead */
	.short 78
	.byte 0x0b,0x4d,0x45,0x4d,0x4f,0x52,0x59,0x57,0x52,0x49,0x54,0x45 /* MemoryWrite */
	.short 79
	.byte 0x0a,0x54,0x49,0x4d,0x45,0x52,0x43,0x4f,0x55,0x4e,0x54 /* TimerCount */
	.short 80
	.byte 0x0b,0x54,0x41,0x53,0x4b,0x53,0x45,0x54,0x43,0x53,0x49,0x50 /* TASKSETCSIP */
	.short 81
	.byte 0x0b,0x54,0x41,0x53,0x4b,0x47,0x45,0x54,0x43,0x53,0x49,0x50 /* TASKGETCSIP */
	.short 82
	.byte 0x0a,0x54,0x41,0x53,0x4b,0x53,0x57,0x49,0x54,0x43,0x48 /* TASKSWITCH */
	.short 83
	.byte 0x0b,0x4c,0x4f,0x43,0x41,0x4c,0x33,0x32,0x49,0x4e,0x46,0x4f /* Local32Info */
	.short 84
	.byte 0x0c,0x4c,0x4f,0x43,0x41,0x4c,0x33,0x32,0x46,0x49,0x52,0x53,0x54 /* Local32First */
	.short 85
	.byte 0x0b,0x4c,0x4f,0x43,0x41,0x4c,0x33,0x32,0x4e,0x45,0x58,0x54 /* Local32Next */
	.short 86
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	.byte 0x31,0x00
	.byte 0x25,0x01
	.byte 0x03
	.short .L__wine_MOUSE_50-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_51-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_52-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_53-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_54-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_55-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_56-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_57-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_58-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_59-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_60-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_61-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_62-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_63-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_64-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_65-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_66-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_67-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_68-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_69-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_70-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_71-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_72-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_73-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_74-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_75-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_76-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_77-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_78-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_79-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_80-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_81-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_82-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_83-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_84-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_85-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MOUSE_86-.L__wine_spec_code_segment
	.byte 0x00
.L__wine_spec_ne_enttab_end:

	.align 2
.L__wine_spec_code_segment:
.L__wine_spec_callfrom16_p_long_wlpl:
	pushl $.L__wine_spec_call16_p_wlpl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00000719,0x00000000
.L__wine_spec_callfrom16_p_word_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_word_pp:
	pushl $.L__wine_spec_call16_p_pp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000024,0x00000000
.L__wine_spec_callfrom16_p_word_pw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000000c,0x00000000
.L__wine_spec_callfrom16_p_word_pww:
	pushl $.L__wine_spec_call16_p_pww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000004c,0x00000000
.L__wine_spec_callfrom16_p_word_pwwww:
	pushl $.L__wine_spec_call16_p_pwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000124c,0x00000000
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
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_MOUSE_50:
	pushw %bp
	pushl $_GlobalHandleToSel16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_MOUSE_51:
	pushw %bp
	pushl $_GlobalFirst16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_52:
	pushw %bp
	pushl $_GlobalNext16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_53:
	pushw %bp
	pushl $_GlobalInfo16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_54:
	pushw %bp
	pushl $_GlobalEntryHandle16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_55:
	pushw %bp
	pushl $_GlobalEntryModule16@12
	callw .L__wine_spec_callfrom16_p_word_pww
.L__wine_MOUSE_56:
	pushw %bp
	pushl $_LocalInfo16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_57:
	pushw %bp
	pushl $_LocalFirst16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_58:
	pushw %bp
	pushl $_LocalNext16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_59:
	pushw %bp
	pushl $_ModuleFirst16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_60:
	pushw %bp
	pushl $_ModuleNext16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_61:
	pushw %bp
	pushl $_ModuleFindName16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_MOUSE_62:
	pushw %bp
	pushl $_ModuleFindHandle16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_63:
	pushw %bp
	pushl $_TaskFirst16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_64:
	pushw %bp
	pushl $_TaskNext16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_65:
	pushw %bp
	pushl $_TaskFindHandle16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_66:
	pushw %bp
	pushl $_StackTraceFirst16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_67:
	pushw %bp
	pushl $_StackTraceCSIPFirst16@20
	callw .L__wine_spec_callfrom16_p_word_pwwww
.L__wine_MOUSE_68:
	pushw %bp
	pushl $_StackTraceNext16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_69:
	pushw %bp
	pushl $___wine_stub_ClassFirst
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MOUSE_70:
	pushw %bp
	pushl $___wine_stub_ClassNext
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MOUSE_71:
	pushw %bp
	pushl $_SystemHeapInfo16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_72:
	pushw %bp
	pushl $_MemManInfo16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_73:
	pushw %bp
	pushl $_NotifyRegister16@12
	callw .L__wine_spec_callfrom16_p_word_wlw
.L__wine_MOUSE_74:
	pushw %bp
	pushl $_NotifyUnregister16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_MOUSE_75:
	pushw %bp
	pushl $_InterruptRegister16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_MOUSE_76:
	pushw %bp
	pushl $_InterruptUnRegister16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_MOUSE_77:
	pushw %bp
	pushl $_TerminateApp16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_MOUSE_78:
	pushw %bp
	pushl $_MemoryRead16@16
	callw .L__wine_spec_callfrom16_p_long_wlpl
.L__wine_MOUSE_79:
	pushw %bp
	pushl $_MemoryWrite16@16
	callw .L__wine_spec_callfrom16_p_long_wlpl
.L__wine_MOUSE_80:
	pushw %bp
	pushl $_TimerCount16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_MOUSE_81:
	pushw %bp
	pushl $___wine_stub_TASKSETCSIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MOUSE_82:
	pushw %bp
	pushl $___wine_stub_TASKGETCSIP
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MOUSE_83:
	pushw %bp
	pushl $___wine_stub_TASKSWITCH
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MOUSE_84:
	pushw %bp
	pushl $_Local32Info16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_85:
	pushw %bp
	pushl $_Local32First16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_MOUSE_86:
	pushw %bp
	pushl $_Local32Next16@4
	callw .L__wine_spec_callfrom16_p_word_p
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
	.def .L__wine_spec_call16_p_pw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
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
	.def .L__wine_spec_call16_p_pwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwwww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wlpl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wlpl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 8(%ecx)
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
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
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_ClassFirst; .scl 2; .type 32; .endef
___wine_stub_ClassFirst:
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
	movl $.L__wine_stub_ClassFirst_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ClassNext; .scl 2; .type 32; .endef
___wine_stub_ClassNext:
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
	movl $.L__wine_stub_ClassNext_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TASKSETCSIP; .scl 2; .type 32; .endef
___wine_stub_TASKSETCSIP:
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
	movl $.L__wine_stub_TASKSETCSIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TASKGETCSIP; .scl 2; .type 32; .endef
___wine_stub_TASKGETCSIP:
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
	movl $.L__wine_stub_TASKGETCSIP_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TASKSWITCH; .scl 2; .type 32; .endef
___wine_stub_TASKSWITCH:
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
	movl $.L__wine_stub_TASKSWITCH_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_ClassFirst_string:
	.string "ClassFirst"
.L__wine_stub_ClassNext_string:
	.string "ClassNext"
.L__wine_stub_TASKSETCSIP_string:
	.string "TASKSETCSIP"
.L__wine_stub_TASKGETCSIP_string:
	.string "TASKGETCSIP"
.L__wine_stub_TASKSWITCH_string:
	.string "TASKSWITCH"

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
	.string "toolhelp.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "MOUSE"
__end:/*?*/
