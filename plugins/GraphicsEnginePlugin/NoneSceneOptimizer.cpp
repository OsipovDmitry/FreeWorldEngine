#include <list>

#include "NoneSceneOptimizer.h"
#include "GraphicsModel.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

NoneSceneOptimizer::NoneSceneOptimizer(GraphicsScene* pScene) :
	AbstractSceneOptimizer(pScene)
{
}

NoneSceneOptimizer::~NoneSceneOptimizer()
{
}

IGraphicsScene::SceneOptimizerType NoneSceneOptimizer::type() const
{
	return IGraphicsScene::SceneOptimizerType_None;
}

void NoneSceneOptimizer::updateRenderData(IGraphicsCamera* pCamera, uint64 frameCounter)
{
	m_renderData.clear();

	std::list<GraphicsSceneNode*> nodes;
	nodes.push_back(static_cast<GraphicsSceneNode*>(m_pScene->rootNode()));
	while (!nodes.empty()) {
		GraphicsSceneNode *pNode = nodes.front();
		nodes.pop_front();
		std::copy(pNode->chilNodesImpl().cbegin(), pNode->chilNodesImpl().cend(), std::back_inserter(nodes));

		GraphicsModel *pModel = pNode->modelImpl();
		if (!pModel)
			continue;

		ModelRenderData data;
		data.modelMatrix = pNode->worldTransformation();
		data.pGpuMesh = pModel->gpuMesh();
		data.lights.clear();
		m_renderData.insert(std::make_pair(pModel->materialImpl(), data));
	}

}


} // namespace
} // namespace
