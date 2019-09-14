#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "winbase.h"
#include "wingdi.h"
#include "vfw.h"

typedef struct
{
    PGETFRAME frame32;
    SEGPTR frame16;
} GETFRAME16;

GETFRAME16* WINAPI AVIStreamGetFrameOpen16(PAVISTREAM pavi, LPBITMAPINFOHEADER lpbiWanted)
{
    GETFRAME16* pframe16;
    PGETFRAME pframe32 = AVIStreamGetFrameOpen(pavi, lpbiWanted);

    if (!pframe32)
        return NULL;

    pframe16 = (GETFRAME16 *)HeapAlloc(GetProcessHeap(), 0, sizeof(GETFRAME16));
    pframe16->frame32 = pframe32;
    pframe16->frame16 = NULL;
    return pframe16;
}

LPVOID WINAPI AVIStreamGetFrame16(GETFRAME16* pg, LONG lPos)
{
    if (pg->frame16)
        UnMapLS(pg->frame16);

    pg->frame16 = MapLS(AVIStreamGetFrame(pg->frame32, lPos));
    return pg->frame16;
}

LRESULT WINAPI AVIStreamGetFrameClose16(GETFRAME16* pg)
{
    if (pg->frame16)
        UnMapLS(pg->frame16);
    
    LRESULT ret = AVIStreamGetFrameClose(pg->frame32);
    HeapFree(GetProcessHeap(), 0, pg);
    return ret;
}
