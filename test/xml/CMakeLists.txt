include_directories(${CMAKE_SOURCE_DIR}/3rdparty/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})

set(SOURCES main.cpp
            ${CMAKE_SOURCE_DIR}/utility/XML.cpp)

add_executable(test_xml ${SOURCES})

set_target_properties(test_xml PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test/xml")
add_custom_command(TARGET test_xml PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy
        ${CMAKE_CURRENT_SOURCE_DIR}/example.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/incorrect.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/menu.xml
    $<TARGET_FILE_DIR:test_xml>
)
