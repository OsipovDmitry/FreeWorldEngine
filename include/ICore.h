#ifndef __ICORE__
#define __ICORE__

#include "IResourceManager.h"

#include "Types.h"

namespace FreeWorldEngine {

class IPluginManager;
class IWindowManager;
class IPhysicsManager;
class IWindow;
class ILogger;
class ILibraryManager;
class IThreadManager;

template <class DataType>
class IContentLoader;

typedef IContentLoader<Raster> IImageLoader;
typedef IContentLoader<SceneData> ISceneLoader;
typedef IContentLoader<SoundData> ISoundLoader;

template <class DataType>
class IContent;

typedef IContent<Raster> IImage;
typedef IContent<SceneData> IScene;
typedef IContent<SoundData> ISound;

namespace Renderer {
	class IGPURenderer;
}

namespace GraphicsEngine {
	class IGraphicsEngine;
}

class ICore {
public:
	virtual ~ICore() = 0 {}

	virtual void initialize() = 0;
	virtual void deinitialize() = 0;

	virtual IResourceManager *createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType = IResourceManager::StorageType_Hash) = 0;
	virtual IResourceManager *findResourceManager(const std::string& resourceManagerName) const = 0;
	virtual void destroyResourceManager(IResourceManager *pResourceManager) = 0;
	virtual void destroyResourceManager(const std::string& resourceManagerName) = 0;

	virtual ILibraryManager *libraryManager() const = 0;
	virtual IPluginManager *pluginManager() const = 0;
	virtual IThreadManager *threadManager() const = 0;

	virtual void setWindowManager(IWindowManager* const pWindowManager) = 0;
	virtual IWindowManager *windowManager() const = 0;

	virtual IImageLoader *imageLoader() const = 0;
	virtual ISceneLoader *sceneLoader() const = 0;
	virtual ISoundLoader *soundLoader() const = 0;
	
	virtual IPhysicsManager *physicsManager() const = 0;

	virtual Renderer::IGPURenderer *renderer() const = 0;
	virtual void setRenderer(Renderer::IGPURenderer *pGPURenderer) = 0;

	virtual GraphicsEngine::IGraphicsEngine *graphicsEngine() const = 0;
	virtual void setGraphicsEngine(GraphicsEngine::IGraphicsEngine *pGraphicsEngine) = 0;

	virtual IWindow *mainWindow() const = 0;

	virtual ILogger *logger() = 0;

}; // class ICore

} // namespace

#endif // __ICORE__
