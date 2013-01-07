
set(ProjectName watcher)
project(${ProjectName})
cmake_minimum_required(VERSION 2.8.2)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type(Debug, Release)" FORCE)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin_d)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib_d)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins_d)
else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")

#add_definitions(-DQT_NO_CAST_FROM_ASCII)

add_subdirectory(Server)
add_subdirectory(library2)
add_subdirectory(Client)
add_subdirectory(ClientTcpSocket)
add_subdirectory(ActionWindow)
#ADD_SUBDIRECTORY(PluginExample)
add_subdirectory(PictureWindow)
