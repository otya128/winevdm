/*
 * Help Viewer
 *
 * Copyright    1996 Ulrich Schmid <uschmid@mail.hh.provi.de>
 *              2002 Sylvain Petreolle <spetreolle@yahoo.fr>
 *              2002, 2008 Eric Pouech <eric.pouech@wanadoo.fr>
 *              2004 Ken Belleau <jamez@ivic.qc.ca>
 *              2008 Kirill K. Smirnov <lich@math.spbu.ru>
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

#define NONAMELESSUNION
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "commdlg.h"
#include "winhelp.h"
#include "winhelp_res.h"
#include "shellapi.h"
#include "richedit.h"
#include "richole.h"
#include "commctrl.h"
#include "psapi.h"
#include "tom.h"

#ifdef _DEBUG
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(winhelp);
#else
#define WINE_TRACE(...)
#define WINE_WARN(...)
#define WINE_FIXME(...)
#define WINE_ERR(...)
#define debugstr_a(...)
#endif

WINHELP_GLOBALS Globals = {3, NULL, TRUE, NULL, NULL, NULL, NULL, NULL, {{{NULL,NULL}},0}, NULL};

#define CTL_ID_BUTTON   0x700
#define CTL_ID_TEXT     0x701

#include "initguid.h"

DEFINE_GUID(IID_ITextDocument2, 0xc241f5e0, 0x7206, 0x11d8, 0xa2, 0xc7, 0x00, 0xa0, 0xd1, 0xd6, 0xc6, 0xb3);

static void comp_xWBTreeKey(void *p, const void *key, int leaf, void **next);
static INT_PTR CALLBACK WINHELP_TopicDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


/***********************************************************************
 *
 *           WINHELP_InitFonts
 */
static void WINHELP_InitFonts(HWND hWnd)
{
    WINHELP_WINDOW *win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
    LOGFONTW logfontlist[] = {
        {-10, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        {-12, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        {-12, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        {-12, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        {-12, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        {-10, 0, 0, 0, 700, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}},
        { -8, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 32, {'H','e','l','v',0}}};

    static HFONT fonts[ARRAY_SIZE(logfontlist)];
    static BOOL init = FALSE;

    win->fonts_len = ARRAY_SIZE(logfontlist);
    win->fonts = fonts;

    if (!init)
    {
        UINT i;

        for (i = 0; i < ARRAY_SIZE(logfontlist); i++)
	{
            fonts[i] = CreateFontIndirectW(&logfontlist[i]);
	}

        init = TRUE;
    }
}

static DWORD CALLBACK WINHELP_RtfStreamIn(DWORD_PTR cookie, BYTE* buff,
                                          LONG cb, LONG* pcb)
{
    struct RtfData*     rd = (struct RtfData*)cookie;

    if (rd->where >= rd->ptr) return 1;
    if (rd->where + cb > rd->ptr)
        cb = rd->ptr - rd->where;
    memcpy(buff, rd->where, cb);
    rd->where += cb;
    *pcb = cb;
    return 0;
}

BOOL WINHELP_SearchKey(char keyfile, LPCSTR keywords, LONG type, LPCSTR topic, LPCSTR window, WINHELP_WINDOW *currwin, HLPFILE* hlpfile)
{
    DWORD ids[20], t;
    int idn = 0;
    char *key;
    HLPFILE_PAGE *page = currwin->page;
    HLPFILE_XW *tree;
    if (type > 1) return FALSE;
    if (!(tree = HLPFILE_GetTreeData(hlpfile, keyfile))) return FALSE;
    key = strtok(keywords, ";");
    while (key)
    {
        key = HLPFILE_BPTreeSearch(tree->tree, key, comp_xWBTreeKey);
        if (key)
        {
            int count = *(SHORT *)(key + strlen(key) + 1);
            int offset = *(ULONG *)(key + strlen(key) + 3);
            for (int i = 0; i < count; i++)
            {
                DWORD ref = *(DWORD *)(tree->data + offset + 9 + (4 * i));
                if (page && (ref == page->offset)) continue;
                ids[idn++] = ref;
                if (idn == 20) break;
            }
            if (idn == 20) break;
        }
        key = strtok(NULL, ";");
    }
    if (!idn) return FALSE;
    if (type && ((type != 1) || (idn > 1)))
    {
        struct index_data idx;
        idx.hlpfile = page->file;
        idx.jump = FALSE;
        idx.offset = (ULONG)ids;
        idx.count = idn;
        t = DialogBoxParamA(NULL, MAKEINTRESOURCE(IDD_TOPIC), currwin->hMainWnd, WINHELP_TopicDlgProc, &idx);
        if (t == 0xFFFFFFFF) return FALSE;
    }
    else
        t = ids[0];
    HLPFILE_WINDOWINFO* win = window ? WINHELP_GetWindowInfo(hlpfile, window) : currwin->info;
    WINHELP_OpenHelpWindow(HLPFILE_PageByOffset, hlpfile, t, win, SW_NORMAL);
    return TRUE;
}

static void WINHELP_SetupText(HWND hTextWnd, WINHELP_WINDOW* win, ULONG relative)
{
    static const WCHAR emptyW[1];
    /* At first clear area - needed by EM_POSFROMCHAR/EM_SETSCROLLPOS */
    SendMessageW(hTextWnd, WM_SETTEXT, 0, (LPARAM)emptyW);
    SendMessageW(hTextWnd, WM_SETREDRAW, FALSE, 0);
    SendMessageW(hTextWnd, EM_SETBKGNDCOLOR, 0, (LPARAM)win->info->sr_color);
    /* set word-wrap to window size (undocumented) */
    SendMessageW(hTextWnd, EM_SETTARGETDEVICE, 0, 0);
    SendMessageW(hTextWnd, EM_EXLIMITTEXT, 0, -1);
    if (win->page)
    {
        struct RtfData  rd;
        EDITSTREAM      es;
        unsigned        cp = 0;
        POINTL          ptl;
        POINT           pt;


        if (HLPFILE_BrowsePage(win->page, &rd, win->font_scale, relative, win->info))
        {
            rd.where = rd.data;
            es.dwCookie = (DWORD_PTR)&rd;
            es.dwError = 0;
            es.pfnCallback = WINHELP_RtfStreamIn;

            SendMessageW(hTextWnd, EM_STREAMIN, SF_RTF, (LPARAM)&es);
        }
        /* FIXME: else leaking potentially the rd.first_link chain */
        HeapFree(GetProcessHeap(), 0, rd.data);
        if (rd.char_pos_rel)
        {
            WCHAR tmp[32];
            FINDTEXTW find;
            const WCHAR search[] = {'s','c','r','o','l','l','_','%','x',0};
            find.chrg.cpMin = 0;
            find.chrg.cpMax = -1;
            find.lpstrText = &tmp;
            swprintf(tmp, 32, search, relative);
            cp = SendMessageW(hTextWnd, EM_FINDTEXTW, FR_DOWN, &find);
        }
        SendMessageW(hTextWnd, EM_POSFROMCHAR, (WPARAM)&ptl, cp || (cp != -1) ? cp - 1 : 0);
        pt.x = 0; pt.y = ptl.y;
        SendMessageW(hTextWnd, EM_SETSCROLLPOS, 0, (LPARAM)&pt);
        IRichEditOle *reole;
        SendMessageW(hTextWnd, EM_GETOLEINTERFACE, 0, &reole);
        if (rd.first_hs)
        {
            REOBJECT obj = {sizeof(REOBJECT)};
            for(int i = 0; i < rd.imgcnt; i++)
            {
                HLPFILE_HOTSPOTLINK* hs = rd.first_hs;
                reole->lpVtbl->GetObject(reole, i, &obj, REO_GETOBJ_NO_INTERFACES);
                while(hs)
                {
                    if (hs->link.bHotSpot && (i == hs->imgidx))
                        hs->link.cpMin = hs->link.cpMax = obj.cp;
                    hs = hs->next;
                }
            }
        }
        if (win->page->first_var_row)
        {
            ITextDocument2 *doc;
            HRESULT result = reole->lpVtbl->QueryInterface(reole, &IID_ITextDocument2, &doc);
            if (SUCCEEDED(result))
            {
                SendMessageW(hTextWnd, EM_SETOPTIONS, ECOOP_AND, ~ECO_READONLY);
                ITextRange2 *range;
                result = doc->lpVtbl->Range(doc, 0, 0, &range);
                if (SUCCEEDED(result))
                {
                    HLPFILE_ROW *next = win->page->first_var_row;
                    BSTR rowtag = SysAllocString(L"var_wid_row");
                    RECT rect;
                    GetClientRect(hTextWnd, &rect);
                    rect.right -= GetSystemMetrics(SM_CXVSCROLL);
                    while (next->next) next = next->next;
                    while (next)
                    {
                        ITextRow *row;
                        long len = 0;
                        long start;
                        range->lpVtbl->FindText(range, rowtag, tomForward, tomMatchCase, &len);
                        if (!len) break;
                        range->lpVtbl->Move(range, tomRow, 1, &len);
                        if (!len) break;
                        range->lpVtbl->GetStart(range, &start);
                        range->lpVtbl->SetStart(range, ++start);
                        result = range->lpVtbl->GetRow(range, &row);
                        if (FAILED(result)) break;
                        next->cpos = start;
                        WORD last = 0, tot = next->width[next->cols - 1];
                        for (int i = 0; i < next->cols; i++)
                        {
                            WORD curr = ((long)(next->width[i] - last) * 100) / tot;
                            last = next->width[i];
                            next->width[i] = curr;
                            row->lpVtbl->SetCellIndex(row, i);
                            row->lpVtbl->SetCellWidth(row, (rect.right * 15 * next->width[i]) / 100);
                        }
                        row->lpVtbl->Apply(row, 1, tomCellStructureChangeOnly);
                        row->lpVtbl->Release(row);
                        next = next->prev;
                    }
                    range->lpVtbl->Release(range);
                    SysFreeString(rowtag);
                }
                doc->lpVtbl->Release(doc);
                SendMessageW(hTextWnd, EM_SETOPTIONS, ECOOP_OR, ECO_READONLY);
            }
        }
        reole->lpVtbl->Release(reole);
    }
    SendMessageW(hTextWnd, WM_SETREDRAW, TRUE, 0);
    RedrawWindow(hTextWnd, NULL, NULL, RDW_FRAME|RDW_INVALIDATE);
}

/***********************************************************************
 *
 *           WINHELP_GetOpenFileName
 */
BOOL WINHELP_GetOpenFileName(LPSTR lpszFile, int len)
{
    OPENFILENAMEA openfilename;
    CHAR szDir[MAX_PATH];
    CHAR szzFilter[2 * MAX_STRING_LEN + 100];
    LPSTR p = szzFilter;

    memset(&openfilename, 0, sizeof(OPENFILENAMEA));
    WINE_TRACE("()\n");

    LoadStringA(Globals.hInstance, STID_HELP_FILES_HLP, p, MAX_STRING_LEN);
    p += strlen(p) + 1;
    strcpy(p, "*.hlp");
    p += strlen(p) + 1;
    LoadStringA(Globals.hInstance, STID_ALL_FILES, p, MAX_STRING_LEN);
    p += strlen(p) + 1;
    strcpy(p, "*.*");
    p += strlen(p) + 1;
    *p = '\0';

    GetCurrentDirectoryA(sizeof(szDir), szDir);

    lpszFile[0]='\0';

    openfilename.lStructSize       = sizeof(openfilename);
    openfilename.hwndOwner         = (Globals.active_win ? Globals.active_win->hMainWnd : 0);
    openfilename.hInstance         = Globals.hInstance;
    openfilename.lpstrFilter       = szzFilter;
    openfilename.lpstrCustomFilter = 0;
    openfilename.nMaxCustFilter    = 0;
    openfilename.nFilterIndex      = 1;
    openfilename.lpstrFile         = lpszFile;
    openfilename.nMaxFile          = len;
    openfilename.lpstrFileTitle    = 0;
    openfilename.nMaxFileTitle     = 0;
    openfilename.lpstrInitialDir   = szDir;
    openfilename.lpstrTitle        = 0;
    openfilename.Flags             = OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_READONLY;
    openfilename.nFileOffset       = 0;
    openfilename.nFileExtension    = 0;
    openfilename.lpstrDefExt       = 0;
    openfilename.lCustData         = 0;
    openfilename.lpfnHook          = 0;
    openfilename.lpTemplateName    = 0;

    return GetOpenFileNameA(&openfilename);
}

/***********************************************************************
 *
 *           WINHELP_MessageBoxIDS_s
 */
static INT WINHELP_MessageBoxIDS_s(UINT ids_text, LPCSTR str, UINT ids_title, WORD type)
{
    CHAR text[MAX_STRING_LEN];
    CHAR caption[MAX_STRING_LEN];
    CHAR newtext[MAX_STRING_LEN + MAX_PATH];

    LoadStringA(Globals.hInstance, ids_text, text, sizeof(text));
    wsprintfA(newtext, text, str);
    LoadStringA(Globals.hInstance, ids_title, caption, sizeof(caption));

    return MessageBoxA(0, newtext, caption, type);
}

/***********************************************************************
 *
 *           WINHELP_LookupHelpFile
 */
HLPFILE* WINHELP_LookupHelpFile(LPCSTR lpszFile)
{
    HLPFILE*        hlpfile;
    char szFullName[MAX_PATH];
    char szAddPath[MAX_PATH];
    char *p;

    /*
     * NOTE: This is needed by popup windows only.
     * In other cases it's not needed but does not hurt though.
     */
    if (Globals.active_win && Globals.active_win->page && Globals.active_win->page->file)
    {
        strcpy(szAddPath, Globals.active_win->page->file->lpszPath);
        p = strrchr(szAddPath, '\\');
        if (p) *p = 0;
    }

    /*
     * FIXME: Should we swap conditions?
     */
    if (!SearchPathA(NULL, lpszFile, ".hlp", MAX_PATH, szFullName, NULL) &&
        !SearchPathA(szAddPath, lpszFile, ".hlp", MAX_PATH, szFullName, NULL))
    {
        if (WINHELP_MessageBoxIDS_s(STID_FILE_NOT_FOUND_s, lpszFile, STID_WHERROR,
                                    MB_YESNO|MB_ICONQUESTION) != IDYES)
            return NULL;
        if (!WINHELP_GetOpenFileName(szFullName, MAX_PATH))
            return NULL;
    }
    hlpfile = HLPFILE_ReadHlpFile(szFullName);
    if (!hlpfile)
        WINHELP_MessageBoxIDS_s(STID_HLPFILE_ERROR_s, lpszFile,
                                STID_WHERROR, MB_OK|MB_ICONSTOP);
    return hlpfile;
}

/******************************************************************
 *		WINHELP_GetWindowInfo
 *
 *
 */
HLPFILE_WINDOWINFO*     WINHELP_GetWindowInfo(HLPFILE* hlpfile, LPCSTR name)
{
    static      HLPFILE_WINDOWINFO      mwi;
    unsigned int     i;

    if (!name || !name[0])
        name = Globals.active_win->info->name;

    if (hlpfile)
        for (i = 0; i < hlpfile->numWindows; i++)
            if (!lstrcmpiA(hlpfile->windows[i].name, name))
                return &hlpfile->windows[i];

    if (lstrcmpiA(name, "main") != 0)
    {
        WINE_FIXME("Couldn't find window info for %s\n", debugstr_a(name));
        assert(0);
        return NULL;
    }
    if (hlpfile && !hlpfile->lpszTitle[0])
        LoadStringA(Globals.hInstance, STID_WINE_HELP, hlpfile->lpszTitle, 128);
    if (!mwi.name[0])
    {
        strcpy(mwi.type, "primary");
        strcpy(mwi.name, "main");
        if (hlpfile)
        {
            char        tmp[128];
            LoadStringA(Globals.hInstance, STID_WINE_HELP, tmp, sizeof(tmp));
            snprintf(mwi.caption, sizeof(mwi.caption), "%s %s - %s",
                     hlpfile->lpszTitle, tmp, hlpfile->lpszPath);
        }
        else
            LoadStringA(Globals.hInstance, STID_WINE_HELP, mwi.caption, sizeof(mwi.caption));
        mwi.origin.x = mwi.origin.y = mwi.size.cx = mwi.size.cy = CW_USEDEFAULT;
        mwi.style = SW_SHOW;
        mwi.win_style = WS_OVERLAPPEDWINDOW;
        mwi.sr_color = mwi.nsr_color = 0xFFFFFF;
    }
    return &mwi;
}

/******************************************************************
 *		HLPFILE_GetPopupWindowInfo
 *
 *
 */
static HLPFILE_WINDOWINFO*     WINHELP_GetPopupWindowInfo(HLPFILE* hlpfile,
                                                          WINHELP_WINDOW* parent, LPARAM mouse)
{
    static      HLPFILE_WINDOWINFO      wi;

    RECT parent_rect;
    
    wi.type[0] = wi.name[0] = wi.caption[0] = '\0';

    /* Calculate horizontal size and position of a popup window */
    GetWindowRect(parent->hMainWnd, &parent_rect);
    wi.size.cx = (parent_rect.right  - parent_rect.left) / 2;
    wi.size.cy = 10; /* need a non null value, so that borders are taken into account while computing */

    wi.origin.x = (short)LOWORD(mouse);
    wi.origin.y = (short)HIWORD(mouse);
    ClientToScreen(parent->hMainWnd, &wi.origin);
    wi.origin.x -= wi.size.cx / 2;
    wi.origin.x  = min(wi.origin.x, GetSystemMetrics(SM_CXSCREEN) - wi.size.cx);
    wi.origin.x  = max(wi.origin.x, 0);

    wi.style = SW_SHOW;
    wi.win_style = WS_POPUP | WS_BORDER;
    if (parent->page->file->has_popup_color)
        wi.sr_color = parent->page->file->popup_color;
    else
        wi.sr_color = parent->info->sr_color;
    wi.nsr_color = 0xFFFFFF;

    return &wi;
}

typedef struct
{
    WORD size;
    WORD command;
    LONG data;
    LONG reserved;
    WORD ofsFilename;
    WORD ofsData;
    WORD ofsPath;
} WINEHELP,*LPWINEHELP;

static BOOL WINHELP_HasWorkingWindow(void)
{
    if (!Globals.active_win) return FALSE;
    if (Globals.active_win->next || Globals.win_list != Globals.active_win) return TRUE;
    return Globals.active_win->page != NULL && Globals.active_win->page->file != NULL;
}

static void comp_xWBTreeKey(void *p, const void *key, int leaf, void **next)
{
    *next = (char*)p + strlen((char*)p) + (leaf?7:3);
    return stricmp(p, key);
}


/******************************************************************
 *		WINHELP_HandleCommand
 *
 *
 */
static LRESULT  WINHELP_HandleCommand(HWND hSrcWnd, WINEHELP *wh, BOOL w32)
{
    if (wh)
    {
        char*   ptr = (wh->ofsFilename) ? (LPSTR)wh + wh->ofsFilename : NULL;
        if (!w32 && wh->ofsPath)
            SetCurrentDirectoryA((LPSTR)wh + wh->ofsPath);
        else if (w32)
        {
            DWORD pid;
            HANDLE proc;
            GetWindowThreadProcessId(hSrcWnd, &pid);
            proc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (proc)
            {
                WCHAR path[MAX_PATH];
                WCHAR *sep;
                path[0] = 0;
                GetModuleFileNameExW(proc, NULL, path, MAX_PATH);
                sep = wcsrchr(path, '\\');
                if (sep)
                {
                    sep[0] = 0;
                    SetCurrentDirectoryW(path);
                }
                CloseHandle(proc);
            }
        }

        WINE_TRACE("Got[%u]: cmd=%u data=%08x fn=%s\n",
                   wh->size, wh->command, wh->data, debugstr_a(ptr));
        switch (wh->command)
        {
        case HELP_CONTEXT:
            if (ptr)
            {
                MACRO_JumpContext(ptr, "main", wh->data);
            }
            if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
            break;
        case HELP_QUIT:
            MACRO_Exit();
            break;
        case HELP_CONTENTS:
            if (ptr)
            {
                MACRO_JumpContents(ptr, "main");
            }
            if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
            break;
        case HELP_HELPONHELP:
            MACRO_HelpOn();
            if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
            break;
        /* case HELP_SETINDEX: */
        case HELP_SETCONTENTS:
            if (ptr)
            {
                MACRO_SetContents(ptr, wh->data);
            }
            break;
        case HELP_CONTEXTPOPUP:
            if (ptr)
            {
                MACRO_PopupContext(ptr, wh->data);
            }
            break;
        /* case HELP_FORCEFILE:*/
        /* case HELP_CONTEXTMENU: */
        case HELP_FINDER:
            /* in fact, should be the topic dialog box */
            WINE_FIXME("HELP_FINDER: stub\n");
            if (ptr)
            {
                MACRO_JumpHash(ptr, "main", 0);
            }
            break;
        case HELP_PARTIALKEY:
        case HELP_KEY:
        {
            HLPFILE *hlpfile = WINHELP_LookupHelpFile(ptr);
            if (!hlpfile)
            {
                if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
                break;
            }
            char *keywords = ((char *)wh + wh->ofsData);
            if (!WINHELP_SearchKey('K', keywords, 0, NULL, NULL, Globals.active_win, hlpfile))
            {
                HLPFILE_FreeHlpFile(hlpfile);
                if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
                break;
            }
            break;
        }
        case HELP_COMMAND:
        {
            // TODO: filename might be different than the open file
            if (ptr)
            {
                MACRO_ExecuteMacro(Globals.active_win, (LPSTR)wh + wh->ofsData);
            }
            break;
        }
        case HELP_MULTIKEY:
        {
            HLPFILE *hlpfile = WINHELP_LookupHelpFile(ptr);
            if (!hlpfile)
            {
                if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
                break;
            }
            MULTIKEYHELPA *mkhelp = (MULTIKEYHELPA *)((char *)wh + wh->ofsData);
            if (!WINHELP_SearchKey(mkhelp->mkKeylist, mkhelp->szKeyphrase, 0, NULL, NULL, Globals.active_win, hlpfile))
            {
                HLPFILE_FreeHlpFile(hlpfile);
                if (!WINHELP_HasWorkingWindow()) MACRO_Exit();
                break;
            }
            break;
        }

        /* case HELP_WM_HELP: */
        /* case HELP_SETPOPUP_POS: */
        /* case HELP_SETWINPOS: */
        default:
            WINE_FIXME("Unhandled command (%x) for remote winhelp control\n", wh->command);
            break;
        }
    }
    /* Always return success for now */
    return 1;
}

void            WINHELP_LayoutMainWindow(WINHELP_WINDOW* win)
{
    RECT        rect, button_box_rect;
    INT         text_top = 0;
    HWND        hButtonBoxWnd = GetDlgItem(win->hMainWnd, CTL_ID_BUTTON);
    HWND        hTextWnd = GetDlgItem(win->hMainWnd, CTL_ID_TEXT);

    GetClientRect(win->hMainWnd, &rect);

    /* Update button box and text Window */
    SetWindowPos(hButtonBoxWnd, HWND_TOP,
                 rect.left, rect.top,
                 rect.right - rect.left,
                 rect.bottom - rect.top, 0);

    if (GetWindowRect(hButtonBoxWnd, &button_box_rect))
        text_top = rect.top + button_box_rect.bottom - button_box_rect.top;

    SetWindowPos(hTextWnd, HWND_TOP,
                 rect.left, text_top,
                 rect.right - rect.left,
                 rect.bottom - text_top, 0);

}

/******************************************************************
 *		WINHELP_DeleteButtons
 *
 */
static void WINHELP_DeleteButtons(WINHELP_WINDOW* win)
{
    WINHELP_BUTTON*     b;
    WINHELP_BUTTON*     bp;

    for (b = win->first_button; b; b = bp)
    {
        DestroyWindow(b->hWnd);
        bp = b->next;
        HeapFree(GetProcessHeap(), 0, b);
    }
    win->first_button = NULL;
}

/******************************************************************
 *		WINHELP_DeleteBackSet
 *
 */
void WINHELP_DeleteBackSet(WINHELP_WINDOW* win)
{
    unsigned int i;

    for (i = 0; i < win->back.index; i++)
    {
        HLPFILE_FreeHlpFile(win->back.set[i].page->file);
        win->back.set[i].page = NULL;
    }
    win->back.index = 0;
}

/******************************************************************
 *             WINHELP_DeletePageLinks
 *
 */
static void WINHELP_DeletePageLinks(HLPFILE_PAGE* page)
{
    HLPFILE_LINK*       curr;
    HLPFILE_LINK*       next;

    for (curr = page->first_link; curr; curr = next)
    {
        next = curr->next;
        HeapFree(GetProcessHeap(), 0, curr);
    }
}

/***********************************************************************
 *
 *           WINHELP_GrabWindow
 */
WINHELP_WINDOW* WINHELP_GrabWindow(WINHELP_WINDOW* win)
{
    WINE_TRACE("Grab %p#%d++\n", win, win->ref_count);
    win->ref_count++;
    return win;
}

/***********************************************************************
 *
 *           WINHELP_ReleaseWindow
 */
BOOL WINHELP_ReleaseWindow(WINHELP_WINDOW* win)
{
    WINE_TRACE("Release %p#%d--\n", win, win->ref_count);

    if (!--win->ref_count)
    {
        WINHELP_WINDOW**    w;

        for (w = &Globals.win_list; *w; w = &(*w)->next)
        {
            if (*w == win)
            {
                *w = win->next;
                break;
            }
        }

        if (Globals.active_win == win)
        {
            Globals.active_win = Globals.win_list;
            if (Globals.win_list)
                SetActiveWindow(Globals.win_list->hMainWnd);
        }

        if (win == Globals.active_popup)
            Globals.active_popup = NULL;
        PostMessageW(win->hMainWnd, WM_CLOSE, 0, 0);
        return FALSE;
    }
    return TRUE;
}

/***********************************************************************
 *
 *           WINHELP_DeleteWindow
 */
static void WINHELP_DeleteWindow(WINHELP_WINDOW* win)
{
    WINHELP_WINDOW**    w;
    BOOL bExit;
    HWND hTextWnd;

    for (w = &Globals.win_list; *w; w = &(*w)->next)
    {
        if (*w == win)
        {
            *w = win->next;
            break;
        }
    }
    bExit = (Globals.wVersion >= 4 && !lstrcmpiA(win->info->name, "main"));

    if (Globals.active_win == win)
    {
        Globals.active_win = Globals.win_list;
        if (Globals.win_list)
            SetActiveWindow(Globals.win_list->hMainWnd);
    }

    if (win == Globals.active_popup)
        Globals.active_popup = NULL;

    hTextWnd = GetDlgItem(win->hMainWnd, CTL_ID_TEXT);
    SetWindowLongPtrA(hTextWnd, GWLP_WNDPROC, (LONG_PTR)win->origRicheditWndProc);

    WINHELP_DeleteButtons(win);

    if (win->page) WINHELP_DeletePageLinks(win->page);
    if (win->hHistoryWnd) DestroyWindow(win->hHistoryWnd);

    DeleteObject(win->hBrush);

    WINHELP_DeleteBackSet(win);

    if (win->page) HLPFILE_FreeHlpFile(win->page->file);
    HeapFree(GetProcessHeap(), 0, win);

    if (bExit) MACRO_Exit();
    if (!Globals.win_list)
        PostQuitMessage(0);
}

static char* WINHELP_GetCaption(WINHELP_WNDPAGE* wpage)
{
    if (((wpage->wininfo->flags & 4) && (wpage->wininfo->caption[0])) || !wpage->page)
        return wpage->wininfo->caption;
    return wpage->page->file->lpszTitle;
}

static void WINHELP_RememberPage(WINHELP_WINDOW* win, WINHELP_WNDPAGE* wpage)
{
    unsigned        num;

    if (!Globals.history.index || Globals.history.set[0].page != wpage->page)
    {
        num = ARRAY_SIZE(Globals.history.set);
        /* we're full, remove latest entry */
        if (Globals.history.index == num)
        {
            HLPFILE_FreeHlpFile(Globals.history.set[num - 1].page->file);
            Globals.history.index--;
        }
        memmove(&Globals.history.set[1], &Globals.history.set[0],
                Globals.history.index * sizeof(Globals.history.set[0]));
        Globals.history.set[0] = *wpage;
        Globals.history.index++;
        wpage->page->file->wRefCount++;
    }
    if (win->hHistoryWnd) InvalidateRect(win->hHistoryWnd, NULL, TRUE);

    num = ARRAY_SIZE(win->back.set);
    if (win->back.index == num)
    {
        /* we're full, remove latest entry */
        HLPFILE_FreeHlpFile(win->back.set[0].page->file);
        memmove(&win->back.set[0], &win->back.set[1],
                (num - 1) * sizeof(win->back.set[0]));
        win->back.index--;
    }
    win->back.set[win->back.index++] = *wpage;
    wpage->page->file->wRefCount++;
}

static HLPFILE_HOTSPOTLINK* WINHELP_FindHotSpot(WINHELP_WINDOW* win, LPARAM pos)
{
    int x = LOWORD(pos), y = HIWORD(pos);
    POINTL pntl = {x, y};
    HWND hwndtext = GetDlgItem(win->hMainWnd, CTL_ID_TEXT);
    LRESULT cp = SendMessageW(hwndtext, EM_CHARFROMPOS, 0, &pntl);
    HLPFILE_HOTSPOTLINK* hs = win->page->first_hs;
    while (hs)
    {
        if ((cp == hs->link.cpMin) || (cp == (hs->link.cpMin + 1)))
        {
            SendMessageW(hwndtext, EM_POSFROMCHAR, &pntl, hs->link.cpMin);
            if ((x > pntl.x + hs->x) && (x <= pntl.x + hs->x + hs->width) &&
                (y > pntl.y + hs->y) && (y <= pntl.y + hs->y + hs->height))
                return hs;
        }
        hs = hs->next;
    }
    return NULL;
}

static LRESULT CALLBACK WINHELP_RicheditWndProc(HWND hWnd, UINT msg,
                                                WPARAM wParam, LPARAM lParam)
{
    WINHELP_WINDOW *win = (WINHELP_WINDOW*) GetWindowLongPtrW(GetParent(hWnd), 0);
    LRESULT result;
    switch(msg)
    {
        case WM_SHOWWINDOW:
            if (!wParam)
            {
                DestroyWindow(hWnd);
                return 0;
            }
        default:
            return CallWindowProcA(win->origRicheditWndProc, hWnd, msg, wParam, lParam);
        case WM_SIZE:
            result = CallWindowProcA(win->origRicheditWndProc, hWnd, msg, wParam, lParam);
            if (win->page && win->page->first_var_row)
            {
                IRichEditOle *reole;
                ITextDocument2 *doc;
                SendMessageW(hWnd, EM_GETOLEINTERFACE, 0, &reole);
                HRESULT res = reole->lpVtbl->QueryInterface(reole, &IID_ITextDocument2, &doc);
                if (SUCCEEDED(res))
                {
                    ITextRange2 *range;
                    SendMessageW(hWnd, EM_SETOPTIONS, ECOOP_AND, ~ECO_READONLY);
                    res = doc->lpVtbl->Range(doc, 0, 0, &range);
                    if (SUCCEEDED(res))
                    {
                        long width = LOWORD(lParam) - GetSystemMetrics(SM_CXVSCROLL);
                        HLPFILE_ROW *next = win->page->first_var_row;
                        while (next->next) next = next->next;
                        while (next)
                        {
                            ITextRow *row;
                            range->lpVtbl->SetStart(range, next->cpos);
                            res = range->lpVtbl->GetRow(range, &row);
                            if (FAILED(res)) break;
                            for (int i = 0; i < next->cols; i++)
                            {
                                row->lpVtbl->SetCellIndex(row, i);
                                row->lpVtbl->SetCellWidth(row, (width * 15 * next->width[i]) / 100);
                            }
                            row->lpVtbl->Apply(row, 1, tomCellStructureChangeOnly);
                            row->lpVtbl->Release(row);
                            next = next->prev;
                        }
                        range->lpVtbl->Release(range);
                    }
                    doc->lpVtbl->Release(doc);
                    SendMessageW(hWnd, EM_SETOPTIONS, ECOOP_OR, ECO_READONLY);
                }
                reole->lpVtbl->Release(reole);
            }
            return result;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_LBUTTONDOWN:
        case WM_MOUSEMOVE:
        case WM_SETFOCUS:
            result = CallWindowProcA(win->origRicheditWndProc, hWnd, msg, wParam, lParam);
            HideCaret(hWnd);
            return result;
         case WM_SETCURSOR:
         {
             DWORD messagePos = GetMessagePos();
             POINT pt = {LOWORD(messagePos), HIWORD(messagePos)};
             ScreenToClient(hWnd, &pt);
             if (win->page && WINHELP_FindHotSpot(win, MAKELPARAM(pt.x, pt.y)))
             {
                 SetCursor(win->hHandCur);
                 return 0;
             }
             return CallWindowProcA(win->origRicheditWndProc, hWnd, msg, wParam, lParam);
         }
    }
}

HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_QueryInterface(IRichEditOleCallback *This, REFIID riid, LPVOID FAR * lplpObj)
{
    return S_OK;
}
ULONG STDMETHODCALLTYPE WINHELP_OLE_Callback_AddRef(IRichEditOleCallback *This)
{
    return 0;
}
ULONG STDMETHODCALLTYPE WINHELP_OLE_Callback_Release (IRichEditOleCallback *This)
{
    return 0;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_GetNewStorage (IRichEditOleCallback *This, LPSTORAGE *lplpstg)
{
    LPLOCKBYTES lpLockBytes = NULL;
    HRESULT result = CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
    if (result != S_OK)
        return result;
    result = StgCreateDocfileOnILockBytes(lpLockBytes, STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, lplpstg);
    if (result != S_OK)
        lpLockBytes->lpVtbl->Release(lpLockBytes);
    return result;

}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_GetInPlaceContext (IRichEditOleCallback *This, LPOLEINPLACEFRAME FAR * lplpFrame,
    LPOLEINPLACEUIWINDOW FAR * lplpDoc,
    LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_ShowContainerUI (IRichEditOleCallback *This, BOOL fShow)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_QueryInsertObject (IRichEditOleCallback *This, LPCLSID lpclsid, LPSTORAGE lpstg,
    LONG cp)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_DeleteObject (IRichEditOleCallback *This, LPOLEOBJECT lpoleobj)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_QueryAcceptData (IRichEditOleCallback *This, LPDATAOBJECT lpdataobj,
    CLIPFORMAT FAR * lpcfFormat, DWORD reco,
    BOOL fReally, HGLOBAL hMetaPict)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_ContextSensitiveHelp  (IRichEditOleCallback *This, BOOL fEnterMode)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_GetClipboardData (IRichEditOleCallback *This, CHARRANGE FAR * lpchrg, DWORD reco, LPDATAOBJECT FAR * lplpdataobj)
{
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_GetDragDropEffect (IRichEditOleCallback *This, BOOL fDrag, DWORD grfKeyState,
    LPDWORD pdwEffect)
{
    return S_OK;
}
HRESULT STDMETHODCALLTYPE WINHELP_OLE_Callback_GetContextMenu (IRichEditOleCallback *This, WORD seltype, LPOLEOBJECT lpoleobj,
    CHARRANGE FAR * lpchrg,
    HMENU FAR * lphmenu)
{
    return S_OK;
}
IRichEditOleCallbackVtbl callbackv =
{
    WINHELP_OLE_Callback_QueryInterface,
    WINHELP_OLE_Callback_AddRef,
    WINHELP_OLE_Callback_Release,
    WINHELP_OLE_Callback_GetNewStorage,
    WINHELP_OLE_Callback_GetInPlaceContext,
    WINHELP_OLE_Callback_ShowContainerUI,
    WINHELP_OLE_Callback_QueryInsertObject,
    WINHELP_OLE_Callback_DeleteObject,
    WINHELP_OLE_Callback_QueryAcceptData,
    WINHELP_OLE_Callback_ContextSensitiveHelp,
    WINHELP_OLE_Callback_GetClipboardData,
    WINHELP_OLE_Callback_GetDragDropEffect,
    WINHELP_OLE_Callback_GetContextMenu
};
IRichEditOleCallback callback = { &callbackv };
/***********************************************************************
 *
 *           WINHELP_CreateHelpWindow
 */
BOOL WINHELP_CreateHelpWindow(WINHELP_WNDPAGE* wpage, int nCmdShow, BOOL remember)
{
    WINHELP_WINDOW*     win = NULL;
    BOOL                bPrimary, bPopup, bReUsed = FALSE;
    HICON               hIcon;
    HWND                hTextWnd = NULL;
    WCHAR               caption[60];

    bPrimary = !lstrcmpiA(wpage->wininfo->name, "main");
    bPopup = !bPrimary && (wpage->wininfo->win_style & WS_POPUP);

    if (!bPopup)
    {
        for (win = Globals.win_list; win; win = win->next)
        {
            if (!lstrcmpiA(win->info->name, wpage->wininfo->name))
            {
                if (win->page == wpage->page && win->info == wpage->wininfo && win->offset == wpage->relative)
                {
                    /* see #22979, some hlp files have a macro (run at page opening), which
                     * jumps to the very same page
                     * Exit gracefully in that case
                     */
                    return TRUE;
                }
                WINHELP_DeleteButtons(win);
                bReUsed = TRUE;
                int codepage = wpage->page ? wpage->page->file->codepage : CP_ACP;
                MultiByteToWideChar(codepage, 0, WINHELP_GetCaption(wpage), -1, caption, 60);
                SetWindowTextW(win->hMainWnd, caption);
                if (win->info != wpage->wininfo)
                {
                    POINT   pt = {0, 0};
                    SIZE    sz = {0, 0};
                    DWORD   flags = SWP_NOSIZE | SWP_NOMOVE;

                    if (wpage->wininfo->origin.x != CW_USEDEFAULT &&
                        wpage->wininfo->origin.y != CW_USEDEFAULT)
                    {
                        pt = wpage->wininfo->origin;
                        flags &= ~SWP_NOSIZE;
                    }
                    if (wpage->wininfo->size.cx != CW_USEDEFAULT &&
                        wpage->wininfo->size.cy != CW_USEDEFAULT)
                    {
                        sz = wpage->wininfo->size;
                        flags &= ~SWP_NOMOVE;
                    }
                    SetWindowPos(win->hMainWnd, HWND_TOP, pt.x, pt.y, sz.cx, sz.cy, flags);
                }

                if (wpage->page && win->page && wpage->page->file != win->page->file)
                    WINHELP_DeleteBackSet(win);
                WINHELP_InitFonts(win->hMainWnd);

                win->page = wpage->page;
                win->info = wpage->wininfo;
                win->offset = wpage->relative;
                hTextWnd = GetDlgItem(win->hMainWnd, CTL_ID_TEXT);
                // hide the window then destroy it in the message loop
                ShowWindow(hTextWnd, 0);
                break;
            }
        }
    }

    if (!win)
    {
        /* Initialize WINHELP_WINDOW struct */
        win = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(WINHELP_WINDOW));
        if (!win) return FALSE;
        win->next = Globals.win_list;
        Globals.win_list = win;

        win->hHandCur = LoadCursorW(0, (LPWSTR)IDC_HAND);
        win->back.index = 0;
        win->font_scale = 1;
        WINHELP_GrabWindow(win);
    }
    win->page = wpage->page;
    win->info = wpage->wininfo;
    WINHELP_GrabWindow(win);

    if (!bPopup && wpage->page && remember)
    {
        WINHELP_RememberPage(win, wpage);
    }

    if (bPopup)
        Globals.active_popup = win;
    else
        Globals.active_win = win;

    /* Initialize default pushbuttons */
    if (bPrimary && wpage->page)
    {
        CHAR    buffer[MAX_STRING_LEN];

        LoadStringA(Globals.hInstance, STID_CONTENTS, buffer, sizeof(buffer));
        MACRO_CreateButton("BTN_CONTENTS", buffer, "Contents()");
        LoadStringA(Globals.hInstance, STID_INDEX, buffer, sizeof(buffer));
        MACRO_CreateButton("BTN_INDEX", buffer, "Finder()");
        LoadStringA(Globals.hInstance, STID_BACK, buffer, sizeof(buffer));
        MACRO_CreateButton("BTN_BACK", buffer, "Back()");
        if (win->back.index <= 1) MACRO_DisableButton("BTN_BACK");
    }

    if (!bReUsed)
    {
        int codepage = wpage->page ? wpage->page->file->codepage : CP_ACP;
        MultiByteToWideChar(codepage, 0, WINHELP_GetCaption(wpage), -1, caption, 60);
        win->hMainWnd = CreateWindowExW((bPopup) ? WS_EX_TOOLWINDOW : WS_EX_CLIENTEDGE, MAIN_WIN_CLASS_NAME,
                                       caption,
                                       bPrimary ? WS_OVERLAPPEDWINDOW : wpage->wininfo->win_style,
                                       wpage->wininfo->origin.x, wpage->wininfo->origin.y,
                                       wpage->wininfo->size.cx, wpage->wininfo->size.cy,
                                       bPopup ? Globals.active_win->hMainWnd : NULL,
                                       bPrimary ? LoadMenuW(Globals.hInstance, MAKEINTRESOURCEW(MAIN_MENU)) : 0,
                                       Globals.hInstance, win);
        if (!bPopup)
            /* Create button box and text Window */
            CreateWindowW(BUTTON_BOX_WIN_CLASS_NAME, "", WS_CHILD | WS_VISIBLE,
                         0, 0, 0, 0, win->hMainWnd, (HMENU)CTL_ID_BUTTON, Globals.hInstance, NULL);
    }

    hTextWnd = CreateWindowW(MSFTEDIT_CLASS, NULL,
                    ES_MULTILINE | ES_READONLY | WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE,
                    0, 0, 0, 0, win->hMainWnd, (HMENU)CTL_ID_TEXT, Globals.hInstance, NULL);
    /* set ole callback for showing bitmaps */
    SendMessageW(hTextWnd, EM_SETOLECALLBACK, NULL, &callback);
    SendMessageW(hTextWnd, EM_SHOWSCROLLBAR, SB_HORZ, FALSE);
    SendMessageW(hTextWnd, EM_SETEVENTMASK, 0,
                    SendMessageW(hTextWnd, EM_GETEVENTMASK, 0, 0) | ENM_MOUSEEVENTS | ENM_LINK);
    win->origRicheditWndProc = (WNDPROC)SetWindowLongPtrA(hTextWnd, GWLP_WNDPROC,
                    (LONG_PTR)WINHELP_RicheditWndProc);

    hIcon = (wpage->page) ? wpage->page->file->hIcon : NULL;
    if (!hIcon) hIcon = LoadImageW(Globals.hInstance, MAKEINTRESOURCEW(IDI_WINHELP), IMAGE_ICON,
                                  GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_SHARED);
    SendMessageW(win->hMainWnd, WM_SETICON, ICON_SMALL, (DWORD_PTR)hIcon);

    /* Initialize file specific pushbuttons */
    if (!(wpage->wininfo->win_style & WS_POPUP) && wpage->page)
    {
        HLPFILE_MACRO  *macro;
        for (macro = wpage->page->file->first_macro; macro; macro = macro->next)
        {
            // don't jump to contents on every page load
            if (!strcmp(macro->lpszMacro, "Contents()"))
                continue;
            MACRO_ExecuteMacro(win, macro->lpszMacro);
        }

        for (macro = wpage->page->first_macro; macro; macro = macro->next)
            MACRO_ExecuteMacro(win, macro->lpszMacro);
    }
    /* See #17681, in some cases, the newly created window is closed by the macros it contains
     * (braindead), so deal with this case
     */
    for (win = Globals.win_list; win; win = win->next)
    {
        if (!lstrcmpiA(win->info->name, wpage->wininfo->name)) break;
    }
    if (!win || !WINHELP_ReleaseWindow(win)) return TRUE;

    if (bPopup)
    {
        DWORD   mask = SendMessageW(hTextWnd, EM_GETEVENTMASK, 0, 0);

        win->font_scale = Globals.active_win->font_scale;
        WINHELP_SetupText(hTextWnd, win, wpage->relative);

        /* we need the window to be shown for richedit to compute the size */
        ShowWindow(win->hMainWnd, nCmdShow);
        SendMessageW(hTextWnd, EM_SETEVENTMASK, 0, mask | ENM_REQUESTRESIZE);
        SendMessageW(hTextWnd, EM_REQUESTRESIZE, 0, 0);
        SendMessageW(hTextWnd, EM_SETEVENTMASK, 0, mask);
    }
    else
    {
        WINHELP_SetupText(hTextWnd, win, wpage->relative);
        WINHELP_LayoutMainWindow(win);
        ShowWindow(win->hMainWnd, nCmdShow);
    }

    return TRUE;
}

/******************************************************************
 *             WINHELP_OpenHelpWindow
 * Main function to search for a page and display it in a window
 */
BOOL WINHELP_OpenHelpWindow(HLPFILE_PAGE* (*lookup)(HLPFILE*, LONG, ULONG*),
                            HLPFILE* hlpfile, LONG val, HLPFILE_WINDOWINFO* wi,
                            int nCmdShow)
{
    WINHELP_WNDPAGE     wpage;

    wpage.page = lookup(hlpfile, val, &wpage.relative);
    if (wpage.page) wpage.page->file->wRefCount++;
    wpage.wininfo = wi;
    return WINHELP_CreateHelpWindow(&wpage, nCmdShow, TRUE);
}

static void WINHELP_DoLink(WINHELP_WINDOW* win, HLPFILE_LINK* link, DWORD pos)
{
    HLPFILE*                hlpfile;
    HLPFILE_WINDOWINFO*     wi;
    switch (link->cookie)
    {
        case hlp_link_link:
            if ((hlpfile = WINHELP_LookupHelpFile(link->string)))
            {
                if (link->window == -1 || link->windowName)
                    wi = WINHELP_GetWindowInfo(hlpfile, link->windowName ? link->windowName : "main");
                else if (link->window == -2)
                {
                    wi = win->info;
                    if (wi->win_style & WS_POPUP) wi = Globals.active_win->info;
                }
                else if (link->window < hlpfile->numWindows)
                    wi = &hlpfile->windows[link->window];
                else
                {
                    WINE_WARN("link to window %d/%d\n", link->window, hlpfile->numWindows);
                    break;
                }
                WINHELP_OpenHelpWindow(HLPFILE_PageByHash, hlpfile, link->hash, wi, SW_NORMAL);
            }
            break;
        case hlp_link_popup:
            if ((hlpfile = WINHELP_LookupHelpFile(link->string)))
                    WINHELP_OpenHelpWindow(HLPFILE_PageByHash, hlpfile, link->hash,
                    WINHELP_GetPopupWindowInfo(hlpfile, win, pos),
                    SW_NORMAL);
            break;
        case hlp_link_macro:
            MACRO_ExecuteMacro(win, link->string);
            break;
        default:
            WINE_FIXME("Unknown link cookie %d\n", link->cookie);
    }
}

/******************************************************************
 *             WINHELP_HandleLink
 *
 */
static BOOL WINHELP_HandleLink(ENLINK* enlink, WINHELP_WINDOW* win)
{
    HLPFILE_LINK*           link;
    BOOL                    ret = FALSE;
    WCHAR                   tmp[32];
    TEXTRANGEW              chars;
    const WCHAR             format[] = {'%', 'p', 0};

    chars.chrg.cpMin = enlink->chrg.cpMin;
    chars.chrg.cpMax = enlink->chrg.cpMax;
    chars.lpstrText = &tmp;
    SendMessageW(enlink->nmhdr.hwndFrom, EM_GETTEXTRANGE, 0, &chars);
    swscanf(tmp, format, &link);

    switch (enlink->msg)
    {
    case WM_LBUTTONDOWN:
        WINHELP_DoLink(win, link, enlink->lParam);
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_NCLBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
        ret = TRUE;
        break;
    }
    return ret;
}

/***********************************************************************
 *
 *           WINHELP_CheckPopup
 */
static BOOL WINHELP_CheckPopup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT* lret)
{
    WINHELP_WINDOW*     popup = Globals.active_popup;
    BOOL                ret = TRUE;

    if (!Globals.active_popup) return FALSE;

    switch (msg)
    {
    case WM_NOTIFY:
        {
            MSGFILTER*  msgf = (MSGFILTER*)lParam;
            switch (msgf->nmhdr.code)
            {
            case EN_MSGFILTER:
                if (!WINHELP_CheckPopup(hWnd, msgf->msg, msgf->wParam, msgf->lParam, NULL))
                    return FALSE;
                if (lret) *lret = 1;
                return TRUE;
            case EN_LINK:
                if (!WINHELP_HandleLink((ENLINK *)lParam, Globals.active_popup))
                    return FALSE;
                break;
            default:
                return FALSE;
            }
            break;
        }
    case WM_ACTIVATE:
        if (LOWORD(wParam) != WA_INACTIVE || (HWND)lParam == Globals.active_win->hMainWnd ||
            (HWND)lParam == Globals.active_popup->hMainWnd ||
            GetWindow((HWND)lParam, GW_OWNER) == Globals.active_win->hMainWnd)
            return FALSE;
        break;
    case WM_LBUTTONDOWN:
        if (hWnd == popup->hMainWnd)
        {
            static inloop = FALSE;
            if (inloop)
                return FALSE;
            inloop = TRUE;
            CallWindowProcA(popup->origRicheditWndProc, GetDlgItem(popup->hMainWnd, CTL_ID_TEXT), msg, wParam, lParam);
            inloop = FALSE;
            break;
        }
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_NCLBUTTONDOWN:
    case WM_NCMBUTTONDOWN:
    case WM_NCRBUTTONDOWN:
        if (hWnd != popup->hMainWnd)
            ret = FALSE;
        break;
    default:
        return FALSE;
    }
    WINHELP_ReleaseWindow(popup);
    if (lret) *lret = 1;
    return ret;
}

/***********************************************************************
 *
 *           WINHELP_ButtonWndProc
 */
static LRESULT CALLBACK WINHELP_ButtonWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (WINHELP_CheckPopup(hWnd, msg, wParam, lParam, NULL)) return 0;

    if (msg == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_UP:
        case VK_DOWN:
        case VK_PRIOR:
        case VK_NEXT:
        case VK_ESCAPE:
            return SendMessageA(GetParent(hWnd), msg, wParam, lParam);
        }
    }

    return CallWindowProcA(Globals.button_proc, hWnd, msg, wParam, lParam);
}

/***********************************************************************
 *
 *           WINHELP_ButtonBoxWndProc
 */
static LRESULT CALLBACK WINHELP_ButtonBoxWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WINDOWPOS      *winpos;
    WINHELP_WINDOW *win;
    WINHELP_BUTTON *button;
    SIZE button_size;
    INT  x, y;

    if (WINHELP_CheckPopup(hWnd, msg, wParam, lParam, NULL)) return 0L;

    switch (msg)
    {
    case WM_WINDOWPOSCHANGING:
        winpos = (WINDOWPOS*) lParam;
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(GetParent(hWnd), 0);

        /* Update buttons */
        button_size.cx = 0;
        button_size.cy = 0;
        for (button = win->first_button; button; button = button->next)
	{
            HDC  hDc;
            SIZE textsize;
            WCHAR u16str[21];
            u16str[20] = 0;
            MultiByteToWideChar(win->page->file->codepage, 0, button->lpszName, -1, u16str, 20);
            if (!button->hWnd)
            {
                button->hWnd = CreateWindowW(STRING_BUTTON, u16str,
                                            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                            0, 0, 0, 0,
                                            hWnd, (HMENU) button->wParam,
                                            Globals.hInstance, 0);
                if (button->hWnd)
                {
                    if (Globals.button_proc == NULL)
                    {
                        NONCLIENTMETRICSW ncm;
                        Globals.button_proc = (WNDPROC) GetWindowLongPtrW(button->hWnd, GWLP_WNDPROC);

                        ncm.cbSize = sizeof(NONCLIENTMETRICSW);
                        SystemParametersInfoW(SPI_GETNONCLIENTMETRICS,
                                              sizeof(NONCLIENTMETRICSW), &ncm, 0);
                        Globals.hButtonFont = CreateFontIndirectW(&ncm.lfMenuFont);
                    }
                    SetWindowLongPtrW(button->hWnd, GWLP_WNDPROC, (LONG_PTR) WINHELP_ButtonWndProc);
                    if (Globals.hButtonFont)
                        SendMessageW(button->hWnd, WM_SETFONT, (WPARAM)Globals.hButtonFont, TRUE);
                }
            }
            hDc = GetDC(button->hWnd);
            GetTextExtentPointW(hDc, u16str, wcslen(u16str), &textsize);
            ReleaseDC(button->hWnd, hDc);

            button_size.cx = max(button_size.cx, textsize.cx + BUTTON_CX);
            button_size.cy = max(button_size.cy, textsize.cy + BUTTON_CY);
	}

        x = 0;
        y = 0;
        for (button = win->first_button; button; button = button->next)
	{
            SetWindowPos(button->hWnd, HWND_TOP, x, y, button_size.cx, button_size.cy, 0);

            if (x + 2 * button_size.cx <= winpos->cx)
                x += button_size.cx;
            else
                x = 0, y += button_size.cy;
	}
        winpos->cy = y + (x ? button_size.cy : 0);
        break;

    case WM_COMMAND:
        SendMessageW(GetParent(hWnd), msg, wParam, lParam);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
        case VK_DOWN:
        case VK_PRIOR:
        case VK_NEXT:
        case VK_ESCAPE:
            return SendMessageA(GetParent(hWnd), msg, wParam, lParam);
        }
        break;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

/******************************************************************
 *		WINHELP_HistoryWndProc
 *
 *
 */
static LRESULT CALLBACK WINHELP_HistoryWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WINHELP_WINDOW*     win;
    PAINTSTRUCT         ps;
    HDC                 hDc;
    TEXTMETRICW         tm;
    unsigned int        i;
    RECT                r;

    switch (msg)
    {
    case WM_NCCREATE:
        win = (WINHELP_WINDOW*)((LPCREATESTRUCTW)lParam)->lpCreateParams;
        SetWindowLongPtrW(hWnd, 0, (ULONG_PTR)win);
        win->hHistoryWnd = hWnd;
        break;
    case WM_CREATE:
        hDc = GetDC(hWnd);
        SelectObject(hDc, GetStockObject(DEFAULT_GUI_FONT));
        GetTextMetricsW(hDc, &tm);
        GetWindowRect(hWnd, &r);

        r.right = r.left + 30 * tm.tmAveCharWidth;
        r.bottom = r.top + ARRAY_SIZE(Globals.history.set) * tm.tmHeight;
        AdjustWindowRect(&r, GetWindowLongW(hWnd, GWL_STYLE), FALSE);
        if (r.left < 0) {r.right -= r.left; r.left = 0;}
        if (r.top < 0) {r.bottom -= r.top; r.top = 0;}

        MoveWindow(hWnd, r.left, r.top, r.right, r.bottom, TRUE);
        ReleaseDC(hWnd, hDc);
        break;
    case WM_LBUTTONDOWN:
        hDc = GetDC(hWnd);
        SelectObject(hDc, GetStockObject(DEFAULT_GUI_FONT));
        GetTextMetricsW(hDc, &tm);
        i = HIWORD(lParam) / tm.tmHeight;
        if (i < Globals.history.index)
            WINHELP_CreateHelpWindow(&Globals.history.set[i], SW_SHOW, TRUE);
        ReleaseDC(hWnd, hDc);
        break;
    case WM_PAINT:
        hDc = BeginPaint(hWnd, &ps);
        SelectObject(hDc, GetStockObject(DEFAULT_GUI_FONT));
        GetTextMetricsW(hDc, &tm);

        for (i = 0; i < Globals.history.index; i++)
        {
            if (Globals.history.set[i].page->file == Globals.active_win->page->file)
            {
                TextOutW(hDc, 0, i * tm.tmHeight,
                        Globals.history.set[i].page->lpszTitle,
                        wcslen(Globals.history.set[i].page->lpszTitle));
            }
            else
            {
                char        buffer[50];
                WCHAR       title[101];
                const char* ptr1;
                const char* ptr2;
                unsigned    len;

                ptr1 = strrchr(Globals.history.set[i].page->file->lpszPath, '\\');
                if (!ptr1) ptr1 = Globals.history.set[i].page->file->lpszPath;
                else ptr1++;
                ptr2 = strrchr(ptr1, '.');
                len = ptr2 ? ptr2 - ptr1 : strlen(ptr1);
                if (len > sizeof(buffer)) len = sizeof(buffer);
                memcpy(buffer, ptr1, len);
                if (len < sizeof(buffer)) buffer[len++] = ':';
                buffer[len] = '\0';
                MultiByteToWideChar(CP_ACP, 0, buffer, -1, title, 100);
                wcsncat(title, Globals.history.set[i].page->lpszTitle, 100);
                title[100] = 0;
                TextOutW(hDc, 0, i * tm.tmHeight, title, wcslen(title));
            }
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
        if (hWnd == win->hHistoryWnd)
            win->hHistoryWnd = 0;
        break;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

static void comp_TTLBTree(void *p, const void *key, int leaf, void **next)
{
    *next = (char *)p + 5 + (leaf?strlen((char *)p + 4):1);
    if (leaf && (*(DWORD *)*next > key))
        return 0;
    return *(DWORD *)p - (DWORD)key;
}

static INT_PTR CALLBACK WINHELP_TopicDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static struct index_data* id;
    HWND hListWnd = GetDlgItem(hWnd, IDC_TOPICS);

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        WCHAR u16str[100];
        id = (struct index_data*)lParam;
        for (int i = 0; i < id->count; i++)
        {
            DWORD toffset = ((DWORD *)id->offset)[i];
            BYTE* ptr = HLPFILE_BPTreeSearch(id->hlpfile->ttlbtree, toffset, comp_TTLBTree) + 4;
            MultiByteToWideChar(id->hlpfile->codepage, 0, ptr, -1, u16str, 100);
            int idx = SendMessageW(hListWnd, LB_ADDSTRING, 0, (LPARAM)u16str);
            SendMessageW(hListWnd, LB_SETITEMDATA, idx, (LPARAM)toffset);
        }
        SendMessageW(hListWnd, LB_SETCURSEL, 0, 0);
    }
    case WM_COMMAND:
    {
        int sel = SendMessageW(hListWnd, LB_GETCURSEL, 0, 0);
        switch (LOWORD(wParam))
        {
        case IDC_TOPICS:
            if (HIWORD(wParam) == LBN_DBLCLK)
                EndDialog(hWnd, SendMessageW(hListWnd, LB_GETITEMDATA, sel, 0));
            break;
        case 1:
            EndDialog(hWnd, SendMessageW(hListWnd, LB_GETITEMDATA, sel, 0));
            break;
        case 2:
            EndDialog(hWnd, -1);
            break;
        }
        break;
    }
    }
    return 0;
}



/**************************************************************************
 * cb_KWBTree
 *
 * HLPFILE_BPTreeCallback enumeration function for '|KWBTREE' internal file.
 *
 */
static void cb_KWBTree(void *p, void **next, void *cookie)
{
    HWND hListWnd = ((DWORD *)cookie)[0];
    WORD cp = ((DWORD *)cookie)[1];
    int count;
    WCHAR u16str[100];

    WINE_TRACE("Adding %s to search list\n", debugstr_a((char *)p));
    MultiByteToWideChar(cp, 0, p, -1, u16str, 100);
    SendMessageW(hListWnd, LB_INSERTSTRING, -1, (LPARAM)u16str);
    count = SendMessageW(hListWnd, LB_GETCOUNT, 0, 0);
    SendMessageW(hListWnd, LB_SETITEMDATA, count-1, (LPARAM)p);
    *next = (char*)p + strlen((char*)p) + 7;
}

/**************************************************************************
 * WINHELP_IndexDlgProc
 *
 */
static INT_PTR CALLBACK WINHELP_IndexDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static struct index_data* id;
    int sel;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        DWORD data[2];
        id = (struct index_data*)((PROPSHEETPAGEA*)lParam)->lParam;
        data[0] = GetDlgItem(hWnd, IDC_INDEXLIST);
        data[1] = id->hlpfile->codepage;
        HLPFILE_BPTreeEnum(id->hlpfile->xw[0].tree, cb_KWBTree, data);
        id->jump = FALSE;
        id->offset = 1;
        SendDlgItemMessageW(hWnd, IDC_INDEXLIST, LB_SETCURSEL, 0, 0);
        return TRUE;
    }
    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case LBN_DBLCLK:
            if (LOWORD(wParam) == IDC_INDEXLIST)
                SendMessageW(GetParent(hWnd), PSM_PRESSBUTTON, PSBTN_OK, 0);
            break;
        }
        break;
    case WM_NOTIFY:
	switch (((NMHDR*)lParam)->code)
	{
	case PSN_APPLY:
            sel = SendDlgItemMessageW(hWnd, IDC_INDEXLIST, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR)
            {
                BYTE *p;

                p = (BYTE*)SendDlgItemMessageW(hWnd, IDC_INDEXLIST, LB_GETITEMDATA, sel, 0);
                p += strlen((char *)p);
                id->count = *(short*)((char *)p + 1);
                id->offset = *(ULONG*)((char *)p + 3);
                id->offset = (DWORD)(id->hlpfile->xw[0].data + id->offset + 9);
                if (id->count > 1)
                {
                    if (id->hlpfile->ttlbtree)
                    {
                        id->offset = DialogBoxParamW(NULL, MAKEINTRESOURCE(IDD_TOPIC), hWnd, WINHELP_TopicDlgProc, id);
                        if (id->offset == 0xFFFFFFFF)
                            return TRUE;
                    }
                    else
                    {
                        MessageBoxA(hWnd, "topic not found", "Error", MB_OK | MB_ICONSTOP);
                        SetWindowLongPtrW(hWnd, DWLP_MSGRESULT, PSNRET_INVALID);
                        return TRUE;
                    }                        
                }
                else
                    id->offset = *((ULONG*)id->offset);
                if (id->offset == 0xFFFFFFFF)
                {
                    MessageBoxA(hWnd, "macro keywords not supported yet", "Error", MB_OK | MB_ICONSTOP);
                    SetWindowLongPtrW(hWnd, DWLP_MSGRESULT, PSNRET_INVALID);
                    return TRUE;
                }
                id->jump = TRUE;
                SetWindowLongPtrW(hWnd, DWLP_MSGRESULT, PSNRET_NOERROR);
            }
            return TRUE;
        default:
            return FALSE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

/**************************************************************************
 * WINHELP_SearchDlgProc
 *
 */
static INT_PTR CALLBACK WINHELP_SearchDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_NOTIFY:
	switch (((NMHDR*)lParam)->code)
	{
	case PSN_APPLY:
            SetWindowLongPtrW(hWnd, DWLP_MSGRESULT, PSNRET_NOERROR);
            return TRUE;
        default:
            return FALSE;
        }
        break;
    default:
        break;
    }
    return FALSE;
}

/***********************************************************************
 *
 *           WINHELP_MainWndProc
 */
static LRESULT CALLBACK WINHELP_MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    WINHELP_WINDOW *win;
    WINHELP_BUTTON *button;
    HWND hTextWnd;
    LRESULT ret;

    if (WINHELP_CheckPopup(hWnd, msg, wParam, lParam, &ret)) return ret;

    switch (msg)
    {
    case WM_NCCREATE:
        win = (WINHELP_WINDOW*) ((LPCREATESTRUCTW) lParam)->lpCreateParams;
        SetWindowLongPtrW(hWnd, 0, (ULONG_PTR) win);
        if (!win->page && Globals.isBook)
            PostMessageW(hWnd, WM_COMMAND, MNID_FILE_OPEN, 0);
        win->hMainWnd = hWnd;
        break;

    case WM_WINDOWPOSCHANGED:
        WINHELP_LayoutMainWindow((WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0));
        break;

    case WM_COMMAND:
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
        switch (LOWORD(wParam))
	{
            /* Menu FILE */
	case MNID_FILE_OPEN:    MACRO_FileOpen();       break;
	case MNID_FILE_PRINT:	MACRO_Print();          break;
	case MNID_FILE_SETUP:	MACRO_PrinterSetup();   break;
	case MNID_FILE_EXIT:	MACRO_Exit();           break;

            /* Menu EDIT */
	case MNID_EDIT_COPYDLG:
            SendDlgItemMessageW(hWnd, CTL_ID_TEXT, WM_COPY, 0, 0);
            break;
	case MNID_EDIT_ANNOTATE:MACRO_Annotate();       break;

            /* Menu Bookmark */
	case MNID_BKMK_DEFINE:  MACRO_BookmarkDefine(); break;

            /* Menu Help */
	case MNID_HELP_HELPON:	MACRO_HelpOn();         break;
	case MNID_HELP_HELPTOP: MACRO_HelpOnTop();      break;
	case MNID_HELP_ABOUT:	MACRO_About();          break;

            /* Context help */
        case MNID_CTXT_ANNOTATE:MACRO_Annotate();       break;
        case MNID_CTXT_COPY:
            SendDlgItemMessageW(hWnd, CTL_ID_TEXT, WM_COPY, 0, 0);
            break;
        case MNID_CTXT_PRINT:   MACRO_Print();          break;
        case MNID_OPTS_HISTORY: MACRO_History();        break;
        case MNID_OPTS_FONTS_SMALL:
        case MNID_CTXT_FONTS_SMALL:
            win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
            if (win->font_scale != 0)
            {
                win->font_scale = 0;
                WINHELP_SetupText(GetDlgItem(hWnd, CTL_ID_TEXT), win, 0 /* FIXME */);
            }
            break;
        case MNID_OPTS_FONTS_NORMAL:
        case MNID_CTXT_FONTS_NORMAL:
            win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
            if (win->font_scale != 1)
            {
                win->font_scale = 1;
                WINHELP_SetupText(GetDlgItem(hWnd, CTL_ID_TEXT), win, 0 /* FIXME */);
            }
            break;
        case MNID_OPTS_FONTS_LARGE:
        case MNID_CTXT_FONTS_LARGE:
            win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
            if (win->font_scale != 2)
            {
                win->font_scale = 2;
                WINHELP_SetupText(GetDlgItem(hWnd, CTL_ID_TEXT), win, 0 /* FIXME */);
            }
            break;

	default:
            /* Buttons */
            for (button = win->first_button; button; button = button->next)
                if (wParam == button->wParam) break;
            if (button)
                MACRO_ExecuteMacro(win, button->lpszMacro);
            else if (!HIWORD(wParam))
                MessageBoxW(0, MAKEINTRESOURCEW(STID_NOT_IMPLEMENTED),
                            MAKEINTRESOURCEW(STID_WHERROR), MB_OK);
            break;
	}
        break;
/* EPP     case WM_DESTROY: */
/* EPP         if (Globals.hPopupWnd) DestroyWindow(Globals.hPopupWnd); */
/* EPP         break; */
    case WM_COPYDATA:
    {
        COPYDATASTRUCT*     cds = (COPYDATASTRUCT*)lParam;
        WINEHELP*           wh;

        if (cds->dwData != 0xA1DE505)
        {
            WINE_FIXME("Wrong magic number (%08lx)\n", cds->dwData);
            return 0;
        }
            
        return WINHELP_HandleCommand((HWND)wParam, cds->lpData, FALSE);
    }

    case 0x38: //WH_WINHELP
        return WINHELP_HandleCommand((HWND)wParam, (WINEHELP *)lParam, TRUE);

    case WM_CHAR:
        if (wParam == 3)
        {
            SendDlgItemMessageW(hWnd, CTL_ID_TEXT, WM_COPY, 0, 0);
            return 0;
        }
        break;

    case WM_KEYDOWN:
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
        hTextWnd = GetDlgItem(win->hMainWnd, CTL_ID_TEXT);

        switch (wParam)
        {
        case VK_UP:
            SendMessageW(hTextWnd, EM_SCROLL, SB_LINEUP, 0);
            return 0;
        case VK_DOWN:
            SendMessageW(hTextWnd, EM_SCROLL, SB_LINEDOWN, 0);
            return 0;
        case VK_PRIOR:
            SendMessageW(hTextWnd, EM_SCROLL, SB_PAGEUP, 0);
            return 0;
        case VK_NEXT:
            SendMessageW(hTextWnd, EM_SCROLL, SB_PAGEDOWN, 0);
            return 0;
        case VK_ESCAPE:
            MACRO_Exit();
            return 0;
        }
        break;

    case WM_NOTIFY:
        if (wParam == CTL_ID_TEXT)
        {
            RECT        rc;
            RECT        orc;

            switch (((NMHDR*)lParam)->code)
            {
            case EN_MSGFILTER:
                {
                    const MSGFILTER*    msgf = (const MSGFILTER*)lParam;
                    switch (msgf->msg)
                    {
                    case WM_LBUTTONDOWN:
                    {
                        win = (WINHELP_WINDOW*)GetWindowLongPtrW(hWnd, 0);
                        HLPFILE_HOTSPOTLINK* hs = WINHELP_FindHotSpot(win, msgf->lParam);
                        if (hs)
                        {
                            WINHELP_DoLink(win, &hs->link, msgf->lParam);
                            return TRUE;
                        }
                        break;
                    }
                    case WM_KEYUP:
                        if (msgf->wParam == VK_ESCAPE)
                            WINHELP_ReleaseWindow((WINHELP_WINDOW*)GetWindowLongPtrW(hWnd, 0));
                        break;
                    case WM_RBUTTONDOWN:
                    {
                        HMENU       hMenu;
                        POINT       pt;

                        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
                        hMenu = LoadMenuW(Globals.hInstance, MAKEINTRESOURCEW(CONTEXT_MENU));
                        switch (win->font_scale)
                        {
                        case 0:
                            CheckMenuItem(hMenu, MNID_CTXT_FONTS_SMALL,
                                          MF_BYCOMMAND|MF_CHECKED);
                            break;
                        default:
                            WINE_FIXME("Unsupported %d\n", win->font_scale);
                            /* fall through */
                        case 1:
                            CheckMenuItem(hMenu, MNID_CTXT_FONTS_NORMAL,
                                          MF_BYCOMMAND|MF_CHECKED);
                            break;
                        case 2:
                            CheckMenuItem(hMenu, MNID_CTXT_FONTS_LARGE,
                                          MF_BYCOMMAND|MF_CHECKED);
                            break;
                        }
                        pt.x = (int)(short)LOWORD(msgf->lParam);
                        pt.y = (int)(short)HIWORD(msgf->lParam);
                        ClientToScreen(msgf->nmhdr.hwndFrom, &pt);
                        TrackPopupMenu(GetSubMenu(hMenu, 0), TPM_LEFTALIGN|TPM_TOPALIGN,
                                       pt.x, pt.y, 0, hWnd, NULL);
                        DestroyMenu(hMenu);
                    }
                    break;
                    default:
                        break;
                    }
                }
                break;

            case EN_REQUESTRESIZE:
                rc = ((REQRESIZE*)lParam)->rc;
                win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
                GetClientRect(win->hMainWnd, &orc);
                /* permit the popup to get wider but not narrower */
                if ((rc.right - rc.left) < orc.right)
                {
                    rc.left = orc.left;
                    rc.right = orc.right;
                }
                AdjustWindowRect(&rc, GetWindowLongW(win->hMainWnd, GWL_STYLE),
                                 FALSE);
                SetWindowPos(win->hMainWnd, HWND_TOP, 0, 0,
                             rc.right - rc.left, rc.bottom - rc.top,
                             SWP_NOMOVE | SWP_NOZORDER);
                WINHELP_LayoutMainWindow(win);
                break;

            case EN_LINK:
                WINHELP_HandleLink((ENLINK *)lParam, (WINHELP_WINDOW*)GetWindowLongPtrW(hWnd, 0));
                break;
            }
        }
        break;

    case WM_INITMENUPOPUP:
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
        CheckMenuItem((HMENU)wParam, MNID_OPTS_FONTS_SMALL,
                      (win->font_scale == 0) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem((HMENU)wParam, MNID_OPTS_FONTS_NORMAL,
                      (win->font_scale == 1) ? MF_CHECKED : MF_UNCHECKED);
        CheckMenuItem((HMENU)wParam, MNID_OPTS_FONTS_LARGE,
                      (win->font_scale == 2) ? MF_CHECKED : MF_UNCHECKED);
        break;
    case WM_DESTROY:
        win = (WINHELP_WINDOW*) GetWindowLongPtrW(hWnd, 0);
        WINHELP_DeleteWindow(win);
        break;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

/**************************************************************************
 * WINHELP_CreateIndexWindow
 *
 * Displays a dialog with keywords of current help file.
 *
 */
BOOL WINHELP_CreateIndexWindow(BOOL is_search)
{
    HPROPSHEETPAGE      psPage[3];
    PROPSHEETPAGEA      psp;
    PROPSHEETHEADERA    psHead;
    struct index_data   id;
    char                buf[256];
    WCHAR               u16buf[256];
    if (Globals.active_win && Globals.active_win->page && Globals.active_win->page->file)
        id.hlpfile = Globals.active_win->page->file;
    else
        return FALSE;

    if (id.hlpfile->xw[0].id != 'K')
    {
        WINE_TRACE("Missing Keyword File\n");
        return FALSE;
    }

    InitCommonControls();

    id.jump = FALSE;
    memset(&psp, 0, sizeof(psp));
    psp.dwSize = sizeof(psp);
    psp.dwFlags = 0;
    psp.hInstance = Globals.hInstance;

    psp.u.pszTemplate = MAKEINTRESOURCEA(IDD_INDEX);
    psp.lParam = (LPARAM)&id;
    psp.pfnDlgProc = WINHELP_IndexDlgProc;
    psPage[0] = CreatePropertySheetPageW(&psp);

    psp.u.pszTemplate = MAKEINTRESOURCEA(IDD_SEARCH);
    psp.lParam = (LPARAM)&id;
    psp.pfnDlgProc = WINHELP_SearchDlgProc;
    psPage[1] = CreatePropertySheetPageW(&psp);

    memset(&psHead, 0, sizeof(psHead));
    psHead.dwSize = sizeof(psHead);

    LoadStringA(Globals.hInstance, STID_PSH_INDEX, buf, sizeof(buf));
    strcat(buf, Globals.active_win->info->caption);
    MultiByteToWideChar(id.hlpfile->codepage, 0, buf, -1, u16buf, 256);

    psHead.pszCaption = u16buf;
    psHead.nPages = 2;
    psHead.u2.nStartPage = is_search ? 1 : 0;
    psHead.hwndParent = Globals.active_win->hMainWnd;
    psHead.u3.phpage = psPage;
    psHead.dwFlags = PSH_NOAPPLYNOW;

    PropertySheetW(&psHead);
    if (id.jump)
    {
        WINE_TRACE("got %d as an offset\n", id.offset);
        WINHELP_OpenHelpWindow(HLPFILE_PageByOffset, id.hlpfile, id.offset,
                               Globals.active_win->info, SW_NORMAL);
    }
    return TRUE;
}

/***********************************************************************
 *
 *           RegisterWinClasses
 */
static BOOL WINHELP_RegisterWinClasses(void)
{
    WNDCLASSEXA class_main, class_button_box, class_history;

    class_main.cbSize              = sizeof(class_main);
    class_main.style               = CS_HREDRAW | CS_VREDRAW;
    class_main.lpfnWndProc         = WINHELP_MainWndProc;
    class_main.cbClsExtra          = 0;
    class_main.cbWndExtra          = sizeof(WINHELP_WINDOW *);
    class_main.hInstance           = Globals.hInstance;
    class_main.hIcon               = LoadIconW(Globals.hInstance, MAKEINTRESOURCEW(IDI_WINHELP));
    class_main.hCursor             = LoadCursorW(0, (LPWSTR)IDC_ARROW);
    class_main.hbrBackground       = (HBRUSH)(COLOR_WINDOW+1);
    class_main.lpszMenuName        = 0;
    class_main.lpszClassName       = MAIN_WIN_CLASS_NAME;
    class_main.hIconSm             = LoadImageW(Globals.hInstance, MAKEINTRESOURCEW(IDI_WINHELP), IMAGE_ICON,
                                               GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
                                               LR_SHARED);

    class_button_box               = class_main;
    class_button_box.lpfnWndProc   = WINHELP_ButtonBoxWndProc;
    class_button_box.cbWndExtra    = 0;
    class_button_box.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    class_button_box.lpszClassName = BUTTON_BOX_WIN_CLASS_NAME;

    class_history                  = class_main;
    class_history.lpfnWndProc      = WINHELP_HistoryWndProc;
    class_history.lpszClassName    = HISTORY_WIN_CLASS_NAME;

    return (RegisterClassExW(&class_main) &&
            RegisterClassExW(&class_button_box) &&
            RegisterClassExW(&class_history));
}

/***********************************************************************
 *
 *           WinMain
 */
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE prev, LPSTR cmdline, int show)
{
    MSG                 msg;
    LONG                lHash = 0;
    HLPFILE*            hlpfile;
    static CHAR         default_wndname[] = "main";
    LPSTR               wndname = default_wndname;
    WINHELP_DLL*        dll;
    HACCEL              hAccel;

    Globals.hInstance = hInstance;

    if (LoadLibraryA("msftedit.dll") == NULL)
        return MessageBoxW(0, MAKEINTRESOURCEW(STID_NO_RICHEDIT),
                           MAKEINTRESOURCEW(STID_WHERROR), MB_OK);

    /* Get options */
    while (*cmdline && (*cmdline == ' ' || *cmdline == '-'))
    {
        CHAR   option;
        LPCSTR topic_id;
        if (*cmdline++ == ' ') continue;

        option = *cmdline;
        if (option) cmdline++;
        while (*cmdline == ' ') cmdline++;
        switch (option)
	{
	case 'i':
	case 'I':
            topic_id = cmdline;
            while (*cmdline && *cmdline != ' ') cmdline++;
            if (*cmdline) *cmdline++ = '\0';
            lHash = HLPFILE_Hash(topic_id);
            break;

	case '3':
	case '4':
            Globals.wVersion = option - '0';
            break;

        case 'x':
            show = SW_HIDE;
            Globals.isBook = FALSE;
            break;

        default:
            WINE_FIXME("Unsupported cmd line: %s\n", debugstr_a(cmdline));
            break;
	}
    }

    /* Create primary window */
    if (!WINHELP_RegisterWinClasses())
    {
        WINE_FIXME("Couldn't register classes\n");
        return 0;
    }

    if (*cmdline)
    {
        char*   ptr;
        if ((*cmdline == '"') && (ptr = strchr(cmdline+1, '"')))
        {
            cmdline++;
            *ptr = '\0';
        }
        if ((ptr = strchr(cmdline, '>')))
        {
            *ptr = '\0';
            wndname = ptr + 1;
        }
        hlpfile = WINHELP_LookupHelpFile(cmdline);
        if (!hlpfile) return 0;
    }
    else hlpfile = NULL;
    WINHELP_OpenHelpWindow(HLPFILE_PageByHash, hlpfile, lHash,
                           WINHELP_GetWindowInfo(hlpfile, wndname), show);

    /* Message loop */
    hAccel = LoadAcceleratorsW(hInstance, MAKEINTRESOURCEW(MAIN_ACCEL));
    while ((Globals.win_list || Globals.active_popup) && GetMessageW(&msg, 0, 0, 0))
    {
        HWND hWnd = Globals.active_win ? Globals.active_win->hMainWnd : NULL;
        if (!TranslateAcceleratorW(hWnd, hAccel, &msg))
	{
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    for (dll = Globals.dlls; dll; dll = dll->next)
    {
        if (dll->class & DC_INITTERM) dll->handler(DW_TERM, 0, 0);
    }
    return 0;
}
