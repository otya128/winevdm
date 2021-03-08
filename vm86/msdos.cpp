/*
	MS-DOS Player for Win32 console

	Author : Takeda.Toshiya
	Date   : 2009.11.09-
*/
int ignore_illegal_insn;
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

//#ifdef _DEBUG
void logerror(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);
	vfprintf(stderr, format, arg);
	va_end(arg);
}
//#else
//#define logerror(...)
//#endif
//#define logerror(...) fprintf(stderr, __VA_ARGS__)
//#define logerror(...)
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
#if 0// defined(HAS_I386)
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
#if 0// defined(HAS_I386)
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
#if 0// defined(HAS_I386)
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
	#include "mame/lib/softfloat/fsincos.c"
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
}

void hardware_finish()
{
#if defined(HAS_I386)
	vtlb_free(m_vtlb);
#endif
}


int pic_ack()
{
	return 0;
}

typedef DWORD (*DOSVM_inport_t)(int port, int size);
typedef void (*DOSVM_outport_t)(int port, int size, DWORD value);
DOSVM_inport_t DOSVM_inport;
DOSVM_outport_t DOSVM_outport;

// i/o bus
UINT8 read_io_byte(offs_t addr)
{
    return (UINT8)DOSVM_inport(addr, 1);
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
    DOSVM_outport(addr, 1, val);
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
#undef min
#undef max

static HMODULE krnl386 = 0;

extern "C"
{
#define EXCEPTION_PROTECTED_MODE       0x80020100
    //kenel16_private.h
#include "wine/exception.h"
#include "../krnl386/kernel16_private.h"
#include <wownt32.h>
#define KRNL386 "krnl386.exe16"
	PVOID dynamic_setWOW32Reserved(PVOID w)
	{
		static PVOID(*setWOW32Reserved)(PVOID);
		if (!setWOW32Reserved)
		{
			if (!krnl386)
				krnl386 = LoadLibraryA(KRNL386);
			setWOW32Reserved = (PVOID(*)(PVOID))GetProcAddress(krnl386, "setWOW32Reserved");
		}
		return setWOW32Reserved(w);
	}
	PVOID dynamic_getWOW32Reserved()
	{
		static PVOID(*getWOW32Reserved)();
		if (!getWOW32Reserved)
		{
			if (!krnl386)
				krnl386 = LoadLibraryA(KRNL386);
			getWOW32Reserved = (PVOID(*)())GetProcAddress(krnl386, "getWOW32Reserved");
		}
		return getWOW32Reserved();
	}
    WINE_VM86_TEB_INFO *dynamic_getGdiTebBatch()
    {
        static WINE_VM86_TEB_INFO*(*getGdiTebBatch)();
        if (!getGdiTebBatch)
        {
            if (!krnl386)
                krnl386 = LoadLibraryA(KRNL386);
            getGdiTebBatch = (WINE_VM86_TEB_INFO*(*)())GetProcAddress(krnl386, "getGdiTebBatch");
        }
        return getGdiTebBatch();
    }
	void dynamic__wine_call_int_handler(CONTEXT *context, BYTE intnum)
	{
		static void(*__wine_call_int_handler)(CONTEXT *context, BYTE intnum);
		if (!__wine_call_int_handler)
		{
			if (!krnl386)
				krnl386 = LoadLibraryA(KRNL386);
			__wine_call_int_handler = (void(*)(CONTEXT *context, BYTE intnum))GetProcAddress(krnl386, "__wine_call_int_handler");
		}
		__wine_call_int_handler(context, intnum);
	}
    __declspec(dllimport) LDT_ENTRY wine_ldt[8192];
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
    WORD reg_fs = 0;
    WORD reg_gs = 0;
    WORD reg_cs = 0;
    WORD reg_ss = 0;
    WORD reg_ds = 0;
    WORD reg_es = 0;
	void save_context(CONTEXT *context)
	{
		context->Eax = REG32(EAX);
		context->Ecx = REG32(ECX);
		context->Edx = REG32(EDX);
		context->Ebx = REG32(EBX);
		context->Esp = REG32(ESP);
		context->Ebp = REG32(EBP);
		context->Esi = REG32(ESI);
		context->Edi = REG32(EDI);
		context->Ebp = REG32(EBP);
		context->Eip = m_eip;
		context->SegEs = SREG(ES);
		context->SegCs = SREG(CS);
		context->SegSs = SREG(SS);
		context->SegDs = SREG(DS);
		context->SegFs = SREG(FS);
		context->SegGs = SREG(GS);
        context->EFlags = get_flags();
        if (!V8086_MODE)
            dynamic_setWOW32Reserved((PVOID)(SREG(SS) << 16 | REG16(SP)));
	}
	void load_context(CONTEXT *context)
	{
		REG32(EAX) = (DWORD)context->Eax;
		REG32(ECX) = (DWORD)context->Ecx;
		REG32(EDX) = (DWORD)context->Edx;
		REG32(EBX) = (DWORD)context->Ebx;
		REG32(ESP) = (DWORD)context->Esp;
		REG32(EBP) = (DWORD)context->Ebp;
		REG32(ESI) = (DWORD)context->Esi;
		REG32(EDI) = (DWORD)context->Edi;
		REG32(EBP) = (DWORD)context->Ebp;
		SREG(ES) = (WORD)context->SegEs;
		SREG(CS) = (WORD)context->SegCs;
		SREG(SS) = (WORD)context->SegSs;
		SREG(DS) = (WORD)context->SegDs;//ES, CS, SS, DS
		//ES, CS, SS, DS, FS, GS
        /* Some programs expect that gs is not a valid selector! */
        /* Some programs expect that fs is not a valid selector! */
		SREG(FS) = (WORD)context->SegFs == reg_fs ? 0 : context->SegFs;
		SREG(GS) = (WORD)context->SegGs == reg_gs ? 0 : context->SegGs;
		i386_load_segment_descriptor(ES);
		i386_load_segment_descriptor(SS);
		i386_load_segment_descriptor(DS);
		i386_load_segment_descriptor(FS);
		i386_load_segment_descriptor(GS);
		i386_jmp_far(SREG(CS), context->Eip);
        m_eip = context->Eip;
		set_flags(context->EFlags);
	}
	unsigned char table[256 * 4 + 2 + 0x8 * 256] = { 0xcf };
	unsigned char iret[256] = { 0xcf };
	WORD SELECTOR_AllocBlock(const void *base, DWORD size, unsigned char flags);
#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")
	const size_t stack_frame_size = 100;
	thread_local void* current_stack_frame[100];
	thread_local size_t current_stack_frame_size;
	void capture_stack_trace()
	{
		auto process = GetCurrentProcess();
		current_stack_frame_size = CaptureStackBackTrace(0, stack_frame_size, current_stack_frame, NULL);
	}
    static DWORD wow_calllback_retaddr;
    static void print_16bit_stack(DWORD ret_addr, DWORD old_ebp)
    {
        auto ret_cs = SELECTOROF(ret_addr);
        auto ret_ip = OFFSETOF(ret_addr);
        typedef void(*vm_debug_get_entry_point_t)(char *module, char *func, WORD *ordinal, STACK16FRAME *frame);
        static vm_debug_get_entry_point_t vm_debug_get_entry_point;
        if (!vm_debug_get_entry_point)
        {
            vm_debug_get_entry_point = (vm_debug_get_entry_point_t)GetProcAddress(LoadLibraryA(KRNL386), "vm_debug_get_entry_point");
        }
        if (ret_addr == wow_calllback_retaddr)
        {
            fprintf(stderr, "(callback)cs:ip=%04x:%04x bp=%04x\n", ret_cs, ret_ip, old_ebp);
        }
        else
        {
            auto call_instr = read_byte((DWORD)wine_ldt_copy.base[ret_cs >> 3] + ret_ip - 5);
            auto call_addr = read_dword((DWORD)wine_ldt_copy.base[ret_cs >> 3] + ret_ip - 4);
            fprintf(stderr, "cs:ip=%04x:%04x bp=%04x", ret_cs, ret_ip, old_ebp);
            if (call_instr == 0x9a) /* call far imm */
            {
                fprintf(stderr, "(call %04x:%04x)", SELECTOROF(call_addr), OFFSETOF(call_addr));
                if (wine_ldt[SELECTOROF(call_addr) >> __AHSHIFT].HighWord.Bits.Default_Big)
                {
                    char module[100], func[100];
                    WORD ordinal = 0;
                    STACK16FRAME frame = { 0 };

                    frame.module_cs = SELECTOROF(call_addr);
                    frame.entry_ip = OFFSETOF(call_addr) + 7;
                    vm_debug_get_entry_point(module, func, &ordinal, &frame);
                    fprintf(stderr, "\n%s.%d: %s", module, ordinal, func);
                }
            }
            else
            {
                fprintf(stderr, "                ");
            }
            fprintf(stderr, " args(");
            BOOL farframe = old_ebp & 1;
            old_ebp &= ~1;
            for (int i = 0; i < 10; i++)
            {
                fprintf(stderr, i ? ",%04x" : "%04x", read_word(SREG_BASE(SS) + old_ebp + (farframe ? 6 : 4) + i * 2));
            }
            fprintf(stderr, ")\n");
        }
    }
    void walk_16bit_stack(void)
    {
        if (V8086_MODE)
            return;
        __TRY
        {
            auto ebp = REG32(EBP);
            auto last_cs = SREG(CS);
            print_16bit_stack(MAKESEGPTR(last_cs, m_eip), ebp);
            for (int i = 0; i < 60; i++)
            {
                auto old_ebp = read_word(SREG_BASE(SS) + ebp);
                DWORD ret_addr = read_dword(SREG_BASE(SS) + ebp + 2);
                if ((old_ebp & 1) || ((wine_ldt_copy.flags[SELECTOROF(ret_addr) >> 3] & WINE_LDT_FLAGS_TYPE_MASK) == WINE_LDT_FLAGS_CODE))
                {
                    old_ebp |= 1;
                    last_cs = SELECTOROF(ret_addr);
                }
                else
                    ret_addr = MAKESEGPTR(last_cs, OFFSETOF(ret_addr));
                print_16bit_stack(ret_addr, old_ebp);
                if (old_ebp == ebp)
                {
                    break;
                }
                ebp = old_ebp & ~1;
            }
        }
        __EXCEPT_PAGE_FAULT
        {
        }
        __ENDTRY
    }
	void dump_stack_trace(void)
	{
		unsigned int   i;
		SYMBOL_INFO  * symbol;
		HANDLE         process;

		process = GetCurrentProcess();

		symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
		symbol->MaxNameLen = 255;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		for (i = 0; i < current_stack_frame_size; i++)
		{
			IMAGEHLP_LINE line = {};
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE);
			DWORD d = 0;
			auto success = SymGetLineFromAddr(process, (DWORD64)current_stack_frame[i], &d, &line);
			SymFromAddr(process, (DWORD64)(current_stack_frame[i]), 0, symbol);
            IMAGEHLP_MODULE mod_info = { 0 };
            mod_info.SizeOfStruct = sizeof(mod_info);
            const char *imgname = "";
            if (SymGetModuleInfo(process, (ULONG_PTR)current_stack_frame[i], &mod_info))
            {
                imgname = strrchr(mod_info.ImageName, '\\');
                if (!imgname)
                    imgname = mod_info.ImageName;
                else
                    imgname = imgname + 1;
            }
            fprintf(stderr, "%i: %s!%s+0x%llx - 0x%llx 0x%lx %s:%d\n", current_stack_frame_size - i - 1, imgname, symbol->Name, (ULONG64)current_stack_frame[i] - symbol->Address, symbol->Address, current_stack_frame[i], line.FileName, line.LineNumber);
			if (!strcmp(symbol->Name, "KiUserExceptionDispatcher"))
			{
				fprintf(stderr, "\n");
			}
		}

		free(symbol);
		current_stack_frame_size = 0;
	}
    void dump_all_modules(void)
    {
        DWORD cb;
        HMODULE *hModules = nullptr;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, GetCurrentProcessId());
        if (!EnumProcessModules(hProcess, nullptr, 0, &cb))
            goto exit;
        hModules = new HMODULE[cb / sizeof(HMODULE)];
        if (!EnumProcessModules(hProcess, hModules, cb, &cb))
            goto exit;
        for (int i = 0; i < cb / sizeof(HMODULE); i++)
        {
            WCHAR name[MAX_PATH];
            MODULEINFO module_info;
            if (GetModuleFileNameExW(hProcess, hModules[i], name, ARRAY_SIZE(name)) && GetModuleInformation(hProcess, hModules[i], &module_info, sizeof(module_info)))
            {
                LPWSTR n = wcsrchr(name, L'\\');
                fwprintf(stderr, L"%p-%p %ls\n", hModules[i], (ULONG_PTR)hModules[i] + module_info.SizeOfImage, n ? n + 1 : name);
            }
        }
    exit:
        CloseHandle(hProcess);
        delete[] hModules;
    }
    BOOL WINAPI dump(DWORD CtrlType)
    {
        static BOOL dump;
        DWORD pid = GetCurrentProcessId();
        DWORD tid = GetCurrentThreadId();
        HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
        THREADENTRY32 te32;
        if (dump)
        {
            ExitProcess(0);
        }
        dump = TRUE;
        hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hThreadSnap == INVALID_HANDLE_VALUE)
            return false;
        FILE *out = fdopen(dup(fileno(stdout)), "w");
        fprintf(out, "dump\n");
        te32.dwSize = sizeof(THREADENTRY32);
        if (!Thread32First(hThreadSnap, &te32))
        {
            CloseHandle(hThreadSnap);
            return false;
        }
        do
        {
            if (te32.th32OwnerProcessID == pid)
            {
                if (tid != te32.th32ThreadID)
                {
                    HANDLE threads = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                    SuspendThread(threads);
                    CloseHandle(threads);
                }
            }
        } while (Thread32Next(hThreadSnap, &te32));
        CloseHandle(hThreadSnap);
        /* prevent deadlock */
        FILE *err = fdopen(dup(fileno(stderr)), "w");

        __TRY
        {
            char buffer[256];
#if defined(HAS_I386)
            UINT64 eip = m_eip;
#else
            UINT64 eip = m_pc - SREG_BASE(CS);
#endif
            UINT8 *oprom = mem + SREG_BASE(CS) + eip;

            int result;
            for (int i = 0; i < 8192; i++)
            {
                LDT_ENTRY entry = wine_ldt[i];
                DWORD base = entry.BaseLow | (entry.HighWord.Bytes.BaseMid << 16);
                DWORD limit = wine_ldt_get_limit(&entry);
                if (!base)
                    continue;
                if ((entry.HighWord.Bytes.Flags1 & 0x0018) == 0x0018)
                {

                }
                else if ((entry.HighWord.Bytes.Flags1 & 0x0010) != 0)
                    continue;
                fprintf(err, "%04X %p-%p\n", i << 3 | 7, (void*)base, (void*)limit);
                __TRY
                {
                    for (int j = 0; j < limit; )
                    {
                        m_eip = j;
                        m_sreg[CS].selector = i << 3 | 7;
                        i386_load_segment_descriptor(CS);
                        m_operand_size = m_sreg[CS].d;
                        m_xmm_operand_size = 0;
                        m_address_size = m_sreg[CS].d;
                        m_operand_prefix = 0;
                        m_address_prefix = 0;
                        m_operand_size = m_address_size = 0;
                        i386_jmp_far(i << 3 | 7, j); eip = m_eip; oprom = mem + SREG_BASE(CS) + eip;
                        /* call */
                        if (*oprom == 0x9A &&!m_sreg[CS].d)
                        {
                            WORD addr = *(LPWORD)(oprom + 1);
                            WORD seg = *(LPWORD)(oprom + 3);
                            LPLDT_ENTRY entry = wine_ldt + (seg >> 3);
                            LPBYTE base = (LPBYTE)wine_ldt_get_base(entry);
                            LPBYTE relay = base + addr;
                            relay += 2; /* push bp */
                            relay += 1; /* push func */
                            LPVOID func = (LPVOID)relay;
                            relay += 4;
                            STACK16FRAME frame = { 0 };
                            frame.module_cs = seg;
                            frame.entry_ip = addr + 7;
                            STACK16FRAME *framep = (STACK16FRAME*)(SREG_BASE(SS) + REG32(ESP));
                            *framep = frame;
                            dynamic_setWOW32Reserved((LPVOID)(SREG(SS) << 16 | REG16(SP)));
                            {
                                typedef void(*vm_debug_get_entry_point_t)(char *module, char *func, WORD *ordinal, STACK16FRAME *frame);
                                static vm_debug_get_entry_point_t vm_debug_get_entry_point;
                                if (!vm_debug_get_entry_point)
                                {
                                    vm_debug_get_entry_point = (vm_debug_get_entry_point_t)GetProcAddress(LoadLibraryA(KRNL386), "vm_debug_get_entry_point");
                                }
                                char module[100], func[100];
                                WORD ordinal = 0;
                                vm_debug_get_entry_point(module, func, &ordinal, NULL);
                                fprintf(err, "call built-in func %p %s.%d: %s\n", entry, module, ordinal, func);
                            }
                        }
                        result = i386_dasm_one_ex(buffer, eip, oprom, 16);// CPU_DISASSEMBLE_CALL(x86_32);//i386_dasm_one_ex(buffer, 0, (UINT8*)(base + j), 16);
                        fprintf(err, "%04X:%04X %s\n", i << 3 | 7, j, buffer);
                        j += result & 0xFF;
                    }
                }
                __EXCEPT_ALL
                {

                }
                __ENDTRY
            }
        }
        __EXCEPT_ALL
        {

        }
        __ENDTRY
        return FALSE;

    }
    static BOOL is_error(PEXCEPTION_RECORD rec)
    {
        return rec->ExceptionCode == EXCEPTION_PROTECTED_MODE || rec->ExceptionCode == EXCEPTION_ACCESS_VIOLATION;
    }
    LONG NTAPI vm86_vectored_exception_handler(struct _EXCEPTION_POINTERS *ExceptionInfo)
    {
        if (is_error(ExceptionInfo->ExceptionRecord))
            capture_stack_trace();
        return EXCEPTION_CONTINUE_SEARCH;
    }
    typedef WORD(*get_native_wndproc_segment_t)();
    typedef DWORD(*call_native_wndproc_context_t)(CONTEXT *context);
    get_native_wndproc_segment_t get_native_wndproc_segment;
    call_native_wndproc_context_t call_native_wndproc_context;
    WORD native_wndproc_segment;
    LPCWSTR(WINAPI*MB_GetString)(int);
    HANDLE inject_event;
    CRITICAL_SECTION inject_crit_section;
    typedef VOID (WINAPI *GetpWin16Lock_t)(SYSLEVEL **lock);
    GetpWin16Lock_t pGetpWin16Lock;
    SYSLEVEL *win16_syslevel;
    HANDLE *vm_idle_event;
    typedef BOOL(WINAPI *WOWCallback16Ex_t)(DWORD vpfn16, DWORD dwFlags,
        DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
    WOWCallback16Ex_t pWOWCallback16Ex;
    static WORD tss[0x68 + 65536 / 8] = { 0 };
    typedef BOOL (WINAPI *vm_inject_t)(DWORD vpfn16, DWORD dwFlags,
        DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
    BOOL WINAPI vm_inject(DWORD vpfn16, DWORD dwFlags,
        DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode);
	__declspec(dllexport) BOOL init_vm86(BOOL is_vm86)
	{
        SymInitialize(GetCurrentProcess(), NULL, TRUE);
        HMODULE user32 = GetModuleHandleW(L"user32.dll");
        MB_GetString = (LPCWSTR(WINAPI*)(int))GetProcAddress(user32, "MB_GetString");
        HMODULE user = LoadLibraryW(L"user.exe16");
        if (user)
        {
            call_native_wndproc_context = (call_native_wndproc_context_t)GetProcAddress(user, "call_native_wndproc_context");
            get_native_wndproc_segment = (get_native_wndproc_segment_t)GetProcAddress(user, "get_native_wndproc_segment");
            native_wndproc_segment = get_native_wndproc_segment();
        }
        if (!krnl386)
            krnl386 = LoadLibraryA(KRNL386);
        DOSVM_inport = (DOSVM_inport_t)GetProcAddress(krnl386, "DOSVM_inport");
        DOSVM_outport = (DOSVM_outport_t)GetProcAddress(krnl386, "DOSVM_outport");
        void(WINAPI *set_vm_inject_cb)(vm_inject_t) = (void(WINAPI *)(vm_inject_t))GetProcAddress(krnl386, "set_vm_inject_cb");
        set_vm_inject_cb(vm_inject);
        inject_event = CreateEventW(NULL, TRUE, FALSE, NULL);
        InitializeCriticalSection(&inject_crit_section);
        pGetpWin16Lock = (GetpWin16Lock_t)GetProcAddress(krnl386, "GetpWin16Lock");
        pGetpWin16Lock(&win16_syslevel);
        pWOWCallback16Ex = (WOWCallback16Ex_t)GetProcAddress(krnl386, "K32WOWCallback16Ex");
        HANDLE *(WINAPI *get_idle_event)() = (HANDLE *(WINAPI *)())GetProcAddress(krnl386, "get_idle_event");
        vm_idle_event = get_idle_event();
        //SetConsoleCtrlHandler(dump, TRUE);
		AddVectoredExceptionHandler(TRUE, vm86_vectored_exception_handler);
		WORD sel = SELECTOR_AllocBlock(iret, 256, WINE_LDT_FLAGS_CODE);
		CPU_INIT_CALL(CPU_MODEL);
		CPU_RESET_CALL(CPU_MODEL);
		m_cr[0] |= 0x20;  //we don't support irq13 for fpe
        UINT8 *base = 0;//mem;
		m_idtr.base = (UINT32)(table - base);
        m_ldtr.limit = 65535;
        m_ldtr.base = (UINT32)((UINT8*)&wine_ldt - base);
        m_gdtr.limit = 65535;
        m_gdtr.base = (UINT32)((UINT8*)&wine_ldt - base);
        m_CPL = 3;
        wine_ldt[4].HighWord.Bits.Type = 0x18;
        wine_ldt[4].HighWord.Bits.Pres = 1;
        wine_ldt[4].HighWord.Bits.Sys = 1;
        wine_ldt[4].LimitLow = 0xFFFF;
        wine_ldt[4].HighWord.Bits.LimitHi = 0xF;
        wine_ldt[4].HighWord.Bits.Default_Big = 1;
        wine_ldt[4].HighWord.Bits.Granularity = 1;
        wine_ldt[4].HighWord.Bits.Dpl = m_CPL;
		memset(iret, 0xcf, 256);
		for (int i = 0; i < 256; i++)
		{
			*(WORD*)&table[i * 4 + 2] = sel;
			*(WORD*)&table[i * 4] = i;
			*(WORD*)&table[i * 4 + 2] = sel;
			*(WORD*)&table[i * 4] = i;
		}
        //PROTECTED MODE
        for (int i = 0; i < 256; i++)
        {
            *(WORD*)&table[i * 8] = i;
            *(WORD*)&table[i * 8 + 2] = sel;
            *(BYTE*)&table[i * 8 + 4] = 0x00;
            *(BYTE*)&table[i * 8 + 5] = 0x66 | 0x80;
            *(WORD*)&table[i * 8 + 6] = 0;
        }
#ifdef _MSC_VER
        __asm
        {
            mov reg_fs, fs;
            mov reg_gs, gs;
            mov reg_cs, cs;
            mov reg_ss, ss;
            mov reg_ds, ds;
            mov reg_es, es;
        }
#else
        reg_fs = wine_get_fs();
        reg_gs = wine_get_gs();
        reg_cs = wine_get_cs();
        reg_ss = wine_get_ss();
        reg_ds = wine_get_ds();
        reg_es = wine_get_es();
#endif

        wine_ldt[reg_gs >> 3].HighWord.Bits.Type = 0x18;
        wine_ldt[reg_gs >> 3].HighWord.Bits.Pres = 1;
        wine_ldt[reg_gs >> 3].HighWord.Bits.Sys = 1;
        wine_ldt[reg_gs >> 3].LimitLow = 0xFFFF;
        wine_ldt[reg_gs >> 3].HighWord.Bits.LimitHi = 0xF;
        wine_ldt[reg_gs >> 3].HighWord.Bits.Default_Big = 1;
        wine_ldt[reg_gs >> 3].HighWord.Bits.Granularity = 1;
        wine_ldt[reg_gs >> 3].HighWord.Bits.Dpl = m_CPL;

        wine_ldt[reg_fs >> 3].HighWord.Bits.Type = 0x18;
        wine_ldt[reg_fs >> 3].HighWord.Bits.Pres = 1;
        wine_ldt[reg_fs >> 3].HighWord.Bits.Sys = 1;
        wine_ldt[reg_fs >> 3].LimitLow = 0xFFFF;
        wine_ldt[reg_fs >> 3].HighWord.Bits.LimitHi = 0xF;
        wine_ldt[reg_fs >> 3].HighWord.Bits.Default_Big = 1;
        wine_ldt[reg_fs >> 3].HighWord.Bits.Granularity = 1;
        wine_ldt[reg_fs >> 3].HighWord.Bits.Dpl = m_CPL;


        wine_ldt[reg_cs >> 3].HighWord.Bits.Type = 0x18;
        wine_ldt[reg_cs >> 3].HighWord.Bits.Pres = 1;
        wine_ldt[reg_cs >> 3].HighWord.Bits.Sys = 1;
        wine_ldt[reg_cs >> 3].LimitLow = 0xFFFF;
        wine_ldt[reg_cs >> 3].HighWord.Bits.LimitHi = 0xFFFF;
        wine_ldt[reg_cs >> 3].HighWord.Bits.Default_Big = 1;
        wine_ldt[reg_cs >> 3].HighWord.Bits.Granularity = 1;
        wine_ldt[reg_cs >> 3].HighWord.Bits.Dpl = m_CPL;


        wine_ldt[reg_ds >> 3].HighWord.Bits.Type = 0x18;
        wine_ldt[reg_ds >> 3].HighWord.Bits.Pres = 1;
        wine_ldt[reg_ds >> 3].HighWord.Bits.Sys = 1;
        wine_ldt[reg_ds >> 3].LimitLow = 0xFFFF;
        wine_ldt[reg_ds >> 3].HighWord.Bits.LimitHi = 0xFFFF;
        wine_ldt[reg_ds >> 3].HighWord.Bits.Default_Big = 1;
        wine_ldt[reg_ds >> 3].HighWord.Bits.Granularity = 1;
        wine_ldt[reg_ds >> 3].HighWord.Bits.Dpl = m_CPL;
#if defined(HAS_I386)
		cpu_type = (REG32(EDX) >> 8) & 0x0f;
		cpu_step = (REG32(EDX) >> 0) & 0x0f;
#endif
		//m_amask = -1;
		m_a20_mask = -1;
        if (is_vm86)
        {
            m_cr[0] |= 1;//PROTECTED MODE
            set_flags(0x20000);//V8086_MODE
           //assert(V8086_MODE);
        }
        else
        {
            m_cr[0] |= 1;//PROTECTED MODE
        }
        /* task state segment */
        m_task.flags = 9;
        m_task.base = (UINT32)tss;
        m_task.limit = sizeof(tss);
        *(WORD*)((char*)tss + 0x66) = sizeof(tss) - 65536 / 8;
        memset((char*)tss + sizeof(tss) - 65536 / 8, 0x00, 65536 / 8);
        return TRUE;
	}
    DWORD mergeReg(DWORD a1, DWORD a2)
    {
        return a1 == a2 ? a1 : a2;
    }
	BOOL initflag;
	void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void),
        int dasm,
        UINT8 *memory_base,
        pm_interrupt_handler pih
		);
	//__declspec(dllexport) void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler);void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler,
	void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void),
        int dasm,
        bool vm86,
        void *memory_base, 
        pm_interrupt_handler pih
		)
	{
        mem = vm86 ? (UINT8*)memory_base : NULL;
		if (!initflag)
			initflag = init_vm86(vm86);
		vm86main(context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, mem, pih);
	}	
	DWORD wine_call_to_16_vm86(DWORD target, DWORD cbArgs, PEXCEPTION_HANDLER handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void),
        int dasm,
        bool vm86,
        void *memory_base,
        pm_interrupt_handler pih
		)
	{
        mem = vm86 ? (UINT8*)memory_base : NULL;
		if (!initflag)
			initflag = init_vm86(vm86);
		CONTEXT context;
        PVOID oldstack = dynamic_getWOW32Reserved();
		save_context(&context);
		//why??
		dynamic_setWOW32Reserved(oldstack);
		context.SegSs = ((size_t)dynamic_getWOW32Reserved() >> 16) & 0xFFFF;
		context.Esp = ((size_t)dynamic_getWOW32Reserved()) & 0xFFFF;
		context.SegCs = target >> 16;
		context.Eip = target & 0xFFFF;//i386_jmp_far(target >> 16, target & 0xFFFF);
		vm86main(&context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, mem, pih);
		return (context.Eax & 0xffff) | context.Edx << 16;
	}
	UINT old_eip = 0;
    BOOL has_x86_exception_err(WORD num)
    {
        switch (num)
        {
        case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:
            return FALSE;
        case 8:
            return TRUE;
        case 9:
            return FALSE;
        case 10:case 11:case 12:case 13:case 14:
            return TRUE;
        case 15:case 16:
            return FALSE;
        case 17:
            return TRUE;
        case 18:case 19:case 20:case 21:case 22:case 23:case 24:case 25:case 26:case 27:case 28:case 29:
            return FALSE;
        case 30:
            return TRUE;
        case 31:
            return FALSE;
        }
        return FALSE;
    }

    static int disassemble(char *buffer)
    {
        __TRY
        {
#if defined(HAS_I386)
            UINT64 eip = m_eip;
#else
            UINT64 eip = m_pc - SREG_BASE(CS);
#endif
            UINT8 *oprom = mem + SREG_BASE(CS) + eip;

            m_operand_size = m_sreg[CS].d;
#if defined(HAS_I386)
            if (m_sreg[CS].d)
            {
                return CPU_DISASSEMBLE_CALL(x86_32) & 0xff;
            }
            else
#endif
            {
                return i386_dasm_one_ex(buffer, m_eip, oprom, 16) & 0xff;//CPU_DISASSEMBLE_CALL(x86_16);
            }
        }
        __EXCEPT_PAGE_FAULT
        {
            *buffer = 0;
            return 0;
        }
        __ENDTRY
    }


    __declspec(dllexport) int disassemble_debug(char *buffer, UINT8 *oprom, BOOL op_size, UINT64 eip)
    {
        int old_m_operand_size = m_operand_size;
        int old_m_eip = m_eip;
        int r = 0;
        __TRY
        {
            m_operand_size = op_size;
            m_eip = eip;
#if defined(HAS_I386)
            if (op_size)
            {
                r = CPU_DISASSEMBLE_CALL(x86_32) & 0xff;
            }
            else
#endif
            {
                r = i386_dasm_one_ex(buffer, m_eip, oprom, 16) & 0xff;//CPU_DISASSEMBLE_CALL(x86_16);
            }
        }
        __EXCEPT_PAGE_FAULT
        {
            *buffer = 0;
            r = 0;
        }
        __ENDTRY
        m_eip = old_m_eip;
        m_operand_size = old_m_operand_size;
        return r;
    }

    void protected_mode_exception_handler(WORD num, const char *name, pm_interrupt_handler pih)
    {
        WORD err = has_x86_exception_err(num) ? POP16() : num;
        WORD ip = POP16();
        WORD cs = POP16();
        WORD flags = POP16();
        m_eip = ip;
        m_sreg[CS].selector = cs;
        i386_load_segment_descriptor(CS);
        if (num == FAULT_GP)
        {
            char buffer[256];
            int len = disassemble(buffer);
            if(err == 0x40)
            {
                // many startups access the BDA directly
                static WORD dosmem_0040H = 0;
                if (!dosmem_0040H)
                {
                    DWORD(WINAPI *GetProcAddress16)(HMODULE16, LPCSTR);
                    HMODULE16(WINAPI *GetModuleHandle16)(LPCSTR);
                    if (!krnl386)
                        krnl386 = LoadLibraryA(KRNL386);
                    GetProcAddress16 = (DWORD(WINAPI *)(HMODULE16, LPCSTR))GetProcAddress(krnl386, "GetProcAddress16");
                    GetModuleHandle16 = (HMODULE16(WINAPI *)(LPCSTR))GetProcAddress(krnl386, "GetModuleHandle16");
                    dosmem_0040H = (WORD)GetProcAddress16(GetModuleHandle16("KERNEL"), (LPCSTR)193);
                    (void(*)(void))GetProcAddress(krnl386, "DOSVM_start_bios_timer")();
                }
                /* allocate segment 40h */
                LPLDT_ENTRY entry = wine_ldt + (dosmem_0040H >> __AHSHIFT);
                wine_ldt[0x40 >> __AHSHIFT] = *entry;
                wine_ldt_copy.base[0x40 >> __AHSHIFT] = wine_ldt_get_base(entry);
                wine_ldt_copy.limit[0x40 >> __AHSHIFT] = wine_ldt_get_limit(entry);
                wine_ldt_copy.flags[0x40 >> __AHSHIFT] = (entry->HighWord.Bits.Type |
                    (entry->HighWord.Bits.Default_Big ? WINE_LDT_FLAGS_32BIT : 0) |
                    (wine_ldt_copy.flags[0x40 >> __AHSHIFT] & WINE_LDT_FLAGS_ALLOCATED));
                i386_jmp_far(cs, ip);
                return;
            }
            HMODULE toolhelp = GetModuleHandleA("toolhelp.dll16");
            if (toolhelp)
            {
                SEGPTR stack = MAKESEGPTR(SREG(SS), REG16(SP));
                FARPROC16 intcb = ((FARPROC16(WINAPI *)(SEGPTR *, SEGPTR, WORD, WORD, WORD))GetProcAddress(toolhelp, "get_intcb"))(&stack, MAKESEGPTR(cs, ip), flags, num, REG16(AX));
                if (intcb)
                {
                    SREG(SS) = SELECTOROF(stack);
                    REG16(SP) = OFFSETOF(stack);
                    i386_jmp_far(SELECTOROF(intcb), OFFSETOF(intcb));
                    return;
                }
            }
        }
        DWORD ret = pih(num, MAKESEGPTR(cs, ip));
        if (ret)
        {
            //TODO:arguments?
            i386_jmp_far((ret >> 16) & 0xFFFF, ret & 0xFFFF);
            return;
        }
        PSTR dasm = (PSTR)calloc(10000, 1);
        ULONG_PTR arguments[7] = { (ULONG_PTR)num, (ULONG_PTR)name, (ULONG_PTR)err, (ULONG_PTR)ip, (ULONG_PTR)cs, (ULONG_PTR)flags, (ULONG_PTR)dasm };

        //jump to next instr
        char buffer[256];
        int opsize = disassemble(buffer);
        memcpy(dasm, buffer, sizeof(buffer));
        if ((int)ip + opsize <= 0xFFFF)
        {
            ip += opsize;
            m_eip = ip;
            m_sreg[CS].selector = cs;
            i386_load_segment_descriptor(CS);
        }
        set_flags(flags);
        RaiseException(EXCEPTION_PROTECTED_MODE, 0, sizeof(arguments) / sizeof(ULONG_PTR), arguments);
    }
    volatile struct
    {
        BOOL inject;
        DWORD vpfn16;
        DWORD dwFlags;
        DWORD cbArgs;
        LPVOID pArgs;
        LPDWORD pdwRetCode;
    } vm_inject_state;
    BOOL WINAPI vm_inject(DWORD vpfn16, DWORD dwFlags,
        DWORD cbArgs, LPVOID pArgs, LPDWORD pdwRetCode)
    {
        assert(dwFlags == WCB16_PASCAL);
        LPVOID args = HeapAlloc(GetProcessHeap(), 0, cbArgs);
        memcpy(args, pArgs, cbArgs);
try_again:
        if (TryEnterCriticalSection(&win16_syslevel->crst))
        {
            vm_inject_state.inject = FALSE;
            /* There are no threads running VM. (e.g. call GetMessage) */
            EnterCriticalSection(&inject_crit_section);
            /* 16-bit stack is allocated by thread_attach(krnl386/kernel.c) */
            BOOL result = pWOWCallback16Ex(vpfn16, dwFlags, cbArgs, args, pdwRetCode);
            LeaveCriticalSection(&inject_crit_section);
            LeaveCriticalSection(&win16_syslevel->crst);
            HeapFree(GetProcessHeap(), 0, args);
            return result;
        }
        EnterCriticalSection(&inject_crit_section);
        {
            if (vm_inject_state.inject)
            {
                /* FIXME: multiple interrupts */
                LeaveCriticalSection(&inject_crit_section);
                return FALSE;
            }
            vm_inject_state.vpfn16 = vpfn16;
            vm_inject_state.dwFlags = dwFlags;
            vm_inject_state.cbArgs = cbArgs;
            vm_inject_state.pArgs = args;
            vm_inject_state.pdwRetCode = pdwRetCode;
            vm_inject_state.inject = TRUE;
            ResetEvent(inject_event);
        }
        LeaveCriticalSection(&inject_crit_section);
        HANDLE objs[2] = { inject_event, vm_idle_event };
        DWORD ret = WaitForMultipleObjects(2, objs, FALSE, INFINITE);
        if (ret == (WAIT_OBJECT_0 + 1))
        {
            SetEvent(inject_event);
            goto try_again;
        }
        return TRUE;
    }
    void vm_inject_call(SEGPTR ret_addr, PEXCEPTION_HANDLER handler,
        void(*from16_reg)(void),
        LONG(*__wine_call_from_16)(void),
        int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
        void(*__wine_call_to_16_ret)(void),
        int dasm,
        pm_interrupt_handler pih)
    {
        CONTEXT context;
        DWORD ret;
        static char intstack[4096];
        static WORD intstksel = 0;
        if (!intstksel)
            intstksel = SELECTOR_AllocBlock(intstack, 4096, WINE_LDT_FLAGS_DATA);
                    
        save_context(&context);
        EnterCriticalSection(&inject_crit_section);
        WORD sp = context.Esp;
        DWORD ss_base = m_sreg[SS].base;
        if (m_sreg[SS].d) // don't call int handler on a large stack
        {
            ss_base = (DWORD)intstack;
            sp = 4096;
            dynamic_setWOW32Reserved((PVOID)MAKESEGPTR(intstksel, 4096));
        }
        {
            char *stack = (char *)ss_base + sp - vm_inject_state.cbArgs;
            vm_inject_state.inject = FALSE;
            memcpy(stack, vm_inject_state.pArgs, vm_inject_state.cbArgs);
            /* push return address */
            stack -= sizeof(SEGPTR);
            *((SEGPTR *)stack) = ret_addr;
            vm_inject_state.cbArgs += sizeof(SEGPTR);
        }
        HeapFree(GetProcessHeap(), 0, vm_inject_state.pArgs);
        LeaveCriticalSection(&inject_crit_section);
        ret = wine_call_to_16_vm86(vm_inject_state.vpfn16, vm_inject_state.cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, FALSE, NULL, pih);
        if (vm_inject_state.pdwRetCode)
            *vm_inject_state.pdwRetCode = ret;
        load_context(&context);
        SetEvent(inject_event);
    }
	LONG catch_exception(_EXCEPTION_POINTERS *ep, PEXCEPTION_ROUTINE er);
#if defined(TEMP_DASM)
    BOOL dd;
    __declspec(dllexport) void temp_dasm(BOOL a)
    {
        printf("temp_dasm\n");
        dd = a;
    }
#endif
    void raise_vm86_int(int vec)
    {
        /* raise EXCEPTION_VM86_INTx */
        CONTEXT vcontext = {};
        save_context(&vcontext);
        //dosvm.c exception_handler
        CONTEXT *ppcontext = &vcontext;
        ULONG exception_args[1 + sizeof(CONTEXT*) / sizeof(ULONG)] = { (ULONG)vec };
        *(CONTEXT**)(&exception_args[1]) = ppcontext;
        RaiseException(0x80000110 /* EXCEPTION_VM86_INTx */, 0, sizeof(exception_args) / sizeof(ULONG), (ULONG_PTR*)exception_args);
        load_context(&vcontext);
    }
	void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_HANDLER handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point,	unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void),
		int dasm, 
        UINT8 *memory_base,
        pm_interrupt_handler pih
		)
	{
        DWORD old_frame16 = PtrToUlong(dynamic_getWOW32Reserved());
		__TRY
        {
            m_task.base = (UINT32)tss - (UINT32)memory_base;
			if (!initflag)
				initflag = init_vm86(false);
			REG32(EAX) = (DWORD)context->Eax;
			REG32(ECX) = (DWORD)context->Ecx;
			REG32(EDX) = (DWORD)context->Edx;
			REG32(EBX) = (DWORD)context->Ebx;
			REG32(ESP) = (DWORD)context->Esp - cbArgs;
			REG32(EBP) = (DWORD)context->Ebp;
			REG32(ESI) = (DWORD)context->Esi;
			REG32(EDI) = (DWORD)context->Edi;
			SREG(ES) = (WORD)context->SegEs;
			SREG(CS) = (WORD)context->SegCs;
			SREG(SS) = (WORD)context->SegSs;
			SREG(DS) = wine_ldt_copy.flags[(WORD)context->SegDs >> 3] & WINE_LDT_FLAGS_ALLOCATED ? (WORD)context->SegDs : 0;
            /* Some programs expect that gs is not a valid selector! */
            /* Some programs expect that fs is not a valid selector! */
            SREG(FS) = (WORD)context->SegFs == reg_fs ? 0 : context->SegFs;
            SREG(GS) = (WORD)context->SegGs == reg_gs ? 0 : context->SegGs;
            set_flags(context->EFlags | 0x200/* IF */);
			i386_load_segment_descriptor(ES);
			i386_jmp_far(context->SegCs, context->Eip);
			i386_load_segment_descriptor(SS);
			i386_load_segment_descriptor(DS);
			i386_load_segment_descriptor(FS);
			i386_load_segment_descriptor(GS);
			m_IOP1 = 1;
			m_IOP2 = 1;
			m_eflags |= 0x3000;
            DWORD ret_addr = 0;
            if (cbArgs >= 2)
            {
                unsigned char *stack = (unsigned char*)i386_translate(SS, REG16(SP), 0);
                ret_addr = *(DWORD*)stack;
                if (!wow_calllback_retaddr)
                    wow_calllback_retaddr = ret_addr;
            }
            bool isVM86mode = false;
            while (!m_halted) {
                if (vm_inject_state.inject && m_IF)
                {
                    vm_inject_call(ret_addr, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret, dasm, pih);
                }
				if ((m_eip & 0xFFFF) == (ret_addr & 0xFFFF) && SREG(CS) == ret_addr >> 16)
				{
					break;//return VM
				}
                if (SREG(CS) == native_wndproc_segment)
                {
                    CONTEXT context;
                    save_context(&context);
                    call_native_wndproc_context(&context);
                    load_context(&context);
                }
				bool reg = false;
				if (m_pc >= (UINT)iret && m_pc <= (UINT)iret + 255)
				{
					CONTEXT context;
                    {
                        UINT32 num = m_pc - (UINT32)iret;
                        const char *name = NULL;
                        //win16 handles int 2/4/6/7
                        switch (num)
                        {
                        case 0:name = "#DE"; break;
                        case 2:name = "int 2h"; break;
                        case 4:name = "#OF"; break;
                        case FAULT_UD:name = "#UD"; break;
                        case FAULT_NM:name = "#NM"; break;
                        case FAULT_DF:name = "#DF"; break;
                        case FAULT_TS:name = "#TS"; break;
                        case FAULT_NP:name = "#NP"; break;
                        case FAULT_SS:name = "#SS"; break;
                        case FAULT_GP:name = "#GP"; break;
                        case FAULT_PF:name = "#PF"; break;
                        case FAULT_MF:name = "#MF"; break;
                        }
                        if (name && num != FAULT_MF)
                        {
                            protected_mode_exception_handler(num, name, pih);
                            continue;
                        }
                        WORD return_ip = POP16();
                        WORD return_cs = POP16();
                        WORD flags = POP16();
                        save_context(&context);
                        context.Eip = return_ip;
                        context.SegCs = return_cs;
                        context.EFlags = flags;
                        if ((num == FAULT_MF) && (m_x87_sw & 0x80))
                            num = 2;  // redirect fpu error to nmi
                        dynamic__wine_call_int_handler(&context, num);
                        mem = memory_base;
                        load_context(&context);
                        continue;
                    }
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
					//for debug
					void *entryf = (void*)entry;
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
					if (relay == (UINT)relay_call_from_16 || 1)
					{
						CONTEXT context;
                        STACK32FRAME dummy_stack32 = { 0 };
                        dummy_stack32.retaddr = ret_addr;
                        dummy_stack32.nb_args = cbArgs;
                        dummy_stack32.frame.Handler = handler;
						DWORD osp = REG32(ESP);
						PUSH16(SREG(GS));
						PUSH16(SREG(FS));
						PUSH16(SREG(ES));
						PUSH16(SREG(DS));
						PUSH32(REG32(EBP));
						PUSH32(REG32(ECX));
						PUSH32(REG32(EDX));
						PUSH32((DWORD)&dummy_stack32);
						save_context(&context);
                        STACK16FRAME *oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp);
						DWORD ooo = (WORD)context.Esp;
                        int off = 0;
                        if (reg)
                        {
                            context.Esp = osp + (SIZE_T)stack - (SIZE_T)stack1 - 4;
                            off = ooo - context.Esp;
                            context.Ebp = (context.Ebp & ~0xffff) | bp;
                            context.Eip = ip19;
                            context.SegCs = cs16;
                        }
                        int fret;
                        LPVOID old;
                        PCONTEXT pctx = NULL;
#ifdef _MSC_VER
                        __asm
                        {
                            mov old, esp
                            push cbArgs
                            push old /* target(esp) */
                            push retaddr /* retaddr */
                            push ebp
                            push ebx
                            push esi
                            push edi
                            push old_frame16 /* frame16 */
                            /* set up exception handler */
                            push handler
                            mov  eax, fs:[0]
                            push eax
                            mov dummy_stack32.frame.Next, eax
                            mov  fs:[0], esp
                            push cs
                            push 0
                            mov eax, [oa]
                            mov[eax], esp
                            lea eax, [context]
                            push eax
                            push args
                            push entry
                            call relay_call_from_16
                            add esp, 12 + 8
                            mov fret, eax
                            pop dword ptr fs:[0]
                            pop eax
                            jmp skip
                            retaddr:
                            mov pctx, ecx
                            skip:
                            mov esp, old
                        }
#else
					    fret = relay_call_from_16((void*)entry, (unsigned char*)args, &context);
#endif
                        mem = memory_base;

                        if (pctx)
                        {
                            /* Throw16 */
                            context = *pctx;
                            reg = TRUE;
                        }
						if (!reg)
						{
							REG32(EAX) = fret;
                            context.SegSs = ((size_t)dynamic_getWOW32Reserved() >> 16) & 0xFFFF;
                            context.Esp = ((size_t)dynamic_getWOW32Reserved()) & 0xFFFF;
                            oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp);
						}
                        else
                        {
                            oa = (STACK16FRAME*)wine_ldt_get_ptr(context.SegSs, context.Esp + off);
                        }
						if (reg) REG32(EAX) = (DWORD)context.Eax;
						REG32(ECX) = reg ? (DWORD)context.Ecx : (DWORD)oa->ecx;
						if (reg) REG32(EDX) = (DWORD)context.Edx;
                        else
                            REG32(EDX) = (DWORD)oa->edx;
						REG32(EBX) = (DWORD)context.Ebx;
						REG32(ESP) = (DWORD)context.Esp;
						REG32(EBP) = (DWORD)context.Ebp;
						REG32(ESI) = (DWORD)context.Esi;
						REG32(EDI) = (DWORD)context.Edi;
                        SREG(ES) = reg ? (WORD)context.SegEs : (WORD)oa->es;
						SREG(SS) = (WORD)context.SegSs;
						SREG(DS) = reg ? (WORD)context.SegDs : (WORD)oa->ds;//ES, CS, SS, DS
						//ES, CS, SS, DS, FS, GS
                        /* Some programs expect that gs is not a valid selector! */
                        /* Some programs expect that fs is not a valid selector! */
                        /* win16 sets 0? */
                        SREG(FS) = 0;//(WORD)context.SegFs == reg_fs ? 0 : context.SegFs;
                        SREG(GS) = 0;//(WORD)context.SegGs == reg_gs ? 0 : context.SegGs;
                        if (reg)
                        {
                            if (!(ip19 != context.Eip || cs16 != context.SegCs))
                            {
                                context.Eip = oa->callfrom_ip;
                                context.SegCs = oa->module_cs;
                            }
                            else
                            {
                                /* CS:IP changed! */
                                context.Eip = context.Eip;
                            }
                            REG32(ESP) = context.Esp;
                            REG32(EBP) = context.Ebp;
                        }
                        else
                        {
                            REG16(SP) = osp + 18 + 2;
                            REG16(SP) -= (ooo - context.Esp);
                            WORD bpp = REG16(SP);
                            REG16(SP) = context.Esp + 0x2c;
                            REG16(BP) = bp;
                        }
                        SREG(CS) = (WORD)context.SegCs;
                        set_flags(context.EFlags);
						i386_load_segment_descriptor(ES);
						i386_load_segment_descriptor(SS);
						i386_load_segment_descriptor(DS);
						i386_load_segment_descriptor(FS);
						i386_load_segment_descriptor(GS);
						m_eip = context.Eip;
						i386_jmp_far(SREG(CS), context.Eip);
					}
				}
                //merge_vm86_pending_flags
                if (V8086_MODE)
                {
                    if (dynamic_getGdiTebBatch()->vm86_pending & 0x100000)//VIP flag
                    {
                        //dlls/ntdll/signal_i386.c merge_vm86_pending_flags
                        BOOL check_pending = TRUE;
                        /*
                        * In order to prevent a race when SIGUSR2 occurs while
                        * we are returning from exception handler, pending events
                        * will be rechecked after each raised exception.
                        */
                        while (check_pending && dynamic_getGdiTebBatch()->vm86_pending)
                        {
                            check_pending = FALSE;

                            //x86_thread_data()->vm86_ptr = NULL;

                            /*
                            * If VIF is set, throw exception.
                            * Note that SIGUSR2 may turn VIF flag off so
                            * VIF check must occur only when TEB.vm86_ptr is NULL.
                            */
                            if (1 || m_VIF)
                            {
                                CONTEXT vcontext = {};
                                save_context(&vcontext);

                                EXCEPTION_RECORD reca, *rec = &reca;
                                rec->ExceptionCode = 0x80000111;
                                rec->ExceptionFlags = 0;
                                rec->ExceptionRecord = NULL;
                                rec->NumberParameters = 0;
                                rec->ExceptionAddress = (LPVOID)vcontext.Eip;

                                vcontext.EFlags &= ~0x100000;
                                dynamic_getGdiTebBatch()->vm86_pending = 0;
                                //dosvm.c exception_handler
                                CONTEXT *ppcontext = &vcontext;
                                ULONG exception_args[1 + sizeof(CONTEXT*) / sizeof(ULONG)] = {};
                                *(CONTEXT**)(&exception_args[1]) = ppcontext;
                                RaiseException(0x80000111, 0, sizeof(exception_args) / sizeof(ULONG), (ULONG_PTR*)exception_args);
                                //NTSTATUS a = NtRaiseException(rec, &vcontext, TRUE);

                                load_context(&vcontext);
                                check_pending = TRUE;
                            }

                            //x86_thread_data()->vm86_ptr = vm86;
                        }

                        /*
                        * Merge VIP flags in a signal safe way. This requires
                        * that the following operation compiles into atomic
                        * instruction.
                        */
                        set_flags(get_flags() | dynamic_getGdiTebBatch()->vm86_pending);
                    }
                    UINT8 *op = mem + SREG_BASE(CS) + m_eip;
                    if (*op == 0xCD)//INT imm8
                    {
                        UINT8 vec = *(op + 1);
                        m_eip += 2;
                        raise_vm86_int(vec);
                        continue;
                    }
                    else if (*op == 0xCC)
                    {
                        m_eip += 1;
                        raise_vm86_int(3);
                        continue;
                    }
                    else if (*op == 0xF1)
                    {
                        m_eip += 1;
                        raise_vm86_int(1);
                        continue;
                    }
                }
#if defined(ADDR_TRACE)
                static DWORD oldk = -1;
                static DWORD old_cs;
                static DWORD old_eip;

                WORD k = 0xdead;
                __try
                {
                    k = !wine_ldt_copy.base[SELECTOROF(ADDR_TRACE) >> 3] ? 0xdead : *(LPWORD)((LPBYTE)wine_ldt_copy.base[SELECTOROF(ADDR_TRACE) >> 3] + OFFSETOF(ADDR_TRACE));
                }
                __except (EXCEPTION_EXECUTE_HANDLER)
                {
                }
#endif
#ifdef SUPPORT_DISASSEMBLER
                if (dasm
#if defined(TEMP_DASM)
                    || dd
#endif
#if defined(ADDR_TRACE)
                    || oldk != k
#endif
                    ) {
                    char *dbuf = NULL;
#if defined(ADDR_TRACE)
                    if (oldk != k)
                    {
                        fprintf(stderr, "CHANGED=========\n%04x:%04x %04x\n", SREG(CS), m_eip, k);
                    }
#endif
                    char buffer[256];
#if defined(HAS_I386)
                    UINT64 eip = m_eip;
#else
                    UINT64 eip = m_pc - SREG_BASE(CS);
#endif
                    UINT8 *oprom = mem + SREG_BASE(CS) + eip;
#if defined(ADDR_TRACE)
                    if (oldk != k)
                    {
                        if ((SIZE_T)wine_ldt_copy.base[old_cs >> 3])
                        {
                            eip = old_eip;
                            oprom = mem + (SIZE_T)wine_ldt_copy.base[old_cs >> 3] + eip;
                        }
                    }
                    oldk = k;
#endif
                    fprintf(stderr, "%04x:%04x", SREG(CS), (unsigned)eip);
                    int result;
#if defined(HAS_I386)
                    if (m_sreg[CS].d)
                    {
                        result = CPU_DISASSEMBLE_CALL(x86_32);
                    }
                    else
#endif
                    {
                        result = i386_dasm_one_ex(buffer, eip, oprom, 16);//CPU_DISASSEMBLE_CALL(x86_16);
                    }
                    int opsize = result & 0xFF;
#ifdef DUMP_INSTRMEM
                    fprintf(stderr, "\t");
                    for (int i = 0; i < opsize; i++)
                    {
                        fprintf(stderr, "%02x", oprom[i]);
                    }
#endif
                    fprintf(stderr, "\t%s\n", buffer);
#if !defined(TRACE_REGS)
                        DWORD stkptr = m_sreg[SS].base + (m_sreg[SS].d ? REG32(ESP) : REG16(SP));
                        if (SREG(FS) || SREG(GS))
                        {
                            fprintf(stderr,
                                "\
EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X,\
ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X,\
ES:%04X,CS:%04X,SS:%04X,DS:%04X,FS:%04x,GS:%04x,\
IP:%04X,stack:%08X,\
EFLAGS:%08X\
\n",
REG32(EAX), REG32(ECX), REG32(EDX), REG32(EBX), REG32(ESP), REG32(EBP), REG32(ESI), REG32(EDI),
SREG(ES), SREG(CS), SREG(SS), SREG(DS), SREG(FS), SREG(GS), m_eip, read_dword(stkptr), m_eflags);
                        }
                        else
                        {
                            fprintf(stderr,
                                "\
EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X,\
ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X,\
ES:%04X,CS:%04X,SS:%04X,DS:%04X,\
IP:%04X,stack:%08X,\
EFLAGS:%08X\
\n",
REG32(EAX), REG32(ECX), REG32(EDX), REG32(EBX), REG32(ESP), REG32(EBP), REG32(ESI), REG32(EDI),
SREG(ES), SREG(CS), SREG(SS), SREG(DS), m_eip, read_dword(stkptr), m_eflags);
                        }
#endif
                }
				
#endif
#if defined(ADDR_TRACE)
                old_cs = SREG(CS);
                old_eip = m_eip;
#endif
#if defined(HAS_I386)
				m_cycles = 1;
				CPU_EXECUTE_CALL(i386);
#else
				CPU_EXECUTE_CALL(CPU_MODEL);
#endif
			}
			save_context(context);
		}
		__EXCEPT_CTX (catch_exception, (PEXCEPTION_ROUTINE)handler)
		{
		}
		__ENDTRY
	}

#include <imagehlp.h>

#pragma comment(lib, "imagehlp.lib")

    void print_stack(void)
    {
        unsigned int   i;
        void         * stack[100];
        unsigned short frames;
        SYMBOL_INFO  * symbol;
        HANDLE         process;

        process = GetCurrentProcess();

        SymInitialize(process, NULL, TRUE);

        frames = CaptureStackBackTrace(0, 100, stack, NULL);
        symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        for (i = 0; i < frames; i++)
        {
            SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

            fprintf(stderr, "%i: %s+0x%llx - 0x%p\n", frames - i - 1, symbol->Name, (ULONG64)stack[i] - symbol->Address, (LPVOID)symbol->Address);
        }

        free(symbol);
    }
    static DWORD thread_messagebox(HWND hWnd, LPCSTR text, LPCSTR caption, DWORD flags)
    {
        DWORD threadId;
        //same thread:
        //MessageBox->WndProc->exception->MessageBox->WndProc->...
        const char *a[4];
        a[0] = (const char*)hWnd;
        a[1] = text;
        a[2] = caption;
        a[3] = (const char*)flags;
        auto lam = [](void *lpThreadParameter)
        {
            DWORD result = 0;
            __TRY
            {
                result = (DWORD)MessageBoxA((HWND)((const char**)lpThreadParameter)[0], ((const char**)lpThreadParameter)[1], ((const char**)lpThreadParameter)[2], (DWORD)((const char**)lpThreadParameter)[3]);
            }
            __EXCEPT_ALL
            {
            }
            __ENDTRY
            return result;
        };
        LPTHREAD_START_ROUTINE routine = lam;
        HANDLE hThread = CreateThread(NULL, 0, routine, a, 0, &threadId);
        WaitForSingleObject(hThread, INFINITE);
        DWORD exitcode;
        BOOL success = GetExitCodeThread(hThread, &exitcode);
        CloseHandle(hThread);
        return exitcode;
    }
    LONG catch_exception(_EXCEPTION_POINTERS *ep, PEXCEPTION_ROUTINE winehandler)
    {
        PEXCEPTION_RECORD rec = ep->ExceptionRecord;
        if (rec->ExceptionCode == 0x80000100/*EXCEPTION_WINE_STUB*/)
        {
            char buffer[1024];
            fprintf(stderr, "stub function %s %s\n", (const char*)rec->ExceptionInformation[0], (const char*)rec->ExceptionInformation[1]);
            LPCWSTR retry = MB_GetString(3);
            LPCWSTR cancel = MB_GetString(1);
            wsprintfA(buffer, "stub function %s %s\nPress %S to continue execution.\nPress %S to terminate this task.", rec->ExceptionInformation[0], rec->ExceptionInformation[1], retry, cancel);
            DWORD result = thread_messagebox(NULL, buffer, "fatal error", MB_ICONERROR | MB_RETRYCANCEL);
            if (result == IDCANCEL)
            {
                ExitThread(rec->ExceptionCode);
            }
            return EXCEPTION_EXECUTE_HANDLER;
        }
        if (!is_error(rec))
            return EXCEPTION_CONTINUE_SEARCH;
        static void(*NE_DumpAllModules)();
        if (!NE_DumpAllModules)
        {
            NE_DumpAllModules = ((void(*)())GetProcAddress(LoadLibraryA(KRNL386), "NE_DumpAllModules"));
        }
        if (NE_DumpAllModules)
        {
            __TRY
            {
                fprintf(stderr, "=====dump all modules=====\n");
                NE_DumpAllModules();
                fprintf(stderr, "=====dump all modules=====\n");
            }
            __EXCEPT_ALL
            {

            }
            __ENDTRY
        }
        CONTEXT context;
        save_context(&context);
        char buf_pre[2048];
        buf_pre[0] = '\0';

        if (rec->ExceptionCode == EXCEPTION_PROTECTED_MODE)
        {
            //ULONG_PTR arguments[6] = { (ULONG_PTR)num, (ULONG_PTR)name, (ULONG_PTR)err, (ULONG_PTR)ip, (ULONG_PTR)cs, (ULONG_PTR)flags };
            WORD num = (WORD)rec->ExceptionInformation[0];
            const char *name = (const char*)rec->ExceptionInformation[1];
            WORD err = (WORD)rec->ExceptionInformation[2];
            WORD ip = (WORD)rec->ExceptionInformation[3];
            WORD cs = (WORD)rec->ExceptionInformation[4];
            WORD flags = (WORD)rec->ExceptionInformation[5];
            LPSTR dasm = (LPSTR)rec->ExceptionInformation[6];
            sprintf(buf_pre, "Interrupt %02X %s (%04X:%04X) flags %04X err %04X\n%s\n", num, name, cs, ip, flags, err, dasm);
            free(dasm);
        }
        else
        {
            snprintf(buf_pre, sizeof(buf_pre), "SEGV");
        }
        char buf[2048];
        sprintf(buf,
"address=%p\naccess address=%p\n\
VM context\n\
EAX:%04X,ECX:%04X,EDX:%04X,EBX:%04X\n\
ESP:%04X,EBP:%04X,ESI:%04X,EDI:%04X\n\
ES:%04X,CS:%04X,SS:%04X,DS:%04X,FS:%04X,GS:%04X\n\
IP:%04X, address:%08X\n\
EFLAGS:%08X\
\n\n%s\n", rec->ExceptionAddress, (void*)rec->ExceptionInformation[1],
REG32(EAX), REG32(ECX), REG32(EDX), REG32(EBX), REG32(ESP), REG32(EBP), REG32(ESI), REG32(EDI),
SREG(ES), SREG(CS), SREG(SS), SREG(DS), SREG(FS), SREG(GS), m_eip, m_pc, m_eflags, buf_pre);
        dump_all_modules();
        dump_stack_trace();
        walk_16bit_stack();
        fprintf(stderr, "\n");
#if 0
        print_stack();
#endif
        fprintf(stderr, "%s", buf);
        fflush(stderr);
#if defined(STACK_DUMP)
        __try
        {
            int i = 0;
            for (int y = 0; y < 16; y++)
            {
                fprintf(stderr, "+%02x", i);
                for (int x = 0; x < 16; x++)
                {
                    fprintf(stderr, " %02x", *(LPBYTE)(SREG_BASE(SS) + REG32(ESP) + i));
                    i++;
                }
                fprintf(stderr, "\n");
            }
            i = 0;
            for (int y = 0; y < 16; y++)
            {
                fprintf(stderr, "-%02x", i);
                for (int x = 0; x < 16; x++)
                {
                    fprintf(stderr, " %02x", *(LPBYTE)(SREG_BASE(SS) + REG32(ESP) - i));
                    i++;
                }
                fprintf(stderr, "\n");
            }
            fflush(stderr);
        }
        __EXCEPT_ALL
        {

        }
#endif
        DWORD exitcode = thread_messagebox(NULL, buf, buf_pre, MB_CANCELTRYCONTINUE | MB_ICONERROR);
        if (exitcode == IDCANCEL)
        {
            ExitThread(rec->ExceptionCode);
        }
        if (exitcode == IDRETRY)
        {
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        if (exitcode == IDCONTINUE)
        {
            return EXCEPTION_EXECUTE_HANDLER;
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }
}
