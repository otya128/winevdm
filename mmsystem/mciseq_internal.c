/*
 * otvdm / winevdm: in-tree MCI sequencer (P2).
 *
 * WHY THIS FILE EXISTS
 * --------------------
 * Upstream Wine ships a complete MCI sequencer driver -- dlls/mciseq/mcimidi.c, 52,867 bytes,
 * LGPL 2.1+, Copyright 1994 Martin Ayotte / 1999 Eric Pouech. ReactOS ships the very same
 * file (identical git blob). It parses a Standard MIDI File itself (MThd/MTrk,
 * variable-length deltas, pulse -> millisecond conversion) and plays it with
 * midiOutShortMsg -- exactly the "SMF -> MIDI short message" fallback otvdm lacks today:
 * mmsystem's mci16.c forwards every MCI message to the host winmm (mciSendCommandW,
 * mci16.c:1012), so when the host has no working sequencer there is no sound.
 *
 * The upstream source is kept byte-identical as `mcimidi_wine.inc` (sha256
 * 30d8b748334a4938ae2b46477e470b000e31c9cde347c53f0bb3c49c07588d02) and is included here
 * rather than compiled on its own: the .inc extension keeps it out of the
 * `file(GLOB SOURCE *.c ...)` in mmsystem/CMakeLists.txt, so it is not also compiled as a
 * separate translation unit (which would duplicate every symbol).
 *
 * WHAT THE GLUE DOES
 * ------------------
 * Upstream expects the MCI system (winmm) to supply three hooks and to own the device table:
 *
 *   mciSetDriverData / mciGetDriverData   -- the driver's own state, keyed by device id
 *   mciDriverNotify                       -- post MM_MCINOTIFY to the app's window
 *
 * mmsystem exports 16 bit versions of all three, but their get/set pair just forwards to the
 * host winmm, whose table knows nothing about a device id that we invented. So the driver is
 * pointed at local implementations instead. The redirection is done with macros around the
 * include, NOT by defining the plain names: mci16.c must keep calling the host winmm for
 * host devices, and defining `mciGetDriverData` here would silently hijack those calls too.
 *
 * Device ids: the sequencer hands out ids from OTVDM_MCISEQ_ID_BASE upwards, well above
 * anything winmm allocates, so mciSendCommand16 can tell the two apart with a range check.
 *
 * Notifications: the driver calls mciDriverNotify(HWND_32(LOWORD(callback)), devID, status),
 * i.e. it already converted the app's 16 bit handle to a 32 bit one, so posting directly is
 * all that is left. MCI notifications carry the device id in lParam, and by then the app
 * knows the id we returned.
 *
 * mmsystem/CMakeLists.txt scopes one extra include directory (../wine/windows) to this file
 * only, because Wine's mmddk.h lives there and MinGW ships no mmddk.h.
 *
 * LICENCE: the included source is LGPL 2.1+; its original copyright header inside
 * mcimidi_wine.inc must be preserved, and the package documentation must note the origin.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "windef.h"
#include "winbase.h"
#include "winnls.h"
#include "wingdi.h"
#include "winuser.h"
#include "wownt32.h"
#include "mmsystem.h"
#include "mmddk.h"
#include "mciseq_internal.h"

/* Provided by krnl386.exe16 (same call mmsystem.c already makes for midiconfig). */
extern DWORD WINAPI krnl386_get_config_string(LPCSTR appname, LPCSTR keyname, LPCSTR def,
                                              LPSTR ret, DWORD size);

#define OTVDM_MCISEQ_SLOTS     8
#define OTVDM_MCISEQ_ID_BASE   0xF000

typedef struct
{
    UINT      id;       /* 0 when the slot is free */
    DWORD_PTR drvData;  /* what the driver stored via mciSetDriverData */
} OTVDM_MCISEQ_SLOT;

static OTVDM_MCISEQ_SLOT otvdm_mciseq_slots[OTVDM_MCISEQ_SLOTS];
static BOOL              otvdm_mciseq_loaded;

static OTVDM_MCISEQ_SLOT *slot_from_mci_id(UINT id)
{
    int i;

    if (id < OTVDM_MCISEQ_ID_BASE || id >= OTVDM_MCISEQ_ID_BASE + OTVDM_MCISEQ_SLOTS)
        return NULL;
    i = id - OTVDM_MCISEQ_ID_BASE;
    return otvdm_mciseq_slots[i].id == id ? &otvdm_mciseq_slots[i] : NULL;
}

static OTVDM_MCISEQ_SLOT *slot_alloc(void)
{
    int i;

    for (i = 0; i < OTVDM_MCISEQ_SLOTS; i++)
    {
        if (!otvdm_mciseq_slots[i].id)
        {
            otvdm_mciseq_slots[i].id      = OTVDM_MCISEQ_ID_BASE + i;
            otvdm_mciseq_slots[i].drvData = 0;
            return &otvdm_mciseq_slots[i];
        }
    }
    return NULL;
}

static void slot_free(OTVDM_MCISEQ_SLOT *slot)
{
    slot->id      = 0;
    slot->drvData = 0;
}

/* --- the three hooks the driver calls (renamed below, see the file header) --- */

DWORD_PTR WINAPI otvdm_mciGetDriverData(UINT uDeviceID)
{
    OTVDM_MCISEQ_SLOT *slot = slot_from_mci_id(uDeviceID);

    return slot ? slot->drvData : 0;
}

BOOL WINAPI otvdm_mciSetDriverData(UINT uDeviceID, DWORD_PTR dwData)
{
    OTVDM_MCISEQ_SLOT *slot = slot_from_mci_id(uDeviceID);

    if (!slot) return FALSE;
    slot->drvData = dwData;
    return TRUE;
}

BOOL WINAPI otvdm_mciDriverNotify(HWND hwndCallback, UINT uDeviceID, UINT uStatus)
{
    /* Same effect as mciDriverNotify16(), but the handle is already 32 bit here. */
    return PostMessageA(hwndCallback, MM_MCINOTIFY, uStatus, uDeviceID);
}

#define mciDriverNotify   otvdm_mciDriverNotify
#define mciGetDriverData  otvdm_mciGetDriverData
#define mciSetDriverData  otvdm_mciSetDriverData

#include "mcimidi_wine.inc"

/* --- helpers --- */

static BOOL wide_contains_ci(LPCWSTR hay, LPCWSTR needle)
{
    int i, j;

    if (!hay) return FALSE;
    for (i = 0; hay[i]; i++)
    {
        for (j = 0; needle[j]; j++)
        {
            WCHAR a = hay[i + j], b = needle[j];
            if (!a) break;
            if (a >= 'A' && a <= 'Z') a += 'a' - 'A';
            if (b >= 'A' && b <= 'Z') b += 'a' - 'A';
            if (a != b) break;
        }
        if (!needle[j]) return TRUE;
    }
    return FALSE;
}

static BOOL wide_ends_ci(LPCWSTR s, LPCWSTR suffix)
{
    int n = 0, m = 0, i;

    if (!s || !suffix) return FALSE;
    while (s[n]) n++;
    while (suffix[m]) m++;
    if (m == 0 || n < m) return FALSE;
    for (i = 0; i < m; i++)
    {
        WCHAR a = s[n - m + i], b = suffix[i];

        if (a >= 'A' && a <= 'Z') a += 'a' - 'A';
        if (b >= 'A' && b <= 'Z') b += 'a' - 'A';
        if (a != b) return FALSE;
    }
    return TRUE;
}

BOOL OTVDM_MCISEQ_Forced(void)
{
    char cfg[32];

    if (!krnl386_get_config_string("otvdm", "midiseq", "", cfg, sizeof(cfg)))
        return FALSE;
    return !_stricmp(cfg, "internal");
}

BOOL OTVDM_MCISEQ_IsSequencerOpen(DWORD dwFlags, DWORD_PTR parms32)
{
    LPMCI_OPEN_PARMSW parms = (LPMCI_OPEN_PARMSW)parms32;

    if (!parms) return FALSE;

    if ((dwFlags & (MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID)) == MCI_OPEN_TYPE_ID)
        return LOWORD((DWORD_PTR)parms->lpstrDeviceType) == MCI_DEVTYPE_SEQUENCER;

    if ((dwFlags & (MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID)) == MCI_OPEN_TYPE)
        return wide_contains_ci(parms->lpstrDeviceType, L"sequencer");

    /* When the caller names no device type, Windows derives it from the element's extension:
     * win.ini's [mci extensions] section maps "mid" to the sequencer, and that is how every
     * 16 bit player we could test actually opens a file - MPLAYER sends MCI_OPEN with
     * MCI_OPEN_ELEMENT|MCI_OPEN_ALIAS and nothing else (observed with WINEDEBUG=+mmsys under
     * otvdm: "mciSendCommand16 (0000, MCI_OPEN, 00000600, ...)"). Without this branch the
     * fallback would never engage on the most common path there is, so recognise the same
     * extensions here. Only the extension decides: the host is still asked first and, should it
     * fail, these files are exactly the ones the in-tree sequencer can read. The extension list
     * follows the environment's own win.ini: a Windows 3.1 install lists both "mid=sequencer"
     * and "rmi=sequencer" under [mci extensions], so .rmi has to be recognised here too or those
     * files would silently get no fallback.
     */
    if ((dwFlags & MCI_OPEN_ELEMENT) && parms->lpstrElementName)
        return wide_ends_ci(parms->lpstrElementName, L".mid") ||
               wide_ends_ci(parms->lpstrElementName, L".midi") ||
               wide_ends_ci(parms->lpstrElementName, L".rmi");

    return FALSE;
}

BOOL OTVDM_MCISEQ_IsInternalDevice(UINT wDevID)
{
    return slot_from_mci_id(wDevID) != NULL;
}

/* --- entry points used by mci16.c --- */

/* Placed after the include of mcimidi_wine.inc on purpose: WARN/TRACE only become macros once
 * that file has pulled in Wine's debug header, and calling WARN above it compiles to a reference
 * to an undefined function named WARN. */

/* winmm owns the last step of an MCI command, not the driver. When a driver returns
 * MCI_RESOURCE_RETURNED it has also packed dwReturn with MAKEMCIRESOURCE(api_value, resource_id)
 * -- MAKELRESULT puts the value the caller wants in the low word -- and winmm hands the caller 0
 * after translating it. We call the driver directly, so nobody does that for us and the app sees
 * the bare 0x00010000 as an MCI error code.
 *
 * That is not hypothetical: with midiseq=internal the Win16 MPLAYER ran MCI_GETDEVCAPS twice,
 * received 0x00010000 both times (traced in the forced-internal run), concluded the device could
 * not play, and stopped issuing commands - the device itself had opened perfectly. Every
 * MCI_RESOURCE_RETURNED site in mcimidi_wine.inc belongs to MIDI_mciStatus (L1345-1410) and
 * MIDI_mciGetDevCaps (L1447-1487), so both messages need their value rewritten; a run of this
 * code with only MCI_GETDEVCAPS handled logged exactly one such warning, for MCI_STATUS_MODE.
 * Status items that return a plain number (LENGTH, POSITION) leave the flag clear and are
 * therefore untouched. MCI_RESOURCE_DRIVER is not used by this driver at all. */
static DWORD otvdm_mciseq_unwrap(DWORD r, UINT wMsg, DWORD_PTR parms32)
{
    if (r == MCI_RESOURCE_RETURNED)
    {
        if (wMsg == MCI_GETDEVCAPS && parms32)
        {
            LPMCI_GETDEVCAPS_PARMS parms = (LPMCI_GETDEVCAPS_PARMS)parms32;

            parms->dwReturn = LOWORD(parms->dwReturn);
        }
        else if (wMsg == MCI_STATUS && parms32)
        {
            LPMCI_STATUS_PARMS parms = (LPMCI_STATUS_PARMS)parms32;

            parms->dwReturn = LOWORD((DWORD)parms->dwReturn);
        }
        else
            WARN("sequencer returned a resource for message %04x: not translated\n", wMsg);
        return 0;
    }
    return r;
}

DWORD OTVDM_MCISEQ_Open(DWORD dwFlags, DWORD_PTR parms32)
{
    OTVDM_MCISEQ_SLOT *slot = slot_alloc();
    MCI_OPEN_DRIVER_PARMSW modp;
    DWORD r;

    if (!slot) return MCIERR_OUT_OF_MEMORY;

    if (!otvdm_mciseq_loaded)
    {
        MCIMIDI_DriverProc(0, 0, DRV_LOAD, 0, 0);
        otvdm_mciseq_loaded = TRUE;
    }

    memset(&modp, 0, sizeof(modp));
    modp.wDeviceID = slot->id;
    r = MCIMIDI_DriverProc(0, 0, DRV_OPEN, (LPARAM)L"mciseq", (LPARAM)&modp);
    if (!r)
    {
        WARN("sequencer DRV_OPEN failed\n");
        slot_free(slot);
        return MCIERR_DRIVER_INTERNAL;
    }

    r = MCIMIDI_DriverProc(slot->id, 0, MCI_OPEN_DRIVER, dwFlags, parms32);
    if (r)
    {
        TRACE("sequencer MCI_OPEN_DRIVER failed (%lu)\n", r);
        MCIMIDI_DriverProc(slot->id, 0, DRV_CLOSE, 0, 0);
        slot_free(slot);
        return r;
    }

    /* hand our device id back to the app, exactly as mciSendCommandW would */
    ((LPMCI_OPEN_PARMSW)parms32)->wDeviceID = slot->id;

    /* The driver's own default port is MIDI_MAPPER, stored in a 16 bit field, so it becomes
     * 0xFFFF - an id that no MIDI output unit has. MIDI_player's midiOutOpen then fails with
     * MMSYSERR_BADDEVICEID, mmr2mci() translates that into MCIERR_SEQ_PORT_NONEXISTENT, and the
     * player returns at once, leaving the device reporting MCI_MODE_PLAY forever without ever
     * sounding. The mapper itself is not the problem: midiOutOpen(MIDI_MAPPER) still succeeds on
     * a current machine (measured); it is simply not a real MIDI out unit id.
     * Measured on a current machine through this very code: default port -> MCI_PLAY with
     * MCI_WAIT returns 338 after 0 ms; port 0 -> returns 0 after 2328 ms for a 2 s file,
     * i.e. it really plays. So point the driver at the first real MIDI out unit, which is
     * what "the system default interface" means here.
     */
    if (midiOutGetNumDevs() > 0)
    {
        MCI_SEQ_SET_PARMS setp;

        memset(&setp, 0, sizeof(setp));
        setp.dwPort = 0;
        MCIMIDI_DriverProc(slot->id, 0, MCI_SET, MCI_SEQ_SET_PORT, (LPARAM)&setp);
    }
    else
        WARN("no host MIDI out device available: playback will not sound\n");

    TRACE("sequencer opened as device %u\n", slot->id);
    return 0;
}

DWORD OTVDM_MCISEQ_Send(UINT wDevID, UINT wMsg, DWORD dwFlags, DWORD_PTR parms32)
{
    /* The driver knows MCI_OPEN_DRIVER/MCI_CLOSE_DRIVER only; the plain MCI_OPEN/MCI_CLOSE
     * in mciSendCommand16 go to the host. Translate on the way in. */
    if (wMsg == MCI_CLOSE) wMsg = MCI_CLOSE_DRIVER;

    return otvdm_mciseq_unwrap(MCIMIDI_DriverProc(wDevID, 0, wMsg, dwFlags, parms32), wMsg, parms32);
}

BOOL OTVDM_MCISEQ_Close(UINT wDevID)
{
    OTVDM_MCISEQ_SLOT *slot = slot_from_mci_id(wDevID);

    if (!slot) return FALSE;

    OTVDM_MCISEQ_Send(wDevID, MCI_CLOSE_DRIVER, 0, 0);
    MCIMIDI_DriverProc(wDevID, 0, DRV_CLOSE, 0, 0);
    slot_free(slot);
    TRACE("sequencer device %u closed\n", wDevID);
    return TRUE;
}
