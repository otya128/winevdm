

#include "config.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
//wrong definition?
#define ExtDeviceMode WINAPI ExtDeviceMode
#include "winspool.h"
#undef ExtDeviceMode
#include "winuser.h"
#include "wownt32.h"
#include "wine/unicode.h"
#include "wine/winbase16.h"

#include "wine/debug.h"
/* Maximum length of a device name */
#define CCHDEVICENAME16 32

/* Maximum length of a paper name */
#define CCHPAPERNAME16  64
#pragma pack(push, 1)
WINE_DEFAULT_DEBUG_CHANNEL(winspool);
typedef struct {
    char  dmDeviceName[CCHDEVICENAME16];
    UINT16  dmSpecVersion;
    UINT16  dmDriverVersion;
    UINT16  dmSize;
    UINT16  dmDriverExtra;
    DWORD dmFields;
    INT16   dmOrientation;
    INT16   dmPaperSize;
    INT16   dmPaperLength;
    INT16   dmPaperWidth;
    INT16   dmScale;
    INT16   dmCopies;
    INT16   dmDefaultSource;
    INT16   dmPrintQuality;
    INT16   dmColor;
    INT16   dmDuplex;
    INT16   dmYResolution;
    INT16   dmTTOption;
} DEVMODE16, *LPDEVMODE16;
#pragma pack(pop)
//sizeof(DEVMODE16) != sizeof(DEVMODEA)
void DEVMODE16To32(CONST DEVMODE16 *src, LPDEVMODEA dst, LONG extra)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME - 1] = 0;
    dst->dmSpecVersion = 0x30a;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = sizeof(DEVMODEA);
    dst->dmDriverExtra = 0;
    dst->dmFields = src->dmFields & 0x7fbf;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
}
void DEVMODE32To16(LPDEVMODE16 dst, const LPDEVMODEA src, LONG extra)
{
    memcpy(dst->dmDeviceName, src->dmDeviceName, min(CCHDEVICENAME16, CCHDEVICENAME));
    dst->dmDeviceName[CCHDEVICENAME16 - 1] = 0;
    dst->dmSpecVersion = 0x30a;
    dst->dmDriverVersion = src->dmDriverVersion;
    dst->dmSize = sizeof(DEVMODE16);
    dst->dmDriverExtra = 0; 
    dst->dmFields = src->dmFields & 0x7fbf;
    dst->dmOrientation = src->dmOrientation;
    dst->dmPaperSize = src->dmPaperSize;
    dst->dmPaperLength = src->dmPaperLength;
    dst->dmPaperWidth = src->dmPaperWidth;
    dst->dmScale = src->dmScale;
    dst->dmCopies = src->dmCopies;
    dst->dmDefaultSource = src->dmDefaultSource;
    dst->dmPrintQuality = src->dmPrintQuality;
    dst->dmColor = src->dmColor;
    dst->dmDuplex = src->dmDuplex;
    dst->dmYResolution = src->dmYResolution;
    dst->dmTTOption = src->dmTTOption;
}
int WINAPI ExtDeviceMode16(HWND16 hwnd16, HANDLE16 hDriver16, LPDEVMODE16 pDevModeOutput, LPSTR pDeviceName, LPSTR pPort, LPDEVMODE16 pDevModeInput, LPSTR pProfile, WORD fMode)
{
    char tmp[256];
    if (pDeviceName && !strcmp(pDeviceName, "DefaultPrinter"))
    {
        int len = 256;
        if (GetDefaultPrinterA(tmp, &len))
            pDeviceName = tmp;
    }
    LONG size = ExtDeviceMode(HWND_32(hwnd16), (HANDLE)hDriver16, NULL, pDeviceName, pPort, NULL, pProfile, 0);
    if (!fMode)
        return sizeof(DEVMODE16);
    char *devmode32 = alloca(size);
    char *input32 = alloca(size);
    if (pDevModeInput)
        DEVMODE16To32(pDevModeInput, (LPDEVMODEA)&input32[0], size);
    LONG result = ExtDeviceMode(HWND_32(hwnd16), (HANDLE)hDriver16, pDevModeOutput ? (LPDEVMODEA)&devmode32[0] : NULL, pDeviceName, pPort, pDevModeInput ? (LPDEVMODEA)&input32[0] : NULL, pProfile, fMode);
    size = result;
    if (pDevModeOutput)
        DEVMODE32To16(pDevModeOutput, (LPDEVMODEA)&devmode32[0], size);
    return fMode == DM_UPDATE ? result : sizeof(DEVMODE16);
}
void WINAPI DeviceMode16(HWND16 hwnd, HMODULE16 hModule16, LPSTR lpszDevice, LPSTR lpszOutput)
{
    ExtDeviceMode16(hwnd, hModule16, NULL, lpszDevice, lpszOutput, NULL, NULL, DM_PROMPT|DM_UPDATE);
    return;
}
DWORD WINAPI DeviceCapabilities16(LPCSTR pDevice, LPCSTR pPort, WORD fwCapability, LPSTR pOutput, CONST DEVMODE16 *pDevMode)
{
    char devmode32[65536] = { 0 };
    if (pDevMode)
        DEVMODE16To32(pDevMode, (LPDEVMODEA)&devmode32[0], 0);
    DWORD result = 0;
    switch (fwCapability)
    {
    case DC_FIELDS:
    case DC_PAPERS:
    case DC_MINEXTENT:
    case DC_MAXEXTENT:
    case DC_BINS:
    case DC_DUPLEX:
    case DC_SIZE:
    case DC_VERSION:
    case DC_DRIVER:
    case DC_BINNAMES:
    case DC_ENUMRESOLUTIONS:
    case DC_FILEDEPENDENCIES:
    case DC_TRUETYPE:
    case DC_PAPERNAMES:
    case DC_ORIENTATION:
    case DC_COPIES:
        result = DeviceCapabilitiesA(pDevice, pPort, fwCapability, pOutput, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
        break;
    case DC_PAPERSIZE:
    {
        DWORD papers = DeviceCapabilitiesA(pDevice, pPort, DC_PAPERS, pOutput, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
        LPPOINT points32 = !pOutput ? NULL : (LPPOINT)HeapAlloc(GetProcessHeap(), 0, papers * sizeof(POINT));
        result = DeviceCapabilitiesA(pDevice, pPort, fwCapability, points32, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
        if (pOutput)
        {
            LPPOINT16 points16 = (LPPOINT16)pOutput;
            for (int i = 0; i < result; i++)
            {
                points16[i].x = points32[i].x;
                points16[i].y = points32[i].y;
            }
            HeapFree(GetProcessHeap(), 0, points32);
        }
        break;
    }
    case DC_EXTRA:
        result = DeviceCapabilitiesA(pDevice, pPort, fwCapability, pOutput, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
        if (result > 65535)
        {
            ERR("DC_EXTRA(%d) > 65535\n", result);
        }
        break;
    default:
    {
        ERR("unknown DeviceCapabilities %d\n", fwCapability);
        result = DeviceCapabilitiesA(pDevice, pPort, fwCapability, pOutput, pDevMode ? (LPDEVMODEA)&devmode32[0] : NULL);
        break;
    }
    }
    return result;
}
