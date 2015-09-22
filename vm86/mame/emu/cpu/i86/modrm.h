static struct
{
	struct
	{
		WREGS w[256];
		BREGS b[256];
	} reg;
	struct
	{
		WREGS w[256];
		BREGS b[256];
	} RM;
} Mod_RM;

#define RegWord(ModRM) m_regs.w[Mod_RM.reg.w[ModRM]]
#define RegByte(ModRM) m_regs.b[Mod_RM.reg.b[ModRM]]

#define GetRMWord(ModRM) \
	((ModRM) >= 0xc0 ? m_regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), ReadWord( m_ea ) ))

#define PutbackRMWord(ModRM,val) \
{ \
	if (ModRM >= 0xc0) m_regs.w[Mod_RM.RM.w[ModRM]]=val; \
	else WriteWord(m_ea,val); \
}

#define GetnextRMWord ReadWord(m_ea+2)

#define GetRMWordOffset(offs) \
		ReadWord(m_ea-m_eo+(UINT16)(m_eo+offs))

#define GetRMByteOffset(offs) \
		ReadByte(m_ea-m_eo+(UINT16)(m_eo+offs))

#define PutRMWord(ModRM,val)                \
{                                           \
	if (ModRM >= 0xc0)                      \
		m_regs.w[Mod_RM.RM.w[ModRM]]=val;   \
	else {                                  \
		(*GetEA[ModRM])();                  \
		WriteWord( m_ea ,val);              \
	}                                       \
}

#define PutRMWordOffset(offs, val) \
		WriteWord( m_ea-m_eo+(UINT16)(m_eo+offs), val)

#define PutRMByteOffset(offs, val) \
		WriteByte( m_ea-m_eo+(UINT16)(m_eo+offs), val)

#define PutImmRMWord(ModRM)                 \
{                                           \
	WORD val;                               \
	if (ModRM >= 0xc0)                      \
		FETCHWORD(m_regs.w[Mod_RM.RM.w[ModRM]]) \
	else {                                  \
		(*GetEA[ModRM])();                  \
		FETCHWORD(val)                      \
		WriteWord( m_ea , val);             \
	}                                       \
}

#define GetRMByte(ModRM) \
	((ModRM) >= 0xc0 ? m_regs.b[Mod_RM.RM.b[ModRM]] : ReadByte( (*GetEA[ModRM])() ))

#define PutRMByte(ModRM,val)                \
{                                           \
	if (ModRM >= 0xc0)                      \
		m_regs.b[Mod_RM.RM.b[ModRM]]=val;   \
	else                                    \
		WriteByte( (*GetEA[ModRM])() ,val); \
}

#define PutImmRMByte(ModRM)                 \
{                                           \
	if (ModRM >= 0xc0)                      \
		m_regs.b[Mod_RM.RM.b[ModRM]]=FETCH; \
	else {                                  \
		(*GetEA[ModRM])();                  \
		WriteByte( m_ea , FETCH );          \
	}                                       \
}

#define PutbackRMByte(ModRM,val)            \
{                                           \
	if (ModRM >= 0xc0)                      \
		m_regs.b[Mod_RM.RM.b[ModRM]]=val;   \
	else                                    \
		WriteByte(m_ea,val);                    \
}

#define DEF_br8(dst,src)                    \
	unsigned ModRM = FETCHOP;               \
	unsigned src = RegByte(ModRM);          \
	unsigned dst = GetRMByte(ModRM)

#define DEF_wr16(dst,src)                   \
	unsigned ModRM = FETCHOP;               \
	unsigned src = RegWord(ModRM);          \
	unsigned dst = GetRMWord(ModRM)

#define DEF_r8b(dst,src)                    \
	unsigned ModRM = FETCHOP;               \
	unsigned dst = RegByte(ModRM);          \
	unsigned src = GetRMByte(ModRM)

#define DEF_r16w(dst,src)                   \
	unsigned ModRM = FETCHOP;               \
	unsigned dst = RegWord(ModRM);          \
	unsigned src = GetRMWord(ModRM)

#define DEF_ald8(dst,src)                   \
	unsigned src = FETCHOP;                 \
	unsigned dst = m_regs.b[AL]

#define DEF_axd16(dst,src)                  \
	unsigned src = FETCHOP;                 \
	unsigned dst = m_regs.w[AX];            \
	src += (FETCH << 8)
