set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../3rdparty)
include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS Core.h
            Library.h
            LibraryManager.h
            Logger.h
	    TextFileLog.h
            PluginManager.h
            ResourceManager.h)

set(SOURCES Core.cpp
            Library.cpp
            LibraryManager.cpp
            Logger.cpp
	    TextFileLog.cpp
            PluginManager.cpp
            ResourceManager.cpp)

add_definitions(-DCORE_EXPORT)

add_library(CoreEngine SHARED ${HEADERS} ${SOURCES})

target_link_libraries(CoreEngine FWEUtility)
