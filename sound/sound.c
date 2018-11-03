/*
 * 16-bit sound support
 *
 *  Copyright  Robert J. Amstadt, 1993
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "windef.h"
#include "winbase.h"
#include "wine/winbase16.h"
#include "wine/windef16.h"
#include "wine/debug.h"
#include "mmsystem.h"

WINE_DEFAULT_DEBUG_CHANNEL(sound);

#define S_SERDVNA (-1)
#define S_SERMACT (-4)
#define S_SERQFUL (-4)
#define S_SERDNT  (-5)
#define S_SERDTP  (-8)
#define S_SERDFQ (-13)
#define S_QUEUEEMPTY 0

typedef struct {
  UINT16 freq;
  INT16 duration;
} NOTE;

static NOTE *queue;
static int queuelen;
static HTASK owner = 0;
static BOOL playing;
static int nextnote;
static HANDLE thread;
static LPHWAVEOUT wohand;
static HANDLE event;
static int pitch;
static int tempo;

INT16 WINAPI StopSound16(void);

/***********************************************************************
 *		OpenSound (SOUND.1)
 */
INT16 WINAPI OpenSound16(void)
{

  if (owner) return S_SERDVNA;
  owner = GetCurrentTask();

  const WAVEFORMATEX wfmt = { WAVE_FORMAT_PCM, 1, 48000, 48000, 1, 8, 0 };
  event = CreateEventA(NULL, FALSE, FALSE, NULL);

  if (waveOutOpen(&wohand, WAVE_MAPPER, &wfmt, event, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR) return S_SERDVNA;
  queue = (NOTE *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 32 * sizeof(NOTE));
  queuelen = 32 * sizeof(NOTE);
  playing = 0;
  nextnote = 0;
  pitch = 0;
  tempo = 120;
  return 1;
}

/***********************************************************************
 *		CloseSound (SOUND.2)
 */
void WINAPI CloseSound16(void)
{
  if (owner != GetCurrentTask()) return;
  StopSound16();
  HeapFree(GetProcessHeap(), 0, (LPVOID)queue);
  waveOutClose(wohand);
  CloseHandle(event);
  playing = 0;
  owner = 0;
}

/***********************************************************************
 *		SetVoiceQueueSize (SOUND.3)
 */
INT16 WINAPI SetVoiceQueueSize16(INT16 nVoice, INT16 nBytes)
{
  if (owner != GetCurrentTask()) return 0;
  if (nVoice != 1) return 0;
  if (playing) return S_SERMACT;
  HeapFree(GetProcessHeap(), 0, (LPVOID)queue);
  queue = (NOTE *)HeapAlloc(GetProcessHeap(), 0, nBytes); 
  queuelen = nBytes;
  nextnote = 0;
  return 0;
}

/***********************************************************************
 *		SetVoiceAccent (SOUND.5)
 */
INT16 WINAPI SetVoiceAccent16(INT16 nVoice, INT16 nTempo, INT16 nVolume,
                              INT16 nMode, INT16 nPitch)
{
  // nVolume is ignored and some programs set it to 0
  if (owner != GetCurrentTask()) return 0;
  if (nVoice != 1) return 0;
  TRACE("(%d,%d,%d,%d,%d):\n", nVoice, nTempo, nVolume, nMode, nPitch);
  if ((nTempo < 32) || (nTempo > 255)) return S_SERDTP;
  if (nPitch > 83) return -12;
  tempo = nTempo;
  pitch = nPitch;
  return 0;
}

/***********************************************************************
 *		SetVoiceEnvelope (SOUND.6)
 */
INT16 WINAPI SetVoiceEnvelope16(INT16 nVoice, INT16 nShape, INT16 nRepeat)
{
  FIXME("(%d,%d,%d): stub\n",nVoice,nShape,nRepeat);
  return 0;
}

/***********************************************************************
 *		SetSoundNoise (SOUND.7)
 */
INT16 WINAPI SetSoundNoise16(INT16 nSource, INT16 nDuration)
{
  FIXME("(%d,%d): stub\n",nSource,nDuration);
  return 0;
}

/***********************************************************************
 *		SetVoiceSound (SOUND.8)
 */
INT16 WINAPI SetVoiceSound16(INT16 nVoice, DWORD lFrequency, INT16 nDuration)
{
  if (owner != GetCurrentTask()) return 0;
  if (nVoice != 1) return 0;
  if (playing) return 0;
  if (nextnote >= (queuelen / sizeof(NOTE))) return S_SERQFUL;
  int freq = HIWORD(lFrequency);
  if ((freq < 37) || (freq > 32767)) return S_SERDFQ;

  TRACE("freq: %d duration: %d\n", freq, nDuration);
  queue[nextnote].freq = freq;
  queue[nextnote].duration = ((nDuration + 1) / 2) * 5; // 2.5 ms per tick rounded up
  nextnote++;
  return 0;
}

/***********************************************************************
 *		SetVoiceNote (SOUND.4)
 */
INT16 WINAPI SetVoiceNote16(INT16 nVoice, INT16 nValue, INT16 nLength,
                            INT16 nCdots)
{
  if (--nValue > 83) return S_SERDNT;
  TRACE("(%d,%d,%d,%d)\n",nVoice,nValue,nLength,nCdots);
  const int notes[] = { 4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902 };
  nValue = (nValue + pitch) % 84;
  int freq = notes[nValue % 12] >> (6 - (nValue / 12));
  int len = (nCdots ? ((nCdots * 3) / 2) : 1) * ((96000 / tempo) / nLength);
  if (nCdots != 0 && len > 32767)
  {
      len = (96000 / tempo) / nLength;
  }
  return SetVoiceSound16(nVoice, freq << 16, len);
}

static DWORD WINAPI play(LPVOID param)
{
  int len = 0;
  const float mspersamp = 1000.0f / 48000.0f;
  for (int i = 0; i < nextnote; i++)
    len += (float)queue[i].duration / mspersamp;
  if (!len) return;
 
  int buflen = len;
  char *wavbuf = (char *)HeapAlloc(GetProcessHeap(), 0, buflen);
  int k = 0;

  // sound.drv that came with Windows 2-3 only supported the pc speaker
  // make square waves to simulate it
  for (int i = 0; i < nextnote; i++)
  {
    int notelen = (float)queue[i].duration / mspersamp;
    int hwavelen = max(1, (int)((1000.0f / ((float)queue[i].freq * mspersamp)) / 2));
    char samp = 0xff;
    for (int j = 0; j < notelen; j++)
    {
      wavbuf[k + j] = samp;
      if (!(j % hwavelen)) samp = ~samp;
    }
    k += notelen;
  }

  WAVEHDR whdr = { wavbuf, buflen, 0, 0, 0, 0, NULL, NULL };
  waveOutPrepareHeader(wohand, &whdr, sizeof(whdr));
  waveOutWrite(wohand, &whdr, sizeof(whdr));
  do
    WaitForSingleObject(event, INFINITE);
  while (!(whdr.dwFlags & WHDR_DONE));
  waveOutUnprepareHeader(wohand, &whdr, sizeof(whdr));
  HeapFree(GetProcessHeap(), 0, wavbuf);

  nextnote = 0;
  playing = 0;
  ExitThread(0);
}

/***********************************************************************
 *		StartSound (SOUND.9)
 */
INT16 WINAPI StartSound16(void)
{
  if (owner != GetCurrentTask()) return 0;
  if (playing) return 0;
  playing = 1;
  thread = CreateThread(NULL, 0, play, 0, 0, NULL);
  return 0;
}

/***********************************************************************
 *		WaitSoundState (SOUND.11)
 */
INT16 WINAPI WaitSoundState16(INT16 x)
{
  if (owner != GetCurrentTask()) return 0;
  if (!playing) return 0;
  if (thread)
  {
    BOOL exitcode;
    GetExitCodeThread(thread, &exitcode);
    if (exitcode == STILL_ACTIVE)
    {
      DWORD count;
      ReleaseThunkLock(&count);
      WaitForSingleObject(thread, INFINITE);
      RestoreThunkLock(count);
    }
  }
  return 0;
}

/***********************************************************************
 *		StopSound (SOUND.10)
 */
INT16 WINAPI StopSound16(void)
{
  if (owner != GetCurrentTask()) return 0;
  if (!playing) return 0;
  waveOutReset(wohand);
  return 0;
}

/***********************************************************************
 *		SyncAllVoices (SOUND.12)
 */
INT16 WINAPI SyncAllVoices16(void)
{
    FIXME("(void): stub\n");
    return 0;
}

/***********************************************************************
 *		CountVoiceNotes (SOUND.13)
 */
INT16 WINAPI CountVoiceNotes16(INT16 x)
{
    FIXME("(%d): stub\n", x);
    return 0;
}

/***********************************************************************
 *		GetThresholdEvent (SOUND.14)
 */
LPINT16 WINAPI GetThresholdEvent16(void)
{
    FIXME("(void): stub\n");
    return NULL;
}

/***********************************************************************
 *		GetThresholdStatus (SOUND.15)
 */
INT16 WINAPI GetThresholdStatus16(void)
{
    FIXME("(void): stub\n");
    return 0;
}

/***********************************************************************
 *		SetVoiceThreshold (SOUND.16)
 */
INT16 WINAPI SetVoiceThreshold16(INT16 a, INT16 b)
{
    FIXME("(%d,%d): stub\n", a, b);
    return 0;
}

/***********************************************************************
 *		DoBeep (SOUND.17)
 */
void WINAPI DoBeep16(void)
{
    FIXME("(void): stub!\n");
}
