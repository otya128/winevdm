1 stub DLLGETCLASSOBJECT
2 pascal SysAllocString(str)		SysAllocString16
3 pascal SysReallocString(ptr str)	SysReAllocString16
4 pascal SysAllocStringLen(str word)	SysAllocStringLen16
5 pascal SysReAllocStringLen(ptr str word) SysReAllocStringLen16
6 pascal SysFreeString(segstr)		SysFreeString16
7 pascal SysStringLen(segstr)		SysStringLen16
8 pascal VariantInit(ptr) VariantInit16
9 pascal VariantClear(ptr) VariantClear16
10 pascal VariantCopy(ptr ptr) VariantCopy16
11 pascal VariantCopyInd(ptr ptr) VariantCopyInd16
12 pascal VariantChangeType(ptr ptr word word) VariantChangeType16
13 stub VARIANTTIMETODOSDATETIME
14 stub DOSDATETIMETOVARIANTTIME
15 pascal SafeArrayCreate(word word ptr) SafeArrayCreate16
16 pascal SafeArrayDestroy(segptr) SafeArrayDestroy16
17 pascal -ret16 SafeArrayGetDim(ptr) SafeArrayGetDim16
18 pascal -ret16 SafeArrayGetElemsize(ptr) SafeArrayGetElemsize16
19 pascal SafeArrayGetUBound(ptr word ptr) SafeArrayGetUBound16
20 pascal SafeArrayGetLBound(ptr word ptr) SafeArrayGetLBound16
21 pascal SafeArrayLock(ptr) SafeArrayLock16
22 pascal SafeArrayUnlock(ptr) SafeArrayUnlock16
23 pascal SafeArrayAccessData(ptr ptr) SafeArrayAccessData16
24 pascal SafeArrayUnaccessData(ptr) SafeArrayUnaccessData16
25 stub SAFEARRAYGETELEMENT
26 stub SAFEARRAYPUTELEMENT
27 pascal SafeArrayCopy(ptr ptr) SafeArrayCopy16
28 pascal DispGetParam(ptr word word ptr ptr) DispGetParam16
29 pascal DispGetIDsOfNames(segptr segptr word segptr) DispGetIDsOfNames16
30 pascal DispInvoke(segptr segptr long word segptr segptr segptr segptr) DispInvoke16
31 pascal CreateDispTypeInfo(ptr long ptr) CreateDispTypeInfo16
32 pascal CreateStdDispatch(segptr segptr segptr ptr) CreateStdDispatch16
33 variable _IID_IDispatch(0x00020400 0x00000000 0x000000c0 0x46000000)
34 variable _IID_IEnumVARIANT(0x00020404 0x00000000 0x000000c0 0x46000000)
35 pascal RegisterActiveObject(segptr ptr long ptr) RegisterActiveObject16
36 pascal RevokeActiveObject(long segptr) RevokeActiveObject16
37 pascal GetActiveObject(ptr segptr ptr) GetActiveObject16
38 pascal SafeArrayAllocDescriptor(word ptr) SafeArrayAllocDescriptor16
39 pascal SafeArrayAllocData(ptr) SafeArrayAllocData16
40 pascal SafeArrayDestroyDescriptor(segptr) SafeArrayDestroyDescriptor16
41 pascal SafeArrayDestroyData(ptr) SafeArrayDestroyData16
42 pascal SafeArrayRedim(ptr ptr) SafeArrayRedim16
43 pascal VarI2FromI4(long ptr) VarI2FromI416
44 pascal VarI2FromR4(float ptr) VarI2FromR416
45 pascal VarI2FromR8(double ptr) VarI2FromR816
46 pascal VarI2FromCy(double ptr) VarI2FromCy16
47 pascal VarI2FromDate(double ptr) VarI2FromDate16
48 pascal VarI2FromStr(str long long ptr) VarI2FromStr16
49 stub VARI2FROMDISP
50 pascal VarI2FromBool(word ptr) VarI2FromBool16
51 stub VARI4FROMI2
52 stub VARI4FROMR4
53 stub VARI4FROMR8
54 stub VARI4FROMCY
55 stub VARI4FROMDATE
56 pascal VarI4FromStr(str long long ptr) VarI4FromStr16
57 stub VARI4FROMDISP
58 stub VARI4FROMBOOL
59 stub VARR4FROMI2
60 stub VARR4FROMI4
61 stub VARR4FROMR8
62 stub VARR4FROMCY
63 stub VARR4FROMDATE
64 pascal VarR4FromStr(str long long ptr) VarR4FromStr16
65 stub VARR4FROMDISP
66 stub VARR4FROMBOOL
67 stub VARR8FROMI2
68 stub VARR8FROMI4
69 stub VARR8FROMR4
70 stub VARR8FROMCY
71 stub VARR8FROMDATE
72 stub VARR8FROMSTR
73 stub VARR8FROMDISP
74 stub VARR8FROMBOOL
75 stub VARDATEFROMI2
76 stub VARDATEFROMI4
77 stub VARDATEFROMR4
78 stub VARDATEFROMR8
79 stub VARDATEFROMCY
80 stub VARDATEFROMSTR
81 stub VARDATEFROMDISP
82 stub VARDATEFROMBOOL
83 stub VARCYFROMI2
84 stub VARCYFROMI4
85 stub VARCYFROMR4
86 stub VARCYFROMR8
87 stub VARCYFROMDATE
88 stub VARCYFROMSTR
89 stub VARCYFROMDISP
90 stub VARCYFROMBOOL
91 pascal VarBstrFromI2(s_word long long ptr) VarBstrFromI216
92 pascal VarBstrFromI4(long long long ptr) VarBstrFromI416
93 pascal VarBstrFromR4(long long long ptr) VarBstrFromR416
94 pascal VarBstrFromR8(double long long ptr) VarBstrFromR816
95 pascal VarBstrFromCy(double long long ptr) VarBstrFromCy16
96 pascal VarBstrFromDate(double long long ptr) VarBstrFromDate16
97 pascal VarBstrFromDisp(segptr long long ptr) VarBstrFromDisp16
98 pascal VarBstrFromBool(word long long ptr) VarBstrFromBool16
99 stub VARBOOLFROMI2
100 stub VARBOOLFROMI4
101 stub VARBOOLFROMR4
102 stub VARBOOLFROMR8
103 stub VARBOOLFROMDATE
104 stub VARBOOLFROMCY
105 stub VARBOOLFROMSTR
106 stub VARBOOLFROMDISP
107 stub DOINVOKEMETHOD
108 pascal VariantChangeTypeEx(ptr ptr long word word) VariantChangeTypeEx16
109 stub SAFEARRAYPTROFINDEX
110 pascal SetErrorInfo(long segptr) SetErrorInfo16
111 pascal GetErrorInfo(long ptr)  GetErrorInfo16
112 pascal CreateErrorInfo(ptr) CreateErrorInfo16
113 variable _IID_IErrorInfo(0x1cf2b120 0x101b547d 0x0008658e 0x19d12b2b)
114 variable _IID_ICreateErrorInfo(0x22f03340 0x101b547d 0x0008658e 0x19d12b2b)
115 variable _IID_ISupportErrorInfo(0xdf0b3d60 0x101b548f 0x0008658e 0x19d12b2b)
116 pascal VarUI1FromI2(word ptr) VarUI1FromI216
117 pascal VarUI1FromI4(long ptr) VarUI1FromI416
118 pascal VarUI1FromR4(long ptr) VarUI1FromR416
119 pascal VarUI1FromR8(double ptr) VarUI1FromR816
120 pascal VarUI1FromCy(int64 ptr) VarUI1FromCy16
121 pascal VarUI1FromDate(double ptr) VarUI1FromDate16
122 pascal VarUI1FromStr(str long long ptr) VarUI1FromStr16
123 pascal VarUI1FromDisp(segptr long ptr) VarUI1FromDisp16
124 pascal VarUI1FromBool(word ptr) VarUI1FromBool16
125 pascal VarI2FromUI1(word ptr) VarI2FromUI116
126 stub VARI4FROMUI1
127 stub VARR4FROMUI1
128 stub VARR8FROMUI1
129 stub VARDATEFROMUI1
130 stub VARCYFROMUI1
131 pascal VarBstrFromUI1(word long long ptr) VarBstrFromUI116
132 stub VARBOOLFROMUI1
133 stub DLLCANUNLOADNOW
#134 stub WEP
#135 stub ___EXPORTEDSTUB

@ stdcall -arch=win32 SysAllocString16(str)
@ stdcall -arch=win32 SysFreeString16(long)
@ stdcall -arch=win32 LoadTypeLib16Impl(wstr ptr)
@ stdcall -arch=win32 SysStringLen16(long)
@ stdcall -arch=win32 SysAllocStringLen16(str long)
