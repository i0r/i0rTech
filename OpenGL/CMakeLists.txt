file( GLOB_RECURSE I0RTECH_OPENGL "*.cpp" "*.hpp" "*.manifest" "*.h" "*.rc" )

ADD_MSVC_FILTERS( "${I0RTECH_OPENGL}" )

list(REMOVE_ITEM I0RTECH_OPENGL "${CMAKE_CURRENT_SOURCE_DIR}/Common.hpp" )
list(REMOVE_ITEM I0RTECH_OPENGL "${CMAKE_CURRENT_SOURCE_DIR}/Common.cpp" )

ADD_MSVC_PRECOMPILED_HEADER( "Common.hpp" "Common.cpp" I0RTECH_OPENGL )

add_library( i0rTechOpenGL MODULE ${I0RTECH_OPENGL} )
target_link_libraries( i0rTechOpenGL i0rTechCommon )

set_target_properties( i0rTechOpenGL PROPERTIES RELEASE_OUTPUT_NAME "i0rTechOpenGL${I0RTECH_ARCHITECTURE}" )
set_target_properties( i0rTechOpenGL PROPERTIES DEBUG_OUTPUT_NAME "i0rTechOpenGL${I0RTECH_ARCHITECTURE}-debug" )

target_include_directories( i0rTechOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )