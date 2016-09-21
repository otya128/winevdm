

#include "config.h"


#include "windef.h"
#include "wine/windef16.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/list.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(winoldap);

//LRESULT WINAPI WndProc(HWND16 h, &)
LRESULT WINAPI WndProc(HWND16 hwnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    ERR("wndproc");
}
LRESULT WINAPI DialogProc(HWND16 hwnd16, UINT16 msg, WPARAM16 wParam, LPARAM lParam)
{
    ERR("dialogproc");
}


