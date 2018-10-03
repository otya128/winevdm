
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

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


typedef struct
{
    IDataAdviseHolder16 iface;
    LONG ref;
    LPDATAADVISEHOLDER iface32;
} IDataAdviseHolderImpl16;

static IDataAdviseHolderImpl16* IDataAdviseHolder16_Construct(LPDATAADVISEHOLDER iface32)
{
    IDataAdviseHolderImpl16* impl;
    static IDataAdviseHolder16Vtbl vt16;
    static SEGPTR msegvt16;
    HMODULE16 hmod = GetModuleHandle16("OLE2");
    TRACE("\n");
    impl = HeapAlloc(GetProcessHeap(), 0, sizeof(IDataAdviseHolderImpl16));
    if (impl == NULL)
        return NULL;
    if (!msegvt16)
    {
#define VTENT(x) vt16.x = (void*)GetProcAddress16(hmod,"IDataAdviseHolder16_"#x);assert(vt16.x)
        VTENT(QueryInterface);
        VTENT(AddRef);
        VTENT(Release);
        VTENT(Advise);
        VTENT(Unadvise);
        VTENT(SendOnDataChange);
#undef VTENT
        msegvt16 = MapLS(&vt16);
    }
    impl->iface.lpVtbl = (IDataAdviseHolder16Vtbl*)msegvt16;
    impl->ref = 0;
    impl->iface32 = iface32;
    return (IDataAdviseHolderImpl16*)MapLS(impl);
}
static inline IDataAdviseHolderImpl16 *impl_from_IDataAdviseHolderImpl16(IDataAdviseHolder16 *iface)
{
    return CONTAINING_RECORD(iface, IDataAdviseHolderImpl16, iface);
}
DWORD CDECL IDataAdviseHolder16_AddRef(IDataAdviseHolder16  *iface)
{
    IDataAdviseHolderImpl16* const This = impl_from_IDataAdviseHolderImpl16(iface);
    TRACE("(%p)\n", This);
    This->iface32->lpVtbl->AddRef(This->iface32);
    return InterlockedIncrement(&This->ref);
}
DWORD CDECL IDataAdviseHolder16_QueryInterface(IDataAdviseHolder16* iface, REFIID riid, void **ppvObject)
{
    IDataAdviseHolderImpl16* const This = MapSL((SEGPTR)iface);
    TRACE("(%p,%s,%p)\n", iface, debugstr_guid(riid), ppvObject);
    if (ppvObject == 0)
        return E_INVALIDARG;
    *ppvObject = 0;
    if (!memcmp(&IID_IUnknown, riid, sizeof(IID_IUnknown)) ||
        !memcmp(&IID_IDataAdviseHolder, riid, sizeof(IID_IDataAdviseHolder))
        )
        *ppvObject = (void*)iface;
    if ((*ppvObject) == 0) {
        FIXME("Unknown IID %s\n", debugstr_guid(riid));
        return E_NOINTERFACE;
    }
    IDataAdviseHolder16_AddRef(&This->iface);
    return S_OK;
}
DWORD CDECL IDataAdviseHolder16_Release(IDataAdviseHolder16  *iface)
{
    IDataAdviseHolderImpl16* const This = impl_from_IDataAdviseHolderImpl16(iface);
    ULONG ref;
    TRACE("(%p)\n", This);
    ref = InterlockedDecrement(&This->ref);
    This->iface32->lpVtbl->Release(This->iface32);
    if (ref == 0)
    {
        TRACE("Destroy (%p)\n", This);
        HeapFree(GetProcessHeap(), 0, This);
    }
    return ref;
}
DWORD CDECL IDataAdviseHolder16_Advise(IDataAdviseHolder16 *iface, LPDATAOBJECT pDataObject, FORMATETC * pFetc, DWORD advf, LPADVISESINK pAdvise, DWORD *pdwConnection)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IDataAdviseHolder16_Unadvise(IDataAdviseHolder16  *iface, DWORD dwConnection)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IDataAdviseHolder16_EnumAdvise(IDataAdviseHolder16  *iface, LPENUMSTATDATA *ppenumAdvise)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IDataAdviseHolder16_SendOnDataChange(IDataAdviseHolder16  *iface, LPDATAOBJECT pDataObject, DWORD dwReserved, DWORD advf)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT WINAPI CreateDataAdviseHolder16(LPDATAADVISEHOLDER16 *ppDAHolder)
{
    LPDATAADVISEHOLDER iface32;
    CreateDataAdviseHolder(&iface32);
    *ppDAHolder = (LPDATAADVISEHOLDER16)IDataAdviseHolder16_Construct(iface32);
    return S_OK;
}

typedef struct
{
    IOleAdviseHolder16 iface;
    LONG ref;
    LPOLEADVISEHOLDER iface32;
} IOleAdviseHolderImpl16;
static inline IOleAdviseHolderImpl16 *impl_from_IOleAdviseHolderImpl16(IOleAdviseHolder16 *iface)
{
    return CONTAINING_RECORD(iface, IOleAdviseHolderImpl16, iface);
}
DWORD CDECL IOleAdviseHolder16_AddRef(IOleAdviseHolder16 *iface)
{
    IOleAdviseHolderImpl16* const This = impl_from_IOleAdviseHolderImpl16(iface);
    TRACE("(%p)\n", This);
    This->iface32->lpVtbl->AddRef(This->iface32);
    return InterlockedIncrement(&This->ref);
}
DWORD CDECL IOleAdviseHolder16_QueryInterface(SEGPTR iface, REFIID riid, void **ppvObject)
{
    IOleAdviseHolderImpl16* const This = MapSL((SEGPTR)iface);
    TRACE("(%p,%s,%p)\n", iface, debugstr_guid(riid), ppvObject);
    if (ppvObject == 0)
        return E_INVALIDARG;
    *ppvObject = 0;
    if (!memcmp(&IID_IUnknown, riid, sizeof(IID_IUnknown)) ||
        !memcmp(&IID_IOleAdviseHolder, riid, sizeof(IID_IOleAdviseHolder))
        )
        *ppvObject = (void*)iface;
    if ((*ppvObject) == 0) {
        FIXME("Unknown IID %s\n", debugstr_guid(riid));
        return E_NOINTERFACE;
    }
    IOleAdviseHolder16_AddRef(&This->iface);
    return S_OK;
}
DWORD CDECL IOleAdviseHolder16_Release(IOleAdviseHolder16 *iface)
{
    IOleAdviseHolderImpl16* const This = impl_from_IOleAdviseHolderImpl16(iface);
    ULONG ref;
    TRACE("(%p)\n", This);
    ref = InterlockedDecrement(&This->ref);
    This->iface32->lpVtbl->Release(This->iface32);
    if (ref == 0)
    {
        TRACE("Destroy (%p)\n", This);
        HeapFree(GetProcessHeap(), 0, This);
    }
    return ref;
}
DWORD CDECL IOleAdviseHolder16_Advise(IOleAdviseHolder16 *iface, LPADVISESINK pAdvise, DWORD *pdwConnection)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IOleAdviseHolder16_Unadvise(IOleAdviseHolder16 *iface, DWORD dwConnection)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IOleAdviseHolder16_EnumAdvise(IOleAdviseHolder16 *iface, LPENUMSTATDATA *ppenumAdvise)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IOleAdviseHolder16_SendOnRename(IOleAdviseHolder16 *iface, LPMONIKER pmk)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IOleAdviseHolder16_SendOnSave(IOleAdviseHolder16 *iface)
{
    FIXME("\n");
    return E_NOTIMPL;
}
DWORD CDECL IOleAdviseHolder16_SendOnClose(IOleAdviseHolder16 *iface)
{
    FIXME("\n");
    return E_NOTIMPL;
}
static IOleAdviseHolderImpl16* IOleAdviseHolder16_Construct(LPOLEADVISEHOLDER iface32)
{
    IOleAdviseHolderImpl16* impl;
    static IOleAdviseHolder16Vtbl vt16;
    static SEGPTR msegvt16;
    HMODULE16 hmod = GetModuleHandle16("OLE2");
    TRACE("\n");
    impl = HeapAlloc(GetProcessHeap(), 0, sizeof(IOleAdviseHolderImpl16));
    if (impl == NULL)
        return NULL;
    /*
     * Set up the virtual function table and reference count.
     */
    if (!msegvt16)
    {
#define VTENT(x) vt16.x = (void*)GetProcAddress16(hmod,"IOleAdviseHolder16_"#x);assert(vt16.x)
        VTENT(QueryInterface);
        VTENT(AddRef);
        VTENT(Release);
        VTENT(Advise);
        VTENT(Unadvise);
        VTENT(SendOnRename);
        VTENT(SendOnSave);
        VTENT(SendOnClose);
#undef VTENT
        msegvt16 = MapLS(&vt16);
    }
    impl->iface.lpVtbl = (IOleAdviseHolder16Vtbl*)msegvt16;
    impl->ref = 0;
    impl->iface32 = iface32;
    return (IOleAdviseHolderImpl16*)MapLS(impl);
}
HRESULT WINAPI CreateOleAdviseHolder16(LPOLEADVISEHOLDER16 *ppOAHolder)
{
    LPOLEADVISEHOLDER iface32;
    CreateOleAdviseHolder(&iface32);
    *ppOAHolder = (LPOLEADVISEHOLDER16)IOleAdviseHolder16_Construct(iface32);
    return S_OK;
}
typedef struct
{
    IRunningObjectTable16 iface;
    LONG ref;
    LPRUNNINGOBJECTTABLE iface32;
} IRunningObjectTableImpl16;

static inline IRunningObjectTableImpl16 *impl_from_IRunningObjectTable16(IRunningObjectTable16 *iface)
{
    return CONTAINING_RECORD(iface, IRunningObjectTableImpl16, iface);
}
static IRunningObjectTableImpl16* IRunningObjectTable16_Construct(LPRUNNINGOBJECTTABLE iface32)
{
    IRunningObjectTableImpl16* impl;
    static IRunningObjectTable16Vtbl vt16;
    static SEGPTR msegvt16;
    HMODULE16 hmod = GetModuleHandle16("OLE2");
    TRACE("\n");
    impl = HeapAlloc(GetProcessHeap(), 0, sizeof(IRunningObjectTableImpl16));
    if (impl == NULL)
        return NULL;
    /*
     * Set up the virtual function table and reference count.
     */
    if (!msegvt16)
    {
#define VTENT(x) vt16.x = (SEGPTR)GetProcAddress16(hmod,"IRunningObjectTable16_"#x);assert(vt16.x)
        VTENT(QueryInterface);
        VTENT(AddRef);
        VTENT(Release);
        VTENT(Register);
        VTENT(Revoke);
        VTENT(IsRunning);
        VTENT(GetObject);
        VTENT(NoteChangeTime);
        VTENT(GetTimeOfLastChange);
        VTENT(EnumRunning);
#undef VTENT
        msegvt16 = MapLS(&vt16);
    }
    impl->iface.lpVtbl = msegvt16;
    impl->ref = 0;
    impl->iface32 = iface32;
    return (IRunningObjectTableImpl16*)MapLS(impl);
}
ULONG CDECL IRunningObjectTable16_AddRef(IRunningObjectTable16 *iface);
HRESULT CDECL IRunningObjectTable16_QueryInterface(SEGPTR iface, REFIID riid, void **ppvObject)
{
    IRunningObjectTableImpl16* const This = impl_from_IRunningObjectTable16((IRunningObjectTable16*)(MapSL((SEGPTR)iface)));
    TRACE("(%p,%s,%p)\n", iface, debugstr_guid(riid), ppvObject);
    if (ppvObject == 0)
        return E_INVALIDARG;
    *ppvObject = 0;
    if (!memcmp(&IID_IUnknown, riid, sizeof(IID_IUnknown)) ||
        !memcmp(&IID_IRunningObjectTable, riid, sizeof(IID_IRunningObjectTable))
        )
        *ppvObject = (void*)iface;
    if ((*ppvObject) == 0) {
        FIXME("Unknown IID %s\n", debugstr_guid(riid));
        return E_NOINTERFACE;
    }
    IRunningObjectTable16_AddRef(&This->iface);
    return S_OK;
}
ULONG CDECL IRunningObjectTable16_AddRef(IRunningObjectTable16 *iface)
{
    FIXME("\n");
    IRunningObjectTableImpl16 *impl = impl_from_IRunningObjectTable16(iface);
    return impl->iface32->lpVtbl->AddRef(impl->iface32);
}
ULONG CDECL IRunningObjectTable16_Release(IRunningObjectTable16 *iface)
{
    FIXME("\n");
    IRunningObjectTableImpl16 *impl = impl_from_IRunningObjectTable16(iface);
    ULONG result = impl->iface32->lpVtbl->Release(impl->iface32);
    if (result == 0)
    {
        TRACE("free %p\n", iface);
        HeapFree(GetProcessHeap(), 0, impl);
    }
    return result;
}
HRESULT CDECL IRunningObjectTable16_Register(IRunningObjectTable16 *iface, DWORD flags, LPUNKNOWN16 punkObject, LPMONIKER16 pmkObjectName, DWORD *pdwRegister)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_Revoke(IRunningObjectTable16 *iface, DWORD dwRegister)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_IsRunning(IRunningObjectTable16 *iface, LPMONIKER16 pmkObjectName)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_GetObject(IRunningObjectTable16 *iface, LPMONIKER16 pmkObjectName, LPUNKNOWN16 FAR* ppunkObject)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_NoteChangeTime(IRunningObjectTable16 *iface, DWORD dwRegister, FILETIME FAR * pfiletime)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_GetTimeOfLastChange(IRunningObjectTable16 *iface, LPMONIKER16 pmkObjectName, FILETIME FAR * pfiletime)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IRunningObjectTable16_EnumRunning(IRunningObjectTable16 *iface, LPENUMMONIKER16 *ppenumMoniker)
{
    FIXME("\n");
    return E_NOTIMPL;
}

/******************************************************************************
 *		GetRunningObjectTable (OLE2.30)
 */
HRESULT WINAPI GetRunningObjectTable16(DWORD reserved, LPRUNNINGOBJECTTABLE16 *pprot)
{
    LPRUNNINGOBJECTTABLE prot;

    TRACE("(%d,%p)\n", reserved, pprot);
    HRESULT result = GetRunningObjectTable(0, &prot);
    *pprot = 0;
    if (!SUCCEEDED(result))
    {
        return result;
    }
    *pprot = &IRunningObjectTable16_Construct(prot)->iface;
    return result;
}

typedef struct
{
    IMoniker16 iface;
    LONG ref;
    LPMONIKER iface32;
} IMonikerImpl16;

static inline IMonikerImpl16 *impl_from_IMoniker16(IMoniker16 *iface)
{
    return CONTAINING_RECORD(iface, IMonikerImpl16, iface);
}
static IMonikerImpl16* IMoniker16_Construct(LPMONIKER iface32)
{
    IMonikerImpl16* impl;
    static IMoniker16Vtbl vt16;
    static SEGPTR msegvt16;
    HMODULE16 hmod = GetModuleHandle16("OLE2");
    TRACE("\n");
    impl = HeapAlloc(GetProcessHeap(), 0, sizeof(IMonikerImpl16));
    if (impl == NULL)
        return NULL;
    /*
     * Set up the virtual function table and reference count.
     */
    if (!msegvt16)
    {
#define VTENT(x) vt16.x = (SEGPTR)GetProcAddress16(hmod,"IMoniker16_"#x);assert(vt16.x)
        VTENT(QueryInterface);
        VTENT(AddRef);
        VTENT(Release);
        VTENT(GetClassID);
        VTENT(IsDirty);
        VTENT(Load);
        VTENT(Save);
        VTENT(GetSizeMax);
        VTENT(BindToObject);
        VTENT(BindToStorage);
        VTENT(Reduce);
        VTENT(ComposeWith);
        VTENT(Enum);
        VTENT(IsEqual);
        VTENT(Hash);
        VTENT(IsRunning);
        VTENT(GetTimeOfLastChange);
        VTENT(Inverse);
        VTENT(CommonPrefixWith);
        VTENT(RelativePathTo);
        VTENT(GetDisplayName);
        VTENT(ParseDisplayName);
        VTENT(IsSystemMoniker);
#undef VTENT
        msegvt16 = MapLS(&vt16);
    }
    impl->iface.lpVtbl = msegvt16;
    impl->ref = 0;
    impl->iface32 = iface32;
    return (IMonikerImpl16*)MapLS(impl);
}
ULONG CDECL IMoniker16_AddRef(IMoniker16 *iface);

HRESULT CDECL IMoniker16_QueryInterface(SEGPTR iface, REFIID riid, SEGPTR *ppvObject)
{
    IMonikerImpl16* const This = impl_from_IMoniker16((IMoniker16*)MapSL(iface));
    TRACE("(%08x,%s,%p)\n", iface, debugstr_guid(riid), ppvObject);
    if (ppvObject == 0)
        return E_INVALIDARG;
    *ppvObject = 0;
    if (!memcmp(&IID_IUnknown, riid, sizeof(IID_IUnknown)) ||
        !memcmp(&IID_IPersist, riid, sizeof(IID_IPersist)) ||
        !memcmp(&IID_IPersistStream, riid, sizeof(IID_IPersistStream)) ||
        !memcmp(&IID_IMoniker, riid, sizeof(IID_IMoniker))
        )
        *ppvObject = iface;
    if ((*ppvObject) == 0) {
        FIXME("Unknown IID %s\n", debugstr_guid(riid));
        return E_NOINTERFACE;
    }
    IMoniker16_AddRef(&This->iface);
    return S_OK;
}

ULONG CDECL IMoniker16_AddRef(IMoniker16 *iface)
{
    IMonikerImpl16 *impl = impl_from_IMoniker16(iface);
    TRACE("(%p)\n", iface);
    ULONG result = impl->iface32->lpVtbl->AddRef(impl->iface32);
    return result;
}

ULONG CDECL IMoniker16_Release(IMoniker16 *iface)
{
    IMonikerImpl16 *impl = impl_from_IMoniker16(iface);
    TRACE("(%p)\n", iface);
    ULONG result = impl->iface32->lpVtbl->Release(impl->iface32);
    if (result == 0)
    {
        TRACE("free %p\n", iface);
        HeapFree(GetProcessHeap(), 0, impl);
    }
    return result;
}

HRESULT CDECL IMoniker16_GetClassID(IMoniker16 *iface, LPCLSID lpClassID)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_IsDirty(IMoniker16 *iface)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Load(IMoniker16 *iface, LPSTREAM16 pStm)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Save(IMoniker16 *iface, LPSTREAM16 pStm, BOOL fClearDirty)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_GetSizeMax(IMoniker16 *iface, ULARGE_INTEGER *pcbSize)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_BindToObject(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, REFIID riidResult, LPVOID *ppvResult)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_BindToStorage(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, REFIID riid, LPVOID *ppvObj)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Reduce(IMoniker16 *iface, LPBC16 pbc, DWORD dwReduceHowFar, LPMONIKER16 *ppmkToLeft, LPMONIKER16 *ppmkReduced)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_ComposeWith(IMoniker16 *iface, LPMONIKER16 pmkRight, BOOL fOnlyIfNotGeneric, LPMONIKER16 *ppmkComposite)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Enum(IMoniker16 *iface, BOOL fForward, LPENUMMONIKER16 *ppenumMoniker)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_IsEqual(IMoniker16 *iface, LPMONIKER16 pmkOtherMoniker)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Hash(IMoniker16 *iface, LPDWORD pdwHash)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_IsRunning(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, LPMONIKER16 pmkNewlyRunning)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_GetTimeOfLastChange(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, FILETIME *pfiletime)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_Inverse(IMoniker16 *iface, LPMONIKER16 *ppmk)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_CommonPrefixWith(IMoniker16 *iface, LPMONIKER16 pmkOther, LPMONIKER16 *ppmkPrefix)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_RelativePathTo(IMoniker16 *iface, LPMONIKER16 pmkOther, LPMONIKER16 *ppmkRelPath)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_GetDisplayName(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, LPSTR *lplpszDisplayName)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_ParseDisplayName(IMoniker16 *iface, LPBC16 pbc, LPMONIKER16 pmkToLeft, LPSTR lpszDisplayName, ULONG *pchEaten, LPMONIKER16 *ppmkOut)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT CDECL IMoniker16_IsSystemMoniker(IMoniker16 *iface, LPDWORD pdwMksys)
{
    FIXME("\n");
    return E_NOTIMPL;
}


/******************************************************************************
 *        CreateFileMoniker (OLE2.28)
 */
HRESULT WINAPI CreateFileMoniker16(LPCOLESTR16 lpszPathName, LPMONIKER16* ppmk)
{
    TRACE("(%s,%p)\n", lpszPathName, ppmk);
    LPMONIKER pmk = NULL;
    SIZE_T len = MultiByteToWideChar(CP_ACP, 0, lpszPathName, -1, NULL, 0);
    LPCOLESTR name = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, lpszPathName, -1, name, len);
    HRESULT result = CreateFileMoniker(name, &pmk);
    HeapFree(GetProcessHeap(), 0, name);
    if (!SUCCEEDED(result))
    {
        return result;
    }
    *ppmk = &IMoniker16_Construct(pmk)->iface;
    return result;
}
