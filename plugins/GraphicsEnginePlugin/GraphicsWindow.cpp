#include <graphics_engine/IGraphicsCamera.h>
#include <IWindow.h>

#include "GraphicsWindow.h"

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

void GraphicsWindow::setGraphicsCamera(IGraphicsCamera *pGraphicsCamera)
{
	m_pCamera = pGraphicsCamera;
	m_pCamera->setAspectRatio((float)m_pTargetWindow->width() / (float)m_pTargetWindow->height());
}

const IGraphicsCamera * GraphicsWindow::graphicsCamera() const
{
	return m_pCamera;
}

void GraphicsWindow::setGraphicsScene(IGraphicsScene * pGraphicsScene)
{
	m_pScene = pGraphicsScene;
}

const IGraphicsScene * GraphicsWindow::graphicsScene() const
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
}

void GraphicsWindow::updateCallBack(uint32 dt, uint32 time, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
}

} // namespace
} // namespace