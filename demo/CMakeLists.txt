set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../3rdparty)
include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES main.cpp) 

add_executable(Demo ${SOURCES})

target_link_libraries(Demo CoreEngine)
