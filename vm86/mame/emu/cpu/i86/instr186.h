/****************************************************************************
*             real mode i286 emulator v1.4 by Fabrice Frances               *
*               (initial work based on David Hedley's pcemu)                *
****************************************************************************/

// file will be included in all cpu variants
// timing value should move to separate array

static void PREFIX186(_pusha)();
static void PREFIX186(_popa)();
static void PREFIX186(_bound)();
static void PREFIX186(_push_d16)();
static void PREFIX186(_imul_d16)();
static void PREFIX186(_push_d8)();
static void PREFIX186(_imul_d8)();
static void PREFIX186(_rotshft_bd8)();
static void PREFIX186(_rotshft_wd8)();
static void PREFIX186(_enter)();
static void PREFIX186(_leave)();
static void PREFIX186(_insb)();
static void PREFIX186(_insw)();
static void PREFIX186(_outsb)();
static void PREFIX186(_outsw)();

/* changed instructions */
static void PREFIX(_pop_ss)();
static void PREFIX(_es)();
static void PREFIX(_cs)();
static void PREFIX(_ss)();
static void PREFIX(_ds)();
static void PREFIX(_mov_sregw)();
static void PREFIX186(_repne)();
static void PREFIX186(_repe)();
static void PREFIX186(_sti)();
static void PREFIX186(_rotshft_bcl)();
static void PREFIX186(_rotshft_wcl)();
