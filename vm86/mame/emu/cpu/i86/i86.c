/****************************************************************************
*             real mode i286 emulator v1.4 by Fabrice Frances               *
*               (initial work based on David Hedley's pcemu)                *
****************************************************************************/
/* 26.March 2000 PeT changed set_irq_line */

//#include "emu.h"
//#include "debugger.h"

//#include "host.h"
#include "i86priv.h"
//#include "i86.h"

extern int i386_dasm_one(char *buffer, UINT32 eip, const UINT8 *oprom, int mode);

#define VERBOSE 0
#define LOG(x) do { if (VERBOSE) mame_printf_debug x; } while (0)


/* All pre-i286 CPUs have a 1MB address space */
#define AMASK   0xfffff
#define AMASK   UINT_MAX


/* I86 registers */
union i8086basicregs
{                                      /* eight general registers */
	UINT16 w[8];                       /* viewed as 16 bits registers */
	UINT8 b[16];                       /* or as 8 bit registers */
};

//struct i8086_state
//{
	i8086basicregs m_regs;
	UINT32 m_pc;
	UINT32 m_prevpc;
	UINT32 m_base[4];
	UINT16 m_sregs[4];
	UINT16 m_flags;
	INT32 m_AuxVal, m_OverVal, m_SignVal, m_ZeroVal, m_CarryVal, m_DirVal;      /* 0 or non-0 valued flags */
	UINT8 m_ParityVal;
	UINT8 m_TF, m_IF;                  /* 0 or 1 valued flags */
	UINT8 m_MF;                          /* V30 mode flag */
	UINT8 m_int_vector;
	INT8 m_nmi_state;
	INT8 m_irq_state;
	INT8 m_test_state;

	int m_halted;         /* Is the CPU halted ? */

	UINT16 m_ip;
	UINT32 m_sp;

	char m_seg_prefix;                   /* prefix segment indicator */
	UINT8   m_prefix_seg;                 /* The prefixed segment */
	unsigned m_ea;
	UINT16 m_eo; /* HJB 12/13/98 effective offset of the address (before segment is added) */
	UINT8 m_ea_seg;   /* effective segment of the address */
//};


//#include "i86time.c"

/***************************************************************************/
/* cpu state                                                               */
/***************************************************************************/


//static struct i80x86_timing timing;

static UINT8 parity_table[256];

/* The interrupt number of a pending external interrupt pending NMI is 2.   */
/* For INTR interrupts, the level is caught on the bus during an INTA cycle */

#define PREFIX(name) i8086##name
#define PREFIX86(name) i8086##name

#define I8086
#include "instr86.h"
#include "ea.h"
#include "modrm.h"
#include "table86.h"

#include "instr86.c"
#undef I8086


/***************************************************************************/
static CPU_INIT( i8086 )
{
	unsigned int i, j, c;
	static const BREGS reg_name[8] = {AL, CL, DL, BL, AH, CH, DH, BH};
	for (i = 0; i < 256; i++)
	{
		for (j = i, c = 0; j > 0; j >>= 1)
			if (j & 1)
				c++;

		parity_table[i] = !(c & 1);
	}

	for (i = 0; i < 256; i++)
	{
		Mod_RM.reg.b[i] = reg_name[(i & 0x38) >> 3];
		Mod_RM.reg.w[i] = (WREGS) ((i & 0x38) >> 3);
	}

	for (i = 0xc0; i < 0x100; i++)
	{
		Mod_RM.RM.w[i] = (WREGS) (i & 7);
		Mod_RM.RM.b[i] = (BREGS) reg_name[i & 7];
	}
}

static CPU_INIT( i8088 )
{
	CPU_INIT_CALL(i8086);
}

static CPU_INIT( i80186 )
{
	CPU_INIT_CALL(i8086);
}

static CPU_RESET( i8086 )
{
	memset(&m_regs, 0, sizeof(m_regs));
	memset(m_base, 0, sizeof(m_base));
	memset(m_sregs, 0, sizeof(m_sregs));
	m_flags = 0;
	m_int_vector = 0;
	m_nmi_state = 0;
	m_irq_state = 0;
	m_test_state = 0;

	m_halted = 0;

	m_ip = 0;
	m_sp = 0;

	m_seg_prefix = 0;
	m_prefix_seg = 0;
	m_ea = 0;
	m_eo = 0;
	m_ea_seg = 0;

	m_sregs[CS] = 0xffff;
	m_base[CS] = SegBase(CS);
	m_pc = 0xffff0 & AMASK;
	ExpandFlags(m_flags);

	m_halted = 0;
}

static CPU_RESET( i8088 )
{
	CPU_INIT_CALL(i8086);
}

static CPU_RESET( i80186 )
{
	CPU_INIT_CALL(i8086);
}

/* ASG 971222 -- added these interface functions */

static void set_irq_line(int irqline, int state)
{
	if (state != CLEAR_LINE && m_halted)
	{
		m_halted = 0;
	}

	if (irqline == INPUT_LINE_NMI)
	{
		if (m_nmi_state == state)
			return;
		m_nmi_state = state;

		/* on a rising edge, signal the NMI */
		if (state != CLEAR_LINE)
		{
			PREFIX(_interrupt)(I8086_NMI_INT_VECTOR);
		}
	}
	else
	{
		m_irq_state = state;

		/* if the IF is set, signal an interrupt */
		if (state != CLEAR_LINE && m_IF)
		{
			PREFIX(_interrupt)((UINT32)-1);
			m_irq_state = CLEAR_LINE;
		}
	}
}

static void set_drq_line(int irqline, int state)
{
	// TODO implement me
}

static void set_tmrin_line(int irqline, int state)
{
	// TODO implement me
}

/* PJB 03/05 */
static void set_test_line(int state)
{
	m_test_state = !state;
}

static CPU_EXECUTE( i8086 )
{
	if (m_halted)
	{
		return;
	}

	m_seg_prefix = FALSE;
	m_prevpc = m_pc;
	TABLE86;
}


static CPU_DISASSEMBLE( i8086 )
{
	return i386_dasm_one(buffer, m_pc, oprom, 1);
}



//#include "i86.h"

#undef PREFIX
#define PREFIX(name) i80186##name
#define PREFIX186(name) i80186##name

#define I80186
#include "instr186.h"
#include "table186.h"

#include "instr86.c"
#include "instr186.c"
#undef I80186

static CPU_EXECUTE( i80186 )
{
	if (m_halted)
	{
		return;
	}

	m_seg_prefix = FALSE;
	m_prevpc = m_pc;
	TABLE186;
}

