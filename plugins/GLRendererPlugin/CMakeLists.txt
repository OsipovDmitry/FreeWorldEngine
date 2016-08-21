include_directories(../../3rdparty)
include_directories(../../include)

link_directories(../../bin/Win32/Debug)

set(HEADERS ../../include/renderer/IGLBufferObject.h
            ../../include/renderer/IGLRenderer.h
            ../../include/renderer/IGLShader.h
            ../../include/renderer/IGLShaderProgram.h
            ../../include/renderer/IGLVertexArrayObject.h
            GLBufferObject.h
            GLRenderer.h
            GLShader.h
            GLShaderProgram.h
            GLVertexArrayObject.h
	    GLRendererPlugin.h)

set(SOURCES GLBufferObject.cpp
            GLRenderer.cpp
            GLShader.cpp
            GLShaderProgram.cpp
            GLVertexArrayObject.cpp
	    GLRendererPlugin.cpp
            main.cpp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_library(GLRendererPlugin SHARED ${HEADERS} ${SOURCES})
	
target_link_libraries(GLRendererPlugin CoreEngine)
target_link_libraries(GLRendererPlugin glew)
target_link_libraries(GLRendererPlugin OpenGL32)