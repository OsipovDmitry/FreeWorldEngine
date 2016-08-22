include_directories(../3rdparty)
include_directories(../include)

set(SOURCES main.cpp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_executable(Demo ${SOURCES})

link_directories(${CMAKE_BINARY_DIR}/bin)
target_link_libraries(Demo CoreEngine)
