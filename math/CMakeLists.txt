include_directories(../3rdparty)
include_directories(../include/math)

set(HEADERS
	../include/math/MathSettings.h
	../include/math/MathTypes.h
	../include/math/MathUtils.h
	../include/math/MeshWrapper.h
	../include/math/BspTree.h
)

set(SOURCES
	MathUtils.cpp
	MeshWrapper.cpp
	BspTree.cpp
)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

add_definitions(-DMATH_EXPORT)

add_library(FWEMath SHARED ${HEADERS} ${SOURCES})
