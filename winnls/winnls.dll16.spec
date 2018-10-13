1   stub WINNLSINQUIRE
2   stub HOOKKEYBOARDMESSAGE
#3   stub ___EXPORTEDSTUB
4   stub ENUMGETFONTTYPE
5   stub ENUMFACENOTTT
6   pascal SendIMEMessage(word long) SendIMEMessageEx16
7   pascal SendIMEMessageEx(word long) SendIMEMessageEx16
9   stub SENDIMEMESSAGEINTERNAL
10  stub WINNLSSETKEYBOARDHOOK
11  pascal -ret16 WINNLSSetIMEHandle(segptr word) WINNLSSetIMEHandle16
12  pascal -ret16 WINNLSSetIMEStatus(word word) WINNLSSetIMEStatus16
14  pascal -ret16 WINNLSSetIMEHotkey(word word) WINNLSSetIMEHotkey16
15  stub WINNLSGETIMEHOTKEY
16  pascal -ret16 WINNLSEnableIME(word word) WINNLSEnableIME16
17  stub WINNLSGETKEYSTATE
18  pascal -ret16 WINNLSGetEnableStatus(word) WINNLSGetEnableStatus16
19  pascal -ret16 WINNLSSetKeyState(word) WINNLSSetKeyState16
20  stub IMPADDIME
21  stub IMPDELETEIME
22  pascal -ret16 IMPQueryIME(ptr) IMPQueryIME16
23  pascal -ret16 IMPGetIME(word ptr) IMPGetIME16
24  pascal -ret16 IMPSetIME(word ptr) IMPSetIME16
25  pascal -ret16 IMPModifyIME(str ptr) IMPModifyIME16
26  pascal -ret16 IMPGetDefaultIME(ptr) IMPGetDefaultIME16
27  pascal -ret16 IMPSetDefaultIME(ptr) IMPSetDefaultIME16
30  pascal -ret16 WINNLSSendString(word word ptr) WINNLSSendString16
31  pascal -ret16 WINNLSPostAppMessage(word word word long) WINNLSPostAppMessage16
32  pascal -ret16 WINNLSSendAppMessage(word word word long) WINNLSSendAppMessage16
33  pascal -ret16 WINNLSSendControl(word word) WINNLSSendControl16
#160 stub FT_WNLSFTHKTHKCONNECTIONDATA
