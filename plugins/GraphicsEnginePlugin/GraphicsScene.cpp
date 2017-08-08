#include <algorithm>
#include <functional>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <utility/AutoNameGenerator.h>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>
#include <math/MathUtils.h>

#include <core/IResourceManager.h>
#include <core/ICore.h>

#include "GraphicsScene.h"
#include "GraphicsModel.h"
#include "GraphicsLight.h"
#include "GPUMesh.h"
#include "AbstractSceneOptimizer.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneNode::GraphicsSceneNode(GraphicsScene *pScene) :
	m_pScene(pScene),
	m_pParentNode(nullptr),
	m_pModel(nullptr),
	m_pOptimizerData(nullptr),
	m_childNodes(),
	m_position(),
	m_orientation(),
	m_needUpdateTransformation(true),
	m_needUpdateBoundingSphere(true)
{
	m_pScene->optimizer()->nodeCreated(this);
}

GraphicsSceneNode::~GraphicsSceneNode()
{
	m_pScene->optimizer()->nodeDestroyed(this);
}

glm::vec3 GraphicsSceneNode::position() const
{
	return m_position;
}

void GraphicsSceneNode::setPosition(const glm::vec3& pos)
{
	m_position = pos;
	updateBoundingSpheresRecursiveDown();
	updateTransformationRecursiveDown();
}

glm::quat GraphicsSceneNode::orientation()
{
	return m_orientation;
}

void GraphicsSceneNode::setOrientation(const glm::quat& orient)
{
	m_orientation = orient;
	updateBoundingSpheresRecursiveDown();
	updateTransformationRecursiveDown();
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
	if (m_needUpdateBoundingSphere)
		recalcBoundingSphere();

	return m_worldSphere;
}

IGraphicsSceneNode *GraphicsSceneNode::parentNode() const
{
	return parentSceneNodeImpl();
}

IGraphicsScene *GraphicsSceneNode::scene() const
{
	return sceneImpl();
}

void GraphicsSceneNode::attachChildNode(IGraphicsSceneNode *pNode)
{
	GraphicsSceneNode *pGraphicsNode = static_cast<GraphicsSceneNode*>(pNode);

	if (pGraphicsNode->sceneImpl() != this->sceneImpl())
		return;

	GraphicsSceneNode *pParentNode = pGraphicsNode->parentSceneNodeImpl();

	if (pParentNode == this)
		return;

	if (pParentNode)
		pParentNode->detachChildNode(pGraphicsNode);

	m_childNodes.push_back(pGraphicsNode);
	pGraphicsNode->m_pParentNode = this;
	m_pScene->optimizer()->nodeAttached(pGraphicsNode);
}

void GraphicsSceneNode::detachChildNode(IGraphicsSceneNode *pNode)
{
	GraphicsSceneNode *pGraphicsNode = static_cast<GraphicsSceneNode*>(pNode);

	if (pGraphicsNode->sceneImpl() != this->sceneImpl())
		return;

	auto it = std::find(m_childNodes.cbegin(), m_childNodes.cend(), pGraphicsNode);
	if (it != m_childNodes.cend()) {
		m_pScene->optimizer()->nodeDetached(pGraphicsNode);
		m_childNodes.erase(it);
		(*it)->m_pParentNode = nullptr;
	}
}

uint32 GraphicsSceneNode::numChildNodes() const
{
	return chilNodesImpl().size();
}

IGraphicsSceneNode *GraphicsSceneNode::childNodeAt(const uint32 idx) const
{
	return chilNodesImpl()[idx];
}

IGraphicsModel *GraphicsSceneNode::model() const
{
	return modelImpl();
}

void GraphicsSceneNode::setModel(IGraphicsModel *pModel)
{
	m_pModel = static_cast<GraphicsModel*>(pModel);
	m_needUpdateBoundingSphere = true;
	m_pScene->optimizer()->nodeModelChanged(this);
}

GraphicsModel *GraphicsSceneNode::modelImpl() const
{
	return m_pModel;
}

GraphicsScene *GraphicsSceneNode::sceneImpl() const
{
	return m_pScene;
}

GraphicsSceneNode*GraphicsSceneNode::parentSceneNodeImpl() const
{
	return m_pParentNode;
}

const GraphicsSceneNode::ChildrenList& GraphicsSceneNode::chilNodesImpl() const
{
	return m_childNodes;
}

void GraphicsSceneNode::setOptimizerData(void* pData)
{
	m_pOptimizerData = pData;
}

void *GraphicsSceneNode::optimizerData() const
{
	return m_pOptimizerData;
}

void GraphicsSceneNode::updateTransformationRecursiveDown()
{
	m_needUpdateTransformation = true;
	m_pScene->optimizer()->nodeTransformationChanged(this);
	
	std::for_each(m_childNodes.begin(), m_childNodes.end(),
		std::mem_fun(&GraphicsSceneNode::updateTransformationRecursiveDown)
	);
}

void GraphicsSceneNode::updateBoundingSpheresRecursiveDown() const
{
	m_needUpdateBoundingSphere = true;

	std::for_each(m_childNodes.begin(), m_childNodes.end(),
		std::mem_fun(&GraphicsSceneNode::updateBoundingSpheresRecursiveDown)
				  );
}

void GraphicsSceneNode::recalcTransformation() const
{
	m_cacheLocalTransform = glm::translate(glm::mat4x4(), m_position) * glm::mat4_cast(m_orientation);
	m_cacheWorldlTransform = (m_pParentNode ? m_pParentNode->worldTransformation() : glm::mat4x4()) * m_cacheLocalTransform;
	m_needUpdateTransformation = false;
}

void GraphicsSceneNode::recalcBoundingSphere() const
{
	Math::Sphere localSphere = m_pModel ? m_pModel->boundingSphere() : Math::buildEmptySphere();
	m_worldSphere = Math::Sphere(glm::vec3(worldTransformation() * glm::vec4(glm::vec3(localSphere), 1.0f)), localSphere.w);
	m_needUpdateBoundingSphere = false;
}

GraphicsScene::GraphicsScene(const std::string& name, SceneOptimizerType optimizerType) :
	m_name(name),
	m_nodes(),
	m_pOptimizer(nullptr),
	m_pRootNode(nullptr),
	m_pLightManager(ICore::instance()->createResourceManager("ResourceManagerForLights_"+name)),
	m_pLightNameGenerator(new Utility::AutoNameGenerator("Scene:"+name+"_Light:"))
{
	m_pOptimizer = AbstractSceneOptimizer::createSceneOptimizer(optimizerType, this);
	m_pRootNode = static_cast<GraphicsSceneNode*>(createNode());
	m_pOptimizer->nodeAttached(m_pRootNode); // Насильно вызываем метод nodeAttached у оптимизатора, так как root-node никогда не будет никуда приаттачен
}

GraphicsScene::~GraphicsScene()
{
	delete m_pLightNameGenerator;
	ICore::instance()->destroyResourceManager(m_pLightManager);

	// TODO: Правильно отсоединение и удаление всех SceneNode из m_nodes

	delete m_pOptimizer;
}

std::string GraphicsScene::name() const
{
	return m_name;
}

IGraphicsSceneNode *GraphicsScene::rootNode() const
{
	return rootNodeImpl();
}

IGraphicsSceneNode *GraphicsScene::createNode()
{
	GraphicsSceneNode *pNewNode = new GraphicsSceneNode(this);
	m_nodes.push_back(pNewNode);
	return pNewNode;
}

void GraphicsScene::destroyNode(IGraphicsSceneNode *pNode)
{
	GraphicsSceneNode *pGraphicsSceneNode = static_cast<GraphicsSceneNode*>(pNode);

	if (pGraphicsSceneNode->sceneImpl() != this)
		return;

	if (pGraphicsSceneNode == m_pRootNode)
		return;

	GraphicsSceneNode *pParentNode = pGraphicsSceneNode->parentSceneNodeImpl();
	if (pParentNode)
		pParentNode->detachChildNode(pGraphicsSceneNode);

	std::list<GraphicsSceneNode*> nodes;
	nodes.push_back(pGraphicsSceneNode);
	while (!nodes.empty()) {
		GraphicsSceneNode *p = nodes.front();
		nodes.pop_front();
		std::copy(p->chilNodesImpl().cbegin(), p->chilNodesImpl().cend(), std::back_inserter(nodes));
		m_nodes.remove(p);
		delete p;
	}
}

IGraphicsLight *GraphicsScene::findLight(const std::string& name) const
{
	return static_cast<IGraphicsLight*>(m_pLightManager->findResource(name));
}

IGraphicsLight *GraphicsScene::createLight(IGraphicsLight::Type type, const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pLightNameGenerator->generateName() : name;
	IGraphicsLight *pLight = findLight(resName);
	if (pLight)
		return pLight;

	pLight = new GraphicsLight(this, type, resName);
	m_pLightManager->addResource(pLight);
	return pLight;
}

void GraphicsScene::destroyLight(const std::string& name)
{
	GraphicsLight *pGraphicsLight = static_cast<GraphicsLight*>(m_pLightManager->findResource(name));

	if (!pGraphicsLight)
		return;

	if (pGraphicsLight->sceneImpl() != this)
		return;

	m_pLightManager->destroyResource(name);
}

void GraphicsScene::destroyLight(IGraphicsLight* pLight)
{
	return destroyLight(pLight->name());
}

IGraphicsScene::SceneOptimizerType GraphicsScene::optimizerType() const
{
	return m_pOptimizer->type();
}

AbstractSceneOptimizer *GraphicsScene::optimizer() const
{
	return m_pOptimizer;
}

GraphicsSceneNode*GraphicsScene::rootNodeImpl() const
{
	return m_pRootNode;
}

IResourceManager*GraphicsScene::lightsImpl() const
{
	return m_pLightManager;
}

} // namespace

} // namespace
