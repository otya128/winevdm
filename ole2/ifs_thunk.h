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
#include "../ole2disp/ole2disp.h"

#ifndef IFS_THUNK_NDEF_DBG_CHANNEL
#include "wine/debug.h"
WINE_DEFAULT_DEBUG_CHANNEL(ole);
#endif
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
    void *vtable_copy;
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
    BOOL is_stdcall;
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
void *copy_iface16_vtbl(SEGPTR iface16);
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
#define IFS1632_OVERWRITE_ISTGMEDIUMRelease_Release
ULONG CDECL ISTGMEDIUMRelease_16_32_Release(SEGPTR This);
#define IFS3216_OVERWRITE_ISTGMEDIUMRelease_Release
extern const IID IID_ISTGMEDIUMRelease;
typedef IUnknown ISTGMEDIUMRelease;
typedef IUnknownVtbl ISTGMEDIUMReleaseVtbl;
ULONG WINAPI ISTGMEDIUMRelease_32_16_Release(ISTGMEDIUMRelease *iface);
#define MAP_STGMEDIUM32_16(a16, a32) map_stgmedium32_16((STGMEDIUM16*)&a16, &a32)
#define MAP_STGMEDIUM16_32(a32, a16) map_stgmedium16_32(&a32, &a16)
#define UNMAP_IID_PTR16_32
#define UNMAP_IID_PTR32_16
#define UNMAP_REFCLSID32_16
#define UNMAP_REFCLSID16_32
#define UNMAP_PTR_FORMATETC32_16(a16, a32) \
	if (((FORMATETC16 *)MapSL(a16))->ptd) { \
		HeapFree(GetProcessHeap(), 0, MapSL(((FORMATETC16 *)MapSL(a16))->ptd)); \
		UnMapLS(((FORMATETC16 *)MapSL(a16))->ptd); \
		UnMapLS(a16); \
	}
#define UNMAP_PTR_FORMATETC16_32(a32, a16) if (a32->ptd) HeapFree(GetProcessHeap(), 0, a32->ptd);
#define UNMAP_STGMEDIUM32_16
#define UNMAP_STGMEDIUM16_32
#define UNMAP_LPINTERFACEINFO32_16
#define UNMAP_LPINTERFACEINFO16_32
#define UNMAP_PTR_STGMEDIUM32_16
#define UNMAP_PTR_STGMEDIUM16_32
#define MAP_PTR_FORMATETC16_32(a32, a16) map_pformatetc16_32(a32 = (FORMATETC*)alloca(sizeof(FORMATETC)), (FORMATETC16*)MapSL(a16))
#define MAP_PTR_FORMATETC32_16(a16, a32) map_pformatetc32_16((FORMATETC16*)MapSL(a16 = MapLS(alloca(sizeof(FORMATETC16)))), a32);

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


void map_interfaceinfo16_32(INTERFACEINFO *a32, const INTERFACEINFO16 *a16);
#define MAP_LPINTERFACEINFO16_32(a32, a16) if (a16) map_interfaceinfo16_32(a32 = (INTERFACEINFO*)alloca(sizeof(INTERFACEINFO)), (INTERFACEINFO16*)MapSL(a16)); else a32 = NULL
void map_interfaceinfo32_16(INTERFACEINFO16 *a16, const INTERFACEINFO *a32);
#define MAP_LPINTERFACEINFO32_16(a16, a32) if (a32) map_interfaceinfo32_16((INTERFACEINFO16*)MapSL(a16 = MapLS(alloca(sizeof(INTERFACEINFO16)))), a32); else a16 = 0

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

#define MAP_STRUCT_tagOleInPlaceFrameInfo32_16(a16, a32) map_oleinplaceframeinfo32_16((struct TYP16_tagOleInPlaceFrameInfo*)&a16, &a32)
#define MAP_STRUCT_tagOleInPlaceFrameInfo16_32(a32, a16) map_oleinplaceframeinfo16_32(&a32, &a16)
#define MAP_SIZE16_32(a32, a16) \
    (a32).cx = a16.cx;\
    (a32).cy = a16.cy
#define MAP_SIZE32_16(a16, a32) \
    ((SIZE16*)&a16)->cx = a32.cx;\
    ((SIZE16*)&a16)->cy = a32.cy

#define MAP_PTR_LOGPALETTE16_32(a32, a16) a32 = (LOGPALETTE*)MapSL(a16)
#define MAP_PTR_LOGPALETTE32_16(a16, a32) a16 = MapLS(a32)
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
#define MAP_LPCRECT16_32(a32, a16) \
    if (a16 == 0)\
        a32 = NULL;\
    else\
        map_rect16_32(a32 = (LPCRECT*)alloca(sizeof(RECT)), (RECT16*)MapSL(a16))
#define MAP_LPCBORDERWIDTHS16_32 MAP_LPCRECT16_32
#define MAP_LPCRECT32_16(a16, a32) \
    if (a32 == NULL)\
        a16 = 0;\
    else\
        map_rect32_16(MapSL(a16 = MapLS((const RECT16*)alloca(sizeof(RECT16)))), (RECT*)a32)
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

static inline void map_statdata32_16(STATDATA16* a16, const STATDATA *a32)
{
    map_formatetc32_16(&a16->formatetc, &a32->formatetc);
    a16->advf = a32->advf;
    a16->pAdvSink = (IAdviseSink*)iface32_16(&IID_IAdviseSink, a32->pAdvSink);
    a16->dwConnection = a32->dwConnection;
}
static inline void map_statdata16_32(STATDATA* a32, const STATDATA16 *a16)
{
    map_formatetc16_32(&a32->formatetc, &a16->formatetc);
    a32->advf = a16->advf;
    a32->pAdvSink = iface16_32(&IID_IAdviseSink, a16->pAdvSink);
    a32->dwConnection = a16->dwConnection;
}
#define MAP_STATDATA32_16(a16, a32) map_statdata32_16(&a16, &a32)
#define MAP_STATDATA16_32(a32, a16) FIXME("\n");

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

void map_oleverb16_32(OLEVERB* a32, const OLEVERB16 *a16);
void map_oleverb32_16(OLEVERB16* a16, const OLEVERB *a32);
#define MAP_STRUCT_tagOLEVERB16_32(a32, a16) map_oleverb16_32(&a32, &a16)
#define MAP_STRUCT_tagOLEVERB32_16(a16, a32) map_oleverb32_16(&a16, &a32)
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
#define INMAP_PTR_PTR_IFACE_IMoniker16_32(a32, a16) a32 = (IMoniker*)iface16_32(&IID_IMoniker, *(SEGPTR*)MapSL(a16))
#define OUTMAP_PTR_IFACE_IMoniker32_16(a16, a32) a16 = iface32_16(&IID_IMoniker, a32)
#define INMAP_PTR_PTR_IFACE_IMoniker32_16(a16, a32) FIXME("\n")
#define INMAP_PTR_STGMEDIUM16_32(a32, a16) map_stgmedium16_32(&a32, (const STGMEDIUM16*)MapSL(a16))
#define INMAP_PTR_STGMEDIUM32_16(a16, a32) map_stgmedium32_16(&a16, a32)
#define INMAP_LPOLEMENUGROUPWIDTHS16_32(a32, a16) *(LPOLEMENUGROUPWIDTHS)&a32 = *(LPOLEMENUGROUPWIDTHS)MapSL(a16)
#define INMAP_LPOLEMENUGROUPWIDTHS32_16(a16, a32) *(LPOLEMENUGROUPWIDTHS)&a16 = *a32
#define INMAP_LPOLEINPLACEFRAMEINFO16_32(a32, a16) map_oleinplaceframeinfo16_32(&a32, (struct TYP16_tagOleInPlaceFrameInfo*)MapSL(a16))
#define INMAP_LPOLEINPLACEFRAMEINFO32_16(a16, a32) map_oleinplaceframeinfo32_16(&a16, a32)
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
#define MAP_INT16_32(a32, a16) a32 = (INT)(INT16)a16
#define MAP_INT32_16(a16, a32) a16 = (INT16)(INT)a32
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
typedef WORD TYP16_DESCKIND;
typedef WORD TYP16_INVOKEKIND;
typedef SEGPTR TYP16_BSTR;
typedef HREFTYPE TYP16_HREFTYPE;
typedef SEGPTR TYP16_REFGUID;
typedef DISPPARAMS16 TYP16_DISPPARAMS;
typedef struct tagEXCEPINFO16 {
	unsigned short wCode;
	unsigned short wReserved;
	TYP16_BSTR bstrSource;
	TYP16_BSTR bstrDescription;
	TYP16_BSTR bstrHelpFile;
	unsigned long dwHelpContext;
	/* void* */SEGPTR pvReserved;	
	SEGPTR pfnDeferredFillIn/* HRESULT (*pfnDeferredFillIn)(EXCEPINFO * pexcepinfo) */;
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
typedef WORD TYP16_TYPEKIND;
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
typedef struct TYP16_tagTYPEDESC
{
    union
    {
        /* VT_PTR */
        /*struct TYP16_tagTYPEDESC* */SEGPTR lptdesc;
        /* VT_CARRAY */
        /* struct TYP16_tagARRAYDESC* */ SEGPTR  lpadesc;
        /* VT_USERDEFINED */
        HREFTYPE hreftype;
    } DUMMYUNIONNAME;
    VARTYPE vt;
} TYPEDESC16;

typedef struct TYP16_tagIDLDESC
{
    /* BSTR */SEGPTR bstrIDLInfo;
    unsigned short wIDLFlags;
} IDLDESC16;

typedef struct TYP16_tagELEMDESC
{
    TYPEDESC16 tdesc;
    IDLDESC16 idldesc;
}  ELEMDESC16;

typedef struct TYP16_tagFUNCDESC
{
    MEMBERID memid;
    /* SCODE* */SEGPTR lprgscode;
    /* ELEMDESC* */SEGPTR lprgelemdescParam;
    /* FUNCKIND */WORD funckind;
    /* INVOKEKIND */WORD invkind;
    /* CALLCONV */WORD callconv;
    short cParams;
    short cParamsOpt;
    short oVft;
    short cScodes;
    ELEMDESC16 elemdescFunc;
    unsigned short wFuncFlags;
}  FUNCDESC16;

typedef struct TYP16_tagARRAYDESC16
{
    TYPEDESC16 tdescElem;
    USHORT cDims;
    SAFEARRAYBOUND16 rgbounds[1];
} ARRAYDESC16;

typedef struct TYP16_tagTYPEATTR
{
    GUID guid;
    LCID lcid;
    unsigned long dwReserved;
    MEMBERID memidConstructor;
    MEMBERID memidDestructor;
    /* char * */SEGPTR lpstrSchema;/* reserved */
    unsigned long cbSizeInstance;
    /* TYPEKIND */WORD typekind;
    unsigned short cFuncs;
    unsigned short cVars;
    unsigned short cImplTypes;
    unsigned short cbSizeVft;
    unsigned short cbAlignment;
    unsigned short wTypeFlags;
    unsigned short wMajorVerNum;
    unsigned short wMinorVerNum;
    TYPEDESC16 tdescAlias;
    IDLDESC16 idldescType;
} TYPEATTR16;

typedef struct TYP16_tagVARDESC
{
    MEMBERID memid;
    /* char* */SEGPTR lpstrSchema;
    union
    {
        /* VAR_PERINSTANCE */
        unsigned long oInst;
        /* VAR_CONST */
        /* VARIANT* */SEGPTR lpvarValue;

    } DUMMYUNIONNAME;
    ELEMDESC16 elemdescVar;
    unsigned short wVarFlags;
    /* VARKIND */WORD varkind;
} VARDESC16;

#define INMAP_LPOLESTR16_32(a32, a16) FIXME("INMAP_LPOLESTR16_32\n")
#define INMAP_LPOLESTR32_16(a16, a32) FIXME("INMAP_LPOLESTR32_16\n")
#define INMAP_LPRECT16_32(a32, a16) FIXME("INMAP_LPRECT16_32\n")
#define INMAP_LPRECT32_16(a16, a32) FIXME("INMAP_LPRECT32_16\n")
void map_dispparams32_16(DISPPARAMS16 *a16, const DISPPARAMS *a32);
void map_dispparams16_32(DISPPARAMS *a32, const DISPPARAMS16 *a16);
#define INMAP_PTR_DISPPARAMS16_32(a32, a16) map_dispparams16_32(&a32, (DISPPARAMS16*)MapSL(a16))
#define INMAP_PTR_DISPPARAMS32_16(a16, a32) map_dispparams32_16(&a16, a32)
#define INMAP_PTR_POINTF16_32(a32, a16) FIXME("INMAP_PTR_POINTF16_32\n")
#define INMAP_PTR_POINTF32_16(a16, a32) FIXME("INMAP_PTR_POINTF32_16\n")
#define INMAP_PTR_POINTL16_32(a32, a16) FIXME("INMAP_PTR_POINTL16_32\n")
#define INMAP_PTR_POINTL32_16(a16, a32) FIXME("INMAP_PTR_POINTL32_16\n")
#define INMAP_PTR_USHORT16_32(a32, a16) args32_pcFound = *(USHORT*)MapSL(args16_pcFound)
#define INMAP_PTR_USHORT32_16(a16, a32) FIXME("INMAP_PTR_USHORT32_16\n")
#define INMAP_PTR_VARIANT16_32(a32, a16) FIXME("INMAP_PTR_VARIANT16_32\n")
#define INMAP_PTR_VARIANT32_16(a16, a32) FIXME("INMAP_PTR_VARIANT32_16\n")
#define MAP_BINDPTR16_32(a32, a16) FIXME("MAP_BINDPTR16_32\n")
#define MAP_BINDPTR32_16(a16, a32) FIXME("MAP_BINDPTR32_16\n")
void map_bstr16_32(BSTR *a32, const SEGPTR *a16);
#define MAP_BSTR16_32(a32, a16) map_bstr16_32(&a32, &a16)
void map_bstr32_16(SEGPTR *a16, const BSTR *a32);
#define MAP_BSTR32_16(a16, a32) map_bstr32_16(&a16, &a32)
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
#define MAP_DESCKIND16_32(a32, a16) a32 = a16
#define MAP_DESCKIND32_16(a16, a32) a16 = a32
#define MAP_DISPID16_32(a32, a16) a32 = a16
#define MAP_DISPID32_16(a16, a32) a16 = a32
#define MAP_DISPPARAMS16_32(a32, a16) map_dispparams16_32(&a32, &a16)
#define MAP_DISPPARAMS32_16(a16, a32) map_dispparams32_16(&a16, &a32)
void map_excepinfo16_32(EXCEPINFO *a32, const EXCEPINFO16 *a16);
void map_excepinfo32_16(EXCEPINFO16 *a16, const EXCEPINFO *a32);
#define MAP_EXCEPINFO16_32(a32, a16) map_excepinfo16_32(&a32, &a16)
#define MAP_EXCEPINFO32_16(a16, a32) map_excepinfo32_16(&a16, &a32)
#define MAP_HFONT16_32(a32, a16) FIXME("MAP_HFONT16_32\n")
#define MAP_HFONT32_16(a16, a32) FIXME("MAP_HFONT32_16\n")
#define MAP_HREFTYPE16_32(a32, a16) a32 = a16
#define MAP_HREFTYPE32_16(a16, a32) a16 = a32
#define MAP_HRGN16_32(a32, a16) FIXME("MAP_HRGN16_32\n")
#define MAP_HRGN32_16(a16, a32) FIXME("MAP_HRGN32_16\n")
#define MAP_IID16_32(a32, a16) FIXME("MAP_IID16_32\n")
#define MAP_IID32_16(a16, a32) FIXME("MAP_IID32_16\n")
#define MAP_INVOKEKIND16_32(a32, a16) a32 = a16
#define MAP_INVOKEKIND32_16(a16, a32) a16 = a32
#define MAP_LCID16_32(a32, a16) a32 = a16
#define MAP_LCID32_16(a16, a32) a16 = a32
#define MAP_LICINFO16_32(a32, a16) FIXME("MAP_LICINFO16_32\n")
#define MAP_LICINFO32_16(a16, a32) FIXME("MAP_LICINFO32_16\n")
#define MAP_LPOLESTR_PTR16_32(a32, a16) FIXME("MAP_LPOLESTR_PTR16_32\n")
#define MAP_LPOLESTR_PTR32_16(a16, a32) FIXME("MAP_LPOLESTR_PTR32_16\n")
#define MAP_MEMBERID16_32(a32, a16) a32 = a16
#define MAP_MEMBERID32_16(a16, a32) a16 = a32
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
FUNCDESC *map_funcdesc32(const FUNCDESC16 *a16);
FUNCDESC16 *map_funcdesc16(const FUNCDESC *a32);
/* GetFuncDesc -> ReleaseFuncDesc */
#define MAP_PTR_FUNCDESC16_32(a32, a16) a32 = map_funcdesc32((FUNCDESC16*)MapSL(a16))
#define MAP_PTR_FUNCDESC32_16(a16, a32) a16 = MapLS(map_funcdesc16(a32))
#define MAP_PTR_HREFTYPE16_32(a32, a16) a32 = MapSL(a16)
#define MAP_PTR_HREFTYPE32_16(a16, a32) a16 = MapLS(a32)
#define MAP_PTR_IDLDESC16_32(a32, a16) FIXME("MAP_PTR_IDLDESC16_32\n")
#define MAP_PTR_IDLDESC32_16(a16, a32) FIXME("MAP_PTR_IDLDESC32_16\n")
#define MAP_PTR_IFACE_IUnknown_PTR16_32(a32, a16) FIXME("MAP_PTR_IFACE_IUnknown_PTR16_32\n")
#define MAP_PTR_IFACE_IUnknown_PTR32_16(a16, a32) FIXME("MAP_PTR_IFACE_IUnknown_PTR32_16\n")
#define MAP_PTR_MSG16_32(a32, a16) FIXME("MAP_PTR_MSG16_32\n")
#define MAP_PTR_MSG32_16(a16, a32) FIXME("MAP_PTR_MSG32_16\n")
typedef struct
{
    GUID guid;
    LCID lcid;
    /*SYSKIND*/WORD syskind;
    unsigned short wMajorVerNum;
    unsigned short wMinorVerNum;
    unsigned short wLibFlags;
} TLIBATTR16;
void map_tlibattr16_32(TLIBATTR *a32, const TLIBATTR16 *a16);
#define MAP_PTR_TLIBATTR16_32(a32, a16) \
{\
    a32 = (TLIBATTR*)HeapAlloc(GetProcessHeap(), 0, sizeof(TLIBATTR)); /* leak */\
    map_tlibattr16_32(a32, (TLIBATTR16*)MapSL(a16));\
}
void map_tlibattr32_16(TLIBATTR16 *a16, const TLIBATTR *a32);
#define MAP_PTR_TLIBATTR32_16(a16, a32) \
{\
    TLIBATTR16 *p16 = (TLIBATTR16*)HeapAlloc(GetProcessHeap(), 0, sizeof(TLIBATTR16)); /* leak */\
    map_tlibattr32_16(p16, a32);\
    a16 = MapLS(p16);\
}
#define MAP_PTR_TYPEATTR16_32(a32, a16) FIXME("MAP_PTR_TYPEATTR16_32\n")
TYPEATTR16 *map_typeattr32_16(const TYPEATTR *a32);
#define MAP_PTR_TYPEATTR32_16(a16, a32) a16 = MapLS(map_typeattr32_16(a32))
#define MAP_PTR_TYPEDESC16_32(a32, a16) FIXME("MAP_PTR_TYPEDESC16_32\n")
#define MAP_PTR_TYPEDESC32_16(a16, a32) FIXME("MAP_PTR_TYPEDESC32_16\n")
VARDESC *map_vardesc32(const VARDESC16 *a16);
#define MAP_PTR_VARDESC16_32(a32, a16) a32 = map_vardesc32(a16)
VARDESC16 *map_vardesc16(const VARDESC *a32);
#define MAP_PTR_VARDESC32_16(a16, a32) a16 = MapLS(map_vardesc16(a32))
#define MAP_PTR_VARIANT16_32(a32, a16) FIXME("MAP_PTR_VARIANT16_32\n")
#define MAP_PTR_VARIANT32_16(a16, a32) FIXME("MAP_PTR_VARIANT32_16\n")
#define MAP_PVOID16_32(a32, a16) MAP_PTR_VOID16_32(a32, a16)
#define MAP_PVOID32_16(a16, a32) MAP_PTR_VOID32_16(a16, a32)
#define MAP_REFGUID16_32(a32, a16) *(GUID**)&a32 = (GUID*)MapSL(a16)
#define MAP_REFGUID32_16(a16, a32) a16 = MapLS(a32)
#define MAP_SHORT16_32(a32, a16) a32 = (SHORT)a16
#define MAP_SHORT32_16(a16, a32) a16 = a32
#define MAP_STRUCT__RECTL16_32(a32, a16) FIXME("MAP_STRUCT__RECTL16_32\n")
#define MAP_STRUCT__RECTL32_16(a16, a32) FIXME("MAP_STRUCT__RECTL32_16\n")
#define MAP_STRUCT_tagCONNECTDATA16_32(a32, a16) FIXME("MAP_STRUCT_tagCONNECTDATA16_32\n")
#define MAP_STRUCT_tagCONNECTDATA32_16(a16, a32) FIXME("MAP_STRUCT_tagCONNECTDATA32_16\n")
#define MAP_TEXTMETRICOLE16_32(a32, a16) FIXME("MAP_TEXTMETRICOLE16_32\n")
#define MAP_TEXTMETRICOLE32_16(a16, a32) FIXME("MAP_TEXTMETRICOLE32_16\n")
#define MAP_TYPEKIND16_32(a32, a16) a32 = a16
#define MAP_TYPEKIND32_16(a16, a32) a16 = a32
#define MAP_UINT16_32(a32, a16) a32 = a16
#define MAP_UINT32_16(a16, a32) a16 = a32
#define MAP_USHORT16_32(a32, a16) a32 = a16
#define MAP_USHORT32_16(a16, a32) a16 = a32
void map_variant16_32(VARIANT *dst, const VARIANT16 *src);
void map_variant32_16(VARIANT16 *dst, const VARIANT *src);
#define MAP_VARIANT16_32(a32, a16) map_variant16_32(&a32, &a16)
#define MAP_VARIANT32_16(a16, a32) map_variant32_16(&a16, &a32)
#define UNMAP_BSTR16_32(a32, a16) FIXME("UNMAP_BSTR16_32\n")
#define UNMAP_BSTR32_16(a16, a32) FIXME("UNMAP_BSTR32_16\n")
#define UNMAP_CY16_32(a32, a16)
#define UNMAP_CY32_16(a16, a32)
#define UNMAP_DISPID16_32(a32, a16)
#define UNMAP_DISPID32_16(a16, a32)
#define UNMAP_HFONT16_32(a32, a16)
#define UNMAP_HFONT32_16(a16, a32)
#define UNMAP_HREFTYPE16_32(a32, a16)
#define UNMAP_HREFTYPE32_16(a16, a32)
#define UNMAP_HRGN16_32(a32, a16)
#define UNMAP_HRGN32_16(a16, a32)
#define UNMAP_INT16_32(a32, a16)
#define UNMAP_INT32_16(a16, a32)
#define UNMAP_INVOKEKIND16_32(a32, a16)
#define UNMAP_INVOKEKIND32_16(a16, a32)
#define UNMAP_LCID16_32(a32, a16)
#define UNMAP_LCID32_16(a16, a32)
#define UNMAP_LPOLESTR_PTR16_32(a32, a16) FIXME("UNMAP_LPOLESTR_PTR16_32\n")
#define UNMAP_LPOLESTR_PTR32_16(a16, a32) FIXME("UNMAP_LPOLESTR_PTR32_16\n")
#define UNMAP_LPRECT16_32(a32, a16) FIXME("UNMAP_LPRECT16_32\n")
#define UNMAP_LPRECT32_16(a16, a32) FIXME("UNMAP_LPRECT32_16\n")
#define UNMAP_MEMBERID16_32(a32, a16)
#define UNMAP_MEMBERID32_16(a16, a32)
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
#define UNMAP_PTR_HREFTYPE16_32(a32, a16)
#define UNMAP_PTR_HREFTYPE32_16(a16, a32)
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
#define UNMAP_PTR_TLIBATTR16_32(a32, a16)
#define UNMAP_PTR_TLIBATTR32_16(a16, a32)
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
#define UNMAP_PVOID16_32(a32, a16)
#define UNMAP_PVOID32_16(a16, a32)
#define UNMAP_REFGUID16_32(a32, a16)
#define UNMAP_REFGUID32_16(a16, a32)
#define UNMAP_SHORT16_32(a32, a16) FIXME("UNMAP_SHORT16_32\n")
#define UNMAP_SHORT32_16(a16, a32) FIXME("UNMAP_SHORT32_16\n")
#define UNMAP_TYPEKIND16_32(a32, a16) FIXME("UNMAP_TYPEKIND16_32\n")
#define UNMAP_TYPEKIND32_16(a16, a32) FIXME("UNMAP_TYPEKIND32_16\n")
#define UNMAP_UINT16_32(a32, a16)
#define UNMAP_UINT32_16(a16, a32)

#define OUTMAP_BIND_OPTS16_32 MAP_BIND_OPTS16_32
#define OUTMAP_BIND_OPTS32_16 MAP_BIND_OPTS32_16
#define OUTMAP_DISPPARAMS16_32 MAP_DISPPARAMS16_32
#define OUTMAP_DISPPARAMS32_16 MAP_DISPPARAMS32_16
#define OUTMAP_DWORD16_32 MAP_DWORD16_32
#define OUTMAP_DWORD32_16 MAP_DWORD32_16
#define OUTMAP_OLECHAR16_32 MAP_OLECHAR16_32
#define OUTMAP_OLECHAR32_16 MAP_OLECHAR32_16
#define OUTMAP_POINTF16_32 MAP_POINTF16_32
#define OUTMAP_POINTF32_16 MAP_POINTF32_16
#define OUTMAP_POINTL16_32 MAP_POINTL16_32
#define OUTMAP_POINTL32_16 MAP_POINTL32_16
#define OUTMAP_STGMEDIUM16_32 MAP_STGMEDIUM16_32
#define OUTMAP_STGMEDIUM32_16 MAP_STGMEDIUM32_16
#define OUTMAP_STRUCT_tagOleInPlaceFrameInfo16_32 MAP_STRUCT_tagOleInPlaceFrameInfo16_32
#define OUTMAP_STRUCT_tagOleInPlaceFrameInfo32_16 MAP_STRUCT_tagOleInPlaceFrameInfo32_16
#define OUTMAP_STRUCT_tagOleMenuGroupWidths16_32 MAP_STRUCT_tagOleMenuGroupWidths16_32
#define OUTMAP_STRUCT_tagOleMenuGroupWidths32_16 MAP_STRUCT_tagOleMenuGroupWidths32_16
#define OUTMAP_STRUCT_tagRECT16_32 MAP_STRUCT_tagRECT16_32
#define OUTMAP_STRUCT_tagRECT32_16 MAP_STRUCT_tagRECT32_16
#define OUTMAP_USHORT16_32 MAP_USHORT16_32
#define OUTMAP_USHORT32_16 MAP_USHORT32_16
#define OUTMAP_VARIANT16_32 MAP_VARIANT16_32
#define OUTMAP_VARIANT32_16 MAP_VARIANT32_16


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
#define IFS1632_OVERWRITE_ITypeLib_IsName
HRESULT CDECL ITypeLib_16_32_IsName(SEGPTR This, SEGPTR args16_szNameBuf, DWORD args16_lHashVal, SEGPTR args16_pfName);
#define IFS3216_OVERWRITE_ITypeLib_IsName
HRESULT STDMETHODCALLTYPE ITypeLib_32_16_IsName(ITypeLib *This, LPOLESTR szNameBuf, ULONG lHashVal, BOOL *pfName);

#define IFS1632_OVERWRITE_ITypeInfo_ReleaseFuncDesc
void CDECL ITypeInfo_16_32_ReleaseFuncDesc(SEGPTR This, SEGPTR args16_pFuncDesc);
#define IFS3216_OVERWRITE_ITypeInfo_ReleaseFuncDesc
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseFuncDesc(ITypeInfo *This, FUNCDESC *pFuncDesc);
#define IFS1632_OVERWRITE_ITypeInfo_ReleaseTypeAttr
void CDECL ITypeInfo_16_32_ReleaseTypeAttr(SEGPTR This, SEGPTR args16_pTypeAttr);
#define IFS3216_OVERWRITE_ITypeInfo_ReleaseTypeAttr
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseTypeAttr(ITypeInfo *This, TYPEATTR *pTypeAttr);
#define IFS1632_OVERWRITE_ITypeComp_Bind
HRESULT CDECL ITypeComp_16_32_Bind(SEGPTR This, SEGPTR args16_szName, DWORD args16_lHashVal, WORD args16_wFlags, SEGPTR args16_ppTInfo, SEGPTR args16_pDescKind, SEGPTR args16_pBindPtr);
#define IFS3216_OVERWRITE_ITypeComp_Bind
HRESULT STDMETHODCALLTYPE ITypeComp_32_16_Bind(ITypeComp *This, LPOLESTR szName, ULONG lHashVal, WORD wFlags, ITypeInfo **ppTInfo, DESCKIND *pDescKind, BINDPTR *pBindPtr);

#define IFS1632_OVERWRITE_ITypeInfo_ReleaseVarDesc
void CDECL ITypeInfo_16_32_ReleaseVarDesc(SEGPTR This, SEGPTR args16_pVarDesc);
#define IFS3216_OVERWRITE_ITypeInfo_ReleaseVarDesc
void STDMETHODCALLTYPE ITypeInfo_32_16_ReleaseVarDesc(ITypeInfo *This, VARDESC *pVarDesc);

typedef struct tagSIZEL
{
    LONG cx;
    LONG cy;
};
#define TYP16_tagSIZEL tagSIZEL
#define MAP_STRUCT_tagSIZEL16_32(a32, a16) *(SIZEL*)&a32 = *(SIZEL*)&a16
#define MAP_STRUCT_tagSIZEL32_16(a16, a32) *(SIZEL*)&a16 = *(SIZEL*)&a32

typedef struct
{
    DWORD(CALLBACK* Get)(LPOLESTREAM, void FAR*, DWORD);
    DWORD(CALLBACK* Put)(LPOLESTREAM, const void FAR*, DWORD);
} OLESTREAM32Vtbl;

typedef struct
{
    OLESTREAM32Vtbl *lpVtbl;
} OLESTREAM32;

#define IFS3216_OVERWRITE_OLESTREAM32_Get
DWORD __stdcall OLESTREAM32_32_16_Get(OLESTREAM32 *This, void *lpszBuf, DWORD cbbuf);
#define IFS3216_OVERWRITE_OLESTREAM32_Put
DWORD __stdcall OLESTREAM32_32_16_Put(OLESTREAM32 *This, const void *lpszBuf, DWORD cbbuf);

#define IFS1632_OVERWRITE_ITypeInfo_AddressOfMember
HRESULT CDECL ITypeInfo_16_32_AddressOfMember(SEGPTR This, DWORD args16_memid, WORD args16_invKind, SEGPTR args16_ppv);

#define IFS1632_OVERWRITE_ITypeInfo_Invoke
HRESULT CDECL ITypeInfo_16_32_Invoke(SEGPTR This, SEGPTR args16_pvInstance, DWORD args16_memid, WORD args16_wFlags, SEGPTR args16_pDispParams, SEGPTR args16_pVarResult, SEGPTR args16_pExcepInfo, SEGPTR args16_puArgErr);

#define IFS1632_OVERWRITE_ITypeLib_FindName
HRESULT CDECL ITypeLib_16_32_FindName(SEGPTR This, SEGPTR args16_szNameBuf, DWORD args16_lHashVal, SEGPTR args16_ppTInfo, SEGPTR args16_rgMemId, SEGPTR args16_pcFound);

#define IFS3216_OVERWRITE_IMessageFilter_HandleInComingCall
DWORD STDMETHODCALLTYPE IMessageFilter_32_16_HandleInComingCall(IMessageFilter *This, DWORD dwCallType,HTASK htaskCaller,DWORD dwTickCount,LPINTERFACEINFO lpInterfaceInfo);

#define IFS1632_OVERWRITE_IDataObject_DAdvise
HRESULT CDECL IDataObject_16_32_DAdvise(SEGPTR This, SEGPTR args16_pformatetc, DWORD args16_advf, SEGPTR args16_pAdvSink, SEGPTR args16_pdwConnection);

#endif
