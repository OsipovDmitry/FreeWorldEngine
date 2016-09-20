set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES main.cpp
            ${CMAKE_SOURCE_DIR}/core/XML.cpp)

add_executable(test_xml ${SOURCES})

target_link_libraries(test_xml CoreEngine)
file(COPY example.xml incorrect.xml menu.xml DESTINATION ${EXECUTABLE_OUTPUT_PATH})
