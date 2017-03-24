#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <IWindow.h>
#include <renderer/IGPURenderer.h>
#include <math/MathUtils.h>

#include "GraphicsWindow.h"
#include "GraphicsCamera.h"
#include "GraphicsModel.h"
#include "GraphicsMaterial.h"
#include "GraphicsScene.h"
#include "GraphicsEngine.h"
#include "GPUMesh.h"



//
#include <FreeWorldEngine.h>
//

namespace FreeWorldEngine {
	
namespace GraphicsEngine {
	
struct GraphicsWindowUserData {
	GraphicsWindow *pThis;

	GraphicsWindowUserData(GraphicsWindow *pWindow) : pThis(pWindow) {}
};

struct ModelRenderData {
	IGraphicsSceneNode *pSceneNode;
	GPUMesh *pGPUMesh;
	
	ModelRenderData(IGraphicsSceneNode *pNode, GPUMesh *pMesh) :
		pSceneNode(pNode), pGPUMesh(pMesh) {}
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
	m_renderSpheres(false)
{
	m_pTargetWindow->registerResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->registerRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->registerUpdateCallBack(GraphicsWindow::updateCallBack);
	m_pTargetWindow->registerCloseCallBack(GraphicsWindow::closeCallBack);
	m_pTargetWindow->registerKeyDownCallBack(GraphicsWindow::keyDownCallBack);

	m_pTargetWindow->setUserData(new GraphicsWindowUserData(this));
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

void GraphicsWindow::resizeCallBack(int32 width, int32 height, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;

	pThis->m_pCamera->setAspectRatio((float)pThis->m_pTargetWindow->width() / (float)pThis->m_pTargetWindow->height());
	pGPURenderer->setViewport(0, 0, width, height);
}

void GraphicsWindow::renderCallBack(IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	IGraphicsScene *pScene = pThis->m_pScene;
	IGraphicsCamera *pCamera = pThis->m_pCamera;
	const Math::Frustum& frustum = pCamera->frustum();

	std::multimap<GraphicsMaterial*, ModelRenderData, GraphicsMaterial::Comparator> renderData;
	std::list<IGraphicsSceneNode*> sceneNodes;
	sceneNodes.push_back(pScene->rootNode());

	uint32 numDips = 0;

	while (!sceneNodes.empty()) {
		IGraphicsSceneNode *pNode = sceneNodes.front();
		sceneNodes.pop_front();
		for (uint32 i = 0; i < pNode->numChildren(); ++i)
			sceneNodes.push_back(pNode->childAt(i));

		GraphicsModel *pModel = static_cast<GraphicsModel*>(pNode->model());
		if (!pModel)
			continue;

		if (pThis->m_frustumCulling) {
			const Math::Sphere& boundSphere = pModel->boundingSphere();
			glm::vec4 newSpherePos = pNode->worldTransformation() * glm::vec4(boundSphere.x, boundSphere.y, boundSphere.z, 1.0f);
			Math::Sphere nodeBoundSphere(newSpherePos.x, newSpherePos.y, newSpherePos.z, boundSphere.w);

			if (!Math::sphereInFrustum(frustum, nodeBoundSphere))
				continue;
		}
		
		++numDips;

		renderData.insert(std::make_pair(
			static_cast<GraphicsMaterial*>(pModel->material()),
			ModelRenderData(pNode, pModel->gpuMesh())));

		if (pThis->m_renderSpheres) {
			renderData.insert(std::make_pair(
				static_cast<GraphicsMaterial*>(pModel->material()),
				ModelRenderData(pNode, pModel->gpuMeshBoundSphere())));
		}
	}

	pGPURenderer->mainFrameBuffer()->clearDepthBuffer();
	pGPURenderer->mainFrameBuffer()->clearColorBuffer(0, 0.0f, 0.0f, 0.5f);

	for (const auto& it : renderData) {
		it.first->bind(pCamera, it.second.pSceneNode->worldTransformation());
		pGPURenderer->renderIndexedGeometry(it.first->program(), it.second.pGPUMesh->bufferContainer(), it.second.pGPUMesh->primitiveFormat(), TYPE_UNSIGNED_INT_32, it.second.pGPUMesh->numIndices());
	}

	getCoreEngine()->logger()->printMessage("Models: " + std::to_string(numDips) + ";        FPS: " + std::to_string(pThis->fps()));

}

void GraphicsWindow::updateCallBack(uint32 time, uint32 dt, IWindow * pWindow)
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
}

} // namespace
} // namespace