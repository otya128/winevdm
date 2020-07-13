/*
 * Task functions
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

#include "config.h"
#include "wine/port.h"
#include "wine/exception.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winnt.h"
#include "windows/wownt32.h"
#include "winuser.h"
#include "ntstatus.h"

#include "wine/winbase16.h"
#include "winternl.h"
#include "kernel16_private.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(task);

/* Workaround for ReactOS */
BOOL is_reactos()
{
    static BOOL detected;
    static BOOL is;
    HKEY hKey;
    CHAR name[100];
    DWORD dwType, dwSize = sizeof(name);
    if (detected)
        return is;
    detected = TRUE;
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_QUERY_VALUE, &hKey))
        return FALSE;
    if (ERROR_SUCCESS != RegQueryValueExA(hKey, "ProductName", NULL, &dwType, (LPBYTE)name, &dwSize))
    {
        RegCloseKey(hKey);
        return FALSE;
    }
    RegCloseKey(hKey);
    if (dwType != REG_SZ)
        return FALSE;
    is = strstr(name, "ReactOS") != NULL;
    return is;
}
#include "pshpack1.h"

struct thunk
{
    BYTE      movw;
    HANDLE16  instance;
    BYTE      ljmp;
    FARPROC16 func;
};

/* Segment containing MakeProcInstance() thunks */
typedef struct
{
    WORD  next;       /* Selector of next segment */
    WORD  magic;      /* Thunks signature */
    WORD  unused;
    WORD  free;       /* Head of the free list */
    struct thunk thunks[1];
} THUNKS;

#include "poppack.h"

#define THUNK_MAGIC  ('P' | ('T' << 8))

  /* Min. number of thunks allocated when creating a new segment */
#define MIN_THUNKS  32

#define TDB_MAGIC    ('T' | ('D' << 8))

static THHOOK DefaultThhook;
THHOOK *pThhook = &DefaultThhook;

#define hFirstTask   (pThhook->HeadTDB)
#define hLockedTask  (pThhook->LockTDB)

static UINT16 nTaskCount = 0;

static HTASK16 initial_task, main_task;

BOOL WINAPI TebTlsSetValue(TEB *teb, DWORD index, LPVOID value)
{
    if (index < TLS_MINIMUM_AVAILABLE)
    {
        teb->TlsSlots[index] = value;
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * sizeof(teb->Peb->TlsExpansionBitmapBits))
        {
            return FALSE;
        }
        teb->TlsExpansionSlots[index] = value;
    }
    return TRUE;
}

LPVOID WINAPI TebTlsGetValue(TEB *teb, DWORD index)
{
    LPVOID ret;

    if (index < TLS_MINIMUM_AVAILABLE)
    {
        ret = teb->TlsSlots[index];
    }
    else
    {
        index -= TLS_MINIMUM_AVAILABLE;
        if (index >= 8 * sizeof(teb->Peb->TlsExpansionBitmapBits))
        {
            return NULL;
        }
        if (!teb->TlsExpansionSlots) ret = NULL;
        else ret = teb->TlsExpansionSlots[index];
    }
    return ret;
}
static DWORD curdir_tls_index = -1;

static HTASK16 task_old = NULL;
static struct kernel_thread_data *task_old_data = NULL;
/***********************************************************************
 *	     TASK_InstallTHHook
 */
void TASK_InstallTHHook( THHOOK *pNewThhook )
{
     THHOOK *pOldThhook = pThhook;

     pThhook = pNewThhook? pNewThhook : &DefaultThhook;

     *pThhook = *pOldThhook;
}

/***********************************************************************
 *	     TASK_GetPtr
 */
static TDB *TASK_GetPtr( HTASK16 hTask )
{
    return GlobalLock16( hTask );
}


/***********************************************************************
 *	     TASK_GetCurrent
 */
TDB *TASK_GetCurrent(void)
{
    return TASK_GetPtr( GetCurrentTask() );
}


/***********************************************************************
 *           TASK_LinkTask
 */
static void TASK_LinkTask( HTASK16 hTask )
{
    HTASK16 *prevTask;
    TDB *pTask;

    if (!(pTask = TASK_GetPtr( hTask ))) return;
    prevTask = &hFirstTask;
    while (*prevTask)
    {
        TDB *prevTaskPtr = TASK_GetPtr( *prevTask );
        if (prevTaskPtr->priority >= pTask->priority) break;
        prevTask = &prevTaskPtr->hNext;
    }
    pTask->hNext = *prevTask;
    *prevTask = hTask;
    nTaskCount++;
}


/***********************************************************************
 *           TASK_UnlinkTask
 */
static void TASK_UnlinkTask( HTASK16 hTask )
{
    HTASK16 *prevTask;
    TDB *pTask;

    prevTask = &hFirstTask;
    while (*prevTask && (*prevTask != hTask))
    {
        pTask = TASK_GetPtr( *prevTask );
        prevTask = &pTask->hNext;
    }
    if (*prevTask)
    {
        pTask = TASK_GetPtr( *prevTask );
        *prevTask = pTask->hNext;
        pTask->hNext = 0;
        nTaskCount--;
    }
}


/***********************************************************************
 *           TASK_CreateThunks
 *
 * Create a thunk free-list in segment 'handle', starting from offset 'offset'
 * and containing 'count' entries.
 */
static void TASK_CreateThunks( HGLOBAL16 handle, WORD offset, WORD count )
{
    int i;
    THUNKS *pThunk;

    pThunk = (THUNKS *)((BYTE *)GlobalLock16( handle ) + offset);
    pThunk->next = 0;
    pThunk->magic = THUNK_MAGIC;
    pThunk->free = FIELD_OFFSET( THUNKS, thunks );
    for (i = 0; i < count-1; i++)
        *(WORD *)&pThunk->thunks[i] = FIELD_OFFSET( THUNKS, thunks[i+1] );
    *(WORD *)&pThunk->thunks[i] = 0;  /* Last thunk */
}


/***********************************************************************
 *           TASK_AllocThunk
 *
 * Allocate a thunk for MakeProcInstance().
 */
static SEGPTR TASK_AllocThunk(void)
{
    TDB *pTask;
    THUNKS *pThunk;
    WORD sel, base;

    if (!(pTask = TASK_GetCurrent())) return 0;
    sel = pTask->hCSAlias;
    pThunk = (THUNKS *)pTask->thunks;
    base = (char *)pThunk - (char *)pTask;
    while (!pThunk->free)
    {
        sel = pThunk->next;
        if (!sel)  /* Allocate a new segment */
        {
            sel = GLOBAL_Alloc( GMEM_FIXED, FIELD_OFFSET( THUNKS, thunks[MIN_THUNKS] ),
                                pTask->hPDB, WINE_LDT_FLAGS_CODE );
            if (!sel) return 0;
            TASK_CreateThunks( sel, 0, MIN_THUNKS );
            pThunk->next = sel;
        }
        pThunk = GlobalLock16( sel );
        base = 0;
    }
    base += pThunk->free;
    pThunk->free = *(WORD *)((BYTE *)pThunk + pThunk->free);
    return MAKESEGPTR( sel, base );
}


/***********************************************************************
 *           TASK_FreeThunk
 *
 * Free a MakeProcInstance() thunk.
 */
static BOOL TASK_FreeThunk( SEGPTR thunk )
{
    TDB *pTask;
    THUNKS *pThunk;
    WORD sel, base;

    if (!(pTask = TASK_GetCurrent())) return FALSE;
    sel = pTask->hCSAlias;
    pThunk = (THUNKS *)pTask->thunks;
    base = (char *)pThunk - (char *)pTask;
    while (sel && (sel != HIWORD(thunk)))
    {
        sel = pThunk->next;
        pThunk = GlobalLock16( sel );
        base = 0;
    }
    if (!sel) return FALSE;
    *(WORD *)((BYTE *)pThunk + LOWORD(thunk) - base) = pThunk->free;
    pThunk->free = LOWORD(thunk) - base;
    return TRUE;
}


/***********************************************************************
 *           TASK_Create
 *
 * NOTE: This routine might be called by a Win32 thread. Thus, we need
 *       to be careful to protect global data structures. We do this
 *       by entering the Win16Lock while linking the task into the
 *       global task list.
 */
static TDB *TASK_Create( NE_MODULE *pModule, UINT16 cmdShow, LPCSTR cmdline, BYTE len )
{
    HTASK16 hTask;
    TDB *pTask;
    FARPROC16 proc;
    char curdir[MAX_PATH];
    HMODULE16 hModule = pModule ? pModule->self : 0;

      /* Allocate the task structure */

    hTask = GlobalAlloc16( GMEM_FIXED | GMEM_ZEROINIT, sizeof(TDB) );
    if (!hTask) return NULL;
    GLOBAL_SetSeg(hTask, 0, GT_TASK);
    pTask = TASK_GetPtr( hTask );
    FarSetOwner16( hTask, hModule );

    /* Fill the task structure */

    pTask->hSelf = hTask;

    pTask->version       = pModule ? pModule->ne_expver : 0x0400;
    pTask->hModule       = hModule;
    pTask->hParent       = GetCurrentTask();
    pTask->magic         = TDB_MAGIC;
    pTask->nCmdShow      = cmdShow;

    GetCurrentDirectoryA( sizeof(curdir), curdir );
    GetShortPathNameA( curdir, curdir, sizeof(curdir) );
    pTask->curdrive = (curdir[0] - 'A') | 0x80;
    lstrcpynA( pTask->curdir, curdir + 2, sizeof(pTask->curdir) );

      /* Create the thunks block */

    TASK_CreateThunks( hTask, (char *)pTask->thunks - (char *)pTask, 7 );

      /* Copy the module name */

    if (hModule)
    {
        char name[sizeof(pTask->module_name)+1];
        size_t len;
        GetModuleName16( hModule, name, sizeof(name) );
        len = strlen(name) + 1;
        memcpy(pTask->module_name, name, min(len,sizeof(pTask->module_name)));
        pTask->compat_flags = GetProfileIntA( "Compatibility", name, 0 );
    }

      /* Allocate a selector for the PDB */

    pTask->hPDB = GLOBAL_CreateBlock( GMEM_FIXED, &pTask->pdb, sizeof(PDB16),
                                      hModule, WINE_LDT_FLAGS_DATA );

      /* Fill the PDB */

    pTask->pdb.int20 = 0x20cd;
    pTask->pdb.dispatcher[0] = 0x9a;  /* ljmp */
    proc = GetProcAddress16( GetModuleHandle16("KERNEL"), "DOS3Call" );
    memcpy( &pTask->pdb.dispatcher[1], &proc, sizeof(proc) );
    pTask->pdb.savedint22 = 0;
    pTask->pdb.savedint23 = 0;
    pTask->pdb.savedint24 = 0;
    pTask->pdb.fileHandlesPtr =
        MAKESEGPTR( GlobalHandleToSel16(pTask->hPDB), FIELD_OFFSET( PDB16, fileHandles ));
    pTask->pdb.hFileHandles = 0;
    memset( pTask->pdb.fileHandles, 0xff, sizeof(pTask->pdb.fileHandles) );
    /* FIXME: should we make a copy of the environment? */
    pTask->pdb.environment    = SELECTOROF(GetDOSEnvironment16());
    pTask->pdb.nbFiles        = 20;

    /* Fill the command line */

    if (!cmdline)
    {
        cmdline = GetCommandLineA();
        /* remove the first word (program name) */
        if (*cmdline == '"')
            if (!(cmdline = strchr( cmdline+1, '"' ))) cmdline = GetCommandLineA();
        while (*cmdline && (*cmdline != ' ') && (*cmdline != '\t')) cmdline++;
        while ((*cmdline == ' ') || (*cmdline == '\t')) cmdline++;
        len = strlen(cmdline);
    }
    if (len >= sizeof(pTask->pdb.cmdLine)) len = sizeof(pTask->pdb.cmdLine)-2;
    pTask->pdb.cmdLine[0] = len;
    memcpy( pTask->pdb.cmdLine + 1, cmdline, len );
    if (len == 0)
    {
        pTask->pdb.cmdLine[len+1] = 0x0d; /* Actual windows behaviour(but why?) */
    }

    TRACE("cmdline='%.*s' task=%04x\n", len, cmdline, hTask );

      /* Allocate a code segment alias for the TDB */

    pTask->hCSAlias = GLOBAL_CreateBlock( GMEM_FIXED, pTask, sizeof(TDB),
                                          pTask->hPDB, WINE_LDT_FLAGS_CODE );

      /* Default DTA overwrites command line */

    pTask->dta = MAKESEGPTR( pTask->hPDB, FIELD_OFFSET( PDB16, cmdLine ));

    /* Create scheduler event for 16-bit tasks */

    if ( !(pTask->flags & TDBF_WIN32) )
        NtCreateEvent( &pTask->hEvent, EVENT_ALL_ACCESS, NULL, NotificationEvent, FALSE );

    if (!initial_task) initial_task = hTask;

    return pTask;
}


/***********************************************************************
 *           TASK_DeleteTask
 */
static void TASK_DeleteTask( HTASK16 hTask )
{
    TDB *pTask;
    HGLOBAL16 hPDB;

    if (!(pTask = TASK_GetPtr( hTask ))) return;
    HeapFree(GetProcessHeap(), 0, TebTlsGetValue(pTask->teb, curdir_tls_index));
    if (task_old == hTask)
    {
        task_old = 0;
        task_old_data = NULL;
        TlsSetValue(curdir_tls_index, 0xdead);
    }
    hPDB = pTask->hPDB;

    pTask->magic = 0xdead; /* invalidate signature */

    /* Free the selector aliases */

    GLOBAL_FreeBlock( pTask->hCSAlias );
    GLOBAL_FreeBlock( pTask->hPDB );

    /* Free the task module */

    FreeModule16( pTask->hModule );

    /* Free the task structure itself */

    GlobalFree16( hTask );

    /* Free all memory used by this task (including the 32-bit stack, */
    /* the environment block and the thunk segments). */

    GlobalFreeAll16( hPDB );
}


/***********************************************************************
 *           TASK_CreateMainTask
 *
 * Create a task for the main (32-bit) process.
 */
void TASK_CreateMainTask(void)
{
    TDB *pTask;
    STARTUPINFOA startup_info;
    UINT cmdShow = 1; /* SW_SHOWNORMAL but we don't want to include winuser.h here */

    GetStartupInfoA( &startup_info );
    if (startup_info.dwFlags & STARTF_USESHOWWINDOW) cmdShow = startup_info.wShowWindow;
    pTask = TASK_Create( NULL, cmdShow, NULL, 0 );
    if (!pTask)
    {
        ERR("could not create task for main process\n");
        ExitProcess(1);
    }

    pTask->flags        |= TDBF_WIN32;
    pTask->hInstance     = 0;
    pTask->hPrevInstance = 0;
    pTask->teb           = NtCurrentTeb();

    /* Add the task to the linked list */
    /* (no need to get the win16 lock, we are the only thread at this point) */
    TASK_LinkTask( pTask->hSelf );
    main_task = pTask->hSelf;
}


struct create_data
{
    TDB                 *task;
    WIN16_SUBSYSTEM_TIB *tib;
    LPCSTR               curdir;
};

/* allocate the win16 TIB for a new 16-bit task */
static WIN16_SUBSYSTEM_TIB *allocate_win16_tib( TDB *pTask )
{
    WCHAR path[MAX_PATH];
    WIN16_SUBSYSTEM_TIB *tib;
    UNICODE_STRING *curdir;
    NE_MODULE *pModule = NE_GetPtr( pTask->hModule );

    if (!(tib = HeapAlloc( GetProcessHeap(), 0, sizeof(*tib) ))) return NULL;
    MultiByteToWideChar( CP_ACP, 0, NE_MODULE_NAME(pModule), -1, path, MAX_PATH );
    GetLongPathNameW( path, path, MAX_PATH );
    if (RtlCreateUnicodeString( &tib->exe_str, path )) tib->exe_name = &tib->exe_str;
    else tib->exe_name = NULL;

    RtlAcquirePebLock();
    if (NtCurrentTeb()->Tib.SubSystemTib)
        curdir = &((WIN16_SUBSYSTEM_TIB *)NtCurrentTeb()->Tib.SubSystemTib)->curdir.DosPath;
    else
        curdir = &NtCurrentTeb()->Peb->ProcessParameters->CurrentDirectory.DosPath;
    tib->curdir.DosPath.MaximumLength = sizeof(tib->curdir_buffer);
    tib->curdir.DosPath.Length = min( curdir->Length, tib->curdir.DosPath.MaximumLength-sizeof(WCHAR) );
    tib->curdir.DosPath.Buffer = tib->curdir_buffer;
    tib->curdir.Handle = 0;
    memcpy( tib->curdir_buffer, curdir->Buffer, tib->curdir.DosPath.Length );
    tib->curdir_buffer[tib->curdir.DosPath.Length/sizeof(WCHAR)] = 0;
    RtlReleasePebLock();
    return tib;
}

static inline void free_win16_tib( WIN16_SUBSYSTEM_TIB *tib )
{
    if (tib->exe_name) RtlFreeUnicodeString( tib->exe_name );
    HeapFree( GetProcessHeap(), 0, tib );
}

static void set_thread_internal_windows_ver(DWORD version)
{
    /* In WOW64, NtCurrentTeb()->Win32ClientInfo is not used */
    BOOL iswow64 = FALSE;
    LPDWORD lpdwExpWinVer;
    IsWow64Process(GetCurrentProcess(), &iswow64);
    typedef struct
    {
        DWORD64 CI_flags;
        DWORD64 cSpins;
        DWORD dwExpWinVer;
        DWORD dwCompatFlags;
        /* teb->Peb->AppCompatFlagsUser.LowPart */
        DWORD dwCompatFlags2;
        /* ... */
    } CLIENTINFO64, *LPCLIENTINFO64;
    typedef struct
    {
        DWORD CI_flags;
        DWORD cSpins;
        DWORD dwExpWinVer;
        DWORD dwCompatFlags;
        /* teb->Peb->AppCompatFlagsUser.LowPart */
        DWORD dwCompatFlags2;
        /* ... */
    } CLIENTINFO32, *LPCLIENTINFO32;
    if (iswow64)
    {
#if 0
        /* only works in windows 10 */
        SSIZE_T teb_wow64teboffset = *(PSSIZE_T)((LPBYTE)NtCurrentTeb() + 0x0FDC);
        LPBYTE wow64teb = (LPBYTE)NtCurrentTeb() + teb_wow64teboffset;
#else
        LPBYTE wow64teb = *(LPBYTE*)((LPBYTE)NtCurrentTeb() + 0x0F70);
#endif
        LPCLIENTINFO64 wow64teb_Win32ClientInfo = (LPCLIENTINFO64)(wow64teb + 0x800);
        lpdwExpWinVer = &wow64teb_Win32ClientInfo->dwExpWinVer;
    }
    else
    {
        /* NT 5.0 and higher */
        LPBYTE teb = (LPBYTE)NtCurrentTeb();
        LPCLIENTINFO32 teb_Win32ClientInfo = (LPCLIENTINFO32)(teb + 0x06CC);
        lpdwExpWinVer = &teb_Win32ClientInfo->dwExpWinVer;
    }
    if (*lpdwExpWinVer == 0)
    {
        MSG msg;
        /* init user32 */
        PeekMessageW(&msg, NULL, 0, 0, 0);
    }
    *lpdwExpWinVer = version;
}

static void cwd_warning(void)
{
    char cd[MAX_PATH];
    DWORD result;
    GetCurrentDirectoryA(MAX_PATH, cd);
    result = GetShortPathNameA(cd, cd, MAX_PATH);
    if (result < MAX_PATH && result > 0)
    {
        char *c = cd;
        while (c != (char*)1)
        {
            char *next = strchr(c, '\\');
            if (next - c > 8)
            {
                char *ext = strchr(c, '.');
                if (!ext || (ext - c > 8 || next - (ext + 1) > 3))
                {
                    ERR("could not get 8.3 filename. %s\n", cd);
                    break;
                }
            }
            c = next + 1;
        }
        if (strlen(cd) > 60)
        {
            ERR("current directory is too long. %s\n", cd);
        }
    }
}

/* startup routine for a new 16-bit thread */
static DWORD CALLBACK task_start( LPVOID p )
{
    struct create_data *data = p;
    TDB *pTask = data->task;
    DWORD ret;

    kernel_get_thread_data()->htask16 = pTask->hSelf;
    kernel_get_thread_data()->idle_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    if (!is_reactos())
        NtCurrentTeb()->Tib.SubSystemTib = data->tib;

    set_thread_internal_windows_ver(0x30A);

    _EnterWin16Lock();
    TASK_LinkTask( pTask->hSelf );
    pTask->teb = NtCurrentTeb();
    typedef HRESULT(WINAPI *SetThreadDescription_t)(HANDLE hThread, PCWSTR lpThreadDescription);
    static BOOL init_SetThreadDescription = FALSE;
    static SetThreadDescription_t SetThreadDescription;
    if (!init_SetThreadDescription)
    {
        init_SetThreadDescription = TRUE;
        SetThreadDescription = (SetThreadDescription_t)GetProcAddress(GetModuleHandleW(L"kernel32"), "SetThreadDescription");
    }
    if (SetThreadDescription) /* Windows 10 1607~ */
    {
        WCHAR buf[1025];
        buf[0] = 0;
        CHAR mod_name[9];
        memcpy(mod_name, pTask->module_name, 8);
        mod_name[8] = 0;
        wsprintfW(buf, L"%S(%s)", mod_name, data->tib->exe_name->Buffer);
        SetThreadDescription(GetCurrentThread(), buf);
    }
    if (data->curdir)
        SetCurrentDirectory16(data->curdir);
    cwd_warning();
    HeapFree(GetProcessHeap(), 0, data);
#ifdef _MSC_VER
    __try
#endif
    {
        ret = NE_StartTask();
    }
#ifdef _MSC_VER
    __except (IsDebuggerPresent() ? EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER)
#endif
    {

    }
    _LeaveWin16Lock();
    return ret;
}


/***********************************************************************
 *           TASK_SpawnTask
 *
 * Spawn a new 16-bit task.
 */
HTASK16 TASK_SpawnTask( NE_MODULE *pModule, WORD cmdShow,
                        LPCSTR cmdline, BYTE len, HANDLE *hThread, LPCSTR curdir )
{
    struct create_data *data = NULL;
    WIN16_SUBSYSTEM_TIB *tib;
    TDB *pTask;

    if (!(pTask = TASK_Create( pModule, cmdShow, cmdline, len ))) return 0;
    if (!(tib = allocate_win16_tib( pTask ))) goto failed;
    if (!(data = HeapAlloc( GetProcessHeap(), 0, sizeof(*data)))) goto failed;
    data->task = pTask;
    data->tib = tib;
    data->curdir = curdir;
    if (!(*hThread = CreateThread( NULL, 0, task_start, data, 0, NULL ))) goto failed;
    return pTask->hSelf;

failed:
    if (tib) free_win16_tib( tib );
    HeapFree( GetProcessHeap(), 0, data );
    TASK_DeleteTask( pTask->hSelf );
    return 0;
}


/***********************************************************************
 *           TASK_GetTaskFromThread
 */
HTASK16 TASK_GetTaskFromThread( DWORD thread )
{
    TDB *p = TASK_GetPtr( hFirstTask );
    while (p)
    {
        if (p->teb->ClientId.UniqueThread == (HANDLE)thread) return p->hSelf;
        p = TASK_GetPtr( p->hNext );
    }
    return 0;
}


/***********************************************************************
 *           TASK_CallTaskSignalProc
 */
static void TASK_CallTaskSignalProc( UINT16 uCode, HANDLE16 hTaskOrModule )
{
    WORD args[5];
    TDB *pTask = TASK_GetCurrent();

    if ( !pTask || !pTask->userhandler ) return;

    args[4] = hTaskOrModule;
    args[3] = uCode;
    args[2] = 0;
    args[1] = pTask->hInstance;
    args[0] = pTask->hQueue;
    WOWCallback16Ex( (DWORD)pTask->userhandler, WCB16_PASCAL, sizeof(args), args, NULL );
}


static void exit_toolhelp()
{
    HMODULE toolhelp = GetModuleHandleA("TOOLHELP.DLL16");
    BOOL16(WINAPI*InterruptUnRegister16)(HTASK16);
    BOOL16(WINAPI*NotifyUnRegister16)(HTASK16);
    if (!toolhelp)
        return;
    InterruptUnRegister16 = (BOOL16(WINAPI*)(HTASK16))GetProcAddress(toolhelp, "InterruptUnRegister16");
    NotifyUnRegister16 = (BOOL16(WINAPI*)(HTASK16))GetProcAddress(toolhelp, "NotifyUnRegister16");
    InterruptUnRegister16(0);
    NotifyUnRegister16(0);
}
/***********************************************************************
 *           TASK_ExitTask
 */
void TASK_ExitTask(void)
{
    WIN16_SUBSYSTEM_TIB *tib;
    TDB *pTask;
    DWORD lockCount;

    /* Enter the Win16Lock to protect global data structures */
    _EnterWin16Lock();

    pTask = TASK_GetCurrent();
    if ( !pTask )
    {
        _LeaveWin16Lock();
        return;
    }

    TRACE("Killing task %04x\n", pTask->hSelf );

    /* Perform USER cleanup */

    TASK_CallTaskSignalProc( USIG16_TERMINATION, pTask->hSelf );
    NE_CallUserSignalProc(pTask->hSelf, USIG16_TERMINATION, 0, 0, 0);

    /* TOOLHELP */
    exit_toolhelp();

    /* Remove the task from the list to be sure we never switch back to it */
    TASK_UnlinkTask( pTask->hSelf );
    SetEvent(kernel_get_thread_data()->idle_event);
    CloseHandle(kernel_get_thread_data()->idle_event);

    if (!nTaskCount || (nTaskCount == 1 && hFirstTask == initial_task))
    {
        TRACE("this is the last task, exiting\n" );
        ExitKernel16();
    }

    pTask->nEvents = 0;

    if ( hLockedTask == pTask->hSelf )
        hLockedTask = 0;

    TASK_DeleteTask( pTask->hSelf );

    if ((tib = NtCurrentTeb()->Tib.SubSystemTib))
    {
        free_win16_tib( tib );
        NtCurrentTeb()->Tib.SubSystemTib = NULL;
    }

    /* ... and completely release the Win16Lock, just in case. */
    ReleaseThunkLock( &lockCount );
}


/***********************************************************************
 *           ExitKernel (KERNEL.2)
 *
 * Clean-up everything and exit the Wine process.
 */
void WINAPI ExitKernel16(void)
{
    WriteOutProfiles16();
#if defined(_MSC_VER) || defined(__MINGW32__)
    _flushall();
#else
    fflush(stderr); /* _flushlbf */
#endif
    TerminateProcess( GetCurrentProcess(), 0 );
}


/***********************************************************************
 *           InitTask  (KERNEL.91)
 *
 * Called by the application startup code.
 */
void WINAPI InitTask16( CONTEXT *context )
{
    TDB *pTask;
    INSTANCEDATA *pinstance;
    SEGPTR ptr;

    context->Eax = 0;
    if (!(pTask = TASK_GetCurrent())) return;

    /* Note: we need to trust that BX/CX contain the stack/heap sizes,
       as some apps, notably Visual Basic apps, *modify* the heap/stack
       size of the instance data segment before calling InitTask() */

    /* Initialize the INSTANCEDATA structure */
    pinstance = MapSL( MAKESEGPTR(CURRENT_DS, 0) );
    pinstance->stackmin    = OFFSETOF(getWOW32Reserved()) + sizeof( STACK16FRAME );
    pinstance->stackbottom = pinstance->stackmin; /* yup, that's right. Confused me too. */
    pinstance->stacktop    = ( pinstance->stackmin > LOWORD(context->Ebx) ?
                               pinstance->stackmin - LOWORD(context->Ebx) : 0 ) + 150;

    /* Initialize the local heap */
    if (LOWORD(context->Ecx))
        LocalInit16( GlobalHandleToSel16(pTask->hInstance), 0, LOWORD(context->Ecx) );

    /* Initialize implicitly loaded DLLs */
    NE_InitializeDLLs( pTask->hModule );
    NE_DllProcessAttach( pTask->hModule );

    /* Registers on return are:
     * ax     1 if OK, 0 on error
     * cx     stack limit in bytes
     * dx     cmdShow parameter
     * si     instance handle of the previous instance
     * di     instance handle of the new task
     * es:bx  pointer to command line inside PSP
     *
     * 0 (=%bp) is pushed on the stack
     */
    ptr = stack16_push( sizeof(WORD) );
    *(WORD *)MapSL(ptr) = 0;
    context->Esp -= 2;

    context->Eax = 1;

    if (!pTask->pdb.cmdLine[0]) context->Ebx = 0x80;
    else
    {
        LPBYTE p = &pTask->pdb.cmdLine[1];
        while ((*p == ' ') || (*p == '\t')) p++;
        context->Ebx = 0x80 + (p - pTask->pdb.cmdLine);
    }
    context->Ecx   = pinstance->stacktop;
    context->Edx   = pTask->nCmdShow;
    context->Esi   = (DWORD)pTask->hPrevInstance;
    context->Edi   = (DWORD)pTask->hInstance;
    context->SegEs = (WORD)pTask->hPDB;
}


/***********************************************************************
 *           WaitEvent  (KERNEL.30)
 */
BOOL16 WINAPI WaitEvent16( HTASK16 hTask )
{
    TDB *pTask;

    if (!hTask) hTask = GetCurrentTask();
    pTask = TASK_GetPtr( hTask );

    if (pTask->flags & TDBF_WIN32)
    {
        FIXME("called for Win32 thread (%04x)!\n", GetCurrentThreadId());
        return TRUE;
    }

    if (pTask->nEvents > 0)
    {
        pTask->nEvents--;
        return FALSE;
    }

    if (pTask->teb == NtCurrentTeb())
    {
        DWORD lockCount;

        NtResetEvent( pTask->hEvent, NULL );
        ReleaseThunkLock( &lockCount );
        SYSLEVEL_CheckNotLevel( 1 );
        WaitForSingleObject( pTask->hEvent, INFINITE );
        RestoreThunkLock( lockCount );
        if (pTask->nEvents > 0) pTask->nEvents--;
    }
    else FIXME("for other task %04x cur=%04x\n",pTask->hSelf,GetCurrentTask());

    return TRUE;
}


/***********************************************************************
 *           PostEvent  (KERNEL.31)
 */
void WINAPI PostEvent16( HTASK16 hTask )
{
    TDB *pTask;

    if (!hTask) hTask = GetCurrentTask();
    if (!(pTask = TASK_GetPtr( hTask ))) return;

    if (pTask->flags & TDBF_WIN32)
    {
        FIXME("called for Win32 thread (%04x)!\n", (DWORD)pTask->teb->ClientId.UniqueThread );
        return;
    }

    pTask->nEvents++;

    if (pTask->nEvents == 1) NtSetEvent( pTask->hEvent, NULL );
}


/***********************************************************************
 *           SetPriority  (KERNEL.32)
 */
void WINAPI SetPriority16( HTASK16 hTask, INT16 delta )
{
    TDB *pTask;
    INT16 newpriority;

    if (!hTask) hTask = GetCurrentTask();
    if (!(pTask = TASK_GetPtr( hTask ))) return;
    newpriority = pTask->priority + delta;
    if (newpriority < -32) newpriority = -32;
    else if (newpriority > 15) newpriority = 15;

    pTask->priority = newpriority + 1;
    TASK_UnlinkTask( pTask->hSelf );
    TASK_LinkTask( pTask->hSelf );
    pTask->priority--;
}


/***********************************************************************
 *           LockCurrentTask  (KERNEL.33)
 */
HTASK16 WINAPI LockCurrentTask16( BOOL16 bLock )
{
    if (bLock) hLockedTask = GetCurrentTask();
    else hLockedTask = 0;
    return hLockedTask;
}


/***********************************************************************
 *           IsTaskLocked  (KERNEL.122)
 */
HTASK16 WINAPI IsTaskLocked16(void)
{
    return hLockedTask;
}


/***********************************************************************
 *           OldYield  (KERNEL.117)
 */
void WINAPI OldYield16(void)
{
   DWORD count;

   SetEvent(tls_get_kernel_thread_data()->idle_event);
   ReleaseThunkLock(&count);
   RestoreThunkLock(count);
}

/***********************************************************************
 *           WIN32_OldYield  (KERNEL.447)
 */
void WINAPI WIN32_OldYield16(void)
{
   DWORD count;

   SetEvent(tls_get_kernel_thread_data()->idle_event);
   ReleaseThunkLock(&count);
   RestoreThunkLock(count);
}

/***********************************************************************
 *           DirectedYield  (KERNEL.150)
 */
void WINAPI DirectedYield16( HTASK16 hTask )
{
    TDB *tdb = TASK_GetPtr(hTask);
    DWORD count;
    struct kernel_thread_data *chdthd;
    HTASK task;
    BOOL failed = FALSE;
    HTASK current = GetCurrentTask();
    if (!tdb->teb)
    {
        OldYield16();
        return;
    }
    SetEvent(tls_get_kernel_thread_data()->idle_event);
    task = hFirstTask;
    chdthd = (struct kernel_thread_data *)TebTlsGetValue(tdb->teb, kernel_thread_data_tls);
    if (chdthd->yield_event)
    {
        WARN("nested DirectedYield doesnt work.\n");
        OldYield16();
        return;
    }
    task = hFirstTask;
    while (task)
    {
        tdb = TASK_GetPtr(task);
        struct kernel_thread_data *td = (struct kernel_thread_data *)TebTlsGetValue(tdb->teb, kernel_thread_data_tls);
        if (td->yield_event)
        {
            if (td->htask16 == current)
            {
                DWORD count;
                SetEvent(tls_get_kernel_thread_data()->idle_event);
                ReleaseThunkLock(&count);
                Sleep(10);
                RestoreThunkLock(count);
                return;
            }
            failed = TRUE;
        }
        if (td->yield_wait_event)
        {
            failed = TRUE;
            return;
        }
        task = tdb->hNext;
    }
    if (failed)
    {
        WARN("nested DirectedYield doesnt work.\n");
        OldYield16();
        return;
    }
    chdthd->yield_event = CreateEventA(NULL, TRUE, FALSE, NULL);
    task = hFirstTask;
    /* All threads expect hTask or current task are locked by yield_wait_event */
    while (task)
    {
        tdb = TASK_GetPtr(task);
        if (task != hTask && tdb->teb->ClientId.UniqueThread != GetCurrentThreadId())
        {
            struct kernel_thread_data *td = (struct kernel_thread_data *)TebTlsGetValue(tdb->teb, kernel_thread_data_tls);
            td->yield_wait_event = CreateEventA(NULL, TRUE, FALSE, NULL);
        }
        task = tdb->hNext;
    }
    ReleaseThunkLock(&count);
    /*
     * In win16, if hTask doesn't wait events, another task will be executed.
     * Here, wait until timeout.
     */
    WaitForSingleObject(chdthd->yield_event, 100);
    RestoreThunkLock(count);
    CloseHandle(chdthd->yield_event);
    chdthd->yield_event = NULL;
    task = hFirstTask;
    while (task)
    {
        tdb = TASK_GetPtr(task);
        if (task != hTask && tdb->teb->ClientId.UniqueThread != GetCurrentThreadId())
        {
            struct kernel_thread_data *td = (struct kernel_thread_data *)TebTlsGetValue(tdb->teb, kernel_thread_data_tls);
            SetEvent(td->yield_wait_event);
            td->yield_wait_event = NULL;
        }
        task = tdb->hNext;
    }
}

/***********************************************************************
 *           Yield  (KERNEL.29)
 */
void WINAPI Yield16(void)
{
    TDB *pCurTask = TASK_GetCurrent();

    if (pCurTask && pCurTask->hQueue)
    {
        DWORD count;
        ReleaseThunkLock(&count);
        HMODULE mod = GetModuleHandleA( "user32.dll" );
        if (mod)
        {
            BOOL (WINAPI *pPeekMessageW)( MSG *msg, HWND hwnd, UINT first, UINT last, UINT flags );
            pPeekMessageW = (void *)GetProcAddress( mod, "PeekMessageW" );
            if (pPeekMessageW)
            {
                MSG msg;
                pPeekMessageW( &msg, 0, 0, 0, PM_REMOVE | PM_QS_SENDMESSAGE );
                RestoreThunkLock(count);
                return;
            }
        }
        RestoreThunkLock(count);
    }
    OldYield16();
}

/***********************************************************************
 *           KERNEL_490  (KERNEL.490)
 */
HTASK16 WINAPI KERNEL_490( HTASK16 someTask )
{
    if ( !someTask ) return 0;

    FIXME("(%04x): stub\n", someTask );
    return 0;
}

/***********************************************************************
 *           MakeProcInstance  (KERNEL.51)
 */
FARPROC16 WINAPI MakeProcInstance16( FARPROC16 func, HANDLE16 hInstance )
{
    struct thunk *thunk;
    BYTE *lfunc;
    SEGPTR thunkaddr;
    WORD hInstanceSelector;

    hInstanceSelector = GlobalHandleToSel16(hInstance);

    TRACE("(%p, %04x);\n", func, hInstance);

    if (!HIWORD(func)) {
      /* Win95 actually protects via SEH, but this is better for debugging */
      WARN("Ouch ! Called with invalid func %p !\n", func);
      return NULL;
    }

    if ( (GlobalHandleToSel16(CURRENT_DS) != hInstanceSelector)
      && (hInstance != 0)
      && (hInstance != 0xffff) )
    {
	/* calling MPI with a foreign DSEG is invalid ! */
        WARN("Problem with hInstance? Got %04x, using %04x instead\n",
                   hInstance,CURRENT_DS);
    }

    /* Always use the DSEG that MPI was entered with.
     * We used to set hInstance to GetTaskDS16(), but this should be wrong
     * as CURRENT_DS provides the DSEG value we need.
     * ("calling" DS, *not* "task" DS !) */
    hInstanceSelector = CURRENT_DS;
    hInstance = GlobalHandle16(hInstanceSelector);

    /* no thunking for DLLs */
    if (NE_GetPtr(FarGetOwner16(hInstance))->ne_flags & NE_FFLAGS_LIBMODULE)
	return func;

    thunkaddr = TASK_AllocThunk();
    if (!thunkaddr) return NULL;
    thunk = MapSL( thunkaddr );
    lfunc = MapSL( (SEGPTR)func );

    TRACE("(%p,%04x): got thunk %08x\n", func, hInstance, thunkaddr );
    if (((lfunc[0]==0x8c) && (lfunc[1]==0xd8)) || /* movw %ds, %ax */
    	((lfunc[0]==0x1e) && (lfunc[1]==0x58))    /* pushw %ds, popw %ax */
    ) {
    	WARN("This was the (in)famous \"thunk useless\" warning. We thought we have to overwrite with nop;nop;, but this isn't true.\n");
    }

    thunk->movw     = 0xb8;    /* movw instance, %ax */
    thunk->instance = hInstanceSelector;
    thunk->ljmp     = 0xea;    /* ljmp func */
    thunk->func     = func;
    return (FARPROC16)thunkaddr;
    /* CX reg indicates if thunkaddr != NULL, implement if needed */
}


/***********************************************************************
 *           FreeProcInstance  (KERNEL.52)
 */
void WINAPI FreeProcInstance16( FARPROC16 func )
{
    TRACE("(%p)\n", func );
    TASK_FreeThunk( (SEGPTR)func );
}

/**********************************************************************
 *	    TASK_GetCodeSegment
 *
 * Helper function for GetCodeHandle/GetCodeInfo: Retrieve the module
 * and logical segment number of a given code segment.
 *
 * 'proc' either *is* already a pair of module handle and segment number,
 * in which case there's nothing to do.  Otherwise, it is a pointer to
 * a function, and we need to retrieve the code segment.  If the pointer
 * happens to point to a thunk, we'll retrieve info about the code segment
 * where the function pointed to by the thunk resides, not the thunk itself.
 *
 * FIXME: if 'proc' is a SNOOP16 return stub, we should retrieve info about
 *        the function the snoop code will return to ...
 *
 */
static BOOL TASK_GetCodeSegment( FARPROC16 proc, NE_MODULE **ppModule,
                                 SEGTABLEENTRY **ppSeg, int *pSegNr )
{
    NE_MODULE *pModule = NULL;
    SEGTABLEENTRY *pSeg = NULL;
    int segNr=0;

    /* Try pair of module handle / segment number */
    pModule = GlobalLock16( HIWORD( proc ) );
    if ( pModule && pModule->ne_magic == IMAGE_OS2_SIGNATURE )
    {
        segNr = LOWORD( proc );
        if ( segNr && segNr <= pModule->ne_cseg )
            pSeg = NE_SEG_TABLE( pModule ) + segNr-1;
    }

    /* Try thunk or function */
    else
    {
        BYTE *thunk = MapSL( (SEGPTR)proc );
        WORD selector;

        if ((thunk[0] == 0xb8) && (thunk[3] == 0xea))
            selector = thunk[6] + (thunk[7] << 8);
        else
            selector = HIWORD( proc );

        pModule = NE_GetPtr( GlobalHandle16( selector ) );
        pSeg = pModule? NE_SEG_TABLE( pModule ) : NULL;

        if ( pModule )
            for ( segNr = 1; segNr <= pModule->ne_cseg; segNr++, pSeg++ )
                if ( GlobalHandleToSel16(pSeg->hSeg) == selector )
                    break;

        if ( pModule && segNr > pModule->ne_cseg )
            pSeg = NULL;
    }

    /* Abort if segment not found */

    if ( !pModule || !pSeg )
        return FALSE;

    /* Return segment data */

    if ( ppModule ) *ppModule = pModule;
    if ( ppSeg    ) *ppSeg    = pSeg;
    if ( pSegNr   ) *pSegNr   = segNr;

    return TRUE;
}

/**********************************************************************
 *	    GetCodeHandle    (KERNEL.93)
 */
DWORD WINAPI GetCodeHandle16( FARPROC16 proc )
{
    SEGTABLEENTRY *pSeg;

    if ( !TASK_GetCodeSegment( proc, NULL, &pSeg, NULL ) )
        return 0;

    return MAKELONG( pSeg->hSeg, GlobalHandleToSel16(pSeg->hSeg) );
}

/**********************************************************************
 *	    GetCodeInfo    (KERNEL.104)
 */
BOOL16 WINAPI GetCodeInfo16( FARPROC16 proc, SEGINFO *segInfo )
{
    NE_MODULE *pModule;
    SEGTABLEENTRY *pSeg;
    int segNr;

    if ( !TASK_GetCodeSegment( proc, &pModule, &pSeg, &segNr ) )
        return FALSE;

    /* Fill in segment information */

    segInfo->offSegment = pSeg->filepos;
    segInfo->cbSegment  = pSeg->size;
    segInfo->flags      = pSeg->flags;
    segInfo->cbAlloc    = pSeg->minsize;
    segInfo->h          = pSeg->hSeg;
    segInfo->alignShift = pModule->ne_align;

    if ( segNr == pModule->ne_autodata )
        segInfo->cbAlloc += pModule->ne_heap + pModule->ne_stack;

    /* Return module handle in %es */

    CURRENT_STACK16->es = GlobalHandleToSel16( pModule->self );

    return TRUE;
}


/**********************************************************************
 *          DefineHandleTable    (KERNEL.94)
 */
BOOL16 WINAPI DefineHandleTable16( WORD wOffset )
{
    FIXME("(%04x): stub ?\n", wOffset);
    return TRUE;
}


/***********************************************************************
 *           SetTaskQueue  (KERNEL.34)
 */
HQUEUE16 WINAPI SetTaskQueue16( HTASK16 hTask, HQUEUE16 hQueue )
{
    TDB *tdb;
    HQUEUE16 old;
    if (!hTask)
        hTask = GetCurrentTask();
    tdb = (TDB*)GlobalLock16(hTask);
    old = tdb->hQueue;
    tdb->hQueue = hQueue;
    return old;
}


/***********************************************************************
 *           GetTaskQueue  (KERNEL.35)
 */
HQUEUE16 WINAPI GetTaskQueue16( HTASK16 hTask )
{
    TDB *tdb;
    if (!hTask)
        hTask = GetCurrentTask();
    tdb = (TDB*)GlobalLock16(hTask);
    return tdb->hQueue;
}

/***********************************************************************
 *           SetThreadQueue  (KERNEL.463)
 */
HQUEUE16 WINAPI SetThreadQueue16( DWORD thread, HQUEUE16 hQueue )
{
    FIXME( "stub, should not get called\n" );
    return 0xbeef;
}

/***********************************************************************
 *           GetThreadQueue  (KERNEL.464)
 */
HQUEUE16 WINAPI GetThreadQueue16( DWORD thread )
{
    FIXME( "stub, should not get called\n" );
    return 0xbeef;
}

/***********************************************************************
 *           SetFastQueue  (KERNEL.624)
 */
VOID WINAPI SetFastQueue16( DWORD thread, HQUEUE16 hQueue )
{
    FIXME( "stub, should not get called\n" );
}

/***********************************************************************
 *           GetFastQueue  (KERNEL.625)
 */
HQUEUE16 WINAPI GetFastQueue16( void )
{
    FIXME( "stub, should not get called\n" );
    return 0xbeef;
}

/***********************************************************************
 *           SwitchStackTo   (KERNEL.108)
 */
void WINAPI SwitchStackTo16( WORD seg, WORD ptr, WORD top )
{
    STACK16FRAME *oldFrame, *newFrame;
    INSTANCEDATA *pData;
    UINT16 copySize;

    if (!(pData = GlobalLock16( seg ))) return;
    TRACE("old=%04x:%04x new=%04x:%04x\n",
          SELECTOROF( getWOW32Reserved() ),
          OFFSETOF( getWOW32Reserved() ), seg, ptr );

    /* Save the old stack */

    oldFrame = CURRENT_STACK16;
    /* pop frame + args and push bp */
    pData->old_ss_sp   = (SEGPTR)getWOW32Reserved() + sizeof(STACK16FRAME)
                           + 2 * sizeof(WORD);
    *(WORD *)MapSL(pData->old_ss_sp) = oldFrame->bp;
    pData->stacktop    = top;
    pData->stackmin    = ptr;
    pData->stackbottom = ptr;

    /* Switch to the new stack */

    /* Note: we need to take the 3 arguments into account; otherwise,
     * the stack will underflow upon return from this function.
     */
    copySize = oldFrame->bp - OFFSETOF(pData->old_ss_sp);
    copySize += 3 * sizeof(WORD) + sizeof(STACK16FRAME);
	setWOW32Reserved((void *)MAKESEGPTR(seg, ptr - copySize));
    newFrame = CURRENT_STACK16;

    /* Copy the stack frame and the local variables to the new stack */

    memmove( newFrame, oldFrame, copySize );
    newFrame->bp = ptr;
    *(WORD *)MapSL( MAKESEGPTR( seg, ptr ) ) = 0;  /* clear previous bp */
}


/***********************************************************************
 *           SwitchStackBack   (KERNEL.109)
 */
void WINAPI SwitchStackBack16( CONTEXT *context )
{
    STACK16FRAME *oldFrame, *newFrame;
    INSTANCEDATA *pData;

	if (!(pData = GlobalLock16(SELECTOROF(getWOW32Reserved()))))
        return;
    if (!pData->old_ss_sp)
    {
        WARN("No previous SwitchStackTo\n" );
        return;
    }
    TRACE("restoring stack %04x:%04x -> %04x:%04x\n",
          SELECTOROF(getWOW32Reserved()), OFFSETOF(getWOW32Reserved()),
          SELECTOROF(pData->old_ss_sp), OFFSETOF(pData->old_ss_sp) );

    oldFrame = CURRENT_STACK16;

    /* Pop bp from the previous stack */

    context->Ebp = (context->Ebp & ~0xffff) | *(WORD *)MapSL(pData->old_ss_sp);
    pData->old_ss_sp += sizeof(WORD);

    /* Switch back to the old stack */

	setWOW32Reserved((void *)(pData->old_ss_sp - sizeof(STACK16FRAME) - 6));
    context->SegSs = SELECTOROF(pData->old_ss_sp);
    context->Esp   = OFFSETOF(pData->old_ss_sp) - sizeof(DWORD) + 4 - sizeof(STACK16FRAME) + 44 - 6; /*ret addr*/
    pData->old_ss_sp = 0;

    /* Build a stack frame for the return */

    newFrame = CURRENT_STACK16;
    newFrame->frame32     = oldFrame->frame32;
    newFrame->module_cs   = oldFrame->module_cs;
    newFrame->callfrom_ip = oldFrame->callfrom_ip;
    newFrame->entry_ip    = oldFrame->entry_ip;
    newFrame->cs          = oldFrame->cs;
    newFrame->ip          = oldFrame->ip;
}


/***********************************************************************
 *           GetTaskQueueDS  (KERNEL.118)
 */
void WINAPI GetTaskQueueDS16(void)
{
    CURRENT_STACK16->ds = GlobalHandleToSel16( GetTaskQueue16(0) );
}


/***********************************************************************
 *           GetTaskQueueES  (KERNEL.119)
 */
void WINAPI GetTaskQueueES16(void)
{
    CURRENT_STACK16->es = GlobalHandleToSel16( GetTaskQueue16(0) );
}


/***********************************************************************
 *           GetCurrentTask   (KERNEL32.@)
 */
HTASK16 WINAPI GetCurrentTask(void)
{
    HTASK16 ret = kernel_get_thread_data()->htask16;
    if (!ret) ret = main_task;
    return ret;
}

/***********************************************************************
 *           GetCurrentTask   (KERNEL.36)
 */
DWORD WINAPI WIN16_GetCurrentTask(void)
{
    /* This is the version used by relay code; the first task is */
    /* returned in the high word of the result */
    return MAKELONG( GetCurrentTask(), hFirstTask );
}


/***********************************************************************
 *           GetCurrentPDB   (KERNEL.37)
 *
 * UNDOC: returns PSP of KERNEL in high word
 */
DWORD WINAPI GetCurrentPDB16(void)
{
    TDB *pTask;

    if (!(pTask = TASK_GetCurrent())) return 0;
    return MAKELONG(pTask->hPDB, 0); /* FIXME */
}


/***********************************************************************
 *           GetCurPID   (KERNEL.157)
 */
DWORD WINAPI GetCurPID16( DWORD unused )
{
    return 0;
}


/***********************************************************************
 *           GetInstanceData   (KERNEL.54)
 */
INT16 WINAPI GetInstanceData16( HINSTANCE16 instance, WORD buffer, INT16 len )
{
    char *ptr = GlobalLock16( instance );
    if (!ptr || !len) return 0;
    if ((int)buffer + len >= 0x10000) len = 0x10000 - buffer;
    memcpy( (char *)GlobalLock16(CURRENT_DS) + buffer, ptr + buffer, len );
    return len;
}


/***********************************************************************
 *           GetExeVersion   (KERNEL.105)
 */
WORD WINAPI GetExeVersion16(void)
{
    TDB *pTask;

    if (!(pTask = TASK_GetCurrent())) return 0;
    return pTask->version;
}


/***********************************************************************
 *           SetErrorMode   (KERNEL.107)
 */
UINT16 WINAPI SetErrorMode16( UINT16 mode )
{
    TDB *pTask;
    if (!(pTask = TASK_GetCurrent())) return 0;
    pTask->error_mode = mode;
    return SetErrorMode( mode );
}


/***********************************************************************
 *           GetNumTasks   (KERNEL.152)
 */
UINT16 WINAPI GetNumTasks16(void)
{
    return nTaskCount;
}


/***********************************************************************
 *           GetTaskDS   (KERNEL.155)
 *
 * Note: this function apparently returns a DWORD with LOWORD == HIWORD.
 * I don't think we need to bother with this.
 */
HINSTANCE16 WINAPI GetTaskDS16(void)
{
    TDB *pTask;

    if (!(pTask = TASK_GetCurrent())) return 0;
    return GlobalHandleToSel16(pTask->hInstance);
}

/***********************************************************************
 *           GetDummyModuleHandleDS   (KERNEL.602)
 */
WORD WINAPI GetDummyModuleHandleDS16(void)
{
    TDB *pTask;
    WORD selector;

    if (!(pTask = TASK_GetCurrent())) return 0;
    if (!(pTask->flags & TDBF_WIN32)) return 0;
    selector = GlobalHandleToSel16( pTask->hModule );
    CURRENT_DS = selector;
    return selector;
}

/***********************************************************************
 *           IsTask   (KERNEL.320)
 */
BOOL16 WINAPI IsTask16( HTASK16 hTask )
{
    TDB *pTask;

    if (!(pTask = TASK_GetPtr( hTask ))) return FALSE;
    if (GlobalSize16( hTask ) < sizeof(TDB)) return FALSE;
    return (pTask->magic == TDB_MAGIC);
}


/***********************************************************************
 *           IsWinOldApTask   (KERNEL.158)
 */
BOOL16 WINAPI IsWinOldApTask16( HTASK16 hTask )
{
	TDB *tdb = TASK_GetPtr(hTask);
    if (!tdb)
        return FALSE;
    NE_MODULE *ne_mod = NE_GetPtr(tdb->hModule);
    if (!ne_mod)
        return FALSE;
	if (!ne_mod->ne_expver)
		return TRUE;
    /* should return bit 0 of byte 0x48 in PSP */
    return FALSE;
}

BOOL16 WINAPI IsOldWindowsTask(HINSTANCE16 hInst)
{
    HMODULE16 hmod = GetExePtr(hInst);
    if (!hmod)
        return FALSE;
    NE_MODULE *ne_mod = NE_GetPtr(hmod);
    if (!ne_mod)
        return FALSE;
	if (ne_mod->ne_expver < 0x300)
		return TRUE;
	return FALSE;
}

/* FIXME: Windows 2.x/386 programs run in protected mode?? */
BOOL WINAPI IsRealModeTask(HINSTANCE16 hInst)
{
    return IsOldWindowsTask(hInst);
}

/***********************************************************************
 *           SetTaskSignalProc   (KERNEL.38)
 */
FARPROC16 WINAPI SetTaskSignalProc( HTASK16 hTask, FARPROC16 proc )
{
    TDB *pTask;
    FARPROC16 oldProc;

    if (!hTask) hTask = GetCurrentTask();
    if (!(pTask = TASK_GetPtr( hTask ))) return NULL;
    oldProc = pTask->userhandler;
    pTask->userhandler = proc;
    return oldProc;
}

/***********************************************************************
 *           SetSigHandler   (KERNEL.140)
 */
WORD WINAPI SetSigHandler16( FARPROC16 newhandler, FARPROC16* oldhandler,
                           UINT16 *oldmode, UINT16 newmode, UINT16 flag )
{
    FIXME("(%p,%p,%p,%d,%d), unimplemented.\n",
	  newhandler,oldhandler,oldmode,newmode,flag );

    if (flag != 1) return 0;
    if (!newmode) newhandler = NULL;  /* Default handler */
    if (newmode != 4)
    {
        TDB *pTask;

        if (!(pTask = TASK_GetCurrent())) return 0;
        if (oldmode) *oldmode = pTask->signal_flags;
        pTask->signal_flags = newmode;
        if (oldhandler) *oldhandler = pTask->sighandler;
        pTask->sighandler = newhandler;
    }
    return 0;
}


/***********************************************************************
 *           GlobalNotify   (KERNEL.154)
 *
 * Note that GlobalNotify does _not_ return the old NotifyProc
 * -- contrary to LocalNotify !!
 */
VOID WINAPI GlobalNotify16( FARPROC16 proc )
{
    TDB *pTask;

    if (!(pTask = TASK_GetCurrent())) return;
    pTask->discardhandler = proc;
}


/***********************************************************************
 *           GetExePtrHelper
 */
static inline HMODULE16 GetExePtrHelper( HANDLE16 handle, HTASK16 *hTask )
{
    char *ptr;
    HANDLE16 owner;

      /* Check for module handle */

    if (!(ptr = GlobalLock16( handle ))) return 0;
    if (((NE_MODULE *)ptr)->ne_magic == IMAGE_OS2_SIGNATURE) return handle;

      /* Search for this handle inside all tasks */

    *hTask = hFirstTask;
    while (*hTask)
    {
        TDB *pTask = TASK_GetPtr( *hTask );
        if ((*hTask == handle) ||
            (pTask->hInstance == handle) ||
            (pTask->hQueue == handle) ||
            (pTask->hPDB == handle)) return pTask->hModule;
        *hTask = pTask->hNext;
    }

      /* Check the owner for module handle */

    owner = FarGetOwner16( handle );
    if (!(ptr = GlobalLock16( owner ))) return 0;
    if (((NE_MODULE *)ptr)->ne_magic == IMAGE_OS2_SIGNATURE) return owner;

      /* Search for the owner inside all tasks */

    *hTask = hFirstTask;
    while (*hTask)
    {
        TDB *pTask = TASK_GetPtr( *hTask );
        if ((*hTask == owner) ||
            (pTask->hInstance == owner) ||
            (pTask->hQueue == owner) ||
            (pTask->hPDB == owner)) return pTask->hModule;
        *hTask = pTask->hNext;
    }

    return 0;
}

/***********************************************************************
 *           GetExePtr   (KERNEL.133)
 */
HMODULE16 WINAPI WIN16_GetExePtr( HANDLE16 handle )
{
    HTASK16 hTask = 0;
    HMODULE16 hModule = GetExePtrHelper( handle, &hTask );
    STACK16FRAME *frame = CURRENT_STACK16;
    frame->ecx = hModule;
    if (hTask) frame->es = hTask;
    return hModule;
}


/***********************************************************************
 *           K228   (KERNEL.228)
 */
HMODULE16 WINAPI GetExePtr( HANDLE16 handle )
{
    HTASK16 hTask = 0;
    return GetExePtrHelper( handle, &hTask );
}


typedef INT (WINAPI *MessageBoxA_funcptr)(HWND hWnd, LPCSTR text, LPCSTR title, UINT type);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtRaiseHardError(
    _In_ NTSTATUS ErrorStatus,
    _In_ ULONG NumberOfParameters,
    _In_ ULONG UnicodeStringParameterMask,
    _In_ PULONG_PTR Parameters,
    _In_ ULONG ValidResponseOptions,
    _Out_ PULONG Response
);
#define HARDERROR_OVERRIDE_ERRORMODE        0x10000000

typedef enum _HARDERROR_RESPONSE_OPTION
{
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;
/**************************************************************************
 *           FatalAppExit   (KERNEL.137)
 */
void WINAPI FatalAppExit16( UINT16 action, LPCSTR str )
{
    TDB *pTask = TASK_GetCurrent();

    if (!pTask || !(pTask->error_mode & SEM_NOGPFAULTERRORBOX))
    {
        UNICODE_STRING uni;
        ANSI_STRING ansi;
        ULONG response;
        PUNICODE_STRING puni;
        RtlInitAnsiString(&ansi, str);
        RtlAnsiStringToUnicodeString(&uni, &ansi, TRUE);
        puni = &uni;
        NTSTATUS status = NtRaiseHardError(STATUS_FATAL_APP_EXIT | HARDERROR_OVERRIDE_ERRORMODE, 1, 1, (PULONG_PTR)&puni /* pointer to pointer to UNICODE_STRING */, OptionOk, &response);
        RtlFreeUnicodeString(&uni);
        if (status == STATUS_NOT_IMPLEMENTED)
        {
            HMODULE mod = GetModuleHandleA( "user32.dll" );
            if (mod)
            {
                MessageBoxA_funcptr pMessageBoxA = (MessageBoxA_funcptr)GetProcAddress( mod, "MessageBoxA" );
                if (pMessageBoxA)
                {
                    pMessageBoxA( 0, str, NULL, MB_SYSTEMMODAL | MB_OK );
                    goto done;
                }
            }
        }
        ERR( "%s\n", debugstr_a(str) );
    }
 done:
    ExitThread(0xff);
}


/***********************************************************************
 *           GetAppCompatFlags   (KERNEL.354)
 */
DWORD WINAPI GetAppCompatFlags16( HTASK16 hTask )
{
    TDB *pTask;

    if (!hTask) hTask = GetCurrentTask();
    if (!(pTask=TASK_GetPtr( hTask ))) return 0;
    if (GlobalSize16(hTask) < sizeof(TDB)) return 0;
    return pTask->compat_flags;
}


const char *env_var_limitation[] =
{
    "COMSPEC", "TEMP", "TMP", "PROCESSOR_ARCHITECTURE"
};
static BOOL env_var_limit(const char *v)
{
    static BOOL init;
    int i;
    static BOOL limit;
    if (!init)
    {
        limit = krnl386_get_config_int("otvdm", "EnvironmentVariableLimitation", TRUE);
    }
    if (!limit)
        return TRUE;
    SIZE_T len = strchr(v, '=') ? strchr(v, '=') - v : strlen(v);
    if ((len > 1) && (v[len - 1] == '\x16'))
        return TRUE;
    for (i = 0; i < ARRAY_SIZE(env_var_limitation); i++)
    {
        if (strlen(env_var_limitation[i]) == len && !memicmp(v, env_var_limitation[i], len))
            return TRUE;
    }
    return FALSE;
}

static void parse_autoexec()
{
    BOOL pathfound = FALSE;
    char autoexec_path[MAX_PATH];
    int len = 0;
    RedirectDriveRoot("C:\\AUTOEXEC.BAT", &autoexec_path, MAX_PATH, TRUE);
    FILE *aeb = fopen(autoexec_path, "r");
    if (aeb)
    {
        while (!feof(aeb))
        {
            char line[1024], name[128], var[256], newvar[512], *ptr;
            int pos;
            ptr = fgets(&line, 1024, aeb);
            if (!ptr)
                continue;
            pos = strlen(line);
            if (line[pos - 1] == '\n')
                line[pos - 1] = '\0';
            pos = strspn(line, " \t");
            if (!strnicmp(line, "SET ", 4))
            {
                pos = strspn(line + pos + 4, " ") + pos + 4;
                ptr = strchr(line + pos, '=');
                if (!ptr)
                    continue;
                int namelen = (int)(ptr - line) - pos;
                if (namelen >= 125)
                    continue;
                strncpy(name, line + pos, namelen);
                strncpy(var, ptr + 1, 256);
                name[namelen] = '\0';
                if (!strlen(var))
                    continue;
            }
            else if (!strnicmp(line, "PATH", 4))
            {
                pos = strspn(line + pos + 4, " ") + pos + 4;
                if (line[pos] == '=')
                {
                    pos++;
                    pos = strspn(line + pos, " ") + pos;
                }
                if (!strlen(line + pos))
                    continue;
                strcpy(name, "PATH");
                strncpy(var, line + pos, 256);
            }
            else
                continue;

            // use host env var if it exists and will be copied
            if (env_var_limit(name) && GetEnvironmentVariable(name, NULL, 0))
                continue;

            // special handling for path because we want to include path16 but exclude the host path
            if (!strcmp(name, "PATH"))
            {
                char path[MAX_PATH];
                if (ptr = strstr(var, "%PATH%"))
                    strcpy(ptr, ptr + 6);
                if (GetEnvironmentVariable("PATH\x16", path, MAX_PATH))
                {
                    strcat(var, ";");
                    strcat(var, path);
                }
                if (GetEnvironmentVariable("PATH16", path, MAX_PATH) && !pathfound)
                {
                    strcat(var, ";");
                    strcat(var, path);
                }
                pathfound = TRUE;
            }
            strcat(name, "\x16");
            ExpandEnvironmentStringsA(var, newvar, 512);
            SetEnvironmentVariable(name, newvar);
        }
        fclose(aeb);
    }
    // at least one program requires some kind of path
    char path[MAX_PATH];
    if (!pathfound)
    {
        if (!GetEnvironmentVariable("PATH16", path, MAX_PATH))
            RedirectSystemDir("C:\\WINDOWS\\", path, MAX_PATH);
        SetEnvironmentVariable("PATH\x16", path);
    }
    GetEnvironmentVariable("PATH\x16", path, MAX_PATH);
    SetEnvironmentVariable("PATH16", path);
}

/***********************************************************************
 *           GetDOSEnvironment     (KERNEL.131)
 *
 * Note: the environment is allocated once, it doesn't track changes
 * made using the Win32 API. This shouldn't matter.
 *
 * Format of a 16-bit environment block:
 * ASCIIZ   string 1 (xx=yy format)
 * ...
 * ASCIIZ   string n
 * BYTE     0
 * WORD     1
 * ASCIIZ   program name (e.g. C:\WINDOWS\SYSTEM\KRNL386.EXE)
 */
SEGPTR WINAPI GetDOSEnvironment16(void)
{
    static const char ENV_program_name[] = "C:\\WINDOWS\\SYSTEM\\KRNL386.EXE";
    static HGLOBAL16 handle;  /* handle to the 16 bit environment */

    if (!handle)
    {
        DWORD size = 0;
        LPSTR p, env;

        parse_autoexec();

        p = env = GetEnvironmentStringsA();
        while (*p)
        {
            if (env_var_limit(p))
            {
                size += strlen(p) + 1;
            }
            p += strlen(p) + 1;
        }
        size++;  /* skip last null */
        size += sizeof(WORD) + sizeof(ENV_program_name) + 1;
        handle = GlobalAlloc16( GMEM_FIXED, size );
        if (handle)
        {
            WORD one = 1;
            LPSTR env16 = GlobalLock16( handle );
            LPSTR env16p = env16;
            p = env;
            while (*p)
            {
                if (env_var_limit(p))
                {
                    int i;
                    for (i = 0; i < strlen(p) + 1; i++)
                    {
                        if (p[i] == '=')
                        {
                            if (i && (p[i - 1] == '\x16'))
                            {
                                i--;
                                p++;
                            }
                            break;
                        }
                        env16p[i] = toupper(p[i]);
                    }
                    memcpy(env16p + i, p + i, strlen(p + i) + 1);
                    env16p += strlen(p) + 1;
                }
                p += strlen(p) + 1;
            }
            *env16p = 0;
            env16p++;
            memcpy( env16p, &one, sizeof(one));
            env16p += sizeof(one);
            memcpy( env16p, ENV_program_name, sizeof(ENV_program_name));
            GlobalUnlock16( handle );
        }
        FreeEnvironmentStringsA( env );
    }
    return WOWGlobalLock16( handle );
}

/*
Emulate an independent current directory for each task
*/
void switch_directory(struct kernel_thread_data *thread_data)
{
    if (!thread_data->htask16)
        return;
    if (curdir_tls_index == -1)
    {
        curdir_tls_index = TlsAlloc();
    }
    if (task_old != thread_data->htask16)
    {
        TDB *tdb = TASK_GetCurrent();
        if (!thread_data->curdir_len)
        {
            thread_data->curdir_len = 32768;
            thread_data->true_curdir = HeapAlloc(GetProcessHeap(), 0, thread_data->curdir_len * 2);
            thread_data->true_curdir[0] = (tdb->curdrive & ~0x80) + 'A';
            thread_data->true_curdir[1] = ':';
            thread_data->curdir_buf = thread_data->true_curdir + thread_data->curdir_len;
            TebTlsSetValue(tdb->teb ? tdb->teb : NtCurrentTeb(), curdir_tls_index, thread_data->true_curdir);

            GetCurrentDirectoryA(thread_data->curdir_len, thread_data->true_curdir);
            GetShortPathNameA(thread_data->true_curdir, thread_data->true_curdir, thread_data->curdir_len);
            memcpy(thread_data->curdir_buf, thread_data->true_curdir, thread_data->curdir_len);
            SetCurrentDirectory16(thread_data->true_curdir);
        }
        if (TlsGetValue(curdir_tls_index) == 0xdead)
        {
            task_old = NULL;
            task_old_data = NULL;
            return;
        }
        if (!task_old || !task_old_data || !task_old_data->curdir_buf)
        {
            task_old = NULL;
            task_old_data = NULL;
        }
        if (task_old)
        {
            /*
            save current directory
            curdir_buf -> new curdir
            true_curdir-> old curdir
            tdb->curdir-> old curdir
            */
            GetCurrentDirectoryA(task_old_data->curdir_len, task_old_data->curdir_buf);
            if (strcmp(task_old_data->curdir_buf, task_old_data->true_curdir))
            {
                /* changed */
                GetShortPathNameA(task_old_data->curdir_buf, task_old_data->curdir_buf, task_old_data->curdir_len);
                strcpy(task_old_data->true_curdir, task_old_data->curdir_buf);
                TDB *old = ((TDB*)GlobalLock16(task_old_data->htask16));
                GetShortPathNameA(task_old_data->true_curdir + 2, old->curdir, sizeof(old->curdir));
                GetShortPathNameA(task_old_data->true_curdir, task_old_data->true_curdir, task_old_data->curdir_len);
                if (!thread_data->htask16)
                    old = NULL;
                old->curdrive = 0x80 | (task_old_data->true_curdir[0] - 'A');
                GlobalUnlock16(task_old_data->htask16);
                /*
                curdir_buf -> new curdir
                true_curdir-> new curdir
                tdb->curdir-> new curdir
                */
                TRACE("%.*s %p save cur dir %s\n", 8, old->module_name, old, task_old_data->true_curdir);
            }
        }
        /* restore current directory */
        SetCurrentDirectoryA(thread_data->true_curdir);
        task_old_data = thread_data;
        task_old = thread_data->htask16;
    }
}
