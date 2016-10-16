#ifndef __ICORE__
#define __ICORE__

#include "IResourceManager.h"

#include "Types.h"

namespace FreeWorldEngine {

class IPluginManager;
class IWindowManager;
class IPhysicsManager;
class IImageLoader;
class ISceneModelManager;
class IWindow;
class ILogger;
class ICamera;
class IRenderSystem;
class IGLRenderer;
class ILibraryManager;
class IThreadManager;

class ICore {
public:
	virtual ~ICore() = 0 {}

	virtual void initialize() = 0;
	virtual void deinitialize() = 0;

	virtual IResourceManager *createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType = IResourceManager::StorageType_Hash) = 0;
	virtual void destroyResourceManager(IResourceManager *pResourceManager) = 0;
	virtual void destroyResourceManager(const std::string& resourceManagerName) = 0;

	virtual ILibraryManager *libraryManager() const = 0;

	virtual IPluginManager *pluginManager() const = 0;

	virtual void setThreadManager(IThreadManager* const pThreadManager) = 0;
	virtual IThreadManager *threadManager() const = 0;

	virtual void setWindowManager(IWindowManager* const pWindowManager) = 0;
	virtual IWindowManager *windowManager() const = 0;

	virtual void setImageLoader(IImageLoader* const pImageLoader) = 0;
	virtual IImageLoader *imageLoader() const = 0;
	
	virtual IPhysicsManager *physicsManager() const = 0;
	virtual ISceneModelManager *sceneModelManager() const = 0;

	virtual IGLRenderer *renderer() const = 0;
	virtual void setRenderer(IGLRenderer *pGLRenderer) = 0;

	virtual IWindow *mainWindow() const = 0;
	virtual ICamera *mainCamera() const = 0;

	virtual ILogger *logger() = 0;

}; // class ICore

} // namespace

#endif // __ICORE__