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
    char drive[5];
    char smvalue[256];
    char *value = path;
    strncpy(drive, path, 3);
    drive[3] = '\0';
    type = GetDriveTypeA(drive);
    if ((type == DRIVE_CDROM) || (type == DRIVE_REMOVABLE) || (type == DRIVE_REMOTE))
    {
        // Based on reactos layer.c
        HANDLE FindHandle;
        WIN32_FIND_DATAA FindData;
        DWORD SignMedia = 0;
        int count = 9;
        drive[3] = '*';
        drive[4] = '\0';
        FindHandle = FindFirstFileA(drive, &FindData);
        if (FindHandle != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindData.nFileSizeLow)
                {
                    SignMedia = ((SignMedia << 1) | (SignMedia >> 31)) ^ FindData.nFileSizeLow;
                    count--;
                }
            } while (FindNextFileA(FindHandle, &FindData) && count);
            FindClose(FindHandle);
        }
        snprintf(smvalue, 256, "SIGN.MEDIA=%X %s", SignMedia, path + 3);
        value = smvalue;
    }

    stat = RegQueryValueExA(hkey, value, 0, &type, &modes, &size);
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
