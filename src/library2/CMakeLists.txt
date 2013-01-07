
set(ProjectName library2)
project(${ProjectName})

add_definitions(-DLIBRARY2_LIBRARY)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

set(HEADER_LIST library2.h
	library2_global.h)
	
set(SOURCE_LIST library2.cpp)

find_package(Qt4 REQUIRED QtCore)
include(${QT_USE_FILE})

add_library(${ProjectName} SHARED
	${HEADER_LIST} ${SOURCE_LIST})	
target_link_libraries(${ProjectName} ${QT_LIBRARIES})

if(WIN32)
	set_target_properties(${ProjectName}
		PROPERTIES
		COMPILE_FLAGS "/Zc:wchar_t-"
	)
endif(WIN32)