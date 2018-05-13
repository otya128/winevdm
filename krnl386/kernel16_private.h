/*
 * Kernel 16-bit private definitions
 *
 * Copyright 1995 Alexandre Julliard
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

#ifndef __WINE_KERNEL16_PRIVATE_H
#define __WINE_KERNEL16_PRIVATE_H
#define inline __inline
#define USE_NATIVE
#ifdef USE_NATIVE
//#define NtCurrentTeb NtCurrentTeb_WINTERNL_H
#define IMAGE_OS2_SIGNATURE_LX 0x584C     /* LX */
#endif
#include "wine/winbase16.h"
//#include "windows/winreg.h"
//#include "windows/winternl.h"
#ifdef USE_NATIVE
#undef _TEB
#undef TEB
#undef PTEB
#undef _PEB
#undef PEB
#undef PPEB
#undef NtCurrentTeb
#undef _EXCEPTION_REGISTRATION_RECORD
#undef EXCEPTION_REGISTRATION_RECORD
#undef _SYSTEM_BASIC_INFORMATION
#undef SYSTEM_BASIC_INFORMATION
#undef PSYSTEM_BASIC_INFORMATION
#define FileDirectoryInformation FileDirectoryInformation__
#define _FILE_INFORMATION_CLASS _FILE_INFORMATION_CLASS__
#define FILE_INFORMATION_CLASS FILE_INFORMATION_CLASS__
#define _RTL_USER_PROCESS_PARAMETERS _RTL_USER_PROCESS_PARAMETERS__
#define RTL_USER_PROCESS_PARAMETERS RTL_USER_PROCESS_PARAMETERS__
#define PRTL_USER_PROCESS_PARAMETERS PRTL_USER_PROCESS_PARAMETERS__
//#define _NT_TIB _NT_TIB__
//#define NT_TIB NT_TIB__
//#define PNT_TIB PNT_TIB__
#define _SYSTEM_BASIC_INFORMATION _SYSTEM_BASIC_INFORMATION__
#define SYSTEM_BASIC_INFORMATION SYSTEM_BASIC_INFORMATION__
#define PSYSTEM_BASIC_INFORMATION PSYSTEM_BASIC_INFORMATION__
//#define _EXCEPTION_REGISTRATION_RECORD _EXCEPTION_REGISTRATION_RECORD__
//#define EXCEPTION_REGISTRATION_RECORD EXCEPTION_REGISTRATION_RECORD__
#define _TEB _TEB_WINTERNL_H
#define TEB TEB_WINTERNL_H
#define PTEB PTEB_WINTERNL_H
#define _PEB _PEB_WINTERNL_H
#define PEB PEB_WINTERNL_H
#define PPEB PPEB_WINTERNL_H
#define NtCurrentTeb NtCurrentTeb__
#undef _WINTERNL_
#include "winreg.h"
#include "winternl.h"
#undef _TEB
#undef TEB
#undef PTEB
#undef _PEB
#undef PEB
#undef PPEB
#undef NtCurrentTeb
#undef _EXCEPTION_REGISTRATION_RECORD
#undef EXCEPTION_REGISTRATION_RECORD
#undef _SYSTEM_BASIC_INFORMATION
#undef SYSTEM_BASIC_INFORMATION
#undef PSYSTEM_BASIC_INFORMATION
#undef _RTL_USER_PROCESS_PARAMETERS
#undef RTL_USER_PROCESS_PARAMETERS
#undef PRTL_USER_PROCESS_PARAMETERS
#undef _FILE_INFORMATION_CLASS
#undef FILE_INFORMATION_CLASS
#undef FileDirectoryInformation


#undef _SYSTEM_BASIC_INFORMATION__
#undef SYSTEM_BASIC_INFORMATION__
#undef PSYSTEM_BASIC_INFORMATION__
#undef _EXCEPTION_REGISTRATION_RECORD__
#undef EXCEPTION_REGISTRATION_RECORD__
#undef _TEB_WINTERNL_H
#undef TEB_WINTERNL_H
#undef PTEB_WINTERNL_H
#undef _PEB_WINTERNL_H
#undef PEB_WINTERNL_H
#undef PPEB_WINTERNL_H
#undef NtCurrentTeb__

#undef _NT_TIB
#undef NT_TIB
#undef PNT_TIB
#undef _SYSTEM_BASIC_INFORMATION //_SYSTEM_BASIC_INFORMATION__
#undef SYSTEM_BASIC_INFORMATION //SYSTEM_BASIC_INFORMATION__
#undef PSYSTEM_BASIC_INFORMATION //PSYSTEM_BASIC_INFORMATION__
#undef _EXCEPTION_REGISTRATION_RECORD //_EXCEPTION_REGISTRATION_RECORD__
#undef EXCEPTION_REGISTRATION_RECORD //EXCEPTION_REGISTRATION_RECORD__
#undef _TEB //_TEB_WINTERNL_H
#undef TEB //TEB_WINTERNL_H
#undef PTEB //PTEB_WINTERNL_H
#undef _PEB //_PEB_WINTERNL_H
#undef PEB //PEB_WINTERNL_H
#undef PPEB//// PPEB_WINTERNL_H
#undef NtCurrentTeb// NtCurrentTeb__
#ifdef _TEB
#error aaa
#endif

typedef struct _GDI_TEB_BATCH
{
	ULONG  Offset;
	HANDLE HDC;
	ULONG  Buffer[0x136];
} GDI_TEB_BATCH;

typedef struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME
{
	struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME *Previous;
	struct _ACTIVATION_CONTEXT                 *ActivationContext;
	ULONG                                       Flags;
} RTL_ACTIVATION_CONTEXT_STACK_FRAME, *PRTL_ACTIVATION_CONTEXT_STACK_FRAME;
typedef struct _ACTIVATION_CONTEXT_STACK
{
	ULONG                               Flags;
	ULONG                               NextCookieSequenceNumber;
	RTL_ACTIVATION_CONTEXT_STACK_FRAME *ActiveFrame;
	LIST_ENTRY                          FrameListCache;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

typedef struct tagRTL_BITMAP {
	ULONG  SizeOfBitMap; /* Number of bits in the bitmap */
	PULONG Buffer; /* Bitmap data, assumed sized to a DWORD boundary */
} RTL_BITMAP, *PRTL_BITMAP;

typedef struct RTL_DRIVE_LETTER_CURDIR
{
	USHORT              Flags;
	USHORT              Length;
	ULONG               TimeStamp;
	UNICODE_STRING      DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;
typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;
typedef struct _RTL_USER_PROCESS_PARAMETERS
{
	ULONG               AllocationSize;
	ULONG               Size;
	ULONG               Flags;
	ULONG               DebugFlags;
	HANDLE              ConsoleHandle;
	ULONG               ConsoleFlags;
	HANDLE              hStdInput;
	HANDLE              hStdOutput;
	HANDLE              hStdError;
	CURDIR              CurrentDirectory;
	UNICODE_STRING      DllPath;
	UNICODE_STRING      ImagePathName;
	UNICODE_STRING      CommandLine;
	PWSTR               Environment;
	ULONG               dwX;
	ULONG               dwY;
	ULONG               dwXSize;
	ULONG               dwYSize;
	ULONG               dwXCountChars;
	ULONG               dwYCountChars;
	ULONG               dwFillAttribute;
	ULONG               dwFlags;
	ULONG               wShowWindow;
	UNICODE_STRING      WindowTitle;
	UNICODE_STRING      Desktop;
	UNICODE_STRING      ShellInfo;
	UNICODE_STRING      RuntimeInfo;
	RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
__declspec(dllexport) PVOID getWOW32Reserved();
__declspec(dllexport) PVOID setWOW32Reserved(PVOID);
/*
* The exception frame, used for registering exception handlers
* Win32 cares only about this, but compilers generally emit
* larger exception frames for their own use.
*/
/*
struct _EXCEPTION_REGISTRATION_RECORD;

typedef DWORD(*PEXCEPTION_HANDLER)(PEXCEPTION_RECORD, struct _EXCEPTION_REGISTRATION_RECORD*,
	PCONTEXT, struct _EXCEPTION_REGISTRATION_RECORD **);
typedef struct _EXCEPTION_REGISTRATION_RECORD
{
	struct _EXCEPTION_REGISTRATION_RECORD *Prev;
	PEXCEPTION_HANDLER       Handler;
} EXCEPTION_REGISTRATION_RECORD;*/
#undef _PEB
/***********************************************************************
* PEB data structure
*/
typedef struct _PEB
{                                                                 /* win32/win64 */
	BOOLEAN                      InheritedAddressSpace;             /* 000/000 */
	BOOLEAN                      ReadImageFileExecOptions;          /* 001/001 */
	BOOLEAN                      BeingDebugged;                     /* 002/002 */
	BOOLEAN                      SpareBool;                         /* 003/003 */
	HANDLE                       Mutant;                            /* 004/008 */
	HMODULE                      ImageBaseAddress;                  /* 008/010 */
	PPEB_LDR_DATA                LdrData;                           /* 00c/018 */
	RTL_USER_PROCESS_PARAMETERS *ProcessParameters;                 /* 010/020 */
	PVOID                        SubSystemData;                     /* 014/028 */
	HANDLE                       ProcessHeap;                       /* 018/030 */
	PRTL_CRITICAL_SECTION        FastPebLock;                       /* 01c/038 */
	PVOID /*PPEBLOCKROUTINE*/    FastPebLockRoutine;                /* 020/040 */
	PVOID /*PPEBLOCKROUTINE*/    FastPebUnlockRoutine;              /* 024/048 */
	ULONG                        EnvironmentUpdateCount;            /* 028/050 */
	PVOID                        KernelCallbackTable;               /* 02c/058 */
	ULONG                        Reserved[2];                       /* 030/060 */
	PVOID /*PPEB_FREE_BLOCK*/    FreeList;                          /* 038/068 */
	ULONG                        TlsExpansionCounter;               /* 03c/070 */
	PRTL_BITMAP                  TlsBitmap;                         /* 040/078 */
	ULONG                        TlsBitmapBits[2];                  /* 044/080 */
	PVOID                        ReadOnlySharedMemoryBase;          /* 04c/088 */
	PVOID                        ReadOnlySharedMemoryHeap;          /* 050/090 */
	PVOID                       *ReadOnlyStaticServerData;          /* 054/098 */
	PVOID                        AnsiCodePageData;                  /* 058/0a0 */
	PVOID                        OemCodePageData;                   /* 05c/0a8 */
	PVOID                        UnicodeCaseTableData;              /* 060/0b0 */
	ULONG                        NumberOfProcessors;                /* 064/0b8 */
	ULONG                        NtGlobalFlag;                      /* 068/0bc */
	LARGE_INTEGER                CriticalSectionTimeout;            /* 070/0c0 */
	SIZE_T                       HeapSegmentReserve;                /* 078/0c8 */
	SIZE_T                       HeapSegmentCommit;                 /* 07c/0d0 */
	SIZE_T                       HeapDeCommitTotalFreeThreshold;    /* 080/0d8 */
	SIZE_T                       HeapDeCommitFreeBlockThreshold;    /* 084/0e0 */
	ULONG                        NumberOfHeaps;                     /* 088/0e8 */
	ULONG                        MaximumNumberOfHeaps;              /* 08c/0ec */
	PVOID                       *ProcessHeaps;                      /* 090/0f0 */
	PVOID                        GdiSharedHandleTable;              /* 094/0f8 */
	PVOID                        ProcessStarterHelper;              /* 098/100 */
	PVOID                        GdiDCAttributeList;                /* 09c/108 */
	PVOID                        LoaderLock;                        /* 0a0/110 */
	ULONG                        OSMajorVersion;                    /* 0a4/118 */
	ULONG                        OSMinorVersion;                    /* 0a8/11c */
	ULONG                        OSBuildNumber;                     /* 0ac/120 */
	ULONG                        OSPlatformId;                      /* 0b0/124 */
	ULONG                        ImageSubSystem;                    /* 0b4/128 */
	ULONG                        ImageSubSystemMajorVersion;        /* 0b8/12c */
	ULONG                        ImageSubSystemMinorVersion;        /* 0bc/130 */
	ULONG                        ImageProcessAffinityMask;          /* 0c0/134 */
	HANDLE                       GdiHandleBuffer[28];               /* 0c4/138 */
	ULONG                        unknown[6];                        /* 134/218 */
	PVOID                        PostProcessInitRoutine;            /* 14c/230 */
	PRTL_BITMAP                  TlsExpansionBitmap;                /* 150/238 */
	ULONG                        TlsExpansionBitmapBits[32];        /* 154/240 */
	ULONG                        SessionId;                         /* 1d4/2c0 */
	ULARGE_INTEGER               AppCompatFlags;                    /* 1d8/2c8 */
	ULARGE_INTEGER               AppCompatFlagsUser;                /* 1e0/2d0 */
	PVOID                        ShimData;                          /* 1e8/2d8 */
	PVOID                        AppCompatInfo;                     /* 1ec/2e0 */
	UNICODE_STRING               CSDVersion;                        /* 1f0/2e8 */
	PVOID                        ActivationContextData;             /* 1f8/2f8 */
	PVOID                        ProcessAssemblyStorageMap;         /* 1fc/300 */
	PVOID                        SystemDefaultActivationData;       /* 200/308 */
	PVOID                        SystemAssemblyStorageMap;          /* 204/310 */
	SIZE_T                       MinimumStackCommit;                /* 208/318 */
	PVOID                       *FlsCallback;                       /* 20c/320 */
	LIST_ENTRY                   FlsListHead;                       /* 210/328 */
	PRTL_BITMAP                  FlsBitmap;                         /* 218/338 */
	ULONG                        FlsBitmapBits[4];                  /* 21c/340 */
} PEB, *PPEB;
/***********************************************************************
* TEB data structure
*/
typedef struct _TEB
{                                                                 /* win32/win64 */
	NT_TIB                       Tib;                               /* 000/0000 */
	PVOID                        EnvironmentPointer;                /* 01c/0038 */
	CLIENT_ID                    ClientId;                          /* 020/0040 */
	PVOID                        ActiveRpcHandle;                   /* 028/0050 */
	PVOID                        ThreadLocalStoragePointer;         /* 02c/0058 */
	PPEB                         Peb;                               /* 030/0060 */
	ULONG                        LastErrorValue;                    /* 034/0068 */
	ULONG                        CountOfOwnedCriticalSections;      /* 038/006c */
	PVOID                        CsrClientThread;                   /* 03c/0070 */
	PVOID                        Win32ThreadInfo;                   /* 040/0078 */
	ULONG                        Win32ClientInfo[31];               /* 044/0080 used for user32 private data in Wine */
	PVOID                        __WOW32Reserved__;                     /* 0c0/0100 */
	ULONG                        CurrentLocale;                     /* 0c4/0108 */
	ULONG                        FpSoftwareStatusRegister;          /* 0c8/010c */
	PVOID                        SystemReserved1[54];               /* 0cc/0110 used for kernel32 private data in Wine */
	LONG                         ExceptionCode;                     /* 1a4/02c0 */
	ACTIVATION_CONTEXT_STACK     ActivationContextStack;            /* 1a8/02c8 */
	BYTE                         SpareBytes1[24];                   /* 1bc/02e8 used for ntdll private data in Wine */
	PVOID                        SystemReserved2[10];               /* 1d4/0300 used for ntdll private data in Wine =>x86_thread_data(dlls/ntdll/signal_i386.c) */
	GDI_TEB_BATCH                GdiTebBatch;                       /* 1fc/0350 used for vm86 private data in Wine */
	HANDLE                       gdiRgn;                            /* 6dc/0838 */
	HANDLE                       gdiPen;                            /* 6e0/0840 */
	HANDLE                       gdiBrush;                          /* 6e4/0848 */
	CLIENT_ID                    RealClientId;                      /* 6e8/0850 */
	HANDLE                       GdiCachedProcessHandle;            /* 6f0/0860 */
	ULONG                        GdiClientPID;                      /* 6f4/0868 */
	ULONG                        GdiClientTID;                      /* 6f8/086c */
	PVOID                        GdiThreadLocaleInfo;               /* 6fc/0870 */
	ULONG                        UserReserved[5];                   /* 700/0878 */
	PVOID                        glDispachTable[280];               /* 714/0890 */
	PVOID                        glReserved1[26];                   /* b74/1150 */
	PVOID                        glReserved2;                       /* bdc/1220 */
	PVOID                        glSectionInfo;                     /* be0/1228 */
	PVOID                        glSection;                         /* be4/1230 */
	PVOID                        glTable;                           /* be8/1238 */
	PVOID                        glCurrentRC;                       /* bec/1240 */
	PVOID                        glContext;                         /* bf0/1248 */
	ULONG                        LastStatusValue;                   /* bf4/1250 */
	UNICODE_STRING               StaticUnicodeString;               /* bf8/1258 used by advapi32 */
	WCHAR                        StaticUnicodeBuffer[261];          /* c00/1268 used by advapi32 */
	PVOID                        DeallocationStack;                 /* e0c/1478 */
	PVOID                        TlsSlots[64];                      /* e10/1480 */
	LIST_ENTRY                   TlsLinks;                          /* f10/1680 */
	PVOID                        Vdm;                               /* f18/1690 */
	PVOID                        ReservedForNtRpc;                  /* f1c/1698 */
	PVOID                        DbgSsReserved[2];                  /* f20/16a0 */
	ULONG                        HardErrorDisabled;                 /* f28/16b0 */
	PVOID                        Instrumentation[16];               /* f2c/16b8 */
	PVOID                        WinSockData;                       /* f6c/1738 */
	ULONG                        GdiBatchCount;                     /* f70/1740 */
	ULONG                        Spare2;                            /* f74/1744 */
	PVOID                        Spare3;                            /* f78/1748 */
	PVOID                        Spare4;                            /* f7c/1750 */
	PVOID                        ReservedForOle;                    /* f80/1758 */
	ULONG                        WaitingOnLoaderLock;               /* f84/1760 */
	PVOID                        Reserved5[3];                      /* f88/1768 */
	PVOID                       *TlsExpansionSlots;                 /* f94/1780 */
	ULONG                        ImpersonationLocale;               /* f98/1788 */
	ULONG                        IsImpersonating;                   /* f9c/178c */
	PVOID                        NlsCache;                          /* fa0/1790 */
	PVOID                        ShimData;                          /* fa4/1798 */
	ULONG                        HeapVirtualAffinity;               /* fa8/17a0 */
	PVOID                        CurrentTransactionHandle;          /* fac/17a8 */
	PVOID                        ActiveFrame;                       /* fb0/17b0 */
#ifdef _WIN64
	PVOID                        unknown[2];                        /*     17b8 */
#endif
	PVOID                       *FlsSlots;                          /* fb4/17c8 */
} TEB, *PTEB;

__inline struct _TEB * NtCurrentTeb(void) { return (struct _TEB *) (ULONG_PTR)__readfsdword(PcTeb); }
/*************************************************************************
* Loader structures
*
* Those are not part of standard Winternl.h
*/
typedef struct _LDR_MODULE
{
	LIST_ENTRY          InLoadOrderModuleList;
	LIST_ENTRY          InMemoryOrderModuleList;
	LIST_ENTRY          InInitializationOrderModuleList;
	void*               BaseAddress;
	void*               EntryPoint;
	ULONG               SizeOfImage;
	UNICODE_STRING      FullDllName;
	UNICODE_STRING      BaseDllName;
	ULONG               Flags;
	SHORT               LoadCount;
	SHORT               TlsIndex;
	HANDLE              SectionHandle;
	ULONG               CheckSum;
	ULONG               TimeDateStamp;
	HANDLE              ActivationContext;
} LDR_MODULE, *PLDR_MODULE;
/* those defines are (some of the) regular LDR_MODULE.Flags values */
#define LDR_IMAGE_IS_DLL                0x00000004
#define LDR_LOAD_IN_PROGRESS            0x00001000
#define LDR_UNLOAD_IN_PROGRESS          0x00002000
#define LDR_NO_DLL_CALLS                0x00040000
#define LDR_PROCESS_ATTACHED            0x00080000
#define LDR_MODULE_REBASED              0x00200000

/* these ones is Wine specific */
#define LDR_DONT_RESOLVE_REFS           0x40000000
#define LDR_WINE_INTERNAL               0x80000000

/* flag for LdrAddRefDll */
#define LDR_ADDREF_DLL_PIN              0x00000001

/* Wine extensions: call register function, context ptr is passed in the return value LPDWORD */
#define WCB16_REGS       2
#define WCB16_REGS_LONG  4  /* function uses 32-bit lret */
/* Windows Exit Procedure flag values */
#define	WEP_FREE_DLL        0
#define	WEP_SYSTEM_EXIT     1
//wine winbase.h
#define OF_SEARCH             0x0400   /* Used without OF_REOPEN */
/* GetTempFileName() Flags */
#define TF_FORCEDRIVE	        0x80
//winnt.h
/* This flag allows it to create heaps shared by all processes under win95,
FIXME: correct name */
#define HEAP_SHARED                     0x04000000
//winternl.h

/* System Information Class 0x00 */

typedef struct _SYSTEM_BASIC_INFORMATION {
#ifdef __WINESRC__
	DWORD     unknown;
	ULONG     KeMaximumIncrement;
	ULONG     PageSize;
	ULONG     MmNumberOfPhysicalPages;
	ULONG     MmLowestPhysicalPage;
	ULONG     MmHighestPhysicalPage;
	ULONG_PTR AllocationGranularity;
	PVOID     LowestUserAddress;
	PVOID     HighestUserAddress;
	ULONG_PTR ActiveProcessorsAffinityMask;
	BYTE      NumberOfProcessors;
#else
	BYTE Reserved1[24];
	PVOID Reserved2[4];
	CCHAR NumberOfProcessors;
#endif
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;
//wine winternl.h

typedef enum _KEY_VALUE_INFORMATION_CLASS {
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
	KeyValueFullInformationAlign64,
	KeyValuePartialInformationAlign64
} KEY_VALUE_INFORMATION_CLASS;
#endif
typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
	ULONG TitleIndex;
	ULONG Type;
	ULONG DataLength;
	UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;
/* FIXME: private structure for vm86 mode, stored in teb->GdiTebBatch */
typedef struct
{
	DWORD        dpmi_vif;
	DWORD        vm86_pending;
} WINE_VM86_TEB_INFO;

static inline WINE_VM86_TEB_INFO *get_vm86_teb_info(void)
{
	return (WINE_VM86_TEB_INFO *)&NtCurrentTeb()->GdiTebBatch;
}

/* The thread information for 16-bit threads */
/* NtCurrentTeb()->SubSystemTib points to this */
typedef struct
{
	void           *unknown;    /* 00 unknown */
	UNICODE_STRING *exe_name;   /* 04 exe module name */

	/* the following fields do not exist under Windows */
	UNICODE_STRING  exe_str;    /* exe name string pointed to by exe_name */
	CURDIR          curdir;     /* current directory */
	WCHAR           curdir_buffer[MAX_PATH];
} WIN16_SUBSYSTEM_TIB;

#include "pshpack1.h"

/* In-memory module structure. See 'Windows Internals' p. 219 */
typedef struct _NE_MODULE
{
    WORD      ne_magic;         /* 00 'NE' signature */
    WORD      count;            /* 02 Usage count (ne_ver/ne_rev on disk) */
    WORD      ne_enttab;        /* 04 Near ptr to entry table */
    HMODULE16 next;             /* 06 Selector to next module (ne_cbenttab on disk) */
    WORD      dgroup_entry;     /* 08 Near ptr to segment entry for DGROUP (ne_crc on disk) */
    WORD      fileinfo;         /* 0a Near ptr to file info (OFSTRUCT) (ne_crc on disk) */
    WORD      ne_flags;         /* 0c Module flags */
    WORD      ne_autodata;      /* 0e Logical segment for DGROUP */
    WORD      ne_heap;          /* 10 Initial heap size */
    WORD      ne_stack;         /* 12 Initial stack size */
    DWORD     ne_csip;          /* 14 Initial cs:ip */
    DWORD     ne_sssp;          /* 18 Initial ss:sp */
    WORD      ne_cseg;          /* 1c Number of segments in segment table */
    WORD      ne_cmod;          /* 1e Number of module references */
    WORD      ne_cbnrestab;     /* 20 Size of non-resident names table */
    WORD      ne_segtab;        /* 22 Near ptr to segment table */
    WORD      ne_rsrctab;       /* 24 Near ptr to resource table */
    WORD      ne_restab;        /* 26 Near ptr to resident names table */
    WORD      ne_modtab;        /* 28 Near ptr to module reference table */
    WORD      ne_imptab;        /* 2a Near ptr to imported names table */
    DWORD     ne_nrestab;       /* 2c File offset of non-resident names table */
    WORD      ne_cmovent;       /* 30 Number of moveable entries in entry table*/
    WORD      ne_align;         /* 32 Alignment shift count */
    WORD      ne_cres;          /* 34 # of resource segments */
    BYTE      ne_exetyp;        /* 36 Operating system flags */
    BYTE      ne_flagsothers;   /* 37 Misc. flags */
    HANDLE16  dlls_to_init;     /* 38 List of DLLs to initialize (ne_pretthunks on disk) */
    HANDLE16  nrname_handle;    /* 3a Handle to non-resident name table (ne_psegrefbytes on disk) */
    WORD      ne_swaparea;      /* 3c Min. swap area size */
    WORD      ne_expver;        /* 3e Expected Windows version */
    /* From here, these are extra fields not present in normal Windows */
    HMODULE   module32;         /* PE module handle for Win32 modules */
    HMODULE   owner32;          /* PE module containing this one for 16-bit builtins */
    HMODULE16 self;             /* Handle for this module */
    WORD      self_loading_sel; /* Selector used for self-loading apps. */
    LPVOID    rsrc32_map;       /* HRSRC 16->32 map (for 32-bit modules) */
    LPCVOID   mapping;          /* mapping of the binary file */
    SIZE_T    mapping_size;     /* size of the file mapping */
} NE_MODULE;

typedef struct
{
    BYTE type;
    BYTE flags;
    BYTE segnum;
    WORD offs;
} ET_ENTRY;

typedef struct
{
    WORD first; /* ordinal */
    WORD last;  /* ordinal */
    WORD next;  /* bundle */
} ET_BUNDLE;


  /* In-memory segment table */
typedef struct
{
    WORD      filepos;   /* Position in file, in sectors */
    WORD      size;      /* Segment size on disk */
    WORD      flags;     /* Segment flags */
    WORD      minsize;   /* Min. size of segment in memory */
    HANDLE16  hSeg;      /* Selector or handle (selector - 1) of segment in memory */
} SEGTABLEENTRY;

/* this structure is always located at offset 0 of the DGROUP segment */
typedef struct
{
    WORD null;        /* Always 0 */
    DWORD old_ss_sp;  /* Stack pointer; used by SwitchTaskTo() */
    WORD heap;        /* Pointer to the local heap information (if any) */
    WORD atomtable;   /* Pointer to the local atom table (if any) */
    WORD stacktop;    /* Top of the stack */
    WORD stackmin;    /* Lowest stack address used so far */
    WORD stackbottom; /* Bottom of the stack */
} INSTANCEDATA;

/* relay entry points */

typedef struct
{
    WORD   pushw_bp;               /* pushw %bp */
    BYTE   pushl;                  /* pushl $target */
    void  *target;
    WORD   call;                   /* call CALLFROM16 */
    short  callfrom16;
} ENTRYPOINT16;

typedef struct
{
    BYTE   pushl;                  /* pushl $relay */
    void  *relay;
    BYTE   lcall;                  /* lcall __FLATCS__:glue */
    void  *glue;
    WORD   flatcs;
    WORD   ret[5];                 /* return sequence */
    WORD   movl;                   /* movl arg_types[1],arg_types[0](%esi) */
    DWORD  arg_types[2];           /* type of each argument */
} CALLFROM16;

/* THHOOK Kernel Data Structure */
typedef struct _THHOOK
{
    HANDLE16   hGlobalHeap;         /* 00 (handle BURGERMASTER) */
    WORD       pGlobalHeap;         /* 02 (selector BURGERMASTER) */
    HMODULE16  hExeHead;            /* 04 hFirstModule */
    HMODULE16  hExeSweep;           /* 06 (unused) */
    HANDLE16   TopPDB;              /* 08 (handle of KERNEL PDB) */
    HANDLE16   HeadPDB;             /* 0A (first PDB in list) */
    HANDLE16   TopSizePDB;          /* 0C (unused) */
    HTASK16    HeadTDB;             /* 0E hFirstTask */
    HTASK16    CurTDB;              /* 10 hCurrentTask */
    HTASK16    LoadTDB;             /* 12 (unused) */
    HTASK16    LockTDB;             /* 14 hLockedTask */
} THHOOK;

extern LONG __wine_call_from_16(void);
extern void __wine_call_from_16_regs(void);

extern THHOOK *pThhook;

#include "poppack.h"

#define NE_SEG_TABLE(pModule) \
    ((SEGTABLEENTRY *)((char *)(pModule) + (pModule)->ne_segtab))

#define NE_MODULE_NAME(pModule) \
    (((OFSTRUCT *)((char*)(pModule) + (pModule)->fileinfo))->szPathName)

#define NE_GET_DATA(pModule,offset,size) \
    ((const void *)(((offset)+(size) <= pModule->mapping_size) ? \
                    (const char *)pModule->mapping + (offset) : NULL))

#define NE_READ_DATA(pModule,buffer,offset,size) \
    (((offset)+(size) <= pModule->mapping_size) ? \
     (memcpy( buffer, (const char *)pModule->mapping + (offset), (size) ), TRUE) : FALSE)

#define CURRENT_STACK16 ((STACK16FRAME*)MapSL(PtrToUlong(getWOW32Reserved())))
#define CURRENT_DS      (CURRENT_STACK16->ds)

/* push bytes on the 16-bit stack of a thread; return a segptr to the first pushed byte */
static inline SEGPTR stack16_push( int size )
{
    STACK16FRAME *frame = CURRENT_STACK16;
    memmove( (char*)frame - size, frame, sizeof(*frame) );
	setWOW32Reserved((char *)getWOW32Reserved() - size);
    return (SEGPTR)((char *)getWOW32Reserved() + sizeof(*frame));
}

/* pop bytes from the 16-bit stack of a thread */
static inline void stack16_pop( int size )
{
    STACK16FRAME *frame = CURRENT_STACK16;
    memmove( (char*)frame + size, frame, sizeof(*frame) );
	setWOW32Reserved((char *)getWOW32Reserved() + size);
}

/* dosmem.c */
extern BOOL   DOSMEM_Init(void);
extern BOOL   DOSMEM_InitDosMemory(void);
extern LPVOID DOSMEM_MapRealToLinear(DWORD); /* real-mode to linear */
extern LPVOID DOSMEM_MapDosToLinear(UINT);   /* linear DOS to Wine */
extern UINT   DOSMEM_MapLinearToDos(LPVOID); /* linear Wine to DOS */
extern BOOL   DOSMEM_MapDosLayout(void);
extern LPVOID DOSMEM_AllocBlock(UINT size, WORD* p);
extern BOOL   DOSMEM_FreeBlock(void* ptr);
extern UINT   DOSMEM_ResizeBlock(void* ptr, UINT size, BOOL exact);
extern UINT   DOSMEM_Available(void);

/* global16.c */
extern HGLOBAL16 GLOBAL_CreateBlock( UINT16 flags, void *ptr, DWORD size,
                                     HGLOBAL16 hOwner, unsigned char selflags );
extern BOOL16 GLOBAL_FreeBlock( HGLOBAL16 handle );
extern BOOL16 GLOBAL_MoveBlock( HGLOBAL16 handle, void *ptr, DWORD size );
extern HGLOBAL16 GLOBAL_Alloc( WORD flags, DWORD size, HGLOBAL16 hOwner, unsigned char selflags );

/* instr.c */
extern DWORD __wine_emulate_instruction( EXCEPTION_RECORD *rec, CONTEXT *context );
extern LONG CALLBACK INSTR_vectored_handler( EXCEPTION_POINTERS *ptrs );

/* ne_module.c */
extern NE_MODULE *NE_GetPtr( HMODULE16 hModule );
extern WORD NE_GetOrdinal( HMODULE16 hModule, const char *name );
extern FARPROC16 WINAPI NE_GetEntryPoint( HMODULE16 hModule, WORD ordinal );
extern FARPROC16 NE_GetEntryPointEx( HMODULE16 hModule, WORD ordinal, BOOL16 snoop );
extern BOOL16 NE_SetEntryPoint( HMODULE16 hModule, WORD ordinal, WORD offset );
extern DWORD NE_StartTask(void);

/* ne_segment.c */
extern BOOL NE_LoadSegment( NE_MODULE *pModule, WORD segnum );
extern BOOL NE_LoadAllSegments( NE_MODULE *pModule );
extern BOOL NE_CreateSegment( NE_MODULE *pModule, int segnum );
extern BOOL NE_CreateAllSegments( NE_MODULE *pModule );
extern HINSTANCE16 NE_GetInstance( NE_MODULE *pModule );
extern void NE_InitializeDLLs( HMODULE16 hModule );
extern void NE_DllProcessAttach( HMODULE16 hModule );
extern void NE_CallUserSignalProc( HMODULE16 hModule, UINT16 code );

/* selector.c */
extern WORD SELECTOR_AllocBlock( const void *base, DWORD size, unsigned char flags );
extern WORD SELECTOR_ReallocBlock( WORD sel, const void *base, DWORD size );
extern void SELECTOR_FreeBlock( WORD sel );
#define IS_SELECTOR_32BIT(sel) \
   (wine_ldt_is_system(sel) || (wine_ldt_copy.flags[LOWORD(sel) >> 3] & WINE_LDT_FLAGS_32BIT))

/* relay16.c */
extern int relay_call_from_16( void *entry_point, unsigned char *args16, CONTEXT *context );

/* snoop16.c */
extern void SNOOP16_RegisterDLL(HMODULE16,LPCSTR);
extern FARPROC16 SNOOP16_GetProcAddress16(HMODULE16,DWORD,FARPROC16);
extern BOOL SNOOP16_ShowDebugmsgSnoop(const char *dll,int ord,const char *fname);

/* syslevel.c */
extern VOID SYSLEVEL_CheckNotLevel( INT level );

/* task.c */
extern void TASK_CreateMainTask(void);
extern HTASK16 TASK_SpawnTask( NE_MODULE *pModule, WORD cmdShow,
                               LPCSTR cmdline, BYTE len, HANDLE *hThread );
extern void TASK_ExitTask(void);
extern HTASK16 TASK_GetTaskFromThread( DWORD thread );
extern TDB *TASK_GetCurrent(void);
extern void TASK_InstallTHHook( THHOOK *pNewThook );

extern BOOL WOWTHUNK_Init(void);

extern WORD DOSMEM_0000H;
extern WORD DOSMEM_BiosDataSeg;
extern WORD DOSMEM_BiosSysSeg;
extern DWORD CallTo16_DataSelector;
extern DWORD CallTo16_TebSelector;
extern SEGPTR CALL32_CBClient_RetAddr;
extern SEGPTR CALL32_CBClientEx_RetAddr;

struct tagSYSLEVEL;

struct kernel_thread_data
{
    WORD                stack_sel;      /* 16-bit stack selector */
    WORD                htask16;        /* Win16 task handle */
    DWORD               sys_count[4];   /* syslevel mutex entry counters */
    struct tagSYSLEVEL *sys_mutex[4];   /* syslevel mutex pointers */
    void               *pad[45];        /* change this if you add fields! */
};

static inline struct kernel_thread_data *kernel_get_thread_data(void)
{
    return (struct kernel_thread_data *)NtCurrentTeb()->SystemReserved1;
}

//#define DEFINE_REGS_ENTRYPOINT( name, args ) \
//    __ASM_GLOBAL_FUNC( name, \
//                       ".byte 0x68\n\t"  /* pushl $__regs_func */       \
//                       ".long " __ASM_NAME("__regs_") #name "-.-11\n\t" \
//                       ".byte 0x6a," #args "\n\t" /* pushl $args */     \
//                       "call " __ASM_NAME("__wine_call_from_regs") "\n\t" \
//                       "ret $(4*" #args ")" )/* fake ret to make copy protections happy */
#undef DEFINE_REGS_ENTRYPOINT
#define DEFINE_REGS_ENTRYPOINT_ARG(name, ...) void __stdcall name(__VA_ARGS__){ERR("notimpl:DEFINE_REGS_ENTRYPOINT(" #name ")\n");}
#define DEFINE_REGS_ENTRYPOINT_0(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1)
#define DEFINE_REGS_ENTRYPOINT_1(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2)
#define DEFINE_REGS_ENTRYPOINT_2(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3)
#define DEFINE_REGS_ENTRYPOINT_3(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4)
#define DEFINE_REGS_ENTRYPOINT_4(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4, int a5)
#define DEFINE_REGS_ENTRYPOINT_5(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4, int a5, int a6)
#define DEFINE_REGS_ENTRYPOINT_6(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
#define DEFINE_REGS_ENTRYPOINT_7(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)
#define DEFINE_REGS_ENTRYPOINT_8(name) DEFINE_REGS_ENTRYPOINT_ARG(name, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
#define DEFINE_REGS_ENTRYPOINT_JOIN(base, count) base ## count
#define DEFINE_REGS_ENTRYPOINT(name, args) DEFINE_REGS_ENTRYPOINT_JOIN(DEFINE_REGS_ENTRYPOINT_, args) (name)
//void __stdcall name(__declspec(align(0)) struct {__declspec(align(0)) char reg;__declspec(align(0))char a[args];} a){ERR("notimpl:DEFINE_REGS_ENTRYPOINT(" #name ")\n");}
#define __ms_va_list va_list
#define __ms_va_start va_start
#define __ms_va_end va_end
//??
#define PEXCEPTION_HANDLER PEXCEPTION_ROUTINE
//ntdef.h
typedef enum _EVENT_TYPE {
    NotificationEvent,
    SynchronizationEvent
} EVENT_TYPE;
#define CP_UNIXCP CP_ACP
//winternl.h

typedef struct _FILE_INTERNAL_INFORMATION {
	LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;
//ntifs.h
typedef enum _FILE_INFORMATION_CLASS {
	FileDirectoryInformation = 1,
	FileFullDirectoryInformation,
	FileBothDirectoryInformation,
	FileBasicInformation,
	FileStandardInformation,
	FileInternalInformation,
	FileEaInformation,
	FileAccessInformation,
	FileNameInformation,
	FileRenameInformation,
	FileLinkInformation,
	FileNamesInformation,
	FileDispositionInformation,
	FilePositionInformation,
	FileFullEaInformation,
	FileModeInformation,
	FileAlignmentInformation,
	FileAllInformation,
	FileAllocationInformation,
	FileEndOfFileInformation,
	FileAlternateNameInformation,
	FileStreamInformation,
	FilePipeInformation,
	FilePipeLocalInformation,
	FilePipeRemoteInformation,
	FileMailslotQueryInformation,
	FileMailslotSetInformation,
	FileCompressionInformation,
	FileObjectIdInformation,
	FileCompletionInformation,
	FileMoveClusterInformation,
	FileQuotaInformation,
	FileReparsePointInformation,
	FileNetworkOpenInformation,
	FileAttributeTagInformation,
	FileTrackingInformation,
	FileIdBothDirectoryInformation,
	FileIdFullDirectoryInformation,
	FileValidDataLengthInformation,
	FileShortNameInformation,
	FileIoCompletionNotificationInformation,
	FileIoStatusBlockRangeInformation,
	FileIoPriorityHintInformation,
	FileSfioReserveInformation,
	FileSfioVolumeInformation,
	FileHardLinkInformation,
	FileProcessIdsUsingFileInformation,
	FileNormalizedNameInformation,
	FileNetworkPhysicalNameInformation,
	FileIdGlobalTxDirectoryInformation,
	FileIsRemoteDeviceInformation,
	FileAttributeCacheInformation,
	FileNumaNodeInformation,
	FileStandardLinkInformation,
	FileRemoteProtocolInformation,
	FileReplaceCompletionInformation,
	FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;
/*typedef NTSTATUS(WINAPI *PNtAllocateVirtualMemory)(HANDLE ProcessHandle,
	PVOID *BaseAddress,
	ULONG_PTR ZeroBits,
	PSIZE_T RegionSize,
	ULONG AllocationType,
	ULONG Protect);
static inline NTSTATUS NtAllocateVirtualMemory(HANDLE ProcessHandle,
	PVOID *BaseAddress,
	ULONG_PTR ZeroBits,
	PSIZE_T RegionSize,
	ULONG AllocationType,
	ULONG Protect)
{
	static PNtAllocateVirtualMemory NtAllocateVirtualMemory = 0;
	if (!NtAllocateVirtualMemory)
		NtAllocateVirtualMemory = (PNtAllocateVirtualMemory)GetProcAddress(GetModuleHandle("NTDLL.DLL"), "NtAllocateVirtualMemory");
	return NtAllocateVirtualMemory(ProcessHandle,
		BaseAddress,
		ZeroBits,
		RegionSize,
		AllocationType,
		Protect);
}*/
//PNtAllocateVirtualMemory NtAllocateVirtualMemory = (PNtAllocateVirtualMemory)GetProcAddress(GetModuleHandle("NTDLL.DLL"), "NtAllocateVirtualMemory");
NTSYSAPI NTSTATUS  WINAPI NtAllocateVirtualMemory(HANDLE, PVOID*, ULONG, SIZE_T*, ULONG, ULONG);
NTSTATUS
NTAPI
NtRaiseException(IN PEXCEPTION_RECORD ExceptionRecord,
IN PCONTEXT Context,
IN BOOLEAN FirstChance);
NTSYSAPI NTSTATUS  WINAPI NtSetEvent(HANDLE, PULONG);
NTSYSAPI HANDLE    WINAPI RtlCreateHeap(ULONG, PVOID, SIZE_T, SIZE_T, PVOID, PVOID);
NTSYSAPI NTSTATUS  WINAPI NtQueryInformationFile(HANDLE, PIO_STATUS_BLOCK, PVOID, LONG, FILE_INFORMATION_CLASS);
NTSYSAPI NTSTATUS  WINAPI NtCreateEvent(PHANDLE, ACCESS_MASK, const OBJECT_ATTRIBUTES *, EVENT_TYPE, BOOLEAN);
NTSYSAPI NTSTATUS  WINAPI NtResetEvent(HANDLE, PULONG);
NTSYSAPI NTSTATUS  WINAPI NtOpenKey(PHANDLE, ACCESS_MASK, const OBJECT_ATTRIBUTES *);
NTSYSAPI NTSTATUS  WINAPI NtQueryValueKey(HANDLE, const UNICODE_STRING *, KEY_VALUE_INFORMATION_CLASS, void *, DWORD, DWORD *);
NTSYSAPI NTSTATUS  WINAPI RtlEnterCriticalSection(RTL_CRITICAL_SECTION *);
NTSYSAPI NTSTATUS  WINAPI RtlLeaveCriticalSection(RTL_CRITICAL_SECTION *);
NTSYSAPI PIMAGE_NT_HEADERS WINAPI RtlImageNtHeader(HMODULE);
NTSYSAPI BOOLEAN   WINAPI RtlCreateUnicodeString(PUNICODE_STRING, LPCWSTR);
NTSYSAPI void      WINAPI RtlAcquirePebLock(void);
NTSYSAPI void      WINAPI RtlReleasePebLock(void);
NTSYSAPI NTSTATUS  WINAPI RtlInitializeCriticalSection(RTL_CRITICAL_SECTION *);
NTSYSAPI PVOID     WINAPI RtlAllocateHeap(HANDLE, ULONG, SIZE_T);
NTSYSAPI NTSTATUS  WINAPI RtlOpenCurrentUser(ACCESS_MASK, PHANDLE);
NTSYSAPI CHAR      WINAPI RtlUpperChar(CHAR);
NTSYSAPI PVOID     WINAPI RtlAddVectoredExceptionHandler(ULONG, PVECTORED_EXCEPTION_HANDLER);
NTSYSAPI NTSTATUS  WINAPI RtlGetVersion(RTL_OSVERSIONINFOEXW*);
NTSYSAPI void      WINAPI RtlSecondsSince1970ToTime(DWORD, LARGE_INTEGER *);
# define strncasecmp _strnicmp
# define strcasecmp _stricmp
HANDLE WINAPI ConvertToGlobalHandle(HANDLE);
NTSYSAPI void WINAPI DbgBreakPoint(void);
NTSYSAPI NTSTATUS  WINAPI LdrFindEntryForAddress(const void*, PLDR_MODULE*);
#endif  /* __WINE_KERNEL16_PRIVATE_H */
