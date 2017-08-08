#ifndef __ABSTRACTSCENEOPTIMIZER__
#define __ABSTRACTSCENEOPTIMIZER__

#include "GraphicsScene.h"
#include "GraphicsMaterial.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GPUMesh;
class GraphicsLight;

struct ModelRenderData {
	glm::mat4x4 modelMatrix;
	GPUMesh *pGpuMesh;
	std::vector<GraphicsLight*> lights;
};

class AbstractSceneOptimizer {
public:
	AbstractSceneOptimizer(GraphicsScene *pScene);
	virtual ~AbstractSceneOptimizer() {}

	virtual IGraphicsScene::SceneOptimizerType type() const = 0;

	virtual void updateRenderData(IGraphicsCamera *pCamera, uint64 frameCounter) {}

	virtual void nodeCreated(GraphicsSceneNode *pNode) {}
	virtual void nodeDestroyed(GraphicsSceneNode *pNode) {}
	virtual void nodeAttached(GraphicsSceneNode *pNode) {}
	virtual void nodeDetached(GraphicsSceneNode *pNode) {}
	virtual void nodeTransformationChanged(GraphicsSceneNode *pNode) {}
	virtual void nodeModelChanged(GraphicsSceneNode *pNode) {}

	virtual void lightCreated(GraphicsLight *pLight) {}
	virtual void lightDestroyed(GraphicsLight *pLight) {}
	virtual void lightTransformationChanged(GraphicsLight *pLight) {}

	typedef std::multimap<GraphicsMaterial*, ModelRenderData, GraphicsMaterial::Comparator> RenderDataContainer;
	const RenderDataContainer& renderDataContainer() const;

	static AbstractSceneOptimizer *createSceneOptimizer(IGraphicsScene::SceneOptimizerType optimizerType, GraphicsScene *pScene);

protected:
	GraphicsScene *m_pScene;
	RenderDataContainer m_renderData;

};


} // namespace
} // namespace


#endif // __ABSTRACT_SCENE_OPTIMIZER__
