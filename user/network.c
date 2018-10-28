/*
 * USER Windows Network functions
 *
 * Copyright 1995 Martin von Loewis
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

#include "config.h"

#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "windef.h"
#include "winbase.h"
#include "winnetwk.h"
#include "wine/winnet16.h"
#include "wine/debug.h"
#include "wownt32.h"

WINE_DEFAULT_DEBUG_CHANNEL(wnet);

WORD map_wnet16_status(DWORD wnet32)
{
    switch (wnet32)
    {
    case -1:
        return WN16_CANCEL;
    case WN_SUCCESS:
        return WN16_SUCCESS;
    case WN_NOT_SUPPORTED:
        return WN16_NOT_SUPPORTED;
    case WN_NET_ERROR:
        return WN16_NET_ERROR;
    case WN_MORE_DATA:
        return WN16_MORE_DATA;
    case WN_BAD_POINTER:
        return WN16_BAD_POINTER;
    case WN_BAD_VALUE:
        return WN16_BAD_VALUE;
    case WN_BAD_PASSWORD:
        return WN16_BAD_PASSWORD;
    case WN_ACCESS_DENIED:
        return WN16_ACCESS_DENIED;
    case WN_FUNCTION_BUSY:
        return WN16_FUNCTION_BUSY;
        /*
    case WN_WINDOWS_ERROR:
        return WN16_WINDOWS_ERROR;
        */
    case WN_BAD_USER:
        return WN16_BAD_USER;
    case WN_OUT_OF_MEMORY:
        return WN16_OUT_OF_MEMORY;
    case WN_CANCEL:
        return WN16_CANCEL;
    case WN_NOT_CONNECTED:
        return WN16_NOT_CONNECTED;
    case WN_OPEN_FILES:
        return WN16_OPEN_FILES;
    case WN_BAD_NETNAME:
        return WN16_BAD_NETNAME;
    case WN_BAD_LOCALNAME:
        return WN16_BAD_LOCALNAME;
    case WN_ALREADY_CONNECTED:
        return WN16_ALREADY_CONNECTED;
    case WN_DEVICE_ERROR:
        return WN16_DEVICE_ERROR;
    case WN_CONNECTION_CLOSED:
        return WN16_CONNECTION_CLOSED;
        /*
    case WN_CONTINUE:
        return WN16_CONTINUE;
    case WN_BAD_JOBID:
        return WN16_BAD_JOBID;
    case WN_JOB_NOT_FOUND:
        return WN16_JOB_NOT_FOUND;
    case WN_JOB_NOT_HELD:
        return WN16_JOB_NOT_HELD;
    case WN_BAD_QUEUE:
        return WN16_BAD_QUEUE;
    case WN_BAD_FILE_HANDLE:
        return WN16_BAD_FILE_HANDLE;
    case WN_CANT_SET_COPIES:
        return WN16_CANT_SET_COPIES;
    case WN_ALREADY_LOCKED:
        return WN16_ALREADY_LOCKED;
    case WN_NO_ERROR:
        return WN16_NO_ERROR;
        */
    }
    return WN16_WINDOWS_ERROR;
}

LPCSTR wnet16_status_error_string(WORD wnet)
{
#define STR(x) case x:\
    return #x
    switch (wnet)
    {
        STR(WN16_SUCCESS);
        STR(WN16_NOT_SUPPORTED);
        STR(WN16_NET_ERROR);
        STR(WN16_MORE_DATA);
        STR(WN16_BAD_POINTER);
        STR(WN16_BAD_VALUE);
        STR(WN16_BAD_PASSWORD);
        STR(WN16_ACCESS_DENIED);
        STR(WN16_FUNCTION_BUSY);
        STR(WN16_WINDOWS_ERROR);
        STR(WN16_BAD_USER);
        STR(WN16_OUT_OF_MEMORY);
        STR(WN16_CANCEL);
        STR(WN16_CONTINUE);
        STR(WN16_NOT_CONNECTED);
        STR(WN16_OPEN_FILES);
        STR(WN16_BAD_NETNAME);
        STR(WN16_BAD_LOCALNAME);
        STR(WN16_ALREADY_CONNECTED);
        STR(WN16_DEVICE_ERROR);
        STR(WN16_CONNECTION_CLOSED);
        STR(WN16_BAD_JOBID);
        STR(WN16_JOB_NOT_FOUND);
        STR(WN16_JOB_NOT_HELD);
        STR(WN16_BAD_QUEUE);
        STR(WN16_BAD_FILE_HANDLE);
        STR(WN16_CANT_SET_COPIES);
        STR(WN16_ALREADY_LOCKED);
        STR(WN16_NO_ERROR);
    }
#undef STR
    return NULL;
}
/*
 * Remote printing
 */

/**************************************************************************
 *              WNetOpenJob       [USER.501]
 */
WORD WINAPI WNetOpenJob16( LPSTR szQueue, LPSTR szJobTitle, WORD nCopies, LPINT16 pfh )
{
    FIXME( "(%s, %s, %d, %p): stub\n",
           debugstr_a(szQueue), debugstr_a(szJobTitle), nCopies, pfh );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetCloseJob      [USER.502]
 */
WORD WINAPI WNetCloseJob16( WORD fh, LPINT16 pidJob, LPSTR szQueue )
{
    FIXME( "(%d, %p, %s): stub\n", fh, pidJob, debugstr_a(szQueue) );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetWriteJob      [USER.524]
 */
WORD WINAPI WNetWriteJob16( HANDLE16 hJob, LPSTR lpData, LPINT16 lpcbData )
{
    FIXME( "(%04x, %p, %p): stub\n", hJob, lpData, lpcbData );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetAbortJob       [USER.503]
 */
WORD WINAPI WNetAbortJob16( LPSTR szQueue, WORD wJobId )
{
    FIXME( "(%s, %d): stub\n", debugstr_a(szQueue), wJobId );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetHoldJob       [USER.504]
 */
WORD WINAPI WNetHoldJob16( LPSTR szQueue, WORD wJobId )
{
    FIXME( "(%s, %d): stub\n", debugstr_a(szQueue), wJobId );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetReleaseJob       [USER.505]
 */
WORD WINAPI WNetReleaseJob16( LPSTR szQueue, WORD wJobId )
{
    FIXME( "(%s, %d): stub\n", debugstr_a(szQueue), wJobId );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetCancelJob       [USER.506]
 */
WORD WINAPI WNetCancelJob16( LPSTR szQueue, WORD wJobId )
{
    FIXME( "(%s, %d): stub\n", debugstr_a(szQueue), wJobId );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetSetJobCopies     [USER.507]
 */
WORD WINAPI WNetSetJobCopies16( LPSTR szQueue, WORD wJobId, WORD nCopies )
{
    FIXME( "(%s, %d, %d): stub\n", debugstr_a(szQueue), wJobId, nCopies );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetWatchQueue       [USER.508]
 */
WORD WINAPI WNetWatchQueue16( HWND16 hWnd, LPSTR szLocal, LPSTR szUser, WORD nQueue )
{
    FIXME( "(%04x, %s, %s, %d): stub\n",
           hWnd, debugstr_a(szLocal), debugstr_a(szUser), nQueue );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetUnwatchQueue     [USER.509]
 */
WORD WINAPI WNetUnwatchQueue16( LPSTR szQueue )
{
    FIXME( "(%s): stub\n", debugstr_a(szQueue) );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetLockQueueData       [USER.510]
 */
WORD WINAPI WNetLockQueueData16( LPSTR szQueue, LPSTR szUser,
                                 LPQUEUESTRUCT16 *lplpQueueStruct )
{
    FIXME( "(%s, %s, %p): stub\n",
           debugstr_a(szQueue), debugstr_a(szUser), lplpQueueStruct );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetUnlockQueueData       [USER.511]
 */
WORD WINAPI WNetUnlockQueueData16( LPSTR szQueue )
{
    FIXME( "(%s): stub\n", debugstr_a(szQueue) );
    return WN16_NET_ERROR;
}


/*
 * Connections
 */

/********************************************************************
 *  WNetAddConnection [USER.517]  Directs a local device to net
 *
 * Redirects a local device (either a disk drive or printer port)
 * to a shared device on a remote server.
 */
WORD WINAPI WNetAddConnection16( LPCSTR lpNetPath, LPCSTR lpPassWord,
                                 LPCSTR lpLocalName )
{
    FIXME( "(%s, %p, %s): stub\n",
           debugstr_a(lpNetPath), lpPassWord, debugstr_a(lpLocalName) );
    return WN16_NET_ERROR;
}

/********************************************************************
 *   WNetCancelConnection [USER.518]  undirects a local device
 */
WORD WINAPI WNetCancelConnection16( LPSTR lpName, BOOL16 bForce )
{
    FIXME( "(%s, %04X): stub\n", debugstr_a(lpName), bForce);
    return WN16_NOT_SUPPORTED;
}

/********************************************************************
 * WNetGetConnection [USER.512] reverse-resolves a local device
 */
WORD WINAPI WNetGetConnection16( LPSTR lpLocalName,
                                 LPSTR lpRemoteName, UINT16 *cbRemoteName )
{
    char label[32];

    TRACE( "local %s\n", lpLocalName );
    switch(GetDriveTypeA(lpLocalName))
    {
    case DRIVE_REMOTE:
        GetVolumeInformationA( lpLocalName, label, sizeof(label), NULL, NULL, NULL, NULL, 0 );
        if (strlen(label) + 1 > *cbRemoteName)
        {
            *cbRemoteName = strlen(label) + 1;
            return WN16_MORE_DATA;
        }
        strcpy( lpRemoteName, label );
        *cbRemoteName = strlen(lpRemoteName) + 1;
        return WN16_SUCCESS;
    case DRIVE_REMOVABLE:
    case DRIVE_FIXED:
    case DRIVE_CDROM:
        TRACE("file is local\n");
        return WN16_NOT_CONNECTED;
    default:
        return WN16_BAD_LOCALNAME;
    }
}

/**************************************************************************
 *              WNetRestoreConnection       [USER.523]
 */
WORD WINAPI WNetRestoreConnection16( HWND16 hwndOwner, LPSTR lpszDevice )
{
    FIXME( "(%04x, %s): stub\n", hwndOwner, debugstr_a(lpszDevice) );
    return WN16_NOT_SUPPORTED;
}


/*
 * Capabilities
 */

/**************************************************************************
 *		WNetGetCaps		[USER.513]
 */
WORD WINAPI WNetGetCaps16( WORD capability )
{
    switch (capability)
    {
    case WNNC16_SPEC_VERSION:
        return 0x30a; /* WfW 3.11 (and apparently other 3.1x) */

    case WNNC16_NET_TYPE:
        /* hi byte = network type,
           lo byte = network vendor (Netware = 0x03) [15 types] */
        return WNNC16_NET_MultiNet | WNNC16_SUBNET_WinWorkgroups;

    case WNNC16_DRIVER_VERSION:
        /* driver version of vendor */
        return 0x100; /* WfW 3.11 */

    case WNNC16_USER:
        /* 1 = WNetGetUser is supported */
        return 1;

    case WNNC16_CONNECTION:
        /* returns mask of the supported connection functions */
        return   WNNC16_CON_AddConnection | WNNC16_CON_CancelConnection
               | WNNC16_CON_GetConnections /* | WNNC16_CON_AutoConnect */
               | WNNC16_CON_BrowseDialog | WNNC16_CON_RestoreConnection;

    case WNNC16_PRINTING:
        /* returns mask of the supported printing functions */
        return   WNNC16_PRT_OpenJob | WNNC16_PRT_CloseJob | WNNC16_PRT_HoldJob
               | WNNC16_PRT_ReleaseJob | WNNC16_PRT_CancelJob
               | WNNC16_PRT_SetJobCopies | WNNC16_PRT_WatchQueue
               | WNNC16_PRT_UnwatchQueue | WNNC16_PRT_LockQueueData
               | WNNC16_PRT_UnlockQueueData | WNNC16_PRT_AbortJob
               | WNNC16_PRT_WriteJob;

    case WNNC16_DIALOG:
        /* returns mask of the supported dialog functions */
        return   WNNC16_DLG_DeviceMode | WNNC16_DLG_BrowseDialog
               | WNNC16_DLG_ConnectDialog | WNNC16_DLG_DisconnectDialog
               | WNNC16_DLG_ViewQueueDialog | WNNC16_DLG_PropertyDialog
               | WNNC16_DLG_ConnectionDialog
            /* | WNNC16_DLG_PrinterConnectDialog
               | WNNC16_DLG_SharesDialog | WNNC16_DLG_ShareAsDialog */;

    case WNNC16_ADMIN:
        /* returns mask of the supported administration functions */
        /* not sure if long file names is a good idea */
        return   WNNC16_ADM_GetDirectoryType
            /* | WNNC16_ADM_DirectoryNotify */ /*not yet supported*/
               | WNNC16_ADM_LongNames /* | WNNC16_ADM_SetDefaultDrive */;

    case WNNC16_ERROR:
        /* returns mask of the supported error functions */
        return   WNNC16_ERR_GetError | WNNC16_ERR_GetErrorText;

    case WNNC16_PRINTMGREXT:
        /* returns the Print Manager version in major and
           minor format if Print Manager functions are available */
        return 0x30e; /* printman version of WfW 3.11 */

    case 0xffff:
        /* Win 3.11 returns HMODULE of network driver here
           FIXME: what should we return ?
           logonoff.exe needs it, msmail crashes with wrong value */
        return 0;

    default:
        return 0;
    }
}


/*
 * Get User
 */

/**************************************************************************
 *		WNetGetUser			[USER.516]
 */
WORD WINAPI WNetGetUser16( LPSTR szUser, LPINT16 nBufferSize )
{
    DWORD lpBufferSize, ret;

    if(!szUser || !nBufferSize) return WN16_BAD_POINTER;

    lpBufferSize = *nBufferSize;
    ret = WNetGetUserA( NULL, szUser, &lpBufferSize );
    *nBufferSize = lpBufferSize;

    switch (ret)
    {
        case NO_ERROR:
            return WN16_SUCCESS;
        case ERROR_MORE_DATA:
            return WN16_MORE_DATA;
        default:
            FIXME("Untranslated return value %d\n", ret);
    }
    return ret;
}


/*
 * Browsing
 */

/**************************************************************************
 *              WNetDeviceMode       [USER.514]
 */
WORD WINAPI WNetDeviceMode16( HWND16 hWndOwner )
{
    FIXME( "(%04x): stub\n", hWndOwner );
    return WN16_NOT_SUPPORTED;
}

/**************************************************************************
 *              WNetBrowseDialog       [USER.515]
 */
WORD WINAPI WNetBrowseDialog16( HWND16 hParent, WORD nType, LPSTR szPath )
{
    FIXME( "(%04x, %x, %s): stub\n", hParent, nType, szPath );
    return WN16_NOT_SUPPORTED;
}

/********************************************************************
 *              WNetConnectDialog       [USER.525]
 */
WORD WINAPI WNetConnectDialog16( HWND16 hWndParent, WORD iType )
{
    return map_wnet16_status(WNetConnectionDialog(HWND_32(hWndParent), iType));
}

/**************************************************************************
 *              WNetDisconnectDialog       [USER.526]
 */
WORD WINAPI WNetDisconnectDialog16( HWND16 hwndOwner, WORD iType )
{
    return map_wnet16_status(WNetDisconnectDialog(HWND_32(hwndOwner), iType));
}

/**************************************************************************
 *              WNetConnectionDialog     [USER.527]
 */
WORD WINAPI WNetConnectionDialog16( HWND16 hWndParent, WORD iType )
{
    return map_wnet16_status(WNetConnectionDialog(HWND_32(hWndParent), iType));
}

/**************************************************************************
 *              WNetViewQueueDialog       [USER.528]
 */
WORD WINAPI WNetViewQueueDialog16( HWND16 hwndOwner, LPSTR lpszQueue )
{
    FIXME(" (%04x, %s): stub\n", hwndOwner, debugstr_a(lpszQueue) );
    return WN16_NOT_SUPPORTED;
}

/**************************************************************************
 *              WNetPropertyDialog       [USER.529]
 */
WORD WINAPI WNetPropertyDialog16( HWND16 hwndParent, WORD iButton,
                                  WORD nPropSel, LPSTR lpszName, WORD nType )
{
    FIXME( "(%04x, %x, %x, %s, %x ): stub\n",
          hwndParent, iButton, nPropSel, debugstr_a(lpszName), nType );
    return WN16_NOT_SUPPORTED;
}

/**************************************************************************
 *              WNetGetPropertyText       [USER.532]
 */
WORD WINAPI WNetGetPropertyText16( WORD iButton, WORD nPropSel, LPSTR lpszName,
                                   LPSTR lpszButtonName, WORD cbButtonName, WORD nType )
{
    FIXME( "(%04x, %04x, %s, %s, %04x): stub\n",
           iButton, nPropSel, debugstr_a(lpszName), debugstr_a(lpszButtonName), nType);
    return WN16_NOT_SUPPORTED;
}


/*
 * Admin
 */

/*********************************************************************
 *  WNetGetDirectoryType [USER.530]  Decides whether resource is local
 *
 * RETURNS
 *    on success,  puts one of the following in *lpType:
 * - WNDT_NETWORK   on a network
 * - WNDT_LOCAL     local
 */
WORD WINAPI WNetGetDirectoryType16( LPSTR lpName, LPINT16 lpType )
{
    UINT type = GetDriveTypeA(lpName);
    if ( type == DRIVE_NO_ROOT_DIR )
        type = GetDriveTypeA(NULL);

    *lpType = (type == DRIVE_REMOTE)? WNDT_NETWORK : WNDT_NORMAL;

    TRACE( "%s is %s\n", debugstr_a(lpName),
           (*lpType == WNDT_NETWORK)? "WNDT_NETWORK" : "WNDT_NORMAL" );
    return WN16_SUCCESS;
}

/**************************************************************************
 *              WNetDirectoryNotify       [USER.531]
 */
WORD WINAPI WNetDirectoryNotify16( HWND16 hwndOwner, LPSTR lpDir, WORD wOper )
{
    FIXME( "(%04x, %s, %s): stub\n", hwndOwner, debugstr_a(lpDir),
           (wOper == WNDN_MKDIR)? "WNDN_MKDIR" :
           (wOper == WNDN_MVDIR)? "WNDN_MVDIR" :
           (wOper == WNDN_RMDIR)? "WNDN_RMDIR" : "unknown" );
    return WN16_NOT_SUPPORTED;
}


/*
 * Error handling
 */

/**************************************************************************
 *              WNetGetError       [USER.519]
 */
WORD WINAPI WNetGetError16( LPINT16 nError )
{
    FIXME( "(%p): stub\n", nError );
    return WN16_NOT_SUPPORTED;
}

/**************************************************************************
 *              WNetGetErrorText       [USER.520]
 */
WORD WINAPI WNetGetErrorText16( WORD nError, LPSTR lpBuffer, LPINT16 nBufferSize )
{
    FIXME( "(%x, %p, %p): stub\n", nError, lpBuffer, nBufferSize );
    return WN16_NET_ERROR;
}

/**************************************************************************
 *              WNetErrorText       [USER.499]
 */
WORD WINAPI WNetErrorText16( WORD nError, LPSTR lpszText, WORD cbText )
{
    LPCSTR str = wnet16_status_error_string(nError);
    SIZE_T len;
    if (!str || !cbText)
        return FALSE;
    len = min(cbText, strlen(str) + 1);
    memcpy(lpszText, str, len);
    lpszText[len] = 0;
    return TRUE;
}
