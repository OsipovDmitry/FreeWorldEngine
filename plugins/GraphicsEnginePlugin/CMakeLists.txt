set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACE_HEADERS
	../../include/graphics_engine/IGraphicsEngine.h
	../../include/graphics_engine/IGraphicsResource.h
	../../include/graphics_engine/IGraphicsModel.h
	../../include/graphics_engine/IGraphicsMaterial.h
	../../include/graphics_engine/IGraphicsScene.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(HEADERS
	GraphicsEngine.h
	GraphicsModel.h
	GraphicsMaterial.h
	GraphicsScene.h
	GraphicsEnginePlugin.h)

set(SOURCES
	GraphicsEngine.cpp
	GraphicsModel.cpp
	GraphicsMaterial.cpp
	GraphicsScene.cpp
	GraphicsEnginePlugin.cpp
	main.cpp)

add_library(GraphicsEnginePlugin SHARED ${HEADERS} ${INTERFACE_HEADERS} ${SOURCES})
	
target_link_libraries(GraphicsEnginePlugin CoreEngine)
target_link_libraries(GraphicsEnginePlugin FWEMath)
