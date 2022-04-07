#1 WEP
2  pascal OleRegisterServer(str segptr ptr word word) OleRegisterServer16
3  pascal OleRevokeServer(long) OleRevokeServer16
4  pascal OleBlockServer(long) OleBlockServer16
5  pascal OleUnblockServer(long ptr) OleUnblockServer16
6  pascal OleRegisterServerDoc(long str segptr ptr) OleRegisterServerDoc16
7  pascal OleRevokeServerDoc(long) OleRevokeServerDoc16
8  pascal OleRenameServerDoc(long str) OleRenameServerDoc16
9  pascal OleRevertServerDoc(long) OleRevertServerDoc16
10 pascal OleSavedServerDoc(long) OleSavedServerDoc16
11 pascal OleRevokeObject(ptr) OleRevokeObject16
12 pascal OleQueryServerVersion() OleQueryServerVersion16
21 stub SRVRWNDPROC
22 stub DOCWNDPROC
23 stub ITEMWNDPROC
24 stub SENDDATAMSG
25 stub FINDITEMWND
26 pascal ItemCallBack(ptr word long) ItemCallBack16
27 stub TERMINATECLIENTS
28 stub TERMINATEDOCCLIENTS
29 stub DELETECLIENTINFO
30 stub SENDRENAMEMSG
31 stub ENUMFORTERMINATE
