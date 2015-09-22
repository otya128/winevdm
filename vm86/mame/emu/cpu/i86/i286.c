/****************************************************************************
*             real mode i286 emulator v1.4 by Fabrice Frances               *
*               (initial work based on David Hedley's pcemu)                *
****************************************************************************/

//#include "emu.h"
//#include "debugger.h"
//#include "host.h"


#define VERBOSE 0
#define LOG(x) do { if (VERBOSE) mame_printf_debug x; } while (0)

/* All post-i286 CPUs have a 16MB address space */
#define AMASK   m_amask


#define INPUT_LINE_A20      1

//#include "i286.h"
#define TRAP(fault, code)  (UINT32)(((fault&0xffff)<<16)|(code&0xffff))

//#include "i86time.c"

/***************************************************************************/
/* cpu state                                                               */
/***************************************************************************/
/* I86 registers */
union i80286basicregs
{                   /* eight general registers */
	UINT16 w[8];    /* viewed as 16 bits registers */
	UINT8  b[16];   /* or as 8 bit registers */
};

//struct i80286_state
//{
	i80286basicregs m_regs;
	UINT32  m_amask;          /* address mask */
	UINT32  m_pc;
	UINT32  m_prevpc;
	UINT16  m_flags;
	UINT16  m_msw;
	UINT32  m_base[4];
	UINT16  m_sregs[4];
	UINT16  m_limit[4];
	UINT8 m_rights[4];
	bool m_valid[4];
	struct {
		UINT32 base;
		UINT16 limit;
	} m_gdtr, m_idtr;
	struct {
		UINT16 sel;
		UINT32 base;
		UINT16 limit;
		UINT8 rights;
	} m_ldtr, m_tr;
	INT32   m_AuxVal, m_OverVal, m_SignVal, m_ZeroVal, m_CarryVal, m_DirVal; /* 0 or non-0 valued flags */
	UINT8   m_ParityVal;
	UINT8   m_TF, m_IF;     /* 0 or 1 valued flags */
	INT8    m_nmi_state;
	INT8    m_irq_state;
	INT8    m_test_state;

	int m_halted;         /* Is the CPU halted ? */
	int m_trap_level;

	char m_seg_prefix;
	UINT8   m_prefix_seg;
	unsigned m_ea;
	UINT16 m_eo; /* HJB 12/13/98 effective offset of the address (before segment is added) */
	UINT8 m_ea_seg;   /* effective segment of the address */
//};

#define INT_IRQ 0x01
#define NMI_IRQ 0x02

static UINT8 parity_table[256];

//static struct i80x86_timing timing;

/***************************************************************************/

#define I80286
#include "i86priv.h"
#define PREFIX(fname) i80286##fname
#define PREFIX86(fname) i80286##fname
#define PREFIX186(fname) i80286##fname
#define PREFIX286(fname) i80286##fname
#define i8086_state i80286_state

#include "ea.h"
#include "modrm286.h"
#include "instr86.h"
#include "instr186.h"
#include "instr286.h"
#include "table286.h"
#include "instr86.c"
#include "instr186.c"
#include "instr286.c"

static void i80286_urinit(void)
{
	unsigned int i,j,c;
	static const BREGS reg_name[8]={ AL, CL, DL, BL, AH, CH, DH, BH };

	for (i = 0;i < 256; i++)
	{
		for (j = i, c = 0; j > 0; j >>= 1)
			if (j & 1) c++;

		parity_table[i] = !(c & 1);
	}

	for (i = 0; i < 256; i++)
	{
		Mod_RM.reg.b[i] = reg_name[(i & 0x38) >> 3];
		Mod_RM.reg.w[i] = (WREGS) ( (i & 0x38) >> 3) ;
	}

	for (i = 0xc0; i < 0x100; i++)
	{
		Mod_RM.RM.w[i] = (WREGS)( i & 7 );
		Mod_RM.RM.b[i] = (BREGS)reg_name[i & 7];
	}
}

static void i80286_set_a20_line(int state)
{
	m_amask = state ? 0x00ffffff : 0x000fffff;
}

static CPU_RESET( i80286 )
{
	memset(&m_regs, 0, sizeof(i80286basicregs));
	m_sregs[CS] = 0xf000;
	m_base[CS] = 0xff0000;
	m_pc = 0xfffff0;
	m_limit[CS]=m_limit[SS]=m_limit[DS]=m_limit[ES]=0xffff;
	m_sregs[DS]=m_sregs[SS]=m_sregs[ES]=0;
	m_base[DS]=m_base[SS]=m_base[ES]=0;
	m_rights[DS]=m_rights[SS]=m_rights[ES]=0x93;
	m_rights[CS]=0x9a;
	m_valid[CS]=m_valid[SS]=m_valid[DS]=m_valid[ES]=1;
	m_msw=0xfff0;
	m_flags=2;
	ExpandFlags(m_flags);
	m_idtr.base=0;m_idtr.limit=0x3ff;
	m_gdtr.base=m_ldtr.base=m_tr.base=0;
	m_gdtr.limit=m_ldtr.limit=m_tr.limit=0;
	m_ldtr.rights=m_tr.rights=0;
	m_ldtr.sel=m_tr.sel=0;
	m_seg_prefix = FALSE;

	CHANGE_PC(m_pc);

	m_halted = 0;
}

/****************************************************************************/

/* ASG 971222 -- added these interface functions */

static void set_irq_line(int irqline, int state)
{
	if (state != CLEAR_LINE && m_halted)
	{
		m_halted = 0;
	}
	try
	{
		if (irqline == INPUT_LINE_NMI)
		{
			if (m_nmi_state == state)
				return;
			m_nmi_state = state;

			/* on a rising edge, signal the NMI */
			if (state != CLEAR_LINE)
				i80286_interrupt_descriptor(I8086_NMI_INT_VECTOR, 2, -1);
		}
		else
		{
			m_irq_state = state;

			/* if the IF is set, signal an interrupt */
			if (state != CLEAR_LINE && m_IF)
			{
				i80286_interrupt_descriptor(pic_ack(), 2, -1);
				m_irq_state = CLEAR_LINE;
			}

		}
	}
	catch (UINT32 e)
	{
		i80286_trap2(e);
	}
}

static CPU_EXECUTE( i80286 )
{
//	if (m_halted)
//	{
//		return;
//	}

	/* run until we're out */
//	while(m_icount>0)
//	{
		m_seg_prefix=FALSE;
		try
		{
			if (PM && ((m_pc-m_base[CS]) > m_limit[CS]))
				throw TRAP(GENERAL_PROTECTION_FAULT, m_sregs[CS] & ~3);
			m_prevpc = m_pc;

			TABLE286 // call instruction
		}
		catch (UINT32 e)
		{
			i80286_trap2(e);
		}
//	}
}

extern int i386_dasm_one(char *buffer, UINT32 eip, const UINT8 *oprom, int mode);

static CPU_DISASSEMBLE( i80286 )
{
	return i386_dasm_one(buffer, m_pc, oprom, 2);
}

static CPU_INIT( i80286 )
{
	/* If a reset parameter is given, take it as pointer to an address mask */
	m_amask = 0x00ffff;

	i80286_urinit();
}

#undef I80286
