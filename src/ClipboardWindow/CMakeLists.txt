
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

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Quick REQUIRED)

pluginDefinitions()

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket/"
	)
	
qt5_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt5_add_resources(RCC_SRCS ${RCC_LIST})
qt5_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST})
	
set(LIBRARY_DEPENDENCIES
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${ProjectClipboard} SHARED ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectClipboard} ${LIBRARY_DEPENDENCIES})
qt5_use_modules(${ProjectClipboard} Core Gui Widgets Network LinguistTools Quick)

# make path for plugin
setOutputPluginDirectory(${ProjectClipboard})
