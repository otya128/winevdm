# winevdm on windows (64bit)
<img src="screenshot.PNG">

[Download](https://github.com/otya128/winevdm/releases)

winevdm on windows (64bit)

An altered version of winevdm (a 16-bit Windows emulator), ported to 64-bit Windows.

# Requirements (likely)

+ VC++
+ gas

# winevdm

```bat
winevdm.exe [--app-name app.exe] command line

winevdm.exe CALC.EXE
```

It can also run DOS executables (DOS emulator-like).
You can set the DOS version by the VDMDOSVER environment variable.

# convspec

convspec is a tool to convert wine spec files for extracting from winebuild and using assembly in VC++.

```bat
convspec .specfile modulename > asm
convspec .specfile -DEF > def

convspec commdlg.dll16.spec COMMDLG > commdlg.dll16.asm
convspec commdlg.dll16.spec -DEF > commdlg.def
as --32 -o commdlg.dll16.obj commdlg.dll16.asm
```

# gdi
GDI.EXE
# krnl386
KRNL386.EXE
# user
USER.EXE
# vm86
CPU emulator
# wine
libwine
# wow32
wow32.dll
