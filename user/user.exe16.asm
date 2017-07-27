/* File generated automatically from user.exe16.spec; do not edit! */
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
	.string "user.exe16.spec"

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
	.byte 0x04,0x55,0x53,0x45,0x52 /* USER */
	.short 0
	.byte 0x0a,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x42,0x4f,0x58 /* MessageBox */
	.short 1
	.byte 0x0e,0x4f,0x4c,0x44,0x45,0x58,0x49,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* OldExitWindows */
	.short 2
	.byte 0x0e,0x45,0x4e,0x41,0x42,0x4c,0x45,0x4f,0x45,0x4d,0x4c,0x41,0x59,0x45,0x52 /* EnableOEMLayer */
	.short 3
	.byte 0x0f,0x44,0x49,0x53,0x41,0x42,0x4c,0x45,0x4f,0x45,0x4d,0x4c,0x41,0x59,0x45,0x52 /* DisableOEMLayer */
	.short 4
	.byte 0x07,0x49,0x4e,0x49,0x54,0x41,0x50,0x50 /* InitApp */
	.short 5
	.byte 0x0f,0x50,0x4f,0x53,0x54,0x51,0x55,0x49,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* PostQuitMessage */
	.short 6
	.byte 0x0b,0x45,0x58,0x49,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* ExitWindows */
	.short 7
	.byte 0x08,0x53,0x45,0x54,0x54,0x49,0x4d,0x45,0x52 /* SetTimer */
	.short 10
	.byte 0x0e,0x53,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52 /* SetSystemTimer */
	.short 11
	.byte 0x09,0x4b,0x49,0x4c,0x4c,0x54,0x49,0x4d,0x45,0x52 /* KillTimer */
	.short 12
	.byte 0x0c,0x47,0x45,0x54,0x54,0x49,0x43,0x4b,0x43,0x4f,0x55,0x4e,0x54 /* GetTickCount */
	.short 13
	.byte 0x12,0x47,0x45,0x54,0x54,0x49,0x4d,0x45,0x52,0x52,0x45,0x53,0x4f,0x4c,0x55,0x54,0x49,0x4f,0x4e /* GetTimerResolution */
	.short 14
	.byte 0x0e,0x47,0x45,0x54,0x43,0x55,0x52,0x52,0x45,0x4e,0x54,0x54,0x49,0x4d,0x45 /* GetCurrentTime */
	.short 15
	.byte 0x0a,0x43,0x4c,0x49,0x50,0x43,0x55,0x52,0x53,0x4f,0x52 /* ClipCursor */
	.short 16
	.byte 0x0c,0x47,0x45,0x54,0x43,0x55,0x52,0x53,0x4f,0x52,0x50,0x4f,0x53 /* GetCursorPos */
	.short 17
	.byte 0x0a,0x53,0x45,0x54,0x43,0x41,0x50,0x54,0x55,0x52,0x45 /* SetCapture */
	.short 18
	.byte 0x0e,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x43,0x41,0x50,0x54,0x55,0x52,0x45 /* ReleaseCapture */
	.short 19
	.byte 0x12,0x53,0x45,0x54,0x44,0x4f,0x55,0x42,0x4c,0x45,0x43,0x4c,0x49,0x43,0x4b,0x54,0x49,0x4d,0x45 /* SetDoubleClickTime */
	.short 20
	.byte 0x12,0x47,0x45,0x54,0x44,0x4f,0x55,0x42,0x4c,0x45,0x43,0x4c,0x49,0x43,0x4b,0x54,0x49,0x4d,0x45 /* GetDoubleClickTime */
	.short 21
	.byte 0x08,0x53,0x45,0x54,0x46,0x4f,0x43,0x55,0x53 /* SetFocus */
	.short 22
	.byte 0x08,0x47,0x45,0x54,0x46,0x4f,0x43,0x55,0x53 /* GetFocus */
	.short 23
	.byte 0x0a,0x52,0x45,0x4d,0x4f,0x56,0x45,0x50,0x52,0x4f,0x50 /* RemoveProp */
	.short 24
	.byte 0x07,0x47,0x45,0x54,0x50,0x52,0x4f,0x50 /* GetProp */
	.short 25
	.byte 0x07,0x53,0x45,0x54,0x50,0x52,0x4f,0x50 /* SetProp */
	.short 26
	.byte 0x09,0x45,0x4e,0x55,0x4d,0x50,0x52,0x4f,0x50,0x53 /* EnumProps */
	.short 27
	.byte 0x0e,0x43,0x4c,0x49,0x45,0x4e,0x54,0x54,0x4f,0x53,0x43,0x52,0x45,0x45,0x4e /* ClientToScreen */
	.short 28
	.byte 0x0e,0x53,0x43,0x52,0x45,0x45,0x4e,0x54,0x4f,0x43,0x4c,0x49,0x45,0x4e,0x54 /* ScreenToClient */
	.short 29
	.byte 0x0f,0x57,0x49,0x4e,0x44,0x4f,0x57,0x46,0x52,0x4f,0x4d,0x50,0x4f,0x49,0x4e,0x54 /* WindowFromPoint */
	.short 30
	.byte 0x08,0x49,0x53,0x49,0x43,0x4f,0x4e,0x49,0x43 /* IsIconic */
	.short 31
	.byte 0x0d,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x52,0x45,0x43,0x54 /* GetWindowRect */
	.short 32
	.byte 0x0d,0x47,0x45,0x54,0x43,0x4c,0x49,0x45,0x4e,0x54,0x52,0x45,0x43,0x54 /* GetClientRect */
	.short 33
	.byte 0x0c,0x45,0x4e,0x41,0x42,0x4c,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* EnableWindow */
	.short 34
	.byte 0x0f,0x49,0x53,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x4e,0x41,0x42,0x4c,0x45,0x44 /* IsWindowEnabled */
	.short 35
	.byte 0x0d,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x45,0x58,0x54 /* GetWindowText */
	.short 36
	.byte 0x0d,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x45,0x58,0x54 /* SetWindowText */
	.short 37
	.byte 0x13,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x45,0x58,0x54,0x4c,0x45,0x4e,0x47,0x54,0x48 /* GetWindowTextLength */
	.short 38
	.byte 0x0a,0x42,0x45,0x47,0x49,0x4e,0x50,0x41,0x49,0x4e,0x54 /* BeginPaint */
	.short 39
	.byte 0x08,0x45,0x4e,0x44,0x50,0x41,0x49,0x4e,0x54 /* EndPaint */
	.short 40
	.byte 0x0c,0x43,0x52,0x45,0x41,0x54,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* CreateWindow */
	.short 41
	.byte 0x0a,0x53,0x48,0x4f,0x57,0x57,0x49,0x4e,0x44,0x4f,0x57 /* ShowWindow */
	.short 42
	.byte 0x0b,0x43,0x4c,0x4f,0x53,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* CloseWindow */
	.short 43
	.byte 0x08,0x4f,0x50,0x45,0x4e,0x49,0x43,0x4f,0x4e /* OpenIcon */
	.short 44
	.byte 0x10,0x42,0x52,0x49,0x4e,0x47,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x4f,0x54,0x4f,0x50 /* BringWindowToTop */
	.short 45
	.byte 0x09,0x47,0x45,0x54,0x50,0x41,0x52,0x45,0x4e,0x54 /* GetParent */
	.short 46
	.byte 0x08,0x49,0x53,0x57,0x49,0x4e,0x44,0x4f,0x57 /* IsWindow */
	.short 47
	.byte 0x07,0x49,0x53,0x43,0x48,0x49,0x4c,0x44 /* IsChild */
	.short 48
	.byte 0x0f,0x49,0x53,0x57,0x49,0x4e,0x44,0x4f,0x57,0x56,0x49,0x53,0x49,0x42,0x4c,0x45 /* IsWindowVisible */
	.short 49
	.byte 0x0a,0x46,0x49,0x4e,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57 /* FindWindow */
	.short 50
	.byte 0x06,0x42,0x45,0x41,0x52,0x35,0x31 /* BEAR51 */
	.short 51
	.byte 0x08,0x41,0x4e,0x59,0x50,0x4f,0x50,0x55,0x50 /* AnyPopup */
	.short 52
	.byte 0x0d,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x57,0x49,0x4e,0x44,0x4f,0x57 /* DestroyWindow */
	.short 53
	.byte 0x0b,0x45,0x4e,0x55,0x4d,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* EnumWindows */
	.short 54
	.byte 0x10,0x45,0x4e,0x55,0x4d,0x43,0x48,0x49,0x4c,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* EnumChildWindows */
	.short 55
	.byte 0x0a,0x4d,0x4f,0x56,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* MoveWindow */
	.short 56
	.byte 0x0d,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x43,0x4c,0x41,0x53,0x53 /* RegisterClass */
	.short 57
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x4e,0x41,0x4d,0x45 /* GetClassName */
	.short 58
	.byte 0x0f,0x53,0x45,0x54,0x41,0x43,0x54,0x49,0x56,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* SetActiveWindow */
	.short 59
	.byte 0x0f,0x47,0x45,0x54,0x41,0x43,0x54,0x49,0x56,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetActiveWindow */
	.short 60
	.byte 0x0c,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57 /* ScrollWindow */
	.short 61
	.byte 0x0c,0x53,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x50,0x4f,0x53 /* SetScrollPos */
	.short 62
	.byte 0x0c,0x47,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x50,0x4f,0x53 /* GetScrollPos */
	.short 63
	.byte 0x0e,0x53,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x52,0x41,0x4e,0x47,0x45 /* SetScrollRange */
	.short 64
	.byte 0x0e,0x47,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x52,0x41,0x4e,0x47,0x45 /* GetScrollRange */
	.short 65
	.byte 0x05,0x47,0x45,0x54,0x44,0x43 /* GetDC */
	.short 66
	.byte 0x0b,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x44,0x43 /* GetWindowDC */
	.short 67
	.byte 0x09,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x44,0x43 /* ReleaseDC */
	.short 68
	.byte 0x09,0x53,0x45,0x54,0x43,0x55,0x52,0x53,0x4f,0x52 /* SetCursor */
	.short 69
	.byte 0x0c,0x53,0x45,0x54,0x43,0x55,0x52,0x53,0x4f,0x52,0x50,0x4f,0x53 /* SetCursorPos */
	.short 70
	.byte 0x0a,0x53,0x48,0x4f,0x57,0x43,0x55,0x52,0x53,0x4f,0x52 /* ShowCursor */
	.short 71
	.byte 0x07,0x53,0x45,0x54,0x52,0x45,0x43,0x54 /* SetRect */
	.short 72
	.byte 0x0c,0x53,0x45,0x54,0x52,0x45,0x43,0x54,0x45,0x4d,0x50,0x54,0x59 /* SetRectEmpty */
	.short 73
	.byte 0x08,0x43,0x4f,0x50,0x59,0x52,0x45,0x43,0x54 /* CopyRect */
	.short 74
	.byte 0x0b,0x49,0x53,0x52,0x45,0x43,0x54,0x45,0x4d,0x50,0x54,0x59 /* IsRectEmpty */
	.short 75
	.byte 0x08,0x50,0x54,0x49,0x4e,0x52,0x45,0x43,0x54 /* PtInRect */
	.short 76
	.byte 0x0a,0x4f,0x46,0x46,0x53,0x45,0x54,0x52,0x45,0x43,0x54 /* OffsetRect */
	.short 77
	.byte 0x0b,0x49,0x4e,0x46,0x4c,0x41,0x54,0x45,0x52,0x45,0x43,0x54 /* InflateRect */
	.short 78
	.byte 0x0d,0x49,0x4e,0x54,0x45,0x52,0x53,0x45,0x43,0x54,0x52,0x45,0x43,0x54 /* IntersectRect */
	.short 79
	.byte 0x09,0x55,0x4e,0x49,0x4f,0x4e,0x52,0x45,0x43,0x54 /* UnionRect */
	.short 80
	.byte 0x08,0x46,0x49,0x4c,0x4c,0x52,0x45,0x43,0x54 /* FillRect */
	.short 81
	.byte 0x0a,0x49,0x4e,0x56,0x45,0x52,0x54,0x52,0x45,0x43,0x54 /* InvertRect */
	.short 82
	.byte 0x09,0x46,0x52,0x41,0x4d,0x45,0x52,0x45,0x43,0x54 /* FrameRect */
	.short 83
	.byte 0x08,0x44,0x52,0x41,0x57,0x49,0x43,0x4f,0x4e /* DrawIcon */
	.short 84
	.byte 0x08,0x44,0x52,0x41,0x57,0x54,0x45,0x58,0x54 /* DrawText */
	.short 85
	.byte 0x08,0x49,0x43,0x4f,0x4e,0x53,0x49,0x5a,0x45 /* IconSize */
	.short 86
	.byte 0x09,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x4f,0x58 /* DialogBox */
	.short 87
	.byte 0x09,0x45,0x4e,0x44,0x44,0x49,0x41,0x4c,0x4f,0x47 /* EndDialog */
	.short 88
	.byte 0x0c,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47 /* CreateDialog */
	.short 89
	.byte 0x0f,0x49,0x53,0x44,0x49,0x41,0x4c,0x4f,0x47,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* IsDialogMessage */
	.short 90
	.byte 0x0a,0x47,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d /* GetDlgItem */
	.short 91
	.byte 0x0e,0x53,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x54,0x45,0x58,0x54 /* SetDlgItemText */
	.short 92
	.byte 0x0e,0x47,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x54,0x45,0x58,0x54 /* GetDlgItemText */
	.short 93
	.byte 0x0d,0x53,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x49,0x4e,0x54 /* SetDlgItemInt */
	.short 94
	.byte 0x0d,0x47,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x49,0x4e,0x54 /* GetDlgItemInt */
	.short 95
	.byte 0x10,0x43,0x48,0x45,0x43,0x4b,0x52,0x41,0x44,0x49,0x4f,0x42,0x55,0x54,0x54,0x4f,0x4e /* CheckRadioButton */
	.short 96
	.byte 0x0e,0x43,0x48,0x45,0x43,0x4b,0x44,0x4c,0x47,0x42,0x55,0x54,0x54,0x4f,0x4e /* CheckDlgButton */
	.short 97
	.byte 0x12,0x49,0x53,0x44,0x4c,0x47,0x42,0x55,0x54,0x54,0x4f,0x4e,0x43,0x48,0x45,0x43,0x4b,0x45,0x44 /* IsDlgButtonChecked */
	.short 98
	.byte 0x0c,0x44,0x4c,0x47,0x44,0x49,0x52,0x53,0x45,0x4c,0x45,0x43,0x54 /* DlgDirSelect */
	.short 99
	.byte 0x0a,0x44,0x4c,0x47,0x44,0x49,0x52,0x4c,0x49,0x53,0x54 /* DlgDirList */
	.short 100
	.byte 0x12,0x53,0x45,0x4e,0x44,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* SendDlgItemMessage */
	.short 101
	.byte 0x10,0x41,0x44,0x4a,0x55,0x53,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x52,0x45,0x43,0x54 /* AdjustWindowRect */
	.short 102
	.byte 0x0d,0x4d,0x41,0x50,0x44,0x49,0x41,0x4c,0x4f,0x47,0x52,0x45,0x43,0x54 /* MapDialogRect */
	.short 103
	.byte 0x0b,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x42,0x45,0x45,0x50 /* MessageBeep */
	.short 104
	.byte 0x0b,0x46,0x4c,0x41,0x53,0x48,0x57,0x49,0x4e,0x44,0x4f,0x57 /* FlashWindow */
	.short 105
	.byte 0x0b,0x47,0x45,0x54,0x4b,0x45,0x59,0x53,0x54,0x41,0x54,0x45 /* GetKeyState */
	.short 106
	.byte 0x0d,0x44,0x45,0x46,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x52,0x4f,0x43 /* DefWindowProc */
	.short 107
	.byte 0x0a,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* GetMessage */
	.short 108
	.byte 0x0b,0x50,0x45,0x45,0x4b,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* PeekMessage */
	.short 109
	.byte 0x0b,0x50,0x4f,0x53,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* PostMessage */
	.short 110
	.byte 0x0b,0x53,0x45,0x4e,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* SendMessage */
	.short 111
	.byte 0x0b,0x57,0x41,0x49,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* WaitMessage */
	.short 112
	.byte 0x10,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* TranslateMessage */
	.short 113
	.byte 0x0f,0x44,0x49,0x53,0x50,0x41,0x54,0x43,0x48,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* DispatchMessage */
	.short 114
	.byte 0x0c,0x52,0x45,0x50,0x4c,0x59,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* ReplyMessage */
	.short 115
	.byte 0x0e,0x50,0x4f,0x53,0x54,0x41,0x50,0x50,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* PostAppMessage */
	.short 116
	.byte 0x0c,0x57,0x49,0x4e,0x44,0x4f,0x57,0x46,0x52,0x4f,0x4d,0x44,0x43 /* WindowFromDC */
	.short 117
	.byte 0x15,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* RegisterWindowMessage */
	.short 118
	.byte 0x0d,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x50,0x4f,0x53 /* GetMessagePos */
	.short 119
	.byte 0x0e,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x54,0x49,0x4d,0x45 /* GetMessageTime */
	.short 120
	.byte 0x0e,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53,0x48,0x4f,0x4f,0x4b /* SetWindowsHook */
	.short 121
	.byte 0x0e,0x43,0x41,0x4c,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x52,0x4f,0x43 /* CallWindowProc */
	.short 122
	.byte 0x0d,0x43,0x41,0x4c,0x4c,0x4d,0x53,0x47,0x46,0x49,0x4c,0x54,0x45,0x52 /* CallMsgFilter */
	.short 123
	.byte 0x0c,0x55,0x50,0x44,0x41,0x54,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* UpdateWindow */
	.short 124
	.byte 0x0e,0x49,0x4e,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x52,0x45,0x43,0x54 /* InvalidateRect */
	.short 125
	.byte 0x0d,0x49,0x4e,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x52,0x47,0x4e /* InvalidateRgn */
	.short 126
	.byte 0x0c,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x52,0x45,0x43,0x54 /* ValidateRect */
	.short 127
	.byte 0x0b,0x56,0x41,0x4c,0x49,0x44,0x41,0x54,0x45,0x52,0x47,0x4e /* ValidateRgn */
	.short 128
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x57,0x4f,0x52,0x44 /* GetClassWord */
	.short 129
	.byte 0x0c,0x53,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x57,0x4f,0x52,0x44 /* SetClassWord */
	.short 130
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x4c,0x4f,0x4e,0x47 /* GetClassLong */
	.short 131
	.byte 0x0c,0x53,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x4c,0x4f,0x4e,0x47 /* SetClassLong */
	.short 132
	.byte 0x0d,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x57,0x4f,0x52,0x44 /* GetWindowWord */
	.short 133
	.byte 0x0d,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x57,0x4f,0x52,0x44 /* SetWindowWord */
	.short 134
	.byte 0x0d,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4c,0x4f,0x4e,0x47 /* GetWindowLong */
	.short 135
	.byte 0x0d,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x4c,0x4f,0x4e,0x47 /* SetWindowLong */
	.short 136
	.byte 0x0d,0x4f,0x50,0x45,0x4e,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44 /* OpenClipboard */
	.short 137
	.byte 0x0e,0x43,0x4c,0x4f,0x53,0x45,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44 /* CloseClipboard */
	.short 138
	.byte 0x0e,0x45,0x4d,0x50,0x54,0x59,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44 /* EmptyClipboard */
	.short 139
	.byte 0x11,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x4f,0x57,0x4e,0x45,0x52 /* GetClipboardOwner */
	.short 140
	.byte 0x10,0x53,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x44,0x41,0x54,0x41 /* SetClipboardData */
	.short 141
	.byte 0x10,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x44,0x41,0x54,0x41 /* GetClipboardData */
	.short 142
	.byte 0x15,0x43,0x4f,0x55,0x4e,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54,0x53 /* CountClipboardFormats */
	.short 143
	.byte 0x14,0x45,0x4e,0x55,0x4d,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54,0x53 /* EnumClipboardFormats */
	.short 144
	.byte 0x17,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54 /* RegisterClipboardFormat */
	.short 145
	.byte 0x16,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54,0x4e,0x41,0x4d,0x45 /* GetClipboardFormatName */
	.short 146
	.byte 0x12,0x53,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x56,0x49,0x45,0x57,0x45,0x52 /* SetClipboardViewer */
	.short 147
	.byte 0x12,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x56,0x49,0x45,0x57,0x45,0x52 /* GetClipboardViewer */
	.short 148
	.byte 0x14,0x43,0x48,0x41,0x4e,0x47,0x45,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x43,0x48,0x41,0x49,0x4e /* ChangeClipboardChain */
	.short 149
	.byte 0x08,0x4c,0x4f,0x41,0x44,0x4d,0x45,0x4e,0x55 /* LoadMenu */
	.short 150
	.byte 0x0a,0x43,0x52,0x45,0x41,0x54,0x45,0x4d,0x45,0x4e,0x55 /* CreateMenu */
	.short 151
	.byte 0x0b,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x4d,0x45,0x4e,0x55 /* DestroyMenu */
	.short 152
	.byte 0x0a,0x43,0x48,0x41,0x4e,0x47,0x45,0x4d,0x45,0x4e,0x55 /* ChangeMenu */
	.short 153
	.byte 0x0d,0x43,0x48,0x45,0x43,0x4b,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d /* CheckMenuItem */
	.short 154
	.byte 0x0e,0x45,0x4e,0x41,0x42,0x4c,0x45,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d /* EnableMenuItem */
	.short 155
	.byte 0x0d,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x4d,0x45,0x4e,0x55 /* GetSystemMenu */
	.short 156
	.byte 0x07,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55 /* GetMenu */
	.short 157
	.byte 0x07,0x53,0x45,0x54,0x4d,0x45,0x4e,0x55 /* SetMenu */
	.short 158
	.byte 0x0a,0x47,0x45,0x54,0x53,0x55,0x42,0x4d,0x45,0x4e,0x55 /* GetSubMenu */
	.short 159
	.byte 0x0b,0x44,0x52,0x41,0x57,0x4d,0x45,0x4e,0x55,0x42,0x41,0x52 /* DrawMenuBar */
	.short 160
	.byte 0x0d,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x53,0x54,0x52,0x49,0x4e,0x47 /* GetMenuString */
	.short 161
	.byte 0x0e,0x48,0x49,0x4c,0x49,0x54,0x45,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d /* HiliteMenuItem */
	.short 162
	.byte 0x0b,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x41,0x52,0x45,0x54 /* CreateCaret */
	.short 163
	.byte 0x0c,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x43,0x41,0x52,0x45,0x54 /* DestroyCaret */
	.short 164
	.byte 0x0b,0x53,0x45,0x54,0x43,0x41,0x52,0x45,0x54,0x50,0x4f,0x53 /* SetCaretPos */
	.short 165
	.byte 0x09,0x48,0x49,0x44,0x45,0x43,0x41,0x52,0x45,0x54 /* HideCaret */
	.short 166
	.byte 0x09,0x53,0x48,0x4f,0x57,0x43,0x41,0x52,0x45,0x54 /* ShowCaret */
	.short 167
	.byte 0x11,0x53,0x45,0x54,0x43,0x41,0x52,0x45,0x54,0x42,0x4c,0x49,0x4e,0x4b,0x54,0x49,0x4d,0x45 /* SetCaretBlinkTime */
	.short 168
	.byte 0x11,0x47,0x45,0x54,0x43,0x41,0x52,0x45,0x54,0x42,0x4c,0x49,0x4e,0x4b,0x54,0x49,0x4d,0x45 /* GetCaretBlinkTime */
	.short 169
	.byte 0x14,0x41,0x52,0x52,0x41,0x4e,0x47,0x45,0x49,0x43,0x4f,0x4e,0x49,0x43,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* ArrangeIconicWindows */
	.short 170
	.byte 0x07,0x57,0x49,0x4e,0x48,0x45,0x4c,0x50 /* WinHelp */
	.short 171
	.byte 0x12,0x53,0x57,0x49,0x54,0x43,0x48,0x54,0x4f,0x54,0x48,0x49,0x53,0x57,0x49,0x4e,0x44,0x4f,0x57 /* SwitchToThisWindow */
	.short 172
	.byte 0x0a,0x4c,0x4f,0x41,0x44,0x43,0x55,0x52,0x53,0x4f,0x52 /* LoadCursor */
	.short 173
	.byte 0x08,0x4c,0x4f,0x41,0x44,0x49,0x43,0x4f,0x4e /* LoadIcon */
	.short 174
	.byte 0x0a,0x4c,0x4f,0x41,0x44,0x42,0x49,0x54,0x4d,0x41,0x50 /* LoadBitmap */
	.short 175
	.byte 0x0a,0x4c,0x4f,0x41,0x44,0x53,0x54,0x52,0x49,0x4e,0x47 /* LoadString */
	.short 176
	.byte 0x10,0x4c,0x4f,0x41,0x44,0x41,0x43,0x43,0x45,0x4c,0x45,0x52,0x41,0x54,0x4f,0x52,0x53 /* LoadAccelerators */
	.short 177
	.byte 0x14,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x41,0x43,0x43,0x45,0x4c,0x45,0x52,0x41,0x54,0x4f,0x52 /* TranslateAccelerator */
	.short 178
	.byte 0x10,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x4d,0x45,0x54,0x52,0x49,0x43,0x53 /* GetSystemMetrics */
	.short 179
	.byte 0x0b,0x47,0x45,0x54,0x53,0x59,0x53,0x43,0x4f,0x4c,0x4f,0x52 /* GetSysColor */
	.short 180
	.byte 0x0c,0x53,0x45,0x54,0x53,0x59,0x53,0x43,0x4f,0x4c,0x4f,0x52,0x53 /* SetSysColors */
	.short 181
	.byte 0x0f,0x4b,0x49,0x4c,0x4c,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45,0x52 /* KillSystemTimer */
	.short 182
	.byte 0x0b,0x47,0x45,0x54,0x43,0x41,0x52,0x45,0x54,0x50,0x4f,0x53 /* GetCaretPos */
	.short 183
	.byte 0x10,0x51,0x55,0x45,0x52,0x59,0x53,0x45,0x4e,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* QuerySendMessage */
	.short 184
	.byte 0x0a,0x47,0x52,0x41,0x59,0x53,0x54,0x52,0x49,0x4e,0x47 /* GrayString */
	.short 185
	.byte 0x0f,0x53,0x57,0x41,0x50,0x4d,0x4f,0x55,0x53,0x45,0x42,0x55,0x54,0x54,0x4f,0x4e /* SwapMouseButton */
	.short 186
	.byte 0x07,0x45,0x4e,0x44,0x4d,0x45,0x4e,0x55 /* EndMenu */
	.short 187
	.byte 0x11,0x53,0x45,0x54,0x53,0x59,0x53,0x4d,0x4f,0x44,0x41,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57 /* SetSysModalWindow */
	.short 188
	.byte 0x11,0x47,0x45,0x54,0x53,0x59,0x53,0x4d,0x4f,0x44,0x41,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetSysModalWindow */
	.short 189
	.byte 0x0d,0x47,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x52,0x45,0x43,0x54 /* GetUpdateRect */
	.short 190
	.byte 0x14,0x43,0x48,0x49,0x4c,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x46,0x52,0x4f,0x4d,0x50,0x4f,0x49,0x4e,0x54 /* ChildWindowFromPoint */
	.short 191
	.byte 0x0d,0x49,0x4e,0x53,0x45,0x4e,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* InSendMessage */
	.short 192
	.byte 0x1a,0x49,0x53,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54,0x41,0x56,0x41,0x49,0x4c,0x41,0x42,0x4c,0x45 /* IsClipboardFormatAvailable */
	.short 193
	.byte 0x14,0x44,0x4c,0x47,0x44,0x49,0x52,0x53,0x45,0x4c,0x45,0x43,0x54,0x43,0x4f,0x4d,0x42,0x4f,0x42,0x4f,0x58 /* DlgDirSelectComboBox */
	.short 194
	.byte 0x12,0x44,0x4c,0x47,0x44,0x49,0x52,0x4c,0x49,0x53,0x54,0x43,0x4f,0x4d,0x42,0x4f,0x42,0x4f,0x58 /* DlgDirListComboBox */
	.short 195
	.byte 0x0d,0x54,0x41,0x42,0x42,0x45,0x44,0x54,0x45,0x58,0x54,0x4f,0x55,0x54 /* TabbedTextOut */
	.short 196
	.byte 0x13,0x47,0x45,0x54,0x54,0x41,0x42,0x42,0x45,0x44,0x54,0x45,0x58,0x54,0x45,0x58,0x54,0x45,0x4e,0x54 /* GetTabbedTextExtent */
	.short 197
	.byte 0x13,0x43,0x41,0x53,0x43,0x41,0x44,0x45,0x43,0x48,0x49,0x4c,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* CascadeChildWindows */
	.short 198
	.byte 0x10,0x54,0x49,0x4c,0x45,0x43,0x48,0x49,0x4c,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* TileChildWindows */
	.short 199
	.byte 0x08,0x4f,0x50,0x45,0x4e,0x43,0x4f,0x4d,0x4d /* OpenComm */
	.short 200
	.byte 0x0c,0x53,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x53,0x54,0x41,0x54,0x45 /* SetCommState */
	.short 201
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x53,0x54,0x41,0x54,0x45 /* GetCommState */
	.short 202
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x45,0x52,0x52,0x4f,0x52 /* GetCommError */
	.short 203
	.byte 0x08,0x52,0x45,0x41,0x44,0x43,0x4f,0x4d,0x4d /* ReadComm */
	.short 204
	.byte 0x09,0x57,0x52,0x49,0x54,0x45,0x43,0x4f,0x4d,0x4d /* WriteComm */
	.short 205
	.byte 0x10,0x54,0x52,0x41,0x4e,0x53,0x4d,0x49,0x54,0x43,0x4f,0x4d,0x4d,0x43,0x48,0x41,0x52 /* TransmitCommChar */
	.short 206
	.byte 0x09,0x43,0x4c,0x4f,0x53,0x45,0x43,0x4f,0x4d,0x4d /* CloseComm */
	.short 207
	.byte 0x10,0x53,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x45,0x56,0x45,0x4e,0x54,0x4d,0x41,0x53,0x4b /* SetCommEventMask */
	.short 208
	.byte 0x10,0x47,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x45,0x56,0x45,0x4e,0x54,0x4d,0x41,0x53,0x4b /* GetCommEventMask */
	.short 209
	.byte 0x0c,0x53,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x42,0x52,0x45,0x41,0x4b /* SetCommBreak */
	.short 210
	.byte 0x0e,0x43,0x4c,0x45,0x41,0x52,0x43,0x4f,0x4d,0x4d,0x42,0x52,0x45,0x41,0x4b /* ClearCommBreak */
	.short 211
	.byte 0x0d,0x55,0x4e,0x47,0x45,0x54,0x43,0x4f,0x4d,0x4d,0x43,0x48,0x41,0x52 /* UngetCommChar */
	.short 212
	.byte 0x0c,0x42,0x55,0x49,0x4c,0x44,0x43,0x4f,0x4d,0x4d,0x44,0x43,0x42 /* BuildCommDCB */
	.short 213
	.byte 0x12,0x45,0x53,0x43,0x41,0x50,0x45,0x43,0x4f,0x4d,0x4d,0x46,0x55,0x4e,0x43,0x54,0x49,0x4f,0x4e /* EscapeCommFunction */
	.short 214
	.byte 0x09,0x46,0x4c,0x55,0x53,0x48,0x43,0x4f,0x4d,0x4d /* FlushComm */
	.short 215
	.byte 0x0d,0x55,0x53,0x45,0x52,0x53,0x45,0x45,0x55,0x53,0x45,0x52,0x44,0x4f /* UserSeeUserDo */
	.short 216
	.byte 0x10,0x4c,0x4f,0x4f,0x4b,0x55,0x50,0x4d,0x45,0x4e,0x55,0x48,0x41,0x4e,0x44,0x4c,0x45 /* LookupMenuHandle */
	.short 217
	.byte 0x11,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x4f,0x58,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* DialogBoxIndirect */
	.short 218
	.byte 0x14,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateDialogIndirect */
	.short 219
	.byte 0x10,0x4c,0x4f,0x41,0x44,0x4d,0x45,0x4e,0x55,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* LoadMenuIndirect */
	.short 220
	.byte 0x08,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x44,0x43 /* ScrollDC */
	.short 221
	.byte 0x10,0x47,0x45,0x54,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x53,0x54,0x41,0x54,0x45 /* GetKeyboardState */
	.short 222
	.byte 0x10,0x53,0x45,0x54,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x53,0x54,0x41,0x54,0x45 /* SetKeyboardState */
	.short 223
	.byte 0x0d,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x41,0x53,0x4b /* GetWindowTask */
	.short 224
	.byte 0x0f,0x45,0x4e,0x55,0x4d,0x54,0x41,0x53,0x4b,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* EnumTaskWindows */
	.short 225
	.byte 0x09,0x4c,0x4f,0x43,0x4b,0x49,0x4e,0x50,0x55,0x54 /* LockInput */
	.short 226
	.byte 0x13,0x47,0x45,0x54,0x4e,0x45,0x58,0x54,0x44,0x4c,0x47,0x47,0x52,0x4f,0x55,0x50,0x49,0x54,0x45,0x4d /* GetNextDlgGroupItem */
	.short 227
	.byte 0x11,0x47,0x45,0x54,0x4e,0x45,0x58,0x54,0x44,0x4c,0x47,0x54,0x41,0x42,0x49,0x54,0x45,0x4d /* GetNextDlgTabItem */
	.short 228
	.byte 0x0c,0x47,0x45,0x54,0x54,0x4f,0x50,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetTopWindow */
	.short 229
	.byte 0x0d,0x47,0x45,0x54,0x4e,0x45,0x58,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetNextWindow */
	.short 230
	.byte 0x13,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x44,0x45,0x42,0x55,0x47,0x53,0x54,0x41,0x54,0x45 /* GetSystemDebugState */
	.short 231
	.byte 0x0c,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* SetWindowPos */
	.short 232
	.byte 0x09,0x53,0x45,0x54,0x50,0x41,0x52,0x45,0x4e,0x54 /* SetParent */
	.short 233
	.byte 0x11,0x55,0x4e,0x48,0x4f,0x4f,0x4b,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53,0x48,0x4f,0x4f,0x4b /* UnhookWindowsHook */
	.short 234
	.byte 0x0b,0x44,0x45,0x46,0x48,0x4f,0x4f,0x4b,0x50,0x52,0x4f,0x43 /* DefHookProc */
	.short 235
	.byte 0x0a,0x47,0x45,0x54,0x43,0x41,0x50,0x54,0x55,0x52,0x45 /* GetCapture */
	.short 236
	.byte 0x0c,0x47,0x45,0x54,0x55,0x50,0x44,0x41,0x54,0x45,0x52,0x47,0x4e /* GetUpdateRgn */
	.short 237
	.byte 0x10,0x45,0x58,0x43,0x4c,0x55,0x44,0x45,0x55,0x50,0x44,0x41,0x54,0x45,0x52,0x47,0x4e /* ExcludeUpdateRgn */
	.short 238
	.byte 0x0e,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x4f,0x58,0x50,0x41,0x52,0x41,0x4d /* DialogBoxParam */
	.short 239
	.byte 0x16,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x4f,0x58,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54,0x50,0x41,0x52,0x41,0x4d /* DialogBoxIndirectParam */
	.short 240
	.byte 0x11,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47,0x50,0x41,0x52,0x41,0x4d /* CreateDialogParam */
	.short 241
	.byte 0x19,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54,0x50,0x41,0x52,0x41,0x4d /* CreateDialogIndirectParam */
	.short 242
	.byte 0x12,0x47,0x45,0x54,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x41,0x53,0x45,0x55,0x4e,0x49,0x54,0x53 /* GetDialogBaseUnits */
	.short 243
	.byte 0x09,0x45,0x51,0x55,0x41,0x4c,0x52,0x45,0x43,0x54 /* EqualRect */
	.short 244
	.byte 0x16,0x45,0x4e,0x41,0x42,0x4c,0x45,0x43,0x4f,0x4d,0x4d,0x4e,0x4f,0x54,0x49,0x46,0x49,0x43,0x41,0x54,0x49,0x4f,0x4e /* EnableCommNotification */
	.short 245
	.byte 0x0f,0x45,0x58,0x49,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53,0x45,0x58,0x45,0x43 /* ExitWindowsExec */
	.short 246
	.byte 0x09,0x47,0x45,0x54,0x43,0x55,0x52,0x53,0x4f,0x52 /* GetCursor */
	.short 247
	.byte 0x16,0x47,0x45,0x54,0x4f,0x50,0x45,0x4e,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetOpenClipboardWindow */
	.short 248
	.byte 0x10,0x47,0x45,0x54,0x41,0x53,0x59,0x4e,0x43,0x4b,0x45,0x59,0x53,0x54,0x41,0x54,0x45 /* GetAsyncKeyState */
	.short 249
	.byte 0x0c,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x53,0x54,0x41,0x54,0x45 /* GetMenuState */
	.short 250
	.byte 0x11,0x53,0x45,0x4e,0x44,0x44,0x52,0x49,0x56,0x45,0x52,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* SendDriverMessage */
	.short 251
	.byte 0x0a,0x4f,0x50,0x45,0x4e,0x44,0x52,0x49,0x56,0x45,0x52 /* OpenDriver */
	.short 252
	.byte 0x0b,0x43,0x4c,0x4f,0x53,0x45,0x44,0x52,0x49,0x56,0x45,0x52 /* CloseDriver */
	.short 253
	.byte 0x15,0x47,0x45,0x54,0x44,0x52,0x49,0x56,0x45,0x52,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x48,0x41,0x4e,0x44,0x4c,0x45 /* GetDriverModuleHandle */
	.short 254
	.byte 0x0d,0x44,0x45,0x46,0x44,0x52,0x49,0x56,0x45,0x52,0x50,0x52,0x4f,0x43 /* DefDriverProc */
	.short 255
	.byte 0x0d,0x47,0x45,0x54,0x44,0x52,0x49,0x56,0x45,0x52,0x49,0x4e,0x46,0x4f /* GetDriverInfo */
	.short 256
	.byte 0x0d,0x47,0x45,0x54,0x4e,0x45,0x58,0x54,0x44,0x52,0x49,0x56,0x45,0x52 /* GetNextDriver */
	.short 257
	.byte 0x0f,0x4d,0x41,0x50,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x49,0x4e,0x54,0x53 /* MapWindowPoints */
	.short 258
	.byte 0x13,0x42,0x45,0x47,0x49,0x4e,0x44,0x45,0x46,0x45,0x52,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* BeginDeferWindowPos */
	.short 259
	.byte 0x0e,0x44,0x45,0x46,0x45,0x52,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* DeferWindowPos */
	.short 260
	.byte 0x11,0x45,0x4e,0x44,0x44,0x45,0x46,0x45,0x52,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* EndDeferWindowPos */
	.short 261
	.byte 0x09,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetWindow */
	.short 262
	.byte 0x10,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x43,0x4f,0x55,0x4e,0x54 /* GetMenuItemCount */
	.short 263
	.byte 0x0d,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x49,0x44 /* GetMenuItemID */
	.short 264
	.byte 0x0f,0x53,0x48,0x4f,0x57,0x4f,0x57,0x4e,0x45,0x44,0x50,0x4f,0x50,0x55,0x50,0x53 /* ShowOwnedPopups */
	.short 265
	.byte 0x0f,0x53,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x51,0x55,0x45,0x55,0x45 /* SetMessageQueue */
	.short 266
	.byte 0x0d,0x53,0x48,0x4f,0x57,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x42,0x41,0x52 /* ShowScrollBar */
	.short 267
	.byte 0x0d,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x41,0x44,0x44,0x41,0x54,0x4f,0x4d /* GlobalAddAtom */
	.short 268
	.byte 0x10,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x44,0x45,0x4c,0x45,0x54,0x45,0x41,0x54,0x4f,0x4d /* GlobalDeleteAtom */
	.short 269
	.byte 0x0e,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x46,0x49,0x4e,0x44,0x41,0x54,0x4f,0x4d /* GlobalFindAtom */
	.short 270
	.byte 0x11,0x47,0x4c,0x4f,0x42,0x41,0x4c,0x47,0x45,0x54,0x41,0x54,0x4f,0x4d,0x4e,0x41,0x4d,0x45 /* GlobalGetAtomName */
	.short 271
	.byte 0x08,0x49,0x53,0x5a,0x4f,0x4f,0x4d,0x45,0x44 /* IsZoomed */
	.short 272
	.byte 0x10,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c,0x50,0x41,0x4e,0x45,0x4c,0x49,0x4e,0x46,0x4f /* ControlPanelInfo */
	.short 273
	.byte 0x12,0x47,0x45,0x54,0x4e,0x45,0x58,0x54,0x51,0x55,0x45,0x55,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetNextQueueWindow */
	.short 274
	.byte 0x0d,0x52,0x45,0x50,0x41,0x49,0x4e,0x54,0x53,0x43,0x52,0x45,0x45,0x4e /* RepaintScreen */
	.short 275
	.byte 0x0a,0x4c,0x4f,0x43,0x4b,0x4d,0x59,0x54,0x41,0x53,0x4b /* LockMyTask */
	.short 276
	.byte 0x0c,0x47,0x45,0x54,0x44,0x4c,0x47,0x43,0x54,0x52,0x4c,0x49,0x44 /* GetDlgCtrlID */
	.short 277
	.byte 0x0e,0x47,0x45,0x54,0x44,0x45,0x53,0x4b,0x54,0x4f,0x50,0x48,0x57,0x4e,0x44 /* GetDesktopHwnd */
	.short 278
	.byte 0x11,0x4f,0x4c,0x44,0x53,0x45,0x54,0x44,0x45,0x53,0x4b,0x50,0x41,0x54,0x54,0x45,0x52,0x4e /* OldSetDeskPattern */
	.short 279
	.byte 0x0d,0x53,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x4d,0x45,0x4e,0x55 /* SetSystemMenu */
	.short 280
	.byte 0x10,0x47,0x45,0x54,0x53,0x59,0x53,0x43,0x4f,0x4c,0x4f,0x52,0x42,0x52,0x55,0x53,0x48 /* GetSysColorBrush */
	.short 281
	.byte 0x0d,0x53,0x45,0x4c,0x45,0x43,0x54,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* SelectPalette */
	.short 282
	.byte 0x0e,0x52,0x45,0x41,0x4c,0x49,0x5a,0x45,0x50,0x41,0x4c,0x45,0x54,0x54,0x45 /* RealizePalette */
	.short 283
	.byte 0x16,0x47,0x45,0x54,0x46,0x52,0x45,0x45,0x53,0x59,0x53,0x54,0x45,0x4d,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45,0x53 /* GetFreeSystemResources */
	.short 284
	.byte 0x10,0x53,0x45,0x54,0x44,0x45,0x53,0x4b,0x57,0x41,0x4c,0x4c,0x50,0x41,0x50,0x45,0x52 /* SetDeskWallPaper */
	.short 285
	.byte 0x10,0x47,0x45,0x54,0x44,0x45,0x53,0x4b,0x54,0x4f,0x50,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetDesktopWindow */
	.short 286
	.byte 0x12,0x47,0x45,0x54,0x4c,0x41,0x53,0x54,0x41,0x43,0x54,0x49,0x56,0x45,0x50,0x4f,0x50,0x55,0x50 /* GetLastActivePopup */
	.short 287
	.byte 0x13,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x45,0x58,0x54,0x52,0x41,0x49,0x4e,0x46,0x4f /* GetMessageExtraInfo */
	.short 288
	.byte 0x0b,0x4b,0x45,0x59,0x42,0x44,0x5f,0x45,0x56,0x45,0x4e,0x54 /* keybd_event */
	.short 289
	.byte 0x0c,0x52,0x45,0x44,0x52,0x41,0x57,0x57,0x49,0x4e,0x44,0x4f,0x57 /* RedrawWindow */
	.short 290
	.byte 0x10,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53,0x48,0x4f,0x4f,0x4b,0x45,0x58 /* SetWindowsHookEx */
	.short 291
	.byte 0x13,0x55,0x4e,0x48,0x4f,0x4f,0x4b,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53,0x48,0x4f,0x4f,0x4b,0x45,0x58 /* UnhookWindowsHookEx */
	.short 292
	.byte 0x0e,0x43,0x41,0x4c,0x4c,0x4e,0x45,0x58,0x54,0x48,0x4f,0x4f,0x4b,0x45,0x58 /* CallNextHookEx */
	.short 293
	.byte 0x10,0x4c,0x4f,0x43,0x4b,0x57,0x49,0x4e,0x44,0x4f,0x57,0x55,0x50,0x44,0x41,0x54,0x45 /* LockWindowUpdate */
	.short 294
	.byte 0x0b,0x4d,0x4f,0x55,0x53,0x45,0x5f,0x45,0x56,0x45,0x4e,0x54 /* mouse_event */
	.short 299
	.byte 0x16,0x55,0x4e,0x4c,0x4f,0x41,0x44,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c,0x45,0x44,0x44,0x52,0x49,0x56,0x45,0x52,0x53 /* UnloadInstalledDrivers */
	.short 300
	.byte 0x0b,0x45,0x44,0x49,0x54,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* EDITWNDPROC */
	.short 301
	.byte 0x0d,0x53,0x54,0x41,0x54,0x49,0x43,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* STATICWNDPROC */
	.short 302
	.byte 0x0d,0x42,0x55,0x54,0x54,0x4f,0x4e,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* BUTTONWNDPROC */
	.short 303
	.byte 0x09,0x53,0x42,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* SBWNDPROC */
	.short 304
	.byte 0x0e,0x44,0x45,0x53,0x4b,0x54,0x4f,0x50,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* DESKTOPWNDPROC */
	.short 305
	.byte 0x0b,0x4d,0x45,0x4e,0x55,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* MENUWNDPROC */
	.short 306
	.byte 0x0e,0x4c,0x42,0x4f,0x58,0x43,0x54,0x4c,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* LBOXCTLWNDPROC */
	.short 307
	.byte 0x0a,0x44,0x45,0x46,0x44,0x4c,0x47,0x50,0x52,0x4f,0x43 /* DefDlgProc */
	.short 308
	.byte 0x0d,0x47,0x45,0x54,0x43,0x4c,0x49,0x50,0x43,0x55,0x52,0x53,0x4f,0x52 /* GetClipCursor */
	.short 309
	.byte 0x0a,0x53,0x49,0x47,0x4e,0x41,0x4c,0x50,0x52,0x4f,0x43 /* SignalProc */
	.short 314
	.byte 0x0e,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58 /* ScrollWindowEx */
	.short 319
	.byte 0x0b,0x53,0x59,0x53,0x45,0x52,0x52,0x4f,0x52,0x42,0x4f,0x58 /* SysErrorBox */
	.short 320
	.byte 0x0c,0x53,0x45,0x54,0x45,0x56,0x45,0x4e,0x54,0x48,0x4f,0x4f,0x4b /* SetEventHook */
	.short 321
	.byte 0x13,0x57,0x49,0x4e,0x4f,0x4c,0x44,0x41,0x50,0x50,0x48,0x41,0x43,0x4b,0x4f,0x4d,0x41,0x54,0x49,0x43 /* WinOldAppHackOMatic */
	.short 322
	.byte 0x0b,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x32 /* GetMessage2 */
	.short 323
	.byte 0x0a,0x46,0x49,0x4c,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57 /* FillWindow */
	.short 324
	.byte 0x09,0x50,0x41,0x49,0x4e,0x54,0x52,0x45,0x43,0x54 /* PaintRect */
	.short 325
	.byte 0x0f,0x47,0x45,0x54,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c,0x42,0x52,0x55,0x53,0x48 /* GetControlBrush */
	.short 326
	.byte 0x13,0x45,0x4e,0x41,0x42,0x4c,0x45,0x48,0x41,0x52,0x44,0x57,0x41,0x52,0x45,0x49,0x4e,0x50,0x55,0x54 /* EnableHardwareInput */
	.short 331
	.byte 0x09,0x55,0x53,0x45,0x52,0x59,0x49,0x45,0x4c,0x44 /* UserYield */
	.short 332
	.byte 0x0a,0x49,0x53,0x55,0x53,0x45,0x52,0x49,0x44,0x4c,0x45 /* IsUserIdle */
	.short 333
	.byte 0x0e,0x47,0x45,0x54,0x51,0x55,0x45,0x55,0x45,0x53,0x54,0x41,0x54,0x55,0x53 /* GetQueueStatus */
	.short 334
	.byte 0x0d,0x47,0x45,0x54,0x49,0x4e,0x50,0x55,0x54,0x53,0x54,0x41,0x54,0x45 /* GetInputState */
	.short 335
	.byte 0x15,0x4c,0x4f,0x41,0x44,0x43,0x55,0x52,0x53,0x4f,0x52,0x49,0x43,0x4f,0x4e,0x48,0x41,0x4e,0x44,0x4c,0x45,0x52 /* LoadCursorIconHandler */
	.short 336
	.byte 0x11,0x47,0x45,0x54,0x4d,0x4f,0x55,0x53,0x45,0x45,0x56,0x45,0x4e,0x54,0x50,0x52,0x4f,0x43 /* GetMouseEventProc */
	.short 337
	.byte 0x07,0x45,0x43,0x47,0x45,0x54,0x44,0x53 /* ECGETDS */
	.short 338
	.byte 0x0f,0x47,0x45,0x54,0x46,0x49,0x4c,0x45,0x50,0x4f,0x52,0x54,0x4e,0x41,0x4d,0x45 /* GetFilePortName */
	.short 343
	.byte 0x12,0x43,0x4f,0x4d,0x42,0x4f,0x42,0x4f,0x58,0x43,0x54,0x4c,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* COMBOBOXCTLWNDPROC */
	.short 344
	.byte 0x07,0x42,0x45,0x41,0x52,0x33,0x34,0x35 /* BEAR345 */
	.short 345
	.byte 0x14,0x4c,0x4f,0x41,0x44,0x44,0x49,0x42,0x43,0x55,0x52,0x53,0x4f,0x52,0x48,0x41,0x4e,0x44,0x4c,0x45,0x52 /* LoadDIBCursorHandler */
	.short 356
	.byte 0x12,0x4c,0x4f,0x41,0x44,0x44,0x49,0x42,0x49,0x43,0x4f,0x4e,0x48,0x41,0x4e,0x44,0x4c,0x45,0x52 /* LoadDIBIconHandler */
	.short 357
	.byte 0x06,0x49,0x53,0x4d,0x45,0x4e,0x55 /* IsMenu */
	.short 358
	.byte 0x07,0x47,0x45,0x54,0x44,0x43,0x45,0x58 /* GetDCEx */
	.short 359
	.byte 0x06,0x44,0x43,0x48,0x4f,0x4f,0x4b /* DCHook */
	.short 362
	.byte 0x1b,0x4c,0x4f,0x4f,0x4b,0x55,0x50,0x49,0x43,0x4f,0x4e,0x49,0x44,0x46,0x52,0x4f,0x4d,0x44,0x49,0x52,0x45,0x43,0x54,0x4f,0x52,0x59,0x45,0x58 /* LookupIconIdFromDirectoryEx */
	.short 364
	.byte 0x08,0x43,0x4f,0x50,0x59,0x49,0x43,0x4f,0x4e /* CopyIcon */
	.short 368
	.byte 0x0a,0x43,0x4f,0x50,0x59,0x43,0x55,0x52,0x53,0x4f,0x52 /* CopyCursor */
	.short 369
	.byte 0x12,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4c,0x41,0x43,0x45,0x4d,0x45,0x4e,0x54 /* GetWindowPlacement */
	.short 370
	.byte 0x12,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4c,0x41,0x43,0x45,0x4d,0x45,0x4e,0x54 /* SetWindowPlacement */
	.short 371
	.byte 0x15,0x47,0x45,0x54,0x49,0x4e,0x54,0x45,0x52,0x4e,0x41,0x4c,0x49,0x43,0x4f,0x4e,0x48,0x45,0x41,0x44,0x45,0x52 /* GetInternalIconHeader */
	.short 372
	.byte 0x0c,0x53,0x55,0x42,0x54,0x52,0x41,0x43,0x54,0x52,0x45,0x43,0x54 /* SubtractRect */
	.short 373
	.byte 0x0d,0x44,0x4c,0x4c,0x45,0x4e,0x54,0x52,0x59,0x50,0x4f,0x49,0x4e,0x54 /* DllEntryPoint */
	.short 374
	.byte 0x0a,0x44,0x52,0x41,0x57,0x54,0x45,0x58,0x54,0x45,0x58 /* DrawTextEx */
	.short 375
	.byte 0x13,0x53,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x45,0x58,0x54,0x52,0x41,0x49,0x4e,0x46,0x4f /* SetMessageExtraInfo */
	.short 376
	.byte 0x09,0x53,0x45,0x54,0x50,0x52,0x4f,0x50,0x45,0x58 /* SetPropEx */
	.short 378
	.byte 0x09,0x47,0x45,0x54,0x50,0x52,0x4f,0x50,0x45,0x58 /* GetPropEx */
	.short 379
	.byte 0x0c,0x52,0x45,0x4d,0x4f,0x56,0x45,0x50,0x52,0x4f,0x50,0x45,0x58 /* RemovePropEx */
	.short 380
	.byte 0x16,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54,0x48,0x45,0x4c,0x50,0x49,0x44 /* SetWindowContextHelpID */
	.short 382
	.byte 0x16,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54,0x48,0x45,0x4c,0x50,0x49,0x44 /* GetWindowContextHelpID */
	.short 383
	.byte 0x14,0x53,0x45,0x54,0x4d,0x45,0x4e,0x55,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54,0x48,0x45,0x4c,0x50,0x49,0x44 /* SetMenuContextHelpId */
	.short 384
	.byte 0x14,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x43,0x4f,0x4e,0x54,0x45,0x58,0x54,0x48,0x45,0x4c,0x50,0x49,0x44 /* GetMenuContextHelpId */
	.short 385
	.byte 0x09,0x4c,0x4f,0x41,0x44,0x49,0x4d,0x41,0x47,0x45 /* LoadImage */
	.short 389
	.byte 0x09,0x43,0x4f,0x50,0x59,0x49,0x4d,0x41,0x47,0x45 /* CopyImage */
	.short 390
	.byte 0x0c,0x53,0x49,0x47,0x4e,0x41,0x4c,0x50,0x52,0x4f,0x43,0x33,0x32 /* SignalProc32 */
	.short 391
	.byte 0x0a,0x44,0x52,0x41,0x57,0x49,0x43,0x4f,0x4e,0x45,0x58 /* DrawIconEx */
	.short 394
	.byte 0x0b,0x47,0x45,0x54,0x49,0x43,0x4f,0x4e,0x49,0x4e,0x46,0x4f /* GetIconInfo */
	.short 395
	.byte 0x0f,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x43,0x4c,0x41,0x53,0x53,0x45,0x58 /* RegisterClassEx */
	.short 397
	.byte 0x0e,0x47,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x49,0x4e,0x46,0x4f,0x45,0x58 /* GetClassInfoEx */
	.short 398
	.byte 0x16,0x43,0x48,0x49,0x4c,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x46,0x52,0x4f,0x4d,0x50,0x4f,0x49,0x4e,0x54,0x45,0x58 /* ChildWindowFromPointEx */
	.short 399
	.byte 0x0d,0x46,0x49,0x4e,0x41,0x4c,0x55,0x53,0x45,0x52,0x49,0x4e,0x49,0x54 /* FinalUserInit */
	.short 400
	.byte 0x1a,0x47,0x45,0x54,0x50,0x52,0x49,0x4f,0x52,0x49,0x54,0x59,0x43,0x4c,0x49,0x50,0x42,0x4f,0x41,0x52,0x44,0x46,0x4f,0x52,0x4d,0x41,0x54 /* GetPriorityClipboardFormat */
	.short 402
	.byte 0x0f,0x55,0x4e,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x43,0x4c,0x41,0x53,0x53 /* UnregisterClass */
	.short 403
	.byte 0x0c,0x47,0x45,0x54,0x43,0x4c,0x41,0x53,0x53,0x49,0x4e,0x46,0x4f /* GetClassInfo */
	.short 404
	.byte 0x0c,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x55,0x52,0x53,0x4f,0x52 /* CreateCursor */
	.short 406
	.byte 0x0a,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x43,0x4f,0x4e /* CreateIcon */
	.short 407
	.byte 0x18,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x55,0x52,0x53,0x4f,0x52,0x49,0x43,0x4f,0x4e,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* CreateCursorIconIndirect */
	.short 408
	.byte 0x0f,0x49,0x4e,0x49,0x54,0x54,0x48,0x52,0x45,0x41,0x44,0x49,0x4e,0x50,0x55,0x54 /* InitThreadInput */
	.short 409
	.byte 0x0a,0x49,0x4e,0x53,0x45,0x52,0x54,0x4d,0x45,0x4e,0x55 /* InsertMenu */
	.short 410
	.byte 0x0a,0x41,0x50,0x50,0x45,0x4e,0x44,0x4d,0x45,0x4e,0x55 /* AppendMenu */
	.short 411
	.byte 0x0a,0x52,0x45,0x4d,0x4f,0x56,0x45,0x4d,0x45,0x4e,0x55 /* RemoveMenu */
	.short 412
	.byte 0x0a,0x44,0x45,0x4c,0x45,0x54,0x45,0x4d,0x45,0x4e,0x55 /* DeleteMenu */
	.short 413
	.byte 0x0a,0x4d,0x4f,0x44,0x49,0x46,0x59,0x4d,0x45,0x4e,0x55 /* ModifyMenu */
	.short 414
	.byte 0x0f,0x43,0x52,0x45,0x41,0x54,0x45,0x50,0x4f,0x50,0x55,0x50,0x4d,0x45,0x4e,0x55 /* CreatePopupMenu */
	.short 415
	.byte 0x0e,0x54,0x52,0x41,0x43,0x4b,0x50,0x4f,0x50,0x55,0x50,0x4d,0x45,0x4e,0x55 /* TrackPopupMenu */
	.short 416
	.byte 0x1a,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x43,0x48,0x45,0x43,0x4b,0x4d,0x41,0x52,0x4b,0x44,0x49,0x4d,0x45,0x4e,0x53,0x49,0x4f,0x4e,0x53 /* GetMenuCheckMarkDimensions */
	.short 417
	.byte 0x12,0x53,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x42,0x49,0x54,0x4d,0x41,0x50,0x53 /* SetMenuItemBitmaps */
	.short 418
	.byte 0x09,0x5f,0x57,0x53,0x50,0x52,0x49,0x4e,0x54,0x46 /* _wsprintf */
	.short 420
	.byte 0x09,0x57,0x56,0x53,0x50,0x52,0x49,0x4e,0x54,0x46 /* wvsprintf */
	.short 421
	.byte 0x0e,0x44,0x4c,0x47,0x44,0x49,0x52,0x53,0x45,0x4c,0x45,0x43,0x54,0x45,0x58 /* DlgDirSelectEx */
	.short 422
	.byte 0x16,0x44,0x4c,0x47,0x44,0x49,0x52,0x53,0x45,0x4c,0x45,0x43,0x54,0x43,0x4f,0x4d,0x42,0x4f,0x42,0x4f,0x58,0x45,0x58 /* DlgDirSelectComboBoxEx */
	.short 423
	.byte 0x0c,0x46,0x49,0x4e,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58 /* FindWindowEx */
	.short 427
	.byte 0x0b,0x54,0x49,0x4c,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* TileWindows */
	.short 428
	.byte 0x0e,0x43,0x41,0x53,0x43,0x41,0x44,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* CascadeWindows */
	.short 429
	.byte 0x07,0x4c,0x53,0x54,0x52,0x43,0x4d,0x50 /* lstrcmp */
	.short 430
	.byte 0x09,0x41,0x4e,0x53,0x49,0x55,0x50,0x50,0x45,0x52 /* AnsiUpper */
	.short 431
	.byte 0x09,0x41,0x4e,0x53,0x49,0x4c,0x4f,0x57,0x45,0x52 /* AnsiLower */
	.short 432
	.byte 0x0b,0x49,0x53,0x43,0x48,0x41,0x52,0x41,0x4c,0x50,0x48,0x41 /* IsCharAlpha */
	.short 433
	.byte 0x12,0x49,0x53,0x43,0x48,0x41,0x52,0x41,0x4c,0x50,0x48,0x41,0x4e,0x55,0x4d,0x45,0x52,0x49,0x43 /* IsCharAlphaNumeric */
	.short 434
	.byte 0x0b,0x49,0x53,0x43,0x48,0x41,0x52,0x55,0x50,0x50,0x45,0x52 /* IsCharUpper */
	.short 435
	.byte 0x0b,0x49,0x53,0x43,0x48,0x41,0x52,0x4c,0x4f,0x57,0x45,0x52 /* IsCharLower */
	.short 436
	.byte 0x0d,0x41,0x4e,0x53,0x49,0x55,0x50,0x50,0x45,0x52,0x42,0x55,0x46,0x46 /* AnsiUpperBuff */
	.short 437
	.byte 0x0d,0x41,0x4e,0x53,0x49,0x4c,0x4f,0x57,0x45,0x52,0x42,0x55,0x46,0x46 /* AnsiLowerBuff */
	.short 438
	.byte 0x0e,0x49,0x4e,0x53,0x45,0x52,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d /* InsertMenuItem */
	.short 441
	.byte 0x0f,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x49,0x4e,0x46,0x4f /* GetMenuItemInfo */
	.short 443
	.byte 0x0c,0x44,0x45,0x46,0x46,0x52,0x41,0x4d,0x45,0x50,0x52,0x4f,0x43 /* DefFrameProc */
	.short 445
	.byte 0x0f,0x53,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x49,0x4e,0x46,0x4f /* SetMenuItemInfo */
	.short 446
	.byte 0x0f,0x44,0x45,0x46,0x4d,0x44,0x49,0x43,0x48,0x49,0x4c,0x44,0x50,0x52,0x4f,0x43 /* DefMDIChildProc */
	.short 447
	.byte 0x11,0x44,0x52,0x41,0x57,0x41,0x4e,0x49,0x4d,0x41,0x54,0x45,0x44,0x52,0x45,0x43,0x54,0x53 /* DrawAnimatedRects */
	.short 448
	.byte 0x09,0x44,0x52,0x41,0x57,0x53,0x54,0x41,0x54,0x45 /* DrawState */
	.short 449
	.byte 0x18,0x43,0x52,0x45,0x41,0x54,0x45,0x49,0x43,0x4f,0x4e,0x46,0x52,0x4f,0x4d,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45,0x45,0x58 /* CreateIconFromResourceEx */
	.short 450
	.byte 0x14,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x4d,0x44,0x49,0x53,0x59,0x53,0x41,0x43,0x43,0x45,0x4c /* TranslateMDISysAccel */
	.short 451
	.byte 0x0e,0x43,0x52,0x45,0x41,0x54,0x45,0x57,0x49,0x4e,0x44,0x4f,0x57,0x45,0x58 /* CreateWindowEx */
	.short 452
	.byte 0x12,0x41,0x44,0x4a,0x55,0x53,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x52,0x45,0x43,0x54,0x45,0x58 /* AdjustWindowRectEx */
	.short 454
	.byte 0x09,0x47,0x45,0x54,0x49,0x43,0x4f,0x4e,0x49,0x44 /* GetIconID */
	.short 455
	.byte 0x0f,0x4c,0x4f,0x41,0x44,0x49,0x43,0x4f,0x4e,0x48,0x41,0x4e,0x44,0x4c,0x45,0x52 /* LoadIconHandler */
	.short 456
	.byte 0x0b,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x49,0x43,0x4f,0x4e /* DestroyIcon */
	.short 457
	.byte 0x0d,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x43,0x55,0x52,0x53,0x4f,0x52 /* DestroyCursor */
	.short 458
	.byte 0x08,0x44,0x55,0x4d,0x50,0x49,0x43,0x4f,0x4e /* DumpIcon */
	.short 459
	.byte 0x14,0x47,0x45,0x54,0x49,0x4e,0x54,0x45,0x52,0x4e,0x41,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* GetInternalWindowPos */
	.short 460
	.byte 0x14,0x53,0x45,0x54,0x49,0x4e,0x54,0x45,0x52,0x4e,0x41,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57,0x50,0x4f,0x53 /* SetInternalWindowPos */
	.short 461
	.byte 0x0f,0x43,0x41,0x4c,0x43,0x43,0x48,0x49,0x4c,0x44,0x53,0x43,0x52,0x4f,0x4c,0x4c /* CalcChildScroll */
	.short 462
	.byte 0x0e,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x43,0x48,0x49,0x4c,0x44,0x52,0x45,0x4e /* ScrollChildren */
	.short 463
	.byte 0x0a,0x44,0x52,0x41,0x47,0x4f,0x42,0x4a,0x45,0x43,0x54 /* DragObject */
	.short 464
	.byte 0x0a,0x44,0x52,0x41,0x47,0x44,0x45,0x54,0x45,0x43,0x54 /* DragDetect */
	.short 465
	.byte 0x0d,0x44,0x52,0x41,0x57,0x46,0x4f,0x43,0x55,0x53,0x52,0x45,0x43,0x54 /* DrawFocusRect */
	.short 466
	.byte 0x0a,0x53,0x54,0x52,0x49,0x4e,0x47,0x46,0x55,0x4e,0x43 /* StringFunc */
	.short 470
	.byte 0x08,0x4c,0x53,0x54,0x52,0x43,0x4d,0x50,0x49 /* lstrcmpi */
	.short 471
	.byte 0x08,0x41,0x4e,0x53,0x49,0x4e,0x45,0x58,0x54 /* AnsiNext */
	.short 472
	.byte 0x08,0x41,0x4e,0x53,0x49,0x50,0x52,0x45,0x56 /* AnsiPrev */
	.short 473
	.byte 0x0d,0x53,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x49,0x4e,0x46,0x4f /* SetScrollInfo */
	.short 475
	.byte 0x0d,0x47,0x45,0x54,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x49,0x4e,0x46,0x4f /* GetScrollInfo */
	.short 476
	.byte 0x15,0x47,0x45,0x54,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54,0x4e,0x41,0x4d,0x45 /* GetKeyboardLayoutName */
	.short 477
	.byte 0x12,0x4c,0x4f,0x41,0x44,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54 /* LoadKeyboardLayout */
	.short 478
	.byte 0x11,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x46,0x52,0x4f,0x4d,0x50,0x4f,0x49,0x4e,0x54 /* MenuItemFromPoint */
	.short 479
	.byte 0x13,0x47,0x45,0x54,0x55,0x53,0x45,0x52,0x4c,0x4f,0x43,0x41,0x4c,0x4f,0x42,0x4a,0x54,0x59,0x50,0x45 /* GetUserLocalObjType */
	.short 480
	.byte 0x0f,0x45,0x4e,0x41,0x42,0x4c,0x45,0x53,0x43,0x52,0x4f,0x4c,0x4c,0x42,0x41,0x52 /* EnableScrollBar */
	.short 482
	.byte 0x14,0x53,0x59,0x53,0x54,0x45,0x4d,0x50,0x41,0x52,0x41,0x4d,0x45,0x54,0x45,0x52,0x53,0x49,0x4e,0x46,0x4f /* SystemParametersInfo */
	.short 483
	.byte 0x08,0x55,0x53,0x45,0x52,0x5f,0x34,0x38,0x39 /* USER_489 */
	.short 489
	.byte 0x08,0x55,0x53,0x45,0x52,0x5f,0x34,0x39,0x30 /* USER_490 */
	.short 490
	.byte 0x08,0x55,0x53,0x45,0x52,0x5f,0x34,0x39,0x32 /* USER_492 */
	.short 492
	.byte 0x08,0x55,0x53,0x45,0x52,0x5f,0x34,0x39,0x36 /* USER_496 */
	.short 496
	.byte 0x07,0x42,0x45,0x41,0x52,0x34,0x39,0x38 /* BEAR498 */
	.short 498
	.byte 0x0d,0x57,0x4e,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* WNetErrorText */
	.short 499
	.byte 0x10,0x46,0x41,0x52,0x43,0x41,0x4c,0x4c,0x4e,0x45,0x54,0x44,0x52,0x49,0x56,0x45,0x52 /* FARCALLNETDRIVER */
	.short 500
	.byte 0x0b,0x57,0x4e,0x45,0x54,0x4f,0x50,0x45,0x4e,0x4a,0x4f,0x42 /* WNetOpenJob */
	.short 501
	.byte 0x0c,0x57,0x4e,0x45,0x54,0x43,0x4c,0x4f,0x53,0x45,0x4a,0x4f,0x42 /* WNetCloseJob */
	.short 502
	.byte 0x0c,0x57,0x4e,0x45,0x54,0x41,0x42,0x4f,0x52,0x54,0x4a,0x4f,0x42 /* WNetAbortJob */
	.short 503
	.byte 0x0b,0x57,0x4e,0x45,0x54,0x48,0x4f,0x4c,0x44,0x4a,0x4f,0x42 /* WNetHoldJob */
	.short 504
	.byte 0x0e,0x57,0x4e,0x45,0x54,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x4a,0x4f,0x42 /* WNetReleaseJob */
	.short 505
	.byte 0x0d,0x57,0x4e,0x45,0x54,0x43,0x41,0x4e,0x43,0x45,0x4c,0x4a,0x4f,0x42 /* WNetCancelJob */
	.short 506
	.byte 0x10,0x57,0x4e,0x45,0x54,0x53,0x45,0x54,0x4a,0x4f,0x42,0x43,0x4f,0x50,0x49,0x45,0x53 /* WNetSetJobCopies */
	.short 507
	.byte 0x0e,0x57,0x4e,0x45,0x54,0x57,0x41,0x54,0x43,0x48,0x51,0x55,0x45,0x55,0x45 /* WNetWatchQueue */
	.short 508
	.byte 0x10,0x57,0x4e,0x45,0x54,0x55,0x4e,0x57,0x41,0x54,0x43,0x48,0x51,0x55,0x45,0x55,0x45 /* WNetUnwatchQueue */
	.short 509
	.byte 0x11,0x57,0x4e,0x45,0x54,0x4c,0x4f,0x43,0x4b,0x51,0x55,0x45,0x55,0x45,0x44,0x41,0x54,0x41 /* WNetLockQueueData */
	.short 510
	.byte 0x13,0x57,0x4e,0x45,0x54,0x55,0x4e,0x4c,0x4f,0x43,0x4b,0x51,0x55,0x45,0x55,0x45,0x44,0x41,0x54,0x41 /* WNetUnlockQueueData */
	.short 511
	.byte 0x11,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e /* WNetGetConnection */
	.short 512
	.byte 0x0b,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x43,0x41,0x50,0x53 /* WNetGetCaps */
	.short 513
	.byte 0x0e,0x57,0x4e,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45,0x4d,0x4f,0x44,0x45 /* WNetDeviceMode */
	.short 514
	.byte 0x10,0x57,0x4e,0x45,0x54,0x42,0x52,0x4f,0x57,0x53,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetBrowseDialog */
	.short 515
	.byte 0x0b,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x55,0x53,0x45,0x52 /* WNetGetUser */
	.short 516
	.byte 0x11,0x57,0x4e,0x45,0x54,0x41,0x44,0x44,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e /* WNetAddConnection */
	.short 517
	.byte 0x14,0x57,0x4e,0x45,0x54,0x43,0x41,0x4e,0x43,0x45,0x4c,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e /* WNetCancelConnection */
	.short 518
	.byte 0x0c,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52 /* WNetGetError */
	.short 519
	.byte 0x10,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* WNetGetErrorText */
	.short 520
	.byte 0x0a,0x57,0x4e,0x45,0x54,0x45,0x4e,0x41,0x42,0x4c,0x45 /* WNetEnable */
	.short 521
	.byte 0x0b,0x57,0x4e,0x45,0x54,0x44,0x49,0x53,0x41,0x42,0x4c,0x45 /* WNetDisable */
	.short 522
	.byte 0x15,0x57,0x4e,0x45,0x54,0x52,0x45,0x53,0x54,0x4f,0x52,0x45,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e /* WNetRestoreConnection */
	.short 523
	.byte 0x0c,0x57,0x4e,0x45,0x54,0x57,0x52,0x49,0x54,0x45,0x4a,0x4f,0x42 /* WNetWriteJob */
	.short 524
	.byte 0x11,0x57,0x4e,0x45,0x54,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetConnectDialog */
	.short 525
	.byte 0x14,0x57,0x4e,0x45,0x54,0x44,0x49,0x53,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetDisconnectDialog */
	.short 526
	.byte 0x14,0x57,0x4e,0x45,0x54,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetConnectionDialog */
	.short 527
	.byte 0x13,0x57,0x4e,0x45,0x54,0x56,0x49,0x45,0x57,0x51,0x55,0x45,0x55,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetViewQueueDialog */
	.short 528
	.byte 0x12,0x57,0x4e,0x45,0x54,0x50,0x52,0x4f,0x50,0x45,0x52,0x54,0x59,0x44,0x49,0x41,0x4c,0x4f,0x47 /* WNetPropertyDialog */
	.short 529
	.byte 0x14,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x44,0x49,0x52,0x45,0x43,0x54,0x4f,0x52,0x59,0x54,0x59,0x50,0x45 /* WNetGetDirectoryType */
	.short 530
	.byte 0x13,0x57,0x4e,0x45,0x54,0x44,0x49,0x52,0x45,0x43,0x54,0x4f,0x52,0x59,0x4e,0x4f,0x54,0x49,0x46,0x59 /* WNetDirectoryNotify */
	.short 531
	.byte 0x13,0x57,0x4e,0x45,0x54,0x47,0x45,0x54,0x50,0x52,0x4f,0x50,0x45,0x52,0x54,0x59,0x54,0x45,0x58,0x54 /* WNetGetPropertyText */
	.short 532
	.byte 0x0e,0x57,0x4e,0x45,0x54,0x49,0x4e,0x49,0x54,0x49,0x41,0x4c,0x49,0x5a,0x45 /* WNetInitialize */
	.short 533
	.byte 0x09,0x57,0x4e,0x45,0x54,0x4c,0x4f,0x47,0x4f,0x4e /* WNetLogon */
	.short 534
	.byte 0x10,0x57,0x4f,0x57,0x57,0x4f,0x52,0x44,0x42,0x52,0x45,0x41,0x4b,0x50,0x52,0x4f,0x43 /* WOWWORDBREAKPROC */
	.short 535
	.byte 0x0a,0x4d,0x4f,0x55,0x53,0x45,0x45,0x56,0x45,0x4e,0x54 /* MOUSEEVENT */
	.short 537
	.byte 0x0a,0x4b,0x45,0x59,0x42,0x44,0x45,0x56,0x45,0x4e,0x54 /* KEYBDEVENT */
	.short 538
	.byte 0x0e,0x4f,0x4c,0x44,0x45,0x58,0x49,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x53 /* OLDEXITWINDOWS */
	.short 595
	.byte 0x0e,0x47,0x45,0x54,0x53,0x48,0x45,0x4c,0x4c,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetShellWindow */
	.short 600
	.byte 0x0d,0x44,0x4f,0x48,0x4f,0x54,0x4b,0x45,0x59,0x53,0x54,0x55,0x46,0x46 /* DoHotkeyStuff */
	.short 601
	.byte 0x13,0x53,0x45,0x54,0x43,0x48,0x45,0x43,0x4b,0x43,0x55,0x52,0x53,0x4f,0x52,0x54,0x49,0x4d,0x45,0x52 /* SetCheckCursorTimer */
	.short 602
	.byte 0x16,0x42,0x52,0x4f,0x41,0x44,0x43,0x41,0x53,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* BroadcastSystemMessage */
	.short 604
	.byte 0x0f,0x48,0x41,0x43,0x4b,0x54,0x41,0x53,0x4b,0x4d,0x4f,0x4e,0x49,0x54,0x4f,0x52 /* HackTaskMonitor */
	.short 605
	.byte 0x0d,0x46,0x4f,0x52,0x4d,0x41,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* FormatMessage */
	.short 606
	.byte 0x13,0x47,0x45,0x54,0x46,0x4f,0x52,0x45,0x47,0x52,0x4f,0x55,0x4e,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57 /* GetForegroundWindow */
	.short 608
	.byte 0x13,0x53,0x45,0x54,0x46,0x4f,0x52,0x45,0x47,0x52,0x4f,0x55,0x4e,0x44,0x57,0x49,0x4e,0x44,0x4f,0x57 /* SetForegroundWindow */
	.short 609
	.byte 0x0d,0x44,0x45,0x53,0x54,0x52,0x4f,0x59,0x49,0x43,0x4f,0x4e,0x33,0x32 /* DestroyIcon32 */
	.short 610
	.byte 0x15,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x49,0x53,0x50,0x4c,0x41,0x59,0x53,0x45,0x54,0x54,0x49,0x4e,0x47,0x53 /* ChangeDisplaySettings */
	.short 620
	.byte 0x13,0x45,0x4e,0x55,0x4d,0x44,0x49,0x53,0x50,0x4c,0x41,0x59,0x53,0x45,0x54,0x54,0x49,0x4e,0x47,0x53 /* EnumDisplaySettings */
	.short 621
	.byte 0x19,0x4d,0x53,0x47,0x57,0x41,0x49,0x54,0x46,0x4f,0x52,0x4d,0x55,0x4c,0x54,0x49,0x50,0x4c,0x45,0x4f,0x42,0x4a,0x45,0x43,0x54,0x53 /* MsgWaitForMultipleObjects */
	.short 640
	.byte 0x16,0x41,0x43,0x54,0x49,0x56,0x41,0x54,0x45,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54 /* ActivateKeyboardLayout */
	.short 650
	.byte 0x11,0x47,0x45,0x54,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54 /* GetKeyboardLayout */
	.short 651
	.byte 0x15,0x47,0x45,0x54,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54,0x4c,0x49,0x53,0x54 /* GetKeyboardLayoutList */
	.short 652
	.byte 0x14,0x55,0x4e,0x4c,0x4f,0x41,0x44,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x59,0x4f,0x55,0x54 /* UnloadKeyboardLayout */
	.short 654
	.byte 0x12,0x50,0x4f,0x53,0x54,0x50,0x4f,0x53,0x54,0x45,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x53 /* PostPostedMessages */
	.short 655
	.byte 0x10,0x44,0x52,0x41,0x57,0x46,0x52,0x41,0x4d,0x45,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c /* DrawFrameControl */
	.short 656
	.byte 0x0f,0x44,0x52,0x41,0x57,0x43,0x41,0x50,0x54,0x49,0x4f,0x4e,0x54,0x45,0x4d,0x50 /* DrawCaptionTemp */
	.short 657
	.byte 0x0d,0x44,0x49,0x53,0x50,0x41,0x54,0x43,0x48,0x49,0x4e,0x50,0x55,0x54 /* DispatchInput */
	.short 658
	.byte 0x08,0x44,0x52,0x41,0x57,0x45,0x44,0x47,0x45 /* DrawEdge */
	.short 659
	.byte 0x0b,0x44,0x52,0x41,0x57,0x43,0x41,0x50,0x54,0x49,0x4f,0x4e /* DrawCaption */
	.short 660
	.byte 0x10,0x53,0x45,0x54,0x53,0x59,0x53,0x43,0x4f,0x4c,0x4f,0x52,0x53,0x54,0x45,0x4d,0x50 /* SetSysColorsTemp */
	.short 661
	.byte 0x0f,0x44,0x52,0x41,0x57,0x4d,0x45,0x4e,0x55,0x42,0x41,0x52,0x54,0x45,0x4d,0x50 /* DrawMenubarTemp */
	.short 662
	.byte 0x12,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x49,0x54,0x45,0x4d /* GetMenuDefaultItem */
	.short 663
	.byte 0x12,0x53,0x45,0x54,0x4d,0x45,0x4e,0x55,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x49,0x54,0x45,0x4d /* SetMenuDefaultItem */
	.short 664
	.byte 0x0f,0x47,0x45,0x54,0x4d,0x45,0x4e,0x55,0x49,0x54,0x45,0x4d,0x52,0x45,0x43,0x54 /* GetMenuItemRect */
	.short 665
	.byte 0x12,0x43,0x48,0x45,0x43,0x4b,0x4d,0x45,0x4e,0x55,0x52,0x41,0x44,0x49,0x4f,0x49,0x54,0x45,0x4d /* CheckMenuRadioItem */
	.short 666
	.byte 0x10,0x54,0x52,0x41,0x43,0x4b,0x50,0x4f,0x50,0x55,0x50,0x4d,0x45,0x4e,0x55,0x45,0x58 /* TrackPopupMenuEx */
	.short 667
	.byte 0x0c,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x52,0x47,0x4e /* SetWindowRgn */
	.short 668
	.byte 0x0c,0x47,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x52,0x47,0x4e /* GetWindowRgn */
	.short 669
	.byte 0x15,0x43,0x48,0x4f,0x4f,0x53,0x45,0x46,0x4f,0x4e,0x54,0x5f,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x31,0x36 /* CHOOSEFONT_CALLBACK16 */
	.short 800
	.byte 0x16,0x46,0x49,0x4e,0x44,0x52,0x45,0x50,0x4c,0x41,0x43,0x45,0x5f,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x31,0x36 /* FINDREPLACE_CALLBACK16 */
	.short 801
	.byte 0x17,0x4f,0x50,0x45,0x4e,0x46,0x49,0x4c,0x45,0x4e,0x41,0x4d,0x45,0x5f,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x31,0x36 /* OPENFILENAME_CALLBACK16 */
	.short 802
	.byte 0x13,0x50,0x52,0x49,0x4e,0x54,0x44,0x4c,0x47,0x5f,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x31,0x36 /* PRINTDLG_CALLBACK16 */
	.short 803
	.byte 0x16,0x43,0x48,0x4f,0x4f,0x53,0x45,0x43,0x4f,0x4c,0x4f,0x52,0x5f,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b,0x31,0x36 /* CHOOSECOLOR_CALLBACK16 */
	.short 804
	.byte 0x0d,0x50,0x45,0x45,0x4b,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* PeekMessage32 */
	.short 819
	.byte 0x0c,0x47,0x45,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* GetMessage32 */
	.short 820
	.byte 0x12,0x54,0x52,0x41,0x4e,0x53,0x4c,0x41,0x54,0x45,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* TranslateMessage32 */
	.short 821
	.byte 0x11,0x44,0x49,0x53,0x50,0x41,0x54,0x43,0x48,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* DispatchMessage32 */
	.short 822
	.byte 0x0f,0x43,0x41,0x4c,0x4c,0x4d,0x53,0x47,0x46,0x49,0x4c,0x54,0x45,0x52,0x33,0x32 /* CallMsgFilter32 */
	.short 823
	.byte 0x0d,0x50,0x4f,0x53,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* PostMessage32 */
	.short 825
	.byte 0x13,0x50,0x4f,0x53,0x54,0x54,0x48,0x52,0x45,0x41,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x33,0x32 /* PostThreadMessage32 */
	.short 826
	.byte 0x12,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x42,0x4f,0x58,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54 /* MessageBoxIndirect */
	.short 827
	.byte 0x16,0x4d,0x53,0x47,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41,0x4c,0x53 /* MsgThkConnectionDataLS */
	.short 851
	.byte 0x1b,0x46,0x54,0x5f,0x55,0x53,0x52,0x46,0x54,0x48,0x4b,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41 /* FT_USRFTHKTHKCONNECTIONDATA */
	.short 853
	.byte 0x1d,0x46,0x54,0x5f,0x5f,0x55,0x53,0x52,0x46,0x32,0x54,0x48,0x4b,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41 /* FT__USRF2THKTHKCONNECTIONDATA */
	.short 854
	.byte 0x18,0x55,0x53,0x52,0x33,0x32,0x54,0x48,0x4b,0x43,0x4f,0x4e,0x4e,0x45,0x43,0x54,0x49,0x4f,0x4e,0x44,0x41,0x54,0x41,0x53,0x4c /* Usr32ThkConnectionDataSL */
	.short 855
	.byte 0x0a,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c,0x49,0x4d,0x54 /* InstallIMT */
	.short 890
	.byte 0x0c,0x55,0x4e,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c,0x49,0x4d,0x54 /* UninstallIMT */
	.short 891
	.byte 0x18,0x4c,0x4f,0x41,0x44,0x53,0x59,0x53,0x54,0x45,0x4d,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x53,0x54,0x52,0x49,0x4e,0x47 /* LoadSystemLanguageString */
	.short 902
	.byte 0x14,0x43,0x48,0x41,0x4e,0x47,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47,0x54,0x45,0x4d,0x50,0x4c,0x41,0x54,0x45 /* ChangeDialogTemplate */
	.short 905
	.byte 0x0f,0x47,0x45,0x54,0x4e,0x55,0x4d,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x53 /* GetNumLanguages */
	.short 906
	.byte 0x0f,0x47,0x45,0x54,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x4e,0x41,0x4d,0x45 /* GetLanguageName */
	.short 907
	.byte 0x0f,0x53,0x45,0x54,0x57,0x49,0x4e,0x44,0x4f,0x57,0x54,0x45,0x58,0x54,0x45,0x58 /* SetWindowTextEx */
	.short 909
	.byte 0x10,0x42,0x49,0x44,0x49,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x42,0x4f,0x58,0x45,0x58 /* BiDiMessageBoxEx */
	.short 910
	.byte 0x10,0x53,0x45,0x54,0x44,0x4c,0x47,0x49,0x54,0x45,0x4d,0x54,0x45,0x58,0x54,0x45,0x58 /* SetDlgItemTextEx */
	.short 911
	.byte 0x16,0x43,0x48,0x41,0x4e,0x47,0x45,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45 /* ChangeKeyboardLanguage */
	.short 912
	.byte 0x15,0x47,0x45,0x54,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45,0x53,0x59,0x53,0x54,0x45,0x4d,0x46,0x4f,0x4e,0x54 /* GetCodePageSystemFont */
	.short 913
	.byte 0x0d,0x51,0x55,0x45,0x52,0x59,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45 /* QueryCodePage */
	.short 914
	.byte 0x0e,0x47,0x45,0x54,0x41,0x50,0x50,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45 /* GetAppCodePage */
	.short 915
	.byte 0x1b,0x43,0x52,0x45,0x41,0x54,0x45,0x44,0x49,0x41,0x4c,0x4f,0x47,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54,0x50,0x41,0x52,0x41,0x4d,0x4d,0x4c /* CreateDialogIndirectParamML */
	.short 916
	.byte 0x18,0x44,0x49,0x41,0x4c,0x4f,0x47,0x42,0x4f,0x58,0x49,0x4e,0x44,0x49,0x52,0x45,0x43,0x54,0x50,0x41,0x52,0x41,0x4d,0x4d,0x4c /* DialogBoxIndirectParamML */
	.short 918
	.byte 0x12,0x4c,0x4f,0x41,0x44,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x53,0x54,0x52,0x49,0x4e,0x47 /* LoadLanguageString */
	.short 919
	.byte 0x0e,0x53,0x45,0x54,0x41,0x50,0x50,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45 /* SetAppCodePage */
	.short 920
	.byte 0x0f,0x47,0x45,0x54,0x42,0x41,0x53,0x45,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45 /* GetBaseCodePage */
	.short 922
	.byte 0x14,0x46,0x49,0x4e,0x44,0x4c,0x41,0x4e,0x47,0x55,0x41,0x47,0x45,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* FindLanguageResource */
	.short 923
	.byte 0x16,0x43,0x48,0x41,0x4e,0x47,0x45,0x4b,0x45,0x59,0x42,0x4f,0x41,0x52,0x44,0x43,0x4f,0x44,0x45,0x50,0x41,0x47,0x45 /* ChangeKeyboardCodePage */
	.short 924
	.byte 0x0c,0x4d,0x45,0x53,0x53,0x41,0x47,0x45,0x42,0x4f,0x58,0x45,0x58 /* MessageBoxEx */
	.short 930
	.byte 0x17,0x53,0x45,0x54,0x50,0x52,0x4f,0x43,0x45,0x53,0x53,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x4c,0x41,0x59,0x4f,0x55,0x54 /* SetProcessDefaultLayout */
	.short 1000
	.byte 0x17,0x47,0x45,0x54,0x50,0x52,0x4f,0x43,0x45,0x53,0x53,0x44,0x45,0x46,0x41,0x55,0x4c,0x54,0x4c,0x41,0x59,0x4f,0x55,0x54 /* GetProcessDefaultLayout */
	.short 1001
	.byte 0x13,0x5f,0x5f,0x57,0x49,0x4e,0x45,0x5f,0x43,0x41,0x4c,0x4c,0x5f,0x57,0x4e,0x44,0x50,0x52,0x4f,0x43 /* __wine_call_wndproc */
	.short 1010
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* USER.1 - USER.7 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_USER_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_6-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_7-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* USER.10 - USER.264 */
	.byte 0xff,0x01
	.byte 0x03
	.short .L__wine_USER_10-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_11-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_12-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_13-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_14-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_15-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_16-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_17-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_18-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_19-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_20-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_21-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_22-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_23-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_24-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_25-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_26-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_27-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_28-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_29-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_31-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_32-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_33-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_34-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_35-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_36-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_37-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_38-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_39-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_40-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_41-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_42-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_43-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_44-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_45-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_46-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_47-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_48-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_49-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_50-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_51-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_52-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_53-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_54-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_55-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_56-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_57-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_58-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_59-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_60-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_61-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_62-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_63-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_64-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_65-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_66-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_67-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_68-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_69-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_70-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_71-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_72-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_73-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_74-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_75-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_76-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_77-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_78-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_79-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_80-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_81-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_82-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_83-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_84-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_85-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_86-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_87-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_88-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_89-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_90-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_91-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_92-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_93-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_94-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_95-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_96-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_97-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_98-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_99-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_100-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_101-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_102-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_103-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_104-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_105-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_106-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_107-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_108-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_109-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_110-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_111-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_112-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_113-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_114-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_115-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_116-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_117-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_118-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_119-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_120-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_121-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_122-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_123-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_124-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_125-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_126-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_127-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_128-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_129-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_130-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_131-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_132-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_133-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_134-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_135-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_136-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_137-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_138-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_139-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_140-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_141-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_142-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_143-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_144-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_145-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_146-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_147-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_148-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_149-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_150-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_151-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_152-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_153-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_154-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_155-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_156-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_157-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_158-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_159-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_160-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_161-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_162-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_163-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_164-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_165-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_166-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_167-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_168-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_169-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_170-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_171-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_172-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_173-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_174-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_175-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_176-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_177-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_178-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_179-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_180-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_181-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_182-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_183-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_184-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_185-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_186-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_187-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_188-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_189-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_190-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_191-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_192-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_193-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_194-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_195-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_196-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_197-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_198-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_199-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_200-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_201-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_202-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_203-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_204-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_205-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_206-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_207-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_208-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_209-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_210-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_211-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_212-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_213-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_214-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_215-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_216-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_217-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_218-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_219-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_220-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_221-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_222-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_223-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_224-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_225-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_226-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_227-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_228-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_229-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_230-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_231-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_232-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_233-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_234-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_235-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_236-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_237-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_238-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_239-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_240-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_241-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_242-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_243-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_244-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_245-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_246-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_247-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_248-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_249-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_250-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_251-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_252-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_253-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_254-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_255-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_256-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_257-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_258-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_259-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_260-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_261-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_262-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_263-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_264-.L__wine_spec_code_segment
	/* USER.265 - USER.294 */
	.byte 0x1e,0x01
	.byte 0x03
	.short .L__wine_USER_265-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_266-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_267-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_268-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_269-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_270-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_271-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_272-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_273-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_274-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_275-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_276-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_277-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_278-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_279-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_280-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_281-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_282-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_283-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_284-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_285-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_286-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_287-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_288-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_289-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_290-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_291-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_292-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_293-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_294-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.299 - USER.309 */
	.byte 0x0b,0x01
	.byte 0x03
	.short .L__wine_USER_299-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_300-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_301-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_302-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_303-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_304-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_305-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_306-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_307-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_308-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_309-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.314 - USER.314 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_314-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.319 - USER.326 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_USER_319-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_320-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_321-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_322-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_323-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_324-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_325-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_326-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.331 - USER.338 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_USER_331-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_332-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_333-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_334-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_335-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_336-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_337-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_338-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.343 - USER.345 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_343-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_344-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_345-.L__wine_spec_code_segment
	.byte 0x0a,0x00
	/* USER.356 - USER.359 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_USER_356-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_357-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_358-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_359-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* USER.362 - USER.362 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_362-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.364 - USER.364 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_364-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* USER.368 - USER.376 */
	.byte 0x09,0x01
	.byte 0x03
	.short .L__wine_USER_368-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_369-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_370-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_371-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_372-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_373-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_374-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_375-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_376-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.378 - USER.380 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_378-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_379-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_380-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.382 - USER.385 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_USER_382-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_383-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_384-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_385-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* USER.389 - USER.391 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_389-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_390-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_391-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* USER.394 - USER.395 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_394-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_395-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.397 - USER.400 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_USER_397-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_398-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_399-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_400-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.402 - USER.404 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_402-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_403-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_404-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.406 - USER.418 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_USER_406-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_407-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_408-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_409-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_410-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_411-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_412-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_413-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_414-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_415-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_416-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_417-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_418-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.420 - USER.423 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_USER_420-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_421-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_422-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_423-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* USER.427 - USER.438 */
	.byte 0x0c,0x01
	.byte 0x03
	.short .L__wine_USER_427-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_428-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_429-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_430-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_431-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_432-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_433-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_434-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_435-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_436-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_437-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_438-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* USER.441 - USER.441 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_441-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.443 - USER.443 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_443-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.445 - USER.452 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_USER_445-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_446-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_447-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_448-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_449-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_450-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_451-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_452-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.454 - USER.466 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_USER_454-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_455-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_456-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_457-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_458-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_459-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_460-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_461-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_462-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_463-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_464-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_465-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_466-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* USER.470 - USER.473 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_USER_470-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_471-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_472-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_473-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.475 - USER.480 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_USER_475-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_476-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_477-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_478-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_479-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_480-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.482 - USER.483 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_482-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_483-.L__wine_spec_code_segment
	.byte 0x05,0x00
	/* USER.489 - USER.490 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_489-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_490-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.492 - USER.492 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_492-.L__wine_spec_code_segment
	.byte 0x03,0x00
	/* USER.496 - USER.496 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_496-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.498 - USER.535 */
	.byte 0x26,0x01
	.byte 0x03
	.short .L__wine_USER_498-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_499-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_500-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_501-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_502-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_503-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_504-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_505-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_506-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_507-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_508-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_509-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_510-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_511-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_512-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_513-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_514-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_515-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_516-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_517-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_518-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_519-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_520-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_521-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_522-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_523-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_524-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_525-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_526-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_527-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_528-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_529-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_530-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_531-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_532-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_533-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_534-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_535-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.537 - USER.538 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_537-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_538-.L__wine_spec_code_segment
	.byte 0x38,0x00
	/* USER.595 - USER.595 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_595-.L__wine_spec_code_segment
	.byte 0x04,0x00
	/* USER.600 - USER.602 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_600-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_601-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_602-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.604 - USER.606 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_604-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_605-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_606-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.608 - USER.610 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_608-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_609-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_610-.L__wine_spec_code_segment
	.byte 0x09,0x00
	/* USER.620 - USER.621 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_620-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_621-.L__wine_spec_code_segment
	.byte 0x12,0x00
	/* USER.640 - USER.640 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_640-.L__wine_spec_code_segment
	.byte 0x09,0x00
	/* USER.650 - USER.652 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_650-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_651-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_652-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.654 - USER.669 */
	.byte 0x10,0x01
	.byte 0x03
	.short .L__wine_USER_654-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_655-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_656-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_657-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_658-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_659-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_660-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_661-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_662-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_663-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_664-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_665-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_666-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_667-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_668-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_669-.L__wine_spec_code_segment
	.byte 0x82,0x00
	/* USER.800 - USER.804 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_USER_800-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_801-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_802-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_803-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_804-.L__wine_spec_code_segment
	.byte 0x0e,0x00
	/* USER.819 - USER.823 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_USER_819-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_820-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_821-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_822-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_823-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.825 - USER.827 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_825-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_826-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_827-.L__wine_spec_code_segment
	.byte 0x17,0x00
	/* USER.851 - USER.851 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_851-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.853 - USER.855 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_853-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_854-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_855-.L__wine_spec_code_segment
	.byte 0x22,0x00
	/* USER.890 - USER.891 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_890-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_891-.L__wine_spec_code_segment
	.byte 0x0a,0x00
	/* USER.902 - USER.902 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_902-.L__wine_spec_code_segment
	.byte 0x02,0x00
	/* USER.905 - USER.907 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_905-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_906-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_907-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.909 - USER.916 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_USER_909-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_910-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_911-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_912-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_913-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_914-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_915-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_916-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.918 - USER.920 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_918-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_919-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_920-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* USER.922 - USER.924 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_USER_922-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_923-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_924-.L__wine_spec_code_segment
	.byte 0x05,0x00
	/* USER.930 - USER.930 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_930-.L__wine_spec_code_segment
	.byte 0x45,0x00
	/* USER.1000 - USER.1001 */
	.byte 0x02,0x01
	.byte 0x03
	.short .L__wine_USER_1000-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_USER_1001-.L__wine_spec_code_segment
	.byte 0x08,0x00
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_USER_1010-.L__wine_spec_code_segment
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
.L__wine_spec_callfrom16_p_long_T:
	pushl $.L__wine_spec_call16_p_l
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000006,0x00000000
.L__wine_spec_callfrom16_p_long_l:
	pushl $.L__wine_spec_call16_p_l
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000003,0x00000000
.L__wine_spec_callfrom16_p_long_lplll:
	pushl $.L__wine_spec_call16_p_lplll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $20
	.short 0x86c7
	.long 0x000036e3,0x00000000
.L__wine_spec_callfrom16_p_long_lppp:
	pushl $.L__wine_spec_call16_p_lppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x00000923,0x00000000
.L__wine_spec_callfrom16_p_long_lswl:
	pushl $.L__wine_spec_call16_p_lswl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00000653,0x00000000
.L__wine_spec_callfrom16_p_long_lwwll:
	pushl $.L__wine_spec_call16_p_lwwll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000364b,0x00000000
.L__wine_spec_callfrom16_p_long_lwwwl:
	pushl $.L__wine_spec_call16_p_lwwwl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x0000324b,0x00000000
.L__wine_spec_callfrom16_p_long_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_long_pl:
	pushl $.L__wine_spec_call16_p_pl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x0000001c,0x00000000
.L__wine_spec_callfrom16_p_long_pw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x0000000c,0x00000000
.L__wine_spec_callfrom16_p_long_pwwww:
	pushl $.L__wine_spec_call16_p_pwwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x0000124c,0x00000000
.L__wine_spec_callfrom16_p_long_sl:
	pushl $.L__wine_spec_call16_p_sl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x0000001a,0x00000000
.L__wine_spec_callfrom16_p_long_slww:
	pushl $.L__wine_spec_call16_p_slww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x0000025a,0x00000000
.L__wine_spec_callfrom16_p_long_swlp:
	pushl $.L__wine_spec_call16_p_swlp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000008ca,0x00000000
.L__wine_spec_callfrom16_p_long_tl:
	pushl $.L__wine_spec_call16_p_pl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x0000001d,0x00000000
.L__wine_spec_callfrom16_p_long_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $2
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_long_wll:
	pushl $.L__wine_spec_call16_p_wll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x000000d9,0x00000000
.L__wine_spec_callfrom16_p_long_wpwwp:
	pushl $.L__wine_spec_call16_p_wpwwp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00004261,0x00000000
.L__wine_spec_callfrom16_p_long_ws:
	pushl $.L__wine_spec_call16_p_ws
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000011,0x00000000
.L__wine_spec_callfrom16_p_long_wsl:
	pushl $.L__wine_spec_call16_p_wsl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x000000d1,0x00000000
.L__wine_spec_callfrom16_p_long_wsspssps:
	pushl $.L__wine_spec_call16_p_wsspssps
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $20
	.short 0x86c7
	.long 0x00512891,0x00000000
.L__wine_spec_callfrom16_p_long_wtwwww:
	pushl $.L__wine_spec_call16_p_wpwwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00009269,0x00000000
.L__wine_spec_callfrom16_p_long_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_long_wwll:
	pushl $.L__wine_spec_call16_p_wwll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000006c9,0x00000000
.L__wine_spec_callfrom16_p_long_wwwl:
	pushl $.L__wine_spec_call16_p_wwwl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000649,0x00000000
.L__wine_spec_callfrom16_p_long_wwwll:
	pushl $.L__wine_spec_call16_p_wwwll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00003649,0x00000000
.L__wine_spec_callfrom16_p_long_wwww:
	pushl $.L__wine_spec_call16_p_wwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000249,0x00000000
.L__wine_spec_callfrom16_p_long_wwwwl:
	pushl $.L__wine_spec_call16_p_wwwwl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00003249,0x00000000
.L__wine_spec_callfrom16_p_long_wwwwww:
	pushl $.L__wine_spec_call16_p_wwwwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00009249,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
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
.L__wine_spec_callfrom16_p_word_lllw:
	pushl $.L__wine_spec_call16_p_lllw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000002db,0x00000000
.L__wine_spec_callfrom16_p_word_llwwpwp:
	pushl $.L__wine_spec_call16_p_llwwpwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $22
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0010c25b,0x00000000
.L__wine_spec_callfrom16_p_word_lttlsssswwwl:
	pushl $.L__wine_spec_call16_p_lpplsssswwwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $34
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0949276b,0x00000019
.L__wine_spec_callfrom16_p_word_lw:
	pushl $.L__wine_spec_call16_p_lw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000000b,0x00000000
.L__wine_spec_callfrom16_p_word_lwwwlw:
	pushl $.L__wine_spec_call16_p_lwwwlw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000b24b,0x00000000
.L__wine_spec_callfrom16_p_word_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_word_pl:
	pushl $.L__wine_spec_call16_p_pl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000001c,0x00000000
.L__wine_spec_callfrom16_p_word_plw:
	pushl $.L__wine_spec_call16_p_plw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000005c,0x00000000
.L__wine_spec_callfrom16_p_word_plwl:
	pushl $.L__wine_spec_call16_p_plwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000065c,0x00000000
.L__wine_spec_callfrom16_p_word_plwlwww:
	pushl $.L__wine_spec_call16_p_plwlwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0004965c,0x00000000
.L__wine_spec_callfrom16_p_word_pp:
	pushl $.L__wine_spec_call16_p_pp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000024,0x00000000
.L__wine_spec_callfrom16_p_word_ppp:
	pushl $.L__wine_spec_call16_p_ppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000124,0x00000000
.L__wine_spec_callfrom16_p_word_ppwp:
	pushl $.L__wine_spec_call16_p_ppwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000864,0x00000000
.L__wine_spec_callfrom16_p_word_ps:
	pushl $.L__wine_spec_call16_p_ps
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000014,0x00000000
.L__wine_spec_callfrom16_p_word_pss:
	pushl $.L__wine_spec_call16_p_pss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000094,0x00000000
.L__wine_spec_callfrom16_p_word_pssss:
	pushl $.L__wine_spec_call16_p_pssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00002494,0x00000000
.L__wine_spec_callfrom16_p_word_ptp:
	pushl $.L__wine_spec_call16_p_ppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000012c,0x00000000
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
.L__wine_spec_callfrom16_p_word_pwww:
	pushl $.L__wine_spec_call16_p_pwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000024c,0x00000000
.L__wine_spec_callfrom16_p_word_pwwww:
	pushl $.L__wine_spec_call16_p_pwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000124c,0x00000000
.L__wine_spec_callfrom16_p_word_pwwwww:
	pushl $.L__wine_spec_call16_p_pwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000924c,0x00000000
.L__wine_spec_callfrom16_p_word_s:
	pushl $.L__wine_spec_call16_p_s
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $2
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000002,0x00000000
.L__wine_spec_callfrom16_p_word_sl:
	pushl $.L__wine_spec_call16_p_sl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000001a,0x00000000
.L__wine_spec_callfrom16_p_word_swss:
	pushl $.L__wine_spec_call16_p_swss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000048a,0x00000000
.L__wine_spec_callfrom16_p_word_t:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000005,0x00000000
.L__wine_spec_callfrom16_p_word_tlp:
	pushl $.L__wine_spec_call16_p_plp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000011d,0x00000000
.L__wine_spec_callfrom16_p_word_tt:
	pushl $.L__wine_spec_call16_p_pp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000002d,0x00000000
.L__wine_spec_callfrom16_p_word_ttl:
	pushl $.L__wine_spec_call16_p_ppl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000ed,0x00000000
.L__wine_spec_callfrom16_p_word_ttlsssswwwl:
	pushl $.L__wine_spec_call16_p_pplsssswwwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $30
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x092924ed,0x00000003
.L__wine_spec_callfrom16_p_word_ttt:
	pushl $.L__wine_spec_call16_p_ppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000016d,0x00000000
.L__wine_spec_callfrom16_p_word_tw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000000d,0x00000000
.L__wine_spec_callfrom16_p_word_tww:
	pushl $.L__wine_spec_call16_p_pww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0000004d,0x00000000
.L__wine_spec_callfrom16_p_word_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $2
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_word_wT:
	pushl $.L__wine_spec_call16_p_wl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000031,0x00000000
.L__wine_spec_callfrom16_p_word_wTp:
	pushl $.L__wine_spec_call16_p_wlp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000131,0x00000000
.L__wine_spec_callfrom16_p_word_wl:
	pushl $.L__wine_spec_call16_p_wl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000019,0x00000000
.L__wine_spec_callfrom16_p_word_wll:
	pushl $.L__wine_spec_call16_p_wll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000d9,0x00000000
.L__wine_spec_callfrom16_p_word_wlw:
	pushl $.L__wine_spec_call16_p_wlw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000059,0x00000000
.L__wine_spec_callfrom16_p_word_wp:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000021,0x00000000
.L__wine_spec_callfrom16_p_word_wpp:
	pushl $.L__wine_spec_call16_p_wpp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000121,0x00000000
.L__wine_spec_callfrom16_p_word_wppp:
	pushl $.L__wine_spec_call16_p_wppp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000921,0x00000000
.L__wine_spec_callfrom16_p_word_wppw:
	pushl $.L__wine_spec_call16_p_wppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000321,0x00000000
.L__wine_spec_callfrom16_p_word_wps:
	pushl $.L__wine_spec_call16_p_wps
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000a1,0x00000000
.L__wine_spec_callfrom16_p_word_wpw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000061,0x00000000
.L__wine_spec_callfrom16_p_word_wpwl:
	pushl $.L__wine_spec_call16_p_wpwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000661,0x00000000
.L__wine_spec_callfrom16_p_word_wpwll:
	pushl $.L__wine_spec_call16_p_wpwll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00003661,0x00000000
.L__wine_spec_callfrom16_p_word_wpwplwwtw:
	pushl $.L__wine_spec_call16_p_wpwplwwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $26
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x01a4b861,0x00000000
.L__wine_spec_callfrom16_p_word_wpww:
	pushl $.L__wine_spec_call16_p_wpww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000261,0x00000000
.L__wine_spec_callfrom16_p_word_wpwww:
	pushl $.L__wine_spec_call16_p_wpwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001261,0x00000000
.L__wine_spec_callfrom16_p_word_ws:
	pushl $.L__wine_spec_call16_p_ws
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000011,0x00000000
.L__wine_spec_callfrom16_p_word_wsp:
	pushl $.L__wine_spec_call16_p_wsp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000111,0x00000000
.L__wine_spec_callfrom16_p_word_wspw:
	pushl $.L__wine_spec_call16_p_wspw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000311,0x00000000
.L__wine_spec_callfrom16_p_word_wsspp:
	pushl $.L__wine_spec_call16_p_wsspp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00004891,0x00000000
.L__wine_spec_callfrom16_p_word_wssppwp:
	pushl $.L__wine_spec_call16_p_wssppwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $20
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0010c891,0x00000000
.L__wine_spec_callfrom16_p_word_wssppwpw:
	pushl $.L__wine_spec_call16_p_wssppwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $22
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0030c891,0x00000000
.L__wine_spec_callfrom16_p_word_wssw:
	pushl $.L__wine_spec_call16_p_wssw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000291,0x00000000
.L__wine_spec_callfrom16_p_word_wsw:
	pushl $.L__wine_spec_call16_p_wsw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000051,0x00000000
.L__wine_spec_callfrom16_p_word_wt:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000029,0x00000000
.L__wine_spec_callfrom16_p_word_wtspw:
	pushl $.L__wine_spec_call16_p_wpspw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000018a9,0x00000000
.L__wine_spec_callfrom16_p_word_wttw:
	pushl $.L__wine_spec_call16_p_wppw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000369,0x00000000
.L__wine_spec_callfrom16_p_word_wttww:
	pushl $.L__wine_spec_call16_p_wppww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001369,0x00000000
.L__wine_spec_callfrom16_p_word_wtw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000069,0x00000000
.L__wine_spec_callfrom16_p_word_wtwl:
	pushl $.L__wine_spec_call16_p_wpwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000669,0x00000000
.L__wine_spec_callfrom16_p_word_wtwll:
	pushl $.L__wine_spec_call16_p_wpwll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00003669,0x00000000
.L__wine_spec_callfrom16_p_word_wtwww:
	pushl $.L__wine_spec_call16_p_wpwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001269,0x00000000
.L__wine_spec_callfrom16_p_word_ww:
	pushl $.L__wine_spec_call16_p_ww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000009,0x00000000
.L__wine_spec_callfrom16_p_word_wwT:
	pushl $.L__wine_spec_call16_p_wwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000189,0x00000000
.L__wine_spec_callfrom16_p_word_wwTww:
	pushl $.L__wine_spec_call16_p_wwlww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001389,0x00000000
.L__wine_spec_callfrom16_p_word_wwl:
	pushl $.L__wine_spec_call16_p_wwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000000c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwll:
	pushl $.L__wine_spec_call16_p_wwll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000006c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwllsssss:
	pushl $.L__wine_spec_call16_p_wwllsssss
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $22
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x024926c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwllwssssw:
	pushl $.L__wine_spec_call16_p_wwllwssssw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $24
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0a4916c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwlw:
	pushl $.L__wine_spec_call16_p_wwlw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x000002c9,0x00000000
.L__wine_spec_callfrom16_p_word_wwp:
	pushl $.L__wine_spec_call16_p_wwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000109,0x00000000
.L__wine_spec_callfrom16_p_word_wwpp:
	pushl $.L__wine_spec_call16_p_wwpp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000909,0x00000000
.L__wine_spec_callfrom16_p_word_wwps:
	pushl $.L__wine_spec_call16_p_wwps
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000509,0x00000000
.L__wine_spec_callfrom16_p_word_wwpsw:
	pushl $.L__wine_spec_call16_p_wwpsw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001509,0x00000000
.L__wine_spec_callfrom16_p_word_wwpw:
	pushl $.L__wine_spec_call16_p_wwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000309,0x00000000
.L__wine_spec_callfrom16_p_word_wwpww:
	pushl $.L__wine_spec_call16_p_wwpww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001309,0x00000000
.L__wine_spec_callfrom16_p_word_wwpwwpw:
	pushl $.L__wine_spec_call16_p_wwpwwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00061309,0x00000000
.L__wine_spec_callfrom16_p_word_wwssswp:
	pushl $.L__wine_spec_call16_p_wwssswp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x0010a489,0x00000000
.L__wine_spec_callfrom16_p_word_wwssw:
	pushl $.L__wine_spec_call16_p_wwssw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001489,0x00000000
.L__wine_spec_callfrom16_p_word_wwsw:
	pushl $.L__wine_spec_call16_p_wwsw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000289,0x00000000
.L__wine_spec_callfrom16_p_word_wwt:
	pushl $.L__wine_spec_call16_p_wwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000149,0x00000000
.L__wine_spec_callfrom16_p_word_wwtt:
	pushl $.L__wine_spec_call16_p_wwpp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000b49,0x00000000
.L__wine_spec_callfrom16_p_word_wwttww:
	pushl $.L__wine_spec_call16_p_wwppww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00009b49,0x00000000
.L__wine_spec_callfrom16_p_word_wwtw:
	pushl $.L__wine_spec_call16_p_wwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000349,0x00000000
.L__wine_spec_callfrom16_p_word_www:
	pushl $.L__wine_spec_call16_p_www
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $6
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000049,0x00000000
.L__wine_spec_callfrom16_p_word_wwwl:
	pushl $.L__wine_spec_call16_p_wwwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000649,0x00000000
.L__wine_spec_callfrom16_p_word_wwwll:
	pushl $.L__wine_spec_call16_p_wwwll
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00003649,0x00000000
.L__wine_spec_callfrom16_p_word_wwwp:
	pushl $.L__wine_spec_call16_p_wwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000849,0x00000000
.L__wine_spec_callfrom16_p_word_wwwplwwtw:
	pushl $.L__wine_spec_call16_p_wwwplwwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $24
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x01a4b849,0x00000000
.L__wine_spec_callfrom16_p_word_wwwpw:
	pushl $.L__wine_spec_call16_p_wwwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001849,0x00000000
.L__wine_spec_callfrom16_p_word_wwwssssw:
	pushl $.L__wine_spec_call16_p_wwwssssw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $16
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00292449,0x00000000
.L__wine_spec_callfrom16_p_word_wwwtw:
	pushl $.L__wine_spec_call16_p_wwwpw
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001a49,0x00000000
.L__wine_spec_callfrom16_p_word_wwww:
	pushl $.L__wine_spec_call16_p_wwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $8
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwl:
	pushl $.L__wine_spec_call16_p_wwwwl
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00003249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwp:
	pushl $.L__wine_spec_call16_p_wwwwp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00004249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwww:
	pushl $.L__wine_spec_call16_p_wwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $10
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00001249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwwpp:
	pushl $.L__wine_spec_call16_p_wwwwwpp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00121249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwww:
	pushl $.L__wine_spec_call16_p_wwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $12
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00009249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwwww:
	pushl $.L__wine_spec_call16_p_wwwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $14
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00049249,0x00000000
.L__wine_spec_callfrom16_p_word_wwwwwwwww:
	pushl $.L__wine_spec_call16_p_wwwwwwwww
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $18
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x01249249,0x00000000
.L__wine_spec_callfrom16_p_regs_:
	pushl $.L__wine_spec_call16_p__regs
	lcall $0,$0
	lretw
	.byte 0x8d,0x74,0x26,0x00,0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_v_word_pt:
	pushl $.L__wine_spec_call16_v_pp
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x000001ec,0x00000000
.L__wine_USER_1:
	pushw %bp
	pushl $_MessageBox16@16
	callw .L__wine_spec_callfrom16_p_word_wttw
.L__wine_USER_2:
	pushw %bp
	pushl $___wine_stub_OldExitWindows
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_3:
	pushw %bp
	pushl $___wine_stub_EnableOEMLayer
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_4:
	pushw %bp
	pushl $___wine_stub_DisableOEMLayer
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_5:
	pushw %bp
	pushl $_InitApp16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_6:
	pushw %bp
	pushl $_PostQuitMessage16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_7:
	pushw %bp
	pushl $_ExitWindows16@8
	callw .L__wine_spec_callfrom16_p_word_lw
.L__wine_USER_10:
	pushw %bp
	pushl $_SetTimer16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_11:
	pushw %bp
	pushl $_SetSystemTimer16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_12:
	pushw %bp
	pushl $_KillTimer16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_13:
	pushw %bp
	pushl $_GetTickCount@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_14:
	pushw %bp
	pushl $_GetTimerResolution16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_15:
	pushw %bp
	pushl $_GetTickCount@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_16:
	pushw %bp
	pushl $_ClipCursor16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_17:
	pushw %bp
	pushl $_GetCursorPos16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_18:
	pushw %bp
	pushl $_SetCapture16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_19:
	pushw %bp
	pushl $_ReleaseCapture16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_20:
	pushw %bp
	pushl $_SetDoubleClickTime16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_21:
	pushw %bp
	pushl $_GetDoubleClickTime16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_22:
	pushw %bp
	pushl $_SetFocus16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_23:
	pushw %bp
	pushl $_GetFocus16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_24:
	pushw %bp
	pushl $_RemoveProp16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_25:
	pushw %bp
	pushl $_GetProp16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_26:
	pushw %bp
	pushl $_SetProp16@12
	callw .L__wine_spec_callfrom16_p_word_wtw
.L__wine_USER_27:
	pushw %bp
	pushl $_EnumProps16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_USER_28:
	pushw %bp
	pushl $_ClientToScreen16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_29:
	pushw %bp
	pushl $_ScreenToClient16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_30:
	pushw %bp
	pushl $_WindowFromPoint16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_USER_31:
	pushw %bp
	pushl $_IsIconic16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_32:
	pushw %bp
	pushl $_GetWindowRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_33:
	pushw %bp
	pushl $_GetClientRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_34:
	pushw %bp
	pushl $_EnableWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_35:
	pushw %bp
	pushl $_IsWindowEnabled16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_36:
	pushw %bp
	pushl $_GetWindowText16@12
	callw .L__wine_spec_callfrom16_p_word_wlw
.L__wine_USER_37:
	pushw %bp
	pushl $_SetWindowText16@8
	callw .L__wine_spec_callfrom16_p_word_wT
.L__wine_USER_38:
	pushw %bp
	pushl $_GetWindowTextLength16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_39:
	pushw %bp
	pushl $_BeginPaint16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_40:
	pushw %bp
	pushl $_EndPaint16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_41:
	pushw %bp
	pushl $_CreateWindow16@44
	callw .L__wine_spec_callfrom16_p_word_ttlsssswwwl
.L__wine_USER_42:
	pushw %bp
	pushl $_ShowWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_43:
	pushw %bp
	pushl $_CloseWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_44:
	pushw %bp
	pushl $_OpenIcon16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_45:
	pushw %bp
	pushl $_BringWindowToTop16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_46:
	pushw %bp
	pushl $_GetParent16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_47:
	pushw %bp
	pushl $_IsWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_48:
	pushw %bp
	pushl $_IsChild16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_49:
	pushw %bp
	pushl $_IsWindowVisible16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_50:
	pushw %bp
	pushl $_FindWindow16@8
	callw .L__wine_spec_callfrom16_p_word_tt
.L__wine_USER_51:
	pushw %bp
	pushl $___wine_stub_BEAR51
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_52:
	pushw %bp
	pushl $_AnyPopup16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_53:
	pushw %bp
	pushl $_DestroyWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_54:
	pushw %bp
	pushl $_EnumWindows16@8
	callw .L__wine_spec_callfrom16_p_word_ll
.L__wine_USER_55:
	pushw %bp
	pushl $_EnumChildWindows16@12
	callw .L__wine_spec_callfrom16_p_word_wll
.L__wine_USER_56:
	pushw %bp
	pushl $_MoveWindow16@24
	callw .L__wine_spec_callfrom16_p_word_wwwwww
.L__wine_USER_57:
	pushw %bp
	pushl $_RegisterClass16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_58:
	pushw %bp
	pushl $_GetClassName16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_59:
	pushw %bp
	pushl $_SetActiveWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_60:
	pushw %bp
	pushl $_GetActiveWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_61:
	pushw %bp
	pushl $_ScrollWindow16@20
	callw .L__wine_spec_callfrom16_p_word_wsspp
.L__wine_USER_62:
	pushw %bp
	pushl $_SetScrollPos16@16
	callw .L__wine_spec_callfrom16_p_word_wwsw
.L__wine_USER_63:
	pushw %bp
	pushl $_GetScrollPos16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_64:
	pushw %bp
	pushl $_SetScrollRange16@20
	callw .L__wine_spec_callfrom16_p_word_wwssw
.L__wine_USER_65:
	pushw %bp
	pushl $_GetScrollRange16@16
	callw .L__wine_spec_callfrom16_p_word_wwpp
.L__wine_USER_66:
	pushw %bp
	pushl $_GetDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_67:
	pushw %bp
	pushl $_GetWindowDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_68:
	pushw %bp
	pushl $_ReleaseDC16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_69:
	pushw %bp
	pushl $_SetCursor16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_70:
	pushw %bp
	pushl $_SetCursorPos16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_71:
	pushw %bp
	pushl $_ShowCursor16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_72:
	pushw %bp
	pushl $_SetRect16@20
	callw .L__wine_spec_callfrom16_p_word_pssss
.L__wine_USER_73:
	pushw %bp
	pushl $_SetRectEmpty16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_74:
	pushw %bp
	pushl $_CopyRect16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_USER_75:
	pushw %bp
	pushl $_IsRectEmpty16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_76:
	pushw %bp
	pushl $_PtInRect16@8
	callw .L__wine_spec_callfrom16_p_word_pl
.L__wine_USER_77:
	pushw %bp
	pushl $_OffsetRect16@12
	callw .L__wine_spec_callfrom16_p_word_pss
.L__wine_USER_78:
	pushw %bp
	pushl $_InflateRect16@12
	callw .L__wine_spec_callfrom16_p_word_pss
.L__wine_USER_79:
	pushw %bp
	pushl $_IntersectRect16@12
	callw .L__wine_spec_callfrom16_p_word_ppp
.L__wine_USER_80:
	pushw %bp
	pushl $_UnionRect16@12
	callw .L__wine_spec_callfrom16_p_word_ppp
.L__wine_USER_81:
	pushw %bp
	pushl $_FillRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_82:
	pushw %bp
	pushl $_InvertRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_83:
	pushw %bp
	pushl $_FrameRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_84:
	pushw %bp
	pushl $_DrawIcon16@16
	callw .L__wine_spec_callfrom16_p_word_wssw
.L__wine_USER_85:
	pushw %bp
	pushl $_DrawText16@20
	callw .L__wine_spec_callfrom16_p_word_wtspw
.L__wine_USER_86:
	pushw %bp
	pushl $_IconSize16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_87:
	pushw %bp
	pushl $_DialogBox16@16
	callw .L__wine_spec_callfrom16_p_word_wtwl
.L__wine_USER_88:
	pushw %bp
	pushl $_EndDialog16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_USER_89:
	pushw %bp
	pushl $_CreateDialog16@16
	callw .L__wine_spec_callfrom16_p_word_wtwl
.L__wine_USER_90:
	pushw %bp
	pushl $_IsDialogMessage16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_91:
	pushw %bp
	pushl $_GetDlgItem16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_92:
	pushw %bp
	pushl $_SetDlgItemText16@12
	callw .L__wine_spec_callfrom16_p_word_wwT
.L__wine_USER_93:
	pushw %bp
	pushl $_GetDlgItemText16@16
	callw .L__wine_spec_callfrom16_p_word_wwlw
.L__wine_USER_94:
	pushw %bp
	pushl $_SetDlgItemInt16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_USER_95:
	pushw %bp
	pushl $_GetDlgItemInt16@16
	callw .L__wine_spec_callfrom16_p_word_wspw
.L__wine_USER_96:
	pushw %bp
	pushl $_CheckRadioButton16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_USER_97:
	pushw %bp
	pushl $_CheckDlgButton16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_98:
	pushw %bp
	pushl $_IsDlgButtonChecked16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_99:
	pushw %bp
	pushl $_DlgDirSelect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_100:
	pushw %bp
	pushl $_DlgDirList16@20
	callw .L__wine_spec_callfrom16_p_word_wtwww
.L__wine_USER_101:
	pushw %bp
	pushl $_SendDlgItemMessage16@20
	callw .L__wine_spec_callfrom16_p_long_wwwwl
.L__wine_USER_102:
	pushw %bp
	pushl $_AdjustWindowRect16@12
	callw .L__wine_spec_callfrom16_p_word_plw
.L__wine_USER_103:
	pushw %bp
	pushl $_MapDialogRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_104:
	pushw %bp
	pushl $_MessageBeep16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_105:
	pushw %bp
	pushl $_FlashWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_106:
	pushw %bp
	pushl $_GetKeyState16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_107:
	pushw %bp
	pushl $_DefWindowProc16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_USER_108:
	pushw %bp
	pushl $_GetMessage16@16
	callw .L__wine_spec_callfrom16_p_word_pwww
.L__wine_USER_109:
	pushw %bp
	pushl $_PeekMessage16@20
	callw .L__wine_spec_callfrom16_p_word_pwwww
.L__wine_USER_110:
	pushw %bp
	pushl $_PostMessage16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_111:
	pushw %bp
	pushl $_SendMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_USER_112:
	pushw %bp
	pushl $_WaitMessage16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_113:
	pushw %bp
	pushl $_TranslateMessage16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_114:
	pushw %bp
	pushl $_DispatchMessage16@4
	callw .L__wine_spec_callfrom16_p_long_p
.L__wine_USER_115:
	pushw %bp
	pushl $_ReplyMessage16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_USER_116:
	pushw %bp
	pushl $_PostAppMessage16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_117:
	pushw %bp
	pushl $_WindowFromDC16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_118:
	pushw %bp
	pushl $_RegisterWindowMessageA@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_USER_119:
	pushw %bp
	pushl $_GetMessagePos@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_120:
	pushw %bp
	pushl $_GetMessageTime@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_121:
	pushw %bp
	pushl $_SetWindowsHook16@8
	callw .L__wine_spec_callfrom16_p_long_sl
.L__wine_USER_122:
	pushw %bp
	pushl $_CallWindowProc16@20
	callw .L__wine_spec_callfrom16_p_long_lwwwl
.L__wine_USER_123:
	pushw %bp
	pushl $_CallMsgFilter16@8
	callw .L__wine_spec_callfrom16_p_word_ps
.L__wine_USER_124:
	pushw %bp
	pushl $_UpdateWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_125:
	pushw %bp
	pushl $_InvalidateRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_126:
	pushw %bp
	pushl $_InvalidateRgn16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_127:
	pushw %bp
	pushl $_ValidateRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_128:
	pushw %bp
	pushl $_ValidateRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_129:
	pushw %bp
	pushl $_GetClassWord16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_USER_130:
	pushw %bp
	pushl $_SetClassWord16@12
	callw .L__wine_spec_callfrom16_p_word_wsw
.L__wine_USER_131:
	pushw %bp
	pushl $_GetClassLong16@8
	callw .L__wine_spec_callfrom16_p_long_ws
.L__wine_USER_132:
	pushw %bp
	pushl $_SetClassLong16@12
	callw .L__wine_spec_callfrom16_p_long_wsl
.L__wine_USER_133:
	pushw %bp
	pushl $_GetWindowWord16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_USER_134:
	pushw %bp
	pushl $_SetWindowWord16@12
	callw .L__wine_spec_callfrom16_p_word_wsw
.L__wine_USER_135:
	pushw %bp
	pushl $_GetWindowLong16@8
	callw .L__wine_spec_callfrom16_p_long_ws
.L__wine_USER_136:
	pushw %bp
	pushl $_SetWindowLong16@12
	callw .L__wine_spec_callfrom16_p_long_wsl
.L__wine_USER_137:
	pushw %bp
	pushl $_OpenClipboard16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_138:
	pushw %bp
	pushl $_CloseClipboard16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_139:
	pushw %bp
	pushl $_EmptyClipboard16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_140:
	pushw %bp
	pushl $_GetClipboardOwner16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_141:
	pushw %bp
	pushl $_SetClipboardData16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_142:
	pushw %bp
	pushl $_GetClipboardData16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_143:
	pushw %bp
	pushl $_CountClipboardFormats16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_144:
	pushw %bp
	pushl $_EnumClipboardFormats16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_145:
	pushw %bp
	pushl $_RegisterClipboardFormat16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_146:
	pushw %bp
	pushl $_GetClipboardFormatName16@12
	callw .L__wine_spec_callfrom16_p_word_wps
.L__wine_USER_147:
	pushw %bp
	pushl $_SetClipboardViewer16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_148:
	pushw %bp
	pushl $_GetClipboardViewer16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_149:
	pushw %bp
	pushl $_ChangeClipboardChain16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_150:
	pushw %bp
	pushl $_LoadMenu16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_151:
	pushw %bp
	pushl $_CreateMenu16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_152:
	pushw %bp
	pushl $_DestroyMenu16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_153:
	pushw %bp
	pushl $_ChangeMenu16@20
	callw .L__wine_spec_callfrom16_p_word_wwTww
.L__wine_USER_154:
	pushw %bp
	pushl $_CheckMenuItem16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_155:
	pushw %bp
	pushl $_EnableMenuItem16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_156:
	pushw %bp
	pushl $_GetSystemMenu16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_157:
	pushw %bp
	pushl $_GetMenu16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_158:
	pushw %bp
	pushl $_SetMenu16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_159:
	pushw %bp
	pushl $_GetSubMenu16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_160:
	pushw %bp
	pushl $_DrawMenuBar16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_161:
	pushw %bp
	pushl $_GetMenuString16@20
	callw .L__wine_spec_callfrom16_p_word_wwpsw
.L__wine_USER_162:
	pushw %bp
	pushl $_HiliteMenuItem16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_USER_163:
	pushw %bp
	pushl $_CreateCaret16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_USER_164:
	pushw %bp
	pushl $_DestroyCaret16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_165:
	pushw %bp
	pushl $_SetCaretPos16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_166:
	pushw %bp
	pushl $_HideCaret16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_167:
	pushw %bp
	pushl $_ShowCaret16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_168:
	pushw %bp
	pushl $_SetCaretBlinkTime16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_169:
	pushw %bp
	pushl $_GetCaretBlinkTime16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_170:
	pushw %bp
	pushl $_ArrangeIconicWindows16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_171:
	pushw %bp
	pushl $_WinHelp16@16
	callw .L__wine_spec_callfrom16_p_word_wtwl
.L__wine_USER_172:
	pushw %bp
	pushl $_SwitchToThisWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_173:
	pushw %bp
	pushl $_LoadCursor16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_174:
	pushw %bp
	pushl $_LoadIcon16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_175:
	pushw %bp
	pushl $_LoadBitmap16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_176:
	pushw %bp
	pushl $_LoadString16@16
	callw .L__wine_spec_callfrom16_p_word_wwps
.L__wine_USER_177:
	pushw %bp
	pushl $_LoadAccelerators16@8
	callw .L__wine_spec_callfrom16_p_word_wt
.L__wine_USER_178:
	pushw %bp
	pushl $_TranslateAccelerator16@12
	callw .L__wine_spec_callfrom16_p_word_wwp
.L__wine_USER_179:
	pushw %bp
	pushl $_GetSystemMetrics16@4
	callw .L__wine_spec_callfrom16_p_word_s
.L__wine_USER_180:
	pushw %bp
	pushl $_GetSysColor16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_USER_181:
	pushw %bp
	pushl $_SetSysColors16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_USER_182:
	pushw %bp
	pushl $_KillSystemTimer16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_183:
	pushw %bp
	pushl $_GetCaretPos16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_184:
	pushw %bp
	pushl $___wine_stub_QuerySendMessage
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_185:
	pushw %bp
	pushl $_GrayString16@36
	callw .L__wine_spec_callfrom16_p_word_wwllsssss
.L__wine_USER_186:
	pushw %bp
	pushl $_SwapMouseButton16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_187:
	pushw %bp
	pushl $_EndMenu@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_188:
	pushw %bp
	pushl $_SetSysModalWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_189:
	pushw %bp
	pushl $_GetSysModalWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_190:
	pushw %bp
	pushl $_GetUpdateRect16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_191:
	pushw %bp
	pushl $_ChildWindowFromPoint16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_USER_192:
	pushw %bp
	pushl $_InSendMessage16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_193:
	pushw %bp
	pushl $_IsClipboardFormatAvailable16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_194:
	pushw %bp
	pushl $_DlgDirSelectComboBox16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_195:
	pushw %bp
	pushl $_DlgDirListComboBox16@20
	callw .L__wine_spec_callfrom16_p_word_wpwww
.L__wine_USER_196:
	pushw %bp
	pushl $_TabbedTextOut16@32
	callw .L__wine_spec_callfrom16_p_long_wsspssps
.L__wine_USER_197:
	pushw %bp
	pushl $_GetTabbedTextExtent16@20
	callw .L__wine_spec_callfrom16_p_long_wpwwp
.L__wine_USER_198:
	pushw %bp
	pushl $_CascadeChildWindows16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_199:
	pushw %bp
	pushl $_TileChildWindows16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_200:
	pushw %bp
	pushl $_OpenComm16@12
	callw .L__wine_spec_callfrom16_p_word_tww
.L__wine_USER_201:
	pushw %bp
	pushl $_SetCommState16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_202:
	pushw %bp
	pushl $_GetCommState16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_203:
	pushw %bp
	pushl $_GetCommError16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_204:
	pushw %bp
	pushl $_ReadComm16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_205:
	pushw %bp
	pushl $_WriteComm16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_206:
	pushw %bp
	pushl $_TransmitCommChar16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_207:
	pushw %bp
	pushl $_CloseComm16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_208:
	pushw %bp
	pushl $_SetCommEventMask16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_USER_209:
	pushw %bp
	pushl $_GetCommEventMask16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_210:
	pushw %bp
	pushl $_SetCommBreak16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_211:
	pushw %bp
	pushl $_ClearCommBreak16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_212:
	pushw %bp
	pushl $_UngetCommChar16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_213:
	pushw %bp
	pushl $_BuildCommDCB16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_USER_214:
	pushw %bp
	pushl $_EscapeCommFunction16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_USER_215:
	pushw %bp
	pushl $_FlushComm16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_216:
	pushw %bp
	pushl $_UserSeeUserDo16@16
	callw .L__wine_spec_callfrom16_p_long_wwww
.L__wine_USER_217:
	pushw %bp
	pushl $_LookupMenuHandle16@8
	callw .L__wine_spec_callfrom16_p_word_ws
.L__wine_USER_218:
	pushw %bp
	pushl $_DialogBoxIndirect16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_219:
	pushw %bp
	pushl $_CreateDialogIndirect16@16
	callw .L__wine_spec_callfrom16_p_word_wpwl
.L__wine_USER_220:
	pushw %bp
	pushl $_LoadMenuIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_221:
	pushw %bp
	pushl $_ScrollDC16@28
	callw .L__wine_spec_callfrom16_p_word_wssppwp
.L__wine_USER_222:
	pushw %bp
	pushl $_GetKeyboardState16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_223:
	pushw %bp
	pushl $_SetKeyboardState16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_224:
	pushw %bp
	pushl $_GetWindowTask16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_225:
	pushw %bp
	pushl $_EnumTaskWindows16@12
	callw .L__wine_spec_callfrom16_p_word_wll
.L__wine_USER_226:
	pushw %bp
	pushl $___wine_stub_LockInput
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_227:
	pushw %bp
	pushl $_GetNextDlgGroupItem16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_228:
	pushw %bp
	pushl $_GetNextDlgTabItem16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_229:
	pushw %bp
	pushl $_GetTopWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_230:
	pushw %bp
	pushl $_GetNextWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_231:
	pushw %bp
	pushl $_GetSystemDebugState16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_232:
	pushw %bp
	pushl $_SetWindowPos16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwwww
.L__wine_USER_233:
	pushw %bp
	pushl $_SetParent16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_234:
	pushw %bp
	pushl $_UnhookWindowsHook16@8
	callw .L__wine_spec_callfrom16_p_word_sl
.L__wine_USER_235:
	pushw %bp
	pushl $_DefHookProc16@16
	callw .L__wine_spec_callfrom16_p_long_swlp
.L__wine_USER_236:
	pushw %bp
	pushl $_GetCapture16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_237:
	pushw %bp
	pushl $_GetUpdateRgn16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_238:
	pushw %bp
	pushl $_ExcludeUpdateRgn16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_239:
	pushw %bp
	pushl $_DialogBoxParam16@20
	callw .L__wine_spec_callfrom16_p_word_wtwll
.L__wine_USER_240:
	pushw %bp
	pushl $_DialogBoxIndirectParam16@20
	callw .L__wine_spec_callfrom16_p_word_wwwll
.L__wine_USER_241:
	pushw %bp
	pushl $_CreateDialogParam16@20
	callw .L__wine_spec_callfrom16_p_word_wtwll
.L__wine_USER_242:
	pushw %bp
	pushl $_CreateDialogIndirectParam16@20
	callw .L__wine_spec_callfrom16_p_word_wpwll
.L__wine_USER_243:
	pushw %bp
	pushl $_GetDialogBaseUnits16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_244:
	pushw %bp
	pushl $_EqualRect16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_USER_245:
	pushw %bp
	pushl $_EnableCommNotification16@16
	callw .L__wine_spec_callfrom16_p_word_swss
.L__wine_USER_246:
	pushw %bp
	pushl $_ExitWindowsExec16@8
	callw .L__wine_spec_callfrom16_p_word_tt
.L__wine_USER_247:
	pushw %bp
	pushl $_GetCursor16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_248:
	pushw %bp
	pushl $_GetOpenClipboardWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_249:
	pushw %bp
	pushl $_GetAsyncKeyState16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_250:
	pushw %bp
	pushl $_GetMenuState16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_251:
	pushw %bp
	pushl $_SendDriverMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_USER_252:
	pushw %bp
	pushl $_OpenDriver16@12
	callw .L__wine_spec_callfrom16_p_word_ttl
.L__wine_USER_253:
	pushw %bp
	pushl $_CloseDriver16@12
	callw .L__wine_spec_callfrom16_p_long_wll
.L__wine_USER_254:
	pushw %bp
	pushl $_GetDriverModuleHandle16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_255:
	pushw %bp
	pushl $_DefDriverProc16@20
	callw .L__wine_spec_callfrom16_p_long_lwwll
.L__wine_USER_256:
	pushw %bp
	pushl $_GetDriverInfo16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_257:
	pushw %bp
	pushl $_GetNextDriver16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_USER_258:
	pushw %bp
	pushl $_MapWindowPoints16@16
	callw .L__wine_spec_callfrom16_p_word_wwpw
.L__wine_USER_259:
	pushw %bp
	pushl $_BeginDeferWindowPos16@4
	callw .L__wine_spec_callfrom16_p_word_s
.L__wine_USER_260:
	pushw %bp
	pushl $_DeferWindowPos16@32
	callw .L__wine_spec_callfrom16_p_word_wwwssssw
.L__wine_USER_261:
	pushw %bp
	pushl $_EndDeferWindowPos16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_262:
	pushw %bp
	pushl $_GetWindow16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_263:
	pushw %bp
	pushl $_GetMenuItemCount16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_264:
	pushw %bp
	pushl $_GetMenuItemID16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_265:
	pushw %bp
	pushl $_ShowOwnedPopups16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_266:
	pushw %bp
	pushl $_SetMessageQueue16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_267:
	pushw %bp
	pushl $_ShowScrollBar16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_268:
	pushw %bp
	pushl $_GlobalAddAtom16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_USER_269:
	pushw %bp
	pushl $_GlobalDeleteAtom16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_270:
	pushw %bp
	pushl $_GlobalFindAtom16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_USER_271:
	pushw %bp
	pushl $_GlobalGetAtomName16@12
	callw .L__wine_spec_callfrom16_p_word_wps
.L__wine_USER_272:
	pushw %bp
	pushl $_IsZoomed16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_273:
	pushw %bp
	pushl $_ControlPanelInfo16@12
	callw .L__wine_spec_callfrom16_p_word_wwt
.L__wine_USER_274:
	pushw %bp
	pushl $___wine_stub_GetNextQueueWindow
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_275:
	pushw %bp
	pushl $___wine_stub_RepaintScreen
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_276:
	pushw %bp
	pushl $___wine_stub_LockMyTask
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_277:
	pushw %bp
	pushl $_GetDlgCtrlID16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_278:
	pushw %bp
	pushl $_GetDesktopHwnd16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_279:
	pushw %bp
	pushl $_SetDeskPattern16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_280:
	pushw %bp
	pushl $_SetSystemMenu16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_281:
	pushw %bp
	pushl $_GetSysColorBrush16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_282:
	pushw %bp
	pushl $_SelectPalette16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_283:
	pushw %bp
	pushl $_RealizePalette16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_284:
	pushw %bp
	pushl $_GetFreeSystemResources16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_285:
	pushw %bp
	pushl $_SetDeskWallPaper16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_286:
	pushw %bp
	pushl $_GetDesktopWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_287:
	pushw %bp
	pushl $_GetLastActivePopup16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_288:
	pushw %bp
	pushl $_GetMessageExtraInfo@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_289:
	pushw %bp
	pushl $_keybd_event16@4
	callw .L__wine_spec_callfrom16_p_regs_
.L__wine_USER_290:
	pushw %bp
	pushl $_RedrawWindow16@16
	callw .L__wine_spec_callfrom16_p_word_wpww
.L__wine_USER_291:
	pushw %bp
	pushl $_SetWindowsHookEx16@16
	callw .L__wine_spec_callfrom16_p_long_slww
.L__wine_USER_292:
	pushw %bp
	pushl $_UnhookWindowsHookEx16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_USER_293:
	pushw %bp
	pushl $_CallNextHookEx16@16
	callw .L__wine_spec_callfrom16_p_long_lswl
.L__wine_USER_294:
	pushw %bp
	pushl $_LockWindowUpdate16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_299:
	pushw %bp
	pushl $_mouse_event16@4
	callw .L__wine_spec_callfrom16_p_regs_
.L__wine_USER_300:
	pushw %bp
	pushl $___wine_stub_UnloadInstalledDrivers
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_301:
	pushw %bp
	pushl $___wine_stub_EDITWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_302:
	pushw %bp
	pushl $___wine_stub_STATICWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_303:
	pushw %bp
	pushl $___wine_stub_BUTTONWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_304:
	pushw %bp
	pushl $___wine_stub_SBWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_305:
	pushw %bp
	pushl $___wine_stub_DESKTOPWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_306:
	pushw %bp
	pushl $___wine_stub_MENUWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_307:
	pushw %bp
	pushl $___wine_stub_LBOXCTLWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_308:
	pushw %bp
	pushl $_DefDlgProc16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_USER_309:
	pushw %bp
	pushl $_GetClipCursor16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_314:
	pushw %bp
	pushl $_SignalProc16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_USER_319:
	pushw %bp
	pushl $_ScrollWindowEx16@32
	callw .L__wine_spec_callfrom16_p_word_wssppwpw
.L__wine_USER_320:
	pushw %bp
	pushl $___wine_stub_SysErrorBox
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_321:
	pushw %bp
	pushl $_SetEventHook16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_USER_322:
	pushw %bp
	pushl $___wine_stub_WinOldAppHackOMatic
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_323:
	pushw %bp
	pushl $___wine_stub_GetMessage2
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_324:
	pushw %bp
	pushl $_FillWindow16@16
	callw .L__wine_spec_callfrom16_p_word_wwww
.L__wine_USER_325:
	pushw %bp
	pushl $_PaintRect16@20
	callw .L__wine_spec_callfrom16_p_word_wwwwp
.L__wine_USER_326:
	pushw %bp
	pushl $_GetControlBrush16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_331:
	pushw %bp
	pushl $_EnableHardwareInput16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_332:
	pushw %bp
	pushl $_UserYield16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_333:
	pushw %bp
	pushl $_IsUserIdle16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_334:
	pushw %bp
	pushl $_GetQueueStatus16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_USER_335:
	pushw %bp
	pushl $_GetInputState16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_336:
	pushw %bp
	pushl $_LoadCursorIconHandler16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_337:
	pushw %bp
	pushl $_GetMouseEventProc16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_338:
	pushw %bp
	pushl $___wine_stub_ECGETDS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_343:
	pushw %bp
	pushl $___wine_stub_GetFilePortName
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_344:
	pushw %bp
	pushl $___wine_stub_COMBOBOXCTLWNDPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_345:
	pushw %bp
	pushl $___wine_stub_BEAR345
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_356:
	pushw %bp
	pushl $_LoadDIBCursorHandler16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_357:
	pushw %bp
	pushl $_LoadDIBIconHandler16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_358:
	pushw %bp
	pushl $_IsMenu16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_359:
	pushw %bp
	pushl $_GetDCEx16@12
	callw .L__wine_spec_callfrom16_p_word_wwl
.L__wine_USER_362:
	pushw %bp
	pushl $_DCHook16@16
	callw .L__wine_spec_callfrom16_p_word_wwll
.L__wine_USER_364:
	pushw %bp
	pushl $_LookupIconIdFromDirectoryEx16@20
	callw .L__wine_spec_callfrom16_p_word_pwwww
.L__wine_USER_368:
	pushw %bp
	pushl $_CopyIcon16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_369:
	pushw %bp
	pushl $_CopyCursor16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_370:
	pushw %bp
	pushl $_GetWindowPlacement16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_371:
	pushw %bp
	pushl $_SetWindowPlacement16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_372:
	pushw %bp
	pushl $___wine_stub_GetInternalIconHeader
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_373:
	pushw %bp
	pushl $_SubtractRect16@12
	callw .L__wine_spec_callfrom16_p_word_ppp
.L__wine_USER_374:
	pushw %bp
	pushl $_DllEntryPoint@24
	callw .L__wine_spec_callfrom16_p_word_lwwwlw
.L__wine_USER_375:
	pushw %bp
	pushl $___wine_stub_DrawTextEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_376:
	pushw %bp
	pushl $___wine_stub_SetMessageExtraInfo
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_378:
	pushw %bp
	pushl $___wine_stub_SetPropEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_379:
	pushw %bp
	pushl $___wine_stub_GetPropEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_380:
	pushw %bp
	pushl $___wine_stub_RemovePropEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_382:
	pushw %bp
	pushl $___wine_stub_SetWindowContextHelpID
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_383:
	pushw %bp
	pushl $___wine_stub_GetWindowContextHelpID
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_384:
	pushw %bp
	pushl $_SetMenuContextHelpId16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_385:
	pushw %bp
	pushl $_GetMenuContextHelpId16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_389:
	pushw %bp
	pushl $_LoadImage16@24
	callw .L__wine_spec_callfrom16_p_long_wtwwww
.L__wine_USER_390:
	pushw %bp
	pushl $_CopyImage16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_USER_391:
	pushw %bp
	pushl $_UserSignalProc@16
	callw .L__wine_spec_callfrom16_p_word_lllw
.L__wine_USER_394:
	pushw %bp
	pushl $_DrawIconEx16@36
	callw .L__wine_spec_callfrom16_p_word_wwwwwwwww
.L__wine_USER_395:
	pushw %bp
	pushl $_GetIconInfo16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_397:
	pushw %bp
	pushl $_RegisterClassEx16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_398:
	pushw %bp
	pushl $_GetClassInfoEx16@12
	callw .L__wine_spec_callfrom16_p_word_wTp
.L__wine_USER_399:
	pushw %bp
	pushl $_ChildWindowFromPointEx16@12
	callw .L__wine_spec_callfrom16_p_word_wlw
.L__wine_USER_400:
	pushw %bp
	pushl $_FinalUserInit16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_402:
	pushw %bp
	pushl $_GetPriorityClipboardFormat16@8
	callw .L__wine_spec_callfrom16_p_word_ps
.L__wine_USER_403:
	pushw %bp
	pushl $_UnregisterClass16@8
	callw .L__wine_spec_callfrom16_p_word_tw
.L__wine_USER_404:
	pushw %bp
	pushl $_GetClassInfo16@12
	callw .L__wine_spec_callfrom16_p_word_wTp
.L__wine_USER_406:
	pushw %bp
	pushl $_CreateCursor16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwwpp
.L__wine_USER_407:
	pushw %bp
	pushl $_CreateIcon16@28
	callw .L__wine_spec_callfrom16_p_word_wwwwwpp
.L__wine_USER_408:
	pushw %bp
	pushl $_CreateCursorIconIndirect16@16
	callw .L__wine_spec_callfrom16_p_word_wppp
.L__wine_USER_409:
	pushw %bp
	pushl $_InitThreadInput16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_410:
	pushw %bp
	pushl $_InsertMenu16@20
	callw .L__wine_spec_callfrom16_p_word_wwwwl
.L__wine_USER_411:
	pushw %bp
	pushl $_AppendMenu16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_412:
	pushw %bp
	pushl $_RemoveMenu16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_413:
	pushw %bp
	pushl $_DeleteMenu16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_414:
	pushw %bp
	pushl $_ModifyMenu16@20
	callw .L__wine_spec_callfrom16_p_word_wwwwl
.L__wine_USER_415:
	pushw %bp
	pushl $_CreatePopupMenu16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_416:
	pushw %bp
	pushl $_TrackPopupMenu16@28
	callw .L__wine_spec_callfrom16_p_word_wwssswp
.L__wine_USER_417:
	pushw %bp
	pushl $_GetMenuCheckMarkDimensions@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_418:
	pushw %bp
	pushl $_SetMenuItemBitmaps16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_USER_420:
	pushw %bp
	pushl $_wsprintf16
	callw .L__wine_spec_callfrom16_v_word_pt
.L__wine_USER_421:
	pushw %bp
	pushl $_wvsprintf16@12
	callw .L__wine_spec_callfrom16_p_word_ptp
.L__wine_USER_422:
	pushw %bp
	pushl $_DlgDirSelectEx16@16
	callw .L__wine_spec_callfrom16_p_word_wpww
.L__wine_USER_423:
	pushw %bp
	pushl $_DlgDirSelectComboBoxEx16@16
	callw .L__wine_spec_callfrom16_p_word_wpww
.L__wine_USER_427:
	pushw %bp
	pushl $_FindWindowEx16@16
	callw .L__wine_spec_callfrom16_p_word_wwtt
.L__wine_USER_428:
	pushw %bp
	pushl $___wine_stub_TileWindows
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_429:
	pushw %bp
	pushl $___wine_stub_CascadeWindows
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_430:
	pushw %bp
	pushl $_lstrcmp16@8
	callw .L__wine_spec_callfrom16_p_word_tt
.L__wine_USER_431:
	pushw %bp
	pushl $_AnsiUpper16@4
	callw .L__wine_spec_callfrom16_p_long_T
.L__wine_USER_432:
	pushw %bp
	pushl $_AnsiLower16@4
	callw .L__wine_spec_callfrom16_p_long_T
.L__wine_USER_433:
	pushw %bp
	pushl $_IsCharAlphaA@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_434:
	pushw %bp
	pushl $_IsCharAlphaNumericA@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_435:
	pushw %bp
	pushl $_IsCharUpperA@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_436:
	pushw %bp
	pushl $_IsCharLowerA@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_437:
	pushw %bp
	pushl $_AnsiUpperBuff16@8
	callw .L__wine_spec_callfrom16_p_word_tw
.L__wine_USER_438:
	pushw %bp
	pushl $_AnsiLowerBuff16@8
	callw .L__wine_spec_callfrom16_p_word_tw
.L__wine_USER_441:
	pushw %bp
	pushl $_InsertMenuItem16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_USER_443:
	pushw %bp
	pushl $___wine_stub_GetMenuItemInfo
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_445:
	pushw %bp
	pushl $_DefFrameProc16@20
	callw .L__wine_spec_callfrom16_p_long_wwwwl
.L__wine_USER_446:
	pushw %bp
	pushl $___wine_stub_SetMenuItemInfo
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_447:
	pushw %bp
	pushl $_DefMDIChildProc16@16
	callw .L__wine_spec_callfrom16_p_long_wwwl
.L__wine_USER_448:
	pushw %bp
	pushl $_DrawAnimatedRects16@16
	callw .L__wine_spec_callfrom16_p_word_wwpp
.L__wine_USER_449:
	pushw %bp
	pushl $_DrawState16@40
	callw .L__wine_spec_callfrom16_p_word_wwllwssssw
.L__wine_USER_450:
	pushw %bp
	pushl $_CreateIconFromResourceEx16@28
	callw .L__wine_spec_callfrom16_p_word_plwlwww
.L__wine_USER_451:
	pushw %bp
	pushl $_TranslateMDISysAccel16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_452:
	pushw %bp
	pushl $_CreateWindowEx16@48
	callw .L__wine_spec_callfrom16_p_word_lttlsssswwwl
.L__wine_USER_454:
	pushw %bp
	pushl $_AdjustWindowRectEx16@16
	callw .L__wine_spec_callfrom16_p_word_plwl
.L__wine_USER_455:
	pushw %bp
	pushl $_GetIconID16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_USER_456:
	pushw %bp
	pushl $_LoadIconHandler16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_457:
	pushw %bp
	pushl $_DestroyIcon16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_458:
	pushw %bp
	pushl $_DestroyCursor16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_459:
	pushw %bp
	pushl $_DumpIcon16@16
	callw .L__wine_spec_callfrom16_p_long_lppp
.L__wine_USER_460:
	pushw %bp
	pushl $_GetInternalWindowPos16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_USER_461:
	pushw %bp
	pushl $_SetInternalWindowPos16@16
	callw .L__wine_spec_callfrom16_p_word_wwpp
.L__wine_USER_462:
	pushw %bp
	pushl $_CalcChildScroll16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_463:
	pushw %bp
	pushl $_ScrollChildren16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_464:
	pushw %bp
	pushl $_DragObject16@24
	callw .L__wine_spec_callfrom16_p_long_wwwwww
.L__wine_USER_465:
	pushw %bp
	pushl $_DragDetect16@8
	callw .L__wine_spec_callfrom16_p_word_wl
.L__wine_USER_466:
	pushw %bp
	pushl $_DrawFocusRect16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_470:
	pushw %bp
	pushl $___wine_stub_StringFunc
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_471:
	pushw %bp
	pushl $_lstrcmpiA@8
	callw .L__wine_spec_callfrom16_p_word_tt
.L__wine_USER_472:
	pushw %bp
	pushl $_AnsiNext16@4
	callw .L__wine_spec_callfrom16_p_long_l
.L__wine_USER_473:
	pushw %bp
	pushl $_AnsiPrev16@8
	callw .L__wine_spec_callfrom16_p_long_tl
.L__wine_USER_475:
	pushw %bp
	pushl $_SetScrollInfo16@16
	callw .L__wine_spec_callfrom16_p_word_wspw
.L__wine_USER_476:
	pushw %bp
	pushl $_GetScrollInfo16@12
	callw .L__wine_spec_callfrom16_p_word_wsp
.L__wine_USER_477:
	pushw %bp
	pushl $_GetKeyboardLayoutName16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_478:
	pushw %bp
	pushl $___wine_stub_LoadKeyboardLayout
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_479:
	pushw %bp
	pushl $___wine_stub_MenuItemFromPoint
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_480:
	pushw %bp
	pushl $___wine_stub_GetUserLocalObjType
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_482:
	pushw %bp
	pushl $_EnableScrollBar16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_483:
	pushw %bp
	pushl $_SystemParametersInfo16@16
	callw .L__wine_spec_callfrom16_p_word_wwpw
.L__wine_USER_489:
	pushw %bp
	pushl $_stub_USER_489@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_490:
	pushw %bp
	pushl $_stub_USER_490@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_492:
	pushw %bp
	pushl $_stub_USER_492@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_496:
	pushw %bp
	pushl $_stub_USER_496@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_498:
	pushw %bp
	pushl $___wine_stub_BEAR498
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_499:
	pushw %bp
	pushl $_WNetErrorText16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_500:
	pushw %bp
	pushl $___wine_stub_FARCALLNETDRIVER
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_501:
	pushw %bp
	pushl $_WNetOpenJob16@16
	callw .L__wine_spec_callfrom16_p_word_ppwp
.L__wine_USER_502:
	pushw %bp
	pushl $_WNetCloseJob16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_USER_503:
	pushw %bp
	pushl $_WNetAbortJob16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_USER_504:
	pushw %bp
	pushl $_WNetHoldJob16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_USER_505:
	pushw %bp
	pushl $_WNetReleaseJob16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_USER_506:
	pushw %bp
	pushl $_WNetCancelJob16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_USER_507:
	pushw %bp
	pushl $_WNetSetJobCopies16@12
	callw .L__wine_spec_callfrom16_p_word_pww
.L__wine_USER_508:
	pushw %bp
	pushl $_WNetWatchQueue16@16
	callw .L__wine_spec_callfrom16_p_word_wppw
.L__wine_USER_509:
	pushw %bp
	pushl $_WNetUnwatchQueue16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_USER_510:
	pushw %bp
	pushl $_WNetLockQueueData16@12
	callw .L__wine_spec_callfrom16_p_word_ppp
.L__wine_USER_511:
	pushw %bp
	pushl $_WNetUnlockQueueData16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_512:
	pushw %bp
	pushl $_WNetGetConnection16@12
	callw .L__wine_spec_callfrom16_p_word_ppp
.L__wine_USER_513:
	pushw %bp
	pushl $_WNetGetCaps16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_514:
	pushw %bp
	pushl $_WNetDeviceMode16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_515:
	pushw %bp
	pushl $_WNetBrowseDialog16@12
	callw .L__wine_spec_callfrom16_p_word_wwp
.L__wine_USER_516:
	pushw %bp
	pushl $_WNetGetUser16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_USER_517:
	pushw %bp
	pushl $_WNetAddConnection16@12
	callw .L__wine_spec_callfrom16_p_word_ttt
.L__wine_USER_518:
	pushw %bp
	pushl $_WNetCancelConnection16@8
	callw .L__wine_spec_callfrom16_p_word_tw
.L__wine_USER_519:
	pushw %bp
	pushl $_WNetGetError16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_520:
	pushw %bp
	pushl $_WNetGetErrorText16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_USER_521:
	pushw %bp
	pushl $___wine_stub_WNetEnable
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_522:
	pushw %bp
	pushl $___wine_stub_WNetDisable
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_523:
	pushw %bp
	pushl $_WNetRestoreConnection16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_524:
	pushw %bp
	pushl $_WNetWriteJob16@12
	callw .L__wine_spec_callfrom16_p_word_wpp
.L__wine_USER_525:
	pushw %bp
	pushl $_WNetConnectDialog@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_526:
	pushw %bp
	pushl $_WNetDisconnectDialog16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_527:
	pushw %bp
	pushl $_WNetConnectionDialog16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_528:
	pushw %bp
	pushl $_WNetViewQueueDialog16@8
	callw .L__wine_spec_callfrom16_p_word_wp
.L__wine_USER_529:
	pushw %bp
	pushl $_WNetPropertyDialog16@20
	callw .L__wine_spec_callfrom16_p_word_wwwtw
.L__wine_USER_530:
	pushw %bp
	pushl $_WNetGetDirectoryType16@8
	callw .L__wine_spec_callfrom16_p_word_pp
.L__wine_USER_531:
	pushw %bp
	pushl $_WNetDirectoryNotify16@12
	callw .L__wine_spec_callfrom16_p_word_wpw
.L__wine_USER_532:
	pushw %bp
	pushl $_WNetGetPropertyText16@24
	callw .L__wine_spec_callfrom16_p_word_wwttww
.L__wine_USER_533:
	pushw %bp
	pushl $___wine_stub_WNetInitialize
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_534:
	pushw %bp
	pushl $___wine_stub_WNetLogon
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_535:
	pushw %bp
	pushl $___wine_stub_WOWWORDBREAKPROC
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_537:
	pushw %bp
	pushl $___wine_stub_MOUSEEVENT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_538:
	pushw %bp
	pushl $___wine_stub_KEYBDEVENT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_595:
	pushw %bp
	pushl $___wine_stub_OLDEXITWINDOWS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_600:
	pushw %bp
	pushl $_GetShellWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_601:
	pushw %bp
	pushl $___wine_stub_DoHotkeyStuff
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_602:
	pushw %bp
	pushl $___wine_stub_SetCheckCursorTimer
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_604:
	pushw %bp
	pushl $___wine_stub_BroadcastSystemMessage
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_605:
	pushw %bp
	pushl $___wine_stub_HackTaskMonitor
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_606:
	pushw %bp
	pushl $_FormatMessage16@28
	callw .L__wine_spec_callfrom16_p_word_llwwpwp
.L__wine_USER_608:
	pushw %bp
	pushl $_GetForegroundWindow16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_609:
	pushw %bp
	pushl $_SetForegroundWindow16@4
	callw .L__wine_spec_callfrom16_p_word_w
.L__wine_USER_610:
	pushw %bp
	pushl $_DestroyIcon32@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_620:
	pushw %bp
	pushl $_ChangeDisplaySettings16@8
	callw .L__wine_spec_callfrom16_p_long_pl
.L__wine_USER_621:
	pushw %bp
	pushl $_EnumDisplaySettings16@12
	callw .L__wine_spec_callfrom16_p_word_tlp
.L__wine_USER_640:
	pushw %bp
	pushl $_MsgWaitForMultipleObjects16@20
	callw .L__wine_spec_callfrom16_p_long_lplll
.L__wine_USER_650:
	pushw %bp
	pushl $___wine_stub_ActivateKeyboardLayout
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_651:
	pushw %bp
	pushl $___wine_stub_GetKeyboardLayout
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_652:
	pushw %bp
	pushl $___wine_stub_GetKeyboardLayoutList
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_654:
	pushw %bp
	pushl $___wine_stub_UnloadKeyboardLayout
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_655:
	pushw %bp
	pushl $___wine_stub_PostPostedMessages
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_656:
	pushw %bp
	pushl $_DrawFrameControl16@16
	callw .L__wine_spec_callfrom16_p_word_wpww
.L__wine_USER_657:
	pushw %bp
	pushl $_DrawCaptionTemp16@28
	callw .L__wine_spec_callfrom16_p_word_wwpwwpw
.L__wine_USER_658:
	pushw %bp
	pushl $___wine_stub_DispatchInput
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_659:
	pushw %bp
	pushl $_DrawEdge16@16
	callw .L__wine_spec_callfrom16_p_word_wpww
.L__wine_USER_660:
	pushw %bp
	pushl $_DrawCaption16@16
	callw .L__wine_spec_callfrom16_p_word_wwpw
.L__wine_USER_661:
	pushw %bp
	pushl $___wine_stub_SetSysColorsTemp
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_662:
	pushw %bp
	pushl $___wine_stub_DrawMenubarTemp
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_663:
	pushw %bp
	pushl $___wine_stub_GetMenuDefaultItem
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_664:
	pushw %bp
	pushl $___wine_stub_SetMenuDefaultItem
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_665:
	pushw %bp
	pushl $_GetMenuItemRect16@16
	callw .L__wine_spec_callfrom16_p_word_wwwp
.L__wine_USER_666:
	pushw %bp
	pushl $_CheckMenuRadioItem16@20
	callw .L__wine_spec_callfrom16_p_word_wwwww
.L__wine_USER_667:
	pushw %bp
	pushl $___wine_stub_TrackPopupMenuEx
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_668:
	pushw %bp
	pushl $_SetWindowRgn16@12
	callw .L__wine_spec_callfrom16_p_word_www
.L__wine_USER_669:
	pushw %bp
	pushl $___wine_stub_GetWindowRgn
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_800:
	pushw %bp
	pushl $___wine_stub_CHOOSEFONT_CALLBACK16
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_801:
	pushw %bp
	pushl $___wine_stub_FINDREPLACE_CALLBACK16
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_802:
	pushw %bp
	pushl $___wine_stub_OPENFILENAME_CALLBACK16
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_803:
	pushw %bp
	pushl $___wine_stub_PRINTDLG_CALLBACK16
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_804:
	pushw %bp
	pushl $___wine_stub_CHOOSECOLOR_CALLBACK16
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_819:
	pushw %bp
	pushl $_PeekMessage32_16@24
	callw .L__wine_spec_callfrom16_p_word_pwwwww
.L__wine_USER_820:
	pushw %bp
	pushl $_GetMessage32_16@20
	callw .L__wine_spec_callfrom16_p_long_pwwww
.L__wine_USER_821:
	pushw %bp
	pushl $_TranslateMessage32_16@8
	callw .L__wine_spec_callfrom16_p_word_pw
.L__wine_USER_822:
	pushw %bp
	pushl $_DispatchMessage32_16@8
	callw .L__wine_spec_callfrom16_p_long_pw
.L__wine_USER_823:
	pushw %bp
	pushl $_CallMsgFilter32_16@12
	callw .L__wine_spec_callfrom16_p_word_pww
.L__wine_USER_825:
	pushw %bp
	pushl $___wine_stub_PostMessage32
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_826:
	pushw %bp
	pushl $___wine_stub_PostThreadMessage32
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_827:
	pushw %bp
	pushl $_MessageBoxIndirect16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_851:
	pushw %bp
	pushl $___wine_stub_MsgThkConnectionDataLS
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_853:
	pushw %bp
	pushl $___wine_stub_FT_USRFTHKTHKCONNECTIONDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_854:
	pushw %bp
	pushl $___wine_stub_FT__USRF2THKTHKCONNECTIONDATA
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_855:
	pushw %bp
	pushl $___wine_stub_Usr32ThkConnectionDataSL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_890:
	pushw %bp
	pushl $___wine_stub_InstallIMT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_891:
	pushw %bp
	pushl $___wine_stub_UninstallIMT
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_USER_902:
	pushw %bp
	pushl $_LoadSystemLanguageString16@20
	callw .L__wine_spec_callfrom16_p_word_wwpww
.L__wine_USER_905:
	pushw %bp
	pushl $_ChangeDialogTemplate16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_906:
	pushw %bp
	pushl $_GetNumLanguages16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_907:
	pushw %bp
	pushl $_GetLanguageName16@16
	callw .L__wine_spec_callfrom16_p_word_wwpw
.L__wine_USER_909:
	pushw %bp
	pushl $_SetWindowTextEx16@12
	callw .L__wine_spec_callfrom16_p_word_wtw
.L__wine_USER_910:
	pushw %bp
	pushl $_BiDiMessageBoxEx16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_USER_911:
	pushw %bp
	pushl $_SetDlgItemTextEx16@16
	callw .L__wine_spec_callfrom16_p_word_wwtw
.L__wine_USER_912:
	pushw %bp
	pushl $_ChangeKeyboardLanguage16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_USER_913:
	pushw %bp
	pushl $_GetCodePageSystemFont16@8
	callw .L__wine_spec_callfrom16_p_word_ww
.L__wine_USER_914:
	pushw %bp
	pushl $_QueryCodePage16@16
	callw .L__wine_spec_callfrom16_p_word_wwwl
.L__wine_USER_915:
	pushw %bp
	pushl $_GetAppCodePage16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_USER_916:
	pushw %bp
	pushl $_CreateDialogIndirectParamML16@36
	callw .L__wine_spec_callfrom16_p_word_wpwplwwtw
.L__wine_USER_918:
	pushw %bp
	pushl $_DialogBoxIndirectParamML16@36
	callw .L__wine_spec_callfrom16_p_word_wwwplwwtw
.L__wine_USER_919:
	pushw %bp
	pushl $_LoadLanguageString16@20
	callw .L__wine_spec_callfrom16_p_word_wwwpw
.L__wine_USER_920:
	pushw %bp
	pushl $_SetAppCodePage16@16
	callw .L__wine_spec_callfrom16_p_long_wwww
.L__wine_USER_922:
	pushw %bp
	pushl $_GetBaseCodePage16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_USER_923:
	pushw %bp
	pushl $_FindLanguageResource16@16
	callw .L__wine_spec_callfrom16_p_word_wttw
.L__wine_USER_924:
	pushw %bp
	pushl $_ChangeKeyboardCodePage16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_USER_930:
	pushw %bp
	pushl $_MessageBoxEx16@20
	callw .L__wine_spec_callfrom16_p_word_wttww
.L__wine_USER_1000:
	pushw %bp
	pushl $_SetProcessDefaultLayout16@4
	callw .L__wine_spec_callfrom16_p_word_l
.L__wine_USER_1001:
	pushw %bp
	pushl $_GetProcessDefaultLayout16@4
	callw .L__wine_spec_callfrom16_p_word_p
.L__wine_USER_1010:
	pushw %bp
	pushl $___wine_call_wndproc@20
	callw .L__wine_spec_callfrom16_p_long_wwwll
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
	.def .L__wine_spec_call16_p__regs; .scl 2; .type 32; .endef
.L__wine_spec_call16_p__regs:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	pushl 16(%ebp)
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
	.def .L__wine_spec_call16_p_llwwpwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_llwwpwp:
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
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	pushl 14(%ecx)
	pushl 18(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lplll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lplll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 16(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lpplsssswwwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lpplsssswwwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movswl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	pushl 18(%ecx)
	movzwl 24(%ecx),%edx
	shr $3,%edx
	movzwl 22(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 28(%ecx),%edx
	shr $3,%edx
	movzwl 26(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	pushl 30(%ecx)
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
	.def .L__wine_spec_call16_p_lswl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lswl:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	pushl 8(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lw:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lwwll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lwwll:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	pushl 12(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lwwwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lwwwl:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	pushl 10(%ecx)
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lwwwlw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lwwwlw:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	pushl 12(%ecx)
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
	.def .L__wine_spec_call16_p_pl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
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
	.def .L__wine_spec_call16_p_plp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_plp:
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
	.def .L__wine_spec_call16_p_plw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_plw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
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
	.def .L__wine_spec_call16_p_plwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_plwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	pushl 6(%ecx)
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_plwlwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_plwlwww:
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
	movzwl 4(%ecx),%eax
	pushl %eax
	pushl 6(%ecx)
	movzwl 10(%ecx),%eax
	pushl %eax
	pushl 12(%ecx)
	movzwl 18(%ecx),%edx
	shr $3,%edx
	movzwl 16(%ecx),%eax
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
	.def .L__wine_spec_call16_p_ppl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
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
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pplsssswwwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pplsssswwwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movswl 12(%ecx),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	pushl 18(%ecx)
	movzwl 24(%ecx),%edx
	shr $3,%edx
	movzwl 22(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 28(%ecx),%edx
	shr $3,%edx
	movzwl 26(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppp:
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
	.def .L__wine_spec_call16_p_ppwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppwp:
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
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ps:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
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
	.def .L__wine_spec_call16_p_pss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pss:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
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
	.def .L__wine_spec_call16_p_pssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pssss:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
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
	.def .L__wine_spec_call16_p_pwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
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
	.def .L__wine_spec_call16_p_pwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwwwww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
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
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_s; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_s:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_sl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_sl:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_slww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_slww:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	pushl 4(%ecx)
	movswl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_swlp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_swlp:
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
	movswl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_swss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_swss:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
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
	.def .L__wine_spec_call16_p_wll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wll:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wlp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wlp:
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
	movzwl 8(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wppp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppp:
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
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wppw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
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
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wppww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wppww:
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
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wps:
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
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpspw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpspw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
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
	.align 4
	.def .L__wine_spec_call16_p_wpwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwplwwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwplwwpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
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
	movzwl 8(%ecx),%eax
	pushl %eax
	pushl 10(%ecx)
	movzwl 16(%ecx),%edx
	shr $3,%edx
	movzwl 14(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	movzwl 22(%ecx),%edx
	shr $3,%edx
	movzwl 20(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 24(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
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
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwwp:
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
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%edx
	shr $3,%edx
	movzwl 8(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwww:
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
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wpwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpwwww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
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
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_ws; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ws:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsl:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsp:
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
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wspw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wspw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsspp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsspp:
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
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssppwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssppwp:
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
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%edx
	shr $3,%edx
	movzwl 6(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssppwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssppwpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
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
	movswl 16(%ecx),%eax
	pushl %eax
	movswl 18(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsspssps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsspssps:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movswl 14(%ecx),%eax
	pushl %eax
	movswl 16(%ecx),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wssw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wssw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wsw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wsw:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
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
	.def .L__wine_spec_call16_p_wwll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwll:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwllsssss; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwllsssss:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	pushl 10(%ecx)
	pushl 14(%ecx)
	movzwl 18(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwllwssssw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwllwssssw:
	pushl %ebp
	movl %esp,%ebp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	pushl 12(%ecx)
	pushl 16(%ecx)
	movzwl 20(%ecx),%eax
	pushl %eax
	movzwl 22(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwlw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwlw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwlww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwlww:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movzwl 2(%ecx),%eax
	pushl %eax
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwp:
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
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwpp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwpp:
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwppww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwppww:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
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
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwps; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwps:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	movswl 0(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%edx
	shr $3,%edx
	movzwl 2(%ecx),%eax
	addl (%esi,%edx,4),%eax
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
	.def .L__wine_spec_call16_p_wwpsw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwpsw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
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
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwpww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwpww:
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwpwwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwpwwpw:
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%edx
	shr $3,%edx
	movzwl 10(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwssswp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwssswp:
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
	movzwl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
	pushl %eax
	movswl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwssw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwssw:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwsw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwsw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
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
	.align 4
	.def .L__wine_spec_call16_p_wwwll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwll:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wwwplwwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwplwwpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
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
	movzwl 8(%ecx),%eax
	pushl %eax
	pushl 10(%ecx)
	movzwl 16(%ecx),%edx
	shr $3,%edx
	movzwl 14(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 18(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%eax
	pushl %eax
	movzwl 22(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwpw:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
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
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwssssw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwssssw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	movswl 2(%ecx),%eax
	pushl %eax
	movswl 4(%ecx),%eax
	pushl %eax
	movswl 6(%ecx),%eax
	pushl %eax
	movswl 8(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wwwwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwl:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	movzwl 4(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwp:
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
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
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
	.align 4
	.def .L__wine_spec_call16_p_wwwwwpp; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwpp:
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
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwww:
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
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwww:
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
	movzwl 6(%ecx),%eax
	pushl %eax
	movzwl 8(%ecx),%eax
	pushl %eax
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_wwwwwwwww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwwwwwwww:
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
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	call *8(%ebp)
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_v_pp; .scl 2; .type 32; .endef
.L__wine_spec_call16_v_pp:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	leal 8(%ecx),%eax
	pushl %eax
	movzwl 6(%ecx),%edx
	shr $3,%edx
	movzwl 4(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 2(%ecx),%edx
	shr $3,%edx
	movzwl 0(%ecx),%eax
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
	.def ___wine_stub_OldExitWindows; .scl 2; .type 32; .endef
___wine_stub_OldExitWindows:
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
	movl $.L__wine_stub_OldExitWindows_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EnableOEMLayer; .scl 2; .type 32; .endef
___wine_stub_EnableOEMLayer:
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
	movl $.L__wine_stub_EnableOEMLayer_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DisableOEMLayer; .scl 2; .type 32; .endef
___wine_stub_DisableOEMLayer:
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
	movl $.L__wine_stub_DisableOEMLayer_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BEAR51; .scl 2; .type 32; .endef
___wine_stub_BEAR51:
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
	movl $.L__wine_stub_BEAR51_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_QuerySendMessage; .scl 2; .type 32; .endef
___wine_stub_QuerySendMessage:
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
	movl $.L__wine_stub_QuerySendMessage_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LockInput; .scl 2; .type 32; .endef
___wine_stub_LockInput:
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
	movl $.L__wine_stub_LockInput_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetNextQueueWindow; .scl 2; .type 32; .endef
___wine_stub_GetNextQueueWindow:
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
	movl $.L__wine_stub_GetNextQueueWindow_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_RepaintScreen; .scl 2; .type 32; .endef
___wine_stub_RepaintScreen:
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
	movl $.L__wine_stub_RepaintScreen_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LockMyTask; .scl 2; .type 32; .endef
___wine_stub_LockMyTask:
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
	movl $.L__wine_stub_LockMyTask_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UnloadInstalledDrivers; .scl 2; .type 32; .endef
___wine_stub_UnloadInstalledDrivers:
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
	movl $.L__wine_stub_UnloadInstalledDrivers_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_EDITWNDPROC; .scl 2; .type 32; .endef
___wine_stub_EDITWNDPROC:
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
	movl $.L__wine_stub_EDITWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_STATICWNDPROC; .scl 2; .type 32; .endef
___wine_stub_STATICWNDPROC:
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
	movl $.L__wine_stub_STATICWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BUTTONWNDPROC; .scl 2; .type 32; .endef
___wine_stub_BUTTONWNDPROC:
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
	movl $.L__wine_stub_BUTTONWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SBWNDPROC; .scl 2; .type 32; .endef
___wine_stub_SBWNDPROC:
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
	movl $.L__wine_stub_SBWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DESKTOPWNDPROC; .scl 2; .type 32; .endef
___wine_stub_DESKTOPWNDPROC:
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
	movl $.L__wine_stub_DESKTOPWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MENUWNDPROC; .scl 2; .type 32; .endef
___wine_stub_MENUWNDPROC:
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
	movl $.L__wine_stub_MENUWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LBOXCTLWNDPROC; .scl 2; .type 32; .endef
___wine_stub_LBOXCTLWNDPROC:
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
	movl $.L__wine_stub_LBOXCTLWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SysErrorBox; .scl 2; .type 32; .endef
___wine_stub_SysErrorBox:
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
	movl $.L__wine_stub_SysErrorBox_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WinOldAppHackOMatic; .scl 2; .type 32; .endef
___wine_stub_WinOldAppHackOMatic:
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
	movl $.L__wine_stub_WinOldAppHackOMatic_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetMessage2; .scl 2; .type 32; .endef
___wine_stub_GetMessage2:
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
	movl $.L__wine_stub_GetMessage2_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ECGETDS; .scl 2; .type 32; .endef
___wine_stub_ECGETDS:
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
	movl $.L__wine_stub_ECGETDS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetFilePortName; .scl 2; .type 32; .endef
___wine_stub_GetFilePortName:
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
	movl $.L__wine_stub_GetFilePortName_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_COMBOBOXCTLWNDPROC; .scl 2; .type 32; .endef
___wine_stub_COMBOBOXCTLWNDPROC:
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
	movl $.L__wine_stub_COMBOBOXCTLWNDPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BEAR345; .scl 2; .type 32; .endef
___wine_stub_BEAR345:
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
	movl $.L__wine_stub_BEAR345_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetInternalIconHeader; .scl 2; .type 32; .endef
___wine_stub_GetInternalIconHeader:
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
	movl $.L__wine_stub_GetInternalIconHeader_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DrawTextEx; .scl 2; .type 32; .endef
___wine_stub_DrawTextEx:
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
	movl $.L__wine_stub_DrawTextEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetMessageExtraInfo; .scl 2; .type 32; .endef
___wine_stub_SetMessageExtraInfo:
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
	movl $.L__wine_stub_SetMessageExtraInfo_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetPropEx; .scl 2; .type 32; .endef
___wine_stub_SetPropEx:
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
	movl $.L__wine_stub_SetPropEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetPropEx; .scl 2; .type 32; .endef
___wine_stub_GetPropEx:
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
	movl $.L__wine_stub_GetPropEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_RemovePropEx; .scl 2; .type 32; .endef
___wine_stub_RemovePropEx:
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
	movl $.L__wine_stub_RemovePropEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetWindowContextHelpID; .scl 2; .type 32; .endef
___wine_stub_SetWindowContextHelpID:
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
	movl $.L__wine_stub_SetWindowContextHelpID_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetWindowContextHelpID; .scl 2; .type 32; .endef
___wine_stub_GetWindowContextHelpID:
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
	movl $.L__wine_stub_GetWindowContextHelpID_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TileWindows; .scl 2; .type 32; .endef
___wine_stub_TileWindows:
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
	movl $.L__wine_stub_TileWindows_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CascadeWindows; .scl 2; .type 32; .endef
___wine_stub_CascadeWindows:
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
	movl $.L__wine_stub_CascadeWindows_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetMenuItemInfo; .scl 2; .type 32; .endef
___wine_stub_GetMenuItemInfo:
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
	movl $.L__wine_stub_GetMenuItemInfo_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetMenuItemInfo; .scl 2; .type 32; .endef
___wine_stub_SetMenuItemInfo:
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
	movl $.L__wine_stub_SetMenuItemInfo_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_StringFunc; .scl 2; .type 32; .endef
___wine_stub_StringFunc:
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
	movl $.L__wine_stub_StringFunc_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_LoadKeyboardLayout; .scl 2; .type 32; .endef
___wine_stub_LoadKeyboardLayout:
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
	movl $.L__wine_stub_LoadKeyboardLayout_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MenuItemFromPoint; .scl 2; .type 32; .endef
___wine_stub_MenuItemFromPoint:
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
	movl $.L__wine_stub_MenuItemFromPoint_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetUserLocalObjType; .scl 2; .type 32; .endef
___wine_stub_GetUserLocalObjType:
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
	movl $.L__wine_stub_GetUserLocalObjType_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BEAR498; .scl 2; .type 32; .endef
___wine_stub_BEAR498:
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
	movl $.L__wine_stub_BEAR498_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FARCALLNETDRIVER; .scl 2; .type 32; .endef
___wine_stub_FARCALLNETDRIVER:
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
	movl $.L__wine_stub_FARCALLNETDRIVER_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WNetEnable; .scl 2; .type 32; .endef
___wine_stub_WNetEnable:
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
	movl $.L__wine_stub_WNetEnable_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WNetDisable; .scl 2; .type 32; .endef
___wine_stub_WNetDisable:
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
	movl $.L__wine_stub_WNetDisable_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WNetInitialize; .scl 2; .type 32; .endef
___wine_stub_WNetInitialize:
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
	movl $.L__wine_stub_WNetInitialize_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WNetLogon; .scl 2; .type 32; .endef
___wine_stub_WNetLogon:
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
	movl $.L__wine_stub_WNetLogon_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_WOWWORDBREAKPROC; .scl 2; .type 32; .endef
___wine_stub_WOWWORDBREAKPROC:
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
	movl $.L__wine_stub_WOWWORDBREAKPROC_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MOUSEEVENT; .scl 2; .type 32; .endef
___wine_stub_MOUSEEVENT:
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
	movl $.L__wine_stub_MOUSEEVENT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_KEYBDEVENT; .scl 2; .type 32; .endef
___wine_stub_KEYBDEVENT:
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
	movl $.L__wine_stub_KEYBDEVENT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OLDEXITWINDOWS; .scl 2; .type 32; .endef
___wine_stub_OLDEXITWINDOWS:
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
	movl $.L__wine_stub_OLDEXITWINDOWS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DoHotkeyStuff; .scl 2; .type 32; .endef
___wine_stub_DoHotkeyStuff:
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
	movl $.L__wine_stub_DoHotkeyStuff_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetCheckCursorTimer; .scl 2; .type 32; .endef
___wine_stub_SetCheckCursorTimer:
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
	movl $.L__wine_stub_SetCheckCursorTimer_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_BroadcastSystemMessage; .scl 2; .type 32; .endef
___wine_stub_BroadcastSystemMessage:
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
	movl $.L__wine_stub_BroadcastSystemMessage_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_HackTaskMonitor; .scl 2; .type 32; .endef
___wine_stub_HackTaskMonitor:
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
	movl $.L__wine_stub_HackTaskMonitor_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_ActivateKeyboardLayout; .scl 2; .type 32; .endef
___wine_stub_ActivateKeyboardLayout:
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
	movl $.L__wine_stub_ActivateKeyboardLayout_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetKeyboardLayout; .scl 2; .type 32; .endef
___wine_stub_GetKeyboardLayout:
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
	movl $.L__wine_stub_GetKeyboardLayout_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetKeyboardLayoutList; .scl 2; .type 32; .endef
___wine_stub_GetKeyboardLayoutList:
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
	movl $.L__wine_stub_GetKeyboardLayoutList_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UnloadKeyboardLayout; .scl 2; .type 32; .endef
___wine_stub_UnloadKeyboardLayout:
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
	movl $.L__wine_stub_UnloadKeyboardLayout_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PostPostedMessages; .scl 2; .type 32; .endef
___wine_stub_PostPostedMessages:
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
	movl $.L__wine_stub_PostPostedMessages_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DispatchInput; .scl 2; .type 32; .endef
___wine_stub_DispatchInput:
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
	movl $.L__wine_stub_DispatchInput_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetSysColorsTemp; .scl 2; .type 32; .endef
___wine_stub_SetSysColorsTemp:
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
	movl $.L__wine_stub_SetSysColorsTemp_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_DrawMenubarTemp; .scl 2; .type 32; .endef
___wine_stub_DrawMenubarTemp:
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
	movl $.L__wine_stub_DrawMenubarTemp_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetMenuDefaultItem; .scl 2; .type 32; .endef
___wine_stub_GetMenuDefaultItem:
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
	movl $.L__wine_stub_GetMenuDefaultItem_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_SetMenuDefaultItem; .scl 2; .type 32; .endef
___wine_stub_SetMenuDefaultItem:
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
	movl $.L__wine_stub_SetMenuDefaultItem_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_TrackPopupMenuEx; .scl 2; .type 32; .endef
___wine_stub_TrackPopupMenuEx:
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
	movl $.L__wine_stub_TrackPopupMenuEx_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_GetWindowRgn; .scl 2; .type 32; .endef
___wine_stub_GetWindowRgn:
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
	movl $.L__wine_stub_GetWindowRgn_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CHOOSEFONT_CALLBACK16; .scl 2; .type 32; .endef
___wine_stub_CHOOSEFONT_CALLBACK16:
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
	movl $.L__wine_stub_CHOOSEFONT_CALLBACK16_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FINDREPLACE_CALLBACK16; .scl 2; .type 32; .endef
___wine_stub_FINDREPLACE_CALLBACK16:
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
	movl $.L__wine_stub_FINDREPLACE_CALLBACK16_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_OPENFILENAME_CALLBACK16; .scl 2; .type 32; .endef
___wine_stub_OPENFILENAME_CALLBACK16:
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
	movl $.L__wine_stub_OPENFILENAME_CALLBACK16_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PRINTDLG_CALLBACK16; .scl 2; .type 32; .endef
___wine_stub_PRINTDLG_CALLBACK16:
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
	movl $.L__wine_stub_PRINTDLG_CALLBACK16_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_CHOOSECOLOR_CALLBACK16; .scl 2; .type 32; .endef
___wine_stub_CHOOSECOLOR_CALLBACK16:
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
	movl $.L__wine_stub_CHOOSECOLOR_CALLBACK16_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PostMessage32; .scl 2; .type 32; .endef
___wine_stub_PostMessage32:
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
	movl $.L__wine_stub_PostMessage32_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_PostThreadMessage32; .scl 2; .type 32; .endef
___wine_stub_PostThreadMessage32:
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
	movl $.L__wine_stub_PostThreadMessage32_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_MsgThkConnectionDataLS; .scl 2; .type 32; .endef
___wine_stub_MsgThkConnectionDataLS:
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
	movl $.L__wine_stub_MsgThkConnectionDataLS_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FT_USRFTHKTHKCONNECTIONDATA; .scl 2; .type 32; .endef
___wine_stub_FT_USRFTHKTHKCONNECTIONDATA:
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
	movl $.L__wine_stub_FT_USRFTHKTHKCONNECTIONDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_FT__USRF2THKTHKCONNECTIONDATA; .scl 2; .type 32; .endef
___wine_stub_FT__USRF2THKTHKCONNECTIONDATA:
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
	movl $.L__wine_stub_FT__USRF2THKTHKCONNECTIONDATA_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_Usr32ThkConnectionDataSL; .scl 2; .type 32; .endef
___wine_stub_Usr32ThkConnectionDataSL:
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
	movl $.L__wine_stub_Usr32ThkConnectionDataSL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_InstallIMT; .scl 2; .type 32; .endef
___wine_stub_InstallIMT:
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
	movl $.L__wine_stub_InstallIMT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_UninstallIMT; .scl 2; .type 32; .endef
___wine_stub_UninstallIMT:
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
	movl $.L__wine_stub_UninstallIMT_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_OldExitWindows_string:
	.string "OldExitWindows"
.L__wine_stub_EnableOEMLayer_string:
	.string "EnableOEMLayer"
.L__wine_stub_DisableOEMLayer_string:
	.string "DisableOEMLayer"
.L__wine_stub_BEAR51_string:
	.string "BEAR51"
.L__wine_stub_QuerySendMessage_string:
	.string "QuerySendMessage"
.L__wine_stub_LockInput_string:
	.string "LockInput"
.L__wine_stub_GetNextQueueWindow_string:
	.string "GetNextQueueWindow"
.L__wine_stub_RepaintScreen_string:
	.string "RepaintScreen"
.L__wine_stub_LockMyTask_string:
	.string "LockMyTask"
.L__wine_stub_UnloadInstalledDrivers_string:
	.string "UnloadInstalledDrivers"
.L__wine_stub_EDITWNDPROC_string:
	.string "EDITWNDPROC"
.L__wine_stub_STATICWNDPROC_string:
	.string "STATICWNDPROC"
.L__wine_stub_BUTTONWNDPROC_string:
	.string "BUTTONWNDPROC"
.L__wine_stub_SBWNDPROC_string:
	.string "SBWNDPROC"
.L__wine_stub_DESKTOPWNDPROC_string:
	.string "DESKTOPWNDPROC"
.L__wine_stub_MENUWNDPROC_string:
	.string "MENUWNDPROC"
.L__wine_stub_LBOXCTLWNDPROC_string:
	.string "LBOXCTLWNDPROC"
.L__wine_stub_SysErrorBox_string:
	.string "SysErrorBox"
.L__wine_stub_WinOldAppHackOMatic_string:
	.string "WinOldAppHackOMatic"
.L__wine_stub_GetMessage2_string:
	.string "GetMessage2"
.L__wine_stub_ECGETDS_string:
	.string "ECGETDS"
.L__wine_stub_GetFilePortName_string:
	.string "GetFilePortName"
.L__wine_stub_COMBOBOXCTLWNDPROC_string:
	.string "COMBOBOXCTLWNDPROC"
.L__wine_stub_BEAR345_string:
	.string "BEAR345"
.L__wine_stub_GetInternalIconHeader_string:
	.string "GetInternalIconHeader"
.L__wine_stub_DrawTextEx_string:
	.string "DrawTextEx"
.L__wine_stub_SetMessageExtraInfo_string:
	.string "SetMessageExtraInfo"
.L__wine_stub_SetPropEx_string:
	.string "SetPropEx"
.L__wine_stub_GetPropEx_string:
	.string "GetPropEx"
.L__wine_stub_RemovePropEx_string:
	.string "RemovePropEx"
.L__wine_stub_SetWindowContextHelpID_string:
	.string "SetWindowContextHelpID"
.L__wine_stub_GetWindowContextHelpID_string:
	.string "GetWindowContextHelpID"
.L__wine_stub_TileWindows_string:
	.string "TileWindows"
.L__wine_stub_CascadeWindows_string:
	.string "CascadeWindows"
.L__wine_stub_GetMenuItemInfo_string:
	.string "GetMenuItemInfo"
.L__wine_stub_SetMenuItemInfo_string:
	.string "SetMenuItemInfo"
.L__wine_stub_StringFunc_string:
	.string "StringFunc"
.L__wine_stub_LoadKeyboardLayout_string:
	.string "LoadKeyboardLayout"
.L__wine_stub_MenuItemFromPoint_string:
	.string "MenuItemFromPoint"
.L__wine_stub_GetUserLocalObjType_string:
	.string "GetUserLocalObjType"
.L__wine_stub_BEAR498_string:
	.string "BEAR498"
.L__wine_stub_FARCALLNETDRIVER_string:
	.string "FARCALLNETDRIVER"
.L__wine_stub_WNetEnable_string:
	.string "WNetEnable"
.L__wine_stub_WNetDisable_string:
	.string "WNetDisable"
.L__wine_stub_WNetInitialize_string:
	.string "WNetInitialize"
.L__wine_stub_WNetLogon_string:
	.string "WNetLogon"
.L__wine_stub_WOWWORDBREAKPROC_string:
	.string "WOWWORDBREAKPROC"
.L__wine_stub_MOUSEEVENT_string:
	.string "MOUSEEVENT"
.L__wine_stub_KEYBDEVENT_string:
	.string "KEYBDEVENT"
.L__wine_stub_OLDEXITWINDOWS_string:
	.string "OLDEXITWINDOWS"
.L__wine_stub_DoHotkeyStuff_string:
	.string "DoHotkeyStuff"
.L__wine_stub_SetCheckCursorTimer_string:
	.string "SetCheckCursorTimer"
.L__wine_stub_BroadcastSystemMessage_string:
	.string "BroadcastSystemMessage"
.L__wine_stub_HackTaskMonitor_string:
	.string "HackTaskMonitor"
.L__wine_stub_ActivateKeyboardLayout_string:
	.string "ActivateKeyboardLayout"
.L__wine_stub_GetKeyboardLayout_string:
	.string "GetKeyboardLayout"
.L__wine_stub_GetKeyboardLayoutList_string:
	.string "GetKeyboardLayoutList"
.L__wine_stub_UnloadKeyboardLayout_string:
	.string "UnloadKeyboardLayout"
.L__wine_stub_PostPostedMessages_string:
	.string "PostPostedMessages"
.L__wine_stub_DispatchInput_string:
	.string "DispatchInput"
.L__wine_stub_SetSysColorsTemp_string:
	.string "SetSysColorsTemp"
.L__wine_stub_DrawMenubarTemp_string:
	.string "DrawMenubarTemp"
.L__wine_stub_GetMenuDefaultItem_string:
	.string "GetMenuDefaultItem"
.L__wine_stub_SetMenuDefaultItem_string:
	.string "SetMenuDefaultItem"
.L__wine_stub_TrackPopupMenuEx_string:
	.string "TrackPopupMenuEx"
.L__wine_stub_GetWindowRgn_string:
	.string "GetWindowRgn"
.L__wine_stub_CHOOSEFONT_CALLBACK16_string:
	.string "CHOOSEFONT_CALLBACK16"
.L__wine_stub_FINDREPLACE_CALLBACK16_string:
	.string "FINDREPLACE_CALLBACK16"
.L__wine_stub_OPENFILENAME_CALLBACK16_string:
	.string "OPENFILENAME_CALLBACK16"
.L__wine_stub_PRINTDLG_CALLBACK16_string:
	.string "PRINTDLG_CALLBACK16"
.L__wine_stub_CHOOSECOLOR_CALLBACK16_string:
	.string "CHOOSECOLOR_CALLBACK16"
.L__wine_stub_PostMessage32_string:
	.string "PostMessage32"
.L__wine_stub_PostThreadMessage32_string:
	.string "PostThreadMessage32"
.L__wine_stub_MsgThkConnectionDataLS_string:
	.string "MsgThkConnectionDataLS"
.L__wine_stub_FT_USRFTHKTHKCONNECTIONDATA_string:
	.string "FT_USRFTHKTHKCONNECTIONDATA"
.L__wine_stub_FT__USRF2THKTHKCONNECTIONDATA_string:
	.string "FT__USRF2THKTHKCONNECTIONDATA"
.L__wine_stub_Usr32ThkConnectionDataSL_string:
	.string "Usr32ThkConnectionDataSL"
.L__wine_stub_InstallIMT_string:
	.string "InstallIMT"
.L__wine_stub_UninstallIMT_string:
	.string "UninstallIMT"

/* export table */

	.data
	.align 4
.L__wine_spec_exports:
	.long 0
	.long 0
	.long 0
	.long .L__wine_spec_exp_names-.L__wine_spec_rva_base
	.long 1
	.long 3
	.long 3
	.long .L__wine_spec_exports_funcs-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_name_ptrs-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_ordinals-.L__wine_spec_rva_base

.L__wine_spec_exports_funcs:
	.long .L__wine_spec_dos_header
	.long .L__wine_spec_main_module
	.long _CreateIcon16@28

.L__wine_spec_exp_name_ptrs:
	.long .L__wine_spec_exp_names+16-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+29-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+52-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 2
	.short 0
	.short 1
	.short 0

.L__wine_spec_exp_names:
	.string "user.exe16.spec"
	.string "CreateIcon16"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
.L__wine_spec_relay_descr:
	.long 0xdeb90001
	.long 0,0
	.long 0
	.long __wine_spec_relay_entry_points
	.long .L__wine_spec_relay_entry_point_offsets
	.long .L__wine_spec_relay_arg_types
	.section .rodata
	.align 4
.L__wine_spec_relay_entry_point_offsets:
	.long 0
	.long 0
	.long .L__wine_spec_relay_entry_point_3-__wine_spec_relay_entry_points
	.align 4
.L__wine_spec_relay_arg_types:
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.text
__wine_spec_relay_entry_points:
	nop
	.align 4
.L__wine_spec_relay_entry_point_3:
	pushl %esp
	pushl $458754
	movl $.L__wine_spec_relay_descr,%eax
	pushl %eax
	call *4(%eax)
	ret $28

	.section .rodata
.L__wine_spec_main_module:
	.string "USER"
__end:/*?*/
