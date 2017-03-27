#ifndef __MESHWRAPPER__
#define __MESHWRAPPER__

#include "../Types.h"
#include "MathTypes.h"
#include "MathSettings.h"

namespace FreeWorldEngine {

namespace Math {

class MATH_DLL MeshWrapper {
public:
	MeshWrapper(Mesh *pTargetMesh);
	~MeshWrapper();

	Mesh *clone() const;

	void setTarget(Mesh *pTargetMesh);
	Mesh *target() const;
	void destroyTarget();

	float *addVertices(const uint32 numVertices); // ���������� ��������� �� ����������� ������
	uint32 *addIndices(const uint32 numIndices); // ���������� ��������� �� ����������� ������

	uint32 numVertices() const;
	uint32 numIndices() const;
	PrimitiveFormat primitiveFormat() const;

	void setAttributeDeclaration(const VertexAttributeType attributeType, const uint16 attributeSize, const uint16 attributeOffset);
	void setPrimitiveFormat(const PrimitiveFormat primitiveFormat);
	void setVertexStride(const uint16 stride);

	void setAttributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex, float *pData);
	float *attributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex) const;

	void setIndexValue(const uint32 index, const uint32 value);
	uint32 indexValue(const uint32 index) const;

	Aabb computeAxisAlignedBoundingBox() const;
	Sphere computeBoundingSphere() const;

	// ������������ ���� ��������� ����� ����� ��������� � ��������� vertIndex0, vertIndex1 �� coef. ��������� ������������ �� ������ pDestVert.
	void interpolateTwoVertices(const uint32 vertIndex0, const uint32 vertIndex1, const float coef, float *pDestVert) const;

	void translateMesh(float *pDeltaValue);
	void scaleMesh(float *pDeltaValue);

	bool combine(const Mesh *pOtherMesh); // true, ���� ������� ���������� (������ ��������� primitiveFrmat, attributes � vertexStride)

private:
	Mesh *m_pMesh;

}; // class MeshWrapper

} // namespace

} // namespace

#endif // __MESHWRAPPER__