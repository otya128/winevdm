add_library(vm86 SHARED msdos.cpp vm86.def)
include_directories(../wine)
add_definitions(-D__i386__ -DHAS_I386 -DSUPPORT_FPU -DNtCurrentTeb=NtCurrentTeb__ -DDECLSPEC_HIDDEN=)
target_link_libraries(vm86 libwine)
