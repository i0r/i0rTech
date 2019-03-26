file( GLOB_RECURSE SAMPLE_PROJECT "*.h" "*.hpp" "*.cpp" "*.manifest" )
add_executable( SampleProject ${GUI_TYPE} ${SAMPLE_PROJECT} )
target_link_libraries( SampleProject i0rTechCommon )

ADD_MSVC_FILTERS( "${SAMPLE_PROJECT}" )

if(WIN32)
		set_target_properties( SampleProject PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
elseif(UNIX)
		set_target_properties( SampleProject PROPERTIES LINK_FLAGS "-Wl,-z,defs" )
		set_target_properties( SampleProject PROPERTIES OUTPUT_NAME "SampleProject${I0RTECH_ARCHITECTURE}" )
endif(WIN32)

set_target_properties( SampleProject PROPERTIES RELEASE_OUTPUT_NAME "SampleProject${I0RTECH_ARCHITECTURE}" )
set_target_properties( SampleProject PROPERTIES DEBUG_OUTPUT_NAME "SampleProject${I0RTECH_ARCHITECTURE}-debug" )

target_include_directories( SampleProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
