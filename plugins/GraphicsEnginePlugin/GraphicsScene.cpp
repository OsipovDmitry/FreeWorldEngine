#include <algorithm>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

#include "GraphicsScene.h"
#include "GraphicsModel.h"
#include "GPUMesh.h"
#include "KdTree.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneNode::GraphicsSceneNode(GraphicsSceneNode *pParentNode) :
	m_pParentNode(pParentNode),
	m_pModel(nullptr),
	m_childNodes(),
	m_position(),
	m_orientation(),
	m_pTree(new KdTree()),
	m_needUpdateBoundingBox(false),
	m_needUpdateTransformation(true)
{
}

GraphicsSceneNode::~GraphicsSceneNode()
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p){delete p; });
	delete m_pTree;
}

IGraphicsSceneNode * GraphicsSceneNode::clone(IGraphicsSceneNode * pParent) const
{
	/*IGraphicsSceneNode *pNewNode = pParent->createChild();
	pNewNode->setOrientation(m_orientation);
	pNewNode->setPosition(m_position);
	pNewNode->setModel(m_pModel);
	std::for_each(m_childNodes.cbegin(), m_childNodes.cend(), [pNewNode](const GraphicsSceneNode *p) {
		p->clone(pNewNode);
	});
	return pNewNode;*/
	return nullptr;
}

glm::vec3 GraphicsSceneNode::position() const
{
	return m_position;
}

void GraphicsSceneNode::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	
	m_needUpdateTransformation = true;
	updateTransformationRecursiveDown();

	updateBoundingBoxRecursiveUp();
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;

	m_needUpdateTransformation = true;
	updateTransformationRecursiveDown();

	m_needUpdateBoundingBox = true;
	updateBoundingBoxRecursiveDown();
	updateBoundingBoxRecursiveUp();
}

const glm::mat4x4& GraphicsSceneNode::localTransformation() const
{
	if (m_needUpdateTransformation) {
		recalcTransformation();
		m_needUpdateTransformation = false;
	}
	return m_cacheLocalTransform;
}

const glm::mat4x4& GraphicsSceneNode::worldTransformation() const
{
	if (m_needUpdateTransformation) {
		recalcTransformation();
		m_needUpdateTransformation = false;
	}
	return m_cacheWorldlTransform;
}

IGraphicsSceneNode *GraphicsSceneNode::createChild()
{
	GraphicsSceneNode *pNode = new GraphicsSceneNode(this);
	m_childNodes.push_back(pNode);
	return pNode;
}

void GraphicsSceneNode::destroyChild(IGraphicsSceneNode *pNode)
{
	auto it = std::find(m_childNodes.begin(), m_childNodes.end(), pNode);
	if (it != m_childNodes.end()) {
		m_childNodes.erase(it);
		delete *it;

		m_needUpdateBoundingBox = true;
		updateBoundingBoxRecursiveUp();
	}
}

uint32 GraphicsSceneNode::numChildren() const
{
	return m_childNodes.size();
}

IGraphicsSceneNode *GraphicsSceneNode::childAt(const uint32 idx) const
{
	return m_childNodes.at(idx);
}

IGraphicsSceneNode *GraphicsSceneNode::parentNode() const
{
	return m_pParentNode;
}

IGraphicsModel *GraphicsSceneNode::model() const
{
	return m_pModel;
}

void GraphicsSceneNode::setModel(IGraphicsModel *pModel)
{
	m_pModel = static_cast<GraphicsModel*>(pModel);
	m_needUpdateBoundingBox = true;
	updateBoundingBoxRecursiveUp();
}

KdTree *GraphicsSceneNode::kdTree() const
{
	if (m_needUpdateBoundingBox) {
		recalcBoundingBox();
		m_needUpdateBoundingBox = false;
	}
	return m_pTree;
}

void GraphicsSceneNode::updateTransformationRecursiveDown() const
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p) {
		p->m_needUpdateTransformation = true;
		p->updateTransformationRecursiveDown();
	});
}

void GraphicsSceneNode::recalcTransformation() const
{
	m_cacheLocalTransform = glm::translate(glm::mat4x4(), m_position) * glm::mat4_cast(m_orientation);
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->worldTransformation() : glm::mat4x4()) * m_cacheLocalTransform;
}

void GraphicsSceneNode::updateBoundingBoxRecursiveUp() const
{
	if (m_pParentNode) {
		m_pParentNode->m_needUpdateBoundingBox = true;
		m_pParentNode->updateBoundingBoxRecursiveUp();
	}
}

void GraphicsSceneNode::updateBoundingBoxRecursiveDown() const
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p) {
		p->m_needUpdateBoundingBox = true;
		p->updateBoundingBoxRecursiveDown();
	});
}

void GraphicsSceneNode::recalcBoundingBox() const
{
	m_pTree->rebuild(this);
}

GraphicsScene::GraphicsScene(const std::string& name) :
	m_name(name),
	m_pRootNode(new GraphicsSceneNode(nullptr))
{
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

} // namespace

} // namespace