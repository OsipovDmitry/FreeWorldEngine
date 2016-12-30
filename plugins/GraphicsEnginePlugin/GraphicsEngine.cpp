#include <FreeWorldEngine.h>

#include "GraphicsEngine.h"
#include "ShaderManager.h"
#include "GraphicsModel.h"
#include "GraphicsMaterial.h"
#include "GraphicsScene.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsEngine *pGraphicsEngine = 0;
Renderer::IGPURenderer *pGPURenderer = 0;

GraphicsEngine::GraphicsEngine() :
	m_pShaderManager(new ShaderManager),
	m_pProgramManager(new ProgramManager),
	m_pModelManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsModels")),
	m_pMaterialManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsMaterials")),
	m_pSceneManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsScenes"))
{
	IGraphicsMaterial *pStandardMaterial = createMaterial("StandardMaterial");
}

GraphicsEngine::~GraphicsEngine()
{
	getCoreEngine()->destroyResourceManager(m_pSceneManager);
	getCoreEngine()->destroyResourceManager(m_pModelManager);
	getCoreEngine()->destroyResourceManager(m_pMaterialManager);
	delete m_pShaderManager;
	delete m_pProgramManager;
}

IGraphicsModel *GraphicsEngine::findModel(const std::string& name) const
{
	return static_cast<IGraphicsModel*>(m_pModelManager->findResource(name));
}

IGraphicsModel *GraphicsEngine::createModel(const std::string& name)
{
	IGraphicsModel *pModel = findModel(name);
	if (pModel)
		return pModel;

	pModel = new GraphicsModel(name);
	m_pModelManager->addResource(pModel);
	return pModel;
}

void GraphicsEngine::destroyModel(const std::string& name)
{
	m_pModelManager->destroyResource(name);
}

IGraphicsMaterial *GraphicsEngine::findMaterial(const std::string& name) const
{
	return static_cast<IGraphicsMaterial*>(m_pMaterialManager->findResource(name));
}

IGraphicsMaterial *GraphicsEngine::createMaterial(const std::string& name)
{
	IGraphicsMaterial *pMaterial = findMaterial(name);
	if (pMaterial)
		return pMaterial;

	pMaterial = new GraphicsMaterial(name);
	m_pMaterialManager->addResource(pMaterial);
	return pMaterial;
}

void GraphicsEngine::destroyMaterial(const std::string& name)
{
	m_pMaterialManager->destroyResource(name);
}

IGraphicsScene *GraphicsEngine::findScene(const std::string& name) const
{
	return static_cast<IGraphicsScene*>(m_pSceneManager->findResource(name));
}

IGraphicsScene *GraphicsEngine::createScene(const std::string& name)
{
	IGraphicsScene *pScene = findScene(name);
	if (pScene)
		return pScene;

	pScene = new GraphicsScene(name);
	m_pSceneManager->addResource(pScene);
	return pScene;
}

void GraphicsEngine::destroyScene(const std::string& name)
{
	m_pSceneManager->destroyResource(name);
}

ShaderManager *GraphicsEngine::shaderManager() const
{
	return m_pShaderManager;
}

ProgramManager *GraphicsEngine::programManager() const
{
	return m_pProgramManager;
}


} // namespace

} // namespace