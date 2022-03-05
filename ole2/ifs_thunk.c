
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stddef.h>

#define COBJMACROS
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
#include "ifs_thunk.h"

extern interface_entry interfaces[];
extern size_t interfaces_count;
/* FIXME */
interface_32 *interface32_instances[1024];
size_t interface32_instance_size = 1024;
size_t interface32_instance_cur = 0;
interface_16 *interface16_instances[1024];
size_t interface16_instance_size = 1024;
size_t interface16_instance_cur = 0;

#ifdef _DEBUG
#define IFS_GUARD_SIZE 500
#else
#define IFS_GUARD_SIZE 0
#endif
static int iid_cmp(const void *p1, const void *p2)
{
    return memcmp(&((const interface_entry*)p1)->iid, &((const interface_entry*)p2)->iid, sizeof(IID));
}
SEGPTR make_thunk_32(void *funcptr, const char *arguments, const char *name, BOOL ret_32bit, BOOL reg_func, BOOL is_cdecl);
static void register_instance32(interface_32 *i32)
{
    if (interface32_instance_cur >= interface32_instance_size)
        return;
    interface32_instances[interface32_instance_cur++] = i32;
}
static void register_instance16(interface_16 *i16)
{
    if (interface16_instance_cur >= interface16_instance_size)
        return;
    interface16_instances[interface16_instance_cur++] = i16;
}
static void unregister_instance32(interface_32 *i32)
{
    for (int i = 0; i < interface32_instance_size; i++)
        if (interface32_instances[i] == i32)
            interface32_instances[i] = NULL;
}
static void init_interface_entry(interface_entry *e)
{
    size_t i = 0;
    SEGPTR *vtbl16 = e->lpVtbl16;
    while (e->vtbl16[i].func16)
    {
        vtbl16[i] = make_thunk_32(e->vtbl16[i].func16, e->vtbl16[i].args, e->vtbl16[i].name, TRUE, FALSE, !e->vtbl16[i].is_stdcall);
        i++;
    }
    e->spVtbl16 = MapLS(e->lpVtbl16);
}
SEGPTR iface32_16(REFIID riid, void *iface32)
{
    interface_entry *result;
    size_t i;
    interface_16 *i16;
    SEGPTR s;
    BOOL is_iunk;
    if (!iface32)
    {
        return 0;
    }
    is_iunk = IsEqualGUID(&IID_IUnknown, riid); /* FIXME */
    result = (interface_entry*)bsearch(riid, interfaces, interfaces_count, sizeof(interfaces[0]), iid_cmp);
    for (i = 0; i < interface16_instance_size; i++)
    {
        if (interface32_instances[i] && &interface32_instances[i]->lpVtbl == iface32)
        {
            s = interface32_instances[i]->iface16;
            if (is_iunk || !memcmp(interface32_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("32-bit interface %p -> %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
                return s;
            }
            else
            {
                TRACE("32-bit interface %p is not %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
        if (interface16_instances[i] && interface16_instances[i]->iface32 == iface32)
        {
            s = MapLS(&interface16_instances[i]->lpVtbl);
            if (is_iunk || !memcmp(interface16_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("32-bit interface %p -> %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
                return s;
            }
            else
            {
                TRACE("32-bit interface %p is not %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
    }
    if (!result)
    {
        ERR("unknown interface %s\n", debugstr_guid(riid));
        return 0;
    }
    i16 = (interface_16*)HeapAlloc(GetProcessHeap(), 0, sizeof(interface_16) + IFS_GUARD_SIZE * 2);
    memset(i16, 0xcd, sizeof(interface_16) + IFS_GUARD_SIZE * 2);
    i16 = (interface_16*)((char*)i16 + IFS_GUARD_SIZE);
    if (!result->spVtbl16)
    {
        init_interface_entry(result);
    }
    s = MapLS(&i16->lpVtbl);
    TRACE("32-bit interface %p -> new %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
    i16->iface32 = iface32;
    i16->lpVtbl = result->spVtbl16;
    i16->riid = &result->iid;
    i16->vtable_copy = NULL;
    register_instance16(i16);
    return s;
}
void *copy_iface16_vtbl(SEGPTR iface16)
{
    interface_16 *i16 = get_interface32_ptr(iface16);
    interface_entry *result;
    vtbl_entry *cur_entry;
    SEGPTR *vtbl;
    int len_vtbl = 0;
    result = (interface_entry*)bsearch(i16->riid, interfaces, interfaces_count, sizeof(interfaces[0]), iid_cmp);
    if (!result)
        return NULL;
    cur_entry = result->vtbl16;
    while (cur_entry++->func16)
        len_vtbl++;
    vtbl = (SEGPTR*)HeapAlloc(GetProcessHeap(), 0, len_vtbl * sizeof(SEGPTR));
    memcpy(vtbl, result->lpVtbl16, len_vtbl * sizeof(SEGPTR));
    i16->lpVtbl = MapLS(vtbl);
    return vtbl;
}
void *iface16_32(REFIID riid, SEGPTR iface16)
{
    interface_entry *result;
    size_t i;
    interface_32 *i32;
    LPVOID piface16 = MapSL(iface16);
    BOOL is_iunk;
    if (!iface16)
    {
        return 0;
    }
    is_iunk = IsEqualGUID(&IID_IUnknown, riid); /* FIXME */
    result = (interface_entry*)bsearch(riid, interfaces, interfaces_count, sizeof(interfaces[0]), iid_cmp);
    for (i = 0; i < interface32_instance_size; i++)
    {
        if (interface16_instances[i] && (LPVOID)&interface16_instances[i]->lpVtbl == piface16)
        {
            if (is_iunk || !memcmp(interface16_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("16-bit interface %04x:%04x -> %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), interface16_instances[i]->iface32, (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
                return interface16_instances[i]->iface32;
            }
            else
            {
                TRACE("16-bit interface %04x:%04x is not %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), interface16_instances[i]->iface32, (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
        if (interface32_instances[i] && interface32_instances[i]->iface16 == iface16)
        {
            if (is_iunk ||!memcmp(interface32_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("16-bit interface %04x:%04x -> %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), (void*)&interface32_instances[i]->lpVtbl, (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
                return (void*)&interface32_instances[i]->lpVtbl;
            }
            else
            {
                TRACE("16-bit interface %04x:%04x is not %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), (void*)&interface32_instances[i]->lpVtbl, (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
    }
    if (!result)
    {
        ERR("unknown interface %s\n", debugstr_guid(riid));
        return 0;
    }
    i32 = (interface_32*)HeapAlloc(GetProcessHeap(), 0, sizeof(interface_32) + IFS_GUARD_SIZE * 2);
    memset(i32, 0xcd, sizeof(interface_32) + IFS_GUARD_SIZE * 2);
    i32 = (interface_32*)((char*)i32 + IFS_GUARD_SIZE);
    if (!result->spVtbl16)
    {
        init_interface_entry(result);
    }
    TRACE("16-bit interface %04x:%04x -> new %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), i32, (const char*)strstr(result->vtbl16[0].name, "::") - (const char*)result->vtbl16[0].name, result->vtbl16[0].name);
    i32->iface16 = iface16;
    i32->lpVtbl = result->lpVtbl32;
    i32->riid = &result->iid;
    register_instance32(i32);
    return (void*)&i32->lpVtbl;
}

void free_iface32(void *iface)
{
    char *i32 = (char*)iface - IFS_GUARD_SIZE;
    if (!iface)
        return;
    unregister_instance32(iface);
    HeapFree(GetProcessHeap(), 0, i32);
}
HRESULT CDECL IOleInPlaceSiteWindowless_16_32_OnDefWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_32_16_OnDefWindowMessage(IOleInPlaceSiteWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IOleInPlaceObjectWindowless_32_16_OnWindowMessage(IOleInPlaceObjectWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IOleInPlaceObjectWindowless_16_32_OnWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT CDECL ISimpleFrameSite_16_32_PreMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, SEGPTR args16_pdwCookie)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PreMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD *pdwCookie)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL ISimpleFrameSite_16_32_PostMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, DWORD args16_dwCookie)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PostMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD dwCookie)
{
    FIXME("\n");
    return E_NOTIMPL;
}

struct hresult_map
{
    HRESULT hresult16;
    HRESULT hresult32;
} hresult_table[] =
{
    { E_UNEXPECTED16, E_UNEXPECTED },
    { E_NOTIMPL16, E_NOTIMPL },
    { E_OUTOFMEMORY16, E_OUTOFMEMORY },
    { E_INVALIDARG16, E_INVALIDARG },
    { E_NOINTERFACE16, E_NOINTERFACE },
    { E_POINTER16, E_POINTER },
    { E_HANDLE16, E_HANDLE },
    { E_ABORT16, E_ABORT },
    { E_FAIL16, E_FAIL },
    { E_ACCESSDENIED16, E_ACCESSDENIED },
};
HRESULT hresult32_16(HRESULT hresult)
{
    int i;
    for (i = 0; i < ARRAYSIZE(hresult_table); i++)
    {
        if (hresult_table[i].hresult32 == hresult)
        {
            TRACE("%08x->%08x\n", hresult, hresult_table[i].hresult16);
            return hresult_table[i].hresult16;
        }
    }
    TRACE("%08x\n", hresult);
    return hresult;
}

HRESULT hresult16_32(HRESULT hresult)
{
    int i;
    for (i = 0; i < ARRAYSIZE(hresult_table); i++)
    {
        if (hresult_table[i].hresult16 == hresult)
        {
            TRACE("%08x->%08x\n", hresult, hresult_table[i].hresult32);
            return hresult_table[i].hresult32;
        }
    }
    TRACE("%08x\n", hresult);
    return hresult;
}
/* {F6989118-9D36-4B65-AE0C-0C20886D50F8} */
const IID IID_ISTGMEDIUMRelease = { 0xf6989118, 0x9d36, 0x4b65, { 0xae, 0xc, 0xc, 0x20, 0x88, 0x6d, 0x50, 0xf8 } };

ULONG CDECL ISTGMEDIUMRelease_16_32_Release(SEGPTR This)
{
    ISTGMEDIUMRelease *iface32 = (ISTGMEDIUMRelease*)get_interface32(This);
    STGMEDIUM *ptr = (STGMEDIUM*)get_interface32_ptr(This);
    ULONG result;
    TRACE("(%04x:%04x(%p))\n", SELECTOROF(This), OFFSETOF(This), iface32);
    result = iface32->lpVtbl->Release(iface32);
    if (result == 0)
    {
        TRACE("(%04x:%04x(%p)) free\n", SELECTOROF(This), OFFSETOF(This), iface32);
    }
    return result;
}
ULONG STDMETHODCALLTYPE ISTGMEDIUMRelease_32_16_Release(ISTGMEDIUMRelease *This)
{
    SEGPTR iface16 = get_interface16(This);
    STGMEDIUM16 *ptr = (STGMEDIUM16*)get_interface16_ptr(This);
    ULONG result;
    TRACE("(%p(%04x:%04x))\n", This, SELECTOROF(iface16), OFFSETOF(iface16));
    result = ISTGMEDIUMRelease16_Release(iface16);
    if (result == 0)
    {
        TRACE("(%p(%04x:%04x)) free\n", This, SELECTOROF(iface16), OFFSETOF(iface16));
    }
    return result;
}

ULONG STDMETHODCALLTYPE ISTGMEDIUMRelease_32_16_Release(ISTGMEDIUMRelease *This);
typedef struct
{
    ISTGMEDIUMRelease ISTGMEDIUMRelease_iface;
    LONG ref;
} ISTGMEDIUM_impl;


static inline ISTGMEDIUM_impl *impl_from_ISTGMEDIUMRelease(ISTGMEDIUMRelease *iface)
{
    return CONTAINING_RECORD(iface, ISTGMEDIUM_impl, ISTGMEDIUMRelease_iface);
}

static ULONG WINAPI ISTGMEDIUMRelease_AddRef(ISTGMEDIUMRelease *iface)
{
    ISTGMEDIUM_impl *This = impl_from_ISTGMEDIUMRelease(iface);
    return InterlockedIncrement(&This->ref);
}

static ULONG WINAPI ISTGMEDIUMRelease_Release(ISTGMEDIUMRelease *iface)
{
    ISTGMEDIUM_impl *This = impl_from_ISTGMEDIUMRelease(iface);
    return InterlockedDecrement(&This->ref);
}

static HRESULT WINAPI ISTGMEDIUMRelease_QueryInterface(ISTGMEDIUMRelease *iface,
    REFIID riid,
    void** ppvObject)
{
    *ppvObject = NULL;

    if (IsEqualIID(riid, &IID_ISTGMEDIUMRelease) ||
        IsEqualIID(riid, &IID_IUnknown))
    {
        *ppvObject = iface;
        IUnknown_AddRef(iface);
    }

    return *ppvObject ? S_OK : E_NOINTERFACE;
}

static const ISTGMEDIUMReleaseVtbl ISTGMEDIUMRelease_VTable =
{
    ISTGMEDIUMRelease_QueryInterface, ISTGMEDIUMRelease_AddRef, ISTGMEDIUMRelease_Release
};

#pragma pack(push, 1)
typedef struct {
    char  dmDeviceName[CCHDEVICENAME];
    UINT16  dmSpecVersion;
    UINT16  dmDriverVersion;
    UINT16  dmSize;
    UINT16  dmDriverExtra;
    DWORD dmFields;
    INT16   dmOrientation;
    INT16   dmPaperSize;
    INT16   dmPaperLength;
    INT16   dmPaperWidth;
    INT16   dmScale;
    INT16   dmCopies;
    INT16   dmDefaultSource;
    INT16   dmPrintQuality;
    INT16   dmColor;
    INT16   dmDuplex;
    INT16   dmYResolution;
    INT16   dmTTOption;
} DEVMODE16, *LPDEVMODE16;
#pragma pack(pop)

static DVTARGETDEVICE *DVTARGETDEVICE16To32W(const DVTARGETDEVICE *src)
{
    int pos = 0;
    int len;
    int size = src->tdSize * 2 - sizeof(DVTARGETDEVICE);
    DVTARGETDEVICE *dst = (DVTARGETDEVICE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, src->tdSize * 2 + sizeof(DEVMODEW) + sizeof(DVTARGETDEVICE));
    if (src->tdDriverNameOffset)
    {
        len = MultiByteToWideChar(CP_ACP, 0, (char *)src + src->tdDriverNameOffset, -1, dst->tdData, size);
        dst->tdDriverNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len * 2;
        size -= len * 2;
    }
    if (src->tdDeviceNameOffset)
    {
        len = MultiByteToWideChar(CP_ACP, 0, (char *)src + src->tdDeviceNameOffset, -1, dst->tdData + pos, size);
        dst->tdDeviceNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len * 2;
        size -= len * 2;
    }
    if (src->tdPortNameOffset)
    {
        len = MultiByteToWideChar(CP_ACP, 0, (char *)src + src->tdPortNameOffset, -1, dst->tdData + pos, size);
        dst->tdPortNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len * 2;
        size -= len * 2;
    }
    if (src->tdExtDevmodeOffset)
    {
        DEVMODE16 *dv16 = (char *)src + src->tdExtDevmodeOffset;
        DEVMODEW *dv32 = dst->tdData + pos;
        dst->tdExtDevmodeOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        MultiByteToWideChar(CP_ACP, 0, dv16->dmDeviceName, CCHDEVICENAME, dv32->dmDeviceName, CCHDEVICENAME);
        dv32->dmDeviceName[CCHDEVICENAME - 1] = 0;
        dv32->dmSpecVersion = 0x30a;
        dv32->dmDriverVersion = dv16->dmDriverVersion;
        dv32->dmSize = sizeof(DEVMODEW);
        dv32->dmDriverExtra = 0;
        dv32->dmFields = dv16->dmFields & 0x7fbf;
        dv32->dmOrientation = dv16->dmOrientation;
        dv32->dmPaperSize = dv16->dmPaperSize;
        dv32->dmPaperLength = dv16->dmPaperLength;
        dv32->dmPaperWidth = dv16->dmPaperWidth;
        dv32->dmScale = dv16->dmScale;
        dv32->dmCopies = dv16->dmCopies;
        dv32->dmDefaultSource = dv16->dmDefaultSource;
        dv32->dmPrintQuality = dv16->dmPrintQuality;
        dv32->dmColor = dv16->dmColor;
        dv32->dmDuplex = dv16->dmDuplex;
        dv32->dmYResolution = dv16->dmYResolution;
        dv32->dmTTOption = dv16->dmTTOption;
        pos += sizeof(DEVMODEW);
    }
    dst->tdSize = pos + offsetof(DVTARGETDEVICE, tdData);
    return dst;
}

static DVTARGETDEVICE *DVTARGETDEVICE32WTo16(const DVTARGETDEVICE *src)
{
    int pos = 0;
    int len;
    int size = src->tdSize - sizeof(DVTARGETDEVICE);
    DVTARGETDEVICE *dst = (DVTARGETDEVICE*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, src->tdSize);
    if (src->tdDriverNameOffset)
    {
        len = WideCharToMultiByte(CP_ACP, 0, (char *)src + src->tdDriverNameOffset, -1, dst->tdData, size, NULL, NULL);
        dst->tdDriverNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len;
        size -= len;
    }
    if (src->tdDeviceNameOffset)
    {
        len = WideCharToMultiByte(CP_ACP, 0, (char *)src + src->tdDeviceNameOffset, -1, dst->tdData + pos, size, NULL, NULL);
        dst->tdDeviceNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len;
        size -= len;
    }
    if (src->tdPortNameOffset)
    {
        len = WideCharToMultiByte(CP_ACP, 0, (char *)src + src->tdPortNameOffset, -1, dst->tdData + pos, size, NULL, NULL);
        dst->tdPortNameOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        pos += len;
        size -= len;
    }
    if (src->tdExtDevmodeOffset)
    {
        DEVMODEW *dv32 = (char *)src + src->tdExtDevmodeOffset;
        DEVMODE16 *dv16 = dst->tdData + pos;
        dst->tdExtDevmodeOffset = pos + offsetof(DVTARGETDEVICE, tdData);
        WideCharToMultiByte(CP_ACP, 0, dv32->dmDeviceName, CCHDEVICENAME, dv16->dmDeviceName, CCHDEVICENAME, NULL, NULL);
        dv16->dmDeviceName[CCHDEVICENAME - 1] = 0;
        dv16->dmSpecVersion = 0x30a;
        dv16->dmDriverVersion = dv32->dmDriverVersion;
        dv16->dmSize = sizeof(DEVMODE16);
        dv16->dmDriverExtra = 0; 
        dv16->dmFields = dv32->dmFields & 0x7fbf;
        dv16->dmOrientation = dv32->dmOrientation;
        dv16->dmPaperSize = dv32->dmPaperSize;
        dv16->dmPaperLength = dv32->dmPaperLength;
        dv16->dmPaperWidth = dv32->dmPaperWidth;
        dv16->dmScale = dv32->dmScale;
        dv16->dmCopies = dv32->dmCopies;
        dv16->dmDefaultSource = dv32->dmDefaultSource;
        dv16->dmPrintQuality = dv32->dmPrintQuality;
        dv16->dmColor = dv32->dmColor;
        dv16->dmDuplex = dv32->dmDuplex;
        dv16->dmYResolution = dv32->dmYResolution;
        dv16->dmTTOption = dv32->dmTTOption;
        pos += sizeof(DEVMODE16);
    }
    dst->tdSize = pos + offsetof(DVTARGETDEVICE, tdData);
    return dst;
}

void map_pformatetc16_32(FORMATETC *a32, const FORMATETC16 *a16)
{
    a32->cfFormat = a16->cfFormat;
    a32->ptd = a16->ptd ? DVTARGETDEVICE16To32W(MapSL(a16->ptd)) : NULL;
    a32->dwAspect = a16->dwAspect;
    a32->lindex = a16->lindex;
    a32->tymed = a16->tymed;
    if (TRACE_ON(ole))
    {
        char buf[100];
        buf[0] = 0;
        GetClipboardFormatNameA(a16->cfFormat, buf, 100);
        TRACE("%s(%04x),%04x:%04x,%d,%d,%d\n", buf, a32->cfFormat, SELECTOROF(a16->ptd), OFFSETOF(a16->ptd), a32->dwAspect, a32->lindex, a32->tymed);
    }
}
void map_pformatetc32_16(FORMATETC16 *a16, const FORMATETC *a32)
{
    a16->cfFormat = a32->cfFormat;
    a16->ptd = a32->ptd ? MapLS(DVTARGETDEVICE32WTo16(a32->ptd)) : NULL;
    a16->dwAspect = a32->dwAspect;
    a16->lindex = a32->lindex;
    a16->tymed = a32->tymed;
    if (TRACE_ON(ole))
    {
        char buf[100];
        buf[0] = 0;
        GetClipboardFormatNameA(a32->cfFormat, buf, 100);
        TRACE("%s(%04x),%p,%d,%d,%d\n", buf, a32->cfFormat, a32->ptd, a32->dwAspect, a32->lindex, a32->tymed);
    }
}

void map_formatetc16_32(FORMATETC *a32, const FORMATETC16 *a16)
{
    a32->cfFormat = a16->cfFormat;
    a32->ptd = (DVTARGETDEVICE*)MapSL(a16->ptd);
    a32->dwAspect = a16->dwAspect;
    a32->lindex = a16->lindex;
    a32->tymed = a16->tymed;
    if (TRACE_ON(ole))
    {
        char buf[100];
        buf[0] = 0;
        GetClipboardFormatNameA(a16->cfFormat, buf, 100);
        TRACE("%s(%04x),%04x:%04x,%d,%d,%d\n", buf, a32->cfFormat, SELECTOROF(a16->ptd), OFFSETOF(a16->ptd), a32->dwAspect, a32->lindex, a32->tymed);
    }
}
void map_formatetc32_16(FORMATETC16 *a16, const FORMATETC *a32)
{
    a16->cfFormat = a32->cfFormat;
    a16->ptd = MapLS(a32->ptd);
    a16->dwAspect = a32->dwAspect;
    a16->lindex = a32->lindex;
    a16->tymed = a32->tymed;
    if (TRACE_ON(ole))
    {
        char buf[100];
        buf[0] = 0;
        GetClipboardFormatNameA(a32->cfFormat, buf, 100);
        TRACE("%s(%04x),%p,%d,%d,%d\n", buf, a32->cfFormat, a32->ptd, a32->dwAspect, a32->lindex, a32->tymed);
    }
}

void map_stgmedium32_16_2(STGMEDIUM16 *a16, const STGMEDIUM *a32, BOOL fixme)
{
    IUnknown *punk = a32->pUnkForRelease;
    interface_16 *i16;
    a16->tymed = a32->tymed;
    a16->pUnkForRelease = iface32_16(&IID_ISTGMEDIUMRelease, punk);
    i16 = get_interface32_ptr(a16->pUnkForRelease);
    TRACE("release=%p\n", punk);
    switch ((TYMED)a32->tymed)
    {
    case TYMED_HGLOBAL:
    {
        LPVOID p = GlobalLock(a32->hGlobal);
        SIZE_T size = GlobalSize(a32->hGlobal);
        SEGPTR g16 = GlobalAlloc16(0, size);
        LPVOID p32 = GlobalLock16(g16);
        TRACE("TYMED_HGLOBAL\n");
        memcpy(p32, p, GlobalSize(a32->hGlobal));
        WOWGlobalUnlock16(g16);
        a16->hGlobal = g16;
        if (fixme)
        {
            FIXME("leak %04x(%p)\n", a16->hGlobal, a32->hGlobal);
        }
        else
        {
            TRACE("leak %04x(%p)\n", a16->hGlobal, a32->hGlobal);
        }
        break;
    }
    case TYMED_FILE:
        TRACE("TYMED_FILE\n");
        a16->lpszFileName = MapLS(strdupWtoA(a32->lpszFileName));
        break;
    case TYMED_ISTREAM:
        TRACE("TYMED_ISTREAM\n");
        a16->pstm = iface32_16(&IID_IStream, a32->pstm);
        break;
    case TYMED_ISTORAGE:
        TRACE("TYMED_ISTORAGE\n");
        a16->pstg = iface32_16(&IID_IStorage, a32->pstg);
        break;
    case TYMED_NULL:
        TRACE("TYMED_NULL\n");
        break;
    case TYMED_GDI:
        TRACE("TYMED_GDI %04x %p\n", a16->hGlobal, a32->hBitmap);
        a16->hGlobal = HBITMAP_16(a32->hBitmap);
        break;
    case TYMED_MFPICT:
    {
        METAFILEPICT16 *pict16;
        HGLOBAL data32 = a32->hGlobal;
        HGLOBAL16 data16;
        UINT size;
        void *ptr;
        METAFILEPICT *pict32 = GlobalLock(data32);

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
            a16->hGlobal = data16;
        }
        if (fixme)
        {
            FIXME("TYMED_MFPICT\n");
        }
        else
        {
            TRACE("TYMED_MFPICT\n");
        }
        break;
    }
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a32->tymed);
        break;
    }
}
void map_stgmedium32_16(STGMEDIUM16 *a16, const STGMEDIUM *a32)
{
    map_stgmedium32_16_2(a16, a32, TRUE);
}
void map_stgmedium16_32_2(STGMEDIUM *a32, const STGMEDIUM16 *a16, BOOL fixme)
{
    a32->tymed = a16->tymed;
    a32->pUnkForRelease = (IUnknown*)iface16_32(&IID_ISTGMEDIUMRelease, a16->pUnkForRelease);
    switch ((TYMED)a32->tymed)
    {
    case TYMED_HGLOBAL:
    {
        SIZE_T size = GlobalSize16(a16->hGlobal);
        LPVOID p16 = GlobalLock16(a16->hGlobal);
        LPVOID p32;
        a32->hGlobal = GlobalAlloc(0, size);
        p32 = GlobalLock(a32->hGlobal);
        memcpy(p32, p16, size);
        GlobalUnlock(a32->hGlobal);
        if (fixme)
        {
            FIXME("TYMED_HGLOBAL leak %p(%04x) %04x:%04x\n", a32->hGlobal, a16->hGlobal, SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        }
        else
        {
            TRACE("TYMED_HGLOBAL %p(%04x) %04x:%04x\n", a32->hGlobal, a16->hGlobal, SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        }
        break;
    }
    case TYMED_FILE:
        TRACE("TYMED_FILE %s %04x:%04x\n", debugstr_a((const char*)MapSL(a16->lpszFileName)), SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        a32->lpszFileName = strdupAtoW(MapSL(a16->lpszFileName));
        break;
    case TYMED_ISTREAM:
        TRACE("TYMED_ISTREAM %04x:%04x %04x:%04x\n", SELECTOROF(a16->pstm), OFFSETOF(a16->pstm), SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        a32->pstm = (IStream*)iface16_32(&IID_IStream, a16->pstm);
        break;
    case TYMED_ISTORAGE:
        TRACE("TYMED_ISTORAGE %04x:%04x %04x:%04x\n", SELECTOROF(a16->pstg), OFFSETOF(a16->pstg), SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        a32->pstg = (IStorage*)iface16_32(&IID_IStorage, a16->pstg);
        break;
    case TYMED_NULL:
        TRACE("TYMED_NULL %04x:%04x\n", SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        break;
    case TYMED_GDI:
        TRACE("TYMED_GDI %04x %04x:%04x\n", a16->hGlobal, SELECTOROF(a16->pUnkForRelease), OFFSETOF(a16->pUnkForRelease));
        a32->hBitmap = HBITMAP_32(a16->hGlobal);
        break;
    case TYMED_MFPICT:
    {
        METAHEADER *header;
        METAFILEPICT *pict32;
        HGLOBAL16 data16 = a16->hGlobal;
        METAFILEPICT16 *pict16 = GlobalLock16(data16);
        HGLOBAL data32;

        a32->hGlobal = NULL;
        if (pict16)
        {
            if (!(data32 = GlobalAlloc(GMEM_MOVEABLE, sizeof(*pict32)))) return 0;
            pict32 = GlobalLock(data32);
            pict32->mm = pict16->mm;
            pict32->xExt = pict16->xExt;
            pict32->yExt = pict16->yExt;
            header = GlobalLock16(pict16->hMF);
            pict32->hMF = SetMetaFileBitsEx(header->mtSize * 2, (BYTE *)header);
            GlobalUnlock16(pict16->hMF);
            GlobalUnlock(data32);
            a32->hGlobal = data32;
        }
        if (fixme)
        {
            FIXME("TYMED_MFPICT %04x %p leak %04x:%04x\n", a16->hGlobal, a32->hGlobal, a16->pUnkForRelease, a16->pUnkForRelease);
        }
        else
        {
            TRACE("TYMED_MFPICT %04x %p leak %04x:%04x\n", a16->hGlobal, a32->hGlobal, a16->pUnkForRelease, a16->pUnkForRelease);
        }
        break;
    }
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a16->tymed);
        break;
    }
}
void map_stgmedium16_32(STGMEDIUM *a32, const STGMEDIUM16 *a16)
{
    map_stgmedium16_32_2(a32, a16, TRUE);
}

void map_oleverb16_32(OLEVERB* a32, const OLEVERB16 *a16)
{
    a32->fuFlags = a16->fuFlags;
    a32->grfAttribs = a16->grfAttribs;
    a32->lpszVerbName = strdupAtoW(MapSL(a16->lpszVerbName));
    a32->lVerb = a16->lVerb;
}
void map_oleverb32_16(OLEVERB16* a16, const OLEVERB *a32)
{
    a16->fuFlags = a32->fuFlags;
    a16->grfAttribs = a32->grfAttribs;
    a16->lpszVerbName = MapLS(strdupWtoA(a32->lpszVerbName));
    a16->lVerb = a32->lVerb;
}

void map_oleinplaceframeinfo16_32(OLEINPLACEFRAMEINFO *a32, const struct TYP16_tagOleInPlaceFrameInfo *a16)
{
    a32->hwndFrame = HWND_32(a16->hwndFrame);
    a32->fMDIApp = a16->fMDIApp;
    a32->cb = a16->cb;
    a32->haccel = HACCEL_32(a16->haccel);
    a32->cAccelEntries = a16->cAccelEntries;
}

void map_oleinplaceframeinfo32_16(struct TYP16_tagOleInPlaceFrameInfo *a16, const OLEINPLACEFRAMEINFO *a32)
{
    a16->cb = a32->cb;
    a16->fMDIApp = a32->fMDIApp;
    a16->hwndFrame = HWND_16(a32->hwndFrame);
    a16->haccel = HACCEL_16(a32->haccel);
    a16->cAccelEntries = a32->cAccelEntries;
}

void map_interfaceinfo16_32(INTERFACEINFO *a32, const INTERFACEINFO16 *a16)
{
    a32->pUnk = (IUnknown*)iface16_32(&IID_IUnknown, a16->pUnk);
    a32->iid = a16->iid;
    a32->wMethod = a16->wMethod;
}

void map_interfaceinfo32_16(INTERFACEINFO16 *a16, const INTERFACEINFO *a32)
{
    a16->pUnk = iface32_16(&IID_IUnknown, a32->pUnk);
    a16->iid = a32->iid;
    a16->wMethod = a32->wMethod;
}

void map_tlibattr16_32(TLIBATTR *a32, const TLIBATTR16 *a16)
{
    a32->guid = a16->guid;
    a32->lcid = a16->lcid;
    a32->syskind = a16->syskind;
    a32->wLibFlags = a16->wLibFlags;
    a32->wMajorVerNum = a16->wMajorVerNum;
    a32->wMinorVerNum = a16->wMinorVerNum;
}

void map_tlibattr32_16(TLIBATTR16 *a16, const TLIBATTR *a32)
{
    a16->guid = a32->guid;
    a16->lcid = a32->lcid;
    a16->syskind = a32->syskind;
    a16->wLibFlags = a32->wLibFlags;
    a16->wMajorVerNum = a32->wMajorVerNum;
    a16->wMinorVerNum = a32->wMinorVerNum;
}

static int dynamic_SysStringLen16(SEGPTR bstr);
static void dynamic_SysFreeString16(SEGPTR bstr);
void map_bstr16_32(BSTR *a32, const SEGPTR *a16)
{
    int len16, len32;
    if (!*a16)
    {
        *a32 = NULL;
        return;
    }
    len16 = dynamic_SysStringLen16(*a16);
    len32 = MultiByteToWideChar(CP_ACP, 0, MapSL(*a16), len16, NULL, 0);
    *a32 = SysAllocStringLen(NULL, len32);
    MultiByteToWideChar(CP_ACP, 0, MapSL(*a16), len16 + 1, *a32, len32 + 1);
}

static int dynamic_SysAllocStringLen16(SEGPTR bstr, int len);
void map_bstr32_16(SEGPTR *a16, const BSTR *a32)
{
    UINT len;
    int len16;
    if (*a32 == NULL)
    {
        *a16 = 0;
        return;
    }
    len = SysStringLen(*a32);
    len16 = WideCharToMultiByte(CP_ACP, 0, *a32, len, NULL, 0, NULL, NULL);
    *a16 = dynamic_SysAllocStringLen16(NULL, len16);
    WideCharToMultiByte(CP_ACP, 0, *a32, len, MapSL(*a16), len16, NULL, NULL);
}

#ifdef IFS1632_OVERWRITE_ITypeLib_IsName
HRESULT CDECL ITypeLib_16_32_IsName(SEGPTR This, SEGPTR args16_szNameBuf, DWORD args16_lHashVal, SEGPTR args16_pfName)
{
    ITypeLib *iface32 = (ITypeLib*)get_interface32(This);
    HRESULT result__ = { 0 };
    TYP16_HRESULT result16__ = { 0 };
    LPOLESTR args32_szNameBuf = { 0 };
    ULONG args32_lHashVal;
    BOOL args32_pfName = { 0 };
    int szNameBufLen = lstrlenA((const char*)MapSL(args16_szNameBuf)) + 1;
    int widelen = MultiByteToWideChar(CP_ACP, 0, (const char*)MapSL(args16_szNameBuf), szNameBufLen, NULL, 0);
    args32_szNameBuf = HeapAlloc(GetProcessHeap(), 0, widelen * sizeof(OLECHAR));
    MultiByteToWideChar(CP_ACP, 0, (const char*)MapSL(args16_szNameBuf), szNameBufLen + 1, args32_szNameBuf, widelen);
    MAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    TRACE("(%04x:%04x(%p),%s,%08x,%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, MapSL(args16_szNameBuf), args16_lHashVal, args16_pfName);
    result__ = (HRESULT)iface32->lpVtbl->IsName(iface32, args32_szNameBuf, args32_lHashVal, &args32_pfName);
    MAP_HRESULT32_16(result16__, result__);
    UNMAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    WideCharToMultiByte(CP_ACP, 0, args32_szNameBuf, widelen, (const char*)MapSL(args16_szNameBuf), szNameBufLen, NULL, NULL);
    HeapFree(GetProcessHeap(), 0, args32_szNameBuf);
    TRACE("(%s,%s)\n", MapSL(args16_szNameBuf), args32_pfName ? "TRUE" : "FALSE");
    if (args16_pfName)
    {
        MAP_BOOL32_16((*(TYP16_BOOL*)MapSL(args16_pfName)), args32_pfName);
    }
    return result16__;
}
#endif

#ifdef IFS1632_OVERWRITE_ITypeLib_FindName
HRESULT CDECL ITypeLib_16_32_FindName(SEGPTR This, SEGPTR args16_szNameBuf, DWORD args16_lHashVal, SEGPTR args16_ppTInfo, SEGPTR args16_rgMemId, SEGPTR args16_pcFound)
{
    ITypeLib *iface32 = (ITypeLib*)get_interface32(This);
    HRESULT result__ = { 0 };
    TYP16_HRESULT result16__ = { 0 };
    OLECHAR *args32_szNameBuf = { 0 };
    ULONG args32_lHashVal;
    int i__;
    void *dst__;
    ITypeInfo * *args32_ppTInfo;
    MEMBERID *args32_rgMemId;
    USHORT args32_pcFound = { 0 };
    int szNameBufLen = lstrlenA((const char*)MapSL(args16_szNameBuf)) + 1;
    int widelen = MultiByteToWideChar(CP_ACP, 0, (const char*)MapSL(args16_szNameBuf), szNameBufLen, NULL, 0);
    args32_szNameBuf = HeapAlloc(GetProcessHeap(), 0, widelen * sizeof(OLECHAR));
    MultiByteToWideChar(CP_ACP, 0, (const char*)MapSL(args16_szNameBuf), szNameBufLen + 1, args32_szNameBuf, widelen);
    MAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    INMAP_PTR_USHORT16_32(args32_pcFound, args16_pcFound);
    args32_ppTInfo = IFACE_ALLOC_ARRAY(ITypeInfo *, *(&args32_pcFound));
    args32_rgMemId = IFACE_ALLOC_ARRAY(MEMBERID, *(&args32_pcFound));
    TRACE("(%04x:%04x(%p),%08x,%08x,%08x,%08x,%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_szNameBuf, args16_lHashVal, args16_ppTInfo, args16_rgMemId, args16_pcFound);
    result__ = (HRESULT)iface32->lpVtbl->FindName(iface32, args32_szNameBuf, args32_lHashVal, args32_ppTInfo, args32_rgMemId, &args32_pcFound);
    MAP_HRESULT32_16(result16__, result__);
    UNMAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    WideCharToMultiByte(CP_ACP, 0, args32_szNameBuf, widelen, (const char*)MapSL(args16_szNameBuf), szNameBufLen, NULL, NULL);
    HeapFree(GetProcessHeap(), 0, args32_szNameBuf);
    dst__ = MapSL(args16_ppTInfo);
    for (i__ = 0; i__ < (*(&args32_pcFound)); i__++)
    {
        ((SEGPTR*)(dst__))[i__] = iface32_16(&IID_ITypeInfo, args32_ppTInfo[i__]);
    }
    IFACE_FREE_ARRAY(args32_ppTInfo);
    dst__ = MapSL(args16_rgMemId);
    for (i__ = 0; i__ < (*(&args32_pcFound)); i__++)
    {
        MAP_MEMBERID32_16((((TYP16_MEMBERID*)(dst__))[i__]), args32_rgMemId[i__]);
    }
    IFACE_FREE_ARRAY(args32_rgMemId);
    if (args16_pcFound)
    {
        OUTMAP_USHORT32_16((*(TYP16_USHORT*)MapSL(args16_pcFound)), args32_pcFound);
    }
    return result16__;
}
#endif

#ifdef IFS3216_OVERWRITE_ITypeLib_IsName
HRESULT STDMETHODCALLTYPE ITypeLib_32_16_IsName(ITypeLib *This, LPOLESTR szNameBuf, ULONG lHashVal, BOOL *pfName)
{
    SEGPTR iface16 = get_interface16(This);
    TYP16_HRESULT result__ = { 0 };
    HRESULT result32__ = { 0 };
    TYP16_OLECHAR args16_szNameBuf = { 0 };
    TYP16_ULONG args16_lHashVal;
    TYP16_BOOL args16_pfName = { 0 };
    INMAP_LPOLESTR32_16(args16_szNameBuf, szNameBuf);
    MAP_ULONG32_16(args16_lHashVal, lHashVal);
    TRACE("(%p(%04x:%04x),%p,%08x,%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), szNameBuf, lHashVal, pfName);
    result__ = (TYP16_HRESULT)ITypeLib16_IsName(iface16, MapLS(&args16_szNameBuf), args16_lHashVal, MapLS(&args16_pfName));
    MAP_HRESULT16_32(result32__, result__);
    UNMAP_LPOLESTR32_16(args16_szNameBuf, szNameBuf);
    UNMAP_ULONG32_16(args16_lHashVal, lHashVal);
    if (szNameBuf)
    {
        MAP_OLECHAR16_32(*szNameBuf, args16_szNameBuf);
    }
    if (pfName)
    {
        MAP_BOOL16_32(*pfName, args16_pfName);
    }
    return result32__;
}
#endif
#define FUNCDESC16_WRAPPER_MAGIC 'FDSC'
typedef struct
{
    DWORD magic;
    FUNCDESC *desc32;
    FUNCDESC16 desc16;
} FUNCDESC16_WRAPPER;
void map_typedesc32_16(TYPEDESC16 *a16, const TYPEDESC *a32);
void map_typedesc16_32(TYPEDESC *a32, const TYPEDESC16 *a16);
void map_arraydesc32_16(ARRAYDESC16 *a16, const ARRAYDESC *a32)
{
    int i;
    a16->cDims = a32->cDims;
    map_typedesc32_16(&a16->tdescElem, &a32->tdescElem);
    for (i = 0; i < a16->cDims; i++)
    {
        a16->rgbounds[i].cElements = a32->rgbounds[i].cElements;
        a16->rgbounds[i].lLbound = a32->rgbounds[i].lLbound;
    }
}
void map_arraydesc16_32(ARRAYDESC16 *a32, const ARRAYDESC *a16)
{
    int i;
    a32->cDims = a16->cDims;
    map_typedesc16_32(&a32->tdescElem, &a16->tdescElem);
    for (i = 0; i < a32->cDims; i++)
    {
        a32->rgbounds[i].cElements = a16->rgbounds[i].cElements;
        a32->rgbounds[i].lLbound = a16->rgbounds[i].lLbound;
    }
}
void map_idldesc32_16(IDLDESC16 *a16, const IDLDESC *a32)
{
    map_bstr32_16(&a16->bstrIDLInfo, a32);
    a16->wIDLFlags = a32->wIDLFlags;
    TRACE("(%04x)\n", a16->wIDLFlags);
}
void map_idldesc16_32(IDLDESC16 *a32, const IDLDESC *a16)
{
    map_bstr16_32(&a32->bstrIDLInfo, a16);
    a32->wIDLFlags = a16->wIDLFlags;
    TRACE("(%04x)\n", a32->wIDLFlags);
}
void map_typedesc32_16(TYPEDESC16 *a16, const TYPEDESC *a32)
{
    a16->vt = a32->vt;
    switch (a32->vt)
    {
    case VT_PTR:
    {
        TYPEDESC16 *typ = (TYPEDESC16*)HeapAlloc(GetProcessHeap(), 0, sizeof(TYPEDESC16));
        map_typedesc32_16(typ, a32->lptdesc);
        a16->lptdesc = MapLS(typ);
        TRACE("VT_PTR\n");
        break;
    }
    case VT_CARRAY:
    {
        ARRAYDESC16 *ary = (ARRAYDESC16*)HeapAlloc(GetProcessHeap(), 0, sizeof(ARRAYDESC16) - sizeof(SAFEARRAYBOUND16) * (a32->lpadesc->cDims - 1));
        map_arraydesc32_16(ary, a32->lpadesc);
        a16->lpadesc = MapLS(ary);
        TRACE("VT_CARRAY\n");
        break;
    }
    case VT_USERDEFINED:
        a16->hreftype = a32->hreftype;
        TRACE("VT_USERDEFINED hreftype:%08x\n", a16->hreftype);
        break;
    default:
        a16->hreftype = 0xdeadbeef;
        TRACE("%s\n", debugstr_vt(a16->vt));
        break;
    }
}
void map_typedesc16_32(TYPEDESC *a32, const TYPEDESC16 *a16)
{
    a32->vt = a16->vt;
    switch (a16->vt)
    {
    case VT_PTR:
    {
        TYPEDESC *typ = (TYPEDESC*)HeapAlloc(GetProcessHeap(), 0, sizeof(TYPEDESC));
        map_typedesc16_32(typ, MapSL(a16->lptdesc));
        a32->lptdesc = typ;
        TRACE("VT_PTR\n");
        break;
    }
    case VT_CARRAY:
    {
        ARRAYDESC16 *ary16 = MapSL(a16->lpadesc);
        ARRAYDESC *ary = (ARRAYDESC*)HeapAlloc(GetProcessHeap(), 0, sizeof(ARRAYDESC) - sizeof(SAFEARRAYBOUND) * (ary16->cDims - 1));
        map_arraydesc16_32(ary, ary16);
        a32->lpadesc = ary;
        TRACE("VT_CARRAY\n");
        break;
    }
    case VT_USERDEFINED:
        a32->hreftype = a16->hreftype;
        TRACE("VT_USERDEFINED hreftype:%08x\n", a32->hreftype);
        break;
    default:
        a32->hreftype = 0xdeadbeef;
        TRACE("%s\n", debugstr_vt(a32->vt));
        break;
    }
}
void map_elemdesc32_16(ELEMDESC16 *a16, const ELEMDESC *a32)
{
    map_typedesc32_16(&a16->tdesc, &a32->tdesc);
    map_idldesc32_16(&a16->idldesc, &a32->idldesc);
    TRACE("\n");
}
void map_elemdesc16_32(ELEMDESC *a32, const ELEMDESC16 *a16)
{
    map_typedesc16_32(&a32->tdesc, &a16->tdesc);
    if (a32->tdesc.vt != VT_EMPTY)
        map_idldesc16_32(&a32->idldesc, &a16->idldesc);
    TRACE("\n");
}

FUNCDESC *map_funcdesc32(const FUNCDESC16 *a16)
{
    ELEMDESC *elm32;
    FUNCDESC *a32 = HeapAlloc(GetProcessHeap(), 0, sizeof(FUNCDESC) + sizeof(ELEMDESC) * a16->cParams);
    elm32 = (ELEMDESC*)(a32 + 1);
    a32->memid = a16->memid;
    a32->lprgscode = MapSL(a16->lprgscode);
    a32->lprgelemdescParam = elm32;
    for (int i = 0; i < a16->cParams; i++)
    {
        map_elemdesc16_32(elm32 + i, (ELEMDESC16 *)MapSL(a16->lprgelemdescParam) + i);
    }
    a32->funckind = a16->funckind;
    a32->invkind = a16->invkind;
    a32->callconv = a16->callconv;
    a32->cParams = a16->cParams;
    a32->cParamsOpt = a16->cParamsOpt;
    a32->oVft = a16->oVft;
    a32->cScodes = a16->cScodes;
    map_elemdesc16_32(&a32->elemdescFunc, &a16->elemdescFunc);
    a32->wFuncFlags = a16->wFuncFlags;
    return a32;
}

FUNCDESC16 *map_funcdesc16(const FUNCDESC *a32)
{
    int i;
    FUNCDESC16 *a16;
    ELEMDESC16 *elm16;
    FUNCDESC16_WRAPPER *w;
    if (!a32)
        return NULL;
    w = HeapAlloc(GetProcessHeap(), 0, sizeof(FUNCDESC16_WRAPPER) + sizeof(ELEMDESC16) * a32->cParams);
    w->magic = FUNCDESC16_WRAPPER_MAGIC;
    w->desc32 = a32;
    a16 = &w->desc16;
    elm16 = (ELEMDESC16*)(a16 + 1);
    a16->memid = a32->memid;
    a16->lprgscode = MapLS(a32->lprgscode);
    a16->lprgelemdescParam = MapLS(elm16);
    for (i = 0; i < a32->cParams; i++)
    {
        map_elemdesc32_16(elm16 + i, a32->lprgelemdescParam + i);
    }
    a16->funckind = a32->funckind;
    a16->invkind = a32->invkind;
    a16->callconv = a32->callconv;
    a16->cParams = a32->cParams;
    a16->cParamsOpt = a32->cParamsOpt;
    a16->oVft = a32->oVft;
    a16->cScodes = a32->cScodes;
    map_elemdesc32_16(&a16->elemdescFunc, &a32->elemdescFunc);
    a16->wFuncFlags = a32->wFuncFlags;
    return a16;
}

void free_arraydesc16(ARRAYDESC16 *a16)
{
}

void free_typedesc16(TYPEDESC16 *a16)
{
    switch (a16->vt)
    {
    case VT_PTR:
    {
        TYPEDESC16 *typ = (TYPEDESC16*)MapSL(a16->lptdesc);
        free_typedesc16(typ);
        HeapFree(GetProcessHeap(), 0, typ);
        break;
    }
    case VT_CARRAY:
    {
        ARRAYDESC16 *ary = (ARRAYDESC16*)MapSL(a16->lpadesc);
        free_arraydesc16(ary);
        HeapFree(GetProcessHeap(), 0, ary);
        break;
    }
    }
}

void free_idldesc16(IDLDESC16 *a16)
{
    dynamic_SysFreeString16(a16->bstrIDLInfo);
}
void free_elemdesc16(ELEMDESC16 *a16)
{
    free_typedesc16(&a16->tdesc);
    free_idldesc16(&a16->idldesc);
}
void free_funcdesc16(FUNCDESC16 *a16)
{
    int i;
    ELEMDESC16 *elm16;
    elm16 = (ELEMDESC16*)(a16 + 1);
    free_elemdesc16(&a16->elemdescFunc);
    for (i = 0; i < a16->cParams; i++)
    {
        free_elemdesc16(elm16 + i);
    }
}
#ifdef IFS1632_OVERWRITE_ITypeInfo_ReleaseFuncDesc
void CDECL ITypeInfo_16_32_ReleaseFuncDesc(SEGPTR This, SEGPTR args16_pFuncDesc)
{
    ITypeInfo *iface32 = (ITypeInfo*)get_interface32(This);
    FUNCDESC16 *desc16 = (FUNCDESC16*)MapSL(args16_pFuncDesc);
    FUNCDESC16_WRAPPER *wrap = CONTAINING_RECORD(desc16, FUNCDESC16_WRAPPER, desc16);
    FUNCDESC *args32_pFuncDesc;
    if (wrap->magic != FUNCDESC16_WRAPPER_MAGIC)
    {
        ERR("wrap->magic != FUNCDESC16_WRAPPER_MAGIC\n");
        return;
    }
    args32_pFuncDesc = wrap->desc32;
    TRACE("(%04x:%04x(%p),%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_pFuncDesc);
    iface32->lpVtbl->ReleaseFuncDesc(iface32, args32_pFuncDesc);
    free_funcdesc16(&wrap->desc16);
    UnMapLS(args16_pFuncDesc);
    HeapFree(GetProcessHeap(), 0, wrap);
}
#endif
#ifdef IFS3216_OVERWRITE_ITypeInfo_ReleaseFuncDesc
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseFuncDesc(ITypeInfo *This, FUNCDESC *pFuncDesc)
{
    FIXME("\n");
    return;
    SEGPTR iface16 = get_interface16(This);
    SEGPTR args16_pFuncDesc;
    MAP_PTR_FUNCDESC32_16(args16_pFuncDesc, pFuncDesc);
    TRACE("(%p(%04x:%04x),%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), pFuncDesc);
    ITypeInfo16_ReleaseFuncDesc(iface16, args16_pFuncDesc);
    UNMAP_PTR_FUNCDESC32_16(args16_pFuncDesc, pFuncDesc);
}
#endif

#define TYPEATTR16_WRAPPER_MAGIC 'tATR'
typedef struct
{
    DWORD magic;
    TYPEATTR *attr32;
    TYPEATTR16 attr16;
} TYPEATTR16_WRAPPER;
TYPEATTR16 *map_typeattr32_16(const TYPEATTR *a32)
{
    TYPEATTR16_WRAPPER *w = (TYPEATTR16_WRAPPER*)HeapAlloc(GetProcessHeap(), 0, sizeof(TYPEATTR16_WRAPPER));
    TYPEATTR16 *a16 = &w->attr16;
    w->magic = TYPEATTR16_WRAPPER_MAGIC;
    w->attr32 = a32;
    a16->guid = a32->guid;
    a16->lcid = a32->lcid;
    a16->dwReserved = a32->dwReserved;
    a16->memidConstructor = a32->memidConstructor;
    a16->memidDestructor = a32->memidDestructor;
    a16->lpstrSchema = a32->lpstrSchema /* FIXME? */;
    a16->typekind = a32->typekind;
    a16->cFuncs = a32->cFuncs;
    a16->cVars = a32->cVars;
    a16->cImplTypes = a32->cImplTypes;
    a16->cbSizeVft = a32->cbSizeVft;
    a16->cbAlignment = a32->cbAlignment;
    a16->wTypeFlags = a32->wTypeFlags;
    a16->wMajorVerNum = a32->wMajorVerNum;
    a16->wMinorVerNum = a32->wMinorVerNum;
    TRACE("{guid:%s,lcid:0x%04x,dwReserved:%d,memidConstructor:0x%08x,memidDestructor:0x%08x,lpstrSchema:%08x"
        ",typekind:%d,cFuncs:%d,cVars:%d,cImplTypes:%d,cbSizeVft:%d,cbAlignment:%d,wTypeFlags:0x%04x,wMajorVerNum:%d,wMinorVerNum:%d}\n",
        debugstr_guid(&a16->guid), a16->lcid, a16->dwReserved, a16->memidConstructor,
        a16->memidDestructor, a16->lpstrSchema, a16->typekind, a16->cFuncs, a16->cVars, 
        a16->cImplTypes, a16->cbSizeVft, a16->cbAlignment, a16->wTypeFlags, a16->wMajorVerNum, a16->wMinorVerNum);
    map_typedesc32_16(&a16->tdescAlias, &a32->tdescAlias);
    map_idldesc32_16(&a16->idldescType, &a32->idldescType);
    return a16;
}

void free_typeattr16(TYPEATTR16 *a16)
{
    free_typedesc16(&a16->tdescAlias);
    free_idldesc16(&a16->idldescType);
}

#ifdef IFS1632_OVERWRITE_ITypeInfo_ReleaseTypeAttr
void CDECL ITypeInfo_16_32_ReleaseTypeAttr(SEGPTR This, SEGPTR args16_pTypeAttr)
{
    ITypeInfo *iface32 = (ITypeInfo*)get_interface32(This);
    TYPEATTR *args32_pTypeAttr;
    TYPEATTR16_WRAPPER *w = CONTAINING_RECORD(MapSL(args16_pTypeAttr), TYPEATTR16_WRAPPER, attr16);
    if (w->magic != TYPEATTR16_WRAPPER_MAGIC)
    {
        ERR("w->magic != TYPEATTR16_WRAPPER_MAGIC\n");
        return;
    }
    args32_pTypeAttr = w->attr32;
    TRACE("(%04x:%04x(%p),%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_pTypeAttr);
    iface32->lpVtbl->ReleaseTypeAttr(iface32, args32_pTypeAttr);
    free_typeattr16(&w->attr16);
    HeapFree(GetProcessHeap(), 0, w);
}
#endif
#ifdef IFS3216_OVERWRITE_ITypeInfo_ReleaseTypeAttr
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseTypeAttr(ITypeInfo *This, TYPEATTR *pTypeAttr)
{
    SEGPTR iface16 = get_interface16(This);
    SEGPTR args16_pTypeAttr;
    FIXME("\n");
    return;
    MAP_PTR_TYPEATTR32_16(args16_pTypeAttr, pTypeAttr);
    TRACE("(%p(%04x:%04x),%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), pTypeAttr);
    ITypeInfo16_ReleaseTypeAttr(iface16, args16_pTypeAttr);
    UNMAP_PTR_TYPEATTR32_16(args16_pTypeAttr, pTypeAttr);
}
#endif

VARDESC *map_vardesc32(const VARDESC16 *a16)
{
    VARDESC *a32 = (VARDESC*)HeapAlloc(GetProcessHeap(), 0, sizeof(VARDESC));
    a16 = (VARDESC16 *)MapSL(a16);
    a32->memid = a16->memid;
    a32->lpstrSchema = a16->lpstrSchema;
    map_elemdesc16_32(&a32->elemdescVar, &a16->elemdescVar);
    a32->wVarFlags = a16->wVarFlags;
    a32->varkind = a16->varkind;
    a32->lpvarValue = 0xcafebabe;
    if (a16->varkind == VAR_CONST)
    {
        FIXME("VAR_CONST\n");
        a32->lpvarValue = MapSL(a16->lpvarValue);
    }
    if (a32->varkind == VAR_PERINSTANCE)
    {
        a32->oInst = a16->oInst;
    }
    TRACE("{memid:0x%08x,lpstrSchema:%d,wVarFlags:0x%04x,varkind:%d,lpvarValue:0x%08x}\n", a32->memid, a32->lpstrSchema, a32->wVarFlags, a32->varkind, a32->lpvarValue);
    return a32;
}

#define VARDESC16_WRAPPER_MAGIC 'Vmag'
typedef struct
{
    DWORD magic;
    VARDESC *desc32;
    VARDESC16 desc16;
} VARDESC16_WRAPPER;

VARDESC16 *map_vardesc16(const VARDESC *a32)
{
    VARDESC16_WRAPPER *w = (VARDESC16_WRAPPER*)HeapAlloc(GetProcessHeap(), 0, sizeof(VARDESC16_WRAPPER));
    VARDESC16 *a16 = &w->desc16;
    w->magic = VARDESC16_WRAPPER_MAGIC;
    w->desc32 = a32;
    a16->memid = a32->memid;
    a16->lpstrSchema = a32->lpstrSchema;
    map_elemdesc32_16(&a16->elemdescVar, &a32->elemdescVar);
    a16->wVarFlags = a32->wVarFlags;
    a16->varkind = a32->varkind;
    a16->lpvarValue = 0xcafebabe;
    if (a16->varkind == VAR_CONST)
    {
        FIXME("VAR_CONST\n");
        a16->lpvarValue = MapLS(a32->lpvarValue);
    }
    if (a16->varkind == VAR_PERINSTANCE)
    {
        a16->oInst = a32->oInst;
    }
    TRACE("{memid:0x%08x,lpstrSchema:%d,wVarFlags:0x%04x,varkind:%d,lpvarValue:0x%08x}\n", a16->memid, a16->lpstrSchema, a16->wVarFlags, a16->varkind, a16->lpvarValue);
    return a16;
}

void free_vardesc16(VARDESC16 *a16)
{
    if (a16->varkind == VAR_CONST)
    {
        FIXME("VAR_CONST\n");
        UnMapLS(a16->lpvarValue);
    }
    free_elemdesc16(&a16->elemdescVar);
}

void map_bindptr16_32(BINDPTR *a32, const BINDPTR16 *a16, const DESCKIND kind)
{
    switch (kind)
    {
    case DESCKIND_NONE:
        TRACE("DESCKIND_NONE\n");
        break;
    case DESCKIND_VARDESC:
        TRACE("DESCKIND_VARDESC\n");
        a32->lpvardesc = map_vardesc32((VARDESC16*)MapSL(a16->lpvardesc));
        break;
    case DESCKIND_FUNCDESC:
        TRACE("DESCKIND_FUNCDESC\n");
        a32->lpfuncdesc = map_funcdesc32((FUNCDESC16*)MapSL(a16->lpfuncdesc));
        break;
    case DESCKIND_TYPECOMP:
        TRACE("DESCKIND_TYPECOMP\n");
        a32->lptcomp = (ITypeComp*)iface16_32(&IID_ITypeComp, a16->lptcomp);
        break;
    case DESCKIND_IMPLICITAPPOBJ:
        TRACE("DESCKIND_IMPLICITAPPOBJ\n");
        a32->lpvardesc = map_vardesc32((VARDESC16*)MapSL(a16->lpvardesc));
        break;
    default:
        FIXME("unknown DESCKIND %d\n", kind);
        break;
    }
}

void map_bindptr32_16(BINDPTR16 *a16, const BINDPTR *a32, TYP16_DESCKIND kind)
{
    switch (kind)
    {
    case DESCKIND_NONE:
        TRACE("DESCKIND_NONE\n");
        break;
    case DESCKIND_VARDESC:
        TRACE("DESCKIND_VARDESC\n");
        a16->lpvardesc = MapLS(map_vardesc16(a32->lpvardesc));
        break;
    case DESCKIND_FUNCDESC:
        TRACE("DESCKIND_FUNCDESC\n");
        a16->lpfuncdesc = MapLS(map_funcdesc16(a32->lpfuncdesc));
        break;
    case DESCKIND_TYPECOMP:
        TRACE("DESCKIND_TYPECOMP\n");
        a16->lptcomp = iface32_16(&IID_ITypeComp, a32->lptcomp);
        break;
    case DESCKIND_IMPLICITAPPOBJ:
        TRACE("DESCKIND_IMPLICITAPPOBJ\n");
        a16->lpvardesc = MapLS(map_vardesc16(a32->lpvardesc));
        break;
    default:
        FIXME("unknown DESCKIND %d\n", kind);
        break;
    }
}
#ifdef IFS1632_OVERWRITE_ITypeComp_Bind
HRESULT CDECL ITypeComp_16_32_Bind(SEGPTR This, SEGPTR args16_szName, DWORD args16_lHashVal, WORD args16_wFlags, SEGPTR args16_ppTInfo, SEGPTR args16_pDescKind, SEGPTR args16_pBindPtr)
{
    ITypeComp *iface32 = (ITypeComp*)get_interface32(This);
    HRESULT result__ = { 0 };
    TYP16_HRESULT result16__ = { 0 };
    LPOLESTR args32_szName;
    ULONG args32_lHashVal;
    WORD args32_wFlags;
    ITypeInfo * args32_ppTInfo = { 0 };
    DESCKIND args32_pDescKind = { 0 };
    BINDPTR args32_pBindPtr = { 0 };
    MAP_LPOLESTR16_32(args32_szName, args16_szName);
    MAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    MAP_WORD16_32(args32_wFlags, args16_wFlags);
    TRACE("(%04x:%04x(%p),%s,%08x,%08x,%08x,%08x,%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, MapSL(args16_szName), args16_lHashVal, args16_wFlags, args16_ppTInfo, args16_pDescKind, args16_pBindPtr);
    result__ = (HRESULT)iface32->lpVtbl->Bind(iface32, args32_szName, args32_lHashVal, args32_wFlags, &args32_ppTInfo, &args32_pDescKind, &args32_pBindPtr);
    MAP_HRESULT32_16(result16__, result__);
    UNMAP_LPOLESTR16_32(args32_szName, args16_szName);
    UNMAP_ULONG16_32(args32_lHashVal, args16_lHashVal);
    UNMAP_WORD16_32(args32_wFlags, args16_wFlags);
    if (args16_ppTInfo)
    {
        *(SEGPTR*)MapSL(args16_ppTInfo) = iface32_16(&IID_ITypeInfo, args32_ppTInfo);
    }
    if (args16_pDescKind)
    {
        MAP_DESCKIND32_16((*(TYP16_DESCKIND*)MapSL(args16_pDescKind)), args32_pDescKind);
    }
    if (args16_pBindPtr)
    {
        /**/
        map_bindptr32_16(&(*(TYP16_BINDPTR*)MapSL(args16_pBindPtr)), &args32_pBindPtr, args32_pDescKind);
        /**/
    }
    return result16__;
}
#endif
#ifdef IFS3216_OVERWRITE_ITypeComp_Bind
HRESULT STDMETHODCALLTYPE ITypeComp_32_16_Bind(ITypeComp *This, LPOLESTR szName, ULONG lHashVal, WORD wFlags, ITypeInfo **ppTInfo, DESCKIND *pDescKind, BINDPTR *pBindPtr)
{
    SEGPTR iface16 = get_interface16(This);
    TYP16_HRESULT result__ = { 0 };
    HRESULT result32__ = { 0 };
    TYP16_LPOLESTR args16_szName;
    TYP16_ULONG args16_lHashVal;
    TYP16_WORD args16_wFlags;
    SEGPTR args16_ppTInfo = { 0 };
    TYP16_DESCKIND args16_pDescKind = { 0 };
    TYP16_BINDPTR args16_pBindPtr = { 0 };
    MAP_LPOLESTR32_16(args16_szName, szName);
    MAP_ULONG32_16(args16_lHashVal, lHashVal);
    MAP_WORD32_16(args16_wFlags, wFlags);
    TRACE("(%p(%04x:%04x),%p,%08x,%08x,%p,%p,%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), szName, lHashVal, wFlags, ppTInfo, pDescKind, pBindPtr);
    result__ = (TYP16_HRESULT)ITypeComp16_Bind(iface16, args16_szName, args16_lHashVal, args16_wFlags, MapLS(&args16_ppTInfo), MapLS(&args16_pDescKind), MapLS(&args16_pBindPtr));
    MAP_HRESULT16_32(result32__, result__);
    UNMAP_LPOLESTR32_16(args16_szName, szName);
    UNMAP_ULONG32_16(args16_lHashVal, lHashVal);
    UNMAP_WORD32_16(args16_wFlags, wFlags);
    if (ppTInfo)
    {
        *ppTInfo = iface16_32(&IID_ITypeInfo, args16_ppTInfo);
    }
    if (pDescKind)
    {
        MAP_DESCKIND16_32(*pDescKind, args16_pDescKind);
    }
    if (pBindPtr)
    {
        /**/
        map_bindptr16_32(&*pBindPtr, &args16_pBindPtr, args16_pDescKind);
        /**/
    }
    return result32__;
}
#endif

#ifdef IFS1632_OVERWRITE_ITypeInfo_ReleaseVarDesc
void CDECL ITypeInfo_16_32_ReleaseVarDesc(SEGPTR This, SEGPTR args16_pVarDesc)
{
    ITypeInfo *iface32 = (ITypeInfo*)get_interface32(This);
    VARDESC * args32_pVarDesc;

    /**/
    VARDESC16 *desc16 = (VARDESC16*)MapSL(args16_pVarDesc);
    VARDESC16_WRAPPER *w = CONTAINING_RECORD(desc16, VARDESC16_WRAPPER, desc16);
    if (w->magic != VARDESC16_WRAPPER_MAGIC)
    {
        ERR("w->magic != VARDESC16_WRAPPER_MAGIC\n");
        return;
    }
    args32_pVarDesc = w->desc32;
    /**/
    
    TRACE("(%04x:%04x(%p),%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_pVarDesc);
    iface32->lpVtbl->ReleaseVarDesc(iface32, args32_pVarDesc);
    /**/
    free_vardesc16(desc16);
    UnMapLS(args16_pVarDesc);
    HeapFree(GetProcessHeap(), 0, w);
    /**/
}
#endif
#ifdef IFS3216_OVERWRITE_ITypeInfo_ReleaseVarDesc
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseVarDesc(ITypeInfo *This, VARDESC *pVarDesc)
{
    SEGPTR iface16 = get_interface16(This);
    SEGPTR args16_pVarDesc;
    MAP_PTR_VARDESC32_16(args16_pVarDesc, pVarDesc);
    FIXME("\n");
    TRACE("(%p(%04x:%04x),%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), pVarDesc);
    ITypeInfo16_ReleaseVarDesc(iface16, args16_pVarDesc);
    UNMAP_PTR_VARDESC32_16(args16_pVarDesc, pVarDesc);
}
#endif

#ifndef VT_EXTRA_TYPE
#define VT_EXTRA_TYPE (VT_VECTOR|VT_ARRAY|VT_BYREF|VT_RESERVED)
#endif
void map_variant32_16(VARIANT16 *dst, const VARIANT *src)
{
    enum VARENUM t = V_VT(src) & VT_TYPEMASK;
    enum VARENUM e = V_VT(src) & VT_EXTRA_TYPE;
    dst->llVal = src->llVal;
    dst->vt = src->vt;
    if (V_ISARRAY(src))
    {
        FIXME("V_ISARRAY\n");
        return;
    }
    else if (V_VT(src) == (VT_BSTR | VT_BYREF))
    {
        FIXME("VT_BSTR | VT_BYREF\n");
    }
    else if (V_VT(src) == (VT_DISPATCH | VT_BYREF) || V_VT(src) == (VT_UNKNOWN | VT_BYREF))
    {
        FIXME("t32 == (VT_DISPATCH | VT_BYREF) || t32 == (VT_UNKNOWN | VT_BYREF)\n");
    }
    else if (V_VT(src) == (VT_VARIANT | VT_BYREF))
    {
        FIXME("VT_VARIANT | VT_BYREF\n");
    }
    else if (V_VT(src) == (VT_DECIMAL | VT_BYREF))
    {
        FIXME("VT_DECIMAL\n");
    }
    else
    {
        if (e & VT_ARRAY)
        {
            FIXME("VT_ARRAY\n");
        }
        if (e & VT_VECTOR)
        {
            FIXME("VT_VECTOR\n");
        }
        if (e & VT_BYREF)
        {
            switch (t)
            {
            case VT_EMPTY:
            case VT_NULL:
            case VT_VOID:
                break;
            case VT_I2:
            case VT_I4:
            case VT_R4:
            case VT_R8:
            case VT_CY:
            case VT_DATE:
            case VT_ERROR:
            case VT_BOOL:
            case VT_I1:
            case VT_UI1:
            case VT_UI2:
            case VT_UI4:
            case VT_I8:
            case VT_UI8:
            case VT_INT:
            case VT_UINT:
            case VT_HRESULT:
                dst->plVal = MapLS(src->plVal);
                return;
            case VT_DECIMAL:
                return;
            case VT_DISPATCH:
            case VT_BSTR:
            case VT_UNKNOWN:
            case VT_VARIANT:
            case VT_PTR:
            case VT_SAFEARRAY:
            case VT_CARRAY:
            case VT_USERDEFINED:
            case VT_LPSTR:
            case VT_LPWSTR:
            case VT_RECORD:
            case VT_INT_PTR:
            case VT_UINT_PTR:
            case VT_FILETIME:
            case VT_BLOB:
            case VT_STREAM:
            case VT_STORAGE:
            case VT_STREAMED_OBJECT:
            case VT_STORED_OBJECT:
            case VT_BLOB_OBJECT:
            case VT_CF:
            case VT_CLSID:
            case VT_VERSIONED_STREAM:
            case VT_BSTR_BLOB:
                FIXME("unsupported %s\n", debugstr_vt(src->vt));
                break;
            default:
                FIXME("unknown %s\n", debugstr_vt(src->vt));
                return;
            }
        }
        switch (t)
        {
        case VT_EMPTY:
        case VT_NULL:
        case VT_VOID:
            break;
        case VT_I2:
        case VT_I4:
        case VT_R4:
        case VT_R8:
        case VT_CY:
        case VT_DATE:
        case VT_ERROR:
        case VT_BOOL:
        case VT_I1:
        case VT_UI1:
        case VT_UI2:
        case VT_UI4:
        case VT_I8:
        case VT_UI8:
        case VT_INT:
        case VT_UINT:
        case VT_HRESULT:
            dst->llVal = src->llVal;
            break;
        case VT_DECIMAL:
            break;
        case VT_DISPATCH:
            dst->pdispVal = iface32_16(&IID_IDispatch, src->pdispVal);
            break;
        case VT_BSTR:
            map_bstr32_16(&dst->bstrVal, &src->bstrVal);
            break;
        case VT_UNKNOWN:
            dst->punkVal = iface32_16(&IID_IUnknown, src->punkVal);
            break;
        case VT_VARIANT:
        case VT_PTR:
        case VT_SAFEARRAY:
        case VT_CARRAY:
        case VT_USERDEFINED:
        case VT_LPSTR:
        case VT_LPWSTR:
        case VT_RECORD:
        case VT_INT_PTR:
        case VT_UINT_PTR:
        case VT_FILETIME:
        case VT_BLOB:
        case VT_STREAM:
        case VT_STORAGE:
        case VT_STREAMED_OBJECT:
        case VT_STORED_OBJECT:
        case VT_BLOB_OBJECT:
        case VT_CF:
        case VT_CLSID:
        case VT_VERSIONED_STREAM:
        case VT_BSTR_BLOB:
            FIXME("unsupported %s\n", debugstr_vt(src->vt));
            break;
        default:
            FIXME("unknown %s\n", debugstr_vt(src->vt));
            return;
        }
    }
}
void map_variant16_32(VARIANT *dst, const VARIANT16 *src)
{
    enum VARENUM t = V_VT(src) & VT_TYPEMASK;
    enum VARENUM e = V_VT(src) & VT_EXTRA_TYPE;
    dst->llVal = src->llVal;
    dst->vt = src->vt;
    if (V_ISARRAY(src))
    {
        FIXME("V_ISARRAY\n");
        return;
    }
    else if (V_VT(src) == (VT_BSTR | VT_BYREF))
    {
        FIXME("VT_BSTR | VT_BYREF\n");
    }
    else if (V_VT(src) == (VT_DISPATCH | VT_BYREF) || V_VT(src) == (VT_UNKNOWN | VT_BYREF))
    {
        FIXME("t32 == (VT_DISPATCH | VT_BYREF) || t32 == (VT_UNKNOWN | VT_BYREF)\n");
    }
    else if (V_VT(src) == (VT_VARIANT | VT_BYREF))
    {
        FIXME("VT_VARIANT | VT_BYREF\n");
    }
    else if (V_VT(src) == (VT_DECIMAL | VT_BYREF))
    {
        FIXME("VT_DECIMAL\n");
    }
    else
    {
        if (e & VT_ARRAY)
        {
            FIXME("VT_ARRAY\n");
        }
        if (e & VT_VECTOR)
        {
            FIXME("VT_VECTOR\n");
        }
        if (e & VT_BYREF)
        {
            switch (t)
            {
            case VT_EMPTY:
            case VT_NULL:
            case VT_VOID:
                break;
            case VT_I2:
            case VT_I4:
            case VT_R4:
            case VT_R8:
            case VT_CY:
            case VT_DATE:
            case VT_ERROR:
            case VT_BOOL:
            case VT_I1:
            case VT_UI1:
            case VT_UI2:
            case VT_UI4:
            case VT_I8:
            case VT_UI8:
            case VT_INT:
            case VT_UINT:
            case VT_HRESULT:
                dst->plVal = MapSL(src->plVal);
                return;
            case VT_DECIMAL:
                return;
            case VT_DISPATCH:
            case VT_BSTR:
            case VT_UNKNOWN:
            case VT_VARIANT:
            case VT_PTR:
            case VT_SAFEARRAY:
            case VT_CARRAY:
            case VT_USERDEFINED:
            case VT_LPSTR:
            case VT_LPWSTR:
            case VT_RECORD:
            case VT_INT_PTR:
            case VT_UINT_PTR:
            case VT_FILETIME:
            case VT_BLOB:
            case VT_STREAM:
            case VT_STORAGE:
            case VT_STREAMED_OBJECT:
            case VT_STORED_OBJECT:
            case VT_BLOB_OBJECT:
            case VT_CF:
            case VT_CLSID:
            case VT_VERSIONED_STREAM:
            case VT_BSTR_BLOB:
                FIXME("unsupported %s\n", debugstr_vt(src->vt));
                break;
            default:
                FIXME("unknown %s\n", debugstr_vt(src->vt));
                return;
            }
        }
        switch (t)
        {
        case VT_EMPTY:
        case VT_NULL:
        case VT_VOID:
            break;
        case VT_I2:
        case VT_I4:
        case VT_R4:
        case VT_R8:
        case VT_CY:
        case VT_DATE:
        case VT_ERROR:
        case VT_BOOL:
        case VT_I1:
        case VT_UI1:
        case VT_UI2:
        case VT_UI4:
        case VT_I8:
        case VT_UI8:
        case VT_INT:
        case VT_UINT:
        case VT_HRESULT:
            dst->llVal = src->llVal;
            break;
        case VT_DECIMAL:
            break;
        case VT_DISPATCH:
            dst->pdispVal = (IDispatch*)iface16_32(&IID_IDispatch, src->pdispVal);
            break;
        case VT_BSTR:
            map_bstr16_32(&dst->bstrVal, &src->bstrVal);
            break;
        case VT_UNKNOWN:
            dst->punkVal = (IUnknown*)iface16_32(&IID_IUnknown, src->punkVal);
            break;
        case VT_VARIANT:
        case VT_PTR:
        case VT_SAFEARRAY:
        case VT_CARRAY:
        case VT_USERDEFINED:
        case VT_LPSTR:
        case VT_LPWSTR:
        case VT_RECORD:
        case VT_INT_PTR:
        case VT_UINT_PTR:
        case VT_FILETIME:
        case VT_BLOB:
        case VT_STREAM:
        case VT_STORAGE:
        case VT_STREAMED_OBJECT:
        case VT_STORED_OBJECT:
        case VT_BLOB_OBJECT:
        case VT_CF:
        case VT_CLSID:
        case VT_VERSIONED_STREAM:
        case VT_BSTR_BLOB:
            FIXME("unsupported %s\n", debugstr_vt(src->vt));
            break;
        default:
            FIXME("unknown %s\n", debugstr_vt(src->vt));
            return;
        }
    }
}

void map_dispparams32_16(DISPPARAMS16 *a16, const DISPPARAMS *a32)
{
    VARIANT16 *v16;
    int i;
    a16->cArgs = a32->cArgs;
    a16->cNamedArgs = a32->cNamedArgs;
    a16->rgdispidNamedArgs = MapLS(a32->rgdispidNamedArgs);
    v16 = HeapAlloc(GetProcessHeap(), 0, sizeof(VARIANT16) * a16->cArgs);
    a16->rgvarg = MapLS(v16);
    for (i = 0; i < a16->cArgs; i++)
    {
        map_variant32_16(v16 + i, a32->rgvarg + i);
    }
}

void map_dispparams16_32(DISPPARAMS *a32, const DISPPARAMS16 *a16)
{
    VARIANT16 *v16;
    int i;
    a32->cArgs = a16->cArgs;
    a32->cNamedArgs = a16->cNamedArgs;
    a32->rgdispidNamedArgs = MapSL(a16->rgdispidNamedArgs);
    a32->rgvarg = HeapAlloc(GetProcessHeap(), 0, sizeof(VARIANT) * a32->cArgs);
    v16 = (VARIANT16*)MapSL(a16->rgvarg);
    for (i = 0; i < a16->cArgs; i++)
    {
        map_variant16_32(a32->rgvarg + i, v16 + i);
    }
}

void free_dispparams32(const DISPPARAMS *a32)
{
    HeapFree(GetProcessHeap(), 0, a32->rgvarg);
}
void free_dispparams16(const DISPPARAMS16 *a16)
{
    HeapFree(GetProcessHeap(), 0, MapSL(a16->rgvarg));
}

void map_excepinfo16_32(EXCEPINFO *a32, const EXCEPINFO16 *a16)
{
    a32->wCode = a16->wCode;
    a32->wReserved = a16->wReserved;
    map_bstr16_32(&a32->bstrSource, &a16->bstrSource);
    map_bstr16_32(&a32->bstrDescription, &a16->bstrDescription);
    map_bstr16_32(&a32->bstrHelpFile, &a16->bstrHelpFile);
    a32->wReserved = a16->dwHelpContext;
    a32->pvReserved = a16->pvReserved;
    if (a16->pfnDeferredFillIn)
        ERR("pfnDeferredFillIn\n");
    a32->pfnDeferredFillIn = 0;
    a32->scode = a16->scode;
}

void map_excepinfo32_16(EXCEPINFO16 *a16, const EXCEPINFO *a32)
{
    a16->wCode = a32->wCode;
    a16->wReserved = a32->wReserved;
    map_bstr32_16(&a16->bstrSource, &a32->bstrSource);
    map_bstr32_16(&a16->bstrDescription, &a32->bstrDescription);
    map_bstr32_16(&a16->bstrHelpFile, &a32->bstrHelpFile);
    a16->wReserved = a32->dwHelpContext;
    a16->pvReserved = a32->pvReserved;
    if (a32->pfnDeferredFillIn)
        ERR("pfnDeferredFillIn\n");
    a16->pfnDeferredFillIn = 0;
    a16->scode = a32->scode;
}

static HMODULE get_hmodule_helper(const char *mod)
{
    HMODULE hmod = GetModuleHandleA(mod);
    if (hmod)
        return hmod;
    return LoadLibraryA(mod);
}
static void dynamic_SysFreeString16(SEGPTR bstr)
{
    static void (WINAPI*pSysFreeString16)(SEGPTR);
    if (!pSysFreeString16)
    {
        pSysFreeString16 = GetProcAddress(get_hmodule_helper("OLE2DISP.DLL16"), "SysFreeString16");
    }
    if (!pSysFreeString16)
        return;
    pSysFreeString16(bstr);
}

static int dynamic_SysStringLen16(SEGPTR bstr)
{
    static int (WINAPI*pSysStringLen16)(SEGPTR);
    if (!pSysStringLen16)
    {
        pSysStringLen16 = GetProcAddress(get_hmodule_helper("OLE2DISP.DLL16"), "SysStringLen16");
    }
    if (!pSysStringLen16)
        return 0;
    return pSysStringLen16(bstr);
}

static int dynamic_SysAllocStringLen16(SEGPTR bstr, int len)
{
    static int (WINAPI*pSysAllocStringLen16)(SEGPTR bstr, int len);
    if (!pSysAllocStringLen16)
    {
        pSysAllocStringLen16 = GetProcAddress(get_hmodule_helper("OLE2DISP.DLL16"), "SysAllocStringLen16");
    }
    if (!pSysAllocStringLen16)
        return 0;
    return pSysAllocStringLen16(bstr, len);
}

void free_excepinfo16(const EXCEPINFO16 *a16)
{
    dynamic_SysFreeString16(a16->bstrSource);
    dynamic_SysFreeString16(a16->bstrDescription);
    dynamic_SysFreeString16(a16->bstrHelpFile);
}

void free_excepinfo32(const EXCEPINFO *a32)
{
    SysFreeString(a32->bstrSource);
    SysFreeString(a32->bstrDescription);
    SysFreeString(a32->bstrHelpFile);
}

#ifdef IFS1632_OVERWRITE_ITypeInfo_Invoke
HRESULT CDECL ITypeInfo_16_32_Invoke(SEGPTR This, SEGPTR args16_pvInstance, DWORD args16_memid, WORD args16_wFlags, SEGPTR args16_pDispParams, SEGPTR args16_pVarResult, SEGPTR args16_pExcepInfo, SEGPTR args16_puArgErr)
{
    ITypeInfo *iface32 = (ITypeInfo*)get_interface32(This);
    HRESULT result__ = { 0 };
    TYP16_HRESULT result16__ = { 0 };
    PVOID args32_pvInstance;
    MEMBERID args32_memid;
    WORD args32_wFlags;
    DISPPARAMS args32_pDispParams = { 0 };
    VARIANT args32_pVarResult = { 0 };
    EXCEPINFO args32_pExcepInfo = { 0 };
    UINT args32_puArgErr = { 0 };
    /**/
    /* MAP_PVOID16_32(args32_pvInstance, args16_pvInstance); */
    args32_pvInstance = args16_pvInstance;
    /**/
    MAP_MEMBERID16_32(args32_memid, args16_memid);
    MAP_WORD16_32(args32_wFlags, args16_wFlags);
    INMAP_PTR_DISPPARAMS16_32(args32_pDispParams, args16_pDispParams);
    TRACE("(%04x:%04x(%p),%08x,%08x,%08x,%08x,%08x,%08x,%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_pvInstance, args16_memid, args16_wFlags, args16_pDispParams, args16_pVarResult, args16_pExcepInfo, args16_puArgErr);
    result__ = (HRESULT)iface32->lpVtbl->Invoke(iface32, args32_pvInstance, args32_memid, args32_wFlags, &args32_pDispParams, &args32_pVarResult, &args32_pExcepInfo, &args32_puArgErr);
    MAP_HRESULT32_16(result16__, result__);
    /* UNMAP_PVOID16_32(args32_pvInstance, args16_pvInstance); */
    UNMAP_MEMBERID16_32(args32_memid, args16_memid);
    UNMAP_WORD16_32(args32_wFlags, args16_wFlags);
    if (args16_pDispParams)
    {
        OUTMAP_DISPPARAMS32_16((*(TYP16_DISPPARAMS*)MapSL(args16_pDispParams)), args32_pDispParams);
    }
    if (args16_pVarResult)
    {
        MAP_VARIANT32_16((*(TYP16_VARIANT*)MapSL(args16_pVarResult)), args32_pVarResult);
    }
    if (args16_pExcepInfo)
    {
        MAP_EXCEPINFO32_16((*(TYP16_EXCEPINFO*)MapSL(args16_pExcepInfo)), args32_pExcepInfo);
    }
    if (args16_puArgErr)
    {
        MAP_UINT32_16((*(TYP16_UINT*)MapSL(args16_puArgErr)), args32_puArgErr);
    }
    return result16__;
}
#endif

#ifdef IFS1632_OVERWRITE_ITypeInfo_AddressOfMember
HRESULT CDECL ITypeInfo_16_32_AddressOfMember(SEGPTR This, DWORD args16_memid, WORD args16_invKind, SEGPTR args16_ppv)
{
    ITypeInfo *iface32 = (ITypeInfo*)get_interface32(This);
    HRESULT result__ = { 0 };
    TYP16_HRESULT result16__ = { 0 };
    MEMBERID args32_memid;
    INVOKEKIND args32_invKind;
    PVOID args32_ppv = { 0 };
    MAP_MEMBERID16_32(args32_memid, args16_memid);
    MAP_INVOKEKIND16_32(args32_invKind, args16_invKind);
    TRACE("(%04x:%04x(%p),%08x,%08x,%08x)\n", SELECTOROF(This), OFFSETOF(This), iface32, args16_memid, args16_invKind, args16_ppv);
    result__ = (HRESULT)iface32->lpVtbl->AddressOfMember(iface32, args32_memid, args32_invKind, &args32_ppv);
    MAP_HRESULT32_16(result16__, result__);
    UNMAP_MEMBERID16_32(args32_memid, args16_memid);
    UNMAP_INVOKEKIND16_32(args32_invKind, args16_invKind);
    if (args16_ppv)
    {
        (*(TYP16_PVOID*)MapSL(args16_ppv)) = args32_ppv;
        /*MAP_PVOID32_16((*(TYP16_PVOID*)MapSL(args16_ppv)), args32_ppv);*/
    }
    return result16__;
}
#endif
/*** OLESTREAM32 methods ***/

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
#ifdef IFS3216_OVERWRITE_OLESTREAM32_Get
DWORD __stdcall OLESTREAM32_32_16_Get(OLESTREAM32 *This, void *lpszBuf, DWORD cbbuf)
{
    SEGPTR iface16 = get_interface16(This);
    WORD seg = AllocSelectorArray16(cbbuf / 0x10000 + 1);
    DWORD ret;
    if (!seg)
        return 0;
    SELECTOR_SetEntries(seg, lpszBuf, cbbuf, WINE_LDT_FLAGS_DATA);
    TRACE("(%p(%04x:%04x),%p,%08x)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), MAKESEGPTR(seg, 0), cbbuf);
    ret = OLESTREAM3216_Get(iface16, MAKESEGPTR(seg, 0), cbbuf);
    for (WORD i = 0; i <= cbbuf / 0x10000; i++)
        FreeSelector16(seg + i);
    return ret;
}
#endif
#ifdef IFS3216_OVERWRITE_OLESTREAM32_Put
DWORD __stdcall OLESTREAM32_32_16_Put(OLESTREAM32 *This, const void *lpszBuf, DWORD cbbuf)
{
    SEGPTR iface16 = get_interface16(This);
    DWORD ret;
    WORD seg = AllocSelectorArray16(cbbuf / 0x10000 + 1);
    if (!seg)
        return 0;
    SELECTOR_SetEntries(seg, lpszBuf, cbbuf, WINE_LDT_FLAGS_DATA);
    TRACE("(%p(%04x:%04x),%p,%08x)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), MAKESEGPTR(seg, 0), cbbuf);
    ret = OLESTREAM3216_Put(iface16, MAKESEGPTR(seg, 0), cbbuf);
    for (WORD i = 0; i <= cbbuf / 0x10000; i++)
        FreeSelector16(seg + i);
    return ret;
}
#endif
#ifdef IFS3216_OVERWRITE_IMessageFilter_HandleInComingCall
DWORD STDMETHODCALLTYPE IMessageFilter_32_16_HandleInComingCall(IMessageFilter *This, DWORD dwCallType,HTASK htaskCaller,DWORD dwTickCount,LPINTERFACEINFO lpInterfaceInfo)
{
    SEGPTR iface16 = get_interface16(This);
    TYP16_DWORD result__ = {0};
    DWORD result32__ = {0};
    TYP16_DWORD args16_dwCallType;
    TYP16_HTASK args16_htaskCaller;
    TYP16_DWORD args16_dwTickCount;
    TYP16_LPINTERFACEINFO args16_lpInterfaceInfo;
    MAP_DWORD32_16(args16_dwCallType, dwCallType);
    MAP_HTASK32_16(args16_htaskCaller, htaskCaller);
    MAP_DWORD32_16(args16_dwTickCount, dwTickCount);
    MAP_LPINTERFACEINFO32_16(args16_lpInterfaceInfo, lpInterfaceInfo);
    TRACE("(%p(%04x:%04x),%08x,%08x,%08x,%p)\n", This, SELECTOROF(iface16), OFFSETOF(iface16), dwCallType, htaskCaller, dwTickCount, lpInterfaceInfo);
    result__ = (TYP16_DWORD)IMessageFilter16_HandleInComingCall(iface16, args16_dwCallType, args16_htaskCaller, args16_dwTickCount, args16_lpInterfaceInfo);
    if (result__ > 2)
        result__ = 0;
    MAP_DWORD16_32(result32__, result__);
    UNMAP_DWORD32_16(args16_dwCallType, dwCallType);
    UNMAP_HTASK32_16(args16_htaskCaller, htaskCaller);
    UNMAP_DWORD32_16(args16_dwTickCount, dwTickCount);
    UNMAP_LPINTERFACEINFO32_16(args16_lpInterfaceInfo, lpInterfaceInfo);
    return result32__;
}
#endif
