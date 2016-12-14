#ifndef __MESHWRAPPER__
#define __MESHWRAPPER__

#include "MathTypes.h"

struct Mesh;

namespace FreeWorldEngine {

namespace Math {

class MeshWrapper {
public:
	MeshWrapper(Mesh *pTargetMesh = 0);
	~MeshWrapper();

	float *addVertices(const uint32 numVertices); // ���������� ��������� �� ����������� ������
	uint32 *addIndices(const uint32 numIndices); // ���������� ��������� �� ����������� ������

	void setAttributeDeclaration(const AttributeType attributeType, const uint16 attributeSize, const uint16 attributeOffset);
	void setPrimitiveFormat(const PrimitiveFormat primitiveFormat);
	void setVertexStride(const uint16 stride);

	void setAttributeValue(const AttributeType attributeType, const uint32 vertexIndex, float *pData);
	float *attributeValue(const AttributeType attributeType, const uint32 vertexIndex) const;

	void setTargetMesh(Mesh *pTargetMesh);
	void destroyTargetMesh();

	Mesh *clone() const;

	// ������������ ���� ��������� ����� ����� ��������� � ��������� vertIndex0, vertIndex1 �� coef. ��������� ������������ �� ������ pDestVert.
	void interpolateTwoVertices(const uint32 vertIndex0, const uint32 vertIndex1, const float coef, float *pDestVert) const;

private:
	Mesh *m_pMesh;

}; // class MeshWrapper

} // namespace

} // namespace

#endif // __MESHWRAPPER__