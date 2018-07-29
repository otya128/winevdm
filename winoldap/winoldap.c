

#include "config.h"


#include "windef.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/list.h"
#include "wine/debug.h"
#include "shellapi.h"
#include "stdio.h"
#include "stdlib.h"

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

//WINE_DEFAULT_DEBUG_CHANNEL(module);
/***********************************************************************
*           wait_input_idle
*
* user32.WaitForInputIdle releases the win16 lock, so here is a replacement.
*/
static DWORD wait_input_idle(HANDLE process, DWORD timeout)
{
    DWORD ret = 0;
    HANDLE handles[2];

    handles[0] = process;
    /*SERVER_START_REQ(get_process_idle_event)
    {
        req->handle = wine_server_obj_handle(process);
        if (!(ret = wine_server_call_err(req))) handles[1] = wine_server_ptr_handle(reply->event);
    }
    SERVER_END_REQ;*/
    if (ret) return WAIT_FAILED;  /* error */
    if (!handles[1]) return 0;  /* no event to wait on */

    return WaitForMultipleObjects(2, handles, FALSE, timeout);
}

LPWSTR convert_wstr(LPSTR str)
{
    size_t size = 100 * sizeof(WCHAR);
    LPWSTR strw = malloc(size);
    size = MultiByteToWideChar(CP_ACP, 0, str, -1, strw, size);
    SetLastError(ERROR_SUCCESS);
    if (GetLastError() == ERROR_SUCCESS && size < 100 * sizeof(WCHAR))
    {
        strw[size] = '\0';
        return strw;
    }
    free(strw);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        return NULL;

    strw = malloc(size * sizeof(WCHAR) + sizeof(WCHAR));
    SetLastError(ERROR_SUCCESS);
    size = MultiByteToWideChar(CP_ACP, 0, str, -1, strw, size);
    if (GetLastError() != ERROR_SUCCESS)
    {
        free(strw);
        return NULL;
    }
    strw[size] = '\0';
    return strw;
}
/**************************************************************************
*           WINOLDAP entry point
*/
WORD WINAPI WinMain16(HINSTANCE16 inst, HINSTANCE16 prev, LPSTR cmdline, WORD show)
{
    PROCESS_INFORMATION info;
    STARTUPINFOW startup;
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
        DWORD exit = 0;
        if (GetExitCodeProcess(handle, &exit))
            return exit;
        return 1;
    }
    memset(&startup, 0, sizeof(startup));
    startup.cb = sizeof(startup);

    LPWSTR p;
    LPWSTR exe = NULL;
    char buf[MAX_PATH];
    LPWSTR *szArglist;
    int nArgs;
    LPWSTR cmdlinew = convert_wstr(cmdline);
    szArglist = CommandLineToArgvW(cmdlinew, &nArgs);
    if (!cmdlinew || !szArglist || nArgs == 0)
    {
        LocalFree(szArglist);
        ExitThread(1);
    }
    WCHAR buffer[MAX_PATH];
    GetModuleFileNameW(GetModuleHandleW(NULL), buffer, _countof(buffer));
    exe = buffer;
    if ((p = wcsrchr(szArglist[0], L'.')) && !_wcsicmp(p, L".pif"))
    {
        if (SearchPathW(NULL, L"otvdm", L".exe", MAX_PATH, buffer, NULL))
        {
            exe = buffer;
        }
    }
    SIZE_T len = wcslen(cmdlinew) + wcslen(exe) + wcslen(L"\" \"");
    PWSTR cmdline2 = (PWSTR)malloc(sizeof(WCHAR) * (len + 1));
    swprintf(cmdline2, len, L"\"%s\" %s", exe, cmdlinew);
    if (CreateProcessW(exe, cmdline2, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &startup, &info))
    {
        /* Give 10 seconds to the app to come up */
        if (wait_input_idle(info.hProcess, 10000) == WAIT_FAILED)
            WINE_WARN("WaitForInputIdle failed: Error %d\n", GetLastError());
        ReleaseThunkLock(&count);
        WaitForSingleObject(info.hProcess, INFINITE);
        RestoreThunkLock(count);

        GetExitCodeProcess(info.hProcess, &exit_code);
        CloseHandle(info.hThread);
        CloseHandle(info.hProcess);
    }

    LocalFree(szArglist);
    free(cmdlinew);
    free(cmdline2);
    HeapFree(GetProcessHeap(), 0, cmdline);
    ExitThread(exit_code);
}

