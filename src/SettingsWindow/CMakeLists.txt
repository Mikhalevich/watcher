
set(ProjectSettings SettingsWindow)
project(${ProjectSettings})

set(HEADER_LIST )
	
set(SOURCE_LIST settingswindow.cpp
    )

set(HEADER_MOC_LIST settingswindow.h
	)

set(RCC_LIST settingswindow.qrc)

set(LANGUAGE_LIST SettingsWindow_ru)
foreach (LANGUAGE ${LANGUAGE_LIST})
    set(TS_LIST ${TS_LIST} "${LANGUAGE_PATH}/${LANGUAGE}.ts")
    #set(QM_LIST ${QM_LIST} "${LANGUAGE}.qm")
endforeach()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

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

add_library(${ProjectSettings} SHARED ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectSettings} ${LIBRARY_DEPENDENCIES})
qt5_use_modules(${ProjectSettings} Core Gui Widgets Network LinguistTools)

# make path for plugin
setOutputPluginDirectory(${ProjectSettings})
