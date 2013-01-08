
set(ProjectName ActionWindow)
project(${ProjectName})

set(HEADER_LIST )
	
set(SOURCE_LIST actionwindow.cpp)

set(HEADER_MOC_LIST actionwindow.h)
	
set(UI_LIST actionwindow.ui)

find_package(Qt4 REQUIRED 
	QtCore 
	QtGui 
	QtNetwork)
include(${QT_USE_FILE})

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Server/helper/")
	
qt4_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt4_wrap_ui(HEADER_UI_LIST ${UI_LIST})
	
set(LIBRARY_DEPENDENCIES
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${ProjectName} SHARED ${HEADER_LIST} ${SOURCE_LIST} ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${HEADER_UI_LIST})	
target_link_libraries(${ProjectName} ${QT_LIBRARIES} ${LIBRARY_DEPENDENCIES})

# make path for plugin
setOutputPluginDirectory(${ProjectName})