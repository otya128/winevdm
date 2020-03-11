#pragma once
#include <stdio.h>
#include <windows.h>
#define WHV_PARTITION_HANDLE WHV_PARTITION_HANDLE32 /* hack! */
#include <WinHvPlatformDefs.h>
#undef WHV_PARTITION_HANDLE
typedef UINT64 WHV_PARTITION_HANDLE;
#include <WinHvPlatform.h>
struct whpx_vcpu_state
{
    union
    {
        struct
        {
            WHV_REGISTER_VALUE rax;
            WHV_REGISTER_VALUE rcx;
            WHV_REGISTER_VALUE rdx;
            WHV_REGISTER_VALUE rbx;
            WHV_REGISTER_VALUE rsp;
            WHV_REGISTER_VALUE rbp;
            WHV_REGISTER_VALUE rsi;
            WHV_REGISTER_VALUE rdi;
            WHV_REGISTER_VALUE rip;
            WHV_REGISTER_VALUE rflags;
            WHV_REGISTER_VALUE es;
            WHV_REGISTER_VALUE cs;
            WHV_REGISTER_VALUE ss;
            WHV_REGISTER_VALUE ds;
            WHV_REGISTER_VALUE fs;
            WHV_REGISTER_VALUE gs;
            WHV_REGISTER_VALUE ldtr;
            WHV_REGISTER_VALUE tr;
            WHV_REGISTER_VALUE idtr;
            WHV_REGISTER_VALUE gdtr;
            WHV_REGISTER_VALUE cr0;
            WHV_REGISTER_VALUE cr3;
            WHV_REGISTER_VALUE dr0;
            WHV_REGISTER_VALUE dr1;
            WHV_REGISTER_VALUE dr2;
            WHV_REGISTER_VALUE dr3;
            WHV_REGISTER_VALUE dr6;
            WHV_REGISTER_VALUE dr7;
        };
        WHV_REGISTER_VALUE values[28];
    };
};

typedef UINT16 uint16;
typedef UINT32 uint32;
typedef UINT64 uint64;
static WHV_REGISTER_NAME whpx_vcpu_reg_names[] =
{
        WHvX64RegisterRax,
        WHvX64RegisterRcx,
        WHvX64RegisterRdx,
        WHvX64RegisterRbx,
        WHvX64RegisterRsp,
        WHvX64RegisterRbp,
        WHvX64RegisterRsi,
        WHvX64RegisterRdi,
        WHvX64RegisterRip,
        WHvX64RegisterRflags,
        WHvX64RegisterEs,
        WHvX64RegisterCs,
        WHvX64RegisterSs,
        WHvX64RegisterDs,
        WHvX64RegisterFs,
        WHvX64RegisterGs,
        WHvX64RegisterLdtr,
        WHvX64RegisterTr,
        WHvX64RegisterIdtr,
        WHvX64RegisterGdtr,
        WHvX64RegisterCr0,
        WHvX64RegisterCr3,
        WHvX64RegisterDr0,
        WHvX64RegisterDr1,
        WHvX64RegisterDr2,
        WHvX64RegisterDr3,
        WHvX64RegisterDr6,
        WHvX64RegisterDr7,
};

/* register utils */
typedef struct whpx_vcpu_state vcpu_state;
typedef WHV_X64_SEGMENT_REGISTER segment;
/* 16-bit regs */
inline uint16 get_ax(vcpu_state *state)
{
    return state->rax.Reg16;
}

inline uint16 get_cx(vcpu_state *state)
{
    return state->rcx.Reg16;
}

inline uint16 get_dx(vcpu_state *state)
{
    return state->rdx.Reg16;
}

inline uint16 get_bx(vcpu_state *state)
{
    return state->rbx.Reg16;
}

inline uint16 get_sp(vcpu_state *state)
{
    return state->rsp.Reg16;
}

inline uint16 get_bp(vcpu_state *state)
{
    return state->rbp.Reg16;
}

inline uint16 get_si(vcpu_state *state)
{
    return state->rsi.Reg16;
}

inline uint16 get_di(vcpu_state *state)
{
    return state->rdi.Reg16;
}

inline uint16 get_ip(vcpu_state *state)
{
    return state->rip.Reg16;
}

inline uint16 get_flags(vcpu_state *state)
{
    return state->rflags.Reg16;
}

inline uint16 set_ax(vcpu_state *state, uint16 val)
{
    return state->rax.Reg16 = val;
}

inline uint16 set_cx(vcpu_state *state, uint16 val)
{
    return state->rcx.Reg16 = val;
}

inline uint16 set_dx(vcpu_state *state, uint16 val)
{
    return state->rdx.Reg16 = val;
}

inline uint16 set_bx(vcpu_state *state, uint16 val)
{
    return state->rbx.Reg16 = val;
}

inline uint16 set_sp(vcpu_state *state, uint16 val)
{
    return state->rsp.Reg16 = val;
}

inline uint16 set_bp(vcpu_state *state, uint16 val)
{
    return state->rbp.Reg16 = val;
}

inline uint16 set_si(vcpu_state *state, uint16 val)
{
    return state->rsi.Reg16 = val;
}

inline uint16 set_di(vcpu_state *state, uint16 val)
{
    return state->rdi.Reg16 = val;
}

inline uint16 set_ip(vcpu_state *state, uint16 val)
{
    return state->rip.Reg16 = val;
}

inline segment *get_es(vcpu_state *state)
{
    return &state->es;
}

inline segment *get_cs(vcpu_state *state)
{
    return &state->cs;
}

inline segment *get_ss(vcpu_state *state)
{
    return &state->ss;
}

inline segment *get_ds(vcpu_state *state)
{
    return &state->ds;
}

inline segment *get_fs(vcpu_state *state)
{
    return &state->fs;
}

inline segment *get_gs(vcpu_state *state)
{
    return &state->gs;
}

inline uint16 get_seg_selector(segment *seg)
{
    return seg->Selector;
}

inline uint32 get_seg_base(segment *seg)
{
    return (uint32)seg->Base;
}

inline uint32 get_seg_limit(segment *seg)
{
    return seg->Limit;
}

/* 32-bit regs */

inline uint32 get_eax(vcpu_state *state)
{
    return state->rax.Reg32;
}

inline uint32 get_ecx(vcpu_state *state)
{
    return state->rcx.Reg32;
}

inline uint32 get_edx(vcpu_state *state)
{
    return state->rdx.Reg32;
}

inline uint32 get_ebx(vcpu_state *state)
{
    return state->rbx.Reg32;
}

inline uint32 get_esp(vcpu_state *state)
{
    return state->rsp.Reg32;
}

inline uint32 get_ebp(vcpu_state *state)
{
    return state->rbp.Reg32;
}

inline uint32 get_esi(vcpu_state *state)
{
    return state->rsi.Reg32;
}

inline uint32 get_edi(vcpu_state *state)
{
    return state->rdi.Reg32;
}

inline uint32 get_eip(vcpu_state *state)
{
    return state->rip.Reg32;
}

inline uint32 get_eflags(vcpu_state *state)
{
    return state->rflags.Reg32;
}

inline uint32 set_eax(vcpu_state *state, uint32 val)
{
    return state->rax.Reg32 = val;
}

inline uint32 set_ecx(vcpu_state *state, uint32 val)
{
    return state->rcx.Reg32 = val;
}

inline uint32 set_edx(vcpu_state *state, uint32 val)
{
    return state->rdx.Reg32 = val;
}

inline uint32 set_ebx(vcpu_state *state, uint32 val)
{
    return state->rbx.Reg32 = val;
}

inline uint32 set_esp(vcpu_state *state, uint32 val)
{
    return state->rsp.Reg32 = val;
}

inline uint32 set_ebp(vcpu_state *state, uint32 val)
{
    return state->rbp.Reg32 = val;
}

inline uint32 set_esi(vcpu_state *state, uint32 val)
{
    return state->rsi.Reg32 = val;
}

inline uint32 set_edi(vcpu_state *state, uint32 val)
{
    return state->rdi.Reg32 = val;
}

inline uint32 set_eip(vcpu_state *state, uint32 val)
{
    return state->rip.Reg32 = val;
}
