#ifndef __IGRAPHICSSCENEMANAGER__
#define __IGRAPHICSSCENEMANAGER__

#include <string>

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsSceneManager {
public:
	virtual ~IGraphicsSceneManager() {}
	
	virtual IGraphicsScene *findScene(const std::string& name) const = 0;
	virtual IGraphicsScene *createScene(const std::string& name = "@utoname", IGraphicsScene::SceneOptimizerType optimizerType = IGraphicsScene::SceneOptimizerType_None) = 0;

	virtual void destroyScene(const std::string& name) = 0;
	virtual void destroyScene(IGraphicsScene *pScene) = 0;

}; // class IGraphicsSceneManager

} // namespace

} // namespace

#endif // __IGRAPHICSSCENEMANAGER__
