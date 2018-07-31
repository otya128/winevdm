# winevdm on 64-bit Windows

<img src="screenshot.PNG">

[Download stable version](https://github.com/otya128/winevdm/releases)

[Download latest version (unstable, slower than stable version)](https://ci.appveyor.com/project/otya128/winevdm/build/artifacts)

16-bit Windows (Windows 1.x, 2.x, 3.0, 3.1, etc.) on 64-bit Windows

An altered version of winevdm (a 16-bit Windows emulator), ported to 64-bit Windows.

# How to compile

+ Install Visual Studio 2017
+ Install binutils
+ Edit PropertySheet.props
+ Compile

# How to install

+ Download or compile
+ Edit install.reg
+ Register install.reg
+ You can execute Win16 binaries directly!

**If you install v0.4.x, you should add these to install.reg and register install.reg again.**
```ini
[HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\NtVdm64\OTVDM]
"CommandLine"="\"%m\" %c"
"InternalName"="*"
"MappedExeName"="C:\\PATH\\TO\\otvdm.exe"
"ProductName"="*"
"ProductVersion"="*"
```

# winevdm
```bat
winevdm.exe [--app-name app.exe] command line
winevdm.exe CALC.EXE
```
It can also run DOS executables (DOS emulator-like).
You can set the DOS version with the VDMDOSVER environment variable.

