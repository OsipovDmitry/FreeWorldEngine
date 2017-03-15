set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES main.cpp) 

add_executable(Demo ${SOURCES})

target_link_libraries(Demo CoreEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt4 4.4.3 REQUIRED QtGui QtOpenGL)
target_link_libraries(Demo Qt4::QtGui)
target_link_libraries(Demo Qt4::QtOpenGL)
target_link_libraries(Demo OpenGL32)
