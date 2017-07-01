set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACE_HEADERS
			../../include/core/IWindow.h
			../../include/core/IWindowManager.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS
			Window.h
            WindowManager.h
            WindowManagerPlugin.h)

set(SOURCES Window.cpp
            WindowManager.cpp
            WindowManagerPlugin.cpp
            main.cpp)

add_library(WindowManagerPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(WindowManagerPlugin CoreEngine)
target_link_libraries(WindowManagerPlugin FWEUtility)
target_link_libraries(WindowManagerPlugin SDL2)
target_link_libraries(WindowManagerPlugin SDL2main)
