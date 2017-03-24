#include <3rdparty/glm/gtc/constants.hpp>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

namespace FreeWorldEngine {
namespace Math {

MeshPainter::MeshPainter(Mesh *pTargetMesh)
{
	setTarget(pTargetMesh);
}

MeshPainter::MeshPainter(const MeshWrapper& wrapper)
{
	setTarget(wrapper.target());
}

MeshPainter::~MeshPainter()
{
}

void MeshPainter::setTarget(Mesh *pTargetMesh)
{
	if (pTargetMesh->primitiveFormat == PrimitiveFormat_Points ||
		pTargetMesh->primitiveFormat == PrimitiveFormat_Lines ||
		pTargetMesh->primitiveFormat == PrimitiveFormat_Triangles)
		m_pMesh = pTargetMesh;
	else
		m_pMesh = nullptr;
}

Mesh *MeshPainter::target() const
{
	return m_pMesh;
}

void MeshPainter::paintEllipse(const float radiusX, const float radiusY, const float radiusZ, const uint32 numLongs, const uint32 numLats)
{
	if (!m_pMesh)
		return;

	if (numLats < 3 || numLongs < 3)
		return;

	bool generateTexCoord = m_pMesh->attributes.count(VertexAttributeType_TexCoord0) != 0;

	MeshWrapper wrapper(m_pMesh);
	uint32 numVertices = wrapper.numVertices();
	wrapper.addVertices(numLats * (numLongs+1));

	float pos[3], tex[2], psi, phi, cospsi;
	uint32 iindex, vindex;
	for (uint32 latitude = 0; latitude < numLats; ++latitude) {
		tex[1] = (float)latitude / (float)(numLats - 1);
		psi = (0.5f - tex[1]) * glm::pi<float>();
		cospsi = glm::cos(psi);
		pos[1] = radiusY * glm::sin(psi);
		for (uint32 longitude = 0; longitude < numLongs+1; ++longitude)
		{
			tex[0] = (float)longitude / (float)numLongs;
			phi = tex[0] * glm::two_pi<float>();
			pos[0] = radiusX * glm::cos(phi) * cospsi;
			pos[2] = radiusZ * glm::sin(phi) * cospsi;
			vindex = latitude*(numLongs + 1) + longitude;
			if (generateTexCoord)
				wrapper.setAttributeValue(VertexAttributeType_TexCoord0, numVertices + vindex, (float*)tex);
			wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + vindex, (float*)pos);
		}
	}

	uint32 numIndices = wrapper.numIndices();
	switch (wrapper.primitiveFormat()) {
	case PrimitiveFormat_Points: {
		wrapper.addIndices(numLats * numLongs);
		for (uint32 latitude = 0; latitude < numLats; ++latitude)
			for (uint32 longitude = 0; longitude < numLongs; ++longitude) {
				iindex = latitude*numLongs + longitude;
				vindex = latitude*(numLongs+1) + longitude;
				wrapper.setIndexValue(numIndices + iindex, numVertices + vindex);
			}
		break;
	}
	case PrimitiveFormat_Lines: {
		wrapper.addIndices(4 * (numLats-1) * numLongs);
		for (uint32 latitude = 0; latitude < numLats-1; ++latitude)
			for (uint32 longitude = 0; longitude < numLongs; ++longitude) {
				iindex = 4 * (latitude*numLongs + longitude);
				vindex = latitude*(numLongs+1) + longitude;
				wrapper.setIndexValue(numIndices + iindex + 0, numVertices + vindex);
				wrapper.setIndexValue(numIndices + iindex + 2, numVertices + vindex);
				vindex = (latitude + 1)*(numLongs+1) + longitude;
				wrapper.setIndexValue(numIndices + iindex + 1, numVertices + vindex);
				vindex = latitude*(numLongs+1) + longitude + 1;
				wrapper.setIndexValue(numIndices + iindex + 3, numVertices + vindex);
			}
		break;
	}
	case PrimitiveFormat_Triangles: {
		wrapper.addIndices(6 * (numLats-1) * numLongs);
		for (uint32 latitude = 0; latitude < numLats - 1; ++latitude)
			for (uint32 longitude = 0; longitude < numLongs; ++longitude) {
				iindex = 6 * (latitude*numLongs + longitude);
				vindex = latitude*(numLongs + 1) + longitude;
				wrapper.setIndexValue(numIndices + iindex + 0, numVertices + vindex);
				wrapper.setIndexValue(numIndices + iindex + 3, numVertices + vindex);
				vindex = (latitude + 1)*(numLongs + 1) + (longitude + 1);
				wrapper.setIndexValue(numIndices + iindex + 2, numVertices + vindex);
				wrapper.setIndexValue(numIndices + iindex + 4, numVertices + vindex);
				vindex = latitude*(numLongs + 1) + (longitude + 1);
				wrapper.setIndexValue(numIndices + iindex + 1, numVertices + vindex);
				vindex = (latitude + 1)*(numLongs + 1) + longitude;
				wrapper.setIndexValue(numIndices + iindex + 5, numVertices + vindex);
			}
		break;
	}
	}
}

void MeshPainter::paintSphere(const float radius, const uint32 numSegs)
{
	paintEllipse(radius, radius, radius, numSegs*2, numSegs);
}

void MeshPainter::paintBox(const float lenX, const float lenY, const float lenZ)
{
	if (!m_pMesh)
		return;

	if (lenX < 0.0f || lenY < 0.0f || lenZ < 0.0f)
		return;

	const float size[3] = { 0.5f*lenX, 0.5f*lenY, 0.5f*lenZ };

	bool generateTexCoord = m_pMesh->attributes.count(VertexAttributeType_TexCoord0) != 0;

	MeshWrapper wrapper(m_pMesh);
	uint32 numVertices = wrapper.numVertices();
	wrapper.addVertices(24);

	const float verts[8][3] = {
		{ -size[0], -size[1], -size[2] },
		{ -size[0], -size[1], +size[2] },
		{ +size[0], -size[1], +size[2] },
		{ +size[0], -size[1], -size[2] },
		{ -size[0], +size[1], -size[2] },
		{ -size[0], +size[1], +size[2] },
		{ +size[0], +size[1], +size[2] },
		{ +size[0], +size[1], -size[2] },
	};
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 0, (float*)verts[0]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 1, (float*)verts[1]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 2, (float*)verts[5]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 3, (float*)verts[4]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 4, (float*)verts[2]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 5, (float*)verts[3]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 6, (float*)verts[7]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 7, (float*)verts[6]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 8, (float*)verts[0]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 9, (float*)verts[3]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 10, (float*)verts[2]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 11, (float*)verts[1]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 12, (float*)verts[5]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 13, (float*)verts[6]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 14, (float*)verts[7]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 15, (float*)verts[4]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 16, (float*)verts[1]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 17, (float*)verts[2]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 18, (float*)verts[6]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 19, (float*)verts[5]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 20, (float*)verts[1]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 21, (float*)verts[2]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 22, (float*)verts[6]);
	wrapper.setAttributeValue(VertexAttributeType_Position, numVertices + 23, (float*)verts[5]);

	uint32 numIndices = wrapper.numIndices();
	switch (wrapper.primitiveFormat()) {
	case PrimitiveFormat_Points: {
		wrapper.addIndices(8);
		for (uint32 i = 0; i < 8; ++i)
			wrapper.setIndexValue(numIndices + i, numVertices + i);
		break;
	}
	case PrimitiveFormat_Lines: {
		wrapper.addIndices(24);
		wrapper.setIndexValue(numIndices + 0, numVertices + 0); wrapper.setIndexValue(numIndices + 1, numVertices + 1);
		wrapper.setIndexValue(numIndices + 2, numVertices + 1); wrapper.setIndexValue(numIndices + 3, numVertices + 2);
		wrapper.setIndexValue(numIndices + 4, numVertices + 2); wrapper.setIndexValue(numIndices + 5, numVertices + 3);
		wrapper.setIndexValue(numIndices + 6, numVertices + 3); wrapper.setIndexValue(numIndices + 7, numVertices + 0);
		wrapper.setIndexValue(numIndices + 8, numVertices + 4); wrapper.setIndexValue(numIndices + 9, numVertices + 5);
		wrapper.setIndexValue(numIndices + 10, numVertices + 5); wrapper.setIndexValue(numIndices + 11, numVertices + 6);
		wrapper.setIndexValue(numIndices + 12, numVertices + 6); wrapper.setIndexValue(numIndices + 13, numVertices + 7);
		wrapper.setIndexValue(numIndices + 14, numVertices + 7); wrapper.setIndexValue(numIndices + 15, numVertices + 4);
		wrapper.setIndexValue(numIndices + 16, numVertices + 0); wrapper.setIndexValue(numIndices + 17, numVertices + 5);
		wrapper.setIndexValue(numIndices + 18, numVertices + 1); wrapper.setIndexValue(numIndices + 19, numVertices + 4);
		wrapper.setIndexValue(numIndices + 20, numVertices + 2); wrapper.setIndexValue(numIndices + 21, numVertices + 7);
		wrapper.setIndexValue(numIndices + 22, numVertices + 3); wrapper.setIndexValue(numIndices + 23, numVertices + 6);
		break;
	}
	case PrimitiveFormat_Triangles: {
		wrapper.addIndices(36);
		for (int i = 0; i < 6; ++i) {
			wrapper.setIndexValue(numIndices + 6 * i + 0, numVertices + 4 * i + 0);
			wrapper.setIndexValue(numIndices + 6 * i + 1, numVertices + 4 * i + 1);
			wrapper.setIndexValue(numIndices + 6 * i + 2, numVertices + 4 * i + 2);
			wrapper.setIndexValue(numIndices + 6 * i + 3, numVertices + 4 * i + 0);
			wrapper.setIndexValue(numIndices + 6 * i + 4, numVertices + 4 * i + 2);
			wrapper.setIndexValue(numIndices + 6 * i + 5, numVertices + 4 * i + 3);
		}
		break;
	}
	}

}

void MeshPainter::paintCube(const float len)
{
	paintBox(len, len, len);
}


} // namespace
} // namespace