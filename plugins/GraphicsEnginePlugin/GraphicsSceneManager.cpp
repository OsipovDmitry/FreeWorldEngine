#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>

#include "GraphicsSceneManager.h"
#include "GraphicsScene.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsSceneManager::GraphicsSceneManager() :
	m_pSceneManager(ICore::instance()->createResourceManager("ResourceManagerForGraphicsScenes")),
	m_pSceneNameGenerator(new Utility::AutoNameGenerator("SceneName"))
{
}

GraphicsSceneManager::~GraphicsSceneManager()
{
	delete m_pSceneNameGenerator;
	ICore::instance()->destroyResourceManager(m_pSceneManager);
}


IGraphicsScene *GraphicsSceneManager::findScene(const std::string& name) const
{
	return static_cast<IGraphicsScene*>(m_pSceneManager->findResource(name));
}

IGraphicsScene *GraphicsSceneManager::createScene(const std::string& name, IGraphicsScene::SceneOptimizerType optimizerType)
{
	const std::string resName = (name == "@utoname") ? m_pSceneNameGenerator->generateName() : name;
	IGraphicsScene *pScene = findScene(resName);
	if (pScene)
		return pScene;

	pScene = new GraphicsScene(resName, optimizerType);
	m_pSceneManager->addResource(pScene);
	return pScene;
}

void GraphicsSceneManager::destroyScene(const std::string& name)
{
	m_pSceneManager->destroyResource(name);
}

void GraphicsSceneManager::destroyScene(IGraphicsScene *pScene)
{
	m_pSceneManager->destroyResource(pScene->name());
}


} // namespace
} // namespace
