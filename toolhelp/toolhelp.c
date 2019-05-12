/*
 * Toolhelp functions
 *
 * Copyright 1996 Marcus Meissner
 * Copyright 2009 Alexandre Julliard
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

#include "config.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <ctype.h>
#include <assert.h>
#include "windef.h"
#include "winbase.h"
//#include "winternl.h"
#include "../krnl386/kernel16_private.h"
#include "wownt32.h"

#include "wine/winbase16.h"
#include "toolhelp.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(toolhelp);

#include "pshpack1.h"

/* Global arena block */
typedef struct
{
    void     *base;          /* Base address (0 if discarded) */
    DWORD     size;          /* Size in bytes (0 indicates a free block) */
    HGLOBAL16 handle;        /* Handle for this block */
    HGLOBAL16 hOwner;        /* Owner of this block */
    BYTE      lockCount;     /* Count of GlobalFix() calls */
    BYTE      pageLockCount; /* Count of GlobalPageLock() calls */
    BYTE      flags;         /* Allocation flags */
    BYTE      selCount;      /* Number of selectors allocated for this block */
    DWORD     dib_avail_size;
    WORD      wSeg;
    WORD      wType;
    BYTE      pad[0x10 - 4 - 2 - 2];     /* win31 GLOBALARENA size = 0x20 */
} GLOBALARENA;

#define GLOBAL_MAX_COUNT  8192        /* Max number of allocated blocks */

typedef struct
{
    WORD check;                 /* 00 Heap checking flag */
    WORD freeze;                /* 02 Heap frozen flag */
    WORD items;                 /* 04 Count of items on the heap */
    WORD first;                 /* 06 First item of the heap */
    WORD pad1;                  /* 08 Always 0 */
    WORD last;                  /* 0a Last item of the heap */
    WORD pad2;                  /* 0c Always 0 */
    BYTE ncompact;              /* 0e Compactions counter */
    BYTE dislevel;              /* 0f Discard level */
    DWORD distotal;             /* 10 Total bytes discarded */
    WORD htable;                /* 14 Pointer to handle table */
    WORD hfree;                 /* 16 Pointer to free handle table */
    WORD hdelta;                /* 18 Delta to expand the handle table */
    WORD expand;                /* 1a Pointer to expand function (unused) */
    WORD pstat;                 /* 1c Pointer to status structure (unused) */
    FARPROC16 notify;           /* 1e Pointer to LocalNotify() function */
    WORD lock;                  /* 22 Lock count for the heap */
    WORD extra;                 /* 24 Extra bytes to allocate when expanding */
    WORD minsize;               /* 26 Minimum size of the heap */
    WORD magic;                 /* 28 Magic number */
} LOCALHEAPINFO;

typedef struct
{
/* Arena header */
    WORD prev;          /* Previous arena | arena type */
    WORD next;          /* Next arena */
/* Start of the memory block or free-list info */
    WORD size;          /* Size of the free block */
    WORD free_prev;     /* Previous free block */
    WORD free_next;     /* Next free block */
} LOCALARENA;

#define LOCAL_ARENA_HEADER_SIZE      4
#define LOCAL_ARENA_HEADER( handle) ((handle) - LOCAL_ARENA_HEADER_SIZE)
#define LOCAL_ARENA_PTR(ptr,arena)  ((LOCALARENA *)((char *)(ptr)+(arena)))

#include "poppack.h"

#define TDB_MAGIC    ('T' | ('D' << 8))

/* FIXME: to make this work, we have to call back all these registered
 * functions from all over the WINE code. Someone with more knowledge than
 * me please do that. -Marcus
 */

static struct notify
{
    HTASK16   htask;
    FARPROC16 lpfnCallback;
    WORD     wFlags;
} *notifys = NULL;

static struct intcb 
{
    HTASK16   htask;
    FARPROC16 lpfnIntCallback;
} *intcbs = NULL;

static int nrofnotifys = 0;
static int nrofintcbs = 0;

static THHOOK *get_thhook(void)
{
    static THHOOK *thhook;

    if (!thhook) thhook = MapSL( (SEGPTR)GetProcAddress16( GetModuleHandle16("KERNEL"), (LPCSTR)332 ));
    return thhook;
}

static GLOBALARENA *get_global_arena(void)
{
    return MapSL(MAKESEGPTR(get_thhook()->hGlobalHeap, 0));
}

static LOCALHEAPINFO *get_local_heap( HANDLE16 ds )
{
    INSTANCEDATA *ptr = MapSL( MAKESEGPTR( ds, 0 ));

    if (!ptr || !ptr->heap) return NULL;
    return (LOCALHEAPINFO*)((char*)ptr + ptr->heap);
}


/***********************************************************************
 *           GlobalHandleToSel   (TOOLHELP.50)
 */
WORD WINAPI GlobalHandleToSel16( HGLOBAL16 handle )
{
    if (!handle) return 0;
    if (!(handle & 7)) return handle - 1;
    return handle | 7;
}


/***********************************************************************
 *           GlobalFirst   (TOOLHELP.51)
 */
BOOL16 WINAPI GlobalFirst16( GLOBALENTRY *pGlobal, WORD wFlags )
{
    if (wFlags == GLOBAL_LRU) return FALSE;
    pGlobal->dwNext = 0;
    return GlobalNext16( pGlobal, wFlags );
}


/***********************************************************************
 *           GlobalNext   (TOOLHELP.52)
 */
BOOL16 WINAPI GlobalNext16( GLOBALENTRY *pGlobal, WORD wFlags)
{
    GLOBALARENA *pGlobalArena = get_global_arena();
    GLOBALARENA *pArena;

    if (pGlobal->dwNext >= GLOBAL_MAX_COUNT) return FALSE;
    pArena = pGlobalArena + pGlobal->dwNext;
    if (wFlags == GLOBAL_FREE)  /* only free blocks */
    {
        int i;
        for (i = pGlobal->dwNext; i < GLOBAL_MAX_COUNT; i++, pArena++)
            if (pArena->size == 0) break;  /* block is free */
        if (i >= GLOBAL_MAX_COUNT) return FALSE;
        pGlobal->dwNext = i;
    }

    pGlobal->dwAddress    = (DWORD_PTR)pArena->base;
    pGlobal->dwBlockSize  = pArena->size;
    pGlobal->hBlock       = pArena->handle;
    pGlobal->wcLock       = pArena->lockCount;
    pGlobal->wcPageLock   = pArena->pageLockCount;
    pGlobal->wFlags       = (GetCurrentPDB16() == pArena->hOwner);
    pGlobal->wHeapPresent = FALSE;
    pGlobal->hOwner       = pArena->hOwner;
    pGlobal->wType        = pArena->wType;
    pGlobal->wData        = pArena->wType == GT_CODE || pArena->wType == GT_DATA || pArena->wType == GT_DGROUP ? pArena->wSeg : 0;
    pGlobal->dwNext++;
    return TRUE;
}


/***********************************************************************
 *           GlobalInfo   (TOOLHELP.53)
 */
BOOL16 WINAPI GlobalInfo16( GLOBALINFO *pInfo )
{
    GLOBALARENA *pGlobalArena = get_global_arena();
    GLOBALARENA *pArena;
    int i;

    pInfo->wcItems = GLOBAL_MAX_COUNT;
    pInfo->wcItemsFree = 0;
    pInfo->wcItemsLRU = 0;
    for (i = 0, pArena = pGlobalArena; i < GLOBAL_MAX_COUNT; i++, pArena++)
        if (pArena->size == 0) pInfo->wcItemsFree++;
    return TRUE;
}


/***********************************************************************
 *           GlobalEntryHandle   (TOOLHELP.54)
 */
BOOL16 WINAPI GlobalEntryHandle16( GLOBALENTRY *pGlobal, HGLOBAL16 hItem )
{
    GLOBALARENA *pGlobalArena = get_global_arena();
    GLOBALARENA *pArena = pGlobalArena + (hItem >> __AHSHIFT);

    pGlobal->dwAddress    = (DWORD_PTR)pArena->base;
    pGlobal->dwBlockSize  = pArena->size;
    pGlobal->hBlock       = pArena->handle;
    pGlobal->wcLock       = pArena->lockCount;
    pGlobal->wcPageLock   = pArena->pageLockCount;
    pGlobal->wFlags       = (GetCurrentPDB16() == pArena->hOwner);
    pGlobal->wHeapPresent = FALSE;
    pGlobal->hOwner       = pArena->hOwner;
    pGlobal->wType        = pArena->wType;
    pGlobal->wData        = pArena->wType == GT_CODE || pArena->wType == GT_DATA || pArena->wType == GT_DGROUP ? pArena->wSeg : 0;
    pGlobal->dwNext       = (hItem >> __AHSHIFT) + 1;
    return TRUE;
}


/***********************************************************************
 *           GlobalEntryModule   (TOOLHELP.55)
 */
BOOL16 WINAPI GlobalEntryModule16( GLOBALENTRY *pGlobal, HMODULE16 hModule,
                                 WORD wSeg )
{
    BOOL r = GlobalFirst16(pGlobal, 0);
    while (r)
    {
        if (pGlobal->hOwner == hModule)
        {
            if ((pGlobal->wType == GT_CODE || pGlobal->wType == GT_DATA || pGlobal->wType == GT_DGROUP) && pGlobal->wData == wSeg)
            {
                return TRUE;
            }
        }
        r = GlobalNext16(pGlobal, 0);
    }
    return FALSE;
}


/***********************************************************************
 *           LocalInfo   (TOOLHELP.56)
 */
BOOL16 WINAPI LocalInfo16( LOCALINFO *pLocalInfo, HGLOBAL16 handle )
{
    LOCALHEAPINFO *pInfo = get_local_heap( SELECTOROF(WOWGlobalLock16(handle)) );
    if (!pInfo) return FALSE;
    pLocalInfo->wcItems = pInfo->items;
    return TRUE;
}


/***********************************************************************
 *           LocalFirst   (TOOLHELP.57)
 */
BOOL16 WINAPI LocalFirst16( LOCALENTRY *pLocalEntry, HGLOBAL16 handle )
{
    WORD ds = GlobalHandleToSel16( handle );
    char *ptr = MapSL( MAKESEGPTR( ds, 0 ) );
    LOCALHEAPINFO *pInfo = get_local_heap( ds );
    if (!pInfo) return FALSE;

    pLocalEntry->hHandle   = pInfo->first + LOCAL_ARENA_HEADER_SIZE;
    pLocalEntry->wAddress  = pLocalEntry->hHandle;
    pLocalEntry->wFlags    = LF_FIXED;
    pLocalEntry->wcLock    = 0;
    pLocalEntry->wType     = LT_NORMAL;
    pLocalEntry->hHeap     = handle;
    pLocalEntry->wHeapType = NORMAL_HEAP;
    pLocalEntry->wNext     = LOCAL_ARENA_PTR(ptr,pInfo->first)->next;
    pLocalEntry->wSize     = pLocalEntry->wNext - pLocalEntry->hHandle;
    return TRUE;
}


/***********************************************************************
 *           LocalNext   (TOOLHELP.58)
 */
BOOL16 WINAPI LocalNext16( LOCALENTRY *pLocalEntry )
{
    WORD ds = GlobalHandleToSel16( pLocalEntry->hHeap );
    char *ptr = MapSL( MAKESEGPTR( ds, 0 ) );
    LOCALARENA *pArena;

    if (!get_local_heap( ds )) return FALSE;
    if (!pLocalEntry->wNext) return FALSE;
    pArena = LOCAL_ARENA_PTR( ptr, pLocalEntry->wNext );

    pLocalEntry->hHandle   = pLocalEntry->wNext + LOCAL_ARENA_HEADER_SIZE;
    pLocalEntry->wAddress  = pLocalEntry->hHandle;
    pLocalEntry->wFlags    = (pArena->prev & 3) + 1;
    pLocalEntry->wcLock    = 0;
    pLocalEntry->wType     = LT_NORMAL;
    if (pArena->next != pLocalEntry->wNext)  /* last one? */
        pLocalEntry->wNext = pArena->next;
    else
        pLocalEntry->wNext = 0;
    pLocalEntry->wSize     = pLocalEntry->wNext - pLocalEntry->hHandle;
    return TRUE;
}


/**********************************************************************
 *	    ModuleFirst    (TOOLHELP.59)
 */
BOOL16 WINAPI ModuleFirst16( MODULEENTRY *lpme )
{
    lpme->wNext = get_thhook()->hExeHead;
    return ModuleNext16( lpme );
}


/**********************************************************************
 *	    ModuleNext    (TOOLHELP.60)
 */
BOOL16 WINAPI ModuleNext16( MODULEENTRY *lpme )
{
    NE_MODULE *pModule;
    char *name;

    if (!lpme->wNext) return FALSE;
    if (!(pModule = GlobalLock16( GetExePtr(lpme->wNext) ))) return FALSE;
    name = (char *)pModule + pModule->ne_restab;
    memcpy( lpme->szModule, name + 1, min(*name, MAX_MODULE_NAME) );
    lpme->szModule[min(*name, MAX_MODULE_NAME)] = '\0';
    lpme->hModule = lpme->wNext;
    lpme->wcUsage = pModule->count;
    name = ((OFSTRUCT *)((char*)pModule + pModule->fileinfo))->szPathName;
    lstrcpynA( lpme->szExePath, name, sizeof(lpme->szExePath) );
    lpme->wNext = pModule->next;
    return TRUE;
}


/**********************************************************************
 *	    ModuleFindName    (TOOLHELP.61)
 */
BOOL16 WINAPI ModuleFindName16( MODULEENTRY *lpme, LPCSTR name )
{
    lpme->wNext = GetModuleHandle16( name );
    return ModuleNext16( lpme );
}


/**********************************************************************
 *	    ModuleFindHandle    (TOOLHELP.62)
 */
BOOL16 WINAPI ModuleFindHandle16( MODULEENTRY *lpme, HMODULE16 hModule )
{
    NE_MODULE *pModule;
    if (!(pModule = GlobalLock16(hModule))) return FALSE;
    if (pModule->ne_magic != IMAGE_OS2_SIGNATURE)
        return FALSE;
    lpme->wNext = hModule;
    return ModuleNext16( lpme );
}


/***********************************************************************
 *           TaskFirst   (TOOLHELP.63)
 */
BOOL16 WINAPI TaskFirst16( TASKENTRY *lpte )
{
    lpte->hNext = get_thhook()->HeadTDB;
    return TaskNext16( lpte );
}


/***********************************************************************
 *           TaskNext   (TOOLHELP.64)
 */
BOOL16 WINAPI TaskNext16( TASKENTRY *lpte )
{
    TDB *pTask;
    INSTANCEDATA *pInstData;

    TRACE_(toolhelp)("(%p): task=%04x\n", lpte, lpte->hNext );
    if (!lpte->hNext) return FALSE;

    /* make sure that task and hInstance are valid (skip initial Wine task !) */
    while (1) {
        pTask = GlobalLock16( lpte->hNext );
        if (!pTask || pTask->magic != TDB_MAGIC) return FALSE;
        if (pTask->hInstance)
            break;
        lpte->hNext = pTask->hNext;
    }
    pInstData = MapSL( MAKESEGPTR( GlobalHandleToSel16(pTask->hInstance), 0 ) );
    lpte->hTask         = lpte->hNext;
    lpte->hTaskParent   = pTask->hParent;
    lpte->hInst         = pTask->hInstance;
    lpte->hModule       = pTask->hModule;
    lpte->wSS           = SELECTOROF(pTask->teb->TlsSlots[WOW32RESERVED_TLS_INDEX]);
    lpte->wSP           = OFFSETOF(pTask->teb->TlsSlots[WOW32RESERVED_TLS_INDEX]);
    lpte->wStackTop     = pInstData->stacktop;
    lpte->wStackMinimum = pInstData->stackmin;
    lpte->wStackBottom  = pInstData->stackbottom;
    lpte->wcEvents      = pTask->nEvents;
    lpte->hQueue        = pTask->hQueue;
    lstrcpynA( lpte->szModule, pTask->module_name, sizeof(lpte->szModule) );
    lpte->wPSPOffset    = 0x100;  /*??*/
    lpte->hNext         = pTask->hNext;
    return TRUE;
}


/***********************************************************************
 *           TaskFindHandle   (TOOLHELP.65)
 */
BOOL16 WINAPI TaskFindHandle16( TASKENTRY *lpte, HTASK16 hTask )
{
    lpte->hNext = hTask;
    return TaskNext16( lpte );
}


typedef NTSTATUS
(NTAPI *NtQuerySystemInformation_t)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
);
NtQuerySystemInformation_t fNtQuerySystemInformation;
/***********************************************************************
 *           MemManInfo   (TOOLHELP.72)
 */
BOOL16 WINAPI MemManInfo16( MEMMANINFO *info )
{
    SYSTEM_BASIC_INFORMATION sbi;
    MEMORYSTATUS status;

    /*
     * Not unsurprisingly although the documentation says you
     * _must_ provide the size in the dwSize field, this function
     * (under Windows) always fills the structure and returns true.
     */
    if (!fNtQuerySystemInformation)
        fNtQuerySystemInformation = (NtQuerySystemInformation_t)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation");
    fNtQuerySystemInformation( SystemBasicInformation, &sbi, sizeof(sbi), NULL );
    GlobalMemoryStatus( &status );
    info->wPageSize            = sbi.PageSize;
    info->dwLargestFreeBlock   = status.dwAvailVirtual;
    info->dwMaxPagesAvailable  = info->dwLargestFreeBlock / info->wPageSize;
    info->dwMaxPagesLockable   = info->dwMaxPagesAvailable;
    info->dwTotalLinearSpace   = status.dwTotalVirtual / info->wPageSize;
    info->dwTotalUnlockedPages = info->dwTotalLinearSpace;
    info->dwFreePages          = info->dwMaxPagesAvailable;
    info->dwTotalPages         = info->dwTotalLinearSpace;
    info->dwFreeLinearSpace    = info->dwMaxPagesAvailable;
    info->dwSwapFilePages      = status.dwTotalPageFile / info->wPageSize;
    return TRUE;
}


/***********************************************************************
 *		NotifyRegister (TOOLHELP.73)
 */
BOOL16 WINAPI NotifyRegister16( HTASK16 htask, FARPROC16 lpfnCallback,
                              WORD wFlags )
{
    int	i;

    FIXME("(%x,%x,%x), semi-stub.\n",
                      htask, (DWORD)lpfnCallback, wFlags );
    if (!htask) htask = GetCurrentTask();
    for (i=0;i<nrofnotifys;i++)
        if (notifys[i].htask==htask)
            break;
    if (i==nrofnotifys) {
        if (notifys==NULL)
            notifys=HeapAlloc( GetProcessHeap(), 0,
                                               sizeof(struct notify) );
        else
            notifys=HeapReAlloc( GetProcessHeap(), 0, notifys,
                                        sizeof(struct notify)*(nrofnotifys+1));
        if (!notifys) return FALSE;
        nrofnotifys++;
    }
    notifys[i].htask=htask;
    notifys[i].lpfnCallback=lpfnCallback;
    notifys[i].wFlags=wFlags;
    return TRUE;
}

/***********************************************************************
 *		NotifyUnRegister (TOOLHELP.74)
 */
BOOL16 WINAPI NotifyUnRegister16( HTASK16 htask )
{
    int	i;

    FIXME("(%x), semi-stub.\n", htask );
    if (!htask) htask = GetCurrentTask();
    for (i=nrofnotifys;i--;)
        if (notifys[i].htask==htask)
            break;
    if (i==-1)
        return FALSE;
    memcpy(notifys+i,notifys+(i+1),sizeof(struct notify)*(nrofnotifys-i-1));
    notifys=HeapReAlloc( GetProcessHeap(), 0, notifys,
                                        (nrofnotifys-1)*sizeof(struct notify));
    nrofnotifys--;
    return TRUE;
}

static int lastcb = 0;
static SEGPTR lastaddr = 0;

void WINAPI next_intcb(CONTEXT *context)
{
    WORD *stkptr = (WORD *)MapSL(MAKESEGPTR(context->SegSs, LOWORD(context->Esp)));
    if (lastcb == nrofintcbs)
    {
        lastcb = 0;
        context->Esp += 12;
        context->Eip = stkptr[3];
        context->SegCs = stkptr[4];
        context->ContextFlags = stkptr[5];
        lastaddr = MAKESEGPTR(context->SegCs, context->Eip);
        return;
    }
    stkptr -= 2;
    FARPROC16 next_intcb16 = GetProcAddress16(GetModuleHandle16("TOOLHELP"), "next_intcb");
    stkptr[0] = OFFSETOF(next_intcb16);
    stkptr[1] = SELECTOROF(next_intcb16);
    context->Esp -= 4;
    context->Eip = OFFSETOF(intcbs[lastcb].lpfnIntCallback);
    context->SegCs = SELECTOROF(intcbs[lastcb].lpfnIntCallback);
    lastcb++;
}

FARPROC16 WINAPI get_intcb(SEGPTR *stack, SEGPTR addr, WORD flags, WORD err, WORD ax)
{
    if(!nrofintcbs)
        return FALSE;
    if(addr == lastaddr) // failed to handle once
        return FALSE;
    lastaddr = 0;

    WORD *stkptr = (WORD *)MapSL(*stack);
    FARPROC16 next_intcb16 = GetProcAddress16(GetModuleHandle16("TOOLHELP"), "next_intcb");
    stkptr -= 8;
    stkptr[0] = OFFSETOF(next_intcb16);
    stkptr[1] = SELECTOROF(next_intcb16);
    stkptr[2] = ax;
    stkptr[3] = err;
    stkptr[4] = 0;
    stkptr[5] = OFFSETOF(addr);
    stkptr[6] = SELECTOROF(addr);
    stkptr[7] = flags;
    *stack -= 16;
    lastcb = 1;
    return intcbs[0].lpfnIntCallback;
}        

/***********************************************************************
 *		InterruptRegister (TOOLHELP.75)
 */
BOOL16 WINAPI InterruptRegister16( HTASK16 htask, FARPROC16 callback )
{
    int	i;

    FIXME("(%04x, %04x:%04x), semi-stub.\n", htask, SELECTOROF(callback), OFFSETOF(callback));
    if (!htask) htask = GetCurrentTask();
    for (i=0;i<nrofintcbs;i++)
        if (intcbs[i].htask==htask)
            break;
    if (i==nrofintcbs) {
        if (intcbs==NULL)
            intcbs=HeapAlloc( GetProcessHeap(), 0,
                                               sizeof(struct intcb) );
        else
            intcbs=HeapReAlloc( GetProcessHeap(), 0, intcbs,
                                        sizeof(struct intcb)*(nrofintcbs+1));
        if (!intcbs) return FALSE;
        nrofintcbs++;
    }
    intcbs[i].htask=htask;
    intcbs[i].lpfnIntCallback=callback;
    return TRUE;
}

/***********************************************************************
 *		InterruptUnRegister (TOOLHELP.76)
 */
BOOL16 WINAPI InterruptUnRegister16( HTASK16 htask )
{
    int	i;

    FIXME("(%x), semi-stub.\n", htask );
    if (!htask) htask = GetCurrentTask();
    for (i=nrofintcbs;i--;)
        if (intcbs[i].htask==htask)
            break;
    if (i==-1)
        return FALSE;
    memcpy(intcbs+i,intcbs+(i+1),sizeof(struct intcb)*(nrofintcbs-i-1));
    intcbs=HeapReAlloc( GetProcessHeap(), 0, intcbs,
                                        (nrofintcbs-1)*sizeof(struct intcb));
    nrofintcbs--;
    return TRUE;
}

/***********************************************************************
 *           TerminateApp   (TOOLHELP.77)
 *
 * See "Undocumented Windows".
 */
void WINAPI TerminateApp16(HTASK16 hTask, WORD wFlags)
{
    if (hTask && hTask != GetCurrentTask())
    {
        FIXME("cannot terminate task %x\n", hTask);
        return;
    }

#if 0  /* FIXME */
    /* check undocumented flag */
    if (!(wFlags & 0x8000))
        TASK_CallTaskSignalProc( USIG16_TERMINATION, hTask );
#endif

    /* UndocWin says to call int 0x21/0x4c exit=0xff here,
       but let's just call ExitThread */
    ExitThread(0xff);
}

/***********************************************************************
 *           MemoryRead   (TOOLHELP.78)
 */
DWORD WINAPI MemoryRead16( WORD sel, DWORD offset, void *buffer, DWORD count )
{
    LDT_ENTRY entry;
    DWORD limit;

    wine_ldt_get_entry( sel, &entry );
    if (wine_ldt_is_empty( &entry )) return 0;
    limit = wine_ldt_get_limit( &entry );
    if (offset > limit) return 0;
    if (offset + count > limit + 1) count = limit + 1 - offset;
    memcpy( buffer, (char *)wine_ldt_get_base(&entry) + offset, count );
    return count;
}


/***********************************************************************
 *           MemoryWrite   (TOOLHELP.79)
 */
DWORD WINAPI MemoryWrite16( WORD sel, DWORD offset, void *buffer, DWORD count )
{
    LDT_ENTRY entry;
    DWORD limit;

    wine_ldt_get_entry( sel, &entry );
    if (wine_ldt_is_empty( &entry )) return 0;
    limit = wine_ldt_get_limit( &entry );
    if (offset > limit) return 0;
    if (offset + count > limit) count = limit + 1 - offset;
    memcpy( (char *)wine_ldt_get_base(&entry) + offset, buffer, count );
    return count;
}

/***********************************************************************
 *           TimerCount   (TOOLHELP.80)
 */
BOOL16 WINAPI TimerCount16( TIMERINFO *pTimerInfo )
{
    /* FIXME
     * In standard mode, dwmsSinceStart = dwmsThisVM
     *
     * I tested this, under Windows in enhanced mode, and
     * if you never switch VM (ie start/stop DOS) these
     * values should be the same as well.
     *
     * Also, Wine should adjust for the hardware timer
     * to reduce the amount of error to ~1ms.
     * I can't be bothered, can you?
     */
    pTimerInfo->dwmsSinceStart = pTimerInfo->dwmsThisVM = GetTickCount();
    return TRUE;
}

/***********************************************************************
 *           SystemHeapInfo   (TOOLHELP.71)
 */
BOOL16 WINAPI SystemHeapInfo16( SYSHEAPINFO *pHeapInfo )
{
    STACK16FRAME* stack16 = MapSL((SEGPTR)/*NtCurrentTeb()->WOW32Reserved*/getWOW32Reserved());
    HANDLE16 oldDS = stack16->ds;
    WORD user = LoadLibrary16( "USER.EXE" );
    WORD gdi = LoadLibrary16( "GDI.EXE" );
    stack16->ds = user;
    pHeapInfo->wUserFreePercent = (int)LocalCountFree16() * 100 / LocalHeapSize16();
    stack16->ds = gdi;
    pHeapInfo->wGDIFreePercent  = (int)LocalCountFree16() * 100 / LocalHeapSize16();
    stack16->ds = oldDS;
    pHeapInfo->hUserSegment = user;
    pHeapInfo->hGDISegment  = gdi;
    FreeLibrary16( user );
    FreeLibrary16( gdi );
    return TRUE;
}

/***********************************************************************
 *           Local32Info   (TOOLHELP.84)
 */
BOOL16 WINAPI Local32Info16( LOCAL32INFO *pLocal32Info, HGLOBAL16 handle )
{
    FIXME( "Call Local32Info16 in kernel\n" );
    return FALSE;
}

/***********************************************************************
 *           Local32First   (TOOLHELP.85)
 */
BOOL16 WINAPI Local32First16( LOCAL32ENTRY *pLocal32Entry, HGLOBAL16 handle )
{
    FIXME( "Call Local32First16 in kernel\n" );
    return FALSE;
}

/***********************************************************************
 *           Local32Next   (TOOLHELP.86)
 */
BOOL16 WINAPI Local32Next16( LOCAL32ENTRY *pLocal32Entry )
{
    FIXME( "Call Local32Next16 in kernel\n" );
    return FALSE;
}
__declspec(dllimport) BOOL16 WINAPI USER_ClassFirst16(CLASSENTRY *pClassEntry);
__declspec(dllimport) BOOL16 WINAPI USER_ClassNext16(CLASSENTRY *pClassEntry);
BOOL16 WINAPI ClassFirst16(CLASSENTRY *pClassEntry)
{
    return USER_ClassFirst16(pClassEntry);
}
BOOL16 WINAPI ClassNext16(CLASSENTRY *pClassEntry)
{
    return USER_ClassNext16(pClassEntry);
}

BOOL WINAPI TOOLHELP_CallNotify(WORD wID, DWORD dwData)
{
    int i;
    DWORD ret = FALSE;
    if (nrofnotifys == 0)
        return FALSE;
    TRACE("(%04x,%08x)\n", wID, dwData);
    for (i = 0; i < nrofnotifys; i++)
    {
        WORD args[3];
        PVOID sssp;
        if ((wID == NFY_TASKIN || wID == NFY_TASKOUT) && (notifys[i].wFlags & NF_TASKSWITCH) != NF_TASKSWITCH)
        {
            continue;
        }
        if ((wID == NFY_RIP) && (notifys[i].wFlags & NF_RIP) != NF_RIP)
        {
            continue;
        }
        sssp = getWOW32Reserved();
        args[2] = wID;
        args[1] = HIWORD(dwData);
        args[0] = LOWORD(dwData);
        WOWCallback16Ex(notifys[i].lpfnCallback, WCB16_PASCAL, 6, &args, &ret);
        setWOW32Reserved(sssp);
        if (LOWORD(ret))
            break;
    }
    TRACE("ret (%04x,%08x) retval=%04x\n", wID, dwData, LOWORD(ret));
    return LOWORD(ret);
}
