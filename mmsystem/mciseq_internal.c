/*
 * otvdm / winevdm adaptation glue for an in-tree MCI sequencer (P2).
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
 * Its three MCI driver callbacks (mciDriverNotify / mciGetDriverData / mciSetDriverData, all
 * declared in Wine's mmddk.h) are resolved from the host winmm import library, which
 * mmsystem already links. That is correct for LINKING an internal driver but NOT yet correct
 * at run time: those host functions key off winmm's own device table, and an in-tree
 * sequencer would own its device IDs. Wiring the sequencer into mciSendCommand16 -- next
 * step -- must give it its own driver-data table instead of borrowing the host's. Until that
 * dispatch exists the code is linked in but never invoked, so behaviour is unchanged.
 *
 * mmsystem/CMakeLists.txt scopes one extra include directory (../wine/windows) to this file
 * only, because Wine's mmddk.h lives there and MinGW ships no mmddk.h.
 *
 * LICENCE: the included source is LGPL 2.1+; its original copyright header inside
 * mcimidi_wine.inc must be preserved, and the package documentation must note the origin.
 */

#include "mcimidi_wine.inc"
