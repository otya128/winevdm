# winevdm
winevdm on windows(64bit)
16bit用Windowsのプログラムを動かすプログラム
winevdmの移植版
# コンパイルに必要そうなもの
+ VC++
+ gas

# winevdm
```bat
winevdm.exe [--app-name app.exe] command line
```
今の所DOS実行ファイルは扱えない(DOSエミュレータは他に存在するためそれを使った方が確実)
# convspec
convert wine spec file
winebuildから必要な部分を取り出しVC++で動かせるようにし、関数名の扱いなどを変え出力されたassemblyをVC++で扱えるようにした
```bat
convspec .specfile modulename > asm
convspec .specfile -DEF > def
```
# gdi
GDI.EXE
# krnl386
KRNL386.EXE
# user
USER.EXE
# vm86
emulate virtual 8086 mode
# wine
libwine
# wow32
wow32.dll
