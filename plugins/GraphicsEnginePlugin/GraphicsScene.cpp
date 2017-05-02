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
	m_needUpdateSphere(true)
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
	m_needUpdateSphere = true;

	if (m_pKdNode)
		m_pScene->kdTree()->reinsertSceneNode(this);
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;
	updateTransformationRecursiveDown();
	m_needUpdateSphere = true;

	if (m_pKdNode)
		m_pScene->kdTree()->reinsertSceneNode(this);
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
	if (m_needUpdateSphere)
		recalcSphere();

	return m_worldSphere;
}

const Math::Aabb& GraphicsSceneNode::worldBoundingBox() const
{
	if (m_needUpdateSphere)
		recalcSphere();

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
	if (pGraphicsNode->m_pParentNode)
		pGraphicsNode->m_pParentNode->detachChildNode(pGraphicsNode);

	m_childNodes.push_back(pGraphicsNode);
	pGraphicsNode->m_pParentNode = this;
}

void GraphicsSceneNode::detachChildNode(IGraphicsSceneNode *pNode)
{
	if (pNode->scene() != this->scene())
		return;

	GraphicsSceneNode *pGraphicsNode = static_cast<GraphicsSceneNode*>(pNode);
	auto it = std::find(m_childNodes.begin(), m_childNodes.end(), pGraphicsNode);
	if (it != m_childNodes.end()) {
		m_childNodes.erase(it);
		(*it)->m_pParentNode = nullptr;
	}
}

uint32 GraphicsSceneNode::numChildNodes() const
{
	return m_childNodes.size();
}

IGraphicsSceneNode *GraphicsSceneNode::childNodeAt(const uint32 idx) const
{
	return m_childNodes.at(idx);
}

IGraphicsModel *GraphicsSceneNode::model() const
{
	return m_pModel;
}

void GraphicsSceneNode::setModel(IGraphicsModel *pModel)
{
	m_pModel = static_cast<GraphicsModel*>(pModel);
	m_needUpdateSphere = true;

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

void GraphicsSceneNode::recalcTransformation() const
{
	m_cacheLocalTransform = glm::translate(glm::mat4x4(), m_position) * glm::mat4_cast(m_orientation);
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->worldTransformation() : glm::mat4x4()) * m_cacheLocalTransform;
	m_needUpdateTransformation = false;
}

void GraphicsSceneNode::recalcSphere() const
{
	Math::Sphere localSphere = m_pModel ? m_pModel->boundingSphere() : Math::Sphere();
	m_worldSphere = Math::Sphere(glm::vec3(worldTransformation() * glm::vec4(glm::vec3(localSphere), 1.0f)), localSphere.w);
	m_worldAabb = Math::Aabb(
		glm::vec3(m_worldSphere) - glm::vec3(m_worldSphere.w), 
		glm::vec3(m_worldSphere) + glm::vec3(m_worldSphere.w)
	);
	m_needUpdateSphere = false;
}

void GraphicsSceneNode::detachFromScene()
{
	m_pParentNode->m_childNodes.remove(this);
	m_pScene->kdTree()->removeSceneNode(this);
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
	delete m_pRootNode;
}

std::string GraphicsScene::name() const
{
	return m_name;
}

IGraphicsSceneNode *GraphicsScene::rootNode() const
{
	return m_pRootNode;
}

IGraphicsSceneNode *GraphicsScene::createNode(IGraphicsSceneNode *pParentNode, const glm::vec3& pos, const glm::quat& orient, IGraphicsModel *pModel)
{
	GraphicsSceneNode *pNewNode = new GraphicsSceneNode(this);
	pNewNode->setPosition(pos);
	pNewNode->setOrientation(orient);
	pNewNode->setModel(pModel);

	m_nodes.push_back(pNewNode);

	if (!pParentNode)
		pParentNode = m_pRootNode;

	pParentNode->attachChildNode(pNewNode);
	return pNewNode;
}

void GraphicsScene::destroyNode(IGraphicsSceneNode *pNode)
{
	if (pNode == m_pRootNode)
		return;

	GraphicsSceneNode *pGraphicsSceneNode = static_cast<GraphicsSceneNode*>(pNode);
	if (pGraphicsSceneNode->scene() != this)
		return;

	while (pNode->numChildNodes())
		destroyNode(pNode->childNodeAt(0));



	m_pTree->removeSceneNode(pGraphicsSceneNode);
	m_nodes.remove(pGraphicsSceneNode);
	if (pGraphicsSceneNode->parentNode())
		pGraphicsSceneNode->parentNode()->detachChildNode(pGraphicsSceneNode);
}

KdTree *GraphicsScene::kdTree() const
{
	return m_pTree;
}

} // namespace

} // namespace