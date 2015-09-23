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
}

void write_word(offs_t byteaddress, UINT16 data)
{
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
	}
}

void write_dword(offs_t byteaddress, UINT32 data)
{
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
	main
---------------------------------------------------------------------------- */

bool is_started_from_command_prompt()
{
	bool ret = false;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot != INVALID_HANDLE_VALUE) {
		DWORD dwParentProcessID = 0;
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if(Process32First(hSnapshot, &pe32)) {
			do {
				if(pe32.th32ProcessID == GetCurrentProcessId()) {
					dwParentProcessID = pe32.th32ParentProcessID;
					break;
				}
			} while(Process32Next(hSnapshot, &pe32));
		}
		CloseHandle(hSnapshot);
		if(dwParentProcessID != 0) {
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwParentProcessID);
			if(hProcess != NULL) {
				HMODULE hMod;
				DWORD cbNeeded;
				if(EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
					char module_name[MAX_PATH];
					if(GetModuleBaseName(hProcess, hMod, module_name, sizeof(module_name))) {
						ret = (_strnicmp(module_name, "cmd.exe", 7) == 0);
					}
				}
				CloseHandle(hProcess);
			}
		}
	}
	return(ret);
}

#define IS_NUMERIC(c) ((c) >= '0' && (c) <= '9')

int main(int argc, char *argv[], char *envp[])
{
	int arg_offset = 0;
	int standard_env = 0;
	BOOL bSuccess;
	
	for(int i = 1; i < argc; i++) {
		if(_strnicmp(argv[i], "-e", 2) == 0) {
			standard_env = 1;
			arg_offset++;
		} else if(_strnicmp(argv[i], "-v", 2) == 0) {
			if(strlen(argv[i]) >= 6 && IS_NUMERIC(argv[i][2]) && argv[i][3] == '.' && IS_NUMERIC(argv[i][4]) && IS_NUMERIC(argv[i][5])) {
				major_version = argv[i][2] - '0';
				minor_version = (argv[i][4] - '0') * 10 + (argv[i][5] - '0');
			}
			arg_offset++;
		} else {
			break;
		}
	}
	
	if(argc < 2 + arg_offset) {
#ifdef _WIN64
		fprintf(stderr, "MS-DOS Player for Win32-x64 console\n\n");
#else
		fprintf(stderr, "MS-DOS Player for Win32 console\n\n");
#endif
		fprintf(stderr, "Usage: MSDOS [-e] [-vX.XX] (command file) [opions]\n");
		
		if(!is_started_from_command_prompt()) {
			fprintf(stderr, "\nStart this program from a command prompt!\n\nHit any key to quit...");
			while(!_kbhit()) {
				Sleep(10);
			}
		}
		return(EXIT_FAILURE);
	}
	
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	bSuccess = GetConsoleScreenBufferInfo(hStdout, &csbi);
	
	for(int y = 0; y < SCR_BUF_SIZE; y++) {
		for(int x = 0; x < 80; x++) {
			scr_buf[y][x].Char.AsciiChar = ' ';
			scr_buf[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}
	}
	scr_buf_size.X = 80;
	scr_buf_size.Y = SCR_BUF_SIZE;
	scr_buf_pos.X = scr_buf_pos.Y = 0;
	if(bSuccess) {
		scr_width = csbi.dwSize.X;
		scr_height = csbi.dwSize.Y;
	} else {
		// for a proof (not a console)
		scr_width = 80;
		scr_height = 25;
	}
	cursor_moved = false;
	
	key_buf_char = new FIFO();
	key_buf_scan = new FIFO();
	
	hardware_init();
	
	if(msdos_init(argc - (arg_offset + 1), argv + (arg_offset + 1), envp, standard_env)) {
		retval = EXIT_FAILURE;
	} else {
		TIMECAPS caps;
		timeGetDevCaps(&caps, sizeof(TIMECAPS));
		timeBeginPeriod(caps.wPeriodMin);
		hardware_run();
		if(bSuccess) {
			if(restore_console_on_exit) {
				SMALL_RECT rect = {0, 0, csbi.srWindow.Right - csbi.srWindow.Left, csbi.srWindow.Bottom - csbi.srWindow.Top};
				SetConsoleScreenBufferSize(hStdout, csbi.dwSize);
				SetConsoleWindowInfo(hStdout, TRUE, &rect);
			}
			SetConsoleTextAttribute(hStdout, csbi.wAttributes); // hStdout (and all handles) will close in msdos_finish()...
		}
		msdos_finish();
		timeEndPeriod(caps.wPeriodMin);
	}
	
	hardware_finish();
	
	delete key_buf_char;
	delete key_buf_scan;
	
//	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	
	return(retval);
}

void change_console_size_to_80x25()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT rect;
	COORD co;
	
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	if(csbi.srWindow.Top != 0 || csbi.dwCursorPosition.Y > 24) {
		if(csbi.srWindow.Right - csbi.srWindow.Left + 1 == 80 && csbi.srWindow.Bottom - csbi.srWindow.Top + 1 == 25) {
			ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &csbi.srWindow);
			SET_RECT(rect, 0, 0, 79, 24);
			WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		} else if(csbi.dwCursorPosition.Y > 24) {
			SET_RECT(rect, 0, csbi.dwCursorPosition.Y - 24, 79, csbi.dwCursorPosition.Y);
			ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
			SET_RECT(rect, 0, 0, 79, 24);
			WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		}
	}
	if(csbi.dwCursorPosition.Y > 24) {
		co.X = csbi.dwCursorPosition.X;
		co.Y = min(24, csbi.dwCursorPosition.Y - csbi.srWindow.Top);
		SetConsoleCursorPosition(hStdout, co);
		cursor_moved = true;
	}
	SET_RECT(rect, 0, 0, 79, 24);
	co.X = 80;
	co.Y = 25;
	SetConsoleWindowInfo(hStdout, TRUE, &rect);
	SetConsoleScreenBufferSize(hStdout, co);
	scr_width = 80;
	scr_height = 25;
}

/* ----------------------------------------------------------------------------
	MS-DOS virtual machine
---------------------------------------------------------------------------- */

void update_key_buffer_tmp()
{
	DWORD dwNumberOfEvents = 0;
	DWORD dwRead;
	INPUT_RECORD ir[16];
	
	if(GetNumberOfConsoleInputEvents(hStdin, &dwNumberOfEvents) && dwNumberOfEvents != 0) {
		if(ReadConsoleInputA(hStdin, ir, 16, &dwRead)) {
			for(int i = 0; i < dwRead; i++) {
				if((ir[i].EventType & KEY_EVENT) && ir[i].Event.KeyEvent.bKeyDown) {
					if(ir[i].Event.KeyEvent.uChar.AsciiChar == 0) {
						// ignore shift, ctrl and alt keys
						if(ir[i].Event.KeyEvent.wVirtualScanCode != 0x1d &&
						   ir[i].Event.KeyEvent.wVirtualScanCode != 0x2a &&
						   ir[i].Event.KeyEvent.wVirtualScanCode != 0x36 &&
						   ir[i].Event.KeyEvent.wVirtualScanCode != 0x38) {
							key_buf_char->write(0x00);
							key_buf_scan->write(ir[i].Event.KeyEvent.dwControlKeyState & ENHANCED_KEY ? 0xe0 : 0x00);
							key_buf_char->write(0x00);
							key_buf_scan->write(ir[i].Event.KeyEvent.wVirtualScanCode & 0xff);
						}
					} else {
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
	
	if(key_buf_char->count() == 0) {
		Sleep(10);
	}
}

int check_key_input()
{
	if(key_input == 0) {
		int prev_count = key_buf_char->count();
		update_key_buffer_tmp();
		key_input = key_buf_char->count() - prev_count;
	}
	int val = key_input;
	key_input = 0;
	return(val);
}

// process info

process_t *msdos_process_info_create(UINT16 psp_seg)
{
	for(int i = 0; i < MAX_PROCESS; i++) {
		if(process[i].psp == 0 || process[i].psp == psp_seg) {
			memset(&process[i], 0, sizeof(process_t));
			process[i].psp = psp_seg;
			return(&process[i]);
		}
	}
	fatalerror("too many processes\n");
	return(NULL);
}

process_t *msdos_process_info_get(UINT16 psp_seg)
{
	for(int i = 0; i < MAX_PROCESS; i++) {
		if(process[i].psp == psp_seg) {
			return(&process[i]);
		}
	}
	fatalerror("invalid psp address\n");
	return(NULL);
}

// dta info

void msdos_dta_info_init()
{
	for (int i = 0; i < MAX_DTAINFO; i++) {
		dtalist[i].find_handle = INVALID_HANDLE_VALUE;
	}
}

dtainfo_t *msdos_dta_info_get(UINT16 psp_seg, UINT32 dta_laddr)
{
	dtainfo_t *free_dta = NULL;
	for (int i = 0; i < MAX_DTAINFO; i++) {
		if (dtalist[i].find_handle == INVALID_HANDLE_VALUE) {
			if (free_dta == NULL) {
				free_dta = &dtalist[i];
			}
		} else if (dta_laddr < LFN_DTA_LADDR && dtalist[i].dta == dta_laddr) {
			return(&dtalist[i]);
		}
	}
	if (free_dta) {
		free_dta->psp = psp_seg;
		free_dta->dta = dta_laddr;
		return(free_dta);
	}
	fatalerror("too many dta\n");
	return(NULL);
}

void msdos_dta_info_free(UINT16 psp_seg)
{
	for (int i = 0; i < MAX_DTAINFO; i++) {
		if (dtalist[i].psp == psp_seg &&
			dtalist[i].find_handle != INVALID_HANDLE_VALUE) {
			FindClose(dtalist[i].find_handle);
			dtalist[i].find_handle = INVALID_HANDLE_VALUE;
		}
	}
}

void msdos_cds_update(int drv)
{
	cds_t *cds = (cds_t *)(mem + CDS_TOP);
	
	memset(mem + CDS_TOP, 0, CDS_SIZE);
	sprintf(cds->path_name, "%c:\\", 'A' + drv);
	cds->drive_attrib = 0x4000;	// physical drive
	cds->physical_drive_number = drv;
}

// dbcs

void msdos_dbcs_table_update()
{
	UINT8 dbcs_data[DBCS_SIZE];
	memset(dbcs_data, 0, sizeof(dbcs_data));
	
	CPINFO info;
	GetCPInfo(active_code_page, &info);
	
	if(info.MaxCharSize != 1) {
		for(int i = 0;; i += 2) {
			UINT8 lo = info.LeadByte[i + 0];
			UINT8 hi = info.LeadByte[i + 1];
			dbcs_data[2 + i + 0] = lo;
			dbcs_data[2 + i + 1] = hi;
			if(lo == 0 && hi == 0) {
				dbcs_data[0] = i + 2;
				break;
			}
		}
	} else {
		dbcs_data[0] = 2;	// ???
	}
	memcpy(mem + DBCS_TOP, dbcs_data, sizeof(dbcs_data));
}

void msdos_dbcs_table_init()
{
	system_code_page = active_code_page = _getmbcp();
	msdos_dbcs_table_update();
}

void msdos_dbcs_table_finish()
{
	if(active_code_page != system_code_page) {
		_setmbcp(system_code_page);
	}
}

int msdos_lead_byte_check(UINT8 code)
{
	UINT8 *dbcs_table = mem + DBCS_TABLE;
	
	for(int i = 0;; i += 2) {
		UINT8 lo = dbcs_table[i + 0];
		UINT8 hi = dbcs_table[i + 1];
		if(lo == 0 && hi == 0) {
			break;
		}
		if(lo <= code && code <= hi) {
			return(1);
		}
	}
	return(0);
}

// file control

char *msdos_trimmed_path(char *path, int lfn)
{
	static char tmp[MAX_PATH];
	
	if(lfn) {
		strcpy(tmp, path);
	} else {
		// remove space in the path
		char *src = path, *dst = tmp;
		
		while(*src != '\0') {
			if(msdos_lead_byte_check(*src)) {
				*dst++ = *src++;
				*dst++ = *src++;
			} else if(*src != ' ') {
				*dst++ = *src++;
			} else {
				src++;	// skip space
			}
		}
		*dst = '\0';
	}
	return(tmp);
}

bool match(char *text, char *pattern)
{
	//http://www.prefield.com/algorithm/string/wildcard.html
	switch (*pattern) {
	case '\0':
		return !*text;
	case '*':
		return match(text, pattern + 1) || *text && match(text + 1, pattern);
	case '?':
		return *text && match(text + 1, pattern + 1);
	default:
		return (*text == *pattern) && match(text + 1, pattern + 1);
	}
}

bool msdos_match_volume_label(char *path, char *volume)
{
	char *p;
	
	if((p = my_strchr(path, ':')) != NULL) {
		return msdos_match_volume_label(p + 1, volume);
	} else if((p = my_strchr(path, '\\')) != NULL) {
		return msdos_match_volume_label(p + 1, volume);
	} else if((p = my_strchr(path, '.')) != NULL) {
		*p = '\0';
		bool result = match(volume, path);
		*p = '.';
		return result;
	} else {
		return match(volume, path);
	}
}

char *msdos_fcb_path(fcb_t *fcb)
{
	static char tmp[MAX_PATH];
	char name[9], ext[4];
	
	memset(name, 0, sizeof(name));
	memcpy(name, fcb->file_name, 8);
	strcpy(name, msdos_trimmed_path(name, 0));
	
	memset(ext, 0, sizeof(ext));
	memcpy(ext, fcb->file_name + 8, 3);
	strcpy(ext, msdos_trimmed_path(ext, 0));
	
	if(name[0] == '\0' || strcmp(name, "????????") == 0) {
		strcpy(name, "*");
	}
	if(ext[0] == '\0') {
		strcpy(tmp, name);
	} else {
		if(strcmp(ext, "???") == 0) {
			strcpy(ext, "*");
		}
		sprintf(tmp, "%s.%s", name, ext);
	}
	return(tmp);
}

void msdos_set_fcb_path(fcb_t *fcb, char *path)
{
	char *ext = my_strchr(path, '.');
	
	memset(fcb->file_name, 0x20, 8 + 3);
	if(ext != NULL && path[0] != '.') {
		*ext = '\0';
		memcpy(fcb->file_name + 8, ext + 1, strlen(ext + 1));
	}
	memcpy(fcb->file_name, path, strlen(path));
}

char *msdos_short_path(char *path)
{
	static char tmp[MAX_PATH];
	
	GetShortPathName(path, tmp, MAX_PATH);
	my_strupr(tmp);
	return(tmp);
}

char *msdos_short_name(WIN32_FIND_DATA *fd)
{
	static char tmp[MAX_PATH];

	if (fd->cAlternateFileName[0]) {
		strcpy(tmp, fd->cAlternateFileName);
	} else {
		strcpy(tmp, fd->cFileName);
	}
	my_strupr(tmp);
	return(tmp);
}

char *msdos_short_full_path(char *path)
{
	static char tmp[MAX_PATH];
	char full[MAX_PATH], *name;
	
	GetFullPathName(path, MAX_PATH, full, &name);
	GetShortPathName(full, tmp, MAX_PATH);
	my_strupr(tmp);
	return(tmp);
}

char *msdos_short_full_dir(char *path)
{
	static char tmp[MAX_PATH];
	char full[MAX_PATH], *name;
	
	GetFullPathName(path, MAX_PATH, full, &name);
	name[-1] = '\0';
	GetShortPathName(full, tmp, MAX_PATH);
	my_strupr(tmp);
	return(tmp);
}

char *msdos_local_file_path(char *path, int lfn)
{
	char *trimmed = msdos_trimmed_path(path, lfn);
	
	if(_access(trimmed, 0) != 0) {
		process_t *process = msdos_process_info_get(current_psp);
		static char tmp[MAX_PATH];
		
		sprintf(tmp, "%s\\%s", process->module_dir, trimmed);
		if(_access(tmp, 0) == 0) {
			return(tmp);
		}
	}
	return(trimmed);
}

bool msdos_is_con_path(char *path)
{
	char full[MAX_PATH], *name;
	
	GetFullPathName(path, MAX_PATH, full, &name);
	return(_stricmp(full, "\\\\.\\CON") == 0);
}

char *msdos_remove_double_quote(char *path)
{
	static char tmp[MAX_PATH];
	
	memset(tmp, 0, sizeof(tmp));
	if(strlen(path) >= 2 && path[0] == '"' && path[strlen(path) - 1] == '"') {
		memcpy(tmp, path + 1, strlen(path) - 2);
	} else {
		sprintf(tmp, path);
	}
	return(tmp);
}

char *msdos_combine_path(char *dir, char *file)
{
	static char tmp[MAX_PATH];
	char *tmp_dir = msdos_remove_double_quote(dir);
	
	if(strlen(tmp_dir) == 0) {
		strcpy(tmp, file);
	} else if(tmp_dir[strlen(tmp_dir) - 1] == '\\') {
		sprintf(tmp, "%s%s", tmp_dir, file);
	} else {
		sprintf(tmp, "%s\\%s", tmp_dir, file);
	}
	return(tmp);
}

char *msdos_search_command_com(char *command_path, char *env_path)
{
	static char tmp[MAX_PATH];
	char path[MAX_PATH], *file_name;
	
	if(GetFullPathName(command_path, MAX_PATH, tmp, &file_name) != 0) {
		sprintf(file_name, "COMMAND.COM");
		if(_access(tmp, 0) == 0) {
			return(tmp);
		}
	}
	if(GetModuleFileName(NULL, path, MAX_PATH) != 0 && GetFullPathName(path, MAX_PATH, tmp, &file_name) != 0) {
		sprintf(file_name, "COMMAND.COM");
		if(_access(tmp, 0) == 0) {
			return(tmp);
		}
	}
	if(GetFullPathName("COMMAND.COM", MAX_PATH, tmp, &file_name) != 0) {
		if(_access(tmp, 0) == 0) {
			return(tmp);
		}
	}
	char *token = my_strtok(env_path, ";");
	while(token != NULL) {
		if(strlen(token) != 0) {
			strcpy(tmp, msdos_combine_path(token, "COMMAND.COM"));
			if(_access(tmp, 0) == 0) {
				return(tmp);
			}
		}
		token = my_strtok(NULL, ";");
	}
	return(NULL);
}

int msdos_drive_number(char *path)
{
	char tmp[MAX_PATH], *name;
	
	GetFullPathName(path, MAX_PATH, tmp, &name);
	if(tmp[0] >= 'a' && tmp[0] <= 'z') {
		return(tmp[0] - 'a');
	} else {
		return(tmp[0] - 'A');
	}
}

char *msdos_volume_label(char *path)
{
	static char tmp[MAX_PATH];
	char volume[] = "A:\\";
	
	if(path[1] == ':') {
		volume[0] = path[0];
	} else {
		volume[0] = 'A' + _getdrive() - 1;
	}
	if(!GetVolumeInformation(volume, tmp, MAX_PATH, NULL, NULL, NULL, NULL, 0)) {
		memset(tmp, 0, sizeof(tmp));
	}
	return(tmp);
}

char *msdos_short_volume_label(char *label)
{
	static char tmp[(8 + 1 + 3) + 1];
	char *src = label;
	int remain = strlen(label);
	char *dst_n = tmp;
	char *dst_e = tmp + 9;
	
	strcpy(tmp, "        .   ");
	for(int i = 0; i < 8 && remain > 0; i++) {
		if(msdos_lead_byte_check(*src)) {
			if(++i == 8) {
				break;
			}
			*dst_n++ = *src++;
			remain--;
		}
		*dst_n++ = *src++;
		remain--;
	}
	if(remain > 0) {
		for(int i = 0; i < 3 && remain > 0; i++) {
			if(msdos_lead_byte_check(*src)) {
				if(++i == 3) {
					break;
				}
				*dst_e++ = *src++;
				remain--;
			}
			*dst_e++ = *src++;
			remain--;
		}
		*dst_e = '\0';
	} else {
		*dst_n = '\0';
	}
	my_strupr(tmp);
	return(tmp);
}

errno_t msdos_maperr(unsigned long oserrno)
{
	_doserrno = oserrno;
	switch (oserrno) {
	case ERROR_FILE_NOT_FOUND:         // 2
	case ERROR_PATH_NOT_FOUND:         // 3
	case ERROR_INVALID_DRIVE:          // 15
	case ERROR_NO_MORE_FILES:          // 18
	case ERROR_BAD_NETPATH:            // 53
	case ERROR_BAD_NET_NAME:           // 67
	case ERROR_BAD_PATHNAME:           // 161
	case ERROR_FILENAME_EXCED_RANGE:   // 206
		return ENOENT;
	case ERROR_TOO_MANY_OPEN_FILES:    // 4
		return EMFILE;
	case ERROR_ACCESS_DENIED:          // 5
	case ERROR_CURRENT_DIRECTORY:      // 16
	case ERROR_NETWORK_ACCESS_DENIED:  // 65
	case ERROR_CANNOT_MAKE:            // 82
	case ERROR_FAIL_I24:               // 83
	case ERROR_DRIVE_LOCKED:           // 108
	case ERROR_SEEK_ON_DEVICE:         // 132
	case ERROR_NOT_LOCKED:             // 158
	case ERROR_LOCK_FAILED:            // 167
		return EACCES;
	case ERROR_INVALID_HANDLE:         // 6
	case ERROR_INVALID_TARGET_HANDLE:  // 114
	case ERROR_DIRECT_ACCESS_HANDLE:   // 130
		return EBADF;
	case ERROR_ARENA_TRASHED:          // 7
	case ERROR_NOT_ENOUGH_MEMORY:      // 8
	case ERROR_INVALID_BLOCK:          // 9
	case ERROR_NOT_ENOUGH_QUOTA:       // 1816
		return ENOMEM;
	case ERROR_BAD_ENVIRONMENT:        // 10
		return E2BIG;
	case ERROR_BAD_FORMAT:             // 11
		return ENOEXEC;
	case ERROR_NOT_SAME_DEVICE:        // 17
		return EXDEV;
	case ERROR_FILE_EXISTS:            // 80
	case ERROR_ALREADY_EXISTS:         // 183
		return EEXIST;
	case ERROR_NO_PROC_SLOTS:          // 89
	case ERROR_MAX_THRDS_REACHED:      // 164
	case ERROR_NESTING_NOT_ALLOWED:    // 215
		return EAGAIN;
	case ERROR_BROKEN_PIPE:            // 109
		return EPIPE;
	case ERROR_DISK_FULL:              // 112
		return ENOSPC;
	case ERROR_WAIT_NO_CHILDREN:       // 128
	case ERROR_CHILD_NOT_COMPLETE:     // 129
		return ECHILD;
	case ERROR_DIR_NOT_EMPTY:          // 145
		return ENOTEMPTY;
	}
	if (oserrno >= ERROR_WRITE_PROTECT /* 19 */ &&
		oserrno <= ERROR_SHARING_BUFFER_EXCEEDED /* 36 */) {
		return EACCES;
	}
	if (oserrno >= ERROR_INVALID_STARTING_CODESEG /* 188 */ &&
		oserrno <= ERROR_INFLOOP_IN_RELOC_CHAIN /* 202 */) {
		return ENOEXEC;
	}
	return EINVAL;
}

int msdos_open(const char *filename, int oflag)
{
	if ((oflag & (_O_RDONLY | _O_WRONLY | _O_RDWR)) != _O_RDONLY) {
		return _open(filename, oflag);
	}

	SECURITY_ATTRIBUTES sa = { sizeof SECURITY_ATTRIBUTES, NULL, !(oflag & _O_NOINHERIT) };
	DWORD disposition;
	switch (oflag & (_O_CREAT | _O_EXCL | _O_TRUNC)) {
	case 0:
	case _O_EXCL:
		disposition = OPEN_EXISTING;
		break;

	case _O_CREAT:
		disposition = OPEN_ALWAYS;
		break;

	case _O_CREAT | _O_EXCL:
	case _O_CREAT | _O_TRUNC | _O_EXCL:
		disposition = CREATE_NEW;
		break;

	case _O_TRUNC:
	case _O_TRUNC | _O_EXCL:
		disposition = TRUNCATE_EXISTING;
		break;

	case _O_CREAT | _O_TRUNC:
		disposition = CREATE_ALWAYS;
		break;
	}

	HANDLE h = CreateFile(filename, GENERIC_READ | FILE_WRITE_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, disposition,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) {
		// FILE_WRITE_ATTRIBUTES may not be granted for standard users.
		// Retry without FILE_WRITE_ATTRIBUTES.
		h = CreateFile(filename, GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, disposition,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (h == INVALID_HANDLE_VALUE) {
			errno = msdos_maperr(GetLastError());
			return -1;
		}
	}

	int fd = _open_osfhandle((intptr_t) h, oflag);
	if (fd == -1) {
		CloseHandle(h);
	}

	return fd;
}

void msdos_file_handler_open(int fd, char *path, int atty, int mode, UINT16 info, UINT16 psp_seg)
{
	static int id = 0;
	char full[MAX_PATH], *name;
	
	if(psp_seg && fd < 20) {
		psp_t *psp = (psp_t *)(mem + (psp_seg << 4));
		psp->file_table[fd] = fd;
	}
	if(GetFullPathName(path, MAX_PATH, full, &name) != 0) {
		strcpy(file_handler[fd].path, full);
	} else {
		strcpy(file_handler[fd].path, path);
	}
	file_handler[fd].valid = 1;
	file_handler[fd].id = id++;	// dummy id for int 21h ax=71a6h
	file_handler[fd].atty = atty;
	file_handler[fd].mode = mode;
	file_handler[fd].info = info;
	file_handler[fd].psp = psp_seg;
}

void msdos_file_handler_dup(int dst, int src, UINT16 psp_seg)
{
	if(psp_seg && dst < 20) {
		psp_t *psp = (psp_t *)(mem + (psp_seg << 4));
		psp->file_table[dst] = dst;
	}
	strcpy(file_handler[dst].path, file_handler[src].path);
	file_handler[dst].valid = 1;
	file_handler[dst].id = file_handler[src].id;
	file_handler[dst].atty = file_handler[src].atty;
	file_handler[dst].mode = file_handler[src].mode;
	file_handler[dst].info = file_handler[src].info;
	file_handler[dst].psp = psp_seg;
}

void msdos_file_handler_close(int fd, UINT16 psp_seg)
{
	if(psp_seg && fd < 20) {
		psp_t *psp = (psp_t *)(mem + (psp_seg << 4));
		psp->file_table[fd] = 0xff;
	}
	file_handler[fd].valid = 0;
}

int msdos_file_attribute_create(UINT16 new_attr)
{
	int attr = 0;
	
	if(REG16(CX) & 0x01) {
		attr |= FILE_ATTRIBUTE_READONLY;
	}
	if(REG16(CX) & 0x02) {
		attr |= FILE_ATTRIBUTE_HIDDEN;
	}
	if(REG16(CX) & 0x04) {
		attr |= FILE_ATTRIBUTE_SYSTEM;
	}
	if(REG16(CX) & 0x20) {
		attr |= FILE_ATTRIBUTE_ARCHIVE;
	}
	return(attr);
}

// find file

int msdos_find_file_check_attribute(int attribute, int allowed_mask, int required_mask)
{
	if((allowed_mask & 0x08) && !(attribute & FILE_ATTRIBUTE_DIRECTORY)) {
		return(0);	// search directory only !!!
	} else if(!(allowed_mask & 0x02) && (attribute & FILE_ATTRIBUTE_HIDDEN)) {
		return(0);
	} else if(!(allowed_mask & 0x04) && (attribute & FILE_ATTRIBUTE_SYSTEM)) {
		return(0);
	} else if(!(allowed_mask & 0x10) && (attribute & FILE_ATTRIBUTE_DIRECTORY)) {
		return(0);
	} else if((attribute & required_mask) != required_mask) {
		return(0);
	} else {
		return(1);
	}
}

int msdos_find_file_has_8dot3name(WIN32_FIND_DATA *fd)
{
	if (fd->cAlternateFileName[0]) {
		return 1;
	}
	size_t len = strlen(fd->cFileName);
	if (len > 12) {
		return 0;
	}
	const char *ext = strrchr(fd->cFileName, '.');
	if ((ext ? ext - fd->cFileName : len) > 8) {
		return 0;
	}
	return 1;
}

void msdos_find_file_conv_local_time(WIN32_FIND_DATA *fd)
{
	FILETIME local;
	
	FileTimeToLocalFileTime(&fd->ftCreationTime, &local);
	fd->ftCreationTime.dwLowDateTime = local.dwLowDateTime;
	fd->ftCreationTime.dwHighDateTime = local.dwHighDateTime;
	
	FileTimeToLocalFileTime(&fd->ftLastAccessTime, &local);
	fd->ftLastAccessTime.dwLowDateTime = local.dwLowDateTime;
	fd->ftLastAccessTime.dwHighDateTime = local.dwHighDateTime;
	
	FileTimeToLocalFileTime(&fd->ftLastWriteTime, &local);
	fd->ftLastWriteTime.dwLowDateTime = local.dwLowDateTime;
	fd->ftLastWriteTime.dwHighDateTime = local.dwHighDateTime;
}

// i/o

void msdos_putch(UINT8 data);

void msdos_stdio_reopen()
{
	if(!file_handler[0].valid) {
		_dup2(DUP_STDIN, 0);
		msdos_file_handler_open(0, "STDIN", _isatty(0), 0, 0x80d3, 0);
	}
	if(!file_handler[1].valid) {
		_dup2(DUP_STDOUT, 1);
		msdos_file_handler_open(1, "STDOUT", _isatty(1), 1, 0x80d3, 0);
	}
	if(!file_handler[2].valid) {
		_dup2(DUP_STDERR, 2);
		msdos_file_handler_open(2, "STDERR", _isatty(2), 1, 0x80d3, 0);
	}
}

int msdos_kbhit()
{
	msdos_stdio_reopen();
	
	if(!file_handler[0].atty) {
		// stdin is redirected to file
		return(eof(0) == 0);
	}
	
	// check keyboard status
	if(key_buf_char->count() != 0 || key_code != 0) {
		return(1);
	} else {
		return(_kbhit());
	}
}

int msdos_getch_ex(int echo)
{
	static char prev = 0;
	
	msdos_stdio_reopen();
	
	if(!file_handler[0].atty) {
		// stdin is redirected to file
retry:
		char data;
		if(_read(0, &data, 1) == 1) {
			char tmp = data;
			if(data == 0x0a) {
				if(prev == 0x0d) {
					goto retry; // CRLF -> skip LF
				} else {
					data = 0x0d; // LF only -> CR
				}
			}
			prev = tmp;
			return(data);
		}
		return(EOF);
	}
	
	// input from console
	int key_char, key_scan;
	if(key_code != 0) {
		key_char = (key_code >> 0) & 0xff;
		key_scan = (key_code >> 8) & 0xff;
		key_code >>= 16;
	} else {
		while(key_buf_char->count() == 0) {
			update_key_buffer();
		}
		key_char = key_buf_char->read();
		key_scan = key_buf_scan->read();
	}
	if(echo && key_char) {
		msdos_putch(key_char);
	}
	return key_char ? key_char : (key_scan != 0xe0) ? key_scan : 0;
}

inline int msdos_getch()
{
	return(msdos_getch_ex(0));
}

inline int msdos_getche()
{
	return(msdos_getch_ex(1));
}

int msdos_write(int fd, const void *buffer, unsigned int count)
{
	static int is_cr = 0;
	
	if(fd == 1 && !file_handler[1].atty) {
		// CR+LF -> LF
		UINT8 *buf = (UINT8 *)buffer;
		for(unsigned int i = 0; i < count; i++) {
			UINT8 data = buf[i];
			if(is_cr) {
				if(data != 0x0a) {
					UINT8 tmp = 0x0d;
					_write(1, &tmp, 1);
				}
				_write(1, &data, 1);
				is_cr = 0;
			} else if(data == 0x0d) {
				is_cr = 1;
			} else {
				_write(1, &data, 1);
			}
		}
		return(count);
	}
	return(_write(fd, buffer, count));
}

void msdos_putch(UINT8 data)
{
	static int p = 0;
	static int is_kanji = 0;
	static int is_esc = 0;
	static int stored_x;
	static int stored_y;
	static WORD stored_a;
	static char tmp[64];
	
	msdos_stdio_reopen();
	
	if(!file_handler[1].atty) {
		// stdout is redirected to file
		msdos_write(1, &data, 1);
		return;
	}
	
	// output to console
	tmp[p++] = data;
	
	if(is_kanji) {
		// kanji character
		is_kanji = 0;
	} else if(is_esc) {
		// escape sequense
		if((tmp[1] == ')' || tmp[1] == '(') && p == 3) {
			p = is_esc = 0;
		} else if(tmp[1] == '=' && p == 4) {
			COORD co;
			co.X = tmp[3] - 0x20;
			co.Y = tmp[2] - 0x20;
			SetConsoleCursorPosition(hStdout, co);
			mem[0x450 + mem[0x462] * 2] = co.X;
			mem[0x451 + mem[0x462] * 2] = co.Y;
			cursor_moved = false;
			p = is_esc = 0;
		} else if((data >= 'a' && data <= 'z') || (data >= 'A' && data <= 'Z') || data == '*') {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			COORD co;
			GetConsoleScreenBufferInfo(hStdout, &csbi);
			co.X = csbi.dwCursorPosition.X;
			co.Y = csbi.dwCursorPosition.Y;
			WORD wAttributes = csbi.wAttributes;
			
			if(tmp[1] == 'D') {
				co.Y++;
			} else if(tmp[1] == 'E') {
				co.X = 0;
				co.Y++;
			} else if(tmp[1] == 'M') {
				co.Y--;
			} else if(tmp[1] == '*') {
				SMALL_RECT rect;
				SET_RECT(rect, 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
				WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
				co.X = co.Y = 0;
			} else if(tmp[1] == '[') {
				int param[256], params = 0;
				memset(param, 0, sizeof(param));
				for(int i = 2; i < p; i++) {
					if(tmp[i] >= '0' && tmp[i] <= '9') {
						param[params] *= 10;
						param[params] += tmp[i] - '0';
					} else {
						params++;
					}
				}
				if(data == 'A') {
					co.Y -= param[0];
				} else if(data == 'B') {
					co.Y += param[0];
				} else if(data == 'C') {
					co.X += param[0];
				} else if(data == 'D') {
					co.X -= param[0];
				} else if(data == 'H' || data == 'f') {
					co.X = param[1] - 1;
					co.Y = param[0] - 1;
				} else if(data == 'J') {
					SMALL_RECT rect;
					if(param[0] == 0) {
						SET_RECT(rect, co.X, co.Y, csbi.dwSize.X - 1, co.Y);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						if(co.Y < csbi.dwSize.Y - 1) {
							SET_RECT(rect, 0, co.Y + 1, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
							WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						}
					} else if(param[0] == 1) {
						if(co.Y > 0) {
							SET_RECT(rect, 0, 0, csbi.dwSize.X - 1, co.Y - 1);
							WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						}
						SET_RECT(rect, 0, co.Y, co.X, co.Y);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					} else if(param[0] == 2) {
						SET_RECT(rect, 0, 0, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						co.X = co.Y = 0;
					}
				} else if(data == 'K') {
					SMALL_RECT rect;
					if(param[0] == 0) {
						SET_RECT(rect, co.X, co.Y, csbi.dwSize.X - 1, co.Y);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					} else if(param[0] == 1) {
						SET_RECT(rect, 0, co.Y, co.X, co.Y);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					} else if(param[0] == 2) {
						SET_RECT(rect, 0, co.Y, csbi.dwSize.X - 1, co.Y);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					}
				} else if(data == 'L') {
					SMALL_RECT rect;
					SET_RECT(rect, 0, co.Y, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
					ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					SET_RECT(rect, 0, co.Y + param[0], csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
					WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					// clear buffer
					for(int y = 0; y < SCR_BUF_SIZE; y++) {
						for(int x = 0; x < 80; x++) {
							scr_buf[y][x].Char.AsciiChar = ' ';
							scr_buf[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
						}
					}
					SET_RECT(rect, 0, co.Y, csbi.dwSize.X - 1, co.Y + param[0] - 1);
					WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					co.X = 0;
				} else if(data == 'M') {
					SMALL_RECT rect;
					if(co.Y + param[0] > csbi.dwSize.Y - 1) {
						SET_RECT(rect, 0, co.Y, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
					} else {
						SET_RECT(rect, 0, co.Y + param[0], csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
						ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						SET_RECT(rect, 0, co.Y, csbi.dwSize.X - 1, csbi.dwSize.Y - 1);
						WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
						// clear buffer
						for(int y = 0; y < SCR_BUF_SIZE; y++) {
							for(int x = 0; x < 80; x++) {
								scr_buf[y][x].Char.AsciiChar = ' ';
								scr_buf[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
							}
						}
					}
					co.X = 0;
				} else if(data == 'h') {
					if(tmp[2] == '>' && tmp[3] == '5') {
						CONSOLE_CURSOR_INFO cur;
						GetConsoleCursorInfo(hStdout, &cur);
						if(cur.bVisible) {
							cur.bVisible = FALSE;
							GetConsoleCursorInfo(hStdout, &cur);
						}
					}
				} else if(data == 'l') {
					if(tmp[2] == '>' && tmp[3] == '5') {
						CONSOLE_CURSOR_INFO cur;
						GetConsoleCursorInfo(hStdout, &cur);
						if(!cur.bVisible) {
							cur.bVisible = TRUE;
							GetConsoleCursorInfo(hStdout, &cur);
						}
					}
				} else if(data == 'm') {
					wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					int reverse = 0, hidden = 0;
					for(int i = 0; i < params; i++) {
						if(param[i] == 1) {
							wAttributes |= FOREGROUND_INTENSITY;
						} else if(param[i] == 4) {
							wAttributes |= COMMON_LVB_UNDERSCORE;
						} else if(param[i] == 7) {
							reverse = 1;
						} else if(param[i] == 8 || param[i] == 16) {
							hidden = 1;
						} else if((param[i] >= 17 && param[i] <= 23) || (param[i] >= 30 && param[i] <= 37)) {
							wAttributes &= ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							if(param[i] >= 17 && param[i] <= 23) {
								param[i] -= 16;
							} else {
								param[i] -= 30;
							}
							if(param[i] & 1) {
								wAttributes |= FOREGROUND_RED;
							}
							if(param[i] & 2) {
								wAttributes |= FOREGROUND_GREEN;
							}
							if(param[i] & 4) {
								wAttributes |= FOREGROUND_BLUE;
							}
						} else if(param[i] >= 40 && param[i] <= 47) {
							wAttributes &= ~(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
							if((param[i] - 40) & 1) {
								wAttributes |= BACKGROUND_RED;
							}
							if((param[i] - 40) & 2) {
								wAttributes |= BACKGROUND_GREEN;
							}
							if((param[i] - 40) & 4) {
								wAttributes |= BACKGROUND_BLUE;
							}
						}
					}
					if(reverse) {
						wAttributes &= ~0xff;
						wAttributes |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
					}
					if(hidden) {
						wAttributes &= ~0x0f;
						wAttributes |= (wAttributes >> 4) & 0x0f;
					}
				} else if(data == 'n') {
					if(param[0] == 6) {
						char tmp[16];
						sprintf(tmp, "\x1b[%d;%dR", co.Y + 1, co.X + 1);
						int len = strlen(tmp);
						for(int i = 0; i < len; i++) {
							key_buf_char->write(tmp[i]);
							key_buf_scan->write(0x00);
						}
					}
				} else if(data == 's') {
					stored_x = co.X;
					stored_y = co.Y;
					stored_a = wAttributes;
				} else if(data == 'u') {
					co.X = stored_x;
					co.Y = stored_y;
					wAttributes = stored_a;
				}
			}
			if(co.X < 0) {
				co.X = 0;
			} else if(co.X >= csbi.dwSize.X) {
				co.X = csbi.dwSize.X - 1;
			}
			if(co.Y < 0) {
				co.Y = 0;
			} else if(co.Y >= csbi.dwSize.Y) {
				co.Y = csbi.dwSize.Y - 1;
			}
			if(co.X != csbi.dwCursorPosition.X || co.Y != csbi.dwCursorPosition.Y) {
				SetConsoleCursorPosition(hStdout, co);
				mem[0x450 + mem[0x462] * 2] = co.X;
				mem[0x451 + mem[0x462] * 2] = co.Y;
				cursor_moved = false;
			}
			if(wAttributes != csbi.wAttributes) {
				SetConsoleTextAttribute(hStdout, wAttributes);
			}
			p = is_esc = 0;
		}
		return;
	} else {
		if(msdos_lead_byte_check(data)) {
			is_kanji = 1;
			return;
		} else if(data == 0x1b) {
			is_esc = 1;
			return;
		}
	}
	tmp[p++] = '\0';
	p = 0;
	printf("%s", tmp);
	cursor_moved = true;
}

int msdos_aux_in()
{
#ifdef SUPPORT_AUX_PRN
	if(file_handler[3].valid && !eof(3)) {
		char data = 0;
		_read(3, &data, 1);
		return(data);
	} else {
		return(EOF);
	}
#else
	return(0);
#endif
}

void msdos_aux_out(char data)
{
#ifdef SUPPORT_AUX_PRN
	if(file_handler[3].valid) {
		msdos_write(3, &data, 1);
	}
#endif
}

void msdos_prn_out(char data)
{
#ifdef SUPPORT_AUX_PRN
	if(file_handler[4].valid) {
		msdos_write(4, &data, 1);
	}
#endif
}

// memory control

mcb_t *msdos_mcb_create(int mcb_seg, UINT8 mz, UINT16 psp, UINT16 paragraphs)
{
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	
	mcb->mz = mz;
	mcb->psp = psp;
	mcb->paragraphs = paragraphs;
	return(mcb);
}

void msdos_mcb_check(mcb_t *mcb)
{
	if(!(mcb->mz == 'M' || mcb->mz == 'Z')) {
		fatalerror("broken mcb\n");
	}
}

int msdos_mem_split(int seg, int paragraphs)
{
	int mcb_seg = seg - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	msdos_mcb_check(mcb);
	
	if(mcb->paragraphs > paragraphs) {
		int new_seg = mcb_seg + 1 + paragraphs;
		int new_paragraphs = mcb->paragraphs - paragraphs - 1;
		
		msdos_mcb_create(new_seg, mcb->mz, 0, new_paragraphs);
		mcb->mz = 'M';
		mcb->paragraphs = paragraphs;
		return(0);
	}
	return(-1);
}

void msdos_mem_merge(int seg)
{
	int mcb_seg = seg - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	msdos_mcb_check(mcb);
	
	while(1) {
		if(mcb->mz == 'Z') {
			break;
		}
		int next_seg = mcb_seg + 1 + mcb->paragraphs;
		mcb_t *next_mcb = (mcb_t *)(mem + (next_seg << 4));
		msdos_mcb_check(next_mcb);
		
		if(next_mcb->psp != 0) {
			break;
		}
		mcb->mz = next_mcb->mz;
		mcb->paragraphs += 1 + next_mcb->paragraphs;
	}
}

int msdos_mem_alloc(int mcb_seg, int paragraphs, int new_process)
{
	while(1) {
		mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
		msdos_mcb_check(mcb);
		
		if(!(new_process && mcb->mz != 'Z')) {
			if(mcb->psp == 0 && mcb->paragraphs >= paragraphs) {
				msdos_mem_split(mcb_seg + 1, paragraphs);
				mcb->psp = current_psp;
				return(mcb_seg + 1);
			}
		}
		if(mcb->mz == 'Z') {
			break;
		}
		mcb_seg += 1 + mcb->paragraphs;
	}
	return(-1);
}

int msdos_mem_realloc(int seg, int paragraphs, int *max_paragraphs)
{
	int mcb_seg = seg - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	msdos_mcb_check(mcb);
	int current_paragraphs = mcb->paragraphs;
	
	msdos_mem_merge(seg);
	if(paragraphs > mcb->paragraphs) {
		*max_paragraphs = mcb->paragraphs;
		msdos_mem_split(seg, current_paragraphs);
		return(-1);
	}
	msdos_mem_split(seg, paragraphs);
	return(0);
}

void msdos_mem_free(int seg)
{
	int mcb_seg = seg - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	msdos_mcb_check(mcb);
	
	mcb->psp = 0;
	msdos_mem_merge(seg);
}

int msdos_mem_get_free(int mcb_seg, int new_process)
{
	int max_paragraphs = 0;
	
	while(1) {
		mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
		msdos_mcb_check(mcb);
		
		if(!(new_process && mcb->mz != 'Z')) {
			if(mcb->psp == 0 && mcb->paragraphs > max_paragraphs) {
				max_paragraphs = mcb->paragraphs;
			}
		}
		if(mcb->mz == 'Z') {
			break;
		}
		mcb_seg += 1 + mcb->paragraphs;
	}
	return(max_paragraphs);
}

int msdos_mem_get_last_mcb(int mcb_seg, UINT16 psp)
{
	int last_seg = -1;
	
	while(1) {
		mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
		msdos_mcb_check(mcb);
		
		if(mcb->mz == 'Z') {
			break;
		} else if(mcb->psp == psp) {
			last_seg = mcb_seg;
		}
		mcb_seg += 1 + mcb->paragraphs;
	}
	return(last_seg);
}

// environment

void msdos_env_set_argv(int env_seg, char *argv)
{
	char *dst = (char *)(mem + (env_seg << 4));
	
	while(1) {
		if(dst[0] == 0) {
			break;
		}
		dst += strlen(dst) + 1;
	}
	*dst++ = 0; // end of environment
	*dst++ = 1; // top of argv[0]
	*dst++ = 0;
	memcpy(dst, argv, strlen(argv));
	dst += strlen(argv);
	*dst++ = 0;
	*dst++ = 0;
}

char *msdos_env_get_argv(int env_seg)
{
	static char env[ENV_SIZE];
	char *src = env;
	
	memcpy(src, mem + (env_seg << 4), ENV_SIZE);
	while(1) {
		if(src[0] == 0) {
			if(src[1] == 1) {
				return(src + 3);
			}
			break;
		}
		src += strlen(src) + 1;
	}
	return(NULL);
}

char *msdos_env_get(int env_seg, const char *name)
{
	static char env[ENV_SIZE];
	char *src = env;
	
	memcpy(src, mem + (env_seg << 4), ENV_SIZE);
	while(1) {
		if(src[0] == 0) {
			break;
		}
		int len = strlen(src);
		char *n = my_strtok(src, "=");
		char *v = src + strlen(n) + 1;
		
		if(_stricmp(name, n) == 0) {
			return(v);
		}
		src += len + 1;
	}
	return(NULL);
}

void msdos_env_set(int env_seg, char *name, char *value)
{
	char env[ENV_SIZE];
	char *src = env;
	char *dst = (char *)(mem + (env_seg << 4));
	char *argv = msdos_env_get_argv(env_seg);
	int done = 0;
	
	memcpy(src, dst, ENV_SIZE);
	memset(dst, 0, ENV_SIZE);
	while(1) {
		if(src[0] == 0) {
			break;
		}
		int len = strlen(src);
		char *n = my_strtok(src, "=");
		char *v = src + strlen(n) + 1;
		char tmp[1024];
		
		if(_stricmp(name, n) == 0) {
			sprintf(tmp, "%s=%s", n, value);
			done = 1;
		} else {
			sprintf(tmp, "%s=%s", n, v);
		}
		memcpy(dst, tmp, strlen(tmp));
		dst += strlen(tmp) + 1;
		src += len + 1;
	}
	if(!done) {
		char tmp[1024];
		
		sprintf(tmp, "%s=%s", name, value);
		memcpy(dst, tmp, strlen(tmp));
		dst += strlen(tmp) + 1;
	}
	if(argv) {
		*dst++ = 0; // end of environment
		*dst++ = 1; // top of argv[0]
		*dst++ = 0;
		memcpy(dst, argv, strlen(argv));
		dst += strlen(argv);
		*dst++ = 0;
		*dst++ = 0;
	}
}

// process

psp_t *msdos_psp_create(int psp_seg, UINT16 mcb_seg, UINT16 parent_psp, UINT16 env_seg)
{
	psp_t *psp = (psp_t *)(mem + (psp_seg << 4));
	
	memset(psp, 0, PSP_SIZE);
	psp->exit[0] = 0xcd;
	psp->exit[1] = 0x20;
	psp->first_mcb = mcb_seg;
	psp->far_call = 0xea;
	psp->cpm_entry.w.l = 0xfff1;	// int 21h, retf
	psp->cpm_entry.w.h = 0xf000;
	psp->int_22h.dw = *(UINT32 *)(mem + 4 * 0x22);
	psp->int_23h.dw = *(UINT32 *)(mem + 4 * 0x23);
	psp->int_24h.dw = *(UINT32 *)(mem + 4 * 0x24);
	psp->parent_psp = parent_psp;
	for(int i = 0; i < 20; i++) {
		if(file_handler[i].valid) {
			psp->file_table[i] = i;
		} else {
			psp->file_table[i] = 0xff;
		}
	}
	psp->env_seg = env_seg;
	psp->stack.w.l = REG16(SP);
	psp->stack.w.h = SREG(SS);
	psp->service[0] = 0xcd;
	psp->service[1] = 0x21;
	psp->service[2] = 0xcb;
	return(psp);
}

int msdos_process_exec(char *cmd, param_block_t *param, UINT8 al)
{
	// load command file
	int fd = -1;
	int dos_command = 0;
	char command[MAX_PATH], path[MAX_PATH], opt[MAX_PATH], *name, name_tmp[MAX_PATH];
	
	strcpy(command, cmd);
	int opt_ofs = (param->cmd_line.w.h << 4) + param->cmd_line.w.l;
	int opt_len = mem[opt_ofs];
	memset(opt, 0, sizeof(opt));
	memcpy(opt, mem + opt_ofs + 1, opt_len);
	
	// check command.com
	GetFullPathName(command, MAX_PATH, path, &name);
	memset(name_tmp, 0, sizeof(name_tmp));
	strcpy(name_tmp, name);
	
	if(_stricmp(name, "COMMAND.COM") == 0 || _stricmp(name, "CMD.EXE") == 0) {
		for(int i = 0; i < opt_len; i++) {
			if(opt[i] == ' ') {
				continue;
			}
			if(opt[i] == '/' && (opt[i + 1] == 'c' || opt[i + 1] == 'C') && opt[i + 2] == ' ') {
				dos_command = 1;
				for(int j = i + 3; j < opt_len; j++) {
					if(opt[j] == ' ') {
						continue;
					}
					char *token = my_strtok(opt + j, " ");
					strcpy(command, token);
					char tmp[MAX_PATH];
					strcpy(tmp, token + strlen(token) + 1);
					strcpy(opt, tmp);
					opt_len = strlen(opt);
					mem[opt_ofs] = opt_len;
					sprintf((char *)(mem + opt_ofs + 1), "%s\x0d", opt);
					break;
				}
			}
			break;
		}
	}
	
	// load command file
	strcpy(path, command);
	if((fd = _open(path, _O_RDONLY | _O_BINARY)) == -1) {
		sprintf(path, "%s.COM", command);
		if((fd = _open(path, _O_RDONLY | _O_BINARY)) == -1) {
			sprintf(path, "%s.EXE", command);
			if((fd = _open(path, _O_RDONLY | _O_BINARY)) == -1) {
				// search path in parent environments
				psp_t *parent_psp = (psp_t *)(mem + (current_psp << 4));
				char *env = msdos_env_get(parent_psp->env_seg, "PATH");
				if(env != NULL) {
					char env_path[1024];
					strcpy(env_path, env);
					char *token = my_strtok(env_path, ";");
					
					while(token != NULL) {
						if(strlen(token) != 0) {
							sprintf(path, "%s", msdos_combine_path(token, command));
							if((fd = _open(path, _O_RDONLY | _O_BINARY)) != -1) {
								break;
							}
							sprintf(path, "%s.COM", msdos_combine_path(token, command));
							if((fd = _open(path, _O_RDONLY | _O_BINARY)) != -1) {
								break;
							}
							sprintf(path, "%s.EXE", msdos_combine_path(token, command));
							if((fd = _open(path, _O_RDONLY | _O_BINARY)) != -1) {
								break;
							}
						}
						token = my_strtok(NULL, ";");
					}
				}
			}
		}
	}
	if(fd == -1) {
		if(dos_command) {
			// may be dos command
			char tmp[MAX_PATH];
			sprintf(tmp, "%s %s", command, opt);
			system(tmp);
			return(0);
		} else {
			return(-1);
		}
	}
	_read(fd, file_buffer, sizeof(file_buffer));
	_close(fd);
	
	// copy environment
	int env_seg, psp_seg;
	
	if((env_seg = msdos_mem_alloc(first_mcb, ENV_SIZE >> 4, 1)) == -1) {
		return(-1);
	}
	if(param->env_seg == 0) {
		psp_t *parent_psp = (psp_t *)(mem + (current_psp << 4));
		memcpy(mem + (env_seg << 4), mem + (parent_psp->env_seg << 4), ENV_SIZE);
	} else {
		memcpy(mem + (env_seg << 4), mem + (param->env_seg << 4), ENV_SIZE);
	}
	msdos_env_set_argv(env_seg, msdos_short_full_path(path));
	
	// check exe header
	exe_header_t *header = (exe_header_t *)file_buffer;
	int paragraphs, free_paragraphs = msdos_mem_get_free(first_mcb, 1);
	UINT16 cs, ss, ip, sp;
	
	if(header->mz == 0x4d5a || header->mz == 0x5a4d) {
		// memory allocation
		int header_size = header->header_size * 16;
		int load_size = header->pages * 512 - header_size;
		if(header_size + load_size < 512) {
			load_size = 512 - header_size;
		}
		paragraphs = (PSP_SIZE + load_size) >> 4;
		if(paragraphs + header->min_alloc > free_paragraphs) {
			msdos_mem_free(env_seg);
			return(-1);
		}
		paragraphs += header->max_alloc ? header->max_alloc : header->min_alloc;
		if(paragraphs > free_paragraphs) {
			paragraphs = free_paragraphs;
		}
		if((psp_seg = msdos_mem_alloc(first_mcb, paragraphs, 1)) == -1) {
			msdos_mem_free(env_seg);
			return(-1);
		}
		// relocation
		int start_seg = psp_seg + (PSP_SIZE >> 4);
		for(int i = 0; i < header->relocations; i++) {
			int ofs = *(UINT16 *)(file_buffer + header->relocation_table + i * 4 + 0);
			int seg = *(UINT16 *)(file_buffer + header->relocation_table + i * 4 + 2);
			*(UINT16 *)(file_buffer + header_size + (seg << 4) + ofs) += start_seg;
		}
		memcpy(mem + (start_seg << 4), file_buffer + header_size, load_size);
		// segments
		cs = header->init_cs + start_seg;
		ss = header->init_ss + start_seg;
		ip = header->init_ip;
		sp = header->init_sp - 2; // for symdeb
	} else {
		// memory allocation
		paragraphs = free_paragraphs;
		if((psp_seg = msdos_mem_alloc(first_mcb, paragraphs, 1)) == -1) {
			msdos_mem_free(env_seg);
			return(-1);
		}
		int start_seg = psp_seg + (PSP_SIZE >> 4);
		memcpy(mem + (start_seg << 4), file_buffer, 0x10000 - PSP_SIZE);
		// segments
		cs = ss = psp_seg;
		ip = 0x100;
		sp = 0xfffe;
	}
	
	// create psp
#if defined(HAS_I386)
	*(UINT16 *)(mem + 4 * 0x22 + 0) = m_eip;
#else
	*(UINT16 *)(mem + 4 * 0x22 + 0) = m_pc - SREG_BASE(CS);
#endif
	*(UINT16 *)(mem + 4 * 0x22 + 2) = SREG(CS);
	psp_t *psp = msdos_psp_create(psp_seg, psp_seg + paragraphs, current_psp, env_seg);
	memcpy(psp->fcb1, mem + (param->fcb1.w.h << 4) + param->fcb1.w.l, sizeof(psp->fcb1));
	memcpy(psp->fcb2, mem + (param->fcb2.w.h << 4) + param->fcb2.w.l, sizeof(psp->fcb2));
	memcpy(psp->buffer, mem + (param->cmd_line.w.h << 4) + param->cmd_line.w.l, sizeof(psp->buffer));
	
	mcb_t *mcb_env = (mcb_t *)(mem + ((env_seg - 1) << 4));
	mcb_t *mcb_psp = (mcb_t *)(mem + ((psp_seg - 1) << 4));
	mcb_psp->psp = mcb_env->psp = psp_seg;
	
	for(int i = 0; i < 8; i++) {
		if(name_tmp[i] == '.') {
			mcb_psp->prog_name[i] = '\0';
			break;
		} else if(i < 7 && msdos_lead_byte_check(name_tmp[i])) {
			mcb_psp->prog_name[i] = name_tmp[i];
			i++;
			mcb_psp->prog_name[i] = name_tmp[i];
		} else if(name_tmp[i] >= 'a' && name_tmp[i] <= 'z') {
			mcb_psp->prog_name[i] = name_tmp[i] - 'a' + 'A';
		} else {
			mcb_psp->prog_name[i] = name_tmp[i];
		}
	}
	
	// process info
	process_t *process = msdos_process_info_create(psp_seg);
	strcpy(process->module_dir, msdos_short_full_dir(path));
	process->dta.w.l = 0x80;
	process->dta.w.h = psp_seg;
	process->switchar = '/';
	process->max_files = 20;
	process->parent_int_10h_feh_called = int_10h_feh_called;
	process->parent_int_10h_ffh_called = int_10h_ffh_called;
	
	current_psp = psp_seg;
	
	if(al == 0x00) {
		int_10h_feh_called = int_10h_ffh_called = false;
		
		// registers and segments
		REG16(AX) = REG16(BX) = 0x00;
		REG16(CX) = 0xff;
		REG16(DX) = psp_seg;
		REG16(SI) = ip;
		REG16(DI) = sp;
		REG16(SP) = sp;
		SREG(DS) = SREG(ES) = psp_seg;
		SREG(SS) = ss;
		i386_load_segment_descriptor(DS);
		i386_load_segment_descriptor(ES);
		i386_load_segment_descriptor(SS);
		
		*(UINT16 *)(mem + (ss << 4) + sp) = 0;
		i386_jmp_far(cs, ip);
	} else if(al == 0x01) {
		// copy ss:sp and cs:ip to param block
		param->sp = sp;
		param->ss = ss;
		param->ip = ip;
		param->cs = cs;
	}
	return(0);
}

void msdos_process_terminate(int psp_seg, int ret, int mem_free)
{
	psp_t *psp = (psp_t *)(mem + (psp_seg << 4));
	
	*(UINT32 *)(mem + 4 * 0x22) = psp->int_22h.dw;
	*(UINT32 *)(mem + 4 * 0x23) = psp->int_23h.dw;
	*(UINT32 *)(mem + 4 * 0x24) = psp->int_24h.dw;
	
	SREG(SS) = psp->stack.w.h;
	i386_load_segment_descriptor(SS);
	REG16(SP) = psp->stack.w.l;
	i386_jmp_far(psp->int_22h.w.h, psp->int_22h.w.l);
	
	process_t *process = msdos_process_info_get(psp_seg);
	int_10h_feh_called = process->parent_int_10h_feh_called;
	int_10h_ffh_called = process->parent_int_10h_ffh_called;
	
	if(mem_free) {
		int mcb_seg;
		while((mcb_seg = msdos_mem_get_last_mcb(first_mcb, psp_seg)) != -1) {
			msdos_mem_free(mcb_seg + 1);
		}
		while((mcb_seg = msdos_mem_get_last_mcb(UMB_TOP >> 4, psp_seg)) != -1) {
			msdos_mem_free(mcb_seg + 1);
		}
		
		for(int i = 0; i < MAX_FILES; i++) {
			if(file_handler[i].valid && file_handler[i].psp == psp_seg) {
				_close(i);
				msdos_file_handler_close(i, psp_seg);
			}
		}
		msdos_stdio_reopen();
		
		msdos_dta_info_free(psp_seg);
	}
	
	memset(process, 0, sizeof(process_t));
	
	current_psp = psp->parent_psp;
	retval = ret;
}

// drive

int msdos_drive_param_block_update(int drive_num, UINT16 *seg, UINT16 *ofs, int force_update)
{
	*seg = DPB_TOP >> 4;
	*ofs = sizeof(dpb_t) * drive_num;
	dpb_t *dpb = (dpb_t *)(mem + (*seg << 4) + *ofs);
	
	if(!force_update && dpb->free_clusters != 0) {
		return(dpb->bytes_per_sector ? 1 : 0);
	}
	memset(dpb, 0, sizeof(dpb_t));
	
	int res = 0;
	char dev[64];
	sprintf(dev, "\\\\.\\%c:", 'A' + drive_num);
	
	HANDLE hFile = CreateFile(dev, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE) {
		DISK_GEOMETRY geo;
		DWORD dwSize;
		if(DeviceIoControl(hFile, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geo, sizeof(geo), &dwSize, NULL)) {
			dpb->bytes_per_sector = (UINT16)geo.BytesPerSector;
			dpb->highest_sector_num = (UINT8)(geo.SectorsPerTrack - 1);
			dpb->highest_cluster_num = (UINT16)(geo.TracksPerCylinder * geo.Cylinders.QuadPart + 1);
			switch(geo.MediaType) {
			case F5_320_512:	// floppy, double-sided, 8 sectors per track (320K)
				dpb->media_type = 0xff;
				break;
			case F5_160_512:	// floppy, single-sided, 8 sectors per track (160K)
				dpb->media_type = 0xfe;
				break;
			case F5_360_512:	// floppy, double-sided, 9 sectors per track (360K)
				dpb->media_type = 0xfd;
				break;
			case F5_180_512:	// floppy, single-sided, 9 sectors per track (180K)
				dpb->media_type = 0xfc;
				break;
			case F5_1Pt2_512:	// floppy, double-sided, 15 sectors per track (1.2M)
			case F3_720_512:	// floppy, double-sided, 9 sectors per track (720K,3.5")
				dpb->media_type = 0xf9;
				break;
			case FixedMedia:	// hard disk
			case RemovableMedia:
				dpb->media_type = 0xf8;
				break;
			default:
				dpb->media_type = 0xf0;
				break;
			}
			res = 1;
		}
		dpb->drive_num = drive_num;
		dpb->unit_num = drive_num;
		dpb->next_dpb_ofs = *ofs + sizeof(dpb_t);
		dpb->next_dpb_seg = *seg;
		dpb->free_clusters = 0xffff;
		CloseHandle(hFile);
	}
	return(res);
}

// pc bios

int get_text_vram_address(int page)
{
	if(/*mem[0x449] == 0x03 || */mem[0x449] == 0x70 || mem[0x449] == 0x71 || mem[0x449] == 0x73) {
		return TEXT_VRAM_TOP;
	} else {
		return TEXT_VRAM_TOP + 0x1000 * (page & 7);
	}
}

int get_shadow_buffer_address(int page)
{
	if(/*mem[0x449] == 0x03 || */mem[0x449] == 0x70 || mem[0x449] == 0x71 || mem[0x449] == 0x73) {
		return SHADOW_BUF_TOP;
	} else {
		return SHADOW_BUF_TOP + 0x1000 * (page & 7);
	}
}

inline int get_shadow_buffer_address(int page, int x, int y)
{
	return get_shadow_buffer_address(page) + (x + y * 80) * 2;
}

inline void pcbios_int_10h_00h()
{
	mem[0x449] = REG8(AL) & 0x7f;
	text_vram_top_address = get_text_vram_address(mem[0x462]);
	text_vram_end_address = text_vram_top_address + 4000;
	shadow_buffer_top_address = get_shadow_buffer_address(mem[0x462]);
	shadow_buffer_end_address = shadow_buffer_top_address + 4000;
	
	if(REG8(AL) & 0x80) {
		mem[0x487] |= 0x80;
	} else {
		for(int y = 0, ofs = get_shadow_buffer_address(mem[0x462]); y < 25; y++) {
			for(int x = 0; x < 80; x++) {
				scr_buf[y][x].Char.AsciiChar = ' ';
				scr_buf[y][x].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				mem[ofs++] = 0x20;
				mem[ofs++] = 0x07;
			}
		}
		SMALL_RECT rect;
		SET_RECT(rect, 0, 0, 79, 24);
		WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		mem[0x487] &= ~0x80;
	}
}

inline void pcbios_int_10h_01h()
{
	mem[0x460] = REG8(CL);
	mem[0x461] = REG8(CH);
}

inline void pcbios_int_10h_02h()
{
	if(mem[0x462] == REG8(BH)) {
		COORD co;
		co.X = REG8(DL);
		co.Y = REG8(DH);
		SetConsoleCursorPosition(hStdout, co);
	}
	mem[0x450 + (REG8(BH) & 7) * 2] = REG8(DL);
	mem[0x451 + (REG8(BH) & 7) * 2] = REG8(DH);
}

inline void pcbios_int_10h_03h()
{
	REG8(DL) = mem[0x450 + (REG8(BH) & 7) * 2];
	REG8(DH) = mem[0x451 + (REG8(BH) & 7) * 2];
	REG8(CL) = mem[0x460];
	REG8(CH) = mem[0x461];
}

inline void pcbios_int_10h_05h()
{
	if(mem[0x462] != REG8(BH)) {
		SMALL_RECT rect;
		SET_RECT(rect, 0, 0, 79, 24);
		ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		
		for(int y = 0, ofs = get_shadow_buffer_address(mem[0x462]); y < 25; y++) {
			for(int x = 0; x < 80; x++) {
				mem[ofs++] = scr_buf[y][x].Char.AsciiChar;
				mem[ofs++] = scr_buf[y][x].Attributes;
			}
		}
		for(int y = 0, ofs = get_shadow_buffer_address(REG8(BH)); y < 25; y++) {
			for(int x = 0; x < 80; x++) {
				scr_buf[y][x].Char.AsciiChar = mem[ofs++];
				scr_buf[y][x].Attributes = mem[ofs++];
			}
		}
		WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		
		COORD co;
		co.X = mem[0x450 + (REG8(BH) & 7) * 2];
		co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
		SetConsoleCursorPosition(hStdout, co);
	}
	mem[0x462] = REG8(BH) & 7;
	mem[0x44e] = 0;
	mem[0x44f] = REG8(BH) << 4;
	text_vram_top_address = get_text_vram_address(mem[0x462]);
	text_vram_end_address = text_vram_top_address + 4000;
	shadow_buffer_top_address = get_shadow_buffer_address(mem[0x462]);
	shadow_buffer_end_address = shadow_buffer_top_address + 4000;
}

inline void pcbios_int_10h_06h()
{
	SMALL_RECT rect;
	SET_RECT(rect, 0, 0, 79, 24);
	ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
	
	if(REG8(AL) == 0) {
		for(int y = REG8(CH); y <= REG8(DH); y++) {
			for(int x = REG8(CL), ofs = get_shadow_buffer_address(mem[0x462], REG8(CL), y); x <= REG8(DL); x++) {
				scr_buf[y][x].Char.AsciiChar = ' ';
				scr_buf[y][x].Attributes = REG8(BH);
				mem[ofs++] = scr_buf[y][x].Char.AsciiChar;
				mem[ofs++] = scr_buf[y][x].Attributes;
			}
		}
	} else {
		ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		for(int y = REG8(CH), y2 = REG8(CH) + REG8(AL); y <= REG8(DH); y++, y2++) {
			for(int x = REG8(CL), ofs = get_shadow_buffer_address(mem[0x462], REG8(CL), y); x <= REG8(DL); x++) {
				if(y2 <= REG8(DH) && y2 >= 0 && y2 < SCR_BUF_SIZE) {
					scr_buf[y][x] = scr_buf[y2][x];
				} else {
					scr_buf[y][x].Char.AsciiChar = ' ';
					scr_buf[y][x].Attributes = REG8(BH);
				}
				mem[ofs++] = scr_buf[y][x].Char.AsciiChar;
				mem[ofs++] = scr_buf[y][x].Attributes;
			}
		}
	}
	WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
}

inline void pcbios_int_10h_07h()
{
	SMALL_RECT rect;
	SET_RECT(rect, 0, 0, 79, 24);
	ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
	
	if(REG8(AL) == 0) {
		for(int y = REG8(CH); y <= REG8(DH); y++) {
			for(int x = REG8(CL), ofs = get_shadow_buffer_address(mem[0x462], REG8(CL), y); x <= REG8(DL); x++) {
				scr_buf[y][x].Char.AsciiChar = ' ';
				scr_buf[y][x].Attributes = REG8(BH);
				mem[ofs++] = scr_buf[y][x].Char.AsciiChar;
				mem[ofs++] = scr_buf[y][x].Attributes;
			}
		}
	} else {
		ReadConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
		for(int y = REG8(DH), y2 = REG8(DH) - REG8(AL); y >= REG8(CH); y--, y2--) {
			for(int x = REG8(CL), ofs = get_shadow_buffer_address(mem[0x462], REG8(CL), y); x <= REG8(DL); x++) {
				if(y2 >= REG8(CH) && y2 >= 0 && y2 < SCR_BUF_SIZE) {
					scr_buf[y][x] = scr_buf[y2][x];
				} else {
					scr_buf[y][x].Char.AsciiChar = ' ';
					scr_buf[y][x].Attributes = REG8(BH);
				}
				mem[ofs++] = scr_buf[y][x].Char.AsciiChar;
				mem[ofs++] = scr_buf[y][x].Attributes;
			}
		}
	}
	WriteConsoleOutput(hStdout, &scr_buf[0][0], scr_buf_size, scr_buf_pos, &rect);
}

inline void pcbios_int_10h_08h()
{
	COORD co;
	DWORD num;
	
	co.X = mem[0x450 + (REG8(BH) & 7) * 2];
	co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
	
	if(mem[0x462] == REG8(BH)) {
		ReadConsoleOutputCharacter(hStdout, scr_char, 1, co, &num);
		ReadConsoleOutputAttribute(hStdout, scr_attr, 1, co, &num);
		REG8(AL) = scr_char[0];
		REG8(AH) = scr_attr[0];
	} else {
		REG16(AX) = *(UINT16 *)(mem + get_shadow_buffer_address(REG8(BH), co.X, co.Y));
	}
}

inline void pcbios_int_10h_09h()
{
	COORD co;
	DWORD num;
	
	co.X = mem[0x450 + (REG8(BH) & 7) * 2];
	co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
	
	if(mem[0x462] == REG8(BH)) {
		for(int i = 0; i < REG16(CX) && i < 80 * 25; i++) {
			scr_char[i] = REG8(AL);
			scr_attr[i] = REG8(BL);
		}
		WriteConsoleOutputCharacter(hStdout, scr_char, REG16(CX), co, &num);
		WriteConsoleOutputAttribute(hStdout, scr_attr, REG16(CX), co, &num);
	} else {
		for(int i = 0, dest = get_shadow_buffer_address(REG8(BH), co.X, co.Y); i < REG16(CX); i++) {
			mem[dest++] = REG8(AL);
			mem[dest++] = REG8(BL);
			if(++co.X == 80) {
				if(++co.Y == 25) {
					break;
				}
				co.X = 0;
			}
		}
	}
}

inline void pcbios_int_10h_0ah()
{
	COORD co;
	DWORD num;
	
	co.X = mem[0x450 + (REG8(BH) & 7) * 2];
	co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
	
	if(mem[0x462] == REG8(BH)) {
		for(int i = 0; i < REG16(CX) && i < 80 * 25; i++) {
			scr_char[i] = REG8(AL);
//			scr_attr[i] = REG8(BL);
		}
		WriteConsoleOutputCharacter(hStdout, scr_char, REG16(CX), co, &num);
//		WriteConsoleOutputAttribute(hStdout, scr_attr, REG16(CX), co, &num);
	} else {
		for(int i = 0, dest = get_shadow_buffer_address(REG8(BH), co.X, co.Y); i < REG16(CX); i++) {
			mem[dest++] = REG8(AL);
//			mem[dest++] = REG8(BL);
			dest++;
			if(++co.X == 80) {
				if(++co.Y == 25) {
					break;
				}
				co.X = 0;
			}
		}
	}
}

inline void pcbios_int_10h_0eh()
{
	msdos_putch(REG8(AL));
}

inline void pcbios_int_10h_0fh()
{
	REG8(AL) = mem[0x449];
	REG8(AH) = mem[0x44a];
	REG8(BH) = mem[0x462];
}

inline void pcbios_int_10h_13h()
{
	int ofs = SREG_BASE(ES) + REG16(BP);
	COORD co;
	DWORD num;
	
	co.X = REG8(DL);
	co.Y = REG8(DH);
	
	switch(REG8(AL)) {
	case 0x00:
	case 0x01:
		if(mem[0x462] == REG8(BH)) {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(hStdout, &csbi);
			SetConsoleCursorPosition(hStdout, co);
			
			if(csbi.wAttributes != REG8(BL)) {
				SetConsoleTextAttribute(hStdout, REG8(BL));
			}
			for(int i = 0; i < REG16(CX); i++) {
				msdos_putch(mem[ofs++]);
			}
			if(csbi.wAttributes != REG8(BL)) {
				SetConsoleTextAttribute(hStdout, csbi.wAttributes);
			}
			if(REG8(AL) == 0x00) {
				co.X = mem[0x450 + (REG8(BH) & 7) * 2];
				co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
				SetConsoleCursorPosition(hStdout, co);
			} else {
				cursor_moved = true;
			}
		} else {
			m_CF = 1;
		}
		break;
	case 0x02:
	case 0x03:
		if(mem[0x462] == REG8(BH)) {
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(hStdout, &csbi);
			SetConsoleCursorPosition(hStdout, co);
			
			WORD wAttributes = csbi.wAttributes;
			for(int i = 0; i < REG16(CX); i++, ofs += 2) {
				if(wAttributes != mem[ofs + 1]) {
					SetConsoleTextAttribute(hStdout, mem[ofs + 1]);
					wAttributes = mem[ofs + 1];
				}
				msdos_putch(mem[ofs]);
			}
			if(csbi.wAttributes != wAttributes) {
				SetConsoleTextAttribute(hStdout, csbi.wAttributes);
			}
			if(REG8(AL) == 0x02) {
				co.X = mem[0x450 + (REG8(BH) & 7) * 2];
				co.Y = mem[0x451 + (REG8(BH) & 7) * 2];
				SetConsoleCursorPosition(hStdout, co);
			} else {
				cursor_moved = true;
			}
		} else {
			m_CF = 1;
		}
		break;
	case 0x10:
	case 0x11:
		if(mem[0x462] == REG8(BH)) {
			ReadConsoleOutputCharacter(hStdout, scr_char, REG16(CX), co, &num);
			ReadConsoleOutputAttribute(hStdout, scr_attr, REG16(CX), co, &num);
			for(int i = 0; i < num; i++) {
				mem[ofs++] = scr_char[i];
				mem[ofs++] = scr_attr[i];
				if(REG8(AL) == 0x11) {
					mem[ofs++] = 0;
					mem[ofs++] = 0;
				}
			}
		} else {
			for(int i = 0, src = get_shadow_buffer_address(REG8(BH), co.X, co.Y); i < REG16(CX); i++) {
				mem[ofs++] = mem[src++];
				mem[ofs++] = mem[src++];
				if(REG8(AL) == 0x11) {
					mem[ofs++] = 0;
					mem[ofs++] = 0;
				}
				if(++co.X == 80) {
					if(++co.Y == 25) {
						break;
					}
					co.X = 0;
				}
			}
		}
		break;
	case 0x20:
	case 0x21:
		if(mem[0x462] == REG8(BH)) {
			for(int i = 0; i < REG16(CX) && i < 80 * 25; i++) {
				scr_char[i] = mem[ofs++];
				scr_attr[i] = mem[ofs++];
				if(REG8(AL) == 0x21) {
					ofs += 2;
				}
			}
			WriteConsoleOutputCharacter(hStdout, scr_char, REG16(CX), co, &num);
			WriteConsoleOutputAttribute(hStdout, scr_attr, REG16(CX), co, &num);
		} else {
			for(int i = 0, dest = get_shadow_buffer_address(REG8(BH), co.X, co.Y); i < REG16(CX); i++) {
				mem[dest++] = mem[ofs++];
				mem[dest++] = mem[ofs++];
				if(REG8(AL) == 0x21) {
					ofs += 2;
				}
				if(++co.X == 80) {
					if(++co.Y == 25) {
						break;
					}
					co.X = 0;
				}
			}
		}
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_10h_1dh()
{
	switch(REG8(AL)) {
	case 0x01:
		break;
	case 0x02:
		REG16(BX) = 0;
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_10h_82h()
{
	static UINT8 mode = 0;
	
	switch(REG8(AL)) {
	case 0:
		if(REG8(BL) != 0xff) {
			mode = REG8(BL);
		}
		REG8(AL) = mode;
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_10h_feh()
{
	if(mem[0x449] == 0x03 || mem[0x449] == 0x70 || mem[0x449] == 0x71 || mem[0x449] == 0x73) {
		SREG(ES) = (SHADOW_BUF_TOP >> 4);
		i386_load_segment_descriptor(ES);
		REG16(DI) = (SHADOW_BUF_TOP & 0x0f);
	}
	int_10h_feh_called = true;
}

inline void pcbios_int_10h_ffh()
{
	if(mem[0x449] == 0x03 || mem[0x449] == 0x70 || mem[0x449] == 0x71 || mem[0x449] == 0x73) {
		COORD co;
		DWORD num;
		
		co.X = (REG16(DI) >> 1) % 80;
		co.Y = (REG16(DI) >> 1) / 80;
		for(int i = 0, ofs = get_shadow_buffer_address(0, co.X, co.Y); i < REG16(CX) && i < 80 * 25; i++) {
			scr_char[i] = mem[ofs++];
			scr_attr[i] = mem[ofs++];
		}
		WriteConsoleOutputCharacter(hStdout, scr_char, REG16(CX), co, &num);
		WriteConsoleOutputAttribute(hStdout, scr_attr, REG16(CX), co, &num);
	}
	int_10h_ffh_called = true;
}

inline void pcbios_int_15h_23h()
{
	switch(REG8(AL)) {
	case 0:
		REG8(CL) = cmos_read(0x2d);
		REG8(CH) = cmos_read(0x2e);
		break;
	case 1:
		cmos_write(0x2d, REG8(CL));
		cmos_write(0x2e, REG8(CH));
		break;
	default:
		REG8(AH) = 0x86;
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_15h_24h()
{
	switch(REG8(AL)) {
	case 0:
		i386_set_a20_line(0);
		REG8(AH) = 0;
		break;
	case 1:
		i386_set_a20_line(1);
		REG8(AH) = 0;
		break;
	case 2:
		REG8(AH) = 0;
		REG8(AL) = (m_a20_mask >> 20) & 1;
		REG16(CX) = 0;
		break;
	case 3:
		REG16(AX) = 0;
		REG16(BX) = 0;
		break;
	}
}

inline void pcbios_int_15h_49h()
{
	REG8(AL) = 0;
	REG8(BL) = 0;	// DOS/V;
}

inline void pcbios_int_15h_86h()
{
	UINT32 usec = (REG16(CX) << 16) | REG16(DX);
	Sleep(usec / 1000);
}

inline void pcbios_int_15h_87h()
{
	// copy extended memory (from DOSBox)
	int len = REG16(CX) * 2;
	int ofs = SREG_BASE(ES) + REG16(SI);
	int src = (*(UINT32 *)(mem + ofs + 0x12) & 0xffffff); // + (mem[ofs + 0x16] << 24);
	int dst = (*(UINT32 *)(mem + ofs + 0x1a) & 0xffffff); // + (mem[ofs + 0x1e] << 24);
	memcpy(mem + dst, mem + src, len);
	REG16(AX) = 0x00;
}

inline void pcbios_int_15h_88h()
{
	REG16(AX) = ((min(MAX_MEM, 0x1000000) - 0x100000) >> 10);
}

inline void pcbios_int_15h_89h()
{
#if defined(HAS_I386) || defined(HAS_I286)
	// switch to protected mode (from DOSBox)
	write_io_byte(0x20, 0x10);
	write_io_byte(0x21, REG8(BH));
	write_io_byte(0x21, 0x00);
	write_io_byte(0xa0, 0x10);
	write_io_byte(0xa1, REG8(BL));
	write_io_byte(0xa1, 0x00);
	i386_set_a20_line(1);
	int ofs = SREG_BASE(ES) + REG16(SI);
	m_gdtr.limit = *(UINT16 *)(mem + ofs + 0x08);
	m_gdtr.base = *(UINT32 *)(mem + ofs + 0x08 + 0x02) & 0xffffff;
	m_idtr.limit = *(UINT16 *)(mem + ofs + 0x10);
	m_idtr.base = *(UINT32 *)(mem + ofs + 0x10 + 0x02) & 0xffffff;
#if defined(HAS_I386)
	m_cr[0] |= 1;
#else
	m_msw |= 1;
#endif
	SREG(DS) = 0x18;
	SREG(ES) = 0x20;
	SREG(SS) = 0x28;
	i386_load_segment_descriptor(DS);
	i386_load_segment_descriptor(ES);
	i386_load_segment_descriptor(SS);
	REG16(SP) += 6;
#if defined(HAS_I386)
	set_flags(0);	// ???
#else
	m_flags = 2;
	ExpandFlags(m_flags);
#endif
	REG16(AX) = 0x00;
	i386_jmp_far(0x30, REG16(CX));
#else
	REG8(AH) = 0x86;
	m_CF = 1;
#endif
}

#if defined(HAS_I386)
inline void pcbios_int_15h_c9h()
{
	REG8(AH) = 0x00;
	REG8(CH) = cpu_type;
	REG8(CL) = cpu_step;
}
#endif

inline void pcbios_int_15h_cah()
{
	switch(REG8(AL)) {
	case 0:
		if(REG8(BL) > 0x3f) {
			REG8(AH) = 0x03;
			m_CF = 1;
		} else if(REG8(BL) < 0x0e) {
			REG8(AH) = 0x04;
			m_CF = 1;
		} else {
			REG8(CL) = cmos_read(REG8(BL));
		}
		break;
	case 1:
		if(REG8(BL) > 0x3f) {
			REG8(AH) = 0x03;
			m_CF = 1;
		} else if(REG8(BL) < 0x0e) {
			REG8(AH) = 0x04;
			m_CF = 1;
		} else {
			cmos_write(REG8(BL), REG8(CL));
		}
		break;
	default:
		REG8(AH) = 0x86;
		m_CF = 1;
		break;
	}
}

UINT32 get_key_code(bool clear_buffer)
{
	UINT32 code = 0;
	
	if(key_buf_char->count() == 0) {
		update_key_buffer();
	}
	if(!clear_buffer) {
		key_buf_char->store_buffer();
		key_buf_scan->store_buffer();
	}
	if(key_buf_char->count() != 0) {
		code = key_buf_char->read() | (key_buf_scan->read() << 8);
	}
	if(key_buf_char->count() != 0) {
		code |= (key_buf_char->read() << 16) | (key_buf_scan->read() << 24);
	}
	if(!clear_buffer) {
		key_buf_char->restore_buffer();
		key_buf_scan->restore_buffer();
	}
	return code;
}

inline void pcbios_int_16h_00h()
{
	while(key_code == 0) {
		key_code = get_key_code(true);
	}
	if((key_code & 0xffff) == 0x0000 || (key_code & 0xffff) == 0xe000) {
		if(REG8(AH) == 0x10) {
			key_code = ((key_code >> 8) & 0xff) | ((key_code >> 16) & 0xff00);
		} else {
			key_code = ((key_code >> 16) & 0xff00);
		}
	}
	REG16(AX) = key_code & 0xffff;
	key_code >>= 16;
}

inline void pcbios_int_16h_01h()
{
	static UINT32 key_code_prev = 0;
	UINT32 key_code_tmp = key_code;
	
	if(key_code_tmp == 0) {
		key_code_tmp = get_key_code(false);
	}
	if(key_code_prev == key_code_tmp && (key_code_tmp & 0xffffff) == 0x00e000 && (key_code_tmp & 0xff000000) != 0) {
		key_code_prev = key_code_tmp = 0;
	} else {
		key_code_prev = key_code_tmp;
	}
	if(key_code_tmp != 0) {
		if((key_code_tmp & 0xffff) == 0x0000 || (key_code_tmp & 0xffff) == 0xe000) {
			if(REG8(AH) == 0x11) {
				key_code_tmp = ((key_code_tmp >> 8) & 0xff) | ((key_code_tmp >> 16) & 0xff00);
			} else {
				key_code_tmp = ((key_code_tmp >> 16) & 0xff00);
			}
		}
	}
	if(key_code_tmp != 0) {
		REG16(AX) = key_code_tmp & 0xffff;
	}
#if defined(HAS_I386)
	m_ZF = (key_code_tmp == 0);
#else
	m_ZeroVal = (key_code_tmp != 0);
#endif
}

inline void pcbios_int_16h_02h()
{
	REG8(AL)  = (GetAsyncKeyState(VK_INSERT ) & 0x0001) ? 0x80 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_CAPITAL) & 0x0001) ? 0x40 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_NUMLOCK) & 0x0001) ? 0x20 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_SCROLL ) & 0x0001) ? 0x10 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_MENU   ) & 0x8000) ? 0x08 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? 0x04 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_LSHIFT ) & 0x8000) ? 0x02 : 0;
	REG8(AL) |= (GetAsyncKeyState(VK_RSHIFT ) & 0x8000) ? 0x01 : 0;
}

inline void pcbios_int_16h_03h()
{
	static UINT16 status = 0;
	
	switch(REG8(AL)) {
	case 0x05:
		status = REG16(BX);
		break;
	case 0x06:
		REG16(BX) = status;
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_16h_05h()
{
	_ungetch(REG8(CL));
	REG8(AL) = 0x00;
}

inline void pcbios_int_16h_12h()
{
	pcbios_int_16h_02h();
	
	REG8(AH)  = 0;//(GetAsyncKeyState(VK_SYSREQ  ) & 0x8000) ? 0x80 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_CAPITAL ) & 0x8000) ? 0x40 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_NUMLOCK ) & 0x8000) ? 0x20 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_SCROLL  ) & 0x8000) ? 0x10 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_RMENU   ) & 0x8000) ? 0x08 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_RCONTROL) & 0x8000) ? 0x04 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_LMENU   ) & 0x8000) ? 0x02 : 0;
	REG8(AH) |= (GetAsyncKeyState(VK_LCONTROL) & 0x8000) ? 0x01 : 0;
}

inline void pcbios_int_16h_13h()
{
	static UINT16 status = 0;
	
	switch(REG8(AL)) {
	case 0x00:
		status = REG16(DX);
		break;
	case 0x01:
		REG16(DX) = status;
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_16h_14h()
{
	static UINT8 status = 0;
	
	switch(REG8(AL)) {
	case 0x00:
	case 0x01:
		status = REG8(AL);
		break;
	case 0x02:
		REG8(AL) = status;
		break;
	default:
		m_CF = 1;
		break;
	}
}

inline void pcbios_int_1ah_00h()
{
	static WORD prev_day = 0;
	SYSTEMTIME time;
	
	GetLocalTime(&time);
	unsigned __int64 msec = ((time.wHour * 60 + time.wMinute) * 60 + time.wSecond) * 1000 + time.wMilliseconds;
	unsigned __int64 tick = msec * 0x1800b0 / 24 / 60 / 60 / 1000;
	REG16(CX) = (tick >> 16) & 0xffff;
	REG16(DX) = (tick      ) & 0xffff;
	REG8(AL) = (prev_day != 0 && prev_day != time.wDay) ? 1 : 0;
	prev_day = time.wDay;
}

inline int to_bcd(int t)
{
	int u = (t % 100) / 10;
	return (u << 4) | (t % 10);
}

inline void pcbios_int_1ah_02h()
{
	SYSTEMTIME time;
	
	GetLocalTime(&time);
	REG8(CH) = to_bcd(time.wHour);
	REG8(CL) = to_bcd(time.wMinute);
	REG8(DH) = to_bcd(time.wSecond);
	REG8(DL) = 0x00;
}

inline void pcbios_int_1ah_04h()
{
	SYSTEMTIME time;
	
	GetLocalTime(&time);
	REG8(CH) = to_bcd(time.wYear / 100);
	REG8(CL) = to_bcd(time.wYear);
	REG8(DH) = to_bcd(time.wMonth);
	REG8(DL) = to_bcd(time.wDay);
}

inline void pcbios_int_1ah_0ah()
{
	SYSTEMTIME time;
	FILETIME file_time;
	WORD dos_date, dos_time;
	
	GetLocalTime(&time);
	SystemTimeToFileTime(&time, &file_time);
	FileTimeToDosDateTime(&file_time, &dos_date, &dos_time);
	REG16(CX) = dos_date;
}

// msdos system call

inline void msdos_int_21h_00h()
{
	msdos_process_terminate(SREG(CS), retval, 1);
}

inline void msdos_int_21h_01h()
{
	REG8(AL) = msdos_getche();
	// some seconds may be passed in console
	hardware_update();
}

inline void msdos_int_21h_02h()
{
	msdos_putch(REG8(DL));
}

inline void msdos_int_21h_03h()
{
	REG8(AL) = msdos_aux_in();
}

inline void msdos_int_21h_04h()
{
	msdos_aux_out(REG8(DL));
}

inline void msdos_int_21h_05h()
{
	msdos_prn_out(REG8(DL));
}

inline void msdos_int_21h_06h()
{
	if(REG8(DL) == 0xff) {
		if(msdos_kbhit()) {
			REG8(AL) = msdos_getch();
#if defined(HAS_I386)
			m_ZF = 0;
#else
			m_ZeroVal = 1;
#endif
		} else {
			REG8(AL) = 0;
#if defined(HAS_I386)
			m_ZF = 1;
#else
			m_ZeroVal = 0;
#endif
			Sleep(10);
		}
	} else {
		msdos_putch(REG8(DL));
	}
}

inline void msdos_int_21h_07h()
{
	REG8(AL) = msdos_getch();
	// some seconds may be passed in console
	hardware_update();
}

inline void msdos_int_21h_08h()
{
	REG8(AL) = msdos_getch();
	// some seconds may be passed in console
	hardware_update();
}

inline void msdos_int_21h_09h()
{
	char tmp[0x10000];
	memcpy(tmp, mem + SREG_BASE(DS) + REG16(DX), sizeof(tmp));
	tmp[sizeof(tmp) - 1] = '\0';
	int len = strlen(my_strtok(tmp, "$"));
	
	if(file_handler[1].valid && !file_handler[1].atty) {
		// stdout is redirected to file
		msdos_write(1, tmp, len);
	} else {
		for(int i = 0; i < len; i++) {
			msdos_putch(tmp[i]);
		}
	}
}

inline void msdos_int_21h_0ah()
{
	int ofs = SREG_BASE(DS) + REG16(DX);
	int max = mem[ofs] - 1;
	UINT8 *buf = mem + ofs + 2;
	int chr, p = 0;
	
	while((chr = msdos_getch()) != 0x0d) {
		if(chr == 0x00) {
			// skip 2nd byte
			msdos_getch();
		} else if(chr == 0x08) {
			// back space
			if(p > 0) {
				p--;
				msdos_putch(chr);
				msdos_putch(' ');
				msdos_putch(chr);
			}
		} else if(p < max) {
			buf[p++] = chr;
			msdos_putch(chr);
		}
	}
	buf[p] = 0x0d;
	mem[ofs + 1] = p;
	// some seconds may be passed in console
	hardware_update();
}

inline void msdos_int_21h_0bh()
{
	if(msdos_kbhit()) {
		REG8(AL) = 0xff;
	} else {
		REG8(AL) = 0x00;
		Sleep(10);
	}
}

inline void msdos_int_21h_0ch()
{
	// clear key buffer
	if(file_handler[0].valid && !file_handler[0].atty) {
		// stdin is redirected to file
	} else {
		while(msdos_kbhit()) {
			msdos_getch();
		}
	}
	
	switch(REG8(AL)) {
	case 0x01:
		msdos_int_21h_01h();
		break;
	case 0x06:
		msdos_int_21h_06h();
		break;
	case 0x07:
		msdos_int_21h_07h();
		break;
	case 0x08:
		msdos_int_21h_08h();
		break;
	case 0x0a:
		msdos_int_21h_0ah();
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_0dh()
{
}

inline void msdos_int_21h_0eh()
{
	if(REG8(DL) < 26) {
		_chdrive(REG8(DL) + 1);
		msdos_cds_update(REG8(DL));
	}
	REG8(AL) = 26; // zdrive
}

inline void msdos_int_21h_11h()
{
	ext_fcb_t *ext_fcb = (ext_fcb_t *)(mem + SREG_BASE(DS) + REG16(DX));
	fcb_t *fcb = (fcb_t *)(mem + SREG_BASE(DS) + REG16(DX) + (ext_fcb->flag == 0xff ? 7 : 0));
	
	process_t *process = msdos_process_info_get(current_psp);
	UINT32 dta_laddr = (process->dta.w.h << 4) + process->dta.w.l;
	ext_fcb_t *ext_find = (ext_fcb_t *)(mem + dta_laddr);
	find_fcb_t *find = (find_fcb_t *)(mem + dta_laddr + (ext_fcb->flag == 0xff ? 7 : 0));
	char *path = msdos_fcb_path(fcb);
	WIN32_FIND_DATA fd;
	
	dtainfo_t *dtainfo = msdos_dta_info_get(current_psp, dta_laddr);
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		FindClose(dtainfo->find_handle);
		dtainfo->find_handle = INVALID_HANDLE_VALUE;
	}
	strcpy(process->volume_label, msdos_volume_label(path));
	process->allowable_mask = (ext_fcb->flag == 0xff) ? ext_fcb->attribute : 0x20;
	
	if((process->allowable_mask & 8) && !msdos_match_volume_label(path, msdos_short_volume_label(process->volume_label))) {
		process->allowable_mask &= ~8;
	}
	if((dtainfo->find_handle = FindFirstFile(path, &fd)) != INVALID_HANDLE_VALUE) {
		while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, 0) ||
		      !msdos_find_file_has_8dot3name(&fd)) {
			if(!FindNextFile(dtainfo->find_handle, &fd)) {
				FindClose(dtainfo->find_handle);
				dtainfo->find_handle = INVALID_HANDLE_VALUE;
				break;
			}
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		if(ext_fcb->flag == 0xff) {
			ext_find->flag = 0xff;
			memset(ext_find->reserved, 0, 5);
			ext_find->attribute = (UINT8)(fd.dwFileAttributes & 0x3f);
		}
		find->drive = _getdrive();
		msdos_set_fcb_path((fcb_t *)find, msdos_short_name(&fd));
		find->attribute = (UINT8)(fd.dwFileAttributes & 0x3f);
		find->nt_res = 0;
		msdos_find_file_conv_local_time(&fd);
		find->create_time_ms = 0;
		FileTimeToDosDateTime(&fd.ftCreationTime, &find->creation_date, &find->creation_time);
		FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->last_access_date, &find->last_write_time);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->last_write_date, &find->last_write_time);
		find->cluster_hi = find->cluster_lo = 0;
		find->file_size = fd.nFileSizeLow;
		REG8(AL) = 0x00;
	} else if(process->allowable_mask & 8) {
		if(ext_fcb->flag == 0xff) {
			ext_find->flag = 0xff;
			memset(ext_find->reserved, 0, 5);
			ext_find->attribute = 8;
		}
		find->drive = _getdrive();
		msdos_set_fcb_path((fcb_t *)find, msdos_short_volume_label(process->volume_label));
		find->attribute = 8;
		find->nt_res = 0;
		msdos_find_file_conv_local_time(&fd);
		find->create_time_ms = 0;
		FileTimeToDosDateTime(&fd.ftCreationTime, &find->creation_date, &find->creation_time);
		FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->last_access_date, &find->last_write_time);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->last_write_date, &find->last_write_time);
		find->cluster_hi = find->cluster_lo = 0;
		find->file_size = 0;
		process->allowable_mask &= ~8;
		REG8(AL) = 0x00;
	} else {
		REG8(AL) = 0xff;
	}
}

inline void msdos_int_21h_12h()
{
	ext_fcb_t *ext_fcb = (ext_fcb_t *)(mem + SREG_BASE(DS) + REG16(DX));
	fcb_t *fcb = (fcb_t *)(mem + SREG_BASE(DS) + REG16(DX) + (ext_fcb->flag == 0xff ? 7 : 0));
	
	process_t *process = msdos_process_info_get(current_psp);
	UINT32 dta_laddr = (process->dta.w.h << 4) + process->dta.w.l;
	ext_fcb_t *ext_find = (ext_fcb_t *)(mem + dta_laddr);
	find_fcb_t *find = (find_fcb_t *)(mem + dta_laddr + (ext_fcb->flag == 0xff ? 7 : 0));
	WIN32_FIND_DATA fd;
	
	dtainfo_t *dtainfo = msdos_dta_info_get(current_psp, dta_laddr);
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		if(FindNextFile(dtainfo->find_handle, &fd)) {
			while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, 0) ||
			      !msdos_find_file_has_8dot3name(&fd)) {
				if(!FindNextFile(dtainfo->find_handle, &fd)) {
					FindClose(dtainfo->find_handle);
					dtainfo->find_handle = INVALID_HANDLE_VALUE;
					break;
				}
			}
		} else {
			FindClose(dtainfo->find_handle);
			dtainfo->find_handle = INVALID_HANDLE_VALUE;
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		if(ext_fcb->flag == 0xff) {
			ext_find->flag = 0xff;
			memset(ext_find->reserved, 0, 5);
			ext_find->attribute = (UINT8)(fd.dwFileAttributes & 0x3f);
		}
		find->drive = _getdrive();
		msdos_set_fcb_path((fcb_t *)find, msdos_short_name(&fd));
		find->attribute = (UINT8)(fd.dwFileAttributes & 0x3f);
		find->nt_res = 0;
		msdos_find_file_conv_local_time(&fd);
		find->create_time_ms = 0;
		FileTimeToDosDateTime(&fd.ftCreationTime, &find->creation_date, &find->creation_time);
		FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->last_access_date, &find->last_write_time);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->last_write_date, &find->last_write_time);
		find->cluster_hi = find->cluster_lo = 0;
		find->file_size = fd.nFileSizeLow;
		REG8(AL) = 0x00;
	} else if(process->allowable_mask & 8) {
		if(ext_fcb->flag == 0xff) {
			ext_find->flag = 0xff;
			memset(ext_find->reserved, 0, 5);
			ext_find->attribute = 8;
		}
		find->drive = _getdrive();
		msdos_set_fcb_path((fcb_t *)find, msdos_short_volume_label(process->volume_label));
		find->attribute = 8;
		find->nt_res = 0;
		msdos_find_file_conv_local_time(&fd);
		find->create_time_ms = 0;
		FileTimeToDosDateTime(&fd.ftCreationTime, &find->creation_date, &find->creation_time);
		FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->last_access_date, &find->last_write_time);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->last_write_date, &find->last_write_time);
		find->cluster_hi = find->cluster_lo = 0;
		find->file_size = 0;
		process->allowable_mask &= ~8;
		REG8(AL) = 0x00;
	} else {
		REG8(AL) = 0xff;
	}
}

inline void msdos_int_21h_13h()
{
	if(remove(msdos_fcb_path((fcb_t *)(mem + SREG_BASE(DS) + REG16(DX))))) {
		REG8(AL) = 0xff;
	} else {
		REG8(AL) = 0x00;
	}
}

inline void msdos_int_21h_18h()
{
	REG8(AL) = 0x00;
}

inline void msdos_int_21h_19h()
{
	REG8(AL) = _getdrive() - 1;
}

inline void msdos_int_21h_1ah()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	process->dta.w.l = REG16(DX);
	process->dta.w.h = SREG(DS);
}

inline void msdos_int_21h_1bh()
{
	int drive_num = _getdrive() - 1;
	UINT16 seg, ofs;
	
	if(msdos_drive_param_block_update(drive_num, &seg, &ofs, 1)) {
		dpb_t *dpb = (dpb_t *)(mem + (seg << 4) + ofs);
		REG8(AL) = dpb->highest_sector_num + 1;
		REG16(CX) = dpb->bytes_per_sector;
		REG16(DX) = dpb->highest_cluster_num - 1;
		*(UINT8 *)(mem + SREG_BASE(DS) + REG16(BX)) = dpb->media_type;
	} else {
		REG8(AL) = 0xff;
		m_CF = 1;
	}

}

inline void msdos_int_21h_1ch()
{
	int drive_num = REG8(DL) ? (REG8(DL) - 1) : (_getdrive() - 1);
	UINT16 seg, ofs;
	
	if(msdos_drive_param_block_update(drive_num, &seg, &ofs, 1)) {
		dpb_t *dpb = (dpb_t *)(mem + (seg << 4) + ofs);
		REG8(AL) = dpb->highest_sector_num + 1;
		REG16(CX) = dpb->bytes_per_sector;
		REG16(DX) = dpb->highest_cluster_num - 1;
		*(UINT8 *)(mem + SREG_BASE(DS) + REG16(BX)) = dpb->media_type;
	} else {
		REG8(AL) = 0xff;
		m_CF = 1;
	}

}

inline void msdos_int_21h_1dh()
{
	REG8(AL) = 0;
}

inline void msdos_int_21h_1eh()
{
	REG8(AL) = 0;
}

inline void msdos_int_21h_1fh()
{
	int drive_num = _getdrive() - 1;
	UINT16 seg, ofs;
	
	if(msdos_drive_param_block_update(drive_num, &seg, &ofs, 1)) {
		REG8(AL) = 0;
		SREG(DS) = seg;
		i386_load_segment_descriptor(DS);
		REG16(BX) = ofs;
	} else {
		REG8(AL) = 0xff;
		m_CF = 1;
	}
}

inline void msdos_int_21h_20h()
{
	REG8(AL) = 0;
}

inline void msdos_int_21h_25h()
{
	*(UINT16 *)(mem + 4 * REG8(AL) + 0) = REG16(DX);
	*(UINT16 *)(mem + 4 * REG8(AL) + 2) = SREG(DS);
}

inline void msdos_int_21h_26h()
{
	psp_t *psp = (psp_t *)(mem + (REG16(DX) << 4));
	
	memcpy(mem + (REG16(DX) << 4), mem + (current_psp << 4), sizeof(psp_t));
	psp->first_mcb = REG16(DX) + 16;
	psp->int_22h.dw = *(UINT32 *)(mem + 4 * 0x22);
	psp->int_23h.dw = *(UINT32 *)(mem + 4 * 0x23);
	psp->int_24h.dw = *(UINT32 *)(mem + 4 * 0x24);
	psp->parent_psp = 0;
}

inline void msdos_int_21h_29h()
{
	int ofs = SREG_BASE(DS) + REG16(SI);
	char name[MAX_PATH], ext[MAX_PATH];
	UINT8 drv = 0;
	char sep_chars[] = ":.;,=+";
	char end_chars[] = "\\<>|/\"[]";
	char spc_chars[] = " \t";
	
	if(REG8(AL) & 1) {
		ofs += strspn((char *)&mem[ofs], spc_chars);
		if(my_strchr(sep_chars, mem[ofs]) && mem[ofs] != '\0') {
			ofs++;
		}
	}
	ofs += strspn((char *)&mem[ofs], spc_chars);
	
	if(mem[ofs + 1] == ':') {
		UINT8 c = mem[ofs];
		if(c <= 0x20 || my_strchr(end_chars, c) || my_strchr(sep_chars, c)) {
			; // invalid drive letter
		} else {
			if(c >= 'a' && c <= 'z') {
				drv = c - 'a' + 1;
			} else {
				drv = c - 'A' + 1;
			}
			ofs += 2;
		}
	}
	memset(name, 0x20, sizeof(name));
	memset(ext, 0x20, sizeof(ext));
	for(int i = 0; i < MAX_PATH; i++) {
		UINT8 c = mem[ofs];
		if(c <= 0x20 || my_strchr(end_chars, c) || my_strchr(sep_chars, c)) {
			break;
		} else if(c >= 'a' && c <= 'z') {
			c -= 0x20;
		}
		ofs++;
		name[i] = c;
	}
	if(mem[ofs] == '.') {
		ofs++;
		for(int i = 0; i < MAX_PATH; i++) {
			UINT8 c = mem[ofs];
			if(c <= 0x20 || my_strchr(end_chars, c) || my_strchr(sep_chars, c)) {
				break;
			} else if(c >= 'a' && c <= 'z') {
				c -= 0x20;
			}
			ofs++;
			ext[i] = c;
		}
	}
	int si = ofs - SREG_BASE(DS);
	int ds = SREG(DS);
	while(si > 0xffff) {
		si -= 0x10;
		ds++;
	}
	REG16(SI) = si;
	SREG(DS) = ds;
	i386_load_segment_descriptor(DS);
	
	UINT8 *fcb = mem + SREG_BASE(ES) + REG16(DI);
	fcb[0] = drv;
	memcpy(fcb + 1, name, 8);
	int found_star = 0;
	for(int i = 1; i < 1 + 8; i++) {
		if(fcb[i] == '*') {
			found_star = 1;
		}
		if(found_star) {
			fcb[i] = '?';
		}
	}
	memcpy(fcb + 9, ext, 3);
	found_star = 0;
	for(int i = 9; i < 9 + 3; i++) {
		if(fcb[i] == '*') {
			found_star = 1;
		}
		if(found_star) {
			fcb[i] = '?';
		}
	}
	
	REG8(AL) = 0x00;
	if(drv == 0 || (drv > 0 && drv <= 26 && (GetLogicalDrives() & ( 1 << (drv - 1) )))) {
		if(memchr(fcb + 1, '?', 8 + 3)) {
			REG8(AL) = 0x01;
		}
	} else {
		REG8(AL) = 0xff;
	}
}

inline void msdos_int_21h_2ah()
{
	SYSTEMTIME sTime;
	
	GetLocalTime(&sTime);
	REG16(CX) = sTime.wYear;
	REG8(DH) = (UINT8)sTime.wMonth;
	REG8(DL) = (UINT8)sTime.wDay;
	REG8(AL) = (UINT8)sTime.wDayOfWeek;
}

inline void msdos_int_21h_2bh()
{
	REG8(AL) = 0x00;
}

inline void msdos_int_21h_2ch()
{
	SYSTEMTIME sTime;
	
	GetLocalTime(&sTime);
	REG8(CH) = (UINT8)sTime.wHour;
	REG8(CL) = (UINT8)sTime.wMinute;
	REG8(DH) = (UINT8)sTime.wSecond;
}

inline void msdos_int_21h_2dh()
{
	REG8(AL) = 0x00;
}

inline void msdos_int_21h_2eh()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	process->verify = REG8(AL);
}

inline void msdos_int_21h_2fh()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	REG16(BX) = process->dta.w.l;
	SREG(ES) = process->dta.w.h;
	i386_load_segment_descriptor(ES);
}

inline void msdos_int_21h_30h()
{
	// Version Flag / OEM
	if(REG8(AL) == 1) {
		REG8(BH) = 0x00;	// not in ROM
	} else {
		REG8(BH) = 0xff;	// OEM = Microsoft
	}
	REG8(AL) = major_version;	// 7
	REG8(AH) = minor_version;	// 10
}

inline void msdos_int_21h_31h()
{
	int mcb_seg = current_psp - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	
	mcb->paragraphs = REG16(DX);
	mcb_seg += mcb->paragraphs + 1;
	msdos_mcb_create(mcb_seg, 'Z', 0, (MEMORY_END >> 4) - mcb_seg - 1);
	
	msdos_process_terminate(current_psp, REG8(AL) | 0x300, 0);
}

inline void msdos_int_21h_32h()
{
	int drive_num = (REG8(DL) == 0) ? (_getdrive() - 1) : (REG8(DL) - 1);
	UINT16 seg, ofs;
	
	if(msdos_drive_param_block_update(drive_num, &seg, &ofs, 1)) {
		REG8(AL) = 0;
		SREG(DS) = seg;
		i386_load_segment_descriptor(DS);
		REG16(BX) = ofs;
	} else {
		REG8(AL) = 0xff;
		m_CF = 1;
	}
}

inline void msdos_int_21h_33h()
{
	static UINT8 state = 0x00;
	char path[MAX_PATH];
	
	switch(REG8(AL)) {
	case 0x00:
		REG8(DL) = state;
		break;
	case 0x01:
		state = REG8(DL);
		break;
	case 0x05:
		GetSystemDirectory(path, MAX_PATH);
		if(path[0] >= 'a' && path[0] <= 'z') {
			REG8(DL) = path[0] - 'a' + 1;
		} else {
			REG8(DL) = path[0] - 'A' + 1;
		}
		break;
	case 0x06:
		// MS-DOS version (7.10)
		REG8(BL) = 7;
		REG8(BH) = 10;
		REG8(DL) = 0;
		REG8(DH) = 0x10; // in HMA
		break;
	case 0x07:
		if(REG8(DL) == 0) {
			((dos_info_t *)(mem + DOS_INFO_TOP))->dos_flag &= ~0x20;
		} else if(REG8(DL) == 1) {
			((dos_info_t *)(mem + DOS_INFO_TOP))->dos_flag |= 0x20;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_35h()
{
	REG16(BX) = *(UINT16 *)(mem + 4 * REG8(AL) + 0);
	SREG(ES) = *(UINT16 *)(mem + 4 * REG8(AL) + 2);
	i386_load_segment_descriptor(ES);
}

inline void msdos_int_21h_36h()
{
	struct _diskfree_t df = {0};
	
	if(_getdiskfree(REG8(DL), &df) == 0) {
		REG16(AX) = (UINT16)df.sectors_per_cluster;
		REG16(CX) = (UINT16)df.bytes_per_sector;
		REG16(BX) = df.avail_clusters > 0xFFFF ? 0xFFFF : (UINT16)df.avail_clusters;
		REG16(DX) = df.total_clusters > 0xFFFF ? 0xFFFF : (UINT16)df.total_clusters;
	} else {
		REG16(AX) = 0xffff;
	}
}

inline void msdos_int_21h_37h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	switch(REG8(AL)) {
	case 0x00:
		REG8(AL) = 0x00;
		REG8(DL) = process->switchar;
		break;
	case 0x01:
		REG8(AL) = 0x00;
		process->switchar = REG8(DL);
		break;
	default:
		REG16(AX) = 1;
		break;
	}
}

inline void msdos_int_21h_39h(int lfn)
{
	if(_mkdir(msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn))) {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3ah(int lfn)
{
	if(_rmdir(msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn))) {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3bh(int lfn)
{
	if(_chdir(msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn))) {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3ch()
{
	char *path = msdos_local_file_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), 0);
	int attr = GetFileAttributes(path);
	int fd = -1;
	UINT16 info;
	
	if(msdos_is_con_path(path)) {
		fd = _open("CON", _O_WRONLY | _O_BINARY);
		info = 0x80d3;
	} else {
		fd = _open(path, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
		info = msdos_drive_number(path);
	}
	if(fd != -1) {
		if(attr == -1) {
			attr = msdos_file_attribute_create(REG16(CX)) & ~FILE_ATTRIBUTE_READONLY;
		}
		SetFileAttributes(path, attr);
		REG16(AX) = fd;
		msdos_file_handler_open(fd, path, _isatty(fd), 2, info, current_psp);
	} else {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3dh()
{
	char *path = msdos_local_file_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), 0);
	int mode = REG8(AL) & 0x03;
	int fd = -1;
	UINT16 info;
	
	if(mode < 0x03) {
		if(msdos_is_con_path(path)) {
			fd = msdos_open("CON", file_mode[mode].mode);
			info = 0x80d3;
		} else {
			fd = msdos_open(path, file_mode[mode].mode);
			info = msdos_drive_number(path);
		}
		if(fd != -1) {
			REG16(AX) = fd;
			msdos_file_handler_open(fd, path, _isatty(fd), mode, info, current_psp);
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x0c;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3eh()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		_close(REG16(BX));
		msdos_file_handler_close(REG16(BX), current_psp);
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_3fh()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(file_mode[file_handler[REG16(BX)].mode].in) {
			if(file_handler[REG16(BX)].atty) {
				// BX is stdin or is redirected to stdin
				UINT8 *buf = mem + SREG_BASE(DS) + REG16(DX);
				int max = REG16(CX);
				int p = 0;
				
				while(max > p) {
					int chr = msdos_getch();
					
					if(chr == 0x00) {
						// skip 2nd byte
						msdos_getch();
					} else if(chr == 0x0d) {
						// carriage return
						buf[p++] = 0x0d;
						if(max > p) {
							buf[p++] = 0x0a;
						}
						break;
					} else if(chr == 0x08) {
						// back space
						if(p > 0) {
							p--;
							msdos_putch(chr);
							msdos_putch(' ');
							msdos_putch(chr);
						}
					} else {
						buf[p++] = chr;
						msdos_putch(chr);
					}
				}
				REG16(AX) = p;
				// some seconds may be passed in console
				hardware_update();
			} else {
				REG16(AX) = _read(REG16(BX), mem + SREG_BASE(DS) + REG16(DX), REG16(CX));
			}
		} else {
			REG16(AX) = 0x05;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_40h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(file_mode[file_handler[REG16(BX)].mode].out) {
			if(REG16(CX)) {
				if(file_handler[REG16(BX)].atty) {
					// BX is stdout/stderr or is redirected to stdout
					for(int i = 0; i < REG16(CX); i++) {
						msdos_putch(mem[SREG_BASE(DS) + REG16(DX) + i]);
					}
					REG16(AX) = REG16(CX);
				} else {
					REG16(AX) = msdos_write(REG16(BX), mem + SREG_BASE(DS) + REG16(DX), REG16(CX));
				}
			} else {
				UINT32 pos = _tell(REG16(BX));
				_lseek(REG16(BX), 0, SEEK_END);
				UINT32 size = _tell(REG16(BX));
				REG16(AX) = 0;
				if(pos < size) {
					_lseek(REG16(BX), pos, SEEK_SET);
					SetEndOfFile((HANDLE)_get_osfhandle(REG16(BX)));
				} else {
					for(UINT32 i = size; i < pos; i++) {
						UINT8 tmp = 0;
						REG16(AX) += msdos_write(REG16(BX), &tmp, 1);
					}
					_lseek(REG16(BX), pos, SEEK_SET);
				}
			}
		} else {
			REG16(AX) = 0x05;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_41h(int lfn)
{
	if(remove(msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn))) {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_42h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(REG8(AL) < 0x03) {
			static int ptrname[] = { SEEK_SET, SEEK_CUR, SEEK_END };
			_lseek(REG16(BX), (REG16(CX) << 16) | REG16(DX), ptrname[REG8(AL)]);
			UINT32 pos = _tell(REG16(BX));
			REG16(AX) = pos & 0xffff;
			REG16(DX) = (pos >> 16);
		} else {
			REG16(AX) = 0x01;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_43h(int lfn)
{
	char *path = msdos_local_file_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn);
	int attr;
	
	switch(REG8(AL)) {
	case 0x00:
		if((attr = GetFileAttributes(path)) != -1) {
			REG16(CX) = 0;
			if(attr & FILE_ATTRIBUTE_READONLY) {
				REG16(CX) |= 0x01;
			}
			if(attr & FILE_ATTRIBUTE_HIDDEN) {
				REG16(CX) |= 0x02;
			}
			if(attr & FILE_ATTRIBUTE_SYSTEM) {
				REG16(CX) |= 0x04;
			}
			if(attr & FILE_ATTRIBUTE_ARCHIVE) {
				REG16(CX) |= 0x20;
			}
		} else {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	case 0x01:
		if(SetFileAttributes(path, msdos_file_attribute_create(REG16(CX))) != 0) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	case 0x02:
		break;
	case 0x03:
		REG16(CX) = 0x00;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_44h()
{
	switch(REG8(AL)) {
	case 0x00: // get ioctrl data
		REG16(DX) = file_handler[REG16(BX)].info;
		break;
	case 0x01: // set ioctrl data
		file_handler[REG16(BX)].info |= REG8(DL);
		break;
	case 0x02: // recv from character device
	case 0x03: // send to character device
		REG16(AX) = 0x06;
		m_CF = 1;
		break;
	case 0x04: // recv from block device
	case 0x05: // send to block device
		REG16(AX) = 0x05;
		m_CF = 1;
		break;
	case 0x06: // get read status
		{
			process_t *process = msdos_process_info_get(current_psp);
			
			if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
				if(file_mode[file_handler[REG16(BX)].mode].in) {
					if(file_handler[REG16(BX)].atty) {
						REG8(AL) = msdos_kbhit() ? 0xff : 0x00;
					} else {
						REG8(AL) = eof(REG16(BX)) ? 0x00 : 0xff;
					}
				} else {
					REG8(AL) = 0x00;
				}
			} else {
				REG16(AX) = 0x06;
				m_CF = 1;
			}
		}
		break;
	case 0x07: // get write status
		{
			process_t *process = msdos_process_info_get(current_psp);
			
			if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
				if(file_mode[file_handler[REG16(BX)].mode].out) {
					REG8(AL) = 0xff;
				} else {
					REG8(AL) = 0x00;
				}
			} else {
				REG16(AX) = 0x06;
				m_CF = 1;
			}
		}
		break;
	case 0x08: // check removable drive
		if(REG8(BL) < ('Z' - 'A' + 1)) {
			UINT32 val;
			if(REG8(BL) == 0) {
				val = GetDriveType(NULL);
			} else if(REG8(BL) < ('Z' - 'A' + 1)) {
				char tmp[8];
				sprintf(tmp, "%c:\\", 'A' + REG8(BL) - 1);
				val = GetDriveType(tmp);
			}
			if(val == DRIVE_NO_ROOT_DIR) {
				// no drive
				REG16(AX) = 0x0f;
				m_CF = 1;
			} else if(val == DRIVE_REMOVABLE || val == DRIVE_CDROM) {
				// removable drive
				REG16(AX) = 0x00;
			} else {
				// fixed drive
				REG16(AX) = 0x01;
			}
		} else {
			// invalid drive number
			REG16(AX) = 0x0f;
			m_CF = 1;
		}
		break;
	case 0x09: // check remote drive
		if(REG8(BL) < ('Z' - 'A' + 1)) {
			UINT32 val;
			if(REG8(BL) == 0) {
				val = GetDriveType(NULL);
			} else if(REG8(BL) < ('Z' - 'A' + 1)) {
				char tmp[8];
				sprintf(tmp, "%c:\\", 'A' + REG8(BL) - 1);
				val = GetDriveType(tmp);
			}
			if(val == DRIVE_NO_ROOT_DIR) {
				// no drive
				REG16(AX) = 0x0f;
				m_CF = 1;
			} else if(val == DRIVE_REMOTE) {
				// remote drive
				REG16(DX) = 0x1000;
			} else {
				// local drive
				REG16(DX) = 0x00;
			}
		} else {
			// invalid drive number
			REG16(AX) = 0x0f;
			m_CF = 1;
		}
		break;
	case 0x0b: // set retry count
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_45h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		int fd = _dup(REG16(BX));
		if(fd != -1) {
			REG16(AX) = fd;
			msdos_file_handler_dup(REG16(AX), REG16(BX), current_psp);
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_46h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && REG16(CX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(_dup2(REG16(BX), REG16(CX)) != -1) {
			msdos_file_handler_dup(REG16(CX), REG16(BX), current_psp);
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_47h(int lfn)
{
	char path[MAX_PATH];
	
	if(_getdcwd(REG8(DL), path, MAX_PATH) != NULL) {
		if(path[1] == ':') {
			// the returned path does not include a drive or the initial backslash
			strcpy((char *)(mem + SREG_BASE(DS) + REG16(SI)), (lfn ? path : msdos_short_path(path)) + 3);
		} else {
			strcpy((char *)(mem + SREG_BASE(DS) + REG16(SI)), lfn ? path : msdos_short_path(path));
		}
	} else {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_48h()
{
	int seg;
	
	if((malloc_strategy & 0xf0) == 0x00) {
		if((seg = msdos_mem_alloc(first_mcb, REG16(BX), 0)) != -1) {
			REG16(AX) = seg;
		} else {
			REG16(AX) = 0x08;
			REG16(BX) = msdos_mem_get_free(first_mcb, 0);
			m_CF = 1;
		}
	} else if((malloc_strategy & 0xf0) == 0x40) {
		if((seg = msdos_mem_alloc(UMB_TOP >> 4, REG16(BX), 0)) != -1) {
			REG16(AX) = seg;
		} else {
			REG16(AX) = 0x08;
			REG16(BX) = msdos_mem_get_free(UMB_TOP >> 4, 0);
			m_CF = 1;
		}
	} else if((malloc_strategy & 0xf0) == 0x80) {
		if((seg = msdos_mem_alloc(UMB_TOP >> 4, REG16(BX), 0)) != -1) {
			REG16(AX) = seg;
		} else if((seg = msdos_mem_alloc(first_mcb, REG16(BX), 0)) != -1) {
			REG16(AX) = seg;
		} else {
			REG16(AX) = 0x08;
			REG16(BX) = max(msdos_mem_get_free(UMB_TOP >> 4, 0), msdos_mem_get_free(first_mcb, 0));
			m_CF = 1;
		}
	}
}

inline void msdos_int_21h_49h()
{
	msdos_mem_free(SREG(ES));
}

inline void msdos_int_21h_4ah()
{
	int max_paragraphs;
	
	if(msdos_mem_realloc(SREG(ES), REG16(BX), &max_paragraphs)) {
		REG16(AX) = 0x08;
		REG16(BX) = max_paragraphs;
		m_CF = 1;
	}
}

inline void msdos_int_21h_4bh()
{
	char *command = (char *)(mem + SREG_BASE(DS) + REG16(DX));
	param_block_t *param = (param_block_t *)(mem + SREG_BASE(ES) + REG16(BX));
	
	switch(REG8(AL)) {
	case 0x00:
	case 0x01:
		if(msdos_process_exec(command, param, REG8(AL))) {
			REG16(AX) = 0x02;
			m_CF = 1;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_4ch()
{
	msdos_process_terminate(current_psp, REG8(AL), 1);
}

inline void msdos_int_21h_4dh()
{
	REG16(AX) = retval;
}

inline void msdos_int_21h_4eh()
{
	process_t *process = msdos_process_info_get(current_psp);
	UINT32 dta_laddr = (process->dta.w.h << 4) + process->dta.w.l;
	find_t *find = (find_t *)(mem + dta_laddr);
	char *path = msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), 0);
	WIN32_FIND_DATA fd;
	
	dtainfo_t *dtainfo = msdos_dta_info_get(current_psp, dta_laddr);
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		FindClose(dtainfo->find_handle);
		dtainfo->find_handle = INVALID_HANDLE_VALUE;
	}
	strcpy(process->volume_label, msdos_volume_label(path));
	process->allowable_mask = REG8(CL);
	
	if((process->allowable_mask & 8) && !msdos_match_volume_label(path, msdos_short_volume_label(process->volume_label))) {
		process->allowable_mask &= ~8;
	}
	if((dtainfo->find_handle = FindFirstFile(path, &fd)) != INVALID_HANDLE_VALUE) {
		while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, 0) ||
		      !msdos_find_file_has_8dot3name(&fd)) {
			if(!FindNextFile(dtainfo->find_handle, &fd)) {
				FindClose(dtainfo->find_handle);
				dtainfo->find_handle = INVALID_HANDLE_VALUE;
				break;
			}
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		find->attrib = (UINT8)(fd.dwFileAttributes & 0x3f);
		msdos_find_file_conv_local_time(&fd);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->date, &find->time);
		find->size = fd.nFileSizeLow;
		strcpy(find->name, msdos_short_name(&fd));
		REG16(AX) = 0;
	} else if(process->allowable_mask & 8) {
		find->attrib = 8;
		find->size = 0;
		strcpy(find->name, msdos_short_volume_label(process->volume_label));
		process->allowable_mask &= ~8;
		REG16(AX) = 0;
	} else {
		REG16(AX) = 0x12;	// NOTE: return 0x02 if file path is invalid
		m_CF = 1;
	}
}

inline void msdos_int_21h_4fh()
{
	process_t *process = msdos_process_info_get(current_psp);
	UINT32 dta_laddr = (process->dta.w.h << 4) + process->dta.w.l;
	find_t *find = (find_t *)(mem + dta_laddr);
	WIN32_FIND_DATA fd;
	
	dtainfo_t *dtainfo = msdos_dta_info_get(current_psp, dta_laddr);
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		if(FindNextFile(dtainfo->find_handle, &fd)) {
			while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, 0) ||
			      !msdos_find_file_has_8dot3name(&fd)) {
				if(!FindNextFile(dtainfo->find_handle, &fd)) {
					FindClose(dtainfo->find_handle);
					dtainfo->find_handle = INVALID_HANDLE_VALUE;
					break;
				}
			}
		} else {
			FindClose(dtainfo->find_handle);
			dtainfo->find_handle = INVALID_HANDLE_VALUE;
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		find->attrib = (UINT8)(fd.dwFileAttributes & 0x3f);
		msdos_find_file_conv_local_time(&fd);
		FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->date, &find->time);
		find->size = fd.nFileSizeLow;
		strcpy(find->name, msdos_short_name(&fd));
		REG16(AX) = 0;
	} else if(process->allowable_mask & 8) {
		find->attrib = 8;
		find->size = 0;
		strcpy(find->name, msdos_short_volume_label(process->volume_label));
		process->allowable_mask &= ~8;
		REG16(AX) = 0;
	} else {
		REG16(AX) = 0x12;
		m_CF = 1;
	}
}

inline void msdos_int_21h_50h()
{
	if(current_psp != REG16(BX)) {
		process_t *process = msdos_process_info_get(current_psp);
		if(process != NULL) {
			process->psp = REG16(BX);
		}
		current_psp = REG16(BX);
	}
}

inline void msdos_int_21h_51h()
{
	REG16(BX) = current_psp;
}

inline void msdos_int_21h_52h()
{
	SREG(ES) = (DOS_INFO_BASE >> 4);
	i386_load_segment_descriptor(ES);
	REG16(BX) = (DOS_INFO_BASE & 0x0f);
}

inline void msdos_int_21h_54h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	REG8(AL) = process->verify;
}

inline void msdos_int_21h_55h()
{
	psp_t *psp = (psp_t *)(mem + (REG16(DX) << 4));
	
	memcpy(mem + (REG16(DX) << 4), mem + (current_psp << 4), sizeof(psp_t));
	psp->int_22h.dw = *(UINT32 *)(mem + 4 * 0x22);
	psp->int_23h.dw = *(UINT32 *)(mem + 4 * 0x23);
	psp->int_24h.dw = *(UINT32 *)(mem + 4 * 0x24);
	psp->parent_psp = current_psp;
}

inline void msdos_int_21h_56h(int lfn)
{
	char src[MAX_PATH], dst[MAX_PATH];
	strcpy(src, msdos_trimmed_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), lfn));
	strcpy(dst, msdos_trimmed_path((char *)(mem + SREG_BASE(ES) + REG16(DI)), lfn));
	
	if(rename(src, dst)) {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_21h_57h()
{
	FILETIME time, local;
	HANDLE handle;
	
	switch(REG8(AL)) {
	case 0x00:
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} if(!GetFileTime(handle, NULL, NULL, &time)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else {
			FileTimeToLocalFileTime(&time, &local);
			FileTimeToDosDateTime(&local, &REG16(DX), &REG16(CX));
		}
		break;
	case 0x01:
		DosDateTimeToFileTime(REG16(DX), REG16(CX), &local);
		LocalFileTimeToFileTime(&local, &time);
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else if(!SetFileTime(handle, NULL, NULL, &time)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	case 0x04:
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} if(!GetFileTime(handle, NULL, &time, NULL)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else {
			FileTimeToLocalFileTime(&time, &local);
			FileTimeToDosDateTime(&local, &REG16(DX), &REG16(CX));
		}
		break;
	case 0x05:
		DosDateTimeToFileTime(REG16(DX), REG16(CX), &local);
		LocalFileTimeToFileTime(&local, &time);
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else if(!SetFileTime(handle, NULL, &time, NULL)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	case 0x06:
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} if(!GetFileTime(handle, &time, NULL, NULL)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else {
			FileTimeToLocalFileTime(&time, &local);
			FileTimeToDosDateTime(&local, &REG16(DX), &REG16(CX));
		}
		break;
	case 0x07:
		DosDateTimeToFileTime(REG16(DX), REG16(CX), &local);
		LocalFileTimeToFileTime(&local, &time);
		if((handle = (HANDLE)_get_osfhandle(REG16(BX))) == INVALID_HANDLE_VALUE) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		} else if(!SetFileTime(handle, &time, NULL, NULL)) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_58h()
{
	switch(REG8(AL)) {
	case 0x00:
		REG16(AX) = malloc_strategy;
		break;
	case 0x01:
		switch(REG16(BX)) {
		case 0x0000:
		case 0x0001:
		case 0x0002:
		case 0x0040:
		case 0x0041:
		case 0x0042:
		case 0x0080:
		case 0x0081:
		case 0x0082:
			malloc_strategy = REG16(BX);
			break;
		default:
			REG16(AX) = 0x01;
			m_CF = 1;
			break;
		}
		break;
	case 0x02:
		REG8(AL) = umb_linked;
		break;
	case 0x03:
		switch(REG16(BX)) {
		case 0x0000:
		case 0x0001:
			umb_linked = REG8(BL);
			break;
		default:
			REG16(AX) = 0x01;
			m_CF = 1;
			break;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_59h()
{
	REG16(AX) = error_code;
	switch(error_code) {
	case  4: // Too many open files
	case  8: // Insufficient memory
		REG8(BH) = 1; // Out of resource
		break;
	case  5: // Access denied
		REG8(BH) = 3; // Authorization
		break;
	case  7: // Memory control block destroyed
		REG8(BH) = 4; // Internal
		break;
	case  2: // File not found
	case  3: // Path not found
	case 15: // Invaid drive specified
	case 18: // No more files
		REG8(BH) = 8; // Not found
		break;
	case 32: // Sharing violation
	case 33: // Lock violation
		REG8(BH) = 10; // Locked
		break;
//	case 16: // Removal of current directory attempted
	case 19: // Attempted write on protected disk
	case 21: // Drive not ready
//	case 29: // Write failure
//	case 30: // Read failure
//	case 82: // Cannot create subdirectory
		REG8(BH) = 11; // Media
		break;
	case 80: // File already exists
		REG8(BH) = 12; // Already exist
		break;
	default:
		REG8(BH) = 13; // Unknown
		break;
	}
	REG8(BL) = 1; // Retry
	REG8(CH) = 1; // Unknown
}

inline void msdos_int_21h_5ah()
{
	char *path = (char *)(mem + SREG_BASE(DS) + REG16(DX));
	int len = strlen(path);
	char tmp[MAX_PATH];
	
	if(GetTempFileName(path, "TMP", 0, tmp)) {
		int fd = _open(tmp, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
		
		SetFileAttributes(tmp, msdos_file_attribute_create(REG16(CX)) & ~FILE_ATTRIBUTE_READONLY);
		REG16(AX) = fd;
		msdos_file_handler_open(fd, path, _isatty(fd), 2, msdos_drive_number(path), current_psp);
		
		strcpy(path, tmp);
		int dx = REG16(DX) + len;
		int ds = SREG(DS);
		while(dx > 0xffff) {
			dx -= 0x10;
			ds++;
		}
		REG16(DX) = dx;
		SREG(DS) = ds;
		i386_load_segment_descriptor(DS);
	} else {
		REG16(AX) = (UINT16)GetLastError();
		m_CF = 1;
	}
}

inline void msdos_int_21h_5bh()
{
	char *path = msdos_local_file_path((char *)(mem + SREG_BASE(DS) + REG16(DX)), 0);
	
	if(_access(path, 0) == 0) {
		// already exists
		REG16(AX) = 0x50;
		m_CF = 1;
	} else {
		int fd = _open(path, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
		
		if(fd != -1) {
			SetFileAttributes(path, msdos_file_attribute_create(REG16(CX)) & ~FILE_ATTRIBUTE_READONLY);
			REG16(AX) = fd;
			msdos_file_handler_open(fd, path, _isatty(fd), 2, msdos_drive_number(path), current_psp);
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
	}
}

inline void msdos_int_21h_5ch()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(REG8(AL) == 0 || REG8(AL) == 1) {
			static int modes[2] = {_LK_LOCK, _LK_UNLCK};
			UINT32 pos = _tell(REG16(BX));
			_lseek(REG16(BX), (REG16(CX) << 16) | REG16(DX), SEEK_SET);
			if(_locking(REG16(BX), modes[REG8(AL)], (REG16(SI) << 16) | REG16(DI))) {
				REG16(AX) = errno;
				m_CF = 1;
			}
			_lseek(REG16(BX), pos, SEEK_SET);
			// some seconds may be passed in _locking()
			hardware_update();
		} else {
			REG16(AX) = 0x01;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_60h(int lfn)
{
	if(lfn) {
		char full[MAX_PATH], *name;
		GetFullPathName((char *)(mem + SREG_BASE(DS) + REG16(SI)), MAX_PATH, full, &name);
		strcpy((char *)(mem + SREG_BASE(ES) + REG16(DI)), full);
	} else {
		strcpy((char *)(mem + SREG_BASE(ES) + REG16(DI)), msdos_short_full_path((char *)(mem + SREG_BASE(DS) + REG16(SI))));
	}
}

inline void msdos_int_21h_61h()
{
	REG8(AL) = 0;
}

inline void msdos_int_21h_62h()
{
	REG16(BX) = current_psp;
}

inline void msdos_int_21h_63h()
{
	switch(REG8(AL)) {
	case 0x00:
		SREG(DS) = (DBCS_TABLE >> 4);
		i386_load_segment_descriptor(DS);
		REG16(SI) = (DBCS_TABLE & 0x0f);
		REG8(AL) = 0x00;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_65h()
{
	char tmp[0x10000];
	
	switch(REG8(AL)) {
	case 0x07:
		*(UINT8  *)(mem + SREG_BASE(ES) + REG16(DI) + 0) = 0x07;
		*(UINT16 *)(mem + SREG_BASE(ES) + REG16(DI) + 1) = (DBCS_TOP & 0x0f);
		*(UINT16 *)(mem + SREG_BASE(ES) + REG16(DI) + 3) = (DBCS_TOP >> 4);
		REG16(CX) = 0x05;
		break;
	case 0x20:
		sprintf(tmp, "%c", REG8(DL));
		my_strupr(tmp);
		REG8(DL) = tmp[0];
		break;
	case 0x21:
		memset(tmp, 0, sizeof(tmp));
		memcpy(tmp, mem + SREG_BASE(DS) + REG16(DX), REG16(CX));
		my_strupr(tmp);
		memcpy(mem + SREG_BASE(DS) + REG16(DX), tmp, REG16(CX));
		break;
	case 0x22:
		my_strupr((char *)(mem + SREG_BASE(DS) + REG16(DX)));
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_66h()
{
	switch(REG8(AL)) {
	case 0x01:
		REG16(BX) = active_code_page;
		REG16(DX) = system_code_page;
		break;
	case 0x02:
		if(active_code_page == REG16(BX)) {
			REG16(AX) = 0xeb41;
		} else if(_setmbcp(REG16(BX)) == 0) {
			active_code_page = REG16(BX);
			msdos_dbcs_table_update();
			REG16(AX) = 0xeb41;
		} else {
			REG16(AX) = 0x25;
			m_CF = 1;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_67h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) <= MAX_FILES) {
		process->max_files = max(REG16(BX), 20);
	} else {
		REG16(AX) = 0x08;
		m_CF = 1;
	}
}

inline void msdos_int_21h_68h()
{
	process_t *process = msdos_process_info_get(current_psp);
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		// fflush(_fdopen(REG16(BX), ""));
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_69h()
{
	drive_info_t *info = (drive_info_t *)(mem + SREG_BASE(DS) + REG16(DX));
	char path[] = "A:\\";
	char volume_label[MAX_PATH];
	DWORD serial_number = 0;
	char file_system[MAX_PATH];
	
	if(REG8(BL) == 0) {
		path[0] = 'A' + _getdrive() - 1;
	} else {
		path[0] = 'A' + REG8(BL) - 1;
	}
	
	switch(REG8(AL)) {
	case 0x00:
		if(GetVolumeInformation(path, volume_label, MAX_PATH, &serial_number, NULL, NULL, file_system, MAX_PATH)) {
			info->info_level = 0;
			info->serial_number = serial_number;
			memset(info->volume_label, 0x20, 11);
			memcpy(info->volume_label, volume_label, min(strlen(volume_label), 11));
			memset(info->file_system, 0x20, 8);
			memcpy(info->file_system, file_system, min(strlen(file_system), 8));
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
		break;
	case 0x01:
		REG16(AX) = 0x03;
		m_CF = 1;
	}
}

inline void msdos_int_21h_6ah()
{
	REG8(AH) = 0x68;
	msdos_int_21h_68h();
}

inline void msdos_int_21h_6bh()
{
	REG8(AL) = 0;
}

inline void msdos_int_21h_6ch(int lfn)
{
	char *path = msdos_local_file_path((char *)(mem + SREG_BASE(DS) + REG16(SI)), lfn);
	int mode = REG8(BL) & 0x03;
	
	if(mode < 0x03) {
		if(_access(path, 0) == 0) {
			// file exists
			if(REG8(DL) & 1) {
				int fd = -1;
				UINT16 info;
				
				if(msdos_is_con_path(path)) {
					fd = msdos_open("CON", file_mode[mode].mode);
					info = 0x80d3;
				} else {
					fd = msdos_open(path, file_mode[mode].mode);
					info = msdos_drive_number(path);
				}
				if(fd != -1) {
					REG16(AX) = fd;
					REG16(CX) = 1;
					msdos_file_handler_open(fd, path, _isatty(fd), mode, info, current_psp);
				} else {
					REG16(AX) = errno;
					m_CF = 1;
				}
			} else if(REG8(DL) & 2) {
				int attr = GetFileAttributes(path);
				int fd = -1;
				UINT16 info;
				
				if(msdos_is_con_path(path)) {
					fd = msdos_open("CON", file_mode[mode].mode);
					info = 0x80d3;
				} else {
					fd = _open(path, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
					info = msdos_drive_number(path);
				}
				if(fd != -1) {
					if(attr == -1) {
						attr = msdos_file_attribute_create(REG16(CX)) & ~FILE_ATTRIBUTE_READONLY;
					}
					SetFileAttributes(path, attr);
					REG16(AX) = fd;
					REG16(CX) = 3;
					msdos_file_handler_open(fd, path, _isatty(fd), 2, info, current_psp);
				} else {
					REG16(AX) = errno;
					m_CF = 1;
				}
			} else {
				REG16(AX) = 0x50;
				m_CF = 1;
			}
		} else {
			// file not exists
			if(REG8(DL) & 0x10) {
				int fd = _open(path, _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE);
				
				if(fd != -1) {
					SetFileAttributes(path, msdos_file_attribute_create(REG16(CX)) & ~FILE_ATTRIBUTE_READONLY);
					REG16(AX) = fd;
					REG16(CX) = 2;
					msdos_file_handler_open(fd, path, _isatty(fd), 2, msdos_drive_number(path), current_psp);
				} else {
					REG16(AX) = errno;
					m_CF = 1;
				}
			} else {
				REG16(AX) = 0x02;
				m_CF = 1;
			}
		}
	} else {
		REG16(AX) = 0x0c;
		m_CF = 1;
	}
}

inline void msdos_int_21h_710dh()
{
	// reset drive
}

inline void msdos_int_21h_714eh()
{
	process_t *process = msdos_process_info_get(current_psp);
	find_lfn_t *find = (find_lfn_t *)(mem + SREG_BASE(ES) + REG16(DI));
	char *path = (char *)(mem + SREG_BASE(DS) + REG16(DX));
	WIN32_FIND_DATA fd;
	
	dtainfo_t *dtainfo = msdos_dta_info_get(current_psp, LFN_DTA_LADDR);
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		FindClose(dtainfo->find_handle);
		dtainfo->find_handle = INVALID_HANDLE_VALUE;
	}
	strcpy(process->volume_label, msdos_volume_label(path));
	process->allowable_mask = REG8(CL);
	process->required_mask = REG8(CH);
	
	if((process->allowable_mask & 8) && !msdos_match_volume_label(path, process->volume_label) && !msdos_match_volume_label(path, msdos_short_volume_label(process->volume_label))) {
		process->allowable_mask &= ~8;
	}
	if((dtainfo->find_handle = FindFirstFile(path, &fd)) != INVALID_HANDLE_VALUE) {
		while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, process->required_mask)) {
			if(!FindNextFile(dtainfo->find_handle, &fd)) {
				FindClose(dtainfo->find_handle);
				dtainfo->find_handle = INVALID_HANDLE_VALUE;
				break;
			}
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		find->attrib = fd.dwFileAttributes;
		msdos_find_file_conv_local_time(&fd);
		if(REG16(SI) == 0) {
			find->ctime_lo.dw = fd.ftCreationTime.dwLowDateTime;
			find->ctime_hi.dw = fd.ftCreationTime.dwHighDateTime;
			find->atime_lo.dw = fd.ftLastAccessTime.dwLowDateTime;
			find->atime_hi.dw = fd.ftLastAccessTime.dwHighDateTime;
			find->mtime_lo.dw = fd.ftLastWriteTime.dwLowDateTime;
			find->mtime_hi.dw = fd.ftLastWriteTime.dwHighDateTime;
		} else {
			FileTimeToDosDateTime(&fd.ftCreationTime, &find->ctime_lo.w.h, &find->ctime_lo.w.l);
			FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->atime_lo.w.h, &find->atime_lo.w.l);
			FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->mtime_lo.w.h, &find->mtime_lo.w.l);
		}
		find->size_hi = fd.nFileSizeHigh;
		find->size_lo = fd.nFileSizeLow;
		strcpy(find->full_name, fd.cFileName);
		strcpy(find->short_name, fd.cAlternateFileName);
		REG16(AX) = dtainfo - dtalist;
	} else if(process->allowable_mask & 8) {
		// volume label
		find->attrib = 8;
		find->size_hi = find->size_lo = 0;
		strcpy(find->full_name, process->volume_label);
		strcpy(find->short_name, msdos_short_volume_label(process->volume_label));
		process->allowable_mask &= ~8;
		REG16(AX) = dtainfo - dtalist;
	} else {
		REG16(AX) = 0x12;	// NOTE: return 0x02 if file path is invalid
		m_CF = 1;
	}
}

inline void msdos_int_21h_714fh()
{
	process_t *process = msdos_process_info_get(current_psp);
	find_lfn_t *find = (find_lfn_t *)(mem + SREG_BASE(ES) + REG16(DI));
	WIN32_FIND_DATA fd;
	
	if (REG16(BX) >= MAX_DTAINFO) {
		REG16(AX) = EBADF;
		m_CF = 1;
		return;
	}
	dtainfo_t *dtainfo = &dtalist[REG16(BX)];
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		if(FindNextFile(dtainfo->find_handle, &fd)) {
			while(!msdos_find_file_check_attribute(fd.dwFileAttributes, process->allowable_mask, process->required_mask)) {
				if(!FindNextFile(dtainfo->find_handle, &fd)) {
					FindClose(dtainfo->find_handle);
					dtainfo->find_handle = INVALID_HANDLE_VALUE;
					break;
				}
			}
		} else {
			FindClose(dtainfo->find_handle);
			dtainfo->find_handle = INVALID_HANDLE_VALUE;
		}
	}
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		find->attrib = fd.dwFileAttributes;
		msdos_find_file_conv_local_time(&fd);
		if(REG16(SI) == 0) {
			find->ctime_lo.dw = fd.ftCreationTime.dwLowDateTime;
			find->ctime_hi.dw = fd.ftCreationTime.dwHighDateTime;
			find->atime_lo.dw = fd.ftLastAccessTime.dwLowDateTime;
			find->atime_hi.dw = fd.ftLastAccessTime.dwHighDateTime;
			find->mtime_lo.dw = fd.ftLastWriteTime.dwLowDateTime;
			find->mtime_hi.dw = fd.ftLastWriteTime.dwHighDateTime;
		} else {
			FileTimeToDosDateTime(&fd.ftCreationTime, &find->ctime_lo.w.h, &find->ctime_lo.w.l);
			FileTimeToDosDateTime(&fd.ftLastAccessTime, &find->atime_lo.w.h, &find->atime_lo.w.l);
			FileTimeToDosDateTime(&fd.ftLastWriteTime, &find->mtime_lo.w.h, &find->mtime_lo.w.l);
		}
		find->size_hi = fd.nFileSizeHigh;
		find->size_lo = fd.nFileSizeLow;
		strcpy(find->full_name, fd.cFileName);
		strcpy(find->short_name, fd.cAlternateFileName);
	} else if(process->allowable_mask & 8) {
		// volume label
		find->attrib = 8;
		find->size_hi = find->size_lo = 0;
		strcpy(find->full_name, process->volume_label);
		strcpy(find->short_name, msdos_short_volume_label(process->volume_label));
		process->allowable_mask &= ~8;
	} else {
		REG16(AX) = 0x12;
		m_CF = 1;
	}
}

inline void msdos_int_21h_71a0h()
{
	DWORD max_component_len, file_sys_flag;
	
	if(GetVolumeInformation((char *)(mem + SREG_BASE(DS) + REG16(DX)), NULL, 0, NULL, &max_component_len, &file_sys_flag, (char *)(mem + SREG_BASE(ES) + REG16(DI)), REG16(CX))) {
		REG16(BX) = (UINT16)file_sys_flag;
		REG16(CX) = (UINT16)max_component_len;		// 255
		REG16(DX) = (UINT16)max_component_len + 5;	// 260
	} else {
		REG16(AX) = (UINT16)GetLastError();
		m_CF = 1;
	}
}

inline void msdos_int_21h_71a1h()
{
	process_t *process = msdos_process_info_get(current_psp);
	find_t *find = (find_t *)(mem + (process->dta.w.h << 4) + process->dta.w.l);
	
	if (REG16(BX) >= MAX_DTAINFO) {
		REG16(AX) = EBADF;
		m_CF = 1;
		return;
	}
	dtainfo_t *dtainfo = &dtalist[REG16(BX)];
	if(dtainfo->find_handle != INVALID_HANDLE_VALUE) {
		FindClose(dtainfo->find_handle);
		dtainfo->find_handle = INVALID_HANDLE_VALUE;
	}
}

inline void msdos_int_21h_71a6h()
{
	process_t *process = msdos_process_info_get(current_psp);
	UINT8 *buffer = (UINT8 *)(mem + SREG_BASE(DS) + REG16(DX));
	struct _stat64 status;
	DWORD serial_number = 0;
	
	if(REG16(BX) < process->max_files && file_handler[REG16(BX)].valid) {
		if(_fstat64(REG16(BX), &status) == 0) {
			if(file_handler[REG16(BX)].path[1] == ':') {
				// NOTE: we need to consider the network file path "\\host\share\"
				char volume[] = "A:\\";
				volume[0] = file_handler[REG16(BX)].path[1];
				GetVolumeInformation(volume, NULL, 0, &serial_number, NULL, NULL, NULL, 0);
			}
			*(UINT32 *)(buffer + 0x00) = GetFileAttributes(file_handler[REG16(BX)].path);
			*(UINT32 *)(buffer + 0x04) = (UINT32)(status.st_ctime & 0xffffffff);
			*(UINT32 *)(buffer + 0x08) = (UINT32)((status.st_ctime >> 32) & 0xffffffff);
			*(UINT32 *)(buffer + 0x0c) = (UINT32)(status.st_atime & 0xffffffff);
			*(UINT32 *)(buffer + 0x10) = (UINT32)((status.st_atime >> 32) & 0xffffffff);
			*(UINT32 *)(buffer + 0x14) = (UINT32)(status.st_mtime & 0xffffffff);
			*(UINT32 *)(buffer + 0x18) = (UINT32)((status.st_mtime >> 32) & 0xffffffff);
			*(UINT32 *)(buffer + 0x1c) = serial_number;
			*(UINT32 *)(buffer + 0x20) = (UINT32)((status.st_size >> 32) & 0xffffffff);
			*(UINT32 *)(buffer + 0x24) = (UINT32)(status.st_size & 0xffffffff);
			*(UINT32 *)(buffer + 0x28) = status.st_nlink;
			// this is dummy id and it will be changed when it is reopend...
			*(UINT32 *)(buffer + 0x2c) = 0;
			*(UINT32 *)(buffer + 0x30) = file_handler[REG16(BX)].id;
		} else {
			REG16(AX) = errno;
			m_CF = 1;
		}
	} else {
		REG16(AX) = 0x06;
		m_CF = 1;
	}
}

inline void msdos_int_21h_71a7h()
{
	switch(REG8(BL)) {
	case 0x00:
		if(!FileTimeToDosDateTime((FILETIME *)(mem + SREG_BASE(DS) + REG16(SI)), &REG16(DX), &REG16(CX))) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	case 0x01:
		// NOTE: we need to check BH that shows 10-millisecond untils past time in CX
		if(!DosDateTimeToFileTime(REG16(DX), REG16(CX), (FILETIME *)(mem + SREG_BASE(ES) + REG16(DI)))) {
			REG16(AX) = (UINT16)GetLastError();
			m_CF = 1;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_21h_71a8h()
{
	if(REG8(DH) == 0) {
		char tmp[MAX_PATH], fcb[MAX_PATH];
		strcpy(tmp, msdos_short_path((char *)(mem + SREG_BASE(DS) + REG16(SI))));
		memset(fcb, 0x20, sizeof(fcb));
		int len = strlen(tmp);
		int pos = 0;
		for(int i = 0; i < len; i++) {
			if(tmp[i] == '.') {
				pos = 8;
			} else {
				if(msdos_lead_byte_check(tmp[i])) {
					fcb[pos++] = tmp[i++];
				}
				fcb[pos++] = tmp[i];
			}
		}
		memcpy((char *)(mem + SREG_BASE(ES) + REG16(DI)), fcb, 11);
	} else {
		strcpy((char *)(mem + SREG_BASE(ES) + REG16(DI)), msdos_short_path((char *)(mem + SREG_BASE(DS) + REG16(SI))));
	}
}

inline void msdos_int_21h_7303h()
{
	char *path = (char *)(mem + SREG_BASE(DS) + REG16(DX));
	ext_space_info_t *info = (ext_space_info_t *)(mem + SREG_BASE(ES) + REG16(DI));
	DWORD sectors_per_cluster, bytes_per_sector, free_clusters, total_clusters;
	
	if(GetDiskFreeSpace(path, &sectors_per_cluster, &bytes_per_sector, &free_clusters, &total_clusters)) {
		info->size_of_structure = sizeof(ext_space_info_t);
		info->structure_version = 0;
		info->sectors_per_cluster = sectors_per_cluster;
		info->bytes_per_sector = bytes_per_sector;
		info->available_clusters_on_drive = free_clusters;
		info->total_clusters_on_drive = total_clusters;
		info->available_sectors_on_drive = sectors_per_cluster * free_clusters;
		info->total_sectors_on_drive = sectors_per_cluster * total_clusters;
		info->available_allocation_units = free_clusters;	// ???
		info->total_allocation_units = total_clusters;		// ???
	} else {
		REG16(AX) = errno;
		m_CF = 1;
	}
}

inline void msdos_int_25h()
{
	UINT16 seg, ofs;
	DWORD dwSize;
	
#if defined(HAS_I386)
	I386OP(pushf)();
#else
	PREFIX86(_pushf());
#endif
	
	if(!(REG8(AL) < 26)) {
		REG8(AL) = 0x01; // unit unknown
		m_CF = 1;
	} else if(!msdos_drive_param_block_update(REG8(AL), &seg, &ofs, 0)) {
		REG8(AL) = 0x02; // drive not ready
		m_CF = 1;
	} else {
		dpb_t *dpb = (dpb_t *)(mem + (seg << 4) + ofs);
		char dev[64];
		sprintf(dev, "\\\\.\\%c:", 'A' + REG8(AL));
		
		HANDLE hFile = CreateFile(dev, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);
		if(hFile == INVALID_HANDLE_VALUE) {
			REG8(AL) = 0x02; // drive not ready
			m_CF = 1;
		} else {
			if(DeviceIoControl(hFile, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwSize, NULL) == 0) {
				REG8(AL) = 0x02; // drive not ready
				m_CF = 1;
			} else if(SetFilePointer(hFile, REG16(DX) * dpb->bytes_per_sector, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
				REG8(AL) = 0x08; // sector not found
				m_CF = 1;
			} else if(ReadFile(hFile, mem + SREG_BASE(DS) + REG16(BX), REG16(CX) * dpb->bytes_per_sector, &dwSize, NULL) == 0) {
				REG8(AL) = 0x0b; // read error
				m_CF = 1;
			}
			CloseHandle(hFile);
		}
	}
}

inline void msdos_int_26h()
{
	// this operation may cause serious damage for drives, so always returns error...
	UINT16 seg, ofs;
	DWORD dwSize;
	
#if defined(HAS_I386)
	I386OP(pushf)();
#else
	PREFIX86(_pushf());
#endif
	
	if(!(REG8(AL) < 26)) {
		REG8(AL) = 0x01; // unit unknown
		m_CF = 1;
	} else if(!msdos_drive_param_block_update(REG8(AL), &seg, &ofs, 0)) {
		REG8(AL) = 0x02; // drive not ready
		m_CF = 1;
	} else {
		dpb_t *dpb = (dpb_t *)(mem + (seg << 4) + ofs);
		char dev[64];
		sprintf(dev, "\\\\.\\%c:", 'A' + REG8(AL));
		
		if(dpb->media_type == 0xf8) {
			// this drive is not a floppy
//			if(!(((dos_info_t *)(mem + DOS_INFO_TOP))->dos_flag & 0x40)) {
//				fatalerror("This application tried the absolute disk write to drive %c:\n", 'A' + REG8(AL));
//			}
			REG8(AL) = 0x02; // drive not ready
			m_CF = 1;
		} else {
			HANDLE hFile = CreateFile(dev, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);
			if(hFile == INVALID_HANDLE_VALUE) {
				REG8(AL) = 0x02; // drive not ready
				m_CF = 1;
			} else {
				if(DeviceIoControl(hFile, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwSize, NULL) == 0) {
					REG8(AL) = 0x02; // drive not ready
					m_CF = 1;
				} else if(SetFilePointer(hFile, REG16(DX) * dpb->bytes_per_sector, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
					REG8(AL) = 0x08; // sector not found
					m_CF = 1;
				} else if(WriteFile(hFile, mem + SREG_BASE(DS) + REG16(BX), REG16(CX) * dpb->bytes_per_sector, &dwSize, NULL) == 0) {
					REG8(AL) = 0x0a; // write error
					m_CF = 1;
				}
				CloseHandle(hFile);
			}
		}
	}
}

inline void msdos_int_27h()
{
	int mcb_seg = SREG(CS) - 1;
	mcb_t *mcb = (mcb_t *)(mem + (mcb_seg << 4));
	
	mcb->paragraphs = (REG16(DX) >> 4);
	mcb_seg += mcb->paragraphs + 1;
	msdos_mcb_create(mcb_seg, 'Z', 0, (MEMORY_END >> 4) - mcb_seg - 1);
	
	msdos_process_terminate(SREG(CS), retval | 0x300, 0);
}

inline void msdos_int_29h()
{
	msdos_putch(REG8(AL));
}

inline void msdos_int_2eh()
{
	char tmp[MAX_PATH], command[MAX_PATH], opt[MAX_PATH];
	memset(tmp, 0, sizeof(tmp));
	strcpy(tmp, (char *)(mem + SREG_BASE(DS) + REG16(SI)));
	char *token = my_strtok(tmp, " ");
	strcpy(command, token);
	strcpy(opt, token + strlen(token) + 1);
	
	param_block_t *param = (param_block_t *)(mem + WORK_TOP);
	param->env_seg = 0;
	param->cmd_line.w.l = 44;
	param->cmd_line.w.h = (WORK_TOP >> 4);
	param->fcb1.w.l = 24;
	param->fcb1.w.h = (WORK_TOP >> 4);
	param->fcb2.w.l = 24;
	param->fcb2.w.h = (WORK_TOP >> 4);
	
	memset(mem + WORK_TOP + 24, 0x20, 20);
	
	cmd_line_t *cmd_line = (cmd_line_t *)(mem + WORK_TOP + 44);
	cmd_line->len = strlen(opt);
	strcpy(cmd_line->cmd, opt);
	cmd_line->cmd[cmd_line->len] = 0x0d;
	
	msdos_process_exec(command, param, 0);
	REG8(AL) = 0;
}

inline void msdos_int_2fh_16h()
{
	switch(REG8(AL)) {
	case 0x00:
		{
			OSVERSIONINFO osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&osvi);
			REG8(AL) = osvi.dwMajorVersion;
			REG8(AH) = osvi.dwMinorVersion;
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_1ah()
{
	switch(REG8(AL)) {
	case 0x00:
		// ansi.sys is installed
		REG8(AL) = 0xff;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_43h()
{
	switch(REG8(AL)) {
	case 0x00:
		// xms is not installed
		REG8(AL) = 0;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_4ah()
{
	switch(REG8(AL)) {
	case 0x01:
	case 0x02:
		// hma is not installed
		REG16(BX) = 0;
		SREG(ES) = 0xffff;
		i386_load_segment_descriptor(ES);
		REG16(DI) = 0xffff;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_4fh()
{
	switch(REG8(AL)) {
	case 0x00:
		REG16(AX) = 0;
		REG8(DL) = 1;	// major version
		REG8(DH) = 0;	// minor version
		break;
	case 0x01:
		REG16(AX) = 0;
		REG16(BX) = active_code_page;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_aeh()
{
	switch(REG8(AL)) {
	case 0x00:
		REG8(AL) = 0;
		break;
	case 0x01:
		{
			char command[MAX_PATH];
			memset(command, 0, sizeof(command));
			memcpy(command, mem + SREG_BASE(DS) + REG16(SI) + 1, mem[SREG_BASE(DS) + REG16(SI)]);
			
			param_block_t *param = (param_block_t *)(mem + WORK_TOP);
			param->env_seg = 0;
			param->cmd_line.w.l = 44;
			param->cmd_line.w.h = (WORK_TOP >> 4);
			param->fcb1.w.l = 24;
			param->fcb1.w.h = (WORK_TOP >> 4);
			param->fcb2.w.l = 24;
			param->fcb2.w.h = (WORK_TOP >> 4);
			
			memset(mem + WORK_TOP + 24, 0x20, 20);
			
			cmd_line_t *cmd_line = (cmd_line_t *)(mem + WORK_TOP + 44);
			cmd_line->len = mem[SREG_BASE(DS) + REG16(BX) + 1];
			memcpy(cmd_line->cmd, mem + SREG_BASE(DS) + REG16(BX) + 2, cmd_line->len);
			cmd_line->cmd[cmd_line->len] = 0x0d;
			
			if(msdos_process_exec(command, param, 0)) {
				REG16(AX) = 0x02;
				m_CF = 1;
			}
		}
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

inline void msdos_int_2fh_b7h()
{
	switch(REG8(AL)) {
	case 0x00:
		// append is not installed
		REG8(AL) = 0;
		break;
	default:
		REG16(AX) = 0x01;
		m_CF = 1;
		break;
	}
}

void msdos_syscall(unsigned num)
{
	switch(num) {
	case 0x00:
		error("division by zero\n");
		msdos_process_terminate(current_psp, (retval & 0xff) | 0x200, 1);
		break;
	case 0x04:
		error("overflow\n");
		msdos_process_terminate(current_psp, (retval & 0xff) | 0x200, 1);
		break;
	case 0x06:
		// NOTE: ish.com has illegal instruction...
//		error("illegal instruction\n");
//		msdos_process_terminate(current_psp, (retval & 0xff) | 0x200, 1);
		break;
	case 0x08:
	case 0x09:
	case 0x0a:
	case 0x0b:
	case 0x0c:
	case 0x0d:
	case 0x0e:
	case 0x0f:
		// EOI
		pic[0].isr &= ~(1 << (num - 0x08));
		pic_update();
		break;
	case 0x10:
		// PC BIOS - Video
		if(!restore_console_on_exit && (scr_width != 80 || scr_height != 25)) {
			change_console_size_to_80x25();
			restore_console_on_exit = true;
		}
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x00: pcbios_int_10h_00h(); break;
		case 0x01: pcbios_int_10h_01h(); break;
		case 0x02: pcbios_int_10h_02h(); break;
		case 0x03: pcbios_int_10h_03h(); break;
		case 0x05: pcbios_int_10h_05h(); break;
		case 0x06: pcbios_int_10h_06h(); break;
		case 0x07: pcbios_int_10h_07h(); break;
		case 0x08: pcbios_int_10h_08h(); break;
		case 0x09: pcbios_int_10h_09h(); break;
		case 0x0a: pcbios_int_10h_0ah(); break;
		case 0x0b: break;
		case 0x0c: break;
		case 0x0d: break;
		case 0x0e: pcbios_int_10h_0eh(); break;
		case 0x0f: pcbios_int_10h_0fh(); break;
		case 0x10: break;
		case 0x11: break;
		case 0x12: REG8(AL) = 0x00; break;
		case 0x13: pcbios_int_10h_13h(); break;
		case 0x18: REG8(AL) = 0x86; break;
		case 0x1a: REG8(AL) = 0x00; break;
		case 0x1b: break;
		case 0x1c: REG8(AL) = 0x00; break;
		case 0x1d: pcbios_int_10h_1dh(); break;
		case 0x82: pcbios_int_10h_82h(); break;
		case 0xef: REG16(DX) = 0xffff; break;
		case 0xfe: pcbios_int_10h_feh(); break;
		case 0xff: pcbios_int_10h_ffh(); break;
		default:
			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			break;
		}
		break;
	case 0x11:
		// PC BIOS - Get Equipment List
#ifdef SUPPORT_FPU
		REG16(AX) = 0x22;
#else
		REG16(AX) = 0x20;
#endif
		break;
	case 0x12:
		// PC BIOS - Get Memory Size
		REG16(AX) = MEMORY_END / 1024;
		break;
	case 0x13:
		// PC BIOS - Disk
//		fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
		REG8(AH) = 0xff;
		m_CF = 1;
		break;
	case 0x14:
		// PC BIOS - Serial I/O
//		fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
		REG8(AH) = 0xff;
		m_CF = 1;
		break;
	case 0x15:
		// PC BIOS
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x23: pcbios_int_15h_23h(); break;
		case 0x24: pcbios_int_15h_24h(); break;
		case 0x49: pcbios_int_15h_49h(); break;
		case 0x86: pcbios_int_15h_86h(); break;
		case 0x87: pcbios_int_15h_87h(); break;
		case 0x88: pcbios_int_15h_88h(); break;
		case 0x89: pcbios_int_15h_89h(); break;
#if defined(HAS_I386)
		case 0xc9: pcbios_int_15h_c9h(); break;
#endif
		case 0xca: pcbios_int_15h_cah(); break;
		default:
//			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			REG8(AH)=0x86;
			m_CF = 1;
			break;
		}
		break;
	case 0x16:
		// PC BIOS - Keyboard
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x00: pcbios_int_16h_00h(); break;
		case 0x01: pcbios_int_16h_01h(); break;
		case 0x02: pcbios_int_16h_02h(); break;
		case 0x03: pcbios_int_16h_03h(); break;
		case 0x05: pcbios_int_16h_05h(); break;
		case 0x10: pcbios_int_16h_00h(); break;
		case 0x11: pcbios_int_16h_01h(); break;
		case 0x12: pcbios_int_16h_12h(); break;
		case 0x13: pcbios_int_16h_13h(); break;
		case 0x14: pcbios_int_16h_14h(); break;
		default:
			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			break;
		}
		break;
	case 0x17:
		// PC BIOS - Printer
		fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
		break;
	case 0x1a:
		// PC BIOS - Timer
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x00: pcbios_int_1ah_00h(); break;
		case 0x01: break;
		case 0x02: pcbios_int_1ah_02h(); break;
		case 0x03: break;
		case 0x04: pcbios_int_1ah_04h(); break;
		case 0x05: break;
		case 0x0a: pcbios_int_1ah_0ah(); break;
		case 0x0b: break;
		default:
			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			break;
		}
		break;
	case 0x20:
		msdos_process_terminate(SREG(CS), retval, 1);
		break;
	case 0x21:
		// MS-DOS System Call
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x00: msdos_int_21h_00h(); break;
		case 0x01: msdos_int_21h_01h(); break;
		case 0x02: msdos_int_21h_02h(); break;
		case 0x03: msdos_int_21h_03h(); break;
		case 0x04: msdos_int_21h_04h(); break;
		case 0x05: msdos_int_21h_05h(); break;
		case 0x06: msdos_int_21h_06h(); break;
		case 0x07: msdos_int_21h_07h(); break;
		case 0x08: msdos_int_21h_08h(); break;
		case 0x09: msdos_int_21h_09h(); break;
		case 0x0a: msdos_int_21h_0ah(); break;
		case 0x0b: msdos_int_21h_0bh(); break;
		case 0x0c: msdos_int_21h_0ch(); break;
		case 0x0d: msdos_int_21h_0dh(); break;
		case 0x0e: msdos_int_21h_0eh(); break;
		// 0x0f: open file with fcb
		// 0x10: close file with fcb
		case 0x11: msdos_int_21h_11h(); break;
		case 0x12: msdos_int_21h_12h(); break;
		case 0x13: msdos_int_21h_13h(); break;
		// 0x14: sequential read with fcb
		// 0x15: sequential write with fcb
		// 0x16: create new file with fcb
		// 0x17: rename file with fcb
		case 0x18: msdos_int_21h_18h(); break;
		case 0x19: msdos_int_21h_19h(); break;
		case 0x1a: msdos_int_21h_1ah(); break;
		case 0x1b: msdos_int_21h_1bh(); break;
		case 0x1c: msdos_int_21h_1ch(); break;
		case 0x1d: msdos_int_21h_1dh(); break;
		case 0x1e: msdos_int_21h_1eh(); break;
		case 0x1f: msdos_int_21h_1fh(); break;
		case 0x20: msdos_int_21h_20h(); break;
		// 0x21: random read with fcb
		// 0x22: randome write with fcb
		// 0x23: get file size with fcb
		// 0x24: set relative record field with fcb
		case 0x25: msdos_int_21h_25h(); break;
		case 0x26: msdos_int_21h_26h(); break;
		// 0x27: random block read with fcb
		// 0x28: random block write with fcb
		case 0x29: msdos_int_21h_29h(); break;
		case 0x2a: msdos_int_21h_2ah(); break;
		case 0x2b: msdos_int_21h_2bh(); break;
		case 0x2c: msdos_int_21h_2ch(); break;
		case 0x2d: msdos_int_21h_2dh(); break;
		case 0x2e: msdos_int_21h_2eh(); break;
		case 0x2f: msdos_int_21h_2fh(); break;
		case 0x30: msdos_int_21h_30h(); break;
		case 0x31: msdos_int_21h_31h(); break;
		case 0x32: msdos_int_21h_32h(); break;
		case 0x33: msdos_int_21h_33h(); break;
		// 0x34: get address of indos flag
		case 0x35: msdos_int_21h_35h(); break;
		case 0x36: msdos_int_21h_36h(); break;
		case 0x37: msdos_int_21h_37h(); break;
		// 0x38: get country-specific information
		case 0x39: msdos_int_21h_39h(0); break;
		case 0x3a: msdos_int_21h_3ah(0); break;
		case 0x3b: msdos_int_21h_3bh(0); break;
		case 0x3c: msdos_int_21h_3ch(); break;
		case 0x3d: msdos_int_21h_3dh(); break;
		case 0x3e: msdos_int_21h_3eh(); break;
		case 0x3f: msdos_int_21h_3fh(); break;
		case 0x40: msdos_int_21h_40h(); break;
		case 0x41: msdos_int_21h_41h(0); break;
		case 0x42: msdos_int_21h_42h(); break;
		case 0x43: msdos_int_21h_43h(0); break;
		case 0x44: msdos_int_21h_44h(); break;
		case 0x45: msdos_int_21h_45h(); break;
		case 0x46: msdos_int_21h_46h(); break;
		case 0x47: msdos_int_21h_47h(0); break;
		case 0x48: msdos_int_21h_48h(); break;
		case 0x49: msdos_int_21h_49h(); break;
		case 0x4a: msdos_int_21h_4ah(); break;
		case 0x4b: msdos_int_21h_4bh(); break;
		case 0x4c: msdos_int_21h_4ch(); break;
		case 0x4d: msdos_int_21h_4dh(); break;
		case 0x4e: msdos_int_21h_4eh(); break;
		case 0x4f: msdos_int_21h_4fh(); break;
		case 0x50: msdos_int_21h_50h(); break;
		case 0x51: msdos_int_21h_51h(); break;
		case 0x52: msdos_int_21h_52h(); break;
		// 0x53: translate bios parameter block to drive param bock
		case 0x54: msdos_int_21h_54h(); break;
		case 0x55: msdos_int_21h_55h(); break;
		case 0x56: msdos_int_21h_56h(0); break;
		case 0x57: msdos_int_21h_57h(); break;
		case 0x58: msdos_int_21h_58h(); break;
		case 0x59: msdos_int_21h_59h(); break;
		case 0x5a: msdos_int_21h_5ah(); break;
		case 0x5b: msdos_int_21h_5bh(); break;
		case 0x5c: msdos_int_21h_5ch(); break;
		// 0x5e: ms-network
		// 0x5f: ms-network
		case 0x60: msdos_int_21h_60h(0); break;
		case 0x61: msdos_int_21h_61h(); break;
		case 0x62: msdos_int_21h_62h(); break;
		case 0x63: msdos_int_21h_63h(); break;
		// 0x64: set device driver lockahead flag
		case 0x65: msdos_int_21h_65h(); break;
		case 0x66: msdos_int_21h_66h(); break;
		case 0x67: msdos_int_21h_67h(); break;
		case 0x68: msdos_int_21h_68h(); break;
		case 0x69: msdos_int_21h_69h(); break;
		case 0x6a: msdos_int_21h_6ah(); break;
		case 0x6b: msdos_int_21h_6bh(); break;
		case 0x6c: msdos_int_21h_6ch(0); break;
		// 0x6d: find first rom program
		// 0x6e: find next rom program
		// 0x6f: get/set rom scan start address
		// 0x70: windows95 get/set internationalization information
		case 0x71:
			// windows95 long filename functions
			switch(REG8(AL)) {
			case 0x0d: msdos_int_21h_710dh(); break;
			case 0x39: msdos_int_21h_39h(1); break;
			case 0x3a: msdos_int_21h_3ah(1); break;
			case 0x3b: msdos_int_21h_3bh(1); break;
			case 0x41: msdos_int_21h_41h(1); break;
			case 0x43: msdos_int_21h_43h(1); break;
			case 0x47: msdos_int_21h_47h(1); break;
			case 0x4e: msdos_int_21h_714eh(); break;
			case 0x4f: msdos_int_21h_714fh(); break;
			case 0x56: msdos_int_21h_56h(1); break;
			case 0x60: msdos_int_21h_60h(1); break;
			case 0x6c: msdos_int_21h_6ch(1); break;
			case 0xa0: msdos_int_21h_71a0h(); break;
			case 0xa1: msdos_int_21h_71a1h(); break;
			case 0xa6: msdos_int_21h_71a6h(); break;
			case 0xa7: msdos_int_21h_71a7h(); break;
			case 0xa8: msdos_int_21h_71a8h(); break;
			// 0xa9: server create/open file
			// 0xaa: create/terminate SUBST
			default:
//				fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
				REG16(AX) = 0x7100;
				m_CF = 1;
				break;
			}
			break;
		// 0x72: Windows95 beta - LFN FindClose
		case 0x73:
			// windows95 fat32 functions
			switch(REG8(AL)) {
			// 0x00: drive locking ???
			// 0x01: drive locking ???
			// 0x02: get extended dpb
			case 0x03: msdos_int_21h_7303h(); break;
			// 0x04: set dpb to use for formatting
			default:
//				fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
				REG16(AX) = 0x7300;
				m_CF = 1;
				break;
			}
			break;
		default:
//			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			REG16(AX) = 0x01;
			m_CF = 1;
			break;
		}
		if(m_CF) {
			error_code = REG16(AX);
		}
		break;
	case 0x22:
		fatalerror("int 22h (terminate address)\n");
	case 0x23:
		msdos_process_terminate(current_psp, (retval & 0xff) | 0x100, 1);
		break;
	case 0x24:
		msdos_process_terminate(current_psp, (retval & 0xff) | 0x200, 1);
		break;
	case 0x25:
		msdos_int_25h();
		break;
	case 0x26:
		msdos_int_26h();
		break;
	case 0x27:
		msdos_int_27h();
		break;
	case 0x28:
		Sleep(10);
		break;
	case 0x29:
		msdos_int_29h();
		break;
	case 0x2e:
		msdos_int_2eh();
		break;
	case 0x2f:
		// multiplex interrupt
		m_CF = 0;
		switch(REG8(AH)) {
		case 0x16: msdos_int_2fh_16h(); break;
		case 0x1a: msdos_int_2fh_1ah(); break;
		case 0x43: msdos_int_2fh_43h(); break;
		case 0x4a: msdos_int_2fh_4ah(); break;
		case 0x4f: msdos_int_2fh_4fh(); break;
		case 0xae: msdos_int_2fh_aeh(); break;
		case 0xb7: msdos_int_2fh_b7h(); break;
		default:
//			fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
			REG16(AX) = 0x01; // ???
			m_CF = 1;
			break;
		}
		if(m_CF) {
			error_code = REG16(AX);
		}
		break;
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
		// EOI
		if((pic[1].isr &= ~(1 << (num - 0x70))) == 0) {
			pic[0].isr &= ~(1 << 2); // master
		}
		pic_update();
		break;
	default:
//		fatalerror("int %02xh (ax=%04xh bx=%04xh cx=%04xh dx=%04x)\n", num, REG16(AX), REG16(BX), REG16(CX), REG16(DX));
		break;
	}
	
	// update cursor position
	if(cursor_moved) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hStdout, &csbi);
		mem[0x450 + mem[0x462] * 2] = csbi.dwCursorPosition.X;
		mem[0x451 + mem[0x462] * 2] = csbi.dwCursorPosition.Y;
		cursor_moved = false;
	}
}

// init

int msdos_init(int argc, char *argv[], char *envp[], int standard_env)
{
	// init file handler
	memset(file_handler, 0, sizeof(file_handler));
	msdos_file_handler_open(0, "STDIN", _isatty(0), 0, 0x80d3, 0);
	msdos_file_handler_open(1, "STDOUT", _isatty(1), 1, 0x80d3, 0);
	msdos_file_handler_open(2, "STDERR", _isatty(2), 1, 0x80d3, 0);
#ifdef SUPPORT_AUX_PRN
	if(_open("stdaux.txt", _O_RDWR | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE) == 3) {
		msdos_file_handler_open(3, 0, 2, 0x80c0, 0);
	}
	if(_open("stdprn.txt", _O_WRONLY | _O_BINARY | _O_CREAT | _O_TRUNC, _S_IREAD | _S_IWRITE) == 4) {
		msdos_file_handler_open(4, 0, 1, 0xa8c0, 0);
	}
#endif
	_dup2(0, DUP_STDIN);
	_dup2(1, DUP_STDOUT);
	_dup2(2, DUP_STDERR);
	
	// init process
	memset(process, 0, sizeof(process));
	
	// init dtainfo
	msdos_dta_info_init();
	
	// init memory
	memset(mem, 0, sizeof(mem));
	for(int i = 0; i < 0x100; i++) {
		*(UINT16 *)(mem + 4 * i + 0) = i;
		*(UINT16 *)(mem + 4 * i + 2) = (IRET_TOP >> 4);
	}
	*(UINT16 *)(mem + 4 * 0x22 + 0) = 0xfff0;
	*(UINT16 *)(mem + 4 * 0x22 + 2) = 0xf000;
	memset(mem + IRET_TOP, 0xcf, IRET_SIZE);
	
	// bios data area
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	
	*(UINT8  *)(mem + 0x411) = 0x20;
	*(UINT16 *)(mem + 0x410) = 0x20;
	*(UINT16 *)(mem + 0x413) = MEMORY_END / 1024;
	*(UINT8  *)(mem + 0x449) = 0x03;//0x73;
	*(UINT16 *)(mem + 0x44a) = 80;
	*(UINT16 *)(mem + 0x44c) = 4096;
	*(UINT16 *)(mem + 0x44e) = 0;
	*(UINT8  *)(mem + 0x450) = csbi.dwCursorPosition.X;
	*(UINT8  *)(mem + 0x451) = csbi.dwCursorPosition.Y;
	*(UINT8  *)(mem + 0x460) = 7;
	*(UINT8  *)(mem + 0x461) = 7;
	*(UINT8  *)(mem + 0x462) = 0;
	*(UINT16 *)(mem + 0x463) = 0x3d4;
	*(UINT8  *)(mem + 0x484) = 24;
	*(UINT8  *)(mem + 0x485) = 19;
	*(UINT8  *)(mem + 0x487) = 0;	// is this okay?
	
	// dos info
	dos_info_t *dos_info = (dos_info_t *)(mem + DOS_INFO_TOP);
	dos_info->first_mcb = MEMORY_TOP >> 4;
	dos_info->first_dpb.w.l = 0;
	dos_info->first_dpb.w.h = DPB_TOP >> 4;
	dos_info->first_sft.w.l = 0;
	dos_info->first_sft.w.h = FILE_TABLE_TOP >> 4;
	dos_info->cds.w.l = 0;
	dos_info->cds.w.h = CDS_TOP >> 4;
	dos_info->fcb_table.w.l = 0;
	dos_info->fcb_table.w.h = FCB_TABLE_TOP >> 4;
	dos_info->last_drive = 'Z' - 'A' + 1;
	dos_info->buffers_x = 20;
	dos_info->buffers_y = 0;
	char *env;
	if((env = getenv("LASTDRIVE")) != NULL) {
		if(env[0] >= 'A' && env[0] <= 'Z') {
			dos_info->last_drive = env[0] - 'A' + 1;
		} else if(env[0] >= 'a' && env[0] <= 'z') {
			dos_info->last_drive = env[0] - 'a' + 1;
		}
	}
	if((env = getenv("windir")) != NULL) {
		if(env[0] >= 'A' && env[0] <= 'Z') {
			dos_info->boot_drive = env[0] - 'A' + 1;
		} else if(env[0] >= 'a' && env[0] <= 'z') {
			dos_info->boot_drive = env[0] - 'a' + 1;
		}
	}
#if defined(HAS_I386)
	dos_info->i386_or_later = 1;
#else
	dos_info->i386_or_later = 0;
#endif
	dos_info->ext_mem_size = (MAX_MEM - 0x100000) >> 10;
	
	// environment
	int seg = MEMORY_TOP >> 4;
	msdos_mcb_create(seg++, 'M', -1, ENV_SIZE >> 4);
	int env_seg = seg;
	int ofs = 0;
	char env_path[4096];
	
	for(char **p = envp; p != NULL && *p != NULL; p++) {
		if(_strnicmp(*p, "PATH=", 5) == 0) {
			strcpy(env_path, *p + 5);
			break;
		}
	}
	for(char **p = envp; p != NULL && *p != NULL; p++) {
		// lower to upper
		char tmp[ENV_SIZE], name[ENV_SIZE], value[ENV_SIZE];
		int value_pos = 0;
		strcpy(tmp, *p);
		for(int i = 0;; i++) {
			if(tmp[i] == '=') {
				tmp[i] = '\0';
				sprintf(name, ";%s;", tmp);
				strcpy(value, tmp + (value_pos = i + 1));
				tmp[i] = '=';
				break;
			} else if(tmp[i] >= 'a' && tmp[i] <= 'z') {
				tmp[i] = tmp[i] - 'a' + 'A';
			}
		}
		if(!(standard_env && strstr(";COMSPEC;INCLUDE;LIB;PATH;PROMPT;TEMP;TMP;TZ;", name) == NULL)) {
			if(strncmp(tmp, "COMSPEC=", 8) == 0) {
				char *path = msdos_search_command_com(argv[0], env_path), tmp_path[MAX_PATH];
				if(path == NULL) {
					path = msdos_remove_double_quote(tmp + 8);
				}
				GetShortPathName(path, tmp_path, MAX_PATH);
				my_strupr(tmp_path);
				sprintf(tmp, "COMSPEC=%s", tmp_path);
			} else if(strncmp(tmp, "PATH=", 5) == 0 || strncmp(tmp, "TEMP=", 5) == 0 || strncmp(tmp, "TMP=", 4) == 0) {
				tmp[value_pos] = '\0';
				char *token = my_strtok(value, ";");
				while(token != NULL) {
					if(strlen(token) != 0) {
						char *path = msdos_remove_double_quote(token), tmp_path[MAX_PATH];
						if(strlen(path) != 0) {
							GetShortPathName(path, tmp_path, MAX_PATH);
							strcat(tmp, tmp_path);
							strcat(tmp, ";");
						}
					}
					token = my_strtok(NULL, ";");
				}
				tmp[strlen(tmp) - 1] = '\0';
				my_strupr(tmp);
			}
			int len = strlen(tmp);
			if (ofs + len + 1 + (2 + (8 + 1 + 3)) + 2 > ENV_SIZE) {
				fatalerror("too many environments\n");
			}
			memcpy(mem + (seg << 4) + ofs, tmp, len);
			ofs += len + 1;
		}
	}
	seg += (ENV_SIZE >> 4);
	
	// psp
	msdos_mcb_create(seg++, 'M', -1, PSP_SIZE >> 4);
	current_psp = seg;
	psp_t *psp = msdos_psp_create(seg, seg + (PSP_SIZE >> 4), -1, env_seg);
	seg += (PSP_SIZE >> 4);
	
	// first mcb in conventional memory
	msdos_mcb_create(seg, 'Z', 0, (MEMORY_END >> 4) - seg - 1);
	first_mcb = seg;
	
	// first mcb in upper memory block
	msdos_mcb_create(UMB_TOP >> 4, 'Z', 0, (UMB_END >> 4) - (UMB_TOP >> 4) - 1);
	
	// boot
	mem[0xffff0] = 0xf4;	// halt
	mem[0xffff1] = 0xcd;	// int 21h
	mem[0xffff2] = 0x21;
	mem[0xffff3] = 0xcb;	// retf
	
	// param block
	// + 0: param block (22bytes)
	// +24: fcb1/2 (20bytes)
	// +44: command tail (128bytes)
	param_block_t *param = (param_block_t *)(mem + WORK_TOP);
	param->env_seg = 0;
	param->cmd_line.w.l = 44;
	param->cmd_line.w.h = (WORK_TOP >> 4);
	param->fcb1.w.l = 24;
	param->fcb1.w.h = (WORK_TOP >> 4);
	param->fcb2.w.l = 24;
	param->fcb2.w.h = (WORK_TOP >> 4);
	
	memset(mem + WORK_TOP + 24, 0x20, 20);
	
	cmd_line_t *cmd_line = (cmd_line_t *)(mem + WORK_TOP + 44);
	if(argc > 1) {
		sprintf(cmd_line->cmd, " %s", argv[1]);
		for(int i = 2; i < argc; i++) {
			char tmp[128];
			sprintf(tmp, "%s %s", cmd_line->cmd, argv[i]);
			strcpy(cmd_line->cmd, tmp);
		}
		cmd_line->len = (UINT8)strlen(cmd_line->cmd);
	} else {
		cmd_line->len = 0;
	}
	cmd_line->cmd[cmd_line->len] = 0x0d;
	
	// system file table
	*(UINT16 *)(mem + FILE_TABLE_TOP +  0) = 6;
	*(UINT16 *)(mem + FILE_TABLE_TOP +  2) = FILE_TABLE_TOP >> 4;
	*(UINT16 *)(mem + FILE_TABLE_TOP +  4) = 100;
	*(UINT32 *)(mem + FILE_TABLE_TOP +  6) = 0xffffffff;
	*(UINT16 *)(mem + FILE_TABLE_TOP + 10) = 100;
	
	// current directory structure
	msdos_cds_update(_getdrive() - 1);
	
	// fcb table
	*(UINT32 *)(mem + FCB_TABLE_TOP + 0) = 0xffffffff;
	*(UINT16 *)(mem + FCB_TABLE_TOP + 4) = 100;
	
	// dbcs table
	msdos_dbcs_table_init();
	
	// execute command
	if(msdos_process_exec(argv[0], param, 0)) {
		fatalerror("'%s' not found\n", argv[0]);
	}
	retval = 0;
	return(0);
}

#define remove_std_file(path) { \
	int fd = _open(path, _O_RDONLY | _O_BINARY); \
	if(fd != -1) { \
		_lseek(fd, 0, SEEK_END); \
		int size = _tell(fd); \
		_close(fd); \
		if(size == 0) { \
			remove(path); \
		} \
	} \
}

void msdos_finish()
{
	for(int i = 0; i < MAX_FILES; i++) {
		if(file_handler[i].valid) {
			_close(i);
		}
	}
#ifdef SUPPORT_AUX_PRN
	remove_std_file("stdaux.txt");
	remove_std_file("stdprn.txt");
#endif
	msdos_dbcs_table_finish();
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
#define CMOS_BCD(v) ((cmos[0x0b] & 4) ? (v) : to_bcd(v))

UINT8 cmos_read(int addr)
{
	static SYSTEMTIME time;
	static UINT32 prev_sec = 0;
	
	switch(addr & 0x7f) {
	case 0x00: CMOS_GET_TIME(); return(CMOS_BCD(time.wSecond));
	case 0x02: CMOS_GET_TIME(); return(CMOS_BCD(time.wMinute));
	case 0x04: CMOS_GET_TIME(); return(CMOS_BCD(time.wHour));
	case 0x06: CMOS_GET_TIME(); return(time.wDayOfWeek + 1);
	case 0x07: CMOS_GET_TIME(); return(CMOS_BCD(time.wDay));
	case 0x08: CMOS_GET_TIME(); return(CMOS_BCD(time.wMonth));
	case 0x09: CMOS_GET_TIME(); return(CMOS_BCD(time.wYear));
//	case 0x0a: return((cmos[0x0a] & 0x7f) | ((timeGetTime() % 1000) < 2 ? 0x80 : 0));	// 2msec
	case 0x0a: return((cmos[0x0a] & 0x7f) | ((timeGetTime() % 1000) < 20 ? 0x80 : 0));	// precision of timeGetTime() may not be 1msec
	case 0x15: return((MEMORY_END >> 10) & 0xff);
	case 0x16: return((MEMORY_END >> 18) & 0xff);
	case 0x17: return(((MAX_MEM - 0x100000) >> 10) & 0xff);
	case 0x18: return(((MAX_MEM - 0x100000) >> 18) & 0xff);
	case 0x30: return(((MAX_MEM - 0x100000) >> 10) & 0xff);
	case 0x31: return(((MAX_MEM - 0x100000) >> 18) & 0xff);
	case 0x32: CMOS_GET_TIME(); return(CMOS_BCD(time.wYear / 100));
	}
	return(cmos[addr & 0x7f]);
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
	case 0x71:
		return(cmos_read(cmos_addr));
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
