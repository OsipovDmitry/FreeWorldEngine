set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS SceneLoaderPlugin.h)

set(SOURCES SceneLoaderPlugin.cpp
            main.cpp)

add_library(SceneLoaderPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(SceneLoaderPlugin CoreEngine)
target_link_libraries(SceneLoaderPlugin FWEMath)
target_link_libraries(SceneLoaderPlugin assimp)
