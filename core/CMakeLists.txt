include_directories(../3rdparty)
include_directories(../include)

set(HEADERS Core.h
            Library.h
            LibraryManager.h
            Logger.h
			TextFileLog.h
            PluginManager.h
            ResourceManager.h
			XML.h)

set(SOURCES Core.cpp
            Library.cpp
            LibraryManager.cpp
            Logger.cpp
			TextFileLog.cpp
            PluginManager.cpp
            ResourceManager.cpp
            XML.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DCORE_EXPORT)

add_library(CoreEngine SHARED ${HEADERS} ${SOURCES})
