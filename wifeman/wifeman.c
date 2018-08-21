
#include "wine/debug.h"
#include "windef.h"
#include "winbase.h"
#include "wownt32.h"
#include "wine/winuser16.h"
WINE_DEFAULT_DEBUG_CHANNEL(wifeman);
/*
 End-User-Defined and Private Use Area Characters(ŠOŽš)
 This API seems to work only in Japanese Windows.

 Japanese, Simplified Chinese, Korean, Traditional Chinese, (Unicode)
*/
WORD WINAPI MiscGetEUDCLeadByteRange16(WORD charaset /* 128: SHIFTJIS_CHARSET? */)
{
    /* HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\NLS\CodePage\EUDCCodeRange */
    if (charaset != SHIFTJIS_CHARSET)
        FIXME("MiscGetEUDCLeadByteRange(%d)\n", charaset);
    /*
       LOBYTE: begin
       HIBYTE: end
    */
    return MAKEWORD(0xF0, 0xF9);
}

BOOL WINAPI MiscIsDBCSLeadByte16(CHAR c)
{
    return IsDBCSLeadByte(c);
}
