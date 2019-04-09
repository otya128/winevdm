file(GLOB SOURCE *.c)
add_library(libwine SHARED ${SOURCE} wine.def)

add_definitions(-D_X86_ -D__i386__ -D__WINESRC__ -Dinline=__inline)
target_link_libraries(libwine shlwapi.lib)
