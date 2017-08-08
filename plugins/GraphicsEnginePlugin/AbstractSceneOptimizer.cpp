#include "AbstractSceneOptimizer.h"
#include "NoneSceneOptimizer.h"
#include "FrustumCullingSceneOptimizer.h"
#include "OctreeSceneOptimizer.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

AbstractSceneOptimizer::AbstractSceneOptimizer(GraphicsScene* pScene) :
	m_pScene(pScene)
{
}

const AbstractSceneOptimizer::RenderDataContainer&AbstractSceneOptimizer::renderDataContainer() const
{
	return m_renderData;
}

AbstractSceneOptimizer *AbstractSceneOptimizer::createSceneOptimizer(IGraphicsScene::SceneOptimizerType optimizerType, GraphicsScene *pScene)
{
	switch (optimizerType) {
	case IGraphicsScene::SceneOptimizerType_None: return new NoneSceneOptimizer(pScene);
	case IGraphicsScene::SceneOptimizerType_FrustumCulling : return new FrustumCullingSceneOptimizer(pScene);
	case IGraphicsScene::SceneOptimizerType_Octree : return new OctreeSceneOptimizer(pScene);
	default: return nullptr;
	}
}




} // namespace
} // namespace


