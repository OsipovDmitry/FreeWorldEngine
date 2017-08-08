#ifndef __NONESCENEOPTIMIZER__
#define __NONESCENEOPTIMIZER__

#include "AbstractSceneOptimizer.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class NoneSceneOptimizer : public AbstractSceneOptimizer {
public:
	NoneSceneOptimizer(GraphicsScene *pScene);
	~NoneSceneOptimizer();

	IGraphicsScene::SceneOptimizerType type() const;

	void updateRenderData(IGraphicsCamera *pCamera, uint64 frameCounter);

};


} // namespace
} // namespace


#endif // __NONE_SCENE_OPTIMIZER__
