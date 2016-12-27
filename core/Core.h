#ifndef __CORE__
#define __CORE__

#include "ICore.h"

#include "Logger.h"
#define LOG_INFO(text) (coreEngine->logger()->printMessage(std::string(text), ILogger::MessageType_Info))
#define LOG_WARNING(text) (coreEngine->logger()->printMessage(std::string(text), ILogger::MessageType_Warning))
#define LOG_ERROR(text) (coreEngine->logger()->printMessage(std::string(text), ILogger::MessageType_Error))
#define LOG_CRITICAL(text) (coreEngine->logger()->printMessage(std::string(text), ILogger::MessageType_Critical))
#define LOG(text) LOG_INFO(text)

namespace FreeWorldEngine {

class Core : public ICore {
public:
	Core();
	~Core();

	void initialize();
	void deinitialize();

	IResourceManager *createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType = IResourceManager::StorageType_Hash);
	void destroyResourceManager(IResourceManager *pResourceManager);
	void destroyResourceManager(const std::string& resourceManagerName);

	ILibraryManager *libraryManager() const;
	IPluginManager *pluginManager() const;
	IThreadManager *threadManager() const;

	void setWindowManager(IWindowManager* const pWindowManager);
	IWindowManager *windowManager() const;

	void setImageLoader(IImageLoader* const pImageLoader);
	IImageLoader *imageLoader() const;

	void setSceneLoader(ISceneLoader* const pSceneLoader);
	ISceneLoader *sceneLoader() const;
	
	IPhysicsManager *physicsManager() const;

	IGPURenderer *renderer() const;
	void setRenderer(IGPURenderer *pGPURenderer);

	GraphicsEngine::IGraphicsEngine *graphicsEngine() const;
	void setGraphicsEngine(GraphicsEngine::IGraphicsEngine *pGraphicsEngine);

	IWindow *mainWindow() const;

	ILogger *logger();

private:
	ILogger *m_pLogger;

	IResourceManager *m_pManagerForOtherManagers;

	ILibraryManager *m_pLibraryManager;
	IPluginManager *m_pPluginManager;
	IThreadManager *m_pThreadManager;
	IWindowManager *m_pWindowManager;
	IGPURenderer *m_pGPURenderer;
	GraphicsEngine::IGraphicsEngine *m_pGraphicsEngine;
	IImageLoader *m_pImageLoader;
	ISceneLoader *m_pSceneLoader;

	IPhysicsManager *m_pPhysicsManager;

	IWindow *m_pMainWindow;

}; // class Core

extern Core *coreEngine;

} // namespace

#endif // __CORE__