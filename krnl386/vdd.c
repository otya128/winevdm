#include <windows.h>
#include "kernel16_private.h"
#include "dosexe.h"
#include "windows/wownt32.h"

typedef struct
{
    HMODULE hvdd;
    FARPROC dispatch;
} vdd_module_t;

static vdd_module_t vdd_modules[5] = {0};
static CONTEXT *last_context = NULL;

void vdd_req(char func, CONTEXT *context)
{
    /*
     * RegisterModule
     * DS:SI DLL
     * ES:DI init func name
     * DS:BX dispatch routine name
     */
    if (func == 0x00)
    {
        LPCSTR dll = (LPCSTR)CTX_SEG_OFF_TO_LIN(context, context->SegDs, context->Esi);
        LPCSTR init = (LPCSTR)CTX_SEG_OFF_TO_LIN(context, context->SegEs, context->Edi);
        LPCSTR dispatch = (LPCSTR)CTX_SEG_OFF_TO_LIN(context, context->SegDs, context->Ebx);
        context->Eip += 4;
        HMODULE hVdd = LoadLibraryA(dll);
        if (!hVdd)
        {
            SET_CFLAG(context);
            SET_AX(context, GetLastError());
            return;
        }
        FARPROC pfnInit = GetProcAddress(hVdd, init);
        FARPROC pfnDispatch = GetProcAddress(hVdd, dispatch);
        int i;
        for (i = 0; i < 5; i++)
        {
            if (!vdd_modules[i].hvdd)
            {
                vdd_modules[i].hvdd = hVdd;
                vdd_modules[i].dispatch = pfnDispatch;
                break;
            }
        }
        if (i == 5)
        {
            FreeLibrary(hVdd);
            SET_CFLAG(context);
            SET_AX(context, 4);
            return;
        }            
        RESET_CFLAG(context);
        SET_AX(context, i);
        if (pfnInit)
        {
            pfnInit();
        }
    }
    /* UnregisterModule */
    else if (func == 0x01)
    {
        WORD handle = LOWORD(context->Eax);
        context->Eip += 4;
        if ((handle > 5) || !vdd_modules[handle].hvdd)
            return; // ntvdm exits here
        FreeLibrary(vdd_modules[handle].hvdd);
        vdd_modules[handle].hvdd = 0;
    }
    /* DispatchCall */
    else if (func == 0x02)
    {
        WORD handle = LOWORD(context->Eax);
        context->Eip += 4;
        if ((handle > 5) || !vdd_modules[handle].hvdd)
            return; // ntvdm exits here
        last_context = context;
        vdd_modules[handle].dispatch();
        last_context = NULL;
    }
}

#define GET_LO_BYTE_REG(reg) ((BYTE)(last_context ? last_context->reg : 0))
#define GET_HI_BYTE_REG(reg) ((BYTE)((last_context ? last_context->reg : 0) >> 8))
#define GET_WORD_REG(reg) ((WORD)(last_context ? last_context->reg : 0))
#define GET_DWORD_REG(reg) (last_context ? last_context->reg : 0)

#define SET_LO_BYTE_REG(reg, val) { if (last_context) last_context->reg = ((last_context->reg & ~0xff) | val); }
#define SET_HI_BYTE_REG(reg, val) { if (last_context) last_context->reg = ((last_context->reg & ~0xff00) | (val << 8)); }
#define SET_WORD_REG(reg, val) { if (last_context) last_context->reg = ((last_context->reg & ~0xffff) | val); }
#define SET_DWORD_REG(reg, val) { if (last_context) last_context->reg = val; }

#define GET_FLAG(bit) (last_context ? (last_context->EFlags >> bit) & 1 : 0)
#define SET_FLAG(bit, val) { if (last_context) last_context->EFlags = ((last_context->EFlags & ~(1 << bit)) | ((val & 1) << bit)); }

BYTE WINAPI getAL()
{
    return GET_LO_BYTE_REG(Eax);
}

BYTE WINAPI getAH()
{
    return GET_HI_BYTE_REG(Eax);
}

WORD WINAPI getAX()
{
    return GET_WORD_REG(Eax);
}

DWORD WINAPI getEAX()
{
    return GET_DWORD_REG(Eax);
}

BYTE WINAPI getBL()
{
    return GET_LO_BYTE_REG(Ebx);
}

BYTE WINAPI getBH()
{
    return GET_HI_BYTE_REG(Ebx);
}

WORD WINAPI getBX()
{
    return GET_WORD_REG(Ebx);
}

DWORD WINAPI getEBX()
{
    return GET_DWORD_REG(Ebx);
}

BYTE WINAPI getCL()
{
    return GET_LO_BYTE_REG(Ecx);
}

BYTE WINAPI getCH()
{
    return GET_HI_BYTE_REG(Ecx);
}

WORD WINAPI getCX()
{
    return GET_WORD_REG(Ecx);
}

DWORD WINAPI getECX()
{
    return GET_DWORD_REG(Ecx);
}

BYTE WINAPI getDL()
{
    return GET_LO_BYTE_REG(Edx);
}

BYTE WINAPI getDH()
{
    return GET_HI_BYTE_REG(Edx);
}

WORD WINAPI getDX()
{
    return GET_WORD_REG(Edx);
}

DWORD WINAPI getEDX()
{
    return GET_DWORD_REG(Edx);
}

WORD WINAPI getSP()
{
    return GET_WORD_REG(Esp);
}

DWORD WINAPI getESP()
{
    return GET_DWORD_REG(Esp);
}

WORD WINAPI getBP()
{
    return GET_WORD_REG(Ebp);
}

DWORD WINAPI getEBP()
{
    return GET_DWORD_REG(Ebp);
}

WORD WINAPI getSI()
{
    return GET_WORD_REG(Esi);
}

DWORD WINAPI getESI()
{
    return GET_DWORD_REG(Esi);
}

WORD WINAPI getDI()
{
    return GET_WORD_REG(Edi);
}

DWORD WINAPI getEDI()
{
    return GET_DWORD_REG(Edi);
}

void WINAPI setAL(BYTE val)
{
    SET_LO_BYTE_REG(Eax, val);
}

void WINAPI setAH(BYTE val)
{
    SET_HI_BYTE_REG(Eax, val);
}

void WINAPI setAX(WORD val)
{
    SET_WORD_REG(Eax, val);
}

void WINAPI setEAX(DWORD val)
{
    SET_DWORD_REG(Eax, val);
}

void WINAPI setBL(BYTE val)
{
    SET_LO_BYTE_REG(Ebx, val);
}

void WINAPI setBH(BYTE val)
{
    SET_HI_BYTE_REG(Ebx, val);
}

void WINAPI setBX(WORD val)
{
    SET_WORD_REG(Ebx, val);
}

void WINAPI setEBX(DWORD val)
{
    SET_DWORD_REG(Ebx, val);
}

void WINAPI setCL(BYTE val)
{
    SET_LO_BYTE_REG(Ecx, val);
}

void WINAPI setCH(BYTE val)
{
    SET_HI_BYTE_REG(Ecx, val);
}

void WINAPI setCX(DWORD val)
{
    SET_WORD_REG(Ecx, val);
}

void WINAPI setECX(BYTE val)
{
    SET_DWORD_REG(Ecx, val);
}

void WINAPI setDL(BYTE val)
{
    SET_LO_BYTE_REG(Edx, val);
}

void WINAPI setDH(BYTE val)
{
    SET_HI_BYTE_REG(Edx, val);
}

void WINAPI setDX(WORD val)
{
    SET_WORD_REG(Edx, val);
}

void WINAPI setEDX(DWORD val)
{
    SET_DWORD_REG(Edx, val);
}

void WINAPI setSP(WORD val)
{
    SET_WORD_REG(Esp, val);
}

void WINAPI setESP(DWORD val)
{
    SET_DWORD_REG(Esp, val);
}

void WINAPI setBP(WORD val)
{
    SET_WORD_REG(Ebp, val);
}

void WINAPI setEBP(DWORD val)
{
    SET_DWORD_REG(Ebp, val);
}

void WINAPI setSI(WORD val)
{
    SET_WORD_REG(Esi, val);
}

void WINAPI setESI(DWORD val)
{
    SET_DWORD_REG(Esi, val);
}

void WINAPI setDI(WORD val)
{
    SET_WORD_REG(Edi, val);
}

void WINAPI setEDI(DWORD val)
{
    SET_DWORD_REG(Edi, val);
}

WORD WINAPI getDS()
{
    return GET_WORD_REG(SegDs);
}

WORD WINAPI getES()
{
    return GET_WORD_REG(SegEs);
}

WORD WINAPI getCS()
{
    return GET_WORD_REG(SegCs);
}

WORD WINAPI getSS()
{
    return GET_WORD_REG(SegSs);
}

WORD WINAPI getFS()
{
    return GET_WORD_REG(SegFs);
}

WORD WINAPI getGS()
{
    return GET_WORD_REG(SegGs);
}

void WINAPI setDS(WORD val)
{
    SET_WORD_REG(SegDs, val);
}

void WINAPI setES(WORD val)
{
    SET_WORD_REG(SegEs, val);
}

void WINAPI setCS(WORD val)
{
    SET_WORD_REG(SegCs, val);
}

void WINAPI setSS(WORD val)
{
    SET_WORD_REG(SegSs, val);
}

void WINAPI setFS(WORD val)
{
    SET_WORD_REG(SegFs, val);
}

void WINAPI setGS(WORD val)
{
    SET_WORD_REG(SegGs, val);
}

WORD WINAPI getIP()
{
    return GET_WORD_REG(Eip);
}

WORD WINAPI getEIP()
{
    return GET_DWORD_REG(Eip);
}

void WINAPI setIP(WORD val)
{
    SET_WORD_REG(Eip, val);
}

void WINAPI setEIP(DWORD val)
{
    SET_DWORD_REG(Eip, val);
}

DWORD WINAPI getCF()
{
    return GET_FLAG(0);
}

DWORD WINAPI getPF()
{
    return GET_FLAG(2);
}

DWORD WINAPI getAF()
{
    return GET_FLAG(4);
}

DWORD WINAPI getZF()
{
    return GET_FLAG(6);
}

DWORD WINAPI getSF()
{
    return GET_FLAG(7);
}

DWORD WINAPI getIF()
{
    return GET_FLAG(9);
}

DWORD WINAPI getDF()
{
    return GET_FLAG(10);
}

DWORD WINAPI getOF()
{
    return GET_FLAG(11);
}

void WINAPI setCF(DWORD val)
{
    SET_FLAG(0, val);
}

void WINAPI setPF(DWORD val)
{
    SET_FLAG(2, val);
}

void WINAPI setAF(DWORD val)
{
    SET_FLAG(4, val);
}

void WINAPI setZF(DWORD val)
{
    SET_FLAG(6, val);
}

void WINAPI setSF(DWORD val)
{
    SET_FLAG(7, val);
}

void WINAPI setIF(DWORD val)
{
    SET_FLAG(9, val);
}

void WINAPI setDF(DWORD val)
{
    SET_FLAG(10, val);
}

void WINAPI setOF(DWORD val)
{
    SET_FLAG(11, val);
}

typedef VOID (WINAPI *PFNVDD_INB)(WORD iport, PBYTE data);
typedef VOID (WINAPI *PFNVDD_INW)(WORD iport, PWORD data);
typedef VOID (WINAPI *PFNVDD_INSB)(WORD iport, PBYTE data, WORD count);
typedef VOID (WINAPI *PFNVDD_INSW)(WORD iport, PWORD data, WORD count);
typedef VOID (WINAPI *PFNVDD_OUTB)(WORD iport, BYTE data);
typedef VOID (WINAPI *PFNVDD_OUTW)(WORD iport, WORD data);
typedef VOID (WINAPI *PFNVDD_OUTSB)(WORD iport, PBYTE data, WORD count);
typedef VOID (WINAPI *PFNVDD_OUTSW)(WORD iport, PWORD data, WORD count);

typedef struct _VDD_IO_HANDLERS
{
  PFNVDD_INB inb_handler;
  PFNVDD_INW inw_handler;
  PFNVDD_INSB insb_handler;
  PFNVDD_INSW insw_handler;
  PFNVDD_OUTB outb_handler;
  PFNVDD_OUTW outw_handler;
  PFNVDD_OUTSB outsb_handler;
  PFNVDD_OUTSW outsw_handler;
} VDD_IO_HANDLERS, *PVDD_IO_HANDLERS;
 
typedef struct _VDD_IO_PORTRANGE
{
  WORD First;
  WORD Last;
} VDD_IO_PORTRANGE, *PVDD_IO_PORTRANGE;

typedef struct
{
    HANDLE hvdd;
    VDD_IO_HANDLERS io_funcs;
    WORD io_range_len;
    PVDD_IO_PORTRANGE io_range;
} vdd_io_t;

static vdd_io_t vdd_io[5] = {0};
 
BOOL WINAPI VDDInstallIOHook(HANDLE hvdd, WORD cPortRange, PVDD_IO_PORTRANGE pPortRange, PVDD_IO_HANDLERS IOhandler)
{
    int handle = (int)hvdd;
    int found = -1;
    for (int i = 0; i < 5; i++)
    {
        if (!vdd_io[i].hvdd)
            found = i;
        if (vdd_io[i].hvdd == hvdd)
            return FALSE;
    }
    if (found == -1 || !IOhandler->inb_handler || !IOhandler->outb_handler)
        return FALSE;

    vdd_io[found].hvdd = hvdd;
    memcpy(&vdd_io[found].io_funcs, IOhandler, sizeof(VDD_IO_HANDLERS));
    vdd_io[found].io_range_len = cPortRange;
    vdd_io[found].io_range = (PVDD_IO_PORTRANGE)HeapAlloc(GetProcessHeap(), 0, cPortRange * sizeof(VDD_IO_PORTRANGE));
    memcpy(&vdd_io[found].io_range, pPortRange, cPortRange * sizeof(VDD_IO_PORTRANGE));
    return TRUE;
}

void WINAPI VDDDeInstallIOHook(HANDLE hvdd, WORD cPortRange, PVDD_IO_PORTRANGE pPortRange)
{
    int handle = (int)hvdd;
    int i;
    for (i = 0; i < 5; i++)
    {
        if (hvdd == vdd_io[i].hvdd)
            break;
    }
    if (i >= 5)
        return;

    vdd_io[i].hvdd = NULL;
    HeapFree(GetProcessHeap(), 0, vdd_io[i].io_range);
    return;
}

BOOL vdd_io_read(int port, int size, WORD *val, CONTEXT *ctx)
{
    for (int i = 0; i < 5; i++)
    {
        if (vdd_io[i].hvdd)
        {
            for (int j = 0; j < vdd_io[i].io_range_len; j++)
            {
                if ((vdd_io[i].io_range[j].First >= port) && (vdd_io[i].io_range[j].Last <= port))
                {
                    last_context = ctx;
                    switch (size)
                    {
                        case 2:
                            if (vdd_io[i].io_funcs.inw_handler)
                                vdd_io[i].io_funcs.inw_handler(port, val);
                            else
                            {
                                vdd_io[i].io_funcs.inb_handler(port, (BYTE *)val);
                                vdd_io[i].io_funcs.inb_handler(port + 1, ((BYTE *)val) + 1);
                            }
                        case 1:
                             vdd_io[i].io_funcs.inb_handler(port, (BYTE *)val);
                    }
                    last_context = NULL;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL vdd_io_write(int port, int size, WORD val, CONTEXT *ctx)
{
    for (int i = 0; i < 5; i++)
    {
        if (vdd_io[i].hvdd)
        {
            for (int j = 0; j < vdd_io[i].io_range_len; j++)
            {
                if ((vdd_io[i].io_range[j].First >= port) && (vdd_io[i].io_range[j].Last <= port))
                {
                    last_context = ctx;
                    switch (size)
                    {
                        case 2:
                            if (vdd_io[i].io_funcs.outw_handler)
                                vdd_io[i].io_funcs.outw_handler(port, val);
                            else
                            {
                                vdd_io[i].io_funcs.outb_handler(port, val);
                                vdd_io[i].io_funcs.outb_handler(port + 1, val >> 8);
                            }
                        case 1:
                             vdd_io[i].io_funcs.outb_handler(port, val);
                    }
                    last_context = NULL;
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BYTE *WINAPI MGetVdmPointer(DWORD addr, DWORD size, BOOL protmode)
{
    return (BYTE *)K32WOWGetVDMPointer(addr, size, protmode);
}
