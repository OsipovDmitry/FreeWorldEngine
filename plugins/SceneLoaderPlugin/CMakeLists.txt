set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS 
            SceneLoaderPlugin.h)

set(SOURCES 
            SceneLoaderPlugin.cpp
            main.cpp)

add_library(SceneLoaderPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(SceneLoaderPlugin CoreEngine)