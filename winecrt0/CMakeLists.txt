file(GLOB SOURCE *.c)
add_library(winecrt0 ${SOURCE})
include_directories(../wine)

add_definitions(-D_X86_ -D__i386__ -D__WINESRC__ -DNtCurrentTeb=NtCurrentTeb__)

