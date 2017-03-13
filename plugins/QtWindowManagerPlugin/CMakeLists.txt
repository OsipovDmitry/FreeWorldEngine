set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)

link_directories(${LIBRARY_OUTPUT_PATH})

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
            main.cpp
			moc_Window.cpp)

add_library(QtWindowManagerPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(QtWindowManagerPlugin CoreEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 4.4.3 REQUIRED QtGui QtOpenGL)
target_link_libraries(QtWindowManagerPlugin Qt4::QtGui Qt4::QtOpenGL)