/*
 * otvdm / winevdm: interface to the in-tree MCI sequencer.
 *
 * mmsystem's mci16.c normally hands every MCI message to the host winmm, which loads its
 * own sequencer driver. When the host has none (or it fails), 16 bit MIDI playback has no
 * sound path at all. mciseq_internal.c builds Wine's dlls/mciseq/mcimidi.c into this DLL to
 * provide that fallback; mci16.c calls the functions below for the sequencer device.
 */

#ifndef __WINE_MMSYSTEM_MCISEQ_INTERNAL_H
#define __WINE_MMSYSTEM_MCISEQ_INTERNAL_H

/* Does this MCI_OPEN (with already 16->32 mapped parameters) ask for a sequencer? */
BOOL OTVDM_MCISEQ_IsSequencerOpen(DWORD dwFlags, DWORD_PTR parms32);

/* Does wDevID belong to the in-tree sequencer rather than to a host winmm device? */
BOOL OTVDM_MCISEQ_IsInternalDevice(UINT wDevID);

/* [otvdm] midiseq=internal in otvdm.ini: try the in-tree sequencer first (testing aid). */
BOOL OTVDM_MCISEQ_Forced(void);

/* Open/close/dispatch. On success Open returns 0 and writes the new device id into the
 * mapped MCI_OPEN_PARMSW, exactly like mciSendCommandW does. */
DWORD OTVDM_MCISEQ_Open(DWORD dwFlags, DWORD_PTR parms32);
DWORD OTVDM_MCISEQ_Send(UINT wDevID, UINT wMsg, DWORD dwFlags, DWORD_PTR parms32);
BOOL OTVDM_MCISEQ_Close(UINT wDevID);

#endif /* __WINE_MMSYSTEM_MCISEQ_INTERNAL_H */
