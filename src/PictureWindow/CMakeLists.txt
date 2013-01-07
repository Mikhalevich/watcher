
set(ProjectName PictureWindow)
project(${ProjectName})

set(HEADER_LIST )
	
set(SOURCE_LIST picturewindow.cpp
    pictureswidget.cpp
    propertydialog.cpp)

set(HEADER_MOC_LIST picturewindow.h
    pictureswidget.h
    propertydialog.h)
	
set(UI_LIST picturewindow.ui
    propertydialog.ui)

set(RCC_LIST picturewindow.qrc)

set(LANGUAGE_LIST PictureWindow_ru)
foreach (LANGUAGE ${LANGUAGE_LIST})
    set(TS_LIST ${TS_LIST} "${CMAKE_SOURCE_DIR}/language/${LANGUAGE}.ts")
    #set(QM_LIST ${QM_LIST} "${LANGUAGE}.qm")
endforeach()

find_package(Qt4 REQUIRED 
	QtCore 
	QtGui 
	QtNetwork
	QtTest)
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
qt4_add_resources(RCC_SRCS ${RCC_LIST})
qt4_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST} ${HEADER_UI_LIST})
	
set(LIBRARY_DEPENDENCIES
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${ProjectName} SHARED ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${HEADER_UI_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectName} ${QT_LIBRARIES} ${LIBRARY_DEPENDENCIES})

if (WIN32)
	if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set_target_properties(${ProjectName}
			PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins_d"
		)
	else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		set_target_properties(${ProjectName}
			PROPERTIES
			RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
		)
	endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endif(WIN32)
