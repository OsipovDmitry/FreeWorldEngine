include_directories(../3rdparty)
include_directories(../include/utility)

set(HEADERS ../include/utility/UtilitySettings.h
	    ../include/utility/XML.h)

set(SOURCES XML.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DUTILITY_EXPORT)

add_library(FWEUtility SHARED ${HEADERS} ${SOURCES})
