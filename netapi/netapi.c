/*
 * 16-bit netapi functions
 *
 * Copyright (C) 2024 John Goodridge
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

#include <stdio.h>
#include <windows.h>
#include <lm.h>
#include "wine/winbase16.h"
#include "wownt32.h"
#include "winuser.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(netapi);

// Structure taken from https://datatracker.ietf.org/doc/html/draft-leach-cifs-rap-spec-00
// and https://learn.microsoft.com/en-us/windows/win32/api/lmwksta/ns-lmwksta-wksta_info_100
typedef struct {
    SEGPTR wki10_computername;   //pointer to a NULL terminated ASCII string that specifies the name of the workstation
    SEGPTR wki10_username;       //pointer to a NULL terminated ASCII string that specifies the user who is logged on at the workstation
    LPSTR wki10_langroup;        //pointer to a NULL terminated ASCII string that specifies the domain to which the workstation belongs
    UCHAR wkil0_ver_major;       //major version number of the operating system the workstation is running
    UCHAR wkil0_ver_minor;       //minor version number of the operating system the workstation is running
    LPSTR wkil0_1ogon_domain;    //pointer to a NULL terminated ASCII string that specifies the domain for which a user is logged on
    LPSTR wkil0_oth_domains;     //pointer to a list of other domains that the workstation is currently browsing.
                                 //The domains are space delimited and the list is zero
} WKSTA_INFO_10, *PWKSTA_INFO_10;

/***********************************************************************
 *              NetWkstaGetInfo
 */
INT16 WINAPI NetWkstaGetInfo16(LPCSTR pszServer, USHORT sLevel, char* pbBuffer, USHORT cbBuffer, PUSHORT pcbTotalAvalaible)
{
    WCHAR serverW[RMLEN] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, pszServer, -1, serverW, RMLEN);
    LPWKSTA_INFO_100 pwkstaInfo100 = NULL;
    NET_API_STATUS status = NetWkstaGetInfo(serverW, 100, (LPBYTE*)&pwkstaInfo100);

    if (status == NERR_Success)
    {
        LPWKSTA_USER_INFO_0 pwkstaUserInfo0 = NULL;
        status = NetWkstaUserGetInfo(NULL, 0, (LPBYTE*)&pwkstaUserInfo0);

        if (status == NERR_Success)
        {
            DWORD byteCount = 0;
            NetApiBufferSize(pwkstaInfo100, &byteCount);

            switch (sLevel) {
                case 10:
                {
                    CHAR strComputerName[RMLEN];
                    sprintf_s(strComputerName, RMLEN, "%S", pwkstaInfo100->wki100_computername); // convert to ansi

                    CHAR strUserName[UNLEN];
                    sprintf_s(strUserName, UNLEN, "%S", pwkstaUserInfo0->wkui0_username); // convert to ansi

                    // Calculate total bytes and location within buffer to store strings
                    DWORD TotalBytesNeeded = sizeof(WKSTA_INFO_10) + (strlen(strComputerName) + strlen(strUserName) + 3) * sizeof(CHAR);
                    LPBYTE FixedDataEnd = (LPBYTE)(pbBuffer + sizeof(WKSTA_INFO_10));

                    // Check if buffer is too small
                    if (cbBuffer < TotalBytesNeeded)
                    {
                        if (pcbTotalAvalaible)
                        {
                            *pcbTotalAvalaible = (USHORT)TotalBytesNeeded;
                        }

                        status = NERR_BufTooSmall;
                    }
                    else
                    {   
                        if (pbBuffer)
                        {
                            // Copy computer name into buffer
                            PWKSTA_INFO_10 pwkstaInfo10 = (PWKSTA_INFO_10)pbBuffer;
                            memcpy(FixedDataEnd, strComputerName, strlen(strComputerName));
                            SEGPTR c = MapLS(FixedDataEnd);
                            pwkstaInfo10->wki10_computername = c;

                            // Copy user name into buffer
                            FixedDataEnd += strlen(strUserName) + sizeof(CHAR);
                            memcpy(FixedDataEnd, strUserName, strlen(strUserName));
                            SEGPTR u = MapLS(FixedDataEnd);
                            pwkstaInfo10->wki10_username = u;

                            // Fill in rest of the fields
                            pwkstaInfo10->wki10_langroup = NULL;     //TODO
                            pwkstaInfo10->wkil0_ver_major = (UCHAR)pwkstaInfo100->wki100_ver_major;
                            pwkstaInfo10->wkil0_ver_minor = (UCHAR)pwkstaInfo100->wki100_ver_minor;
                            pwkstaInfo10->wkil0_1ogon_domain = NULL; //TODO
                            pwkstaInfo10->wkil0_oth_domains = NULL;  //TODO
                            status = NERR_Success;
                        }
                        else
                        {
                            status = NERR_BufTooSmall;
                        }
                    }

                    break;
                }

                default:
                    FIXME("Unsupported level %d", sLevel);
                    status = ERROR_INVALID_LEVEL;
            }

            NetApiBufferFree(pwkstaUserInfo0);
        }

        NetApiBufferFree(pwkstaInfo100);
    }

    return (INT16)status;
}
