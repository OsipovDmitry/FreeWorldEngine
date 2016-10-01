set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS ../../include/IThread.h
            ../../include/IThreadManager.h
	    ThreadManager.h
            ThreadManagerPlugin.h)

set(SOURCES ThreadManager.cpp
	    ThreadManagerPlugin.cpp
            main.cpp)

add_library(ThreadManagerPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(ThreadManagerPlugin CoreEngine)