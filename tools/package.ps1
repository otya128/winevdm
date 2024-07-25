Param([parameter(mandatory)] [string]$root, [parameter(mandatory)] [string]$src, [parameter(mandatory)] [string]$dst, [parameter(mandatory)] [string]$objcopy, [parameter(mandatory)] [string]$as, [string]$dll)
$ErrorActionPreference = "Stop"
if (!($dll))
{
  $dll = (Join-Path $dst "dll/")
}
function Make-Dll([string]$spec, [string]$name, [string]$dst)
{
  Write-Host $name
  cmd.exe /c (Join-Path $src convspec.exe) $spec $name -ver | cmd.exe /c $as -o "$dst.o"
  cmd.exe /c $objcopy -O binary -j .data "$dst.o" $dst
  rm "$dst.o"
}
mkdir $dst -ErrorAction SilentlyContinue > $null
copy (Join-Path $src "otvdm.exe") $dst
copy (Join-Path $src "otvdmw.exe") $dst
copy (Join-Path $src "winhlp32.exe") $dst
copy (Join-Path $root "otvdm.ini") $dst
copy (Join-Path $root "LICENSE") $dst
copy (Join-Path $root "README.md") (Join-Path $dst "README.txt")
copy (Join-Path $root "install.inf") $dst
copy (Join-Path $root "install.lnk") $dst
copy (Join-Path $root "installw.inf") $dst
copy (Join-Path $root "install (no console).lnk") $dst
copy (Join-Path $root "uninstall.reg") $dst
mkdir $dll -ErrorAction SilentlyContinue > $null
copy (Join-Path $src "*16") $dll
copy (Join-Path $src "haxmvm.dll") $dll -ErrorAction SilentlyContinue > $null
copy (Join-Path $src "gvm.dll") $dll -ErrorAction SilentlyContinue > $null
copy (Join-Path $src "whpxvm.dll") $dll -ErrorAction SilentlyContinue > $null
copy (Join-Path $src "vm86.dll") $dll
copy (Join-Path $src "wow32.dll") $dll
copy (Join-Path $src "libwine.dll") $dst
mkdir (Join-Path $dst "WINDOWS") -ErrorAction SilentlyContinue > $null
ni (Join-Path $dst "WINDOWS/WIN.INI") -ErrorAction SilentlyContinue > $null
ni (Join-Path $dst "WINDOWS/SYSTEM.INI") -ErrorAction SilentlyContinue > $null
mkdir (Join-Path $dst "dummydll") -ErrorAction SilentlyContinue > $null
mkdir (Join-Path $dst "dummydll/SYSTEM") -ErrorAction SilentlyContinue > $null
mkdir (Join-Path $dst "dummydll/SYSTEM32") -ErrorAction SilentlyContinue > $null
Make-Dll (Join-Path $root "ctl3d/ctl3d.dll16.spec") "CTL3D" (Join-Path $dst "dummydll/SYSTEM/CTL3D.DLL")
Make-Dll (Join-Path $root "ctl3dv2/ctl3dv2.dll16.spec") "CTL3DV2" (Join-Path $dst "dummydll/SYSTEM/CTL3DV2.DLL")
Make-Dll (Join-Path $root "avifile/avifile.dll16.spec") "AVIFILE" (Join-Path $dst "dummydll/SYSTEM/AVIFILE.DLL")
Make-Dll (Join-Path $root "comm/comm.drv16.spec") "COMM" (Join-Path $dst "dummydll/SYSTEM/COMM.DRV")
Make-Dll (Join-Path $root "commdlg/commdlg.dll16.spec") "COMMDLG" (Join-Path $dst "dummydll/SYSTEM/COMMDLG.DLL")
Make-Dll (Join-Path $root "compobj/compobj.dll16.spec") "COMPOBJ" (Join-Path $dst "dummydll/SYSTEM/COMPOBJ.DLL")
Make-Dll (Join-Path $root "ddeml/ddeml.dll16.spec") "DDEML" (Join-Path $dst "dummydll/SYSTEM/DDEML.DLL")
Make-Dll (Join-Path $root "display/display.drv16.spec") "DISPLAY" (Join-Path $dst "dummydll/SYSTEM/DISPLAY.DRV")
Make-Dll (Join-Path $root "gdi/gdi.exe16.spec") "GDI" (Join-Path $dst "dummydll/SYSTEM/GDI.EXE")
Make-Dll (Join-Path $root "keyboard/keyboard.drv16.spec") "KEYBOARD" (Join-Path $dst "dummydll/SYSTEM/KEYBOARD.DRV")
Make-Dll (Join-Path $root "krnl386/krnl386.exe16.spec") "KERNEL" (Join-Path $dst "dummydll/SYSTEM/KRNL386.EXE")
Make-Dll (Join-Path $root "lzexpand/lzexpand.dll16.spec") "LZEXPAND" (Join-Path $dst "dummydll/SYSTEM/LZEXPAND.DLL")
Make-Dll (Join-Path $root "mmsystem/mmsystem.dll16.spec") "MMSYSTEM" (Join-Path $dst "dummydll/SYSTEM/MMSYSTEM.DLL")
Make-Dll (Join-Path $root "mouse/mouse.drv16.spec") "MOUSE" (Join-Path $dst "dummydll/SYSTEM/MOUSE.DRV")
Make-Dll (Join-Path $root "msacm/msacm.dll16.spec") "MSACM" (Join-Path $dst "dummydll/SYSTEM/MSACM.DLL")
Make-Dll (Join-Path $root "msvideo/msvideo.dll16.spec") "MSVIDEO" (Join-Path $dst "dummydll/SYSTEM/MSVIDEO.DLL")
Make-Dll (Join-Path $root "netapi/netapi.dll16.spec") "NETAPI" (Join-Path $dst "dummydll/SYSTEM/NETAPI.DLL")
Make-Dll (Join-Path $root "nddeapi/nddeapi.dll16.spec") "NDDEAPI" (Join-Path $dst "dummydll/SYSTEM/NDDEAPI.DLL")
Make-Dll (Join-Path $root "ole2/ole2.dll16.spec") "OLE2" (Join-Path $dst "dummydll/SYSTEM/OLE2.DLL")
Make-Dll (Join-Path $root "ole2conv/ole2conv.dll16.spec") "OLE2CONV" (Join-Path $dst "dummydll/SYSTEM/OLE2CONV.DLL")
Make-Dll (Join-Path $root "ole2disp/ole2disp.dll16.spec") "OLE2DISP" (Join-Path $dst "dummydll/SYSTEM/OLE2DISP.DLL")
Make-Dll (Join-Path $root "ole2nls/ole2nls.dll16.spec") "OLE2NLS" (Join-Path $dst "dummydll/SYSTEM/OLE2NLS.DLL")
Make-Dll (Join-Path $root "ole2prox/ole2prox.dll16.spec") "OLE2PROX" (Join-Path $dst "dummydll/SYSTEM/OLE2PROX.DLL")
Make-Dll (Join-Path $root "ole2thk/ole2thk.dll16.spec") "OLE2THK" (Join-Path $dst "dummydll/SYSTEM/OLE2THK.DLL")
Make-Dll (Join-Path $root "olecli/olecli.dll16.spec") "OLECLI" (Join-Path $dst "dummydll/SYSTEM/OLECLI.DLL")
Make-Dll (Join-Path $root "olesvr/olesvr.dll16.spec") "OLESVR" (Join-Path $dst "dummydll/SYSTEM/OLESVR.DLL")
Make-Dll (Join-Path $root "shell/shell.dll16.spec") "SHELL" (Join-Path $dst "dummydll/SYSTEM/SHELL.DLL")
Make-Dll (Join-Path $root "sound/sound.drv16.spec") "SOUND" (Join-Path $dst "dummydll/SYSTEM/SOUND.DRV")
Make-Dll (Join-Path $root "storage/storage.dll16.spec") "STORAGE" (Join-Path $dst "dummydll/SYSTEM/STORAGE.DLL")
Make-Dll (Join-Path $root "system/system.drv16.spec") "SYSTEM" (Join-Path $dst "dummydll/SYSTEM/SYSTEM.DRV")
Make-Dll (Join-Path $root "toolhelp/toolhelp.dll16.spec") "TOOLHELP" (Join-Path $dst "dummydll/SYSTEM/TOOLHELP.DLL")
Make-Dll (Join-Path $root "user/user.exe16.spec") "USER" (Join-Path $dst "dummydll/SYSTEM/USER.EXE")
Make-Dll (Join-Path $root "ver/ver.dll16.spec") "VER" (Join-Path $dst "dummydll/SYSTEM/VER.DLL")
Make-Dll (Join-Path $root "wifeman/wifeman.dll16.spec") "WIFEMAN" (Join-Path $dst "dummydll/SYSTEM/WIFEMAN.DLL")
Make-Dll (Join-Path $root "win87em/win87em.dll16.spec") "WIN87EM" (Join-Path $dst "dummydll/SYSTEM/WIN87EM.DLL")
Make-Dll (Join-Path $root "wing/wing.dll16.spec") "WING" (Join-Path $dst "dummydll/SYSTEM/WING.DLL")
Make-Dll (Join-Path $root "winnls/winnls.dll16.spec") "WINNLS" (Join-Path $dst "dummydll/SYSTEM/WINNLS.DLL")
Make-Dll (Join-Path $root "winoldap/winoldap.mod16.spec") "WINOLDAP" (Join-Path $dst "dummydll/SYSTEM/WINOLDAP.MOD")
Make-Dll (Join-Path $root "winsock/winsock.dll16.spec") "WINSOCK" (Join-Path $dst "dummydll/SYSTEM/WINSOCK.DLL")
Make-Dll (Join-Path $root "winspool/winspool.drv16.spec") "WINSPOOL" (Join-Path $dst "dummydll/SYSTEM/WINSPOOL.DRV")
copy (Join-Path $root "dummydll/dummydll.dll") (Join-Path $dst "dummydll/SYSTEM/KRNL286.EXE") -ErrorAction SilentlyContinue
copy (Join-Path $root "dummydll/dummydll.dll") (Join-Path $dst "dummydll/SYSTEM/VGA.DRV") -ErrorAction SilentlyContinue
copy (Join-Path $root "dummydll/vgaoem.fon") (Join-Path $dst "dummydll/SYSTEM/VGAOEM.FON") -ErrorAction SilentlyContinue
copy (Join-Path $root "dummydll/dummydll.dll") (Join-Path $dst "dummydll/SYSTEM32/WOWEXEC.EXE") -ErrorAction SilentlyContinue
Copy-Item -Filter * -Path (Join-Path $dst "dummydll/*") -Recurse -Destination (Join-Path $dst "WINDOWS/") -ErrorAction SilentlyContinue
ni (Join-Path $dst "WINDOWS/SYSTEM/MIDIMAP.CFG") -ErrorAction SilentlyContinue > $null
