
set(ProjectName Client)
project(${ProjectName})

set(HEADER_LIST )
	
set(SOURCE_LIST main.cpp 
    mainwindow.cpp 
    connectdialog.cpp 
    aurorizationdialog.cpp)

set(HEADER_MOC_LIST mainwindow.h 
    connectdialog.h 
    aurorizationdialog.h)
	
set(UI_LIST mainwindow.ui)

set(RCC_LIST client.qrc)

set(LANGUAGE_LIST Client_en Client_ru)
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

# removes console from build	
ADD_DEFINITIONS(-DQT_DLL)

qt4_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})
qt4_wrap_ui(HEADER_UI_LIST ${UI_LIST})
qt4_add_resources(RCC_SRCS ${RCC_LIST})
qt4_create_translation(QM_LIST ${TS_LIST} ${HEADER_MOC_LIST} ${SOURCE_LIST} ${HEADER_UI_LIST})

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
target_link_libraries(${ProjectName} ${QT_LIBRARIES} ${LIBRARY_DEPENDENCIES})
