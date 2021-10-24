#pragma once
#include <stdint.h>
#include "gvm-interface.h"
#include <stdio.h>
#include <windows.h>

struct gvm_vcpu_state
{
    struct gvm_regs regs;
    struct gvm_sregs sregs;
};

typedef UINT16 uint16;
typedef UINT32 uint32;
typedef UINT64 uint64;
/* register utils */
typedef struct gvm_vcpu_state vcpu_state;
typedef struct gvm_segment segment;
/* 16-bit regs */
static inline uint16 get_ax(vcpu_state *state)
{
    return (uint16)state->regs.rax;
}

static inline uint16 get_cx(vcpu_state *state)
{
    return (uint16)state->regs.rcx;
}

static inline uint16 get_dx(vcpu_state *state)
{
    return (uint16)state->regs.rdx;
}

static inline uint16 get_bx(vcpu_state *state)
{
    return (uint16)state->regs.rbx;
}

static inline uint16 get_sp(vcpu_state *state)
{
    return (uint16)state->regs.rsp;
}

static inline uint16 get_bp(vcpu_state *state)
{
    return (uint16)state->regs.rbp;
}

static inline uint16 get_si(vcpu_state *state)
{
    return (uint16)state->regs.rsi;
}

static inline uint16 get_di(vcpu_state *state)
{
    return (uint16)state->regs.rdi;
}

static inline uint16 get_ip(vcpu_state *state)
{
    return (uint16)state->regs.rip;
}

static inline uint16 get_flags(vcpu_state *state)
{
    return (uint16)state->regs.rflags;
}

static inline uint16 set_ax(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rax = val;
}

static inline uint16 set_cx(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rcx = val;
}

static inline uint16 set_dx(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rdx = val;
}

static inline uint16 set_bx(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rbx = val;
}

static inline uint16 set_sp(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rsp = val;
}

static inline uint16 set_bp(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rbp = val;
}

static inline uint16 set_si(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rsi = val;
}

static inline uint16 set_di(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rdi = val;
}

static inline uint16 set_ip(vcpu_state *state, uint16 val)
{
    return *(uint16*)&state->regs.rip = val;
}

static inline segment *get_es(vcpu_state *state)
{
    return &state->sregs.es;
}

static inline segment *get_cs(vcpu_state *state)
{
    return &state->sregs.cs;
}

static inline segment *get_ss(vcpu_state *state)
{
    return &state->sregs.ss;
}

static inline segment *get_ds(vcpu_state *state)
{
    return &state->sregs.ds;
}

static inline segment *get_fs(vcpu_state *state)
{
    return &state->sregs.fs;
}

static inline segment *get_gs(vcpu_state *state)
{
    return &state->sregs.gs;
}

static inline uint16 get_seg_selector(segment *seg)
{
    return seg->selector;
}

static inline uint32 get_seg_base(segment *seg)
{
    return (uint32)seg->base;
}

static inline uint32 get_seg_limit(segment *seg)
{
    return seg->limit;
}

/* 32-bit regs */

static inline uint32 get_eax(vcpu_state *state)
{
    return (uint32)state->regs.rax;
}

static inline uint32 get_ecx(vcpu_state *state)
{
    return (uint32)state->regs.rcx;
}

static inline uint32 get_edx(vcpu_state *state)
{
    return (uint32)state->regs.rdx;
}

static inline uint32 get_ebx(vcpu_state *state)
{
    return (uint32)state->regs.rbx;
}

static inline uint32 get_esp(vcpu_state *state)
{
    return (uint32)state->regs.rsp;
}

static inline uint32 get_ebp(vcpu_state *state)
{
    return (uint32)state->regs.rbp;
}

static inline uint32 get_esi(vcpu_state *state)
{
    return (uint32)state->regs.rsi;
}

static inline uint32 get_edi(vcpu_state *state)
{
    return (uint32)state->regs.rdi;
}

static inline uint32 get_eip(vcpu_state *state)
{
    return (uint32)state->regs.rip;
}

static inline uint32 get_eflags(vcpu_state *state)
{
    return (uint32)state->regs.rflags;
}

static inline uint32 set_eax(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rax = val;
}

static inline uint32 set_ecx(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rcx = val;
}

static inline uint32 set_edx(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rdx = val;
}

static inline uint32 set_ebx(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rbx = val;
}

static inline uint32 set_esp(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rsp = val;
}

static inline uint32 set_ebp(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rbp = val;
}

static inline uint32 set_esi(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rsi = val;
}

static inline uint32 set_edi(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rdi = val;
}

static inline uint32 set_eip(vcpu_state *state, uint32 val)
{
    return *(uint32*)&state->regs.rip = val;
}
