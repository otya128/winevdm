/*
 * Windows regedit.exe registry editor implementation.
 *
 * Copyright 1999 Sylvain St-Germain
 * Copyright 2002 Andriy Palamarchuk
 * Copyright 2008 Alexander N. Sørnes <alex@thehandofagony.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include "windows.h"
#include "winnt.h"
#include "winreg.h"
#include "assert.h"
#include "wine/debug.h"

#define REG_VAL_BUF_SIZE        4096
DWORD WINAPI RegSetValue16(HKEY hkey, LPCSTR name, DWORD type, LPCSTR data, DWORD count);

/* version for Windows 3.1 */
static void processRegEntry31(char *line)
{
    int key_end = 0;
    char *value;
    int res;
    static const char hkcr[] = "HKEY_CLASSES_ROOT";
   
    if (line[0] == '\0') return;
    if (memcmp(line, hkcr, sizeof(hkcr) - 1)) return;
    line += sizeof(hkcr) - 1;

    /* get key name */
    while (line[key_end] && !isspace(line[key_end])) key_end++;

    value = line + key_end;
    while (isspace(value[0])) value++;

    if (value[0] == '=') value++;
    if (value[0] == ' ') value++; /* at most one space is skipped */

    line[key_end] = '\0';

    res = RegSetValue16(HKEY_CLASSES_ROOT, line, REG_SZ, value, strlen(value) + 1);
}

/******************************************************************************
 * Processes a registry file.
 * Correctly processes comments (in # and ; form), line continuation.
 *
 * Parameters:
 *   in - input stream to read from
 *   first_chars - beginning of stream, read due to Unicode check
 */
static void processRegLinesA(FILE *in)
{
    char *buf = NULL;  /* the line read from the input stream */
    unsigned long line_size = REG_VAL_BUF_SIZE;
    size_t chars_in_buf = -1;
    char *s; /* A pointer to buf for fread */
    char *line; /* The start of the current line */
    unsigned long version = 0;

    static const char header_31[] = "REGEDIT";

    buf = HeapAlloc(GetProcessHeap(), 0, line_size);
    s = buf;
    line = buf;

    while (!feof(in)) {
        size_t size_remaining;
        int size_to_get;
        char *s_eol = NULL; /* various local uses */

        /* Do we need to expand the buffer? */
        assert(s >= buf && s <= buf + line_size);
        size_remaining = line_size - (s - buf);
        if (size_remaining < 3) /* we need at least 3 bytes of room for \r\n\0 */
        {
            char *new_buffer;
            size_t new_size = line_size + REG_VAL_BUF_SIZE;
            if (new_size > line_size) /* no arithmetic overflow */
                new_buffer = HeapReAlloc(GetProcessHeap(), 0, buf, new_size);
            else
                new_buffer = NULL;
            buf = new_buffer;
            line = buf;
            s = buf + line_size - size_remaining;
            line_size = new_size;
            size_remaining = line_size - (s - buf);
        }

        /* Get as much as possible into the buffer, terminating on EOF,
         * error or once we have read the maximum amount. Abort on error.
         */
        size_to_get = (size_remaining > INT_MAX ? INT_MAX : size_remaining);

        chars_in_buf = fread(s, 1, size_to_get - 1, in);
        s[chars_in_buf] = 0;

        if (chars_in_buf == 0) {
            if (ferror(in)) {
                ExitThread(1);
            } else {
                assert(feof(in));
                *s = '\0';
            }
        }

        /* If we didn't read the end-of-line sequence or EOF, go around again */
        while (1)
        {
            s_eol = strpbrk(line, "\r\n");
            if (!s_eol) {
                /* Move the stub of the line to the start of the buffer so
                 * we get the maximum space to read into, and so we don't
                 * have to recalculate 'line' if the buffer expands */
                MoveMemory(buf, line, strlen(line) + 1);
                line = buf;
                s = strchr(line, '\0');
                break;
            }

            /* If we find a comment line, discard it and go around again */
            if (line [0] == '#' || line [0] == ';') {
                if (*s_eol == '\r' && *(s_eol + 1) == '\n')
                    line = s_eol + 2;
                else
                    line = s_eol + 1;
                continue;
            }

            /* Remove any line feed. Leave s_eol on the last \0 */
            if (*s_eol == '\r' && *(s_eol + 1) == '\n')
                *s_eol++ = '\0';
            *s_eol = '\0';

    	    /* Check if the line is a header string */
            if (!memcmp(line, header_31, sizeof(header_31)))
                version = 1;
            else if (version)
                processRegEntry31(line);
            line = s_eol + 1;
        }
    }
    HeapFree(GetProcessHeap(), 0, buf);
}

 /******************************************************************************
 * Copies file name from command line string to the buffer.
 * Rewinds the command line string pointer to the next non-spece character
 * after the file name.
 * Buffer contains an empty string if no filename was found;
 *
 * params:
 * command_line - command line current position pointer
 *      where *s[0] is the first symbol of the file name.
 * file_name - buffer to write the file name to.
 */
void get_file_name(CHAR **command_line, CHAR *file_name)
{
    CHAR *s = *command_line;
    int pos = 0;                /* position of pointer "s" in *command_line */
    file_name[0] = 0;

    if (!s[0])
    {
        return;
    }

    if (s[0] == '"')
    {
        s++;
        (*command_line)++;
        while(s[0] != '"')
        {
            if (!s[0])
            {
                ExitThread(1);
            }
            s++;
            pos++;
        }
    } else {
        while(s[0] && !isspace(s[0]))
        {
            s++;
            pos++;
        }
    }
    memcpy(file_name, *command_line, pos * sizeof((*command_line)[0]));
    /* remove the last backslash */
    if (file_name[pos - 1] == '\\')
    {
        file_name[pos - 1] = '\0';
    } else {
        file_name[pos] = '\0';
    }

    if (s[0])
    {
        s++;
        pos++;
    }
    while(s[0] && isspace(s[0]))
    {
        s++;
        pos++;
    }
    (*command_line) += pos;
}

int WINAPI WinMain16(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
    LPSTR s = lpCmdLine;        /* command line pointer */
    CHAR ch = *s;               /* current character */
   
    while (isspace(ch))
    {
        s++;
        ch = *s;
    } 
 
    while (ch && ((ch == '-') || (ch == '/')))
    {
        char chu;
        char ch2;
        
        s++;
        ch = *s;
        ch2 = *(s+1);
        chu = toupper(ch);
        if (!ch2 || isspace(ch2)) 
        {
            if (chu == 'S' || chu == 'V')
            {
                /* ignore these switches */
            } else {
                ExitThread(1);
                break;
            }
            s++;
        } else {
            if (ch2 == ':')
            {
                switch (chu)
                {
                case 'L':
                    /* fall through */
                case 'R':
                    s += 2;
                    while (*s && !isspace(*s))
                    {
                        s++;
                    }
                    break;
                default:
                    ExitThread(1);
                    break;
                }
            } else {
                ExitThread(1);
                break;
            }
        }
        /* skip spaces to the next parameter */
        ch = *s;
        while (ch && isspace(ch))
        {
            s++;
            ch = *s;
        }
    }

    CHAR filename[MAX_PATH];
    FILE *reg_file;

    get_file_name(&s, filename);
    if (!filename[0])
        ExitThread(1);

    while(filename[0])
    {
        reg_file = fopen(filename, "r");
        if (reg_file)
        {
            processRegLinesA(reg_file);
        }
        get_file_name(&s, filename);
    }
    ExitThread(0);
}
