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
        if (ole_clients[i].used && &ole_clients[i].client16 == client)
        {
            return ole_clients + i;
        }
    }
    return NULL;
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
OLESTATUS WINAPI OleEnumObjects16(LHCLIENTDOC hServerDoc, SEGPTR data)
{
    FIXME("(%d, %04x:%04x): stub\n", hServerDoc, HIWORD(data),
	LOWORD(data));
    return OLE_OK;
}

/******************************************************************************
 *	     OleCreateLinkFromClip	[OLECLI.11]
 */
OLESTATUS WINAPI OleCreateLinkFromClip16( LPCSTR name, SEGPTR olecli, LHCLIENTDOC hclientdoc,
                                          LPCSTR xname, SEGPTR lpoleob, UINT16 render,
                                          UINT16 clipformat )
{
    LPOLECLIENT olecli32 = find_ole_client_16(olecli);
    LPOLEOBJECT *oleobj = (LPOLEOBJECT*)MapSL(lpoleob);
    OLESTATUS status = OleCreateLinkFromClip(name, olecli32, hclientdoc, xname, oleobj, render, clipformat);
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
OLESTATUS WINAPI OleQueryType16(OLEOBJECT16 *oleob,  SEGPTR xlong) {
    OLEOBJECT oleobject32;
    oleobject32.lpvtbl = MapSL(oleob->lpvtbl);
    OLESTATUS result =  OleQueryType(&oleobject32, MapSL(xlong));
    oleob->lpvtbl = MapLS(oleobject32.lpvtbl);
    return result;
	//FIXME("(%p, %p): stub!\n", oleob, MapSL(xlong));
	//return OLE_OK;
}

/******************************************************************************
 *		OleCreateFromClip	[OLECLI.12]
 */
OLESTATUS WINAPI OleCreateFromClip16( LPCSTR name, SEGPTR olecli, LHCLIENTDOC hclientdoc,
                                      LPCSTR xname, SEGPTR lpoleob,
                                      UINT16 render, UINT16 clipformat )
{
    return OleCreateFromClip(name, MapSL(olecli), MapSL(hclientdoc), xname, MapSL(lpoleob), render, clipformat);
	//FIXME("(%s, %04x:%04x, %d, %s, %04x:%04x, %d, %d): stub!\n",
    //          name, HIWORD(olecli), LOWORD(olecli), hclientdoc, xname, HIWORD(lpoleob),
    //          LOWORD(lpoleob), render, clipformat);
	//return OLE_OK;
}

int CALLBACK ole_client_Callback(LPOLECLIENT client, OLE_NOTIFICATION notif, LPOLEOBJECT oleobject)
{
    struct ole_client_info *info = find_ole_client(client);
    int ret;
    WORD args[100];
    args[4] = HIWORD(info->client16);
    args[3] = LOWORD(info->client16);
    args[2] = (WORD)notif;
    args[1] = HIWORD(oleobject);
    args[0] = LOWORD(oleobject);
    WOWCallback16Ex(((OLECLIENTVTBL16*)MapSL(info->vtable16))->CallBack, WCB16_PASCAL, sizeof(args), args, &ret);
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
OLESTATUS WINAPI OleCreate16(LPCSTR name, SEGPTR client, LPCSTR xname, LHCLIENTDOC hclientdoc, LPCSTR xxname, LPOLEOBJECT FAR *oleobject, OLEOPT_RENDER render, OLECLIPFORMAT format)
{
    LPOLECLIENT client32 = get_ole_client32(client);
    OLESTATUS status = OleCreate(name, client32, xname, hclientdoc, xxname, oleobject, render, format);
    return status;
}
OLESTATUS WINAPI OleSetHostNames16(DWORD oleobj, LPCSTR name, LPCSTR name2)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
    OLESTATUS status = OleSetHostNames(oleobj32, name, name2);
    return status = OLE_OK/*workaround fir WRITE.EXE*/;
}
OLE_RELEASE_METHOD WINAPI OleQueryReleaseMethod16(DWORD oleobj)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
    OLE_RELEASE_METHOD ret = OleQueryReleaseMethod(oleobj);
    return ret;
}
OLESTATUS WINAPI OleQueryReleaseError16(DWORD oleobj)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
    OLESTATUS ret = OleQueryReleaseError(oleobj);
    return ret;
}

OLESTATUS WINAPI OleQuerySize16(DWORD oleobj, DWORD *size)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
    OLESTATUS ret = OleQuerySize(oleobj, size);
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


OLESTATUS WINAPI OleQueryBounds16(DWORD oleobj, RECT16 *rect)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
    RECT rect32;
    OLESTATUS ret = OleQueryBounds(oleobj, &rect32);
    RECT32to16(&rect32, rect);
    return ret;
}

OLESTATUS WINAPI OleDraw16(DWORD oleobj, HDC16 hdc1, const RECT16 *rect1, const RECT16 *rect2, HDC16 hdc2)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
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
OLESTATUS WINAPI OleActivate16(LPOLEOBJECT oleobj, UINT uint, BOOL b1, BOOL b2, HWND hwnd, const RECT FAR * rect)
{
    LPOLEOBJECT oleobj32 = (LPOLEOBJECT)oleobj;
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
OLESTATUS WINAPI OleSetTargetDevice16(LPOLEOBJECT oleobj, HGLOBAL16 hotd)
{
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
    return status;
}
