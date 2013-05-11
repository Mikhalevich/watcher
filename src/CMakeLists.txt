
set(ProjectName watcher)
project(${ProjectName})
cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type(Debug, Release)" FORCE)

# set output path for targets
#if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#	set(EXECUTABLE_OUTPUT ${CMAKE_BINARY_DIR}/bin_d)
#	set(SHARED_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib_d)
#	set(STATIC_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib_d)
#	set(PLUGIN_OUTPUT ${CMAKE_BINARY_DIR}/plugins_d)
#else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#	set(EXECUTABLE_OUTPUT ${CMAKE_BINARY_DIR}/bin)
#	set(SHARED_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib)
#	set(STATIC_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib)
#	set(PLUGIN_OUTPUT ${CMAKE_BINARY_DIR}/plugins)
#endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

set(EXECUTABLE_OUTPUT ${CMAKE_BINARY_DIR}/bin)
set(SHARED_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib)
set(STATIC_LIBRARY_OUTPUT ${CMAKE_BINARY_DIR}/lib)
set(PLUGIN_OUTPUT ${CMAKE_BINARY_DIR}/plugins)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${STATIC_LIBRARY_OUTPUT})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SHARED_LIBRARY_OUTPUT})

function(setOutputPluginDirectory PROJECT_NAME)
	if(WIN32)
		set_target_properties(${PROJECT_NAME}
				PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT}
			)
        else(WIN32)
		set_target_properties(${PROJECT_NAME}
				PROPERTIES
				LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT}
			)
	endif(WIN32)
endfunction(setOutputPluginDirectory)

# directory where save *.ts and *.qm files
set(LANGUAGE_PATH "${CMAKE_SOURCE_DIR}/language")

#add_definitions(-DQT_NO_CAST_FROM_ASCII)

add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(ClientTcpSocket)
add_subdirectory(SettingsWindow)
# ADD_SUBDIRECTORY(PluginExample)
#add_subdirectory(PictureWindow)
