include_directories(../3rdparty)
include_directories(../include)

set(HEADERS Core.h
            Library.h
            LibraryManager.h
            Log.h
            PluginManager.h
            ResourceManager.h)

set(SOURCES Core.cpp
            Library.cpp
            LibraryManager.cpp
            Log.cpp
            PluginManager.cpp
            ResourceManager.cpp
            XML.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DCORE_EXPORT)

add_library(CoreEngine SHARED ${HEADERS} ${SOURCES})
