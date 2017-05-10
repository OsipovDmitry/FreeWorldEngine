set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS 
	FreeImageLoaderPlugin.h)

set(SOURCES
	FreeImageLoaderPlugin.cpp
	main.cpp)

add_library(FreeImageLoaderPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(FreeImageLoaderPlugin CoreEngine)
target_link_libraries(FreeImageLoaderPlugin FreeImaged)

