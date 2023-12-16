
#include "config.h"
#include "wine/port.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "windef.h"
#include "winbase.h"
#include "wine/library.h"
#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(ldt);
static inline int is_gdt_sel(unsigned short sel) { return !(sel & 4); }
static const LDT_ENTRY null_entry;  /* all-zeros, used to clear LDT entries */
struct __wine_ldt_copy wine_ldt_copy;
LDT_ENTRY wine_ldt[8192];
// windows allocates selectors from a linked list of avaliable so
// if a sel is freed, the next alloced will be the last freed, simulate for one
static WORD last_freed = 0;
/* empty function for default locks */
static void nop(void) { }

static void(*lock_ldt)(void) = nop;
static void(*unlock_ldt)(void) = nop;
#define LDT_FIRST_ENTRY 512
#define LDT_SIZE 8192

/***********************************************************************
 *           wine_ldt_get_ptr
 *
 * Convert a segment:offset pair to a linear pointer.
 * Note: we don't lock the LDT since this has to be fast.
 */
void *wine_ldt_get_ptr(unsigned short sel, unsigned long offset)
{
	int index;

	if (is_gdt_sel(sel))  /* GDT selector */
		return (void *)offset;
	if ((index = (sel >> 3)) < LDT_FIRST_ENTRY)  /* system selector */
		return (void *)offset;
	if (!(wine_ldt_copy.flags[index] & WINE_LDT_FLAGS_32BIT)) offset &= 0xffff;
	return (char *)wine_ldt_copy.base[index] + offset;
}
static BOOL intel_vt_x_workaround = FALSE;
static void (*intel_vt_x_workaround_update_entry)(int seg, const LDT_ENTRY *entry);
void set_intel_vt_x_workaround(void(*func)(int seg, const LDT_ENTRY *entry))
{
    intel_vt_x_workaround = TRUE;
    intel_vt_x_workaround_update_entry = func;
    for (int index = LDT_FIRST_ENTRY; index < LDT_SIZE; index++)
    {
        if (intel_vt_x_workaround_update_entry)
            intel_vt_x_workaround_update_entry(index << 3, wine_ldt + index);
    }
}
/***********************************************************************
*           wine_ldt_set_entry
*
* Set an LDT entry.
*/
int wine_ldt_set_entry(unsigned short sel, const LDT_ENTRY *entry)
{
	int ret = 0, index = sel >> 3;
	ret = TRUE;
	if (ret >= 0)
	{
        wine_ldt[index] = *entry;
		if (intel_vt_x_workaround)
		{
			intel_vt_x_workaround_update_entry(sel, entry);
		}
		wine_ldt_copy.base[index] = wine_ldt_get_base(entry);
		wine_ldt_copy.limit[index] = wine_ldt_get_limit(entry);
		wine_ldt_copy.flags[index] = (entry->HighWord.Bits.Type |
			(entry->HighWord.Bits.Default_Big ? WINE_LDT_FLAGS_32BIT : 0) |
			(wine_ldt_copy.flags[index] & WINE_LDT_FLAGS_ALLOCATED));
	}

	TRACE("wine_ldt_set_entry(0x%04X, %p)\n", sel, entry);
	return 0;
}
/***********************************************************************
*           wine_ldt_get_entry
*
* Retrieve an LDT entry. Return a null entry if selector is not allocated.
*/
void wine_ldt_get_entry(unsigned short sel, LDT_ENTRY *entry)
{
	int index = sel >> 3;

	if (is_gdt_sel(sel))
	{
		*entry = null_entry;
		return;
	}
	lock_ldt();
	if (wine_ldt_copy.flags[index] & WINE_LDT_FLAGS_ALLOCATED)
	{
		wine_ldt_set_base(entry, wine_ldt_copy.base[index]);
		wine_ldt_set_limit(entry, wine_ldt_copy.limit[index]);
		wine_ldt_set_flags(entry, wine_ldt_copy.flags[index]);
	}
	else *entry = null_entry;
	unlock_ldt();
	TRACE("wine_ldt_get_entry(0x%04X, %p)\n", sel, entry);
	return;
}
/***********************************************************************
*           wine_ldt_alloc_entries
*
* Allocate a number of consecutive ldt entries, without setting the LDT contents.
* Return a selector for the first entry.
*/
unsigned short sel = 0x1000;
unsigned short wine_ldt_alloc_entries(int count)
{

	int i, index, size = 0;

	if (count <= 0)
	{
		TRACE("wine_ldt_alloc_entries(%d) = %d\n", count, 0);
		return 0;
	}
	lock_ldt();
	if ((count == 1) && last_freed)
	{
		WORD e = last_freed >> 3;
		last_freed = 0;
	 	if (!(wine_ldt_copy.flags[e] & WINE_LDT_FLAGS_ALLOCATED))
		{
			wine_ldt_copy.flags[e] |= WINE_LDT_FLAGS_ALLOCATED;
			unlock_ldt();
			return (e << 3) | 7;
		}
	}

	for (i = LDT_FIRST_ENTRY; i < LDT_SIZE; i++)
	{
		if (wine_ldt_copy.flags[i] & WINE_LDT_FLAGS_ALLOCATED) size = 0;
		else if (++size >= count)  /* found a large enough block */
		{
			index = i - size + 1;

			/* mark selectors as allocated */
			for (i = 0; i < count; i++) wine_ldt_copy.flags[index + i] |= WINE_LDT_FLAGS_ALLOCATED;
			unlock_ldt();
			//DPRINTF("NOTIMPL:wine_ldt_alloc_entries(%d) = %d\n", count, (index << 3) | 7);
			return (index << 3) | 7;
		}
	}
	unlock_ldt();
	TRACE("wine_ldt_alloc_entries(%d) = %d\n", count, 0);
	return 0;
	TRACE("wine_ldt_alloc_entries(%d) = %d\n", count, sel);
	count *= 0x10000;
	unsigned short e = sel;
	sel += (count + 16) / 16;
	return e;
}
/***********************************************************************
*           wine_ldt_realloc_entries
*
* Reallocate a number of consecutive ldt entries, without changing the LDT contents.
* Return a selector for the first entry.
*/
#include <wownt32.h>
unsigned short wine_ldt_realloc_entries(unsigned short sel, int oldcount, int newcount)
{
	int i;

	if (oldcount < newcount)  /* we need to add selectors */
	{
		int index = sel >> 3;

		lock_ldt();
		/* check if the next selectors are free */
		if (index + newcount > LDT_SIZE) i = oldcount;
		else
			for (i = oldcount; i < newcount; i++)
				if (wine_ldt_copy.flags[index + i] & WINE_LDT_FLAGS_ALLOCATED) break;

		if (i < newcount)  /* they are not free */
		{
			wine_ldt_free_entries(sel, oldcount);
			sel = wine_ldt_alloc_entries(newcount);
		}
		else  /* mark the selectors as allocated */
		{
			for (i = oldcount; i < newcount; i++)
				wine_ldt_copy.flags[index + i] |= WINE_LDT_FLAGS_ALLOCATED;
		}
		unlock_ldt();
	}
	else if (oldcount > newcount) /* we need to remove selectors */
	{
		wine_ldt_free_entries(sel + (newcount << 3), newcount - oldcount);
	}
	return sel;
	TRACE("wine_ldt_realloc_entries(0x%04X,%d,%d)\n", sel, oldcount, newcount);
	return 0;
}
/***********************************************************************
*           internal_set_entry
*
* Set an LDT entry, without locking. For internal use only.
*/
static int internal_set_entry(unsigned short sel, const LDT_ENTRY *entry)
{
	int ret = 0, index = sel >> 3;

	if (ret >= 0)
	{
		wine_ldt[index] = *entry;
		wine_ldt_copy.base[index] = wine_ldt_get_base(entry);
		wine_ldt_copy.limit[index] = wine_ldt_get_limit(entry);
		wine_ldt_copy.flags[index] = (entry->HighWord.Bits.Type |
			(entry->HighWord.Bits.Default_Big ? WINE_LDT_FLAGS_32BIT : 0) |
			(wine_ldt_copy.flags[index] & WINE_LDT_FLAGS_ALLOCATED));
	}
	return ret;
}
/***********************************************************************
*           wine_ldt_free_entries
*
* Free a number of consecutive ldt entries and clear their contents.
*/
void wine_ldt_free_entries(unsigned short sel, int count)
{
	int index;

	lock_ldt();
	for (index = sel >> 3; count > 0; count--, index++)
	{
		internal_set_entry(sel, &null_entry);
		wine_ldt_copy.flags[index] = 0;
	}
	last_freed = sel;
	unlock_ldt();
	TRACE("wine_ldt_free_entries(0x%04X,%d)\n", sel, count);
}
/***********************************************************************
*           wine_ldt_is_system
*
* Check if the selector is a system selector (i.e. not managed by Wine).
*/
int wine_ldt_is_system(unsigned short sel)
{
	return is_gdt_sel(sel) || ((sel >> 3) < LDT_FIRST_ENTRY);
	TRACE("wine_ldt_is_system(%04X)\n", sel);
	return 0;
}


#if USE_SREG_WORKAROUND
/***********************************************************************
 *           selector access functions
 */
# if defined(__GNUC__) && ((__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 2)))
#  define __DEFINE_GET_SEG(seg) \
    unsigned short wine_get_##seg(void) \
    { unsigned short res; __asm__ __volatile__("movw %%" #seg ",%w0" : "=r"(res)); return res; }
#  define __DEFINE_SET_SEG(seg) \
    void wine_set_##seg(unsigned int val) \
    { __asm__("movw %w0,%%" #seg : : "r" (val)); }
# elif defined(_MSC_VER)
/* This cache is a workaround for ARM64 mac.  */
#  define __DEFINE_GET_SEG(seg) \
    unsigned short wine_get_##seg(void) \
    {\
        static int cache;\
        static unsigned short res;\
        if (!cache)\
        {\
            cache = 1;\
            __asm { mov res, seg }\
        }\
        return res;\
    }
#  define __DEFINE_SET_SEG(seg) \
    void wine_set_##seg(unsigned int val) { __asm { mov seg, word ptr [val] } }
# else  /* __GNUC__ || _MSC_VER */

# endif /* __GNUC__ || _MSC_VER */

__DEFINE_GET_SEG(cs)
__DEFINE_GET_SEG(ds)
__DEFINE_GET_SEG(es)
__DEFINE_GET_SEG(fs)
__DEFINE_GET_SEG(gs)
__DEFINE_GET_SEG(ss)
__DEFINE_SET_SEG(fs)
__DEFINE_SET_SEG(gs)
#undef __DEFINE_GET_SEG
#undef __DEFINE_SET_SEG
#endif
