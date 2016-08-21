#ifndef __TYPES__
#define __TYPES__

typedef signed char            int8;
typedef signed short int       int16;
typedef signed int             int32;
typedef signed long long int   int64;
typedef unsigned char          uint8;
typedef unsigned short int     uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

enum Type {
	TYPE_SIGNED_INT_8 = 0,
	TYPE_SIGNED_INT_16,
	TYPE_SIGNED_INT_32,
	TYPE_UNSIGNED_INT_8,
	TYPE_UNSIGNED_INT_16,
	TYPE_UNSIGNED_INT_32,
	TYPE_FLOAT
}; // enum Type

struct VertexFormat { // Все значения указываются в штуках float. Если size == 0, то соответсвующего атрибута нет.
	uint8 positionSize, positionStride;
	uint8 tangentSize, tangentStride;
	uint8 binormalSize, binormalStride;
	uint8 normalSize, normalStride;
	uint8 texCoord0Size, texCoord0Stride;
	uint8 texCoord1Size, texCoord1Stride;
	uint8 texCoord2Size, texCoord2Stride;
	uint8 texCoord3Size, texCoord3Stride;

	VertexFormat() : positionSize(0), positionStride(0),
					 tangentSize(0), tangentStride(0),
					 binormalSize(0), binormalStride(0),
					 normalSize(0), normalStride(0), 
					 texCoord0Size(0), texCoord0Stride(0), 
					 texCoord1Size(0), texCoord1Stride(0),
					 texCoord2Size(0), texCoord2Stride(0),
					 texCoord3Size(0), texCoord3Stride(0) {}

}; // struct VertexFormat

struct TriMesh {
	float *pVertexData;
	uint32 *pIndexData;
	uint32 numVertices;
	uint32 numIndices;
	VertexFormat vertexFormat;

	TriMesh() : pVertexData(0), pIndexData(0), numVertices(0), numIndices(0), vertexFormat() {} 

}; // struct TriMesh

#endif // __TYPES__