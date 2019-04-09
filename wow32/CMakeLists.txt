file(GLOB SOURCE *.c *.cpp)
add_library(wow32 SHARED ${SOURCE} wow32.def)
add_definitions(-D_X86_ -D__WINESRC__ -D__i386__ -DUSE_COMPILER_EXCEPTIONS -D_WINTERNL_ -DNtCurrentTeb=NtCurrentTeb__ -Dinline=__inline)

