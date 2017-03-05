cmake_minimum_required(VERSION 3.4.0)
project(FreeWorldEngine)

set(FWE_3RDPARTY OFF CACHE BOOL "Build 3rdparty libraries")
set(FWE_TESTS OFF CACHE BOOL "Build tests")

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")
endif ()

if (FWE_3RDPARTY)
    add_subdirectory(3rdparty)
endif ()

add_subdirectory(utility)
add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(plugins)
add_subdirectory(demo)
add_subdirectory(demo2)

if (FWE_TESTS)
    add_subdirectory(test/xml)
endif ()
