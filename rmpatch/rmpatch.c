#include "windef.h"
#include "winbase.h"
#include "wine/winbase16.h"
#include "../krnl386/kernel16_private.h"

// htask is an ldt selector above 0x1000, use a gdt selector below 0x1000 here
// it's only used as an index
#define FAKE_HTASK16 0xf01

BOOL16 WINAPI InterruptRegister16(HTASK16 htask, FARPROC16 callback);
BOOL16 WINAPI InterruptUnRegister16(HTASK16 htask);
BOOL16 WINAPI NotifyRegister16(HTASK16 htask, FARPROC16 lpfnCallback, WORD wFlags);
BOOL16 WINAPI NotifyUnRegister16(HTASK16 htask);
BOOL16 WINAPI IsOldWindowsTask(HINSTANCE16 hinst);
void WINAPI next_intcb(CONTEXT *context);

#include <pshpack1.h>
typedef struct {
	DWORD	dwSize;
	WORD	wSelector;
	WORD	wSegNum;
	WORD	wType;		/* bit 0 set if this is a code segment */
	WORD	wcInstance;	/* only valid for data segment */
	SEGPTR  lpstrModuleName;
} NFYLOADSEG;
#include <poppack.h>

BOOL WINAPI checkpatch(WORD id, DWORD data)
{
	if (id != 1 /*NFY_LOADSEG*/)
		return;
	NFYLOADSEG *seg = (NFYLOADSEG *)MapSL(data);
	char *name = (char *)MapSL(seg->lpstrModuleName);
	if (GetExpWinVer16(GetModuleHandle16(name)) >= 0x300)
		return 0;
	char *mem = MapSL(MAKELONG(0, seg->wSelector));
	WORD segment = seg->wSegNum;
	if (!memcmp(mem, "MSEM87", 6)) // check for linked in MS 8087 emulator
	{
		FARPROC16 fpmath = GetProcAddress16(LoadLibrary16("WIN87EM.DLL"), "__FPMATH");
		mem[0x24] = 0xea;  // JMP FAR
		*(FARPROC16 *)(mem + 0x25) = fpmath;
	}
	else if (!strcmp(name, "MGXWIN20"))
	{
		// drawing lib from Micrographics Designer 1.1 (segment arithmitic)
		if ((segment == 13) && (*(DWORD *)(mem + 0x1e4d) == 0x000fe781))
		{
			const char patch[] = {0x90, 0x90, 0x90, 0x90, 0x03, 0xf9, 0xc3};
			memcpy(mem + 0x1e4d, patch, sizeof(patch));
		}
	}
	else if (!strcmp(name, "GLIB"))
	{
		// drawing lib from Micrographics Portfolio 1.0 (segment arithmitic)
		if ((segment == 1) && (*(DWORD *)(mem + 0x274c) == 0x000fe781))
		{
			const char patch[] = {0x90, 0x90, 0x90, 0x90, 0x03, 0xf9, 0xc3};
			memcpy(mem + 0x274c, patch, sizeof(patch));
		}
	}
	else if (!strcmp(name, "XP"))
	{
		// Xerox Presents (calls int 13h to parse root dir unnecessarily)
		if ((segment == 71) && (*(DWORD *)(mem + 0x11f2) == 0xec8b5545))
			mem[0x11f2] = 0xcb;
	}
	else if (!strcmp(name, "BOP"))
	{
		// use MM_ANISOTROPIC instead of MM_ISOTROPIC and fix viewport
		if ((segment == 2) && (*(BYTE *)(mem + 0x0d) == 0x07))
		{
			const char patch[] = {0xb8, 0xd0, 0x02, 0x50, 0xb8, 0x49, 0x01, 0x50};
			memcpy(mem + 0x2a, patch, sizeof(patch));
			mem[0x0d] = 0x08;
			mem[0x2c1] = 0x34;
		}
	}
	else if (!strcmp(name, "WFE"))
	{
		// ZSoft PTF Outline Editor (uninitialized stack variable)
		if ((segment == 3) && (*(DWORD *)(mem + 0x8c5) == 0x5efc468b))
		{
			mem[0x8c5] = 0xb8;
			mem[0x8c6] = 0x01;
			mem[0x8c7] = 0x00;
		}
	}
	return 0;
}

#include <pshpack1.h>
struct stkframe
{
	DWORD ret;
	WORD ax;
	WORD err;
	WORD handle;
	DWORD addr;
	WORD flags;
};
#include <poppack.h>

void WINAPI intcb(CONTEXT *context)
{
	struct stkframe *frame = MapSL(MAKESEGPTR(context->SegSs, context->Esp));
	if((frame->err == 0xd) && IsOldWindowsTask(GetCurrentTask()))
	{
		context->Esp += sizeof(struct stkframe);
		context->Eip = OFFSETOF(frame->addr);
		context->SegCs = SELECTOROF(frame->addr);
		context->EFlags = frame->flags;
		if(rmpatch_emulate_instruction(context))
			return;
		context->Esp -= sizeof(struct stkframe);
	}
	context->Esp += 4;
	next_intcb(context);
}

BOOL WINAPI DllEntryPoint(DWORD fdwReason, HINSTANCE16 hinstDLL, WORD ds,
			  WORD wHeapSize, DWORD dwReserved1, WORD wReserved2)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		LoadLibrary16("TOOLHELP");
		InterruptRegister16(FAKE_HTASK16, GetProcAddress16(GetModuleHandle16("RMPATCH"), "intcb"));
		NotifyRegister16(FAKE_HTASK16, GetProcAddress16(GetModuleHandle16("RMPATCH"), "checkpatch"), 0 /*NF_NORMAL*/);
		break;
	case DLL_PROCESS_DETACH:
		InterruptUnRegister16(FAKE_HTASK16);
		NotifyUnRegister16(FAKE_HTASK16);
		FreeLibrary16("TOOLHELP");
		break;
	}
	return TRUE;
}
