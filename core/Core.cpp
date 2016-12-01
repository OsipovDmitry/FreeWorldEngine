#include <ctime>

#include "FreeWorldEngine.h"

#include "Core.h"
#include "PluginManager.h"
#include "ResourceManager.h"
#include "LibraryManager.h"
#include "TextFileLog.h"

namespace FreeWorldEngine {

Core::Core() :
	m_pLogger(0),
	m_pManagerForOtherManagers(0),
	m_pLibraryManager(0),
	m_pPluginManager(0),
	m_pThreadManager(0),
	m_pImageLoader(0),
	m_pWindowManager(0),
	m_pGPURenderer(0)
{
}

Core::~Core()
{
}

void Core::initialize()
{
	m_pManagerForOtherManagers = new ResourceManagerHash("ResourceManagerForOtherManagers");

	m_pLogger = new Logger;
	m_pLogger->addLog(new TextFileLog("TextLog.txt", false));

	m_pLibraryManager = new LibraryManager();

	m_pPluginManager = new PluginManager();
	
	/*m_pPhysicsManager = new PhysicsManager;
	m_pImageManager = new ImageManager(createResourceManager("ResourceManagerForImages"));
	m_pSceneModelManager = new SceneModelManager(createResourceManager("ResourceManagerForSceneModels"));
	
	m_pMainCamera = new Camera;

	*m_pLogger << "Free World Engine created";*/

	m_pPluginManager->loadPlugins("plugins.xml");

	if (m_pWindowManager)
		m_pMainWindow = m_pWindowManager->createWindow("Free World Engine Demo", 1024, 768, IWindow::Flags_Show | IWindow::Flags_Resizable);
}

void Core::deinitialize()
{
	m_pPluginManager->unloadPlugins();

	/*delete m_pMainCamera;

	delete m_pSceneModelManager;
	delete m_pImageManager;
	delete m_pPhysicsManager;*/

	delete m_pGPURenderer;
	m_pGPURenderer = 0;

	delete m_pWindowManager;
	m_pWindowManager = 0;

	delete m_pImageLoader;
	m_pImageLoader = 0;

	delete m_pPluginManager;
	m_pPluginManager = 0;

	delete m_pLibraryManager;
	m_pLibraryManager = 0;

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

void Core::setThreadManager(IThreadManager* const pThreadManager)
{
	m_pThreadManager = pThreadManager;
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

IPhysicsManager *Core::physicsManager() const
{
	return m_pPhysicsManager;
}

ISceneModelManager *Core::sceneModelManager() const
{
	return m_pSceneModelManager;
}

IGPURenderer *Core::renderer() const
{
	return m_pGPURenderer;
}

void Core::setRenderer(IGPURenderer *pGPURenderer)
{
	m_pGPURenderer = pGPURenderer;
}

IWindow *Core::mainWindow() const
{
	return m_pMainWindow;
}

ICamera *Core::mainCamera() const
{
	return m_pMainCamera;
}

ILogger *Core::logger()
{
	return m_pLogger;
}

Core *coreEngine = 0;

bool initCoreEngine()
{
	/*if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		destroyCoreEngine();
		return false;
	}

	if (!dInitODE2(0)) {
		destroyCoreEngine();
		return false;
	}*/

	// “ут создаем временное окно с OpenGL и контекст дл€ него, чтобы можно было инициализировать GLRenderer. ѕотом его сразу уничтожаем.
	/*SDL_Window *pSDLWindow = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_OPENGL);
	if (!pSDLWindow) {
		destroyCoreEngine();
		return false;
	}
	void *pSDLGLContext = SDL_GL_CreateContext(pSDLWindow);
	if (!pSDLGLContext) {
		SDL_DestroyWindow(pSDLWindow);
		destroyCoreEngine();
		return false;
	}
	bool GLRendererIsOk = true;//initGLRenderer(); // »нициализируем модуль с рендерером
	SDL_GL_DeleteContext(pSDLGLContext);
	SDL_DestroyWindow(pSDLWindow);
	if (!GLRendererIsOk) {
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
	//SDL_Quit();
}



} // namespace
