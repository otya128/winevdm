/*
 * File handling functions
 *
 * Copyright 1993 John Burton
 * Copyright 1996 Alexandre Julliard
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
 *
 * TODO:
 *    Fix the CopyFileEx methods to implement the "extended" functionality.
 *    Right now, they simply call the CopyFile method.
 */

#include "config.h"
#include "wine/port.h"

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "winerror.h"
#include "windef.h"
#include "winbase.h"
#include "winternl.h"
#include "wine/winbase16.h"
#include "kernel16_private.h"
#include "wine/unicode.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(file);

#include <shlwapi.h>
#include <imagehlp.h>
#include <aclapi.h>
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "shlwapi.lib")
#ifdef ENABLEREDIRECTSYSTEMDIR
BOOL EnableRedirectSystemDir = TRUE;
#else
BOOL EnableRedirectSystemDir = FALSE;
#endif
const char *GetRedirectWindowsDir();
static void RedirectPrivateProfileStringWindowsDir(LPCSTR filename, LPCSTR output);

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheck(IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN HANDLE TokenHandle,
    IN ACCESS_MASK DesiredAccess,
    IN PGENERIC_MAPPING GenericMapping,
    OUT PPRIVILEGE_SET PrivilegeSet OPTIONAL,
    IN OUT PULONG PrivilegeSetLength,
    OUT PACCESS_MASK GrantedAccess,
    OUT PNTSTATUS AccessStatus);
BOOL can_write_directory(LPCSTR path)
{
    HANDLE hToken = INVALID_HANDLE_VALUE, hTokenImpersonation = INVALID_HANDLE_VALUE;
    PSECURITY_DESCRIPTOR pSecurityDescriptor = NULL;
    GENERIC_MAPPING genericMapping = { 0 };
    PRIVILEGE_SET privilegeSet = { 0 };
    NTSTATUS accessStatus = 0;
    DWORD dwDesiredAccess = FILE_GENERIC_WRITE;
    DWORD dwSize = sizeof(PRIVILEGE_SET);
    DWORD dwGrantedAccess = 0;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken))
        goto fail;
    if (!DuplicateTokenEx(hToken, GENERIC_ALL, NULL, SecurityImpersonation, TokenImpersonation, &hTokenImpersonation))
        goto fail;
    if (GetNamedSecurityInfoA(path, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, NULL, NULL, NULL, &pSecurityDescriptor) != ERROR_SUCCESS)
        goto fail;
    genericMapping.GenericRead = FILE_GENERIC_READ;
    genericMapping.GenericWrite = FILE_GENERIC_WRITE;
    genericMapping.GenericExecute = FILE_GENERIC_EXECUTE;
    genericMapping.GenericAll = FILE_ALL_ACCESS;
    MapGenericMask(&dwDesiredAccess, &genericMapping);
    /* Because app compatibility layer hooks AccessCheck and returns incorrect result, call NtAccessCheck. (NS_WRPMitigation::APIHook_AccessCheck) */
    if (!NT_SUCCESS(NtAccessCheck(pSecurityDescriptor, hTokenImpersonation, dwDesiredAccess, &genericMapping, &privilegeSet, &dwSize, &dwGrantedAccess, &accessStatus)))
        goto fail;
fail:
    CloseHandle(hToken);
    CloseHandle(hTokenImpersonation);
    LocalFree(pSecurityDescriptor);
    return NT_SUCCESS(accessStatus);
}
BOOL is_readonly_directory(LPCSTR path)
{
    DWORD attr = GetFileAttributesA(path);
    if (attr == -1)
        return FALSE;
    if (GetDriveTypeA(path) == DRIVE_CDROM)
        return TRUE;
    return (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY;
}
__declspec(dllexport) LPCSTR RedirectDriveRoot(LPCSTR path, LPSTR to, size_t max_len, BOOL silence)
{
    LPCSTR path_old = path;
    enum WRITABLE_CACHE
    {
        NOTCACHED,
        ALLOWED,
        DENIED,
        READONLY,
    };
    static enum WRITABLE_CACHE writable_cache[26];
    char drive = path ? path[0] : 0;
    char root[4] = { drive, ':', '\\', 0 };
    char drive_buf[5] = { '.', '.', '\\', drive, 0 };
    if (!path)
        return NULL;
    if (!(path[0] && path[1] == ':' && (path[2] == 0 || path[2] == '\\' || path[2] == '/')))
        return path;
    if (!((path[0] >= 'A' && path[0] <= 'Z') || (path[0] >= 'a' && path[0] <= 'z')))
        return path;
    if (drive >= 'a' && drive <= 'z')
    {
        drive -= 'a' - 'A';
    }
    path += 2;
    if (path[0])
        path += 1;
    if (strchr(path, '\\'))
        return path_old;
    if (strchr(path, '/'))
        return path_old;
    if (writable_cache[drive - 'A'] == NOTCACHED)
    {
        if (is_readonly_directory(root))
        {
            writable_cache[drive - 'A'] = READONLY;
            return path_old;
        }
        if (can_write_directory(root))
        {
            writable_cache[drive - 'A'] = ALLOWED;
            return path_old;
        }
        else
        {
            writable_cache[drive - 'A'] = DENIED;
        }
    }
    else
    {
        if (writable_cache[drive - 'A'] == ALLOWED || writable_cache[drive - 'A'] == READONLY)
        {
            return path_old;
        }
    }

    PathCombineA(to, GetRedirectWindowsDir(), drive_buf);
    CreateDirectoryA(to, NULL);
    PathCombineA(to, to, path);
    if (!silence)
    {
        ERR("%s => %s\n", path_old, to);
    }
    return to;
}
/* buf: X:\XXX\YYY\C\ZZZ => C:\ZZZ */
void UnredirectDriveRoot(LPSTR buf, SIZE_T buf_len)
{
    char drive_root[MAX_PATH];
    PathCombineA(drive_root, GetRedirectWindowsDir(), "..\\");
    if (memcmp(drive_root, buf, min(strlen(buf), strlen(drive_root))) == 0)
    {
        LPCSTR drive_dir = buf + strlen(drive_root);
        if (drive_dir[0] >= 'A' && drive_dir[0] <= 'Z' && drive_dir[1] == '\\')
        {
            buf[0] = drive_dir[0];
            buf[1] = ':';
            buf[2] = '\\';
            memmove(buf + 3, drive_dir + 2, strlen(drive_dir + 2) + 1);
        }
        return;
    }
    return;
}
//SYSTEM DIR
//%WINDIR%->
__declspec(dllexport) LPCSTR RedirectSystemDir(LPCSTR path, LPSTR to, size_t max_len)
{
	char dirbuf[240];
	if (!EnableRedirectSystemDir) return path;
	char buf[MAX_PATH];
	if (!PathCanonicalizeA(buf, path))
	{
		return path;
	}
	//\SYSTEM32
	const char *windir = getenv("windir");
	if (strlen(windir) + 10 >= MAX_PATH)
	{
		return path;
	}
	char bufwdir[MAX_PATH];
	strcpy(bufwdir, windir);
	if (PathCommonPrefixA(buf, bufwdir, NULL) < strlen(bufwdir))//if (strncmp(buf, windir, strlen(windir)))
	{
		return path;
	}
	if (!PathAppendA(bufwdir, "system32"))
	{
		return path;
	}
	//system32->system
	if (PathCommonPrefixA(buf, bufwdir, NULL) >= strlen(bufwdir))//if (strncmp(buf, windir, strlen(windir)))
	{
		memcpy(buf + strlen(bufwdir) - 2, buf + strlen(bufwdir), strlen(buf) - strlen(bufwdir) + 1);
	}
	//.\windir\ 
	dirbuf[0] = '\0';
	char*dir = &dirbuf[0];
	GetModuleFileNameA(GetModuleHandleA("otvdm.exe"), dir, MAX_PATH);
	char *file = PathFindFileNameA(dir);
	if (file != dir) *file = '\0';
	if (!PathAppendA(dir, GetRedirectWindowsDir()))
		return path;
		//FIXME:dir//wine_get_server_dir();
//	size_t len = strlen(dir);
//	if (len >= max_len) return path;
//	strcpy(to, dir);
	if (!PathAppendA(dir, buf + strlen(windir)))
		return path;
	if (strlen(dir) >= max_len)
		return path;
	file = PathFindFileNameA(dir);
	char f = *file;
	*file = '\0';
	GetShortPathNameA(dir, to, max_len);
	*file = f;
	PathCombineA(to, to, file);
//	strcpy(to, dir);
	return to;
}

NTSYSAPI NTSTATUS NTAPI RtlUpcaseUnicodeStringToOemString(
    POEM_STRING      DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN          AllocateDestinationString
);
static LPWSTR strdupAtoW(LPCSTR str)
{
    LPWSTR ret;
    INT len;

    if (!str) return NULL;
    len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (ret) MultiByteToWideChar(CP_ACP, 0, str, -1, ret, len);
    return ret;
}
static LPWSTR strdupOEMtoW(LPCSTR str)
{
    LPWSTR ret;
    INT len;

    if (!str) return NULL;
    len = MultiByteToWideChar(CP_OEMCP, 0, str, -1, NULL, 0);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (ret) MultiByteToWideChar(CP_OEMCP, 0, str, -1, ret, len);
    return ret;
}
static LPCSTR strdupWtoOEM(LPCWSTR str)
{
    LPSTR ret;
    INT len;

    if (!str) return NULL;
    len = WideCharToMultiByte(CP_OEMCP, 0, str, -1, NULL, 0, NULL, NULL);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(CHAR));
    if (ret) WideCharToMultiByte(CP_OEMCP, 0, str, -1, ret, len, NULL, NULL);
    return ret;
}

#define DOS_TABLE_SIZE 256

static HANDLE dos_handles[DOS_TABLE_SIZE];

/***********************************************************************
 *           FILE_InitProcessDosHandles
 *
 * Allocates the default DOS handles for a process. Called either by
 * Win32HandleToDosFileHandle below or by the DOSVM stuff.
 */
static void FILE_InitProcessDosHandles( void )
{
    HANDLE hStdInput, hStdOutput, hStdError, hNull;
    static BOOL init_done /* = FALSE */;
    HANDLE cp = GetCurrentProcess();

    if (init_done) return;
    init_done = TRUE;
    hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    hStdError = GetStdHandle(STD_ERROR_HANDLE);
    hNull = CreateFileA("NUL", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
    /* Invalid console handles need to translate to real DOS handles in a new process */
    if (!hStdInput) hStdInput = hNull;
    if (!hStdOutput) hStdOutput = hNull;
    if (!hStdError) hStdError = hNull;
    DuplicateHandle(cp, hStdInput, cp, &dos_handles[0], 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(cp, hStdOutput, cp, &dos_handles[1], 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(cp, hStdError, cp, &dos_handles[2], 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(cp, hStdError, cp, &dos_handles[3], 0, TRUE, DUPLICATE_SAME_ACCESS);
    DuplicateHandle(cp, hStdError, cp, &dos_handles[4], 0, TRUE, DUPLICATE_SAME_ACCESS);
    CloseHandle(hNull);
}

/***********************************************************************
 *           DosFileHandleToWin32Handle   (KERNEL32.20)
 *
 * Return the Win32 handle for a DOS handle.
 *
 * Note: this is not exactly right, since on Win95 the Win32 handles
 *       are on top of DOS handles and we do it the other way
 *       around. Should be good enough though.
 */
HANDLE WINAPI DosFileHandleToWin32Handle( HFILE handle )
{
    HFILE16 hfile = (HFILE16)handle;
    if (hfile < 5) FILE_InitProcessDosHandles();
    if ((hfile >= DOS_TABLE_SIZE) || !dos_handles[hfile])
    {
        SetLastError( ERROR_INVALID_HANDLE );
        return INVALID_HANDLE_VALUE;
    }
    return dos_handles[hfile];
}

/***********************************************************************
 *           Win32HandleToDosFileHandle   (KERNEL32.21)
 *
 * Allocate a DOS handle for a Win32 handle. The Win32 handle is no
 * longer valid after this function (even on failure).
 *
 * Note: this is not exactly right, since on Win95 the Win32 handles
 *       are on top of DOS handles and we do it the other way
 *       around. Should be good enough though.
 */
HFILE WINAPI Win32HandleToDosFileHandle( HANDLE handle )
{
    int i;

    if (!handle || (handle == INVALID_HANDLE_VALUE))
        return HFILE_ERROR;

    for (i = 5; i < DOS_TABLE_SIZE; i++)
        if (!dos_handles[i])
        {
            dos_handles[i] = handle;
            TRACE("Got %d for h32 %p\n", i, handle );
            return (HFILE)i;
        }
    CloseHandle( handle );
    SetLastError( ERROR_TOO_MANY_OPEN_FILES );
    return HFILE_ERROR;
}

/***********************************************************************
 *           DisposeLZ32Handle   (KERNEL32.22)
 *
 * Note: this is not entirely correct, we should only close the
 *       32-bit handle and not the 16-bit one, but we cannot do
 *       this because of the way our DOS handles are implemented.
 *       It shouldn't break anything though.
 */
void WINAPI DisposeLZ32Handle( HANDLE handle )
{
    int i;

    if (!handle || (handle == INVALID_HANDLE_VALUE)) return;

    for (i = 5; i < DOS_TABLE_SIZE; i++)
        if (dos_handles[i] == handle)
        {
            dos_handles[i] = 0;
            /* lzexpand.dll16 uses wine-based lzexpand implementation. so call _lclose instead of LZClose */
            _lclose( handle );
            break;
        }
}

/***********************************************************************
 *           GetProfileInt   (KERNEL.57)
 */
UINT16 WINAPI GetProfileInt16( LPCSTR section, LPCSTR entry, INT16 def_val )
{
    return GetPrivateProfileInt16( section, entry, def_val, "win.ini" );
}


/***********************************************************************
 *           GetProfileString   (KERNEL.58)
 */
INT16 WINAPI GetProfileString16( LPCSTR section, LPCSTR entry, LPCSTR def_val,
                                 LPSTR buffer, UINT16 len )
{
    char tmp[256];
    int tmplen = 256;
    if (section && entry && !stricmp(section, "devices") && !stricmp(entry, "DefaultPrinter"))
    {
        if (GetDefaultPrinterA(tmp, &tmplen))
            entry = tmp;
    }
    INT16 ret = GetPrivateProfileString16( section, entry, def_val,
                                      buffer, len, "win.ini" );
    if (ret && section && entry && !stricmp(section, "windows") && !stricmp(entry, "device"))
    {
        char *comma = strchr(buffer, ',');
        if (!comma && ((len - 1) == ret))
        {
            ret = GetPrivateProfileString16( section, entry, def_val,
                                      tmp, tmplen, "win.ini" );
            comma = strchr(tmp, ',');
            if (comma && (len > (strlen(comma) + 14)))
            {
                strcpy(buffer, "DefaultPrinter");
                strcpy(buffer + 14, comma);
            }
        }
        else if (comma && ((comma - buffer) > 32))
        {
            strcpy(buffer, "DefaultPrinter");
            strcpy(buffer + 14, comma);
        }
        ret = strlen(buffer);
    }
    else if (ret && !PathIsRelativeA(buffer) && PathFileExistsA(buffer))
    {
        int cnt = GetShortPathNameA(buffer, tmp, len <= 256 ? len : 256);
        if (cnt)
            strcpy(buffer, tmp);
    }
    return ret;
}

/***********************************************************************
 *           WriteProfileString   (KERNEL.59)
 */
BOOL16 WINAPI WriteProfileString16( LPCSTR section, LPCSTR entry,
                                    LPCSTR string )
{
    return WritePrivateProfileString16( section, entry, string, "win.ini" );
}


/* get the search path for the current module; helper for OpenFile16 */
/*static */char *get_search_path(void)
{
    UINT len;
    char *ret, *p, module[OFS_MAXPATHNAME];

    module[0] = 0;
    if (GetCurrentTask() && GetModuleFileName16( GetCurrentTask(), module, sizeof(module) ))
    {
        if (!(p = strrchr( module, '\\' ))) p = module;
        *p = 0;
    }
    char windir[MAX_PATH];
    char windir2[MAX_PATH];
    char *windir3;
    GetWindowsDirectoryA(windir, MAX_PATH);
    windir3 = RedirectSystemDir(windir, windir2, MAX_PATH);
    char vdmpath[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA(NULL), vdmpath, MAX_PATH);
    PathRemoveFileSpecA(vdmpath);
    GetShortPathNameA(vdmpath, vdmpath, MAX_PATH);

    len = (strlen(vdmpath) + 1 + 2 +                                              /* search order: first current dir */
           GetSystemDirectory16( NULL, 0 ) + 1 +            /* then system dir */
           strlen(windir3) + 1 +            /* then windows dir */
           strlen( module ) + 1 +                           /* then module path */
           GetEnvironmentVariableA( "PATH16", NULL, 0 ) + 1); /* then look in PATH */
    if (!(ret = HeapAlloc( GetProcessHeap(), 0, len ))) return NULL;
    strcpy(ret, ".;");
    p = ret + 2;
    strcpy(p, vdmpath);
    p += strlen(vdmpath);
    *p++ = ';';

    GetSystemDirectory16( p, ret + len - p );
    p += strlen( p );
    *p++ = ';';
    strcpy(p, windir3);//GetWindowsDirectoryA( p, ret + len - p );
    p += strlen( p );
    *p++ = ';';
    if (module[0])
    {
        strcpy( p, module );
        p += strlen( p );
        *p++ = ';';
    }
    if (!GetEnvironmentVariableA("PATH16", p, ret + len - p))
    {
        *(p - 1) = '\0';
    }
    return ret;
}
char *krnl386_get_search_path(void)
{
    return get_search_path();
}
#define STRSAFE_NO_DEPRECATE
#include <strsafe.h>
LPCSTR krnl386_search_executable_file(LPCSTR lpFile, LPSTR buf, SIZE_T size, BOOL search_builtin)
{
    char *path = krnl386_get_search_path();
    LPCSTR result = lpFile;
    if (SearchPathA(path, lpFile, NULL, size, buf, NULL))
    {
        result = buf;
    }
    else
    {
        char dlldir[MAX_PATH];
        GetDllDirectoryA(MAX_PATH, dlldir);
        if (SearchPathA(dlldir, lpFile, NULL, size, buf, NULL))
        {
            result = buf;
        }
    }
    if (result == lpFile && search_builtin)
    {
        char filewin32[MAX_PATH];
        LPSTR ext = PathFindExtensionA(lpFile);
        if (ext && !stricmp(ext, ".dll") || !stricmp(ext, ".exe") || !stricmp(ext, ".drv"))
        {
            filewin32[0] = '\0';
            if (StringCbCatA(filewin32, ARRAYSIZE(filewin32), lpFile) == S_OK &&
                StringCbCatA(filewin32, ARRAYSIZE(filewin32), "16") == S_OK)
            {
                if (SearchPathA(path, filewin32, NULL, size, buf, NULL))
                {
                    result = buf;
                }
                else
                {
                    char dlldir[MAX_PATH];
                    GetDllDirectoryA(MAX_PATH, dlldir);
                    if (SearchPathA(dlldir, filewin32, NULL, size, buf, NULL))
                    {
                        result = buf;
                    }
                }
            }
        }
    }
    HeapFree(GetProcessHeap(), 0, path);
    return result;
}

/***********************************************************************
 *              create_file_OF
 *
 * Wrapper for CreateFile that takes OF_* mode flags.
 */
static HANDLE create_file_OF( LPCWSTR path, INT mode )
{
    DWORD access, sharing, creation;

    if (mode & OF_CREATE)
    {
        creation = CREATE_ALWAYS;
        access = GENERIC_READ | GENERIC_WRITE;
    }
    else
    {
        creation = OPEN_EXISTING;
        switch(mode & 0x03)
        {
        case OF_READ:      access = GENERIC_READ; break;
        case OF_WRITE:     access = GENERIC_WRITE; break;
        case OF_READWRITE: access = GENERIC_READ | GENERIC_WRITE; break;
        default:           access = 0; break;
        }
    }

    switch(mode & 0x70)
    {
    case OF_SHARE_EXCLUSIVE:  sharing = 0; break;
    case OF_SHARE_DENY_WRITE: sharing = FILE_SHARE_READ; break;
    case OF_SHARE_DENY_READ:  sharing = FILE_SHARE_WRITE; break;
    case OF_SHARE_DENY_NONE:
    case OF_SHARE_COMPAT:
    default:                  sharing = FILE_SHARE_READ | FILE_SHARE_WRITE; break;
    }
    return CreateFileW( path, access, sharing | FILE_SHARE_DELETE, NULL, creation, FILE_ATTRIBUTE_NORMAL, 0 );
}
/***********************************************************************
 *           OpenFile   (KERNEL.74)
 *           OpenFileEx (KERNEL.360)
 */
HFILE16 WINAPI WIN16_OpenFile16( LPCSTR name, SEGPTR ofs, UINT16 mode )
{
    CURRENT_STACK16->es = SELECTOROF(ofs);
    return OpenFile16(name, MapSL(ofs), mode);
}

HFILE16 WINAPI OpenFile16( LPCSTR name, OFSTRUCT *ofs, UINT16 mode )
{
    HFILE hFileRet = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;
    FILETIME filetime;
    WORD filedatetime[2];
    OEM_STRING oem;
    UNICODE_STRING uni;
    FILETIME lfiletime;
    WCHAR pathname[sizeof(ofs->szPathName)];
    LPWSTR namew;
    const WCHAR *p, *filename;
    CHAR buf[OFS_MAXPATHNAME];
    CHAR redir[MAX_PATH];
    BOOL result;
    oem.Buffer = ofs->szPathName;
    oem.Length = 0;
    oem.MaximumLength = sizeof(ofs->szPathName);

    if (!ofs) return HFILE_ERROR;

    TRACE("%s %s %s %s%s%s%s%s%s%s%s%s\n",debugstr_a(name),
          ((mode & 0x3 )==OF_READ)?"OF_READ":
          ((mode & 0x3 )==OF_WRITE)?"OF_WRITE":
          ((mode & 0x3 )==OF_READWRITE)?"OF_READWRITE":"unknown",
          ((mode & 0x70 )==OF_SHARE_COMPAT)?"OF_SHARE_COMPAT":
          ((mode & 0x70 )==OF_SHARE_DENY_NONE)?"OF_SHARE_DENY_NONE":
          ((mode & 0x70 )==OF_SHARE_DENY_READ)?"OF_SHARE_DENY_READ":
          ((mode & 0x70 )==OF_SHARE_DENY_WRITE)?"OF_SHARE_DENY_WRITE":
          ((mode & 0x70 )==OF_SHARE_EXCLUSIVE)?"OF_SHARE_EXCLUSIVE":"unknown",
          ((mode & OF_PARSE )==OF_PARSE)?"OF_PARSE ":"",
          ((mode & OF_DELETE )==OF_DELETE)?"OF_DELETE ":"",
          ((mode & OF_VERIFY )==OF_VERIFY)?"OF_VERIFY ":"",
          ((mode & OF_SEARCH )==OF_SEARCH)?"OF_SEARCH ":"",
          ((mode & OF_CANCEL )==OF_CANCEL)?"OF_CANCEL ":"",
          ((mode & OF_CREATE )==OF_CREATE)?"OF_CREATE ":"",
          ((mode & OF_PROMPT )==OF_PROMPT)?"OF_PROMPT ":"",
          ((mode & OF_EXIST )==OF_EXIST)?"OF_EXIST ":"",
          ((mode & OF_REOPEN )==OF_REOPEN)?"OF_REOPEN ":""
        );
    name = RedirectSystemDir(name, redir, MAX_PATH);

    if ((mode & (OF_CREATE | OF_SHARE_EXCLUSIVE)) == (OF_CREATE | OF_SHARE_EXCLUSIVE))
    {
        mode &= ~OF_SHARE_EXCLUSIVE;
    }

    {
        ofs->cBytes = sizeof(OFSTRUCT);
        ofs->nErrCode = 0;
        if (mode & OF_REOPEN) name = ofs->szPathName;

        if (!name)
        {
            return HFILE_ERROR;
        }
        name = RedirectDriveRoot(name, buf, ARRAY_SIZE(buf), FALSE);
        if (mode & OF_REOPEN)
            namew = strdupOEMtoW(name);
        else
            namew = strdupAtoW(name);

        /* the watcom 10.6 IDE relies on a valid path returned in ofs->szPathName
           Are there any cases where getting the path here is wrong?
           Uwe Bonnes 1997 Apr 2 */
        if (!*namew)
        {
            /* empty path => cwd */
            result = GetCurrentDirectoryW(sizeof(ofs->szPathName), pathname);
            if (result)
            {
                size_t len = wcslen(pathname);
                if (len < sizeof(ofs->szPathName) - 1 && pathname[len - 1] != '\\' && pathname[len - 1] != '/')
                {
                    pathname[len] = '\\';
                    pathname[len + 1] = 0;
                }
            }
            if (result && !(mode & OF_PARSE))
            {
                SetLastError(ERROR_FILE_NOT_FOUND);
                RtlInitUnicodeString(&uni, pathname);
                if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
                {
                    ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                    ofs->szPathName[0] = '\0';
                }
                goto error;
            }
        }
        else
        {
            result = GetFullPathNameW(namew, sizeof(ofs->szPathName), pathname, NULL);
        }
        if (!result)
        {
            RtlInitUnicodeString(&uni, namew);
            if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
            {
                ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                ofs->szPathName[0] = '\0';
            }
            goto error;
        }

        /* If OF_SEARCH is set, ignore the given path */

        filename = namew;
        if ((mode & OF_SEARCH) && !(mode & OF_REOPEN))
        {
            /* First try the file name as is */
            if (GetFileAttributesW( filename ) != INVALID_FILE_ATTRIBUTES) filename = NULL;
            else
            {
                /* Now remove the path */
                if (filename[0] && (filename[1] == ':')) filename += 2;
                if ((p = wcsrchr( filename, '\\' ))) filename = p + 1;
                if ((p = wcsrchr( filename, '/' ))) filename = p + 1;
                if (!filename[0])
                {
                    RtlInitUnicodeString(&uni, filename);
                    if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
                    {
                        ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                        ofs->szPathName[0] = '\0';
                    }
                    SetLastError( ERROR_FILE_NOT_FOUND );
                    goto error;
                }
            }
        }

        /* Now look for the file */

        if (!(mode & OF_PARSE) && !(mode & OF_CREATE) && filename)
        {
            BOOL found;
            char *path = get_search_path();
            WCHAR *pathw;

            if (!path)
            {
                RtlInitUnicodeString(&uni, filename);
                if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
                {
                    ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                    ofs->szPathName[0] = '\0';
                }
                goto error;
            }
            pathw = strdupAtoW(path);
            found = SearchPathW( pathw, filename, NULL, sizeof(ofs->szPathName),
                                 pathname, NULL );
            HeapFree( GetProcessHeap(), 0, path );
            HeapFree( GetProcessHeap(), 0, pathw );
            if (!found)
            {
                RtlInitUnicodeString(&uni, pathname);
                if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
                {
                    ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                    ofs->szPathName[0] = '\0';
                }
                goto error;
            }
        }

        TRACE("found %s\n", debugstr_w(pathname) );

        if (mode & OF_DELETE)
        {
            RtlInitUnicodeString(&uni, pathname);
            if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
            {
                ERR("RtlUpcaseUnicodeStringToOemString failed\n");
                ofs->szPathName[0] = '\0';
            }
            if (!DeleteFileW(pathname)) goto error;
            TRACE("(%s): OF_DELETE return = OK\n", name);
            HeapFree(GetProcessHeap(), 0, namew);
            return 1;
        }

        GetShortPathNameW(pathname, pathname, ARRAY_SIZE(pathname));
        if (pathname[0] && pathname[wcslen(pathname) - 1] == '\\')
        {
            SetLastError(ERROR_FILE_NOT_FOUND);
            handle = INVALID_HANDLE_VALUE;
            hFileRet = HFILE_ERROR16;
        }
        else
        {
            handle = create_file_OF(pathname, mode);
        }
        RtlInitUnicodeString(&uni, pathname);
        if (!NT_SUCCESS(RtlUpcaseUnicodeStringToOemString(&oem, &uni, FALSE)))
        {
            ERR("RtlUpcaseUnicodeStringToOemString failed\n");
            ofs->szPathName[0] = '\0';
        }
        UnredirectDriveRoot(ofs->szPathName, ARRAY_SIZE(ofs->szPathName));
        if (!(mode & OF_PARSE))
        {
            if (handle == INVALID_HANDLE_VALUE) goto error;
        }

        if (mode & OF_PARSE)
        {
            SYSTEMTIME systime;
            GetSystemTime(&systime);
            SystemTimeToFileTime(&systime, &filetime);
        }
        else
        {
            GetFileTime( handle, NULL, NULL, &filetime );
        }
        FileTimeToLocalFileTime(&filetime, &lfiletime);
        FileTimeToDosDateTime( &lfiletime, &filedatetime[0], &filedatetime[1] );
        if ((mode & OF_VERIFY) && (mode & OF_REOPEN))
        {
            if (ofs->Reserved1 != filedatetime[0] || ofs->Reserved2 != filedatetime[1] )
            {
                CloseHandle( handle );
                WARN("(%s): OF_VERIFY failed\n", name );
                /* FIXME: what error here? */
                SetLastError( ERROR_FILE_NOT_FOUND );
                goto error;
            }
        }
        ofs->Reserved1 = filedatetime[0];
        ofs->Reserved2 = filedatetime[1];
    }

    if (!(mode & OF_PARSE))
    {
        hFileRet = Win32HandleToDosFileHandle(handle);
        TRACE("(%s): OK, return = %p\n", name, handle);
    }
    if (hFileRet == HFILE_ERROR16) goto error;
    HeapFree(GetProcessHeap(), 0, namew);
    if (!(mode & OF_PARSE) && mode & OF_EXIST) _lclose16( hFileRet ); /* Return the handle, but close it first */
    return hFileRet;

error:  /* We get here if there was an error opening the file */
    ofs->nErrCode = GetLastError();
    WARN("(%s): return = HFILE_ERROR error= %d\n", name,ofs->nErrCode );
    HeapFree(GetProcessHeap(), 0, namew);
    return HFILE_ERROR16;
}


/***********************************************************************
 *           _lclose   (KERNEL.81)
 */
HFILE16 WINAPI _lclose16( HFILE16 hFile )
{
    if ((hFile >= DOS_TABLE_SIZE) || !dos_handles[hFile])
    {
        SetLastError( ERROR_INVALID_HANDLE );
        return HFILE_ERROR16;
    }
    TRACE("%d (handle32=%p)\n", hFile, dos_handles[hFile] );
    CloseHandle( dos_handles[hFile] );
    dos_handles[hFile] = 0;
    return 0;
}

/***********************************************************************
 *           _lcreat   (KERNEL.83)
 */
HFILE16 WINAPI _lcreat16( LPCSTR path, INT16 attr )
{
    CHAR buf[MAX_PATH];
    path = RedirectSystemDir(path, buf, MAX_PATH);
    path = RedirectDriveRoot(path, buf, MAX_PATH, FALSE);
    return Win32HandleToDosFileHandle( (HANDLE)_lcreat( path, attr ) );
}

/***********************************************************************
 *           _llseek   (KERNEL.84)
 */
LONG WINAPI _llseek16( HFILE16 hFile, LONG lOffset, INT16 nOrigin )
{
    HANDLE fd = DosFileHandleToWin32Handle(hFile);
    LONG high = 0;
    DWORD offset = (DWORD)lOffset;
    switch (nOrigin)
    {
        case FILE_BEGIN:
            break;
        case FILE_END:
            offset += GetFileSize(fd, NULL);
            break;
        case FILE_CURRENT:
            offset += SetFilePointer(fd, 0, NULL, FILE_CURRENT);
            break;
        default:
            SetLastError(ERROR_INVALID_PARAMETER);
            return -1;
    }
    return SetFilePointer(fd, offset, &high, FILE_BEGIN);
}


/***********************************************************************
 *           _lopen   (KERNEL.85)
 */
HFILE16 WINAPI _lopen16( LPCSTR path, INT16 mode )
{
    CHAR buf[MAX_PATH];
    path = RedirectSystemDir(path, buf, MAX_PATH);
    path = RedirectDriveRoot(path, buf, MAX_PATH, FALSE);
    return Win32HandleToDosFileHandle( (HANDLE)_lopen( path, mode ) );
}


/***********************************************************************
 *           _lread16 (internal)
 */
UINT16 WINAPI _lread16( HFILE16 hFile, LPVOID buffer, UINT16 count )
{
    return (UINT16)_lread((HFILE)DosFileHandleToWin32Handle(hFile), buffer, (LONG)count );
}


/***********************************************************************
 *           _lwrite   (KERNEL.86)
 */
UINT16 WINAPI _lwrite16( HFILE16 hFile, LPCSTR buffer, UINT16 count )
{
    return (UINT16)_hwrite( (HFILE)DosFileHandleToWin32Handle(hFile), buffer, (LONG)count );
}

/***********************************************************************
 *           _hread (KERNEL.349)
 */
LONG WINAPI WIN16_hread( HFILE16 hFile, SEGPTR buffer, LONG count )
{
    LONG maxlen;

    TRACE("%d %08x %d\n", hFile, (DWORD)buffer, count );

    /* Some programs pass a count larger than the allocated buffer */
    maxlen = GetSelectorLimit16( SELECTOROF(buffer) ) - OFFSETOF(buffer) + 1;
    if (count > maxlen)
    {
        LPVOID temp_buffer = HeapAlloc(GetProcessHeap(), 0, count);
        HFILE result = _lread((HFILE)DosFileHandleToWin32Handle(hFile), temp_buffer, count );
        if (result != HFILE_ERROR)
        {
            memcpy(MapSL(buffer), temp_buffer, (size_t)result);
        }
        HeapFree(GetProcessHeap(), 0, temp_buffer);
        return result;
    }

    return _lread((HFILE)DosFileHandleToWin32Handle(hFile), MapSL(buffer), count );
}


/***********************************************************************
 *           _lread (KERNEL.82)
 */
UINT16 WINAPI WIN16_lread( HFILE16 hFile, SEGPTR buffer, UINT16 count )
{
    return (UINT16)WIN16_hread( hFile, buffer, (LONG)count );
}


/***********************************************************************
 *           _hwrite   (KERNEL.350)
 */
LONG WINAPI _hwrite16( HFILE16 hFile, LPCSTR buffer, LONG count )
{
    return _hwrite( (HFILE)DosFileHandleToWin32Handle(hFile), buffer, count );
}


/***********************************************************************
 *           GetTempDrive   (KERNEL.92)
 * A closer look at krnl386.exe shows what the SDK doesn't mention:
 *
 * returns:
 *   AL: driveletter
 *   AH: ':'		- yes, some kernel code even does stosw with
 *                            the returned AX.
 *   DX: 1 for success
 */
UINT WINAPI GetTempDrive( BYTE ignored )
{
    WCHAR buffer[MAX_PATH];
    BYTE ret;

    if (GetTempPathW( MAX_PATH, buffer )) ret = (BYTE)toupperW(buffer[0]);
    else ret = 'C';
    return MAKELONG( ret | (':' << 8), 1 );
}


#define MAX_TEMP_PATH 144
/***********************************************************************
 *           GetTempFileName   (KERNEL.97)
 */
UINT16 WINAPI GetTempFileName16( BYTE drive, LPCSTR prefix, UINT16 unique,
                                 LPSTR buffer )
{
    char temppath[MAX_TEMP_PATH];
    char *unique_buf;

    if (!(drive & ~TF_FORCEDRIVE)) /* drive 0 means current default drive */
    {
        WCHAR curdir[MAX_PATH];
        DWORD r = GetCurrentDirectoryW(MAX_PATH, curdir);
        if (r >= MAX_PATH || r == 0 || curdir[1] != ':')
        {
            ERR("could not get current drive\n");
            return 0;
        }
        drive |= curdir[0];
    }

    if (drive & TF_FORCEDRIVE)
    {
        /* C:~preuuuu.TMP */
        sprintf(temppath, "%c:", drive & ~TF_FORCEDRIVE);
    }
    else
    {
        DWORD r = GetTempPathA(MAX_TEMP_PATH, temppath);
        if (r >= MAX_TEMP_PATH)
        {
            ERR("temp path is too long\n");
            return 0;
        }
        if (r < 3)
        {
            return 0;
        }
    }
    GetShortPathNameA(temppath, temppath, MAX_TEMP_PATH);
    if (strlen(temppath) + 12 /* ~preuuuu.TMP */ >= MAX_TEMP_PATH - 1)
    {
        ERR("temp path is too long\n");
        return 0;
    }
    unique_buf = temppath + strlen(temppath);
    *unique_buf++ = '~';
    if (prefix[0])
    {
        *unique_buf++ = prefix[0];
        if (prefix[1])
        {
            *unique_buf++ = prefix[1];
            if (prefix[2])
            {
                *unique_buf++ = prefix[2];
            }
        }
    }

    if (!unique)
    {
        int num = GetTickCount() & 0xffff;
        if (num == 0)
        {
            num = 1;
        }
        unique = num;
        do
        {
            HANDLE handle;
            sprintf(unique_buf, "%04X.TMP", unique);

            handle = CreateFileA(temppath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
            if (handle != INVALID_HANDLE_VALUE)
            {
                CloseHandle(handle);
                break;
            }
            if (GetLastError() == ERROR_ACCESS_DENIED)
            {
                ERR("%s ERROR_ACCESS_DENIED\n", temppath);
            }
            if (GetLastError() != ERROR_FILE_EXISTS &&
                GetLastError() != ERROR_SHARING_VIOLATION)
                break;
            if (!(++unique & 0xffff)) unique = 1;
        } while (unique != num);
    }
    else
    {
        sprintf(unique_buf, "%04X.TMP", unique);
    }
    if (strlen(temppath) + 1 > MAX_TEMP_PATH)
    {
        ERR("temp path is too long\n");
        return 0;
    }
    else
    {
        strcpy(buffer, temppath);
    }

    return unique;
}


typedef const char* (*ini_redirect_str_func)();
typedef int(*ini_redirect_int_func)();
struct ini_redirect_data
{
    const char *file;
    const char *section;
    const char *entry;
    const char *value;
    ini_redirect_str_func get_str;
    ini_redirect_int_func get_int;
    BOOL overwrite_section;
};
int system_ini_keyboard_type()
{
    return GetKeyboardType(0);
}
int system_ini_keyboard_subtype()
{
    return GetKeyboardType(1);
}
const char *system_init_boot_description_language_dll()
{
    LANGID lang = GetUserDefaultUILanguage();
    static char buf[256];
    if (GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SNATIVELANGNAME, buf, ARRAY_SIZE(buf)))
    {
        return buf;
    }
    return "English (American)";
}
const char *system_init_boot_description_keyboard_typ()
{
    char buf[KL_NAMELENGTH];
    if (GetKeyboardLayoutNameA(buf))
    {
        HKEY hkey;
        HKEY hkey2;
        static char buf2[256];
        DWORD cb = ARRAY_SIZE(buf2);
        if (RegOpenKeyA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\", &hkey) == ERROR_SUCCESS)
        {
            if (RegOpenKeyA(hkey, buf, &hkey2) == ERROR_SUCCESS)
            {
                LRESULT result = RegQueryValueExA(hkey2, "Layout Text", NULL, NULL, buf2, &cb);
                RegCloseKey(hkey2);
                RegCloseKey(hkey);
                if (result == ERROR_SUCCESS)
                {
                    return buf2;
                }
            }
            else
            {
                RegCloseKey(hkey);
            }
        }
    }
    return "Enhanced 101 or 102 key US and Non US keyboards";
}
int system_init_boot_description_codepage()
{
    return GetACP();
}
/* HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\WOW */
/* HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\IniFileMapping */
/* HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\IniFileMapping */
struct ini_redirect_data ini_redirect_list[] =
{
    {"system.ini", "boot", "sound.drv", "sound.drv"},
    {"system.ini", "boot", "comm.drv", "comm.drv"},
    {"system.ini", "boot", "keyboard.drv", "keyboard.drv"},
    {"system.ini", "boot", "system.drv", "system.drv"},
    {"system.ini", "boot", "display.drv", "display.drv"},
    {"system.ini", "boot", "shell", "progman.exe"},
    {"system.ini", "boot", "mouse.drv", "mouse.drv"},
    {"system.ini", "boot", "network.drv", "network.drv"},
    {"system.ini", "boot", "language.dll", ""},
    {"system.ini", "boot", "drivers", "mmsystem.dll"},
    /* {"system.ini", "boot", "oemfonts.fon", "", NULL}, */
    /* {"system.ini", "boot", "fixedfon.fon", "", NULL}, */
    /* {"system.ini", "boot", "fonts.fon", "", NULL}, */


    {"system.ini", "keyboard", "type", NULL, NULL, system_ini_keyboard_type},
    {"system.ini", "keyboard", "subtype", NULL, NULL, system_ini_keyboard_subtype},
    {"system.ini", "keyboard", "keyboard.dll", ""},

    {"system.ini", "boot.description", "system.drv", "MS-DOS or PC-DOS System"},
    /* {"system.ini", "boot.description", "aspect", "100,96,96"}, */
    {"system.ini", "boot.description", "display.drv", "VGA"},
    {"system.ini", "boot.description", "keyboard.typ", NULL, system_init_boot_description_keyboard_typ},
    {"system.ini", "boot.description", "mouse.drv", "Microsoft, or IBM PS/2"},
    {"system.ini", "boot.description", "network.drv", "LAN Support"},
    {"system.ini", "boot.description", "language.dll", NULL, system_init_boot_description_language_dll},
    {"system.ini", "boot.description", "codepage", NULL, NULL, system_init_boot_description_codepage},
    /* {"system.ini", "boot.description", "woafont.fon", ""}, */

    /* HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI (Only available in 32-bit windows) */
    {"system.ini", "mci", "AVIVideo", "mciavi.drv"},

    {"system.ini", "mci", "Sequencer", "mciseq.drv"},
    {"system.ini", "mci", "CDAudio", "mcicda.drv"},
    {"system.ini", "mci", "WaveAudio", "mciwave.drv"},

    /* MPEGVideo! */
    {"system.ini", "mci", "MPEGVideo", "mciqtz.drv"},

    /*
    HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\MCI Extensions
    HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\MCI Extensions
    */
#if 0
    {"win.ini", "mci extensions", "aiff", "MPEGVideo"},
    {"win.ini", "mci extensions", "dat", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2t", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpa", "MPEGVideo"},
    {"win.ini", "mci extensions", "wmx", "MPEGVideo"},
    {"win.ini", "mci extensions", "Mid", "MPEGVideo"},
    {"win.ini", "mci extensions", "wmv", "MPEGVideo"},
    {"win.ini", "mci extensions", "m3u", "MPEGVideo"},
    {"win.ini", "mci extensions", "avi", "MPEGVideo"},
    {"win.ini", "mci extensions", "ivf", "MPEGVideo"},
    {"win.ini", "mci extensions", "wvx", "MPEGVideo"},
    {"win.ini", "mci extensions", "m4v", "MPEGVideo"},
    {"win.ini", "mci extensions", "mts", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp4v", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp2v", "MPEGVideo"},
    {"win.ini", "mci extensions", "adts", "MPEGVideo"},
    {"win.ini", "mci extensions", "wma", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpeg", "MPEGVideo"},
    {"win.ini", "mci extensions", "tts", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpv2", "MPEGVideo"},
    {"win.ini", "mci extensions", "au", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gpp", "MPEGVideo"},
    {"win.ini", "mci extensions", "m4a", "MPEGVideo"},
    {"win.ini", "mci extensions", "wax", "MPEGVideo"},
    {"win.ini", "mci extensions", "aif", "MPEGVideo"},
    {"win.ini", "mci extensions", "asx", "MPEGVideo"},
    {"win.ini", "mci extensions", "Wav", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2ts", "MPEGVideo"},
    {"win.ini", "mci extensions", "mov", "MPEGVideo"},
    {"win.ini", "mci extensions", "wpl", "MPEGVideo"},
    {"win.ini", "mci extensions", "aac", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gp2", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp4", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp3", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp2", "MPEGVideo"},
    {"win.ini", "mci extensions", "wm", "MPEGVideo"},
    {"win.ini", "mci extensions", "adt", "MPEGVideo"},
    {"win.ini", "mci extensions", "cda", "MPEGVideo"},
    {"win.ini", "mci extensions", "3g2", "MPEGVideo"},
    {"win.ini", "mci extensions", "asf", "MPEGVideo"},
    {"win.ini", "mci extensions", "mod", "MPEGVideo"},
    {"win.ini", "mci extensions", "m1v", "MPEGVideo"},
    {"win.ini", "mci extensions", "ts", "MPEGVideo"},
    {"win.ini", "mci extensions", "rmi", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpg", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gp", "MPEGVideo"},
    {"win.ini", "mci extensions", "aifc", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpe", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2v", "MPEGVideo"},
    {"win.ini", "mci extensions", "snd", "MPEGVideo"},
#else
    {"win.ini", "mci extensions", "Mid", "Sequencer"},
    {"win.ini", "mci extensions", "Wav", "WaveAudio"},
    {"win.ini", "mci extensions", "avi", "avivideo"},
    {"win.ini", "mci extensions", "cda", "CDAudio"},

    {"win.ini", "mci extensions", "rmi", "Sequencer"},
    {"win.ini", "mci extensions", "wma", "MPEGVideo"},
    {"win.ini", "mci extensions", "m4v", "MPEGVideo"},
    {"win.ini", "mci extensions", "m4a", "MPEGVideo"},
    {"win.ini", "mci extensions", "aac", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp4", "MPEGVideo"},
    {"win.ini", "mci extensions", "mov", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp3", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpg", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp2", "MPEGVideo"},
    {"win.ini", "mci extensions", "dat", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2t", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpa", "MPEGVideo"},
    {"win.ini", "mci extensions", "wmx", "MPEGVideo"},
    {"win.ini", "mci extensions", "wmv", "MPEGVideo"},
    {"win.ini", "mci extensions", "m3u", "MPEGVideo"},
    {"win.ini", "mci extensions", "ivf", "MPEGVideo"},
    {"win.ini", "mci extensions", "mts", "MPEGVideo"},
    {"win.ini", "mci extensions", "au", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpe", "MPEGVideo"},
    {"win.ini", "mci extensions", "mod", "MPEGVideo"},
    {"win.ini", "mci extensions", "aif", "MPEGVideo"},
    /* When there are many extensions, MPLAYER causes buffer overflow. */
    /*
    {"win.ini", "mci extensions", "wax", "MPEGVideo"},
    {"win.ini", "mci extensions", "asx", "MPEGVideo"},
    {"win.ini", "mci extensions", "wpl", "MPEGVideo"},
    {"win.ini", "mci extensions", "wm", "MPEGVideo"},
    {"win.ini", "mci extensions", "adt", "MPEGVideo"},
    {"win.ini", "mci extensions", "3g2", "MPEGVideo"},
    {"win.ini", "mci extensions", "asf", "MPEGVideo"},
    {"win.ini", "mci extensions", "m1v", "MPEGVideo"},
    {"win.ini", "mci extensions", "tts", "MPEGVideo"},
    {"win.ini", "mci extensions", "ts", "MPEGVideo"},
    {"win.ini", "mci extensions", "wvx", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gp", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2v", "MPEGVideo"},
    {"win.ini", "mci extensions", "snd", "MPEGVideo"},

    {"win.ini", "mci extensions", "mpeg", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp4v", "MPEGVideo"},
    {"win.ini", "mci extensions", "mp2v", "MPEGVideo"},
    {"win.ini", "mci extensions", "adts", "MPEGVideo"},
    {"win.ini", "mci extensions", "aifc", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gp2", "MPEGVideo"},
    {"win.ini", "mci extensions", "m2ts", "MPEGVideo"},
    {"win.ini", "mci extensions", "aiff", "MPEGVideo"},
    {"win.ini", "mci extensions", "mpv2", "MPEGVideo"},
    {"win.ini", "mci extensions", "3gpp", "MPEGVideo"},
    */
#endif
    /* overwrite section */
    {"win.ini", "mci extensions", NULL, NULL, NULL, NULL, TRUE},
};

/***********************************************************************
 *           GetPrivateProfileInt   (KERNEL.127)
 */
UINT16 WINAPI GetPrivateProfileInt16( LPCSTR section, LPCSTR entry,
                                      INT16 def_val, LPCSTR filename )
{
    /* we used to have some elaborate return value limitation (<= -32768 etc.)
     * here, but Win98SE doesn't care about this at all, so I deleted it.
     * AFAIR versions prior to Win9x had these limits, though. */
    char ini[MAX_PATH];

    LPCSTR filename_file = PathFindFileNameA(filename);
    for (int i = 0; i < ARRAY_SIZE(ini_redirect_list); i++)
    {
        if (!ini_redirect_list[i].entry)
        {
            continue;
        }
        if (!stricmp(ini_redirect_list[i].file, filename_file) && !stricmp(section, ini_redirect_list[i].section) && !stricmp(entry, ini_redirect_list[i].entry))
        {
            if (ini_redirect_list[i].get_int)
                return ini_redirect_list[i].get_int();
        }
    }
    RedirectPrivateProfileStringWindowsDir(filename,ini);
    return (INT16)GetPrivateProfileIntA(section,entry,def_val,ini);
}


char windowsPath[MAX_PATH];
const char *GetRedirectWindowsDir()
{
    if (*windowsPath != '\0')
        return windowsPath;
#ifdef ENABLEREDIRECTSYSTEMDIR
    GetModuleFileNameA(GetModuleHandleA(NULL), windowsPath, MAX_PATH);
    PathRemoveFileSpecA(windowsPath);
    char ini[MAX_PATH];
    krnl386_get_config_string("otvdm", "WINDIR", "WINDOWS", ini, sizeof(ini));
    PathCombineA(windowsPath, windowsPath, ini);
    GetShortPathNameA(windowsPath, windowsPath, sizeof(windowsPath));
    return windowsPath;
#else
    GetWindowsDirectoryA(windowsPath, sizeof(windowsPath));
    GetShortPathNameA(windowsPath, windowsPath, sizeof(windowsPath));
    return windowsPath;
#endif
}

static void RedirectPrivateProfileStringWindowsDir(LPCSTR filename, LPCSTR output)
{
    if (!filename)
        filename = "win.ini";
    if (PathIsFileSpecA(filename))
    {
        PathCombineA(output, GetRedirectWindowsDir(), filename);
    }
    else
    {
        strcpy(output, filename);
    }
}
static BOOL append_ini_section(LPSTR data, SIZE_T *buf_pos, SIZE_T len, LPCSTR entry)
{
    SIZE_T ent_len = strlen(entry) + 1;
    memcpy(data + *buf_pos, entry, min(ent_len, len - *buf_pos - 2));
    *buf_pos += min(ent_len, len - *buf_pos - 2);
    data[*buf_pos] = '\0';
    data[*buf_pos + 1] = '\0';
    return TRUE;
}
static int construct_redirected_ini_section(LPCSTR section, LPSTR data, UINT16 size, LPCSTR filename)
{
    LPCSTR filename_file = PathFindFileNameA(filename);
    SIZE_T buf_pos = 0;
    if (size == 0)
    {
        return 0;
    }
    if (size == 1)
    {
        data[0] = '\0';
        return 0;
    }
    data[0] = '\0';
    data[1] = '\0';
    if (size == 2)
    {
        return 0;
    }
    for (int i = 0; i < ARRAY_SIZE(ini_redirect_list); i++)
    {
        if (!ini_redirect_list[i].entry)
        {
            continue;
        }
        if (!stricmp(ini_redirect_list[i].file, filename_file) && !stricmp(section, ini_redirect_list[i].section))
        {
            if (!append_ini_section(data, &buf_pos, size, ini_redirect_list[i].entry))
            {
                break;
            }
        }
    }
    return buf_pos;
}
/***********************************************************************
 *           GetPrivateProfileString   (KERNEL.128)
 */
INT16 WINAPI GetPrivateProfileString16( LPCSTR section, LPCSTR entry,
                                        LPCSTR def_val, LPSTR buffer,
                                        UINT16 len, LPCSTR filename )
{
    char filenamebuf[MAX_PATH];
    BOOL overwrite_section = FALSE;
    TRACE("%s %s %s\n", filename, section, entry);
    if (!section || !filename)
    {
        if (buffer && len) buffer[0] = 0;
        return 0;
    }
    LPCSTR filename_file = PathFindFileNameA(filename);
    if (entry)
    {
        for (int i = 0; i < ARRAY_SIZE(ini_redirect_list); i++)
        {
            if (!ini_redirect_list[i].entry)
            {
                continue;
            }
            if (!stricmp(ini_redirect_list[i].file, filename_file) && !stricmp(section, ini_redirect_list[i].section) && !stricmp(entry, ini_redirect_list[i].entry))
            {
                LPCSTR val = ini_redirect_list[i].value;
                SIZE_T size;
                if (!val && ini_redirect_list[i].get_str)
                {
                    val = ini_redirect_list[i].get_str();
                }
                if (!val)
                {
                    break;
                }
                if (len == 0)
                {
                    return 0;
                }
                else if (len == 1)
                {
                    buffer[0] = '\0';
                    return 0;
                }
                size = min(strlen(val), len - 1);
                memcpy(buffer, val, size);
                buffer[size] = '\0';
                return strlen(buffer);
            }
        }
    }
    else
    {
        for (int i = 0; i < ARRAY_SIZE(ini_redirect_list); i++)
        {
            if (ini_redirect_list[i].entry)
            {
                continue;
            }
            if (ini_redirect_list[i].overwrite_section && !stricmp(ini_redirect_list[i].file, filename_file) && !stricmp(section, ini_redirect_list[i].section))
            {
                overwrite_section = TRUE;
            }
        }
    }
    RedirectPrivateProfileStringWindowsDir(filename, filenamebuf);
    filename = filenamebuf;
    TRACE("(%s, %s, %s, %p, %u, %s)\n", debugstr_a(section), debugstr_a(entry),
          debugstr_a(def_val), buffer, len, debugstr_a(filename));

    /* len = 0 means unlimited buffer length (windows bug?) */
    if (!entry && len == 0)
    {
        len = 0xffff;
    }
    if (!entry)
    {
        /* We have to return the list of keys in the section but without the values
         * so we need to massage the results of GetPrivateProfileSectionA.
         */
        UINT ret, oldlen = len, size = min( len, 1024 );
        LPSTR data, src;

        if (overwrite_section)
            return construct_redirected_ini_section(section, buffer, oldlen, filename);
        for (;;)
        {
            if (!(data = HeapAlloc(GetProcessHeap(), 0, size ))) return 0;
            ret = GetPrivateProfileSectionA( section, data, size, filename );
            if (!ret)
            {
                HeapFree( GetProcessHeap(), 0, data );
                ret = construct_redirected_ini_section(section, buffer, oldlen, filename);
                if (!ret)
                    ret = GetPrivateProfileStringA(section, entry, def_val, buffer, len, filename);
                return ret;
            }
            if (ret != size - 2) break;
            /* overflow, try again */
            size *= 2;
            HeapFree( GetProcessHeap(), 0, data );
        }

        src = data;
        while (len && *src)
        {
            char *p = strchr( src, '=' );

            /* A valid entry is formed by name = value */
            if (!p)
            {
                src += strlen(src) + 1;
                continue;
            }
            if (p - src < len)
            {
                memcpy( buffer, src, p - src );
                buffer += p - src;
                *buffer++ = 0;
                len -= (p - src) + 1;
                src += strlen(src) + 1;
            }
            else  /* overflow */
            {
                memcpy( buffer, src, len );
                buffer += len;
                len = 0;
            }
        }
        HeapFree( GetProcessHeap(), 0, data );

        if (len)
        {
            *buffer = 0;
            return oldlen - len;
        }
        if (oldlen > 2)
        {
            buffer[-2] = 0;
            buffer[-1] = 0;
            return oldlen - 2;
        }
        return 0;
    }
    return GetPrivateProfileStringA( section, entry, def_val, buffer, len, filename );
}

static BOOL16 check_write_profile_error(LPCSTR filename, DWORD error)
{
    if (error == ERROR_ACCESS_DENIED)
    {
        OFSTRUCT ofstr;
        HFILE ret = OpenFile(filename, &ofstr, OF_EXIST);
        if (ret != HFILE_ERROR)
        {
            WARN("Failed to write profile data because it was redirected to the registry");
            return TRUE; // fake success, any written data can't be read anyway
        }
    }
    return FALSE;
}

/***********************************************************************
 *           WritePrivateProfileString   (KERNEL.129)
 */
BOOL16 WINAPI WritePrivateProfileString16( LPCSTR section, LPCSTR entry,
                                           LPCSTR string, LPCSTR filename )
{
    char filenamebuf[MAX_PATH];
    RedirectPrivateProfileStringWindowsDir(filename, &filenamebuf);
    filename = filenamebuf;
    BOOL ret = WritePrivateProfileStringA(section,entry,string,filename);
    if (!ret)
        return check_write_profile_error(filename, GetLastError());
    return ret;
}


/***********************************************************************
 *           GetWindowsDirectory   (KERNEL.134)
 */
UINT16 WINAPI GetWindowsDirectory16( LPSTR path, UINT16 count )
{
    const char *w = GetRedirectWindowsDir();
    UINT16 len = strlen(w);
    if (len + 1 > count)
        return strlen(w) + 1;
    strcpy(path, w);
    return len;
    //return GetWindowsDirectoryA( path, count );
}


/***********************************************************************
 *           GetSystemDirectory   (KERNEL.135)
 */
UINT16 WINAPI GetSystemDirectory16( LPSTR path, UINT16 count )
{
    static const char system16[] = "\\SYSTEM";
    char windir[MAX_PATH];
    UINT16 len;

    len = GetWindowsDirectory16(windir, sizeof(windir) - sizeof(system16) + 1) + sizeof(system16);
    if (count >= len)
    {
        lstrcpyA(path, windir);
        lstrcatA(path, system16);
        len--;  /* space for the terminating zero is not included on success */
    }
    return len;
}


/***********************************************************************
 *           GetDriveType   (KERNEL.136)
 * Get the type of a drive in Win16.
 *
 * RETURNS
 *  The type of the Drive. For a list see GetDriveTypeW from kernel32.
 *
 * NOTES
 *  Note that it returns DRIVE_REMOTE for CD-ROMs, since MSCDEX uses the
 *  remote drive API. The return value DRIVE_REMOTE for CD-ROMs has been
 *  verified on Win 3.11 and Windows 95. Some programs rely on it, so don't
 *  do any pseudo-clever changes.
 */
UINT16 WINAPI GetDriveType16( UINT16 drive ) /* [in] number (NOT letter) of drive */
{
    UINT type;
    WCHAR root[3];

    root[0] = 'A' + drive;
    root[1] = ':';
    root[2] = 0;
    type = GetDriveTypeW( root );
    if (type == DRIVE_CDROM) type = DRIVE_REMOTE;
    else if (type == DRIVE_NO_ROOT_DIR) type = DRIVE_UNKNOWN;
    return type;
}


/***********************************************************************
 *           GetProfileSectionNames   (KERNEL.142)
 */
WORD WINAPI GetProfileSectionNames16(LPSTR buffer, WORD size)

{
    return GetPrivateProfileSectionNamesA(buffer,size,"win.ini");
}


/***********************************************************************
 *           GetPrivateProfileSectionNames   (KERNEL.143)
 */
WORD WINAPI GetPrivateProfileSectionNames16( LPSTR buffer, WORD size,
                                             LPCSTR filename )
{
    return GetPrivateProfileSectionNamesA(buffer,size,filename);
}


/***********************************************************************
 *           CreateDirectory   (KERNEL.144)
 */
BOOL16 WINAPI CreateDirectory16( LPCSTR path, LPVOID dummy )
{
    return CreateDirectoryA( path, NULL );
}


/***********************************************************************
 *           RemoveDirectory   (KERNEL.145)
 */
BOOL16 WINAPI RemoveDirectory16( LPCSTR path )
{
    return RemoveDirectoryA( path );
}


/***********************************************************************
 *           DeleteFile   (KERNEL.146)
 */
BOOL16 WINAPI DeleteFile16( LPCSTR path )
{
    return DeleteFileA( path );
}


/***********************************************************************
 *           SetHandleCount   (KERNEL.199)
 */
UINT16 WINAPI SetHandleCount16( UINT16 count )
{
    return SetHandleCount( count );
}


/***********************************************************************
 *           GetShortPathName   (KERNEL.274)
 */
WORD WINAPI GetShortPathName16( LPCSTR longpath, LPSTR shortpath, WORD len )
{
    return GetShortPathNameA( longpath, shortpath, len );
}


/***********************************************************************
 *           WriteOutProfiles   (KERNEL.315)
 */
void WINAPI WriteOutProfiles16(void)
{
    WritePrivateProfileSectionW( NULL, NULL, NULL );
}


/***********************************************************************
 *           WritePrivateProfileStruct (KERNEL.406)
 */
BOOL16 WINAPI WritePrivateProfileStruct16 (LPCSTR section, LPCSTR key,
                                           LPVOID buf, UINT16 bufsize, LPCSTR filename)
{
    char filenamebuf[MAX_PATH];
    RedirectPrivateProfileStringWindowsDir(filename, &filenamebuf);
    filename = filenamebuf;
    return WritePrivateProfileStructA( section, key, buf, bufsize, filename );
}


/***********************************************************************
 *           GetPrivateProfileStruct (KERNEL.407)
 */
BOOL16 WINAPI GetPrivateProfileStruct16(LPCSTR section, LPCSTR key,
                                        LPVOID buf, UINT16 len, LPCSTR filename)
{
    char filenamebuf[MAX_PATH];
    RedirectPrivateProfileStringWindowsDir(filename, &filenamebuf);
    filename = filenamebuf;
    return GetPrivateProfileStructA( section, key, buf, len, filename );
}


/***********************************************************************
 *           GetCurrentDirectory   (KERNEL.411)
 */
UINT16 WINAPI GetCurrentDirectory16( UINT16 buflen, LPSTR buf )
{
    return GetCurrentDirectoryA( buflen, buf );
}


/***********************************************************************
 *           SetCurrentDirectory   (KERNEL.412)
 */
BOOL16 WINAPI SetCurrentDirectory16( LPCSTR dir )
{
    char fulldir[MAX_PATH];

    if (!GetFullPathNameA( dir, MAX_PATH, fulldir, NULL )) return FALSE;

    if (!SetCurrentDirectoryA( dir )) return FALSE;

    if (fulldir[0] && fulldir[1] == ':')
    {
        TDB *pTask = GlobalLock16( GetCurrentTask() );
        char env_var[4] = "=A:";

        env_var[1] = fulldir[0];
        SetEnvironmentVariableA( env_var, fulldir );

        /* update the directory in the TDB */
        if (pTask)
        {
            pTask->curdrive = 0x80 | (fulldir[0] - 'A');
            GetShortPathNameA( fulldir + 2, pTask->curdir, sizeof(pTask->curdir) );
        }
    }
    return TRUE;
}


/*************************************************************************
 *           FindFirstFile   (KERNEL.413)
 */
HANDLE16 WINAPI FindFirstFile16( LPCSTR path, WIN32_FIND_DATAA *data )
{
    HGLOBAL16 h16;
    HANDLE handle, *ptr;

    if (!(h16 = GlobalAlloc16( GMEM_MOVEABLE, sizeof(handle) ))) return INVALID_HANDLE_VALUE16;
    ptr = GlobalLock16( h16 );
    *ptr = handle = FindFirstFileA( path, data );
    GlobalUnlock16( h16 );

    if (handle == INVALID_HANDLE_VALUE)
    {
        GlobalFree16( h16 );
        h16 = INVALID_HANDLE_VALUE16;
    }
    return h16;
}


/*************************************************************************
 *           FindNextFile   (KERNEL.414)
 */
BOOL16 WINAPI FindNextFile16( HANDLE16 handle, WIN32_FIND_DATAA *data )
{
    HANDLE *ptr;
    BOOL ret = FALSE;

    if ((handle == INVALID_HANDLE_VALUE16) || !(ptr = GlobalLock16( handle )))
    {
        SetLastError( ERROR_INVALID_HANDLE );
        return ret;
    }
    ret = FindNextFileA( *ptr, data );
    GlobalUnlock16( handle );
    return ret;
}


/*************************************************************************
 *           FindClose   (KERNEL.415)
 */
BOOL16 WINAPI FindClose16( HANDLE16 handle )
{
    HANDLE *ptr;

    if ((handle == INVALID_HANDLE_VALUE16) || !(ptr = GlobalLock16( handle )))
    {
        SetLastError( ERROR_INVALID_HANDLE );
        return FALSE;
    }
    FindClose( *ptr );
    GlobalUnlock16( handle );
    GlobalFree16( handle );
    return TRUE;
}


/***********************************************************************
 *           WritePrivateProfileSection   (KERNEL.416)
 */
BOOL16 WINAPI WritePrivateProfileSection16( LPCSTR section,
                                            LPCSTR string, LPCSTR filename )
{
    return WritePrivateProfileSectionA( section, string, filename );
}


/***********************************************************************
 *           WriteProfileSection   (KERNEL.417)
 */
BOOL16 WINAPI WriteProfileSection16( LPCSTR section, LPCSTR keys_n_values)
{
    return WritePrivateProfileSection16( section, keys_n_values, "win.ini");
}


/***********************************************************************
 *           GetPrivateProfileSection   (KERNEL.418)
 */
INT16 WINAPI GetPrivateProfileSection16( LPCSTR section, LPSTR buffer,
                                         UINT16 len, LPCSTR filename )
{
    return GetPrivateProfileSectionA( section, buffer, len, filename );
}


/***********************************************************************
 *           GetProfileSection   (KERNEL.419)
 */
INT16 WINAPI GetProfileSection16( LPCSTR section, LPSTR buffer, UINT16 len )
{
    return GetPrivateProfileSection16( section, buffer, len, "win.ini" );
}


/**************************************************************************
 *           GetFileAttributes   (KERNEL.420)
 */
DWORD WINAPI GetFileAttributes16( LPCSTR name )
{
    return GetFileAttributesA( name );
}


/**************************************************************************
 *              SetFileAttributes	(KERNEL.421)
 */
BOOL16 WINAPI SetFileAttributes16( LPCSTR lpFileName, DWORD attributes )
{
    return SetFileAttributesA( lpFileName, attributes );
}


/***********************************************************************
 *           GetDiskFreeSpace   (KERNEL.422)
 */
BOOL16 WINAPI GetDiskFreeSpace16( LPCSTR root, LPDWORD cluster_sectors,
                                  LPDWORD sector_bytes, LPDWORD free_clusters,
                                  LPDWORD total_clusters )
{
    return GetDiskFreeSpaceA( root, cluster_sectors, sector_bytes,
                                free_clusters, total_clusters );
}

/***********************************************************************
 *	FileCDR (KERNEL.130)
 */
FARPROC16 WINAPI FileCDR16(FARPROC16 x)
{
    FIXME("(%p): stub\n", x);
    return (FARPROC16)TRUE;
}
