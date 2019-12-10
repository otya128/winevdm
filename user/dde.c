/*
 * Shell DDE Handling
 *
 * Copyright 2004 Robert Shearman
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

#include <stdarg.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "winuser.h"
#include "ddeml.h"
#include "shellapi.h"
#include "shobjidl.h"
#include "shlwapi.h"

/* #include "shell32_main.h" */

#include "wine/debug.h"
/* winnt.h */
#define __WINE_ALLOC_SIZE(x)
#include "wine/heap.h"
#include "wine/unicode.h"
#define ARRAY_SIZE ARRAYSIZE
#include "shlobj.h"
#pragma comment(lib, "shlwapi.lib")

static inline WCHAR *strndupW(const WCHAR *src, DWORD len)
{
    WCHAR *dest;
    if (!src) return NULL;
    dest = heap_alloc((len + 1) * sizeof(*dest));
    if (dest)
    {
        memcpy(dest, src, len * sizeof(WCHAR));
        dest[len] = '\0';
    }
    return dest;
}

WINE_DEFAULT_DEBUG_CHANNEL(shell);

/* String handles */
static HSZ hszProgmanTopic;
static HSZ hszProgmanService;
static HSZ hszAsterisk;
static HSZ hszShell;
static HSZ hszAppProperties;
static HSZ hszFolders;
static HSZ hszGroups;
/* DDE Instance ID */
static DWORD dwDDEInst;

static const char *debugstr_hsz( HSZ hsz )
{
    WCHAR buffer[256];
    if (!DdeQueryStringW( dwDDEInst, hsz, buffer, ARRAY_SIZE(buffer), CP_WINUNICODE ))
        return "<unknown>";
    return debugstr_w( buffer );
}

static inline BOOL Dde_OnConnect(HSZ hszTopic, HSZ hszService)
{
    if ((hszTopic == hszProgmanTopic) && (hszService == hszProgmanService))
        return TRUE;
    if ((hszTopic == hszProgmanTopic) && (hszService == hszAppProperties))
        return TRUE;
    if ((hszTopic == hszShell) && (hszService == hszFolders))
        return TRUE;
    if ((hszTopic == hszShell) && (hszService == hszAppProperties))
        return TRUE;
    return FALSE;
}

static inline void Dde_OnConnectConfirm(HCONV hconv, HSZ hszTopic, HSZ hszService)
{
    TRACE( "%p %s %s\n", hconv, debugstr_hsz(hszTopic), debugstr_hsz(hszService) );
}

static HSZPAIR wildarr[5];
static HDDEDATA hddewild = 0;

static inline HDDEDATA Dde_OnWildConnect(HSZ hszTopic, HSZ hszService)
{
    if (!hddewild)
    {
        wildarr[0].hszSvc = hszProgmanService;
        wildarr[0].hszTopic = hszProgmanTopic;
        wildarr[1].hszSvc = hszAppProperties;
        wildarr[1].hszTopic = hszProgmanTopic;
        wildarr[2].hszSvc = hszFolders;
        wildarr[2].hszTopic = hszShell;
        wildarr[3].hszSvc = hszAppProperties;
        wildarr[3].hszTopic = hszShell;
        wildarr[4].hszSvc = 0;
        wildarr[4].hszTopic = 0;
        hddewild = DdeCreateDataHandle(dwDDEInst, &wildarr, sizeof(wildarr), 0, 0, 0, 0);
    }
    if (hszTopic || hszService)
        return (HDDEDATA)FALSE;

    return hddewild;
}

/* Returned string must be freed by caller */
static WCHAR *get_programs_path(const WCHAR *name)
{
    static const WCHAR slashW[] = {'/',0};
    WCHAR *path;
    WCHAR programs[MAX_PATH];
    WCHAR clnname[MAX_PATH];
    int len;

    wcsncpy(clnname, name, MAX_PATH);
    PathCleanupSpec(NULL, clnname);
    SHGetFolderPathW(NULL, CSIDL_PROGRAMS, NULL, 0, &programs);

    len = lstrlenW(programs) + 1 + lstrlenW(name);
    path = heap_alloc((len + 1) * sizeof(*path));
    lstrcpyW(path, programs);
    lstrcatW(path, slashW);
    lstrcatW(path, clnname);

    return path;
}

static inline HDDEDATA Dde_OnRequest(UINT uFmt, HCONV hconv, HSZ hszTopic,
                                     HSZ hszItem)
{
    if (hszTopic == hszProgmanTopic && (hszItem == hszGroups || hszItem == hszProgmanService) && uFmt == CF_TEXT)
    {
        static const WCHAR asteriskW[] = {'*',0};
        static const WCHAR newlineW[] = {'\r','\n',0};
        static const WCHAR dotW[] = {'.',0};
        static const WCHAR dotdotW[] = {'.','.',0};
        static const WCHAR placeholdW[] = {'x',0};
        WCHAR *programs;
        WIN32_FIND_DATAW finddata;
        HANDLE hfind;
        int len;
        WCHAR *groups_data = heap_alloc(sizeof(WCHAR));
        char *groups_dataA;
        HDDEDATA ret;

        groups_data[0] = 0;
        programs = get_programs_path(placeholdW); // PathCleanupSpec will remove the * so use a placeholder
        len = wcslen(programs);
        programs[len - 1] = '*';
        hfind = FindFirstFileW(programs, &finddata);
        if (hfind != INVALID_HANDLE_VALUE)
        {
            len = 1;
            do
            {
                if ((finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
                    lstrcmpW(finddata.cFileName, dotW) && lstrcmpW(finddata.cFileName, dotdotW))
                {
                    len += lstrlenW(finddata.cFileName) + 2;
                    groups_data = heap_realloc(groups_data, len * sizeof(WCHAR));
                    lstrcatW(groups_data, finddata.cFileName);
                    lstrcatW(groups_data, newlineW);
                }
            } while (FindNextFileW(hfind, &finddata));
            FindClose(hfind);
        }

        len = WideCharToMultiByte(CP_ACP, 0, groups_data, -1, NULL, 0, NULL, NULL);
        groups_dataA = heap_alloc(len * sizeof(WCHAR));
        WideCharToMultiByte(CP_ACP, 0, groups_data, -1, groups_dataA, len, NULL, NULL);
        ret = DdeCreateDataHandle(dwDDEInst, (BYTE *)groups_dataA, len, 0, hszItem, uFmt, 0);

        heap_free(groups_dataA);
        heap_free(groups_data);
        heap_free(programs);
        return ret;
    }
    FIXME( "%u %p %s %s: stub\n", uFmt, hconv, debugstr_hsz(hszTopic), debugstr_hsz(hszItem) );
    return NULL;
}

__declspec(dllimport) HICON NE_ExtractIcon(LPCSTR lpszExeFileName, HICON * RetPtr, INT nIconIndex,
                UINT nIcons, UINT cxDesired, UINT cyDesired, UINT *pIconId, UINT flags);

static void seticon(IShellLinkW *link, WCHAR *path, int iconidx)
{
    HICON icon = 0;
    UINT id;
    ICONINFO iinfo;
    BITMAP mbmap, cbmap;
    int fd, i, count;
    int cx = GetSystemMetrics(SM_CXICON);
    int cy = GetSystemMetrics(SM_CYICON);
    char *mbits, *cbits;
    WCHAR *fileext;
    WCHAR icofile[MAX_PATH];
    char pathascii[MAX_PATH];
    BITMAPINFOHEADER bmapi = {0};
    HANDLE hicofile;
#include <pshpack1.h>
    struct {
        WORD zero;
        WORD type;
        WORD count;
        BYTE width;
        BYTE height;
        BYTE colors;
        BYTE zeroa;
        WORD planes;
        WORD bpp;
        DWORD size;
        DWORD off;
    } header = {0, 1, 1, cx, cy, 0, 0, 0, 0}; 
#include <poppack.h>
    WideCharToMultiByte(CP_ACP, 0, path, -1, pathascii, MAX_PATH, NULL, NULL);
    NE_ExtractIcon(pathascii, &icon, iconidx, 1, cx, cy, &id, 0);
    if (icon)
    {
        wcscpy(icofile, path);
        fileext = wcschr(icofile, '.');
        fileext[1] = 'i';
        fileext[2] = 'c';
        fileext[3] = 'o';

        hicofile = CreateFileW(icofile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hicofile != INVALID_HANDLE_VALUE)
        {
            GetIconInfo(icon, &iinfo);
            GetObject(iinfo.hbmMask, sizeof(BITMAP), &mbmap);
            if (iinfo.hbmColor)
            {
                GetObject(iinfo.hbmColor, sizeof(BITMAP), &cbmap);
                cbits = (char *)HeapAlloc(GetProcessHeap(), 0, cy * cbmap.bmWidthBytes);
                GetBitmapBits(iinfo.hbmColor, cy * cbmap.bmWidthBytes, cbits);
                DeleteObject(iinfo.hbmColor);
                bmapi.biBitCount = 32;
            }
            else
            {
                cbits = NULL;
                bmapi.biBitCount = 1;
                header.colors = 2;
                cbmap.bmWidthBytes = 0;
                cy *= 2;
            }
            mbits = (char *)HeapAlloc(GetProcessHeap(), 0, cy * mbmap.bmWidthBytes);
            GetBitmapBits(iinfo.hbmMask, cy * mbmap.bmWidthBytes, mbits);
            DeleteObject(iinfo.hbmMask);

            bmapi.biSize = sizeof(bmapi);
            bmapi.biWidth = cx;
            bmapi.biHeight = (cbits ? cy * 2 : cy);
            bmapi.biPlanes = 1;

            header.size = cy * (mbmap.bmWidthBytes + cbmap.bmWidthBytes) + sizeof(bmapi) + (cbits ? 0 : 8);
            header.off = sizeof(header);

            WriteFile(hicofile, &header, sizeof(header), &count, NULL);
            WriteFile(hicofile, &bmapi, sizeof(bmapi), &count, NULL);
            if (cbits)
            {
                for(i = cy - 1; i >= 0; i--)
                    WriteFile(hicofile, cbits + (i * cbmap.bmWidthBytes), cbmap.bmWidthBytes, &count, NULL);
                HeapFree(GetProcessHeap(), 0, cbits);
            }
            else
            {
                COLORREF pal[] = {0, 0xffffff};
                WriteFile(hicofile, &pal, sizeof(COLORREF) * 2, &count, NULL);
            }
            for(i = cy - 1; i >= 0; i--)
                WriteFile(hicofile, mbits + (i * mbmap.bmWidthBytes), mbmap.bmWidthBytes, &count, NULL);
            CloseHandle(hicofile);

            HeapFree(GetProcessHeap(), 0, mbits);
            path = icofile;
            iconidx = 0;
        }
        else if (GetLastError() == ERROR_FILE_EXISTS)
        {
            path = icofile;
            iconidx = 0;
        }
        DestroyIcon(icon);
    }
    IShellLinkW_SetIconLocation(link, path, iconidx);
}

static DWORD PROGMAN_OnExecute(WCHAR *command, int argc, WCHAR **argv)
{
    static const WCHAR create_groupW[] = {'C','r','e','a','t','e','G','r','o','u','p',0};
    static const WCHAR delete_groupW[] = {'D','e','l','e','t','e','G','r','o','u','p',0};
    static const WCHAR show_groupW[] = {'S','h','o','w','G','r','o','u','p',0};
    static const WCHAR add_itemW[] = {'A','d','d','I','t','e','m',0};
    static const WCHAR delete_itemW[] = {'D','e','l','e','t','e','I','t','e','m',0};
    static const WCHAR replace_itemW[] = {'R','e','p','l','a','c','e','I','t','e','m',0};
    static const WCHAR exit_progmanW[] = {'E','x','i','t','P','r','o','g','m','a','n',0};

    static const WCHAR dotexeW[] = {'.','e','x','e',0};
    static const WCHAR dotlnkW[] = {'.','l','n','k',0};
    static const WCHAR slashW[] = {'\\',0};

    static WCHAR *last_group;

    if (!strcmpiW(command, create_groupW))
    {
        WCHAR *path;

        if (argc < 1) return DDE_FNOTPROCESSED;

        path = get_programs_path(argv[0]);

        CreateDirectoryW(path, NULL);
        heap_free(last_group);
        last_group = path;
        ShellExecuteW(NULL, NULL, path, NULL, NULL, SW_SHOWNORMAL);
    }
    else if (!strcmpiW(command, delete_groupW))
    {
        WCHAR *path, *path2;
        SHFILEOPSTRUCTW shfos = {0};
        int ret;

        if (argc < 1) return DDE_FNOTPROCESSED;

        path = get_programs_path(argv[0]);

        path2 = heap_alloc((strlenW(path) + 2) * sizeof(*path));
        strcpyW(path2, path);
        path2[strlenW(path) + 1] = 0;

        shfos.wFunc = FO_DELETE;
        shfos.pFrom = path2;
        shfos.fFlags = FOF_NOCONFIRMATION;

        ret = SHFileOperationW(&shfos);

        heap_free(path2);
        heap_free(path);

        if (ret || shfos.fAnyOperationsAborted) return DDE_FNOTPROCESSED;
    }
    else if (!strcmpiW(command, show_groupW))
    {
        WCHAR *path;

        /* Win32 requires the second parameter to be present but seems to
         * ignore its actual value. */
        if (argc < 2) return DDE_FNOTPROCESSED;

        path = get_programs_path(argv[0]);

        ShellExecuteW(NULL, NULL, path, NULL, NULL, SW_SHOWNORMAL);

        heap_free(last_group);
        last_group = path;
    }
    else if (!strcmpiW(command, add_itemW))
    {
        WCHAR *path, *name;
        DWORD len;
        IShellLinkW *link;
        IPersistFile *file;
        HRESULT hres;
        int cmd_argc;
        WCHAR **cmd_argv;
        WCHAR *prg_name, *dirend;

        if (argc < 1) return DDE_FNOTPROCESSED;

        hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                                &IID_IShellLinkW, (void **)&link);
        if (FAILED(hres)) return DDE_FNOTPROCESSED;

        /* windows 3.1 progman: command line */
        /* explorer DDE: program path */
        cmd_argv = CommandLineToArgvW(argv[0], &cmd_argc);
        if (!cmd_argv || !cmd_argc)
            return DDE_FNOTPROCESSED;
        prg_name = cmd_argv[0];
        len = SearchPathW(NULL, prg_name, dotexeW, 0, NULL, NULL);
        if (len == 0)
        {
            LocalFree(cmd_argv);
            IShellLinkW_Release(link);
            return DDE_FNOTPROCESSED;
        }
        path = heap_alloc(len * sizeof(WCHAR));
        SearchPathW(NULL, prg_name, dotexeW, len, path, &dirend);
        IShellLinkW_SetPath(link, path);
        if (cmd_argc > 1)
        {
            WCHAR *args = strstrW(argv[0], " ");/* FIXME? */
            if (args)
                IShellLinkW_SetArguments(link, args + 1);
        }

        if (argc >= 2) IShellLinkW_SetDescription(link, argv[1]);
        if (argc >= 4)
            seticon(link, *argv[2] ? argv[2] : path, atoiW(argv[3]));
        else
            seticon(link, path, 0);
        if (argc >= 7)
            IShellLinkW_SetWorkingDirectory(link, argv[6]);
        else
        {
            *dirend = 0;
            IShellLinkW_SetWorkingDirectory(link, path);
        }
        if (argc >= 8) IShellLinkW_SetHotkey(link, atoiW(argv[7]));
        if (argc >= 9)
        {
            if (atoiW(argv[8]) == 0) IShellLinkW_SetShowCmd(link, SW_SHOWMINNOACTIVE);
            else if (atoiW(argv[8]) == 1) IShellLinkW_SetShowCmd(link, SW_SHOWNORMAL);
        }
        heap_free(path);

        hres = IShellLinkW_QueryInterface(link, &IID_IPersistFile, (void **)&file);
        if (FAILED(hres))
        {
            LocalFree(cmd_argv);
            IShellLinkW_Release(link);
            return DDE_FNOTPROCESSED;
        }
        if (argc >= 2)
        {
            name = heap_alloc((strlenW(last_group) + 1 + strlenW(argv[1]) + 5) * sizeof(*name));
            lstrcpyW(name, last_group);
            lstrcatW(name, slashW);
            lstrcatW(name, argv[1]);
            lstrcatW(name, dotlnkW);
        }
        else
        {
            const WCHAR *filename = PathFindFileNameW(prg_name);
            int len = PathFindExtensionW(filename) - filename;
            name = heap_alloc((strlenW(last_group) + 1 + len + 5) * sizeof(*name));
            lstrcpyW(name, last_group);
            lstrcatW(name, slashW);
            lstrcpynW(name+strlenW(name), filename, len + 1);
            lstrcatW(name, dotlnkW);
        }
        hres = IPersistFile_Save(file, name, TRUE);

        heap_free(name);
        IPersistFile_Release(file);
        LocalFree(cmd_argv);
        IShellLinkW_Release(link);

        if (FAILED(hres)) return DDE_FNOTPROCESSED;
    }
    else if (!strcmpiW(command, delete_itemW) || !strcmpiW(command, replace_itemW))
    {
        WCHAR *name;
        BOOL ret;

        if (argc < 1) return DDE_FNOTPROCESSED;

        name = heap_alloc((strlenW(last_group) + 1 + strlenW(argv[0]) + 5) * sizeof(*name));
        lstrcpyW(name, last_group);
        lstrcatW(name, slashW);
        lstrcatW(name, argv[0]);
        lstrcatW(name, dotlnkW);

        ret = DeleteFileW(name);

        heap_free(name);

        if (!ret) return DDE_FNOTPROCESSED;
    }
    else if (!strcmpiW(command, exit_progmanW))
    {
        /* do nothing */
    }
    else
    {
        FIXME("unhandled command %s\n", debugstr_w(command));
        return DDE_FNOTPROCESSED;
    }
    return DDE_FACK;
}

static DWORD parse_dde_command(HSZ hszTopic, WCHAR *command)
{
    static const WCHAR opcode_end[] = {' ',',','(',')','[',']','"',0};
    static const WCHAR param_end[] = {',','(',')','[',']',0};

    WCHAR *original = command;
    WCHAR *opcode = NULL, **argv = NULL, *p;
    int argc = 0, i;
    DWORD ret = DDE_FACK;

    while (*command == ' ') command++;

    if (*command != '[') goto error;
    while (*command == '[')
    {
        argc = 0;
        argv = heap_alloc(sizeof(*argv));

        command++;
        while (*command == ' ') command++;
        if (!(p = strpbrkW(command, opcode_end))) goto error;

        opcode = strndupW(command, p - command);

        command = p;
        while (*command == ' ') command++;
        if (*command == '(')
        {
            command++;

            while (*command != ')')
            {
                while (*command == ' ') command++;
                if (*command != ',')
                {
                    if (*command == '"')
                    {
                        command++;
                        if (!(p = strchrW(command, '"'))) goto error;
                    }
                    else if (*command == ')')
                        break;
                    else
                    {
                        if (!(p = strpbrkW(command, param_end))) goto error;
                        while (p[-1] == ' ') p--;
                    }
                }
                else
                    p = command;

                argc++;
                argv = heap_realloc(argv, argc * sizeof(*argv));
                argv[argc-1] = strndupW(command, p - command);

                command = p;
                if (*command == '"') command++;
                while (*command == ' ') command++;
                if (*command == ',') command++;
                else if (*command != ')') goto error;
            }
            command++;

            while (*command == ' ') command++;
        }

        if (*command != ']') goto error;
        command++;
        while (*command == ' ') command++;

        if (hszTopic == hszProgmanTopic)
            ret = PROGMAN_OnExecute(opcode, argc, argv);
        else
        {
            FIXME("unhandled topic %s, command %s\n", debugstr_hsz(hszTopic), debugstr_w(opcode));
            ret = DDE_FNOTPROCESSED;
        }

        heap_free(opcode);
        for (i = 0; i < argc; i++) heap_free(argv[i]);
        heap_free(argv);

        if (ret == DDE_FNOTPROCESSED) break;
    }

    return ret;

error:
    ERR("failed to parse command %s\n", debugstr_w(original));
    heap_free(opcode);
    for (i = 0; i < argc; i++) heap_free(argv[i]);
    heap_free(argv);
    return DDE_FNOTPROCESSED;
}

static DWORD Dde_OnExecute(HCONV hconv, HSZ hszTopic, HDDEDATA hdata)
{
    WCHAR *command;
    DWORD len;
    DWORD ret;

    len = DdeGetData(hdata, NULL, 0, 0);
    if (!len) return DDE_FNOTPROCESSED;
    command = heap_alloc(len);
    DdeGetData(hdata, (BYTE *)command, len, 0);

    TRACE("conv=%p topic=%s data=%s\n", hconv, debugstr_hsz(hszTopic), debugstr_w(command));

    ret = parse_dde_command(hszTopic, command);

    heap_free(command);
    return ret;
}

static inline void Dde_OnDisconnect(HCONV hconv)
{
    TRACE( "%p\n", hconv );
}

static HDDEDATA CALLBACK DdeCallback(      
    UINT uType,
    UINT uFmt,
    HCONV hconv,
    HSZ hsz1,
    HSZ hsz2,
    HDDEDATA hdata,
    ULONG_PTR dwData1,
    ULONG_PTR dwData2)
{
    switch (uType)
    {
    case XTYP_CONNECT:
        return (HDDEDATA)(DWORD_PTR)Dde_OnConnect(hsz1, hsz2);
    case XTYP_CONNECT_CONFIRM:
        Dde_OnConnectConfirm(hconv, hsz1, hsz2);
        return NULL;
    case XTYP_WILDCONNECT:
        return Dde_OnWildConnect(hsz1, hsz2);
    case XTYP_REQUEST:
        return Dde_OnRequest(uFmt, hconv, hsz1, hsz2);
    case XTYP_EXECUTE:
        return (HDDEDATA)(DWORD_PTR)Dde_OnExecute(hconv, hsz1, hdata);
    case XTYP_DISCONNECT:
        Dde_OnDisconnect(hconv);
        return NULL;
    default:
        return NULL;
    }
}

LRESULT WINAPI progman16_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DDE_INITIATE:
        {
            SendMessage(wparam, WM_DDE_ACK, hwnd, lparam);
            break;
        }
        case WM_DDE_EXECUTE:
        {
            char *connect = GlobalLock(lparam);
            if (connect)
            {
                WCHAR command[256] = {0};
                MultiByteToWideChar(CP_ACP, 0, connect, -1, command, 255);
                HDDEDATA ret = parse_dde_command(hszProgmanTopic, command);
                PostMessage(wparam, WM_DDE_ACK, hwnd, ret == DDE_FACK ? 0x8000 : 0);
                GlobalUnlock(lparam);
            }
            break;
        }
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*************************************************************************
 * ShellDDEInit (SHELL32.@)
 *
 * Registers the Shell DDE services with the system so that applications
 * can use them.
 *
 * PARAMS
 *  bInit [I] TRUE to initialize the services, FALSE to uninitialize.
 *
 * RETURNS
 *  Nothing.
 */
void WINAPI ShellDDEInit(BOOL bInit)
{
    static const WCHAR wszProgman[] = {'P','r','o','g','m','a','n','1','6',0};
    static HWND progman16_hwnd;
    static HINSTANCE mod_instance;
    TRACE("bInit = %s\n", bInit ? "TRUE" : "FALSE");

    if (bInit)
    {
        static const WCHAR wszAsterisk[] = {'*',0};
        static const WCHAR wszShell[] = {'S','h','e','l','l',0};
        static const WCHAR wszAppProperties[] =
            {'A','p','p','P','r','o','p','e','r','t','i','e','s',0};
        static const WCHAR wszFolders[] = {'F','o','l','d','e','r','s',0};
        static const WCHAR wszGroups[] = {'G','r','o','u','p','s',0};

        DdeInitializeW(&dwDDEInst, DdeCallback, CBF_FAIL_ADVISES | CBF_FAIL_POKES, 0);

        hszProgmanTopic = DdeCreateStringHandleW(dwDDEInst, wszProgman, CP_WINUNICODE);
        hszProgmanService = DdeCreateStringHandleW(dwDDEInst, wszProgman, CP_WINUNICODE);
        hszAsterisk = DdeCreateStringHandleW(dwDDEInst, wszAsterisk, CP_WINUNICODE);
        hszShell = DdeCreateStringHandleW(dwDDEInst, wszShell, CP_WINUNICODE);
        hszAppProperties = DdeCreateStringHandleW(dwDDEInst, wszAppProperties, CP_WINUNICODE);
        hszFolders = DdeCreateStringHandleW(dwDDEInst, wszFolders, CP_WINUNICODE);
        hszGroups = DdeCreateStringHandleW(dwDDEInst, wszGroups, CP_WINUNICODE);

        DdeNameService(dwDDEInst, hszFolders, 0, DNS_REGISTER);
        DdeNameService(dwDDEInst, hszProgmanService, 0, DNS_REGISTER);
        /* DdeNameService(dwDDEInst, hszShell, 0, DNS_REGISTER); */
        WNDCLASSW class = {0};
        class.lpfnWndProc = &progman16_wndproc;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                        (LPCSTR)&progman16_wndproc, &mod_instance);
        class.hInstance = mod_instance;
        class.lpszClassName = &wszProgman;
        RegisterClassW(&class);
        progman16_hwnd = CreateWindowW(wszProgman, wszProgman, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, mod_instance, NULL);
        CoInitialize(NULL);
    }
    else
    {
        /* unregister all services */
        DdeNameService(dwDDEInst, 0, 0, DNS_UNREGISTER);

        DdeFreeStringHandle(dwDDEInst, hszFolders);
        DdeFreeStringHandle(dwDDEInst, hszAppProperties);
        DdeFreeStringHandle(dwDDEInst, hszShell);
        DdeFreeStringHandle(dwDDEInst, hszAsterisk);
        DdeFreeStringHandle(dwDDEInst, hszProgmanService);
        DdeFreeStringHandle(dwDDEInst, hszProgmanTopic);

        DdeUninitialize(dwDDEInst);
        DestroyWindow(progman16_hwnd);
        UnregisterClassW(wszProgman, mod_instance);
    }
}
