#ifndef __GRAPHICSENGINE__
#define __GRAPHICSENGINE__

#include <graphics_engine/IGraphicsEngine.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace GraphicsEngine {

class GraphicsEngine : public IGraphicsEngine {
public:
	GraphicsEngine();
	~GraphicsEngine();

	IGraphicsModel *findModel(const std::string& name) const;
	IGraphicsModel *createModel(const std::string& name);
	void destroyModel(const std::string& name);

	IGraphicsMaterial *findMaterial(const std::string& name) const;
	IGraphicsMaterial *createMaterial(const std::string& name);
	void destroyMaterial(const std::string& name);

	IGraphicsScene *findScene(const std::string& name) const;
	IGraphicsScene *createScene(const std::string& name);
	void destroyScene(const std::string& name);

private:
	IResourceManager *m_pModelManager, *m_pMaterialManager, *m_pSceneManager;

}; // class GraphicsEngine

extern GraphicsEngine *pGraphicsEngine;

} // namespace

} // namespace

#endif // __GRAPHICSENGINE__