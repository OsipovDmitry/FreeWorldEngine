#include <algorithm>
#include <functional>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

#include "GraphicsScene.h"
#include "GraphicsModel.h"
#include "GPUMesh.h"
#include "KdTree.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneNode::GraphicsSceneNode(GraphicsScene *pScene) :
	m_pScene(pScene),
	m_pParentNode(nullptr),
	m_pModel(nullptr),
	m_pKdNode(nullptr),
	m_childNodes(),
	m_position(),
	m_orientation(),
	m_needUpdateTransformation(true),
	m_needUpdateBoundingVolumes(true)
{
}

GraphicsSceneNode::~GraphicsSceneNode()
{
}

glm::vec3 GraphicsSceneNode::position() const
{
	return m_position;
}

void GraphicsSceneNode::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	updateTransformationRecursiveDown();
	updateBoundingVolumesRecursiveDown();
	updateKdNodesRecursiveDown();
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;
	updateTransformationRecursiveDown();
	updateBoundingVolumesRecursiveDown();
	updateKdNodesRecursiveDown();
}

const glm::mat4x4& GraphicsSceneNode::localTransformation() const
{
	if (m_needUpdateTransformation)
		recalcTransformation();

	return m_cacheLocalTransform;
}

const glm::mat4x4& GraphicsSceneNode::worldTransformation() const
{
	if (m_needUpdateTransformation)
		recalcTransformation();

	return m_cacheWorldlTransform;
}

const Math::Sphere& GraphicsSceneNode::worldBoundingSphere() const
{
	if (m_needUpdateBoundingVolumes)
		recalcBoundingVolumes();

	return m_worldSphere;
}

const Math::Aabb& GraphicsSceneNode::worldBoundingBox() const
{
	if (m_needUpdateBoundingVolumes)
		recalcBoundingVolumes();

	return m_worldAabb;
}

IGraphicsSceneNode *GraphicsSceneNode::parentNode() const
{
	return m_pParentNode;
}

IGraphicsScene *GraphicsSceneNode::scene() const
{
	return m_pScene;
}

void GraphicsSceneNode::attachChildNode(IGraphicsSceneNode *pNode)
{
	if (pNode->scene() != this->scene())
		return;

	GraphicsSceneNode *pGraphicsNode = static_cast<GraphicsSceneNode*>(pNode);

	IGraphicsSceneNode *pParentNode = pGraphicsNode->parentNode();
	if (pParentNode)
		pParentNode->detachChildNode(pGraphicsNode);

	m_childNodes.push_back(pGraphicsNode);
	pGraphicsNode->m_pParentNode = this;

	KdTree *pKdTree = m_pScene->kdTree();
	std::list<GraphicsSceneNode*> nodes;
	nodes.push_back(pGraphicsNode);
	while (!nodes.empty()) {
		GraphicsSceneNode *p = nodes.front();
		nodes.pop_front();
		std::copy(p->m_childNodes.cbegin(), p->m_childNodes.cend(), std::back_inserter(nodes));
		pKdTree->reinsertSceneNode(p);
	}
}

void GraphicsSceneNode::detachChildNode(IGraphicsSceneNode *pNode)
{
	if (pNode->scene() != this->scene())
		return;

	auto it = std::find(m_childNodes.cbegin(), m_childNodes.cend(), static_cast<GraphicsSceneNode*>(pNode));
	if (it != m_childNodes.cend()) {
		m_childNodes.erase(it);
		(*it)->m_pParentNode = nullptr;
		KdTree *pKdTree = m_pScene->kdTree();
		std::list<GraphicsSceneNode*> nodes;
		nodes.push_back(*it);
		while (!nodes.empty()) {
			GraphicsSceneNode *p = nodes.front();
			nodes.pop_front();
			std::copy(p->m_childNodes.cbegin(), p->m_childNodes.cend(), std::back_inserter(nodes));
			pKdTree->removeSceneNode(p);
		}
	}
}

uint32 GraphicsSceneNode::numChildNodes() const
{
	return m_childNodes.size();
}

IGraphicsSceneNode *GraphicsSceneNode::childNodeAt(const uint32 idx) const
{
	return m_childNodes[idx];
}

IGraphicsModel *GraphicsSceneNode::model() const
{
	return m_pModel;
}

void GraphicsSceneNode::setModel(IGraphicsModel *pModel)
{
	m_pModel = static_cast<GraphicsModel*>(pModel);
	m_needUpdateBoundingVolumes = true;

	if (m_pKdNode) {
		m_pKdNode->removeSceneNode(this);
		m_pScene->kdTree()->reinsertSceneNode(this);
	}
}

KdNode *GraphicsSceneNode::kdNode() const
{
	return m_pKdNode;
}

void GraphicsSceneNode::setKdNode(KdNode *pKdNode)
{
	m_pKdNode = pKdNode;
}

void GraphicsSceneNode::updateTransformationRecursiveDown() const
{
	m_needUpdateTransformation = true;
	
	std::for_each(m_childNodes.begin(), m_childNodes.end(),
		std::mem_fun(&GraphicsSceneNode::updateTransformationRecursiveDown)
	);
}

void GraphicsSceneNode::updateBoundingVolumesRecursiveDown() const
{
	m_needUpdateBoundingVolumes = false;

	std::for_each(m_childNodes.begin(), m_childNodes.end(),
		std::mem_fun(&GraphicsSceneNode::updateBoundingVolumesRecursiveDown)
		);
}

void GraphicsSceneNode::recalcTransformation() const
{
	m_cacheLocalTransform = glm::translate(glm::mat4x4(), m_position) * glm::mat4_cast(m_orientation);
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->worldTransformation() : glm::mat4x4()) * m_cacheLocalTransform;
	m_needUpdateTransformation = false;
}

void GraphicsSceneNode::recalcBoundingVolumes() const
{
	Math::Sphere localSphere = m_pModel ? m_pModel->boundingSphere() : Math::Sphere();
	m_worldSphere = Math::Sphere(glm::vec3(worldTransformation() * glm::vec4(glm::vec3(localSphere), 1.0f)), localSphere.w);
	m_worldAabb = Math::Aabb(
		glm::vec3(m_worldSphere) - glm::vec3(m_worldSphere.w), 
		glm::vec3(m_worldSphere) + glm::vec3(m_worldSphere.w)
	);
	m_needUpdateBoundingVolumes = false;
}

void GraphicsSceneNode::updateKdNodesRecursiveDown()
{
	if (m_pKdNode) {
		std::for_each(m_childNodes.begin(), m_childNodes.end(), std::mem_fun(&GraphicsSceneNode::updateKdNodesRecursiveDown));
		m_pScene->kdTree()->reinsertSceneNode(this);
	}
}

GraphicsScene::GraphicsScene(const std::string& name) :
	m_name(name),
	m_pTree(new KdTree),
	m_nodes()
{
	m_pRootNode = static_cast<GraphicsSceneNode*>(createNode());
	m_pTree->reinsertSceneNode(m_pRootNode);
}

GraphicsScene::~GraphicsScene()
{
	delete m_pTree;
	std::for_each(m_nodes.begin(), m_nodes.end(), [](GraphicsSceneNode *p) {
		delete p;
	});
}

std::string GraphicsScene::name() const
{
	return m_name;
}

IGraphicsSceneNode *GraphicsScene::rootNode() const
{
	return m_pRootNode;
}

IGraphicsSceneNode *GraphicsScene::createNode()
{
	GraphicsSceneNode *pNewNode = new GraphicsSceneNode(this);
	m_nodes.push_back(pNewNode);
	return pNewNode;
}

void GraphicsScene::destroyNode(IGraphicsSceneNode *pNode)
{
	if (pNode == m_pRootNode)
		return;

	GraphicsSceneNode *pGraphicsSceneNode = static_cast<GraphicsSceneNode*>(pNode);
	if (pGraphicsSceneNode->scene() != this)
		return;

	IGraphicsSceneNode *pParentNode = pGraphicsSceneNode->parentNode();
	if (pParentNode)
		pParentNode->detachChildNode(pGraphicsSceneNode);

	std::list<GraphicsSceneNode*> nodes;
	nodes.push_back(pGraphicsSceneNode);
	while (!nodes.empty()) {
		GraphicsSceneNode *p = nodes.front();
		nodes.pop_front();
		uint32 numChildNodes = p->numChildNodes();
		for (uint32 i = 0; i < numChildNodes; ++i)
			nodes.push_back(static_cast<GraphicsSceneNode*>(p->childNodeAt(i)));
		m_nodes.remove(p);
		delete p;
	}
}

KdTree *GraphicsScene::kdTree() const
{
	return m_pTree;
}

} // namespace

} // namespace