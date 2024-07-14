/*
 * DEC 93 Erik Bos <erik@xs4all.nl>
 *
 * Copyright 1996 Marcus Meissner
 *
 * Copyright 2001 Mike McCormack
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
 *
 * History:
 *
 * Mar 31, 1999. Ove Kåven <ovek@arcticnet.no>
 * - Implemented buffers and EnableCommNotification.
 *
 * Apr 3, 1999.  Lawson Whitney <lawson_whitney@juno.com>
 * - Fixed the modem control part of EscapeCommFunction16.
 *
 * Mar 3, 1999. Ove Kåven <ovek@arcticnet.no>
 * - Use port indices instead of unixfds for win16
 * - Moved things around (separated win16 and win32 routines)
 * - Added some hints on how to implement buffers and EnableCommNotification.
 *
 * May 26, 1997.  Fixes and comments by Rick Richardson <rick@dgii.com> [RER]
 * - ptr->fd wasn't getting cleared on close.
 * - GetCommEventMask() and GetCommError() didn't do much of anything.
 *   IMHO, they are still wrong, but they at least implement the RXCHAR
 *   event and return I/O queue sizes, which makes the app I'm interested
 *   in (analog devices EZKIT DSP development system) work.
 *
 * August 12, 1997.  Take a bash at SetCommEventMask - Lawson Whitney
 *                                     <lawson_whitney@juno.com>
 * July 6, 1998. Fixes and comments by Valentijn Sessink
 *                                     <vsessink@ic.uva.nl> [V]
 * Oktober 98, Rein Klazes [RHK]
 * A program that wants to monitor the modem status line (RLSD/DCD) may
 * poll the modem status register in the commMask structure. I update the bit
 * in GetCommError, waiting for an implementation of communication events.
 *
 */

#include "config.h"
#include "wine/port.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "windef.h"
#include "winbase.h"
#include "wine/winuser16.h"
#include "user_private.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(comm);

/* window's semi documented modem status register */
#define COMM_MSR_OFFSET  35
#define MSR_CTS  0x10
#define MSR_DSR  0x20
#define MSR_RI   0x40
#define MSR_RLSD 0x80
#define MSR_MASK (MSR_CTS|MSR_DSR|MSR_RI|MSR_RLSD)

#define FLAG_LPT 0x80

#define MAX_PORTS   9

struct DosDeviceStruct {
    HANDLE handle;
    BOOL unget;
    int ungetchar;
    int evtchar;
    /* events */
    int commerror, eventmask;
    /* buffers */
    char *outbuf;
    unsigned obuf_size,obuf_head,obuf_tail;
    /* notifications */
    HWND wnd;
    int n_read, n_write;
    /* save terminal states */
    DCB16 dcb;
    /* pointer to unknown(==undocumented) comm structure */
    SEGPTR seg_unknown;
    BYTE unknown[40];
    HANDLE eventth;
    HANDLE writeth;
    HANDLE writeev;
    CRITICAL_SECTION writelock;
    BOOL exit;
};

static struct DosDeviceStruct COM[MAX_PORTS];
static struct DosDeviceStruct LPT[MAX_PORTS];

static struct DosDeviceStruct *GetDeviceStruct(int index)
{
	if ((index&0x7F)<=MAX_PORTS) {
            if (!(index&FLAG_LPT)) {
		if (COM[index].handle)
		    return &COM[index];
	    } else {
		index &= 0x7f;
		if (LPT[index].handle)
		    return &LPT[index];
	    }
	}

	return NULL;
}

static int WinError(void)
{
        TRACE("errno = %d\n", errno);
	switch (errno) {
		default:
			return CE_IOE;
		}
}

static DWORD WINAPI eventth(LPVOID cid)
{
	struct DosDeviceStruct *ptr = GetDeviceStruct(cid);
	DWORD evtmask = EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_PERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;
	DWORD temperror, mstat = 0;
	COMSTAT stat;
	DWORD count = -1;
	OVERLAPPED ov;
	int ret = 0;

	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEventW(NULL, 0, 0, NULL);
	if(ov.hEvent == INVALID_HANDLE_VALUE)
		return -1;
	
	ClearCommError(ptr->handle, &temperror, &stat);
	SetCommMask(ptr->handle, evtmask);

	while (ptr->handle)
	{
		DWORD ret;
		if (!WaitCommEvent(ptr->handle, &evtmask, &ov))
		{
			DWORD ret = 0, error = GetLastError();
			if(error == ERROR_IO_PENDING)
			{
				if (!(ret = GetOverlappedResult(ptr->handle, &ov, &count, TRUE)))
					error = GetLastError();
			}
			if (!ret && (error == ERROR_OPERATION_ABORTED))
				break;
		}
		if (ptr->exit)
			break;

		if(GetCommModemStatus(ptr->handle, &mstat))
			*((WORD *)(ptr->unknown + COMM_MSR_OFFSET)) = mstat;

		*(WORD*)(ptr->unknown) |= evtmask;
		if ((evtmask & ptr->eventmask) && ptr->wnd)
			PostMessageA(ptr->wnd, WM_COMMNOTIFY, cid, CN_EVENT);

		if (!ClearCommError(ptr->handle, &temperror, &stat))
		{
			ret = -1;
			break;
		}
		
		// send cn_receive unconditionally since win31 has a 100ms timeout after
		// which it sends the notification even if the count is less than cbWriteNotify
		if ((ptr->n_read != -1) && ptr->n_read && stat.cbInQue && ptr->wnd)
			PostMessageA(ptr->wnd, WM_COMMNOTIFY, cid, CN_RECEIVE);
			
		if ((ptr->n_write != -1) && (ptr->n_write > stat.cbOutQue) && ptr->wnd)
			PostMessageA(ptr->wnd, WM_COMMNOTIFY, cid, CN_TRANSMIT);
	}
	CloseHandle(ov.hEvent);
	return ret;
}

static DWORD WINAPI writeth(LPVOID cid)
{
	struct DosDeviceStruct *ptr = GetDeviceStruct(cid);
	OVERLAPPED write_ov = {0};
	write_ov.hEvent = CreateEventW(NULL, 0, TRUE, NULL);
	HANDLE handles[] = {ptr->writeev, write_ov.hEvent};

	while (ptr->handle)
	{
		DWORD ret;
		ret = WaitForSingleObjectEx(ptr->writeev, INFINITE, TRUE);
		if ((ret == WAIT_ABANDONED) || ptr->exit)
			break;
		if (ret == WAIT_OBJECT_0)
		{
			EnterCriticalSection(&ptr->writelock);
			int len, bleft = ((ptr->obuf_tail <= ptr->obuf_head) ? ptr->obuf_head : ptr->obuf_size) - ptr->obuf_tail;
			while (bleft)
			{
				if (!WriteFile(ptr->handle, ptr->outbuf + ptr->obuf_tail, bleft, &len, &write_ov))
				{
					DWORD ret = 0, error = GetLastError();
					if(error == ERROR_IO_PENDING)
					{
						LeaveCriticalSection(&ptr->writelock);
						while ((ret = WaitForMultipleObjects(2, handles, FALSE, INFINITE)) == WAIT_OBJECT_0)
						{
							if (ptr->exit)
							{
								CancelIo(ptr->handle);
								CloseHandle(write_ov.hEvent);
								return 0;
							}
						}
						EnterCriticalSection(&ptr->writelock);
						if (ret == (WAIT_OBJECT_0 + 1))
						{
							
							if (!(ret = GetOverlappedResult(ptr->handle, &write_ov, &len, TRUE)))
								error = GetLastError();
						}
						else if (ret == -1)
						{
							error = GetLastError();
							ret = 0;
						}
						else
							ret = error;
					}
					if (!ret)
					{
						ERR("async write failed, error %d\n", error);
						ptr->commerror = CE_RXOVER;
						CancelIo(ptr->handle);
						break;
					}
				}
				TRACE("async write completed %d bytes\n",len);

				/* update the buffer pointers */
				int prev = ((ptr->obuf_tail > ptr->obuf_head) ? ptr->obuf_size : 0) + ptr->obuf_head - ptr->obuf_tail;
				ptr->obuf_tail += len;
				if (ptr->obuf_tail >= ptr->obuf_size)
					ptr->obuf_tail = 0;

				/* write from output queue */
				bleft = ((ptr->obuf_tail <= ptr->obuf_head) ? ptr->obuf_head : ptr->obuf_size) - ptr->obuf_tail;
			}
			LeaveCriticalSection(&ptr->writelock);
		}
	}
	CloseHandle(write_ov.hEvent);
	return 0;
}

/*****************************************************************************
 *	COMM16_DCBtoDCB16	(Internal)
 */
static INT16 COMM16_DCBtoDCB16(const DCB *lpdcb, LPDCB16 lpdcb16)
{
	if(lpdcb->BaudRate<0x10000)
		lpdcb16->BaudRate = lpdcb->BaudRate;
	else if(lpdcb->BaudRate==115200)
			lpdcb16->BaudRate = 57601;
	else {
		WARN("Baud rate can't be converted\n");
		lpdcb16->BaudRate = 57601;
	}
	lpdcb16->ByteSize = lpdcb->ByteSize;
	lpdcb16->fParity = lpdcb->fParity;
	lpdcb16->Parity = lpdcb->Parity;
	lpdcb16->StopBits = lpdcb->StopBits;

	lpdcb16->RlsTimeout = 50;
	lpdcb16->CtsTimeout = 50;
	lpdcb16->DsrTimeout = 50;
	lpdcb16->fNull = 0;
	lpdcb16->fChEvt = 0;
	lpdcb16->fBinary = 1;

	lpdcb16->fDtrflow = (lpdcb->fDtrControl==DTR_CONTROL_HANDSHAKE);
	lpdcb16->fRtsflow = (lpdcb->fRtsControl==RTS_CONTROL_HANDSHAKE);
	lpdcb16->fOutxCtsFlow = lpdcb->fOutxCtsFlow;
	lpdcb16->fOutxDsrFlow = lpdcb->fOutxDsrFlow;
	lpdcb16->fDtrDisable = (lpdcb->fDtrControl==DTR_CONTROL_DISABLE);
	lpdcb16->fRtsDisable = (lpdcb->fDtrControl==RTS_CONTROL_DISABLE);

	lpdcb16->fInX = lpdcb->fInX;

	lpdcb16->fOutX = lpdcb->fOutX;
/*
	lpdcb16->XonChar =
	lpdcb16->XoffChar =
 */
	lpdcb16->XonLim = 10;
	lpdcb16->XoffLim = 10;

	return 0;
}


/**************************************************************************
 *         BuildCommDCB		(USER.213)
 *
 * According to the ECMA-234 (368.3) the function will return FALSE on
 * success, otherwise it will return -1.
 */
INT16 WINAPI BuildCommDCB16(LPCSTR device, LPDCB16 lpdcb)
{
	/* "COM1:96,n,8,1"	*/
	/*  012345		*/
	int port;
	DCB dcb;

	TRACE("(%s), ptr %p\n", device, lpdcb);

	if (strncasecmp(device,"COM",3))
		return -1;
	port = device[3] - '0';

	if (port-- == 0) {
		ERR("BUG ! COM0 can't exist!\n");
		return -1;
	}
	device += 4;
	do
	{
		if (!*device)
			return -1;
		device++;
	}
	while (!isalnum(*device));

	lpdcb->Id = port;
	dcb.DCBlength = sizeof(DCB);

	if (strchr(device,'=')) /* block new style */
		return -1;

	if(!BuildCommDCBA(device,&dcb))
		return -1;

	return COMM16_DCBtoDCB16(&dcb, lpdcb);
}

/*****************************************************************************
 *	OpenComm		(USER.200)
 */
INT16 WINAPI OpenComm16(LPCSTR device,UINT16 cbInQueue,UINT16 cbOutQueue)
{
	int port;
	HANDLE handle;

	TRACE("%s, %d, %d\n", device, cbInQueue, cbOutQueue);

	if (strlen(device) < 4)
	   return IE_BADID;

	port = device[3] - '0';

	if (port-- == 0)
		ERR("BUG ! COM0 or LPT0 don't exist !\n");

	if (!strncasecmp(device,"COM",3))
	{
		if (COM[port].handle)
			return IE_OPEN;

		handle = CreateFileA(device, GENERIC_READ|GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0 );
		if (handle == INVALID_HANDLE_VALUE) {
			ERR("Open %s failed Err %d\n", device, GetLastError());
			return IE_HARDWARE;
		} else {
			memset(COM[port].unknown, 0, sizeof(COM[port].unknown));
			COM[port].seg_unknown = 0;
			COM[port].handle = handle;
			COM[port].commerror = 0;
			COM[port].eventmask = 0;
			COM[port].evtchar = 0; /* FIXME: default? */
			/* save terminal state */
			GetCommState16(port,&COM[port].dcb);
			/* init priority characters */
			COM[port].unget = FALSE;
			/* allocate buffers */
			COM[port].obuf_size = cbOutQueue;
			COM[port].obuf_head = COM[port].obuf_tail = 0;

			COM[port].outbuf = HeapAlloc( GetProcessHeap(), 0, cbOutQueue);
			if(!COM[port].outbuf || !SetupComm(handle, cbInQueue, cbOutQueue))
			{
				CloseHandle(COM[port].handle);
				return IE_MEMORY;
			}

			COMMTIMEOUTS cto = { MAXDWORD, 0, 0, 0, 1000 };
			SetCommTimeouts(handle, &cto);


			COM[port].exit = FALSE;
			COM[port].writeev = CreateEventW(NULL, 0, 0, NULL);
			InitializeCriticalSection(&COM[port].writelock);
			COM[port].eventth = CreateThread(NULL, 0, eventth, (LPVOID)port, 0, NULL);
			COM[port].writeth = CreateThread(NULL, 0, writeth, (LPVOID)port, 0, NULL);
			return port;
		}
	}
	else
	if (!strncasecmp(device,"LPT",3)) {

		if (LPT[port].handle)
			return IE_OPEN;

		handle = CreateFileA(device, GENERIC_READ|GENERIC_WRITE,
			0, NULL, OPEN_EXISTING, 0, 0 );
		if (handle == INVALID_HANDLE_VALUE) {
			return IE_HARDWARE;
		} else {
			LPT[port].handle = handle;
			LPT[port].commerror = 0;
			LPT[port].eventmask = 0;
			return port|FLAG_LPT;
		}
	}
	return IE_BADID;
}

/*****************************************************************************
 *	CloseComm		(USER.207)
 */
INT16 WINAPI CloseComm16(INT16 cid)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid=%d\n", cid);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no cid=%d found!\n", cid);
		return -1;
	}
	if (!(cid&FLAG_LPT)) {
		DWORD count;
		/* COM port */
		UnMapLS( COM[cid].seg_unknown );
		/* reset modem lines */
		SetCommState16(&COM[cid].dcb);
		HeapFree(GetProcessHeap(), 0, ptr->outbuf);
		ptr->exit = TRUE;
		ptr->obuf_tail = ptr->obuf_head;
		SetCommMask(ptr->handle, 0);
		PurgeComm(ptr->handle, PURGE_TXABORT);
		SetEvent(ptr->writeev);
		WaitForSingleObject(ptr->eventth, 1000);
		WaitForSingleObject(ptr->writeth, 1000);
		CloseHandle(ptr->writeev);
		DeleteCriticalSection(&ptr->writelock);
	}

	if (!CloseHandle(ptr->handle)) {
		ptr->commerror = WinError();
		/* FIXME: should we clear ptr->handle here? */
		return -1;
	} else {
		ptr->commerror = 0;
		ptr->handle = 0;
		return 0;
	}
}

/*****************************************************************************
 *	SetCommBreak		(USER.210)
 */
INT16 WINAPI SetCommBreak16(INT16 cid)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid=%d\n", cid);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no cid=%d found!\n", cid);
		return -1;
	}

	return !SetCommBreak(ptr->handle);
}

/*****************************************************************************
 *	ClearCommBreak	(USER.211)
 */
INT16 WINAPI ClearCommBreak16(INT16 cid)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid=%d\n", cid);
	if (!(ptr = GetDeviceStruct(cid))) {
		FIXME("no cid=%d found!\n", cid);
		return -1;
	}

	return !ClearCommBreak(ptr->handle);
}

/*****************************************************************************
 *	EscapeCommFunction	(USER.214)
 */
LONG WINAPI EscapeCommFunction16(UINT16 cid,UINT16 nFunction)
{
	struct  DosDeviceStruct *ptr;

	TRACE("cid=%d, function=%d\n", cid, nFunction);

	switch(nFunction) {
	case GETMAXCOM:
	        TRACE("GETMAXCOM\n");
                return 4;  /* FIXME */

	case GETMAXLPT:
		TRACE("GETMAXLPT\n");
                return FLAG_LPT + 3;  /* FIXME */

	case GETBASEIRQ:
		TRACE("GETBASEIRQ\n");
		/* FIXME: use tables */
		/* just fake something for now */
		if (cid & FLAG_LPT) {
			/* LPT1: irq 7, LPT2: irq 5 */
			return (cid & 0x7f) ? 5 : 7;
		} else {
			/* COM1: irq 4, COM2: irq 3,
			   COM3: irq 4, COM4: irq 3 */
			return 4 - (cid & 1);
		}
	}

	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no cid=%d found!\n", cid);
		return -1;
	}

	switch (nFunction) {
	case RESETDEV:
	case CLRDTR:
	case CLRRTS:
	case SETDTR:
	case SETRTS:
	case SETXOFF:
	case SETXON:
		if(EscapeCommFunction(ptr->handle,nFunction))
			return 0;
		else {
			ptr->commerror = WinError();
			return -1;
		}

	case CLRBREAK:
	case SETBREAK:
	default:
		WARN("(cid=%d,nFunction=%d): Unknown function\n",
			cid, nFunction);
	}
	return -1;
}

/*****************************************************************************
 *	FlushComm	(USER.215)
 */
INT16 WINAPI FlushComm16(INT16 cid,INT16 fnQueue)
{
	DWORD queue;
	struct DosDeviceStruct *ptr;

	TRACE("cid=%d, queue=%d\n", cid, fnQueue);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no cid=%d found!\n", cid);
		return -1;
	}
	switch (fnQueue) {
	case 0:
		queue = PURGE_TXABORT;
		ptr->obuf_tail = ptr->obuf_head;
		break;
	case 1:
		queue = PURGE_RXABORT;
		break;
	default:
		WARN("(cid=%d,fnQueue=%d):Unknown queue\n",
		            cid, fnQueue);
		return -1;
	}

	if (!PurgeComm(ptr->handle,queue)) {
		ptr->commerror = WinError();
		return -1;
	} else {
		ptr->commerror = 0;
		return 0;
	}
}

/********************************************************************
 *	GetCommError	(USER.203)
 */
INT16 WINAPI GetCommError16(INT16 cid,LPCOMSTAT16 lpStat)
{
	DWORD temperror;
	struct DosDeviceStruct *ptr;

	if ((UINT16)cid >= 256)
		return 0;

	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		/* Some programs frequently call GetCommError(0, ) */
		/* FIXME("no handle for cid = %0x!\n",cid); */
		return CE_MODE;
	}
        if (cid&FLAG_LPT) {
            WARN(" cid %d not comm port\n",cid);
            return CE_MODE;
        }
	if (lpStat) {
		COMSTAT stat;
		if (!ClearCommError(ptr->handle, &temperror, &stat))
			return CE_MODE;
		lpStat->status = *((BYTE *)&stat);
		lpStat->cbInQue = stat.cbInQue + ptr->unget;
		lpStat->cbOutQue = stat.cbOutQue;
		TRACE("cid %d, error %d, stat %d in %d out %d\n",
				cid, ptr->commerror, lpStat->status, lpStat->cbInQue,
				lpStat->cbOutQue);
	}
	else {
		if (!ClearCommError(ptr->handle, &temperror, NULL))
			return CE_MODE;
		TRACE("cid %d, error %d, lpStat NULL\n",
				cid, ptr->commerror);
	}

	temperror |= ptr->commerror;
	ptr->commerror = 0;

	return(temperror);
}

/*****************************************************************************
 *	SetCommEventMask	(USER.208)
 */
SEGPTR WINAPI SetCommEventMask16(INT16 cid,UINT16 fuEvtMask)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid %d,mask %d\n",cid,fuEvtMask);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
            return 0;
	}

	ptr->eventmask = fuEvtMask;

        if (cid&FLAG_LPT) {
            WARN(" cid %d not comm port\n",cid);
            return 0;
        }
        if (!COM[cid].seg_unknown) COM[cid].seg_unknown = MapLS( COM[cid].unknown );
        return COM[cid].seg_unknown;
}

/*****************************************************************************
 *	GetCommEventMask	(USER.209)
 */
UINT16 WINAPI GetCommEventMask16(INT16 cid,UINT16 fnEvtClear)
{
	struct DosDeviceStruct *ptr;
	WORD events;

	TRACE("cid %d, mask %d\n", cid, fnEvtClear);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
	    return 0;
	}

        if (cid&FLAG_LPT) {
            WARN(" cid %d not comm port\n",cid);
            return 0;
        }

	events = *(WORD*)(COM[cid].unknown) & COM[cid].eventmask;
	*(WORD*)(COM[cid].unknown) &= ~fnEvtClear;
	return events;
}

/*****************************************************************************
 *	SetCommState	(USER.201)
 */
INT16 WINAPI SetCommState16(LPDCB16 lpdcb)
{
	struct DosDeviceStruct *ptr;
	DCB dcb;

	TRACE("cid %d, ptr %p\n", lpdcb->Id, lpdcb);
	if ((ptr = GetDeviceStruct(lpdcb->Id)) == NULL) {
		FIXME("no handle for cid = %0x!\n",lpdcb->Id);
		return -1;
	}

	memset(&dcb,0,sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);

	/*
	 * according to MSDN, we should first interpret lpdcb->BaudRate as follows:
	 * 1. if the baud rate is a CBR constant, interpret it.
	 * 2. if it is greater than 57600, the baud rate is 115200
	 * 3. use the actual baudrate
	 * steps 2 and 3 are equivalent to 16550 baudrate divisor = 115200/BaudRate
	 */
	switch(lpdcb->BaudRate)
	{
	case CBR_110:    dcb.BaudRate = 110;    break;
	case CBR_300:    dcb.BaudRate = 300;    break;
	case CBR_600:    dcb.BaudRate = 600;    break;
	case CBR_1200:   dcb.BaudRate = 1200;   break;
	case CBR_2400:   dcb.BaudRate = 2400;   break;
	case CBR_4800:   dcb.BaudRate = 4800;   break;
	case CBR_9600:   dcb.BaudRate = 9600;   break;
	case CBR_14400:  dcb.BaudRate = 14400;  break;
	case CBR_19200:  dcb.BaudRate = 19200;  break;
	case CBR_38400:  dcb.BaudRate = 38400;  break;
	case CBR_56000:  dcb.BaudRate = 56000;  break;
	case CBR_128000: dcb.BaudRate = 128000; break;
	case CBR_256000: dcb.BaudRate = 256000; break;
	default:
		if(lpdcb->BaudRate>57600)
		dcb.BaudRate = 115200;
        else
		dcb.BaudRate = lpdcb->BaudRate;
	}

	dcb.ByteSize=lpdcb->ByteSize;
	dcb.StopBits=lpdcb->StopBits;
	dcb.fBinary = 1;

	dcb.fParity=lpdcb->fParity;
	dcb.Parity=lpdcb->Parity;

	dcb.fOutxCtsFlow = lpdcb->fOutxCtsFlow;
	dcb.fOutxDsrFlow = lpdcb->fOutxDsrFlow;

	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	if (lpdcb->fDtrflow)
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;

	if (lpdcb->fDtrDisable)
		dcb.fDtrControl = DTR_CONTROL_DISABLE;

	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	if (lpdcb->fRtsflow)
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;

	if (lpdcb->fRtsDisable)
		dcb.fRtsControl = RTS_CONTROL_DISABLE;

	ptr->evtchar = lpdcb->EvtChar;

	dcb.fInX = lpdcb->fInX;
	dcb.fOutX = lpdcb->fOutX;

	if (!SetCommState(ptr->handle,&dcb)) {
		ptr->commerror = WinError();
		return -1;
	} else {
		ptr->commerror = 0;
		return 0;
	}
}

/*****************************************************************************
 *	GetCommState	(USER.202)
 */
INT16 WINAPI GetCommState16(INT16 cid, LPDCB16 lpdcb)
{
	struct DosDeviceStruct *ptr;
	DCB dcb;

	TRACE("cid %d, ptr %p\n", cid, lpdcb);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}
	if (!GetCommState(ptr->handle,&dcb)) {
		ptr->commerror = WinError();
		return -1;
	}

	lpdcb->Id = cid;

	COMM16_DCBtoDCB16(&dcb,lpdcb);

	lpdcb->EvtChar = ptr->evtchar;

	return 0;
}

/*****************************************************************************
 *	TransmitCommChar	(USER.206)
 */
INT16 WINAPI TransmitCommChar16(INT16 cid,CHAR chTransmit)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid %d, data %d\n", cid, chTransmit);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}
	
	return TransmitCommChar(ptr->handle, chTransmit) ? 0 : -1;
}

/*****************************************************************************
 *	UngetCommChar	(USER.212)
 */
INT16 WINAPI UngetCommChar16(INT16 cid,CHAR chUnget)
{
	struct DosDeviceStruct *ptr;

	TRACE("cid %d (char %d)\n", cid, chUnget);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}

	if (ptr->unget) {
	  /* character already queued */
	  return -1;
	}

	ptr->ungetchar = chUnget;
	ptr->unget = TRUE;
	ptr->commerror = 0;
	return 0;
}

/*****************************************************************************
 *	ReadComm	(USER.204)
 */
INT16 WINAPI ReadComm16(INT16 cid,LPSTR lpvBuf,INT16 cbRead)
{
	int length;
	struct DosDeviceStruct *ptr;
	LPSTR orgBuf = lpvBuf;
	OVERLAPPED ov;
	DWORD count = -1;

	ZeroMemory(&ov, sizeof(ov));
	ov.hEvent = CreateEventW(NULL, 0, 0, NULL);
	if(ov.hEvent == INVALID_HANDLE_VALUE)
		return -1;
	
	TRACE("cid %d, ptr %p, length %d\n", cid, lpvBuf, cbRead);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}
	if (ptr->unget)
	{
		*orgBuf = ptr->ungetchar;
		orgBuf++;
		cbRead--;
	}

	if (!ReadFile(ptr->handle, orgBuf, cbRead, &length, &ov))
	{
		if (GetLastError() == ERROR_IO_PENDING)
			GetOverlappedResult(ptr->handle, &ov, &length, TRUE);
	}

	CloseHandle(ov.hEvent);
	
	if (ptr->unget)
	{
		ptr->unget = FALSE;
		length++;
	}

	if (length)
		TRACE("%d bytes read\n", length);

	return length;
}

/*****************************************************************************
 *	WriteComm	(USER.205)
 */
INT16 WINAPI WriteComm16(INT16 cid, LPSTR lpvBuf, INT16 cbWrite)
{
	int length;
	struct DosDeviceStruct *ptr;

	TRACE("cid %d, ptr %p, length %d\n",
		cid, lpvBuf, cbWrite);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}

	if (cid&FLAG_LPT)
		WriteFile(ptr->handle, lpvBuf, cbWrite, &length, NULL);
	else
	{
		int count;
		length = 0;
		EnterCriticalSection(&ptr->writelock);
		while (length < cbWrite) {
			count = ((ptr->obuf_tail > ptr->obuf_head) ?
						(ptr->obuf_tail-1) : ptr->obuf_size) - ptr->obuf_head;
			if (!count) break;
			if ((cbWrite - length) < count)
				count = cbWrite - length;
			memcpy(ptr->outbuf + ptr->obuf_head, lpvBuf, count);
			ptr->obuf_head += count;
			if (ptr->obuf_head >= ptr->obuf_size)
				ptr->obuf_head = 0;
			lpvBuf += count;
			length += count;
		}
		LeaveCriticalSection(&ptr->writelock);
		SetEvent(ptr->writeev);
	}

	if (length < cbWrite)
	{
		ptr->commerror = CE_TXFULL;
		return -length;
	}
	return length;
}

/***********************************************************************
 *           EnableCommNotification   (USER.245)
 */
BOOL16 WINAPI EnableCommNotification16( INT16 cid, HWND16 hwnd,
                                      INT16 cbWriteNotify, INT16 cbOutQueue )
{
	struct DosDeviceStruct *ptr;

	TRACE("(%d, %x, %d, %d)\n", cid, hwnd, cbWriteNotify, cbOutQueue);
	if ((ptr = GetDeviceStruct(cid)) == NULL) {
		FIXME("no handle for cid = %0x!\n",cid);
		return -1;
	}
	ptr->wnd = WIN_Handle32( hwnd );
	ptr->n_read = cbWriteNotify;
	ptr->n_write = cbOutQueue;
	return TRUE;
}
