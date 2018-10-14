#include <Windows.h>

// this only currently will get the flags for the first task
// it might be work on a per-task basis
static char modes[256];

BOOL WINAPI krnl386_get_compat_mode(const LPCSTR mode)
{
    int size = strnlen(mode, 256);
    if (size == 256)
        return FALSE;
    char lowermode[256];
    for (int i = 0; i < size; i++)
        lowermode[i] = tolower(mode[i]);
    lowermode[size] = '\0';
    return strstr(modes, lowermode) ? TRUE : FALSE;
}

void WINAPI krnl386_set_compat_path(const LPCSTR path)
{
    HKEY hkey;
    LSTATUS stat = RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers", &hkey);
    modes[0] = '\0';
    if (stat)
        return;
    int size = 256;
    int type;
    stat = RegQueryValueExA(hkey, path, 0, &type, &modes, &size);
    RegCloseKey(hkey);
    if (stat || (type != REG_SZ))
    {
        modes[0] = '\0';
        return;
    }
    for (int i = 0; i < size; i++)
        modes[i] = tolower(modes[i]);
    return;
}
