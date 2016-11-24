#ifndef __CORE__
#define __CORE__

#include "ICore.h"

#include "Logger.h"
#define LOG_INFO(text) (coreEngine->logger()->printMessage(std::string(text), ILog::MessageType_Info))
#define LOG_WARNING(text) (coreEngine->logger()->printMessage(std::string(text), ILog::MessageType_Warning))
#define LOG_ERROR(text) (coreEngine->logger()->printMessage(std::string(text), ILog::MessageType_Error))
#define LOG_CRITICAL(text) (coreEngine->logger()->printMessage(std::string(text), ILog::MessageType_Critical))
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

	void setThreadManager(IThreadManager* const pThreadManager);
	IThreadManager *threadManager() const;

	void setWindowManager(IWindowManager* const pWindowManager);
	IWindowManager *windowManager() const;

	void setImageLoader(IImageLoader* const pImageLoader);
	IImageLoader *imageLoader() const;
	
	IPhysicsManager *physicsManager() const;
	ISceneModelManager *sceneModelManager() const;

	IGPURenderer *renderer() const;
	void setRenderer(IGPURenderer *pGPURenderer);

	IWindow *mainWindow() const;
	ICamera *mainCamera() const;

	ILogger *logger();

private:
	ILogger *m_pLogger;

	IResourceManager *m_pManagerForOtherManagers;

	ILibraryManager *m_pLibraryManager;
	IPluginManager *m_pPluginManager;
	IThreadManager *m_pThreadManager;
	IWindowManager *m_pWindowManager;
	IGPURenderer *m_pGPURenderer;
	IImageLoader *m_pImageLoader;

	IPhysicsManager *m_pPhysicsManager;
	ISceneModelManager *m_pSceneModelManager;

	IWindow *m_pMainWindow;

	ICamera *m_pMainCamera;

}; // class Core

extern Core *coreEngine;

} // namespace

#endif // __CORE__