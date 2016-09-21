cmake_minimum_required(VERSION 3.4.0)
project(FreeWorldEngine)

if (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
endif ()

add_subdirectory(3rdparty)
add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(demo)
add_subdirectory(test)
