add_library(haxmvm SHARED haxmvm.c)
include_directories(../wine)
add_definitions(-D__i386__ -DNtCurrentTeb=NtCurrentTeb__ -DDECLSPEC_HIDDEN=)
target_link_libraries(haxmvm libwine)
