#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>
#include <math/MathUtils.h>

#include "KdTree.h"
#include "GPUMesh.h"
#include "GraphicsScene.h"
#include "GraphicsModel.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {


KdNode::KdNode(KdNode *pParent) :
	m_pParent(pParent),
	m_localBoundingBox(),
	m_sceneNodes(),
	m_pBoundingBoxGpuMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines))
{
	m_childNodes[0] = m_childNodes[1] = nullptr;
}

KdNode::~KdNode()
{
	clear();
	delete m_pBoundingBoxGpuMesh;
}

void KdNode::clear()
{
	if (m_childNodes[0]) {
		m_childNodes[0]->clear();
		delete m_childNodes[0];
		m_childNodes[0] = nullptr;
	}
	if (m_childNodes[1]) {
		m_childNodes[1]->clear();
		delete m_childNodes[1];
		m_childNodes[1] = nullptr;
	}
	
	m_pBoundingBoxGpuMesh->setMesh(nullptr);
	m_localBoundingBox = Math::Aabb();
	m_sceneNodes.clear();
}

void KdNode::build(const GraphicsSceneNode* pNode)
{
	SceneNodesList list;
	list.reserve(pNode->m_childNodes.size());
	std::copy(pNode->m_childNodes.cbegin(), pNode->m_childNodes.cend(), std::back_inserter(list));
	buildRecursive(list, pNode->worldTransformation(), static_cast<GraphicsModel*>(pNode->model()));

	pNode->updateBoundingBoxRecursiveUp();
}

KdNode *KdNode::firstChild() const
{
	return m_childNodes[0];
}

KdNode *KdNode::secondChild() const
{
	return m_childNodes[1];
}

const KdNode::SceneNodesList& KdNode::sceneNodes() const
{
	return m_sceneNodes;
}

const Math::Aabb& KdNode::localBoundingBox() const
{
	return m_localBoundingBox;
}

Math::Aabb KdNode::worldBoundingBox(const glm::mat4x4& worldTransformation) const
{
	glm::vec3 sceneNodePos(worldTransformation[3]);
	return Math::Aabb(sceneNodePos + m_localBoundingBox.vMin, sceneNodePos + m_localBoundingBox.vMax);
}

GPUMesh *KdNode::boundingBoxGpuMesh() const
{
	return m_pBoundingBoxGpuMesh;
}

void KdNode::buildRecursive(const SceneNodesList& nodesList, const glm::mat4x4& sceneNodeWorldTransformation, GraphicsModel *pModel)
{
	const glm::vec3 sceneNodeWorldPosition(sceneNodeWorldTransformation[3]);

	glm::vec3 worldBoxMin(FLT_MAX);
	glm::vec3 worldBoxMax(-FLT_MAX);

	if (pModel) {
		const Math::Sphere& modelSphere = pModel->boundingSphere();
		const glm::vec3 sphereWorldPos(sceneNodeWorldTransformation * glm::vec4(glm::vec3(modelSphere), 1.0f));
		const glm::vec3 boxSize(modelSphere.w);
		worldBoxMin = sphereWorldPos - boxSize;
		worldBoxMax = sphereWorldPos + boxSize;
	}

	for (const auto& it : nodesList) {
		const Math::Aabb& childWorldBox = it->kdTree()->rootNode()->worldBoundingBox(it->worldTransformation());
		if (childWorldBox.vMin.x < worldBoxMin.x) worldBoxMin.x = childWorldBox.vMin.x;
		if (childWorldBox.vMin.y < worldBoxMin.y) worldBoxMin.y = childWorldBox.vMin.y;
		if (childWorldBox.vMin.z < worldBoxMin.z) worldBoxMin.z = childWorldBox.vMin.z;
		if (childWorldBox.vMax.x > worldBoxMax.x) worldBoxMax.x = childWorldBox.vMax.x;
		if (childWorldBox.vMax.y > worldBoxMax.y) worldBoxMax.y = childWorldBox.vMax.y;
		if (childWorldBox.vMax.z > worldBoxMax.z) worldBoxMax.z = childWorldBox.vMax.z;
	}

	const glm::vec3 worldBoxSize(worldBoxMax - worldBoxMin);
	const glm::vec3 worldBoxPos(0.5f * (worldBoxMax + worldBoxMin));

	if (worldBoxSize.x < 0.0f || worldBoxSize.y < 0.0f || worldBoxSize.z < 0.0f) {
		m_sceneNodes.clear();
		m_localBoundingBox = Math::Aabb();
		m_pBoundingBoxGpuMesh->setMesh(nullptr);
		m_childNodes[0] = m_childNodes[1] = nullptr;
	}

	int32 mainAxis = (worldBoxSize[1] > worldBoxSize[0]) ? 1 : 0;
	if (worldBoxSize[2] > worldBoxSize[mainAxis]) mainAxis = 2;

	Math::Plane splitPlane(0.0f, 0.0f, 0.0f, -worldBoxPos[mainAxis]);
	splitPlane[mainAxis] = (worldBoxPos[mainAxis] >= 0.0f) ? 1.0f : -1.0f;

	SceneNodesList thisList, leftList, rightList;
	for (const auto& it : nodesList) {
		Math::ClassifyPlane classify = Math::classifyRelativePlane(splitPlane, it->kdTree()->rootNode()->worldBoundingBox(it->worldTransformation()));
		if (classify == Math::ClassifyPlane_Front)
			rightList.push_back(it);
		else if (classify == Math::ClassifyPlane_Back)
			leftList.push_back(it);
		else
			thisList.push_back(it);
	}

	m_sceneNodes.reserve(thisList.size());
	std::copy(thisList.cbegin(), thisList.cend(), std::back_inserter(m_sceneNodes));

	if (!leftList.empty()) {
		m_childNodes[0] = new KdNode(this);
		m_childNodes[0]->buildRecursive(leftList, sceneNodeWorldTransformation, nullptr);
	}

	if (!rightList.empty()) {
		m_childNodes[1] = new KdNode(this);
		m_childNodes[1]->buildRecursive(rightList, sceneNodeWorldTransformation, nullptr);
	}

	m_localBoundingBox.vMin = worldBoxMin - sceneNodeWorldPosition;
	m_localBoundingBox.vMax = worldBoxMax - sceneNodeWorldPosition;

	Mesh aabbMesh;
	Math::MeshWrapper aabbWrapper(&aabbMesh);
	aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	aabbWrapper.setVertexStride(3);
	glm::vec3 aabbSize = m_localBoundingBox.vMax - m_localBoundingBox.vMin;
	glm::vec3 aabbPos = sceneNodeWorldPosition + 0.5f * (m_localBoundingBox.vMin + m_localBoundingBox.vMax);
	Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
	aabbWrapper.translateMesh((float*)&(aabbPos));
	m_pBoundingBoxGpuMesh->setMesh(&aabbMesh);

}

KdTree::KdTree() :
	m_pRootNode(new KdNode(nullptr))
{
}

KdTree::~KdTree()
{
	m_pRootNode->clear();
	delete m_pRootNode;
}

void KdTree::rebuild(const GraphicsSceneNode *pNode)
{
	m_pRootNode->clear();
	m_pRootNode->build(pNode);
}

KdNode *KdTree::rootNode() const
{
	return m_pRootNode;
}

} // namespace
} // namespace