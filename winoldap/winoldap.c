

#include "config.h"


#include "windef.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "winuser.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/list.h"
#include "wine/debug.h"
#include "shellapi.h"
#include "stdio.h"
#include "stdlib.h"
#include "shlwapi.h"

WINE_DEFAULT_DEBUG_CHANNEL(winoldap);

//LRESULT WINAPI WndProc(HWND16 h, &)
LRESULT WINAPI WndProc(HWND16 hwnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    ERR("wndproc");
}
LRESULT WINAPI DialogProc(HWND16 hwnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    ERR("dialogproc");
}

/*** PIF file structures ***/
#include "pshpack1.h"

/* header of a PIF file */
typedef struct {
    BYTE unk1[2];               /* 0x00 */
    CHAR windowtitle[ 30 ];     /* 0x02 seems to be padded with blanks*/ 
    WORD memmax;                /* 0x20 */
    WORD memmin;                /* 0x22 */
    CHAR program[63];           /* 0x24 seems to be zero terminated */
    BYTE hdrflags1;             /* 0x63 various flags:
                                 *  02 286: text mode selected
                                 *  10 close window at exit
                                 */
    BYTE startdrive;            /* 0x64 */
    char startdir[64];          /* 0x65 */
    char optparams[64];         /* 0xa5 seems to be zero terminated */
    BYTE videomode;             /* 0xe5 */
    BYTE unkn2;                 /* 0xe6 ?*/
    BYTE irqlow;                /* 0xe7 */
    BYTE irqhigh;               /* 0xe8 */
    BYTE rows;                  /* 0xe9 */
    BYTE cols;                  /* 0xea */
    BYTE winY;                  /* 0xeb */
    BYTE winX;                  /* 0xec */
    WORD unkn3;                 /* 0xed 7??? */
    CHAR unkn4[64];             /* 0xef */
    CHAR unkn5[64];             /* 0x12f */
    BYTE hdrflags2;             /* 0x16f */
    BYTE hdrflags3;             /* 0x170 */
    } pifhead_t;

/* record header: present on every record */
typedef struct {
    CHAR recordname[16];  /* zero terminated */
    WORD posofnextrecord; /* file offset, 0xffff if last */
    WORD startofdata;     /* file offset */
    WORD sizeofdata;      /* data is expected to follow directly */
} recordhead_t;

/* 386 -enhanced mode- record */
typedef struct {
    WORD memmax;         /* memory desired, overrides the pif header*/
    WORD memmin;         /* memory required, overrides the pif header*/
    WORD prifg;          /* foreground priority */
    WORD pribg;          /* background priority */
    WORD emsmax;         /* EMS memory limit */
    WORD emsmin;         /* EMS memory required */
    WORD xmsmax;         /* XMS memory limit */
    WORD xmsmin;         /* XMS memory required */
    WORD optflags;        /* option flags:
                           *  0008 full screen
                           *  0004 exclusive
                           *  0002 background
                           *  0001 close when active
                           */
    WORD memflags;       /* various memory flags*/
    WORD videoflags;     /* video flags:
                          *   0010 text
                          *   0020 med. res. graphics
                          *   0040 hi. res. graphics
                          */
    WORD hotkey[9];      /* Hot key info */
    CHAR optparams[64];  /* optional params, replaces those in the pif header */
} pif386rec_t;

#include "poppack.h"

/***********************************************************************
 *           start_dos_exe
 */
static VOID start_dos_exe( LPCSTR filename, LPCSTR cmdline )
{
    char fullcmdline[MAX_PATH*2];
    PROCESS_INFORMATION pi;
    STARTUPINFOA si = {0};
    DWORD count;
    si.cb = sizeof(STARTUPINFOA);
    char exename[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA(NULL), exename, sizeof(exename));
    krnl386_get_config_string("otvdm", "dosemu", exename, exename, sizeof(exename));
    snprintf(fullcmdline, 512, "\"%s\" %s %s", exename, filename, cmdline);
    BOOL ret = CreateProcessA(NULL, fullcmdline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    
    if (!ret)
    {
        char *msg;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, &msg, 256, NULL);
        WINE_MESSAGE( "winevdm: Cannot start DOS application %s err: %s\n", filename, msg );
        LocalFree(msg);
        ReleaseThunkLock(&count);
        Sleep(100); // wait for other tasks to finish, hopefully preventing deadlock
        RestoreThunkLock(count);
        ExitThread(1);
    }
    /* Give 10 seconds to the app to come up */
    if (WaitForInputIdle(pi.hProcess, 10000) == WAIT_FAILED)
        WINE_WARN("WaitForInputIdle failed: Error %d\n", GetLastError());
    ReleaseThunkLock(&count);
    WaitForSingleObject(pi.hProcess, INFINITE);
    RestoreThunkLock(count);

    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    ExitThread(exit_code);
}

static VOID start_bat(LPCSTR filename, LPCSTR cmdline)
{
    char fullcmdline[MAX_PATH*2];
    PROCESS_INFORMATION pi;
    STARTUPINFOA si = {0};
    DWORD count;
    si.cb = sizeof(STARTUPINFOA);
    char exename[MAX_PATH];
    if (!GetEnvironmentVariableA("COMSPEC", exename, MAX_PATH))
        strcpy(exename, "CMD.EXE");
    snprintf(fullcmdline, 512, "\"%s\" /c %s %s", exename, filename, cmdline);
    BOOL ret = CreateProcessA(NULL, fullcmdline, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    
    if (!ret)
    {
        char *msg;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, &msg, 256, NULL);
        WINE_MESSAGE( "winevdm: Cannot start batch file %s err: %s\n", filename, msg );
        LocalFree(msg);
        ReleaseThunkLock(&count);
        Sleep(100); // wait for other tasks to finish, hopefully preventing deadlock
        RestoreThunkLock(count);
        ExitThread(1);
    }
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    ExitThread(1);
}

/***********************************************************************
 *           read_pif_file
 *pif386rec_tu
 * Read a pif file and return the header and possibly the 286 (real mode)
 * record or 386 (enhanced mode) record. Returns FALSE if the file is
 * invalid otherwise TRUE.
 */
static BOOL read_pif_file( HANDLE hFile, char *progname, char *title,
        char *optparams, char *startdir, int *closeonexit, int *textmode)
{
    DWORD nread;
    LARGE_INTEGER filesize;
    recordhead_t rhead;
    BOOL found386rec = FALSE;
    pif386rec_t pif386rec;
    pifhead_t pifheader;
    if( !GetFileSizeEx( hFile, &filesize) ||
            filesize.QuadPart <  (sizeof(pifhead_t) + sizeof(recordhead_t))) {
        WINE_ERR("Invalid pif file: size error %d\n", (int)filesize.QuadPart);
        return FALSE;
    }
    SetFilePointer( hFile, 0, NULL, FILE_BEGIN);
    if( !ReadFile( hFile, &pifheader, sizeof(pifhead_t), &nread, NULL))
        return FALSE;
    WINE_TRACE("header: program %s title %s startdir %s params %s\n",
            wine_dbgstr_a(pifheader.program),
            wine_dbgstr_an(pifheader.windowtitle, sizeof(pifheader.windowtitle)),
            wine_dbgstr_a(pifheader.startdir),
            wine_dbgstr_a(pifheader.optparams)); 
    WINE_TRACE("header: memory req'd %d desr'd %d drive %d videomode %d\n",
            pifheader.memmin, pifheader.memmax, pifheader.startdrive,
            pifheader.videomode);
    WINE_TRACE("header: flags 0x%x 0x%x 0x%x\n",
            pifheader.hdrflags1, pifheader.hdrflags2, pifheader.hdrflags3);
    ReadFile( hFile, &rhead, sizeof(recordhead_t), &nread, NULL);
    if( strncmp( rhead.recordname, "MICROSOFT PIFEX", 15)) {
        WINE_ERR("Invalid pif file: magic string not found\n");
        return FALSE;
    }
    /* now process the following records */
    while( 1) {
        WORD nextrecord = rhead.posofnextrecord;
        if( (nextrecord & 0x8000) || 
                filesize.QuadPart <( nextrecord + sizeof(recordhead_t))) break;
        if( !SetFilePointer( hFile, nextrecord, NULL, FILE_BEGIN) ||
                !ReadFile( hFile, &rhead, sizeof(recordhead_t), &nread, NULL))
            return FALSE;
        if( !rhead.recordname[0]) continue; /* deleted record */
        WINE_TRACE("reading record %s size %d next 0x%x\n",
                wine_dbgstr_a(rhead.recordname), rhead.sizeofdata,
                rhead.posofnextrecord );
        if( !strncmp( rhead.recordname, "WINDOWS 386", 11)) {
            found386rec = TRUE;
            ReadFile( hFile, &pif386rec, sizeof(pif386rec_t), &nread, NULL);
            WINE_TRACE("386rec: memory req'd %d des'd %d EMS req'd %d des'd %d XMS req'd %d des'd %d\n",
                    pif386rec.memmin, pif386rec.memmax,
                    pif386rec.emsmin, pif386rec.emsmax,
                    pif386rec.xmsmin, pif386rec.xmsmax);
            WINE_TRACE("386rec: option 0x%x memory 0x%x video 0x%x\n",
                    pif386rec.optflags, pif386rec.memflags,
                    pif386rec.videoflags);
            WINE_TRACE("386rec: optional parameters %s\n",
                    wine_dbgstr_a(pif386rec.optparams));
        }
    }
    /* prepare the return data */
    lstrcpynA( progname, pifheader.program, sizeof(pifheader.program)+1);
    lstrcpynA( title, pifheader.windowtitle, sizeof(pifheader.windowtitle)+1);
    if( found386rec)
        lstrcpynA( optparams, pif386rec.optparams, sizeof( pif386rec.optparams)+1);
    else
        lstrcpynA( optparams, pifheader.optparams, sizeof(pifheader.optparams)+1);
    lstrcpynA( startdir, pifheader.startdir, sizeof(pifheader.startdir)+1);
    *closeonexit = pifheader.hdrflags1 & 0x10;
    *textmode = found386rec ? pif386rec.videoflags & 0x0010
                            : pifheader.hdrflags1 & 0x0002;
    return TRUE;
}

/***********************************************************************
 *              pif_cmd
 *
 * execute a pif file.
 */
static VOID pif_cmd( char *filename, char *cmdline)
{ 
    HANDLE hFile;
    char progpath[MAX_PATH];
    char buf[128];
    char progname[64];
    char title[31];
    char optparams[65];
    char startdir[65];
    char *p;
    int closeonexit;
    int textmode;
    if( (hFile = CreateFileA( filename, GENERIC_READ, FILE_SHARE_READ,
                    NULL, OPEN_EXISTING, 0, 0 )) == INVALID_HANDLE_VALUE)
    {
        WINE_ERR("open file %s failed\n", wine_dbgstr_a(filename));
        return;
    }
    if( !read_pif_file( hFile, progname, title, optparams, startdir,
                &closeonexit, &textmode)) {
        WINE_ERR( "failed to read %s\n", wine_dbgstr_a(filename));
        CloseHandle( hFile);
        sprintf( buf, "%s\nInvalid file format. Check your pif file.", 
                filename);
        MessageBoxA( NULL, buf, "16 bit DOS subsystem", MB_OK|MB_ICONWARNING);
        SetLastError( ERROR_BAD_FORMAT);
        return;
    }
    CloseHandle( hFile);
    if( (p = strrchr( progname, '.')) && !_stricmp( p, ".bat"))
        WINE_FIXME(".bat programs in pif files are not supported.\n"); 
    /* first change dir, so the search below can start from there */
    if( startdir[0] && !SetCurrentDirectoryA( startdir)) {
        WINE_ERR("Cannot change directory %s\n", wine_dbgstr_a( startdir));
        sprintf( buf, "%s\nInvalid startup directory. Check your pif file.", 
                filename);
        MessageBoxA( NULL, buf, "16 bit DOS subsystem", MB_OK|MB_ICONWARNING);
    }
    /* search for the program */
    if( !SearchPathA( NULL, progname, NULL, MAX_PATH, progpath, NULL )) {
        sprintf( buf, "%s\nInvalid program file name. Check your pif file.", 
                filename);
        MessageBoxA( NULL, buf, "16 bit DOS subsystem", MB_OK|MB_ICONERROR);
        SetLastError( ERROR_FILE_NOT_FOUND);
        return;
    }
    /* if no arguments on the commandline, use them from the pif file */
    if( !cmdline[0] && optparams[0])
        cmdline = optparams;
    /* FIXME: do something with:
     * - close on exit
     * - graphic modes
     * - hot key's
     * - etc.
     */ 
    start_dos_exe( progpath, cmdline );
}

/**************************************************************************
*           WINOLDAP entry point
*/
WORD WINAPI WinMain16(HINSTANCE16 inst, HINSTANCE16 prev, LPSTR cmdline, WORD show)
{
    DWORD count, exit_code = 1;

    WINE_TRACE("%x %x %s %u\n", inst, prev, wine_dbgstr_a(cmdline), show);

    if (!memcmp("-WoAWoW32", cmdline, sizeof("-WoAWoW32") / sizeof(char) - sizeof(char)))
    {
        HANDLE handle;
        handle = (HANDLE)strtoull(cmdline + sizeof("-WoAWoW32") / sizeof(char) - sizeof(char), NULL, 16);
        DWORD count;
        ReleaseThunkLock(&count);
        WaitForSingleObject(handle, INFINITE);
        RestoreThunkLock(count);
        GetExitCodeProcess(handle, &exit_code);
        ExitThread(exit_code);
    }

    char path[MAX_PATH];
    LPCSTR filename = path;
    LPCSTR name_beg, name_end, p, args = NULL;
    if (cmdline == '"') /* has to be only one and only at beginning ! */
    {
        name_beg = cmdline + 1;
        p = strchr(cmdline + 1, '"');
        if (p)
        {
            name_end = p;
            args = strchr(p, ' ');
        }
        else /* yes, even valid with trailing '"' missing */
            name_end = cmdline + strlen(cmdline);
    }
    else
    {
        name_beg = cmdline;
        args = strchr(cmdline, ' ');
        name_end = args ? args : cmdline + strlen(cmdline);
    }
    if ((name_beg == cmdline) && (!args))
    { /* just use the original cmdline string as file name */
        filename = cmdline;
        args = "";
    }
    else
    {
        memcpy(path, name_beg, name_end - name_beg);
        path[name_end - name_beg] = '\0';
    }

    if (StrStrIA(filename, ".pif"))
        pif_cmd(filename, args);
    else if (StrStrIA(filename, ".bat"))
        start_bat(filename, args);
    else
        start_dos_exe(filename, args);
    
    ExitThread(1);
}

