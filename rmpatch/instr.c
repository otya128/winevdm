/*
 * Emulation of privileged instructions
 *
 * Copyright 1995 Alexandre Julliard
 * Copyright 2005 Ivan Leo Puoti
 * Copyright 2005 Laurent Pinchart
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; ifnot, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

// modified to only apply to real mode windows programs running in protected mode

#include "windef.h"
#include "winbase.h"
#include "wine/winuser16.h"
#include "wine/exception.h"

/* macros to set parts of a DWORD */
#define SET_LOWORD(dw,val)  ((dw) = ((dw) & 0xffff0000) | LOWORD(val))
#define SET_LOBYTE(dw,val)  ((dw) = ((dw) & 0xffffff00) | LOBYTE(val))
#define ADD_LOWORD(dw,val)  ((dw) = ((dw) & 0xffff0000) | LOWORD((DWORD)(dw)+(val)))

static WORD dosmem_0040H = 0; 
static WORD dosmem_0000H = 0;

static inline void add_stack(CONTEXT *context, int offset)
{
    ADD_LOWORD(context->Esp, offset);
}

static inline void *make_ptr(CONTEXT *context, DWORD seg, DWORD off, int long_addr)
{
    return (char *) MapSL(MAKESEGPTR(seg, off));
}

static inline void *get_stack(CONTEXT *context)
{
    return (char *) MapSL(MAKESEGPTR(context->SegSs, context->Esp));
}

/***********************************************************************
 *           INSTR_ReplaceSelector
 *
 * Try to replace an invalid selector by a valid one.
 * The only selector where it is allowed to do "mov ax,40;mov es,ax"
 * is the so called 'bimodal' selector 0x40, which points to the BIOS
 * data segment. Used by (at least) Borland products (and programs compiled
 * using Borland products).
 *
 * See Undocumented Windows, Chapter 5, __0040.
 */
static BOOL INSTR_ReplaceSelector(CONTEXT *context, WORD *sel)
{
    if(*sel == 0x40)
    {
        DOSVM_start_bios_timer();
        *sel = dosmem_0040H;
        return TRUE;
    }
    *sel = dosmem_0000H; // xerox presents does a use after free with a global memory block
    return TRUE;
}

static WORD get_reg_word(CONTEXT *context, BYTE regmodrm)
{
    WORD ret;
    switch((regmodrm >> 3) & 7)
    {
    case 0: ret = context->Eax; break;
    case 1: ret = context->Ecx; break;
    case 2: ret = context->Edx; break;
    case 3: ret = context->Ebx; break;
    case 4: ret = context->Esp; break;
    case 5: ret = context->Ebp; break;
    case 6: ret = context->Esi; break;
    case 7: ret = context->Edi; break;
    }
    return ret;
}

static BYTE get_reg_byte(CONTEXT *context, BYTE regmodrm)
{
    BYTE ret;
    switch((regmodrm >> 3) & 7)
    {
    case 0: ret = context->Eax; break;
    case 1: ret = context->Ecx; break;
    case 2: ret = context->Edx; break;
    case 3: ret = context->Ebx; break;
    case 4: ret = context->Eax >> 8; break;
    case 5: ret = context->Ecx >> 8; break;
    case 6: ret = context->Edx >> 8; break;
    case 7: ret = context->Ebx >> 8; break;
    }
    return ret;
}

/* store an operand into a register */
static void store_reg_word(CONTEXT *context, BYTE regmodrm, const BYTE *addr)
{
    WORD data;
    __TRY
    {
        data = *(const WORD *)addr;
    }
    __EXCEPT_ALL
    {
        data = 0;
    }
    __ENDTRY
    switch((regmodrm >> 3) & 7)
    {
    case 0:
        SET_LOWORD(context->Eax, data);
        break;
    case 1:
        SET_LOWORD(context->Ecx, data);
        break;
    case 2:
        SET_LOWORD(context->Edx, data);
        break;
    case 3:
        SET_LOWORD(context->Ebx, data);
        break;
    case 4:
        SET_LOWORD(context->Esp, data);
        break;
    case 5:
        SET_LOWORD(context->Ebp, data);
        break;
    case 6:
        SET_LOWORD(context->Esi, data);
        break;
    case 7:
        SET_LOWORD(context->Edi, data);
        break;
    }
}

/* store an operand into a byte register */
static void store_reg_byte(CONTEXT *context, BYTE regmodrm, const BYTE *addr)
{
    BYTE data;
    __TRY
    {
        data = *addr;
    }
    __EXCEPT_ALL
    {
        data = 0;
    }
    __ENDTRY
    switch((regmodrm >> 3) & 7)
    {
    case 0: context->Eax = (context->Eax & 0xffffff00) | data; break;
    case 1: context->Ecx = (context->Ecx & 0xffffff00) | data; break;
    case 2: context->Edx = (context->Edx & 0xffffff00) | data; break;
    case 3: context->Ebx = (context->Ebx & 0xffffff00) | data; break;
    case 4: context->Eax = (context->Eax & 0xffff00ff) | (data << 8); break;
    case 5: context->Ecx = (context->Ecx & 0xffff00ff) | (data << 8); break;
    case 6: context->Edx = (context->Edx & 0xffff00ff) | (data << 8); break;
    case 7: context->Ebx = (context->Ebx & 0xffff00ff) | (data << 8); break;
    }
}

static BYTE *get_addr(int seg, int base)
{
    LDT_ENTRY entry;
    /* Make sure the segment and offset are valid */
    if(wine_ldt_is_system(seg)) return (BYTE *)base;
    if((seg & 7) != 7) return NULL;
    wine_ldt_get_entry(seg, &entry);
    if(wine_ldt_is_empty(&entry)) return NULL;
    if(wine_ldt_get_limit(&entry) < base) return NULL;
    return (BYTE *)wine_ldt_get_base(&entry) + base;
}

/***********************************************************************
 *           INSTR_GetOperandAddr
 *
 * Return the address of an instruction operand (from the mod/rm byte).
 */
static BYTE *INSTR_GetOperandAddr(CONTEXT *context, BYTE *instr, int segprefix, int *len)
{
    int mod, rm, base = 0, seg = 0, off;

#define GET_VAL(val,type) \
    { *val = *(type *)instr; instr += sizeof(type); *len += sizeof(type); }

    *len = 0;
    GET_VAL(&mod, BYTE);
    rm = mod & 7;
    mod >>= 6;

    if(mod == 3)
    {
        switch(rm)
        {
        case 0: return (BYTE *)&context->Eax;
        case 1: return (BYTE *)&context->Ecx;
        case 2: return (BYTE *)&context->Edx;
        case 3: return (BYTE *)&context->Ebx;
        case 4: return (BYTE *)&context->Esp;
        case 5: return (BYTE *)&context->Ebp;
        case 6: return (BYTE *)&context->Esi;
        case 7: return (BYTE *)&context->Edi;
        }
    }

    switch(rm)
    {
    case 0:  /* ds:(bx,si) */
        base = LOWORD(context->Ebx) + LOWORD(context->Esi);
        seg  = context->SegDs;
        break;
    case 1:  /* ds:(bx,di) */
        base = LOWORD(context->Ebx) + LOWORD(context->Edi);
        seg  = context->SegDs;
        break;
    case 2:  /* ss:(bp,si) */
        base = LOWORD(context->Ebp) + LOWORD(context->Esi);
        seg  = context->SegSs;
        break;
    case 3:  /* ss:(bp,di) */
        base = LOWORD(context->Ebp) + LOWORD(context->Edi);
        seg  = context->SegSs;
        break;
    case 4:  /* ds:(si) */
        base = LOWORD(context->Esi);
        seg  = context->SegDs;
        break;
    case 5:  /* ds:(di) */
        base = LOWORD(context->Edi);
        seg  = context->SegDs;
        break;
    case 6:  /* ss:(bp) */
        base = LOWORD(context->Ebp);
        seg  = context->SegSs;
        break;
    case 7:  /* ds:(bx) */
        base = LOWORD(context->Ebx);
        seg  = context->SegDs;
        break;
    }

    switch(mod)
    {
    case 0:
        if(rm == 6)  /* special case: ds:(disp16) */
        {
            GET_VAL(&base, WORD);
            seg  = context->SegDs;
        }
        break;

    case 1:  /* 8-bit disp */
        GET_VAL(&off, BYTE);
        base += (signed char)off;
        break;

    case 2:  /* 16-bit disp */
        GET_VAL(&off, WORD);
        base += (signed short)off;
        break;
    }
    base &= 0xffff;

    if(segprefix != -1) seg = segprefix;

    if(!seg) seg = dosmem_0000H;

    return get_addr(seg, base);
#undef GET_VAL
}


/***********************************************************************
 *           INSTR_EmulateLDS
 *
 * Emulate the LDS instruction.
 */
static BOOL INSTR_EmulateLDS(CONTEXT *context, BYTE *instr, int segprefix, int *len)
{
    WORD seg;
    BYTE *regmodrm = instr + 1;
    BYTE *addr = INSTR_GetOperandAddr(context, regmodrm, segprefix, len);
    if(!addr)
        return FALSE;  /* Unable to emulate it */
    seg = *(WORD *)(addr + 2);

    if(!INSTR_ReplaceSelector(context, &seg))
        return FALSE;  /* Unable to emulate it */

    /* Now store the offset in the correct register */

    store_reg_word(context, *regmodrm, addr);

    /* Store the correct segment in the segment register */

    switch(*instr)
    {
    case 0xc4: context->SegEs = seg; break;  /* les */
    case 0xc5: context->SegDs = seg; break;  /* lds */
    }

    /* Add the opcode size to the total length */

    *len += 1;
    return TRUE;
}

/***********************************************************************
 *           rmpatch_emulate_instruction
 *
 * Emulate a privileged instruction.
 * Returns exception continuation status.
 */
BOOL rmpatch_emulate_instruction(CONTEXT *context)
{
    int prefix, segprefix, prefixlen, len, repX;
    BYTE *instr;

    if(!dosmem_0000H)
        dosmem_0000H = (WORD)GetProcAddress16(GetModuleHandle16("KERNEL"), (LPCSTR)183);
    if(!dosmem_0040H)
        dosmem_0040H = (WORD)GetProcAddress16(GetModuleHandle16("KERNEL"), (LPCSTR)193);

    instr = make_ptr(context, context->SegCs, context->Eip, TRUE);
    if(!instr) return FALSE;

    /* First handle any possible prefix */

    segprefix = -1;  /* no prefix */
    prefix = 1;
    repX = 0;
    prefixlen = 0;
    while(prefix)
    {
        switch(*instr)
        {
        case 0x2e:
            segprefix = context->SegCs;
            break;
        case 0x36:
            segprefix = context->SegSs;
            break;
        case 0x3e:
            segprefix = context->SegDs;
            break;
        case 0x26:
            segprefix = context->SegEs;
            if (!segprefix)
            {
                context->SegEs = dosmem_0000H;
                return;
            }
            break;
        case 0x64:
            break;
        case 0xf0:  /* lock */
	    break;
        case 0xf2:  /* repne */
	    repX = 1;
	    break;
        case 0xf3:  /* repe */
	    repX = 2;
            break;
        default:
            prefix = 0;  /* no more prefixes */
            break;
        }
        if(prefix)
        {
            instr++;
            prefixlen++;
        }
    }

    /* Now look at the actual instruction */

    switch(*instr)
    {
        case 0x07: /* pop es */
        case 0x17: /* pop ss */
        case 0x1f: /* pop ds */
            {
                WORD seg = *(WORD *)get_stack(context);
                if(INSTR_ReplaceSelector(context, &seg))
                {
                    switch(*instr)
                    {
                    case 0x07: context->SegEs = seg; break;
                    case 0x17: context->SegSs = seg; break;
                    case 0x1f: context->SegDs = seg; break;
                    }
                    add_stack(context, 2);
                    context->Eip += prefixlen + 1;
                    return TRUE;
                }
            }
            break;  /* Unable to emulate it */

        case 0x2b:
            {
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                if (addr) // this is likely unnecessary as there's no other way this can be reached                 
                    break;
                context->Eip += prefixlen + len + 1;
                return TRUE;
            }                

        case 0xfe:
            {
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                switch (instr[1] & 0x38)
                {
                    case 0:
                        *addr++;
                        break;
                    case 8:
                        *addr--;
                        break;
                    default:
                        return FALSE;
                }
                context->Eip += prefixlen + len + 1;
                return TRUE;
            }                

        case 0x88: /* mov Gb, Eb */
        case 0x89: /* mov Gv, Ev */
            {
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                switch (*instr)
                {
                    case 0x88: *addr = get_reg_byte(context, instr[1]); break;
                    case 0x89: *(WORD *)addr = get_reg_word(context, instr[1]); break;
                }
                context->Eip += prefixlen + len + 1;
                return TRUE;
            }
            break;  /* Unable to emulate it */

        case 0x8a: /* mov Eb, Gb */
        case 0x8b: /* mov Ev, Gv */
            {
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                switch (*instr)
                {
                    case 0x8a: store_reg_byte(context, instr[1], addr); break;
                    case 0x8b: store_reg_word(context, instr[1], addr); break;
                }
                context->Eip += prefixlen + len + 1;
                return TRUE;
            }
            break;  /* Unable to emulate it */

        case 0x8c:
            {
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                DWORD seg;
                switch((instr[1] >> 3) & 7)
                {
                    case 0:
                        seg = context->SegEs;
                        break;
                    case 1:
                        seg = context->SegCs;
                        break;
                    case 2:
                        seg = context->SegSs;
                        break;
                    case 3:
                        seg = context->SegDs;
                        break;
                    default:
                        seg = -1;
                        break;
                }
                if(seg != -1)
                {
                    *(WORD *)addr = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                }
            }
            break;  /* Unable to emulate it */


        case 0x8e: /* mov XX,segment_reg */
            {
                WORD seg;
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                if(!addr)
                    break;  /* Unable to emulate it */
                seg = *(WORD *)addr;
                if(!INSTR_ReplaceSelector(context, &seg))
                    break;  /* Unable to emulate it */

                switch((instr[1] >> 3) & 7)
                {
                case 0:
                    context->SegEs = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                case 1:  /* cs */
                    break;
                case 2:
                    context->SegSs = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                case 3:
                    context->SegDs = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                case 4:
                    context->SegFs = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                case 5:
                    context->SegGs = seg;
                    context->Eip += prefixlen + len + 1;
                    return TRUE;
                case 6:  /* unused */
                case 7:  /* unused */
                    break;
                }
            }
            break;  /* Unable to emulate it */

        case 0x8f: /* pop Ev */
            {
                if (instr[1] & 0x38)
                    break;
                WORD data = *(WORD *)get_stack(context);
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                *(WORD *)addr = data;
                add_stack(context, 2);
                context->Eip += prefixlen + len + 1;
                return TRUE;
            }

        case 0xa2:
        case 0xa3:
            {
                BYTE *addr = get_addr((segprefix != -1) ? segprefix : context->SegDs, *(WORD *)(instr + 1));
                if(addr)
                {
                    if(*instr == 0xa3)
                        *(WORD *)addr = context->Eax;
                    else
                        *addr = context->Eax;
                    context->Eip += prefixlen + 3;
                    return TRUE;
                }
                break;
            }

        case 0xc4: /* les addr,reg */
        case 0xc5: /* lds addr,reg */
            if(INSTR_EmulateLDS(context, instr, segprefix, &len))
            {
                context->Eip += prefixlen + len;
                return TRUE;
            }
            break;  /* Unable to emulate it */

        case 0xc6: /* mov Eb, Ib */
        case 0xc7: /* mov Ev, Iv */
            {
                if (instr[1] & 0x38)
                    break;
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                __TRY
                {
                    switch (*instr)
                    {
                        case 0xc6: *addr = instr[2]; break;
                        case 0xc7: *(WORD *)addr = instr[2] | (instr[3] << 8); break;
                    }
                }
                __EXCEPT_ALL
                {
                }
                __ENDTRY
                context->Eip += prefixlen + ((*instr == 0xc7) ? 2 : 1) + len + 1;
                return TRUE;
            }
            break;  /* Unable to emulate it */

        case 0xff:
            {
                if ((instr[1] & 0x38) != 0x18)
                    break;
                BYTE *addr = INSTR_GetOperandAddr(context, instr + 1, segprefix, &len);
                // this is a standard pattern for MSC _int86
                if (*(WORD *)(addr + 2) != context->SegSs)
                    break;
                static WORD stackcode = NULL;
                if (!stackcode)
                    stackcode = AllocDStoCSAlias16(context->SegSs);
                *(WORD *)(addr + 2) = stackcode;
                return TRUE;
            }
            break;

    }
    return FALSE;
}

