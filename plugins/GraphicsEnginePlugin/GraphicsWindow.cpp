#include <IWindow.h>

#include "GraphicsWindow.h"
#include "GraphicsCamera.h"
#include "GraphicsModel.h"
#include "GraphicsMaterial.h"
#include "GraphicsScene.h"

namespace FreeWorldEngine {
	
namespace GraphicsEngine {
	
struct GraphicsWindowUserData {
	GraphicsWindow *pThis;

	GraphicsWindowUserData(GraphicsWindow *pWindow) : pThis(pWindow) {}
};

GraphicsWindow::GraphicsWindow(const std::string& name, IWindow *pTargetWindow) :
	m_name(name),
	m_pTargetWindow(pTargetWindow),
	m_pCamera(0)
{
	m_pTargetWindow->registerResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->registerRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->registerUpdateCallBack(GraphicsWindow::updateCallBack);

	m_pTargetWindow->setUserData(new GraphicsWindowUserData(this));
}

GraphicsWindow::~GraphicsWindow()
{
	delete m_pTargetWindow->userData();
	m_pTargetWindow->setUserData(nullptr);

	m_pTargetWindow->unregisterResizeCallBack(GraphicsWindow::resizeCallBack);
	m_pTargetWindow->unregisterRenderCallBack(GraphicsWindow::renderCallBack);
	m_pTargetWindow->unregisterUpdateCallBack(GraphicsWindow::updateCallBack);
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

void GraphicsWindow::resizeCallBack(int32 width, int32 height, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;

	pThis->m_pCamera->setAspectRatio((float)pThis->m_pTargetWindow->width() / (float)pThis->m_pTargetWindow->height());
}

void GraphicsWindow::renderCallBack(IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	IGraphicsScene *pScene = pThis->m_pScene;
	IGraphicsCamera *pCamera = pThis->m_pCamera;

	std::multimap<IGraphicsMaterial*, GraphicsModel::RenderData*> renderData;
	std::list<IGraphicsScene::Node*> sceneNodes;
	sceneNodes.push_back(pScene->rootNode());

	while (!sceneNodes.empty()) {
		IGraphicsScene::Node *pNode = sceneNodes.front();
		sceneNodes.pop_front();
		std::copy(pNode->beginChildNodes(), pNode->endChildNodes(), std::back_inserter(sceneNodes));
		if (GraphicsModel *pModel = static_cast<GraphicsModel*>(pNode->model()))
			renderData.insert(std::make_pair(pModel->material(), pModel->renderData()));
	}


}

void GraphicsWindow::updateCallBack(uint32 dt, uint32 time, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
}

} // namespace
} // namespace