#include "config.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include "wine/debug.h"
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include <WowNT32.h>
#include "wine/winuser16.h"

WINE_DEFAULT_DEBUG_CHANNEL(win);
/*
*  ReactOS kernel
*  Copyright (C) 1998, 1999, 2000, 2001, 2002 ReactOS Team
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/*
* PROJECT:         ReactOS user32.dll
* FILE:            win32ss/user/user32/misc/winhelp.c
* PURPOSE:         WinHelp
* PROGRAMMER:      Robert Dickenson(robd@reactos.org)
* UPDATE HISTORY:
*      23-08-2002  RDD  Created from wine sources
*/


/* WinHelp internal structure */
typedef struct
{
    WORD size;
    WORD command;
    LONG data;
    LONG reserved;
    WORD ofsFilename;
    WORD ofsData;
} WINHELP, *LPWINHELP;

typedef struct {
    WORD   mkSize;
    CHAR   mkKeylist;
    CHAR   szKeyphrase[1];
} MULTIKEYHELP16, *PMULTIKEYHELP16, *LPMULTIKEYHELP16;
typedef struct {
    INT16  wStructSize;
    INT16  x;
    INT16  y;
    INT16  dx;
    INT16  dy;
    INT16  wMax;
    CHAR   rgchMember[2];
} HELPWININFO16, *PHELPWININFO16, *LPHELPWININFO16;
//16bit WINHELP.EXE
HWND16 WINAPI FindWindow16(LPCSTR className, LPCSTR title);
BOOL
WINAPI
wine_WinHelp16A(HWND16 hWnd, LPCSTR lpszHelp, UINT uCommand, DWORD_PTR dwData, BOOL *success_exec)
{
    static WORD WM_WINHELP = 0;
    HWND hDest;
    LPWINHELP lpwh;
    HGLOBAL16 hwh;
    int size, dsize, nlen;
    *success_exec = FALSE;

    if (!WM_WINHELP) {
        WM_WINHELP = RegisterWindowMessageA("WM_WINHELP");
        if (!WM_WINHELP)
            return FALSE;
    }

    hDest = HWND_32(FindWindow16("MS_WINHELP", NULL));
    if (!hDest) {
        if (uCommand == HELP_QUIT)
        {
            *success_exec = TRUE;
            return TRUE;
        }
        if (WinExec16("winhelp.exe -x", SW_SHOWNORMAL) < 32) {
            ERR("can't start winhelp.exe -x ?\n");
            return FALSE;
        }
        //sleep 100ms~2000ms...
        for (int i = 0; i < 20; i++)
        {
            hDest = HWND_32(FindWindow16("MS_WINHELP", NULL));
            if (hDest)
                break;
            Sleep(100);
        }
        if (!hDest) {
            FIXME("did not find MS_WINHELP\n");
            return FALSE;
        }
    }
    *success_exec = TRUE;
    switch (uCommand) {
    case HELP_CONTEXT:
    case HELP_SETCONTENTS:
    case HELP_CONTENTS:
    case HELP_CONTEXTPOPUP:
    case HELP_FORCEFILE:
    case HELP_HELPONHELP:
    case HELP_FINDER:
    case HELP_QUIT:
        dsize = 0;
        break;
    case HELP_KEY:
    case HELP_PARTIALKEY:
    case HELP_COMMAND:
        dsize = dwData ? strlen((LPSTR)dwData) + 1 : 0;
        break;
    case HELP_MULTIKEY:
        dsize = ((LPMULTIKEYHELP16)dwData)->mkSize;
        break;
    case HELP_SETWINPOS:
        dsize = ((LPHELPWININFO16)dwData)->wStructSize;
        break;
    default:
        FIXME("Unknown help command %d\n",uCommand);
        return FALSE;
    }
    if (lpszHelp)
        nlen = strlen(lpszHelp) + 1;
    else
        nlen = 0;
    size = sizeof(WINHELP) + nlen + dsize;
    hwh = GlobalAlloc16(0, size);
    if (hwh == NULL)
        return FALSE;
    lpwh = GlobalLock16(hwh);
    lpwh->size = size;
    lpwh->command = uCommand;
    lpwh->data = dwData;
    if (nlen) {
        strcpy(((char*)lpwh) + sizeof(WINHELP), lpszHelp);
        lpwh->ofsFilename = sizeof(WINHELP);
    }
    else {
        lpwh->ofsFilename = 0;
    }
    if (dsize) {
        memcpy(((char*)lpwh) + sizeof(WINHELP) + nlen, (LPSTR)dwData, dsize);
        lpwh->ofsData = sizeof(WINHELP) + nlen;
    }
    else {
        lpwh->ofsData = 0;
    }
    GlobalUnlock16(hwh);
    LRESULT result = SendMessageA(hDest, WM_WINHELP, (WPARAM)hWnd, (LPARAM)hwh);
    if (!result)
    {
        //failure
        //maybe winhlp32 launcher?
        GlobalFree16(hwh);
        *success_exec = FALSE;
    }
    return result;
}

/* WinHelp internal structure */
typedef struct
{
    WORD size;
    WORD command;
    LONG data;
    LONG reserved;
    WORD ofsFilename;
    WORD ofsData;
    WORD ofsPath;
} WINEHELP, *LPWINEHELP;

/* magic number for this message:
*  aide means help is French ;-)
*  SOS means ???
*/
#define WINHELP_MAGIC   0xA1DE505

/**********************************************************************
*		WinHelpA (USER32.@)
*/
BOOL WINAPI wine_WinHelp32A(HWND hWnd, LPCSTR lpHelpFile, UINT wCommand, ULONG_PTR dwData, BOOL *success_exec)
{
    COPYDATASTRUCT      cds;
    HWND                hDest;
    int                 size, dsize, nlen, plen;
    char                path[MAX_PATH];
    WINEHELP*           lpwh;
    LRESULT             ret;

    hDest = FindWindowA("MS_WINHELP", NULL);
    *success_exec = FALSE;
    if (!hDest)
    {
        if (wCommand == HELP_QUIT)
        {
            *success_exec = TRUE;
            return TRUE;
        }
        if (WinExec16("winhlp32.exe -x", SW_SHOWNORMAL) < 32)
        {
            ERR("can't start winhlp32.exe -x ?\n");
            return FALSE;
        }
        //sleep 100ms~2000ms...
        for (int i = 0; i < 20; i++)
        {
            hDest = FindWindowA("MS_WINHELP", NULL);
            if (hDest)
                break;
            Sleep(100);
        }
        if (!(hDest = FindWindowA("MS_WINHELP", NULL)))
        {
            FIXME("Did not find a MS_WINHELP Window\n");
            return FALSE;
        }
    }
    *success_exec = TRUE;
    MULTIKEYHELPA multikey32;
    HELPWININFOA info32;
    switch (wCommand)
    {
    case HELP_CONTEXT:
    case HELP_SETCONTENTS:
    case HELP_CONTENTS:
    case HELP_CONTEXTPOPUP:
    case HELP_FORCEFILE:
    case HELP_HELPONHELP:
    case HELP_FINDER:
    case HELP_QUIT:
        dsize = 0;
        break;
    case HELP_KEY:
    case HELP_PARTIALKEY:
    case HELP_COMMAND:
        dsize = dwData ? strlen((LPSTR)dwData) + 1 : 0;
        break;
    case HELP_MULTIKEY:
    {
        //this conversion is not tested.
        MESSAGE("HELP_MULTIKEY\n");
        LPMULTIKEYHELP16 multikey16 = (LPMULTIKEYHELP16)dwData;
        multikey32.mkKeylist = multikey16->mkKeylist;
        multikey32.mkSize = multikey16->mkSize - (sizeof(MULTIKEYHELPA) - sizeof(MULTIKEYHELP16));
        memcpy(&multikey32.szKeyphrase, multikey16->szKeyphrase, sizeof(MULTIKEYHELP16) - offsetof(MULTIKEYHELP16, szKeyphrase));
        dwData = &multikey32;
        dsize = ((LPMULTIKEYHELPA)dwData)->mkSize;
    }
    break;
    case HELP_SETWINPOS:
    {
        //this conversion is not tested.
        MESSAGE("HELP_SETWINPOS\n");
        LPHELPWININFO16 info16 = (LPHELPWININFO16)dwData;
        info32.dx = info16->dx;
        info32.dy = info16->dy;
        info32.x = info16->x;
        info32.y = info16->y;
        info32.wStructSize = info16->wStructSize - (sizeof(HELPWININFOA) - sizeof(HELPWININFO16));
        info32.rgchMember[0] = info16->rgchMember[0];
        info32.rgchMember[2] = info16->rgchMember[1];
        dwData = &info32;
        dsize = ((LPHELPWININFOA)dwData)->wStructSize;
    }
    break;
    default:
        FIXME("Unknown help command %d\n", wCommand);
        return FALSE;
    }
    if (lpHelpFile)
    {
        GetCurrentDirectoryA(MAX_PATH, path);
        plen = strlen(path) + 1;
        nlen = strlen(lpHelpFile) + 1;
    }
    else
    {
        plen = 0;
        nlen = 0;
    }
    size = sizeof(WINEHELP) + nlen + dsize + plen;

    lpwh = HeapAlloc(GetProcessHeap(), 0, size);
    if (!lpwh) return FALSE;

    cds.dwData = WINHELP_MAGIC;
    cds.cbData = size;
    cds.lpData = (void*)lpwh;

    lpwh->size = size;
    lpwh->command = wCommand;
    lpwh->data = dwData;
    if (nlen)
    {
        strcpy(((char*)lpwh) + sizeof(WINEHELP), lpHelpFile);
        lpwh->ofsFilename = sizeof(WINEHELP);
    }
    else
        lpwh->ofsFilename = 0;
    if (dsize)
    {
        memcpy(((char*)lpwh) + sizeof(WINEHELP) + nlen, (LPSTR)dwData, dsize);
        lpwh->ofsData = sizeof(WINEHELP) + nlen;
    }
    else
        lpwh->ofsData = 0;
    if (plen)
    {
        strcpy(((char*)lpwh) + sizeof(WINEHELP) + nlen + dsize, path);
        lpwh->ofsPath = sizeof(WINEHELP) + nlen + dsize;
    }
    else
        lpwh->ofsPath = 0;
    TRACE("Sending[%u]: cmd=%u data=%08x fn=%s\n",
        lpwh->size, lpwh->command, lpwh->data,
        lpwh->ofsFilename ? (LPSTR)lpwh + lpwh->ofsFilename : "");

    ret = SendMessageA(hDest, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
    HeapFree(GetProcessHeap(), 0, lpwh);
    return ret;
}

