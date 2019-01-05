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
    case SB_GETBORDERS:
    {
        INT borders32[3];
        INT16 *borders = MapSL(lParam);
        LRESULT result;
        int i;
        if (!HIWORD(borders))
            break;
        result = CallWindowProcA(status_window_class.lpfnWndProc, hwnd, msg, wParam, (LPARAM)&borders32);
        for (i = 0; i < 3; i++)
            borders[i] = borders32[i];
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
        SendMessageA (hwndTB, TB_ADDBUTTONSA, iNumButtons, (LPARAM)MapLS(lpButtons));
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


#include <pshpack1.h>
typedef struct
{
    WORD command_id_off;
    WORD menu_id_off;
    struct
    {
        WORD popup_id;
        HMENU16 hPopup;
    } popup_ids[1];
} MENUHELP16;
#include <poppack.h>
static int get_menu_off(HMENU hMainMenu, HMENU hSubMenu)
{
    int i = 0;
    int count = GetMenuItemCount(hMainMenu);
    for (i = 0; i < count; i++)
    {
        HMENU s = GetSubMenu(hMainMenu, i);
        if (s == hSubMenu)
        {
            return i;
        }
    }
    return -1;
}
/* based on wine */
VOID WINAPI
MenuHelp16 (UINT16 uMsg, WPARAM16 wParam, LPARAM lParam, HMENU16 hMainMenu, HINSTANCE16 hInst, HWND16 hwndStatus, MENUHELP16 *lpwIDs)
{
    UINT uMenuID = 0;
    WORD wIDItem = wParam;
    WORD fwMenu = LOWORD(lParam);
    HMENU hMenu = HMENU_32(HIWORD(lParam));

    if (!IsWindow (HWND_32(hwndStatus)))
	return;

    switch (uMsg) {
	case WM_MENUSELECT:
	    TRACE("WM_MENUSELECT wParam=0x%lX lParam=0x%lX\n",
		   wParam, lParam);

            if ((fwMenu == 0xFFFF) && (hMenu == 0)) {
                /* menu was closed */
		TRACE("menu was closed!\n");
                SendMessageA (HWND_32(hwndStatus), SB_SIMPLE, FALSE, 0);
            }
	    else {
		/* menu item was selected */
		if (fwMenu & MF_POPUP)
        {
            if (!uMenuID)
            {
                int i = 0;
                HMENU hMainMenu32 = HMENU_32(hMainMenu);
                HMENU hSubMenu = HMENU_32(wParam);
                int off = get_menu_off(hMainMenu32, hSubMenu);
                if (off != -1)
                {
                    uMenuID = off + lpwIDs->menu_id_off;
                }
                else
                {
                    while (TRUE)
                    {
                        if (lpwIDs->popup_ids[i].hPopup == 0 && lpwIDs->popup_ids[i].popup_id)
                            break;
                        if (lpwIDs->popup_ids[i].hPopup == wIDItem)
                        {
                            uMenuID = lpwIDs->popup_ids[i].popup_id;
                            break;
                        }
                        i++;
                    }
                }
            }
        }
		else
		    uMenuID = lpwIDs->command_id_off + (UINT)wIDItem;
		TRACE("uMenuID = %u\n", uMenuID);

		if (uMenuID) {
		    CHAR szText[256];

		    if (!LoadString16 (hInst, uMenuID, szText, ARRAYSIZE(szText)))
			szText[0] = '\0';

		    SendMessageA (HWND_32(hwndStatus), SB_SETTEXTA,
				    255 | SBT_NOBORDERS, (LPARAM)szText);
		    SendMessageA (HWND_32(hwndStatus), SB_SIMPLE, TRUE, 0);
		}
	    }
	    break;

        case WM_COMMAND :
	    TRACE("WM_COMMAND wParam=0x%lX lParam=0x%lX\n",
		   wParam, lParam);
	    /* WM_COMMAND is not invalid since it is documented
	     * in the windows api reference. So don't output
             * any FIXME for WM_COMMAND
             */
	    WARN("We don't care about the WM_COMMAND\n");
	    break;

	default:
	    FIXME("Invalid Message 0x%x!\n", uMsg);
	    break;
    }
}

HBITMAP16 WINAPI CreateMappedBitmap16(HINSTANCE16 hInstance, INT16 idBitmap, UINT16 wFlags, LPCOLORMAP lpColorMap, INT16 iNumMaps)
{
    /* wine */
    HGLOBAL16 hglb;
    HRSRC16 hRsrc;
    const BITMAPINFOHEADER *lpBitmap;
    LPBITMAPINFOHEADER lpBitmapInfo;
    UINT nSize, nColorTableSize, iColor;
    RGBQUAD *pColorTable;
    INT i, iMaps, nWidth, nHeight;
    HDC hdcScreen;
    HBITMAP hbm;
    LPCOLORMAP sysColorMap;
    COLORREF cRef;
    COLORMAP internalColorMap[4] =
	{{0x000000, 0}, {0x808080, 0}, {0xC0C0C0, 0}, {0xFFFFFF, 0}};

    /* initialize pointer to colortable and default color table */
    if (lpColorMap) {
	iMaps = iNumMaps;
	sysColorMap = lpColorMap;
    }
    else {
	internalColorMap[0].to = GetSysColor (COLOR_BTNTEXT);
	internalColorMap[1].to = GetSysColor (COLOR_BTNSHADOW);
	internalColorMap[2].to = GetSysColor (COLOR_BTNFACE);
	internalColorMap[3].to = GetSysColor (COLOR_BTNHIGHLIGHT);
	iMaps = 4;
	sysColorMap = internalColorMap;
    }

    hRsrc = FindResource16 (hInstance, (LPSTR)idBitmap, (LPSTR)RT_BITMAP);
    if (hRsrc == 0)
	return 0;
    hglb = LoadResource16 (hInstance, hRsrc);
    if (hglb == 0)
	return 0;
    lpBitmap = LockResource16 (hglb);
    if (lpBitmap == NULL)
	return 0;

    if (lpBitmap->biSize >= sizeof(BITMAPINFOHEADER) && lpBitmap->biClrUsed)
        nColorTableSize = lpBitmap->biClrUsed;
    else if (lpBitmap->biBitCount <= 8)	
        nColorTableSize = (1 << lpBitmap->biBitCount);
    else
        nColorTableSize = 0;
    nSize = lpBitmap->biSize;
    if (nSize == sizeof(BITMAPINFOHEADER) && lpBitmap->biCompression == BI_BITFIELDS)
        nSize += 3 * sizeof(DWORD);
    nSize += nColorTableSize * sizeof(RGBQUAD);
    lpBitmapInfo = GlobalAlloc (GMEM_FIXED, nSize);
    if (lpBitmapInfo == NULL)
	return 0;
    RtlMoveMemory (lpBitmapInfo, lpBitmap, nSize);

    pColorTable = (RGBQUAD*)(((LPBYTE)lpBitmapInfo) + lpBitmapInfo->biSize);

    for (iColor = 0; iColor < nColorTableSize; iColor++) {
	for (i = 0; i < iMaps; i++) {
            cRef = RGB(pColorTable[iColor].rgbRed,
                       pColorTable[iColor].rgbGreen,
                       pColorTable[iColor].rgbBlue);
	    if ( cRef  == sysColorMap[i].from) {
#if 0
		if (wFlags & CBS_MASKED) {
		    if (sysColorMap[i].to != COLOR_BTNTEXT)
			pColorTable[iColor] = RGB(255, 255, 255);
		}
		else
#endif
                    pColorTable[iColor].rgbBlue  = GetBValue(sysColorMap[i].to);
                    pColorTable[iColor].rgbGreen = GetGValue(sysColorMap[i].to);
                    pColorTable[iColor].rgbRed   = GetRValue(sysColorMap[i].to);
		break;
	    }
	}
    }
    nWidth  = lpBitmapInfo->biWidth;
    nHeight = lpBitmapInfo->biHeight;
    hdcScreen = GetDC (NULL);
    hbm = CreateCompatibleBitmap (hdcScreen, nWidth, nHeight);
    if (hbm) {
	HDC hdcDst = CreateCompatibleDC (hdcScreen);
	HBITMAP hbmOld = SelectObject (hdcDst, hbm);
	const BYTE *lpBits = (const BYTE *)lpBitmap + nSize;
	StretchDIBits (hdcDst, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
		         lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS,
		         SRCCOPY);
	SelectObject (hdcDst, hbmOld);
	DeleteDC (hdcDst);
    }
    ReleaseDC (NULL, hdcScreen);
    GlobalFree (lpBitmapInfo);
    FreeResource16 (hglb);

    return HBITMAP_16(hbm);
}

HWND16 WINAPI CreateToolbar16(HWND16 hwnd, DWORD style, UINT16 wID, INT16 nBitmaps, HINSTANCE16 hBMInst, UINT16 wBMID, LPTBBUTTON16 lpButtons, INT16 iNumButtons)
{
    return CreateToolbarEx16(hwnd, style | CCS_NODIVIDER, wID, nBitmaps,
        hBMInst, wBMID, lpButtons,
        iNumButtons, 0, 0, 0, 0, sizeof(TBBUTTON16));
}

