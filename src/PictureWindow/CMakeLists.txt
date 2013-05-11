
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
    set(TS_LIST ${TS_LIST} "${LANGUAGE_PATH}/${LANGUAGE}.ts")
    #set(QM_LIST ${QM_LIST} "${LANGUAGE}.qm")
endforeach()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_NO_DEBUG)
add_definitions(-DQT_SHARED)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Server/helper/")
	
qt5_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt5_wrap_ui(HEADER_UI_LIST ${UI_LIST})
qt5_add_resources(RCC_SRCS ${RCC_LIST})
qt5_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST} ${HEADER_UI_LIST})
	
set(LIBRARY_DEPENDENCIES
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(${ProjectName} SHARED ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${HEADER_UI_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectName} ${LIBRARY_DEPENDENCIES})
qt5_use_modules(${ProjectName} Core Gui Widgets Network LinguistTools)

# make path for plugin
setOutputPluginDirectory(${ProjectName})
