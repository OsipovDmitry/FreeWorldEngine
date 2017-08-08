#ifndef __GRAPHICSENGINE__
#define __GRAPHICSENGINE__

#include <graphics_engine/IGraphicsEngine.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Renderer {
	class IGPURenderer;
} // namespace

namespace Utility {
	class AutoNameGenerator;
} // namespace

namespace GraphicsEngine {

class ShaderManager;
class ProgramManager;

class GraphicsEngine : public IGraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	IGraphicsCamera *findCamera(const std::string& name) const;
	IGraphicsCamera *createCamera(const std::string& name = "@utoname");
	void destroyCamera(const std::string& name);
	void destroyCamera(IGraphicsCamera *pCamera);

	IGraphicsModel *findModel(const std::string& name) const;
	IGraphicsModel *createModel(const std::string& name = "@utoname");
	void destroyModel(const std::string& name);
	void destroyModel(IGraphicsModel *pModel);

	IGraphicsMaterialManager *materialManager() const;
	IGraphicsTextureManager *textureManager() const;
	IGraphicsSceneManager *sceneManager() const;

	IGraphicsWindow *findWindow(const std::string& name) const;
	IGraphicsWindow *createWindow(IWindow *pTargetWindow, const std::string& name = "@utoname");
	void destroyWindow(const std::string& name);
	void destroyWindow(IGraphicsWindow *pWindow);

	ShaderManager *shaderManager() const;
	ProgramManager *programManager() const;

private:
	ShaderManager *m_pShaderManager;
	ProgramManager *m_pProgramManager;
	IGraphicsMaterialManager *m_pMaterialManager;
	IGraphicsTextureManager *m_pTextureManager;
	IGraphicsSceneManager *m_pSceneManager;
	IResourceManager *m_pCameraManager, *m_pModelManager, *m_pWindowManager;
	Utility::AutoNameGenerator *m_pCameraNameGenerator, *m_pModelNameGenerator, *m_pWindowNameGenerator;

}; // class GraphicsEngine

extern GraphicsEngine *pGraphicsEngine;
extern Renderer::IGPURenderer *pGPURenderer;

} // namespace

} // namespace

#endif // __GRAPHICSENGINE__
