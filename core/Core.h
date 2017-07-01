#ifndef __CORE__
#define __CORE__

#include <memory>
#include <core/ICore.h>

#include "Logger.h"
#define LOG_INFO(text) (Core::s_instance->logger()->printMessage(std::string(text), ILogger::MessageType_Info))
#define LOG_WARNING(text) (Core::s_instance->logger()->printMessage(std::string(text), ILogger::MessageType_Warning))
#define LOG_ERROR(text) (Core::s_instance->logger()->printMessage(std::string(text), ILogger::MessageType_Error))
#define LOG_CRITICAL(text) (Core::s_instance->logger()->printMessage(std::string(text), ILogger::MessageType_Critical))
#define LOG(text) LOG_INFO(text)

namespace FreeWorldEngine {

class Core : public ICore {
public:
	Core();
	~Core();

	int argc() const;
	char** argv() const;
	
	void initialize(int argc, char **argv);
	void deinitialize();

	IResourceManager *createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType = IResourceManager::StorageType_Hash);
	IResourceManager *findResourceManager(const std::string& resourceManagerName) const;
	void destroyResourceManager(IResourceManager *pResourceManager);
	void destroyResourceManager(const std::string& resourceManagerName);

	ILibraryManager *libraryManager() const;
	IPluginManager *pluginManager() const;
	IThreadManager *threadManager() const;

	void setWindowManager(IWindowManager* const pWindowManager);
	IWindowManager *windowManager() const;

	IImageLoader *imageLoader() const;
	ISceneLoader *sceneLoader() const;
	ISoundLoader *soundLoader() const;
	
	IPhysicsManager *physicsManager() const;

	Renderer::IGPURenderer *renderer() const;
	void setRenderer(Renderer::IGPURenderer *pGPURenderer);

	GraphicsEngine::IGraphicsEngine *graphicsEngine() const;
	void setGraphicsEngine(GraphicsEngine::IGraphicsEngine *pGraphicsEngine);

	IWindow *mainWindow() const;

	ILogger *logger();

	static std::unique_ptr<Core> s_instance;

private:
	int m_argc;
	char **m_argv;
	
	ILogger *m_pLogger;

	IResourceManager *m_pManagerForOtherManagers;

	ILibraryManager *m_pLibraryManager;
	IPluginManager *m_pPluginManager;
	IThreadManager *m_pThreadManager;
	IWindowManager *m_pWindowManager;
	Renderer::IGPURenderer *m_pGPURenderer;
	GraphicsEngine::IGraphicsEngine *m_pGraphicsEngine;
	IImageLoader *m_pImageLoader;
	ISceneLoader *m_pSceneLoader;
	ISoundLoader *m_pSoundLoader;

	IPhysicsManager *m_pPhysicsManager;

	IWindow *m_pMainWindow;

}; // class Core

} // namespace

#endif // __CORE__
