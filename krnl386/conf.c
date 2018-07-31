#include <Windows.h>
static BOOL init;

static CHAR filename[MAX_PATH];
static CRITICAL_SECTION critical_section;
DWORD WINAPI krnl386_get_config_string(LPCSTR appname, LPCSTR keyname, LPCSTR def, LPSTR ret, DWORD size);
DWORD WINAPI krnl386_get_config_int(LPCSTR appname, LPCSTR keyname, INT def);
void init_config()
{
    init = TRUE;
    InitializeCriticalSection(&critical_section);
    EnterCriticalSection(&critical_section);
    DWORD filename_len;
    filename_len = GetModuleFileNameA(GetModuleHandleA(NULL), filename, MAX_PATH);
    if (!filename_len)
        return 0;
    CHAR ininame[] = "otvdm.ini";
    if (_countof(ininame) + filename_len >= MAX_PATH)
        return 0;
    LPSTR last = strrchr(filename, '\\');
    memcpy(last + 1, ininame, sizeof(ininame));

    LeaveCriticalSection(&critical_section);
}
DWORD WINAPI krnl386_get_config_string(LPCSTR appname, LPCSTR keyname, LPCSTR def, LPSTR ret, DWORD size)
{
    if (!init)
        init_config();
    EnterCriticalSection(&critical_section);
    DWORD result = GetPrivateProfileStringA(appname, keyname, def, ret, size, filename);
    LeaveCriticalSection(&critical_section);
    return result;
}

DWORD WINAPI krnl386_get_config_int(LPCSTR appname, LPCSTR keyname, INT def)
{
    if (!init)
        init_config();
    EnterCriticalSection(&critical_section);
    DWORD result = GetPrivateProfileIntA(appname, keyname, def, filename);
    LeaveCriticalSection(&critical_section);
    return result;
}
