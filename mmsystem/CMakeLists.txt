file(GLOB SOURCE *.c *.cpp *.rc)
add_library(mmsystem SHARED ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/mmsystem.def mmsystem.dll16.obj)
include_directories(../wine)
# The in-tree MCI sequencer (mciseq_internal.c) is Wine driver source and needs Wine's
# mmddk.h, which lives in ../wine/windows -- the target only adds ../wine, and MinGW ships
# no mmddk.h. Scope the extra include to that single file so every other translation unit
# keeps building against the MinGW headers exactly as before.
set_source_files_properties(mciseq_internal.c PROPERTIES COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/../wine/windows")
add_definitions(-D_X86_ -D__WINESRC__ -D__i386__ -DHAVE_STRNCASECMP -DHAVE__STRNICMP -D_WINTERNL_ -DNtCurrentTeb=NtCurrentTeb__ -DDECLSPEC_HIDDEN= -Dstrncasecmp=_strnicmp)
spec_build(mmsystem.dll16 mmsystem)
target_link_libraries(mmsystem libwine winecrt0 krnl386 winmm.lib)
set_target_properties(mmsystem PROPERTIES SUFFIX ".dll16")
