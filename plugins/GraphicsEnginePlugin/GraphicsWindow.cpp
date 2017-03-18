#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <IWindow.h>
#include <renderer/IGPURenderer.h>

#include "GraphicsWindow.h"
#include "GraphicsCamera.h"
#include "GraphicsModel.h"
#include "GraphicsMaterial.h"
#include "GraphicsScene.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {
	
namespace GraphicsEngine {
	
struct GraphicsWindowUserData {
	GraphicsWindow *pThis;

	GraphicsWindowUserData(GraphicsWindow *pWindow) : pThis(pWindow) {}
};

struct ModelRenderData {
	IGraphicsSceneNode *pSceneNode;
	Renderer::IGPUBufferContainer *pBufferContainer;
	uint32 numIndices;
	PrimitiveFormat primitiveFormat;
	
	ModelRenderData(IGraphicsSceneNode *pNode, Renderer::IGPUBufferContainer *pVAO, uint32 numInds, PrimitiveFormat primFormat) :
		pSceneNode(pNode), pBufferContainer(pVAO), numIndices(numInds), primitiveFormat(primFormat) {}
};

GraphicsWindow::GraphicsWindow(const std::string& name, IWindow *pTargetWindow) :
	m_name(name),
	m_pTargetWindow(pTargetWindow),
	m_pCamera(0),
	m_lastFpsTime(0),
	m_frameCounter(-1),
	m_frameNumber(-1),
	m_fps(-1.0f)
{
	m_pTargetWindow->registerResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->registerRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->registerUpdateCallBack(GraphicsWindow::updateCallBack);
	m_pTargetWindow->registerCloseCallBack(GraphicsWindow::closeCallBack);

	m_pTargetWindow->setUserData(new GraphicsWindowUserData(this));
}

GraphicsWindow::~GraphicsWindow()
{
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

	std::multimap<GraphicsMaterial*, ModelRenderData, GraphicsMaterial::Comparator> renderData;
	std::list<IGraphicsSceneNode*> sceneNodes;
	sceneNodes.push_back(pScene->rootNode());

	while (!sceneNodes.empty()) {
		IGraphicsSceneNode *pNode = sceneNodes.front();
		sceneNodes.pop_front();
		for (uint32 i = 0; i < pNode->numChildren(); ++i)
			sceneNodes.push_back(pNode->childAt(i));
		if (GraphicsModel *pModel = static_cast<GraphicsModel*>(pNode->model()))
			renderData.insert(std::make_pair(
				static_cast<GraphicsMaterial*>(pModel->material()),
				ModelRenderData(pNode, pModel->bufferContainer(), pModel->numIndices(), pModel->primitiveFormat())));
	}

	pGPURenderer->mainFrameBuffer()->clearDepthBuffer();
	pGPURenderer->mainFrameBuffer()->clearColorBuffer(0, 0.0f, 0.0f, 0.5f);

	for (const auto& it : renderData) {
		it.first->bind(pCamera, it.second.pSceneNode->worldTransformation());
		pGPURenderer->renderIndexedGeometry(it.first->program(), it.second.pBufferContainer, it.second.primitiveFormat, TYPE_UNSIGNED_INT_32, it.second.numIndices);
	}
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

} // namespace
} // namespace