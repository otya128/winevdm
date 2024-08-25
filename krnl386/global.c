/*
 * Global heap functions
 *
 * Copyright 1995 Alexandre Julliard
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
/* 0xffff sometimes seems to mean: CURRENT_DS */

#include "config.h"
#include "wine/port.h"

#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <string.h>
#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#ifdef HAVE_SYS_SYSCTL_H
#include <sys/sysctl.h>
#endif

#include "wine/winbase16.h"
#include "winternl.h"
#include "kernel16_private.h"
#include "wine/debug.h"
#include "winuser.h"
#include "wingdi.h"

WINE_DEFAULT_DEBUG_CHANNEL(global);

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
    HGLOBAL   link_hndl;
    BYTE      pad[0x10 - 4 - 2 - 2 - 4];     /* win31 GLOBALARENA size = 0x20 */
} GLOBALARENA;

  /* Flags definitions */
#define GA_MOVEABLE     0x02  /* same as GMEM_MOVEABLE */
#define GA_DGROUP       0x04
#define GA_DISCARDABLE  0x08
#define GA_IPCSHARE     0x10  /* same as GMEM_DDESHARE */
#define GA_DOSMEM       0x20

/* Arena array (FIXME) */
static GLOBALARENA *pGlobalArena;
static int globalArenaSize;
static DWORD *selTable;

#define GLOBAL_MAX_ALLOC_SIZE 0x00ff0000  /* Largest allocation is 16M - 64K */
#define GLOBAL_MAX_COUNT      8192        /* Max number of allocated blocks */

#define VALID_HANDLE(handle) (((handle)&4)&&(((handle)>>__AHSHIFT)<globalArenaSize))
#define GET_ARENA_PTR(handle)  (pGlobalArena + ((handle) >> __AHSHIFT))

static HANDLE get_win16_heap(void)
{
    static HANDLE win16_heap;

    /* we create global memory block with execute permission. The access can be limited
     * for 16-bit code on selector level */
    if (!win16_heap) win16_heap = HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
    return win16_heap;
}

static void clear_sel_table(WORD sel, WORD selcount)
{
    for (int i = 0; i < selcount; i++)
    {
        selTable[(sel + i) >> __AHSHIFT] = 0;
    }
}
static void set_sel_table(WORD sel, WORD selcount)
{
    for (int i = 0; i < selcount; i++)
    {
        selTable[(sel + i) >> __AHSHIFT] = (sel >> __AHSHIFT) * sizeof(GLOBALARENA);
    }
}


/***********************************************************************
 *           GLOBAL_GetArena
 *
 * Return the arena for a given selector, growing the arena array if needed.
 */
static GLOBALARENA *GLOBAL_GetArena( WORD sel, WORD selcount )
{
    if (((sel >> __AHSHIFT) + selcount) > globalArenaSize)
    {
        int newsize = ((sel >> __AHSHIFT) + selcount + 0xff) & ~0xff;

        if (!pGlobalArena)
        {
            SIZE_T size;
            pThhook->SelTableLen = GLOBAL_MAX_COUNT * sizeof(DWORD);
            size = GLOBAL_MAX_COUNT * sizeof(GLOBALARENA) + 0x80 /* unknown */ + pThhook->SelTableLen;
            pGlobalArena = HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, size);
            if (!pGlobalArena) return 0;
            WORD sel = SELECTOR_AllocBlock(pGlobalArena, 0x10000, WINE_LDT_FLAGS_DATA);
            SetSelectorLimit16(sel, size - 1);
            pThhook->hGlobalHeap = sel;
            pThhook->pGlobalHeap = sel;
            pThhook->SelTableStart = GLOBAL_MAX_COUNT * sizeof(GLOBALARENA) + 0x80; /* 0x00040080; */
            selTable = (DWORD*)((BYTE*)(pGlobalArena) + pThhook->SelTableStart);
            set_sel_table(sel, 1);
        }
        if (newsize > GLOBAL_MAX_COUNT) return 0;
        globalArenaSize = newsize;
    }
    return pGlobalArena + (sel >> __AHSHIFT);
}

void debug_handles(void)
{
    int printed=0;
    int i;
    for (i = globalArenaSize-1 ; i>=0 ; i--) {
	if (pGlobalArena[i].size!=0 && (pGlobalArena[i].handle & 0x8000)){
	    printed=1;
	    DPRINTF("0x%08x, ",pGlobalArena[i].handle);
	}
    }
    if (printed)
	DPRINTF("\n");
}


/***********************************************************************
 *           GLOBAL_CreateBlock
 *
 * Create a global heap block for a fixed range of linear memory.
 */
HGLOBAL16 GLOBAL_CreateBlock( WORD flags, void *ptr, DWORD size,
                              HGLOBAL16 hOwner, unsigned char selflags, WORD sel )
{
    WORD selcount;
    GLOBALARENA *pArena;

      /* Allocate the selector(s) */

    if (sel)
    {
        if (size > 65536)
        {
            ERR("prealloced sel must not be larger than 65536");
            return 0;
        }
        SELECTOR_ReallocBlock(sel, ptr, size);
        SelectorAccessRights16(sel, 1, selflags);
    }
    else
        sel = SELECTOR_AllocBlock( ptr, size, selflags );
    if (!sel) return 0;
    selcount = (size + 0xffff) / 0x10000;

    if (!(pArena = GLOBAL_GetArena( sel, selcount )))
    {
        SELECTOR_FreeBlock( sel );
        return 0;
    }

      /* Fill the arena block */

    pArena->base = ptr;
    pArena->size = size == 1 && ptr == NULL ? 0 : GetSelectorLimit16(sel) + 1;
    pArena->handle = (flags & GMEM_MOVEABLE) ? sel - 1 : sel;
    pArena->hOwner = hOwner;
    pArena->lockCount = 0;
    pArena->pageLockCount = 0;
    pArena->wSeg = 0;
    pArena->wType = GT_UNKNOWN;
    pArena->flags = flags & GA_MOVEABLE;
    pArena->link_hndl = NULL;
    if (flags & GMEM_DISCARDABLE) pArena->flags |= GA_DISCARDABLE;
    if (flags & GMEM_DDESHARE) pArena->flags |= GA_IPCSHARE;
    if (!(selflags & (WINE_LDT_FLAGS_CODE^WINE_LDT_FLAGS_DATA))) pArena->flags |= GA_DGROUP;
    pArena->selCount = selcount;
    if (selcount > 1)  /* clear the next arena blocks */
        memset( pArena + 1, 0, (selcount - 1) * sizeof(GLOBALARENA) );

    set_sel_table(sel, selcount);
    return pArena->handle;
}


/***********************************************************************
 *           GLOBAL_FreeBlock
 *
 * Free a block allocated by GLOBAL_CreateBlock, without touching
 * the associated linear memory range.
 */
BOOL16 GLOBAL_FreeBlock( HGLOBAL16 handle )
{
    WORD sel;
    GLOBALARENA *pArena;

    if (!handle) return TRUE;
    sel = GlobalHandleToSel16( handle );
    if (!VALID_HANDLE(sel)) return FALSE;
    pArena = GET_ARENA_PTR(sel);
    SELECTOR_FreeBlock( sel );
    clear_sel_table(sel, pArena->selCount);
    memset( pArena, 0, sizeof(GLOBALARENA) );
    return TRUE;
}

/***********************************************************************
 *           GLOBAL_MoveBlock
 */
BOOL16 GLOBAL_MoveBlock( HGLOBAL16 handle, void *ptr, DWORD size )
{
    WORD sel;
    GLOBALARENA *pArena;

    if (!handle) return TRUE;
    sel = GlobalHandleToSel16( handle );
    if (!VALID_HANDLE(sel)) return FALSE;
    pArena = GET_ARENA_PTR(sel);
    if (pArena->selCount != 1)
        return FALSE;

    pArena->base = ptr;
    pArena->size = size;
    SELECTOR_ReallocBlock( sel, ptr, size );
    return TRUE;
}

/***********************************************************************
 *           GLOBAL_Alloc
 *
 * Implementation of GlobalAlloc16()
 */
HGLOBAL16 GLOBAL_Alloc( UINT16 flags, DWORD size, HGLOBAL16 hOwner, unsigned char selflags, WORD sel )
{
    void *ptr;
    HGLOBAL16 handle;

    TRACE("%d flags=%04x\n", size, flags );

    /* If size is 0, create a discarded block */

    if (size == 0) return GLOBAL_CreateBlock( flags, NULL, 1, hOwner, selflags, sel );

    /* Fixup the size */
    DWORD fixup_size = (size < 0x10000) ? 0x1f : 0xfff; // selectors larger than 64k need page granularity

    if (size > GLOBAL_MAX_ALLOC_SIZE) return 0;
    size = (size + fixup_size) & ~fixup_size;

    /* Allocate the linear memory */
    ptr = HeapAlloc( get_win16_heap(), 0, size);
      /* FIXME: free discardable blocks and try again? */
    if (!ptr) return 0;

      /* Allocate the selector(s) */

    handle = GLOBAL_CreateBlock( flags, ptr, size, hOwner, selflags, sel );
    if (!handle)
    {
        HeapFree( get_win16_heap(), 0, ptr );
        return 0;
    }

    if (flags & GMEM_ZEROINIT) memset( ptr, 0, size );
    else ((char *)ptr)[size - 1] = 0xff; // some programs depend on the block not being cleared
    return handle;
}
WORD GLOBAL_GetSegNum(HGLOBAL16 hg)
{
    return GET_ARENA_PTR(hg)->wSeg;
}

void GLOBAL_SetSeg(HGLOBAL16 hg, WORD wSeg, WORD type)
{
    GET_ARENA_PTR(hg)->wSeg = wSeg;
    GET_ARENA_PTR(hg)->wType = type;
}

HGLOBAL GLOBAL_GetLink(HGLOBAL16 hg)
{
    return GET_ARENA_PTR(hg)->link_hndl;
}

void GLOBAL_SetLink(HGLOBAL16 hg16, HGLOBAL hg)
{
    GET_ARENA_PTR(hg16)->link_hndl = hg;
}

HGLOBAL16 GLOBAL_FindLink(HGLOBAL hg)
{
    int i;
    GLOBALARENA *pArena = pGlobalArena;
    for (i = 0; i < globalArenaSize; i++, pArena++)
    {
        if ((pArena->size != 0) && (pArena->link_hndl == hg))
            return pArena->handle;
    }
    return 0;
}

/***********************************************************************
 *           GlobalAlloc     (KERNEL.15)
 *           GlobalAlloc16   (KERNEL32.24)
 *
 * Allocate a global memory object.
 *
 * RETURNS
 *	Handle: Success
 *	NULL: Failure
 */
HGLOBAL16 WINAPI GlobalAlloc16(
                 UINT16 flags, /* [in] Object allocation attributes */
                 DWORD size    /* [in] Number of bytes to allocate */
) {
    HANDLE16 owner = GetCurrentPDB16();

    if (flags & GMEM_DDESHARE)
    {
        /* make it owned by the calling module */
        STACK16FRAME *frame = CURRENT_STACK16;
        owner = GetExePtr( frame->cs );
    }
    HGLOBAL16 handle = GLOBAL_Alloc( flags, size, owner, WINE_LDT_FLAGS_DATA, 0 );
    if (!handle)
    {
        ERR("Could not allocate %04X,%08X\n", flags, size);
    }
    return handle;
}


/***********************************************************************
 *           GlobalReAlloc     (KERNEL.16)
 *
 * Change the size or attributes of a global memory object.
 *
 * RETURNS
 *	Handle: Success
 *	NULL: Failure
 */
HGLOBAL16 WINAPI GlobalReAlloc16(
                 HGLOBAL16 handle, /* [in] Handle of global memory object */
                 DWORD size,       /* [in] New size of block */
                 UINT16 flags      /* [in] How to reallocate object */
) {
    WORD selcount;
    DWORD oldsize;
    void *ptr, *newptr;
    GLOBALARENA *pArena, *pNewArena;
    WORD sel = GlobalHandleToSel16( handle );
    HANDLE heap = get_win16_heap();

    TRACE("%04x %d flags=%04x\n",
                    handle, size, flags );
    if (!handle) return 0;

    if (!VALID_HANDLE(handle))
    {
        WARN("Invalid handle 0x%04x!\n", handle);
        return 0;
    }
    pArena = GET_ARENA_PTR( handle );

    if (pArena->wType == GT_INTERNAL)
        return 0;
      /* Discard the block if requested */

    if ((size == 0) && (flags & GMEM_MOVEABLE) && !(flags & GMEM_MODIFY))
    {
        if (!(pArena->flags & GA_MOVEABLE) ||
            !(pArena->flags & GA_DISCARDABLE) ||
            (pArena->lockCount > 0) || (pArena->pageLockCount > 0)) return 0;

        if (pArena->dib_avail_size)
        {
            FIXME("DIB.DRV\n");
        }
        else if (pArena->flags & GA_DOSMEM)
            DOSMEM_FreeBlock( pArena->base );
        else
            HeapFree( heap, 0, pArena->base );
        pArena->base = 0;

        /* Note: we rely on the fact that SELECTOR_ReallocBlock won't
         * change the selector if we are shrinking the block.
	 * FIXME: shouldn't we keep selectors until the block is deleted?
	 */
        SELECTOR_ReallocBlock( sel, 0, 1 );
        return handle;
    }

      /* Change the flags */

    if (flags & GMEM_MODIFY)
    {
          /* Change the flags, leaving GA_DGROUP alone */
        pArena->flags = (pArena->flags & GA_DGROUP) | (flags & GA_MOVEABLE);
        if (flags & GMEM_DISCARDABLE) pArena->flags |= GA_DISCARDABLE;
        return handle;
    }


      /* Fixup the size */
    DWORD fixup_size = 0x1f;
    BOOL old = IsOldWindowsTask(GetCurrentTask());
    DWORD add_size = old ? 0x100 : 0;
    if (size + add_size > 0x10000)
        add_size = 0;

    if (size > GLOBAL_MAX_ALLOC_SIZE - (fixup_size + 1)) return 0;
    if (size == 0) size = fixup_size + 1;
    else size = (size + fixup_size) & ~fixup_size;

      /* Reallocate the linear memory */

    ptr = pArena->base;
    oldsize = pArena->size;
    TRACE("oldbase %p oldsize %08x newsize %08x\n", ptr,oldsize,size);
    if (ptr && (size == oldsize)) return handle;  /* Nothing to do */

    if (pArena->dib_avail_size)
    {
        if (size > pArena->dib_avail_size)
        {
            ERR("could not realloc dib memory\n");
            return 0;
        }
        pArena->size = size;
        SetSelectorLimit16(sel, size - 1);
        return handle;
    }
    if (pArena->flags & GA_DOSMEM)
    {
        if (DOSMEM_ResizeBlock(ptr, size, TRUE) == size) 
            newptr = ptr;
        else if(pArena->pageLockCount > 0)
            newptr = 0;
        else
        {
            newptr = DOSMEM_AllocBlock( size, NULL );
            if (newptr)
            {
                memcpy( newptr, ptr, oldsize );
                DOSMEM_FreeBlock( ptr );
            }
        }
    }
    else
    {
        /*
         * if more than one reader (e.g. some pointer has been 
         * given out by GetVDMPointer32W16),
         * only try to realloc in place
         */

	if (ptr)
            newptr = HeapReAlloc( heap,
		(pArena->pageLockCount > 0) ? HEAP_REALLOC_IN_PLACE_ONLY : 0, 
                              ptr, size + add_size );
	else
            newptr = HeapAlloc( heap, 0, size + add_size );

    }

    if (!newptr)
    {
        FIXME("Realloc failed lock %d\n",pArena->pageLockCount);
        if (pArena->pageLockCount <1)
        {
            if (pArena->flags & GA_DOSMEM)
                DOSMEM_FreeBlock( pArena->base );
            else
                HeapFree( heap, 0, ptr );
            SELECTOR_FreeBlock( sel );
            memset( pArena, 0, sizeof(GLOBALARENA) );
        }
        return 0;
    }
    ptr = newptr;

      /* Reallocate the selector(s) */

    sel = SELECTOR_ReallocBlock( sel, ptr, size + add_size);
    if (!sel)
    {
        if (pArena->flags & GA_DOSMEM)
            DOSMEM_FreeBlock( pArena->base );
        else
            HeapFree( heap, 0, ptr );
        memset( pArena, 0, sizeof(GLOBALARENA) );
        return 0;
    }
    selcount = (size + 0xffff) / 0x10000;

    if (!(pNewArena = GLOBAL_GetArena( sel, selcount )))
    {
        if (pArena->flags & GA_DOSMEM)
            DOSMEM_FreeBlock( pArena->base );
        else
            HeapFree( heap, 0, ptr );
        SELECTOR_FreeBlock( sel );
        return 0;
    }

      /* Fill the new arena block
         As we may have used HEAP_REALLOC_IN_PLACE_ONLY, areas may overlap*/

    if (pNewArena != pArena)
    {
        clear_sel_table( handle, pArena->selCount );
        memmove( pNewArena, pArena, sizeof(GLOBALARENA) );
        memset( pArena, 0, sizeof(GLOBALARENA) );
        set_sel_table( pNewArena->handle, selcount );
    }
    pNewArena->base = ptr;
    pNewArena->size = GetSelectorLimit16(sel) + 1 - add_size;
    pNewArena->selCount = selcount;
    pNewArena->handle = (pNewArena->flags & GA_MOVEABLE) ? sel - 1 : sel;

    if (selcount > 1)  /* clear the next arena blocks */
        memset( pNewArena + 1, 0, (selcount - 1) * sizeof(GLOBALARENA) );

    if ((oldsize < size) && (flags & GMEM_ZEROINIT))
        memset( (char *)ptr + oldsize, 0, size - oldsize );
    return !(pNewArena->handle ^ handle & ~1) ? handle : pNewArena->handle;
}


/***********************************************************************
 *           GlobalFree     (KERNEL.17)
 *           GlobalFree16   (KERNEL32.31)
 * RETURNS
 *	NULL: Success
 *	Handle: Failure
 */
HGLOBAL16 WINAPI GlobalFree16(
                 HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    void *ptr;

    if (!VALID_HANDLE(handle))
    {
        WARN("Invalid handle 0x%04x passed to GlobalFree16!\n",handle);
        return 0;
    }
    GLOBALARENA *pArena = GET_ARENA_PTR(handle);
    ptr = pArena->base;
    if (pArena->wType == GT_INTERNAL)
        return 0;

    TRACE("%04x\n", handle );
    if (pArena->dib_avail_size)
    {
        FIXME("DIB.DRV\n");
        return 0;
    }
    HGLOBAL ddehndl = GLOBAL_GetLink(handle);
    if (!GLOBAL_FreeBlock( handle )) return handle;  /* failed */
    HeapFree( get_win16_heap(), 0, ptr );
    if (ddehndl) GlobalFree(ddehndl);
    return 0;
}


/**********************************************************************
 *           K32WOWGlobalLock16         (KERNEL32.60)
 */
SEGPTR WINAPI K32WOWGlobalLock16( HGLOBAL16 handle )
{
    WORD sel = GlobalHandleToSel16( handle );
    TRACE("(%04x) -> %08x\n", handle, MAKELONG( 0, sel ) );

    if (handle)
    {
	if (handle == (HGLOBAL16)-1) handle = CURRENT_DS;

	if (!VALID_HANDLE(handle)) {
	    WARN("Invalid handle 0x%04x passed to WIN16_GlobalLock16!\n",handle);
	    sel = 0;
	}
	else if (!GET_ARENA_PTR(handle)->base)
            sel = 0;
        else if ((GET_ARENA_PTR(handle)->flags & GA_DISCARDABLE) || IsOldWindowsTask(GetCurrentTask()))
            GET_ARENA_PTR(handle)->lockCount++;
    }

    return MAKESEGPTR( sel, 0 );

}

/*
WOW32
GLOBALALLOC
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=000016ce,edx=00040002,ecx=000016ce,eax=000016ce
GLOBALLOCK
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=000016cf,ecx=000016cf,eax=00000000
GLOBALUNLOCK
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=000016ce,ecx=00000000,eax=00000000
GLOBALREALLOC
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=000002a0,edx=00000000,ecx=000016ce,eax=000016ce
GLOBALSIZE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=0000267e,edx=00000000,ecx=00000200,eax=00002000
GLOBALHANDLE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00002676,edx=000016cf,ecx=00000000,eax=000016ce
GLOBALFLAGS
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=0000267e,edx=000016cf,ecx=00000000,eax=00000000
GLOBALFREE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=16e7,edi=00000000,esi=00000000,ebx=00000fc0,edx=00000000,ecx=00000000,eax=00000000

GLOBALALLOC
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=000016ce,edx=00040002,ecx=000016ce,eax=000016ce
GLOBALLOCK
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcd16cf,ecx=cdcd16cf,eax=00000000
GLOBALUNLOCK
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcd16ce,ecx=cdcd0000,eax=00000000
GLOBALREALLOC
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=000002a0,edx=00000000,ecx=000016ce,eax=000016ce
GLOBALSIZE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd267e,edx=cdcd0000,ecx=cdcd0200,eax=00002000
GLOBALHANDLE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd2676,edx=cdcd16cf,ecx=cdcdcdcd,eax=000016ce
GLOBALFLAGS
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd267e,edx=cdcd16cf,ecx=cdcd0000,eax=00000000
GLOBALFREE
gs=0000,fs=0000,es=16e7,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=16e7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=00000fc0,edx=00000000,ecx=cdcd0000,eax=00000000

WIN31
GLOBALALLOC
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=00001dce,edx=810b0002,ecx=00001dce,eax=00001dce
GLOBALLOCK
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00001dcf,ecx=00001dcf,eax=00000000
GLOBALUNLOCK
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00001dce,ecx=00000000,eax=00000000
GLOBALREALLOC
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00001dce,eax=00001dce
GLOBALSIZE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=0000267e,edx=00000000,ecx=00000200,eax=00002000
GLOBALHANDLE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00002676,edx=00001dcf,ecx=00000000,eax=00001dce
GLOBALFLAGS
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=0000267e,edx=00001dcf,ecx=00000000,eax=00000000
GLOBALFREE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=00000000,esi=00000000,ebx=00000000,edx=00000000,ecx=00000000,eax=00000000
gs=0000,fs=0000,es=0000,ds=1de7,edi=00000000,esi=00000000,ebx=00004c20,edx=00000000,ecx=00000000,eax=00000000

GLOBALALLOC
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=00001dce,edx=810b0002,ecx=00001dce,eax=00001dce
GLOBALLOCK
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcd1dcf,ecx=cdcd1dcf,eax=00000000
GLOBALUNLOCK
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcd1dce,ecx=cdcd0000,eax=00000000
GLOBALREALLOC
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=00000000,edx=00000000,ecx=00001dce,eax=00001dce
GLOBALSIZE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd267e,edx=cdcd0000,ecx=cdcd0200,eax=00002000
GLOBALHANDLE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd2676,edx=cdcd1dcf,ecx=cdcdcdcd,eax=00001dce
GLOBALFLAGS
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcd267e,edx=cdcd1dcf,ecx=cdcd0000,eax=00000000
GLOBALFREE
gs=0000,fs=0000,es=1de7,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=cdcdcdcd,edx=cdcdcdcd,ecx=cdcdcdcd,eax=cdcdcdcd
gs=0000,fs=0000,es=0000,ds=1de7,edi=cdcdcdcd,esi=cdcdcdcd,ebx=00004bc0,edx=00000000,ecx=cdcd0000,eax=00000000

GLOBALALLOC
es=0 ecx=ebx=eax=handle
edx=???
GLOBALLOCK
es=0 cx=dx=segment
eax=offset
GLOBALUNLOCK
es=0 dx=handle
eax=result
cx=0?result?
GLOBALREALLOC
es=0 ecx=eax=handle
edx=0?
ebx=???
GLOBALSIZE
es=0 eax=size(low)
cx=size>>4?
dx=size(high)
bx=???
GLOBALHANDLE
es=es
eax=handle dx=segment
bx=???
GLOBALFLAGS
es=0 eax=result
cx=0?result?
dx=segment
bx=???
GLOBALFREE
es=0 eax=result
cx=0?result?
edx=0?result?
ebx=???
*/
/* yes, win16 sets es to 0 */

void WINAPI WIN16_GlobalAlloc16(UINT16 flags, DWORD size, CONTEXT *context)
{
    context->SegEs= 0;
    context->Ecx = context->Ebx = context->Eax = GlobalAlloc16(flags, size);
}

/***********************************************************************
 *           GlobalLock   (KERNEL.18)
 *
 * This is the GlobalLock16() function used by 16-bit code.
 */
SEGPTR WINAPI WIN16_GlobalLock16(HGLOBAL16 handle)
{
    SEGPTR ret = K32WOWGlobalLock16(handle);
    CURRENT_STACK16->ecx &= ~0xffff;
    CURRENT_STACK16->ecx |= SELECTOROF(ret);  /* selector must be returned in CX as well */
    CURRENT_STACK16->es = 0;
    return ret;
}

HGLOBAL16 WINAPI WIN16_GlobalReAlloc16(HGLOBAL16 handle, DWORD size, UINT16 flags)
{
    CURRENT_STACK16->es = 0;
    return CURRENT_STACK16->ecx = GlobalReAlloc16(handle, size, flags);
}
DWORD WINAPI WIN16_GlobalSize16(HGLOBAL16 handle)
{
    DWORD size = GlobalSize16(handle);
    CURRENT_STACK16->es = 0;
    CURRENT_STACK16->ecx &= ~0xffff;
    CURRENT_STACK16->ecx |= (WORD)((size >> 4) & 0xffff);
    return size;
}
DWORD WINAPI WIN16_GlobalFlags16(HGLOBAL16 handle)
{
    CURRENT_STACK16->es = 0;
    return MAKELONG(GlobalFlags16(handle), GlobalHandleToSel16(handle));
}
DWORD WINAPI WIN16_GlobalFree16(HGLOBAL16 handle)
{
    CURRENT_STACK16->es = 0;
    CURRENT_STACK16->fs = 0;
    CURRENT_STACK16->gs = 0;
    return GlobalFree16(handle);
}
void regen_icon(HICON16 icon);
DWORD WINAPI WIN16_GlobalUnlock16(HGLOBAL16 handle)
{
    DWORD ret = MAKELONG(GlobalUnlock16(handle), handle);
    CURRENT_STACK16->es = 0;
    if (!ret) return 0;
    GLOBALARENA *pArena = GET_ARENA_PTR(handle);
    if (pArena->wType == (GT_RESOURCE | (12 << 4))) // GD_CURSOR
    {
        static void (*regen_icon)(HICON16) = 0;
        if (!regen_icon)
            regen_icon = (void (*)(HICON16))GetProcAddress(GetModuleHandle("user.exe16"), "regen_icon");
        regen_icon((HICON16)handle);
    }
    return ret;
}


/***********************************************************************
 *           GlobalLock16   (KERNEL32.25)
 *
 * This is the GlobalLock16() function used by 32-bit code.
 *
 * RETURNS
 *	Pointer to first byte of memory block
 *	NULL: Failure
 */
LPVOID WINAPI GlobalLock16(
              HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    if (!handle) return 0;
    if (!VALID_HANDLE(handle))
        return 0;
    // don't use IsOldWindowsTask here as it'll cause an infinite loop
    if (GET_ARENA_PTR(handle)->flags & GA_DISCARDABLE)
        GET_ARENA_PTR(handle)->lockCount++;
    return GET_ARENA_PTR(handle)->base;
}


/***********************************************************************
 *           GlobalUnlock     (KERNEL.19)
 *           GlobalUnlock16   (KERNEL32.26)
 * NOTES
 *	Should the return values be cast to booleans?
 *
 * RETURNS
 *	TRUE: Object is still locked
 *	FALSE: Object is unlocked
 */
BOOL16 WINAPI GlobalUnlock16(
              HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    GLOBALARENA *pArena = GET_ARENA_PTR(handle);
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalUnlock16!\n",handle);
        return FALSE;
    }
    TRACE("%04x\n", handle );
    if (pArena->lockCount) pArena->lockCount--;
    if (CURRENT_STACK16)
        CURRENT_STACK16->es = 0;
    return pArena->lockCount;
}

BOOL16 WINAPI WIN32_GlobalUnlock16(
              HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    GLOBALARENA *pArena = GET_ARENA_PTR(handle);
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalUnlock16!\n",handle);
        return FALSE;
    }
    TRACE("%04x\n", handle );
    if (pArena->lockCount) pArena->lockCount--;
    return pArena->lockCount;
}


/***********************************************************************
 *     GlobalChangeLockCount               (KERNEL.365)
 *
 * This is declared as a register function as it has to preserve
 * *all* registers, even AX/DX !
 *
 */
void WINAPI GlobalChangeLockCount16( HGLOBAL16 handle, INT16 delta, CONTEXT *context )
{
    if ( delta == 1 )
        GlobalLock16( handle );
    else if ( delta == -1 )
        GlobalUnlock16( handle );
    else
        ERR("(%04X, %d): strange delta value\n", handle, delta );
}

/***********************************************************************
 *           GlobalSize     (KERNEL.20)
 *           GlobalSize16   (KERNEL32.32)
 * 
 * Get the current size of a global memory object.
 *
 * RETURNS
 *	Size in bytes of object
 *	0: Failure
 */
DWORD WINAPI GlobalSize16(
             HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    TRACE("%04x\n", handle );
    if (!handle) return 0;
    if (!VALID_HANDLE(handle))
	return 0;
    return GET_ARENA_PTR(handle)->size;
}


/***********************************************************************
 *           GlobalHandle   (KERNEL.21)
 *
 * Get the handle associated with a pointer to the global memory block.
 *
 * NOTES
 *	Why is GlobalHandleToSel used here with the sel as input?
 *
 * RETURNS
 *	Handle: Success
 *	NULL: Failure
 */
DWORD WINAPI GlobalHandle16(
             WORD sel /* [in] Address of global memory block */
) {
    TRACE("%04x\n", sel );
    if (!VALID_HANDLE(sel)) {
	WARN("Invalid handle 0x%04x passed to GlobalHandle16!\n",sel);
	return 0;
    }
    WORD handle = GET_ARENA_PTR(sel)->handle;
    if (!handle)
        return 0;
    return MAKELONG( handle, GlobalHandleToSel16(sel) );
}

/***********************************************************************
 *           GlobalHandleNoRIP   (KERNEL.159)
 */
DWORD WINAPI GlobalHandleNoRIP16( WORD sel )
{
    int i;
    for (i = globalArenaSize-1 ; i>=0 ; i--) {
        if (pGlobalArena[i].size!=0 && pGlobalArena[i].handle == sel)
		return MAKELONG( GET_ARENA_PTR(sel)->handle, GlobalHandleToSel16(sel) );
    }
    return 0;
}


/***********************************************************************
 *           GlobalFlags     (KERNEL.22)
 *
 * Get information about a global memory object.
 *
 * NOTES
 *	Should this return GMEM_INVALID_HANDLE instead of 0 on invalid
 *	handle?
 *
 * RETURNS
 *	Value specifying flags and lock count
 *	GMEM_INVALID_HANDLE: Invalid handle
 */
UINT16 WINAPI GlobalFlags16(
              HGLOBAL16 handle /* [in] Handle of global memory object */
) {
    GLOBALARENA *pArena;

    TRACE("%04x\n", handle );
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalFlags16!\n",handle);
	return 0;
    }
    pArena = GET_ARENA_PTR(handle);
    return pArena->lockCount |
           ((pArena->flags & GA_DISCARDABLE) ? GMEM_DISCARDABLE : 0) |
           ((pArena->base == 0) ? GMEM_DISCARDED : 0);
}


/***********************************************************************
 *           LockSegment   (KERNEL.23)
 */
HGLOBAL16 WINAPI LockSegment16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (handle == (HGLOBAL16)-1) handle = CURRENT_DS;
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to LockSegment16!\n",handle);
	return 0;
    }
    if ((GET_ARENA_PTR(handle)->flags & GA_DISCARDABLE) || IsOldWindowsTask(GetCurrentTask()))
        GET_ARENA_PTR(handle)->lockCount++;
    return handle;
}


/***********************************************************************
 *           UnlockSegment   (KERNEL.24)
 */
void WINAPI UnlockSegment16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (handle == (HGLOBAL16)-1) handle = CURRENT_DS;
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to UnlockSegment16!\n",handle);
	return;
    }
    if(GET_ARENA_PTR(handle)->lockCount) GET_ARENA_PTR(handle)->lockCount--;
    /* FIXME: this ought to return the lock count in CX (go figure...) */
}


/***********************************************************************
 *           GlobalCompact   (KERNEL.25)
 */
DWORD WINAPI GlobalCompact16( DWORD desired )
{
    return GLOBAL_MAX_ALLOC_SIZE;
}


/***********************************************************************
 *           GlobalFreeAll   (KERNEL.26)
 */
void WINAPI GlobalFreeAll16( HGLOBAL16 owner )
{
    int i;
    GLOBALARENA *pArena;

    pArena = pGlobalArena;
    for (i = 0; i < globalArenaSize; i++, pArena++)
    {
        if ((pArena->size != 0) && (pArena->hOwner == owner))
            GlobalFree16( pArena->handle );
    }
}


/***********************************************************************
 *           GlobalWire     (KERNEL.111)
 *           GlobalWire16   (KERNEL32.29)
 */
SEGPTR WINAPI GlobalWire16( HGLOBAL16 handle )
{
    return WIN16_GlobalLock16( handle );
}


/***********************************************************************
 *           GlobalUnWire     (KERNEL.112)
 *           GlobalUnWire16   (KERNEL32.30)
 */
BOOL16 WINAPI GlobalUnWire16( HGLOBAL16 handle )
{
    return !GlobalUnlock16( handle );
}


/***********************************************************************
 *           SetSwapAreaSize   (KERNEL.106)
 */
LONG WINAPI SetSwapAreaSize16( WORD size )
{
    FIXME("(%d) - stub!\n", size );
    return MAKELONG( size, 0xffff );
}


/***********************************************************************
 *           GlobalLRUOldest   (KERNEL.163)
 */
HGLOBAL16 WINAPI GlobalLRUOldest16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (handle == (HGLOBAL16)-1) handle = CURRENT_DS;
    return handle;
}


/***********************************************************************
 *           GlobalLRUNewest   (KERNEL.164)
 */
HGLOBAL16 WINAPI GlobalLRUNewest16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (handle == (HGLOBAL16)-1) handle = CURRENT_DS;
    return handle;
}


/***********************************************************************
 *           GetFreeSpace   (KERNEL.169)
 */
DWORD WINAPI GetFreeSpace16( UINT16 wFlags )
{
    MEMORYSTATUS ms;
    GlobalMemoryStatus( &ms );
    return min(ms.dwAvailVirtual, 102400000);
}

/***********************************************************************
 *           GlobalDOSAlloc   (KERNEL.184)
 *
 * Allocate memory in the first MB.
 *
 * RETURNS
 *	Address (HW=Paragraph segment; LW=Selector)
 */
DWORD WINAPI GlobalDOSAlloc16(
             DWORD size /* [in] Number of bytes to be allocated */
) {
   UINT16    uParagraph;
   LPVOID    lpBlock;
   DWORD fixup_size = 0x1f;
   size = (size + fixup_size) & ~fixup_size;
   lpBlock = DOSMEM_AllocBlock(size, &uParagraph);

   if( lpBlock )
   {
       HMODULE16 hModule = GetModuleHandle16("KERNEL");
       WORD	 wSelector;
       GLOBALARENA *pArena;

       wSelector = GLOBAL_CreateBlock(GMEM_FIXED, lpBlock, size, hModule, WINE_LDT_FLAGS_DATA, 0 );
       pArena = GET_ARENA_PTR(wSelector);
       pArena->flags |= GA_DOSMEM;
       return MAKELONG(wSelector,uParagraph);
   }
   return 0;
}


/***********************************************************************
 *           GlobalDOSFree      (KERNEL.185)
 *
 * Free memory allocated with GlobalDOSAlloc
 *
 * RETURNS
 *	NULL: Success
 *	sel: Failure
 */
WORD WINAPI GlobalDOSFree16(
            WORD sel /* [in] Selector */
) {
   DWORD   block = GetSelectorBase(sel);

   if( block && block < 0x100000 )
   {
       LPVOID lpBlock = DOSMEM_MapDosToLinear( block );
       if( DOSMEM_FreeBlock( lpBlock ) )
	   GLOBAL_FreeBlock( sel );
       sel = 0;
   }
   return sel;
}


/***********************************************************************
 *           GlobalPageLock   (KERNEL.191)
 *           GlobalSmartPageLock(KERNEL.230)
 */
WORD WINAPI GlobalPageLock16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalPageLock!\n",handle);
	return 0;
    }
    return ++(GET_ARENA_PTR(handle)->pageLockCount);
}


/***********************************************************************
 *           GlobalPageUnlock   (KERNEL.192)
 *           GlobalSmartPageUnlock(KERNEL.231)
 */
WORD WINAPI GlobalPageUnlock16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalPageUnlock!\n",handle);
	return 0;
    }
    return --(GET_ARENA_PTR(handle)->pageLockCount);
}


/***********************************************************************
 *           GlobalFix     (KERNEL.197)
 *           GlobalFix16   (KERNEL32.27)
 */
WORD WINAPI GlobalFix16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalFix16!\n",handle);
	return 0;
    }
    if ((GET_ARENA_PTR(handle)->flags & GA_DISCARDABLE) || IsOldWindowsTask(GetCurrentTask()))
        GET_ARENA_PTR(handle)->lockCount++;

    return GlobalHandleToSel16(handle);
}


/***********************************************************************
 *           GlobalUnfix     (KERNEL.198)
 *           GlobalUnfix16   (KERNEL32.28)
 */
void WINAPI GlobalUnfix16( HGLOBAL16 handle )
{
    TRACE("%04x\n", handle );
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalUnfix16!\n",handle);
	return;
    }
    if (GET_ARENA_PTR(handle)->lockCount)
        GET_ARENA_PTR(handle)->lockCount--;
}


/***********************************************************************
 *           FarSetOwner   (KERNEL.403)
 */
void WINAPI FarSetOwner16( HGLOBAL16 handle, HANDLE16 hOwner )
{
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to FarSetOwner!\n",handle);
	return;
    }
    GET_ARENA_PTR(handle)->hOwner = hOwner;
}


/***********************************************************************
 *           FarGetOwner   (KERNEL.404)
 */
HANDLE16 WINAPI FarGetOwner16( HGLOBAL16 handle )
{
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to FarGetOwner!\n",handle);
	return 0;
    }
    return GET_ARENA_PTR(handle)->hOwner;
}


/************************************************************************
 *              GlobalMasterHandle (KERNEL.28)
 *
 *
 * Should return selector and handle of the information structure for
 * the global heap. selector and handle are stored in the THHOOK as
 * pGlobalHeap and hGlobalHeap.
 * As Wine doesn't have this structure, we return both values as zero
 * Applications should interpret this as "No Global Heap"
 */
DWORD WINAPI GlobalMasterHandle16(void)
{
    DWORD ret = 0;
    FIXME(": stub\n");
    // some real mode programs don't check for null, provide enough to prevent crashes
    if (IsOldWindowsTask(GetCurrentTask()))
    {
        // this is an early form of LOCALHEAPINFO
        static WORD dummyheap[0x18];
        static WORD sel = 0;
        if (!sel)
        {
            sel = AllocSelector16(0);
            SetSelectorBase(sel, (DWORD)dummyheap);
            SetSelectorLimit16(sel, 0x30);
            SelectorAccessRights16(sel, 1, 0); // data, readonly
            dummyheap[0] = 'Z'; // check value don't care so pretend MCB end of chain
            dummyheap[2] = 0; // count is 0
            dummyheap[3] = sel; // start of chain
            dummyheap[4] = sel; // pretend MCB pointer
            dummyheap[18] = -1; // unknown
        }
        ret = sel << 16;
    }
    return ret;
}

/***********************************************************************
 *           GlobalHandleToSel   (TOOLHELP.50)
 *
 * FIXME: This is in TOOLHELP but we keep a copy here for now.
 */
WORD WINAPI GlobalHandleToSel16( HGLOBAL16 handle )
{
    if (!handle) return 0;
    if (!VALID_HANDLE(handle)) {
	WARN("Invalid handle 0x%04x passed to GlobalHandleToSel!\n",handle);
	return 0;
    }
    if (!(handle & 7))
    {
        WARN("Program attempted invalid selector conversion\n" );
        return handle - 1;
    }
    return handle | 7;
}


/***********************************************************************
 *           GetFreeMemInfo   (KERNEL.316)
 */
DWORD WINAPI GetFreeMemInfo16(void)
{
    SYSTEM_BASIC_INFORMATION info;
    MEMORYSTATUS status;

    NtQuerySystemInformation( SystemBasicInformation, &info, sizeof(info), NULL );
    GlobalMemoryStatus( &status );
    return MAKELONG( status.dwTotalVirtual / info.PageSize, status.dwAvailVirtual / info.PageSize );
}

/***********************************************************************
 *           A20Proc   (KERNEL.165)
 */
void WINAPI A20Proc16( WORD unused )
{
    /* this is also a NOP in Windows */
}

/***********************************************************************
 *           LimitEMSPages   (KERNEL.156)
 */
DWORD WINAPI LimitEMSPages16( DWORD unused )
{
    return 0;
}

void WINAPI DibMapGlobalMemory(WORD sel, void *base, DWORD size)
{
    GLOBALARENA *pArena = GET_ARENA_PTR(sel);
    int i;
    if (!sel) /* not hglobal */
    {
        SetSelectorBase(sel, base);
        return;
    }
    pArena->dib_avail_size = size;
    pArena->base = base;
    for (i = 0; i < pArena->selCount; i++)
    {
        SetSelectorBase(sel + i * 8, (LPBYTE)base + i * 0x10000);
    }
}

void WINAPI DibUnmapGlobalMemory(void *base, DWORD size)
{
    int i;
    LPVOID heap = HeapAlloc(get_win16_heap(), 0, size);
    memcpy(heap, base, size);
    for (i = 0; i < globalArenaSize; i++)
    {
        GLOBALARENA *pArena = pGlobalArena + i;
        if (!pArena->dib_avail_size)
            continue;
        if (pArena->base >= base && (LPBYTE)pArena->base < (LPBYTE)base + size)
        {
            pArena->dib_avail_size = 0;
            pArena->base = (LPBYTE)heap + ((SIZE_T)pArena->base - (SIZE_T)base);
            for (i = 0; i < pArena->selCount; i++)
                SetSelectorBase(pArena->handle + i * 8, (LPBYTE)pArena->base + i * 0x10000);
        }
    }
}

void WINAPI GlobalMapInternal(WORD sel, void *base, DWORD size)
{
    GLOBALARENA *pArena = GET_ARENA_PTR(sel);
    if (!sel)
        return;
    if (!base || !size)
    {
        memset(pArena, 0, sizeof(GLOBALARENA));
        return;
    }
    pArena->base = base;
    pArena->size = size;
    pArena->wType = GT_INTERNAL;
}
