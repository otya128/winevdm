#define __inout_bcount_part_opt(x, y)
#include <windef.h>
#define DS_MODALFRAME       0x80L
#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L

#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#ifndef __MSABI_LONG
# if defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)
#  define __MSABI_LONG(x)         x ## l
# else
#  define __MSABI_LONG(x)         x
# endif
#endif
#define LBS_NOTIFY             __MSABI_LONG(0x00000001)
#define LBS_SORT               __MSABI_LONG(0x00000002)
#define LBS_NOREDRAW           __MSABI_LONG(0x00000004)
#define LBS_MULTIPLESEL        __MSABI_LONG(0x00000008)
#define LBS_OWNERDRAWFIXED     __MSABI_LONG(0x00000010)
#define LBS_OWNERDRAWVARIABLE  __MSABI_LONG(0x00000020)
#define LBS_HASSTRINGS         __MSABI_LONG(0x00000040)
#define LBS_USETABSTOPS        __MSABI_LONG(0x00000080)
#define LBS_NOINTEGRALHEIGHT   __MSABI_LONG(0x00000100)
#define LBS_MULTICOLUMN        __MSABI_LONG(0x00000200)
#define LBS_WANTKEYBOARDINPUT  __MSABI_LONG(0x00000400)
#define LBS_EXTENDEDSEL        __MSABI_LONG(0x00000800)
#define LBS_DISABLENOSCROLL    __MSABI_LONG(0x00001000)
#define LBS_NODATA             __MSABI_LONG(0x00002000)
#define LBS_NOSEL              __MSABI_LONG(0x00004000)
#define LBS_COMBOBOX           __MSABI_LONG(0x00008000)
#define LBS_STANDARD           (LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER)
#define VK_F1             0x70

#define MNID_FILE_OPEN          0x101
#define MNID_FILE_PRINT         0x104
#define MNID_FILE_SETUP         0x106
#define MNID_FILE_EXIT          0x108

#define MNID_EDIT_COPYDLG       0x111
#define MNID_EDIT_ANNOTATE      0x112

#define MNID_BKMK_DEFINE        0x121

#define MNID_OPTS_HISTORY       0x134
#define MNID_OPTS_FONTS_SMALL   0x135
#define MNID_OPTS_FONTS_NORMAL  0x136
#define MNID_OPTS_FONTS_LARGE   0x137

#define MNID_HELP_HELPON        0x141
#define MNID_HELP_HELPTOP       0x142
#define MNID_HELP_ABOUT         0x143

#define MNID_CTXT_ANNOTATE      0x200
#define MNID_CTXT_COPY          0x201
#define MNID_CTXT_PRINT         0x202
#define MNID_CTXT_FONTS_SMALL   0x210
#define MNID_CTXT_FONTS_NORMAL  0x211
#define MNID_CTXT_FONTS_LARGE   0x212

#define MAIN_MENU		0xF000
#define CONTEXT_MENU            0xF001
#define MAIN_ACCEL              0xF002

#define STID_WINE_HELP		0x120
#define STID_WHERROR		0x121
#define STID_WARNING  		0x122
#define STID_INFO  		0x123
#define STID_NOT_IMPLEMENTED 	0x124
#define STID_HLPFILE_ERROR_s 	0x125
#define STID_CONTENTS 		0x126
#define STID_INDEX	 	0x127
#define STID_BACK 		0x128
#define STID_ALL_FILES 		0x12B
#define STID_HELP_FILES_HLP	0x12C
#define STID_DIALOG_TEST	0x12D
#define STID_FILE_NOT_FOUND_s	0x12E
#define STID_NO_RICHEDIT        0x12F
#define STID_PSH_INDEX          0x130

#define IDD_INDEX               0x150
#define IDC_INDEXLIST           0x151
#define IDD_SEARCH              0x152

#define IDI_WINHELP             0xF00
