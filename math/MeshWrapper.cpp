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
		return 0;

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

Sphere MeshWrapper::calculateBoundingSphere() const
{
	if ((m_pMesh->numVertices == 0) || (m_pMesh->attributes.count(VertexAttributeType_Position) == 0))
		return Sphere(0.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 pos(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_pMesh->numVertices; ++i)
		pos += *((glm::vec3*)attributeValue(VertexAttributeType_Position, i));
	pos /= m_pMesh->numVertices;

	float r = 0.0f;
	for (int i = 0; i < m_pMesh->numVertices; ++i) {
		glm::vec3 v = *(glm::vec3*)attributeValue(VertexAttributeType_Position, i) - pos;
		float lSq = glm::dot(v,v);
		if (lSq > r)
			r = lSq;
	}

	return Sphere(pos, glm::sqrt(r));
}

void MeshWrapper::interpolateTwoVertices(const uint32 vertIndex0, const uint32 vertIndex1, const float coef, float *pDestVert) const
{
	float *p0 = m_pMesh->pVertexData + vertIndex0 * m_pMesh->vertexStride;
	float *p1 = m_pMesh->pVertexData + vertIndex1 * m_pMesh->vertexStride;

	for (uint32 i = 0; i < m_pMesh->vertexStride; ++i)
		interpolate(*(p0+i), *(p1+i), coef, *(pDestVert+i));

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