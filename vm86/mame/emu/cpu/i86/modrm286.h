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
	((ModRM) >= 0xc0 ? m_regs.w[Mod_RM.RM.w[ModRM]] : ( (*GetEA[ModRM])(), i80286_check_permission(m_ea_seg, m_eo, I80286_WORD, I80286_READ), ReadWord( m_ea ) ))

#define PutbackRMWord(ModRM,val)                                                                        \
{                                                                                                       \
	if (ModRM >= 0xc0) m_regs.w[Mod_RM.RM.w[ModRM]]=val;                                        \
	else                                                                                                \
	{                                                                                                   \
		i80286_check_permission(m_ea_seg, m_eo, I80286_WORD, I80286_WRITE);   \
		WriteWord(m_ea,val);                                                                    \
	}                                                                                                   \
}

#define GetnextRMWord                                                                                                                   \
	(                                                                                                                                   \
	i80286_check_permission(m_ea_seg, m_ea + 2 - m_base[m_ea_seg], I80286_WORD, I80286_READ), \
	ReadWord(m_ea+2)                                                                                                            \
	)

#define GetRMWordOffset(offs)                                                                                       \
		(                                                                                                           \
		i80286_check_permission(m_ea_seg, (UINT16)(m_eo+offs), I80286_WORD, I80286_READ), \
		ReadWord(m_ea-m_eo+(UINT16)(m_eo+offs))                                             \
		)

#define GetRMByteOffset(offs)                                                                                       \
		(                                                                                                           \
		i80286_check_permission(m_ea_seg, (UINT16)(m_eo+offs), I80286_BYTE, I80286_READ), \
		ReadByte(m_ea-m_eo+(UINT16)(m_eo+offs))                                             \
		)

#define PutRMWord(ModRM,val)                                                                            \
{                                                                                                       \
	if (ModRM >= 0xc0)                                                                                  \
		m_regs.w[Mod_RM.RM.w[ModRM]]=val;                                                       \
	else {                                                                                              \
		(*GetEA[ModRM])();                                                                      \
		i80286_check_permission(m_ea_seg, m_eo, I80286_WORD, I80286_WRITE);   \
		WriteWord( m_ea ,val);                                                                  \
	}                                                                                                   \
}

#define PutRMWordOffset(offs, val)                                                                                      \
		i80286_check_permission(m_ea_seg, (UINT16)(m_eo+offs), I80286_WORD, I80286_WRITE);    \
		WriteWord( m_ea-m_eo+(UINT16)(m_eo+offs), val)

#define PutRMByteOffset(offs, val)                                                                                      \
		i80286_check_permission(m_ea_seg, (UINT16)(m_eo+offs), I80286_BYTE, I80286_WRITE);    \
		WriteByte( m_ea-m_eo+(UINT16)(m_eo+offs), val)

#define PutImmRMWord(ModRM)                                                                             \
{                                                                                                       \
	WORD val;                                                                                           \
	if (ModRM >= 0xc0)                                                                                  \
		FETCHWORD(m_regs.w[Mod_RM.RM.w[ModRM]])                                                 \
	else {                                                                                              \
		(*GetEA[ModRM])();                                                                      \
		i80286_check_permission(m_ea_seg, m_eo, I80286_WORD, I80286_WRITE);   \
		FETCHWORD(val)                                                                                  \
		WriteWord( m_ea , val);                                                                 \
	}                                                                                                   \
}

#define GetRMByte(ModRM) \
	((ModRM) >= 0xc0 ? m_regs.b[Mod_RM.RM.b[ModRM]] : ( (*GetEA[ModRM])(), i80286_check_permission(m_ea_seg, m_eo, I80286_BYTE, I80286_READ), ReadByte( m_ea )) )

#define PutRMByte(ModRM,val)                                                                            \
{                                                                                                       \
	if (ModRM >= 0xc0)                                                                                  \
		m_regs.b[Mod_RM.RM.b[ModRM]]=val;                                                       \
	else                                                                                                \
	{                                                                                                   \
		(*GetEA[ModRM])();                                                                      \
		i80286_check_permission(m_ea_seg, m_eo, I80286_BYTE, I80286_WRITE);   \
		WriteByte( m_ea,val);                                                                   \
	}                                                                                                   \
}

#define PutImmRMByte(ModRM)                                                                             \
{                                                                                                       \
	if (ModRM >= 0xc0)                                                                                  \
		m_regs.b[Mod_RM.RM.b[ModRM]]=FETCH;                                                     \
	else {                                                                                              \
		(*GetEA[ModRM])();                                                                      \
		i80286_check_permission(m_ea_seg, m_eo, I80286_BYTE, I80286_WRITE);   \
		WriteByte( m_ea , FETCH );                                                              \
	}                                                                                                   \
}

#define PutbackRMByte(ModRM,val)                                                                        \
{                                                                                                       \
	if (ModRM >= 0xc0)                                                                                  \
		m_regs.b[Mod_RM.RM.b[ModRM]]=val;                                                       \
	else                                                                                                \
	{                                                                                                   \
		i80286_check_permission(m_ea_seg, m_eo, I80286_BYTE, I80286_WRITE);   \
		WriteByte(m_ea,val);                                                                    \
	}                                                                                                   \
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
	unsigned dst = m_regs.w[AX];    \
	src += (FETCH << 8)
