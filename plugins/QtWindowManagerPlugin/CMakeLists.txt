set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
# «¿À»œ”’¿!!!
include_directories(C:/Qt/4.8.6/include)

link_directories(${LIBRARY_OUTPUT_PATH})
# «¿À»œ”’¿!!!
link_directories(C:/Qt/4.8.6/lib)

set(INTERFACE_HEADERS
			../../include/IWindow.h
            ../../include/IWindowManager.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS Window.h
            WindowManager.h
            WindowManagerPlugin.h)

set(SOURCES Window.cpp
            WindowManager.cpp
            WindowManagerPlugin.cpp
            main.cpp)

add_library(QtWindowManagerPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(QtWindowManagerPlugin CoreEngine)
