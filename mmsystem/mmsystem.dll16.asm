/* File generated automatically from ..\mmsystem\mmsystem.dll16.spec; do not edit! */
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
	.string "..\mmsystem\mmsystem.dll16.spec"

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
	.byte 0x08,0x4d,0x4d,0x53,0x59,0x53,0x54,0x45,0x4d /* MMSYSTEM */
	.short 0
	.byte 0x03,0x57,0x45,0x50 /* WEP */
	.short 1
	.byte 0x0c,0x53,0x4e,0x44,0x50,0x4c,0x41,0x59,0x53,0x4f,0x55,0x4e,0x44 /* sndPlaySound */
	.short 2
	.byte 0x09,0x50,0x4c,0x41,0x59,0x53,0x4f,0x55,0x4e,0x44 /* PlaySound */
	.short 3
	.byte 0x0d,0x44,0x4c,0x4c,0x45,0x4e,0x54,0x52,0x59,0x50,0x4f,0x49,0x4e,0x54 /* DllEntryPoint */
	.short 4
	.byte 0x12,0x4d,0x4d,0x53,0x59,0x53,0x54,0x45,0x4d,0x47,0x45,0x54,0x56,0x45,0x52,0x53,0x49,0x4f,0x4e /* mmsystemGetVersion */
	.short 5
	.byte 0x0a,0x44,0x52,0x49,0x56,0x45,0x52,0x50,0x52,0x4f,0x43 /* DriverProc */
	.short 6
	.byte 0x0e,0x57,0x4d,0x4d,0x4d,0x49,0x44,0x49,0x52,0x55,0x4e,0x4f,0x4e,0x43,0x45 /* WMMMidiRunOnce */
	.short 8
	.byte 0x0e,0x4f,0x55,0x54,0x50,0x55,0x54,0x44,0x45,0x42,0x55,0x47,0x53,0x54,0x52 /* OutputDebugStr */
	.short 30
	.byte 0x0e,0x44,0x52,0x49,0x56,0x45,0x52,0x43,0x41,0x4c,0x4c,0x42,0x41,0x43,0x4b /* DriverCallback */
	.short 31
	.byte 0x0a,0x53,0x54,0x41,0x43,0x4b,0x45,0x4e,0x54,0x45,0x52 /* StackEnter */
	.short 32
	.byte 0x0a,0x53,0x54,0x41,0x43,0x4b,0x4c,0x45,0x41,0x56,0x45 /* StackLeave */
	.short 33
	.byte 0x0c,0x4d,0x4d,0x44,0x52,0x56,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c /* MMDRVINSTALL */
	.short 34
	.byte 0x0d,0x4a,0x4f,0x59,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* joyGetNumDevs */
	.short 101
	.byte 0x0d,0x4a,0x4f,0x59,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* joyGetDevCaps */
	.short 102
	.byte 0x09,0x4a,0x4f,0x59,0x47,0x45,0x54,0x50,0x4f,0x53 /* joyGetPos */
	.short 103
	.byte 0x0f,0x4a,0x4f,0x59,0x47,0x45,0x54,0x54,0x48,0x52,0x45,0x53,0x48,0x4f,0x4c,0x44 /* joyGetThreshold */
	.short 104
	.byte 0x11,0x4a,0x4f,0x59,0x52,0x45,0x4c,0x45,0x41,0x53,0x45,0x43,0x41,0x50,0x54,0x55,0x52,0x45 /* joyReleaseCapture */
	.short 105
	.byte 0x0d,0x4a,0x4f,0x59,0x53,0x45,0x54,0x43,0x41,0x50,0x54,0x55,0x52,0x45 /* joySetCapture */
	.short 106
	.byte 0x0f,0x4a,0x4f,0x59,0x53,0x45,0x54,0x54,0x48,0x52,0x45,0x53,0x48,0x4f,0x4c,0x44 /* joySetThreshold */
	.short 107
	.byte 0x11,0x4a,0x4f,0x59,0x53,0x45,0x54,0x43,0x41,0x4c,0x49,0x42,0x52,0x41,0x54,0x49,0x4f,0x4e /* joySetCalibration */
	.short 109
	.byte 0x0b,0x4a,0x4f,0x59,0x47,0x45,0x54,0x50,0x4f,0x53,0x45,0x58 /* joyGetPosEx */
	.short 110
	.byte 0x10,0x4a,0x4f,0x59,0x43,0x4f,0x4e,0x46,0x49,0x47,0x43,0x48,0x41,0x4e,0x47,0x45,0x44 /* JOYCONFIGCHANGED */
	.short 111
	.byte 0x11,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* midiOutGetNumDevs */
	.short 201
	.byte 0x11,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* midiOutGetDevCaps */
	.short 202
	.byte 0x13,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* midiOutGetErrorText */
	.short 203
	.byte 0x0b,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x4f,0x50,0x45,0x4e /* midiOutOpen */
	.short 204
	.byte 0x0c,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x43,0x4c,0x4f,0x53,0x45 /* midiOutClose */
	.short 205
	.byte 0x14,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* midiOutPrepareHeader */
	.short 206
	.byte 0x16,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x55,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* midiOutUnprepareHeader */
	.short 207
	.byte 0x0f,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x53,0x48,0x4f,0x52,0x54,0x4d,0x53,0x47 /* midiOutShortMsg */
	.short 208
	.byte 0x0e,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x4c,0x4f,0x4e,0x47,0x4d,0x53,0x47 /* midiOutLongMsg */
	.short 209
	.byte 0x0c,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x52,0x45,0x53,0x45,0x54 /* midiOutReset */
	.short 210
	.byte 0x10,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x47,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* midiOutGetVolume */
	.short 211
	.byte 0x10,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x53,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* midiOutSetVolume */
	.short 212
	.byte 0x13,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x43,0x41,0x43,0x48,0x45,0x50,0x41,0x54,0x43,0x48,0x45,0x53 /* midiOutCachePatches */
	.short 213
	.byte 0x17,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x43,0x41,0x43,0x48,0x45,0x44,0x52,0x55,0x4d,0x50,0x41,0x54,0x43,0x48,0x45,0x53 /* midiOutCacheDrumPatches */
	.short 214
	.byte 0x0c,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x47,0x45,0x54,0x49,0x44 /* midiOutGetID */
	.short 215
	.byte 0x0e,0x4d,0x49,0x44,0x49,0x4f,0x55,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* midiOutMessage */
	.short 216
	.byte 0x12,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x50,0x52,0x4f,0x50,0x45,0x52,0x54,0x59 /* midiStreamProperty */
	.short 250
	.byte 0x0e,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x4f,0x50,0x45,0x4e /* midiStreamOpen */
	.short 251
	.byte 0x0f,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x43,0x4c,0x4f,0x53,0x45 /* midiStreamClose */
	.short 252
	.byte 0x12,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e /* midiStreamPosition */
	.short 253
	.byte 0x0d,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x4f,0x55,0x54 /* midiStreamOut */
	.short 254
	.byte 0x0f,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x50,0x41,0x55,0x53,0x45 /* midiStreamPause */
	.short 255
	.byte 0x11,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x52,0x45,0x53,0x54,0x41,0x52,0x54 /* midiStreamRestart */
	.short 256
	.byte 0x0e,0x4d,0x49,0x44,0x49,0x53,0x54,0x52,0x45,0x41,0x4d,0x53,0x54,0x4f,0x50 /* midiStreamStop */
	.short 257
	.byte 0x10,0x4d,0x49,0x44,0x49,0x49,0x4e,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* midiInGetNumDevs */
	.short 301
	.byte 0x10,0x4d,0x49,0x44,0x49,0x49,0x4e,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* midiInGetDevCaps */
	.short 302
	.byte 0x12,0x4d,0x49,0x44,0x49,0x49,0x4e,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* midiInGetErrorText */
	.short 303
	.byte 0x0a,0x4d,0x49,0x44,0x49,0x49,0x4e,0x4f,0x50,0x45,0x4e /* midiInOpen */
	.short 304
	.byte 0x0b,0x4d,0x49,0x44,0x49,0x49,0x4e,0x43,0x4c,0x4f,0x53,0x45 /* midiInClose */
	.short 305
	.byte 0x13,0x4d,0x49,0x44,0x49,0x49,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* midiInPrepareHeader */
	.short 306
	.byte 0x15,0x4d,0x49,0x44,0x49,0x49,0x4e,0x55,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* midiInUnprepareHeader */
	.short 307
	.byte 0x0f,0x4d,0x49,0x44,0x49,0x49,0x4e,0x41,0x44,0x44,0x42,0x55,0x46,0x46,0x45,0x52 /* midiInAddBuffer */
	.short 308
	.byte 0x0b,0x4d,0x49,0x44,0x49,0x49,0x4e,0x53,0x54,0x41,0x52,0x54 /* midiInStart */
	.short 309
	.byte 0x0a,0x4d,0x49,0x44,0x49,0x49,0x4e,0x53,0x54,0x4f,0x50 /* midiInStop */
	.short 310
	.byte 0x0b,0x4d,0x49,0x44,0x49,0x49,0x4e,0x52,0x45,0x53,0x45,0x54 /* midiInReset */
	.short 311
	.byte 0x0b,0x4d,0x49,0x44,0x49,0x49,0x4e,0x47,0x45,0x54,0x49,0x44 /* midiInGetID */
	.short 312
	.byte 0x0d,0x4d,0x49,0x44,0x49,0x49,0x4e,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* midiInMessage */
	.short 313
	.byte 0x0d,0x41,0x55,0x58,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* auxGetNumDevs */
	.short 350
	.byte 0x0d,0x41,0x55,0x58,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* auxGetDevCaps */
	.short 351
	.byte 0x0c,0x41,0x55,0x58,0x47,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* auxGetVolume */
	.short 352
	.byte 0x0c,0x41,0x55,0x58,0x53,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* auxSetVolume */
	.short 353
	.byte 0x0d,0x41,0x55,0x58,0x4f,0x55,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* auxOutMessage */
	.short 354
	.byte 0x11,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* waveOutGetNumDevs */
	.short 401
	.byte 0x11,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* waveOutGetDevCaps */
	.short 402
	.byte 0x13,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* waveOutGetErrorText */
	.short 403
	.byte 0x0b,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x4f,0x50,0x45,0x4e /* waveOutOpen */
	.short 404
	.byte 0x0c,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x43,0x4c,0x4f,0x53,0x45 /* waveOutClose */
	.short 405
	.byte 0x14,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* waveOutPrepareHeader */
	.short 406
	.byte 0x16,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x55,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* waveOutUnprepareHeader */
	.short 407
	.byte 0x0c,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x57,0x52,0x49,0x54,0x45 /* waveOutWrite */
	.short 408
	.byte 0x0c,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x50,0x41,0x55,0x53,0x45 /* waveOutPause */
	.short 409
	.byte 0x0e,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x52,0x45,0x53,0x54,0x41,0x52,0x54 /* waveOutRestart */
	.short 410
	.byte 0x0c,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x52,0x45,0x53,0x45,0x54 /* waveOutReset */
	.short 411
	.byte 0x12,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e /* waveOutGetPosition */
	.short 412
	.byte 0x0f,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x50,0x49,0x54,0x43,0x48 /* waveOutGetPitch */
	.short 413
	.byte 0x0f,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x53,0x45,0x54,0x50,0x49,0x54,0x43,0x48 /* waveOutSetPitch */
	.short 414
	.byte 0x10,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* waveOutGetVolume */
	.short 415
	.byte 0x10,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x53,0x45,0x54,0x56,0x4f,0x4c,0x55,0x4d,0x45 /* waveOutSetVolume */
	.short 416
	.byte 0x16,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x50,0x4c,0x41,0x59,0x42,0x41,0x43,0x4b,0x52,0x41,0x54,0x45 /* waveOutGetPlaybackRate */
	.short 417
	.byte 0x16,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x53,0x45,0x54,0x50,0x4c,0x41,0x59,0x42,0x41,0x43,0x4b,0x52,0x41,0x54,0x45 /* waveOutSetPlaybackRate */
	.short 418
	.byte 0x10,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x42,0x52,0x45,0x41,0x4b,0x4c,0x4f,0x4f,0x50 /* waveOutBreakLoop */
	.short 419
	.byte 0x0c,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x47,0x45,0x54,0x49,0x44 /* waveOutGetID */
	.short 420
	.byte 0x0e,0x57,0x41,0x56,0x45,0x4f,0x55,0x54,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* waveOutMessage */
	.short 421
	.byte 0x10,0x57,0x41,0x56,0x45,0x49,0x4e,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* waveInGetNumDevs */
	.short 501
	.byte 0x10,0x57,0x41,0x56,0x45,0x49,0x4e,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* waveInGetDevCaps */
	.short 502
	.byte 0x12,0x57,0x41,0x56,0x45,0x49,0x4e,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x54,0x45,0x58,0x54 /* waveInGetErrorText */
	.short 503
	.byte 0x0a,0x57,0x41,0x56,0x45,0x49,0x4e,0x4f,0x50,0x45,0x4e /* waveInOpen */
	.short 504
	.byte 0x0b,0x57,0x41,0x56,0x45,0x49,0x4e,0x43,0x4c,0x4f,0x53,0x45 /* waveInClose */
	.short 505
	.byte 0x13,0x57,0x41,0x56,0x45,0x49,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* waveInPrepareHeader */
	.short 506
	.byte 0x15,0x57,0x41,0x56,0x45,0x49,0x4e,0x55,0x4e,0x50,0x52,0x45,0x50,0x41,0x52,0x45,0x48,0x45,0x41,0x44,0x45,0x52 /* waveInUnprepareHeader */
	.short 507
	.byte 0x0f,0x57,0x41,0x56,0x45,0x49,0x4e,0x41,0x44,0x44,0x42,0x55,0x46,0x46,0x45,0x52 /* waveInAddBuffer */
	.short 508
	.byte 0x0b,0x57,0x41,0x56,0x45,0x49,0x4e,0x53,0x54,0x41,0x52,0x54 /* waveInStart */
	.short 509
	.byte 0x0a,0x57,0x41,0x56,0x45,0x49,0x4e,0x53,0x54,0x4f,0x50 /* waveInStop */
	.short 510
	.byte 0x0b,0x57,0x41,0x56,0x45,0x49,0x4e,0x52,0x45,0x53,0x45,0x54 /* waveInReset */
	.short 511
	.byte 0x11,0x57,0x41,0x56,0x45,0x49,0x4e,0x47,0x45,0x54,0x50,0x4f,0x53,0x49,0x54,0x49,0x4f,0x4e /* waveInGetPosition */
	.short 512
	.byte 0x0b,0x57,0x41,0x56,0x45,0x49,0x4e,0x47,0x45,0x54,0x49,0x44 /* waveInGetID */
	.short 513
	.byte 0x0d,0x57,0x41,0x56,0x45,0x49,0x4e,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* waveInMessage */
	.short 514
	.byte 0x11,0x54,0x49,0x4d,0x45,0x47,0x45,0x54,0x53,0x59,0x53,0x54,0x45,0x4d,0x54,0x49,0x4d,0x45 /* timeGetSystemTime */
	.short 601
	.byte 0x0c,0x54,0x49,0x4d,0x45,0x53,0x45,0x54,0x45,0x56,0x45,0x4e,0x54 /* timeSetEvent */
	.short 602
	.byte 0x0d,0x54,0x49,0x4d,0x45,0x4b,0x49,0x4c,0x4c,0x45,0x56,0x45,0x4e,0x54 /* timeKillEvent */
	.short 603
	.byte 0x0e,0x54,0x49,0x4d,0x45,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* timeGetDevCaps */
	.short 604
	.byte 0x0f,0x54,0x49,0x4d,0x45,0x42,0x45,0x47,0x49,0x4e,0x50,0x45,0x52,0x49,0x4f,0x44 /* timeBeginPeriod */
	.short 605
	.byte 0x0d,0x54,0x49,0x4d,0x45,0x45,0x4e,0x44,0x50,0x45,0x52,0x49,0x4f,0x44 /* timeEndPeriod */
	.short 606
	.byte 0x0b,0x54,0x49,0x4d,0x45,0x47,0x45,0x54,0x54,0x49,0x4d,0x45 /* timeGetTime */
	.short 607
	.byte 0x0e,0x4d,0x43,0x49,0x53,0x45,0x4e,0x44,0x43,0x4f,0x4d,0x4d,0x41,0x4e,0x44 /* mciSendCommand */
	.short 701
	.byte 0x0d,0x4d,0x43,0x49,0x53,0x45,0x4e,0x44,0x53,0x54,0x52,0x49,0x4e,0x47 /* mciSendString */
	.short 702
	.byte 0x0e,0x4d,0x43,0x49,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45,0x49,0x44 /* mciGetDeviceID */
	.short 703
	.byte 0x16,0x4d,0x43,0x49,0x4c,0x4f,0x41,0x44,0x43,0x4f,0x4d,0x4d,0x41,0x4e,0x44,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* mciLoadCommandResource */
	.short 705
	.byte 0x11,0x4d,0x43,0x49,0x47,0x45,0x54,0x45,0x52,0x52,0x4f,0x52,0x53,0x54,0x52,0x49,0x4e,0x47 /* mciGetErrorString */
	.short 706
	.byte 0x10,0x4d,0x43,0x49,0x53,0x45,0x54,0x44,0x52,0x49,0x56,0x45,0x52,0x44,0x41,0x54,0x41 /* mciSetDriverData */
	.short 707
	.byte 0x10,0x4d,0x43,0x49,0x47,0x45,0x54,0x44,0x52,0x49,0x56,0x45,0x52,0x44,0x41,0x54,0x41 /* mciGetDriverData */
	.short 708
	.byte 0x0e,0x4d,0x43,0x49,0x44,0x52,0x49,0x56,0x45,0x52,0x59,0x49,0x45,0x4c,0x44 /* mciDriverYield */
	.short 710
	.byte 0x0f,0x4d,0x43,0x49,0x44,0x52,0x49,0x56,0x45,0x52,0x4e,0x4f,0x54,0x49,0x46,0x59 /* mciDriverNotify */
	.short 711
	.byte 0x0a,0x4d,0x43,0x49,0x45,0x58,0x45,0x43,0x55,0x54,0x45 /* mciExecute */
	.short 712
	.byte 0x16,0x4d,0x43,0x49,0x46,0x52,0x45,0x45,0x43,0x4f,0x4d,0x4d,0x41,0x4e,0x44,0x52,0x45,0x53,0x4f,0x55,0x52,0x43,0x45 /* mciFreeCommandResource */
	.short 713
	.byte 0x0f,0x4d,0x43,0x49,0x53,0x45,0x54,0x59,0x49,0x45,0x4c,0x44,0x50,0x52,0x4f,0x43 /* mciSetYieldProc */
	.short 714
	.byte 0x1b,0x4d,0x43,0x49,0x47,0x45,0x54,0x44,0x45,0x56,0x49,0x43,0x45,0x49,0x44,0x46,0x52,0x4f,0x4d,0x45,0x4c,0x45,0x4d,0x45,0x4e,0x54,0x49,0x44 /* mciGetDeviceIDFromElementID */
	.short 715
	.byte 0x0f,0x4d,0x43,0x49,0x47,0x45,0x54,0x59,0x49,0x45,0x4c,0x44,0x50,0x52,0x4f,0x43 /* mciGetYieldProc */
	.short 716
	.byte 0x11,0x4d,0x43,0x49,0x47,0x45,0x54,0x43,0x52,0x45,0x41,0x54,0x4f,0x52,0x54,0x41,0x53,0x4b /* mciGetCreatorTask */
	.short 717
	.byte 0x0f,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x4e,0x55,0x4d,0x44,0x45,0x56,0x53 /* mixerGetNumDevs */
	.short 800
	.byte 0x0f,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x44,0x45,0x56,0x43,0x41,0x50,0x53 /* mixerGetDevCaps */
	.short 801
	.byte 0x09,0x4d,0x49,0x58,0x45,0x52,0x4f,0x50,0x45,0x4e /* mixerOpen */
	.short 802
	.byte 0x0a,0x4d,0x49,0x58,0x45,0x52,0x43,0x4c,0x4f,0x53,0x45 /* mixerClose */
	.short 803
	.byte 0x0c,0x4d,0x49,0x58,0x45,0x52,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* mixerMessage */
	.short 804
	.byte 0x10,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x4c,0x49,0x4e,0x45,0x49,0x4e,0x46,0x4f /* mixerGetLineInfo */
	.short 805
	.byte 0x0a,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x49,0x44 /* mixerGetID */
	.short 806
	.byte 0x14,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x4c,0x49,0x4e,0x45,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c,0x53 /* mixerGetLineControls */
	.short 807
	.byte 0x16,0x4d,0x49,0x58,0x45,0x52,0x47,0x45,0x54,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c,0x44,0x45,0x54,0x41,0x49,0x4c,0x53 /* mixerGetControlDetails */
	.short 808
	.byte 0x16,0x4d,0x49,0x58,0x45,0x52,0x53,0x45,0x54,0x43,0x4f,0x4e,0x54,0x52,0x4f,0x4c,0x44,0x45,0x54,0x41,0x49,0x4c,0x53 /* mixerSetControlDetails */
	.short 809
	.byte 0x0c,0x4d,0x4d,0x54,0x41,0x53,0x4b,0x43,0x52,0x45,0x41,0x54,0x45 /* mmTaskCreate */
	.short 900
	.byte 0x0b,0x4d,0x4d,0x54,0x41,0x53,0x4b,0x42,0x4c,0x4f,0x43,0x4b /* mmTaskBlock */
	.short 902
	.byte 0x0c,0x4d,0x4d,0x54,0x41,0x53,0x4b,0x53,0x49,0x47,0x4e,0x41,0x4c /* mmTaskSignal */
	.short 903
	.byte 0x10,0x4d,0x4d,0x47,0x45,0x54,0x43,0x55,0x52,0x52,0x45,0x4e,0x54,0x54,0x41,0x53,0x4b /* mmGetCurrentTask */
	.short 904
	.byte 0x0b,0x4d,0x4d,0x54,0x41,0x53,0x4b,0x59,0x49,0x45,0x4c,0x44 /* mmTaskYield */
	.short 905
	.byte 0x07,0x44,0x52,0x56,0x4f,0x50,0x45,0x4e /* DrvOpen */
	.short 1100
	.byte 0x08,0x44,0x52,0x56,0x43,0x4c,0x4f,0x53,0x45 /* DrvClose */
	.short 1101
	.byte 0x0e,0x44,0x52,0x56,0x53,0x45,0x4e,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* DrvSendMessage */
	.short 1102
	.byte 0x12,0x44,0x52,0x56,0x47,0x45,0x54,0x4d,0x4f,0x44,0x55,0x4c,0x45,0x48,0x41,0x4e,0x44,0x4c,0x45 /* DrvGetModuleHandle */
	.short 1103
	.byte 0x10,0x44,0x52,0x56,0x44,0x45,0x46,0x44,0x52,0x49,0x56,0x45,0x52,0x50,0x52,0x4f,0x43 /* DrvDefDriverProc */
	.short 1104
	.byte 0x0e,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x43,0x52,0x45,0x41,0x54,0x45 /* mmThreadCreate */
	.short 1120
	.byte 0x0e,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x53,0x49,0x47,0x4e,0x41,0x4c /* mmThreadSignal */
	.short 1121
	.byte 0x0d,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x42,0x4c,0x4f,0x43,0x4b /* mmThreadBlock */
	.short 1122
	.byte 0x11,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x49,0x53,0x43,0x55,0x52,0x52,0x45,0x4e,0x54 /* mmThreadIsCurrent */
	.short 1123
	.byte 0x0f,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x49,0x53,0x56,0x41,0x4c,0x49,0x44 /* mmThreadIsValid */
	.short 1124
	.byte 0x0f,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x47,0x45,0x54,0x54,0x41,0x53,0x4b /* mmThreadGetTask */
	.short 1125
	.byte 0x18,0x4d,0x4d,0x53,0x48,0x4f,0x57,0x4d,0x4d,0x43,0x50,0x4c,0x50,0x52,0x4f,0x50,0x45,0x52,0x54,0x59,0x53,0x48,0x45,0x45,0x54 /* mmShowMMCPLPropertySheet */
	.short 1150
	.byte 0x08,0x4d,0x4d,0x49,0x4f,0x4f,0x50,0x45,0x4e /* mmioOpen */
	.short 1210
	.byte 0x09,0x4d,0x4d,0x49,0x4f,0x43,0x4c,0x4f,0x53,0x45 /* mmioClose */
	.short 1211
	.byte 0x08,0x4d,0x4d,0x49,0x4f,0x52,0x45,0x41,0x44 /* mmioRead */
	.short 1212
	.byte 0x09,0x4d,0x4d,0x49,0x4f,0x57,0x52,0x49,0x54,0x45 /* mmioWrite */
	.short 1213
	.byte 0x08,0x4d,0x4d,0x49,0x4f,0x53,0x45,0x45,0x4b /* mmioSeek */
	.short 1214
	.byte 0x0b,0x4d,0x4d,0x49,0x4f,0x47,0x45,0x54,0x49,0x4e,0x46,0x4f /* mmioGetInfo */
	.short 1215
	.byte 0x0b,0x4d,0x4d,0x49,0x4f,0x53,0x45,0x54,0x49,0x4e,0x46,0x4f /* mmioSetInfo */
	.short 1216
	.byte 0x0d,0x4d,0x4d,0x49,0x4f,0x53,0x45,0x54,0x42,0x55,0x46,0x46,0x45,0x52 /* mmioSetBuffer */
	.short 1217
	.byte 0x09,0x4d,0x4d,0x49,0x4f,0x46,0x4c,0x55,0x53,0x48 /* mmioFlush */
	.short 1218
	.byte 0x0b,0x4d,0x4d,0x49,0x4f,0x41,0x44,0x56,0x41,0x4e,0x43,0x45 /* mmioAdvance */
	.short 1219
	.byte 0x12,0x4d,0x4d,0x49,0x4f,0x53,0x54,0x52,0x49,0x4e,0x47,0x54,0x4f,0x46,0x4f,0x55,0x52,0x43,0x43 /* mmioStringToFOURCC */
	.short 1220
	.byte 0x11,0x4d,0x4d,0x49,0x4f,0x49,0x4e,0x53,0x54,0x41,0x4c,0x4c,0x49,0x4f,0x50,0x52,0x4f,0x43 /* mmioInstallIOProc */
	.short 1221
	.byte 0x0f,0x4d,0x4d,0x49,0x4f,0x53,0x45,0x4e,0x44,0x4d,0x45,0x53,0x53,0x41,0x47,0x45 /* mmioSendMessage */
	.short 1222
	.byte 0x0b,0x4d,0x4d,0x49,0x4f,0x44,0x45,0x53,0x43,0x45,0x4e,0x44 /* mmioDescend */
	.short 1223
	.byte 0x0a,0x4d,0x4d,0x49,0x4f,0x41,0x53,0x43,0x45,0x4e,0x44 /* mmioAscend */
	.short 1224
	.byte 0x0f,0x4d,0x4d,0x49,0x4f,0x43,0x52,0x45,0x41,0x54,0x45,0x43,0x48,0x55,0x4e,0x4b /* mmioCreateChunk */
	.short 1225
	.byte 0x0a,0x4d,0x4d,0x49,0x4f,0x52,0x45,0x4e,0x41,0x4d,0x45 /* mmioRename */
	.short 1226
	.byte 0x19,0x5f,0x5f,0x57,0x49,0x4e,0x45,0x5f,0x4d,0x4d,0x54,0x48,0x52,0x45,0x41,0x44,0x45,0x4e,0x54,0x52,0x59,0x50,0x4f,0x49,0x4e,0x54 /* __wine_mmThreadEntryPoint */
	.short 2047
	.byte 0

	.align 2
.L__wine_spec_ne_modtab:
.L__wine_spec_ne_imptab:
	.byte 0,0

.L__wine_spec_ne_enttab:
	/* MMSYSTEM.1 - MMSYSTEM.6 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_1-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_2-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_3-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_4-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_5-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_6-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* MMSYSTEM.8 - MMSYSTEM.8 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_8-.L__wine_spec_code_segment
	.byte 0x15,0x00
	/* MMSYSTEM.30 - MMSYSTEM.34 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_30-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_31-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_32-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_33-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_34-.L__wine_spec_code_segment
	.byte 0x42,0x00
	/* MMSYSTEM.101 - MMSYSTEM.107 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_101-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_102-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_103-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_104-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_105-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_106-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_107-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* MMSYSTEM.109 - MMSYSTEM.111 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_109-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_110-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_111-.L__wine_spec_code_segment
	.byte 0x59,0x00
	/* MMSYSTEM.201 - MMSYSTEM.216 */
	.byte 0x10,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_201-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_202-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_203-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_204-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_205-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_206-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_207-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_208-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_209-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_210-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_211-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_212-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_213-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_214-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_215-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_216-.L__wine_spec_code_segment
	.byte 0x21,0x00
	/* MMSYSTEM.250 - MMSYSTEM.257 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_250-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_251-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_252-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_253-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_254-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_255-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_256-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_257-.L__wine_spec_code_segment
	.byte 0x2b,0x00
	/* MMSYSTEM.301 - MMSYSTEM.313 */
	.byte 0x0d,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_301-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_302-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_303-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_304-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_305-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_306-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_307-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_308-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_309-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_310-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_311-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_312-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_313-.L__wine_spec_code_segment
	.byte 0x24,0x00
	/* MMSYSTEM.350 - MMSYSTEM.354 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_350-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_351-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_352-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_353-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_354-.L__wine_spec_code_segment
	.byte 0x2e,0x00
	/* MMSYSTEM.401 - MMSYSTEM.421 */
	.byte 0x15,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_401-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_402-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_403-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_404-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_405-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_406-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_407-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_408-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_409-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_410-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_411-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_412-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_413-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_414-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_415-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_416-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_417-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_418-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_419-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_420-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_421-.L__wine_spec_code_segment
	.byte 0x4f,0x00
	/* MMSYSTEM.501 - MMSYSTEM.514 */
	.byte 0x0e,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_501-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_502-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_503-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_504-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_505-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_506-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_507-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_508-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_509-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_510-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_511-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_512-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_513-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_514-.L__wine_spec_code_segment
	.byte 0x56,0x00
	/* MMSYSTEM.601 - MMSYSTEM.607 */
	.byte 0x07,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_601-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_602-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_603-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_604-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_605-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_606-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_607-.L__wine_spec_code_segment
	.byte 0x5d,0x00
	/* MMSYSTEM.701 - MMSYSTEM.703 */
	.byte 0x03,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_701-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_702-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_703-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* MMSYSTEM.705 - MMSYSTEM.708 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_705-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_706-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_707-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_708-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* MMSYSTEM.710 - MMSYSTEM.717 */
	.byte 0x08,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_710-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_711-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_712-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_713-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_714-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_715-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_716-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_717-.L__wine_spec_code_segment
	.byte 0x52,0x00
	/* MMSYSTEM.800 - MMSYSTEM.809 */
	.byte 0x0a,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_800-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_801-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_802-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_803-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_804-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_805-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_806-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_807-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_808-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_809-.L__wine_spec_code_segment
	.byte 0x5a,0x00
	/* MMSYSTEM.900 - MMSYSTEM.900 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_900-.L__wine_spec_code_segment
	.byte 0x01,0x00
	/* MMSYSTEM.902 - MMSYSTEM.905 */
	.byte 0x04,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_902-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_903-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_904-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_905-.L__wine_spec_code_segment
	.byte 0xc2,0x00
	/* MMSYSTEM.1100 - MMSYSTEM.1104 */
	.byte 0x05,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_1100-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1101-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1102-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1103-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1104-.L__wine_spec_code_segment
	.byte 0x0f,0x00
	/* MMSYSTEM.1120 - MMSYSTEM.1125 */
	.byte 0x06,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_1120-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1121-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1122-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1123-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1124-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1125-.L__wine_spec_code_segment
	.byte 0x18,0x00
	/* MMSYSTEM.1150 - MMSYSTEM.1150 */
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_1150-.L__wine_spec_code_segment
	.byte 0x3b,0x00
	/* MMSYSTEM.1210 - MMSYSTEM.1226 */
	.byte 0x11,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_1210-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1211-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1212-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1213-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1214-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1215-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1216-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1217-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1218-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1219-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1220-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1221-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1222-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1223-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1224-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1225-.L__wine_spec_code_segment
	.byte 0x03
	.short .L__wine_MMSYSTEM_1226-.L__wine_spec_code_segment
	.byte 0xff,0x00
	.byte 0xff,0x00
	.byte 0xff,0x00
	.byte 0x37,0x00
	.byte 0x01,0x01
	.byte 0x03
	.short .L__wine_MMSYSTEM_2047-.L__wine_spec_code_segment
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
.L__wine_spec_callfrom16_p_long_lpl:
	pushl $.L__wine_spec_call16_p_lpl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000000e3,0x00000000
.L__wine_spec_callfrom16_p_long_lpll:
	pushl $.L__wine_spec_call16_p_lpll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x000006e3,0x00000000
.L__wine_spec_callfrom16_p_long_lpw:
	pushl $.L__wine_spec_call16_p_lpw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000063,0x00000000
.L__wine_spec_callfrom16_p_long_lwwll:
	pushl $.L__wine_spec_call16_p_lwwll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000364b,0x00000000
.L__wine_spec_callfrom16_p_long_lwwwlll:
	pushl $.L__wine_spec_call16_p_lwwwlll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $22
	.short 0x86c7
	.long 0x000db24b,0x00000000
.L__wine_spec_callfrom16_p_long_lwwwlw:
	pushl $.L__wine_spec_call16_p_lwwwlw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x0000b24b,0x00000000
.L__wine_spec_callfrom16_p_long_p:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $4
	.short 0x86c7
	.long 0x00000004,0x00000000
.L__wine_spec_callfrom16_p_long_ppllll:
	pushl $.L__wine_spec_call16_p_ppllll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $24
	.short 0x86c7
	.long 0x0001b6e4,0x00000000
.L__wine_spec_callfrom16_p_long_pppl:
	pushl $.L__wine_spec_call16_p_pppl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $16
	.short 0x86c7
	.long 0x00000724,0x00000000
.L__wine_spec_callfrom16_p_long_pw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x0000000c,0x00000000
.L__wine_spec_callfrom16_p_long_pwl:
	pushl $.L__wine_spec_call16_p_pwl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x000000cc,0x00000000
.L__wine_spec_callfrom16_p_long_pwlll:
	pushl $.L__wine_spec_call16_p_pwlll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $18
	.short 0x86c7
	.long 0x000036cc,0x00000000
.L__wine_spec_callfrom16_p_long_pwplll:
	pushl $.L__wine_spec_call16_p_pwplll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $22
	.short 0x86c7
	.long 0x0001b70c,0x00000000
.L__wine_spec_callfrom16_p_long_tpl:
	pushl $.L__wine_spec_call16_p_ppl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000000e5,0x00000000
.L__wine_spec_callfrom16_p_long_tpww:
	pushl $.L__wine_spec_call16_p_ppww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00000265,0x00000000
.L__wine_spec_callfrom16_p_long_ttl:
	pushl $.L__wine_spec_call16_p_ppl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000000ed,0x00000000
.L__wine_spec_callfrom16_p_long_tw:
	pushl $.L__wine_spec_call16_p_pw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x0000000d,0x00000000
.L__wine_spec_callfrom16_p_long_w:
	pushl $.L__wine_spec_call16_p_w
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $2
	.short 0x86c7
	.long 0x00000001,0x00000000
.L__wine_spec_callfrom16_p_long_wl:
	pushl $.L__wine_spec_call16_p_wl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000019,0x00000000
.L__wine_spec_callfrom16_p_long_wll:
	pushl $.L__wine_spec_call16_p_wll
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x000000d9,0x00000000
.L__wine_spec_callfrom16_p_long_wllw:
	pushl $.L__wine_spec_call16_p_wllw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x000002d9,0x00000000
.L__wine_spec_callfrom16_p_long_wlw:
	pushl $.L__wine_spec_call16_p_wlw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000059,0x00000000
.L__wine_spec_callfrom16_p_long_wp:
	pushl $.L__wine_spec_call16_p_wp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000021,0x00000000
.L__wine_spec_callfrom16_p_long_wpl:
	pushl $.L__wine_spec_call16_p_wpl
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x000000e1,0x00000000
.L__wine_spec_callfrom16_p_long_wppw:
	pushl $.L__wine_spec_call16_p_wppw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $12
	.short 0x86c7
	.long 0x00000321,0x00000000
.L__wine_spec_callfrom16_p_long_wpw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000061,0x00000000
.L__wine_spec_callfrom16_p_long_wttt:
	pushl $.L__wine_spec_call16_p_wppp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x00000b69,0x00000000
.L__wine_spec_callfrom16_p_long_wtw:
	pushl $.L__wine_spec_call16_p_wpw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000069,0x00000000
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
.L__wine_spec_callfrom16_p_long_wwllw:
	pushl $.L__wine_spec_call16_p_wwllw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $14
	.short 0x86c7
	.long 0x000016c9,0x00000000
.L__wine_spec_callfrom16_p_long_wwpw:
	pushl $.L__wine_spec_call16_p_wwpw
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000309,0x00000000
.L__wine_spec_callfrom16_p_long_www:
	pushl $.L__wine_spec_call16_p_www
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $6
	.short 0x86c7
	.long 0x00000049,0x00000000
.L__wine_spec_callfrom16_p_long_wwwp:
	pushl $.L__wine_spec_call16_p_wwwp
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $10
	.short 0x86c7
	.long 0x00000849,0x00000000
.L__wine_spec_callfrom16_p_long_wwww:
	pushl $.L__wine_spec_call16_p_wwww
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw $8
	.short 0x86c7
	.long 0x00000249,0x00000000
.L__wine_spec_callfrom16_p_word_:
	pushl $.L__wine_spec_call16_p_
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw
	.byte 0x8d,0x74,0x26,0x00
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_spec_callfrom16_p_word_t:
	pushl $.L__wine_spec_call16_p_p
	lcall $0,$0
	orw %ax,%ax
	nop
	lretw $4
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000005,0x00000000
.L__wine_spec_callfrom16_c_long_:
	pushl $.L__wine_spec_call16_c_
	lcall $0,$0
	shld $16,%eax,%edx
	orl %eax,%eax
	lretw
	.byte 0x89,0xf6
	.short 0x86c7
	.long 0x00000000,0x00000000
.L__wine_MMSYSTEM_1:
	pushw %bp
	pushl $_MMSYSTEM_WEP@16
	callw .L__wine_spec_callfrom16_p_long_wwwp
.L__wine_MMSYSTEM_2:
	pushw %bp
	pushl $_sndPlaySound16@8
	callw .L__wine_spec_callfrom16_p_long_pw
.L__wine_MMSYSTEM_3:
	pushw %bp
	pushl $_PlaySound16@12
	callw .L__wine_spec_callfrom16_p_long_pwl
.L__wine_MMSYSTEM_4:
	pushw %bp
	pushl $_MMSYSTEM_LibMain@24
	callw .L__wine_spec_callfrom16_p_long_lwwwlw
.L__wine_MMSYSTEM_5:
	pushw %bp
	pushl $_mmsystemGetVersion16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_6:
	pushw %bp
	pushl $_DriverProc16@20
	callw .L__wine_spec_callfrom16_p_long_lwwll
.L__wine_MMSYSTEM_8:
	pushw %bp
	pushl $_WMMMidiRunOnce16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_30:
	pushw %bp
	pushl $_OutputDebugStr16@4
	callw .L__wine_spec_callfrom16_p_word_t
.L__wine_MMSYSTEM_31:
	pushw %bp
	pushl $_DriverCallback16@28
	callw .L__wine_spec_callfrom16_p_long_lwwwlll
.L__wine_MMSYSTEM_32:
	pushw %bp
	pushl $_StackEnter16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_33:
	pushw %bp
	pushl $_StackLeave16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_34:
	pushw %bp
	pushl $___wine_stub_MMDRVINSTALL
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MMSYSTEM_101:
	pushw %bp
	pushl $_joyGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_102:
	pushw %bp
	pushl $_joyGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_103:
	pushw %bp
	pushl $_joyGetPos16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_104:
	pushw %bp
	pushl $_joyGetThreshold16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_105:
	pushw %bp
	pushl $_joyReleaseCapture16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_106:
	pushw %bp
	pushl $_joySetCapture16@16
	callw .L__wine_spec_callfrom16_p_long_wwww
.L__wine_MMSYSTEM_107:
	pushw %bp
	pushl $_joySetThreshold16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_MMSYSTEM_109:
	pushw %bp
	pushl $_joySetCalibration16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_110:
	pushw %bp
	pushl $_joyGetPosEx16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_111:
	pushw %bp
	pushl $___wine_stub_JOYCONFIGCHANGED
	callw .L__wine_spec_callfrom16_c_long_
.L__wine_MMSYSTEM_201:
	pushw %bp
	pushl $_midiOutGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_202:
	pushw %bp
	pushl $_midiOutGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_203:
	pushw %bp
	pushl $_midiOutGetErrorText16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_204:
	pushw %bp
	pushl $_midiOutOpen16@20
	callw .L__wine_spec_callfrom16_p_long_pwlll
.L__wine_MMSYSTEM_205:
	pushw %bp
	pushl $_midiOutClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_206:
	pushw %bp
	pushl $_midiOutPrepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_207:
	pushw %bp
	pushl $_midiOutUnprepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_208:
	pushw %bp
	pushl $_midiOutShortMsg16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_209:
	pushw %bp
	pushl $_midiOutLongMsg16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_210:
	pushw %bp
	pushl $_midiOutReset16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_211:
	pushw %bp
	pushl $_midiOutGetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_212:
	pushw %bp
	pushl $_midiOutSetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_213:
	pushw %bp
	pushl $_midiOutCachePatches16@16
	callw .L__wine_spec_callfrom16_p_long_wwpw
.L__wine_MMSYSTEM_214:
	pushw %bp
	pushl $_midiOutCacheDrumPatches16@16
	callw .L__wine_spec_callfrom16_p_long_wwpw
.L__wine_MMSYSTEM_215:
	pushw %bp
	pushl $_midiOutGetID16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_216:
	pushw %bp
	pushl $_midiOutMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_250:
	pushw %bp
	pushl $_midiStreamProperty16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_251:
	pushw %bp
	pushl $_midiStreamOpen16@24
	callw .L__wine_spec_callfrom16_p_long_ppllll
.L__wine_MMSYSTEM_252:
	pushw %bp
	pushl $_midiStreamClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_253:
	pushw %bp
	pushl $_midiStreamPosition16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_254:
	pushw %bp
	pushl $_midiStreamOut16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_255:
	pushw %bp
	pushl $_midiStreamPause16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_256:
	pushw %bp
	pushl $_midiStreamRestart16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_257:
	pushw %bp
	pushl $_midiStreamStop16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_301:
	pushw %bp
	pushl $_midiInGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_302:
	pushw %bp
	pushl $_midiInGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_303:
	pushw %bp
	pushl $_midiOutGetErrorText16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_304:
	pushw %bp
	pushl $_midiInOpen16@20
	callw .L__wine_spec_callfrom16_p_long_pwlll
.L__wine_MMSYSTEM_305:
	pushw %bp
	pushl $_midiInClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_306:
	pushw %bp
	pushl $_midiInPrepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_307:
	pushw %bp
	pushl $_midiInUnprepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_308:
	pushw %bp
	pushl $_midiInAddBuffer16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_309:
	pushw %bp
	pushl $_midiInStart16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_310:
	pushw %bp
	pushl $_midiInStop16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_311:
	pushw %bp
	pushl $_midiInReset16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_312:
	pushw %bp
	pushl $_midiInGetID16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_313:
	pushw %bp
	pushl $_midiInMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_350:
	pushw %bp
	pushl $_auxGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_351:
	pushw %bp
	pushl $_auxGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_352:
	pushw %bp
	pushl $_auxGetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_353:
	pushw %bp
	pushl $_auxSetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_354:
	pushw %bp
	pushl $_auxOutMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_401:
	pushw %bp
	pushl $_waveOutGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_402:
	pushw %bp
	pushl $_waveOutGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_403:
	pushw %bp
	pushl $_waveOutGetErrorText16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_404:
	pushw %bp
	pushl $_waveOutOpen16@24
	callw .L__wine_spec_callfrom16_p_long_pwplll
.L__wine_MMSYSTEM_405:
	pushw %bp
	pushl $_waveOutClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_406:
	pushw %bp
	pushl $_waveOutPrepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_407:
	pushw %bp
	pushl $_waveOutUnprepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_408:
	pushw %bp
	pushl $_waveOutWrite16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_409:
	pushw %bp
	pushl $_waveOutPause16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_410:
	pushw %bp
	pushl $_waveOutRestart16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_411:
	pushw %bp
	pushl $_waveOutReset16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_412:
	pushw %bp
	pushl $_waveOutGetPosition16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_413:
	pushw %bp
	pushl $_waveOutGetPitch16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_414:
	pushw %bp
	pushl $_waveOutSetPitch16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_415:
	pushw %bp
	pushl $_waveOutGetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_416:
	pushw %bp
	pushl $_waveOutSetVolume16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_417:
	pushw %bp
	pushl $_waveOutGetPlaybackRate16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_418:
	pushw %bp
	pushl $_waveOutSetPlaybackRate16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_419:
	pushw %bp
	pushl $_waveOutBreakLoop16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_420:
	pushw %bp
	pushl $_waveOutGetID16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_421:
	pushw %bp
	pushl $_waveOutMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_501:
	pushw %bp
	pushl $_waveInGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_502:
	pushw %bp
	pushl $_waveInGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_503:
	pushw %bp
	pushl $_waveOutGetErrorText16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_504:
	pushw %bp
	pushl $_waveInOpen16@24
	callw .L__wine_spec_callfrom16_p_long_pwplll
.L__wine_MMSYSTEM_505:
	pushw %bp
	pushl $_waveInClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_506:
	pushw %bp
	pushl $_waveInPrepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_507:
	pushw %bp
	pushl $_waveInUnprepareHeader16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_508:
	pushw %bp
	pushl $_waveInAddBuffer16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_509:
	pushw %bp
	pushl $_waveInStart16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_510:
	pushw %bp
	pushl $_waveInStop16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_511:
	pushw %bp
	pushl $_waveInReset16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_512:
	pushw %bp
	pushl $_waveInGetPosition16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_513:
	pushw %bp
	pushl $_waveInGetID16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_514:
	pushw %bp
	pushl $_waveInMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_601:
	pushw %bp
	pushl $_timeGetSystemTime16@8
	callw .L__wine_spec_callfrom16_p_long_pw
.L__wine_MMSYSTEM_602:
	pushw %bp
	pushl $_timeSetEvent16@20
	callw .L__wine_spec_callfrom16_p_long_wwllw
.L__wine_MMSYSTEM_603:
	pushw %bp
	pushl $_timeKillEvent16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_604:
	pushw %bp
	pushl $_timeGetDevCaps16@8
	callw .L__wine_spec_callfrom16_p_long_pw
.L__wine_MMSYSTEM_605:
	pushw %bp
	pushl $_timeBeginPeriod16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_606:
	pushw %bp
	pushl $_timeEndPeriod16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_607:
	pushw %bp
	pushl $_timeGetTime16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_701:
	pushw %bp
	pushl $_mciSendCommand16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_702:
	pushw %bp
	pushl $_mciSendString16@16
	callw .L__wine_spec_callfrom16_p_long_tpww
.L__wine_MMSYSTEM_703:
	pushw %bp
	pushl $_mciGetDeviceID16@4
	callw .L__wine_spec_callfrom16_p_long_p
.L__wine_MMSYSTEM_705:
	pushw %bp
	pushl $_mciLoadCommandResource16@12
	callw .L__wine_spec_callfrom16_p_long_wtw
.L__wine_MMSYSTEM_706:
	pushw %bp
	pushl $_mciGetErrorString16@12
	callw .L__wine_spec_callfrom16_p_long_lpw
.L__wine_MMSYSTEM_707:
	pushw %bp
	pushl $_mciSetDriverData16@8
	callw .L__wine_spec_callfrom16_p_long_wl
.L__wine_MMSYSTEM_708:
	pushw %bp
	pushl $_mciGetDriverData16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_710:
	pushw %bp
	pushl $_mciDriverYield16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_711:
	pushw %bp
	pushl $_mciDriverNotify16@12
	callw .L__wine_spec_callfrom16_p_long_www
.L__wine_MMSYSTEM_712:
	pushw %bp
	pushl $_mciExecute16@4
	callw .L__wine_spec_callfrom16_p_long_p
.L__wine_MMSYSTEM_713:
	pushw %bp
	pushl $_mciFreeCommandResource16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_714:
	pushw %bp
	pushl $_mciSetYieldProc16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_715:
	pushw %bp
	pushl $_mciGetDeviceIDFromElementID16@8
	callw .L__wine_spec_callfrom16_p_long_lp
.L__wine_MMSYSTEM_716:
	pushw %bp
	pushl $_mciGetYieldProc16@8
	callw .L__wine_spec_callfrom16_p_long_wp
.L__wine_MMSYSTEM_717:
	pushw %bp
	pushl $_mciGetCreatorTask16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_800:
	pushw %bp
	pushl $_mixerGetNumDevs16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_801:
	pushw %bp
	pushl $_mixerGetDevCaps16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_802:
	pushw %bp
	pushl $_mixerOpen16@20
	callw .L__wine_spec_callfrom16_p_long_pwlll
.L__wine_MMSYSTEM_803:
	pushw %bp
	pushl $_mixerClose16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_804:
	pushw %bp
	pushl $_mixerMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_805:
	pushw %bp
	pushl $_mixerGetLineInfo16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_806:
	pushw %bp
	pushl $_mixerGetID16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_807:
	pushw %bp
	pushl $_mixerGetLineControls16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_808:
	pushw %bp
	pushl $_mixerGetControlDetails16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_809:
	pushw %bp
	pushl $_mixerSetControlDetails16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_900:
	pushw %bp
	pushl $_mmTaskCreate16@12
	callw .L__wine_spec_callfrom16_p_long_lpl
.L__wine_MMSYSTEM_902:
	pushw %bp
	pushl $_mmTaskBlock16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_903:
	pushw %bp
	pushl $_mmTaskSignal16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_904:
	pushw %bp
	pushl $_mmGetCurrentTask16@0
	callw .L__wine_spec_callfrom16_p_word_
.L__wine_MMSYSTEM_905:
	pushw %bp
	pushl $_mmTaskYield16@0
	callw .L__wine_spec_callfrom16_p_long_
.L__wine_MMSYSTEM_1100:
	pushw %bp
	pushl $_DrvOpen16@12
	callw .L__wine_spec_callfrom16_p_long_ttl
.L__wine_MMSYSTEM_1101:
	pushw %bp
	pushl $_DrvClose16@12
	callw .L__wine_spec_callfrom16_p_long_wll
.L__wine_MMSYSTEM_1102:
	pushw %bp
	pushl $_DrvSendMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_1103:
	pushw %bp
	pushl $_DrvGetModuleHandle16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1104:
	pushw %bp
	pushl $_DrvDefDriverProc16@20
	callw .L__wine_spec_callfrom16_p_long_lwwll
.L__wine_MMSYSTEM_1120:
	pushw %bp
	pushl $_mmThreadCreate16@16
	callw .L__wine_spec_callfrom16_p_long_lpll
.L__wine_MMSYSTEM_1121:
	pushw %bp
	pushl $_mmThreadSignal16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1122:
	pushw %bp
	pushl $_mmThreadBlock16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1123:
	pushw %bp
	pushl $_mmThreadIsCurrent16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1124:
	pushw %bp
	pushl $_mmThreadIsValid16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1125:
	pushw %bp
	pushl $_mmThreadGetTask16@4
	callw .L__wine_spec_callfrom16_p_long_w
.L__wine_MMSYSTEM_1150:
	pushw %bp
	pushl $_mmShowMMCPLPropertySheet16@16
	callw .L__wine_spec_callfrom16_p_long_wttt
.L__wine_MMSYSTEM_1210:
	pushw %bp
	pushl $_mmioOpen16@12
	callw .L__wine_spec_callfrom16_p_long_tpl
.L__wine_MMSYSTEM_1211:
	pushw %bp
	pushl $_mmioClose16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_MMSYSTEM_1212:
	pushw %bp
	pushl $_mmioRead16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_1213:
	pushw %bp
	pushl $_mmioWrite16@12
	callw .L__wine_spec_callfrom16_p_long_wpl
.L__wine_MMSYSTEM_1214:
	pushw %bp
	pushl $_mmioSeek16@12
	callw .L__wine_spec_callfrom16_p_long_wlw
.L__wine_MMSYSTEM_1215:
	pushw %bp
	pushl $_mmioGetInfo16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_1216:
	pushw %bp
	pushl $_mmioSetInfo16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_1217:
	pushw %bp
	pushl $_mmioSetBuffer16@16
	callw .L__wine_spec_callfrom16_p_long_wllw
.L__wine_MMSYSTEM_1218:
	pushw %bp
	pushl $_mmioFlush16@8
	callw .L__wine_spec_callfrom16_p_long_ww
.L__wine_MMSYSTEM_1219:
	pushw %bp
	pushl $_mmioAdvance16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_1220:
	pushw %bp
	pushl $_mmioStringToFOURCC16@8
	callw .L__wine_spec_callfrom16_p_long_tw
.L__wine_MMSYSTEM_1221:
	pushw %bp
	pushl $_mmioInstallIOProc16@12
	callw .L__wine_spec_callfrom16_p_long_lpl
.L__wine_MMSYSTEM_1222:
	pushw %bp
	pushl $_mmioSendMessage16@16
	callw .L__wine_spec_callfrom16_p_long_wwll
.L__wine_MMSYSTEM_1223:
	pushw %bp
	pushl $_mmioDescend16@16
	callw .L__wine_spec_callfrom16_p_long_wppw
.L__wine_MMSYSTEM_1224:
	pushw %bp
	pushl $_mmioAscend16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_1225:
	pushw %bp
	pushl $_mmioCreateChunk16@12
	callw .L__wine_spec_callfrom16_p_long_wpw
.L__wine_MMSYSTEM_1226:
	pushw %bp
	pushl $_mmioRename16@16
	callw .L__wine_spec_callfrom16_p_long_pppl
.L__wine_MMSYSTEM_2047:
	pushw %bp
	pushl $_WINE_mmThreadEntryPoint@4
	callw .L__wine_spec_callfrom16_p_long_l
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
	.def .L__wine_spec_call16_p_lpl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lpl:
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
	pushl 8(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_lpll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lpll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $4,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
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
	.def .L__wine_spec_call16_p_lpw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lpw:
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
	pushl 6(%ecx)
	call *8(%ebp)
	movl -4(%ebp),%esi
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
	.def .L__wine_spec_call16_p_lwwwlll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_lwwwlll:
	pushl %ebp
	movl %esp,%ebp
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 14(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	pushl 18(%ecx)
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
	.def .L__wine_spec_call16_p_ppllll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppllll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	pushl 12(%ecx)
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
	.def .L__wine_spec_call16_p_pppl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pppl:
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
	.def .L__wine_spec_call16_p_ppww; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_ppww:
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
	.def .L__wine_spec_call16_p_pwl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwl:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $8,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
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
	.def .L__wine_spec_call16_p_pwlll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwlll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 12(%ecx),%eax
	pushl %eax
	movzwl 16(%ecx),%edx
	shr $3,%edx
	movzwl 14(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	call *8(%ebp)
	movl -4(%ebp),%esi
	leave
	ret
	.align 4
	.def .L__wine_spec_call16_p_pwplll; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_pwplll:
	pushl %ebp
	movl %esp,%ebp
	pushl %esi
	movl $__imp__wine_ldt_copy,%esi
	subl $12,%esp
	movl 12(%ebp),%ecx
	pushl 0(%ecx)
	pushl 4(%ecx)
	pushl 8(%ecx)
	movzwl 14(%ecx),%edx
	shr $3,%edx
	movzwl 12(%ecx),%eax
	addl (%esi,%edx,4),%eax
	pushl %eax
	movzwl 16(%ecx),%eax
	pushl %eax
	movzwl 20(%ecx),%edx
	shr $3,%edx
	movzwl 18(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wllw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wllw:
	pushl %ebp
	movl %esp,%ebp
	subl $8,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	pushl 6(%ecx)
	movzwl 10(%ecx),%eax
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
	.def .L__wine_spec_call16_p_wpl; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wpl:
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
	.def .L__wine_spec_call16_p_wwllw; .scl 2; .type 32; .endef
.L__wine_spec_call16_p_wwllw:
	pushl %ebp
	movl %esp,%ebp
	subl $4,%esp
	movl 12(%ebp),%ecx
	movzwl 0(%ecx),%eax
	pushl %eax
	pushl 2(%ecx)
	pushl 6(%ecx)
	movzwl 10(%ecx),%eax
	pushl %eax
	movzwl 12(%ecx),%eax
	pushl %eax
	call *8(%ebp)
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
	.data
wine_ldt_copy_ptr:
	.long __imp__wine_ldt_copy

/* stub functions */

	.text
	.align 4
	.def ___wine_stub_MMDRVINSTALL; .scl 2; .type 32; .endef
___wine_stub_MMDRVINSTALL:
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
	movl $.L__wine_stub_MMDRVINSTALL_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.align 4
	.def ___wine_stub_JOYCONFIGCHANGED; .scl 2; .type 32; .endef
___wine_stub_JOYCONFIGCHANGED:
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
	movl $.L__wine_stub_JOYCONFIGCHANGED_string,4(%esp)
	movl $.L__wine_spec_file_name,(%esp)
	call ___wine_spec_unimplemented_stub
	.section .rodata
.L__wine_stub_MMDRVINSTALL_string:
	.string "MMDRVINSTALL"
.L__wine_stub_JOYCONFIGCHANGED_string:
	.string "JOYCONFIGCHANGED"

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
	.long .L__wine_spec_exp_names+32-.L__wine_spec_rva_base
	.long .L__wine_spec_exp_names+55-.L__wine_spec_rva_base

.L__wine_spec_exp_ordinals:
	.short 0
	.short 1

.L__wine_spec_exp_names:
	.string "..\mmsystem\mmsystem.dll16.spec"
	.string "__wine_spec_dos_header"
	.string "__wine_spec_main_module"
	.align 4
.L__wine_spec_exports_end:
	.long 0

	.section .rodata
.L__wine_spec_main_module:
	.string "MMSYSTEM"
__end:/*?*/
