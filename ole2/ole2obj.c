
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
