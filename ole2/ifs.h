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
#include "ifs_16.h"

SEGPTR iface32_16(REFIID riid, void *iface32);
void *iface16_32(REFIID riid, SEGPTR iface16);
#undef STGMEDIUM16

/**********************************************************************/

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

#endif /* __WINE_OLE_IFS_H */
