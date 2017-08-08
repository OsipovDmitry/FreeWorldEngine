#include <list>
#include <algorithm>

#include <3rdparty/glm/gtc/type_ptr.hpp>

#include <math/MathUtils.h>
#include <math/MeshPainter.h>
#include <math/MeshWrapper.h>

#include <graphics_engine/IGraphicsCamera.h>
#include <graphics_engine/IGraphicsMaterialManager.h>

#include "FrustumCullingSceneOptimizer.h"
#include "GraphicsModel.h"
#include "GraphicsEngine.h"
#include "GPUMesh.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

struct FrustumCullingOptimizerData {
	Math::Aabb worldAabb;
	GPUMesh *pGpuMesh;
	bool needUpdateWorldAabb;

	FrustumCullingOptimizerData() :
		worldAabb(Math::buildEmptyBoundingBox()),
		pGpuMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines)),
		needUpdateWorldAabb(false)
	{}

	~FrustumCullingOptimizerData() {
		pGpuMesh->destroy();
		delete pGpuMesh;
	}
};

FrustumCullingSceneOptimizer::FrustumCullingSceneOptimizer(GraphicsScene* pScene) :
	AbstractSceneOptimizer(pScene)
{
}

FrustumCullingSceneOptimizer::~FrustumCullingSceneOptimizer()
{
	std::list<GraphicsSceneNode*> nodes{ m_pScene->rootNodeImpl() };
	while (!nodes.empty()) {
		GraphicsSceneNode *pNode = nodes.front();
		nodes.pop_front();
		std::copy(pNode->chilNodesImpl().begin(), pNode->chilNodesImpl().end(), std::back_inserter(nodes));
		nodeDestroyed(pNode);
	}
}

IGraphicsScene::SceneOptimizerType FrustumCullingSceneOptimizer::type() const
{
	return IGraphicsScene::SceneOptimizerType_FrustumCulling;
}

void FrustumCullingSceneOptimizer::updateRenderData(IGraphicsCamera* pCamera, uint64 frameCounter)
{
	m_renderData.clear();

	const Math::Frustum& frustum = pCamera->frustum();

	std::list<GraphicsSceneNode*> nodes;
	nodes.push_back(static_cast<GraphicsSceneNode*>(m_pScene->rootNode()));
	while (!nodes.empty()) {
		GraphicsSceneNode *pNode = nodes.front();
		nodes.pop_front();

		if (!Math::geomInFrustum(frustum, worldAabb(pNode)))
			continue;

		std::copy(pNode->chilNodesImpl().cbegin(), pNode->chilNodesImpl().cend(), std::back_inserter(nodes));

		ModelRenderData data;
		data.modelMatrix = glm::mat4x4();
		data.pGpuMesh = gpuMeshWorldAabb(pNode);
		data.lights.clear();
		m_renderData.insert(std::make_pair(static_cast<GraphicsMaterial*>(pGraphicsEngine->materialManager()->findMaterial("ColorMaterial")), data));

		Math::Sphere sph = pNode->worldBoundingSphere();
		if (!Math::geomInFrustum(frustum, sph))
			continue;

		GraphicsModel *pModel = pNode->modelImpl();
		if (!pModel)
			continue;

		data.modelMatrix = pNode->worldTransformation();
		data.pGpuMesh = pModel->gpuMesh();
		data.lights.clear();
		m_renderData.insert(std::make_pair(pModel->materialImpl(), data));
	}
}

void FrustumCullingSceneOptimizer::nodeCreated(GraphicsSceneNode* pNode)
{
	FrustumCullingOptimizerData *pData = new FrustumCullingOptimizerData;
	pNode->setOptimizerData(pData);
}

void FrustumCullingSceneOptimizer::nodeDestroyed(GraphicsSceneNode* pNode)
{
	FrustumCullingOptimizerData *pData = static_cast<FrustumCullingOptimizerData*>(pNode->optimizerData());
	pNode->setOptimizerData(nullptr);
	delete pData;
}

void FrustumCullingSceneOptimizer::nodeAttached(GraphicsSceneNode *pNode)
{
	updateRecursiveDown(pNode);
	updateRecursiveUp(pNode);
}

void FrustumCullingSceneOptimizer::nodeDetached(GraphicsSceneNode *pNode)
{
	updateRecursiveDown(pNode);
	updateRecursiveUp(pNode);
}

void FrustumCullingSceneOptimizer::nodeTransformationChanged(GraphicsSceneNode *pNode)
{
	updateRecursiveDown(pNode);
	updateRecursiveUp(pNode);
}

void FrustumCullingSceneOptimizer::nodeModelChanged(GraphicsSceneNode *pNode)
{
	updateRecursiveUp(pNode);
}

GPUMesh *FrustumCullingSceneOptimizer::gpuMeshWorldAabb(GraphicsSceneNode* pNode)
{
	FrustumCullingOptimizerData *pData = static_cast<FrustumCullingOptimizerData*>(pNode->optimizerData());

	Math::Aabb box = worldAabb(pNode);

	Mesh aabbMesh;
	Math::MeshWrapper aabbWrapper(&aabbMesh);
	aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	aabbWrapper.setVertexStride(3);
	glm::vec3 aabbSize = box.vMax - box.vMin;
	glm::vec3 aabbPos = 0.5f * (box.vMin + box.vMax);
	Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
	aabbWrapper.translateMesh(glm::value_ptr(aabbPos));
	pData->pGpuMesh->setMesh(&aabbMesh);

	return pData->pGpuMesh;
}

Math::Aabb& FrustumCullingSceneOptimizer::worldAabb(GraphicsSceneNode *pNode)
{
	FrustumCullingOptimizerData *pData = static_cast<FrustumCullingOptimizerData*>(pNode->optimizerData());

	if (pData->needUpdateWorldAabb) {
		pData->worldAabb = Math::sphereToAabb(pNode->worldBoundingSphere());
		for (GraphicsSceneNode::ChildrenList::const_iterator it = pNode->chilNodesImpl().cbegin(); it != pNode->chilNodesImpl().cend(); ++it)
			pData->worldAabb = Math::mergeBoundingBoxes(pData->worldAabb, worldAabb(*it));
	}

	return pData->worldAabb;
}

void FrustumCullingSceneOptimizer::updateRecursiveUp(GraphicsSceneNode* pNode)
{
	if (!pNode)
		return;

	static_cast<FrustumCullingOptimizerData*>(pNode->optimizerData())->needUpdateWorldAabb = true;

	updateRecursiveUp(pNode->parentSceneNodeImpl());
}

void FrustumCullingSceneOptimizer::updateRecursiveDown(GraphicsSceneNode* pNode)
{
	static_cast<FrustumCullingOptimizerData*>(pNode->optimizerData())->needUpdateWorldAabb = true;
	std::for_each(pNode->chilNodesImpl().begin(), pNode->chilNodesImpl().end(), updateRecursiveDown);
}


} // namespace
} // namespace
