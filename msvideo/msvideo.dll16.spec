#1 pascal WEP(word)
2 pascal VideoForWindowsVersion() VideoForWindowsVersion
3 pascal DllEntryPoint(long word word word long word) VIDEO_LibMain
20 pascal videoGetNumDevs() videoGetNumDevs16
21 pascal videoGetErrorText(word word str word) videoGetErrorText16
22 pascal VideoCapDriverDescAndVer(word ptr word ptr word) VideoCapDriverDescAndVer16
28 pascal videoOpen(ptr long long) videoOpen16
29 pascal videoClose(word) videoClose16
30 pascal videoDialog(word word long) videoDialog16
31 pascal videoFrame(word ptr) videoFrame16
32 pascal videoConfigure(word word long ptr ptr long ptr long) videoConfigure16
33 pascal videoConfigureStorage(word str long) videoConfigureStorage16
34 pascal videoGetChannelCaps(word ptr long) videoGetChannelCaps16
35 pascal videoUpdate(word word word) videoUpdate16
40 pascal videoStreamAddBuffer(word ptr long) videoStreamAddBuffer16
41 pascal videoStreamFini(word) videoStreamFini16
42 pascal videoStreamGetError(word ptr ptr) videoStreamGetError16
43 pascal videoStreamGetPosition(word ptr long) videoStreamGetPosition16
44 pascal videoStreamInit(word long long long long) videoStreamInit16
46 pascal videoStreamPrepareHeader(word ptr long) videoStreamPrepareHeader16
47 pascal videoStreamReset(word) videoStreamReset16
49 pascal videoStreamStart(word) videoStreamStart16
50 pascal videoStreamStop(word) videoStreamStop16
51 pascal videoStreamUnprepareHeader(word ptr long) videoStreamUnprepareHeader16
52 pascal videoStreamAllocHdrAndBuffer(word ptr long) videoStreamAllocHdrAndBuffer16
53 pascal videoStreamFreeHdrAndBuffer(word ptr) videoStreamFreeHdrAndBuffer16
60 pascal videoMessage(word word long long) videoMessage16
102 pascal -ret16 DrawDibOpen() DrawDibOpen16
103 pascal -ret16 DrawDibClose(word) DrawDibClose16
104 pascal -ret16 DrawDibBegin(word word s_word s_word ptr s_word s_word word) DrawDibBegin16
105 pascal -ret16 DrawDibEnd(word) DrawDibEnd16
106 pascal -ret16 DrawDibDraw(word word s_word s_word s_word s_word ptr ptr s_word s_word s_word s_word word) DrawDibDraw16
108 pascal -ret16 DrawDibGetPalette(word) DrawDibGetPalette16
110 pascal -ret16 DrawDibSetPalette(word word) DrawDibSetPalette16
111 stub DRAWDIBCHANGEPALETTE
112 pascal -ret16 DrawDibRealize(word word word) DrawDibRealize16
113 stub DRAWDIBTIME
114 pascal -ret16 DrawDibProfileDisplay(ptr) DrawDibProfileDisplay16
115 stub STRETCHDIB
118 pascal -ret16 DrawDibStart(word long) DrawDibStart16
119 pascal -ret16 DrawDibStop(word) DrawDibStop16
120 stub DRAWDIBGETBUFFER
200 pascal -ret16 ICInfo(long long segptr) ICInfo16
201 stub ICINSTALL
202 stub ICREMOVE
203 pascal -ret16 ICOpen(long long word) ICOpen16
204 pascal ICClose(word) ICClose16
205 pascal ICSendMessage(word word long long) ICSendMessage16
206 pascal -ret16 ICOpenFunction(long long word segptr) ICOpenFunction16
207 varargs _ICMessage(word word word) ICMessage16
212 pascal ICGetInfo(word segptr long) ICGetInfo16
213 pascal -ret16 ICLocate(long long ptr ptr word) ICLocate16
224 cdecl _ICCompress(word long segptr segptr segptr segptr segptr segptr long long long segptr segptr) ICCompress16
230 cdecl _ICDecompress(word long segptr segptr segptr segptr) ICDecompress16
232 cdecl _ICDrawBegin(word long word word word s_word s_word s_word s_word segptr s_word s_word s_word s_word long long) ICDrawBegin16
234 cdecl _ICDraw(word long segptr segptr long long) ICDraw16
239 pascal -ret16 ICGetDisplayFormat(word ptr ptr s_word s_word s_word) ICGetDisplayFormat16
240 stub ICIMAGECOMPRESS
241 stub ICIMAGEDECOMPRESS
242 stub ICCOMPRESSORCHOOSE
243 stub ICCOMPRESSORFREE
244 stub ICSEQCOMPRESSFRAMESTART
245 stub ICSEQCOMPRESSFRAMEEND
246 stub ICSEQCOMPRESSFRAME
250 cdecl _MCIWndCreate(word word long str) MCIWndCreate16
251 cdecl _MCIWndRegisterClass() MCIWndRegisterClass16
252 stub GETOPENFILENAMEPREVIEW
253 stub GETSAVEFILENAMEPREVIEW
