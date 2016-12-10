#include <queue>

#include <BspTree.h>
#include <MeshWrapper.h>
#include <MathUtils.h>

namespace FreeWorldEngine {

namespace Math {

BspTree::BspTree() :
	m_pRootNode(0),
	m_pDestMesh(0)
{
}

BspTree::~BspTree()
{
	destroy();
}

void BspTree::destroy()
{
	std::queue<BspNode*> nodes;
	nodes.push(m_pRootNode);

	while (nodes.size()) {
		BspNode *pNode = nodes.front();
		nodes.pop();

		if (!pNode)
			continue;

		nodes.push(pNode->pLeftNode);
		nodes.push(pNode->pRightNode);

		delete pNode->pIndices;
		delete pNode;
	}

	m_pRootNode = 0;
	MeshWrapper(m_pDestMesh).destroyTargetMesh();
	m_pDestMesh = 0;
}

void BspTree::build(Mesh *pSourceMesh, const float eps)
{
	destroy();

	if (!pSourceMesh)
		return;

	// ���������, ��� ��� ���������� ���� PrimitiveFormat_Trangles � ������� ������� AttributeType_Position
	if (pSourceMesh->primitiveFormat != PrimitiveFormat_Trangles ||
		pSourceMesh->attributes.find(AttributeType_Position) == pSourceMesh->attributes.end() ||
		pSourceMesh->numIndices < 3)
		return;

	m_pDestMesh = MeshWrapper(pSourceMesh).clone();

	std::vector<uint32> indices(m_pDestMesh->numIndices);
	memcpy(indices.data(), m_pDestMesh->pIndexData, m_pDestMesh->numIndices * sizeof(uint32));
	
	m_pRootNode = buildNode(indices, eps);
}

Plane BspTree::calcSeparatingPlane(const std::vector<uint32>& indices) const
{
	// ���� ��� ���������� ��������� ����������� �� ������� ������������. ������� ���� ���.
	MeshWrapper mesh(m_pDestMesh);
	return buildPlane(
		*((glm::vec3*)mesh.attributeValue(AttributeType_Position, indices.at(0))),
		*((glm::vec3*)mesh.attributeValue(AttributeType_Position, indices.at(1))), 
		*((glm::vec3*)mesh.attributeValue(AttributeType_Position, indices.at(2))));
}

BspNode *BspTree::buildNode(const std::vector<uint32>& indices, const float eps)
{
	if (indices.size() < 3)
		return 0;

	Plane sepPlane = calcSeparatingPlane(indices);
	std::vector<uint32> leftIndices(0), rightIndices(0), thisIndices(0);

	MeshWrapper mesh(m_pDestMesh);

	for (uint32 i = 0; i < indices.size(); i += 3) {
		uint32 idxs[3] = { indices.at(i+0), indices.at(i+1), indices.at(i+2) };
		glm::vec3 *verts[3] = {(glm::vec3*)mesh.attributeValue(AttributeType_Position, idxs[0]),
							   (glm::vec3*)mesh.attributeValue(AttributeType_Position, idxs[1]),
							   (glm::vec3*)mesh.attributeValue(AttributeType_Position, idxs[2])};
		ClassifyPlane classifyResult = classifyPolygonRelativePlane(sepPlane, verts, 3, eps);
		switch (classifyResult) {
		case ClassifyPlane_Touch: { thisIndices.push_back(idxs[0]); thisIndices.push_back(idxs[1]); thisIndices.push_back(idxs[2]); break; }
		case ClassifyPlane_Back: { leftIndices.push_back(idxs[0]); leftIndices.push_back(idxs[1]); leftIndices.push_back(idxs[2]); break; }
		case ClassifyPlane_Front: { rightIndices.push_back(idxs[0]); rightIndices.push_back(idxs[1]); rightIndices.push_back(idxs[2]); break; }
		case ClassifyPlane_Intersect: {
			float coefs[3];
			cutTriangle(verts, sepPlane, coefs[0], coefs[1], coefs[2], eps);
			int32 numCuts = (int32)(coefs[0] > -eps) + (int32)(coefs[1] > -eps) + (int32)(coefs[2] > -eps);
			if (numCuts == 2) {
				int32 edgeIdx = 0;
				for ( ; coefs[edgeIdx] > -eps; ++edgeIdx) ; // ���� ������ �����, ������� �� ����� ������
				uint32 cutIdx[3] = { idxs[(edgeIdx+1)%3], idxs[(edgeIdx+2)%3], idxs[edgeIdx] }; // ������� ������ ������������, ����� ����� ��������, ����� ���������.
				uint32 newIdx = m_pDestMesh->numVertices;
				float *pV = mesh.addVertices(2); // ��������� 2 �������
				mesh.interpolateTwoVertices(cutIdx[0], cutIdx[1], coefs[(edgeIdx+1)%3], pV); // ������������� ��������
				mesh.interpolateTwoVertices(cutIdx[1], cutIdx[2], coefs[(edgeIdx+2)%3], pV+m_pDestMesh->vertexStride);
				uint32 *pI = mesh.addIndices(6); // ��������� 6 �������� � ���������
				m_pDestMesh->pIndexData[indices[i+0]] = cutIdx[1];
				m_pDestMesh->pIndexData[indices[i+1]] = newIdx+1;
				m_pDestMesh->pIndexData[indices[i+2]] = newIdx;
				pI[0] = newIdx; pI[1] = newIdx+1; pI[2] = cutIdx[2];
				pI[3] = newIdx; pI[4] = cutIdx[2]; pI[5] = cutIdx[0];
				if (distToPlane(sepPlane, *(verts[edgeIdx])) > 0.0f) { // ��������� �� ����� ������� �� ��������� ����� ����� ����������� � ���������
					rightIndices.push_back(newIdx); rightIndices.push_back(newIdx+1); rightIndices.push_back(cutIdx[2]);
					rightIndices.push_back(newIdx); rightIndices.push_back(cutIdx[2]); rightIndices.push_back(cutIdx[0]);
					leftIndices.push_back(cutIdx[1]); leftIndices.push_back(newIdx+1); leftIndices.push_back(newIdx);
				} else {
					leftIndices.push_back(newIdx); leftIndices.push_back(newIdx+1); leftIndices.push_back(cutIdx[2]);
					leftIndices.push_back(newIdx); leftIndices.push_back(cutIdx[2]); leftIndices.push_back(cutIdx[0]);
					rightIndices.push_back(cutIdx[1]); rightIndices.push_back(newIdx+1); rightIndices.push_back(newIdx);
				}
			} else if (numCuts == 1) {
				int32 edgeIdx = 0;
				for ( ; coefs[edgeIdx] < -eps; ++edgeIdx) ; // ���� ������ �����, ������� ����� ������
				uint32 cutIdx[2] = { idxs[edgeIdx], idxs[(edgeIdx+1)%3] }; // ������� ������ ������������, ����� ����� ��������, ����� ���������.
				uint32 vertPlaneIdx = idxs[(edgeIdx+2)%3]; // ������ �������������� ������� (��� �� ���������)
				uint32 newIdx = m_pDestMesh->numVertices;
				float *pV = mesh.addVertices(1); // ��������� 1 �������
				mesh.interpolateTwoVertices(cutIdx[0], cutIdx[1], coefs[edgeIdx], pV); // ������������� � ��� ��������
				uint32 *pI = mesh.addIndices(3); // ��������� 3 ������� � ���������
				m_pDestMesh->pIndexData[indices[i+0]] = vertPlaneIdx;
				m_pDestMesh->pIndexData[indices[i+1]] = cutIdx[0];
				m_pDestMesh->pIndexData[indices[i+2]] = newIdx;
				pI[0] = newIdx; pI[1] = cutIdx[1]; pI[2] = vertPlaneIdx;
				if (distToPlane(sepPlane, *(verts[edgeIdx])) > 0.0f) { // ��������� �� ����� ������� �� ��������� ����� ����� ����������� � ���������
					rightIndices.push_back(vertPlaneIdx); rightIndices.push_back(cutIdx[0]); rightIndices.push_back(newIdx);
					leftIndices.push_back(newIdx); leftIndices.push_back(cutIdx[1]); leftIndices.push_back(vertPlaneIdx);
				} else {
					leftIndices.push_back(vertPlaneIdx); leftIndices.push_back(cutIdx[0]); leftIndices.push_back(newIdx);
					rightIndices.push_back(newIdx); rightIndices.push_back(cutIdx[1]); rightIndices.push_back(vertPlaneIdx);
				}
			}
			else { thisIndices.push_back(idxs[0]); thisIndices.push_back(idxs[1]); thisIndices.push_back(idxs[2]); }
			break;
		}
		}
	}

	BspNode *pNode = new BspNode;
	pNode->numIndices = thisIndices.size();
	pNode->pIndices = new uint32[thisIndices.size()];
	memcpy(pNode->pIndices, thisIndices.data(), thisIndices.size()*sizeof(uint32));
	pNode->pLeftNode = buildNode(leftIndices, eps);
	pNode->pRightNode = buildNode(rightIndices, eps);

	return pNode;
}

} // namespace
} // namespace
