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
#include <ole.h>
#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "objbase.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

#define HINSTANCE_32(h16)      ((HINSTANCE)(ULONG_PTR)(h16))
#define HINSTANCE_16(h32)      (LOWORD(h32))
typedef LONG LHSERVER;
typedef LONG LHSERVERDOC;
typedef LPCSTR LPCOLESTR16;

typedef struct _OLESERVERDOC16 *LPOLESERVERDOC16;

struct _OLESERVERDOCVTBL16;
typedef struct _OLESERVERDOC16
{
    const struct _OLESERVERDOCVTBL16 *lpvtbl;
} OLESERVERDOC16;

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
#define MAX_OLESERVER 32
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
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_create_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, LPOLESERVERDOC FAR* e)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_create_from_template_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, OLE_LPCSTR e, LPOLESERVERDOC FAR* f)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_edit_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, OLE_LPCSTR d, LPOLESERVERDOC FAR* e)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_exit_callback(LPOLESERVER a)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_release_callback(LPOLESERVER a)
{
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
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Close(LPOLESERVERDOC document)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_SetHostNames(LPOLESERVERDOC document, OLE_LPCSTR name1, OLE_LPCSTR name2)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_SetDocDimensions(LPOLESERVERDOC document, OLE_CONST RECT FAR* rect)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_GetObject(LPOLESERVERDOC document, OLE_LPCSTR name, LPOLEOBJECT FAR* object, LPOLECLIENT client)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Release(LPOLESERVERDOC document)
{
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
