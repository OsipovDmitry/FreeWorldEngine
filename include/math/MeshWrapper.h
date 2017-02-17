#ifndef __MESHWRAPPER__
#define __MESHWRAPPER__

#include <Types.h>

#include <math/MathTypes.h>
#include <math/MathSettings.h>

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

	float *addVertices(const uint32 numVertices); // Возвращает указатель на добавленную память
	uint32 *addIndices(const uint32 numIndices); // Возвращает указатель на добавленную память

	void setAttributeDeclaration(const VertexAttributeType attributeType, const uint16 attributeSize, const uint16 attributeOffset);
	void setPrimitiveFormat(const PrimitiveFormat primitiveFormat);
	void setVertexStride(const uint16 stride);

	void setAttributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex, float *pData);
	float *attributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex) const;

	// Интерполяция всех атрибутов мужду двумя вершинами с индексами vertIndex0, vertIndex1 по coef. Результат записывается по адресу pDestVert.
	void interpolateTwoVertices(const uint32 vertIndex0, const uint32 vertIndex1, const float coef, float *pDestVert) const;

private:
	Mesh *m_pMesh;

}; // class MeshWrapper

} // namespace

} // namespace

#endif // __MESHWRAPPER__