set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS ../../include/IImage.h
            ../../include/IImageManager.h
            Image.h
            ImageManager.h
            ImageManagerPlugin.h)

set(SOURCES Image.cpp
            ImageManager.cpp
            ImageManagerPlugin.cpp
            main.cpp)

add_library(ImageManagerPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(ImageManagerPlugin CoreEngine)
target_link_libraries(ImageManagerPlugin libjpeg)
target_link_libraries(ImageManagerPlugin libpng)