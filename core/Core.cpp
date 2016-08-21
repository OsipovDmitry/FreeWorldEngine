#include "FreeWorldEngine.h"
#include "IWindowManager.h"

#include "Core.h"
#include "PluginManager.h"
#include "ResourceManager.h"
#include "LibraryManager.h"

namespace FreeWorldEngine {

Core::Core() :
	m_pLog(0),
	m_pManagerForOtherManagers(0),
	m_pLibraryManager(0),
	m_pPluginManager(0),
	m_pWindowManager(0),
	m_pGLRenderer(0)
{
}

Core::~Core()
{
}

void Core::initialize()
{
	m_pLog = new Log("log.txt");

	m_pManagerForOtherManagers = new ResourceManagerHash("ResourceManagerForOtherManagers");

	m_pLibraryManager = new LibraryManager();

	m_pPluginManager = new PluginManager();
	
	/*m_pPhysicsManager = new PhysicsManager;
	m_pImageManager = new ImageManager(createResourceManager("ResourceManagerForImages"));
	m_pSceneModelManager = new SceneModelManager(createResourceManager("ResourceManagerForSceneModels"));
	
	m_pMainCamera = new Camera;

	*m_pLog << "Free World Engine created.";*/

	m_pPluginManager->loadPlugins("C:/Users/Dmitry/Documents/plugins.xml");

	if (m_pWindowManager)
		m_pMainWindow = m_pWindowManager->createWindow("Free World Engine Demo", 1024, 768, IWindow::IWindow_Show | IWindow::IWindow_Resizable);
}

void Core::deinitialize()
{
	m_pPluginManager->unloadPlugins();

	/*delete m_pMainCamera;

	delete m_pSceneModelManager;
	delete m_pImageManager;
	delete m_pPhysicsManager;*/

	delete m_pGLRenderer;
	m_pGLRenderer = 0;

	delete m_pWindowManager;
	m_pWindowManager = 0;

	delete m_pPluginManager;
	m_pPluginManager = 0;

	delete m_pLibraryManager;
	m_pLibraryManager = 0;

	delete m_pManagerForOtherManagers;
	m_pManagerForOtherManagers = 0;

	delete m_pLog;
	m_pLog = 0;
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
	m_pManagerForOtherManagers->deleteResource(pResourceManager);
}

void Core::destroyResourceManager(const std::string& resourceManagerName)
{
	m_pManagerForOtherManagers->deleteResource(resourceManagerName);
}

ILibraryManager *Core::libraryManager() const
{
	return m_pLibraryManager;
}

IPluginManager *Core::pluginManager() const
{
	return m_pPluginManager;
}

void Core::setWindowManager(IWindowManager* const pWindowManager)
{
	m_pWindowManager = pWindowManager;
}

IWindowManager *Core::windowManager() const
{
	return m_pWindowManager;
}

IPhysicsManager *Core::physicsManager() const
{
	return m_pPhysicsManager;
}

IImageManager *Core::imageManager() const
{
	return m_pImageManager;
}

ISceneModelManager *Core::sceneModelManager() const
{
	return m_pSceneModelManager;
}

IGLRenderer *Core::renderer() const
{
	return m_pGLRenderer;
}

void Core::setRenderer(IGLRenderer *pGLRenderer)
{
	m_pGLRenderer = pGLRenderer;
}

IWindow *Core::mainWindow() const
{
	return m_pMainWindow;
}

ICamera *Core::mainCamera() const
{
	return m_pMainCamera;
}

const ILog& Core::log() const
{
	return *m_pLog;
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
