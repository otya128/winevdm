file(GLOB SOURCE *.c)
add_executable(convspec ${SOURCE})
include_directories(../wine)
add_definitions(-D__i386__ -Dpopen=_popen -Dpclose=_pclose
 -Dstrncasecmp=_strnicmp -Dstrcasecmp=_stricmp)
if (MSVC)
    add_definitions(-DWINEBUILD_MSVC)
endif()

