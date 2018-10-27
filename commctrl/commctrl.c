#include <Windows.h>
#include <CommCtrl.h>
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "wine/winuser16.h"
#include "wine/debug.h"
#include <WowNT32.h>

WINE_DEFAULT_DEBUG_CHANNEL(commctrl);

/* FIXME: CreateWindow("ToolbarWindow") fail */
#include <pshpack1.h>

typedef struct  {
    INT16 iBitmap;
    INT16 idCommand;
    BYTE fsState;
    BYTE fsStyle;
    INT16 idsHelp; /* ? */
} TBBUTTON16, NEAR* PTBBUTTON16, *LPTBBUTTON16;
typedef struct {
    SEGPTR ini_section;
    SEGPTR ini_file;
} TBSAVEPARAMS16, *LPTBSAVEPARAMS16;
#include <poppack.h>
#define TOOLBARCLASSNAME16 "ToolbarWindow"
#define STATUSCLASSNAME16 "msctls_statusbar"
HINSTANCE16 hInstance16;
WNDCLASSEXA toolbar_window_class = { sizeof(WNDCLASSEXA) };
WNDCLASSEXA status_window_class = { sizeof(WNDCLASSEXA) };
LRESULT WINAPI ToolbarWindowProc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI StatusWindowProc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void WINAPI InitCommonControls16(void)
{
    static BOOL init;
    if (init)
        return;
    init = TRUE;
    InitCommonControls();
    if (GetClassInfoExA(NULL, TOOLBARCLASSNAMEA, &toolbar_window_class))
    {
        WNDCLASSEXA cls16 = toolbar_window_class;
        cls16.hInstance = hInstance16 = LoadLibrary16("COMMCTRL");
        cls16.lpszClassName = TOOLBARCLASSNAME16;
        cls16.lpfnWndProc = ToolbarWindowProc16;
        if (!RegisterClassExA(&cls16))
        {
            ERR("\n");
        }
    }
    if (GetClassInfoExA(NULL, STATUSCLASSNAMEA, &status_window_class))
    {
        WNDCLASSEXA cls16 = status_window_class;
        cls16.hInstance = hInstance16;
        cls16.lpszClassName = STATUSCLASSNAME16;
        cls16.lpfnWndProc = StatusWindowProc16;
        if (!RegisterClassExA(&cls16))
        {
            ERR("\n");
        }
    }
}
LRESULT WINAPI StatusWindowProc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case SB_GETTEXTA:
    case SB_SETTEXTA:
    {
        LPCSTR str = (LPCSTR)MapSL(lParam);
        if (!HIWORD(str))
        {
            str = (LPCSTR)lParam;
        }
        return CallWindowProcA(status_window_class.lpfnWndProc, hwnd, msg, wParam, (LPARAM)str);
    }
    case SB_GETPARTS:
    case SB_SETPARTS:
    {
        LPINT16 ary = (LPINT16)MapSL(lParam);
        LRESULT result;
        INT ary32[256];
        if (wParam > 256)
            return FALSE;
        if (!HIWORD(ary))
        {
            break;
        }
        for (int i = 0; i < wParam; i++)
        {
            ary32[i] = ary[i];
        }
        result = CallWindowProcA(status_window_class.lpfnWndProc, hwnd, msg, wParam, (LPARAM)&ary32);
        for (int i = 0; i < wParam; i++)
        {
            ary[i] = ary32[i];
        }
        return result;
    }
    }
    return CallWindowProcA(status_window_class.lpfnWndProc, hwnd, msg, wParam, lParam);
}
void TBBUTTON16_32(LPTBBUTTON btn32, LPTBBUTTON16 lpButtons)
{
    btn32->iBitmap = lpButtons->iBitmap;
    btn32->idCommand = lpButtons->idCommand;
    btn32->fsState = lpButtons->fsState;
    btn32->fsStyle = lpButtons->fsStyle;
    btn32->dwData = lpButtons->idsHelp;
    btn32->iString = 0;
}
LRESULT WINAPI ToolbarWindowProc16(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result;
    switch (msg)
    {
    case TB_BUTTONSTRUCTSIZE:
    {
        SetPropW(hwnd, L"TB_BUTTONSTRUCTSIZE", wParam);
        wParam = sizeof(TBBUTTON);
        break;
    }
    break;
    case TB_ADDBITMAP:
    {
        HINSTANCE16 hInst16 = LOWORD(lParam);
        WORD nID16 = HIWORD(lParam);
        TBADDBITMAP tb32 = { 0 };
        if (!hInst16)
        {
            tb32.nID = HBITMAP_32(nID16);
        }
        else
        {
            tb32.nID = HBITMAP_32(LoadBitmap16(hInst16, (LPCSTR)nID16));
        }
        lParam = &tb32;
        return CallWindowProcA(toolbar_window_class.lpfnWndProc, hwnd, msg, wParam, lParam);
    }
    case TB_ADDBUTTONSA:
    {
        LPTBBUTTON16 btn = (LPTBBUTTON16)MapSL(lParam);
        LPTBBUTTON btn32;
        SIZE_T button_struct_size;
        if (!HIWORD(btn))
            break;
        button_struct_size = GetPropW(hwnd, L"TB_BUTTONSTRUCTSIZE");
        btn32 = HeapAlloc(GetProcessHeap(), 0, sizeof(TBBUTTON) * wParam);
        for (int i = 0; i < wParam; i++)
        {
            TBBUTTON16_32(btn32 + i, (LPTBBUTTON16)((LPBYTE)btn + button_struct_size * i));
        }
        result = CallWindowProcA(toolbar_window_class.lpfnWndProc, hwnd, msg, wParam, (LPARAM)btn32);
        HeapFree(GetProcessHeap(), 0, btn32);
        return result;
    }
    case TB_SAVERESTOREA:
    {
        LPTBSAVEPARAMS16 sp = (LPTBSAVEPARAMS16)MapSL(lParam);
        TBSAVEPARAMSA sp32 = { 0 };
        if (!HIWORD(sp))
            break;
        if (!wParam)
        {
            sp32.hkr = NULL;
            sp32.pszSubKey = (LPCSTR)MapSL(sp->ini_section);
            sp32.pszValueName = (LPCSTR)MapSL(sp->ini_file);
        }
        /* ini file?? */
        FIXME("TB_SAVERESTORE\n");
        result = 0;
        /* result = CallWindowProcA(toolbar_window_class.lpfnWndProc, hwnd, msg, wParam, (LPARAM)&sp32); */
        if (wParam)
        {
            sp->ini_section = MapLS(sp32.pszSubKey);
            sp->ini_file = MapLS(sp32.pszValueName);
        }
        return result;
    }
    default:
        break;
    }
    return CallWindowProcA(toolbar_window_class.lpfnWndProc, hwnd, msg, wParam, lParam);
}
/* based on wine */

HWND16 WINAPI CreateToolbarEx16(HWND16 hwnd, DWORD style /* window style */, UINT16 wID /* control id */, INT16 nBitmaps,
    HINSTANCE16 hBMInst, WORD wBMID /* bitmap resource id or bitmap handle(hBMInst == NULL) */, LPTBBUTTON16 lpButtons,
    INT16 iNumButtons, INT16 dxButton, INT16 dyButton,
    INT16 dxBitmap, INT16 dyBitmap, UINT16 uStructSize)
{
    TBBUTTON buttons32 = { 0 };
    HWND hwndTB;
    HBITMAP bitmap32 = NULL;
    InitCommonControls16();

    hwndTB =
        CreateWindowExA(0, TOOLBARCLASSNAME16, NULL, style|WS_CHILD, 0,0,100,30,
                        HWND_32(hwnd), (HMENU)(DWORD_PTR)wID, hInstance16, NULL);
    if(hwndTB) {

        SendMessageA (hwndTB, TB_BUTTONSTRUCTSIZE, uStructSize, 0);

       /* set bitmap and button size */
       /*If CreateToolbarEx receives 0, windows sets default values*/
       if (dxBitmap < 0)
           dxBitmap = 16;
       if (dyBitmap < 0)
           dyBitmap = 16;
       if (dxBitmap == 0 || dyBitmap == 0)
           dxBitmap = dyBitmap = 16;
       SendMessageA(hwndTB, TB_SETBITMAPSIZE, 0, MAKELPARAM(dxBitmap, dyBitmap));

       if (dxButton < 0)
           dxButton = dxBitmap;
       if (dyButton < 0)
           dyButton = dyBitmap;
       /* TB_SETBUTTONSIZE -> TB_SETBITMAPSIZE bug introduced for Windows compatibility */
       if (dxButton != 0 && dyButton != 0)
            SendMessageA(hwndTB, TB_SETBITMAPSIZE, 0, MAKELPARAM(dxButton, dyButton));


	/* add bitmaps */
	if (nBitmaps > 0 || hBMInst == HINST_COMMCTRL)
	{
            SendMessageA (hwndTB, TB_ADDBITMAP, nBitmaps, MAKELONG(hBMInst, wBMID));
	}
	/* add buttons */
	if(iNumButtons > 0)
        SendMessageA (hwndTB, TB_ADDBUTTONSA, iNumButtons, (LPARAM)lpButtons);
    }
    return HWND_16(hwndTB);
}

HWND16 WINAPI CreateStatusWindow16(LONG style, LPCSTR text, HWND16 hwndParent, UINT16 wid)
{
    HWND parent = HWND_32(hwndParent);
    return HWND_16(CreateWindowA(STATUSCLASSNAME16, text, style,
			   CW_USEDEFAULT, CW_USEDEFAULT,
			   CW_USEDEFAULT, CW_USEDEFAULT,
			   parent, (HMENU)(DWORD_PTR)wid, 0, 0));
}
LPRECT RECT16_32(LPRECT rect32, LPRECT16 lprc)
{
    if (!lprc)
        return NULL;
    rect32->left = lprc->left;
    rect32->right = lprc->right;
    rect32->top = lprc->top;
    rect32->bottom = lprc->bottom;
    return rect32;
}
LPRECT16 RECT32_16(LPRECT rect32, LPRECT16 lprc)
{
    if (!lprc)
        return NULL;
    lprc->left = rect32->left;
    lprc->right = rect32->right;
    lprc->top = rect32->top;
    lprc->bottom = rect32->bottom;
    return lprc;
}

void WINAPI GetEffectiveClientRect16(HWND16 hWnd, LPRECT16 lprc, const INT16 *lpInfo)
{
    HWND hWnd32 = HWND_32(hWnd);
    RECT rect32;
    SIZE_T len = 0;
    LPINT info32;
    const INT16 *lpRun = lpInfo;
    do
    {
        lpRun += 2;
        if (*lpRun == 0)
            break;
        lpRun += 2;
    } while (*lpRun);
    len = (lpRun - lpInfo) + 1;
    info32 = (LPINT)HeapAlloc(GetProcessHeap(), 0, len * sizeof(INT));
    for (int i = 0; i < len; i++)
    {
        info32[i] = lpInfo[i];
    }
    GetEffectiveClientRect(hWnd32, RECT16_32(&rect32, lprc), info32);
    RECT32_16(&rect32, lprc);
    HeapFree(GetProcessHeap(), 0, info32);
}
static int find_sub_menu( HMENU *hmenu, HMENU16 target )
{
    int i, pos, count = GetMenuItemCount( *hmenu );

    for (i = 0; i < count; i++)
    {
        HMENU sub = GetSubMenu( *hmenu, i );
        if (!sub) continue;
        if (HMENU_16(sub) == target) return i;
        if ((pos = find_sub_menu( &sub, target )) != -1)
        {
            *hmenu = sub;
            return pos;
        }
    }
    return -1;
}

void WINAPI MenuHelp16(UINT16 uMsg, WPARAM16 wParam, LPARAM lParam, HMENU16 hMainMenu, HINSTANCE16 hInst, HWND16 hwndStatus, UINT16 *lpwIDs)
{
    LPARAM lp32 = lParam;
    WPARAM wp32 = wParam;
    SIZE_T len = 0;
    UINT16 *run = lpwIDs;
    UINT *ids32;
    while (TRUE)
    {
        run += 2;
        if (run[0] == 0 && run[1] == 0)
            break;
    }
    len = (SIZE_T)(run + 2 - lpwIDs);
    ids32 = (UINT*)HeapAlloc(GetProcessHeap(), 0, len * sizeof(UINT));
    for (int i = 0; i < len - 2; i += 2)
    {
        /* FIXME: LoadString */
        ids32[i] = lpwIDs[i + 2];
        ids32[i + 1] = lpwIDs[i + 2 + 1];
    }
    if (uMsg == WM_MENUSELECT)
    {
        if ((LOWORD(lParam) & MF_POPUP) && (LOWORD(lParam) != 0xFFFF))
        {
            HMENU hmenu = HMENU_32(HIWORD(lParam));
            int pos = find_sub_menu(&hmenu, wParam);
            if (pos == -1) pos = 0;
            wParam = pos;
        }
        wp32 = MAKEWPARAM(wParam, LOWORD(lParam));
        lp32 = (LPARAM)HMENU_32(HIWORD(lParam));
    }
    if (uMsg == WM_COMMAND)
    {
        wp32 = MAKEWPARAM(wParam, HIWORD(lParam));
        lp32 = (LPARAM)HWND_32(LOWORD(lParam));
    }
    FIXME("(%04x, %04x, %08x, %04x, %04x, %04x, %p) stub\n", uMsg, wParam, lParam, hMainMenu, hInst, hwndStatus, lpwIDs);
    MenuHelp(uMsg, wp32, lp32, HMENU_32(hMainMenu), hInst, HWND_32(hwndStatus), ids32);
    HeapFree(GetProcessHeap(), 0, ids32);
}
