/*
 * Copyright 1997 Marcus Meissner
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

#ifndef __WINE_OLE_IFS_H
#define __WINE_OLE_IFS_H

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "objbase.h"

typedef CHAR OLECHAR16;
typedef LPSTR LPOLESTR16;
typedef LPCSTR LPCOLESTR16;

#define STDMETHOD16CALLTYPE __cdecl
#define STDMETHOD16(m) HRESULT (STDMETHOD16CALLTYPE *m)
#define STDMETHOD16_(t,m) t (STDMETHOD16CALLTYPE *m)

#define GET_SEGPTR_METHOD_ADDR(ifacename,segptr,methodname) \
    ((SEGPTR)((const ifacename##Vtbl*)MapSL((SEGPTR)((ifacename*)MapSL(segptr))->lpVtbl))->methodname)
#include "wownt32.h"
#define STGMEDIUM16 /* FIXME*/
#define tagBINDPTR /* FIXME */
#include "ifs_16.h"

SEGPTR iface32_16(REFIID riid, void *iface32);
void *iface16_32(REFIID riid, SEGPTR iface16);
/* COM error codes */
#define E_UNEXPECTED16      MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 0xFFFF )
#define E_NOTIMPL16         MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 1 )
#define E_OUTOFMEMORY16     MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 2 )
#define E_INVALIDARG16      MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 3 )
#define E_NOINTERFACE16     MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 4 )
#define E_POINTER16         MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 5 )
#define E_HANDLE16          MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 6 )
#define E_ABORT16           MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 7 )
#define E_FAIL16            MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 8 )
#define E_ACCESSDENIED16    MAKE_SCODE( SEVERITY_ERROR, FACILITY_NULL, 9 )
HRESULT hresult32_16(HRESULT hresult);
HRESULT hresult16_32(HRESULT hresult);
#undef STGMEDIUM16
#undef tagBINDPTR

static LPWSTR strdupAtoW(LPCSTR str)
{
    LPWSTR ret;
    INT len;

    if (!str) return NULL;
    len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (ret) MultiByteToWideChar(CP_ACP, 0, str, -1, ret, len);
    return ret;
}
static LPCSTR strdupWtoA(LPCWSTR str)
{
    LPSTR ret;
    INT len;

    if (!str) return NULL;
    len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(CHAR));
    if (ret) WideCharToMultiByte(CP_ACP, 0, str, -1, ret, len, NULL, NULL);
    return ret;
}
/**********************************************************************/

#include <pshpack1.h>
typedef struct tagSTATSTG16
{
    LPOLESTR16 pwcsName;
    DWORD type;
    ULARGE_INTEGER cbSize;
    FILETIME mtime;
    FILETIME ctime;
    FILETIME atime;
    DWORD grfMode;
    DWORD grfLocksSupported;
    CLSID clsid;
    DWORD grfStateBits;
    DWORD reserved;
} STATSTG16;

typedef struct
{
    CLIPFORMAT cfFormat;
    SEGPTR/*DVTARGETDEVICE FAR**/ ptd;
    DWORD dwAspect;
    LONG lindex;
    DWORD tymed;
} FORMATETC16;
typedef struct {
    DWORD tymed;
    union {
        HANDLE16 hGlobal;
        SEGPTR/*LPSTR*/lpszFileName;
        SEGPTR/*LPSTREAM*/pstm;
        SEGPTR/*LPSTORAGE*/pstg;
    };
    SEGPTR /*LPUNKNOWN*/pUnkForRelease;
} STGMEDIUM16;
typedef struct
{
    FORMATETC16 formatetc;
    DWORD advf;
    /*LPADVISESINK*/SEGPTR pAdvSink;
    DWORD dwConnection;
} STATDATA16;
#include <poppack.h>

void map_stgmedium32_16(STGMEDIUM16 *a16, const STGMEDIUM *a32);
void map_stgmedium16_32(STGMEDIUM *a32, const STGMEDIUM16 *a16);

void map_formatetc16_32(FORMATETC *a32, const FORMATETC16 *a16);
void map_formatetc32_16(FORMATETC16 *a16, const FORMATETC *a32);

#endif /* __WINE_OLE_IFS_H */
