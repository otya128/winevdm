// vdmwrapper.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


int main(int argc, const char **argv)
{/*
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    return 0;*/
    if (argc != 4)
    {
        puts("VDMWRAP.EXE <VDM PATH> <WIN16EXEFILE> <OUTPUT FILE>");
        return 1;
    }
    const char *vdmpath = argv[1];
    const char *win16exefile = argv[2];
    const char *output = argv[3];
    if (!CopyFileA("vdmwrapperexe.exe", output, !TRUE))
    {
        HLOCAL msgbuf;
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgbuf, 0, NULL);
        printf("%s\n", msgbuf);
        LocalFree(msgbuf);
        return 1;
    }
    FILE *file = fopen(output, "rb+");
    if (!file)
    {
        puts("Could not open \"vdmwrapperexe.exe\".\n");
        return 1;
    }
    IMAGE_DOS_HEADER MZheader;
    fread(&MZheader, 1, sizeof(IMAGE_DOS_HEADER), file);
    fseek(file, MZheader.e_lfanew, 0);
    IMAGE_NT_HEADERS ntHeader;
    fread(&ntHeader, 1, sizeof(IMAGE_NT_HEADERS), file);
    fseek(file, ntHeader.OptionalHeader.SizeOfImage, 0);
    fprintf(file, "%s%c", vdmpath, '\0');
    fprintf(file, "%s%c", win16exefile, '\0');
    fclose(file);
    return 0;
}
