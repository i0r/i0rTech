file( GLOB_RECURSE I0RTECH_COMMON "*.cpp" "*.hpp" "*.c" "*.h" )

add_library( i0rTechCommon STATIC ${I0RTECH_COMMON} )

ADD_MSVC_FILTERS( "${I0RTECH_COMMON}" )

set_target_properties( i0rTechCommon PROPERTIES RELEASE_OUTPUT_NAME "i0rTechCommon${I0RTECH_ARCHITECTURE}" )
set_target_properties( i0rTechCommon PROPERTIES DEBUG_OUTPUT_NAME "i0rTechCommon${I0RTECH_ARCHITECTURE}-debug" )

if(UNIX)
		set_target_properties( i0rTechCommon PROPERTIES OUTPUT_NAME "i0rTechCommon${I0RTECH_ARCHITECTURE}" )
endif(UNIX)

target_include_directories( i0rTechCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
