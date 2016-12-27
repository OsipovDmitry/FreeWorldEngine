#include "GraphicsScene.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsScene::GraphicsScene(const std::string& name) :
	m_name(name),
	m_pRootNode(new Node(0))
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

IGraphicsScene::Node *GraphicsScene::rootNode() const
{
	return m_pRootNode;
}

} // namespace

} // namespace