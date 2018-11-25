#define IFS_THUNK_H
#ifdef IFS_THUNK_H
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "wownt32.h"
#include "ole2.h"
#include "winerror.h"
#include "oaidl.h"
#include "ocidl.h"

#include "wine/winbase16.h"
#include "wine/wingdi16.h"
#include "wine/winuser16.h"
#include "ifs.h"

#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(ole);
#include <pshpack1.h>
typedef struct
{
    void *lpVtbl;
    SEGPTR iface16;
    IID *riid;
    void *ptr;
} interface_32;
typedef struct
{
    SEGPTR lpVtbl;
    void *iface32;
    IID *riid;
    void *ptr;
} interface_16;
static void *get_interface32(SEGPTR iface16)
{
    return ((interface_16*)MapSL(iface16))->iface32;
}
static SEGPTR get_interface16(void *iface32)
{
    return ((interface_32*)iface32)->iface16;
}
static interface_16 *get_interface32_ptr(SEGPTR iface16)
{
    return ((interface_16*)MapSL(iface16));
}
static interface_32 *get_interface16_ptr(void *iface32)
{
    return ((interface_32*)iface32);
}
typedef struct
{
    void *func16;
    void *name;
    const char *args;
} vtbl_entry;
typedef struct
{
    IID iid;
    void *lpVtbl32;
    vtbl_entry *vtbl16; /* null-terminated */
    void *lpVtbl16;
    SEGPTR spVtbl16;
} interface_entry;
SEGPTR iface32_16(REFIID riid, void *iface32);
void *iface16_32(REFIID riid, SEGPTR iface16);
#define IFACE_ALLOC_ARRAY(type, size) ((type*)HeapAlloc(GetProcessHeap(), 0, (size) * sizeof(type)))
#define IFACE_FREE_ARRAY(ptr)



typedef SEGPTR TYP16_REFIID;
typedef SEGPTR LPvoid;
typedef SEGPTR TYP16_LPVOID;
typedef ULONG TYP16_ULONG;
typedef DWORD TYP16_DWORD;
typedef CLSID TYP16_CLSID;
typedef DWORD TYP16_DWORD;
typedef BOOL16 TYP16_BOOL;
typedef LONG TYP16_LONG;
typedef DWORD TYP16_SIZE_T;
typedef SEGPTR TYP16_LPOLESTR;
typedef SEGPTR TYP16_LPCOLESTR;
typedef BIND_OPTS TYP16_BIND_OPTS;
typedef FILETIME TYP16_FILETIME;
typedef ULARGE_INTEGER TYP16_ULARGE_INTEGER;
typedef ULARGE_INTEGER TYP16_LARGE_INTEGER;
typedef STATSTG16 TYP16_STATSTG;
typedef SEGPTR TYP16_SNB;
typedef SEGPTR TYP16_REFCLSID;
typedef struct
{
    CLIPFORMAT cfFormat;
    SEGPTR/*DVTARGETDEVICE FAR**/ ptd;
    DWORD dwAspect;
    LONG lindex;
    DWORD tymed;
} FORMATETC16;
typedef struct {
    DWORD tymed;
    union {
        HANDLE16 hGlobal;
        SEGPTR/*LPSTR*/lpszFileName;
        SEGPTR/*LPSTREAM*/pstm;
        SEGPTR/*LPSTORAGE*/pstg;
    };
    SEGPTR /*LPUNKNOWN*/pUnkForRelease;
} STGMEDIUM16;
typedef struct
{
    FORMATETC16 formatetc;
    DWORD advf;
    /*LPADVISESINK*/SEGPTR pAdvSink;
    DWORD dwConnection;
} STATDATA16;
typedef STATDATA16 TYP16_STATDATA;
typedef FORMATETC16 TYP16_FORMATETC;
typedef IID TYP16_IID;
typedef HTASK16 TYP16_HTASK;
typedef STGMEDIUM16 TYP16_STGMEDIUM;
typedef SEGPTR TYP16_LPINTERFACEINFO;

#define MAP_REFIID16_32(a32, a16) \
*(IID**)&a32 = (REFIID)MapSL(a16)
#define MAP_PTR_CLSID16_32(a32, a16) \
a32 = (CLSID*)MapSL(a16)
#define UNMAP_REFIID16_32
#define UNMAP_PTR_VOID16_32
#define UNMAP_PTR_CLSID16_32
#define UNMAP_PTR_PTR_VOID16_32
#define MAP_REFIID32_16(a16, a32) \
a16 = MapLS(a32)
#define UNMAP_REFIID32_16
#define MAP_PTR_VOID16_32(a32, a16) \
a32 = MapSL(a16)
#define MAP_PTR_VOID32_16(a16, a32) \
a16 = MapLS(a32)
#define UNMAP_SIZE_T16_32
#define UNMAP_SIZE_T32_16
#define MAP_SIZE_T16_32(a32, a16) a32 = a16
#define MAP_SIZE_T32_16(a16, a32) a16 = a32

#define MAP_CLSID32_16(a16, a32) *(CLSID*)&a16 = a32
#define UNMAP_PTR_VOID32_16
#define MAP_CLSID16_32(a32, a16) a32 = a16

#define MAP_LPVOID16_32 MAP_PTR_VOID16_32
#define MAP_LPVOID32_16 MAP_PTR_VOID32_16
#define UNMAP_LPVOID16_32 UNMAP_PTR_VOID16_32
#define UNMAP_LPVOID32_16 UNMAP_PTR_VOID32_16
#define MAP_VOID_PTR16_32 MAP_PTR_VOID16_32
#define MAP_VOID_PTR32_16 MAP_PTR_VOID32_16

#define MAP_PTR_BIND_OPTS16_32(a32, a16) a32 = (BIND_OPTS*)MapSL(a16)
#define MAP_PTR_BIND_OPTS32_16(a16, a32) a16 = MapLS(a32)
#define MAP_PTR_FILETIME16_32(a32, a16) a32 = (FILETIME*)MapSL(a16)
#define MAP_PTR_FILETIME32_16(a16, a32) a16 = MapLS(a32)
#define MAP_BIND_OPTS16_32(a32, a16) a32 = a16
#define MAP_BIND_OPTS32_16(a16, a32) *(BIND_OPTS*)&a16 = a32
#define UNMAP_PTR_BIND_OPTS16_32
#define UNMAP_PTR_BIND_OPTS32_16
#define UNMAP_PTR_FILETIME16_32
#define UNMAP_PTR_FILETIME32_16
#define MAP_FILETIME32_16(a16, a32) *(FILETIME*)&a16 = a32
#define MAP_FILETIME16_32(a32, a16) a32 = a16
#define UNMAP_HTASK32_16
#define UNMAP_VOID_PTR16_32
#define UNMAP_VOID_PTR32_16
#define UNMAP_HTASK16_32

#define MAP_LARGE_INTEGER16_32(a32, a16) a32 = a16
#define UNMAP_LARGE_INTEGER16_32
#define MAP_ULARGE_INTEGER32_16(a16, a32) *(ULARGE_INTEGER*)&a16 = a32
#define MAP_LARGE_INTEGER32_16 MAP_LARGE_INTEGER16_32
#define MAP_LARGE_INTEGER32_16(a16, a32) *(LARGE_INTEGER*)&a16 = a32
#define MAP_ULARGE_INTEGER16_32(a32, a16) a32 = a16
#define UNMAP_LARGE_INTEGER32_16
#define UNMAP_ULARGE_INTEGER16_32
#define UNMAP_ULARGE_INTEGER32_16

static void map_statstg32_16(STATSTG16 *a16, STATSTG *a32)
{
    a16->atime = a32->atime;
    a16->pwcsName = MapLS(strdupWtoA(a32->pwcsName));
    a16->type = a32->type;
    a16->atime = a32->atime;
    a16->cbSize = a32->cbSize;
    a16->mtime = a32->mtime;
    a16->ctime = a32->ctime;
    a16->atime = a32->atime;
    a16->grfMode = a32->grfMode;
    a16->grfLocksSupported = a32->grfLocksSupported;
    a16->clsid = a32->clsid;
    a16->grfStateBits = a32->grfStateBits;
    a16->reserved = a32->reserved;
}
#define MAP_STATSTG32_16(a16, a32) map_statstg32_16(&a16, &a32)
static void map_statstg16_32(STATSTG *a32, STATSTG16 *a16)
{
    /* FIXME: leak */
    a32->pwcsName = strdupAtoW(MapSL(a16->pwcsName));
    a32->type = a16->type;
    a32->atime = a16->atime;
    a32->cbSize = a16->cbSize;
    a32->mtime = a16->mtime;
    a32->ctime = a16->ctime;
    a32->atime = a16->atime;
    a32->grfMode = a16->grfMode;
    a32->grfLocksSupported = a16->grfLocksSupported;
    a32->clsid = a16->clsid;
    a32->grfStateBits = a16->grfStateBits;
    a32->reserved = a16->reserved;
}
#define MAP_STATSTG16_32(a32, a16) map_statstg16_32(&a32, &a16) 

#define MAP_LPCOLESTR16_32 MAP_LPOLESTR16_32
#define MAP_LPCOLESTR32_16 MAP_LPOLESTR32_16
#define UNMAP_LPCOLESTR16_32 UNMAP_LPOLESTR16_32
#define UNMAP_LPCOLESTR32_16 UNMAP_LPOLESTR32_16
#define MAP_SNB16_32 MAP_LPOLESTR16_32
#define MAP_SNB32_16 MAP_LPOLESTR32_16
#define UNMAP_SNB16_32 UNMAP_LPOLESTR16_32
#define UNMAP_SNB32_16 UNMAP_LPOLESTR32_16
extern const IID IID_ISTGMEDIUMRelease;
typedef IUnknown ISTGMEDIUMRelease;
typedef IUnknownVtbl ISTGMEDIUMReleaseVtbl;
#define IFS1632_OVERWRITE_ISTGMEDIUMRelease_Release
ULONG CDECL ISTGMEDIUMRelease_16_32_Release(SEGPTR This);
#define IFS3216_OVERWRITE_ISTGMEDIUMRelease_Release
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
static void map_stgmedium32_16(STGMEDIUM16 *a16, STGMEDIUM *a32)
{
    IUnknown *punk = a32->pUnkForRelease;
    interface_16 *i16;
    a16->tymed = a32->tymed;
    a16->pUnkForRelease = iface32_16(&IID_ISTGMEDIUMRelease, punk);
    i16 = get_interface32_ptr(a16->pUnkForRelease);
    switch ((TYMED)a32->tymed)
    {
    case TYMED_HGLOBAL:
    {
        GlobalFree(0);
        LPVOID p = GlobalLock(a32->hGlobal);
        SIZE_T size = GlobalSize(a32->hGlobal);
        SEGPTR g16 = GlobalAlloc16(0, size);
        LPVOID p32 = GlobalLock16(g16);
        memcpy(p32, p, GlobalSize(a32->hGlobal));
        WOWGlobalUnlock16(g16);
        a16->hGlobal = g16;
        FIXME("leak %04x(%p)\n", a16->hGlobal, a32->hGlobal);
        break;
    }
    case TYMED_FILE:
        a16->lpszFileName = MapLS(strdupWtoA(a32->lpszFileName));
        break;
    case TYMED_ISTREAM:
        a16->pstm = iface32_16(&IID_IStream, a32->pstm);
        break;
    case TYMED_ISTORAGE:
        a16->pstg = iface32_16(&IID_IStorage, a32->pstg);
        break;
    case TYMED_NULL:
        break;
    case TYMED_GDI:
    case TYMED_MFPICT:
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a32->tymed);
        break;
    }
}
#define MAP_STGMEDIUM32_16(a16, a32) map_stgmedium32_16((STGMEDIUM16*)&a16, &a32)
static void map_stgmedium16_32(STGMEDIUM *a32, STGMEDIUM16 *a16)
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
        FIXME("leak %p(%04x)\n", a32->hGlobal, a16->hGlobal);
        break;
    }
    case TYMED_FILE:
        a32->lpszFileName = strdupAtoW(MapSL(a16->lpszFileName));
        break;
    case TYMED_ISTREAM:
        a32->pstm = (IStream*)iface16_32(&IID_IStream, a16->pstm);
        break;
    case TYMED_ISTORAGE:
        a32->pstg = (IStorage*)iface16_32(&IID_IStorage, a16->pstg);
        break;
    case TYMED_NULL:
        break;
    case TYMED_GDI:
    case TYMED_MFPICT:
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a16->tymed);
        break;
    }
}
#define MAP_STGMEDIUM16_32(a32, a16) map_stgmedium16_32(&a32, &a16)
#define UNMAP_IID_PTR16_32
#define UNMAP_IID_PTR32_16
#define UNMAP_REFCLSID32_16
#define UNMAP_REFCLSID16_32
#define UNMAP_PTR_FORMATETC32_16
#define UNMAP_PTR_FORMATETC16_32
#define UNMAP_STGMEDIUM32_16
#define UNMAP_STGMEDIUM16_32
#define UNMAP_LPINTERFACEINFO32_16
#define UNMAP_LPINTERFACEINFO16_32
#define UNMAP_PTR_STGMEDIUM32_16
#define UNMAP_PTR_STGMEDIUM16_32

static void map_formatetc16_32(FORMATETC *a32, FORMATETC16 *a16)
{
    a32->cfFormat = a16->cfFormat;
    a32->ptd = (DVTARGETDEVICE*)MapSL(a16->ptd);
    a32->dwAspect = a16->dwAspect;
    a32->lindex = a16->lindex;
    a32->tymed = a16->tymed;
}
#define MAP_PTR_FORMATETC16_32(a32, a16) map_formatetc16_32(a32 = (FORMATETC*)alloca(sizeof(FORMATETC)), (FORMATETC16*)MapSL(a16))
static void map_formatetc32_16(FORMATETC16 *a16, FORMATETC *a32)
{
    a16->cfFormat = a32->cfFormat;
    a16->ptd = MapLS(a32->ptd);
    a16->dwAspect = a32->dwAspect;
    a16->lindex = a32->lindex;
    a16->tymed = a32->tymed;
}
#define MAP_PTR_FORMATETC32_16(a16, a32) map_formatetc32_16((FORMATETC16*)MapSL(a16 = MapLS(alloca(sizeof(FORMATETC16)))), a32);

#define MAP_PTR_STGMEDIUM16_32(a32, a16) map_stgmedium16_32(a32 = (STGMEDIUM*)alloca(sizeof(STGMEDIUM)), (STGMEDIUM16*)MapSL(a16))

#define MAP_PTR_STGMEDIUM32_16(a16, a32) map_stgmedium32_16((STGMEDIUM16*)MapSL(a16 = MapLS(alloca(sizeof(STGMEDIUM16)))), a32);

#define MAP_FORMATETC32_16(a16, a32) map_formatetc32_16((FORMATETC16*)&a16, &a32)
#define MAP_FORMATETC16_32(a32, a16) map_formatetc16_32(&a32, (FORMATETC16*)&a16) 

#define MAP_HTASK16_32(a32, a16) a32 = HTASK_32(a16)
#define MAP_HTASK32_16(a16, a32) a16 = HTASK_16(a32)
#define MAP_HDC16_32(a32, a16) a32 = HDC_32(a16)
#define MAP_HDC32_16(a16, a32) a16 = HDC_16(a32)
#define MAP_HMENU16_32(a32, a16) a32 = HMENU_32(a16)
#define MAP_HMENU32_16(a16, a32) a16 = HMENU_16(a32)
#define MAP_HWND16_32(a32, a16) a32 = HWND_32(a16)
#define MAP_HWND32_16(a16, a32) a16 = HWND_16(a32)

#define UNMAP_HDC16_32(a32, a16)
#define UNMAP_HDC32_16(a16, a32)
#define UNMAP_HMENU16_32(a32, a16)
#define UNMAP_HMENU32_16(a16, a32)
#define UNMAP_HWND16_32(a32, a16)
#define UNMAP_HWND32_16(a16, a32)
#define MAP_LPOLESTR32_16(a16, a32) a16 = MapLS(strdupWtoA(a32))
#define MAP_LPOLESTR16_32(a32, a16) a32 = strdupAtoW(MapSL(a16))

#define UNMAP_LPOLESTR16_32(a32, a16) HeapFree(GetProcessHeap(), 0, a32)
#define UNMAP_LPOLESTR32_16(a16, a32) HeapFree(GetProcessHeap(), 0, MapSL(a16))
typedef struct
{
    /*LPUNKNOWN */SEGPTR pUnk;
    IID iid;
    WORD wMethod;
} INTERFACEINFO16;


static void map_interfaceinfo16_32(INTERFACEINFO *a32, INTERFACEINFO16 *a16)
{
    FIXME("\n");
}
#define MAP_LPINTERFACEINFO16_32(a32, a16) map_interfaceinfo16_32(a32 = (INTERFACEINFO*)alloca(sizeof(INTERFACEINFO)), (INTERFACEINFO16*)MapSL(a16))
static void map_interfaceinfo32_16(INTERFACEINFO16 *a16, INTERFACEINFO *a32)
{
    FIXME("\n");
}
#define MAP_LPINTERFACEINFO32_16(a16, a32) map_interfaceinfo32_16((INTERFACEINFO16*)MapSL(a16 = MapLS(alloca(sizeof(INTERFACEINFO16)))), a32);

#define MAP_IID_PTR16_32(a32, a16) a32 = (const IID*)MapSL(a16)
#define MAP_IID_PTR32_16(a16, a32) a16 = MapLS(a32)
#define MAP_REFCLSID16_32(a32, a16) *(CLSID**)&a32 = (const IID*)MapSL(a16)
#define MAP_REFCLSID32_16(a16, a32) a16 = MapLS(a32)

typedef SEGPTR TYP16_LPBINDCTX;
typedef GUID TYP16_GUID;

#define MAP_GUID32_16(a16, a32) *(GUID*)&a16 = a32
#define MAP_GUID16_32(a16, a32) a32 = a16

typedef HWND16 TYP16_HWND;
typedef WORD TYP16_WORD;
typedef SEGPTR TYP16_LPMSG;
typedef SIZE16 TYP16_SIZE;
typedef SIZEL TYP16_SIZEL;
typedef SEGPTR TYP16_IViewObjectCallback;/*FIXME*/
typedef void *IViewObjectCallback;/*FIXME*/
typedef HDC16 TYP16_HDC;
typedef POINTL TYP16_POINTL;
typedef HGLOBAL16 TYP16_HOLEMENU;
typedef HMENU16 TYP16_HMENU;
typedef DWORD TYP16_ULONG_PTR;
struct TYP16_tagOleMenuGroupWidths
{
    LONG    width[6];
};
struct tagOleInPlaceFrameInfo
{
    UINT cb;
    BOOL fMDIApp;
    HWND hwndFrame;
    HACCEL haccel;
    INT cAccelEntries;
};
struct TYP16_tagOleInPlaceFrameInfo
{
    UINT16 cb;
    BOOL16 fMDIApp;
    HWND16 hwndFrame;
    HACCEL16 haccel;
    INT16 cAccelEntries;
};
struct TYP16_tagRECT
{
    INT16  left;
    INT16  top;
    INT16  right;
    INT16  bottom;
};
#define UNMAP_ULONG_PTR16_32
#define UNMAP_LPCRECTL16_32
#define UNMAP_PTR_DVTARGETDEVICE16_32
#define UNMAP_ULONG_PTR32_16
#define UNMAP_LPCRECTL32_16
#define UNMAP_PTR_DVTARGETDEVICE32_16
#define UNMAP_SIZE32_16
#define UNMAP_SIZE16_32
#define UNMAP_PTR_SIZEL16_32
#define UNMAP_PTR_SIZEL32_16
#define UNMAP_PTR_LOGPALETTE16_32
#define UNMAP_PTR_LOGPALETTE32_16
#define UNMAP_POINTL16_32
#define UNMAP_POINTL32_16
#define UNMAP_LPMSG16_32
#define UNMAP_LPMSG32_16
#define UNMAP_LPCRECT16_32
#define UNMAP_LPCRECT32_16
#define UNMAP_LPCBORDERWIDTHS16_32
#define UNMAP_LPCBORDERWIDTHS32_16

#define UNMAP_IViewObjectCallback16_32
#define UNMAP_IViewObjectCallback32_16
#define UNMAP_HOLEMENU16_32 FIXME("UNMAP_HOLEMENU16_32\n");
#define UNMAP_HOLEMENU32_16 FIXME("UNMAP_HOLEMENU32_16\n");
#define MAP_ULONG_PTR16_32(a32, a16) a32 = a16
#define MAP_ULONG_PTR32_16(a32, a16) a16 = a32
#define MAP_SIZEL16_32(a32, a16) a32 = a16
#define MAP_SIZEL32_16(a16, a32) *(SIZEL*)&a16 = a32
#define MAP_POINTL16_32(a32, a16) a32 = a16
#define MAP_POINTL32_16(a16, a32) *(POINTL*)&a16 = a32
#define MAP_LPCRECTL16_32(a32, a16) a32 = (LPCRECTL)MapSL(a16)
#define MAP_LPCRECTL32_16(a16, a32) a16 = MapLS(a32)
#define MAP_PTR_SIZEL16_32(a32, a16) a32 = (SIZEL*)MapSL(a16)
#define MAP_PTR_SIZEL32_16(a16, a32) a16 = MapLS(a32)
static void map_rect32_16(RECT16 *a16, RECT *a32)
{
    a16->bottom = a32->bottom;
    a16->top = a32->top;
    a16->left = a32->left;
    a16->right = a32->right;
}
static void map_rect16_32(RECT *a32, RECT16 *a16)
{
    a32->bottom = a16->bottom;
    a32->top = a16->top;
    a32->left = a16->left;
    a32->right = a16->right;
}
#define MAP_STRUCT_tagRECT32_16(a16, a32) map_rect32_16((RECT16*)&a16, (RECT*)&a32)
#define MAP_STRUCT_tagRECT16_32(a32, a16) map_rect16_32((RECT*)&a32, (RECT16*)&a16)
#define MAP_STRUCT_tagOleMenuGroupWidths32_16(a16, a32) \
*(struct tagOleMenuGroupWidths*)&a16 = a32
#define MAP_STRUCT_tagOleMenuGroupWidths16_32(a32, a16) \
a32 = *(struct tagOleMenuGroupWidths*)&a16
#define MAP_LPCRECTL16_32(a32, a16) a32 = (LPCRECTL)MapSL(a16)
#define MAP_LPCRECTL32_16(a16, a32) a16 = MapLS(a16)

#define MAP_STRUCT_tagOleInPlaceFrameInfo32_16 FIXME("\n");
#define MAP_STRUCT_tagOleInPlaceFrameInfo16_32 FIXME("\n");
#define MAP_SIZE16_32 FIXME("\n");
#define MAP_SIZE32_16 FIXME("\n");
#define MAP_PTR_LOGPALETTE16_32 FIXME("\n");
#define MAP_PTR_LOGPALETTE32_16 FIXME("\n");
#define MAP_PTR_DVTARGETDEVICE16_32(a32, a16) a32 = (DVTARGETDEVICE*)MapSL(a16)
#define MAP_PTR_DVTARGETDEVICE32_16(a16, a32) a16 = MapLS(a32)
void WINAPI window_message32_16(const MSG *msg32, MSG16 *msg16);
void WINAPI window_message16_32(const MSG16 *msg16, MSG *msg32);
static void map_msg16_32(MSG *a32, const MSG16 *a16)
{
    FIXME("unicode <-> ANSI\n");
    window_message16_32(a16, a32);
}
static void map_msg32_16(MSG16 *a16, const MSG *a32)
{
    FIXME("unicode <-> ANSI\n");
    window_message32_16(a32, a16);
}
#define MAP_LPMSG16_32(a32, a16) \
    if (a16 == 0)\
        a32 = NULL;\
    else\
        map_msg16_32(a32 = (LPMSG)alloca(sizeof(MSG)), (const MSG16*)MapSL(a16))
#define MAP_LPMSG32_16(a16, a32) \
    if (a32 == NULL)\
        a16 = 0;\
    else\
        map_msg32_16((MSG16*)MapSL(a16 = MapLS(alloca(sizeof(MSG16)))), a32)
#define MAP_LPCRECT16_32(a32, a16) map_rect16_32(a32 = (LPCRECT*)alloca(sizeof(RECT)), (RECT16*)MapSL(a16))
#define MAP_LPCBORDERWIDTHS16_32 MAP_LPCRECT16_32
#define MAP_LPCRECT32_16(a16, a32) map_rect32_16(MapSL(a16 = MapLS((const RECT16*)alloca(sizeof(RECT16)))), (RECT*)a32)
#define MAP_LPCBORDERWIDTHS32_16 MAP_LPCRECT32_16
#define MAP_IViewObjectCallback16_32(a32, a16) \
    if (a16)\
        FIXME("16-bit callback func ptr-> 32-bit callback func ptr\n");\
    a32 = 0
#define MAP_IViewObjectCallback32_16(a16, a32) \
    if (a32)\
        FIXME("32-bit callback func ptr -> 16-bit callback func ptr\n");\
    a16 = 0
#define MAP_HOLEMENU16_32(a32,a16) a32 = HWND_32(a16);FIXME("MAP_HOLEMENU16_32\n");
#define MAP_HOLEMENU32_16(a16, a32) a16 = HWND_16(a32);FIXME("MAP_HOLEMENU32_16\n");

static void map_statdata32_16(STATDATA16* a16, const STATDATA *a32)
{
    map_formatetc32_16(&a16->formatetc, &a32->formatetc);
    a16->advf = a32->advf;
    a16->pAdvSink = (IAdviseSink*)iface32_16(&IID_IAdviseSink, a32->pAdvSink);
    a16->dwConnection = a32->dwConnection;
}
static void map_statdata16_32(STATDATA* a32, const STATDATA16 *a16)
{
    map_formatetc16_32(&a32->formatetc, &a16->formatetc);
    a32->advf = a16->advf;
    a32->pAdvSink = iface16_32(&IID_IAdviseSink, a16->pAdvSink);
    a32->dwConnection = a16->dwConnection;
}
#define MAP_STATDATA32_16(a16, a32) map_statdata32_16(&a16, &a32)
#define MAP_STATDATA16_32(a32, a16) FIXME("\n");

#define MAP_STRUCT_tagOLEVERB32_16 FIXME("\n");
#define MAP_STRUCT_tagOLEVERB16_32 FIXME("\n");
typedef unsigned int TYP16_LPOLEINPLACEFRAMEINFO;
typedef unsigned int TYP16_LPOLEMENUGROUPWIDTHS;
typedef SEGPTR TYP16_LPCRECT, TYP16_LPDATAOBJECT, TYP16_LPCBORDERWIDTHS, TYP16_LPCRECTL;

typedef struct TYP16_tagOLEVERB
{
    LONG lVerb;
    /*LPOLESTR*/SEGPTR lpszVerbName;
    DWORD fuFlags;
    DWORD grfAttribs;
} 	OLEVERB16;

#define MAP_PTR_DWORD16_32(a32, a16) a32 = (DWORD*)MapSL(a16)
#define MAP_PTR_DWORD32_16(a16, a32) a16 = MapLS(a32)
#define MAP_PTR_PTR_IFACE_IMoniker16_32(a32, a16) FIXME("\n")
#define MAP_PTR_PTR_IFACE_IMoniker32_16(a16, a32) FIXME("\n")
#define MAP_LPOLEINPLACEFRAMEINFO16_32(a32, a16) FIXME("\n")
#define MAP_LPOLEINPLACEFRAMEINFO32_16(a16, a32) FIXME("\n")
#define MAP_LPOLEMENUGROUPWIDTHS16_32(a32, a16) FIXME("\n")
#define MAP_LPOLEMENUGROUPWIDTHS32_16(a16, a32) FIXME("\n")

#define UNMAP_PTR_DWORD16_32
#define UNMAP_PTR_DWORD32_16
#define UNMAP_PTR_PTR_IFACE_IMoniker16_32
#define UNMAP_PTR_PTR_IFACE_IMoniker32_16
#define UNMAP_LPOLEINPLACEFRAMEINFO16_32
#define UNMAP_LPOLEINPLACEFRAMEINFO32_16
#define UNMAP_LPOLEMENUGROUPWIDTHS16_32
#define UNMAP_LPOLEMENUGROUPWIDTHS32_16

#define INMAP_PTR_BIND_OPTS16_32(a32, a16) FIXME("\n")
#define INMAP_PTR_BIND_OPTS32_16(a16, a32) FIXME("\n")
#define INMAP_PTR_PTR_IFACE_IMoniker16_32(a32, a16) FIXME("\n")
#define INMAP_PTR_PTR_IFACE_IMoniker32_16(a16, a32) FIXME("\n")
#define INMAP_PTR_STGMEDIUM16_32(a32, a16) FIXME("\n")
#define INMAP_PTR_STGMEDIUM32_16(a16, a32) FIXME("\n")
#define INMAP_LPOLEMENUGROUPWIDTHS16_32(a32, a16) FIXME("\n")
#define INMAP_LPOLEMENUGROUPWIDTHS32_16(a16, a32) FIXME("\n")
#define INMAP_LPOLEINPLACEFRAMEINFO16_32(a32, a16) FIXME("\n")
#define INMAP_LPOLEINPLACEFRAMEINFO32_16(a16, a32) FIXME("\n")
#define INMAP_PTR_DWORD16_32(a32, a16) a32 = *(DWORD*)MapSL(a16)
#define INMAP_PTR_DWORD32_16(a16, a32) a16 = *a32
typedef HRESULT TYP16_HRESULT;

#define MAP_HRESULT16_32(a32, a16) a32 = hresult16_32(a16)
#define MAP_HRESULT32_16(a16, a32) a16 = hresult32_16(a32)
#define MAP_ULONG16_32(a32, a16) a32 = a16
#define MAP_ULONG32_16(a16, a32) a16 = a32
#define MAP_BOOL16_32(a32, a16) a32 = a16
#define MAP_BOOL32_16(a16, a32) a16 = a32
#define MAP_DWORD16_32(a32, a16) a32 = a16
#define MAP_DWORD32_16(a16, a32) a16 = a32
#define MAP_INT16_32(a32, a16) a32 = a16
#define MAP_INT32_16(a16, a32) a16 = a32
#define MAP_LONG16_32(a32, a16) a32 = a16
#define MAP_LONG32_16(a16, a32) a16 = a32
#define MAP_WORD16_32(a32, a16) a32 = a16
#define MAP_WORD32_16(a16, a32) a16 = a32

#define UNMAP_BOOL16_32
#define UNMAP_BOOL32_16
#define UNMAP_DWORD16_32
#define UNMAP_DWORD32_16
#define UNMAP_ULONG16_32
#define UNMAP_ULONG32_16
#define UNMAP_LONG16_32
#define UNMAP_LONG32_16
#define UNMAP_WORD16_32
#define UNMAP_WORD32_16


typedef UINT16 TYP16_UINT;
typedef LCID TYP16_LCID;
typedef DISPID TYP16_DISPID;
typedef MEMBERID TYP16_MEMBERID;
typedef union tagBINDPTR16
{
    /*FUNCDESC **/SEGPTR lpfuncdesc;
    /*VARDESC **/SEGPTR lpvardesc;
    /*ITypeComp **/SEGPTR lptcomp;
} 	BINDPTR16;
typedef BINDPTR16 TYP16_BINDPTR;
typedef DESCKIND TYP16_DESCKIND; /* 32-bit enum!! */
typedef INVOKEKIND TYP16_INVOKEKIND; /* 32-bit enum!! */
typedef SEGPTR TYP16_BSTR;
typedef HREFTYPE TYP16_HREFTYPE;
typedef SEGPTR TYP16_REFGUID;
typedef struct tagDISPPARAMS16
{
    /* VARIANTARG* */SEGPTR rgvarg;
    /* DISPID* */SEGPTR rgdispidNamedArgs;
    UINT16 cArgs;
    UINT16 cNamedArgs;
} 	DISPPARAMS16;
typedef DISPPARAMS16 TYP16_DISPPARAMS;
typedef struct tagEXCEPINFO16 {
	unsigned short wCode;
	unsigned short wReserved;
	TYP16_BSTR bstrSource;
	TYP16_BSTR bstrDescription;
	TYP16_BSTR bstrHelpFile;
	unsigned long dwHelpContext;
	/* void* */SEGPTR pvReserved;	
	SEGPTR pexcepinfo/* HRESULT (*pfnDeferredFillIn)(EXCEPINFO * pexcepinfo) */;
	SCODE scode;
} EXCEPINFO16;
typedef EXCEPINFO16 TYP16_EXCEPINFO;
typedef INT16 TYP16_INT;
typedef USHORT TYP16_USHORT;
typedef BYTE TYP16_BYTE;
typedef SEGPTR TYP16_PVOID;
typedef SHORT TYP16_SHORT;
typedef HFONT TYP16_HFONT;
typedef HRGN TYP16_HRGN;
typedef CY TYP16_CY;
typedef TYPEKIND TYP16_TYPEKIND;
typedef UINT16 TYP16_OLE_HANDLE;
typedef OLE_XPOS_HIMETRIC TYP16_OLE_XPOS_HIMETRIC;
typedef OLE_YPOS_HIMETRIC TYP16_OLE_YPOS_HIMETRIC;
typedef OLE_XSIZE_HIMETRIC TYP16_OLE_XSIZE_HIMETRIC;
typedef OLE_YSIZE_HIMETRIC TYP16_OLE_YSIZE_HIMETRIC;
typedef POINTF TYP16_POINTF;
typedef struct
{
    /*size_t*/UINT16 cb;
    TYP16_LPOLESTR pszTitle;
    SIZE16 size;
    TYP16_LPOLESTR pszDocString;
    TYP16_LPOLESTR pszHelpFile;
    DWORD dwHelpContext;
} PROPPAGEINFO16;
typedef PROPPAGEINFO16 TYP16_PROPPAGEINFO;
typedef struct
{
	ULONG cb;
	HACCEL16 hAccel;
	USHORT cAccel;
	DWORD dwFlags;

} CONTROLINFO16;
typedef CONTROLINFO16 TYP16_CONTROLINFO;
typedef struct
{
    ULONG cElems;
    /*TYP16_LPOLESTR* */SEGPTR pElems;

} CALPOLESTR16;
typedef CALPOLESTR16 TYP16_CALPOLESTR;
typedef struct
{
    ULONG cElems;
    /* DWORD* */SEGPTR pElems;
} CADWORD16;
typedef CADWORD16 TYP16_CADWORD;
typedef struct
{
    long cbLicInfo;
    BOOL16 fRuntimeKeyAvail;
    BOOL16 fLicVerified;
} LICINFO16;
typedef LICINFO16 TYP16_LICINFO;
typedef struct
{
    ULONG cElems;
    /* GUID* */SEGPTR pElems;
} CAUUID16;
typedef CAUUID16 TYP16_CAUUID;
typedef SEGPTR TYP16_LPCONNECTIONPOINT;
typedef SEGPTR TYP16_LPSTREAM;
typedef POINT16 TYP16_POINT;
struct TYP16__RECTL
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};
typedef TEXTMETRIC16 TYP16_TEXTMETRICOLE;

typedef struct TYP16_tagCONNECTDATA
{
    /*LPUNKNOWN*/SEGPTR pUnk;
    DWORD dwCookie;

} CONNECTDATA16;
#define INMAP_LPOLESTR16_32(a32, a16) FIXME("INMAP_LPOLESTR16_32\n")
#define INMAP_LPOLESTR32_16(a16, a32) FIXME("INMAP_LPOLESTR32_16\n")
#define INMAP_LPRECT16_32(a32, a16) FIXME("INMAP_LPRECT16_32\n")
#define INMAP_LPRECT32_16(a16, a32) FIXME("INMAP_LPRECT32_16\n")
#define INMAP_PTR_DISPPARAMS16_32(a32, a16) FIXME("INMAP_PTR_DISPPARAMS16_32\n")
#define INMAP_PTR_DISPPARAMS32_16(a16, a32) FIXME("INMAP_PTR_DISPPARAMS32_16\n")
#define INMAP_PTR_POINTF16_32(a32, a16) FIXME("INMAP_PTR_POINTF16_32\n")
#define INMAP_PTR_POINTF32_16(a16, a32) FIXME("INMAP_PTR_POINTF32_16\n")
#define INMAP_PTR_POINTL16_32(a32, a16) FIXME("INMAP_PTR_POINTL16_32\n")
#define INMAP_PTR_POINTL32_16(a16, a32) FIXME("INMAP_PTR_POINTL32_16\n")
#define INMAP_PTR_USHORT16_32(a32, a16) FIXME("INMAP_PTR_USHORT16_32\n")
#define INMAP_PTR_USHORT32_16(a16, a32) FIXME("INMAP_PTR_USHORT32_16\n")
#define INMAP_PTR_VARIANT16_32(a32, a16) FIXME("INMAP_PTR_VARIANT16_32\n")
#define INMAP_PTR_VARIANT32_16(a16, a32) FIXME("INMAP_PTR_VARIANT32_16\n")
#define MAP_BINDPTR16_32(a32, a16) FIXME("MAP_BINDPTR16_32\n")
#define MAP_BINDPTR32_16(a16, a32) FIXME("MAP_BINDPTR32_16\n")
#define MAP_BSTR16_32(a32, a16) FIXME("MAP_BSTR16_32\n")
#define MAP_BSTR32_16(a16, a32) FIXME("MAP_BSTR32_16\n")
#define MAP_CADWORD16_32(a32, a16) FIXME("MAP_CADWORD16_32\n")
#define MAP_CADWORD32_16(a16, a32) FIXME("MAP_CADWORD32_16\n")
#define MAP_CALPOLESTR16_32(a32, a16) FIXME("MAP_CALPOLESTR16_32\n")
#define MAP_CALPOLESTR32_16(a16, a32) FIXME("MAP_CALPOLESTR32_16\n")
#define MAP_CAUUID16_32(a32, a16) FIXME("MAP_CAUUID16_32\n")
#define MAP_CAUUID32_16(a16, a32) FIXME("MAP_CAUUID32_16\n")
#define MAP_CONTROLINFO16_32(a32, a16) FIXME("MAP_CONTROLINFO16_32\n")
#define MAP_CONTROLINFO32_16(a16, a32) FIXME("MAP_CONTROLINFO32_16\n")
#define MAP_CY16_32(a32, a16) FIXME("MAP_CY16_32\n")
#define MAP_CY32_16(a16, a32) FIXME("MAP_CY32_16\n")
#define MAP_DESCKIND16_32(a32, a16) FIXME("MAP_DESCKIND16_32\n")
#define MAP_DESCKIND32_16(a16, a32) FIXME("MAP_DESCKIND32_16\n")
#define MAP_DISPID16_32(a32, a16) FIXME("MAP_DISPID16_32\n")
#define MAP_DISPID32_16(a16, a32) FIXME("MAP_DISPID32_16\n")
#define MAP_DISPPARAMS16_32(a32, a16) FIXME("MAP_DISPPARAMS16_32\n")
#define MAP_DISPPARAMS32_16(a16, a32) FIXME("MAP_DISPPARAMS32_16\n")
#define MAP_EXCEPINFO16_32(a32, a16) FIXME("MAP_EXCEPINFO16_32\n")
#define MAP_EXCEPINFO32_16(a16, a32) FIXME("MAP_EXCEPINFO32_16\n")
#define MAP_HFONT16_32(a32, a16) FIXME("MAP_HFONT16_32\n")
#define MAP_HFONT32_16(a16, a32) FIXME("MAP_HFONT32_16\n")
#define MAP_HREFTYPE16_32(a32, a16) FIXME("MAP_HREFTYPE16_32\n")
#define MAP_HREFTYPE32_16(a16, a32) FIXME("MAP_HREFTYPE32_16\n")
#define MAP_HRGN16_32(a32, a16) FIXME("MAP_HRGN16_32\n")
#define MAP_HRGN32_16(a16, a32) FIXME("MAP_HRGN32_16\n")
#define MAP_IID16_32(a32, a16) FIXME("MAP_IID16_32\n")
#define MAP_IID32_16(a16, a32) FIXME("MAP_IID32_16\n")
#define MAP_INVOKEKIND16_32(a32, a16) FIXME("MAP_INVOKEKIND16_32\n")
#define MAP_INVOKEKIND32_16(a16, a32) FIXME("MAP_INVOKEKIND32_16\n")
#define MAP_LCID16_32(a32, a16) FIXME("MAP_LCID16_32\n")
#define MAP_LCID32_16(a16, a32) FIXME("MAP_LCID32_16\n")
#define MAP_LICINFO16_32(a32, a16) FIXME("MAP_LICINFO16_32\n")
#define MAP_LICINFO32_16(a16, a32) FIXME("MAP_LICINFO32_16\n")
#define MAP_LPOLESTR_PTR16_32(a32, a16) FIXME("MAP_LPOLESTR_PTR16_32\n")
#define MAP_LPOLESTR_PTR32_16(a16, a32) FIXME("MAP_LPOLESTR_PTR32_16\n")
#define MAP_MEMBERID16_32(a32, a16) FIXME("MAP_MEMBERID16_32\n")
#define MAP_MEMBERID32_16(a16, a32) FIXME("MAP_MEMBERID32_16\n")
#define MAP_OLECHAR16_32(a32, a16) FIXME("MAP_OLECHAR16_32\n")
#define MAP_OLECHAR32_16(a16, a32) FIXME("MAP_OLECHAR32_16\n")
#define MAP_OLE_HANDLE16_32(a32, a16) FIXME("MAP_OLE_HANDLE16_32\n")
#define MAP_OLE_HANDLE32_16(a16, a32) FIXME("MAP_OLE_HANDLE32_16\n")
#define MAP_OLE_XPOS_HIMETRIC16_32(a32, a16) FIXME("MAP_OLE_XPOS_HIMETRIC16_32\n")
#define MAP_OLE_XPOS_HIMETRIC32_16(a16, a32) FIXME("MAP_OLE_XPOS_HIMETRIC32_16\n")
#define MAP_OLE_XSIZE_HIMETRIC16_32(a32, a16) FIXME("MAP_OLE_XSIZE_HIMETRIC16_32\n")
#define MAP_OLE_XSIZE_HIMETRIC32_16(a16, a32) FIXME("MAP_OLE_XSIZE_HIMETRIC32_16\n")
#define MAP_OLE_YPOS_HIMETRIC16_32(a32, a16) FIXME("MAP_OLE_YPOS_HIMETRIC16_32\n")
#define MAP_OLE_YPOS_HIMETRIC32_16(a16, a32) FIXME("MAP_OLE_YPOS_HIMETRIC32_16\n")
#define MAP_OLE_YSIZE_HIMETRIC16_32(a32, a16) FIXME("MAP_OLE_YSIZE_HIMETRIC16_32\n")
#define MAP_OLE_YSIZE_HIMETRIC32_16(a16, a32) FIXME("MAP_OLE_YSIZE_HIMETRIC32_16\n")
#define MAP_POINT16_32(a32, a16) FIXME("MAP_POINT16_32\n")
#define MAP_POINT32_16(a16, a32) FIXME("MAP_POINT32_16\n")
#define MAP_POINTF16_32(a32, a16) FIXME("MAP_POINTF16_32\n")
#define MAP_POINTF32_16(a16, a32) FIXME("MAP_POINTF32_16\n")
#define MAP_PROPPAGEINFO16_32(a32, a16) FIXME("MAP_PROPPAGEINFO16_32\n")
#define MAP_PROPPAGEINFO32_16(a16, a32) FIXME("MAP_PROPPAGEINFO32_16\n")
#define MAP_PTR_DVEXTENTINFO16_32(a32, a16) FIXME("MAP_PTR_DVEXTENTINFO16_32\n")
#define MAP_PTR_DVEXTENTINFO32_16(a16, a32) FIXME("MAP_PTR_DVEXTENTINFO32_16\n")
#define MAP_PTR_EXCEPINFO16_32(a32, a16) FIXME("MAP_PTR_EXCEPINFO16_32\n")
#define MAP_PTR_EXCEPINFO32_16(a16, a32) FIXME("MAP_PTR_EXCEPINFO32_16\n")
#define MAP_PTR_FUNCDESC16_32(a32, a16) FIXME("MAP_PTR_FUNCDESC16_32\n")
#define MAP_PTR_FUNCDESC32_16(a16, a32) FIXME("MAP_PTR_FUNCDESC32_16\n")
#define MAP_PTR_HREFTYPE16_32(a32, a16) FIXME("MAP_PTR_HREFTYPE16_32\n")
#define MAP_PTR_HREFTYPE32_16(a16, a32) FIXME("MAP_PTR_HREFTYPE32_16\n")
#define MAP_PTR_IDLDESC16_32(a32, a16) FIXME("MAP_PTR_IDLDESC16_32\n")
#define MAP_PTR_IDLDESC32_16(a16, a32) FIXME("MAP_PTR_IDLDESC32_16\n")
#define MAP_PTR_IFACE_IUnknown_PTR16_32(a32, a16) FIXME("MAP_PTR_IFACE_IUnknown_PTR16_32\n")
#define MAP_PTR_IFACE_IUnknown_PTR32_16(a16, a32) FIXME("MAP_PTR_IFACE_IUnknown_PTR32_16\n")
#define MAP_PTR_MSG16_32(a32, a16) FIXME("MAP_PTR_MSG16_32\n")
#define MAP_PTR_MSG32_16(a16, a32) FIXME("MAP_PTR_MSG32_16\n")
#define MAP_PTR_TLIBATTR16_32(a32, a16) FIXME("MAP_PTR_TLIBATTR16_32\n")
#define MAP_PTR_TLIBATTR32_16(a16, a32) FIXME("MAP_PTR_TLIBATTR32_16\n")
#define MAP_PTR_TYPEATTR16_32(a32, a16) FIXME("MAP_PTR_TYPEATTR16_32\n")
#define MAP_PTR_TYPEATTR32_16(a16, a32) FIXME("MAP_PTR_TYPEATTR32_16\n")
#define MAP_PTR_TYPEDESC16_32(a32, a16) FIXME("MAP_PTR_TYPEDESC16_32\n")
#define MAP_PTR_TYPEDESC32_16(a16, a32) FIXME("MAP_PTR_TYPEDESC32_16\n")
#define MAP_PTR_VARDESC16_32(a32, a16) FIXME("MAP_PTR_VARDESC16_32\n")
#define MAP_PTR_VARDESC32_16(a16, a32) FIXME("MAP_PTR_VARDESC32_16\n")
#define MAP_PTR_VARIANT16_32(a32, a16) FIXME("MAP_PTR_VARIANT16_32\n")
#define MAP_PTR_VARIANT32_16(a16, a32) FIXME("MAP_PTR_VARIANT32_16\n")
#define MAP_PVOID16_32(a32, a16) FIXME("MAP_PVOID16_32\n")
#define MAP_PVOID32_16(a16, a32) FIXME("MAP_PVOID32_16\n")
#define MAP_REFGUID16_32(a32, a16) FIXME("MAP_REFGUID16_32\n")
#define MAP_REFGUID32_16(a16, a32) FIXME("MAP_REFGUID32_16\n")
#define MAP_SHORT16_32(a32, a16) FIXME("MAP_SHORT16_32\n")
#define MAP_SHORT32_16(a16, a32) FIXME("MAP_SHORT32_16\n")
#define MAP_STRUCT__RECTL16_32(a32, a16) FIXME("MAP_STRUCT__RECTL16_32\n")
#define MAP_STRUCT__RECTL32_16(a16, a32) FIXME("MAP_STRUCT__RECTL32_16\n")
#define MAP_STRUCT_tagCONNECTDATA16_32(a32, a16) FIXME("MAP_STRUCT_tagCONNECTDATA16_32\n")
#define MAP_STRUCT_tagCONNECTDATA32_16(a16, a32) FIXME("MAP_STRUCT_tagCONNECTDATA32_16\n")
#define MAP_TEXTMETRICOLE16_32(a32, a16) FIXME("MAP_TEXTMETRICOLE16_32\n")
#define MAP_TEXTMETRICOLE32_16(a16, a32) FIXME("MAP_TEXTMETRICOLE32_16\n")
#define MAP_TYPEKIND16_32(a32, a16) FIXME("MAP_TYPEKIND16_32\n")
#define MAP_TYPEKIND32_16(a16, a32) FIXME("MAP_TYPEKIND32_16\n")
#define MAP_UINT16_32(a32, a16) FIXME("MAP_UINT16_32\n")
#define MAP_UINT32_16(a16, a32) FIXME("MAP_UINT32_16\n")
#define MAP_USHORT16_32(a32, a16) FIXME("MAP_USHORT16_32\n")
#define MAP_USHORT32_16(a16, a32) FIXME("MAP_USHORT32_16\n")
#define MAP_VARIANT16_32(a32, a16) FIXME("MAP_VARIANT16_32\n")
#define MAP_VARIANT32_16(a16, a32) FIXME("MAP_VARIANT32_16\n")
#define UNMAP_BSTR16_32(a32, a16) FIXME("UNMAP_BSTR16_32\n")
#define UNMAP_BSTR32_16(a16, a32) FIXME("UNMAP_BSTR32_16\n")
#define UNMAP_CY16_32(a32, a16) FIXME("UNMAP_CY16_32\n")
#define UNMAP_CY32_16(a16, a32) FIXME("UNMAP_CY32_16\n")
#define UNMAP_DISPID16_32(a32, a16) FIXME("UNMAP_DISPID16_32\n")
#define UNMAP_DISPID32_16(a16, a32) FIXME("UNMAP_DISPID32_16\n")
#define UNMAP_HFONT16_32(a32, a16) FIXME("UNMAP_HFONT16_32\n")
#define UNMAP_HFONT32_16(a16, a32) FIXME("UNMAP_HFONT32_16\n")
#define UNMAP_HREFTYPE16_32(a32, a16) FIXME("UNMAP_HREFTYPE16_32\n")
#define UNMAP_HREFTYPE32_16(a16, a32) FIXME("UNMAP_HREFTYPE32_16\n")
#define UNMAP_HRGN16_32(a32, a16) FIXME("UNMAP_HRGN16_32\n")
#define UNMAP_HRGN32_16(a16, a32) FIXME("UNMAP_HRGN32_16\n")
#define UNMAP_INT16_32(a32, a16) FIXME("UNMAP_INT16_32\n")
#define UNMAP_INT32_16(a16, a32) FIXME("UNMAP_INT32_16\n")
#define UNMAP_INVOKEKIND16_32(a32, a16) FIXME("UNMAP_INVOKEKIND16_32\n")
#define UNMAP_INVOKEKIND32_16(a16, a32) FIXME("UNMAP_INVOKEKIND32_16\n")
#define UNMAP_LCID16_32(a32, a16) FIXME("UNMAP_LCID16_32\n")
#define UNMAP_LCID32_16(a16, a32) FIXME("UNMAP_LCID32_16\n")
#define UNMAP_LPOLESTR_PTR16_32(a32, a16) FIXME("UNMAP_LPOLESTR_PTR16_32\n")
#define UNMAP_LPOLESTR_PTR32_16(a16, a32) FIXME("UNMAP_LPOLESTR_PTR32_16\n")
#define UNMAP_LPRECT16_32(a32, a16) FIXME("UNMAP_LPRECT16_32\n")
#define UNMAP_LPRECT32_16(a16, a32) FIXME("UNMAP_LPRECT32_16\n")
#define UNMAP_MEMBERID16_32(a32, a16) FIXME("UNMAP_MEMBERID16_32\n")
#define UNMAP_MEMBERID32_16(a16, a32) FIXME("UNMAP_MEMBERID32_16\n")
#define UNMAP_OLE_HANDLE16_32(a32, a16) FIXME("UNMAP_OLE_HANDLE16_32\n")
#define UNMAP_OLE_HANDLE32_16(a16, a32) FIXME("UNMAP_OLE_HANDLE32_16\n")
#define UNMAP_OLE_XPOS_HIMETRIC16_32(a32, a16) FIXME("UNMAP_OLE_XPOS_HIMETRIC16_32\n")
#define UNMAP_OLE_XPOS_HIMETRIC32_16(a16, a32) FIXME("UNMAP_OLE_XPOS_HIMETRIC32_16\n")
#define UNMAP_OLE_XSIZE_HIMETRIC16_32(a32, a16) FIXME("UNMAP_OLE_XSIZE_HIMETRIC16_32\n")
#define UNMAP_OLE_XSIZE_HIMETRIC32_16(a16, a32) FIXME("UNMAP_OLE_XSIZE_HIMETRIC32_16\n")
#define UNMAP_OLE_YPOS_HIMETRIC16_32(a32, a16) FIXME("UNMAP_OLE_YPOS_HIMETRIC16_32\n")
#define UNMAP_OLE_YPOS_HIMETRIC32_16(a16, a32) FIXME("UNMAP_OLE_YPOS_HIMETRIC32_16\n")
#define UNMAP_OLE_YSIZE_HIMETRIC16_32(a32, a16) FIXME("UNMAP_OLE_YSIZE_HIMETRIC16_32\n")
#define UNMAP_OLE_YSIZE_HIMETRIC32_16(a16, a32) FIXME("UNMAP_OLE_YSIZE_HIMETRIC32_16\n")
#define UNMAP_POINT16_32(a32, a16) FIXME("UNMAP_POINT16_32\n")
#define UNMAP_POINT32_16(a16, a32) FIXME("UNMAP_POINT32_16\n")
#define UNMAP_PTR_DISPPARAMS16_32(a32, a16) FIXME("UNMAP_PTR_DISPPARAMS16_32\n")
#define UNMAP_PTR_DISPPARAMS32_16(a16, a32) FIXME("UNMAP_PTR_DISPPARAMS32_16\n")
#define UNMAP_PTR_DVEXTENTINFO16_32(a32, a16) FIXME("UNMAP_PTR_DVEXTENTINFO16_32\n")
#define UNMAP_PTR_DVEXTENTINFO32_16(a16, a32) FIXME("UNMAP_PTR_DVEXTENTINFO32_16\n")
#define UNMAP_PTR_EXCEPINFO16_32(a32, a16) FIXME("UNMAP_PTR_EXCEPINFO16_32\n")
#define UNMAP_PTR_EXCEPINFO32_16(a16, a32) FIXME("UNMAP_PTR_EXCEPINFO32_16\n")
#define UNMAP_PTR_FUNCDESC16_32(a32, a16) FIXME("UNMAP_PTR_FUNCDESC16_32\n")
#define UNMAP_PTR_FUNCDESC32_16(a16, a32) FIXME("UNMAP_PTR_FUNCDESC32_16\n")
#define UNMAP_PTR_HREFTYPE16_32(a32, a16) FIXME("UNMAP_PTR_HREFTYPE16_32\n")
#define UNMAP_PTR_HREFTYPE32_16(a16, a32) FIXME("UNMAP_PTR_HREFTYPE32_16\n")
#define UNMAP_PTR_IDLDESC16_32(a32, a16) FIXME("UNMAP_PTR_IDLDESC16_32\n")
#define UNMAP_PTR_IDLDESC32_16(a16, a32) FIXME("UNMAP_PTR_IDLDESC32_16\n")
#define UNMAP_PTR_IFACE_IUnknown_PTR16_32(a32, a16) FIXME("UNMAP_PTR_IFACE_IUnknown_PTR16_32\n")
#define UNMAP_PTR_IFACE_IUnknown_PTR32_16(a16, a32) FIXME("UNMAP_PTR_IFACE_IUnknown_PTR32_16\n")
#define UNMAP_PTR_MSG16_32(a32, a16) FIXME("UNMAP_PTR_MSG16_32\n")
#define UNMAP_PTR_MSG32_16(a16, a32) FIXME("UNMAP_PTR_MSG32_16\n")
#define UNMAP_PTR_POINTF16_32(a32, a16) FIXME("UNMAP_PTR_POINTF16_32\n")
#define UNMAP_PTR_POINTF32_16(a16, a32) FIXME("UNMAP_PTR_POINTF32_16\n")
#define UNMAP_PTR_POINTL16_32(a32, a16) FIXME("UNMAP_PTR_POINTL16_32\n")
#define UNMAP_PTR_POINTL32_16(a16, a32) FIXME("UNMAP_PTR_POINTL32_16\n")
#define UNMAP_PTR_TLIBATTR16_32(a32, a16) FIXME("UNMAP_PTR_TLIBATTR16_32\n")
#define UNMAP_PTR_TLIBATTR32_16(a16, a32) FIXME("UNMAP_PTR_TLIBATTR32_16\n")
#define UNMAP_PTR_TYPEATTR16_32(a32, a16) FIXME("UNMAP_PTR_TYPEATTR16_32\n")
#define UNMAP_PTR_TYPEATTR32_16(a16, a32) FIXME("UNMAP_PTR_TYPEATTR32_16\n")
#define UNMAP_PTR_TYPEDESC16_32(a32, a16) FIXME("UNMAP_PTR_TYPEDESC16_32\n")
#define UNMAP_PTR_TYPEDESC32_16(a16, a32) FIXME("UNMAP_PTR_TYPEDESC32_16\n")
#define UNMAP_PTR_USHORT16_32(a32, a16) FIXME("UNMAP_PTR_USHORT16_32\n")
#define UNMAP_PTR_USHORT32_16(a16, a32) FIXME("UNMAP_PTR_USHORT32_16\n")
#define UNMAP_PTR_VARDESC16_32(a32, a16) FIXME("UNMAP_PTR_VARDESC16_32\n")
#define UNMAP_PTR_VARDESC32_16(a16, a32) FIXME("UNMAP_PTR_VARDESC32_16\n")
#define UNMAP_PTR_VARIANT16_32(a32, a16) FIXME("UNMAP_PTR_VARIANT16_32\n")
#define UNMAP_PTR_VARIANT32_16(a16, a32) FIXME("UNMAP_PTR_VARIANT32_16\n")
#define UNMAP_PVOID16_32(a32, a16) FIXME("UNMAP_PVOID16_32\n")
#define UNMAP_PVOID32_16(a16, a32) FIXME("UNMAP_PVOID32_16\n")
#define UNMAP_REFGUID16_32(a32, a16) FIXME("UNMAP_REFGUID16_32\n")
#define UNMAP_REFGUID32_16(a16, a32) FIXME("UNMAP_REFGUID32_16\n")
#define UNMAP_SHORT16_32(a32, a16) FIXME("UNMAP_SHORT16_32\n")
#define UNMAP_SHORT32_16(a16, a32) FIXME("UNMAP_SHORT32_16\n")
#define UNMAP_TYPEKIND16_32(a32, a16) FIXME("UNMAP_TYPEKIND16_32\n")
#define UNMAP_TYPEKIND32_16(a16, a32) FIXME("UNMAP_TYPEKIND32_16\n")
#define UNMAP_UINT16_32(a32, a16) FIXME("UNMAP_UINT16_32\n")
#define UNMAP_UINT32_16(a16, a32) FIXME("UNMAP_UINT32_16\n")

#define IFS3216_OVERWRITE_IOleInPlaceSiteWindowless_OnDefWindowMessage
HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_32_16_OnDefWindowMessage(IOleInPlaceSiteWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
#define IFS1632_OVERWRITE_IOleInPlaceSiteWindowless_OnDefWindowMessage
HRESULT CDECL IOleInPlaceSiteWindowless_16_32_OnDefWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult);
#define IFS3216_OVERWRITE_IOleInPlaceObjectWindowless_OnWindowMessage
HRESULT STDMETHODCALLTYPE IOleInPlaceObjectWindowless_32_16_OnWindowMessage(IOleInPlaceObjectWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
#define IFS1632_OVERWRITE_IOleInPlaceObjectWindowless_OnWindowMessage
HRESULT CDECL IOleInPlaceObjectWindowless_16_32_OnWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult);
#define IFS1632_OVERWRITE_ISimpleFrameSite_PreMessageFilter
HRESULT CDECL ISimpleFrameSite_16_32_PreMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, SEGPTR args16_pdwCookie);
#define IFS3216_OVERWRITE_ISimpleFrameSite_PreMessageFilter
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PreMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD *pdwCookie);
#define IFS1632_OVERWRITE_ISimpleFrameSite_PostMessageFilter
HRESULT CDECL ISimpleFrameSite_16_32_PostMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, DWORD args16_dwCookie);
#define IFS3216_OVERWRITE_ISimpleFrameSite_PostMessageFilter
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PostMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD dwCookie);
#include "../ole2disp/ole2disp.h"
typedef VARIANT16 TYP16_VARIANT;
typedef OLECHAR16 TYP16_OLECHAR;
#include <poppack.h>
#endif
