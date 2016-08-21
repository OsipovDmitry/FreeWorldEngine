include_directories(../../3rdparty)
include_directories(../../include)

link_directories(../../bin/Win32/Debug)

set(HEADERS ../../include/IWindow.h
            ../../include/IWindowManager.h
            Window.h
            WindowManager.h
            WindowManagerPlugin.h)

set(SOURCES Window.cpp
            WindowManager.cpp
            WindowManagerPlugin.cpp
            main.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_library(WindowManagerPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(WindowManagerPlugin CoreEngine)
target_link_libraries(WindowManagerPlugin SDL2)
target_link_libraries(WindowManagerPlugin SDL2main)