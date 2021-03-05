/*
 * Copyright 1993 Bob Amstadt
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdlib.h>
#include "windef.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "wine/debug.h"
#include "wownt32.h"
#include "../krnl386/dosexe.h"
/*
#define __asm__(xxx)
#define __asm__
#define __volatile__(xxx)
*/
WINE_DEFAULT_DEBUG_CHANNEL(int);

//VM86.DLL
typedef void(*fldcw_t)(WORD);
typedef void(*fldsw_t)(WORD);
typedef void(*wait_t)();
typedef void(*fninit_t)();
typedef void(*fstcw_t)(WORD*);
typedef void(*fstsw_t)(WORD*);
typedef void(*frndint_t)();
typedef void(*fclex_t)();
typedef void(*fsave_t)(char*);
typedef void(*frstor_t)(const char*);
typedef void(*fstenv32_t)(char*);
typedef DWORD(*fistp_t)(WORD);

void fldcw_stub(WORD a)
{
    FIXME("stub\n");
}
void wait_stub()
{
    FIXME("stub\n");
}
void fninit_stub()
{
    FIXME("stub\n");
}
void fstcw_stub(WORD* a)
{
    FIXME("stub\n");
}
void fstsw_stub(WORD* a)
{
    FIXME("stub\n");
}
void frndint_stub()
{
    FIXME("stub\n");
}
void fclex_stub()
{
    FIXME("stub\n");
}
void fsave_stub(char* a)
{
    FIXME("stub\n");
}
void fstenv32_stub(char* a)
{
    FIXME("stub\n");
}
void frstor_stub(const char* a)
{
    FIXME("stub\n");
}
DWORD fistp_stub(WORD round)
{
    FIXME("stub\n");
    return 0;
}
typedef struct
{
    fldcw_t fldcw;
    wait_t wait;
    fninit_t fninit;
    fstcw_t fstcw;
    fstsw_t fstsw;
    frndint_t frndint;
    fclex_t fclex;
    fsave_t fsave;
    frstor_t frstor;
    fstenv32_t fstenv32;
    fistp_t fistp;
} x87function;
x87function x87;
static INTPROC oldproc = 0;

typedef void (*load_x87function_t)(x87function *func);
static void WINAPI fpu_exception(CONTEXT *context);
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_DETACH)
        DOSVM_SetBuiltinVector(2, oldproc);
    if (fdwReason != DLL_PROCESS_ATTACH)
        return TRUE;
    char dllname[MAX_PATH];
    load_x87function_t load_x87function;
    krnl386_get_config_string("otvdm", "vm", "vm86.dll", dllname, sizeof(dllname));
    HMODULE vm = LoadLibraryA(dllname);
    load_x87function = (load_x87function_t)GetProcAddress(vm, "load_x87function");
    if (load_x87function)
        load_x87function(&x87);

    if (!x87.fldcw)
        x87.fldcw = fldcw_stub;
    if (!x87.wait)
        x87.wait = wait_stub;
    if (!x87.fninit)
        x87.fninit = fninit_stub;
    if (!x87.fstcw)
        x87.fstcw = fstcw_stub;
    if (!x87.fstsw)
        x87.fstsw = fstsw_stub;
    if (!x87.frndint)
        x87.frndint = frndint_stub;
    if (!x87.fclex)
        x87.fclex = fclex_stub;
    if (!x87.fsave)
        x87.fsave = fsave_stub;
    if (!x87.frstor)
        x87.frstor = frstor_stub;
    if (!x87.fistp)
        x87.fistp = fistp_stub;
    if (!x87.fstenv32)
        x87.fstenv32 = fstenv32_stub;
    oldproc = DOSVM_SetBuiltinVector(2, fpu_exception);
    return TRUE;
}
#define USE_VM86_DLL 1
#include <pshpack1.h>
struct Win87EmInfoStruct
{
    unsigned short Version;
    unsigned short SizeSaveArea;
    unsigned short WinDataSeg;
    unsigned short WinCodeSeg;
    unsigned short Have80x87;
    unsigned short Unused;
};
#include <poppack.h>

/* Implementing this is easy cause Linux and *BSD* ALWAYS have a numerical
 * coprocessor. (either real or emulated on kernellevel)
 */
/* win87em.dll also sets interrupt vectors: 2 (NMI), 0x34 - 0x3f (emulator
 * calls of standard libraries, see Ralph Browns interrupt list), 0x75
 * (int13 error reporting of coprocessor)
 */

/* have a look at /usr/src/linux/arch/i386/math-emu/ *.[ch] for more info
 * especially control_w.h and status_w.h
 */
/* FIXME: Still rather skeletal implementation only */

static WORD RefCount = 0;
static WORD CtrlWord_1 = 0;
static WORD CtrlWord_2 = 0;
static WORD CtrlWord_Internal = 0;
static WORD StatusWord_1 = 0x000b;
static WORD StatusWord_2 = 0;
static WORD StackTop = 175;
static WORD StackBottom = 0;
static WORD Inthandler02hVar = 1;

// the fpe handler is documented as not needing to return
// if a program longjmps out we don't want an extra stale context in the call stack 
// so clean everything up and use a trampoline for the return
void WINAPI fpe_return(CONTEXT *context)
{
    WORD *stkptr = (WORD *)MapSL(MAKESEGPTR(context->SegSs, LOWORD(context->Esp)));
    context->SegDs = stkptr[0];
    context->Eax = stkptr[3];
    context->Ebp = stkptr[4] - 1;
    context->Eip = stkptr[5];
    context->SegCs = stkptr[6];
    context->EFlags = stkptr[7];
    context->Esp += 16;
}

static void WINAPI fpu_exception(CONTEXT *context)
{
    UINT32 fpenv[7];

    // check if program sets own nmi handler, maybe irq13 also
    //FARPROC16 handler2 = DOSVM_GetPMHandler16(2);

    // we need to use the 32bit fstenv because the pmode fpenv
    // doesn't contain the fp code or data segments in later intel cpus
    // so fstenv32 is the only way to get the exception opcode 
    // and also makes it hard/impossible to fix errors with memory operands
    x87.fstenv32((char *)&fpenv);
    x87.fclex();
    StatusWord_2 |= fpenv[1];
    FARPROC16 handler3e = DOSVM_GetPMHandler16(0x3e);
    if (!handler3e)
    {
        x87.fldcw(fpenv[0]);
        return;
    }
    WORD opcode = fpenv[4] >> 16;
    WORD unmasked = ~fpenv[0] & fpenv[1] & 0x3f;
    int errcode = 0;
    // we don't attempt to fix errors that the real win87em would
    // currently, just check the error type for the first exception
    if (unmasked & 1) // EM_INVALID
    {
        errcode = 0x81; // FPE_INVALID
        if (fpenv[1] & 0x40)
        {
            if (fpenv[1] & 0x200)
                errcode = 0x8a; // FPE_STACKOVERFLOW
            else
                errcode = 0x8b; // FPE_STACKUNDERFLOW
        }
        else if (opcode == 0x1fa)
            errcode = 0x88; // FPE_SQRTNEG
    }
    else if (unmasked & 2) // EM_DENORMAL
        errcode = 0x82;
    else if (unmasked & 4) // EM_ZERODIVIDE
        errcode = 0x83;
    else if (unmasked & 8) // EM_OVERFLOW
        errcode = 0x84;
    else if (unmasked & 0x10) // EM_UNDERFLOW
        errcode = 0x85;
    else if (unmasked & 0x20) // EM_INEXACT
        errcode = 0x86;
    // Watcom expects ds to be on the stack above the ret addr
    // VB3 expects bp to point to a previous bp which points to the fault context ax and bp
    context->Esp -= 20;
    DWORD new_sp = MAKESEGPTR(context->SegSs, LOWORD(context->Esp));
    WORD *stack = (WORD *)MapSL(new_sp);
    static FARPROC16 trampoline16 = 0;
    if (!trampoline16)
	trampoline16 = GetProcAddress16(GetModuleHandle16("WIN87EM"), "fpe_return");
    stack[9] = context->EFlags;
    stack[8] = context->SegCs;
    stack[7] = context->Eip;
    stack[6] = context->Ebp + 1;
    stack[5] = context->Eax;
    stack[4] = context->SegDs;
    stack[3] = OFFSETOF(new_sp) + 12 + 1;
    stack[2] = context->SegDs;
    stack[1] = SELECTOROF(trampoline16);
    stack[0] = OFFSETOF(trampoline16);
    context->EFlags |= 0x200; // orig win87em unconditionally sets IF
    context->SegCs = HIWORD(handler3e);
    context->Eip = LOWORD(handler3e);
    context->Eax = errcode;
    context->Ebp = OFFSETOF(new_sp) + 6;
    x87.fldcw(fpenv[0]);
} 

static void WIN87_ClearStatus( CONTEXT *context )
{
    context->Eax &= ~0xffff;  /* set AX to 0 */
#if USE_VM86_DLL
    x87.fclex();
#else/*USE_VM86_DLL*/
#ifdef __i386__
#ifndef _MSC_VER
    __asm__("fclex");
#else
    __asm
    {
            fclex
    }
#endif
#endif
#endif/*USE_VM86_DLL*/
    StatusWord_2 = 0;
}

static void WIN87_SetCtrlWord( CONTEXT *context )
{
    CtrlWord_1 = LOWORD(context->Eax);
    context->Eax &= ~0x00c3;
    CtrlWord_Internal = LOWORD(context->Eax);
#if USE_VM86_DLL
    x87.fldcw(CtrlWord_Internal);
#else/*USE_VM86_DLL*/
#ifdef __i386__
#ifndef _MSC_VER
    __asm__("wait;fldcw %0" : : "m" (CtrlWord_Internal));
#else
    __asm
    {
            wait;
            fldcw CtrlWord_Internal
    }
#endif
#endif/*USE_VM86_DLL*/
#endif
    CtrlWord_2 = LOWORD(context->Eax);
}

static void WIN87_Init( CONTEXT *context )
{
#if USE_VM86_DLL
        x87.fninit();
        x87.fninit();
#else/*USE_VM86_DLL*/
#ifdef __i386__
#ifndef _MSC_VER
        __asm__("fninit");
        __asm__("fninit");
#else
        __asm
        {
                fninit
                        fninit
        }
#endif
#endif
#endif/*USE_VM86_DLL*/
    StackBottom = StackTop;
    context->Eax = (context->Eax & ~0xffff) | 0x1332;
    WIN87_SetCtrlWord(context);
    WIN87_ClearStatus(context);
}

/***********************************************************************
 *		_fpMath (WIN87EM.1)
 */
void WINAPI _fpMath( CONTEXT *context )
{
    TRACE("(cs:eip=%04x:%04x es=%04x bx=%04x ax=%04x dx=%04x)\n",
          context->SegCs, context->Eip, context->SegEs, context->Ebx,
          context->Eax, context->Edx );

    switch(LOWORD(context->Ebx))
    {
    case 0: /* install (increase instanceref) emulator, install NMI vector */
        RefCount++;
#if 0
        InstallIntVecs02hAnd75h();
#endif
        WIN87_Init(context);
        context->Eax &= ~0xffff;  /* set AX to 0 */
        break;

    case 1: /* Init Emulator */
        WIN87_Init(context);
        break;

    case 2: /* deinstall emulator (decrease instanceref), deinstall NMI vector
             * if zero. Every '0' call should have a matching '2' call.
             */
        WIN87_Init(context);
	RefCount--;
#if 0
        if (!RefCount)
            RestoreInt02h();
#endif

        break;

    case 3: /* The fpe handler is stored in int 3eh but the handler returns
             * with a retf so it can't be called with an int instruction
             * the old handler is saved and restored by the program
             * the tdb contains a per-task int 3eh handler
             */
        DOSVM_SetPMHandler16(0x3e, (FARPROC16)MAKESEGPTR(context->Edx, context->Eax));
        break;

    case 4: /* set control word (& ~(CW_Denormal|CW_Invalid)) */
        /* OUT: newset control word in AX */
        WIN87_SetCtrlWord(context);
        break;

    case 5: /* return internal control word in AX */
        context->Eax = (context->Eax & ~0xffff) | CtrlWord_1;
        break;

    case 6: /* round top of stack to integer using method AX & 0x0C00 */
        /* returns current controlword */
        {
            WORD save,mask;
            /* I don't know much about asm() programming. This could be
             * wrong.
             */
#if USE_VM86_DLL
			x87.fstcw(&save);
			mask = (save & ~0xc00) | (context->Eax & 0xc00);
			x87.fldcw(mask);
			x87.frndint();
			x87.fldcw(save);
#else/*USE_VM86_DLL*/
#ifdef __i386__
#ifndef _MSC_VER
			__asm__ __volatile__("fstcw %0;wait" : "=m" (save) : : "memory");
			__asm__ __volatile__("fstcw %0;wait" : "=m" (mask) : : "memory");
			__asm__ __volatile__("orw $0xC00,%0" : "=m" (mask) : : "memory");
			__asm__ __volatile__("fldcw %0;wait" : : "m" (mask));
			__asm__ __volatile__("frndint");
#else
			__asm
			{
				fstcw save
				wait
				fstcw mask
				wait
				or mask, 0C00h
				fldcw mask
				wait
				frndint
			}
#endif
#endif
#endif/*USE_VM86_DLL*/
        }
        break;

    case 7: /* POP top of stack as integer into DX:AX */
        /* IN: AX&0x0C00 rounding protocol */
        /* OUT: DX:AX variable popped */
        {
            DWORD dw=x87.fistp((context->Eax >> 10) & 3);
            /* I don't know much about asm() programming. This could be
             * wrong.
             */
/* FIXME: could someone who really understands asm() fix this please? --AJ */
/*            __asm__("fistp %0;wait" : "=m" (dw) : : "memory"); */
            TRACE("On top of stack was %d\n",dw);
            context->Eax = (context->Eax & ~0xffff) | LOWORD(dw);
            context->Edx = (context->Edx & ~0xffff) | HIWORD(dw);
        }
        break;

    case 8: /* restore internal status words from emulator status word */
        context->Eax &= ~0xffff;  /* set AX to 0 */
#if USE_VM86_DLL
      x87.fstsw(&StatusWord_1);
#else/*USE_VM86_DLL*/
#ifdef __i386__
#ifndef _MSC_VER
        __asm__("fstsw %0;wait" : "=m" (StatusWord_1));
#else
        __asm
        {
                fstsw StatusWord_1
                wait
        }
#endif
#endif
#endif/*USE_VM86_DLL*/
        context->Eax |= StatusWord_1 & 0x3f;
        context->Eax = (context->Eax | StatusWord_2) & ~0xe000;
        StatusWord_2 = LOWORD(context->Eax);
        break;

    case 9: /* clear fpu exceptions and some other things */
        WIN87_ClearStatus(context);
        break;

    case 10: /* dunno. but looks like returning nr. of things on stack in AX */
    {
        UINT32 fpustate[7];
        WORD count = 0;
        x87.fstenv32(&fpustate);
        WORD tagword = fpustate[2];
        for (int i = 0; i < 8; i++)
            count += (((tagword >> (i * 2)) & 3) != 3);
        context->Eax = (context->Eax & ~0xffff) | count;
        break;
    }
    case 11: /* just returns the installed flag in DX:AX */
        context->Edx &= ~0xffff;  /* set DX to 0 */
        context->Eax = (context->Eax & ~0xffff) | 1; // fpu is always available
        break;

    case 12: /* save AX in some internal state var */
        Inthandler02hVar = LOWORD(context->Eax);
        break;

    default: /* error. Say that loud and clear */
        FIXME("unhandled switch %d\n",LOWORD(context->Ebx));
        context->Eax |= 0xffff;
        context->Edx |= 0xffff;
        break;
    }
}

SEGPTR WINAPI K32WOWGlobalLock16(HGLOBAL16 handle);
static HGLOBAL16 hGlobalD;
static HGLOBAL16 hGlobalC;
static WORD D;
static WORD C;
/***********************************************************************
 *		__WinEm87Info (WIN87EM.3)
 */
void WINAPI __WinEm87Info(struct Win87EmInfoStruct *pWIS, int cbWin87EmInfoStruct)
{
    if (!hGlobalD)
    {
        hGlobalD = GlobalAlloc16(0, 65535);
        hGlobalC = GlobalAlloc16(0, 65535);
        D = K32WOWGlobalLock16(hGlobalD) >> 16;
        C = K32WOWGlobalLock16(hGlobalC) >> 16;
    }
    if (cbWin87EmInfoStruct != sizeof(*pWIS))
    {
        ERR("(%p,%d) %d != sizeof(struct Win87EmInfoStruct)!\n", pWIS, cbWin87EmInfoStruct, cbWin87EmInfoStruct);
    }
    FIXME("(%p,%d), stub !\n",pWIS,cbWin87EmInfoStruct);
    //WOW32
    pWIS->Version = 0x0600;
    pWIS->SizeSaveArea= 0x1D5;
    pWIS->Have80x87 = TRUE;
    pWIS->Unused = 0;
    //unknown
    pWIS->WinCodeSeg = C;
    pWIS->WinDataSeg = D;
    return 0;
}

/***********************************************************************
 *		__WinEm87Restore (WIN87EM.4)
 */
void WINAPI __WinEm87Restore(void *pWin87EmSaveArea, int cbWin87EmSaveArea)
{
  FIXME("(%p,%d), stub !\n",
	pWin87EmSaveArea,cbWin87EmSaveArea);
}

/***********************************************************************
 *		__WinEm87Save (WIN87EM.5)
 */
void WINAPI __WinEm87Save(void *pWin87EmSaveArea, int cbWin87EmSaveArea)
{
  FIXME("(%p,%d), stub !\n",
	pWin87EmSaveArea,cbWin87EmSaveArea);
}
