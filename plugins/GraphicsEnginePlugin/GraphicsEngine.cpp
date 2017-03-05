#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>

#include "GraphicsEngine.h"
#include "ShaderManager.h"
#include "GraphicsCamera.h"
#include "GraphicsModel.h"
#include "GraphicsMaterial.h"
#include "GraphicsScene.h"
#include "GraphicsWindow.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsEngine *pGraphicsEngine = 0;
Renderer::IGPURenderer *pGPURenderer = 0;

GraphicsEngine::GraphicsEngine() :
	m_pShaderManager(new ShaderManager),
	m_pProgramManager(new ProgramManager),
	m_pCameraManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsCameras")),
	m_pModelManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsModels")),
	m_pMaterialManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsMaterials")),
	m_pSceneManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsScenes")),
	m_pWindowManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsWindows")),
	m_pCameraNameGenerator(new Utility::AutoNameGenerator("CameraName")),
	m_pModelNameGenerator(new Utility::AutoNameGenerator("ModelName")),
	m_pMaterialNameGenerator(new Utility::AutoNameGenerator("MaterialName")),
	m_pSceneNameGenerator(new Utility::AutoNameGenerator("SceneName")),
	m_pWindowNameGenerator(new Utility::AutoNameGenerator("WindowName"))

{
	//IGraphicsMaterial *pStandardMaterial = createMaterial("StandardMaterial");
}

GraphicsEngine::~GraphicsEngine()
{
	delete m_pModelNameGenerator;
	delete m_pMaterialNameGenerator;
	delete m_pSceneNameGenerator;
	delete m_pWindowNameGenerator;
	delete m_pCameraNameGenerator;
	getCoreEngine()->destroyResourceManager(m_pWindowManager);
	getCoreEngine()->destroyResourceManager(m_pSceneManager);
	getCoreEngine()->destroyResourceManager(m_pModelManager);
	getCoreEngine()->destroyResourceManager(m_pMaterialManager);
	getCoreEngine()->destroyResourceManager(m_pCameraManager);
	delete m_pShaderManager;
	delete m_pProgramManager;
}

IGraphicsCamera *GraphicsEngine::findCamera(const std::string& name) const
{
	return static_cast<IGraphicsCamera*>(m_pCameraManager->findResource(name));
}

IGraphicsCamera *GraphicsEngine::createCamera(const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pCameraNameGenerator->generateName() : name;
	IGraphicsCamera *pCamera = findCamera(resName);
	if (pCamera)
		return pCamera;

	pCamera = new GraphicsCamera(resName);
	m_pCameraManager->addResource(pCamera);
	return pCamera;
}

void GraphicsEngine::destroyCamera(const std::string& name)
{
	m_pCameraManager->destroyResource(name);
}

void GraphicsEngine::destroyCamera(IGraphicsCamera *pCamera)
{
	m_pCameraManager->destroyResource(pCamera);
}

IGraphicsModel *GraphicsEngine::findModel(const std::string& name) const
{
	return static_cast<IGraphicsModel*>(m_pModelManager->findResource(name));
}

IGraphicsModel *GraphicsEngine::createModel(const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pModelNameGenerator->generateName() : name;
	IGraphicsModel *pModel = findModel(resName);
	if (pModel)
		return pModel;

	pModel = new GraphicsModel(resName);
	m_pModelManager->addResource(pModel);
	return pModel;
}

void GraphicsEngine::destroyModel(const std::string& name)
{
	m_pModelManager->destroyResource(name);
}

void GraphicsEngine::destroyModel(IGraphicsModel *pModel)
{
	m_pModelManager->destroyResource(pModel);
}

IGraphicsMaterial *GraphicsEngine::findMaterial(const std::string& name) const
{
	return static_cast<IGraphicsMaterial*>(m_pMaterialManager->findResource(name));
}

IGraphicsMaterial *GraphicsEngine::createMaterial(Renderer::IGPUProgram *pPrgram, const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pMaterialNameGenerator->generateName() : name;
	IGraphicsMaterial *pMaterial = findMaterial(resName);
	if (pMaterial)
		return pMaterial;

	pMaterial = new GraphicsMaterial(resName, pPrgram);
	m_pMaterialManager->addResource(pMaterial);
	return pMaterial;
}

void GraphicsEngine::destroyMaterial(const std::string& name)
{
	m_pMaterialManager->destroyResource(name);
}

void GraphicsEngine::destroyMaterial(IGraphicsMaterial *pMaterial)
{
	m_pMaterialManager->destroyResource(pMaterial);
}

IGraphicsScene *GraphicsEngine::findScene(const std::string& name) const
{
	return static_cast<IGraphicsScene*>(m_pSceneManager->findResource(name));
}

IGraphicsScene *GraphicsEngine::createScene(const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pSceneNameGenerator->generateName() : name;
	IGraphicsScene *pScene = findScene(resName);
	if (pScene)
		return pScene;

	pScene = new GraphicsScene(resName);
	m_pSceneManager->addResource(pScene);
	return pScene;
}

void GraphicsEngine::destroyScene(const std::string& name)
{
	m_pSceneManager->destroyResource(name);
}

void GraphicsEngine::destroyScene(IGraphicsScene *pScene)
{
	m_pSceneManager->destroyResource(pScene);
}

IGraphicsWindow *GraphicsEngine::findWindow(const std::string& name) const
{
	return static_cast<IGraphicsWindow*>(m_pWindowManager->findResource(name));
}

IGraphicsWindow *GraphicsEngine::createWindow(IWindow *pTargetWindow, const std::string& name)
{
	const std::string resName = (name == "@utoname") ? m_pWindowNameGenerator->generateName() : name;
	IGraphicsWindow *pWindow = findWindow(resName);
	if (pWindow)
		return pWindow;

	pWindow = new GraphicsWindow(resName, pTargetWindow);
	m_pWindowManager->addResource(pWindow);
	return pWindow;
}

void GraphicsEngine::destroyWindow(const std::string& name)
{
	m_pWindowManager->destroyResource(name);
}

void GraphicsEngine::destroyWindow(IGraphicsWindow *pWindow)
{
	m_pWindowManager->destroyResource(pWindow);
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