#include <math/MathUtils.h>
#include <math/MeshWrapper.h>

namespace FreeWorldEngine {

namespace Math {

MeshWrapper::MeshWrapper(Mesh *pTargetMesh) :
	m_pMesh(pTargetMesh)
{
}

MeshWrapper::~MeshWrapper()
{
}

Mesh *MeshWrapper::clone() const
{
	if (!m_pMesh)
		return nullptr;

	Mesh *pNewMesh = new Mesh;
	pNewMesh->attributes = m_pMesh->attributes;
	pNewMesh->numIndices = m_pMesh->numIndices;
	pNewMesh->numVertices = m_pMesh->numVertices;
	pNewMesh->primitiveFormat = m_pMesh->primitiveFormat;
	pNewMesh->vertexStride = m_pMesh->vertexStride;
	if (m_pMesh->pVertexData) {
		pNewMesh->pVertexData = new float[m_pMesh->numVertices * m_pMesh->vertexStride];
		memcpy(pNewMesh->pVertexData, m_pMesh->pVertexData, m_pMesh->numVertices * m_pMesh->vertexStride * sizeof(float));
	}
	if (m_pMesh->pIndexData) {
		pNewMesh->pIndexData = new uint32[m_pMesh->numIndices];
		memcpy(pNewMesh->pIndexData, m_pMesh->pIndexData, m_pMesh->numIndices * sizeof(uint32));
	}

	return pNewMesh;
}

void MeshWrapper::setTarget(Mesh *pTargetMesh)
{
	m_pMesh = pTargetMesh;
}

Mesh *MeshWrapper::target() const
{
	return m_pMesh;
}

void MeshWrapper::destroyTarget()
{
	delete m_pMesh;
	m_pMesh = 0;
}

float *MeshWrapper::addVertices(const uint32 numVertices)
{
	uint32 oldSize = m_pMesh->numVertices * m_pMesh->vertexStride;
	uint32 newSize = oldSize + numVertices * m_pMesh->vertexStride;
	float *p = new float[newSize];
	if (m_pMesh->pVertexData) {
		memcpy(p, m_pMesh->pVertexData, oldSize * sizeof(float));
		delete [] m_pMesh->pVertexData;
	}
	m_pMesh->pVertexData = p;
	m_pMesh->numVertices += numVertices;
	return p + oldSize;
}

uint32 *MeshWrapper::addIndices(const uint32 numIndices)
{
	uint32 oldSize = m_pMesh->numIndices;
	uint32 newSize = oldSize + numIndices;
	uint32 *p = new uint32[newSize];
	if (m_pMesh->pIndexData) {
		memcpy(p, m_pMesh->pIndexData, oldSize * sizeof(uint32));
		delete [] m_pMesh->pIndexData;
	}
	m_pMesh->pIndexData = p;
	m_pMesh->numIndices += numIndices;
	return p + oldSize;
}

uint32 MeshWrapper::numVertices() const
{
	return m_pMesh->numVertices;
}

uint32 MeshWrapper::numIndices() const
{
	return m_pMesh->numIndices;
}

PrimitiveFormat MeshWrapper::primitiveFormat() const
{
	return m_pMesh->primitiveFormat;
}

void MeshWrapper::setAttributeDeclaration(const VertexAttributeType attributeType, const uint16 attributeSize, const uint16 attributeOffset)
{
	m_pMesh->attributes[attributeType] = std::make_pair(attributeSize, attributeOffset);
}

void MeshWrapper::setPrimitiveFormat(const PrimitiveFormat primitiveFormat)
{
	m_pMesh->primitiveFormat = primitiveFormat;
}

void MeshWrapper::setVertexStride(const uint16 stride)
{
	m_pMesh->vertexStride = stride;
}

void MeshWrapper::setAttributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex, float *pData)
{
	const std::pair<uint16, uint16> size_offset = m_pMesh->attributes.at(attributeType);
	memcpy(m_pMesh->pVertexData + m_pMesh->vertexStride * vertexIndex + size_offset.second, pData, size_offset.first * sizeof(float));
}

float *MeshWrapper::attributeValue(const VertexAttributeType attributeType, const uint32 vertexIndex) const
{
	const uint16 offset = m_pMesh->attributes.at(attributeType).second;
	return m_pMesh->pVertexData + m_pMesh->vertexStride * vertexIndex + offset;
}

void MeshWrapper::setIndexValue(const uint32 index, const uint32 value)
{
	m_pMesh->pIndexData[index] = value;
}

uint32 MeshWrapper::indexValue(const uint32 index) const
{
	return m_pMesh->pIndexData[index];
}

Aabb MeshWrapper::computeAxisAlignedBoundingBox() const
{
	if ((m_pMesh->numVertices == 0) || (m_pMesh->attributes.count(VertexAttributeType_Position) == 0))
		return Aabb(glm::vec3(), glm::vec3());

	glm::vec3 a, b;
	a = b = *((glm::vec3*)attributeValue(VertexAttributeType_Position, 0));
	for (int i = 1; i < m_pMesh->numVertices; ++i) {
		glm::vec3 *v = (glm::vec3*)attributeValue(VertexAttributeType_Position, i);
		if (v->x < a.x) a.x = v->x;
		else if (v->x > b.x) b.x = v->x;
		if (v->y < a.y) a.y = v->y;
		else if (v->y > b.y) b.y = v->y;
		if (v->z < a.z) a.z = v->z;
		else if (v->z > b.z) b.z = v->z;
	}

	return Aabb(a, b);
}

Sphere MeshWrapper::computeBoundingSphere() const
{
	Aabb box;
	Sphere sphere;
	computeAabbAndBoundingSphere(box, sphere);
	return sphere;
}

void MeshWrapper::computeAabbAndBoundingSphere(Aabb& resAabb, Sphere& resBoundingSphere) const
{
	if ((m_pMesh->numVertices == 0) || (m_pMesh->attributes.count(VertexAttributeType_Position) == 0)) {
		resAabb = Aabb(glm::vec3(), glm::vec3());
		resBoundingSphere = Sphere(0.0f, 0.0f, 0.0f, 0.0f);
		return;
	}

	resAabb = computeAxisAlignedBoundingBox();
	glm::vec3 pos = 0.5f * (resAabb.vMin + resAabb.vMax);

	float rSq = 0.0f;
	for (int i = 0; i < m_pMesh->numVertices; ++i) {
		glm::vec3 v = *(glm::vec3*)attributeValue(VertexAttributeType_Position, i) - pos;
		float lSq = glm::dot(v, v);
		if (lSq > rSq)
			rSq = lSq;
	}

	resBoundingSphere = Sphere(pos, glm::sqrt(rSq));
}

void MeshWrapper::interpolateTwoVertices(const uint32 vertIndex0, const uint32 vertIndex1, const float coef, float *pDestVert) const
{
	float *p0 = m_pMesh->pVertexData + vertIndex0 * m_pMesh->vertexStride;
	float *p1 = m_pMesh->pVertexData + vertIndex1 * m_pMesh->vertexStride;

	for (uint32 i = 0; i < m_pMesh->vertexStride; ++i)
		interpolate(*(p0+i), *(p1+i), coef, *(pDestVert+i));

}

void MeshWrapper::translateMesh(float *pDeltaValue)
{
	if (m_pMesh->attributes.count(VertexAttributeType_Position) == 0)
		return;

	std::pair<uint16, uint16> size_offset = m_pMesh->attributes[VertexAttributeType_Position];
	uint16 size = size_offset.first;
	uint16 offs = size_offset.second;

	for (uint32 v = 0; v < m_pMesh->numVertices; ++v) {
		float *pVertex = attributeValue(VertexAttributeType_Position, v) + offs;
		for (int i = 0; i < size; ++i)
			pVertex[i] += pDeltaValue[i];
	}
}

void MeshWrapper::scaleMesh(float *pDeltaValue)
{
	if (m_pMesh->attributes.count(VertexAttributeType_Position) == 0)
		return;

	std::pair<uint16, uint16> size_offset = m_pMesh->attributes[VertexAttributeType_Position];
	uint16 size = size_offset.first;
	uint16 offs = size_offset.second;

	for (uint32 v = 0; v < m_pMesh->numVertices; ++v) {
		float *pVertex = attributeValue(VertexAttributeType_Position, v) + offs;
		for (int i = 0; i < size; ++i)
			pVertex[i] *= pDeltaValue[i];
	}
}

bool MeshWrapper::combine(const Mesh *pOtherMesh)
{
	if (!m_pMesh)
		return false;

	if (!pOtherMesh)
		return true;

	if (m_pMesh->primitiveFormat != pOtherMesh->primitiveFormat ||
		m_pMesh->attributes != pOtherMesh->attributes ||
		m_pMesh->vertexStride != pOtherMesh->vertexStride)
		return false;

	memcpy(addVertices(pOtherMesh->numVertices), pOtherMesh->pVertexData, pOtherMesh->numVertices * pOtherMesh->vertexStride * sizeof(float));

	const uint32 numNewIndices = pOtherMesh->numIndices;
	const uint32 numThisVertices = m_pMesh->numVertices;
	uint32 *pOldIndices = pOtherMesh->pIndexData;
	uint32 *pNewIndices = addIndices(numNewIndices);

	for (uint32 i = 0; i < numNewIndices; ++i)
		pNewIndices[i] = pOldIndices[i] + numThisVertices;

	return true;
}

/*void MeshWrapper::primitiveIndices(const uint32 primitiveIndex, uint32 *pIndices) const
{
	uint32 *p = m->pMesh->meshData.pIndexData;
	switch (m->pMesh->meshDecl.primitiveFormat) {
	case PrimitiveFormat_Points: {
		pIndices[0] = p[primitiveIndex];
		break;
	}
	case PrimitiveFormat_Lines: {
		pIndices[0] = p[2*primitiveIndex+0];
		pIndices[1] = p[2*primitiveIndex+1];
		break;
	}
	case PrimitiveFormat_LineStrip: {
		pIndices[0] = p[primitiveIndex+0];
		pIndices[1] = p[primitiveIndex+1];
		break;
	}
	case PrimitiveFormat_LineLoop: {
		pIndices[0] = p[(primitiveIndex+0) % m->pMesh->meshDecl.numPrimitives];
		pIndices[1] = p[(primitiveIndex+1) % m->pMesh->meshDecl.numPrimitives];
		break;
	}
	case PrimitiveFormat_Trangles: {
		pIndices[0] = p[3*primitiveIndex+0];
		pIndices[1] = p[3*primitiveIndex+1];
		pIndices[2] = p[3*primitiveIndex+2];
		break;
	}
	case PrimitiveFormat_TriangleStrip: {
		pIndices[0] = p[primitiveIndex+0];
		pIndices[1] = p[primitiveIndex+1];
		pIndices[2] = p[primitiveIndex+2];
		break;
	}
	case PrimitiveFormat_TrangleFan: {
		pIndices[0] = p[0];
		pIndices[1] = p[primitiveIndex+1];
		pIndices[2] = p[primitiveIndex+2];
		break;
	}
	};
}*/

} // namespace

} // namespace