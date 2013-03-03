
set(ProjectServer Server)
project(${ProjectServer})

set(MAIN_HEADER_LIST)

set(NETWORK_HEADER_LIST network/tcpserver.h
    network/tcpsocket.h
	network/socketthread.h
	network/querythread.h
	)

set(NETWORK_QUERY_HEADER_LIST network/query/networkquery.h
    network/query/picturequery.h
	network/query/allpicturesquery.h
    network/query/currentdatepicturesquery.h
    network/query/periodpicturesquery.h
	network/query/autorizationquery.h
    network/query/mailpropertiesquery.h
    network/query/getmailpropertiesquery.h
    network/query/getpicturepropertiesquery.h
    network/query/timedpicturesquery.h
	network/query/settingsquery.h
	network/query/clipboardquery.h
	)
	
set(GUI_HEADER_LIST gui/traysettings.h
    gui/usersettings.h
    gui/adduserdialog.h
    gui/settingsdialog.h
    gui/commonsettingsdialog.h
	gui/autorizationdialog.h
	)
	
set(DATABASE_HEADER_LIST database/databasemanager.h
	database/databasetypes.h
	)
	
set(DATABASE_QUERY_HEADER_LIST database/query/abstractdatabasequery.h
	database/query/userquery.h
	database/query/picturequery.h
	database/query/autorizationquery.h
	database/query/databasequerycommon.h
	database/query/mailquery.h
	database/query/picturepropertiesquery.h
	database/query/settingsquery.h
	)
	
set(MAIN_HEADER_LIST picturesingleton.h
    picturetimer.h
    singletonthread.h
    commonfunctions.h
    helper/objectfactory.h
	clipboardsingleton.h
	)

set(MOC_HEADER_LIST ${NETWORK_HEADER_LIST}
	${NETWORK_QUERY_HEADER_LIST}
	${GUI_HEADER_LIST}
	${DATABASE_HEADER_LIST}
	${DATABASE_QUERY_HEADER_LIST}
	${MAIN_HEADER_LIST}
	)

set(NETWORK_SOURCE_LIST network/tcpserver.cpp 
    network/tcpsocket.cpp
	network/querythread.cpp
	)
	
set(NETWORK_QUERY_SOURCE_LIST network/query/picturequery.cpp  
	network/query/allpicturesquery.cpp  
    network/query/currentdatepicturesquery.cpp 
    network/query/periodpicturesquery.cpp
	network/query/autorizationquery.cpp 
    network/query/mailpropertiesquery.cpp 
    network/query/getmailpropertiesquery.cpp 
    network/query/getpicturepropertiesquery.cpp 
    network/query/timedpicturesquery.cpp
	network/query/settingsquery.cpp
	network/query/clipboardquery.cpp
	)
	
set(GUI_SOURCE_LIST gui/traysettings.cpp 
    gui/usersettings.cpp 
    gui/adduserdialog.cpp 
    gui/settingsdialog.cpp 
    gui/commonsettingsdialog.cpp 
	gui/autorizationdialog.cpp
	)
	
set(DATABASE_SOURCE_LIST database/databasemanager.cpp)

set(DATABASE_QUERY_SOURCE_LIST database/query/userquery.cpp
	database/query/picturequery.cpp
	database/query/autorizationquery.cpp
	database/query/databasequerycommon.cpp
	database/query/mailquery.cpp
	database/query/picturepropertiesquery.cpp
	database/query/settingsquery.cpp
	)
	
set(MAIN_SOURCE_LIST main.cpp     
    picturetimer.cpp   
    commonfunctions.cpp 
	clipboardsingleton.cpp
	)

set(SOURCE_LIST ${NETWORK_SOURCE_LIST}
	${NETWORK_QUERY_SOURCE_LIST}
	${GUI_SOURCE_LIST}
	${DATABASE_SOURCE_LIST}
	${DATABASE_QUERY_SOURCE_LIST}
	${MAIN_SOURCE_LIST}
    )

set(UI_LIST gui/commonsettingsdialog.ui)

set(RCC_LIST server.qrc)
set(QM_FILES ) #nofile_ru.qm)
set(TS_FILES ) #nofile_ru.ts)

# Python libs
find_package(PythonLibs REQUIRED)
# Qt
find_package(Qt4 REQUIRED 
	QtCore 
	QtGui 
	QtNetwork 
	QtSql)
	
include(${QT_USE_FILE})

# removes console from build	
ADD_DEFINITIONS(-DQT_DLL)

qt4_wrap_ui(UI_HEADERS ${UI_LIST})
qt4_wrap_cpp(MOC_SRCS ${MOC_HEADER_LIST})
qt4_add_resources(RCC_SRCS ${RCC_LIST})

source_group(main\\headers FILES ${MAIN_HEADER_LIST})
source_group(main\\sources FILES ${MAIN_SOURCE_LIST})
source_group(network\\headers FILES ${NETWORK_HEADER_LIST})
source_group(network\\sources FILES ${NETWORK_SOURCE_LIST})
source_group(network\\query\\headers FILES ${NETWORK_QUERY_HEADER_LIST})
source_group(network\\query\\sources FILES ${NETWORK_QUERY_SOURCE_LIST})
source_group(gui\\headers FILES ${GUI_HEADER_LIST})
source_group(gui\\sources FILES ${GUI_SOURCE_LIST})
source_group(database\\headers FILES ${DATABASE_HEADER_LIST})
source_group(database\\sources FILES ${DATABASE_SOURCE_LIST})
source_group(database\\query\\headers FILES ${DATABASE_QUERY_HEADER_LIST})
source_group(database\\query\\sources FILES ${DATABASE_QUERY_SOURCE_LIST})
source_group(moc FILES ${MOC_SRCS})

link_directories(${CMAKE_CURRENT_BINARY_DIR})
	
set(LIBRARY_DEPENDENCIES 
	${QT_QTMAIN_LIBRARY} # removes console from build
	)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${PYTHON_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/../library2"
    "${CMAKE_CURRENT_SOURCE_DIR}/../ClientTcpSocket"
	"${CMAKE_CURRENT_SOURCE_DIR}/database")

add_executable(${ProjectServer} WIN32 ${SOURCE_LIST} ${MOC_HEADER_LIST} ${MOC_SRCS} ${UI_HEADERS} ${RCC_SRCS} ${QM_FILES})
target_link_libraries(${ProjectServer} ${QT_LIBRARIES} ${PYTHON_LIBRARIES} ${LIBRARY_DEPENDENCIES})
add_dependencies(${ProjectServer} ${LIBRARY_DEPENDENCIES})

#add_custom_target(translations DEPENDS ${QM_FILES})
#qt4_add_translation(${QM_FILES} ${SOURCES} ${MOC_SRCS} ${UI_HEADERS} ${TS_FILES}) 
