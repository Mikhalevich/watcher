
set(ProjectClientSocket ClientTcpSocket)
project(${ProjectClientSocket})

set(HEADER_MAIN_LIST 
	global.h
	clienttcpsocket.h 
    abstractwidget.h 
    basewidget.h 
	responcedata.h
	)
# objectfactory.h

set(HEADER_QUERY_LIST
    querry/windowsquerry.h 
    querry/picturequerry.h 
    querry/getcontrolinfoquerry.h 
    querry/controlsquerry.h 
    querry/abstractquerry.h 
    querry/getmailpropertiesquerry.h 
    querry/executionreportquerry.h 
    querry/getpicturepropertiesquerry.h 
	querry/settingsquery.h
	)
	
set(HEADER_MOC_LIST 
	${HEADER_MAIN_LIST}
	${HEADER_QUERY_LIST}
	)
	
set(SOURCE_MAIN_LIST 
	abstractwidget.cpp
	clienttcpsocket.cpp 
	)
	
set(SOURCE_QUERY_LIST 
	querry/windowsquerry.cpp 
    querry/picturequerry.cpp 
    querry/getcontrolinfoquerry.cpp 
    querry/controlsquerry.cpp 
    querry/getmailpropertiesquerry.cpp 
    querry/executionreportquerry.cpp 
    querry/getpicturepropertiesquerry.cpp
	querry/settingsquery.cpp
	)

find_package(Qt4 REQUIRED QtCore QtGui QtNetwork)
include(${QT_USE_FILE})

qt4_wrap_cpp(SOURCE_MOC_LIST ${HEADER_MOC_LIST})

source_group(main\\headers FILES ${HEADER_MAIN_LIST})
source_group(main\\sources FILES ${SOURCE_MAIN_LIST})
source_group(query\\headers FILES ${HEADER_QUERY_LIST})
source_group(query\\sources FILES ${SOURCE_QUERY_LIST})
source_group(moc FILES ${SOURCE_MOC_LIST})

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_SOURCE_DIR}/../Server/helper/")

add_library(${ProjectClientSocket} STATIC
	${HEADER_MAIN_LIST} ${HEADER_QUERY_LIST} ${SOURCE_MAIN_LIST} ${SOURCE_QUERY_LIST} ${SOURCE_MOC_LIST})	
target_link_libraries(${ProjectClientSocket} ${QT_LIBRARIES})