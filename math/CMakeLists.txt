include_directories(../include)

set(HEADERS
	../include/math/MathSettings.h
	../include/math/MathTypes.h
	../include/math/MathUtils.h
	../include/math/MeshWrapper.h
	../include/math/RasterWrapper.h
	../include/math/SceneDataWrapper.h
	../include/math/BspTree.h
)

set(SOURCES
	MathUtils.cpp
	MeshWrapper.cpp
	RasterWrapper.cpp
	SceneDataWrapper.cpp
	BspTree.cpp
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DMATH_EXPORT)

add_library(FWEMath SHARED ${HEADERS} ${SOURCES})
