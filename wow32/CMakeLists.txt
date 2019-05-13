
include_directories(../wine)
file(GLOB SOURCE *.c *.cpp)
def_file_build32(wow32)
add_library(wow32 SHARED ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/wow32.def)
add_definitions(-D_X86_ -D__WINESRC__ -D__i386__ -D_WINTERNL_ -DNtCurrentTeb=NtCurrentTeb__)
add_dependencies(wow32 generate_def_wow32 krnl386)
target_link_libraries(wow32 krnl386)
set_target_properties(wow32 PROPERTIES PREFIX "")
