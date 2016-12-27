#ifndef __TYPES__
#define __TYPES__

#include <utility>
#include <map> 
#include <string>
#include <vector>
#include <3rdparty/glm/mat4x4.hpp>

namespace FreeWorldEngine {

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
    TYPE_FLOAT,
	TYPE_COUNT
}; // enum Type

const uint32 TypeSize[TYPE_COUNT] = { sizeof(int8), sizeof(int16), sizeof(int32), sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(float) };
const bool TypeIsFloatingPoint[TYPE_COUNT] = { false, false, false, false, false, false, true };

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

enum VertexAttributeType {
	VertexAttributeType_Position,
	VertexAttributeType_Tangent,
	VertexAttributeType_Binormal,
	VertexAttributeType_Normal,
	VertexAttributeType_TexCoord0,
	VertexAttributeType_TexCoord1,
	VertexAttributeType_TexCoord2,
	VertexAttributeType_TexCoord3,
}; // enum AttributeType

struct Mesh {
	float *pVertexData; // ������ � ���������� ����������. ������ = numVertices * vertexStride * sizeof(float)
	uint32 *pIndexData; // ������ � ���������. ������ = numIndices * sizeof(uint32)
	uint32 numVertices; // ���������� ������
	uint32 numIndices; // ���������� ��������
	PrimitiveFormat primitiveFormat; // ��� ����������
	std::map<VertexAttributeType, std::pair<uint16, uint16> > attributes; // ����������� ���� �������� �� ���� (������, ��������). ������ � �������� � ������ float
	uint16 vertexStride; // ������ ����� ������� � ������ float

	Mesh() : pVertexData(0), pIndexData(0), numVertices(0), numIndices(0), primitiveFormat(PrimitiveFormat_Trangles), attributes(), vertexStride(0) {}
}; // struct Mesh

struct Material {
	std::string ambientMap;
	std::string diffuseMap;
	std::string specularMap;
	std::string emissionMap;
	std::string heightMap;
	std::string normalMap;
	std::string shininessMap;
	std::string opacityMap;

	uint8 ambientColor[4]; // ����������
	uint8 diffuseColor[4]; // �����������
	uint8 specularColor[4]; // ����������
	uint8 emissionColor[4]; // ��������
	float shininess; // ������� ������
	float opacity; // ������� ������������

	enum BlendMode { BlendMode_Off, BlendMode_Default, BlendMode_Additive };
	BlendMode blendMode; // ����������

	bool isTwoSided; // ������������

}; // struct Material

struct Scene {

	struct Material {
		std::string name;
		FreeWorldEngine::Material *pMaterialData;
	};
	typedef std::vector<Material*> MaterialList;

	struct Mesh {
		std::string name;
		FreeWorldEngine::Mesh *pMeshData;
		uint32 materialIndex;
	};
	typedef std::vector<Mesh*> MeshList;

	struct Node {
		std::string name;
		std::vector<uint32> meshesIndices;
		std::vector<Node*> childNodes;
		Node *pParentNode;
		glm::mat4x4 transform;
	};

	MaterialList materials;
	MeshList meshes;
	Node *rootNode;

	Scene() : materials(), meshes(), rootNode(0) {}

}; // struct Scene

} // namespace

#endif // __TYPES__
