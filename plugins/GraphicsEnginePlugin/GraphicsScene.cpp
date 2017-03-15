#include <algorithm>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include "GraphicsScene.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneNode::GraphicsSceneNode(GraphicsSceneNode *pParentNode) :
	m_pParentNode(pParentNode),
	m_pModel(nullptr),
	m_childNodes(),
	m_position(),
	m_orientation()
{
	updateTransform();
}

GraphicsSceneNode::~GraphicsSceneNode()
{
	std::for_each(m_childNodes.begin(), m_childNodes.end(), [](GraphicsSceneNode *p){delete p; });
}

glm::vec3 GraphicsSceneNode::position() const
{
	return m_position;
}

void GraphicsSceneNode::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	updateTransform();
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;
	updateTransform();
}

glm::mat4x4 GraphicsSceneNode::transformation() const
{
	return m_cacheTransform;
}

glm::mat4x4 GraphicsSceneNode::worldTransformation() const
{
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
	m_pModel = pModel;
}

void GraphicsSceneNode::updateTransform()
{
	m_cacheTransform = glm::translate(glm::mat4x4(), m_position) * glm::mat4x4(m_orientation);
	updateWorldTransform();
}

void GraphicsSceneNode::updateWorldTransform()
{
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->m_cacheWorldlTransform : glm::mat4x4()) * m_cacheTransform;
	for (auto it : m_childNodes)
		it->updateWorldTransform();
}

GraphicsScene::GraphicsScene(const std::string& name) :
	m_name(name),
	m_pRootNode(new GraphicsSceneNode(0))
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