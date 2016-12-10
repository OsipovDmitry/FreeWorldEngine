set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
# ��������!!!
include_directories(C:/Qt/5.5/msvc2010/include)

link_directories(${LIBRARY_OUTPUT_PATH})
# ��������!!!
link_directories(C:/Qt/5.5/msvc2010/lib)

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