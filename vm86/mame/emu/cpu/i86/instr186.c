/****************************************************************************
*             real mode i286 emulator v1.4 by Fabrice Frances               *
*               (initial work based on David Hedley's pcemu)                *
****************************************************************************/

// file will be included in all cpu variants
// function renaming will be added when necessary
// timing value should move to separate array

static void PREFIX186(_pusha)()    /* Opcode 0x60 */
{
	unsigned tmp=m_regs.w[SP];

#ifdef I80286
	if(PM) i80286_check_permission(SS, m_regs.w[SP]-16, 16, I80286_WRITE);
#endif
	PUSH(m_regs.w[AX]);
	PUSH(m_regs.w[CX]);
	PUSH(m_regs.w[DX]);
	PUSH(m_regs.w[BX]);
	PUSH(tmp);
	PUSH(m_regs.w[BP]);
	PUSH(m_regs.w[SI]);
	PUSH(m_regs.w[DI]);
}

static unsigned i186_popa_tmp;  // hack around GCC 4.6 error because we need the side effects of POP
static void PREFIX186(_popa)()    /* Opcode 0x61 */
{
#ifdef I80286
	if(PM) i80286_check_permission(SS, m_regs.w[SP], 16, I80286_READ);
#endif
	POP(m_regs.w[DI]);
	POP(m_regs.w[SI]);
	POP(m_regs.w[BP]);
	POP(i186_popa_tmp);
	POP(m_regs.w[BX]);
	POP(m_regs.w[DX]);
	POP(m_regs.w[CX]);
	POP(m_regs.w[AX]);
}

static void PREFIX186(_bound)()    /* Opcode 0x62 */
{
	unsigned ModRM = FETCHOP;
	int low = (INT16)GetRMWord(ModRM);
	int high= (INT16)GetnextRMWord;
	int tmp= (INT16)RegWord(ModRM);
	if (tmp<low || tmp>high) {
		m_pc-= ( m_seg_prefix ? 3 : 2 );
		PREFIX86(_interrupt)(5);
	}
}

static void PREFIX186(_push_d16)()    /* Opcode 0x68 */
{
	unsigned tmp = FETCH;

	tmp += FETCH << 8;
	PUSH(tmp);
}

static void PREFIX186(_imul_d16)()    /* Opcode 0x69 */
{
	DEF_r16w(dst,src);
	unsigned src2=FETCH;
	src2+=(FETCH<<8);

	dst = (INT32)((INT16)src)*(INT32)((INT16)src2);
	m_CarryVal = m_OverVal = (((INT32)dst) >> 15 != 0) && (((INT32)dst) >> 15 != -1);
	RegWord(ModRM)=(WORD)dst;
}


static void PREFIX186(_push_d8)()    /* Opcode 0x6a */
{
	unsigned tmp = (WORD)((INT16)((INT8)FETCH));

	PUSH(tmp);
}

static void PREFIX186(_imul_d8)()    /* Opcode 0x6b */
{
	DEF_r16w(dst,src);
	unsigned src2= (WORD)((INT16)((INT8)FETCH));

	dst = (INT32)((INT16)src)*(INT32)((INT16)src2);
	m_CarryVal = m_OverVal = (((INT32)dst) >> 15 != 0) && (((INT32)dst) >> 15 != -1);
	RegWord(ModRM)=(WORD)dst;
}

static void PREFIX186(_insb)()    /* Opcode 0x6c */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	PutMemB(ES,m_regs.w[DI],read_port_byte(m_regs.w[DX]));
	m_regs.w[DI] += m_DirVal;
}

static void PREFIX186(_insw)()    /* Opcode 0x6d */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	PutMemW(ES,m_regs.w[DI],read_port_word(m_regs.w[DX]));
	m_regs.w[DI] += 2 * m_DirVal;
}

static void PREFIX186(_outsb)()    /* Opcode 0x6e */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	write_port_byte(m_regs.w[DX],GetMemB(DS,m_regs.w[SI]));
	m_regs.w[SI] += m_DirVal; /* GOL 11/27/01 */
}

static void PREFIX186(_outsw)()    /* Opcode 0x6f */
{
#ifdef I80286
	if (PM && (CPL>IOPL)) throw TRAP(GENERAL_PROTECTION_FAULT, 0);
#endif
	write_port_word(m_regs.w[DX],GetMemW(DS,m_regs.w[SI]));
	m_regs.w[SI] += 2 * m_DirVal; /* GOL 11/27/01 */
}

static void PREFIX186(_rotshft_bd8)()    /* Opcode 0xc0 */
{
	unsigned ModRM = FETCH;
	unsigned src = GetRMByte(ModRM);
	unsigned count = FETCH;

	PREFIX86(_rotate_shift_Byte)(ModRM,count & 0x1f,src);
}

static void PREFIX186(_rotshft_wd8)()    /* Opcode 0xc1 */
{
	unsigned ModRM = FETCH;
	unsigned src = GetRMWord(ModRM);
	unsigned count = FETCH;

	PREFIX86(_rotate_shift_Word)(ModRM,count & 0x1f,src);
}

static void PREFIX186(_enter)()    /* Opcode 0xc8 */
{
	unsigned nb = FETCH;
	unsigned i,level;
	UINT16 fp;

	nb += FETCH << 8;
#ifdef I80286
	level = FETCH & 0x1f;
	if(PM) i80286_check_permission(SS, m_regs.w[SP]-2-(level*2), 2+(level*2), I80286_WRITE);
#else
	level = FETCH;
#endif
	PUSH(m_regs.w[BP]);
	fp = m_regs.w[SP];
	for (i=1;i<level;i++)
		PUSH(GetMemW(SS,m_regs.w[BP]-i*2));
	if (level) PUSH(fp);
	m_regs.w[BP] = fp;
	m_regs.w[SP] -= nb;
}

static void PREFIX186(_leave)()    /* Opcode 0xc9 */
{
	m_regs.w[SP]=m_regs.w[BP];
	POP(m_regs.w[BP]);
}

static void PREFIX186(_rotshft_bcl)()    /* Opcode 0xd2 */
{
	unsigned ModRM = FETCHOP;
	PREFIX86(_rotate_shift_Byte)(ModRM,m_regs.b[CL] & 0x1f,GetRMByte(ModRM));
}

static void PREFIX186(_rotshft_wcl)()    /* Opcode 0xd3 */
{
	unsigned ModRM = FETCHOP;
	PREFIX86(_rotate_shift_Word)(ModRM,m_regs.b[CL] & 0x1f,GetRMWord(ModRM));
}
