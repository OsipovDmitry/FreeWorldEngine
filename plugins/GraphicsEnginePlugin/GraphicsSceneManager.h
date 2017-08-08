#ifndef __GRAPHICSSCENEMANAGER__
#define __GRAPHICSSCENEMANAGER__

#include <graphics_engine/IGraphicsSceneManager.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
} // namespace

namespace GraphicsEngine {

class GraphicsSceneManager : public IGraphicsSceneManager {
public:
	GraphicsSceneManager();
	~GraphicsSceneManager();

	IGraphicsScene *findScene(const std::string& name) const;
	IGraphicsScene *createScene(const std::string& name = "@utoname", IGraphicsScene::SceneOptimizerType optimizerType = IGraphicsScene::SceneOptimizerType_None);

	void destroyScene(const std::string& name);
	void destroyScene(IGraphicsScene *pScene);

private:
	IResourceManager *m_pSceneManager;
	Utility::AutoNameGenerator *m_pSceneNameGenerator;

}; // class GraphicsSceneManager

} // namespace
} // namespace

#endif // __GRAPHICSSCENEMANAGER__
