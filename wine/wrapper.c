#include <Windows.h>
#include <Shlwapi.h>
//PROGMAN.EXEÇ»Ç«Ç≈COMMDLGÇ™ì«Ç›çûÇ‹ÇÍÇ»Ç©Ç¡ÇΩÇÃÇ≈èCê≥
HMODULE LoadLibraryAWrapper(const char *name)
{
    HMODULE m = LoadLibraryA(name);
    if (m)
        return m;
    else
    {
        char buf[MAX_PATH];
        char buf2[MAX_PATH];
        GetModuleFileNameA(GetModuleHandleA("libwine.dll"), buf, sizeof(buf));
        *PathFindFileNameA(buf) = '\0';
        PathCombineA(buf2, buf, name);
        
        m = LoadLibraryA(buf2);
    }
    return m;
}
