file(GLOB SOURCE *.c)
include_directories(../wine .)
bison_target(wpp_parser ppy.y ${CMAKE_CURRENT_BINARY_DIR}/ppy.tab.c COMPILE_FLAGS "-p ppy_")
flex_target(wpp_scanner ppl.l ${CMAKE_CURRENT_BINARY_DIR}/ppl.c)
add_flex_bison_dependency(wpp_scanner wpp_parser)
add_library(wpp ${SOURCE} ${BISON_wpp_parser_OUTPUTS} ${FLEX_wpp_scanner_OUTPUTS})
add_definitions(-D_X86_ -D__i386__ -Dpopen=_popen -Dpclose=_pclose -Dstrncasecmp=_strnicmp -Dstrcasecmp=_stricmp)
target_link_libraries(wpp libwine)

