
set(ProjectName Client)
project(${ProjectName})

set(HEADER_LIST databasemanager.h
	databasetypes.h
	)
	
set(SOURCE_LIST main.cpp 
    mainwindow.cpp 
    connectdialog.cpp 
    aurorizationdialog.cpp
	databasemanager.cpp
	)

set(HEADER_MOC_LIST mainwindow.h 
    connectdialog.h 
    aurorizationdialog.h)
	
set(UI_LIST mainwindow.ui)

set(RCC_LIST client.qrc)

set(LANGUAGE_LIST Client_en Client_ru)
foreach (LANGUAGE ${LANGUAGE_LIST})
    set(TS_LIST ${TS_LIST} "${LANGUAGE_PATH}/${LANGUAGE}.ts")
    #set(QM_LIST ${QM_LIST} "${LANGUAGE}.qm")
endforeach()

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

# removes console from build	
ADD_DEFINITIONS(-DQT_DLL)

qt5_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt5_wrap_ui(HEADER_UI_LIST ${UI_LIST})
qt5_add_resources(RCC_SRCS ${RCC_LIST})
qt5_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST} ${HEADER_UI_LIST})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket/"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Server/helper/")
	
set(LIBRARY_DEPENDENCIES
	${QT_QTMAIN_LIBRARY} # removes console from build
	ClientTcpSocket)
	
link_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(${ProjectName} WIN32 ${HEADER_LIST} ${SOURCE_LIST} 
        ${HEADER_MOC_LIST} ${SOURCE_MOC_LIST} ${HEADER_UI_LIST} ${RCC_SRCS} ${QM_LIST})
target_link_libraries(${ProjectName} ${LIBRARY_DEPENDENCIES})
qt5_use_modules(${ProjectName} Core Gui Widgets Network Test Sql LinguistTools)
