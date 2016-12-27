#include <ctime>

#include "FreeWorldEngine.h"

#include "Core.h"
#include "PluginManager.h"
#include "ResourceManager.h"
#include "LibraryManager.h"
#include "TextFileLog.h"
#include "ThreadManager.h"

namespace {
	const std::string c_pluginsFileName = "plugins.xml";
	const std::string c_textLogFileName = "TextLog.txt";
}

namespace FreeWorldEngine {

Core::Core() :
	m_pLogger(0),
	m_pManagerForOtherManagers(0),
	m_pLibraryManager(0),
	m_pPluginManager(0),
	m_pThreadManager(0),
	m_pImageLoader(0),
	m_pSceneLoader(0),
	m_pWindowManager(0),
	m_pGPURenderer(0),
	m_pGraphicsEngine(0)
{
}

Core::~Core()
{
}

void Core::initialize()
{
	m_pManagerForOtherManagers = new ResourceManagerHash("ResourceManagerForOtherManagers");

	m_pLogger = new Logger;
	m_pLogger->addLog(new TextFileLog(c_textLogFileName, false));

	m_pThreadManager = new ThreadManager();

	m_pLibraryManager = new LibraryManager();

	m_pPluginManager = new PluginManager();
	m_pPluginManager->loadPlugins(c_pluginsFileName);

	if (m_pWindowManager)
		m_pMainWindow = m_pWindowManager->createWindow("Free World Engine Demo", 1024, 768, IWindow::Flags_Show | IWindow::Flags_Resizable);
}

void Core::deinitialize()
{
	m_pPluginManager->unloadPlugins();

	delete m_pGPURenderer;
	m_pGPURenderer = 0;

	delete m_pWindowManager;
	m_pWindowManager = 0;

	delete m_pImageLoader;
	m_pImageLoader = 0;

	delete m_pSceneLoader;
	m_pSceneLoader = 0;

	delete m_pPluginManager;
	m_pPluginManager = 0;

	delete m_pLibraryManager;
	m_pLibraryManager = 0;

	delete m_pThreadManager;
	m_pThreadManager = 0;

	delete m_pLogger;
	m_pLogger = 0;

	delete m_pManagerForOtherManagers;
	m_pManagerForOtherManagers = 0;
}

IResourceManager *Core::createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType)
{
	IResourceManager *pResourceManager = 0;
	switch (storageType) {
	case IResourceManager::StorageType_List: { 
		pResourceManager = new ResourceManagerList(resourceManagerName);
		break;
											 }
	case IResourceManager::StorageType_Hash: {
		pResourceManager = new ResourceManagerHash(resourceManagerName);
		break;
											 }
	case IResourceManager::StorageType_Map: {
		pResourceManager = new ResourceManagerMap(resourceManagerName);
		break;
											 }
	default:
		break;
	}

	if (pResourceManager)
		m_pManagerForOtherManagers->addResource(pResourceManager);

	return pResourceManager;
}

void Core::destroyResourceManager(IResourceManager *pResourceManager)
{
	m_pManagerForOtherManagers->destroyResource(pResourceManager);
}

void Core::destroyResourceManager(const std::string& resourceManagerName)
{
	m_pManagerForOtherManagers->destroyResource(resourceManagerName);
}

ILibraryManager *Core::libraryManager() const
{
	return m_pLibraryManager;
}

IPluginManager *Core::pluginManager() const
{
	return m_pPluginManager;
}

IThreadManager *Core::threadManager() const
{
	return m_pThreadManager;
}

void Core::setWindowManager(IWindowManager* const pWindowManager)
{
	m_pWindowManager = pWindowManager;
}

IWindowManager *Core::windowManager() const
{
	return m_pWindowManager;
}

void Core::setImageLoader(IImageLoader* const pImageLoader)
{
	m_pImageLoader = pImageLoader;
}

IImageLoader *Core::imageLoader() const
{
	return m_pImageLoader;
}

void Core::setSceneLoader(ISceneLoader* const pSceneLoader)
{
	m_pSceneLoader = pSceneLoader;
}

ISceneLoader *Core::sceneLoader() const
{
	return m_pSceneLoader;
}

IPhysicsManager *Core::physicsManager() const
{
	return m_pPhysicsManager;
}

IGPURenderer *Core::renderer() const
{
	return m_pGPURenderer;
}

void Core::setRenderer(IGPURenderer *pGPURenderer)
{
	m_pGPURenderer = pGPURenderer;
}

GraphicsEngine::IGraphicsEngine *Core::graphicsEngine() const
{
	return m_pGraphicsEngine;
}

void Core::setGraphicsEngine(GraphicsEngine::IGraphicsEngine *pGraphicsEngine)
{
	m_pGraphicsEngine = pGraphicsEngine;
}

IWindow *Core::mainWindow() const
{
	return m_pMainWindow;
}

ILogger *Core::logger()
{
	return m_pLogger;
}

Core *coreEngine = 0;

bool initCoreEngine()
{
	/*if (!dInitODE2(0)) {
		destroyCoreEngine();
		return false;
	}*/

	if (!coreEngine)
		coreEngine = new Core;
	return true;
}

ICore *getCoreEngine()
{
	return coreEngine;
}

void destroyCoreEngine()
{
	delete coreEngine;
	coreEngine = 0;

	//dCloseODE();
}



} // namespace
