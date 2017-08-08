#ifndef __TYPES__
#define __TYPES__

#include <map> 
#include <string>
#include <vector>
#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>
#include <utility/Tree.h>

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

enum ColorChannel {
	ColorChannel_Red,
	ColorChannel_Green,
	ColorChannel_Blue,
	ColorChannel_Alpha
}; // enum ColorChannel

struct Raster {
	uint32 size[3]; // width, height, depth
	uint32 numComponents;
	void *pData;
    Type type;

	Raster() : numComponents(0), type(TYPE_UNSIGNED_INT_8), pData(0) { size[0] = 0; size[1] = size[2] = 1; }
	~Raster() { delete pData; }
}; // struct Raster

enum PrimitiveFormat {
    PrimitiveFormat_Points,
    PrimitiveFormat_Lines,
    PrimitiveFormat_LineStrip,
    PrimitiveFormat_LineLoop,
    PrimitiveFormat_Triangles,
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

	Mesh() : pVertexData(0), pIndexData(0), numVertices(0), numIndices(0), primitiveFormat(PrimitiveFormat_Triangles), attributes(), vertexStride(0) {}
	~Mesh() { delete pVertexData; delete pIndexData; }
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

	Material() : blendMode(BlendMode_Off), isTwoSided(false) {}
	~Material() {}
}; // struct Material

struct Light {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 attenuationCoefficent;
	float innerCone, outerCone;

	uint8 ambientColor[4]; // ����������
	uint8 diffuseColor[4]; // �����������
	uint8 specularColor[4]; // ����������

	enum Type { Type_Directional, Type_Point, Type_Spot };
	Type type;

	Light() : type(Type_Point), attenuationCoefficent(0.0f, 1.0f, 0.0f) {}
};

struct SceneData {

	struct Material {
		std::string name;
		FreeWorldEngine::Material *pMaterialData;
	};
	typedef std::vector<Material*> MaterialList;

	struct Ligth {
		std::string name;
		FreeWorldEngine::Light *pLightData;
	};
	typedef std::vector<Ligth*> LightList;

	struct Mesh {
		std::string name;
		FreeWorldEngine::Mesh *pMeshData;
		uint32 materialIndex;
	};
	typedef std::vector<Mesh*> MeshList;

	struct NodeData {
		std::string name;
		std::vector<uint32> meshesIndices;
		glm::quat orientation;
		glm::vec3 position;
	};

	MaterialList materials;
	LightList lights;
	MeshList meshes;
	Utility::Tree<NodeData*> treeNodes;

	SceneData() : materials(), lights(), meshes(), treeNodes() {}
	~SceneData() {
		for (LightList::iterator it = lights.begin(); it != lights.end(); delete *(it++)) ;
		for (MeshList::iterator it = meshes.begin(); it != meshes.end(); delete *(it++)) ;
		for (MaterialList::iterator it = materials.begin(); it != materials.end(); delete *(it++)) ;
		for (Utility::Tree<NodeData*>::DepthIterator it = treeNodes.beginDepth(); it != treeNodes.endDepth(); delete (*(it++))->data()) ;
	}

}; // struct SceneData

enum SoundFormat {
	SoundFormat_Mono8,
	SoundFormat_Mono16,
	SoundFormat_Stereo8,
	SoundFormat_Stereo16
}; // enum SoundFormat

struct SoundData {
	void *pData;
	uint32 size;
	uint32 frequency;
	SoundFormat format;

	SoundData(void *d = nullptr, uint32 sz = 0, uint32 fr = 0, SoundFormat fmt = SoundFormat_Mono8) : pData(d), size(sz), frequency(fr), format(fmt) {}
	~SoundData() { delete pData; }
}; // struct SoundData

} // namespace

#endif // __TYPES__
