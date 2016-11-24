set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS ../../include/renderer/IGPURenderer.h
			../../include/renderer/IGPUTexture.h
			../../include/renderer/IGPUShader.h
			../../include/renderer/IGPUBuffer.h
			../../include/renderer/IGPUBufferContainer.h
			GLTexture.h
			GLBuffer.h
			GLBufferContainer.h
			GLShader.h
			GLRenderer.h)

set(SOURCES GLTexture.cpp
			GLBuffer.cpp
			GLBufferContainer.cpp
			GLShader.cpp
			GLRenderer.cpp
			
			GLRendererPlugin.cpp
            main.cpp)


add_library(GLRendererPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(GLRendererPlugin CoreEngine)
target_link_libraries(GLRendererPlugin glew)
target_link_libraries(GLRendererPlugin OpenGL32)
