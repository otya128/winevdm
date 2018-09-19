#1 WEP
2 pascal -ret16 OleDelete(segptr) OleDelete16
3 pascal -ret16 OleSaveToStream(segptr segptr) OleSaveToStream16
4 pascal -ret16 OleLoadFromStream(segptr str segptr long str ptr) OleLoadFromStream16
6 pascal -ret16 OleClone(segptr segptr long str ptr) OleClone16
7 stub OLECOPYFROMLINK
8 pascal -ret16 OleEqual(segptr segptr) OleEqual16
9 pascal -ret16 OleQueryLinkFromClip(str word word) OleQueryLinkFromClip16
10 pascal -ret16 OleQueryCreateFromClip(str word word) OleQueryCreateFromClip16
11 pascal -ret16 OleCreateLinkFromClip (str segptr long str ptr word word) OleCreateLinkFromClip16
12 pascal -ret16 OleCreateFromClip(str segptr long str long word word) OleCreateFromClip16
13 pascal -ret16 OleCopyToClipboard(segptr) OleCopyToClipboard16
14 pascal -ret16 OleQueryType(ptr segptr) OleQueryType16
15 pascal -ret16 OleSetHostNames(long str str) OleSetHostNames16
16 pascal -ret16 OleSetTargetDevice(long word) OleSetTargetDevice16
17 stub OLESETBOUNDS
18 pascal -ret16 OleQueryBounds(long ptr) OleQueryBounds16
19 pascal -ret16 OleDraw(long word ptr ptr word) OleDraw16
20 pascal -ret16 OleQueryOpen(segptr) OleQueryOpen16
21 pascal -ret16 OleActivate(long word word word word ptr) OleActivate16
22 pascal -ret16 OleUpdate(segptr) OleUpdate16
23 pascal -ret16 OleReconnect(segptr) OleReconnect16
24 stub OLEGETLINKUPDATEOPTIONS
25 stub OLESETLINKUPDATEOPTIONS
26 pascal -ret16 OleEnumFormats(segptr word) OleEnumFormats16
27 pascal -ret16 OleClose(segptr) OleClose16
28 pascal -ret16 OleGetData(segptr word ptr) OleGetData16
29 stub OLESETDATA
30 stub OLEQUERYPROTOCOL
31 stub OLEQUERYOUTOFDATE
32 stub OLEOBJECTCONVERT
33 stub OLECREATEFROMTEMPLATE
34 pascal -ret16 OleCreate(str segptr str long str ptr word word) OleCreate16
35 pascal -ret16 OleQueryReleaseStatus(segptr) OleQueryReleaseStatus16
36 pascal -ret16 OleQueryReleaseError(long) OleQueryReleaseError16
37 pascal -ret16 OleQueryReleaseMethod(long) OleQueryReleaseMethod16
38 pascal -ret16 OleCreateFromFile(str segptr str str long str ptr word word) OleCreateFromFile16
39 stub OLECREATELINKFROMFILE
40 pascal -ret16 OleRelease(segptr) OleRelease16
41 pascal -ret16 OleRegisterClientDoc(ptr ptr long ptr) OleRegisterClientDoc16
42 pascal -ret16 OleRevokeClientDoc(long) OleRevokeClientDoc16
43 pascal -ret16 OleRenameClientDoc(long ptr) OleRenameClientDoc16
44 pascal -ret16 OleRevertClientDoc(long) OleRevertClientDoc16
45 pascal -ret16 OleSavedClientDoc(long) OleSavedClientDoc16
46 pascal -ret16 OleRename(segptr str) OleRename16
47 pascal -ret16 OleEnumObjects(long ptr) OleEnumObjects16
48 stub OLEQUERYNAME
49 stub OLESETCOLORSCHEME
50 stub OLEREQUESTDATA
54 pascal -ret16 OleLockServer(segptr ptr) OleLockServer16
55 pascal -ret16 OleUnlockServer(segptr) OleUnlockServer16
56 pascal -ret16 OleQuerySize(long ptr) OleQuerySize16
57 stub OLEEXECUTE
58 stub OLECREATEINVISIBLE
59 pascal -ret16 OleQueryClientVersion() OleQueryClientVersion16
60 pascal -ret16 OleIsDcMeta(word) OleIsDcMeta16
100 stub DOCWNDPROC
101 stub SRVRWNDPROC
102 stub MFCALLBACKFUNC
110 stub DEFLOADFROMSTREAM
111 stub DEFCREATEFROMCLIP
112 stub DEFCREATELINKFROMCLIP
113 stub DEFCREATEFROMTEMPLATE
114 stub DEFCREATE
115 stub DEFCREATEFROMFILE
116 stub DEFCREATELINKFROMFILE
117 stub DEFCREATEINVISIBLE
200 stub LERELEASE
201 stub LESHOW
202 stub LEGETDATA
203 stub LESETDATA
204 stub LESETHOSTNAMES
205 stub LESETTARGETDEVICE
206 stub LESETBOUNDS
207 stub LESAVETOSTREAM
208 stub LECLONE
209 stub LECOPYFROMLINK
210 stub LEEQUAL
211 stub LECOPY
212 stub LEQUERYTYPE
213 stub LEQUERYBOUNDS
214 stub LEDRAW
215 stub LEQUERYOPEN
216 stub LEACTIVATE
218 stub LEUPDATE
219 stub LERECONNECT
220 stub LEENUMFORMAT
221 stub LEQUERYPROTOCOL
222 stub LEQUERYOUTOFDATE
223 stub LEOBJECTCONVERT
224 stub LECHANGEDATA
225 stub LECLOSE
226 stub LEGETUPDATEOPTIONS
227 stub LESETUPDATEOPTIONS
228 stub LEEXECUTE
229 stub LEOBJECTLONG
230 stub LECREATEINVISIBLE
300 stub MFRELEASE
301 stub MFGETDATA
302 stub MFSAVETOSTREAM
303 stub MFCLONE
304 stub MFEQUAL
305 stub MFCOPY
307 stub MFQUERYBOUNDS
308 stub MFDRAW
309 stub MFENUMFORMAT
310 stub MFCHANGEDATA
400 stub BMRELEASE
401 stub BMGETDATA
402 stub BMSAVETOSTREAM
403 stub BMCLONE
404 stub BMEQUAL
405 stub BMCOPY
407 stub BMQUERYBOUNDS
408 stub BMDRAW
409 stub BMENUMFORMAT
410 stub BMCHANGEDATA
500 stub DIBRELEASE
501 stub DIBGETDATA
502 stub DIBSAVETOSTREAM
503 stub DIBCLONE
504 stub DIBEQUAL
505 stub DIBCOPY
507 stub DIBQUERYBOUNDS
508 stub DIBDRAW
509 stub DIBENUMFORMAT
510 stub DIBCHANGEDATA
600 stub GENRELEASE
601 stub GENGETDATA
602 stub GENSETDATA
603 stub GENSAVETOSTREAM
604 stub GENCLONE
605 stub GENEQUAL
606 stub GENCOPY
608 stub GENQUERYBOUNDS
609 stub GENDRAW
610 stub GENENUMFORMAT
611 stub GENCHANGEDATA
701 stub ERRSHOW
702 stub ERRSETDATA
703 stub ERRSETHOSTNAMES
704 stub ERRSETTARGETDEVICE
705 stub ERRSETBOUNDS
706 stub ERRCOPYFROMLINK
707 stub ERRQUERYOPEN
708 stub ERRACTIVATE
709 stub ERRCLOSE
710 stub ERRUPDATE
711 stub ERRRECONNECT
712 stub ERRQUERYPROTOCOL
713 stub ERRQUERYOUTOFDATE
714 stub ERROBJECTCONVERT
715 stub ERRGETUPDATEOPTIONS
716 stub ERRSETUPDATEOPTIONS
717 stub ERREXECUTE
718 stub ERROBJECTLONG
800 stub PBLOADFROMSTREAM
801 stub PBCREATEFROMCLIP
802 stub PBCREATELINKFROMCLIP
803 stub PBCREATEFROMTEMPLATE
804 stub PBCREATE
805 stub PBDRAW
806 stub PBQUERYBOUNDS
807 stub PBCOPYTOCLIPBOARD
808 stub PBCREATEFROMFILE
809 stub PBCREATELINKFROMFILE
810 stub PBENUMFORMATS
811 stub PBGETDATA
812 stub PBCREATEINVISIBLE
910 stub OBJQUERYNAME
911 stub OBJRENAME
912 stub OBJQUERYTYPE
913 stub OBJQUERYSIZE
950 stub CONNECTDLGPROC
951 stub SETNETNAME
952 stub CHECKNETDRIVE
953 stub SETNEXTNETDRIVE
954 stub GETTASKVISIBLEWINDOW
