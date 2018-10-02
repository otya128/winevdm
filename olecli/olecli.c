/*
 *	OLECLI library
 *
 *	Copyright 1995	Martin von Loewis
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

/*	At the moment, these are only empty stubs.
 */

#include "config.h"
//?????
#define __oleidl_h__
#define _OLE2_H_
#include <stdarg.h>

#include <windows.h>
#include <ole.h>
 //#include "windef.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "wine/wingdi16.h"
#include "winbase.h"
#include "wingdi.h"
#include "wownt32.h"
#include "objbase.h"
//#include "olecli.h"
#include "wine/debug.h"

typedef LONG LHCLIENTDOC;
typedef struct _OLEOBJECT *_LPOLEOBJECT;
typedef struct _OLECLIENT *LPOLECLIENT;
typedef OLEOPT_UPDATE *LPOLEOPT_UPDATE;
typedef LPCSTR LPCOLESTR16;

struct _OLESTREAM;
WINE_DEFAULT_DEBUG_CHANNEL(ole);

typedef struct _OLEOBJECTVTBL16 {
    void  *(CALLBACK*QueryProtocol)(_LPOLEOBJECT,LPCOLESTR16);
    OLESTATUS      (CALLBACK *Release)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *Show)(_LPOLEOBJECT,BOOL16);
    OLESTATUS      (CALLBACK *DoVerb)(_LPOLEOBJECT,UINT16,BOOL16,BOOL16);
    OLESTATUS      (CALLBACK *GetData)(_LPOLEOBJECT,OLECLIPFORMAT,HANDLE16 *);
    OLESTATUS      (CALLBACK *SetData)(_LPOLEOBJECT,OLECLIPFORMAT,HANDLE16);
    OLESTATUS      (CALLBACK *SetTargetDevice)(_LPOLEOBJECT,HGLOBAL16);
    OLESTATUS      (CALLBACK *SetBounds)(_LPOLEOBJECT,LPRECT16);
    OLESTATUS      (CALLBACK *EnumFormats)(_LPOLEOBJECT,OLECLIPFORMAT);
    OLESTATUS      (CALLBACK *SetColorScheme)(_LPOLEOBJECT,struct tagLOGPALETTE*);
    OLESTATUS      (CALLBACK *Delete)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *SetHostNames)(_LPOLEOBJECT,LPCOLESTR16,LPCOLESTR16);
    OLESTATUS      (CALLBACK *SaveToStream)(_LPOLEOBJECT,struct _OLESTREAM*);
    OLESTATUS      (CALLBACK *Clone)(_LPOLEOBJECT,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT *);
    OLESTATUS      (CALLBACK *CopyFromLink)(_LPOLEOBJECT,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT *);
    OLESTATUS      (CALLBACK *Equal)(_LPOLEOBJECT,_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *CopyToClipBoard)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *Draw)(_LPOLEOBJECT,HDC16,LPRECT16,LPRECT16,HDC16);
    OLESTATUS      (CALLBACK *Activate)(_LPOLEOBJECT,UINT16,BOOL16,BOOL16,HWND16,LPRECT16);
    OLESTATUS      (CALLBACK *Execute)(_LPOLEOBJECT,HGLOBAL16,UINT16);
    OLESTATUS      (CALLBACK *Close)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *Update)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *Reconnect)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *ObjectConvert)(_LPOLEOBJECT,LPCOLESTR16,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT*);
    OLESTATUS      (CALLBACK *GetLinkUpdateOptions)(_LPOLEOBJECT,LPOLEOPT_UPDATE);
    OLESTATUS      (CALLBACK *SetLinkUpdateOptions)(_LPOLEOBJECT,OLEOPT_UPDATE);
    OLESTATUS      (CALLBACK *Rename)(_LPOLEOBJECT,LPCOLESTR16);
    OLESTATUS      (CALLBACK *QueryName)(_LPOLEOBJECT,LPSTR,LPUINT16);
    OLESTATUS      (CALLBACK *QueryType)(_LPOLEOBJECT,LPLONG);
    OLESTATUS      (CALLBACK *QueryBounds)(_LPOLEOBJECT,LPRECT16);
    OLESTATUS      (CALLBACK *QuerySize)(_LPOLEOBJECT,LPDWORD);
    OLESTATUS      (CALLBACK *QueryOpen)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *QueryOutOfDate)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *QueryReleaseStatus)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *QueryReleaseError)(_LPOLEOBJECT);
    OLE_RELEASE_METHOD (CALLBACK *QueryReleaseMethod)(_LPOLEOBJECT);
    OLESTATUS      (CALLBACK *RequestData)(_LPOLEOBJECT,OLECLIPFORMAT);
    OLESTATUS      (CALLBACK *ObjectLong)(_LPOLEOBJECT,UINT16,LPLONG);
} OLEOBJECTVTBL16;
typedef OLEOBJECTVTBL16 *LPOLEOBJECTVTBL16;

typedef struct _OLEOBJECT16
{
    const OLEOBJECTVTBL16 *lpvtbl;
} OLEOBJECT16;

static LONG OLE_current_handle;

/* ole client definitions */
typedef struct _OLECLIENTVTBL16
{
    SEGPTR CallBack;
} OLECLIENTVTBL16;

typedef  OLECLIENTVTBL16 FAR*  LPOLECLIENTVTBL16;

typedef struct _OLECLIENT16
{
    SEGPTR   lpvtbl;
} OLECLIENT16;
DWORD WINAPI OleQueryClientVersion16()
{
    return OleQueryClientVersion();
}
#define MAX_OLECLIENT 256
struct ole_client_info
{
    BOOL used;
    OLECLIENT client;
    OLECLIENTVTBL vtable;
    /*OLECLIENT16 */SEGPTR client16;
    /*OLECLIENTVTBL16 */SEGPTR vtable16;
} ole_clients[MAX_OLECLIENT];
struct ole_client_info *find_ole_client(LPOLECLIENT client)
{
    for (int i = 0; i < MAX_OLECLIENT; i++)
    {
        if (ole_clients[i].used && &ole_clients[i].client == client)
        {
            return ole_clients + i;
        }
    }
    return NULL;
}

struct ole_client_info *find_ole_client_16(SEGPTR client)
{
    for (int i = 0; i < MAX_OLECLIENT; i++)
    {
        if (ole_clients[i].used && ole_clients[i].client16 == client)
        {
            return ole_clients + i;
        }
    }
    return NULL;
}

static SEGPTR OLEOBJ16(LPOLEOBJECT oleobj)
{
    return (SEGPTR)oleobj;
}

static LPOLEOBJECT OLEOBJ32(SEGPTR oleobj)
{
    return (LPOLEOBJECT)oleobj;
}
#include "wine/list.h"

typedef struct _OLESTREAM16 *LPOLESTREAM16;
typedef struct _OLESTREAMVTBL16
{
    DWORD(CALLBACK* Get)(LPOLESTREAM16, void*, DWORD);
    DWORD(CALLBACK* Put)(LPOLESTREAM16, OLE_CONST void*, DWORD);
} OLESTREAMVTBL16;
typedef  OLESTREAMVTBL16 FAR*  LPOLESTREAMVTBL16;

typedef struct _OLESTREAM16
{
    SEGPTR    lpstbl;
} OLESTREAM16;
struct olestream_entry
{
    struct list entry;
    SEGPTR olestream16;
    OLESTREAM olestream32;
};
/***********************************************************************
 *           SELECTOR_SetEntries
 *
 * Set the LDT entries for an array of selectors.
 */
static BOOL SELECTOR_SetEntries(WORD sel, const void *base, DWORD size, unsigned char flags)
{
    LDT_ENTRY entry;
    WORD i, count;

    wine_ldt_set_base(&entry, base);
    wine_ldt_set_limit(&entry, size - 1);
    wine_ldt_set_flags(&entry, flags);
    count = (size + 0xffff) / 0x10000;
    for (i = 0; i < count; i++)
    {
        if (wine_ldt_set_entry(sel + (i << __AHSHIFT), &entry) < 0) return FALSE;
        wine_ldt_set_base(&entry, (char*)wine_ldt_get_base(&entry) + 0x10000);
        /* yep, Windows sets limit like that, not 64K sel units */
        wine_ldt_set_limit(&entry, wine_ldt_get_limit(&entry) - 0x10000);
    }
    return TRUE;
}
static SEGPTR OLESTREAM_16(LPOLESTREAM stream32);
LPOLESTREAMVTBL16 get_olestream16_vtbl(SEGPTR this16)
{
    LPOLESTREAM16 o16 = (LPOLESTREAM16)MapSL(this16);
    LPOLESTREAMVTBL16 vtbnl16 = (LPOLESTREAMVTBL16)MapSL(o16->lpstbl);
    return vtbnl16;
}
DWORD CALLBACK OLESTREAM32_Get(LPOLESTREAM this_, void *a, DWORD b)
{
    WORD args[2 + 2 + 2];
    SEGPTR this16 = OLESTREAM_16(this_);
    WORD seg = AllocSelectorArray16(b / 0x10000 + 1);
    DWORD ret;
    if (!seg)
        return 0;
    SELECTOR_SetEntries(seg, a, b, WINE_LDT_FLAGS_DATA);
    args[5] = HIWORD(this16);
    args[4] = LOWORD(this16);
    args[3] = seg;
    args[2] = 0;
    args[1] = HIWORD(b);
    args[0] = LOWORD(b);
    TRACE("seg=%04x size=%08x\n", seg, b);
    WOWCallback16Ex(get_olestream16_vtbl(this16)->Get, WCB16_PASCAL, sizeof(args), args, &ret);
    for (WORD i = 0; i <= b / 0x10000; i++)
        FreeSelector16(seg + i);
    TRACE("result=%08x\n", ret);
    return ret;
}
DWORD CALLBACK OLESTREAM32_Put(LPOLESTREAM this_, OLE_CONST void *a, DWORD b)
{
    WORD args[2 + 2 + 2];
    SEGPTR this16 = OLESTREAM_16(this_);
    WORD seg = AllocSelectorArray16(b / 0x10000 + 1);
    if (!seg)
        return 0;
    SELECTOR_SetEntries(seg, a, b, WINE_LDT_FLAGS_DATA);
    DWORD ret;
    args[5] = HIWORD(this16);
    args[4] = LOWORD(this16);
    args[3] = seg;
    args[2] = 0;
    args[1] = HIWORD(b);
    args[0] = LOWORD(b);
    TRACE("seg=%04x size=%08x\n", seg, b);
    WOWCallback16Ex(get_olestream16_vtbl(this16)->Put, WCB16_PASCAL, sizeof(args), args, &ret);
    for (WORD i = 0; i <= b / 0x10000; i++)
        FreeSelector16(seg + i);
    TRACE("result=%08x\n", ret);
    return ret;
}
static struct list olestream_list = LIST_INIT(olestream_list);
OLESTREAMVTBL olestream32_vtbl = { OLESTREAM32_Get, OLESTREAM32_Put };
static struct olestream_entry *add_olestream_thunk(SEGPTR s16)
{
    struct olestream_entry *data = HeapAlloc(GetProcessHeap(), 0, sizeof(struct olestream_entry));
    data->olestream16 = s16;
    data->olestream32.lpstbl = &olestream32_vtbl;
    list_add_tail(&olestream_list, &data->entry);
    return data;
}
static SEGPTR OLESTREAM_16(LPOLESTREAM stream32)
{
    struct olestream_entry *cur, *next;
    LIST_FOR_EACH_ENTRY_SAFE(cur, next, &olestream_list, struct olestream_entry, entry)
    {
        if (&cur->olestream32 == stream32)
            return cur->olestream16;
    }
    FIXME("\n");
    return 0;
}
static LPOLESTREAM OLESTREAM_32(SEGPTR stream16)
{
    struct olestream_entry *cur, *next;
    LIST_FOR_EACH_ENTRY_SAFE(cur, next, &olestream_list, struct olestream_entry, entry)
    {
        if (cur->olestream16 == stream16)
            return &cur->olestream32;
    }
    return &add_olestream_thunk(stream16)->olestream32;
}

/******************************************************************************
 *		OleSavedClientDoc	[OLECLI.45]
 */
OLESTATUS WINAPI OleSavedClientDoc16(LHCLIENTDOC hDoc)
{
    return OleSavedClientDoc(hDoc);
}

/******************************************************************************
 *		OleRegisterClientDoc	[OLECLI.41]
 */
OLESTATUS WINAPI OleRegisterClientDoc16(LPCSTR classname, LPCSTR docname,
                                        LONG reserved, LHCLIENTDOC *hRet )
{
    return OleRegisterClientDoc(classname, docname, reserved, hRet);
}

/******************************************************************************
 *		OleRenameClientDoc	[OLECLI.43]
 */
OLESTATUS WINAPI OleRenameClientDoc16(LHCLIENTDOC hDoc, LPCSTR newName)
{
    return OleRenameClientDoc(hDoc, newName);
}

/******************************************************************************
 *		OleRevokeClientDoc	[OLECLI.42]
 */
OLESTATUS WINAPI OleRevokeClientDoc16(LHCLIENTDOC hServerDoc)
{
    return OleRevokeClientDoc(hServerDoc);
}

/******************************************************************************
 *		OleRevertClientDoc	[OLECLI.44]
 */
OLESTATUS WINAPI OleRevertClientDoc16(LHCLIENTDOC hServerDoc)
{
    return OleRevertClientDoc(hServerDoc);
}

/******************************************************************************
 *		OleEnumObjects	[OLECLI.47]
 */
OLESTATUS WINAPI OleEnumObjects16(LHCLIENTDOC hServerDoc, SEGPTR *data)
{
    /*FIXME("(%d, %04x:%04x): stub\n", hServerDoc, HIWORD(data),
	LOWORD(data));*/
    LPOLEOBJECT obj32 = 0;
    OLESTATUS result = OleEnumObjects(hServerDoc, &obj32);
    *data = OLEOBJ16(obj32);
    return result;
}

/******************************************************************************
 *	     OleCreateLinkFromClip	[OLECLI.11]
 */
OLESTATUS WINAPI OleCreateLinkFromClip16( LPCSTR name, SEGPTR olecli, LHCLIENTDOC hclientdoc,
                                          LPCSTR xname, SEGPTR *lpoleob, OLEOPT_RENDER render,
                                          OLECLIPFORMAT clipformat )
{
    LPOLECLIENT olecli32 = find_ole_client_16(olecli);
    LPOLEOBJECT obj32 = 0;
    OLESTATUS status = OleCreateLinkFromClip(name, olecli32, hclientdoc, xname, &obj32, render, clipformat);
    *lpoleob = OLEOBJ16(obj32);
	return status;
}

/******************************************************************************
 *           OleQueryLinkFromClip	[OLECLI.9]
 */
OLESTATUS WINAPI OleQueryLinkFromClip16(LPCSTR name, UINT16 render, UINT16 clipformat)
{
	return OleQueryLinkFromClip(name, render, clipformat);
}

/******************************************************************************
 *           OleQueryCreateFromClip	[OLECLI.10]
 */
OLESTATUS WINAPI OleQueryCreateFromClip16(LPCSTR name, UINT16 render, UINT16 clipformat)
{
    return OleQueryCreateFromClip(name, render, clipformat);
}

/******************************************************************************
 *		OleIsDcMeta	[OLECLI.60]
 */
BOOL16 WINAPI OleIsDcMeta16(HDC16 hdc)
{
    return GetObjectType( HDC_32(hdc) ) == OBJ_METADC;
}

/******************************************************************************
 *		OleQueryType	[OLECLI.14]
 */
OLESTATUS WINAPI OleQueryType16(SEGPTR oleobj16,  LPLONG xlong) {
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result =  OleQueryType(oleobj, xlong);
    return result;
}

/******************************************************************************
 *		OleCreateFromClip	[OLECLI.12]
 */
OLESTATUS WINAPI OleCreateFromClip16( LPCSTR name, SEGPTR olecli, LHCLIENTDOC hclientdoc,
                                      LPCSTR xname, SEGPTR lpoleob,
                                      UINT16 render, UINT16 clipformat )
{
    return OleCreateFromClip(name, MapSL(olecli), hclientdoc, xname, MapSL(lpoleob), render, clipformat);
}

int CALLBACK ole_client_Callback(LPOLECLIENT client, OLE_NOTIFICATION notif, LPOLEOBJECT oleobject)
{
    struct ole_client_info *info = find_ole_client(client);
    int ret;
    WORD args[5];
    SEGPTR obj16 = OLEOBJ16(oleobject);
    args[4] = HIWORD(info->client16);
    args[3] = LOWORD(info->client16);
    args[2] = (WORD)notif;
    args[1] = HIWORD(obj16);
    args[0] = LOWORD(obj16);
    TRACE("(%08x,%04x,%08x)\n", info->client16, notif, obj16);
    WOWCallback16Ex(((OLECLIENTVTBL16*)MapSL(((OLECLIENT16*)MapSL(info->client16))->lpvtbl))->CallBack, WCB16_PASCAL, sizeof(args), args, &ret);
    return ret;
}
LPOLECLIENT get_ole_client32(SEGPTR client)
{
    struct ole_client_info *info = NULL;
    for (int i = 0; i < MAX_OLECLIENT; i++)
    {
        if (!ole_clients[i].used)
        {
            info = ole_clients + i;
            break;
        }
    }
    if (!info)
        return NULL;
    info->used = TRUE;
    info->client.lpvtbl = &info->vtable;
    info->vtable.CallBack = ole_client_Callback;
    info->client16 = client;
    OLECLIENT16 *clientp = MapSL(client);
    info->vtable16 = clientp->lpvtbl;
    return &info->client;
}
OLESTATUS WINAPI OleCreate16(LPCSTR name, SEGPTR client, LPCSTR xname, LHCLIENTDOC hclientdoc, LPCSTR xxname, SEGPTR *oleobject, OLEOPT_RENDER render, OLECLIPFORMAT format)
{
    LPOLECLIENT client32 = get_ole_client32(client);
    LPOLEOBJECT obj32 = 0;
    OLESTATUS status = OleCreate(name, client32, xname, hclientdoc, xxname, &obj32, render, format);
    *oleobject = OLEOBJ16(obj32);
    return status;
}
OLESTATUS WINAPI OleSetHostNames16(DWORD oleobj, LPCSTR name, LPCSTR name2)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    OLESTATUS status = OleSetHostNames(oleobj32, name, name2);
    return status = OLE_OK/*workaround for WRITE.EXE*/;
}
OLE_RELEASE_METHOD WINAPI OleQueryReleaseMethod16(SEGPTR oleobj)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    OLE_RELEASE_METHOD ret = OleQueryReleaseMethod(oleobj32);
    return ret;
}
OLESTATUS WINAPI OleQueryReleaseError16(SEGPTR oleobj)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    OLESTATUS ret = OleQueryReleaseError(oleobj32);
    return ret;
}

OLESTATUS WINAPI OleQuerySize16(SEGPTR oleobj, DWORD *size)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    OLESTATUS ret = OleQuerySize(oleobj32, size);
    return ret;
}

static void RECT16to32(const RECT16 *from, RECT *to)
{
    to->left = from->left;
    to->top = from->top;
    to->right = from->right;
    to->bottom = from->bottom;
}

static void RECT32to16(const RECT *from, RECT16 *to)
{
    to->left = from->left;
    to->top = from->top;
    to->right = from->right;
    to->bottom = from->bottom;
}


OLESTATUS WINAPI OleQueryBounds16(SEGPTR oleobj, RECT16 *rect)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    RECT rect32;
    OLESTATUS ret = OleQueryBounds(oleobj32, &rect32);
    RECT32to16(&rect32, rect);
    return ret;
}

OLESTATUS WINAPI OleDraw16(DWORD oleobj, HDC16 hdc1, const RECT16 *rect1, const RECT16 *rect2, HDC16 hdc2)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    RECT rect132, rect232;
    if (rect1)
    {
        RECT16to32(rect1, &rect132);
    }
    if (rect2)
    {
        RECT16to32(rect2, &rect232);
    }
    OLESTATUS status = OleDraw(oleobj32, HDC_32(hdc1), rect1 ? &rect132 : NULL, rect2 ? &rect232 : NULL, HDC_32(hdc2));
    return status;
}
OLESTATUS WINAPI OleActivate16(SEGPTR oleobj, UINT uint, BOOL b1, BOOL b2, HWND16 hwnd, const RECT16 FAR * rect)
{
    LPOLEOBJECT oleobj32 = OLEOBJ32(oleobj);
    RECT rect32;
    if (rect)
    {
        RECT16to32(rect, &rect32);
    }
    OLESTATUS status = OleActivate(oleobj32, uint, b1, b2, HWND_32(hwnd), rect ? &rect32 : NULL);
    return status;
}

#include <pshpack1.h>
typedef struct {
    UINT16 otdDeviceNameOffset;
    UINT16 otdDriverNameOffset;
    UINT16 otdPortNameOffset;
    UINT16 otdExtDevmodeOffset;
    UINT16 otdExtDevmodeSize;
    UINT16 otdEnvironmentOffset;
    UINT16 otdEnvironmentSize;
    BYTE otdData[1];
} OLETARGETDEVICE16;
#include <poppack.h>
OLESTATUS WINAPI OleSetTargetDevice16(SEGPTR oleobj16, HGLOBAL16 hotd)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLETARGETDEVICE16 *otd = GlobalLock16(hotd);
    DWORD size = GlobalSize16(hotd);
    DWORD exdata = size - sizeof(OLETARGETDEVICE16) + 1;
    HGLOBAL hotd32 = GlobalAlloc(0, sizeof(OLETARGETDEVICE) + exdata);
    OLETARGETDEVICE *otd32 = GlobalLock(hotd32);
    otd32->otdDeviceNameOffset = otd->otdDeviceNameOffset;
    otd32->otdDriverNameOffset = otd->otdDriverNameOffset;
    otd32->otdPortNameOffset = otd->otdPortNameOffset;
    otd32->otdExtDevmodeOffset = otd->otdExtDevmodeOffset;
    otd32->otdExtDevmodeSize = otd->otdExtDevmodeSize;
    otd32->otdEnvironmentOffset = otd->otdEnvironmentOffset;
    otd32->otdEnvironmentSize = otd->otdEnvironmentSize;
    memcpy(&otd32->otdData, &otd->otdData, exdata);
    OLESTATUS status = OleSetTargetDevice(oleobj, otd32);
    GlobalUnlock(otd32);
    GlobalFree(hotd32);
    GlobalUnlock16(hotd);
    /* workaround for WRITE.EXE */
    if (status == OLE_BUSY)
        return OLE_OK;
    return status;
}

OLESTATUS WINAPI OleCreateFromFile16(LPCSTR protocol, SEGPTR client, LPCSTR lpszClass, LPCSTR file, LHCLIENTDOC clientdoc, LPCSTR objname, SEGPTR *lplpObject, OLEOPT_RENDER render, OLECLIPFORMAT fmt)
{
    LPOLECLIENT client32 = get_ole_client32(client);
    LPOLEOBJECT obj32 = 0;
    OLESTATUS result = OleCreateFromFile(protocol, client32, lpszClass, file, clientdoc, objname, &obj32, render, fmt);
    *lplpObject = OLEOBJ16(obj32);
    if (result == OLE_ERROR_OPEN)
    {
        int count = 5;
        while (--count && result == OLE_ERROR_OPEN)
        {
            ERR("(%s,%08x,%s,%s,%08x,%s,%p,%d,%d):result == OLE_ERROR_OPEN\n", protocol, client32, lpszClass, file, clientdoc, objname, lplpObject, render, fmt);
            result = OleCreateFromFile(protocol, client32, lpszClass, file, clientdoc, objname, &obj32, render, fmt);
        }
        *lplpObject = OLEOBJ16(obj32);
        if (result == OLE_OK)
            ERR("success.\n");
    }
    return result;
}

OLESTATUS WINAPI OleQueryReleaseStatus16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleQueryReleaseStatus(oleobj);
    return result;
}

OLESTATUS WINAPI OleLockServer16(SEGPTR oleobj16, LHSERVER *server)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleLockServer(oleobj, server);
    return result;
}

OLESTATUS WINAPI OleUnlockServer16(LHSERVER hserver16)
{
    OLESTATUS result = OleUnlockServer(hserver16);
    return result;
}

OLESTATUS WINAPI OleDelete16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleDelete(oleobj);
    return result;
}

OLESTATUS WINAPI OleQueryOpen16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleQueryOpen(oleobj);
    return result;
}

OLESTATUS WINAPI OleRelease16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleRelease(oleobj);
    return result;
}

OLESTATUS WINAPI OleUpdate16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleUpdate(oleobj);
    return result;
}

OLESTATUS WINAPI OleClose16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleClose(oleobj);
    return result;
}

OLESTATUS WINAPI OleReconnect16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleReconnect(oleobj);
    return result;
}

OLESTATUS WINAPI OleEqual16(SEGPTR oleobj16, SEGPTR oleobj162)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    LPOLEOBJECT oleobj2 = OLEOBJ32(oleobj162);
    OLESTATUS result = OleEqual(oleobj, oleobj2);
    return result;
}

OLESTATUS WINAPI OleRename16(SEGPTR oleobj16, LPCSTR name)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleRename(oleobj, name);
    return result;
}

OLESTATUS WINAPI OleGetData16(SEGPTR oleobj16, OLECLIPFORMAT fmt, HANDLE16 *handle)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    HANDLE handle32;
    OLESTATUS result;
    *handle = 0;
    if (TRACE_ON(ole))
    {
        char buf[100];
        if (GetClipboardFormatNameA(fmt, buf, 100))
        {
            TRACE("(%p,%04x(%s),%p)\n", oleobj, fmt, debugstr_a(buf), handle);
        }
        else
        {
            TRACE("(%p,%04x,%p)\n", oleobj, fmt, handle);
        }
    }
    if (fmt == CF_METAFILEPICT)
    {
        /* user.c SetClipboardData16 */
        HANDLE16 data16 = 0;
        UINT size;
        void *ptr;
        result = OleGetData(oleobj, fmt, &handle32);
        METAFILEPICT16 *pict16;
        METAFILEPICT *pict32 = GlobalLock(handle32);

        if (pict32)
        {
            if (!(data16 = GlobalAlloc16(GMEM_MOVEABLE, sizeof(*pict16)))) return 0;
            pict16 = GlobalLock16(data16);
            pict16->mm = pict32->mm;
            pict16->xExt = pict32->xExt;
            pict16->yExt = pict32->yExt;
            size = GetMetaFileBitsEx(pict32->hMF, 0, NULL);
            pict16->hMF = GlobalAlloc16(GMEM_MOVEABLE, size);
            ptr = GlobalLock16(pict16->hMF);
            GetMetaFileBitsEx(pict32->hMF, size, ptr);
            GlobalUnlock16(pict16->hMF);
            GlobalUnlock16(data16);
        }
        *handle = data16;
        return result;
    }
    if (fmt == CF_BITMAP)
    {
        result = OleGetData(oleobj, fmt, &handle32);
        *handle = HBITMAP_16(handle32);
        return result;
    }
    if (fmt < 0xc000)
    {
        FIXME("unknown format %04x\n", fmt);
    }
    result = OleGetData(oleobj, fmt, &handle32);
    if (result == OLE_OK && handle32)
    {
        SIZE_T size = GlobalSize(handle32);
        HGLOBAL16 handle16 = GlobalAlloc16(GMEM_MOVEABLE, size);
        *handle = handle16;
        LPVOID mem = GlobalLock16(handle16);
        memcpy(mem, GlobalLock(handle32), size);
        GlobalFree(handle32);
        GlobalUnlock16(handle16);
    }
    return result;
}

OLESTATUS WINAPI OleSetData16(SEGPTR oleobj16, OLECLIPFORMAT fmt, HANDLE16 handle)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result;
    if (TRACE_ON(ole))
    {
        char buf[100];
        if (GetClipboardFormatNameA(fmt, buf, 100))
        {
            TRACE("(%p,%04x(%s),%04x)\n", oleobj, fmt, debugstr_a(buf), handle);
        }
        else
        {
            TRACE("(%p,%04x,%04x)\n", oleobj, fmt, handle);
        }
    }
    if (fmt == CF_METAFILEPICT)
    {
        FIXME("fmt == CF_METAFILEPICT\n");
        return OLE_ERROR_OBJECT;
    }
    if (fmt == CF_BITMAP)
    {
        FIXME("fmt == CF_BITMAP\n");
        return OLE_ERROR_OBJECT;
    }
    if (fmt < 0xc000)
    {
        FIXME("unknown format %04x\n", fmt);
    }
    DWORD size = GlobalSize16(handle);
    LPVOID mem = GlobalLock16(handle);
    HGLOBAL hg = GlobalAlloc(0, size);
    LPVOID mem32 = GlobalLock(hg);
    memcpy(mem32, mem, size);
    GlobalUnlock(hg);
    GlobalUnlock16(handle);
    result = OleSetData(oleobj, fmt, hg);
    GlobalFree(hg);
    return result;
}

OLECLIPFORMAT WINAPI OleEnumFormats16(SEGPTR oleobj16, OLECLIPFORMAT fmt)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    return OleEnumFormats(oleobj, fmt);
}

OLESTATUS WINAPI OleCopyToClipboard16(SEGPTR oleobj16)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    OLESTATUS result = OleCopyToClipboard(oleobj);
    return result;
}

OLESTATUS WINAPI OleClone16(SEGPTR oleobj16, SEGPTR client, LHCLIENTDOC doc, LPCSTR name, LPOLEOBJECT *lplpobj)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    LPOLECLIENT client32 = get_ole_client32(client);
    OLESTATUS result = OleClone(oleobj, client32, doc, name, lplpobj);
    return result;
}
OLESTATUS WINAPI OleSaveToStream16(SEGPTR oleobj16, SEGPTR lpStream)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    LPOLESTREAM stream = OLESTREAM_32(lpStream);
    OLESTATUS result = OleSaveToStream(oleobj, stream);
    return result;
}
OLESTATUS WINAPI OleLoadFromStream16(SEGPTR lpStream16, LPCSTR protocol, SEGPTR client, LHCLIENTDOC doc, LPCSTR objname, SEGPTR *lplpoleobj16)
{
    LPOLESTREAM stream32 = OLESTREAM_32(lpStream16);
    LPOLECLIENT client32 = get_ole_client32(client);
    LPOLEOBJECT obj = 0;
    OLESTATUS result = OleLoadFromStream(stream32, protocol, client32, doc, objname, &obj);
    *lplpoleobj16 = OLEOBJ16(obj);
    return result;
}

OLESTATUS WINAPI OleSetBounds16(SEGPTR oleobj16, RECT16 *rect)
{
    RECT rect32;
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    if (rect)
        RECT16to32(rect, &rect32);
    OLESTATUS result = OleSetBounds(oleobj, rect ? &rect32 : NULL);
    return result;
}

OLESTATUS WINAPI OleQueryName16(SEGPTR oleobj16, LPSTR name, UINT16 *lpbufsize)
{
    LPOLEOBJECT oleobj = OLEOBJ32(oleobj16);
    UINT bufsize = 0;
    bufsize = *lpbufsize;
    OLESTATUS result = OleQueryName(oleobj, name, &bufsize);
    *lpbufsize = bufsize;
    return result;
}

OLESTATUS WINAPI OleCreateInvisible16(LPCSTR protocol, SEGPTR olecli16, LPCSTR cls, LHCLIENTDOC doc, LPCSTR obj, SEGPTR *lplpoleobj16, OLEOPT_RENDER renderopt, OLECLIPFORMAT fmt, BOOL actv)
{
    LPOLECLIENT client32 = get_ole_client32(olecli16);
    LPOLEOBJECT obj32 = NULL;
    OLESTATUS result = OleCreateInvisible(protocol, client32, cls, doc, obj, &obj32, renderopt, fmt, actv);
    *lplpoleobj16 = OLEOBJ16(obj32);
    return result;
}

OLESTATUS WINAPI OleSetColorScheme16(SEGPTR oleobj16, const LOGPALETTE *pal)
{
    LPOLEOBJECT obj32 = OLEOBJ32(oleobj16);
    OLESTATUS result = OleSetColorScheme(obj32, pal);
    return result;
}
