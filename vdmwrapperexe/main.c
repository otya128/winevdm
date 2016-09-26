#include <Windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance,      // 現在のインスタンスのハンドル
    HINSTANCE hPrevInstance,  // 以前のインスタンスのハンドル
    LPSTR lpCmdLine,          // コマンドライン
    int nCmdShow              // 表示状態
)
{
    /*while (*lpCmdLine)
    {
        if (*lpCmdLine == "\"")
        {

        }
    }*/
    LPBYTE image = (LPBYTE)GetModuleHandleA(NULL);
    PIMAGE_DOS_HEADER MZheader = image;
    PIMAGE_NT_HEADERS ntHeader = image + MZheader->e_lfanew;
    char exefilepath[MAX_PATH];
    if (!GetModuleFileNameA(image, exefilepath, MAX_PATH))
    {
        MessageBoxA(NULL, "can't open exefile.", "", MB_OK | MB_ICONERROR);
        return 0;
    }
    HFILE exefile = CreateFileA(
        exefilepath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );
    if (exefile == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, "can't open exefile.", "", MB_OK | MB_ICONERROR);
        return 0;
    }
    SetFilePointer(exefile, ntHeader->OptionalHeader.SizeOfImage, NULL, FILE_BEGIN);
    char buf[8192];
    DWORD n;
    ReadFile(exefile, buf, sizeof(buf), &n, NULL);
    if (n == 0)
    {
        MessageBoxA(NULL, "invalid file.", "", MB_OK | MB_ICONERROR);
        return 0;
    }
    const char *vdmpath = buf;
    const char *exepath = vdmpath;
    while (*exepath++);
    HLOCAL format;
    const char *args[2] = { exepath, lpCmdLine };
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY, "\"%1\" %2", NULL, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT), &format, NULL, args);
    ShellExecuteA(NULL, "open", vdmpath, (LPCSTR)format, NULL, nCmdShow);
    LocalFree(format);
    CloseHandle(exefile);
}
