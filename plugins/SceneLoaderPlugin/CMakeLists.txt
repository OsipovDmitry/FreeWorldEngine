set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACE_HEADERS
	../../include/IScene.h
	../../include/ISceneLoader.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS Scene.h
			SceneLoader.h
            SceneLoaderPlugin.h)

set(SOURCES Scene.cpp
			SceneLoader.cpp
            SceneLoaderPlugin.cpp
            main.cpp)

add_library(SceneLoaderPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(SceneLoaderPlugin CoreEngine)
target_link_libraries(SceneLoaderPlugin FWEMath)
target_link_libraries(SceneLoaderPlugin assimp)
