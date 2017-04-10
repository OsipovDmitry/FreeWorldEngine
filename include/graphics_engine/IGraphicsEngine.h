#ifndef __IGRAPHICSENGINE__
#define __IGRAPHICSENGINE__

#include <string>

namespace FreeWorldEngine {

class IWindow;

namespace Renderer {
	class IGPUProgram;
} // namespace

namespace GraphicsEngine {

class IGraphicsMaterialManager;
class IGraphicsTextureManager;
class IGraphicsSceneManager;

class IGraphicsCamera;
class IGraphicsModel;
class IGraphicsWindow;

class IGraphicsEngine {
public:
	virtual ~IGraphicsEngine() {}

	virtual IGraphicsCamera *findCamera(const std::string& name) const = 0;
	virtual IGraphicsCamera *createCamera(const std::string& name = "@utoname") = 0;
	virtual void destroyCamera(const std::string& name) = 0;
	virtual void destroyCamera(IGraphicsCamera *pCamera) = 0;

	virtual IGraphicsModel *findModel(const std::string& name) const = 0;
	virtual IGraphicsModel *createModel(const std::string& name = "@utoname") = 0;
	virtual void destroyModel(const std::string& name) = 0;
	virtual void destroyModel(IGraphicsModel *pModel) = 0;

	virtual IGraphicsMaterialManager *materialManager() const = 0;
	virtual IGraphicsTextureManager *textureManager() const = 0;
	virtual IGraphicsSceneManager *sceneManager() const = 0;

	virtual IGraphicsWindow *findWindow(const std::string& name) const = 0;
	virtual IGraphicsWindow *createWindow(IWindow *pTargetWindow, const std::string& name = "@utoname") = 0;
	virtual void destroyWindow(const std::string& name) = 0;
	virtual void destroyWindow(IGraphicsWindow *pWindow) = 0;

}; // class IGraphicsEngine

} // namespace

} // namespace

#endif // __IGRAPHICSENGINE__