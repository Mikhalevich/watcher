
set(ProjectClipboard ClipboardWindow)
project(${ProjectClipboard})

set(HEADER_LIST )
	
set(SOURCE_LIST clipboardwindow.cpp
    )

set(HEADER_MOC_LIST clipboardwindow.h
	)

set(RCC_LIST clipboardwindow.qrc)

set(LANGUAGE_LIST ClipboardWindow_ru)
foreach (LANGUAGE ${LANGUAGE_LIST})
    set(TS_LIST ${TS_LIST} "${LANGUAGE_PATH}/${LANGUAGE}.ts")
    #set(QM_LIST ${QM_LIST} "${LANGUAGE}.qm")
endforeach()

find_package(Qt4 REQUIRED 
	QtCore 
	QtGui 
	QtNetwork
	QtDeclarative
	)
include(${QT_USE_FILE})

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket/"
	)
	
qt4_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt4_add_resources(RCC_SRCS ${RCC_LIST})
qt4_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST})
	
set(LIBRARY_DEPENDENCIES
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${ProjectClipboard} SHARED ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectClipboard} ${QT_LIBRARIES} ${LIBRARY_DEPENDENCIES})

# make path for plugin
setOutputPluginDirectory(${ProjectClipboard})
