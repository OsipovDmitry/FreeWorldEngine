include_directories(../include)

set(HEADERS ../include/utility/UtilitySettings.h
			../include/utility/XML.h
			../include/utility/DateTime.h
			../include/utility/File.h
			../include/utility/Tree.h
			../include/utility/KeyGenerator.h)

set(SOURCES XML.cpp
			DateTime.cpp
			File.cpp
			KeyGenerator.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DUTILITY_EXPORT)

add_library(FWEUtility SHARED ${HEADERS} ${SOURCES})
