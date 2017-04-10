#include <vector>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <IWindow.h>
#include <renderer/IGPURenderer.h>
#include <math/MathUtils.h>

#include "GraphicsWindow.h"
#include "GraphicsCamera.h"
#include "GraphicsModel.h"
#include "GraphicsScene.h"
#include "GraphicsEngine.h"
#include "GPUMesh.h"
#include "KdTree.h"

//
#include <FreeWorldEngine.h>
#include <utility/File.h>
//

namespace FreeWorldEngine {
	
namespace GraphicsEngine {
	
struct GraphicsWindowUserData {
	GraphicsWindow *pThis;

	GraphicsWindowUserData(GraphicsWindow *pWindow) : pThis(pWindow) {}
};

GraphicsWindow::GraphicsWindow(const std::string& name, IWindow *pTargetWindow) :
	m_name(name),
	m_pTargetWindow(pTargetWindow),
	m_pCamera(0),
	m_lastFpsTime(0),
	m_frameCounter(-1),
	m_frameNumber(-1),
	m_fps(-1.0f),
	m_frustumCulling(false),
	m_renderSpheres(false),
	m_renderNodeBoxes(false)
{
	m_pTargetWindow->registerResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->registerRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->registerUpdateCallBack(GraphicsWindow::updateCallBack);
	m_pTargetWindow->registerCloseCallBack(GraphicsWindow::closeCallBack);
	m_pTargetWindow->registerKeyDownCallBack(GraphicsWindow::keyDownCallBack);

	m_pTargetWindow->setUserData(new GraphicsWindowUserData(this));

	std::string logString;
	IGraphicsMaterial *pColorMaterial = pGraphicsEngine->materialManager()->loadMaterial(Utility::File("color_material_vertex.glsl"), Utility::File("color_material_fragment.glsl"), "ColorMaterial", &logString);
	Renderer::IGPUProgram *pProgram = pColorMaterial->program();
	pColorMaterial->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);
	pColorMaterial->setUniform(pProgram->uniformLocationByName("color"), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (!logString.empty())
		getCoreEngine()->logger()->printMessage(logString, ILogger::MessageType_Error);
}

GraphicsWindow::~GraphicsWindow()
{
	m_pTargetWindow->unregisterKeyDownCallBack(GraphicsWindow::keyDownCallBack);
	m_pTargetWindow->unregisterCloseCallBack(GraphicsWindow::closeCallBack);
	m_pTargetWindow->unregisterResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->unregisterRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->unregisterUpdateCallBack(GraphicsWindow::updateCallBack);

	delete m_pTargetWindow->userData();
	m_pTargetWindow->setUserData(nullptr);
}

std::string GraphicsWindow::name() const
{
	return m_name;
}

void GraphicsWindow::setCamera(IGraphicsCamera *pGraphicsCamera)
{
	m_pCamera = pGraphicsCamera;
	m_pCamera->setAspectRatio((float)m_pTargetWindow->width() / (float)m_pTargetWindow->height());
}

const IGraphicsCamera * GraphicsWindow::camera() const
{
	return m_pCamera;
}

void GraphicsWindow::setScene(IGraphicsScene * pGraphicsScene)
{
	m_pScene = pGraphicsScene;
}

const IGraphicsScene * GraphicsWindow::scene() const
{
	return m_pScene;
}

float GraphicsWindow::fps() const
{
	return m_fps;
}

uint64 GraphicsWindow::frameNumber() const
{
	return m_frameNumber;
}

void GraphicsWindow::switchFrustumCulling()
{
	m_frustumCulling = !m_frustumCulling;
}

void GraphicsWindow::switchRenderSpheres()
{
	m_renderSpheres = !m_renderSpheres;
}

void GraphicsWindow::switchRenderNodeBoxes()
{
	m_renderNodeBoxes = !m_renderNodeBoxes;
}

void GraphicsWindow::renderSolid(RenderDataContainer::const_iterator itBegin, RenderDataContainer::const_iterator itEnd)
{
	while (itBegin != itEnd) {
		itBegin->first->bind(m_pCamera, itBegin->second.modelMatrix);
		const GPUMesh *pGpuMesh = itBegin->second.pGpuMesh;
		pGPURenderer->renderIndexedGeometry(itBegin->first->program(), pGpuMesh->bufferContainer(), pGpuMesh->primitiveFormat(), TYPE_UNSIGNED_INT_32, pGpuMesh->numIndices());
		++itBegin;
	}
}

void GraphicsWindow::renderTransparent(RenderDataContainer::const_iterator itBegin, RenderDataContainer::const_iterator itEnd)
{
	glm::vec3 cameraPos = m_pCamera->position();

	typedef std::pair<float, const RenderDataContainer::value_type*> SortedDataItem;
	std::vector<SortedDataItem> sortedData;
	sortedData.reserve(std::distance(itBegin, itEnd));
	std::transform(itBegin, itEnd, std::back_inserter(sortedData), [&cameraPos](const RenderDataContainer::value_type& val) {
		glm::vec3 v = cameraPos - glm::vec3(val.second.modelMatrix[3]);
		return std::make_pair(glm::dot(v, v), &val);
	});

	std::sort(sortedData.begin(), sortedData.end(), [](const SortedDataItem& val1, const SortedDataItem& val2) {
		return (bool)(val1.first > val2.first);
	});

	for (const auto& it : sortedData) {
		it.second->first->bind(m_pCamera, it.second->second.modelMatrix);
		const GPUMesh *pGpuMesh = it.second->second.pGpuMesh;
		pGPURenderer->renderIndexedGeometry(it.second->first->program(), pGpuMesh->bufferContainer(), pGpuMesh->primitiveFormat(), TYPE_UNSIGNED_INT_32, pGpuMesh->numIndices());
	}
}

void GraphicsWindow::renderHud(RenderDataContainer::const_iterator itBegin, RenderDataContainer::const_iterator itEnd)
{
	while (itBegin != itEnd) {
		++itBegin;
	}
}

void GraphicsWindow::renderData(const IGraphicsMaterial::Tag tag, RenderDataContainer::const_iterator itBegin, RenderDataContainer::const_iterator itEnd)
{
	typedef void (GraphicsWindow::*RenderFunc)(RenderDataContainer::const_iterator itBegin, RenderDataContainer::const_iterator itEnd);
	static RenderFunc funcs[IGraphicsMaterial::Tag_Count] = {
		&GraphicsWindow::renderSolid,
		&GraphicsWindow::renderTransparent,
		&GraphicsWindow::renderHud,
	};
	(this->*funcs[tag])(itBegin, itEnd);
}

void GraphicsWindow::resizeCallBack(int32 width, int32 height, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;

	pThis->m_pCamera->setAspectRatio((float)pThis->m_pTargetWindow->width() / (float)pThis->m_pTargetWindow->height());
	pGPURenderer->setViewport(0, 0, width, height);
}

void GraphicsWindow::renderCallBack(IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	IGraphicsScene *pScene = pThis->m_pScene;
	IGraphicsCamera *pCamera = pThis->m_pCamera;
	const Math::Frustum& frustum = pCamera->frustum();
	IGraphicsMaterial *pColorMaterial = pGraphicsEngine->materialManager()->findMaterial("ColorMaterial");

	std::list<GraphicsSceneNode*> sceneNodes;
	sceneNodes.push_back(static_cast<GraphicsSceneNode*>(pScene->rootNode()));

	uint32 numDips = 0;
	pThis->m_renderData.clear();

	while (!sceneNodes.empty()) {
		GraphicsSceneNode *pSceneNode = sceneNodes.front();
		sceneNodes.pop_front();

		const glm::mat4x4& nodeWorldTransformation = pSceneNode->worldTransformation();

		std::list<KdNode*> treeNodes;
		treeNodes.push_back(pSceneNode->kdTree()->rootNode());
		while (!treeNodes.empty()) {
			KdNode *pTreeNode = treeNodes.front();
			treeNodes.pop_front();

			if (!pTreeNode)
				continue;

			if (pThis->m_frustumCulling && !Math::geomInFrustum(frustum, pTreeNode->worldBoundingBox(nodeWorldTransformation)))
				continue;

			const KdNode::SceneNodesList& treeNodeSceneNodes = pTreeNode->sceneNodes();
			std::copy(treeNodeSceneNodes.cbegin(), treeNodeSceneNodes.cend(), std::back_inserter(sceneNodes));

			treeNodes.push_back(pTreeNode->firstChild());
			treeNodes.push_back(pTreeNode->secondChild());

			if (pThis->m_renderNodeBoxes)
				pThis->m_renderData.insert(std::make_pair(
					static_cast<GraphicsMaterial*>(pColorMaterial), ModelRenderData(pTreeNode->boundingBoxGpuMesh(), glm::mat4x4())));
		}

		GraphicsModel *pModel = static_cast<GraphicsModel*>(pSceneNode->model());
		if (!pModel)
			continue;

		if (pThis->m_frustumCulling) {
			const Math::Sphere& modelBoundSphere = pModel->boundingSphere();
			glm::vec4 newSpherePos = nodeWorldTransformation * glm::vec4(modelBoundSphere.x, modelBoundSphere.y, modelBoundSphere.z, 1.0f);
			Math::Sphere nodeBoundSphere(newSpherePos.x, newSpherePos.y, newSpherePos.z, modelBoundSphere.w);

			if (!Math::geomInFrustum(frustum, nodeBoundSphere))
				continue;
		}

		++numDips;

		pThis->m_renderData.insert(std::make_pair(
			static_cast<GraphicsMaterial*>(pModel->material()), ModelRenderData(pModel->gpuMesh(), nodeWorldTransformation)));
	
		if (pThis->m_renderSpheres)
			pThis->m_renderData.insert(std::make_pair(
				static_cast<GraphicsMaterial*>(pModel->material()), ModelRenderData(pModel->gpuMeshBoundSphere(), nodeWorldTransformation)));
	}

	pGPURenderer->mainFrameBuffer()->clearColorBuffer(0, 0.3f, 0.3f, 0.3f, 1.0f);
	pGPURenderer->mainFrameBuffer()->clearDepthBuffer(1.0f);

	RenderDataContainer::const_iterator beginIterator, endIterator = pThis->m_renderData.cbegin();
	while (endIterator != pThis->m_renderData.cend()) {
		beginIterator = endIterator;
		endIterator = std::upper_bound(beginIterator, pThis->m_renderData.cend(), *beginIterator,
			[](const RenderDataContainer::value_type& val1, const RenderDataContainer::value_type& val2) {
			return val1.first->tag() != val2.first->tag();
		});
		pThis->renderData(beginIterator->first->tag(), beginIterator, endIterator);
	}
	
	getCoreEngine()->logger()->printMessage("Models: " + std::to_string(numDips) + ";        FPS: " + std::to_string(pThis->fps()));
}

void GraphicsWindow::updateCallBack(uint32 time, uint32 dt, IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	static_cast<GraphicsCamera*>(pThis->m_pCamera)->update();

	++(pThis->m_frameNumber);
	if (pThis->m_frameCounter < 0) { // только при первом вызове updateCallBack
		pThis->m_lastFpsTime = time;
		pThis->m_frameCounter = 0;
		pThis->m_fps = -1.0f;
	}
	else {
		++pThis->m_frameCounter;
		if (time - pThis->m_lastFpsTime > 1000) {
			pThis->m_fps = (float)(pThis->m_frameCounter) / (float)(time - pThis->m_lastFpsTime) * 1000.0f;
			pThis->m_frameCounter = 0;
			pThis->m_lastFpsTime = time;
		}
	}
}

void GraphicsWindow::closeCallBack(IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	pGraphicsEngine->destroyWindow(pThis);
}

void GraphicsWindow::keyDownCallBack(IWindow::KeyCode keyCode, IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;

	if (keyCode == IWindow::KeyCode_F)
		pThis->switchFrustumCulling();

	if (keyCode == IWindow::KeyCode_B)
		pThis->switchRenderSpheres();

	if (keyCode == IWindow::KeyCode_N)
		pThis->switchRenderNodeBoxes();
}

} // namespace
} // namespace