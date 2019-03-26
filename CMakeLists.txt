cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
cmake_policy( VERSION 2.8 )

#=====================================================================================================================================================

option( IORTECH_FLAG_OPENGL "Build OpenGL library" ON )
							
#=====================================================================================================================================================

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
		if( MSVC )
			GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
			SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
			SET(Sources ${${SourcesVar}})

			SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
					PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
								 OBJECT_OUTPUTS "${PrecompiledBinary}")
			SET_SOURCE_FILES_PROPERTIES(${Sources}
					PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
								 OBJECT_DEPENDS "${PrecompiledBinary}")  

			LIST(APPEND ${SourcesVar} ${PrecompiledSource})
		endif( MSVC )
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

MACRO(ADD_MSVC_FILTERS CurProject)
	set( _src_root_path ${CMAKE_CURRENT_SOURCE_DIR} )
	foreach(_source IN ITEMS ${CurProject})
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
ENDMACRO(ADD_MSVC_FILTERS)

#=====================================================================================================================================================

set( IORTECH i0rTech )
set( I0RTECH_BIN_FOLDER "../bin" )
set( I0RTECH_ARCHITECTURE "86" )

#=====================================================================================================================================================

project( ${IORTECH} )

if( WIN32 )
	if( ${CMAKE_GENERATOR} MATCHES "64" )
		set( I0RTECH_ARCHITECTURE "64" )
	endif( ${CMAKE_GENERATOR} MATCHES "64" )
endif( WIN32 )

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${I0RTECH_BIN_FOLDER} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${I0RTECH_BIN_FOLDER} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${I0RTECH_BIN_FOLDER} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${I0RTECH_BIN_FOLDER} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${I0RTECH_BIN_FOLDER} )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${I0RTECH_BIN_FOLDER} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${I0RTECH_BIN_FOLDER} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${I0RTECH_BIN_FOLDER} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${I0RTECH_BIN_FOLDER} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${I0RTECH_BIN_FOLDER} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${I0RTECH_BIN_FOLDER} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${I0RTECH_BIN_FOLDER} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${I0RTECH_BIN_FOLDER} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${I0RTECH_BIN_FOLDER} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${I0RTECH_BIN_FOLDER} )

#=====================================================================================================================================================

# main
add_subdirectory( Common )
add_subdirectory( Core )
add_subdirectory( Render )

# api specific library
if( IORTECH_FLAG_OPENGL )
	add_subdirectory( OpenGL )
endif( IORTECH_FLAG_OPENGL )

# projects should always be at the end of the cmake file
add_subdirectory( SampleProject )
