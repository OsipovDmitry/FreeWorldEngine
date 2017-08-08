set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES main.cpp) 

add_executable(Demo3 ${SOURCES})

target_link_libraries(Demo3 CoreEngine)
