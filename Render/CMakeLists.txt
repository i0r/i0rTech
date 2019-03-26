file( GLOB_RECURSE I0RTECH_RENDER "*.cpp" "*.hpp" "*.manifest" "*.h" "*.rc" )

ADD_MSVC_FILTERS( "${I0RTECH_RENDER}" )

list(REMOVE_ITEM I0RTECH_RENDER "${CMAKE_CURRENT_SOURCE_DIR}/Common.hpp" )
list(REMOVE_ITEM I0RTECH_RENDER "${CMAKE_CURRENT_SOURCE_DIR}/Common.cpp" )

ADD_MSVC_PRECOMPILED_HEADER( "Common.hpp" "Common.cpp" I0RTECH_RENDER )

add_library( i0rTechRender MODULE ${I0RTECH_RENDER} )
target_link_libraries( i0rTechRender i0rTechCommon )

set_target_properties( i0rTechRender PROPERTIES RELEASE_OUTPUT_NAME "i0rTechRender${I0RTECH_ARCHITECTURE}" )
set_target_properties( i0rTechRender PROPERTIES DEBUG_OUTPUT_NAME "i0rTechRender${I0RTECH_ARCHITECTURE}-debug" )

target_include_directories( i0rTechRender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )