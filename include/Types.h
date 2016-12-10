#ifndef __TYPES__
#define __TYPES__

#include <utility>
#include <map> 
#include <string>
#include <vector>

typedef signed char            int8;
typedef signed short int       int16;
typedef signed int             int32;
typedef signed long long int   int64;
typedef unsigned char          uint8;
typedef unsigned short int     uint16;
typedef unsigned int           uint32;
typedef unsigned long long int uint64;

enum Type {
	TYPE_SIGNED_INT_8,
	TYPE_SIGNED_INT_16,
	TYPE_SIGNED_INT_32,
	TYPE_UNSIGNED_INT_8,
	TYPE_UNSIGNED_INT_16,
	TYPE_UNSIGNED_INT_32,
    TYPE_FLOAT
}; // enum Type

struct Raster {
    uint32 size[3]; // width, height, depth
    uint32 numComponents;
	void *pData;
    Type type;

    Raster() : numComponents(0), type(TYPE_UNSIGNED_INT_8), pData(0) { size[0] = 0; size[1] = size[2] = 1; }
}; // struct Raster

enum PrimitiveFormat {
    PrimitiveFormat_Points,
    PrimitiveFormat_Lines,
    PrimitiveFormat_LineStrip,
    PrimitiveFormat_LineLoop,
    PrimitiveFormat_Trangles,
    PrimitiveFormat_TriangleStrip,
    PrimitiveFormat_TrangleFan
}; //enum PrimitiveFormat

enum AttributeType {
	AttributeType_Position,
	AttributeType_Tangent,
	AttributeType_Binormal,
	AttributeType_Normal,
	AttributeType_TexCoord0,
	AttributeType_TexCoord1,
	AttributeType_TexCoord2,
	AttributeType_TexCoord3,
}; // enum AttributeType

/*struct MeshData {
	float *pVertexData;
	uint32 vertexDataLen;
	uint32 *pIndexData;
	uint32 indexDataLen;

	MeshData() : pVertexData(0), pIndexData(0), vertexDataLen(0), indexDataLen(0) {}
}; // struct MeshData

struct MeshDeclaration {
	std::map<AttributeType, std::pair<uint16, uint16> > attributes; // отображение типа атрибута на пару (размер, смещение). Размер и смещение в штуках float
	uint32 numVertices, numPrimitives; // количство примитивов
	PrimitiveFormat primitiveFormat; // тип примитива
	uint16 vertexStride; // смещение между вершинами в штуках float

	MeshDeclaration() : attributes(), numVertices(0), numPrimitives(0), primitiveFormat(PrimitiveFormat_Points), vertexStride(0) {}
}; // struct MeshDeclaration
*/
struct Mesh {
	/*MeshData meshData;
	MeshDeclaration meshDecl;*/

	float *pVertexData; // массив с вершинными атрибутами. Размер = numVertices * vertexStride * sizeof(float)
	uint32 *pIndexData; // массив с индексами. Размер = numIndices * sizeof(uint32)
	uint32 numVertices; // количество вершин
	uint32 numIndices; // количество индексов
	PrimitiveFormat primitiveFormat; // тип примитивов
	std::map<AttributeType, std::pair<uint16, uint16> > attributes; // отображение типа атрибута на пару (размер, смещение). Размер и смещение в штуках float
	uint16 vertexStride; // размер одной вершины в штуках float

	Mesh() : pVertexData(0), pIndexData(0), numVertices(0), numIndices(0), primitiveFormat(PrimitiveFormat_Trangles), attributes(), vertexStride(0) {}
}; // struct Mesh

#endif // __TYPES__
