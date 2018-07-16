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
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "objbase.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

#define HINSTANCE_32(h16)	((HINSTANCE)(ULONG_PTR)(h16))
#define HINSTANCE_16(h32)	(LOWORD(h32))
typedef LONG LHSERVER;
typedef LONG LHSERVERDOC;
typedef LPCSTR LPCOLESTR16;

typedef struct _OLESERVERDOC16 *LPOLESERVERDOC16;

struct _OLESERVERDOCVTBL16;
typedef struct _OLESERVERDOC16
{
	const struct _OLESERVERDOCVTBL16 *lpvtbl;
	/* server provided state info */
} OLESERVERDOC16;

typedef struct _OLESERVER16 *LPOLESERVER16;
typedef struct _OLESERVERVTBL16
{
	OLESTATUS(CALLBACK *Open)(LPOLESERVER, LHSERVERDOC, LPCOLESTR16, LPOLESERVERDOC *);
	OLESTATUS(CALLBACK *Create)(LPOLESERVER, LHSERVERDOC, LPCOLESTR16, LPCOLESTR16, LPOLESERVERDOC*);
	OLESTATUS(CALLBACK *CreateFromTemplate)(LPOLESERVER, LHSERVERDOC, LPCOLESTR16, LPCOLESTR16, LPCOLESTR16, LPOLESERVERDOC *);
	OLESTATUS(CALLBACK *Edit)(LPOLESERVER, LHSERVERDOC, LPCOLESTR16, LPCOLESTR16, LPOLESERVERDOC *);
	OLESTATUS(CALLBACK *Exit)(LPOLESERVER);
	OLESTATUS(CALLBACK *Release)(LPOLESERVER);
	OLESTATUS(CALLBACK *Execute)(LPOLESERVER);
} OLESERVERVTBL16, *LPOLESERVERVTBL16;

typedef struct _OLESERVER16
{
	const OLESERVERVTBL16 *lpvtbl;
	/* server specific data */
} OLESERVER16;

static LONG OLE_current_handle;
//
//OLESTATUS(CALLBACK* Open)  (LPOLESERVER, LHSERVERDOC, OLE_LPCSTR, LPOLESERVERDOC FAR*);
///* long handle to doc(privtate to DLL)  */
///* lp to OLESERVER                      */
///* document name                        */
///* place holder for returning oledoc.   */
//
//OLESTATUS(CALLBACK* Create)(LPOLESERVER, LHSERVERDOC, OLE_LPCSTR, OLE_LPCSTR, LPOLESERVERDOC FAR*);
///* long handle to doc(privtate to DLL)  */
///* lp to OLESERVER                      */
///* lp class name                        */
///* lp doc name                          */
///* place holder for returning oledoc.   */
//
//OLESTATUS(CALLBACK* CreateFromTemplate)(LPOLESERVER, LHSERVERDOC, OLE_LPCSTR, OLE_LPCSTR, OLE_LPCSTR, LPOLESERVERDOC FAR*);
///* long handle to doc(privtate to DLL)  */
///* lp to OLESERVER                      */
///* lp class name                        */
///* lp doc name                          */
///* lp template name                     */
///* place holder for returning oledoc.   */
//
//OLESTATUS(CALLBACK* Edit)  (LPOLESERVER, LHSERVERDOC, OLE_LPCSTR, OLE_LPCSTR, LPOLESERVERDOC FAR*);
///* long handle to doc(privtate to DLL)  */
///* lp to OLESERVER                      */
///* lp class name                        */
///* lp doc name                          */
///* place holder for returning oledoc.   */
//
//_Analysis_noreturn_
//OLESTATUS(CALLBACK* Exit)  (LPOLESERVER);
///* lp OLESERVER                         */
//
//OLESTATUS(CALLBACK* Release)  (LPOLESERVER);
///* lp OLESERVER                         */
//
//OLESTATUS(CALLBACK* Execute)(LPOLESERVER, HGLOBAL);
///* lp OLESERVER                         */
///* handle to command strings            */
#define MAX_OLESERVER 32
int current_oleserver = 0;
OLESERVER oleservers[MAX_OLESERVER];
OLESERVERVTBL oleservervtbls[MAX_OLESERVER];
LPOLESERVERVTBL16 oleservervtbl16s[MAX_OLESERVER];
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
static OLESTATUS CALLBACK ole_edit_callback(LPOLESERVER a, LHSERVERDOC b, OLE_LPCSTR c, LPOLESERVERDOC FAR* d)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_exit_callback(LPOLESERVER a)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_release_callback(LPOLESERVER a)
{
    return OLE_OK;
}
static OLESTATUS CALLBACK ole_excute_callback(LPOLESERVER a, HGLOBAL b)
{
    return OLE_OK;
}
static OLESERVER *register_oleserver(LPOLESERVERVTBL16 vtbl16)
{
    if (current_oleserver >= MAX_OLESERVER)
        return NULL;
    current_oleserver++;
    LPOLESERVER server = &oleservers[current_oleserver - 1];
    server->lpvtbl = &oleservervtbls[current_oleserver - 1];
    server->lpvtbl->Open = ole_open_callback;
    server->lpvtbl->Create = ole_create_callback;
    server->lpvtbl->CreateFromTemplate = ole_create_from_template_callback;
    server->lpvtbl->Edit = ole_edit_callback;
    server->lpvtbl->Exit = ole_exit_callback;
    server->lpvtbl->Release = ole_release_callback;
    server->lpvtbl->Execute = ole_excute_callback;
    oleservervtbl16s[current_oleserver - 1] = vtbl16;
    return server;
}

/******************************************************************************
*		OleRegisterServer	[OLESVR.2]
*/
OLESTATUS WINAPI OleRegisterServer16(LPCSTR name, LPOLESERVER16 serverStruct,
	LHSERVER *hRet, HINSTANCE16 hServer,
	OLE_SERVER_USE use)
{
    OLESERVER *s = register_oleserver(MapSL(serverStruct->lpvtbl));
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
    return OleUnblockServer(hServer, block);
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
} OLESERVERDOCVTBL16;
struct ole_server_doc_wrapper
{
    OLESERVERDOC document;
    OLESERVERDOCVTBL vtable;
    OLESERVERDOCVTBL16 *vtable16;
};
OLESTATUS CALLBACK ole_doc_Save(LPOLESERVERDOC document)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Close      (LPOLESERVERDOC document)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_SetHostNames (LPOLESERVERDOC document, OLE_LPCSTR name1, OLE_LPCSTR name2)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_SetDocDimensions (LPOLESERVERDOC document, OLE_CONST RECT FAR* rect)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_GetObject  (LPOLESERVERDOC document, OLE_LPCSTR name, LPOLEOBJECT FAR* object, LPOLECLIENT client)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Release    (LPOLESERVERDOC document)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_SetColorScheme (LPOLESERVERDOC document, OLE_CONST LOGPALETTE FAR* palette)
{
    return OLE_OK;
}
OLESTATUS CALLBACK ole_doc_Execute   (LPOLESERVERDOC document, HGLOBAL h)
{
    return OLE_OK;
}
struct ole_server_doc_wrapper ole_docs[MAX_OLESERVER];
int current_oleserver_doc = 0;
static LPOLESERVERDOC register_oleserver_doc(OLESERVERDOCVTBL16 *vtbl16)
{
    if (current_oleserver_doc >= MAX_OLESERVER)
        return NULL;
    current_oleserver_doc++;
    LPOLESERVERDOC doc = &ole_docs[current_oleserver_doc - 1].document;
    doc->lpvtbl = &ole_docs[current_oleserver_doc - 1].vtable;
    doc->lpvtbl->Save = ole_doc_Save;
    doc->lpvtbl->Close = ole_doc_Close;
    doc->lpvtbl->SetHostNames = ole_doc_SetHostNames;
    doc->lpvtbl->SetDocDimensions = ole_doc_SetDocDimensions;
    doc->lpvtbl->GetObject = ole_doc_GetObject;
    doc->lpvtbl->Release = ole_doc_Release;
    doc->lpvtbl->SetColorScheme = ole_doc_SetColorScheme;
    doc->lpvtbl->Execute = ole_doc_Execute;
    ole_docs[current_oleserver_doc - 1].vtable16 = vtbl16;
    return doc;
}
/***********************************************************************
*		OleRegisterServerDoc	[OLESVR.6]
*/
OLESTATUS WINAPI OleRegisterServerDoc16(LHSERVER hServer, LPCSTR docname,
	LPOLESERVERDOC document,
	LHSERVERDOC *hRet)
{
    LPOLESERVERDOC doc32 = register_oleserver_doc(document->lpvtbl);
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
