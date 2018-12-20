#include "windef.h"
#include "winbase.h"
#include "wine/debug.h"
#include "wine/winbase16.h"
#include "wownt32.h"

WINE_DEFAULT_DEBUG_CHANNEL(timer);
LRESULT WINAPI DriverProc16(DWORD dwDevID, HDRVR16 hDrv, WORD wMsg,
			    DWORD dwParam1, DWORD dwParam2)
{
	FIXME("\n");
	return 0;
}
