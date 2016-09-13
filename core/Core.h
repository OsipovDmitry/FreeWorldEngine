#ifndef __CORE__
#define __CORE__

#include "ICore.h"

#include "Log.h"
#define LOG(text) (coreEngine->log().add(std::string(text).c_str()))

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

	void setWindowManager(IWindowManager* const pWindowManager);
	IWindowManager *windowManager() const;

	void setImageManager(IImageManager* const pImageManager);
	IImageManager *imageManager() const;
	
	IPhysicsManager *physicsManager() const;
	ISceneModelManager *sceneModelManager() const;

	IGLRenderer *renderer() const;
	void setRenderer(IGLRenderer *pGLRenderer);

	IWindow *mainWindow() const;
	ICamera *mainCamera() const;

	const ILog& log() const;

private:
	ILog *m_pLog;

	IResourceManager *m_pManagerForOtherManagers;

	ILibraryManager *m_pLibraryManager;
	IPluginManager *m_pPluginManager;
	IWindowManager *m_pWindowManager;
	IGLRenderer *m_pGLRenderer;
	IImageManager *m_pImageManager;

	IPhysicsManager *m_pPhysicsManager;
	ISceneModelManager *m_pSceneModelManager;

	IWindow *m_pMainWindow;

	ICamera *m_pMainCamera;

}; // class Core

extern Core *coreEngine;

} // namespace

#endif // __CORE__