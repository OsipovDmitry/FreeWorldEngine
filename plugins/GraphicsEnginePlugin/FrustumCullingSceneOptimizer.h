#ifndef __FRUSTUMCULLINGSCENEOPTIMIZER__
#define __FRUSTUMCULLINGSCENEOPTIMIZER__

#include "AbstractSceneOptimizer.h"

namespace FreeWorldEngine {

namespace Math {
	struct Aabb;
}

namespace GraphicsEngine {

class GPUMesh;

class FrustumCullingSceneOptimizer : public AbstractSceneOptimizer {
public:
	FrustumCullingSceneOptimizer(GraphicsScene *pScene);
	~FrustumCullingSceneOptimizer();

	IGraphicsScene::SceneOptimizerType type() const;

	void updateRenderData(IGraphicsCamera *pCamera, uint64 frameCounter);

	void nodeCreated(GraphicsSceneNode *pNode);
	void nodeDestroyed(GraphicsSceneNode *pNode);
	void nodeAttached(GraphicsSceneNode *pNode);
	void nodeDetached(GraphicsSceneNode *pNode);
	void nodeTransformationChanged(GraphicsSceneNode *pNode);
	void nodeModelChanged(GraphicsSceneNode *pNode);


	static GPUMesh *gpuMeshWorldAabb(GraphicsSceneNode *pNode);
	static Math::Aabb& worldAabb(GraphicsSceneNode *pNode);
	static void updateRecursiveUp(GraphicsSceneNode *pNode);
	static void updateRecursiveDown(GraphicsSceneNode *pNode);
};


} // namespace
} // namespace


#endif // __FRUSTUM_CULLING_SCENE_OPTIMIZER__
