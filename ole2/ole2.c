
/*
 *	OLE2 library - 16 bit only interfaces
 *
 *	Copyright 1995	Martin von Loewis
 *      Copyright 1999  Francis Beaudet
 *      Copyright 1999  Noel Borthwick
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

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define NONAMELESSUNION
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "wownt32.h"
#include "ole2.h"
#include "winerror.h"

#include "wine/winbase16.h"
#include "wine/wingdi16.h"
#include "wine/winuser16.h"
#include "ifs.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

#define E_INVALIDARG16 MAKE_SCODE(SEVERITY_ERROR, FACILITY_NULL, 3)

static HICON convert_icon_to_32( HICON16 icon16 )
{
    CURSORICONINFO *info = GlobalLock16( icon16 );
    void *and_bits = info + 1;
    void *xor_bits = (BYTE *)and_bits + info->nHeight * 2 * ((info->nWidth + 15) / 16);
    HICON ret = CreateIcon( 0, info->nWidth, info->nHeight, info->bPlanes, info->bBitsPerPixel,
                            and_bits, xor_bits );
    GlobalUnlock16( icon16 );
    return ret;
}

/******************************************************************************
 *		OleBuildVersion	(OLE2.1)
 */
DWORD WINAPI OleBuildVersion16(void)
{
    return OleBuildVersion();
}

/***********************************************************************
 *           OleInitialize       (OLE2.2)
 */
HRESULT WINAPI OleInitialize16(LPMALLOC pMalloc)
{
    if (pMalloc)
    {
        FIXME("OleInitialize(pMalloc) is not supported.\n");
    }
    return OleInitialize( NULL );
}

/******************************************************************************
 *		OleUninitialize	(OLE2.3)
 */
void WINAPI OleUninitialize16(void)
{
    OleUninitialize();
}

/***********************************************************************
 *           DllGetClassObject (OLE2.4)
 */
HRESULT WINAPI DllGetClassObject16(REFCLSID rclsid, REFIID iid, LPVOID *ppv)
{
  FIXME("(%s, %s, %p): stub\n", debugstr_guid(rclsid), debugstr_guid(iid), ppv);
  return E_NOTIMPL;
}

/***********************************************************************
 *           RegisterDragDrop (OLE2.35)
 */
HRESULT WINAPI RegisterDragDrop16(
	HWND16 hwnd,
	/*LPDROPTARGET*/SEGPTR pDropTarget
) {
	TRACE("(0x%04x,%p)\n",hwnd,pDropTarget);
	return hresult32_16(RegisterDragDrop(HWND_32(hwnd), (IDropTarget*)iface16_32(&IID_IDropTarget, pDropTarget)));
}

/***********************************************************************
 *           RevokeDragDrop (OLE2.36)
 */
HRESULT WINAPI RevokeDragDrop16(
	HWND16 hwnd
) {
	TRACE("(0x%04x)\n",hwnd);
	return hresult32_16(RevokeDragDrop(HWND_32(hwnd)));
}

/******************************************************************************
 * OleMetaFilePictFromIconAndLabel (OLE2.56)
 *
 * Returns a global memory handle to a metafile which contains the icon and
 * label given.
 * I guess the result of that should look somehow like desktop icons.
 * If no hIcon is given, we load the icon via lpszSourceFile and iIconIndex.
 * This code might be wrong at some places.
 */
HGLOBAL16 WINAPI OleMetafilePictFromIconAndLabel16(
	HICON16 icon16,
	LPCOLESTR16 lpszLabel,
	LPCOLESTR16 lpszSourceFile,
	UINT16 iIconIndex
) {
    METAFILEPICT *pict;
    HGLOBAL hmf;
    HGLOBAL16 hmf16;
    LPWSTR label = NULL, source = NULL;
    DWORD len;
    HICON icon = convert_icon_to_32( icon16 );

    if (lpszLabel)
    {
        len = MultiByteToWideChar( CP_ACP, 0, lpszLabel, -1, NULL, 0 );
        label = HeapAlloc( GetProcessHeap(), 0, len * sizeof(WCHAR) );
        MultiByteToWideChar( CP_ACP, 0, lpszLabel, -1, label, len );
    }
    if (lpszSourceFile)
    {
        len = MultiByteToWideChar( CP_ACP, 0, lpszSourceFile, -1, NULL, 0 );
        source = HeapAlloc( GetProcessHeap(), 0, len * sizeof(WCHAR) );
        MultiByteToWideChar( CP_ACP, 0, lpszSourceFile, -1, source, len );
    }
    hmf = OleMetafilePictFromIconAndLabel( icon, label, source, iIconIndex );
    HeapFree( GetProcessHeap(), 0, label );
    HeapFree( GetProcessHeap(), 0, source );
    DestroyIcon( icon );

    if (!hmf) return 0;
    pict = GlobalLock( hmf );

    hmf16 = GlobalAlloc16(0, sizeof(METAFILEPICT16));
    if (hmf16)
    {
        METAFILEPICT16 *pict16 = GlobalLock16( hmf16 );
        pict16->mm   = pict->mm;
        pict16->xExt = pict->xExt;
        pict16->yExt = pict->yExt;
        len = GetMetaFileBitsEx( pict->hMF, 0, 0 );
        pict16->hMF = GlobalAlloc16( GMEM_MOVEABLE, len );
        GetMetaFileBitsEx( pict->hMF, len, GlobalLock16( pict16->hMF) );
        GlobalUnlock16( pict16->hMF );
        GlobalUnlock16( hmf16 );
    }
    DeleteMetaFile( pict->hMF );
    GlobalUnlock( hmf );
    GlobalFree( hmf );
    return hmf16;
}


/******************************************************************************
 *        CreateItemMoniker	(OLE2.27)
 */
HRESULT WINAPI CreateItemMoniker16(LPCOLESTR16 lpszDelim,LPCOLESTR16 lpszItem,SEGPTR* ppmk)
{
    LPMONIKER pmk;
    HRESULT result;
    LPOLESTR wdelim = strdupAtoW(lpszDelim);
    LPOLESTR witem = strdupAtoW(lpszItem);
    TRACE("(%s,%s,%p)\n",lpszDelim,lpszItem,ppmk);
    result = hresult32_16(CreateItemMoniker(wdelim, witem, &pmk));
    HeapFree(GetProcessHeap(), 0, wdelim);
    HeapFree(GetProcessHeap(), 0, witem);
    *ppmk = iface32_16(&IID_IMoniker, pmk);
    return result;
}


/******************************************************************************
 *        OleSetMenuDescriptor (OLE2.41)
 *
 * PARAMS
 *  hOleMenu  FIXME: Should probably be an HOLEMENU16.
 */
HRESULT WINAPI OleSetMenuDescriptor16(
    HOLEMENU               hOleMenu,
    HWND16                 hwndFrame,
    HWND16                 hwndActiveObject,
    LPOLEINPLACEFRAME        lpFrame,
    LPOLEINPLACEACTIVEOBJECT lpActiveObject)
{
    HOLEMENU hOleMenu32;
    TRACE("(%p, %x, %x, %p, %p)\n", hOleMenu, hwndFrame, hwndActiveObject, lpFrame, lpActiveObject);
    hOleMenu32 = HWND_32(hOleMenu); /* FIXME */
    return hresult32_16(OleSetMenuDescriptor(hOleMenu32, HWND_32(hwndFrame), HWND_32(hwndActiveObject),
        (IOleInPlaceFrame*)iface16_32(&IID_IOleInPlaceFrame, lpFrame),
        (IOleInPlaceActiveObject*)iface16_32(&IID_IOleInPlaceActiveObject, lpActiveObject)));
}

/******************************************************************************
 *              OleLoad        [OLE2.12]
 *
 * PARAMS
 *  pStg Segmented LPSTORAGE pointer.
 *  pClientSite Segmented LPOLECLIENTSITE pointer.
 */
HRESULT WINAPI OleLoad16(
    SEGPTR		pStg,
    REFIID            	riid,
    SEGPTR		pClientSite,
    SEGPTR*		ppvObj)
{
    HRESULT result;
    LPVOID pvObj = NULL;
    TRACE("(%08x,%s,%08x,%p)\n", pStg, debugstr_guid(riid), pClientSite, ppvObj);
    result = hresult32_16(OleLoad((IStorage*)iface16_32(&IID_IStorage, pStg), riid, (IOleClientSite*)iface16_32(&IID_IOleClientSite, pClientSite), &pvObj));
    *ppvObj = iface32_16(riid, pvObj);
    return result;
}

/******************************************************************************
 *              OleDoAutoConvert        [OLE2.79]
 */
HRESULT WINAPI OleDoAutoConvert16(LPSTORAGE pStg, LPCLSID pClsidNew)
{
    FIXME("(%p,%p) : stub\n",pStg,pClsidNew);
    return E_NOTIMPL;
}

/***********************************************************************
 *           OleSetClipboard                            [OLE2.49]
 */
HRESULT WINAPI OleSetClipboard16(/* IDataObject* */SEGPTR pDataObj)
{
    TRACE("(%p)\n", pDataObj);
    return hresult32_16(OleSetClipboard((IDataObject*)iface16_32(&IID_IDataObject, pDataObj)));
}

/***********************************************************************
 *           OleGetClipboard                            [OLE2.50]
 */
HRESULT WINAPI OleGetClipboard16(/* IDataObject* */SEGPTR* ppDataObj)
{
    HRESULT result;
    IDataObject *pDataObj = NULL;
    TRACE("(%p)\n", ppDataObj);
    result = hresult32_16(OleGetClipboard(&pDataObj));
    *ppDataObj = iface32_16(&IID_IDataObject, pDataObj);
    return result;
}

/***********************************************************************
 *           OleFlushClipboard   [OLE2.76]
 */

HRESULT WINAPI OleFlushClipboard16(void)
{
  return hresult32_16(OleFlushClipboard());
}

#define GET_SEGPTR_METHOD_ADDR(ifacename,segptr,methodname) \
    ((SEGPTR)((const ifacename##Vtbl*)MapSL((SEGPTR)((ifacename*)MapSL(segptr))->lpVtbl))->methodname)

/***********************************************************************
 *    ReadClassStg (OLE2.18)
 *
 * This method reads the CLSID previously written to a storage object with
 * the WriteClassStg.
 *
 * PARAMS
 *  pstg    [I] Segmented LPSTORAGE pointer.
 *  pclsid  [O] Pointer to where the CLSID is written
 *
 * RETURNS
 *  Success: S_OK.
 *  Failure: HRESULT code.
 */
HRESULT WINAPI ReadClassStg16(SEGPTR pstg, CLSID *pclsid)
{
	STATSTG16 statstg;
	HANDLE16 hstatstg;
	HRESULT	hres;
	DWORD args[3];

	TRACE("(%x, %p)\n", pstg, pclsid);

	if (!pclsid)
	    return E_INVALIDARG16;

	memset(pclsid, 0, sizeof(*pclsid));

	if (!pstg)
	    return E_INVALIDARG16;

	/*
	 * read a STATSTG structure (contains the clsid) from the storage
	 */
	args[0] = pstg; /* iface */
	args[1] = WOWGlobalAllocLock16( 0, sizeof(STATSTG16), &hstatstg );
	args[2] = STATFLAG_DEFAULT;

	if (!WOWCallback16Ex(
	    GET_SEGPTR_METHOD_ADDR(IStorage16, pstg, Stat),
	    WCB16_PASCAL,
	    3*sizeof(DWORD),
	    args,
	    (LPDWORD)&hres
	)) {
	    WOWGlobalUnlockFree16(args[1]);
            ERR("CallTo16 IStorage16::Stat() failed, hres %x\n",hres);
	    return hres;
	}
	memcpy(&statstg, MapSL(args[1]), sizeof(STATSTG16));
	WOWGlobalUnlockFree16(args[1]);

	if(SUCCEEDED(hres)) {
		*pclsid=statstg.clsid;
		TRACE("clsid is %s\n", debugstr_guid(&statstg.clsid));
	}
	return hres;
}

/***********************************************************************
 *    ReadClassStm (OLE2.20)
 */
HRESULT WINAPI ReadClassStm16(SEGPTR stream, CLSID *clsid)
{
    HANDLE16 hclsid, hread;
    HRESULT hres;
    DWORD args[4];

    TRACE("(0x%x, %p)\n", stream, clsid);

    if (!clsid)
        return E_INVALIDARG16;

    memset(clsid, 0, sizeof(*clsid));

    if (!stream)
        return E_INVALIDARG16;

    args[0] = stream; /* iface */
    args[1] = WOWGlobalAllocLock16( 0, sizeof(CLSID), &hclsid );
    args[2] = sizeof(CLSID);
    args[3] = WOWGlobalAllocLock16( 0, sizeof(ULONG), &hread );

    if (WOWCallback16Ex(
        GET_SEGPTR_METHOD_ADDR(IStream16, stream, Read),
        WCB16_PASCAL,
        4*sizeof(DWORD),
        args,
        (DWORD*)&hres))
    {
        ULONG readlen;

        memcpy(&readlen, MapSL(args[3]), sizeof(readlen));
        if (readlen == sizeof(CLSID))
            memcpy(clsid, MapSL(args[1]), sizeof(CLSID));
        else
            hres = STG_E_READFAULT;

        TRACE("clsid is %s\n", debugstr_guid(clsid));
    }
    else
    {
        ERR("CallTo16 IStream16::Read() failed, hres %x\n", hres);
        hres = E_FAIL;
    }
    WOWGlobalUnlockFree16(args[1]);
    WOWGlobalUnlockFree16(args[3]);

    return hres;
}

/***********************************************************************
 *    WriteClassStm (OLE2.21)
 */
HRESULT WINAPI WriteClassStm16(SEGPTR pStm, REFCLSID rclsid)
{
    TRACE("(%08x,%s)\n", pStm, debugstr_guid(rclsid));
    return hresult32_16(WriteClassStm((IStream*)iface16_32(&IID_IStream, pStm), rclsid));
}

/***********************************************************************
 *              GetConvertStg (OLE2.82)
 */
HRESULT WINAPI GetConvertStg16(LPSTORAGE stg)
{
    FIXME("unimplemented stub!\n");
    return E_FAIL;
}

/***********************************************************************
 *              ReleaseStgMedium (OLE2.32)
 */
VOID WINAPI ReleaseStgMedium16(STGMEDIUM16 *pmedium)
{
    switch (pmedium->tymed)
    {
    case TYMED_HGLOBAL:
    {
        if ((pmedium->pUnkForRelease == 0) &&
            (pmedium->u.hGlobal != 0))
            GlobalFree16(pmedium->u.hGlobal);
        break;
    }
    case TYMED_FILE:
    {
        if (pmedium->u.lpszFileName != 0)
        {
            if (pmedium->pUnkForRelease == 0)
            {
                DeleteFileA(MapSL(pmedium->u.lpszFileName));
            }
            FIXME("\n");
            /*CoTaskMemFree(pmedium->u.lpszFileName);*/
        }
        break;
    }
    case TYMED_ISTREAM:
    {
        if (pmedium->u.pstm != 0)
        {
            IStream16_Release(pmedium->u.pstm);
        }
        break;
    }
    case TYMED_ISTORAGE:
    {
        if (pmedium->u.pstg != 0)
        {
            IStorage16_Release(pmedium->u.pstg);
        }
        break;
    }
    case TYMED_GDI:
    {
        if ((pmedium->pUnkForRelease == 0) &&
            (pmedium->u.hGlobal != 0))
            DeleteObject(HBITMAP_32(pmedium->u.hGlobal));
        break;
    }
    case TYMED_MFPICT:
    {
        if ((pmedium->pUnkForRelease == 0) &&
            (pmedium->u.hGlobal != 0))
        {
            LPMETAFILEPICT16 pMP = GlobalLock16(pmedium->u.hGlobal);
            GlobalFree16(pMP->hMF);
            GlobalUnlock16(pmedium->u.hGlobal);
            GlobalFree16(pmedium->u.hGlobal);
        }
        break;
    }
    case TYMED_ENHMF:
    {
        FIXME("\n");
    }
    case TYMED_NULL:
    default:
        break;
    }
    pmedium->tymed = TYMED_NULL;

    /*
     * After cleaning up, the unknown is released
     */
    if (pmedium->pUnkForRelease != 0)
    {
        IUnknown16_Release(pmedium->pUnkForRelease);
        pmedium->pUnkForRelease = 0;
    }
}

/***********************************************************************
 *              WriteClassStg16 (OLE2.19)
 */
HRESULT WINAPI WriteClassStg16(/*IStorage **/SEGPTR stg, REFCLSID clsid)
{
    TRACE("(%p %s)\n", stg, debugstr_guid(clsid));
    return hresult32_16(WriteClassStg((IStorage*)iface16_32(&IID_IStorage, stg), clsid));
}

HRESULT WINAPI DoDragDrop16(LPDATAOBJECT pDataObj, LPDROPSOURCE pDropSource, DWORD dwOKEffects, LPDWORD pdwEffect)
{
    TRACE("(%p,%p,%x,%p)\n", pDataObj, pDropSource, dwOKEffects, pdwEffect);
    return hresult32_16(DoDragDrop((IDataObject*)iface16_32(&IID_IDataObject, pDataObj), (IDropSource*)iface16_32(&IID_IDropSource, pDropSource), dwOKEffects, pdwEffect));
}

HRESULT WINAPI OleIsCurrentClipboard16(/*LPDATAOBJECT*/SEGPTR pDataObj)
{
    TRACE("(%p)\n", pDataObj);
    return hresult32_16(OleIsCurrentClipboard((IDataObject*)iface16_32(&IID_IDataObject, pDataObj)));
}

HRESULT WINAPI ReadFmtUserTypeStg16(/*LPSTORAGE16*/SEGPTR pstg, CLIPFORMAT *pcf, /*LPOLESTR16*/SEGPTR *lplpszUserType)
{
    LPOLESTR lpszUserType = NULL;
    HRESULT result;
    TRACE("(%p,%p,%p)\n", pstg, pcf, lplpszUserType);
    result = hresult32_16(ReadFmtUserTypeStg((IStorage*)iface16_32(&IID_IStorage, pstg), pcf, lplpszUserType ? lpszUserType : NULL));
    if (lplpszUserType)
    {
        LPOLESTR16 a = strdupWtoA(lpszUserType);
        TRACE("%s\n", a);
        *lplpszUserType = MapLS(a);
        CoTaskMemFree(lpszUserType);
    }
    if (!SUCCEEDED(result))
    {
        TRACE("failed: %08x\n", result);
    }
    return result;
}

HRESULT WINAPI WriteFmtUserTypeStg16(/*LPSTORAGE16*/SEGPTR pstg, CLIPFORMAT cf, LPOLESTR16 lpszUserType)
{
    HRESULT result;
    LPOLESTR w = strdupAtoW(lpszUserType);
    TRACE("(%p,%04x,%s)\n", pstg, cf, lpszUserType);
    result = hresult32_16(WriteFmtUserTypeStg((IStorage*)iface16_32(&IID_IStorage, pstg), cf, w));
    HeapFree(GetProcessHeap(), 0, w);
    return result;
}

HRESULT WINAPI CreateDataAdviseHolder16(/*LPDATAADVISEHOLDER16*/SEGPTR *ppDAHolder)
{
    LPDATAADVISEHOLDER pDAHolder = NULL;
    HRESULT result;
    TRACE("(%p)\n", ppDAHolder);
    result = hresult32_16(CreateDataAdviseHolder(&pDAHolder));
    *ppDAHolder = iface32_16(&IID_IDataAdviseHolder, pDAHolder);
    return result;
}
HRESULT WINAPI CreateOleAdviseHolder16(/*LPOLEADVISEHOLDER16*/SEGPTR *ppOAHolder)
{
    LPOLEADVISEHOLDER pOAHolder = NULL;
    HRESULT result;
    TRACE("(%p)\n", ppOAHolder);
    result = hresult32_16(CreateOleAdviseHolder(&pOAHolder));
    *ppOAHolder = iface32_16(&IID_IOleAdviseHolder, pOAHolder);
    return result;
}
/******************************************************************************
 *        CreateFileMoniker (OLE2.28)
 */
HRESULT WINAPI CreateFileMoniker16(LPCOLESTR16 lpszPathName, /*LPMONIKER16*/SEGPTR* ppmk)
{
    LPOLESTR w = strdupAtoW(lpszPathName);
    LPMONIKER pmk = NULL;
    HRESULT result;
    TRACE("(%s,%p)\n", debugstr_a(lpszPathName), ppmk);
    result = hresult32_16(CreateFileMoniker(w, &pmk));
    *ppmk = iface32_16(&IID_IMoniker, pmk);
    HeapFree(GetProcessHeap(), 0, w);
    return result;
}

/******************************************************************************
 *		GetRunningObjectTable (OLE2.30)
 */
HRESULT WINAPI GetRunningObjectTable16(DWORD reserved, /*LPRUNNINGOBJECTTABLE16*/SEGPTR *pprot)
{
    LPRUNNINGOBJECTTABLE prot = NULL;
    HRESULT result;
    TRACE("(%d,%p)\n", reserved, pprot);
    result = hresult32_16(GetRunningObjectTable(reserved, &prot));
    *pprot = iface32_16(&IID_IRunningObjectTable, prot);
    return result;
}

HRESULT WINAPI OleCreateEmbeddingHelper16(REFCLSID rclsid, SEGPTR pUnkOuter, DWORD flags, SEGPTR pCF, REFIID riid, SEGPTR *ppvObj)
{
    LPVOID pvObj = NULL;
    HRESULT result;
    TRACE("(%s,%08x,%08x,%08x,%s,%p)\n", debugstr_guid(rclsid), pUnkOuter, flags, pCF, debugstr_guid(riid), ppvObj);
    result = hresult32_16(OleCreateEmbeddingHelper(rclsid, (IUnknown*)iface16_32(&IID_IUnknown, pUnkOuter), flags, (IClassFactory*)iface16_32(&IID_IClassFactory, pCF), riid, &pvObj));
    *ppvObj = iface32_16(riid, pvObj);
    return result;
}

HRESULT WINAPI OleCreateDefaultHandler16(REFCLSID clsid, SEGPTR pUnkOuter, REFIID riid, SEGPTR* ppvObj)
{
    LPVOID obj = NULL;
    LPUNKNOWN a = (LPUNKNOWN)iface16_32(&IID_IUnknown, pUnkOuter);
    TRACE("(%s,%08,%s,%p)\n", debugstr_guid(clsid), pUnkOuter, debugstr_guid(riid), ppvObj);
    HRESULT hr = hresult32_16(OleCreateDefaultHandler(clsid, a, riid, &obj));
    *ppvObj = iface32_16(riid, obj);
    return hr;
}

HRESULT WINAPI OleSetContainedObject16(SEGPTR pUnk, BOOL fCon)
{
    TRACE("(%08x,%d)\n", pUnk, fCon);
    return hresult32_16(OleSetContainedObject((IUnknown*)iface16_32(&IID_IUnknown, pUnk), fCon));
}

HRESULT WINAPI OleLockRunning16(SEGPTR pUnk, BOOL fLock, BOOL fLastUnlockCloses)
{
    TRACE("(%08x,%d,%d)\n", pUnk, fLock, fLastUnlockCloses);
    return hresult32_16(OleLockRunning((IUnknown*)iface16_32(&IID_IUnknown, pUnk), fLock, fLastUnlockCloses));
}

BOOL WINAPI OleIsRunning16(SEGPTR pOleObject)
{
    TRACE("(%08x)\n", pOleObject);
    return hresult32_16(OleIsRunning((IOleObject*)iface16_32(&IID_IOleObject, pOleObject)));
}

HOLEMENU16 WINAPI OleCreateMenuDescriptor16(HMENU16 hmenu, LPOLEMENUGROUPWIDTHS width)
{
    HOLEMENU om32 = OleCreateMenuDescriptor(HMENU_32(hmenu), width);
    /* FIXME! */
    return HWND_16(om32);
}

void WINAPI OleDestroyMenuDescriptor16(HOLEMENU16 olemenu)
{
    /* FIXME! */
    OleDestroyMenuDescriptor(HWND_32(olemenu));
}

HRESULT WINAPI OleGetAutoConvert16(REFCLSID clsidOld, LPCLSID pclsidNew)
{
    HRESULT result = hresult32_16(OleGetAutoConvert(clsidOld, pclsidNew));
    if (result == REGDB_E_KEYMISSING || result == REGDB_E_CLASSNOTREG)
    {
        FIXME("\n");
        return result;
    }
    return result;
}

HRESULT WINAPI OleRun16(SEGPTR pUnk)
{
    return hresult32_16(OleRun((IUnknown*)iface16_32(&IID_IUnknown, pUnk)));
}

HRESULT WINAPI OleSave16(SEGPTR pPS, SEGPTR pStg, BOOL fSameAsLoad)
{
    return hresult32_16(OleSave((IPersistStorage*)iface16_32(&IID_IPersistStorage, pPS), (IStorage*)iface16_32(&IID_IStorage, pStg), fSameAsLoad));
}


HRESULT WINAPI OleCreate16(REFCLSID rclsid, REFIID riid, DWORD renderopt, LPFORMATETC16 pFormatetc, SEGPTR pClientSite, SEGPTR lpStg, SEGPTR *ppvObj)
{
    HRESULT result;
    FORMATETC formatetc32;
    void *pvObj = NULL;
    if (pFormatetc)
        map_formatetc16_32(&formatetc32, pFormatetc);
    TRACE("(%s,%s,%08x,%p,%08x,%08x,%p)\n", debugstr_guid(rclsid), debugstr_guid(riid), renderopt, pFormatetc, pClientSite, lpStg, ppvObj);
    result = hresult32_16(OleCreate(rclsid, riid, renderopt, pFormatetc ? &formatetc32 : NULL, (IOleClientSite*)iface16_32(&IID_IOleClientSite, pClientSite), (IStorage*)iface16_32(&IID_IStorage, lpStg), &pvObj));
    *ppvObj = iface32_16(riid, pvObj);
    return result;
}

HANDLE16 WINAPI OleDuplicateData16(HANDLE16 hSrc, CLIPFORMAT cfFormat, UINT16 uiFlags)
{
    HGLOBAL16 hDst;
    LPVOID pDst;
    DWORD size;
    TRACE("(%04x,%04x,%04x)\n", hSrc, cfFormat, uiFlags);
    if (cfFormat == CF_BITMAP)
    {
        return HBITMAP_16(OleDuplicateData(HBITMAP_32(hSrc), cfFormat, uiFlags));
    }
    if (cfFormat == CF_PALETTE)
    {
        return HPALETTE_16(OleDuplicateData(HPALETTE_32(hSrc), cfFormat, uiFlags));
    }
    if (cfFormat == CF_METAFILEPICT)
    {
        METAFILEPICT16 *pictSrc;
        METAFILEPICT16 *pictDst;
        void *pmfSrc;
        void *pmfDst;
        pictSrc = (METAFILEPICT16*)GlobalLock16(hSrc);
        hDst = GlobalAlloc16(uiFlags, sizeof(*pictSrc));
        pictDst = (METAFILEPICT16*)GlobalLock16(hDst);
        pictDst->mm = pictSrc->mm;
        pictDst->xExt = pictSrc->xExt;
        pictDst->yExt = pictSrc->yExt;
        size = GlobalSize16(pictSrc->hMF);
        pictDst->hMF = GlobalAlloc16(uiFlags /* ? */, size);
        pmfSrc = GlobalLock16(pictSrc->hMF);
        pmfDst = GlobalLock16(pictDst->hMF);
        memcpy(pmfDst, pmfSrc, size);
        GlobalUnlock16(pictSrc->hMF);
        GlobalUnlock16(pictDst->hMF);
        GlobalUnlock16(hSrc);
        GlobalUnlock16(hDst);
        return hDst;
    }
    if (uiFlags == 0)
        uiFlags = GMEM_MOVEABLE;
    size = GlobalSize16(hSrc);
    if (!size)
        return 0;
    hDst = GlobalAlloc16(uiFlags, size);
    memcpy(GlobalLock16(hDst), GlobalLock16(hSrc), size);
    GlobalUnlock16(hDst);
    GlobalUnlock16(hSrc);
    return hDst;
}

HRESULT WINAPI CreateGenericComposite16(SEGPTR pmkFirst, SEGPTR pmkRest, SEGPTR *ppmkComposite)
{
    HRESULT result;
    LPMONIKER pmkComposite = NULL;
    TRACE("(%08x,%08x,%p)\n", pmkFirst, pmkRest, ppmkComposite);
    result = hresult32_16(CreateGenericComposite((IMoniker*)iface16_32(&IID_IMoniker, pmkFirst), (IMoniker*)iface16_32(&IID_IMoniker, pmkRest), &pmkComposite));
    *ppmkComposite = iface32_16(&IID_IMoniker, pmkComposite);
    return result;
}

HRESULT WINAPI OleQueryCreateFromData16(SEGPTR pDataObject)
{
    TRACE("(%08x)\n", pDataObject);
    return hresult32_16(OleQueryCreateFromData((IDataObject*)iface16_32(&IID_IDataObject, pDataObject)));
}

HRESULT WINAPI OleTranslateAccelerator16(SEGPTR lpFrame, LPOLEINPLACEFRAMEINFO16 lpFrameInfo, LPMSG16 lpmsg16)
{
    MSG msg32;
    HRESULT result;
    IOleInPlaceFrame *lpFrame32 = (IOleInPlaceFrame*)iface16_32(&IID_IOleInPlaceFrame, lpFrameInfo);
    OLEINPLACEFRAMEINFO frameInfo32;
    void WINAPI window_message16_32(const MSG16 *msg16, MSG *msg32);
    TRACE("(%08x,%p,%p)\n", lpFrame, lpFrameInfo, lpmsg16);
    map_oleinplaceframeinfo16_32(&lpFrame32, lpFrameInfo);
    window_message16_32(lpmsg16, &msg32);
    result = OleTranslateAccelerator(lpFrame32, &frameInfo32, &msg32);
    return hresult32_16(result);
}

HRESULT WINAPI OleQueryLinkFromData16(SEGPTR pDataObject)
{
    TRACE("(%08x)\n", pDataObject);
    return hresult32_16(OleQueryLinkFromData((IDataObject*)iface16_32(&IID_IDataObject, pDataObject)));
}

HRESULT WINAPI OleRegGetMiscStatus16(REFCLSID clsid, DWORD dwAspect, DWORD *pdwStatus)
{
    TRACE("(%s,%d,%p)\n", debugstr_guid(clsid), dwAspect, pdwStatus);
    return hresult32_16(OleRegGetMiscStatus(clsid, dwAspect, pdwStatus));
}

SEGPTR WINAPI dynamic_CoGetMalloc16(MEMCTX dwMemContext, SEGPTR *lpMalloc)
{
    static HRESULT(WINAPI*pCoGetMalloc16)(MEMCTX dwMemContext, SEGPTR *lpMalloc);
    if (!pCoGetMalloc16)
    {
        HMODULE compobj = GetModuleHandleW(L"COMPOBJ.DLL16");
        if (!compobj)
        {
            compobj = LoadLibraryW(L"COMPOBJ.DLL16");
        }
        pCoGetMalloc16 = (HRESULT(WINAPI*)(MEMCTX dwMemContext, SEGPTR *lpMalloc))GetProcAddress(compobj, "CoGetMalloc16");
        return E_FAIL;
    }
    return pCoGetMalloc16(dwMemContext, lpMalloc);
}

SEGPTR CoTaskMemAlloc16(DWORD size)
{
    SEGPTR imalloc = 0;
    if (FAILED(dynamic_CoGetMalloc16(MEMCTX_TASK, &imalloc)))
        return 0;
    return IMalloc16_Alloc(imalloc, size);
}

void CoTaskMemFree16(SEGPTR ptr)
{
    SEGPTR imalloc = 0;
    if (FAILED(dynamic_CoGetMalloc16(MEMCTX_TASK, &imalloc)))
        return 0;
    IMalloc16_Free(imalloc, ptr);
}

static SEGPTR taskmem_strdupWtoA(LPCWSTR str)
{
    SEGPTR ret;
    INT len;

    if (!str) return NULL;
    len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    ret = CoTaskMemAlloc16(len * sizeof(CHAR));
    if (ret) WideCharToMultiByte(CP_ACP, 0, str, -1, MapSL(ret), len, NULL, NULL);
    return ret;
}

HRESULT WINAPI OleRegGetUserType16(REFCLSID clsid, DWORD dwFormOfType, SEGPTR *pszUserType)
{
    LPOLESTR szUserType;
    HRESULT result;
    TRACE("(%s,%d,%p)\n", debugstr_guid(clsid), dwFormOfType, pszUserType);
    result = OleRegGetUserType(clsid, dwFormOfType, &szUserType);
    *pszUserType = taskmem_strdupWtoA(szUserType);
    CoTaskMemFree(szUserType);
    return hresult32_16(result);
}

HRESULT WINAPI OleCreateFromData16(SEGPTR pDataObject, REFIID riid, DWORD renderopt, FORMATETC16 *pFormatetc, SEGPTR pClientSite, SEGPTR pStg, SEGPTR *ppvObj)
{
    HRESULT result;
    IDataObject *pDataObject32 = (IDataObject*)iface16_32(&IID_IDataObject, pDataObject);
    FORMATETC fmt32;
    IOleClientSite *pClientSite32 = (IOleClientSite*)iface16_32(&IID_IOleClientSite, pClientSite);
    IStorage *pStg32 = (IStorage*)iface16_32(&IID_IStorage, pStg);
    LPVOID pvObj = NULL;
    if (pFormatetc)
        map_formatetc16_32(&fmt32, pFormatetc);
    TRACE("(%08x,%p,%d,%p,%08x,%08x,%p)\n", pDataObject, debugstr_guid(riid), renderopt, pFormatetc, pClientSite, pStg, ppvObj);
    result = OleCreateFromData(pDataObject32, riid, renderopt, pFormatetc ? &fmt32 : NULL, pClientSite32, pStg32, &pvObj);
    *ppvObj = iface32_16(riid, pvObj);
    return hresult32_16(result);
}

HRESULT WINAPI OleCreateStaticFromData16(SEGPTR pDataObject, REFIID riid, DWORD renderopt, FORMATETC16 *pFormatetc, SEGPTR pClientSite, SEGPTR pStg, SEGPTR *ppvObj)
{
    HRESULT result;
    IDataObject *pDataObject32 = (IDataObject*)iface16_32(&IID_IDataObject, pDataObject);
    FORMATETC fmt32;
    IOleClientSite *pClientSite32 = (IOleClientSite*)iface16_32(&IID_IOleClientSite, pClientSite);
    IStorage *pStg32 = (IStorage*)iface16_32(&IID_IStorage, pStg);
    LPVOID pvObj = NULL;
    if (pFormatetc)
        map_formatetc16_32(&fmt32, pFormatetc);
    TRACE("(%08x,%p,%d,%p,%08x,%08x,%p)\n", pDataObject, debugstr_guid(riid), renderopt, pFormatetc, pClientSite, pStg, ppvObj);
    result = OleCreateStaticFromData(pDataObject32, riid, renderopt, pFormatetc ? &fmt32 : NULL, pClientSite32, pStg32, &pvObj);
    *ppvObj = iface32_16(riid, pvObj);
    return hresult32_16(result);
}

HRESULT WINAPI CreateBindCtx16(DWORD dwReserved, SEGPTR *ppbc)
{
    IBindCtx *pbc = NULL;
    HRESULT result;
    if (dwReserved)
    {
        ERR("dwReserved must be zero.(%d)\n", dwReserved);
    }
    TRACE("(%d,%p)\n", dwReserved, ppbc);
    result = CreateBindCtx(0, &pbc);
    *ppbc = iface32_16(&IID_IBindCtx, pbc);
    return hresult32_16(result);
}

HRESULT WINAPI OleNoteObjectVisible16(SEGPTR pUnk, BOOL fVisible)
{
    TRACE("(%08x,%d)\n", pUnk, fVisible);
    return hresult32_16(OleNoteObjectVisible((IUnknown*)iface16_32(&IID_IUnknown, pUnk), fVisible));
}

HRESULT WINAPI OleRegEnumVerbs16(REFCLSID clsid, SEGPTR *ppenum)
{
    IEnumOLEVERB *penum = NULL;
    HRESULT result;
    TRACE("(%s,%p)\n", debugstr_guid(clsid), ppenum);
    result = OleRegEnumVerbs(clsid, &penum);
    *ppenum = iface32_16(&IID_IEnumOLEVERB, penum);
    return hresult32_16(result);
}

HRESULT WINAPI OleDraw16(SEGPTR pUnk, DWORD dwAspect, HDC16 hdcDraw, const RECT16 const *lpcBounds)
{
    HRESULT result;
    RECT rect32;
    IUnknown *pUnk32;
    if (lpcBounds)
    {
        TRACE("(%08x,%d,%04x,%p{%d,%d,%d,%d})\n", pUnk, dwAspect, hdcDraw, lpcBounds, lpcBounds->left, lpcBounds->top, lpcBounds->right, lpcBounds->bottom);
    }
    else
    {
        TRACE("(%08x,%d,%04x,%p)\n", pUnk, dwAspect, hdcDraw, lpcBounds);
    }
    pUnk32 = (IUnknown*)iface16_32(&IID_IUnknown, pUnk);
    if (lpcBounds)
    {
        rect32.left = lpcBounds->left;
        rect32.right = lpcBounds->right;
        rect32.bottom = lpcBounds->bottom;
        rect32.top = lpcBounds->top;
    }
    result = OleDraw(pUnk32, dwAspect, HDC_32(hdcDraw), lpcBounds ? &rect32 : NULL);
    return hresult32_16(result);
}

HRESULT WINAPI OleLoadFromStream16(SEGPTR pStm, REFIID riid, SEGPTR *ppvObj)
{
    IStream *pStm32;
    void *pvObj = NULL;
    HRESULT result;
    TRACE("(%08x,%s,%p)\n", pStm, debugstr_guid(riid), ppvObj);
    pStm32 = (IStream*)iface16_32(&IID_IStream, pStm);
    result = OleLoadFromStream(pStm32, riid, &pvObj);
    *ppvObj = iface32_16(riid, pvObj);
    return hresult32_16(result);
}

HRESULT WINAPI MkParseDisplayName16(SEGPTR pbc, LPSTR szUserName, ULONG *pchEaten, SEGPTR *ppmk)
{
    HRESULT result;
    IBindCtx *pbc32;
    LPOLESTR szUserName32;
    IMoniker *pmk = NULL;
    TRACE("(%08x,%s,%p,%p)\n", pbc, debugstr_a(szUserName), pchEaten, ppmk);
    szUserName32 = strdupAtoW(szUserName);
    pbc32 = (IBindCtx*)iface16_32(&IID_IBindCtx, pbc);
    result = MkParseDisplayName(pbc32, szUserName32, pchEaten, &pmk);
    HeapFree(GetProcessHeap(), 0, szUserName32);
    *ppmk = iface32_16(&IID_IMoniker, pmk);
    return hresult32_16(result);
}

HRESULT WINAPI BindMoniker16(SEGPTR pmk, DWORD grfOpt, REFIID riid, SEGPTR *ppvObj)
{
    HRESULT result;
    IMoniker *pmk32;
    LPVOID pvObj = NULL;
    TRACE("(%08x,%d,%s,%p)\n", pmk, grfOpt, debugstr_guid(riid), ppvObj);
    pmk32 = (IMoniker*)iface16_32(&IID_IMoniker, pmk);
    result = BindMoniker(pmk32, grfOpt, riid, &pvObj);
    *ppvObj = iface32_16(riid, pvObj);
    return hresult32_16(result);
}
