/*
 * 16 bit ole functions
 *
 * Copyright 1995 Martin von Loewis
 * Copyright 1998 Justin Bradford
 * Copyright 1999 Francis Beaudet
 * Copyright 1999 Sylvain St-Germain
 * Copyright 2002 Marcus Meissner
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

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "winuser.h"
#include "objbase.h"
#include "ole2.h"
#include "rpc.h"
#include "winerror.h"
#include "winreg.h"
#include "wownt32.h"
#include "wtypes.h"
#include "wine/unicode.h"
#include "wine/winbase16.h"

#include "wine/debug.h"
#include "../ole2/ifs.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);
WINE_DECLARE_DEBUG_CHANNEL(olemalloc);

#define CHARS_IN_GUID 39


static HTASK16 hETask = 0;
static WORD Table_ETask[62];

static LPMALLOC16 currentMalloc16=NULL;

/* --- IMalloc16 implementation */


typedef struct
{
        IMalloc16 IMalloc16_iface;
        DWORD     ref;
} IMalloc16Impl;

static inline IMalloc16Impl *impl_from_IMalloc16(IMalloc16 *iface)
{
        return CONTAINING_RECORD(iface, IMalloc16Impl, IMalloc16_iface);
}

/******************************************************************************
 *		IMalloc16_QueryInterface	[COMPOBJ.500]
 */
HRESULT CDECL IMalloc16_fnQueryInterface(IMalloc16* iface,REFIID refiid,LPVOID *obj) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

	TRACE_(olemalloc)("(%p)->QueryInterface(%s,%p)\n",This,debugstr_guid(refiid),obj);
	if (	!memcmp(&IID_IUnknown,refiid,sizeof(IID_IUnknown)) ||
		!memcmp(&IID_IMalloc,refiid,sizeof(IID_IMalloc))
	) {
		*obj = This;
		return 0;
	}
	return OLE_E_ENUM_NOMORE;
}

/******************************************************************************
 *		IMalloc16_AddRef	[COMPOBJ.501]
 */
ULONG CDECL IMalloc16_fnAddRef(IMalloc16* iface) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

	TRACE_(olemalloc)("(%p)->AddRef()\n",This);
	return 1; /* cannot be freed */
}

/******************************************************************************
 *		IMalloc16_Release	[COMPOBJ.502]
 */
ULONG CDECL IMalloc16_fnRelease(IMalloc16* iface) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

	TRACE_(olemalloc)("(%p)->Release()\n",This);
	return 1; /* cannot be freed */
}

/******************************************************************************
 * IMalloc16_Alloc [COMPOBJ.503]
 */
SEGPTR CDECL IMalloc16_fnAlloc(IMalloc16* iface,DWORD cb) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

        LPVOID a = HeapAlloc( GetProcessHeap(), 0, cb );
        SEGPTR s = MapLS(a);
	TRACE_(olemalloc)("(%p)->Alloc(%d)=>%p(%04x:%04x)\n",This,cb,a,SELECTOROF(s),OFFSETOF(s));
        return s;
}

/******************************************************************************
 * IMalloc16_Free [COMPOBJ.505]
 */
VOID CDECL IMalloc16_fnFree(IMalloc16* iface,SEGPTR pv)
{
    void *ptr = MapSL(pv);
    IMalloc16Impl *This = impl_from_IMalloc16(iface);
    TRACE_(olemalloc)("(%p)->Free(%p(%04x:%04x))\n",This,ptr,SELECTOROF(pv),OFFSETOF(pv));
//    UnMapLS(pv);
    /*
     * Corel Move 5.0 passes the allocated pointer plus 4 bytes.
     * https://support.microsoft.com/en-us/help/286470/how-to-use-pageheap-exe-in-windows-xp-windows-2000-and-windows-server
     * The Windows heap managers (all versions) have always guaranteed that the
     * heap allocations have a start address that is 8-byte aligned (on 64-bit
     * platforms the alignment is 16-bytes).
     */
    HeapFree( GetProcessHeap(), 0, (LPVOID)((SIZE_T)ptr & ~7) );
}

/******************************************************************************
 * IMalloc16_Realloc [COMPOBJ.504]
 */
SEGPTR CDECL IMalloc16_fnRealloc(IMalloc16* iface,SEGPTR pv,DWORD cb)
{
    SEGPTR ret;
    IMalloc16Impl *This = impl_from_IMalloc16(iface);

    TRACE_(olemalloc)("(%p)->Realloc(%08x,%d)\n",This,pv,cb);
    if (!pv)
	ret = IMalloc16_fnAlloc(iface, cb);
    else if (cb) {
        ret = MapLS( HeapReAlloc( GetProcessHeap(), 0, MapSL(pv), cb ) );
        UnMapLS(pv);
    } else {
	IMalloc16_fnFree(iface, pv);
	ret = 0;
    }
    return ret;
}

/******************************************************************************
 * IMalloc16_GetSize [COMPOBJ.506]
 */
DWORD CDECL IMalloc16_fnGetSize(IMalloc16* iface,SEGPTR pv)
{
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

        TRACE_(olemalloc)("(%p)->GetSize(%08x)\n",This,pv);
        return HeapSize( GetProcessHeap(), 0, MapSL(pv) );
}

/******************************************************************************
 * IMalloc16_DidAlloc [COMPOBJ.507]
 */
INT16 CDECL IMalloc16_fnDidAlloc(IMalloc16* iface,LPVOID pv) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

	TRACE_(olemalloc)("(%p)->DidAlloc(%p)\n",This,pv);
	return (INT16)-1;
}

/******************************************************************************
 * IMalloc16_HeapMinimize [COMPOBJ.508]
 */
LPVOID CDECL IMalloc16_fnHeapMinimize(IMalloc16* iface) {
        IMalloc16Impl *This = impl_from_IMalloc16(iface);

	TRACE_(olemalloc)("(%p)->HeapMinimize()\n",This);
	return NULL;
}

/******************************************************************************
 * IMalloc16_Constructor [VTABLE]
 */
static LPMALLOC16
IMalloc16_Constructor(void)
{
    static IMalloc16Vtbl vt16;
    static SEGPTR msegvt16;
    IMalloc16Impl* This;
    HMODULE16 hcomp = GetModuleHandle16("COMPOBJ");

    This = HeapAlloc( GetProcessHeap(), 0, sizeof(IMalloc16Impl) );
    if (!msegvt16)
    {
#define VTENT(x) vt16.x = (void*)GetProcAddress16(hcomp,"IMalloc16_"#x);assert(vt16.x)
        VTENT(QueryInterface);
        VTENT(AddRef);
        VTENT(Release);
        VTENT(Alloc);
        VTENT(Realloc);
        VTENT(Free);
        VTENT(GetSize);
        VTENT(DidAlloc);
        VTENT(HeapMinimize);
#undef VTENT
        msegvt16 = MapLS( &vt16 );
    }
    This->IMalloc16_iface.lpVtbl = (const IMalloc16Vtbl*)msegvt16;
    This->ref = 1;
    return (LPMALLOC16)MapLS( This );
}


/******************************************************************************
 *           CoBuildVersion [COMPOBJ.1]
 */
DWORD WINAPI CoBuildVersion16(void)
{
    return CoBuildVersion();
}

typedef struct
{
    SEGPTR malloc16;
} ole16_task_data;
static BOOL init_current_task_tls;
static DWORD current_task_tls;
static ole16_task_data *get_current_task_data()
{
    ole16_task_data *d;
    if (!init_current_task_tls)
    {
        current_task_tls = TlsAlloc();
        init_current_task_tls = TRUE;
    }
    d = (ole16_task_data*)TlsGetValue(current_task_tls);
    if (!d)
    {
        d = (ole16_task_data*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ole16_task_data));
        TlsSetValue(current_task_tls, d);
    }
    return d;
}
SEGPTR shared_malloc16;
HRESULT WINAPI get_task_imalloc16(SEGPTR *lpMalloc)
{
    ole16_task_data *d = get_current_task_data();
    if (!d->malloc16)
    {
        *lpMalloc = 0;
        ERR("CO_E_NOTINITIALIZED\n");
        return CO_E_NOTINITIALIZED;
    }
    *lpMalloc = d->malloc16;
    return S_OK;
}
/***********************************************************************
 *           CoGetMalloc    [COMPOBJ.4]
 *
 * Retrieve the current win16 IMalloc interface.
 *
 * RETURNS
 *	The current win16 IMalloc
 */
HRESULT WINAPI CoGetMalloc16(
    MEMCTX dwMemContext,	/* [in] memory context */
    SEGPTR * lpMalloc	/* [out] current win16 malloc interface */
) {
    ole16_task_data *d;
    if (dwMemContext == MEMCTX_SHARED)
    {
        if (!shared_malloc16)
        {
            shared_malloc16 = IMalloc16_Constructor();
        }
        *lpMalloc = shared_malloc16;
        return S_OK;
    }
    d = get_current_task_data();
    if (!d->malloc16)
    {
        *lpMalloc = 0;
        ERR("CO_E_NOTINITIALIZED\n");
        return CO_E_NOTINITIALIZED;
    }
    if (dwMemContext == MEMCTX_TASK)
    {
        *lpMalloc = d->malloc16;
        IMalloc16_AddRef(d->malloc16);
        return S_OK;
    }
    *lpMalloc = 0;
    return E_INVALIDARG16;
}

/***********************************************************************
 *           CoCreateStandardMalloc [COMPOBJ.71]
 */
HRESULT WINAPI CoCreateStandardMalloc16(MEMCTX dwMemContext,
					  SEGPTR *lpMalloc)
{
    /* FIXME: docu says we shouldn't return the same allocator as in
     * CoGetMalloc16 */
    *lpMalloc = IMalloc16_Constructor();
    return S_OK;
}

/******************************************************************************
 *		CoInitialize	[COMPOBJ.2]
 * Set the win16 IMalloc used for memory management
 */
HRESULT WINAPI CoInitialize16(
	SEGPTR lpReserved	/* [in] pointer to win16 malloc interface */
) {
    ole16_task_data *d = get_current_task_data();
    if (d->malloc16)
        return S_FALSE;
    if (!lpReserved)
    {
        lpReserved = IMalloc16_Constructor();
    }
    d->malloc16 = (LPMALLOC16)lpReserved;
    TRACE("IMalloc->QueryInterface: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, QueryInterface));
    TRACE("IMalloc->AddRef: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, AddRef));
    TRACE("IMalloc->Release: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, Release));
    TRACE("IMalloc->Alloc: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, Alloc));
    TRACE("IMalloc->Realloc: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, Realloc));
    TRACE("IMalloc->Free: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, Free));
    TRACE("IMalloc->GetSize: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, GetSize));
    TRACE("IMalloc->DidAlloc: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, DidAlloc));
    TRACE("IMalloc->HeapMinimize: %08x\n", GET_SEGPTR_METHOD_ADDR(IMalloc16, lpReserved, HeapMinimize));
    IMalloc16_AddRef(d->malloc16);
    return S_OK;
}

/***********************************************************************
 *           CoUninitialize   [COMPOBJ.3]
 * Don't know what it does.
 * 3-Nov-98 -- this was originally misspelled, I changed it to what I
 *   believe is the correct spelling
 */
void WINAPI CoUninitialize16(void)
{
  ole16_task_data *d = get_current_task_data();
  TRACE("()\n");
  if (d->malloc16)
  {
    IMalloc16_Release(d->malloc16);
  }
  CoFreeAllLibraries();
}

/***********************************************************************
 *           CoFreeUnusedLibraries [COMPOBJ.17]
 */
void WINAPI CoFreeUnusedLibraries16(void)
{
    CoFreeUnusedLibraries();
}

/***********************************************************************
 *           IsEqualGUID [COMPOBJ.18]
 *
 * Compares two Unique Identifiers.
 *
 * RETURNS
 *	TRUE if equal
 */
BOOL16 WINAPI IsEqualGUID16(
	GUID* g1,	/* [in] unique id 1 */
	GUID* g2)	/* [in] unique id 2 */
{
    return !memcmp( g1, g2, sizeof(GUID) );
}

HRESULT WINAPI guid_str_to_clsid(LPCOLESTR16 idstr, CLSID *id);
HRESULT WINAPI progid_to_clsid(LPCOLESTR16 idstr, CLSID *id);
HRESULT WINAPI IIDFromString16(LPSTR lpsz, LPIID lpiid)
{
    HRESULT r;
    if (!lpiid)
        return E_INVALIDARG16;
    if (!lpsz)
    {
        memset(lpiid, 0, sizeof(CLSID));
        return S_OK;
    }
    r = guid_str_to_clsid((LPCOLESTR16)lpsz, (CLSID*)lpiid);
    if (r == CO_E_CLASSSTRING)
        r = CO_E_IIDSTRING;
    return r;
}

/******************************************************************************
 *		CLSIDFromString	[COMPOBJ.20]
 * Converts a unique identifier from its string representation into
 * the GUID struct.
 *
 * Class id: DWORD-WORD-WORD-BYTES[2]-BYTES[6]
 *
 * RETURNS
 *	the converted GUID
 */
HRESULT WINAPI CLSIDFromString16(
	LPCOLESTR16 idstr,	/* [in] string representation of (guid or ProgId) */
	CLSID *id)		/* [out] GUID converted from string */
{
    HRESULT r = guid_str_to_clsid(idstr, id);
    if (SUCCEEDED(r))
        return r;
    return progid_to_clsid(idstr, id);
}
HRESULT WINAPI guid_str_to_clsid(LPCOLESTR16 idstr, CLSID *id)
{
  const BYTE *s;
  int	i;
  BYTE table[256];

  /* validate the CLSID string */
  if (strlen(idstr) != 38)
      goto invalid_guid;

  s = (const BYTE *) idstr;
  if ((s[0]!='{') || (s[9]!='-') || (s[14]!='-') || (s[19]!='-') || (s[24]!='-') || (s[37]!='}'))
      goto invalid_guid;

  for (i=1; i<37; i++) {
    if ((i == 9)||(i == 14)||(i == 19)||(i == 24)) continue;
    if (!(((s[i] >= '0') && (s[i] <= '9'))  ||
          ((s[i] >= 'a') && (s[i] <= 'f'))  ||
          ((s[i] >= 'A') && (s[i] <= 'F'))))
        goto invalid_guid;
  }

  TRACE("%s -> %p\n", s, id);

  /* quick lookup table */
  memset(table, 0, 256);

  for (i = 0; i < 10; i++) {
    table['0' + i] = i;
  }
  for (i = 0; i < 6; i++) {
    table['A' + i] = i+10;
    table['a' + i] = i+10;
  }

  /* in form {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} */

  id->Data1 = (table[s[1]] << 28 | table[s[2]] << 24 | table[s[3]] << 20 | table[s[4]] << 16 |
               table[s[5]] << 12 | table[s[6]] << 8  | table[s[7]] << 4  | table[s[8]]);
  id->Data2 = table[s[10]] << 12 | table[s[11]] << 8 | table[s[12]] << 4 | table[s[13]];
  id->Data3 = table[s[15]] << 12 | table[s[16]] << 8 | table[s[17]] << 4 | table[s[18]];

  /* these are just sequential bytes */
  id->Data4[0] = table[s[20]] << 4 | table[s[21]];
  id->Data4[1] = table[s[22]] << 4 | table[s[23]];
  id->Data4[2] = table[s[25]] << 4 | table[s[26]];
  id->Data4[3] = table[s[27]] << 4 | table[s[28]];
  id->Data4[4] = table[s[29]] << 4 | table[s[30]];
  id->Data4[5] = table[s[31]] << 4 | table[s[32]];
  id->Data4[6] = table[s[33]] << 4 | table[s[34]];
  id->Data4[7] = table[s[35]] << 4 | table[s[36]];

  return S_OK;
  invalid_guid:
  return CO_E_CLASSSTRING;
}
HRESULT WINAPI progid_to_clsid(LPCOLESTR16 idstr, CLSID *id)
{
  HKEY hkey = NULL;
  CHAR clsid[50];
  DWORD cbclsid = sizeof(clsid);
  LPOLESTR w;
  HRESULT r;
  if (RegOpenKey16(HKEY_CLASSES_ROOT, idstr, &hkey))
  {
      goto error;
  }
  if (RegQueryValue16(hkey, "CLSID", clsid, &cbclsid))
  {
      RegCloseKey16(hkey);
      goto error;
  }
  RegCloseKey16(hkey);
  return guid_str_to_clsid(clsid, id);
error:
  w = strdupAtoW(idstr);
  r = CLSIDFromString(w, id);
  HeapFree(GetProcessHeap(), 0, w);
  return hresult32_16(r);
}

/******************************************************************************
 *		_xmalloc16	[internal]
 * Allocates size bytes from the standard ole16 allocator.
 *
 * RETURNS
 *	the allocated segmented pointer and a HRESULT
 */
static HRESULT
_xmalloc16(DWORD size, SEGPTR *ptr) {
  LPMALLOC16 mllc;
  DWORD args[2];

  if (get_task_imalloc16(&mllc))
    return E_OUTOFMEMORY;

  args[0] = (DWORD)mllc;
  args[1] = size;
  /* No need for a Callback entry, we have WOWCallback16Ex which does
   * everything we need.
   */
  if (!WOWCallback16Ex(
      (DWORD)((const IMalloc16Vtbl*)MapSL(
	  (SEGPTR)((LPMALLOC16)MapSL((SEGPTR)mllc))->lpVtbl  )
      )->Alloc,
      WCB16_CDECL,
      2*sizeof(DWORD),
      (LPVOID)args,
      (LPDWORD)ptr
  )) {
      ERR("CallTo16 IMalloc16 (%d) failed\n",size);
      return E_FAIL;
  }
  return S_OK;
}

/******************************************************************************
 *		StringFromCLSID	[COMPOBJ.19]
 *              StringFromIID   [COMPOBJ.14]
 * Converts a GUID into the respective string representation.
 * The target string is allocated using the OLE IMalloc.
 *
 * RETURNS
 *	the string representation and HRESULT
 */

HRESULT WINAPI StringFromCLSID16(
  REFCLSID id,		/* [in] the GUID to be converted */
  LPOLESTR16 *idstr )	/* [out] a pointer to a to-be-allocated segmented pointer pointing to the resulting string */
{
    WCHAR buffer[40];
    HRESULT ret;

    ret = _xmalloc16(40,(SEGPTR*)idstr);
    if (ret != S_OK)
        return ret;
    StringFromGUID2( id, buffer, 40 );
    WideCharToMultiByte( CP_ACP, 0, buffer, -1, MapSL((SEGPTR)*idstr), 40, NULL, NULL );
    return ret;
}

/******************************************************************************
 * ProgIDFromCLSID [COMPOBJ.62]
 *
 * Converts a class id into the respective Program ID. (By using a registry lookup)
 *
 * RETURNS
 *  S_OK on success
 *  riid associated with the progid
 */
HRESULT WINAPI ProgIDFromCLSID16(
  REFCLSID clsid, /* [in] class id as found in registry */
  LPOLESTR16 *lplpszProgID )/* [out] associated Program ID */
{
    LPOLESTR progid;
    HRESULT ret;

    if (!lplpszProgID)
    {
        return E_INVALIDARG16;
    }
    ret = ProgIDFromCLSID( clsid, &progid );
    if (ret == S_OK)
    {
        INT len = WideCharToMultiByte( CP_ACP, 0, progid, -1, NULL, 0, NULL, NULL );
        ret = _xmalloc16(len, (SEGPTR*)lplpszProgID);
        if (ret == S_OK)
            WideCharToMultiByte( CP_ACP, 0, progid, -1, MapSL((SEGPTR)*lplpszProgID), len, NULL, NULL );
        CoTaskMemFree( progid );
    }
    else
    {
        *lplpszProgID = 0;
    }
    return ret;
}

/***********************************************************************
 *           LookupETask (COMPOBJ.94)
 */
HRESULT WINAPI LookupETask16(HTASK16 *hTask,LPVOID p) {
	FIXME("(%p,%p),stub!\n",hTask,p);
	if ((*hTask = GetCurrentTask()) == hETask) {
		memcpy(p, Table_ETask, sizeof(Table_ETask));
	}
	return 0;
}

/***********************************************************************
 *           SetETask (COMPOBJ.95)
 */
HRESULT WINAPI SetETask16(HTASK16 hTask, LPVOID p) {
        FIXME("(%04x,%p),stub!\n",hTask,p);
	hETask = hTask;
	return 0;
}

/***********************************************************************
 *           CALLOBJECTINWOW (COMPOBJ.201)
 */
HRESULT WINAPI CallObjectInWOW(LPVOID p1,LPVOID p2) {
	FIXME("(%p,%p),stub!\n",p1,p2);
	return 0;
}

/******************************************************************************
 *		CoRegisterClassObject	[COMPOBJ.5]
 *
 * Don't know where it registers it ...
 */
HRESULT WINAPI CoRegisterClassObject16(
	REFCLSID rclsid,
	/*LPUNKNOWN16*/SEGPTR pUnk,
	DWORD dwClsContext, /* [in] CLSCTX flags indicating the context in which to run the executable */
	DWORD flags,        /* [in] REGCLS flags indicating how connections are made */
	LPDWORD lpdwRegister
) {
    HRESULT result;
    TRACE("(%s,%p,0x%08x,0x%08x,%p)\n",
		debugstr_guid(rclsid),pUnk,dwClsContext,flags,lpdwRegister
	);
    result = hresult32_16(CoRegisterClassObject(rclsid, (IUnknown*)iface16_32(&IID_IUnknown, pUnk), dwClsContext, flags, lpdwRegister));
    TRACE("%08x\n", result);
    return result;
}

/******************************************************************************
 *      CoRevokeClassObject [COMPOBJ.6]
 *
 */
HRESULT WINAPI CoRevokeClassObject16(DWORD dwRegister) /* [in] token on class obj */
{
    TRACE("(0x%08x)\n", dwRegister);
    return hresult32_16(CoRevokeClassObject(dwRegister));
}

/******************************************************************************
 *              IsValidInterface        [COMPOBJ.23]
 *
 * Determines whether a pointer is a valid interface.
 *
 * PARAMS
 *  punk [I] Interface to be tested.
 *
 * RETURNS
 *  TRUE, if the passed pointer is a valid interface, or FALSE otherwise.
 */
BOOL WINAPI IsValidInterface16(SEGPTR punk)
{
	DWORD **ptr;

	if (IsBadReadPtr16(punk,4))
		return FALSE;
	ptr = MapSL(punk);
	if (IsBadReadPtr16((SEGPTR)ptr[0],4))	/* check vtable ptr */
		return FALSE;
	ptr = MapSL((SEGPTR)ptr[0]);		/* ptr to first method */
	if (IsBadReadPtr16((SEGPTR)ptr[0],2))
		return FALSE;
	return TRUE;
}

/******************************************************************************
 *      CoFileTimeToDosDateTime [COMPOBJ.30]
 */
BOOL16 WINAPI CoFileTimeToDosDateTime16(const FILETIME *ft, LPWORD lpDosDate, LPWORD lpDosTime)
{
    return FileTimeToDosDateTime(ft, lpDosDate, lpDosTime);
}

/******************************************************************************
 *      CoDosDateTimeToFileTime [COMPOBJ.31]
 */
BOOL16 WINAPI CoDosDateTimeToFileTime16(WORD wDosDate, WORD wDosTime, FILETIME *ft)
{
    return DosDateTimeToFileTime(wDosDate, wDosTime, ft);
}

/******************************************************************************
 *		CoGetCurrentProcess	[COMPOBJ.34]
 */
DWORD WINAPI CoGetCurrentProcess16(void)
{
    return CoGetCurrentProcess();
}

/******************************************************************************
 *		CoRegisterMessageFilter	[COMPOBJ.27]
 */
HRESULT WINAPI CoRegisterMessageFilter16(
	SEGPTR lpMessageFilter,
	SEGPTR *lplpMessageFilter
) {
    HRESULT result;
    LPMESSAGEFILTER lpmsgf = NULL;
	TRACE("(%p,%p)\n",lpMessageFilter,lplpMessageFilter);
    result = hresult32_16(CoRegisterMessageFilter((IMessageFilter*)iface16_32(&IID_IMessageFilter, lpMessageFilter), lplpMessageFilter ? &lpmsgf : NULL));
    if (lplpMessageFilter)
        *lplpMessageFilter = iface32_16(&IID_IMessageFilter, lpmsgf);
	return result;
}

/******************************************************************************
 *		CoLockObjectExternal	[COMPOBJ.63]
 */
HRESULT WINAPI CoLockObjectExternal16(
    /*LPUNKNOWN*/SEGPTR pUnk,		/* [in] object to be locked */
    BOOL16 fLock,		/* [in] do lock */
    BOOL16 fLastUnlockReleases	/* [in] ? */
) {
    TRACE("(%p,%d,%d)\n",pUnk,fLock,fLastUnlockReleases);
    return CoLockObjectExternal((IUnknown*)iface16_32(&IID_IUnknown, pUnk), fLock, fLastUnlockReleases);
}

/***********************************************************************
 *           CoGetState [COMPOBJ.115]
 */
HRESULT WINAPI CoGetState16(LPDWORD state)
{
    FIXME("(%p),stub!\n", state);

    *state = 0;
    return S_OK;
}

/***********************************************************************
 *      DllEntryPoint                   [COMPOBJ.116]
 *
 *    Initialization code for the COMPOBJ DLL
 *
 * RETURNS:
 */
BOOL WINAPI COMPOBJ_DllEntryPoint(DWORD Reason, HINSTANCE16 hInst, WORD ds, WORD HeapSize, DWORD res1, WORD res2)
{
        TRACE("(%08x, %04x, %04x, %04x, %08x, %04x)\n", Reason, hInst, ds, HeapSize, res1, res2);
        return TRUE;
}

/***********************************************************************
 *           CoMemAlloc [COMPOBJ.151]
 */
SEGPTR WINAPI CoMemAlloc(DWORD size, MEMCTX dwMemContext, DWORD x) {
	HRESULT		hres;
	SEGPTR		segptr;

	/* FIXME: check context handling */
	TRACE("(%d, 0x%08x, 0x%08x)\n", size, dwMemContext, x);
	hres = _xmalloc16(size, &segptr);
	if (hres != S_OK)
		return 0;
	return segptr;
}

/******************************************************************************
 *		CLSIDFromProgID [COMPOBJ.61]
 *
 * Converts a program ID into the respective GUID.
 *
 * PARAMS
 *  progid       [I] program id as found in registry
 *  riid         [O] associated CLSID
 *
 * RETURNS
 *	Success: S_OK
 *  Failure: CO_E_CLASSSTRING - the given ProgID cannot be found.
 */
HRESULT WINAPI CLSIDFromProgID16(LPCOLESTR16 progid, LPCLSID riid)
{
	char	*buf,buf2[80];
	LONG	buf2len;
	HKEY	xhkey;

	buf = HeapAlloc(GetProcessHeap(),0,strlen(progid)+8);
	sprintf(buf,"%s\\CLSID",progid);
	if (RegOpenKeyA(HKEY_CLASSES_ROOT,buf,&xhkey)) {
		HeapFree(GetProcessHeap(),0,buf);
                return CO_E_CLASSSTRING;
	}
	HeapFree(GetProcessHeap(),0,buf);
	buf2len = sizeof(buf2);
	if (RegQueryValueA(xhkey,NULL,buf2,&buf2len)) {
		RegCloseKey(xhkey);
                return CO_E_CLASSSTRING;
	}
	RegCloseKey(xhkey);
	return CLSIDFromString16(buf2,riid);
}

/******************************************************************************
 *		StringFromGUID2	[COMPOBJ.76]
 */
INT16 WINAPI StringFromGUID216(REFGUID id, LPOLESTR16 str, INT16 cmax)
{
    static const char format[] = "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}";
    if (!id || cmax < CHARS_IN_GUID) return 0;
    sprintf( str, format, id->Data1, id->Data2, id->Data3,
             id->Data4[0], id->Data4[1], id->Data4[2], id->Data4[3],
             id->Data4[4], id->Data4[5], id->Data4[6], id->Data4[7] );
    return CHARS_IN_GUID;
}


/***********************************************************************
 *           CoFileTimeNow [COMPOBJ.82]
 */
HRESULT WINAPI CoFileTimeNow16( FILETIME *lpFileTime )
{
    return CoFileTimeNow( lpFileTime );
}

/***********************************************************************
 *           CoGetClassObject [COMPOBJ.7]
 *
 */
HRESULT WINAPI CoGetClassObject16(
    SEGPTR rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo,
    SEGPTR riid, SEGPTR ppv)
{
    LPVOID *ppvl = MapSL(ppv);
    char idstr[CHARS_IN_GUID];
    char buf_key[CHARS_IN_GUID+190], dllpath[MAX_PATH+1];
    LONG dllpath_len = sizeof(dllpath);
    HRESULT err_last = E_NOTIMPL16;

    TRACE("CLSID: %s, IID: %s\n", debugstr_guid(MapSL(rclsid)), debugstr_guid(MapSL(riid)));

    *ppvl = NULL;
    StringFromGUID216(MapSL(rclsid), idstr, CHARS_IN_GUID);

    if (pServerInfo) {
        FIXME("pServerInfo->name=%s pAuthInfo=%p\n",
              debugstr_w(pServerInfo->pwszName), pServerInfo->pAuthInfo);
    }

    if (CLSCTX_INPROC_SERVER & dwClsContext)
    {

        HMODULE16 dll;
        FARPROC16 DllGetClassObject;

        WORD args[6];
        DWORD dwRet;

        sprintf(buf_key, "CLSID\\%s\\InprocServer", idstr);
        if (RegQueryValue16(HKEY_CLASSES_ROOT, buf_key, dllpath, &dllpath_len))
        {
            /* 64-bit COM...? */
            sprintf(buf_key, "CLSID\\%s\\InprocServer32", idstr);
            dllpath_len = sizeof(dllpath);
            if (!RegQueryValue16(HKEY_CLASSES_ROOT, buf_key, dllpath, &dllpath_len))
            {
                PVOID pv = NULL;
                HRESULT result;
                result = CoGetClassObject((REFCLSID)MapSL(rclsid), CLSCTX_INPROC_SERVER, pServerInfo, (REFIID)MapSL(riid), &pv);
                *(SEGPTR*)MapSL(ppv) = iface32_16((REFIID)MapSL(riid), pv);
                return hresult32_16(result);
            }
            ERR("class %s not registered\n", debugstr_guid(MapSL(rclsid)));
            err_last = REGDB_E_CLASSNOTREG;
            goto fail_inproc;
        }

        dll = LoadLibrary16(dllpath);
        if (!dll)
        {
            ERR("couldn't load in-process dll %s\n", debugstr_a(dllpath));
            err_last = E_ACCESSDENIED16; /* FIXME: or should this be CO_E_DLLNOTFOUND? */
            goto fail_inproc;
        }

        DllGetClassObject = GetProcAddress16(dll, "DllGetClassObject");
        if (!DllGetClassObject)
        {
            ERR("couldn't find function DllGetClassObject in %s\n", debugstr_a(dllpath));
            FreeLibrary16(dll);
            err_last = CO_E_DLLNOTFOUND;
            goto fail_inproc;
        }

        TRACE("calling DllGetClassObject %p\n", DllGetClassObject);
        args[5] = SELECTOROF(rclsid);
        args[4] = OFFSETOF(rclsid);
        args[3] = SELECTOROF(riid);
        args[2] = OFFSETOF(riid);
        args[1] = SELECTOROF(ppv);
        args[0] = OFFSETOF(ppv);
        WOWCallback16Ex((DWORD) DllGetClassObject, WCB16_PASCAL, sizeof(args), args, &dwRet);
        if (dwRet != S_OK)
        {
            ERR("DllGetClassObject returned error 0x%08x\n", dwRet);
            FreeLibrary16(dll);
            return dwRet;
        }

        return S_OK;
    }

    fail_inproc:
    if (CLSCTX_LOCAL_SERVER & dwClsContext)
    {
        HRESULT result;
        PVOID pv = NULL;
        result = CoGetClassObject((REFCLSID)MapSL(rclsid), dwClsContext, pServerInfo, (REFIID)MapSL(riid), &pv);
        *(SEGPTR*)MapSL(ppv) = iface32_16((REFCLSID)MapSL(riid), pv);
        return hresult32_16(result);
    }
    fail_svr:
    FIXME("semi-stub\n");
    return err_last;
}

/******************************************************************************
 *		CoCreateGuid [COMPOBJ.73]
 */
HRESULT WINAPI CoCreateGuid16(GUID *pguid)
{
    return CoCreateGuid( pguid );
}

/***********************************************************************
 *           CoCreateInstance [COMPOBJ.13]
 */
HRESULT WINAPI CoCreateInstance16(
	/* REFCLSID */SEGPTR srclsid,
	SEGPTR pUnkOuter,
	DWORD dwClsContext,
	/* REFIID */SEGPTR siid,
	/* SEGPTR* */SEGPTR sppv)
{
  LPVOID pv;
  HRESULT result;
  REFCLSID rclsid = (REFCLSID)MapSL(srclsid);
  REFIID iid = (REFIID)MapSL(siid);
  SEGPTR *ppv = (SEGPTR*)MapSL(sppv);
  TRACE("(%s, %p, %x, %s, %p)\n",
	debugstr_guid(rclsid), pUnkOuter, dwClsContext, debugstr_guid(iid),
	ppv
  );
  result = CoCreateInstance(rclsid, (IUnknown*)iface16_32(&IID_IUnknown, pUnkOuter), dwClsContext, iid, &pv);
  if (result == REGDB_E_CLASSNOTREG)
  {
      SEGPTR cf;
      result = CoGetClassObject16(srclsid, dwClsContext, NULL, MapLS(&IID_IClassFactory), MapLS(&cf));
      if (SUCCEEDED(result))
      {
          result = IClassFactory16_CreateInstance(cf, pUnkOuter, siid, sppv);
          IClassFactory16_Release(cf);
          return result;
      }
      return hresult32_16(result);
  }
  *ppv = iface32_16(iid, pv);
  return hresult32_16(result);
}

/***********************************************************************
 *           CoDisconnectObject [COMPOBJ.15]
 */
HRESULT WINAPI CoDisconnectObject16( /*LPUNKNOWN*/SEGPTR lpUnk, DWORD reserved )
{
  TRACE("(%p, 0x%08x)\n", lpUnk, reserved);
  return hresult32_16(CoDisconnectObject((IUnknown*)iface16_32(&IID_IUnknown, lpUnk), reserved));
}

HRESULT WINAPI CoIsOle1Class16(REFCLSID rclsid)
{
    TRACE("(%p)\n", rclsid);
    return hresult32_16(CoIsOle1Class(rclsid));
}

HRESULT WINAPI FnAssert16(LPCSTR expr, LPCSTR msg, LPCSTR file, WORD line)
{
    TRACE("(%s,%s,%s,%d)\n", debugstr_a(expr), debugstr_a(msg), debugstr_a(msg), line);
    return S_OK;
}

HRESULT WINAPI CoGetTreatAsClass16(REFCLSID clsidOld, LPCLSID pClsidNew)
{
    HRESULT r;
    TRACE("(%s,%p)\n", debugstr_guid(clsidOld), pClsidNew);
    r = CoGetTreatAsClass(clsidOld, pClsidNew);
    return hresult32_16(r);
}

HRESULT WINAPI CoTreatAsClass16(REFCLSID clsidOld, REFCLSID clsidNew)
{
    HRESULT r;
    TRACE("(%s,%p)\n", debugstr_guid(clsidOld), debugstr_guid(clsidNew));
    r = CoTreatAsClass(clsidOld, clsidNew);
    if (r == REGDB_E_WRITEREGDB)
    {
        ERR("REGDB_E_WRITEREGDB\n");
    }
    return hresult32_16(r);
}
