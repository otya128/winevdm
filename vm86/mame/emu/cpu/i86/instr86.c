/****************************************************************************
*             real mode i286 emulator v1.4 by Fabrice Frances               *
*               (initial work based on David Hedley's pcemu)                *
****************************************************************************/

/*
 * file will be included in all cpu variants
 * put non i86 instructions in own files (i286, i386, nec)
 * function renaming will be added when necessary
 * timing value should move to separate array
 */

/*
    PHS - 2010-12-29

    Moved several instruction stubs so that they are compiled separately for
    the 8086 and 80186. The instructions affected are :

    _pop_ss, _es, _cs, _ss, _ds, _mov_sregw and _sti

    This is because they call the next instruction directly as it cannot be
    interrupted. If they are not compiled separately when executing on an
    80186, the wrong set of instructions are used (the 8086 set). This has
    the serious effect of ignoring the next instruction, as invalid, *IF*
    it is an 80186 specific instruction.

*/

#if !defined(I80186)
static void PREFIX86(_interrupt)(unsigned int_num)
{
	unsigned dest_seg, dest_off;
	WORD ip = m_pc - m_base[CS];

	if (int_num == -1)
		int_num = pic_ack();

#ifdef I80286
	if (PM) {
		i80286_interrupt_descriptor(int_num, 0, 0);
	} else {
#endif
		dest_off = ReadWord(int_num*4);
		dest_seg = ReadWord(int_num*4+2);

		PREFIX(_pushf());
		m_TF = m_IF = 0;
		PUSH(m_sregs[CS]);
		PUSH(ip);
		m_sregs[CS] = (WORD)dest_seg;
		m_base[CS] = SegBase(CS);
		m_pc = (m_base[CS] + dest_off) & AMASK;
		CHANGE_PC(m_pc);
#ifdef I80286
	}
#endif
}

static void PREFIX86(_trap)()
{
	PREFIX(_instruction)[FETCHOP]();
	PREFIX(_interrupt)(1);
}
#endif

#ifndef I80186
static void PREFIX86(_rotate_shift_Byte)(unsigned ModRM, unsigned count, unsigned src)
{
//  unsigned src = (unsigned)GetRMByte(ModRM);
	unsigned dst=src;

	if (count==0)
	{
	}
	else if (count==1)
	{
		switch (ModRM & 0x38)
		{
		case 0x00:  /* ROL eb,1 */
			m_CarryVal = src & 0x80;
			dst=(src<<1)+CF;
			PutbackRMByte(ModRM,dst);
			m_OverVal = (src^dst)&0x80;
			break;
		case 0x08:  /* ROR eb,1 */
			m_CarryVal = src & 0x01;
			dst = ((CF<<8)+src) >> 1;
			PutbackRMByte(ModRM,dst);
			m_OverVal = (src^dst)&0x80;
			break;
		case 0x10:  /* RCL eb,1 */
			dst=(src<<1)+CF;
			PutbackRMByte(ModRM,dst);
			SetCFB(dst);
			m_OverVal = (src^dst)&0x80;
			break;
		case 0x18:  /* RCR eb,1 */
			dst = ((CF<<8)+src) >> 1;
			PutbackRMByte(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = (src^dst)&0x80;
			break;
		case 0x20:  /* SHL eb,1 */
		case 0x30:
			dst = src << 1;
			PutbackRMByte(ModRM,dst);
			SetCFB(dst);
			m_OverVal = (src^dst)&0x80;
			m_AuxVal = 1;
			SetSZPF_Byte(dst);
			break;
		case 0x28:  /* SHR eb,1 */
			dst = src >> 1;
			PutbackRMByte(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = src & 0x80;
			m_AuxVal = 1;
			SetSZPF_Byte(dst);
			break;
		case 0x38:  /* SAR eb,1 */
			dst = ((INT8)src) >> 1;
			PutbackRMByte(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = 0;
			m_AuxVal = 1;
			SetSZPF_Byte(dst);
			break;
		}
	}
	else
	{
		int tmpcf = CF;

		switch (ModRM & 0x38)
		{
		case 0x00:  /* ROL eb,count */
			for (; count > 0; count--)
			{
				m_CarryVal = dst & 0x80;
				dst = (dst << 1) + CF;
			}
			PutbackRMByte(ModRM,(BYTE)dst);
			break;
		case 0x08:  /* ROR eb,count */
			for (; count > 0; count--)
			{
				m_CarryVal = dst & 0x01;
				dst = (dst >> 1) + (CF << 7);
			}
			PutbackRMByte(ModRM,(BYTE)dst);
			break;
		case 0x10:  /* RCL eb,count */
			for (; count > 0; count--)
			{
				dst = (dst << 1) + tmpcf;
				tmpcf = (int)((dst & 0x100) != 0);
			}
			PutbackRMByte(ModRM,(BYTE)dst);
			m_CarryVal = tmpcf;
			break;
		case 0x18:  /* RCR eb,count */
			for (; count > 0; count--)
			{
				dst = (tmpcf<<8)+dst;
				tmpcf = dst & 0x01;
				dst >>= 1;
			}
			PutbackRMByte(ModRM,(BYTE)dst);
			m_CarryVal = tmpcf;
			break;
		case 0x20:
		case 0x30:  /* SHL eb,count */
			for(int i=0;i<count;i++) dst<<= 1;
			SetCFB(dst);
			m_AuxVal = 1;
			SetSZPF_Byte(dst);
			PutbackRMByte(ModRM,(BYTE)dst);
			break;
		case 0x28:  /* SHR eb,count */
			for(int i=0;i<count-1;i++) dst>>= 1;
			m_CarryVal = dst & 0x1;
			dst >>= 1;
			SetSZPF_Byte(dst);
			m_AuxVal = 1;
			PutbackRMByte(ModRM,(BYTE)dst);
			break;
		case 0x38:  /* SAR eb,count */
			for(int i=0;i<count-1;i++) dst = ((INT8)dst) >> 1;
			m_CarryVal = dst & 0x1;
			dst = ((INT8)((BYTE)dst)) >> 1;
			SetSZPF_Byte(dst);
			m_AuxVal = 1;
			PutbackRMByte(ModRM,(BYTE)dst);
			break;
		}
	}
}

static void PREFIX86(_rotate_shift_Word)(unsigned ModRM, unsigned count, unsigned src)
{
//  unsigned src = GetRMWord(ModRM);
	unsigned dst=src;

	if (count==0)
	{
	}
	else if (count==1)
	{
		switch (ModRM & 0x38)
		{
#if 0
		case 0x00:  /* ROL ew,1 */
			tmp2 = (tmp << 1) + CF;
			SetCFW(tmp2);
			m_OverVal = !(!(tmp & 0x4000)) != CF;
			PutbackRMWord(ModRM,tmp2);
			break;
		case 0x08:  /* ROR ew,1 */
			m_CarryVal = tmp & 0x01;
			tmp2 = (tmp >> 1) + ((unsigned)CF << 15);
			m_OverVal = !(!(tmp & 0x8000)) != CF;
			PutbackRMWord(ModRM,tmp2);
			break;
		case 0x10:  /* RCL ew,1 */
			tmp2 = (tmp << 1) + CF;
			SetCFW(tmp2);
			m_OverVal = (tmp ^ (tmp << 1)) & 0x8000;
			PutbackRMWord(ModRM,tmp2);
			break;
		case 0x18:  /* RCR ew,1 */
			tmp2 = (tmp >> 1) + ((unsigned)CF << 15);
			m_OverVal = !(!(tmp & 0x8000)) != CF;
			m_CarryVal = tmp & 0x01;
			PutbackRMWord(ModRM,tmp2);
			break;
		case 0x20:  /* SHL ew,1 */
		case 0x30:
			tmp <<= 1;

			SetCFW(tmp);
			SetOFW_Add(tmp,tmp2,tmp2);
			m_AuxVal = 1;
			SetSZPF_Word(tmp);

			PutbackRMWord(ModRM,tmp);
			break;
		case 0x28:  /* SHR ew,1 */
			m_CarryVal = tmp & 0x01;
			m_OverVal = tmp & 0x8000;

			tmp2 = tmp >> 1;

			SetSZPF_Word(tmp2);
			m_AuxVal = 1;
			PutbackRMWord(ModRM,tmp2);
			break;
			case 0x38:  /* SAR ew,1 */
			m_CarryVal = tmp & 0x01;
			m_OverVal = 0;

			tmp2 = (tmp >> 1) | (tmp & 0x8000);

			SetSZPF_Word(tmp2);
			m_AuxVal = 1;
			PutbackRMWord(ModRM,tmp2);
			break;
#else
		case 0x00:  /* ROL ew,1 */
			m_CarryVal = src & 0x8000;
			dst=(src<<1)+CF;
			PutbackRMWord(ModRM,dst);
			m_OverVal = (src^dst)&0x8000;
			break;
		case 0x08:  /* ROR ew,1 */
			m_CarryVal = src & 0x01;
			dst = ((CF<<16)+src) >> 1;
			PutbackRMWord(ModRM,dst);
			m_OverVal = (src^dst)&0x8000;
			break;
		case 0x10:  /* RCL ew,1 */
			dst=(src<<1)+CF;
			PutbackRMWord(ModRM,dst);
			SetCFW(dst);
			m_OverVal = (src^dst)&0x8000;
			break;
		case 0x18:  /* RCR ew,1 */
			dst = ((CF<<16)+src) >> 1;
			PutbackRMWord(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = (src^dst)&0x8000;
			break;
		case 0x20:  /* SHL ew,1 */
		case 0x30:
			dst = src << 1;
			PutbackRMWord(ModRM,dst);
			SetCFW(dst);
			m_OverVal = (src^dst)&0x8000;
			m_AuxVal = 1;
			SetSZPF_Word(dst);
			break;
		case 0x28:  /* SHR ew,1 */
			dst = src >> 1;
			PutbackRMWord(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = src & 0x8000;
			m_AuxVal = 1;
			SetSZPF_Word(dst);
			break;
		case 0x38:  /* SAR ew,1 */
			dst = ((INT16)src) >> 1;
			PutbackRMWord(ModRM,dst);
			m_CarryVal = src & 0x01;
			m_OverVal = 0;
			m_AuxVal = 1;
			SetSZPF_Word(dst);
			break;
#endif
		}
	}
	else
	{
		int tmpcf = CF;

		switch (ModRM & 0x38)
		{
		case 0x00:  /* ROL ew,count */
			for (; count > 0; count--)
			{
				m_CarryVal = dst & 0x8000;
				dst = (dst << 1) + CF;
			}
			PutbackRMWord(ModRM,dst);
			break;
		case 0x08:  /* ROR ew,count */
			for (; count > 0; count--)
			{
				m_CarryVal = dst & 0x01;
				dst = (dst >> 1) + (CF << 15);
			}
			PutbackRMWord(ModRM,dst);
			break;
		case 0x10:  /* RCL ew,count */
			for (; count > 0; count--)
			{
				dst = (dst << 1) + tmpcf;
				tmpcf = (int)((dst & 0x10000) != 0);
			}
			PutbackRMWord(ModRM,dst);
			m_CarryVal = tmpcf;
			break;
		case 0x18:  /* RCR ew,count */
			for (; count > 0; count--)
			{
				dst = dst + (tmpcf << 16);
				tmpcf = dst & 0x01;
				dst >>= 1;
			}
			PutbackRMWord(ModRM,dst);
			m_CarryVal = tmpcf;
			break;
		case 0x20:
		case 0x30:  /* SHL ew,count */
			for(int i=0;i<count;i++) dst<<= 1;
			SetCFW(dst);
			m_AuxVal = 1;
			SetSZPF_Word(dst);
			PutbackRMWord(ModRM,dst);
			break;
		case 0x28:  /* SHR ew,count */
			for(int i=0;i<count-1;i++) dst>>= 1;
			m_CarryVal = dst & 0x1;
			dst >>= 1;
			SetSZPF_Word(dst);
			m_AuxVal = 1;
			PutbackRMWord(ModRM,dst);
			break;
		case 0x38:  /* SAR ew,count */
			for(int i=0;i<count-1;i++) dst = ((INT16)dst) >> 1;
			m_CarryVal = dst & 0x01;
			dst = ((INT16)((WORD)dst)) >> 1;
			SetSZPF_Word(dst);
			m_AuxVal = 1;
			PutbackRMWord(ModRM,dst);
			break;
		}
	}
}
#endif

static void PREFIX(rep)(int flagval)
{
	/* Handles rep- and repnz- prefixes. flagval is the value of ZF for the
	     loop  to continue for CMPS and SCAS instructions. */

	unsigned next = FETCHOP;

	switch(next)
	{
	case 0x26:  /* ES: */
		m_seg_prefix = TRUE;
		m_prefix_seg = ES;
		PREFIX(rep)(flagval);
		break;
	case 0x2e:  /* CS: */
		m_seg_prefix = TRUE;
		m_prefix_seg = CS;
		PREFIX(rep)(flagval);
		break;
	case 0x36:  /* SS: */
		m_seg_prefix = TRUE;
		m_prefix_seg = SS;
		PREFIX(rep)(flagval);
		break;
	case 0x3e:  /* DS: */
		m_seg_prefix = TRUE;
		m_prefix_seg = DS;
		PREFIX(rep)(flagval);
		break;
#ifndef I8086
	case 0x6c:  /* REP INSB */
#ifdef I80286
		if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
		while(m_regs.w[CX])
		{
			PutMemB(ES,m_regs.w[DI],read_port_byte(m_regs.w[DX]));
			m_regs.w[CX]--;
			m_regs.w[DI] += m_DirVal;
		}
		break;
	case 0x6d:  /* REP INSW */
#ifdef I80286
		if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
		while(m_regs.w[CX])
		{
			PutMemW(ES,m_regs.w[DI],read_port_word(m_regs.w[DX]));
			m_regs.w[CX]--;
			m_regs.w[DI] += 2 * m_DirVal;
		}
		break;
	case 0x6e:  /* REP OUTSB */
#ifdef I80286
		if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
		while(m_regs.w[CX])
		{
			write_port_byte(m_regs.w[DX],GetMemB(DS,m_regs.w[SI]));
			m_regs.w[CX]--;
			m_regs.w[SI] += m_DirVal; /* GOL 11/27/01 */
		}
		break;
	case 0x6f:  /* REP OUTSW */
#ifdef I80286
		if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
		while(m_regs.w[CX])
		{
			write_port_word(m_regs.w[DX],GetMemW(DS,m_regs.w[SI]));
			m_regs.w[CX]--;
			m_regs.w[SI] += 2 * m_DirVal; /* GOL 11/27/01 */
		}
		break;
#endif
	case 0xa4:  /* REP MOVSB */
		while(m_regs.w[CX])
		{
			BYTE tmp;

			tmp = GetMemB(DS,m_regs.w[SI]);
			PutMemB(ES,m_regs.w[DI], tmp);
			m_regs.w[CX]--;
			m_regs.w[DI] += m_DirVal;
			m_regs.w[SI] += m_DirVal;
		}
		break;
	case 0xa5:  /* REP MOVSW */
		while(m_regs.w[CX])
		{
			WORD tmp;

			tmp = GetMemW(DS,m_regs.w[SI]);
			PutMemW(ES,m_regs.w[DI], tmp);
			m_regs.w[CX]--;
			m_regs.w[DI] += 2 * m_DirVal;
			m_regs.w[SI] += 2 * m_DirVal;
		}
		break;
	case 0xa6:  /* REP(N)E CMPSB */
		m_ZeroVal = !flagval;
		while(m_regs.w[CX] && (ZF == flagval))
		{
			unsigned dst, src;

			dst = GetMemB(ES, m_regs.w[DI]);
			src = GetMemB(DS, m_regs.w[SI]);
			SUBB(src,dst); /* opposite of the usual convention */
			m_regs.w[CX]--;
			m_regs.w[DI] += m_DirVal;
			m_regs.w[SI] += m_DirVal;
		}
		break;
	case 0xa7:  /* REP(N)E CMPSW */
		m_ZeroVal = !flagval;
		while(m_regs.w[CX] && (ZF == flagval))
		{
			unsigned dst, src;

			dst = GetMemW(ES, m_regs.w[DI]);
			src = GetMemW(DS, m_regs.w[SI]);
			SUBW(src,dst); /* opposite of the usual convention */
			m_regs.w[CX]--;
			m_regs.w[DI] += 2 * m_DirVal;
			m_regs.w[SI] += 2 * m_DirVal;
		}
		break;
	case 0xaa:  /* REP STOSB */
		while(m_regs.w[CX])
		{
			PutMemB(ES,m_regs.w[DI],m_regs.b[AL]);
			m_regs.w[CX]--;
			m_regs.w[DI] += m_DirVal;
		}
		break;
	case 0xab:  /* REP STOSW */
		while(m_regs.w[CX])
		{
			PutMemW(ES,m_regs.w[DI],m_regs.w[AX]);
			m_regs.w[CX]--;
			m_regs.w[DI] += 2 * m_DirVal;
		}
		break;
	case 0xac:  /* REP LODSB */
		while(m_regs.w[CX])
		{
			m_regs.b[AL] = GetMemB(DS,m_regs.w[SI]);
			m_regs.w[CX]--;
			m_regs.w[SI] += m_DirVal;
		}
		break;
	case 0xad:  /* REP LODSW */
		while(m_regs.w[CX])
		{
			m_regs.w[AX] = GetMemW(DS,m_regs.w[SI]);
			m_regs.w[CX]--;
			m_regs.w[SI] += 2 * m_DirVal;
		}
		break;
	case 0xae:  /* REP(N)E SCASB */
		m_ZeroVal = !flagval;
		while(m_regs.w[CX] && (ZF == flagval))
		{
			unsigned src, dst;

			src = GetMemB(ES, m_regs.w[DI]);
			dst = m_regs.b[AL];
			SUBB(dst,src);
			m_regs.w[CX]--;
			m_regs.w[DI] += m_DirVal;
		}
		break;
	case 0xaf:  /* REP(N)E SCASW */
		m_ZeroVal = !flagval;
		while(m_regs.w[CX] && (ZF == flagval))
		{
			unsigned src, dst;

			src = GetMemW(ES, m_regs.w[DI]);
			dst = m_regs.w[AX];
			SUBW(dst,src);
			m_regs.w[CX]--;
			m_regs.w[DI] += 2 * m_DirVal;
		}
		break;
	default:
		PREFIX(_instruction)[next]();
	}
}

#ifndef I80186
static void PREFIX86(_add_br8)()    /* Opcode 0x00 */
{
	DEF_br8(dst,src);
	ADDB(dst,src);
	PutbackRMByte(ModRM,dst);
}

static void PREFIX86(_add_wr16)()    /* Opcode 0x01 */
{
	DEF_wr16(dst,src);
	ADDW(dst,src);
	PutbackRMWord(ModRM,dst);
}

static void PREFIX86(_add_r8b)()    /* Opcode 0x02 */
{
	DEF_r8b(dst,src);
	ADDB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_add_r16w)()    /* Opcode 0x03 */
{
	DEF_r16w(dst,src);
	ADDW(dst,src);
	RegWord(ModRM)=dst;
}


static void PREFIX86(_add_ald8)()    /* Opcode 0x04 */
{
	DEF_ald8(dst,src);
	ADDB(dst,src);
	m_regs.b[AL]=dst;
}


static void PREFIX86(_add_axd16)()    /* Opcode 0x05 */
{
	DEF_axd16(dst,src);
	ADDW(dst,src);
	m_regs.w[AX]=dst;
}


static void PREFIX86(_push_es)()    /* Opcode 0x06 */
{
	PUSH(m_sregs[ES]);
}


static void PREFIX86(_pop_es)()    /* Opcode 0x07 */
{
#ifdef I80286
	i80286_pop_seg(ES);
#else
	POP(m_sregs[ES]);
	m_base[ES] = SegBase(ES);
#endif
}

static void PREFIX86(_or_br8)()    /* Opcode 0x08 */
{
	DEF_br8(dst,src);
	ORB(dst,src);
	PutbackRMByte(ModRM,dst);
}

static void PREFIX86(_or_wr16)()    /* Opcode 0x09 */
{
	DEF_wr16(dst,src);
	ORW(dst,src);
	PutbackRMWord(ModRM,dst);
}

static void PREFIX86(_or_r8b)()    /* Opcode 0x0a */
{
	DEF_r8b(dst,src);
	ORB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_or_r16w)()    /* Opcode 0x0b */
{
	DEF_r16w(dst,src);
	ORW(dst,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_or_ald8)()    /* Opcode 0x0c */
{
	DEF_ald8(dst,src);
	ORB(dst,src);
	m_regs.b[AL]=dst;
}

static void PREFIX86(_or_axd16)()    /* Opcode 0x0d */
{
	DEF_axd16(dst,src);
	ORW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_push_cs)()    /* Opcode 0x0e */
{
	PUSH(m_sregs[CS]);
}

#ifndef I80286
static void PREFIX86(_pop_cs)()    /* Opcode 0x0f */
{
	int ip = m_pc - m_base[CS];
	POP(m_sregs[CS]);
	m_base[CS] = SegBase(CS);
	m_pc = (ip + m_base[CS]) & AMASK;
	CHANGE_PC(m_pc);
}
#endif

static void PREFIX86(_adc_br8)()    /* Opcode 0x10 */
{
	int tmpcf;
	DEF_br8(dst,src);
	src+=CF;
	ADCB(dst,src,tmpcf);
	PutbackRMByte(ModRM,dst);
	m_CarryVal = tmpcf;
}

static void PREFIX86(_adc_wr16)()    /* Opcode 0x11 */
{
	int tmpcf;
	DEF_wr16(dst,src);
	src+=CF;
	ADCW(dst,src,tmpcf);
	PutbackRMWord(ModRM,dst);
	m_CarryVal = tmpcf;
}

static void PREFIX86(_adc_r8b)()    /* Opcode 0x12 */
{
	DEF_r8b(dst,src);
	src+=CF;
	ADDB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_adc_r16w)()    /* Opcode 0x13 */
{
	DEF_r16w(dst,src);
	src+=CF;
	ADDW(dst,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_adc_ald8)()    /* Opcode 0x14 */
{
	DEF_ald8(dst,src);
	src+=CF;
	ADDB(dst,src);
	m_regs.b[AL] = dst;
}

static void PREFIX86(_adc_axd16)()    /* Opcode 0x15 */
{
	DEF_axd16(dst,src);
	src+=CF;
	ADDW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_push_ss)()    /* Opcode 0x16 */
{
	PUSH(m_sregs[SS]);
}

static void PREFIX86(_sbb_br8)()    /* Opcode 0x18 */
{
	int tmpcf;
	DEF_br8(dst,src);
	src+=CF;
	SBBB(dst,src,tmpcf);
	PutbackRMByte(ModRM,dst);
	m_CarryVal = tmpcf;
}

static void PREFIX86(_sbb_wr16)()    /* Opcode 0x19 */
{
	int tmpcf;
	DEF_wr16(dst,src);
	src+=CF;
	SBBW(dst,src,tmpcf);
	PutbackRMWord(ModRM,dst);
	m_CarryVal = tmpcf;
}

static void PREFIX86(_sbb_r8b)()    /* Opcode 0x1a */
{
	DEF_r8b(dst,src);
	src+=CF;
	SUBB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_sbb_r16w)()    /* Opcode 0x1b */
{
	DEF_r16w(dst,src);
	src+=CF;
	SUBW(dst,src);
	RegWord(ModRM)= dst;
}

static void PREFIX86(_sbb_ald8)()    /* Opcode 0x1c */
{
	DEF_ald8(dst,src);
	src+=CF;
	SUBB(dst,src);
	m_regs.b[AL] = dst;
}

static void PREFIX86(_sbb_axd16)()    /* Opcode 0x1d */
{
	DEF_axd16(dst,src);
	src+=CF;
	SUBW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_push_ds)()    /* Opcode 0x1e */
{
	PUSH(m_sregs[DS]);
}

static void PREFIX86(_pop_ds)()    /* Opcode 0x1f */
{
#ifdef I80286
	i80286_pop_seg(DS);
#else
	POP(m_sregs[DS]);
	m_base[DS] = SegBase(DS);
#endif
}

static void PREFIX86(_and_br8)()    /* Opcode 0x20 */
{
	DEF_br8(dst,src);
	ANDB(dst,src);
	PutbackRMByte(ModRM,dst);
}

static void PREFIX86(_and_wr16)()    /* Opcode 0x21 */
{
	DEF_wr16(dst,src);
	ANDW(dst,src);
	PutbackRMWord(ModRM,dst);
}

static void PREFIX86(_and_r8b)()    /* Opcode 0x22 */
{
	DEF_r8b(dst,src);
	ANDB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_and_r16w)()    /* Opcode 0x23 */
{
	DEF_r16w(dst,src);
	ANDW(dst,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_and_ald8)()    /* Opcode 0x24 */
{
	DEF_ald8(dst,src);
	ANDB(dst,src);
	m_regs.b[AL] = dst;
}

static void PREFIX86(_and_axd16)()    /* Opcode 0x25 */
{
	DEF_axd16(dst,src);
	ANDW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_daa)()    /* Opcode 0x27 */
{
	if (AF || ((m_regs.b[AL] & 0xf) > 9))
	{
		int tmp;
		m_regs.b[AL] = tmp = m_regs.b[AL] + 6;
		m_AuxVal = 1;
		m_CarryVal |= tmp & 0x100;
	}

	if (CF || (m_regs.b[AL] > 0x9f))
	{
		m_regs.b[AL] += 0x60;
		m_CarryVal = 1;
	}

	SetSZPF_Byte(m_regs.b[AL]);
}

static void PREFIX86(_sub_br8)()    /* Opcode 0x28 */
{
	DEF_br8(dst,src);
	SUBB(dst,src);
	PutbackRMByte(ModRM,dst);
}

static void PREFIX86(_sub_wr16)()    /* Opcode 0x29 */
{
	DEF_wr16(dst,src);
	SUBW(dst,src);
	PutbackRMWord(ModRM,dst);
}

static void PREFIX86(_sub_r8b)()    /* Opcode 0x2a */
{
	DEF_r8b(dst,src);
	SUBB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_sub_r16w)()    /* Opcode 0x2b */
{
	DEF_r16w(dst,src);
	SUBW(dst,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_sub_ald8)()    /* Opcode 0x2c */
{
	DEF_ald8(dst,src);
	SUBB(dst,src);
	m_regs.b[AL] = dst;
}

static void PREFIX86(_sub_axd16)()    /* Opcode 0x2d */
{
	DEF_axd16(dst,src);
	SUBW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_das)()    /* Opcode 0x2f */
{
	UINT8 tmpAL=m_regs.b[AL];
	if (AF || ((m_regs.b[AL] & 0xf) > 9))
	{
		int tmp;
		m_regs.b[AL] = tmp = m_regs.b[AL] - 6;
		m_AuxVal = 1;
		m_CarryVal |= tmp & 0x100;
	}

	if (CF || (tmpAL > 0x9f))
	{
		m_regs.b[AL] -= 0x60;
		m_CarryVal = 1;
	}

	SetSZPF_Byte(m_regs.b[AL]);
}

static void PREFIX86(_xor_br8)()    /* Opcode 0x30 */
{
	DEF_br8(dst,src);
	XORB(dst,src);
	PutbackRMByte(ModRM,dst);
}

static void PREFIX86(_xor_wr16)()    /* Opcode 0x31 */
{
	DEF_wr16(dst,src);
	XORW(dst,src);
	PutbackRMWord(ModRM,dst);
}

static void PREFIX86(_xor_r8b)()    /* Opcode 0x32 */
{
	DEF_r8b(dst,src);
	XORB(dst,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_xor_r16w)()    /* Opcode 0x33 */
{
	DEF_r16w(dst,src);
	XORW(dst,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_xor_ald8)()    /* Opcode 0x34 */
{
	DEF_ald8(dst,src);
	XORB(dst,src);
	m_regs.b[AL] = dst;
}

static void PREFIX86(_xor_axd16)()    /* Opcode 0x35 */
{
	DEF_axd16(dst,src);
	XORW(dst,src);
	m_regs.w[AX]=dst;
}

static void PREFIX86(_aaa)()    /* Opcode 0x37 */
{
	UINT8 ALcarry=1;
	if (m_regs.b[AL]>0xf9) ALcarry=2;

	if (AF || ((m_regs.b[AL] & 0xf) > 9))
	{
		m_regs.b[AL] += 6;
		m_regs.b[AH] += ALcarry;
		m_AuxVal = 1;
		m_CarryVal = 1;
	}
	else
	{
		m_AuxVal = 0;
		m_CarryVal = 0;
	}
	m_regs.b[AL] &= 0x0F;
}

static void PREFIX86(_cmp_br8)()    /* Opcode 0x38 */
{
	DEF_br8(dst,src);
	SUBB(dst,src);
}

static void PREFIX86(_cmp_wr16)()    /* Opcode 0x39 */
{
	DEF_wr16(dst,src);
	SUBW(dst,src);
}

static void PREFIX86(_cmp_r8b)()    /* Opcode 0x3a */
{
	DEF_r8b(dst,src);
	SUBB(dst,src);
}

static void PREFIX86(_cmp_r16w)()    /* Opcode 0x3b */
{
	DEF_r16w(dst,src);
	SUBW(dst,src);
}

static void PREFIX86(_cmp_ald8)()    /* Opcode 0x3c */
{
	DEF_ald8(dst,src);
	SUBB(dst,src);
}

static void PREFIX86(_cmp_axd16)()    /* Opcode 0x3d */
{
	DEF_axd16(dst,src);
	SUBW(dst,src);
}

static void PREFIX86(_aas)()    /* Opcode 0x3f */
{
//  UINT8 ALcarry=1;
//  if (m_regs.b[AL]>0xf9) ALcarry=2;

	if (AF || ((m_regs.b[AL] & 0xf) > 9))
	{
		m_regs.b[AL] -= 6;
		m_regs.b[AH] -= 1;
		m_AuxVal = 1;
		m_CarryVal = 1;
	}
	else
	{
		m_AuxVal = 0;
		m_CarryVal = 0;
	}
	m_regs.b[AL] &= 0x0F;
}

#define IncWordReg(Reg)                     \
{                                           \
	unsigned tmp = (unsigned)m_regs.w[Reg]; \
	unsigned tmp1 = tmp+1;                  \
	SetOFW_Add(tmp1,tmp,1);                 \
	SetAF(tmp1,tmp,1);                      \
	SetSZPF_Word(tmp1);                     \
	m_regs.w[Reg]=tmp1;                     \
}

static void PREFIX86(_inc_ax)()    /* Opcode 0x40 */
{
	IncWordReg(AX);
}

static void PREFIX86(_inc_cx)()    /* Opcode 0x41 */
{
	IncWordReg(CX);
}

static void PREFIX86(_inc_dx)()    /* Opcode 0x42 */
{
	IncWordReg(DX);
}

static void PREFIX(_inc_bx)()    /* Opcode 0x43 */
{
	IncWordReg(BX);
}

static void PREFIX86(_inc_sp)()    /* Opcode 0x44 */
{
	IncWordReg(SP);
}

static void PREFIX86(_inc_bp)()    /* Opcode 0x45 */
{
	IncWordReg(BP);
}

static void PREFIX86(_inc_si)()    /* Opcode 0x46 */
{
	IncWordReg(SI);
}

static void PREFIX86(_inc_di)()    /* Opcode 0x47 */
{
	IncWordReg(DI);
}

#define DecWordReg(Reg)                     \
{                                           \
	unsigned tmp = (unsigned)m_regs.w[Reg]; \
	unsigned tmp1 = tmp-1;                  \
	SetOFW_Sub(tmp1,1,tmp);                 \
	SetAF(tmp1,tmp,1);                      \
	SetSZPF_Word(tmp1);                     \
	m_regs.w[Reg]=tmp1;                     \
}

static void PREFIX86(_dec_ax)()    /* Opcode 0x48 */
{
	DecWordReg(AX);
}

static void PREFIX86(_dec_cx)()    /* Opcode 0x49 */
{
	DecWordReg(CX);
}

static void PREFIX86(_dec_dx)()    /* Opcode 0x4a */
{
	DecWordReg(DX);
}

static void PREFIX86(_dec_bx)()    /* Opcode 0x4b */
{
	DecWordReg(BX);
}

static void PREFIX86(_dec_sp)()    /* Opcode 0x4c */
{
	DecWordReg(SP);
}

static void PREFIX86(_dec_bp)()    /* Opcode 0x4d */
{
	DecWordReg(BP);
}

static void PREFIX86(_dec_si)()    /* Opcode 0x4e */
{
	DecWordReg(SI);
}

static void PREFIX86(_dec_di)()    /* Opcode 0x4f */
{
	DecWordReg(DI);
}

static void PREFIX86(_push_ax)()    /* Opcode 0x50 */
{
	PUSH(m_regs.w[AX]);
}

static void PREFIX86(_push_cx)()    /* Opcode 0x51 */
{
	PUSH(m_regs.w[CX]);
}

static void PREFIX86(_push_dx)()    /* Opcode 0x52 */
{
	PUSH(m_regs.w[DX]);
}

static void PREFIX86(_push_bx)()    /* Opcode 0x53 */
{
	PUSH(m_regs.w[BX]);
}

static void PREFIX86(_push_sp)()    /* Opcode 0x54 */
{
#ifdef I80286
	PUSH(m_regs.w[SP]+2);
#else
	PUSH(m_regs.w[SP]);
#endif
}

static void PREFIX86(_push_bp)()    /* Opcode 0x55 */
{
	PUSH(m_regs.w[BP]);
}


static void PREFIX86(_push_si)()    /* Opcode 0x56 */
{
	PUSH(m_regs.w[SI]);
}

static void PREFIX86(_push_di)()    /* Opcode 0x57 */
{
	PUSH(m_regs.w[DI]);
}

static void PREFIX86(_pop_ax)()    /* Opcode 0x58 */
{
	POP(m_regs.w[AX]);
}

static void PREFIX86(_pop_cx)()    /* Opcode 0x59 */
{
	POP(m_regs.w[CX]);
}

static void PREFIX86(_pop_dx)()    /* Opcode 0x5a */
{
	POP(m_regs.w[DX]);
}

static void PREFIX86(_pop_bx)()    /* Opcode 0x5b */
{
	POP(m_regs.w[BX]);
}

static void PREFIX86(_pop_sp)()    /* Opcode 0x5c */
{
	POP(m_regs.w[SP]);
}

static void PREFIX86(_pop_bp)()    /* Opcode 0x5d */
{
	POP(m_regs.w[BP]);
}

static void PREFIX86(_pop_si)()    /* Opcode 0x5e */
{
	POP(m_regs.w[SI]);
}

static void PREFIX86(_pop_di)()    /* Opcode 0x5f */
{
	POP(m_regs.w[DI]);
}

static void PREFIX86(_jo)()    /* Opcode 0x70 */
{
	int tmp = (int)((INT8)FETCH);
	if (OF)
	{
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jno)()    /* Opcode 0x71 */
{
	int tmp = (int)((INT8)FETCH);
	if (!OF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jb)()    /* Opcode 0x72 */
{
	int tmp = (int)((INT8)FETCH);
	if (CF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnb)()    /* Opcode 0x73 */
{
	int tmp = (int)((INT8)FETCH);
	if (!CF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jz)()    /* Opcode 0x74 */
{
	int tmp = (int)((INT8)FETCH);
	if (ZF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnz)()    /* Opcode 0x75 */
{
	int tmp = (int)((INT8)FETCH);
	if (!ZF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jbe)()    /* Opcode 0x76 */
{
	int tmp = (int)((INT8)FETCH);
	if (CF || ZF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnbe)()    /* Opcode 0x77 */
{
	int tmp = (int)((INT8)FETCH);
	if (!(CF || ZF)) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_js)()    /* Opcode 0x78 */
{
	int tmp = (int)((INT8)FETCH);
	if (SF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jns)()    /* Opcode 0x79 */
{
	int tmp = (int)((INT8)FETCH);
	if (!SF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jp)()    /* Opcode 0x7a */
{
	int tmp = (int)((INT8)FETCH);
	if (PF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnp)()    /* Opcode 0x7b */
{
	int tmp = (int)((INT8)FETCH);
	if (!PF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jl)()    /* Opcode 0x7c */
{
	int tmp = (int)((INT8)FETCH);
	if ((SF!=OF)&&!ZF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnl)()    /* Opcode 0x7d */
{
	int tmp = (int)((INT8)FETCH);
	if (ZF||(SF==OF)) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jle)()    /* Opcode 0x7e */
{
	int tmp = (int)((INT8)FETCH);
	if (ZF||(SF!=OF)) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jnle)()    /* Opcode 0x7f */
{
	int tmp = (int)((INT8)FETCH);
	if ((SF==OF)&&!ZF) {
		m_pc += tmp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_80pre)()    /* Opcode 0x80 */
{
	unsigned ModRM = FETCHOP;
	unsigned dst = GetRMByte(ModRM);
	unsigned src = FETCH;
	int tmpcf;

	switch (ModRM & 0x38)
	{
	case 0x00:  /* ADD eb,d8 */
		ADDB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x08:  /* OR eb,d8 */
		ORB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x10:  /* ADC eb,d8 */
		src+=CF;
		ADCB(dst,src,tmpcf);
		PutbackRMByte(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x18:  /* SBB eb,b8 */
		src+=CF;
		SBBB(dst,src,tmpcf);
		PutbackRMByte(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x20:  /* AND eb,d8 */
		ANDB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x28:  /* SUB eb,d8 */
		SUBB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x30:  /* XOR eb,d8 */
		XORB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x38:  /* CMP eb,d8 */
		SUBB(dst,src);
		break;
	}
}


static void PREFIX86(_81pre)()    /* Opcode 0x81 */
{
	unsigned ModRM = FETCH;
	unsigned dst = GetRMWord(ModRM);
	unsigned src = FETCH;
	int tmpcf;
	src+= (FETCH << 8);

	switch (ModRM & 0x38)
	{
	case 0x00:  /* ADD ew,d16 */
		ADDW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x08:  /* OR ew,d16 */
		ORW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x10:  /* ADC ew,d16 */
		src+=CF;
		ADCW(dst,src,tmpcf);
		PutbackRMWord(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x18:  /* SBB ew,d16 */
		src+=CF;
		SBBW(dst,src,tmpcf);
		PutbackRMWord(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x20:  /* AND ew,d16 */
		ANDW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x28:  /* SUB ew,d16 */
		SUBW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x30:  /* XOR ew,d16 */
		XORW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x38:  /* CMP ew,d16 */
		SUBW(dst,src);
		break;
	}
}

static void PREFIX86(_82pre)()  /* Opcode 0x82 */
{
	unsigned ModRM = FETCH;
	unsigned dst = GetRMByte(ModRM);
	unsigned src = FETCH;
	int tmpcf;

	switch (ModRM & 0x38)
	{
	case 0x00:  /* ADD eb,d8 */
		ADDB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x08:  /* OR eb,d8 */
		ORB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x10:  /* ADC eb,d8 */
		src+=CF;
		ADCB(dst,src,tmpcf);
		PutbackRMByte(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x18:  /* SBB eb,d8 */
		src+=CF;
		SBBB(dst,src,tmpcf);
		PutbackRMByte(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x20:  /* AND eb,d8 */
		ANDB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x28:  /* SUB eb,d8 */
		SUBB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x30:  /* XOR eb,d8 */
		XORB(dst,src);
		PutbackRMByte(ModRM,dst);
		break;
	case 0x38:  /* CMP eb,d8 */
		SUBB(dst,src);
		break;
	}
}

static void PREFIX86(_83pre)()    /* Opcode 0x83 */
{
	unsigned ModRM = FETCH;
	unsigned dst = GetRMWord(ModRM);
	unsigned src = (WORD)((INT16)((INT8)FETCH));
	int tmpcf;

	switch (ModRM & 0x38)
	{
	case 0x00:  /* ADD ew,d16 */
		ADDW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x08:  /* OR ew,d16 */
		ORW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x10:  /* ADC ew,d16 */
		src+=CF;
		ADCW(dst,src,tmpcf);
		PutbackRMWord(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x18:  /* SBB ew,d16 */
		src+=CF;
		SBBW(dst,src,tmpcf);
		PutbackRMWord(ModRM,dst);
		m_CarryVal = tmpcf;
		break;
	case 0x20:  /* AND ew,d16 */
		ANDW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x28:  /* SUB ew,d16 */
		SUBW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x30:  /* XOR ew,d16 */
		XORW(dst,src);
		PutbackRMWord(ModRM,dst);
		break;
	case 0x38:  /* CMP ew,d16 */
		SUBW(dst,src);
		break;
	}
}

static void PREFIX86(_test_br8)()    /* Opcode 0x84 */
{
	DEF_br8(dst,src);
	ANDB(dst,src);
}

static void PREFIX86(_test_wr16)()    /* Opcode 0x85 */
{
	DEF_wr16(dst,src);
	ANDW(dst,src);
}

static void PREFIX86(_xchg_br8)()    /* Opcode 0x86 */
{
	DEF_br8(dst,src);
	PutbackRMByte(ModRM,src);
	RegByte(ModRM)=dst;
}

static void PREFIX86(_xchg_wr16)()    /* Opcode 0x87 */
{
	DEF_wr16(dst,src);
	PutbackRMWord(ModRM,src);
	RegWord(ModRM)=dst;
}

static void PREFIX86(_mov_br8)()    /* Opcode 0x88 */
{
	unsigned ModRM = FETCH;
	BYTE src = RegByte(ModRM);
	PutRMByte(ModRM,src);
}

static void PREFIX86(_mov_wr16)()    /* Opcode 0x89 */
{
	unsigned ModRM = FETCH;
	WORD src = RegWord(ModRM);
	PutRMWord(ModRM,src);
}

static void PREFIX86(_mov_r8b)()    /* Opcode 0x8a */
{
	unsigned ModRM = FETCH;
	BYTE src = GetRMByte(ModRM);
	RegByte(ModRM)=src;
}

static void PREFIX86(_mov_r16w)()    /* Opcode 0x8b */
{
	unsigned ModRM = FETCH;
	WORD src = GetRMWord(ModRM);
	RegWord(ModRM)=src;
}

static void PREFIX86(_mov_wsreg)()    /* Opcode 0x8c */
{
	unsigned ModRM = FETCH;
	if (ModRM & 0x20) { /* HJB 12/13/98 1xx is invalid */
		m_pc = m_prevpc;
		return PREFIX86(_invalid)();
	}

	PutRMWord(ModRM,m_sregs[(ModRM & 0x38) >> 3]);
}

static void PREFIX86(_lea)()    /* Opcode 0x8d */
{
	unsigned ModRM = FETCH;
	(void)(*GetEA[ModRM])();
	RegWord(ModRM)=m_eo;    /* HJB 12/13/98 effective offset (no segment part) */
}

static void PREFIX86(_popw)()    /* Opcode 0x8f */
{
	unsigned ModRM = FETCH;
		WORD tmp;
	tmp = ReadWord(m_base[SS] + m_regs.w[SP]);
	PutRMWord(ModRM,tmp);
	m_regs.w[SP] += 2;
}


#define XchgAXReg(Reg)              \
{                                   \
	WORD tmp;                       \
	tmp = m_regs.w[Reg];            \
	m_regs.w[Reg] = m_regs.w[AX];   \
	m_regs.w[AX] = tmp;             \
}


static void PREFIX86(_nop)()    /* Opcode 0x90 */
{
	/* this is XchgAXReg(AX); */
}

static void PREFIX86(_xchg_axcx)()    /* Opcode 0x91 */
{
	XchgAXReg(CX);
}

static void PREFIX86(_xchg_axdx)()    /* Opcode 0x92 */
{
	XchgAXReg(DX);
}

static void PREFIX86(_xchg_axbx)()    /* Opcode 0x93 */
{
	XchgAXReg(BX);
}

static void PREFIX86(_xchg_axsp)()    /* Opcode 0x94 */
{
	XchgAXReg(SP);
}

static void PREFIX86(_xchg_axbp)()    /* Opcode 0x95 */
{
	XchgAXReg(BP);
}

static void PREFIX86(_xchg_axsi)()    /* Opcode 0x96 */
{
	XchgAXReg(SI);
}

static void PREFIX86(_xchg_axdi)()    /* Opcode 0x97 */
{
	XchgAXReg(DI);
}

static void PREFIX86(_cbw)()    /* Opcode 0x98 */
{
	m_regs.b[AH] = (m_regs.b[AL] & 0x80) ? 0xff : 0;
}

static void PREFIX86(_cwd)()    /* Opcode 0x99 */
{
	m_regs.w[DX] = (m_regs.b[AH] & 0x80) ? 0xffff : 0;
}

static void PREFIX86(_call_far)()
{
	unsigned int tmp, tmp2;
	WORD cs, ip;

	tmp = FETCH;
	tmp += FETCH << 8;

	tmp2 = FETCH;
	tmp2 += FETCH << 8;

	ip = m_pc - m_base[CS];
	cs = m_sregs[CS];

#ifdef I80286
	i80286_code_descriptor(tmp2, tmp, 2);
#else
	m_sregs[CS] = (WORD)tmp2;
	m_base[CS] = SegBase(CS);
	m_pc = (m_base[CS] + (WORD)tmp) & AMASK;
#endif
	PUSH(cs);
	PUSH(ip);
	CHANGE_PC(m_pc);
}

static void PREFIX86(_wait)()    /* Opcode 0x9b */
{
#ifdef I80286
	if ((m_msw&0x0a) == 0x0a) throw TRAP(FPU_UNAVAILABLE,-1);
#endif
	if (m_test_state)
	{
		m_pc--;
	}
}

static void PREFIX86(_pushf)()    /* Opcode 0x9c */
{
	unsigned tmp;

	tmp = CompressFlags();
#ifdef I80286
	if(!PM) ( tmp &= ~0xf000 );
#endif
	PUSH( tmp );
}

#ifndef I80286
static void PREFIX86(_popf)()    /* Opcode 0x9d */
{
	unsigned tmp;
	POP(tmp);

	ExpandFlags(tmp);
	m_flags = tmp;
	m_flags = CompressFlags();

	if (m_TF) PREFIX(_trap)();

	/* if the IF is set, and an interrupt is pending, signal an interrupt */
	if (m_IF && m_irq_state)
	{
		PREFIX(_interrupt)((UINT32)-1);
		m_irq_state = CLEAR_LINE;
	}
}
#endif

static void PREFIX86(_sahf)()    /* Opcode 0x9e */
{
	unsigned tmp = (CompressFlags() & 0xff00) | (m_regs.b[AH] & 0xd5);
	ExpandFlags(tmp);
}

static void PREFIX86(_lahf)()    /* Opcode 0x9f */
{
	m_regs.b[AH] = CompressFlags() & 0xff;
}


static void PREFIX86(_mov_aldisp)()    /* Opcode 0xa0 */
{
	unsigned addr;

	addr = FETCH;
	addr += FETCH << 8;

	m_regs.b[AL] = GetMemB(DS, addr);
}

static void PREFIX86(_mov_axdisp)()    /* Opcode 0xa1 */
{
	unsigned addr;

	addr = FETCH;
	addr += FETCH << 8;

	m_regs.w[AX] = GetMemW(DS, addr);
}

static void PREFIX86(_mov_dispal)()    /* Opcode 0xa2 */
{
	unsigned addr;

	addr = FETCH;
	addr += FETCH << 8;

	PutMemB(DS, addr, m_regs.b[AL]);
}

static void PREFIX86(_mov_dispax)()    /* Opcode 0xa3 */
{
	unsigned addr;

	addr = FETCH;
	addr += FETCH << 8;

	PutMemW(DS, addr, m_regs.w[AX]);
}

static void PREFIX86(_movsb)()    /* Opcode 0xa4 */
{
	BYTE tmp = GetMemB(DS,m_regs.w[SI]);
	PutMemB(ES,m_regs.w[DI], tmp);
	m_regs.w[DI] += m_DirVal;
	m_regs.w[SI] += m_DirVal;
}

static void PREFIX86(_movsw)()    /* Opcode 0xa5 */
{
	WORD tmp = GetMemW(DS,m_regs.w[SI]);
	PutMemW(ES,m_regs.w[DI], tmp);
	m_regs.w[DI] += 2 * m_DirVal;
	m_regs.w[SI] += 2 * m_DirVal;
}

static void PREFIX86(_cmpsb)()    /* Opcode 0xa6 */
{
	unsigned dst = GetMemB(ES, m_regs.w[DI]);
	unsigned src = GetMemB(DS, m_regs.w[SI]);
	SUBB(src,dst); /* opposite of the usual convention */
	m_regs.w[DI] += m_DirVal;
	m_regs.w[SI] += m_DirVal;
}

static void PREFIX86(_cmpsw)()    /* Opcode 0xa7 */
{
	unsigned dst = GetMemW(ES, m_regs.w[DI]);
	unsigned src = GetMemW(DS, m_regs.w[SI]);
	SUBW(src,dst); /* opposite of the usual convention */
	m_regs.w[DI] += 2 * m_DirVal;
	m_regs.w[SI] += 2 * m_DirVal;
}

static void PREFIX86(_test_ald8)()    /* Opcode 0xa8 */
{
	DEF_ald8(dst,src);
	ANDB(dst,src);
}

static void PREFIX86(_test_axd16)()    /* Opcode 0xa9 */
{
	DEF_axd16(dst,src);
	ANDW(dst,src);
}

static void PREFIX86(_stosb)()    /* Opcode 0xaa */
{
	PutMemB(ES,m_regs.w[DI],m_regs.b[AL]);
	m_regs.w[DI] += m_DirVal;
}

static void PREFIX86(_stosw)()    /* Opcode 0xab */
{
	PutMemW(ES,m_regs.w[DI],m_regs.w[AX]);
	m_regs.w[DI] += 2 * m_DirVal;
}

static void PREFIX86(_lodsb)()    /* Opcode 0xac */
{
	m_regs.b[AL] = GetMemB(DS,m_regs.w[SI]);
	m_regs.w[SI] += m_DirVal;
}

static void PREFIX86(_lodsw)()    /* Opcode 0xad */
{
	m_regs.w[AX] = GetMemW(DS,m_regs.w[SI]);
	m_regs.w[SI] += 2 * m_DirVal;
}

static void PREFIX86(_scasb)()    /* Opcode 0xae */
{
	unsigned src = GetMemB(ES, m_regs.w[DI]);
	unsigned dst = m_regs.b[AL];
	SUBB(dst,src);
	m_regs.w[DI] += m_DirVal;
}

static void PREFIX86(_scasw)()    /* Opcode 0xaf */
{
	unsigned src = GetMemW(ES, m_regs.w[DI]);
	unsigned dst = m_regs.w[AX];
	SUBW(dst,src);
	m_regs.w[DI] += 2 * m_DirVal;
}

static void PREFIX86(_mov_ald8)()    /* Opcode 0xb0 */
{
	m_regs.b[AL] = FETCH;
}

static void PREFIX86(_mov_cld8)()    /* Opcode 0xb1 */
{
	m_regs.b[CL] = FETCH;
}

static void PREFIX86(_mov_dld8)()    /* Opcode 0xb2 */
{
	m_regs.b[DL] = FETCH;
}

static void PREFIX86(_mov_bld8)()    /* Opcode 0xb3 */
{
	m_regs.b[BL] = FETCH;
}

static void PREFIX86(_mov_ahd8)()    /* Opcode 0xb4 */
{
	m_regs.b[AH] = FETCH;
}

static void PREFIX86(_mov_chd8)()    /* Opcode 0xb5 */
{
	m_regs.b[CH] = FETCH;
}

static void PREFIX86(_mov_dhd8)()    /* Opcode 0xb6 */
{
	m_regs.b[DH] = FETCH;
}

static void PREFIX86(_mov_bhd8)()    /* Opcode 0xb7 */
{
	m_regs.b[BH] = FETCH;
}

static void PREFIX86(_mov_axd16)()    /* Opcode 0xb8 */
{
	m_regs.b[AL] = FETCH;
	m_regs.b[AH] = FETCH;
}

static void PREFIX86(_mov_cxd16)()    /* Opcode 0xb9 */
{
	m_regs.b[CL] = FETCH;
	m_regs.b[CH] = FETCH;
}

static void PREFIX86(_mov_dxd16)()    /* Opcode 0xba */
{
	m_regs.b[DL] = FETCH;
	m_regs.b[DH] = FETCH;
}

static void PREFIX86(_mov_bxd16)()    /* Opcode 0xbb */
{
	m_regs.b[BL] = FETCH;
	m_regs.b[BH] = FETCH;
}

static void PREFIX86(_mov_spd16)()    /* Opcode 0xbc */
{
	m_regs.b[SPL] = FETCH;
	m_regs.b[SPH] = FETCH;
}

static void PREFIX86(_mov_bpd16)()    /* Opcode 0xbd */
{
	m_regs.b[BPL] = FETCH;
	m_regs.b[BPH] = FETCH;
}

static void PREFIX86(_mov_sid16)()    /* Opcode 0xbe */
{
	m_regs.b[SIL] = FETCH;
	m_regs.b[SIH] = FETCH;
}

static void PREFIX86(_mov_did16)()    /* Opcode 0xbf */
{
	m_regs.b[DIL] = FETCH;
	m_regs.b[DIH] = FETCH;
}

static void PREFIX86(_ret_d16)()    /* Opcode 0xc2 */
{
	unsigned count = FETCH;
	count += FETCH << 8;
	POP(m_pc);
	m_pc = (m_pc + m_base[CS]) & AMASK;
	m_regs.w[SP]+=count;
	CHANGE_PC(m_pc);
}

static void PREFIX86(_ret)()    /* Opcode 0xc3 */
{
	POP(m_pc);
	m_pc = (m_pc + m_base[CS]) & AMASK;
	CHANGE_PC(m_pc);
}

static void PREFIX86(_les_dw)()    /* Opcode 0xc4 */
{
	unsigned ModRM = FETCH;
	WORD tmp = GetRMWord(ModRM);

#ifdef I80286
	i80286_data_descriptor(ES,GetnextRMWord);
#else
	m_sregs[ES] = GetnextRMWord;
	m_base[ES] = SegBase(ES);
#endif
	RegWord(ModRM)= tmp;
}

static void PREFIX86(_lds_dw)()    /* Opcode 0xc5 */
{
	unsigned ModRM = FETCH;
	WORD tmp = GetRMWord(ModRM);

#ifdef I80286
	i80286_data_descriptor(DS,GetnextRMWord);
#else
	m_sregs[DS] = GetnextRMWord;
	m_base[DS] = SegBase(DS);
#endif
	RegWord(ModRM)=tmp;
}

static void PREFIX86(_mov_bd8)()    /* Opcode 0xc6 */
{
	unsigned ModRM = FETCH;
	PutImmRMByte(ModRM);
}

static void PREFIX86(_mov_wd16)()    /* Opcode 0xc7 */
{
	unsigned ModRM = FETCH;
	PutImmRMWord(ModRM);
}

#ifndef I80286
static void PREFIX86(_retf_d16)()    /* Opcode 0xca */
{
	unsigned count = FETCH;
	count += FETCH << 8;

	POP(m_pc);
	POP(m_sregs[CS]);
	m_base[CS] = SegBase(CS);
	m_pc = (m_pc + m_base[CS]) & AMASK;
	m_regs.w[SP]+=count;
	CHANGE_PC(m_pc);
}

static void PREFIX86(_retf)()    /* Opcode 0xcb */
{
	POP(m_pc);
	POP(m_sregs[CS]);
	m_base[CS] = SegBase(CS);
	m_pc = (m_pc + m_base[CS]) & AMASK;
	CHANGE_PC(m_pc);
}
#endif

static void PREFIX86(_int3)()    /* Opcode 0xcc */
{
	PREFIX(_interrupt)(3);
}

static void PREFIX86(_int)()    /* Opcode 0xcd */
{
	unsigned int_num = FETCH;
	PREFIX(_interrupt)(int_num);
}

static void PREFIX86(_into)()    /* Opcode 0xce */
{
	if (OF) {
		PREFIX(_interrupt)(4);
	}
}

#ifndef I80286
static void PREFIX86(_iret)()    /* Opcode 0xcf */
{
	UINT32 old = m_pc - 1;

	POP(m_pc);
	POP(m_sregs[CS]);
	m_base[CS] = SegBase(CS);
	m_pc = (m_pc + m_base[CS]) & AMASK;
		PREFIX(_popf)();
	CHANGE_PC(m_pc);

	/* if the IF is set, and an interrupt is pending, signal an interrupt */
	if (m_IF && m_irq_state)
	{
		PREFIX(_interrupt)((UINT32)-1);
		m_irq_state = CLEAR_LINE;
	}

	// MS-DOS system call
	if(IRET_TOP <= old && old < (IRET_TOP + IRET_SIZE)) {
		msdos_syscall(old - IRET_TOP);
	}
}
#endif

static void PREFIX86(_rotshft_b)()    /* Opcode 0xd0 */
{
	unsigned ModRM = FETCHOP;
	PREFIX(_rotate_shift_Byte)(ModRM,1,GetRMByte(ModRM));
}


static void PREFIX86(_rotshft_w)()    /* Opcode 0xd1 */
{
	unsigned ModRM = FETCHOP;
	PREFIX(_rotate_shift_Word)(ModRM,1,GetRMWord(ModRM));
}


#ifdef I8086
static void PREFIX86(_rotshft_bcl)()    /* Opcode 0xd2 */
{
	unsigned ModRM = FETCHOP;
	PREFIX(_rotate_shift_Byte)(ModRM,m_regs.b[CL],GetRMByte(ModRM));
}

static void PREFIX86(_rotshft_wcl)()    /* Opcode 0xd3 */
{
	unsigned ModRM = FETCHOP;
	PREFIX(_rotate_shift_Word)(ModRM,m_regs.b[CL],GetRMWord(ModRM));
}
#endif

/* OB: Opcode works on NEC V-Series but not the Variants              */
/*     one could specify any byte value as operand but the NECs */
/*     always substitute 0x0a.              */
static void PREFIX86(_aam)()    /* Opcode 0xd4 */
{
	unsigned mult = FETCH;

	if (mult == 0)
		PREFIX(_interrupt)(0);
	else
	{
		m_regs.b[AH] = m_regs.b[AL] / mult;
		m_regs.b[AL] %= mult;

		SetSZPF_Word(m_regs.w[AX]);
	}
}

static void PREFIX86(_aad)()    /* Opcode 0xd5 */
{
	unsigned mult = FETCH;

	m_regs.b[AL] = m_regs.b[AH] * mult + m_regs.b[AL];
	m_regs.b[AH] = 0;

	SetZF(m_regs.b[AL]);
	SetPF(m_regs.b[AL]);
	m_SignVal = 0;
}


static void PREFIX86(_xlat)()    /* Opcode 0xd7 */
{
	unsigned dest = m_regs.w[BX]+m_regs.b[AL];

	m_regs.b[AL] = GetMemB(DS, dest);
}

#ifndef I80286
static void PREFIX86(_escape)()    /* Opcodes 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde and 0xdf */
{
	unsigned ModRM = FETCH;
	GetRMByte(ModRM);
}
#endif

static void PREFIX86(_loopne)()    /* Opcode 0xe0 */
{
	int disp = (int)((INT8)FETCH);
	unsigned tmp = m_regs.w[CX]-1;

	m_regs.w[CX]=tmp;

	if (!ZF && tmp) {
		m_pc += disp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_loope)()    /* Opcode 0xe1 */
{
	int disp = (int)((INT8)FETCH);
	unsigned tmp = m_regs.w[CX]-1;

	m_regs.w[CX]=tmp;

	if (ZF && tmp) {
		m_pc += disp;
/* ASG - can probably assume this is safe
         CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_loop)()    /* Opcode 0xe2 */
{
	int disp = (int)((INT8)FETCH);
	unsigned tmp = m_regs.w[CX]-1;

	m_regs.w[CX]=tmp;

	if (tmp) {
		m_pc += disp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_jcxz)()    /* Opcode 0xe3 */
{
	int disp = (int)((INT8)FETCH);

	if (m_regs.w[CX] == 0) {
		m_pc += disp;
/* ASG - can probably assume this is safe
        CHANGE_PC(m_pc);*/
	}
}

static void PREFIX86(_inal)()    /* Opcode 0xe4 */
{
	unsigned port;
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	port = FETCH;

	m_regs.b[AL] = read_port_byte(port);
}

static void PREFIX86(_inax)()    /* Opcode 0xe5 */
{
	unsigned port;
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	port = FETCH;

	m_regs.w[AX] = read_port_word(port);
}

static void PREFIX86(_outal)()    /* Opcode 0xe6 */
{
	unsigned port;
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	port = FETCH;

	write_port_byte(port, m_regs.b[AL]);
}

static void PREFIX86(_outax)()    /* Opcode 0xe7 */
{
	unsigned port;
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	port = FETCH;

	write_port_word(port, m_regs.w[AX]);
}

static void PREFIX86(_call_d16)()    /* Opcode 0xe8 */
{
	WORD ip, tmp;

	FETCHWORD(tmp);
	ip = m_pc - m_base[CS];
	PUSH(ip);
	ip += tmp;
	m_pc = (ip + m_base[CS]) & AMASK;
	CHANGE_PC(m_pc);
}

static void PREFIX86(_jmp_d16)()    /* Opcode 0xe9 */
{
	WORD ip, tmp;

	FETCHWORD(tmp);
	ip = m_pc - m_base[CS] + tmp;
	m_pc = (ip + m_base[CS]) & AMASK;
	CHANGE_PC(m_pc);
}

static void PREFIX86(_jmp_far)()    /* Opcode 0xea */
{
	unsigned tmp,tmp1;

	tmp = FETCH;
	tmp += FETCH << 8;

	tmp1 = FETCH;
	tmp1 += FETCH << 8;

#ifdef I80286
	i80286_code_descriptor(tmp1,tmp, 1);
#else
	m_sregs[CS] = (WORD)tmp1;
	m_base[CS] = SegBase(CS);
	m_pc = (m_base[CS] + tmp) & AMASK;
#endif
	CHANGE_PC(m_pc);
}

static void PREFIX86(_jmp_d8)()    /* Opcode 0xeb */
{
	int tmp = (int)((INT8)FETCH);
	m_pc += tmp;
/* ASG - can probably assume this is safe
    CHANGE_PC(m_pc);*/
}

static void PREFIX86(_inaldx)()    /* Opcode 0xec */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	m_regs.b[AL] = read_port_byte(m_regs.w[DX]);
}

static void PREFIX86(_inaxdx)()    /* Opcode 0xed */
{
	unsigned port = m_regs.w[DX];
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	m_regs.w[AX] = read_port_word(port);
}

static void PREFIX86(_outdxal)()    /* Opcode 0xee */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	write_port_byte(m_regs.w[DX], m_regs.b[AL]);
}

static void PREFIX86(_outdxax)()    /* Opcode 0xef */
{
	unsigned port = m_regs.w[DX];
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	write_port_word(port, m_regs.w[AX]);
}

/* I think thats not a V20 instruction...*/
static void PREFIX86(_lock)()    /* Opcode 0xf0 */
{
#ifdef I80286
	if(PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT,0);
#endif
	PREFIX(_instruction)[FETCHOP]();  /* un-interruptible */
}
#endif

static void PREFIX(_pop_ss)()    /* Opcode 0x17 */
{
#ifdef I80286
	i80286_pop_seg(SS);
#else
	POP(m_sregs[SS]);
	m_base[SS] = SegBase(SS);
#endif
	PREFIX(_instruction)[FETCHOP](); /* no interrupt before next instruction */
}

static void PREFIX(_es)()    /* Opcode 0x26 */
{
	m_seg_prefix = TRUE;
	m_prefix_seg = ES;
	PREFIX(_instruction)[FETCHOP]();
}

static void PREFIX(_cs)()    /* Opcode 0x2e */
{
	m_seg_prefix = TRUE;
	m_prefix_seg = CS;
	PREFIX(_instruction)[FETCHOP]();
}

static void PREFIX(_ss)()    /* Opcode 0x36 */
{
	m_seg_prefix = TRUE;
	m_prefix_seg = SS;
	PREFIX(_instruction)[FETCHOP]();
}

static void PREFIX(_ds)()    /* Opcode 0x3e */
{
	m_seg_prefix = TRUE;
	m_prefix_seg = DS;
	PREFIX(_instruction)[FETCHOP]();
}

static void PREFIX(_mov_sregw)()    /* Opcode 0x8e */
{
	unsigned ModRM = FETCH;
	WORD src = GetRMWord(ModRM);

#ifdef I80286
	switch (ModRM & 0x38)
	{
	case 0x00:  /* mov es,ew */
		i80286_data_descriptor(ES,src);
		break;
	case 0x18:  /* mov ds,ew */
		i80286_data_descriptor(DS,src);
		break;
	case 0x10:  /* mov ss,ew */
		i80286_data_descriptor(SS,src);
		m_seg_prefix = FALSE;
		PREFIX(_instruction)[FETCHOP]();
		break;
	case 0x08:  /* mov cs,ew */
		PREFIX(_invalid)();
		break;  /* doesn't do a jump far */
	}
#else
	switch (ModRM & 0x38)
	{
	case 0x00:  /* mov es,ew */
		m_sregs[ES] = src;
		m_base[ES] = SegBase(ES);
		break;
	case 0x18:  /* mov ds,ew */
		m_sregs[DS] = src;
		m_base[DS] = SegBase(DS);
		break;
	case 0x10:  /* mov ss,ew */
		m_sregs[SS] = src;
		m_base[SS] = SegBase(SS); /* no interrupt allowed before next instr */
		m_seg_prefix = FALSE;
		PREFIX(_instruction)[FETCHOP]();
		break;
	case 0x08:  /* mov cs,ew */
#ifndef I80186
		int ip = m_pc - m_base[CS];
		m_sregs[CS] = src;
		m_base[CS] = SegBase(CS);
		m_pc = (ip + m_base[CS]) & AMASK;
		CHANGE_PC(m_pc);
#endif
		break;
	}
#endif
}

static void PREFIX(_repne)()    /* Opcode 0xf2 */
{
		PREFIX(rep)(0);
}

static void PREFIX(_repe)()    /* Opcode 0xf3 */
{
	PREFIX(rep)(1);
}

static void PREFIX(_sti)()    /* Opcode 0xfb */
{
#ifdef I80286
	if(PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT,0);
#endif
	SetIF(1);
	PREFIX(_instruction)[FETCHOP](); /* no interrupt before next instruction */

	/* if an interrupt is pending, signal an interrupt */
	if (m_irq_state)
	{
#ifdef I80286
		i80286_interrupt_descriptor(pic_ack(), 2, -1);
#else
		PREFIX86(_interrupt)((UINT32)-1);
#endif
		m_irq_state = CLEAR_LINE;
	}
}

#ifndef I80186
static void PREFIX86(_hlt)()    /* Opcode 0xf4 */
{
#ifdef I80286
	if(PM && (CPL!=0)) throw TRAP(GENERAL_PROTECTION_FAULT,0);
#endif
	m_halted=1;
}

static void PREFIX86(_cmc)()    /* Opcode 0xf5 */
{
	m_CarryVal = !CF;
}

static void PREFIX86(_f6pre)()
{
	/* Opcode 0xf6 */
	unsigned ModRM = FETCH;
	unsigned tmp = (unsigned)GetRMByte(ModRM);
	unsigned tmp2;


	switch (ModRM & 0x38)
	{
	case 0x00:  /* TEST Eb, data8 */
	case 0x08:  /* ??? */
		tmp &= FETCH;

		m_CarryVal = m_OverVal = m_AuxVal = 0;
		SetSZPF_Byte(tmp);
		break;

	case 0x10:  /* NOT Eb */
		PutbackRMByte(ModRM,~tmp);
		break;

	case 0x18:  /* NEG Eb */
		tmp2=0;
		SUBB(tmp2,tmp);
		PutbackRMByte(ModRM,tmp2);
		break;
	case 0x20:  /* MUL AL, Eb */
		{
			UINT16 result;
			tmp2 = m_regs.b[AL];

			SetSF((INT8)tmp2);
			SetPF(tmp2);

			result = (UINT16)tmp2*tmp;
			m_regs.w[AX]=(WORD)result;

			SetZF(m_regs.w[AX]);
			m_CarryVal = m_OverVal = (m_regs.b[AH] != 0);
		}
		break;
	case 0x28:  /* IMUL AL, Eb */
		{
			INT16 result;

			tmp2 = (unsigned)m_regs.b[AL];

			SetSF((INT8)tmp2);
			SetPF(tmp2);

			result = (INT16)((INT8)tmp2)*(INT16)((INT8)tmp);
			m_regs.w[AX]=(WORD)result;

			SetZF(m_regs.w[AX]);

			m_CarryVal = m_OverVal = (result >> 7 != 0) && (result >> 7 != -1);
		}
		break;
	case 0x30:  /* DIV AL, Ew */
		{
			UINT16 result;

			result = m_regs.w[AX];

			if (tmp)
			{
				if ((result / tmp) > 0xff)
				{
					PREFIX(_interrupt)(0);
					break;
				}
				else
				{
					m_regs.b[AH] = result % tmp;
					m_regs.b[AL] = result / tmp;
				}
			}
			else
			{
				PREFIX(_interrupt)(0);
				break;
			}
		}
		break;
	case 0x38:  /* IDIV AL, Ew */
		{
			INT16 result;

			result = m_regs.w[AX];

			if (tmp)
			{
				tmp2 = result % (INT16)((INT8)tmp);

				if ((result /= (INT16)((INT8)tmp)) > 0xff)
				{
					PREFIX(_interrupt)(0);
					break;
				}
				else
				{
					m_regs.b[AL] = result;
					m_regs.b[AH] = tmp2;
				}
			}
			else
			{
				PREFIX(_interrupt)(0);
				break;
			}
		}
		break;
	}
}


static void PREFIX86(_f7pre)()
{
	/* Opcode 0xf7 */
	unsigned ModRM = FETCH;
		unsigned tmp = GetRMWord(ModRM);
	unsigned tmp2;


	switch (ModRM & 0x38)
	{
	case 0x00:  /* TEST Ew, data16 */
	case 0x08:  /* ??? */
		tmp2 = FETCH;
		tmp2 += FETCH << 8;

		tmp &= tmp2;

		m_CarryVal = m_OverVal = m_AuxVal = 0;
		SetSZPF_Word(tmp);
		break;

	case 0x10:  /* NOT Ew */
		tmp = ~tmp;
		PutbackRMWord(ModRM,tmp);
		break;

	case 0x18:  /* NEG Ew */
		tmp2 = 0;
		SUBW(tmp2,tmp);
		PutbackRMWord(ModRM,tmp2);
		break;
	case 0x20:  /* MUL AX, Ew */
		{
			UINT32 result;
			tmp2 = m_regs.w[AX];

			SetSF((INT16)tmp2);
			SetPF(tmp2);

			result = (UINT32)tmp2*tmp;
			m_regs.w[AX]=(WORD)result;
			result >>= 16;
			m_regs.w[DX]=result;

			SetZF(m_regs.w[AX] | m_regs.w[DX]);
			m_CarryVal = m_OverVal = (m_regs.w[DX] != 0);
		}
		break;

	case 0x28:  /* IMUL AX, Ew */
		{
			INT32 result;

			tmp2 = m_regs.w[AX];

			SetSF((INT16)tmp2);
			SetPF(tmp2);

			result = (INT32)((INT16)tmp2)*(INT32)((INT16)tmp);
			m_CarryVal = m_OverVal = (result >> 15 != 0) && (result >> 15 != -1);

			m_regs.w[AX]=(WORD)result;
			result = (WORD)(result >> 16);
			m_regs.w[DX]=result;

			SetZF(m_regs.w[AX] | m_regs.w[DX]);
		}
		break;
	case 0x30:  /* DIV AX, Ew */
		{
			UINT32 result;

			result = (m_regs.w[DX] << 16) + m_regs.w[AX];

			if (tmp)
			{
				tmp2 = result % tmp;
				if ((result / tmp) > 0xffff)
				{
					PREFIX(_interrupt)(0);
					break;
				}
				else
				{
					m_regs.w[DX]=tmp2;
					result /= tmp;
					m_regs.w[AX]=result;
				}
			}
			else
			{
				PREFIX(_interrupt)(0);
				break;
			}
		}
		break;
	case 0x38:  /* IDIV AX, Ew */
		{
			INT32 result;

			result = (m_regs.w[DX] << 16) + m_regs.w[AX];

			if (tmp)
			{
				tmp2 = result % (INT32)((INT16)tmp);
				if ((result /= (INT32)((INT16)tmp)) > 0xffff)
				{
					PREFIX(_interrupt)(0);
					break;
				}
				else
				{
					m_regs.w[AX]=result;
					m_regs.w[DX]=tmp2;
				}
			}
			else
			{
				PREFIX(_interrupt)(0);
				break;
			}
		}
		break;
	}
}


static void PREFIX86(_clc)()    /* Opcode 0xf8 */
{
	m_CarryVal = 0;
}

static void PREFIX86(_stc)()    /* Opcode 0xf9 */
{
	m_CarryVal = 1;
}

static void PREFIX86(_cli)()    /* Opcode 0xfa */
{
#ifdef I80286
	if(PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT,0);
#endif
	SetIF(0);
}

static void PREFIX86(_cld)()    /* Opcode 0xfc */
{
	SetDF(0);
}

static void PREFIX86(_std)()    /* Opcode 0xfd */
{
	SetDF(1);
}

static void PREFIX86(_fepre)()    /* Opcode 0xfe */
{
	unsigned ModRM = FETCH;
	unsigned tmp = GetRMByte(ModRM);
	unsigned tmp1;

	if ((ModRM & 0x38) == 0)  /* INC eb */
	{
		tmp1 = tmp+1;
		SetOFB_Add(tmp1,tmp,1);
	}
	else  /* DEC eb */
	{
		tmp1 = tmp-1;
		SetOFB_Sub(tmp1,1,tmp);
	}

	SetAF(tmp1,tmp,1);
	SetSZPF_Byte(tmp1);

	PutbackRMByte(ModRM,(BYTE)tmp1);
}


static void PREFIX86(_ffpre)()    /* Opcode 0xff */
{
	unsigned ModRM = FETCHOP;
	unsigned tmp;
	unsigned tmp1;
	WORD ip;

	switch(ModRM & 0x38)
	{
	case 0x00:  /* INC ew */
		tmp = GetRMWord(ModRM);
		tmp1 = tmp+1;

		SetOFW_Add(tmp1,tmp,1);
		SetAF(tmp1,tmp,1);
		SetSZPF_Word(tmp1);

		PutbackRMWord(ModRM,(WORD)tmp1);
		break;

	case 0x08:  /* DEC ew */
		tmp = GetRMWord(ModRM);
		tmp1 = tmp-1;

		SetOFW_Sub(tmp1,1,tmp);
		SetAF(tmp1,tmp,1);
		SetSZPF_Word(tmp1);

		PutbackRMWord(ModRM,(WORD)tmp1);
		break;

	case 0x10:  /* CALL ew */
		tmp = GetRMWord(ModRM);
		ip = m_pc - m_base[CS];
		PUSH(ip);
		m_pc = (m_base[CS] + (WORD)tmp) & AMASK;
		CHANGE_PC(m_pc);
		break;

	case 0x18:  /* CALL FAR ea */
		tmp = m_sregs[CS];  /* HJB 12/13/98 need to skip displacements of m_ea */
		tmp1 = GetRMWord(ModRM);
		ip = m_pc - m_base[CS];
#ifdef I80286
		i80286_code_descriptor(GetnextRMWord, tmp1, 2);
#else
		m_sregs[CS] = GetnextRMWord;
		m_base[CS] = SegBase(CS);
		m_pc = (m_base[CS] + tmp1) & AMASK;
#endif
		PUSH(tmp);
		PUSH(ip);
		CHANGE_PC(m_pc);
		break;

	case 0x20:  /* JMP ea */
		ip = GetRMWord(ModRM);
		m_pc = (m_base[CS] + ip) & AMASK;
		CHANGE_PC(m_pc);
		break;

	case 0x28:  /* JMP FAR ea */

#ifdef I80286
		tmp = GetRMWord(ModRM);
		i80286_code_descriptor(GetnextRMWord, tmp, 1);
#else
		m_pc = GetRMWord(ModRM);
		m_sregs[CS] = GetnextRMWord;
		m_base[CS] = SegBase(CS);
		m_pc = (m_pc + m_base[CS]) & AMASK;
#endif
		CHANGE_PC(m_pc);
		break;

	case 0x30:  /* PUSH ea */
		tmp = GetRMWord(ModRM);
		PUSH(tmp);
		break;
	default:
		tmp = GetRMWord(ModRM);  // 286 doesn't matter but 8086?
		return PREFIX(_invalid)();
	}
}


static void PREFIX86(_invalid)()
{
	logerror("illegal instruction %.2x at %.5x\n",PEEKBYTE(m_pc-1), m_pc);
#ifdef I80286
	throw TRAP(ILLEGAL_INSTRUCTION,-1);
#else
	/* i8086/i8088 ignore an invalid opcode. */
	/* i80186/i80188 probably also ignore an invalid opcode. */
#endif
}

#ifndef I80286
static void PREFIX86(_invalid_2b)()
{
	unsigned ModRM = FETCH;
	GetRMByte(ModRM);
	logerror("illegal 2 byte instruction %.2x at %.5x\n",PEEKBYTE(m_pc-2), m_pc-2);
}
#endif
#endif
