#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "winbase.h"
#include "wingdi.h"
#include "vfw.h"

SEGPTR frame16 = NULL;

LPVOID WINAPI AVIStreamGetFrame16(PGETFRAME pg, LONG lPos)
{
    if (frame16)
        UnMapLS(frame16);

    frame16 = MapLS(AVIStreamGetFrame(pg, lPos));
    return frame16;
}

LRESULT WINAPI AVIStreamGetFrameClose16(PGETFRAME pg)
{
    if (frame16)
    {
        UnMapLS(frame16);
        frame16 = NULL;
    }
    return AVIStreamGetFrameClose(pg);
}
