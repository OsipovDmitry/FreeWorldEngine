set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS ../../include/IImage.h
            ../../include/IImageLoader.h
            Image.h
            ImageLoader.h
            ImageLoaderPlugin.h)

set(SOURCES Image.cpp
            ImageLoader.cpp
            ImageLoaderPlugin.cpp
            main.cpp)

add_library(ImageLoaderPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(ImageLoaderPlugin CoreEngine)
target_link_libraries(ImageLoaderPlugin FWEUtility)
target_link_libraries(ImageLoaderPlugin FWEMath)
target_link_libraries(ImageLoaderPlugin libjpeg)
target_link_libraries(ImageLoaderPlugin libpng)
