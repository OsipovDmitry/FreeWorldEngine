#include <functional>

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
	m_boundingBox(Math::buildEmptyBoundingBox()),
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
	m_boundingBox = Math::buildEmptyBoundingBox();
	m_sceneNodes.clear();
}

void KdNode::build(const GraphicsSceneNode* pNode)
{
	/*SceneNodesList list;
	list.reserve(pNode->m_childNodes.size());
	std::copy(pNode->m_childNodes.cbegin(), pNode->m_childNodes.cend(), std::back_inserter(list));
	buildRecursive(list, pNode->worldTransformation(), static_cast<GraphicsModel*>(pNode->model()));

	pNode->updateBoundingBoxRecursiveUp();*/
}

const KdNode::SceneNodesList& KdNode::sceneNodes() const
{
	return m_sceneNodes;
}

void KdNode::insertSceneNode(GraphicsSceneNode *pSceneNode)
{
	m_sceneNodes.push_back(pSceneNode);
	pSceneNode->setKdNode(this);
	if (m_sceneNodes.size() > s_maxSceneNodeInKdNode)
		rebuild();
	updateBoundingBox(pSceneNode->worldBoundingBox());
}

void KdNode::removeSceneNode(GraphicsSceneNode *pSceneNode)
{
	KdNode::SceneNodesList::iterator it = std::find(m_sceneNodes.begin(), m_sceneNodes.end(), pSceneNode);
	if (it == m_sceneNodes.end())
		return;
	m_sceneNodes.erase(it);
	pSceneNode->setKdNode(nullptr);
	// todo: οεπερχες boundbox
}

const Math::Aabb& KdNode::boundingBox() const
{
	return m_boundingBox;
}

GPUMesh *KdNode::boundingBoxGpuMesh() const
{
	return m_pBoundingBoxGpuMesh;
}

void KdNode::rebuild()
{
	SceneNodesList newList;
	newList.reserve(m_sceneNodes.size() * 2);

	std::list<KdNode*> queue;
	queue.push_back(this);
	while (!queue.empty()) {
		KdNode *pNode = queue.front();
		queue.pop_front();
		std::copy(pNode->m_sceneNodes.cbegin(), pNode->m_sceneNodes.cend(), std::back_inserter(newList));
		if (!pNode->isLeaf()) {
			queue.push_back(pNode->m_childNodes[0]);
			queue.push_back(pNode->m_childNodes[1]);
		}
	}

	buildRecursive(newList);
}

void KdNode::updateBoundingBox(const Math::Aabb& newChildBox)
{
	if (!Math::containsBoundingBox(m_boundingBox, newChildBox)) {
		m_boundingBox = Math::mergeBoundingBoxes(m_boundingBox, newChildBox);
		if (m_pParent)
			m_pParent->updateBoundingBox(m_boundingBox);

		Mesh aabbMesh;
		Math::MeshWrapper aabbWrapper(&aabbMesh);
		aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
		aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
		aabbWrapper.setVertexStride(3);
		glm::vec3 aabbSize = m_boundingBox.vMax - m_boundingBox.vMin;
		glm::vec3 aabbPos = 0.5f * (m_boundingBox.vMin + m_boundingBox.vMax);
		Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
		aabbWrapper.translateMesh((float*)&(aabbPos));
		m_pBoundingBoxGpuMesh->setMesh(&aabbMesh);
	}
}

void KdNode::buildRecursive(const SceneNodesList& sceneNodesList)
{
	clear();

	if (sceneNodesList.empty())
		return;

	std::vector<Math::Aabb> boxes;
	int32 numBoxes = sceneNodesList.size() / 2;
	boxes.resize(numBoxes);
	for (int32 i = 0; i < numBoxes; ++i)
		boxes[i] = Math::mergeBoundingBoxes(sceneNodesList[2*i+0]->worldBoundingBox(), sceneNodesList[2*i+1]->worldBoundingBox());
	if (sceneNodesList.size() % 2)
		boxes[0] = numBoxes ? 
		Math::mergeBoundingBoxes(boxes[0], sceneNodesList.back()->worldBoundingBox()) :
		sceneNodesList.back()->worldBoundingBox();
	while (numBoxes > 1) {
		if (numBoxes%2)
			boxes[0] = Math::mergeBoundingBoxes(boxes[0], boxes.back());
		numBoxes /= 2;
		for (int32 i = 0; i < numBoxes; ++i)
			boxes[i] = Math::mergeBoundingBoxes(boxes[2*i+0], boxes[2*i+1]);
	}

	const glm::vec3 worldBoxSize(boxes[0].vMax - boxes[0].vMin);
	const glm::vec3 worldBoxPos(0.5f * (boxes[0].vMax + boxes[0].vMin));

	if (worldBoxSize.x <= glm::epsilon<float>() || worldBoxSize.y <= glm::epsilon<float>() || worldBoxSize.z <= glm::epsilon<float>())
		return;

	int32 mainAxis = (worldBoxSize[1] > worldBoxSize[0]) ? 1 : 0;
	if (worldBoxSize[2] > worldBoxSize[mainAxis]) mainAxis = 2;

	Math::Plane splitPlane(0.0f, 0.0f, 0.0f, -worldBoxPos[mainAxis]);
	splitPlane[mainAxis] = (worldBoxPos[mainAxis] >= 0.0f) ? 1.0f : -1.0f;
	
	SceneNodesList backList, frontList, thisList;
	for (const auto& it : sceneNodesList) {
		Math::ClassifyPlane classify = Math::classifyRelativePlane(splitPlane, it->worldBoundingBox());
		if (classify == Math::ClassifyPlane_Front)
			frontList.push_back(it);
		else if (classify == Math::ClassifyPlane_Back)
			backList.push_back(it);
		else
			thisList.push_back(it);
	}

	m_boundingBox = boxes[0];
	m_sceneNodes.swap(thisList);
	std::for_each(m_sceneNodes.begin(), m_sceneNodes.end(), std::bind2nd(std::mem_fun(&GraphicsSceneNode::setKdNode), this));

	if (!backList.empty() || !frontList.empty()) {
		m_childNodes[0] = new KdNode(this);
		m_childNodes[0]->buildRecursive(backList);
		m_childNodes[1] = new KdNode(this);
		m_childNodes[1]->buildRecursive(frontList);
	}

	Mesh aabbMesh;
	Math::MeshWrapper aabbWrapper(&aabbMesh);
	aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	aabbWrapper.setVertexStride(3);
	glm::vec3 aabbSize = m_boundingBox.vMax - m_boundingBox.vMin;
	glm::vec3 aabbPos = 0.5f * (m_boundingBox.vMin + m_boundingBox.vMax);
	Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
	aabbWrapper.translateMesh((float*)&(aabbPos));
	m_pBoundingBoxGpuMesh->setMesh(&aabbMesh);
}

KdNode *KdNode::findDeepestCoveringNode(const Math::Aabb& boundBox)
{
	if (!isLeaf() && Math::containsBoundingBox(m_childNodes[0]->boundingBox(), boundBox))
		return m_childNodes[0]->findDeepestCoveringNode(boundBox);
	else if (!isLeaf() && Math::containsBoundingBox(m_childNodes[1]->boundingBox(), boundBox))
		return m_childNodes[1]->findDeepestCoveringNode(boundBox);
	else
		return Math::containsBoundingBox(m_boundingBox, boundBox) ? this : nullptr;
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

KdNode *KdTree::rootNode() const
{
	return m_pRootNode;
}

void KdTree::reinsertSceneNode(GraphicsSceneNode *pSceneNode)
{
	KdNode *pNode = m_pRootNode->findDeepestCoveringNode(pSceneNode->worldBoundingBox());
	if (!pNode)
		pNode = m_pRootNode;
	
	if (pNode != pSceneNode->kdNode()) {
		removeSceneNode(pSceneNode);
		pNode->insertSceneNode(pSceneNode);
	}
}

void KdTree::removeSceneNode(GraphicsSceneNode *pSceneNode)
{
	KdNode *pNode = pSceneNode->kdNode();
	if (pNode)
		pNode->removeSceneNode(pSceneNode);
}

} // namespace
} // namespace