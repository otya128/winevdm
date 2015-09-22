static unsigned EA_000() { m_eo=(WORD)(m_regs.w[BX]+m_regs.w[SI]); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_001() { m_eo=(WORD)(m_regs.w[BX]+m_regs.w[DI]); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_002() { m_eo=(WORD)(m_regs.w[BP]+m_regs.w[SI]); m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+m_eo; return m_ea; }
static unsigned EA_003() { m_eo=(WORD)(m_regs.w[BP]+m_regs.w[DI]); m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+m_eo; return m_ea; }
static unsigned EA_004() { m_eo=m_regs.w[SI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_005() { m_eo=m_regs.w[DI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_006() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_007() { m_eo=m_regs.w[BX]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }

static unsigned EA_100() { m_eo=(WORD)(m_regs.w[BX]+m_regs.w[SI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_101() { m_eo=(WORD)(m_regs.w[BX]+m_regs.w[DI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_102() { m_eo=(WORD)(m_regs.w[BP]+m_regs.w[SI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+m_eo; return m_ea; }
static unsigned EA_103() { m_eo=(WORD)(m_regs.w[BP]+m_regs.w[DI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+m_eo; return m_ea; }
static unsigned EA_104() { m_eo=(WORD)(m_regs.w[SI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_105() { m_eo=(WORD)(m_regs.w[DI]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }
static unsigned EA_106() { m_eo=(WORD)(m_regs.w[BP]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+m_eo; return m_ea; }
static unsigned EA_107() { m_eo=(WORD)(m_regs.w[BX]+(INT8)FETCHOP); m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+m_eo; return m_ea; }

static unsigned EA_200() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BX]+m_regs.w[SI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+(WORD)m_eo; return m_ea; }
static unsigned EA_201() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BX]+m_regs.w[DI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+(WORD)m_eo; return m_ea; }
static unsigned EA_202() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BP]+m_regs.w[SI]; m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+(WORD)m_eo; return m_ea; }
static unsigned EA_203() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BP]+m_regs.w[DI]; m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+(WORD)m_eo; return m_ea; }
static unsigned EA_204() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[SI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+(WORD)m_eo; return m_ea; }
static unsigned EA_205() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[DI]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+(WORD)m_eo; return m_ea; }
static unsigned EA_206() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BP]; m_ea_seg=DefaultSeg(SS); m_ea=DefaultBase(SS)+(WORD)m_eo; return m_ea; }
static unsigned EA_207() { m_eo=FETCHOP; m_eo+=FETCHOP<<8; m_eo+=m_regs.w[BX]; m_ea_seg=DefaultSeg(DS); m_ea=DefaultBase(DS)+(WORD)m_eo; return m_ea; }

static unsigned (*const GetEA[192])()={
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,
	EA_000, EA_001, EA_002, EA_003, EA_004, EA_005, EA_006, EA_007,

	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,
	EA_100, EA_101, EA_102, EA_103, EA_104, EA_105, EA_106, EA_107,

	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207,
	EA_200, EA_201, EA_202, EA_203, EA_204, EA_205, EA_206, EA_207
};
