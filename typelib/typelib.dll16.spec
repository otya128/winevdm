2  stub CREATETYPELIB
3  pascal LoadTypeLib(ptr ptr) LoadTypeLib16
4  pascal LHashValOfNameSys(word long str) LHashValOfNameSys16
5  variable _IID_ICreateTypeInfo(0x00020405 0x00000000 0x000000c0 0x46000000)
6  variable _IID_ICreateTypeLib(0x00020406 0x00000000 0x000000c0 0x46000000)
7  variable _IID_ITypeComp(0x00020403 0x00000000 0x000000c0 0x46000000)
8  variable _IID_ITypeInfo(0x00020401 0x00000000 0x000000c0 0x46000000)
9  variable _IID_ITypeLib(0x00020402 0x00000000 0x000000c0 0x46000000)
10 pascal RegisterTypeLib(segptr ptr ptr) RegisterTypeLib16
11 pascal LoadRegTypeLib(ptr word word long ptr) LoadRegTypeLib16
14 pascal QueryPathOfRegTypeLib(ptr word word long ptr) QueryPathOfRegTypeLib16
15 pascal OaBuildVersion() OaBuildVersion16
