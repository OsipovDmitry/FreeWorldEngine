#include <list>
#include <algorithm>

#include <3rdparty/glm/gtc/type_ptr.hpp>

#include <math/MathUtils.h>
#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

#include <core/IResourceManager.h>
#include <graphics_engine/IGraphicsCamera.h>
#include <graphics_engine/IGraphicsMaterialManager.h>

#include "OctreeSceneOptimizer.h"
#include "GraphicsModel.h"
#include "GraphicsLight.h"
#include "GraphicsEngine.h"
#include "GPUMesh.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

struct OctreeNode {
	Math::Aabb worldAabb;
	GPUMesh *pGpuMesh;
	OctreeNode *childOctreeNodes[8];
	std::vector<GraphicsSceneNode*> sceneNodes;
	std::vector<GraphicsLight*> lights;

	OctreeNode(const Math::Aabb& box) :
		worldAabb(box),
		sceneNodes(),
		lights(),
		pGpuMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines))
	{
		for (int32 i = 0; i < 8; ++i) childOctreeNodes[i] = nullptr;

		Mesh aabbMesh;
		Math::MeshWrapper aabbWrapper(&aabbMesh);
		aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
		aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
		aabbWrapper.setVertexStride(3);
		glm::vec3 aabbSize = worldAabb.vMax - worldAabb.vMin;
		glm::vec3 aabbPos = 0.5f * (worldAabb.vMin + worldAabb.vMax);
		Math::MeshPainter(aabbWrapper).paintBox(aabbSize.x, aabbSize.y, aabbSize.z);
		aabbWrapper.translateMesh(glm::value_ptr(aabbPos));
		pGpuMesh->setMesh(&aabbMesh);
	}

	~OctreeNode() {
		pGpuMesh->destroy();
		delete pGpuMesh;
	}

};

struct OctreeOptimizerData {
	OctreeNode *pOctreeNode;
	uint64 lastRenderFrame;
	bool needRecalcLights;
	std::vector<GraphicsLight*> lights;

	OctreeOptimizerData() :
		pOctreeNode(nullptr),
		lastRenderFrame(0),
		needRecalcLights(true),
		lights()
	{}
};

struct OctreeLightOptimizerData {
	std::vector<OctreeNode*> octreeNodes;

	OctreeLightOptimizerData() :
		octreeNodes()
	{}
};

OctreeSceneOptimizer::OctreeSceneOptimizer(GraphicsScene* pScene) :
	AbstractSceneOptimizer(pScene),
	m_pRootOctreeNode(new OctreeNode(Math::buildEmptyBoundingBox())),
	m_needRebuild(false)
{

}

OctreeSceneOptimizer::~OctreeSceneOptimizer()
{
	nodeDetached(m_pScene->rootNodeImpl());

	std::list<GraphicsSceneNode*> nodes{ m_pScene->rootNodeImpl() };
	while (!nodes.empty()) {
		GraphicsSceneNode *pNode = nodes.front();
		nodes.pop_front();

		std::copy(pNode->chilNodesImpl().begin(), pNode->chilNodesImpl().end(), std::back_inserter(nodes));

		nodeDestroyed(pNode);
	}

	std::list<OctreeNode*> octreeNodes { m_pRootOctreeNode };
	while (!octreeNodes.empty()) {
		OctreeNode *pOctreeNode = octreeNodes.front();
		octreeNodes.pop_front();
		if (pOctreeNode->childOctreeNodes[0])
			std::copy(pOctreeNode->childOctreeNodes, pOctreeNode->childOctreeNodes + 8, std::back_inserter(octreeNodes));
		delete pOctreeNode;
	}
}

IGraphicsScene::SceneOptimizerType OctreeSceneOptimizer::type() const
{
	return IGraphicsScene::SceneOptimizerType_Octree;
}

void OctreeSceneOptimizer::updateRenderData(IGraphicsCamera* pCamera, uint64 frameCounter)
{
	if (m_needRebuild)
		rebuildOctree();

	m_renderData.clear();

	const Math::Frustum& frustum = pCamera->frustum();

	std::list<OctreeNode*> nodes { m_pRootOctreeNode };
	while (!nodes.empty()) {
		OctreeNode *pOctreeNode = nodes.front();
		nodes.pop_front();

		if (!Math::geomInFrustum(frustum, pOctreeNode->worldAabb))
			continue;

		if (pOctreeNode->childOctreeNodes[0])
			std::copy(pOctreeNode->childOctreeNodes, pOctreeNode->childOctreeNodes+8, std::back_inserter(nodes));

		std::for_each(pOctreeNode->sceneNodes.cbegin(), pOctreeNode->sceneNodes.cend(), [&frustum, &frameCounter, this](GraphicsSceneNode *pNode) {
			OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
			Math::Sphere sph = pNode->worldBoundingSphere();
			if (Math::geomInFrustum(frustum, sph) && pOptimizerData->lastRenderFrame != frameCounter) {
				pOptimizerData->lastRenderFrame = frameCounter;
				GraphicsModel *pModel = pNode->modelImpl();
				if (pModel) {
					ModelRenderData data;
					data.modelMatrix = pNode->worldTransformation();
					data.pGpuMesh = pModel->gpuMesh();
					data.lights = std::move(lightsForSceneNode(pNode));
					this->m_renderData.insert(std::make_pair(pModel->materialImpl(), data));
				}
			}
		});

//		ModelRenderData data;
//		data.modelMatrix = glm::mat4x4();
//		data.pGpuMesh = pOctreeNode->pGpuMesh;
//		data.lights.clear();
//		m_renderData.insert(std::make_pair(static_cast<GraphicsMaterial*>(pGraphicsEngine->materialManager()->findMaterial("ColorMaterial")), data));
	}
}

void OctreeSceneOptimizer::nodeCreated(GraphicsSceneNode* pNode)
{
	OctreeOptimizerData *pData = new OctreeOptimizerData;
	pNode->setOptimizerData(pData);
}

void OctreeSceneOptimizer::nodeDestroyed(GraphicsSceneNode* pNode)
{
	OctreeOptimizerData *pData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
	pNode->setOptimizerData(nullptr);
	delete pData;
}

void OctreeSceneOptimizer::nodeAttached(GraphicsSceneNode *pNode)
{
	GraphicsSceneNode *pParentNode = pNode;
	while (pParentNode->parentSceneNodeImpl())
		pParentNode = pParentNode->parentSceneNodeImpl();

	if (pParentNode != m_pScene->rootNodeImpl())
		return;

	std::list<GraphicsSceneNode*> nodes{ pNode };
	while (!nodes.empty()) {
		GraphicsSceneNode *p = nodes.front();
		nodes.pop_front();

		std::copy(p->chilNodesImpl().begin(), p->chilNodesImpl().end(), std::back_inserter(nodes));

		pushSceneNodeToOctreeNode(p, m_pRootOctreeNode);
		reinsertSceneNode(p);
	}
}

void OctreeSceneOptimizer::nodeDetached(GraphicsSceneNode *pNode)
{
	std::list<GraphicsSceneNode*> nodes{ pNode };
	while (!nodes.empty()) {
		GraphicsSceneNode *p = nodes.front();
		nodes.pop_front();

		std::copy(p->chilNodesImpl().begin(), p->chilNodesImpl().end(), std::back_inserter(nodes));

		popSceneNodeFromOctreeNode(p);
	}
}

void OctreeSceneOptimizer::nodeTransformationChanged(GraphicsSceneNode *pNode)
{
	reinsertSceneNode(pNode);
}

void OctreeSceneOptimizer::nodeModelChanged(GraphicsSceneNode *pNode)
{
	reinsertSceneNode(pNode);
}

void OctreeSceneOptimizer::lightCreated(GraphicsLight* pLight)
{
	OctreeLightOptimizerData *pLightOptimizerData = new OctreeLightOptimizerData;
	pLight->setOptimizerData(pLightOptimizerData);

	m_pRootOctreeNode->lights.push_back(pLight);
	pLightOptimizerData->octreeNodes.push_back(m_pRootOctreeNode);
}

void OctreeSceneOptimizer::lightDestroyed(GraphicsLight* pLight)
{
	OctreeLightOptimizerData *pLightOptimizerData = static_cast<OctreeLightOptimizerData*>(pLight->optimizerData());
	pLight->setOptimizerData(nullptr);
	delete pLightOptimizerData;
}

void OctreeSceneOptimizer::lightTransformationChanged(GraphicsLight* pLight)
{
	detachLight(pLight);
	attachLight(pLight);
}

OctreeNode *OctreeSceneOptimizer::rebuildOctree()
{
	std::vector<GraphicsSceneNode*> sceneNodes;
	std::list<OctreeNode*> octreeNodes { m_pRootOctreeNode };

	while (!octreeNodes.empty()) {
		OctreeNode *pOctreeNode = octreeNodes.front();
		octreeNodes.pop_front();

		if (pOctreeNode->childOctreeNodes[0])
			std::copy(pOctreeNode->childOctreeNodes, pOctreeNode->childOctreeNodes + 8, std::back_inserter(octreeNodes));

		std::copy(pOctreeNode->sceneNodes.cbegin(), pOctreeNode->sceneNodes.cend(), std::back_inserter(sceneNodes));

		delete pOctreeNode;
	}

	Math::Aabb box = Math::buildEmptyBoundingBox();
	std::for_each(sceneNodes.cbegin(), sceneNodes.cend(), [&box](GraphicsSceneNode *p) {
		box = Math::mergeBoundingBoxes(box, Math::sphereToAabb(p->worldBoundingSphere()));
	});
	const glm::vec3 boxCenter = 0.5f * (box.vMin + box.vMax);
	const glm::vec3 boxHalfSize = 0.5f * (box.vMax - box.vMin);
	const glm::vec3 maxSize(2.0f * glm::compMax(boxHalfSize)); // расширяем бокс в 2 раза по каждой оси, чтобы объекты не выезжали за пределы октри при мелких трансформациях
	box.vMin = boxCenter - maxSize;
	box.vMax = boxCenter + maxSize;

	m_pRootOctreeNode = new OctreeNode(box);
	std::for_each(sceneNodes.cbegin(), sceneNodes.cend(), [this](GraphicsSceneNode *p) {
		OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(p->optimizerData());
		pOptimizerData->pOctreeNode = this->m_pRootOctreeNode;
		pOptimizerData->needRecalcLights = true;
	});
	m_pRootOctreeNode->sceneNodes = std::move(sceneNodes);
	cutOctreeNode(m_pRootOctreeNode);

	std::for_each(m_pScene->lightsImpl()->begin(), m_pScene->lightsImpl()->end(), [this](IResource *pResource) {
		GraphicsLight *pLight = static_cast<GraphicsLight*>(pResource);
		OctreeLightOptimizerData *pLightOptimizerData = static_cast<OctreeLightOptimizerData*>(pLight->optimizerData());
		pLightOptimizerData->octreeNodes.clear();
		this->attachLight(pLight);
	});

	m_needRebuild = false;
}

void OctreeSceneOptimizer::reinsertSceneNode(GraphicsSceneNode* pNode)
{
	OctreeOptimizerData *pData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
	OctreeNode *pOldOctreeNode = pData->pOctreeNode;

	if (!pOldOctreeNode)
		return;

	OctreeNode *pNewOctreeNode = findBestOctreeNode(pNode);

	if (!pNewOctreeNode) {
		m_needRebuild = true;
		return;
	}

	if (pNewOctreeNode == pOldOctreeNode)
		return;

	popSceneNodeFromOctreeNode(pNode);
	pushSceneNodeToOctreeNode(pNode, pNewOctreeNode);
	pData->needRecalcLights = true;

	cutOctreeNode(pNewOctreeNode);
}

OctreeNode *OctreeSceneOptimizer::findBestOctreeNode(GraphicsSceneNode* pNode)
{
	Math::Aabb nodeAabb = Math::sphereToAabb(pNode->worldBoundingSphere());

	if (!Math::containsBoundingBox(m_pRootOctreeNode->worldAabb, nodeAabb))
		return nullptr;

	OctreeNode *pBestOctreeNode = m_pRootOctreeNode;
	while (pBestOctreeNode->childOctreeNodes[0]) {
		int32 childIndex = calcChildIndexFromBox(pBestOctreeNode, nodeAabb);
		if (childIndex < 0)
			break;
		pBestOctreeNode = pBestOctreeNode->childOctreeNodes[childIndex];
	}

	return pBestOctreeNode;
}

int32 OctreeSceneOptimizer::calcChildIndexFromBox(OctreeNode *pOctreeNode, const Math::Aabb& box)
{
	glm::vec3 octreeNodePos = 0.5f * (pOctreeNode->worldAabb.vMin + pOctreeNode->worldAabb.vMax);
	int32 childIndex = 0;
	bool find = true;

	for (int32 i = 0; i < 3; ++i) {
		if (box.vMin[i] >= octreeNodePos[i])
			childIndex |= (4 >> i);
		else if (box.vMax[i] <= octreeNodePos[i])
			;
		else {
			find = false;
			break;
		}
	}

	return (find) ? childIndex : -1;
}

void OctreeSceneOptimizer::cutOctreeNode(OctreeNode* pOctreeNode)
{
	if (pOctreeNode->childOctreeNodes[0])
		return;

	if (pOctreeNode->sceneNodes.size() < s_maxSceneNodesPerOctreeNode)
		return;

	const glm::vec3 centerBox = 0.5f * (pOctreeNode->worldAabb.vMin + pOctreeNode->worldAabb.vMax);
	const glm::vec3 halfSizeBox = 0.5f * (pOctreeNode->worldAabb.vMax - pOctreeNode->worldAabb.vMin);

	for (int32 i = 0; i < 8; ++i) {
		glm::vec3 offs(((i&4)>>2)*halfSizeBox.x, ((i&2)>>1)*halfSizeBox.y, (i&1)*halfSizeBox.z);
		pOctreeNode->childOctreeNodes[i] = new OctreeNode(Math::Aabb(pOctreeNode->worldAabb.vMin + offs, centerBox + offs));
	}

	std::vector<GraphicsSceneNode*> thisSceneNodes;
	std::for_each(pOctreeNode->sceneNodes.cbegin(), pOctreeNode->sceneNodes.cend(), [&thisSceneNodes, pOctreeNode](GraphicsSceneNode *p) {
		int32 childIndex = calcChildIndexFromBox(pOctreeNode, Math::sphereToAabb(p->worldBoundingSphere()));
		if (childIndex < 0)
			thisSceneNodes.push_back(p);
		else
			pushSceneNodeToOctreeNode(p, pOctreeNode->childOctreeNodes[childIndex]);
	});

	pOctreeNode->sceneNodes = std::move(thisSceneNodes);
	std::for_each(pOctreeNode->childOctreeNodes, pOctreeNode->childOctreeNodes+8, cutOctreeNode);
}

void OctreeSceneOptimizer::pushSceneNodeToOctreeNode(GraphicsSceneNode* pNode, OctreeNode* pOctreeNode)
{
	OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
	pOptimizerData->pOctreeNode = pOctreeNode;
	pOctreeNode->sceneNodes.push_back(pNode);
}

void OctreeSceneOptimizer::popSceneNodeFromOctreeNode(GraphicsSceneNode* pNode)
{
	OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
	OctreeNode *pOctreeNode = pOptimizerData->pOctreeNode;
	if (pOctreeNode) {
		auto it = std::find(pOctreeNode->sceneNodes.begin(), pOctreeNode->sceneNodes.end(), pNode);
		if (it != pOctreeNode->sceneNodes.end())
			pOctreeNode->sceneNodes.erase(it);
	}
	pOptimizerData->pOctreeNode = nullptr;
}

std::vector<GraphicsLight*> OctreeSceneOptimizer::lightsForSceneNode(GraphicsSceneNode* pNode)
{	
	OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());

	if (pOptimizerData->needRecalcLights) {
		const std::vector<GraphicsLight*>& octreeNodeLights = pOptimizerData->pOctreeNode->lights;

		std::multimap<float, GraphicsLight*> lights;
		std::for_each(octreeNodeLights.cbegin(), octreeNodeLights.cend(), [&lights, pNode](GraphicsLight *pLight) {
			lights.insert(std::make_pair(pLight->powerOfLighting(glm::vec3(pNode->worldTransformation()[3])), pLight));
		});

		pOptimizerData->lights.clear();
		pOptimizerData->lights.reserve(s_maxLightsPerSceneNode);

		int32 lightindex = 0;
		for (auto it = lights.rbegin(); (it != lights.rend()) && (lightindex < s_maxLightsPerSceneNode); ++it, ++lightindex)
			pOptimizerData->lights.push_back(it->second);

		pOptimizerData->needRecalcLights = false;
	}

	return pOptimizerData->lights;
}

void OctreeSceneOptimizer::detachLight(GraphicsLight* pLight)
{
	OctreeLightOptimizerData *pLightOptimizerData = static_cast<OctreeLightOptimizerData*>(pLight->optimizerData());

	std::for_each(pLightOptimizerData->octreeNodes.begin(), pLightOptimizerData->octreeNodes.end(), [pLight](OctreeNode *pOctreeNode) {
		auto it = std::find(pOctreeNode->lights.begin(), pOctreeNode->lights.end(), pLight);
		if (it != pOctreeNode->lights.end())
			pOctreeNode->lights.erase(it);

		std::for_each(pOctreeNode->sceneNodes.begin(), pOctreeNode->sceneNodes.end(), [](GraphicsSceneNode *pNode) {
			OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
			pOptimizerData->needRecalcLights = true;
		});
	});

	pLightOptimizerData->octreeNodes.clear();
}

void OctreeSceneOptimizer::attachLight(GraphicsLight* pLight)
{
	OctreeLightOptimizerData *pLightOptimizerData = static_cast<OctreeLightOptimizerData*>(pLight->optimizerData());

	std::list<OctreeNode*> nodes{m_pRootOctreeNode};

	while (!nodes.empty()) {
		OctreeNode *pOctreeNode = nodes.front();
		nodes.pop_front();

		if (pLight->isBoundingBoxLighted(pOctreeNode->worldAabb) == false)
			continue;

		pLightOptimizerData->octreeNodes.push_back(pOctreeNode);
		pOctreeNode->lights.push_back(pLight);

		std::for_each(pOctreeNode->sceneNodes.begin(), pOctreeNode->sceneNodes.end(), [](GraphicsSceneNode *pNode) {
			OctreeOptimizerData *pOptimizerData = static_cast<OctreeOptimizerData*>(pNode->optimizerData());
			pOptimizerData->needRecalcLights = true;
		});

		if (pOctreeNode->childOctreeNodes[0])
			std::copy(pOctreeNode->childOctreeNodes, pOctreeNode->childOctreeNodes+8, std::back_inserter(nodes));
	}
}

} // namespace
} // namespace
