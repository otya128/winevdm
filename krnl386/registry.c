/*
 * 16-bit registry functions
 *
 * Copyright 1996 Marcus Meissner
 * Copyright 1998 Matthew Becker
 * Copyright 1999 Sylvain St-Germain
 * Copyright 2002 Alexandre Julliard
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

#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "wine/debug.h"
#include "wine/winbase16.h"
#include "wine/exception.h"
#include "kernel16_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(reg);

NTSYSAPI NTSTATUS  WINAPI NtCreateKey(PHANDLE,ACCESS_MASK,const OBJECT_ATTRIBUTES*,ULONG,const UNICODE_STRING*,ULONG,PULONG);

static DWORD (WINAPI *pRegCloseKey)(HKEY);
static DWORD (WINAPI *pRegCreateKeyA)(HKEY,LPCSTR,PHKEY);
static DWORD (WINAPI *pRegDeleteKeyA)(HKEY,LPCSTR);
static DWORD (WINAPI *pRegDeleteValueA)(HKEY,LPCSTR);
static DWORD (WINAPI *pRegEnumKeyA)(HKEY,DWORD,LPSTR,DWORD);
static DWORD (WINAPI *pRegEnumValueA)(HKEY,DWORD,LPSTR,LPDWORD,LPDWORD,LPDWORD,LPBYTE,LPDWORD);
static DWORD (WINAPI *pRegFlushKey)(HKEY);
static DWORD (WINAPI *pRegOpenKeyA)(HKEY,LPCSTR,PHKEY);
static DWORD (WINAPI *pRegQueryValueA)(HKEY,LPCSTR,LPSTR,LPLONG);
static DWORD (WINAPI *pRegQueryValueExA)(HKEY,LPCSTR,LPDWORD,LPDWORD,LPBYTE,LPDWORD);
static DWORD (WINAPI *pRegSetValueA)(HKEY,LPCSTR,DWORD,LPCSTR,DWORD);
static DWORD (WINAPI *pRegSetValueExA)(HKEY,LPCSTR,DWORD,DWORD,const BYTE*,DWORD);

static HMODULE advapi32;

static BOOL enable_registry_redirection;
static HKEY registry_redirection_root;
static HKEY registry_redirection_classes;
static HKEY registry_redirection_current_user;
static HKEY registry_redirection_local_machine;

/* 0 and 1 are valid rootkeys in win16 shell.dll and are used by
 * some programs. Do not remove those cases. -MM
 */
static inline void fix_win16_hkey( HKEY *hkey )
{
    if (*hkey == 0 || *hkey == (HKEY)1) *hkey = HKEY_CLASSES_ROOT;
    if (enable_registry_redirection && *hkey == HKEY_CLASSES_ROOT)
    {
        *hkey = registry_redirection_classes;
    }
    if (enable_registry_redirection && *hkey == HKEY_CURRENT_USER)
    {
        *hkey = registry_redirection_current_user;
    }
    if (enable_registry_redirection && *hkey == HKEY_LOCAL_MACHINE)
    {
        *hkey = registry_redirection_local_machine;
    }
}

static BOOL is_redir_root_key(HKEY key)
{
    if (!enable_registry_redirection)
        return FALSE;
    return key == registry_redirection_classes;
}
static void init_func_ptrs(void)
{
    enable_registry_redirection = krnl386_get_config_int("otvdm", "EnableRegistryRedirection", FALSE);
    if (enable_registry_redirection)
    {
        LONG result = 0;
        result = result || RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\otvdm\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registry_redirection_root, NULL);
        result = result || RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\otvdm\\HKEY_CLASSES_ROOT\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registry_redirection_classes, NULL);
        result = result || RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\otvdm\\HKEY_CURRENT_USER\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registry_redirection_current_user, NULL);
        result = result || RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\otvdm\\HKEY_LOCAL_MACHINE\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &registry_redirection_local_machine, NULL);
        if (result)
        {
            ERR("Could not init EnableRegistryRedirection\n");
            enable_registry_redirection = FALSE;
        }
    }
    advapi32 = LoadLibraryA("advapi32.dll");
    if (!advapi32)
    {
        ERR( "Unable to load advapi32.dll\n" );
        ExitProcess(1);
    }
#define GET_PTR(name)  p##name = (void *)GetProcAddress(advapi32,#name);
    GET_PTR( RegCloseKey );
    GET_PTR( RegCreateKeyA );
    GET_PTR( RegDeleteKeyA );
    GET_PTR( RegDeleteValueA );
    GET_PTR( RegEnumKeyA );
    GET_PTR( RegEnumValueA );
    GET_PTR( RegFlushKey );
    GET_PTR( RegOpenKeyA );
    GET_PTR( RegQueryValueA );
    GET_PTR( RegQueryValueExA );
    GET_PTR( RegSetValueA );
    GET_PTR( RegSetValueExA );
#undef GET_PTR
}


static BOOL is_empty(LPCSTR name)
{
    return name == NULL || name[0] == 0;
}

/******************************************************************************
 *           RegEnumKey   [KERNEL.216]
 */
DWORD WINAPI RegEnumKey16( HKEY hkey, DWORD index, LPSTR name, DWORD name_len )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegEnumKeyA( hkey, index, name, name_len );
    return result;
}

static void fix_redir_key(PHKEY retkey, LPDWORD result)
{
    if (!enable_registry_redirection)
        return;
    if (*retkey == registry_redirection_classes)
    {
        *retkey = HKEY_CLASSES_ROOT;
    }
    if (*retkey == registry_redirection_local_machine)
    {
        *retkey = HKEY_LOCAL_MACHINE;
    }
    if (*retkey == registry_redirection_current_user)
    {
        *retkey = HKEY_CURRENT_USER;
    }
}

/******************************************************************************
 *           RegOpenKey   [KERNEL.217]
 */
DWORD WINAPI RegOpenKey16( HKEY hkey, LPCSTR name, PHKEY retkey )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegOpenKeyA( hkey, name, retkey );
    fix_redir_key(retkey, &result);
    return result;
}

static LPWSTR strdupAtoW(LPCSTR str)
{
    LPWSTR ret;
    INT len;

    if (!str) return NULL;
    len = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    ret = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    if (ret) MultiByteToWideChar(CP_ACP, 0, str, -1, ret, len);
    return ret;
}

/******************************************************************************
 *           RegCreateKey   [KERNEL.218]
 */
DWORD WINAPI RegCreateKey16( HKEY hkey, LPCSTR name, PHKEY retkey )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result;
    TRACE("%x %s\n", hkey, name);
    if (is_redir_root_key(hkey) && is_empty(name))
    {
        *retkey = hkey;
        fix_redir_key(retkey, &result);
        return ERROR_SUCCESS;
    }
    // try to create with write access
    result = RegCreateKeyExA(hkey, name, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, retkey, NULL);
    // try to redirect to HKEY_CURRENT_USER if possible
    if (name && !enable_registry_redirection && (result != ERROR_SUCCESS))
    {
        if (hkey == HKEY_CLASSES_ROOT)
        {
            const char softclass[] = "Software\\Classes\\";
            char *hkcukey = HeapAlloc(GetProcessHeap(), 0, 17 + strlen(name) + 1);
            strcpy(hkcukey, softclass);
            strcat(hkcukey, name);
            result = RegCreateKeyExA(HKEY_CURRENT_USER, hkcukey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, retkey, NULL);
            if (result == ERROR_SUCCESS)
                WARN("HKCR CreateKey redirected to HKCU\n");
            HeapFree(GetProcessHeap(), 0, hkcukey);
        }
        else if(hkey < 0x80000000)
        {
            // regcreatekeyex will convert hkcu\software\classes subkeys into hkcr so try ntcreatekey
            UNICODE_STRING nameU;
            OBJECT_ATTRIBUTES objattr;
            WCHAR *nameW = strdupAtoW(name);
            DWORD dis, ret;
            RtlCreateUnicodeString(&nameU, nameW);
            HeapFree(GetProcessHeap(), 0, nameW);
            InitializeObjectAttributes(&objattr, &nameU, 0, hkey, NULL);
            ret = NtCreateKey(retkey, KEY_ALL_ACCESS, &objattr, 0, NULL, REG_OPTION_NON_VOLATILE, &dis);
            result = RtlNtStatusToDosError(ret);
            RtlFreeUnicodeString(&nameU);
        }
    }
    // failed, try to open for reading
    if (result != ERROR_SUCCESS)
        result = RegOpenKeyA(hkey, name, retkey);
    fix_redir_key(retkey, &result);
    TRACE("%x, %x\n", result, *retkey);
    return result;
}

/******************************************************************************
 *           RegDeleteKey   [KERNEL.219]
 */
DWORD WINAPI RegDeleteKey16( HKEY hkey, LPCSTR name )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    if (is_redir_root_key(hkey) && is_empty(name))
        return ERROR_SUCCESS;
    DWORD result = pRegDeleteKeyA( hkey, name );
    return result;
}

/******************************************************************************
 *           RegCloseKey   [KERNEL.220]
 */
DWORD WINAPI RegCloseKey16( HKEY hkey )
{
    if (!advapi32) init_func_ptrs();
    TRACE("%x\n", hkey);
    fix_win16_hkey( &hkey );
    if (is_redir_root_key(hkey))
        return ERROR_SUCCESS;
    DWORD result = pRegCloseKey( hkey );
    return result;
}

/******************************************************************************
 *           RegSetValue   [KERNEL.221]
 */
DWORD WINAPI RegSetValue16( HKEY hkey, LPCSTR name, DWORD type, LPCSTR data, DWORD count )
{
    HKEY subkey;
    DWORD result;
    if (type != REG_SZ) return 7; // ntvdm returns 7 in this case for some reason
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    __TRY
    {
        count = strlen(data);
    }
    __EXCEPT_ALL
    {
        return ERROR_INVALID_PARAMETER;
    }
    __ENDTRY
    if (!name)
        return RegSetValueEx16(hkey, NULL, 0, type, data, count);

    result = RegCreateKey16(hkey, name, &subkey);
    if (result == ERROR_SUCCESS)
    {
        result = RegSetValueEx16(subkey, NULL, 0, type, data, count);
        RegCloseKey16(subkey);
    }
    return result;
}

/******************************************************************************
 *           RegDeleteValue  [KERNEL.222]
 */
DWORD WINAPI RegDeleteValue16( HKEY hkey, LPSTR name )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    if (is_redir_root_key(hkey) && is_empty(name))
        return ERROR_SUCCESS;
    DWORD result = pRegDeleteValueA( hkey, name );
    return result;
}

/******************************************************************************
 *           RegEnumValue   [KERNEL.223]
 */
DWORD WINAPI RegEnumValue16( HKEY hkey, DWORD index, LPSTR value, LPDWORD val_count,
                             LPDWORD reserved, LPDWORD type, LPBYTE data, LPDWORD count )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegEnumValueA( hkey, index, value, val_count, reserved, type, data, count );
    return result;
}

/******************************************************************************
 *           RegQueryValue   [KERNEL.224]
 *
 * NOTES
 *    Is this HACK still applicable?
 *
 * HACK
 *    The 16bit RegQueryValue doesn't handle selectorblocks anyway, so we just
 *    mask out the high 16 bit.  This (not so much incidentally) hopefully fixes
 *    Aldus FH4)
 */
DWORD WINAPI RegQueryValue16( HKEY hkey, LPCSTR name, LPSTR data, LPDWORD count )
{
    DWORD incount = *count;
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    if (count) *count &= 0xffff;
    DWORD result = pRegQueryValueA( hkey, name, data, (LONG*) count );
    if (result == ERROR_MORE_DATA)
    {
        DWORD realcount = *count + 1;
        char *buf = HeapAlloc(GetProcessHeap(), 0, realcount);
        result = pRegQueryValueA(hkey, name, buf, &realcount);
        memcpy(data, buf, incount);
        HeapFree(GetProcessHeap(), 0, buf);
        *count = incount;
    }
    return result;
}

/******************************************************************************
 *           RegQueryValueEx   [KERNEL.225]
 */
DWORD WINAPI RegQueryValueEx16( HKEY hkey, LPCSTR name, LPDWORD reserved, LPDWORD type,
                                LPBYTE data, LPDWORD count )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegQueryValueExA( hkey, name, reserved, type, data, count );
    return result;
}

/******************************************************************************
 *           RegSetValueEx   [KERNEL.226]
 */
DWORD WINAPI RegSetValueEx16( HKEY hkey, LPCSTR name, DWORD reserved, DWORD type,
                              const BYTE *data, DWORD count )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegSetValueExA( hkey, name, reserved, type, data, count );
    return result;
}

/******************************************************************************
 *           RegFlushKey   [KERNEL.227]
 */
DWORD WINAPI RegFlushKey16( HKEY hkey )
{
    if (!advapi32) init_func_ptrs();
    fix_win16_hkey( &hkey );
    DWORD result = pRegFlushKey( hkey );
    return result;
}
