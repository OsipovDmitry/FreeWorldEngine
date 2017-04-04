set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACE_HEADERS
	../../include/renderer/IGPURenderer.h
	../../include/renderer/IGPUFrameBuffer.h
	../../include/renderer/IGPUTexture.h
	../../include/renderer/IGPUShader.h
	../../include/renderer/IGPUBuffer.h
	../../include/renderer/IGPUBufferContainer.h
	../../include/renderer/IGPUOcclusionQuery.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS
	GLBuffer.h
	GLBufferContainer.h
	GLShader.h
	GLTexture.h
	GLFrameBuffer.h
	GLOcclusionQuery.h
	GLRenderer.h
	GLRendererPlugin.h)

set(SOURCES
	GLBuffer.cpp
	GLBufferContainer.cpp
	GLShader.cpp
	GLTexture.cpp
	GLFrameBuffer.cpp
	GLOcclusionQuery.cpp
	GLRenderer.cpp
	GLRendererPlugin.cpp
	main.cpp)


add_library(GLRendererPlugin SHARED ${INTERFACE_HEADERS} ${HEADERS} ${SOURCES})
	
target_link_libraries(GLRendererPlugin CoreEngine)
target_link_libraries(GLRendererPlugin glew)
target_link_libraries(GLRendererPlugin OpenGL32)