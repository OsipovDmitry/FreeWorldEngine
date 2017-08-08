cmake_minimum_required(VERSION 3.4.0)
project(FreeWorldEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
endif ()

if (UNIX)
	add_definitions(-DPLATFORM_UNIX)
elseif (WIN32)
	add_definitions(-DPLATFORM_WINDOWS)
endif()

add_subdirectory(utility)
add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(plugins)
#add_subdirectory(demo)
add_subdirectory(demo2)
#add_subdirectory(demo3)
