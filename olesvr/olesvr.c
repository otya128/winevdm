/*
 *	OLESVR library
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
#include <windows.h>
#ifndef _MSC_VER
#define _Analysis_noreturn_
#endif
#define SERVERONLY
#include <ole.h>
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "objbase.h"
#include "wine/debug.h"
#include "wine/wingdi16.h"
#include "wingdi.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

#define HINSTANCE_32(h16)      ((HINSTANCE)(ULONG_PTR)(h16))
#define HINSTANCE_16(h32)      (LOWORD(h32))
#define MAX_OLESERVER 32
typedef LONG LHSERVER;
typedef LONG LHSERVERDOC;
typedef LPCSTR LPCOLESTR16;
typedef INT16 OLE_NOTIFICATION16;
typedef OLEOPT_UPDATE *LPOLEOPT_UPDATE;

typedef struct
{
    OLEOBJECT obj;
    SEGPTR obj16;
} ole_object_wrapper;
ole_object_wrapper ole_objs[MAX_OLESERVER];
ole_object_wrapper *impl_from_oleobject(LPOLEOBJECT obj)
{
    return (ole_object_wrapper*)CONTAINING_RECORD(obj, ole_object_wrapper, obj);
}
ole_object_wrapper *find_oleobject(SEGPTR obj16)
{
    for (int i = 0; i < MAX_OLESERVER; i++)
        if (ole_objs[i].obj16 == obj16) return &ole_objs[i];
    return NULL;
}
typedef struct _OLEOBJECTVTBL16 {
    SEGPTR QueryProtocol; /* void *(CALLBACK *QueryProtocol)(_LPOLEOBJECT,LPCOLESTR16);*/
    SEGPTR Release; /* OLESTATUS (CALLBACK *Release)(_LPOLEOBJECT);*/
    SEGPTR Show; /* OLESTATUS (CALLBACK *Show)(_LPOLEOBJECT,BOOL16);*/
    SEGPTR DoVerb; /* OLESTATUS (CALLBACK *DoVerb)(_LPOLEOBJECT,UINT16,BOOL16,BOOL16);*/
    SEGPTR GetData; /* OLESTATUS (CALLBACK *GetData)(_LPOLEOBJECT,OLECLIPFORMAT,HANDLE16 *);*/
    SEGPTR SetData; /* OLESTATUS (CALLBACK *SetData)(_LPOLEOBJECT,OLECLIPFORMAT,HANDLE16);*/
    SEGPTR SetTargetDevice; /* OLESTATUS (CALLBACK *SetTargetDevice)(_LPOLEOBJECT,HGLOBAL16);*/
    SEGPTR SetBounds; /* OLESTATUS (CALLBACK *SetBounds)(_LPOLEOBJECT,LPRECT16);*/
    SEGPTR EnumFormats; /* OLESTATUS (CALLBACK *EnumFormats)(_LPOLEOBJECT,OLECLIPFORMAT);*/
    SEGPTR SetColorScheme; /* OLESTATUS (CALLBACK *SetColorScheme)(_LPOLEOBJECT,struct tagLOGPALETTE*);*/
#ifndef SERVERONLY // nothing below used here
    SEGPTR Delete; /* OLESTATUS (CALLBACK *Delete)(_LPOLEOBJECT);*/
    SEGPTR SetHostNames; /* OLESTATUS (CALLBACK *SetHostNames)(_LPOLEOBJECT,LPCOLESTR16,LPCOLESTR16);*/
    SEGPTR SaveToStream; /* OLESTATUS (CALLBACK *SaveToStream)(_LPOLEOBJECT,struct _OLESTREAM*);*/
    SEGPTR Clone; /* OLESTATUS (CALLBACK *Clone)(_LPOLEOBJECT,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT *);*/
    SEGPTR CopyFromLink; /* OLESTATUS (CALLBACK *CopyFromLink)(_LPOLEOBJECT,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT *);*/
    SEGPTR Equal; /* OLESTATUS (CALLBACK *Equal)(_LPOLEOBJECT,_LPOLEOBJECT);*/
    SEGPTR CopyToClipBoard; /* OLESTATUS (CALLBACK *CopyToClipBoard)(_LPOLEOBJECT);*/
    SEGPTR Draw; /* OLESTATUS (CALLBACK *Draw)(_LPOLEOBJECT,HDC16,LPRECT16,LPRECT16,HDC16);*/
    SEGPTR Activate; /* OLESTATUS (CALLBACK *Activate)(_LPOLEOBJECT,UINT16,BOOL16,BOOL16,HWND16,LPRECT16);*/
    SEGPTR Execute; /* OLESTATUS (CALLBACK *Execute)(_LPOLEOBJECT,HGLOBAL16,UINT16);*/
    SEGPTR Close; /* OLESTATUS (CALLBACK *Close)(_LPOLEOBJECT);*/
    SEGPTR Update; /* OLESTATUS (CALLBACK *Update)(_LPOLEOBJECT);*/
    SEGPTR Reconnect; /* OLESTATUS (CALLBACK *Reconnect)(_LPOLEOBJECT);*/
    SEGPTR ObjectConvert; /* OLESTATUS (CALLBACK *ObjectConvert)(_LPOLEOBJECT,LPCOLESTR16,LPOLECLIENT,LHCLIENTDOC,LPCOLESTR16,_LPOLEOBJECT*);*/
    SEGPTR GetLinkUpdateOptions; /* OLESTATUS (CALLBACK *GetLinkUpdateOptions)(_LPOLEOBJECT,LPOLEOPT_UPDATE);*/
    SEGPTR SetLinkUpdateOptions; /* OLESTATUS (CALLBACK *SetLinkUpdateOptions)(_LPOLEOBJECT,OLEOPT_UPDATE);*/
    SEGPTR Rename; /* OLESTATUS (CALLBACK *Rename)(_LPOLEOBJECT,LPCOLESTR16);*/
    SEGPTR QueryName; /* OLESTATUS (CALLBACK *QueryName)(_LPOLEOBJECT,LPSTR,LPUINT16);*/
    SEGPTR QueryType; /* OLESTATUS (CALLBACK *QueryType)(_LPOLEOBJECT,LPLONG);*/
    SEGPTR QueryBounds; /* OLESTATUS (CALLBACK *QueryBounds)(_LPOLEOBJECT,LPRECT16);*/
    SEGPTR QuerySize; /* OLESTATUS (CALLBACK *QuerySize)(_LPOLEOBJECT,LPDWORD);*/
    SEGPTR QueryOpen; /* OLESTATUS (CALLBACK *QueryOpen)(_LPOLEOBJECT);*/
    SEGPTR QueryOutOfDate; /* OLESTATUS (CALLBACK *QueryOutOfDate)(_LPOLEOBJECT);*/
    SEGPTR QueryReleaseStatus; /* OLESTATUS (CALLBACK *QueryReleaseStatus)(_LPOLEOBJECT);*/
    SEGPTR QueryReleaseError; /* OLESTATUS (CALLBACK *QueryReleaseError)(_LPOLEOBJECT);*/
    SEGPTR QueryReleaseMethod; /* OLE_RELEASE_METHOD (CALLBACK *QueryReleaseMethod)(_LPOLEOBJECT);*/
    SEGPTR RequestData; /* OLESTATUS (CALLBACK *RequestData)(_LPOLEOBJECT,OLECLIPFORMAT);*/
    SEGPTR ObjectLong; /* OLESTATUS (CALLBACK *ObjectLong)(_LPOLEOBJECT,UINT16,LPLONG);*/
#endif
} OLEOBJECTVTBL16, *LPOLEOBJECTVTBL16;

typedef struct _OLEOBJECT16
{
    SEGPTR lpvtbl; /* const OLEOBJECTVTBL16 *lpvtbl; */
} OLEOBJECT16, *LPOLEOBJECT16;

LPOLEOBJECTVTBL16 get_oleobjectvtbl16(ole_object_wrapper *obj)
{
    return ((LPOLEOBJECTVTBL16)MapSL(((LPOLEOBJECT16)MapSL(obj->obj16))->lpvtbl));
}

void * CALLBACK ole_obj_QueryProtocol(LPOLEOBJECT obj, LPCOLESTR a)
{
   TRACE("\n");
   return OLE_OK;
}
OLESTATUS CALLBACK ole_obj_Release(LPOLEOBJECT obj)
{
    TRACE("\n");
    WORD args[2];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    args[1] = HIWORD(objw->obj16);
    args[0] = LOWORD(objw->obj16);
    WOWCallback16Ex(vtbl16->Release, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_Show(LPOLEOBJECT obj,BOOL a)
{
    TRACE("\n");
    WORD args[3];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    args[2] = HIWORD(objw->obj16);
    args[1] = LOWORD(objw->obj16);
    args[0] = LOWORD(a);
    WOWCallback16Ex(vtbl16->Show, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_DoVerb(LPOLEOBJECT obj,UINT a,BOOL b,BOOL c)
{
    TRACE("\n");
    WORD args[5];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    args[4] = HIWORD(objw->obj16);
    args[3] = LOWORD(objw->obj16);
    args[2] = LOWORD(a);
    args[1] = LOWORD(b);
    args[0] = LOWORD(c);
    WOWCallback16Ex(vtbl16->DoVerb, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_GetData(LPOLEOBJECT obj,OLECLIPFORMAT a,HANDLE *b)
{
    TRACE("\n");
    WORD args[5];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD hndl = MapLS(b);
    DWORD ret;
    args[4] = HIWORD(objw->obj16);
    args[3] = LOWORD(objw->obj16);
    args[2] = LOWORD(a);
    args[1] = HIWORD(hndl);
    args[0] = LOWORD(hndl);
    WOWCallback16Ex(vtbl16->GetData, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(hndl);
    HGLOBAL16 hmem16 = *b;
    if (hmem16)
    {
        if (a == CF_BITMAP)
            *b = HBITMAP_32(hmem16);
        else if (a == CF_METAFILEPICT)
        {
            HGLOBAL hmem = GlobalAlloc(GMEM_DDESHARE, sizeof(METAFILEPICT));
            METAFILEPICT16 *pic16 = GlobalLock16(hmem16);
            METAFILEPICT *pic = GlobalLock(hmem);
            pic->mm = pic16->mm;
            pic->xExt = pic16->xExt;
            pic->yExt = pic16->yExt;
            void *mf = GlobalLock16(pic16->hMF);
            pic->hMF = SetMetaFileBitsEx(GlobalSize16(pic16->hMF), mf);
            GlobalUnlock16(pic16->hMF);
            GlobalFree16(pic16->hMF);
            GlobalUnlock16(hmem16);
            GlobalFree16(hmem16);
            GlobalUnlock(hmem);
            *b = hmem;
        }
        else
        {
            if (a < 0xc000) FIXME("unknown format %04x\n", a);
            int size = GlobalSize16(hmem16);
            void *optr = GlobalLock16(hmem16);
            HGLOBAL hmem = GlobalAlloc(GMEM_MOVEABLE, size);
            void *nptr = GlobalLock(hmem);
            memcpy(nptr, optr, size);
            GlobalUnlock16(hmem16);
            GlobalUnlock(hmem);
            GlobalFree16(hmem16);
            *b = hmem;
        }
    }
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_SetData(LPOLEOBJECT obj,OLECLIPFORMAT fmt,HANDLE handle32)
{
    TRACE("\n");
    HANDLE16 handle = 0;
    if (fmt == CF_METAFILEPICT)
    {
        /* user.c SetClipboardData16 */
        HANDLE16 data16 = 0;
        UINT size;
        void *ptr;
        METAFILEPICT16 *pict16;
        METAFILEPICT *pict32 = GlobalLock(handle32);

        if (pict32)
        {
            data16 = GlobalAlloc16(GMEM_MOVEABLE, sizeof(*pict16));
            pict16 = GlobalLock16(data16);
            pict16->mm = pict32->mm;
            pict16->xExt = pict32->xExt;
            pict16->yExt = pict32->yExt;
            size = GetMetaFileBitsEx(pict32->hMF, 0, NULL);
            pict16->hMF = GlobalAlloc16(GMEM_MOVEABLE, size);
            ptr = GlobalLock16(pict16->hMF);
            GetMetaFileBitsEx(pict32->hMF, size, ptr);
            DeleteMetaFile(pict32->hMF);
            GlobalUnlock16(pict16->hMF);
            GlobalUnlock16(data16);
        }
        GlobalUnlock(handle32);
        GlobalFree(handle32);
        handle = data16;
    }
    else if (fmt == CF_BITMAP)
        handle = HBITMAP_16(handle32);
    else
    {
        if (fmt < 0xc000) FIXME("unknown format %04x\n", fmt);
        int size = GlobalSize(handle32);
        void *optr = GlobalLock(handle32);
        HGLOBAL hmem = GlobalAlloc16(GMEM_MOVEABLE, size);
        void *nptr = GlobalLock16(hmem);
        memcpy(nptr, optr, size);
        GlobalUnlock16(hmem);
        GlobalUnlock(handle32);
        GlobalFree(handle32);
        handle = hmem;
    }
    WORD args[4];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    args[3] = HIWORD(objw->obj16);
    args[2] = LOWORD(objw->obj16);
    args[1] = LOWORD(fmt);
    args[0] = handle;
    WOWCallback16Ex(vtbl16->SetData, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_SetTargetDevice(LPOLEOBJECT obj,HGLOBAL a)
{
    TRACE("\n");
    WORD args[3];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    if (a)
    {
        int size = GlobalSize(a);
        void *optr = GlobalLock(a);
        HGLOBAL16 hmem16 = GlobalAlloc16(GMEM_MOVEABLE, size);
        void *nptr = GlobalLock16(hmem16);
        memcpy(nptr, optr, size);
        GlobalUnlock16(hmem16);
        GlobalUnlock(a);
        GlobalFree(a);
        a = hmem16;
    }
    args[2] = HIWORD(objw->obj16);
    args[1] = LOWORD(objw->obj16);
    args[0] = LOWORD(a);
    WOWCallback16Ex(vtbl16->SetTargetDevice, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_SetBounds(LPOLEOBJECT obj,LPRECT a)
{
   TRACE("\n");
   return OLE_OK;
}
OLECLIPFORMAT CALLBACK ole_obj_EnumFormats(LPOLEOBJECT obj,OLECLIPFORMAT a)
{
    TRACE("\n");
    WORD args[3];
    ole_object_wrapper *objw = impl_from_oleobject(obj);
    LPOLEOBJECTVTBL16 vtbl16 = get_oleobjectvtbl16(objw);
    DWORD ret;
    args[2] = HIWORD(objw->obj16);
    args[1] = LOWORD(objw->obj16);
    args[0] = LOWORD(a);
    WOWCallback16Ex(vtbl16->EnumFormats, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLECLIPFORMAT)(WORD)ret;
}
OLESTATUS CALLBACK ole_obj_SetColorScheme(LPOLEOBJECT obj,struct tagLOGPALETTE* a)
{
   TRACE("\n");
   return OLE_OK;
}

OLEOBJECTVTBL oleobject_vtbl = 
{
    ole_obj_QueryProtocol,
    ole_obj_Release,
    ole_obj_Show,
    ole_obj_DoVerb,
    ole_obj_GetData,
    ole_obj_SetData,
    ole_obj_SetTargetDevice,
    ole_obj_SetBounds,
    ole_obj_EnumFormats,
    ole_obj_SetColorScheme
};

int current_oleobject = 0;
static LPOLEOBJECT register_oleobject(SEGPTR obj16)
{
    if (current_oleobject >= MAX_OLESERVER)
        return NULL;
    current_oleobject++;
    LPOLEOBJECT obj = &ole_objs[current_oleobject - 1].obj;
    obj->lpvtbl = &oleobject_vtbl;
    ole_objs[current_oleobject - 1].obj16 = obj16;
    return obj;
}

INT16 CALLBACK ItemCallBack16(LPOLECLIENT client, OLE_NOTIFICATION16 notif, SEGPTR oleobject);

typedef struct _OLESERVERDOCVTBL16 {
    SEGPTR Save;
    SEGPTR Close;
    SEGPTR SetHostNames;
    SEGPTR SetDocDimensions;
    SEGPTR GetObject;
    SEGPTR Release;
    SEGPTR SetColorScheme;
    SEGPTR Execute;
} OLESERVERDOCVTBL16, *LPOLESERVERDOCVTBL16;

typedef struct _OLESERVERDOC16
{
    const struct _OLESERVERDOCVTBL16 *lpvtbl;
} OLESERVERDOC16, *LPOLESERVERDOC16;

typedef struct
{
    OLESERVERDOC document;
    SEGPTR doc16;
} ole_server_doc_wrapper;
ole_server_doc_wrapper ole_docs[MAX_OLESERVER];
ole_server_doc_wrapper *impl_from_oleserverdoc(LPOLESERVERDOC document)
{
    return (ole_server_doc_wrapper*)CONTAINING_RECORD(document, ole_server_doc_wrapper, document);
}
LPOLESERVERDOCVTBL16 get_oleserverdocvtbl16(ole_server_doc_wrapper *doc)
{
    return ((LPOLESERVERDOCVTBL16)MapSL(((LPOLESERVERDOC16)MapSL(doc->doc16))->lpvtbl));
}
OLESTATUS CALLBACK ole_doc_Save(LPOLESERVERDOC document)
{
    TRACE("\n");
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Close(LPOLESERVERDOC document)
{
    TRACE("\n");
    WORD args[2];
    ole_server_doc_wrapper *doc = impl_from_oleserverdoc(document);
    LPOLESERVERDOCVTBL16 vtbl16 = get_oleserverdocvtbl16(doc);
    DWORD ret;
    args[1] = HIWORD(doc->doc16);
    args[0] = LOWORD(doc->doc16);
    WOWCallback16Ex(vtbl16->Close, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_doc_SetHostNames(LPOLESERVERDOC document, OLE_LPCSTR name1, OLE_LPCSTR name2)
{
    TRACE("\n");
    ole_server_doc_wrapper *doc = impl_from_oleserverdoc(document);
    LPOLESERVERDOCVTBL16 vtbl16 = get_oleserverdocvtbl16(doc);
    WORD args[6];
    DWORD name116 = MapLS(name1);
    DWORD name216 = MapLS(name2);
    DWORD ret;
    args[5] = HIWORD(doc->doc16);
    args[4] = LOWORD(doc->doc16);
    args[3] = HIWORD(name116);
    args[2] = LOWORD(name116);
    args[1] = HIWORD(name216);
    args[0] = LOWORD(name216);
    WOWCallback16Ex(vtbl16->SetHostNames, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(name116);
    UnMapLS(name216);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_doc_SetDocDimensions(LPOLESERVERDOC document, OLE_CONST RECT FAR* rect)
{
    TRACE("\n");
    return OLE_OK;
}

OLESTATUS CALLBACK ole_doc_GetObject(LPOLESERVERDOC document, OLE_LPCSTR name, LPOLEOBJECT FAR* object, LPOLECLIENT client)
{
    TRACE("\n");
    static OLECLIENTVTBL oleclientvtbl16 = {0};
    static SEGPTR ptrvtbl16;
    ole_server_doc_wrapper *doc = impl_from_oleserverdoc(document);
    LPOLESERVERDOCVTBL16 vtbl16 = get_oleserverdocvtbl16(doc);
    WORD args[8];
    DWORD name16 = MapLS(name);
    DWORD obj16 = MapLS(object);
    DWORD cli16 = MapLS(client); // FIXME: this can't be freed until Release
    DWORD ret;
    if (!oleclientvtbl16.CallBack)
    {
        oleclientvtbl16.CallBack = GetProcAddress16(GetModuleHandle16("OLESVR"), "ITEMCALLBACK");
        ptrvtbl16 = MapLS(&oleclientvtbl16);
    }
    client->lpvtbl = ptrvtbl16;
    args[7] = HIWORD(doc->doc16);
    args[6] = LOWORD(doc->doc16);
    args[5] = HIWORD(name16);
    args[4] = LOWORD(name16);
    args[3] = HIWORD(obj16);
    args[2] = LOWORD(obj16);
    args[1] = HIWORD(cli16);
    args[0] = LOWORD(cli16);
    WOWCallback16Ex(vtbl16->GetObject, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(name16);
    UnMapLS(obj16);
    if ((WORD)ret == OLE_OK)
        *object = register_oleobject(*object);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_doc_Release(LPOLESERVERDOC document)
{
    TRACE("\n");
    WORD args[2];
    ole_server_doc_wrapper *doc = impl_from_oleserverdoc(document);
    LPOLESERVERDOCVTBL16 vtbl16 = get_oleserverdocvtbl16(doc);
    DWORD ret;
    args[1] = HIWORD(doc->doc16);
    args[0] = LOWORD(doc->doc16);
    WOWCallback16Ex(vtbl16->Release, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
OLESTATUS CALLBACK ole_doc_SetColorScheme(LPOLESERVERDOC document, OLE_CONST LOGPALETTE FAR* palette)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Execute(LPOLESERVERDOC document, HGLOBAL h)
{
    return OLE_OK;
}
OLESERVERDOCVTBL oleserverdoc_vtable =
{
    ole_doc_Save,
    ole_doc_Close,
    ole_doc_SetHostNames,
    ole_doc_SetDocDimensions,
    ole_doc_GetObject,
    ole_doc_Release,
    ole_doc_SetColorScheme,
    ole_doc_Execute
};
int current_oleserver_doc = 0;
static LPOLESERVERDOC register_oleserver_doc(SEGPTR doc16)
{
    if (current_oleserver_doc >= MAX_OLESERVER)
        return NULL;
    current_oleserver_doc++;
    LPOLESERVERDOC doc = &ole_docs[current_oleserver_doc - 1].document;
    doc->lpvtbl = &oleserverdoc_vtable;
    ole_docs[current_oleserver_doc - 1].doc16 = doc16;
    return doc;
}
typedef struct _OLESERVER16 *LPOLESERVER16;
typedef struct _OLESERVERVTBL16
{
    OLESTATUS (CALLBACK *Open)(LPOLESERVER,LHSERVERDOC,LPCOLESTR16,LPOLESERVERDOC *);
    OLESTATUS (CALLBACK *Create)(LPOLESERVER,LHSERVERDOC,LPCOLESTR16,LPCOLESTR16,LPOLESERVERDOC*);
    OLESTATUS (CALLBACK *CreateFromTemplate)(LPOLESERVER,LHSERVERDOC,LPCOLESTR16,LPCOLESTR16,LPCOLESTR16,LPOLESERVERDOC *);
    OLESTATUS (CALLBACK *Edit)(LPOLESERVER,LHSERVERDOC,LPCOLESTR16,LPCOLESTR16,LPOLESERVERDOC *);
    OLESTATUS (CALLBACK *Exit)(LPOLESERVER);
    OLESTATUS (CALLBACK *Release)(LPOLESERVER);
    OLESTATUS (CALLBACK *Execute)(LPOLESERVER);
} OLESERVERVTBL16, *LPOLESERVERVTBL16;

typedef struct _OLESERVER16
{
    const OLESERVERVTBL16 *lpvtbl;
} OLESERVER16;

static LONG OLE_current_handle;
int current_oleserver = 0;
typedef struct
{
    OLESERVER server;
    SEGPTR server16;
} ole_server_wrapper;
ole_server_wrapper ole_servers[MAX_OLESERVER];
ole_server_wrapper *impl_from_oleserver(LPOLESERVER server)
{
    return (ole_server_wrapper*)CONTAINING_RECORD(server, ole_server_wrapper, server);
}
LPOLESERVERVTBL16 get_oleservervtbl16(ole_server_wrapper *server)
{
    return ((LPOLESERVERVTBL16)MapSL(((LPOLESERVER16)MapSL(server->server16))->lpvtbl));
}
static OLESTATUS CALLBACK ole_open_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR doc_name, LPOLESERVERDOC FAR* d)
{
    TRACE("\n");
    ole_server_wrapper *server = impl_from_oleserver(a);
    WORD args[8];
    LPOLESERVERVTBL16 vtbl16 = get_oleservervtbl16(server);
    DWORD doc_name16 = MapLS(doc_name);
    DWORD doc16 = MapLS(d);
    DWORD ret;
    args[7] = HIWORD(server->server16);
    args[6] = LOWORD(server->server16);
    args[5] = HIWORD(b);
    args[4] = LOWORD(b);
    args[3] = HIWORD(doc_name16);
    args[2] = LOWORD(doc_name16);
    args[1] = HIWORD(doc16);
    args[0] = LOWORD(doc16);
    WOWCallback16Ex(vtbl16->Open, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(doc_name16);
    UnMapLS(doc16);
    if ((WORD)ret == OLE_OK)
        *d = register_oleserver_doc(*d);
    return (OLESTATUS)(WORD)ret;
}
static OLESTATUS CALLBACK ole_create_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, LPOLESERVERDOC FAR* e)
{
    TRACE("\n");
    ole_server_wrapper *server = impl_from_oleserver(a);
    WORD args[10];
    LPOLESERVERVTBL16 vtbl16 = get_oleservervtbl16(server);
    DWORD doc_name16 = MapLS(d);
    DWORD class16 = MapLS(c);
    DWORD doc16 = MapLS(e);
    DWORD ret;
    args[9] = HIWORD(server->server16);
    args[8] = LOWORD(server->server16);
    args[7] = HIWORD(b);
    args[6] = LOWORD(b);
    args[5] = HIWORD(class16);
    args[4] = LOWORD(class16);
    args[3] = HIWORD(doc_name16);
    args[2] = LOWORD(doc_name16);
    args[1] = HIWORD(doc16);
    args[0] = LOWORD(doc16);
    WOWCallback16Ex(vtbl16->Create, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(doc_name16);
    UnMapLS(class16);
    UnMapLS(doc16);
    if ((WORD)ret == OLE_OK)
        *e = register_oleserver_doc(*e);
    return (OLESTATUS)(WORD)ret;
}
static OLESTATUS CALLBACK ole_create_from_template_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, OLE_LPCSTR e, LPOLESERVERDOC FAR* f)
{
    TRACE("\n");
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_edit_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, LPOLESERVERDOC FAR* e)
{
    TRACE("\n");
    ole_server_wrapper *server = impl_from_oleserver(a);
    WORD args[10];
    LPOLESERVERVTBL16 vtbl16 = get_oleservervtbl16(server);
    DWORD doc_name16 = MapLS(d);
    DWORD class16 = MapLS(c);
    DWORD doc16 = MapLS(e);
    DWORD ret;
    args[9] = HIWORD(server->server16);
    args[8] = LOWORD(server->server16);
    args[7] = HIWORD(b);
    args[6] = LOWORD(b);
    args[5] = HIWORD(class16);
    args[4] = LOWORD(class16);
    args[3] = HIWORD(doc_name16);
    args[2] = LOWORD(doc_name16);
    args[1] = HIWORD(doc16);
    args[0] = LOWORD(doc16);
    WOWCallback16Ex(vtbl16->Edit, WCB16_PASCAL, sizeof(args), args, &ret);
    UnMapLS(doc_name16);
    UnMapLS(class16);
    UnMapLS(doc16);
    if ((WORD)ret == OLE_OK)
        *e = register_oleserver_doc(*e);
    return (OLESTATUS)(WORD)ret;
}
static OLESTATUS CALLBACK ole_exit_callback(LPOLESERVER a)
{
    TRACE("\n");
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_release_callback(LPOLESERVER a)
{
    TRACE("\n");
    ole_server_wrapper *server = impl_from_oleserver(a);
    WORD args[2];
    LPOLESERVERVTBL16 vtbl16 = get_oleservervtbl16(server);
    DWORD ret;
    args[1] = HIWORD(server->server16);
    args[0] = LOWORD(server->server16);
    WOWCallback16Ex(vtbl16->Release, WCB16_PASCAL, sizeof(args), args, &ret);
    return (OLESTATUS)(WORD)ret;
}
static OLESTATUS CALLBACK ole_excute_callback(LPOLESERVER a, HGLOBAL b)
{
    return OLE_OK;
}
static OLESERVERVTBL oleserver_vtbl =
{
    ole_open_callback,
    ole_create_callback,
    ole_create_from_template_callback,
    ole_edit_callback,
    ole_exit_callback,
    ole_release_callback,
    ole_excute_callback
};
static OLESERVER *register_oleserver(SEGPTR s16)
{
    if (current_oleserver >= MAX_OLESERVER)
        return NULL;
    ole_servers[current_oleserver].server16 = s16;
    ole_servers[current_oleserver].server.lpvtbl = &oleserver_vtbl;
    current_oleserver++;
    return &ole_servers[current_oleserver - 1].server;
}

/******************************************************************************
 *		OleRegisterServer	[OLESVR.2]
 */
OLESTATUS WINAPI OleRegisterServer16( LPCSTR name, LPOLESERVER16 serverStruct,
                                      LHSERVER *hRet, HINSTANCE16 hServer,
                                      OLE_SERVER_USE use )
{
    OLESERVER *s = register_oleserver(serverStruct);
    OLESTATUS status = OleRegisterServer(name, s, hRet, HINSTANCE_32(hServer), use);
    return status;
}

/******************************************************************************
 *		OleRevokeServer	[OLESVR.3]
 */
OLESTATUS WINAPI OleRevokeServer16(LHSERVER hServer)
{
    return OleRevokeServer(hServer);
}

/******************************************************************************
 *		OleBlockServer	[OLESVR.4]
 */
OLESTATUS WINAPI OleBlockServer16(LHSERVER hServer)
{
    return OleBlockServer(hServer);
}

/******************************************************************************
 *		OleUnblockServer	[OLESVR.5]
 */
OLESTATUS WINAPI OleUnblockServer16(LHSERVER hServer, BOOL16 *block)
{
    BOOL block32 = *block;
    OLESTATUS status = OleUnblockServer(hServer, &block32);
    *block = block32;
    return status;
}
/***********************************************************************
 *		OleRegisterServerDoc	[OLESVR.6]
 */
OLESTATUS WINAPI OleRegisterServerDoc16( LHSERVER hServer, LPCSTR docname,
                                         SEGPTR document,
                                         LHSERVERDOC *hRet)
{
    LPOLESERVERDOC doc32 = register_oleserver_doc(document);
    OLESTATUS result = OleRegisterServerDoc(hServer, docname, doc32, hRet);
    return result;
}

/******************************************************************************
 *		OleRevokeServerDoc	[OLESVR.7]
 */
OLESTATUS WINAPI OleRevokeServerDoc16(LHSERVERDOC hServerDoc)
{
    return OleRevokeServerDoc(hServerDoc);
}

/******************************************************************************
 *		OleRenameServerDoc	[OLESVR.8]
 *
 */
OLESTATUS WINAPI OleRenameServerDoc16(LHSERVERDOC hDoc, LPCSTR newName)
{
    return OleRenameServerDoc(hDoc, newName);
}

/******************************************************************************
 *		OleRevertServerDoc	[OLESVR.9]
 *
 */
OLESTATUS WINAPI OleRevertServerDoc16(LHSERVERDOC hDoc)
{
    return OleRevertServerDoc(hDoc);
}

/******************************************************************************
 *		OleSavedServerDoc	[OLESVR.10]
 *
 */
OLESTATUS WINAPI OleSavedServerDoc16(LHSERVERDOC hDoc)
{
    return OleSavedServerDoc(hDoc);
}

OLESTATUS WINAPI OleRevokeObject16(LPOLECLIENT client)
{
    return OleRevokeObject(client);
}

DWORD WINAPI OleQueryServerVersion16()
{
    return OleQueryServerVersion();
}

INT16 CALLBACK ItemCallBack16(LPOLECLIENT client, OLE_NOTIFICATION16 notif, SEGPTR oleobject)
{
    static INT (CALLBACK *ItemCallBack)(LPOLECLIENT, OLE_NOTIFICATION, LPOLEOBJECT) = NULL;
    if (!ItemCallBack) ItemCallBack = (INT (CALLBACK *)(LPOLECLIENT, OLE_NOTIFICATION, LPOLEOBJECT))GetProcAddress(GetModuleHandleA("olesvr32"), "ItemCallBack");
    // oleobject must have been created with GetObject
    return ItemCallBack(client, notif, &find_oleobject(oleobject)->obj);
}
