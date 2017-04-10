set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS 
	ImageLoaderPlugin.h)

set(SOURCES
	ImageLoaderPlugin.cpp
	main.cpp)

add_library(ImageLoaderPlugin SHARED ${HEADERS} ${INTERFACE_HEADERS} ${SOURCES})
	
target_link_libraries(ImageLoaderPlugin CoreEngine)
target_link_libraries(ImageLoaderPlugin libjpeg)
#target_link_libraries(ImageLoaderPlugin libpng)
