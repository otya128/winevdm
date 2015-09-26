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

// i/o bus

UINT8 read_io_byte(offs_t addr)
{
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
		context->Edi = REG16(DI);
		context->Ebp = REG16(BP);
		context->Eip = m_eip;
		context->SegEs = SREG(ES);
		context->SegCs = SREG(CS);
		context->SegSs = SREG(SS);
		context->SegDs = SREG(DS);
		context->EFlags = m_eflags | 0x20000;
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
		REG16(BP) = (WORD)context->Ebp;
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
		m_eip = context->Eip;
		i386_jmp_far(SREG(CS), context->Eip);
		m_eflags = context->EFlags;
	}
	void __wine_call_int_handler(CONTEXT *context, BYTE intnum);
	void WINAPI DOSVM_Int21Handler(CONTEXT *context);
	unsigned char iret[100] = { 0xcf };
	WORD SELECTOR_AllocBlock(const void *base, DWORD size, unsigned char flags);
	__declspec(dllexport) BOOL init_vm86()
	{
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
		return TRUE;
	}
	void WINAPI InitTask16(CONTEXT*);
	BOOL initflag;
	void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void)
		);
	//__declspec(dllexport) void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler);void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler,
	void wine_call_to_16_regs_vm86(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void)
		)
	{
		if (!initflag)
			initflag = init_vm86();
		vm86main(context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret);
	}	
	void wine_call_to_16_vm86(DWORD target, DWORD cbArgs, PEXCEPTION_RECORD handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point, unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void)
		)
	{
		if (!initflag)
			initflag = init_vm86();
		CONTEXT context;
		save_context(&context);
		//why??
		context.SegSs = ((size_t)getWOW32Reserved() >> 16) & 0xFFFF;
		context.Esp = ((size_t)getWOW32Reserved()) & 0xFFFF;
		vm86main(&context, cbArgs, handler, from16_reg, __wine_call_from_16, relay_call_from_16, __wine_call_to_16_ret);
		i386_jmp_far(target >> 16, target & 0xFFFF);
	}
	void vm86main(CONTEXT *context, DWORD cbArgs, PEXCEPTION_RECORD handler,
		void(*from16_reg)(void),
		LONG(*__wine_call_from_16)(void),
		int(*relay_call_from_16)(void *entry_point,	unsigned char *args16, CONTEXT *context),
		void(*__wine_call_to_16_ret)(void)
		)
	{
		if (!initflag)
			initflag = init_vm86();
		//wine_ldt_copy.base[0] = iret;
		REG16(AX) = (WORD)context->Eax;
		REG16(CX) = (WORD)context->Ecx;
		REG16(DX) = (WORD)context->Edx;
		REG16(BX) = (WORD)context->Ebx;
		REG16(SP) = (WORD)context->Esp - cbArgs;
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
		DWORD ret_addr = 0;
		if (cbArgs >= 2)
		{
			unsigned char *stack = (unsigned char*)i386_translate(SS, REG16(SP), 0);
			ret_addr = *(DWORD*)stack;
		}
		//dasm = true;
		while (!m_halted) {
			bool reg = false;
			if (m_pc == (UINT)/*ptr!*/iret)
			{
				CONTEXT context;
				save_context(&context);
				__wine_call_int_handler(&context, 0x21);
				load_context(&context);
			}
			if ((m_eip & 0xFFFF) == (ret_addr & 0xFFFF) && SREG(CS) == ret_addr >> 16)
			{
				__wine_call_to_16_ret();
				break;//return VM
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
				if ((void*)relay != relay_call_from_16)
				{
					//SNOOP???
					stack = stack1;
					ip = *(DWORD*)stack;
					stack += sizeof(DWORD);
					cs = *(DWORD*)stack;//4
					stack += sizeof(DWORD);
					relay = *(DWORD*)stack;//8
					stack += sizeof(DWORD);
					WORD ax = *(WORD*)stack;//10
					stack += sizeof(WORD);
					DWORD eax = *(DWORD*)stack;//14
					stack += sizeof(DWORD);
					bp = *(WORD*)stack;//16
					stack += sizeof(WORD);
					ip19 = *(WORD*)stack;
					stack += sizeof(WORD);
					cs16 = *(WORD*)stack;
					stack += sizeof(WORD);
					args = (WORD*)stack;

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
					int fret = ((int(WINAPI*)(void *, unsigned char *, CONTEXT *))relay)((void*)entry, (unsigned char*)args, &context);
					//int fret = relay_call_from_16((void*)entry, (unsigned char*)args, &context);
					if (!reg)
					{
						REG16(AX) = fret & 0xFFFF;
						REG16(DX) = (fret >> 16) & 0xFFFF;
					}
					if (reg) REG16(AX) = (WORD)context.Eax;
					REG16(CX) = (WORD)context.Ecx;
					if (reg) REG16(DX) = (WORD)context.Edx;
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
					if (entryf != InitTask16)//??????????????????????//(!reg)
						REG16(SP) += 2;
					REG16(BP) = bp;
					i386_load_segment_descriptor(ES);
					i386_load_segment_descriptor(SS);
					i386_load_segment_descriptor(DS);
					i386_load_segment_descriptor(FS);
					i386_load_segment_descriptor(GS);
					m_eip = context.Eip;
					i386_jmp_far(SREG(CS), context.Eip);
					i386_jmp_far(cs16, ip19);
					m_eflags = context.EFlags;
				}
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
					DWORD ooo = (WORD)context.Esp;
					int fret = ((int(*)(void *, unsigned char *, CONTEXT *))relay)((void*)entry, (unsigned char*)args, &context);
					//int fret = relay_call_from_16((void*)entry, (unsigned char*)args, &context);
					if (!reg)
					{
						//REG16(AX) = fret & 0xFFFF;
						//REG16(DX) = (fret >> 16) & 0xFFFF;
						//shld‚³‚ê‚é
						REG32(EAX) = fret;
					}
					if (reg) REG16(AX) = (WORD)context.Eax;
					REG16(CX) = (WORD)context.Ecx;
					if (reg) REG16(DX) = (WORD)context.Edx;
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
					REG16(SP) = osp + 18 + 2;
					//if (IsInitTask)//??????????????????????//(!reg)
					//	REG16(SP) += 2;
					REG16(SP) -= (ooo - context.Esp);
					REG16(BP) = bp;
					i386_load_segment_descriptor(ES);
					i386_load_segment_descriptor(SS);
					i386_load_segment_descriptor(DS);
					i386_load_segment_descriptor(FS);
					i386_load_segment_descriptor(GS);
					m_eip = context.Eip;
					i386_jmp_far(SREG(CS), context.Eip);
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

				fprintf(stderr, "%04x:%04x", SREG(CS), (unsigned)eip);
				fflush(stderr);
#if defined(HAS_I386)
				if (m_operand_size) {
					CPU_DISASSEMBLE_CALL(x86_32);
				}
				else
#endif
					CPU_DISASSEMBLE_CALL(x86_16);
				fprintf(stderr, "\t%s\n", buffer);
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
