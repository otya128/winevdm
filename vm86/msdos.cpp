/*
	MS-DOS Player for Win32 console

	Author : Takeda.Toshiya
	Date   : 2009.11.09-
*/

#include "msdos.h"

#define my_strchr(str, chr) (char *)_mbschr((unsigned char *)(str), (unsigned int)(chr))
#define my_strtok(tok, del) (char *)_mbstok((unsigned char *)(tok), (const unsigned char *)(del))
#define my_strupr(str) (char *)_mbsupr((unsigned char *)(str))

#define fatalerror(...) { \
	fprintf(stderr, __VA_ARGS__); \
	exit(1); \
}
#define error(...) fprintf(stderr, "error: " __VA_ARGS__)

#if defined(__MINGW32__)
extern "C" int _CRT_glob = 0;
#endif

/*
	kludge for "more-standardized" C++
*/
#if !defined(_MSC_VER)
inline int kludge_min(int a, int b) { return (a<b ? a:b); }
inline int kludge_max(int a, int b) { return (a>b ? a:b); }
#define min(a,b) kludge_min(a,b)
#define max(a,b) kludge_max(a,b)
#endif

void change_console_size_to_80x25();

/* ----------------------------------------------------------------------------
	MAME i86/i386
---------------------------------------------------------------------------- */

#define SUPPORT_DISASSEMBLER

#if defined(HAS_I86)
	#define CPU_MODEL i8086
#elif defined(HAS_I186)
	#define CPU_MODEL i80186
#elif defined(HAS_I286)
	#define CPU_MODEL i80286
#elif defined(HAS_I386)
	#define CPU_MODEL i386
#else
	#if defined(HAS_I386SX)
		#define CPU_MODEL i386SX
	#else
		#if defined(HAS_I486)
			#define CPU_MODEL i486
		#else
			#if defined(HAS_PENTIUM)
				#define CPU_MODEL pentium
			#elif defined(HAS_MEDIAGX)
				#define CPU_MODEL mediagx
			#elif defined(HAS_PENTIUM_PRO)
				#define CPU_MODEL pentium_pro
			#elif defined(HAS_PENTIUM_MMX)
				#define CPU_MODEL pentium_mmx
			#elif defined(HAS_PENTIUM2)
				#define CPU_MODEL pentium2
			#elif defined(HAS_PENTIUM3)
				#define CPU_MODEL pentium3
			#elif defined(HAS_PENTIUM4)
				#define CPU_MODEL pentium4
			#endif
			#define SUPPORT_RDTSC
		#endif
		#define SUPPORT_FPU
	#endif
	#define HAS_I386
#endif

#ifndef __BIG_ENDIAN__
#define LSB_FIRST
#endif

#ifndef INLINE
#define INLINE inline
#endif
#define U64(v) UINT64(v)

//#define logerror(...) fprintf(stderr, __VA_ARGS__)
#define logerror(...)
//#define popmessage(...) fprintf(stderr, __VA_ARGS__)
#define popmessage(...)

/*****************************************************************************/
/* src/emu/devcpu.h */

// CPU interface functions
#define CPU_INIT_NAME(name)			cpu_init_##name
#define CPU_INIT(name)				void CPU_INIT_NAME(name)()
#define CPU_INIT_CALL(name)			CPU_INIT_NAME(name)()

#define CPU_RESET_NAME(name)			cpu_reset_##name
#define CPU_RESET(name)				void CPU_RESET_NAME(name)()
#define CPU_RESET_CALL(name)			CPU_RESET_NAME(name)()

#define CPU_EXECUTE_NAME(name)			cpu_execute_##name
#define CPU_EXECUTE(name)			void CPU_EXECUTE_NAME(name)()
#define CPU_EXECUTE_CALL(name)			CPU_EXECUTE_NAME(name)()

#define CPU_TRANSLATE_NAME(name)		cpu_translate_##name
#define CPU_TRANSLATE(name)			int CPU_TRANSLATE_NAME(name)(address_spacenum space, int intention, offs_t *address)
#define CPU_TRANSLATE_CALL(name)		CPU_TRANSLATE_NAME(name)(space, intention, address)

#define CPU_DISASSEMBLE_NAME(name)		cpu_disassemble_##name
#define CPU_DISASSEMBLE(name)			int CPU_DISASSEMBLE_NAME(name)(char *buffer, offs_t eip, const UINT8 *oprom)
#define CPU_DISASSEMBLE_CALL(name)		CPU_DISASSEMBLE_NAME(name)(buffer, eip, oprom)

/*****************************************************************************/
/* src/emu/didisasm.h */

// Disassembler constants
const UINT32 DASMFLAG_SUPPORTED     = 0x80000000;   // are disassembly flags supported?
const UINT32 DASMFLAG_STEP_OUT      = 0x40000000;   // this instruction should be the end of a step out sequence
const UINT32 DASMFLAG_STEP_OVER     = 0x20000000;   // this instruction should be stepped over by setting a breakpoint afterwards
const UINT32 DASMFLAG_OVERINSTMASK  = 0x18000000;   // number of extra instructions to skip when stepping over
const UINT32 DASMFLAG_OVERINSTSHIFT = 27;           // bits to shift after masking to get the value
const UINT32 DASMFLAG_LENGTHMASK    = 0x0000ffff;   // the low 16-bits contain the actual length

/*****************************************************************************/
/* src/emu/diexec.h */

// I/O line states
enum line_state
{
	CLEAR_LINE = 0,				// clear (a fired or held) line
	ASSERT_LINE,				// assert an interrupt immediately
	HOLD_LINE,				// hold interrupt line until acknowledged
	PULSE_LINE				// pulse interrupt line instantaneously (only for NMI, RESET)
};

// I/O line definitions
enum
{
	INPUT_LINE_IRQ = 0,
	INPUT_LINE_NMI
};

/*****************************************************************************/
/* src/emu/dimemory.h */

// Translation intentions
const int TRANSLATE_TYPE_MASK       = 0x03;     // read write or fetch
const int TRANSLATE_USER_MASK       = 0x04;     // user mode or fully privileged
const int TRANSLATE_DEBUG_MASK      = 0x08;     // debug mode (no side effects)

const int TRANSLATE_READ            = 0;        // translate for read
const int TRANSLATE_WRITE           = 1;        // translate for write
const int TRANSLATE_FETCH           = 2;        // translate for instruction fetch
const int TRANSLATE_READ_USER       = (TRANSLATE_READ | TRANSLATE_USER_MASK);
const int TRANSLATE_WRITE_USER      = (TRANSLATE_WRITE | TRANSLATE_USER_MASK);
const int TRANSLATE_FETCH_USER      = (TRANSLATE_FETCH | TRANSLATE_USER_MASK);
const int TRANSLATE_READ_DEBUG      = (TRANSLATE_READ | TRANSLATE_DEBUG_MASK);
const int TRANSLATE_WRITE_DEBUG     = (TRANSLATE_WRITE | TRANSLATE_DEBUG_MASK);
const int TRANSLATE_FETCH_DEBUG     = (TRANSLATE_FETCH | TRANSLATE_DEBUG_MASK);

/*****************************************************************************/
/* src/emu/emucore.h */

// constants for expression endianness
enum endianness_t
{
	ENDIANNESS_LITTLE,
	ENDIANNESS_BIG
};

// declare native endianness to be one or the other
#ifdef LSB_FIRST
const endianness_t ENDIANNESS_NATIVE = ENDIANNESS_LITTLE;
#else
const endianness_t ENDIANNESS_NATIVE = ENDIANNESS_BIG;
#endif

// endian-based value: first value is if 'endian' is little-endian, second is if 'endian' is big-endian
#define ENDIAN_VALUE_LE_BE(endian,leval,beval)	(((endian) == ENDIANNESS_LITTLE) ? (leval) : (beval))

// endian-based value: first value is if native endianness is little-endian, second is if native is big-endian
#define NATIVE_ENDIAN_VALUE_LE_BE(leval,beval)	ENDIAN_VALUE_LE_BE(ENDIANNESS_NATIVE, leval, beval)

// endian-based value: first value is if 'endian' matches native, second is if 'endian' doesn't match native
#define ENDIAN_VALUE_NE_NNE(endian,leval,beval)	(((endian) == ENDIANNESS_NATIVE) ? (neval) : (nneval))

/*****************************************************************************/
/* src/emu/memory.h */

// address spaces
enum address_spacenum
{
	AS_0,                           // first address space
	AS_1,                           // second address space
	AS_2,                           // third address space
	AS_3,                           // fourth address space
	ADDRESS_SPACES,                 // maximum number of address spaces

	// alternate address space names for common use
	AS_PROGRAM = AS_0,              // program address space
	AS_DATA = AS_1,                 // data address space
	AS_IO = AS_2                    // I/O address space
};

// offsets and addresses are 32-bit (for now...)
typedef UINT32	offs_t;
extern "C" void *wine_ldt_get_ptr(unsigned short sel, unsigned long offset);
void *read_ptr(offs_t byteaddress)
{
	return nullptr;
}
// read accessors
UINT8 read_byte(offs_t byteaddress)
{
#if defined(HAS_I386)
	if(byteaddress < MAX_MEM) {
		return mem[byteaddress];
//	} else if((byteaddress & 0xfffffff0) == 0xfffffff0) {
//		return read_byte(byteaddress & 0xfffff);
	}
	return 0;
#else
	return mem[byteaddress];
#endif
}

UINT16 read_word(offs_t byteaddress)
{
#if defined(HAS_I386)
	if(byteaddress < MAX_MEM - 1) {
		return *(UINT16 *)(mem + byteaddress);
//	} else if((byteaddress & 0xfffffff0) == 0xfffffff0) {
//		return read_word(byteaddress & 0xfffff);
	}
	return 0;
#else
	return *(UINT16 *)(mem + byteaddress);
#endif
}

UINT32 read_dword(offs_t byteaddress)
{
#if defined(HAS_I386)
	if(byteaddress < MAX_MEM - 3) {
		return *(UINT32 *)(mem + byteaddress);
//	} else if((byteaddress & 0xfffffff0) == 0xfffffff0) {
//		return read_dword(byteaddress & 0xfffff);
	}
	return 0;
#else
	return *(UINT32 *)(mem + byteaddress);
#endif
}

// write accessors
void write_text_vram_byte(offs_t offset, UINT8 data)
{
	// XXX: we need to consider a multi-byte character
	COORD co;
	DWORD num;
	
	co.X = (offset >> 1) % 80;
	co.Y = (offset >> 1) / 80;
	
	if(offset & 1) {
		scr_attr[0] = data;
		WriteConsoleOutputAttribute(hStdout, scr_attr, 1, co, &num);
	} else {
		scr_char[0] = data;
		WriteConsoleOutputCharacter(hStdout, scr_char, 1, co, &num);
	}
}

void write_text_vram_word(offs_t offset, UINT16 data)
{
	// XXX: we need to consider a multi-byte character
	if(offset & 1) {
		// Attr, Char
		write_text_vram_byte(offset    , (data     ) & 0xff);
		write_text_vram_byte(offset + 1, (data >> 8) & 0xff);
	} else {
		// Char, Attr
		COORD co;
		DWORD num;
		
		co.X = (offset >> 1) % 80;
		co.Y = (offset >> 1) / 80;
		
		scr_char[0] = (data     ) & 0xff;
		scr_attr[0] = (data >> 8) & 0xff;
		
		WriteConsoleOutputCharacter(hStdout, scr_char, 1, co, &num);
		WriteConsoleOutputAttribute(hStdout, scr_attr, 1, co, &num);
	}
}

void write_text_vram_dword(offs_t offset, UINT32 data)
{
	// XXX: we need to consider a multi-byte character
	if(offset & 1) {
		// Attr, Char, Attr, Char
		write_text_vram_byte(offset    , (data      ) & 0x00ff);
		write_text_vram_word(offset + 1, (data >>  8) & 0xffff);
		write_text_vram_byte(offset + 3, (data >> 24) & 0x00ff);
	} else {
		// Char, Attr, Char, Attr
		COORD co;
		DWORD num;
		
		co.X = (offset >> 1) % 80;
		co.Y = (offset >> 1) / 80;
		
		scr_char[0] = (data      ) & 0xff;
		scr_attr[0] = (data >>  8) & 0xff;
		scr_char[1] = (data >> 16) & 0xff;
		scr_attr[1] = (data >> 24) & 0xff;
		
		WriteConsoleOutputCharacter(hStdout, scr_char, 2, co, &num);
		WriteConsoleOutputAttribute(hStdout, scr_attr, 2, co, &num);
	}
}

void write_byte(offs_t byteaddress, UINT8 data)
{
	/*
	if(byteaddress < MEMORY_END) {
		mem[byteaddress] = data;
	} else if(byteaddress >= text_vram_top_address && byteaddress < text_vram_end_address) {
		if(!restore_console_on_exit && (scr_width != 80 || scr_height != 25)) {
			change_console_size_to_80x25();
			restore_console_on_exit = true;
		}
		write_text_vram_byte(byteaddress - text_vram_top_address, data);
		mem[byteaddress] = data;
	} else if(byteaddress >= shadow_buffer_top_address && byteaddress < shadow_buffer_end_address) {
		if(int_10h_feh_called && !int_10h_ffh_called) {
			write_text_vram_byte(byteaddress - shadow_buffer_top_address, data);
		}
		mem[byteaddress] = data;
#if defined(HAS_I386)
	} else if(byteaddress < MAX_MEM) {
#else
	} else {
#endif
		mem[byteaddress] = data;
	}
	*/
	mem[byteaddress] = data;
}

void write_word(offs_t byteaddress, UINT16 data)
{
	*(UINT16 *)(mem + byteaddress) = data;
	/*
	if(byteaddress < MEMORY_END) {
		*(UINT16 *)(mem + byteaddress) = data;
	} else if(byteaddress >= text_vram_top_address && byteaddress < text_vram_end_address) {
		if(!restore_console_on_exit && (scr_width != 80 || scr_height != 25)) {
			change_console_size_to_80x25();
			restore_console_on_exit = true;
		}
		write_text_vram_word(byteaddress - text_vram_top_address, data);
		*(UINT16 *)(mem + byteaddress) = data;
	} else if(byteaddress >= shadow_buffer_top_address && byteaddress < shadow_buffer_end_address) {
		if(int_10h_feh_called && !int_10h_ffh_called) {
			write_text_vram_word(byteaddress - shadow_buffer_top_address, data);
		}
		*(UINT16 *)(mem + byteaddress) = data;
#if defined(HAS_I386)
	} else if(byteaddress < MAX_MEM - 1) {
#else
	} else {
#endif
		*(UINT16 *)(mem + byteaddress) = data;
	}*/
}

void write_dword(offs_t byteaddress, UINT32 data)
{
	*(UINT32 *)(mem + byteaddress) = data;
	/*
	if(byteaddress < MEMORY_END) {
		*(UINT32 *)(mem + byteaddress) = data;
	} else if(byteaddress >= text_vram_top_address && byteaddress < text_vram_end_address) {
		if(!restore_console_on_exit && (scr_width != 80 || scr_height != 25)) {
			change_console_size_to_80x25();
			restore_console_on_exit = true;
		}
		write_text_vram_dword(byteaddress - text_vram_top_address, data);
		*(UINT32 *)(mem + byteaddress) = data;
	} else if(byteaddress >= shadow_buffer_top_address && byteaddress < shadow_buffer_end_address) {
		if(int_10h_feh_called && !int_10h_ffh_called) {
			write_text_vram_dword(byteaddress - shadow_buffer_top_address, data);
		}
		*(UINT32 *)(mem + byteaddress) = data;
#if defined(HAS_I386)
	} else if(byteaddress < MAX_MEM - 3) {
#else
	} else {
#endif
		*(UINT32 *)(mem + byteaddress) = data;
	}
	*/
}

#define read_decrypted_byte read_byte
#define read_decrypted_word read_word
#define read_decrypted_dword read_dword

#define read_raw_byte read_byte
#define write_raw_byte write_byte

#define read_word_unaligned read_word
#define write_word_unaligned write_word

#define read_io_word_unaligned read_io_word
#define write_io_word_unaligned write_io_word

UINT8 read_io_byte(offs_t byteaddress);
UINT16 read_io_word(offs_t byteaddress);
UINT32 read_io_dword(offs_t byteaddress);

void write_io_byte(offs_t byteaddress, UINT8 data);
void write_io_word(offs_t byteaddress, UINT16 data);
void write_io_dword(offs_t byteaddress, UINT32 data);

/*****************************************************************************/
/* src/osd/osdcomm.h */

/* Highly useful macro for compile-time knowledge of an array size */
#define ARRAY_LENGTH(x)     (sizeof(x) / sizeof(x[0]))

#if defined(HAS_I386)
	static CPU_TRANSLATE(i386);
	#include "mame/lib/softfloat/softfloat.c"
	#include "mame/emu/cpu/i386/i386.c"
	#include "mame/emu/cpu/vtlb.c"
#elif defined(HAS_I286)
	#include "mame/emu/cpu/i86/i286.c"
#else
	#include "mame/emu/cpu/i86/i86.c"
#endif
#ifdef SUPPORT_DISASSEMBLER
	#include "mame/emu/cpu/i386/i386dasm.c"
	bool dasm = false;
#endif

#if defined(HAS_I386)
	#define SREG(x)				m_sreg[x].selector
	#define SREG_BASE(x)			m_sreg[x].base

	int cpu_type, cpu_step;
#else
	#define REG8(x)				m_regs.b[x]
	#define REG16(x)			m_regs.w[x]
	#define SREG(x)				m_sregs[x]
	#define SREG_BASE(x)			m_base[x]
	#define m_CF				m_CarryVal
	#define m_a20_mask			AMASK
	//#define i386_load_segment_descriptor(x)	m_base[x] = SegBase(x)
	void load_segment_descriptor_wine(int sreg);
#define i386_load_segment_descriptor(x) load_segment_descriptor_wine(x)
	#if defined(HAS_I286)
		#define i386_set_a20_line(x)	i80286_set_a20_line(x)
	#else
		#define i386_set_a20_line(x)
	#endif
	#define i386_set_irq_line(x, y)		set_irq_line(x, y)
#endif

void i386_jmp_far(UINT16 selector, UINT32 address)
{
#if defined(HAS_I386)
	if(PROTECTED_MODE && !V8086_MODE) {
		i386_protected_mode_jump(selector, address, 1, m_operand_size);
	} else {
		SREG(CS) = selector;
		m_performed_intersegment_jump = 1;
		i386_load_segment_descriptor(CS);
		m_eip = address;
		CHANGE_PC(m_eip);
	}
#elif defined(HAS_I286)
	i80286_code_descriptor(selector, address, 1);
#else
	SREG(CS) = selector;
	i386_load_segment_descriptor(CS);
	m_pc = (SREG_BASE(CS) + address) & m_a20_mask;
#endif
}

/* ----------------------------------------------------------------------------
MS-DOS virtual machine
---------------------------------------------------------------------------- */
void msdos_syscall(unsigned int a)
{
}
void update_key_buffer_tmp()
{
	DWORD dwNumberOfEvents = 0;
	DWORD dwRead;
	INPUT_RECORD ir[16];

	if (GetNumberOfConsoleInputEvents(hStdin, &dwNumberOfEvents) && dwNumberOfEvents != 0) {
		if (ReadConsoleInputA(hStdin, ir, 16, &dwRead)) {
			for (int i = 0; i < dwRead; i++) {
				if ((ir[i].EventType & KEY_EVENT) && ir[i].Event.KeyEvent.bKeyDown) {
					if (ir[i].Event.KeyEvent.uChar.AsciiChar == 0) {
						// ignore shift, ctrl and alt keys
						if (ir[i].Event.KeyEvent.wVirtualScanCode != 0x1d &&
							ir[i].Event.KeyEvent.wVirtualScanCode != 0x2a &&
							ir[i].Event.KeyEvent.wVirtualScanCode != 0x36 &&
							ir[i].Event.KeyEvent.wVirtualScanCode != 0x38) {
							key_buf_char->write(0x00);
							key_buf_scan->write(ir[i].Event.KeyEvent.dwControlKeyState & ENHANCED_KEY ? 0xe0 : 0x00);
							key_buf_char->write(0x00);
							key_buf_scan->write(ir[i].Event.KeyEvent.wVirtualScanCode & 0xff);
						}
					}
					else {
						key_buf_char->write(ir[i].Event.KeyEvent.uChar.AsciiChar & 0xff);
						key_buf_scan->write(ir[i].Event.KeyEvent.wVirtualScanCode & 0xff);
					}
				}
			}
		}
	}
}

void update_key_buffer()
{
	int prev_count = key_buf_char->count();
	update_key_buffer_tmp();
	key_input += key_buf_char->count() - prev_count;

	if (key_buf_char->count() == 0) {
		Sleep(10);
	}
}

int check_key_input()
{
	if (key_input == 0) {
		int prev_count = key_buf_char->count();
		update_key_buffer_tmp();
		key_input = key_buf_char->count() - prev_count;
	}
	int val = key_input;
	key_input = 0;
	return(val);
}

/* ----------------------------------------------------------------------------
	PC/AT hardware emulation
---------------------------------------------------------------------------- */

void hardware_init()
{
	CPU_INIT_CALL(CPU_MODEL);
	CPU_RESET_CALL(CPU_MODEL);
#if defined(HAS_I386)
	cpu_type = (REG32(EDX) >> 8) & 0x0f;
	cpu_step = (REG32(EDX) >> 0) & 0x0f;
#endif
	i386_set_a20_line(0);
	pic_init();
#ifdef PIT_ALWAYS_RUNNING
	pit_init();
#else
	pit_active = 0;
#endif
	cmos_init();
	kbd_init();
}

void hardware_finish()
{
#if defined(HAS_I386)
	vtlb_free(m_vtlb);
#endif
}

void hardware_run()
{
	int ops = 0;
	
	while(!m_halted) {
#ifdef SUPPORT_DISASSEMBLER
		if(dasm) {
			char buffer[256];
#if defined(HAS_I386)
			UINT64 eip = m_eip;
#else
			UINT64 eip = m_pc - SREG_BASE(CS);
#endif
			UINT8 *oprom = mem + SREG_BASE(CS) + eip;
			
#if defined(HAS_I386)
			if(m_operand_size) {
				CPU_DISASSEMBLE_CALL(x86_32);
			} else
#endif
			CPU_DISASSEMBLE_CALL(x86_16);
			fprintf(stderr, "%04x:%04x\t%s\n", SREG(CS), (unsigned)eip, buffer);
		}
#endif
#if defined(HAS_I386)
		m_cycles = 1;
		CPU_EXECUTE_CALL(i386);
#else
		CPU_EXECUTE_CALL(CPU_MODEL);
#endif
		if(++ops == 16384) {
			hardware_update();
			ops = 0;
		}
	}
}

void hardware_update()
{
	static UINT32 prev_time = 0;
	UINT32 cur_time = timeGetTime();
	
	if(prev_time != cur_time) {
		// update pit and raise irq0
#ifndef PIT_ALWAYS_RUNNING
		if(pit_active)
#endif
		{
			if(pit_run(0, cur_time)) {
				pic_req(0, 0, 1);
			}
			pit_run(1, cur_time);
			pit_run(2, cur_time);
		}
		// check key input and raise irq1
		static UINT32 prev_100ms = 0;
		UINT32 cur_100ms = cur_time / 100;
		if(prev_100ms != cur_100ms) {
			if(check_key_input()) {
				pic_req(0, 1, 1);
			}
			prev_100ms = cur_100ms;
		}
		prev_time = cur_time;
	}
}

// pic

void pic_init()
{
	memset(pic, 0, sizeof(pic));
	pic[0].imr = pic[1].imr = 0xff;
	
	// from bochs bios
	pic_write(0, 0, 0x11);	// icw1 = 11h
	pic_write(0, 1, 0x08);	// icw2 = 08h
	pic_write(0, 1, 0x04);	// icw3 = 04h
	pic_write(0, 1, 0x01);	// icw4 = 01h
	pic_write(0, 1, 0xb8);	// ocw1 = b8h
	pic_write(1, 0, 0x11);	// icw1 = 11h
	pic_write(1, 1, 0x70);	// icw2 = 70h
	pic_write(1, 1, 0x02);	// icw3 = 02h
	pic_write(1, 1, 0x01);	// icw4 = 01h
}

void pic_write(int c, UINT32 addr, UINT8 data)
{
	if(addr & 1) {
		if(pic[c].icw2_r) {
			// icw2
			pic[c].icw2 = data;
			pic[c].icw2_r = 0;
		} else if(pic[c].icw3_r) {
			// icw3
			pic[c].icw3 = data;
			pic[c].icw3_r = 0;
		} else if(pic[c].icw4_r) {
			// icw4
			pic[c].icw4 = data;
			pic[c].icw4_r = 0;
		} else {
			// ocw1
			pic[c].imr = data;
		}
	} else {
		if(data & 0x10) {
			// icw1
			pic[c].icw1 = data;
			pic[c].icw2_r = 1;
			pic[c].icw3_r = (data & 2) ? 0 : 1;
			pic[c].icw4_r = data & 1;
			pic[c].irr = 0;
			pic[c].isr = 0;
			pic[c].imr = 0;
			pic[c].prio = 0;
			if(!(pic[c].icw1 & 1)) {
				pic[c].icw4 = 0;
			}
			pic[c].ocw3 = 0;
		} else if(data & 8) {
			// ocw3
			if(!(data & 2)) {
				data = (data & ~1) | (pic[c].ocw3 & 1);
			}
			if(!(data & 0x40)) {
				data = (data & ~0x20) | (pic[c].ocw3 & 0x20);
			}
			pic[c].ocw3 = data;
		} else {
			// ocw2
			int level = 0;
			if(data & 0x40) {
				level = data & 7;
			} else {
				if(!pic[c].isr) {
					return;
				}
				level = pic[c].prio;
				while(!(pic[c].isr & (1 << level))) {
					level = (level + 1) & 7;
				}
			}
			if(data & 0x80) {
				pic[c].prio = (level + 1) & 7;
			}
			if(data & 0x20) {
				pic[c].isr &= ~(1 << level);
			}
		}
	}
	pic_update();
}

UINT8 pic_read(int c, UINT32 addr)
{
	if(addr & 1) {
		return(pic[c].imr);
	} else {
		// polling mode is not supported...
		//if(pic[c].ocw3 & 4) {
		//	return ???;
		//}
		if(pic[c].ocw3 & 1) {
			return(pic[c].isr);
		} else {
			return(pic[c].irr);
		}
	}
}

void pic_req(int c, int level, int signal)
{
	if(signal) {
		pic[c].irr |= (1 << level);
	} else {
		pic[c].irr &= ~(1 << level);
	}
	pic_update();
}

int pic_ack()
{
	// ack (INTA=L)
	pic[pic_req_chip].isr |= pic_req_bit;
	pic[pic_req_chip].irr &= ~pic_req_bit;
	if(pic_req_chip > 0) {
		// update isr and irr of master
		UINT8 slave = 1 << (pic[pic_req_chip].icw3 & 7);
		pic[pic_req_chip - 1].isr |= slave;
		pic[pic_req_chip - 1].irr &= ~slave;
	}
	//if(pic[pic_req_chip].icw4 & 1) {
		// 8086 mode
		int vector = (pic[pic_req_chip].icw2 & 0xf8) | pic_req_level;
	//} else {
	//	// 8080 mode
	//	UINT16 addr = (UINT16)pic[pic_req_chip].icw2 << 8;
	//	if(pic[pic_req_chip].icw1 & 4) {
	//		addr |= (pic[pic_req_chip].icw1 & 0xe0) | (pic_req_level << 2);
	//	} else {
	//		addr |= (pic[pic_req_chip].icw1 & 0xc0) | (pic_req_level << 3);
	//	}
	//	vector = 0xcd | (addr << 8);
	//}
	if(pic[pic_req_chip].icw4 & 2) {
		// auto eoi
		pic[pic_req_chip].isr &= ~pic_req_bit;
	}
	return(vector);
}

void pic_update()
{
	for(int c = 0; c < 2; c++) {
		UINT8 irr = pic[c].irr;
		if(c + 1 < 2) {
			// this is master
			if(pic[c + 1].irr & (~pic[c + 1].imr)) {
				// request from slave
				irr |= 1 << (pic[c + 1].icw3 & 7);
			}
		}
		irr &= (~pic[c].imr);
		if(!irr) {
			break;
		}
		if(!(pic[c].ocw3 & 0x20)) {
			irr |= pic[c].isr;
		}
		int level = pic[c].prio;
		UINT8 bit = 1 << level;
		while(!(irr & bit)) {
			level = (level + 1) & 7;
			bit = 1 << level;
		}
		if((c + 1 < 2) && (pic[c].icw3 & bit)) {
			// check slave
			continue;
		}
		if(pic[c].isr & bit) {
			break;
		}
		// interrupt request
		pic_req_chip = c;
		pic_req_level = level;
		pic_req_bit = bit;
		i386_set_irq_line(INPUT_LINE_IRQ, HOLD_LINE);
		return;
	}
	i386_set_irq_line(INPUT_LINE_IRQ, CLEAR_LINE);
}

// pit

#define PIT_FREQ 1193182
#define PIT_COUNT_VALUE(n) ((pit[n].count_reg == 0) ? 0x10000 : (pit[n].mode == 3 && pit[n].count_reg == 1) ? 0x10001 : pit[n].count_reg)

void pit_init()
{
	memset(pit, 0, sizeof(pit));
	for(int ch = 0; ch < 3; ch++) {
		pit[ch].count = 0x10000;
		pit[ch].ctrl_reg = 0x34;
		pit[ch].mode = 3;
	}
	
	// from bochs bios
	pit_write(3, 0x34);
	pit_write(0, 0x00);
	pit_write(0, 0x00);
}

void pit_write(int ch, UINT8 val)
{
#ifndef PIT_ALWAYS_RUNNING
	if(!pit_active) {
		pit_active = 1;
		pit_init();
	}
#endif
	switch(ch) {
	case 0:
	case 1:
	case 2:
		// write count register
		if(!pit[ch].low_write && !pit[ch].high_write) {
			if(pit[ch].ctrl_reg & 0x10) {
				pit[ch].low_write = 1;
			}
			if(pit[ch].ctrl_reg & 0x20) {
				pit[ch].high_write = 1;
			}
		}
		if(pit[ch].low_write) {
			pit[ch].count_reg = val;
			pit[ch].low_write = 0;
		} else if(pit[ch].high_write) {
			if((pit[ch].ctrl_reg & 0x30) == 0x20) {
				pit[ch].count_reg = val << 8;
			} else {
				pit[ch].count_reg |= val << 8;
			}
			pit[ch].high_write = 0;
		}
		// start count
		if(!pit[ch].low_write && !pit[ch].high_write) {
			if(pit[ch].mode == 0 || pit[ch].mode == 4 || pit[ch].prev_time == 0) {
				pit[ch].count = PIT_COUNT_VALUE(ch);
				pit[ch].prev_time = timeGetTime();
				pit[ch].expired_time = pit[ch].prev_time + pit_get_expired_time(ch);
			}
		}
		break;
	case 3: // ctrl reg
		if((val & 0xc0) == 0xc0) {
			// i8254 read-back command
			for(ch = 0; ch < 3; ch++) {
				UINT8 bit = 2 << ch;
				if(!(val & 0x10) && !pit[ch].status_latched) {
					pit[ch].status = pit[ch].ctrl_reg & 0x3f;
					pit[ch].status_latched = 1;
				}
				if(!(val & 0x20) && !pit[ch].count_latched) {
					pit_latch_count(ch);
				}
			}
			break;
		}
		ch = (val >> 6) & 3;
		if(val & 0x30) {
			static int modes[8] = {0, 1, 2, 3, 4, 5, 2, 3};
			pit[ch].mode = modes[(val >> 1) & 7];
			pit[ch].count_latched = 0;
			pit[ch].low_read = pit[ch].high_read = 0;
			pit[ch].low_write = pit[ch].high_write = 0;
			pit[ch].ctrl_reg = val;
			// stop count
			pit[ch].prev_time = pit[ch].expired_time = 0;
			pit[ch].count_reg = 0;
		} else if(!pit[ch].count_latched) {
			pit_latch_count(ch);
		}
		break;
	}
}

UINT8 pit_read(int ch)
{
#ifndef PIT_ALWAYS_RUNNING
	if(!pit_active) {
		pit_active = 1;
		pit_init();
	}
#endif
	switch(ch) {
	case 0:
	case 1:
	case 2:
		if(pit[ch].status_latched) {
			pit[ch].status_latched = 0;
			return(pit[ch].status);
		}
		// if not latched, through current count
		if(!pit[ch].count_latched) {
			if(!pit[ch].low_read && !pit[ch].high_read) {
				pit_latch_count(ch);
			}
		}
		// return latched count
		if(pit[ch].low_read) {
			pit[ch].low_read = 0;
			if(!pit[ch].high_read) {
				pit[ch].count_latched = 0;
			}
			return(pit[ch].latch & 0xff);
		} else if(pit[ch].high_read) {
			pit[ch].high_read = 0;
			pit[ch].count_latched = 0;
			return((pit[ch].latch >> 8) & 0xff);
		}
	}
	return(0xff);
}

int pit_run(int ch, UINT32 cur_time)
{
	if(pit[ch].expired_time != 0 && cur_time >= pit[ch].expired_time) {
		pit[ch].count = PIT_COUNT_VALUE(ch);
		pit[ch].prev_time = pit[ch].expired_time;
		pit[ch].expired_time = pit[ch].prev_time + pit_get_expired_time(ch);
		if(cur_time >= pit[ch].expired_time) {
			pit[ch].prev_time = cur_time;
			pit[ch].expired_time = pit[ch].prev_time + pit_get_expired_time(ch);
		}
		return(1);
	}
	return(0);
}

void pit_latch_count(int ch)
{
	UINT32 cur_time = timeGetTime();
	
	if(pit[ch].expired_time != 0) {
		pit_run(ch, cur_time);
		UINT32 tmp = (pit[ch].count * (pit[ch].expired_time - cur_time)) / (pit[ch].expired_time - pit[ch].prev_time);
		pit[ch].latch = (tmp != 0) ? (UINT16)tmp : 1;
	} else {
		pit[ch].latch = (UINT16)pit[ch].count;
	}
	pit[ch].count_latched = 1;
	if((pit[ch].ctrl_reg & 0x30) == 0x10) {
		// lower byte
		pit[ch].low_read = 1;
		pit[ch].high_read = 0;
	} else if((pit[ch].ctrl_reg & 0x30) == 0x20) {
		// upper byte
		pit[ch].low_read = 0;
		pit[ch].high_read = 1;
	} else {
		// lower -> upper
		pit[ch].low_read = pit[ch].low_read = 1;
	}
}

int pit_get_expired_time(int ch)
{
	UINT32 val = (1000 * pit[ch].count) / PIT_FREQ;
	return((val > 0) ? val : 1);
}

// cmos

void cmos_init()
{
	memset(cmos, 0, sizeof(cmos));
	cmos_addr = 0;
	
	// from DOSBox
	cmos_write(0x0a, 0x26);
	cmos_write(0x0b, 0x02);
	cmos_write(0x0d, 0x80);
}

void cmos_write(int addr, UINT8 val)
{
	cmos[addr & 0x7f] = val;
}

#define CMOS_GET_TIME() { \
	UINT32 cur_sec = timeGetTime() / 1000 ; \
	if(prev_sec != cur_sec) { \
		GetLocalTime(&time); \
		prev_sec = cur_sec; \
	} \
}

// kbd (a20)

void kbd_init()
{
	kbd_data = kbd_command = 0;
	kbd_status = 0x18;
}

UINT8 kbd_read_data()
{
	kbd_status &= ~1;
	return(kbd_data);
}

void kbd_write_data(UINT8 val)
{
	switch(kbd_command) {
	case 0xd1:
		i386_set_a20_line((val >> 1) & 1);
		break;
	}
	kbd_command = 0;
	kbd_status &= ~8;
}

UINT8 kbd_read_status()
{
	return(kbd_status);
}

void kbd_write_command(UINT8 val)
{
	switch(val) {
	case 0xd0:
		kbd_data = ((m_a20_mask >> 19) & 2) | 1;
		kbd_status |= 1;
		break;
	case 0xdd:
		i386_set_a20_line(0);
		break;
	case 0xdf:
		i386_set_a20_line(1);
		break;
	case 0xf0:
	case 0xf1:
	case 0xf2:
	case 0xf3:
	case 0xf4:
	case 0xf5:
	case 0xf6:
	case 0xf7:
	case 0xf8:
	case 0xf9:
	case 0xfa:
	case 0xfb:
	case 0xfc:
	case 0xfd:
	case 0xfe:
	case 0xff:
		if(!(val & 1)) {
			if((cmos[0x0f] & 0x7f) == 5) {
				// reset pic
				pic_init();
				pic[0].irr = pic[1].irr = 0x00;
				pic[0].imr = pic[1].imr = 0xff;
			}
			CPU_RESET_CALL(CPU_MODEL);
			i386_jmp_far(0x40, 0x67);
		}
		i386_set_a20_line((val >> 1) & 1);
		break;
	}
	kbd_command = val;
	kbd_status |= 8;
}

// vga

UINT8 vga_read_status()
{
	// 60hz
	static const int period[3] = {16, 17, 17};
	static int index = 0;
	UINT32 time = timeGetTime() % period[index];
	
	index = (index + 1) % 3;
	return((time < 4 ? 0x08 : 0) | 0x01);
}

// i/o bus

UINT8 read_io_byte(offs_t addr)
{
	switch(addr) {
	case 0x20:
	case 0x21:
		return(pic_read(0, addr));
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
		return(pit_read(addr & 0x03));
	case 0x60:
		return(kbd_read_data());
	case 0x61:
		return(system_port);
	case 0x64:
		return(kbd_read_status());
	//case 0x71:
	//	return(cmos_read(cmos_addr));
	case 0x92:
		return((m_a20_mask >> 19) & 2);
	case 0xa0:
	case 0xa1:
		return(pic_read(1, addr));
	case 0x3ba:
	case 0x3da:
		return(vga_read_status());
	default:
//		error("inb %4x\n", addr);
		break;
	}
	return(0xff);
}

UINT16 read_io_word(offs_t addr)
{
	return(read_io_byte(addr) | (read_io_byte(addr + 1) << 8));
}

UINT32 read_io_dword(offs_t addr)
{
	return(read_io_byte(addr) | (read_io_byte(addr + 1) << 8) | (read_io_byte(addr + 2) << 16) | (read_io_byte(addr + 3) << 24));
}

void write_io_byte(offs_t addr, UINT8 val)
{
	switch(addr) {
	case 0x20:
	case 0x21:
		pic_write(0, addr, val);
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
		pit_write(addr & 0x03, val);
		break;
	case 0x60:
		kbd_write_data(val);
		break;
	case 0x61:
		if((system_port & 3) != 3 && (val & 3) == 3) {
			// beep on
//			MessageBeep(-1);
		} else if((system_port & 3) == 3 && (val & 3) != 3) {
			// beep off
		}
		system_port = val;
		break;
	case 0x64:
		kbd_write_command(val);
		break;
	case 0x70:
		cmos_addr = val;
		break;
	case 0x71:
		cmos_write(cmos_addr, val);
		break;
	case 0x92:
		i386_set_a20_line((val >> 1) & 1);
		break;
	case 0xa0:
	case 0xa1:
		pic_write(1, addr, val);
		break;
	default:
//		error("outb %4x,%2x\n", addr, val);
		break;
	}
}

void write_io_word(offs_t addr, UINT16 val)
{
	write_io_byte(addr + 0, (val >> 0) & 0xff);
	write_io_byte(addr + 1, (val >> 8) & 0xff);
}

void write_io_dword(offs_t addr, UINT32 val)
{
	write_io_byte(addr + 0, (val >>  0) & 0xff);
	write_io_byte(addr + 1, (val >>  8) & 0xff);
	write_io_byte(addr + 2, (val >> 16) & 0xff);
	write_io_byte(addr + 3, (val >> 24) & 0xff);
}

BOOL is_32bit_segment(int sreg);
extern "C"
{
	__declspec(dllimport) PVOID getWOW32Reserved();
	__declspec(dllimport) PVOID setWOW32Reserved(PVOID w);
	unsigned short wine_ldt_alloc_entries(int count);
	int wine_ldt_set_entry(unsigned short sel, const LDT_ENTRY *entry);
	void wine_ldt_get_entry(unsigned short sel, LDT_ENTRY *entry);
	//_declspec(dllimport) struct __wine_ldt_copy
	//{
	//	void         *base[8192];  /* base address or 0 if entry is free   */
	//	unsigned long limit[8192]; /* limit in bytes or 0 if entry is free */
	//	unsigned char flags[8192]; /* flags (defined below) */
	//} wine_ldt_copy;
#define WINE_LDT_FLAGS_DATA      0x13  /* Data segment */
	/* helper functions to manipulate the LDT_ENTRY structure */
	static inline void wine_ldt_set_base(LDT_ENTRY *ent, const void *base)
	{
		ent->BaseLow = (WORD)(ULONG_PTR)base;
		ent->HighWord.Bits.BaseMid = (BYTE)((ULONG_PTR)base >> 16);
		ent->HighWord.Bits.BaseHi = (BYTE)((ULONG_PTR)base >> 24);
	}
	static inline void wine_ldt_set_limit(LDT_ENTRY *ent, unsigned int limit)
	{
		if ((ent->HighWord.Bits.Granularity = (limit >= 0x100000))) limit >>= 12;
		ent->LimitLow = (WORD)limit;
		ent->HighWord.Bits.LimitHi = (limit >> 16);
	}
	static inline void *wine_ldt_get_base(const LDT_ENTRY *ent)
	{
		return (void *)(ent->BaseLow |
			(ULONG_PTR)ent->HighWord.Bits.BaseMid << 16 |
			(ULONG_PTR)ent->HighWord.Bits.BaseHi << 24);
	}
	static inline unsigned int wine_ldt_get_limit(const LDT_ENTRY *ent)
	{
		unsigned int limit = ent->LimitLow | (ent->HighWord.Bits.LimitHi << 16);
		if (ent->HighWord.Bits.Granularity) limit = (limit << 12) | 0xfff;
		return limit;
	}
#define WINE_LDT_FLAGS_32BIT     0x40  /* Segment is 32-bit (code or stack) */
	static inline void wine_ldt_set_flags(LDT_ENTRY *ent, unsigned char flags)
	{
		ent->HighWord.Bits.Dpl = 3;
		ent->HighWord.Bits.Pres = 1;
		ent->HighWord.Bits.Type = flags;
		ent->HighWord.Bits.Sys = 0;
		ent->HighWord.Bits.Reserved_0 = 0;
		ent->HighWord.Bits.Default_Big = (flags & WINE_LDT_FLAGS_32BIT) != 0;
	}
	static inline unsigned char wine_ldt_get_flags(const LDT_ENTRY *ent)
	{
		unsigned char ret = ent->HighWord.Bits.Type;
		if (ent->HighWord.Bits.Default_Big) ret |= WINE_LDT_FLAGS_32BIT;
		return ret;
	}
	static inline int wine_ldt_is_empty(const LDT_ENTRY *ent)
	{
		const DWORD *dw = (const DWORD *)ent;
		return (dw[0] | dw[1]) == 0;
	}
	/***********************************************************************
	*           SELECTOR_SetEntries
	*
	* Set the LDT entries for an array of selectors.
	*/
	static BOOL SELECTOR_SetEntries(WORD sel, const void *base, DWORD size, unsigned char flags)
	{
		LDT_ENTRY entry;
		WORD i, count;

		wine_ldt_set_base(&entry, base);
		wine_ldt_set_limit(&entry, size - 1);
		wine_ldt_set_flags(&entry, flags);
		count = (size + 0xffff) / 0x10000;
		for (i = 0; i < count; i++)
		{
			if (wine_ldt_set_entry(sel + (i << 3), &entry) < 0) return FALSE;
			wine_ldt_set_base(&entry, (char*)wine_ldt_get_base(&entry) + 0x10000);
			/* yep, Windows sets limit like that, not 64K sel units */
			wine_ldt_set_limit(&entry, wine_ldt_get_limit(&entry) - 0x10000);
		}
		return TRUE;
	}
	void wine_ldt_free_entries(unsigned short sel, int count);
	/***********************************************************************
	*           SELECTOR_AllocBlock
	*
	* Allocate selectors for a block of linear memory.
	*/
	WORD SELECTOR_AllocBlock(const void *base, DWORD size, unsigned char flags)
	{
		WORD sel, count;

		if (!size) return 0;
		count = (size + 0xffff) / 0x10000;
		if ((sel = wine_ldt_alloc_entries(count)))
		{
			if (SELECTOR_SetEntries(sel, base, size, flags)) return sel;
			wine_ldt_free_entries(sel, count);
			sel = 0;
		}
		return sel;
	}
	void save_context(CONTEXT *context)
	{
		context->Eax = REG16(AX);
		context->Ecx = REG16(CX);
		context->Edx = REG16(DX);
		context->Ebx = REG16(BX);
		context->Esp = REG16(SP);
		context->Ebp = REG16(BP);
		context->Esi = REG16(SI);
		context->Edi = REG16(DI);
		context->SegEs = SREG(ES);
		context->SegCs = SREG(CS);
		context->SegSs = SREG(SS);
		context->SegDs = SREG(DS);
		setWOW32Reserved((PVOID)(SREG(SS) << 16 | REG16(SP)));
	}
	void load_context(CONTEXT *context)
	{
		REG16(AX) = (WORD)context->Eax;
		REG16(CX) = (WORD)context->Ecx;
		REG16(DX) = (WORD)context->Edx;
		REG16(BX) = (WORD)context->Ebx;
		REG16(SP) = (WORD)context->Esp;
		REG16(BP) = (WORD)context->Ebp;
		REG16(SI) = (WORD)context->Esi;
		REG16(DI) = (WORD)context->Edi;
		SREG(ES) = (WORD)context->SegEs;
		SREG(CS) = (WORD)context->SegCs;
		SREG(SS) = (WORD)context->SegSs;
		SREG(DS) = (WORD)context->SegDs;//ES, CS, SS, DS
		//ES, CS, SS, DS, FS, GS
		SREG(FS) = (WORD)context->SegFs;
		SREG(GS) = (WORD)context->SegGs;
		i386_load_segment_descriptor(ES);
		i386_load_segment_descriptor(SS);
		i386_load_segment_descriptor(DS);
		i386_load_segment_descriptor(FS);
		i386_load_segment_descriptor(GS);
	}
	void __wine_call_int_handler(CONTEXT *context, BYTE intnum);
	void WINAPI DOSVM_Int21Handler(CONTEXT *context);
	unsigned char iret[100] = { 0xcf };
	WORD SELECTOR_AllocBlock(const void *base, DWORD size, unsigned char flags);
	//__declspec(dllexport) void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler);
	void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler, void(*from16_reg)(void), LONG (*__wine_call_from_16)(void), int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context))
	{
		//wine_ldt_copy.base[0] = iret;
		WORD sel = SELECTOR_AllocBlock(iret, 100, WINE_LDT_FLAGS_DATA);
		CPU_INIT_CALL(CPU_MODEL);
		CPU_RESET_CALL(CPU_MODEL);
		m_idtr.base = (UINT32)iret;
		*(WORD*)&iret[0x86] = sel;
		*(WORD*)&iret[0x84] = 0;
#if defined(HAS_I386)
		cpu_type = (REG32(EDX) >> 8) & 0x0f;
		cpu_step = (REG32(EDX) >> 0) & 0x0f;
#endif
		//m_amask = -1;
		m_a20_mask = -1;
		set_flags(0x20000);//V8086_MODE
		assert(V8086_MODE);
		REG16(AX) = (WORD)context->Eax;
		REG16(CX) = (WORD)context->Ecx;
		REG16(DX) = (WORD)context->Edx;
		REG16(BX) = (WORD)context->Ebx;
		REG16(SP) = (WORD)context->Esp;
		REG16(BP) = (WORD)context->Ebp;
		REG16(SI) = (WORD)context->Esi;
		REG16(DI) = (WORD)context->Edi;
		SREG(ES) = (WORD)context->SegEs;
		SREG(CS) = (WORD)context->SegCs;
		SREG(SS) = (WORD)context->SegSs;
		SREG(DS) = (WORD)context->SegDs;//ES, CS, SS, DS
		//ES, CS, SS, DS, FS, GS
		SREG(FS) = (WORD)context->SegFs;
		SREG(GS) = (WORD)context->SegGs;
		i386_load_segment_descriptor(ES);
		i386_jmp_far(context->SegCs, context->Eip);
		i386_load_segment_descriptor(SS);
		i386_load_segment_descriptor(DS);
		i386_load_segment_descriptor(FS);
		i386_load_segment_descriptor(GS);
		dasm = true;
		while (!m_halted) {
			bool reg = false;
			if (m_pc == (UINT)/*ptr!*/iret)
			{
				CONTEXT context;
				save_context(&context);
				__wine_call_int_handler(&context, 0x21);
				load_context(&context);
			}
			if ((void(*)(void))m_eip == from16_reg)
			{
				reg = true;
			}
			if ((LONG(*)(void))m_eip == __wine_call_from_16 || reg)
			{
				unsigned char *stack1 = (unsigned char*)i386_translate(SS, REG16(SP), 0);
				unsigned char *stack = stack1;
				/*
				* (sp+24) word   first 16-bit arg
				* (sp+22) word   cs
				* (sp+20) word   ip
				* (sp+18) word   bp
				* (sp+14) long   32-bit entry point (reused for Win16 mutex recursion count)
				* (sp+12) word   ip of actual entry point (necessary for relay debugging)
				* (sp+8)  long   relay (argument conversion) function entry point
				* (sp+4)  long   cs of 16-bit entry point
				* (sp)    long   ip of 16-bit entry point
				*/
				DWORD ip = *(DWORD*)stack;
				stack += sizeof(DWORD);
				DWORD cs = *(DWORD*)stack;
				stack += sizeof(DWORD);
				DWORD relay = *(DWORD*)stack;
				stack += sizeof(DWORD);
				WORD ip2 = *(WORD*)stack;
				stack += sizeof(WORD);
				DWORD entry = *(DWORD*)stack;
				stack += sizeof(DWORD);
				WORD bp = *(WORD*)stack;
				stack += sizeof(WORD);
				WORD ip19 = *(WORD*)stack;
				stack += sizeof(WORD);
				WORD cs16 = *(WORD*)stack;
				stack += sizeof(WORD);
				WORD *args = (WORD*)stack;
				m_eip = ip;
				m_sreg[CS].selector = cs;
				i386_load_segment_descriptor(CS);
				CHANGE_PC(m_eip);
				if (relay == (UINT)relay_call_from_16)
				{
					CONTEXT context;
					WORD osp = REG16(SP);
					PUSH16(SREG(GS));
					PUSH16(SREG(FS));
					PUSH16(SREG(ES));
					PUSH16(SREG(DS));
					PUSH32(REG32(EBP));
					PUSH32(REG32(ECX));
					PUSH32(REG32(EDX));
					PUSH32(/*context.Esp*/osp);
					save_context(&context);
					int fret = relay_call_from_16((void*)entry, (unsigned char*)args, &context);
					REG16(AX) = (WORD)context.Eax;
					REG16(CX) = (WORD)context.Ecx;
					REG16(DX) = (WORD)context.Edx;
					REG16(BX) = (WORD)context.Ebx;
					REG16(SP) = (WORD)context.Esp;
					REG16(BP) = (WORD)context.Ebp;
					REG16(SI) = (WORD)context.Esi;
					REG16(DI) = (WORD)context.Edi;
					SREG(ES) = (WORD)context.SegEs;
					SREG(CS) = (WORD)context.SegCs;
					SREG(SS) = (WORD)context.SegSs;
					SREG(DS) = (WORD)context.SegDs;//ES, CS, SS, DS
					//ES, CS, SS, DS, FS, GS
					SREG(FS) = (WORD)context.SegFs;
					SREG(GS) = (WORD)context.SegGs;
					REG16(SP) = osp + 18;
					if (!reg) REG16(SP) += 2;
					i386_load_segment_descriptor(ES);
					i386_load_segment_descriptor(SS);
					i386_load_segment_descriptor(DS);
					i386_load_segment_descriptor(FS);
					i386_load_segment_descriptor(GS);
				}
				/*
				m_eip = POP32();
				m_sreg[CS].selector = POP32();
				i386_load_segment_descriptor(CS);
				CHANGE_PC(m_eip);
				from16_reg();
				UINT addr = POP32();
				if (addr == (UINT)relay_call_from_16)
				{
					UINT16 retadr2 = POP16();
					void *func = (void*)POP32();
					UINT16 bp = POP16();
					m_eip = POP16();
					m_sreg[CS].selector = POP16();
					i386_load_segment_descriptor(CS);
					CHANGE_PC(m_eip);
					unsigned char *args = (unsigned char*)i386_translate(SS, REG32(SP), 0);
					CONTEXT context;
					context.Eax = REG16(AX);
					context.Ecx = REG16(CX);
					context.Edx = REG16(DX);
					context.Ebx = REG16(BX);
					context.Esp = REG16(SP);
					context.Ebp = REG16(BP);
					context.Esi = REG16(SI);
					context.Edi = REG16(DI);
					context.SegEs = SREG(ES);
					context.SegCs = SREG(CS);
					context.SegSs = SREG(SS);
					context.SegDs = SREG(DS);
					int fret = relay_call_from_16(func, args, &context);
					REG16(AX) = fret & 0xFFFF;
					REG16(DX) = fret >> 16;
				}
				else
				{
					printf("????\n");
				}*/
				//i386_jmp_far(SREG(CS), addr);
				//
			}
			if (is_32bit_segment(CS))
			{
				if (m_VM)
				{
					printf("==ENTER 32BIT CODE==\n");
				}
				m_VM = 0;
				m_eflags &= ~0x20000;
				//????
				m_sreg[CS].d = 1;
				m_operand_size = 1;
			}
			else
			{
				if (!m_VM)
				{
					printf("==ENTER 16BIT CODE==\n");
				}
				m_VM = 1;
				m_eflags |= 0x20000;
				m_sreg[CS].d = 0;
				m_operand_size = 0;
			}
#ifdef SUPPORT_DISASSEMBLER
			if (dasm) {
				char buffer[256];
#if defined(HAS_I386)
				UINT64 eip = m_eip;
#else
				UINT64 eip = m_pc - SREG_BASE(CS);
#endif
				UINT8 *oprom = mem + SREG_BASE(CS) + eip;

#if defined(HAS_I386)
				if (m_operand_size) {
					CPU_DISASSEMBLE_CALL(x86_32);
				}
				else
#endif
					CPU_DISASSEMBLE_CALL(x86_16);
				fprintf(stderr, "%04x:%04x\t%s\n", SREG(CS), (unsigned)eip, buffer);
			}
#endif
#if defined(HAS_I386)
			m_cycles = 1;
			CPU_EXECUTE_CALL(i386);
#else
			CPU_EXECUTE_CALL(CPU_MODEL);
#endif
		}
	}
	void *wine_ldt_get_ptr(unsigned short sel, unsigned long offset);
	void wine_ldt_get_entry(unsigned short sel, LDT_ENTRY *entry);
	/*static inline unsigned char wine_ldt_get_flags(const LDT_ENTRY *ent)
	{
		unsigned char ret = ent->HighWord.Bits.Type;
		if (ent->HighWord.Bits.Default_Big) ret |= WINE_LDT_FLAGS_32BIT;
		return ret;
	}*/
}
BOOL is_32bit_segment(int sreg)
{
	LDT_ENTRY entry;
	wine_ldt_get_entry(SREG(sreg), &entry);
	if (wine_ldt_get_flags(&entry) & WINE_LDT_FLAGS_32BIT)
	{
		return TRUE;
	}
	return FALSE;
}
UINT get_segment_descriptor_wine(int sreg)
{
	LDT_ENTRY entry;
	wine_ldt_get_entry(SREG(sreg), &entry);
	if (wine_ldt_get_flags(&entry) & WINE_LDT_FLAGS_32BIT)
	{
		//printf("WINE_LDT_FLAGS_32BIT\n");
	}
	void *ptr = wine_ldt_get_ptr(SREG(sreg), 0);
	return (UINT)ptr;
	//m_base[sreg] = SegBase(sreg);
}
void load_segment_descriptor_wine(int sreg)
{
	LDT_ENTRY entry;
	wine_ldt_get_entry(SREG(sreg), &entry);
	if (wine_ldt_get_flags(&entry) & WINE_LDT_FLAGS_32BIT)
	{
		//printf("WINE_LDT_FLAGS_32BIT\n");
	}
	void *ptr = wine_ldt_get_ptr(SREG(sreg), 0);
	SREG_BASE(sreg) = (UINT)ptr;
	//m_base[sreg] = SegBase(sreg);
}
