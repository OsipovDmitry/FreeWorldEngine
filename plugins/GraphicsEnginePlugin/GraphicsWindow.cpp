#include <vector>

#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include <core/IWindow.h>
#include <renderer/IGPURenderer.h>

#include "GraphicsWindow.h"
#include "GraphicsScene.h"
#include "GraphicsEngine.h"
#include "GPUMesh.h"
#include "AbstractSceneOptimizer.h"

//
#include <FreeWorldEngine.h>
#include <utility/File.h>
//

#include <math/MathTypes.h>
#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

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
	m_renderKdNodesBoxes(false)
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
		ICore::instance()->logger()->printMessage(logString, ILogger::MessageType_Error);
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

void GraphicsWindow::setScene(IGraphicsScene *pGraphicsScene)
{
	m_pScene = static_cast<GraphicsScene*>(pGraphicsScene);
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
	m_renderKdNodesBoxes = !m_renderKdNodesBoxes;
}

template <class Iter>
void GraphicsWindow::renderSolid(Iter itBegin, Iter itEnd)
{
	while (itBegin != itEnd) {
		itBegin->first->bind(m_pCamera, itBegin->second);
		const GPUMesh *pGpuMesh = itBegin->second.pGpuMesh;
		pGPURenderer->renderIndexedGeometry(itBegin->first->program(), pGpuMesh->bufferContainer(), pGpuMesh->primitiveFormat(), TYPE_UNSIGNED_INT_32, pGpuMesh->numIndices());
		++itBegin;
	}
}

template <class Iter>
void GraphicsWindow::renderTransparent(Iter itBegin, Iter itEnd)
{
	glm::vec3 cameraPos = m_pCamera->position();

	typedef std::pair<float, const typename Iter::value_type*> SortedDataItem;
	std::vector<SortedDataItem> sortedData;
	sortedData.reserve(std::distance(itBegin, itEnd));
	std::transform(itBegin, itEnd, std::back_inserter(sortedData), [&cameraPos](const typename Iter::value_type& val) {
		glm::vec3 v = cameraPos - glm::vec3(val.second.modelMatrix[3]);
		return std::make_pair(glm::dot(v, v), &val);
	});

	std::sort(sortedData.begin(), sortedData.end(), [](const SortedDataItem& val1, const SortedDataItem& val2) {
		return (bool)(val1.first > val2.first);
	});

	for (const auto& it : sortedData) {
		it.second->first->bind(m_pCamera, it.second->second);
		const GPUMesh *pGpuMesh = it.second->second.pGpuMesh;
		pGPURenderer->renderIndexedGeometry(it.second->first->program(), pGpuMesh->bufferContainer(), pGpuMesh->primitiveFormat(), TYPE_UNSIGNED_INT_32, pGpuMesh->numIndices());
	}
}

template <class Iter>
void GraphicsWindow::renderHud(Iter itBegin, Iter itEnd)
{
	while (itBegin != itEnd) {
		++itBegin;
	}
}

template <class Iter>
void GraphicsWindow::renderData(const IGraphicsMaterial::Tag tag, Iter itBegin, Iter itEnd)
{
	typedef void (GraphicsWindow::*RenderFunc)(Iter, Iter);
	static RenderFunc funcs[IGraphicsMaterial::Tag_Count] = {
		&GraphicsWindow::renderSolid,
		&GraphicsWindow::renderTransparent,
		&GraphicsWindow::renderHud,
	};
	(this->*funcs[tag])(itBegin, itEnd);
}

void GraphicsWindow::resize(int32 width, int32 height)
{
	if (m_pCamera)
		m_pCamera->setAspectRatio((float)m_pTargetWindow->width() / (float)m_pTargetWindow->height());
	pGPURenderer->setViewport(0, 0, width, height);
}

void GraphicsWindow::render()
{
	pGPURenderer->mainFrameBuffer()->clearColorBuffer(0, 0.5f, 0.5f, 0.5f, 1.0f);
	pGPURenderer->mainFrameBuffer()->clearDepthBuffer(1.0f);

	if (!m_pScene || !m_pCamera)
		return;

	const AbstractSceneOptimizer::RenderDataContainer& renderDataCont = m_pScene->optimizer()->renderDataContainer();

	AbstractSceneOptimizer::RenderDataContainer::const_iterator beginIterator, endIterator = renderDataCont.cbegin();
	while (endIterator != renderDataCont.cend()) {
		beginIterator = endIterator;
		endIterator = std::upper_bound(beginIterator, renderDataCont.cend(), *beginIterator,
			[](const AbstractSceneOptimizer::RenderDataContainer::value_type& val1, const AbstractSceneOptimizer::RenderDataContainer::value_type& val2) {
			return val1.first->tag() != val2.first->tag();
		});
		renderData(beginIterator->first->tag(), beginIterator, endIterator);
	}

	ICore::instance()->logger()->printMessage("FPS: " + std::to_string(fps()));
}

void GraphicsWindow::update(uint32 time, uint32 dt)
{
	++m_frameNumber;

	m_pScene->optimizer()->updateRenderData(m_pCamera, m_frameNumber);

	if (m_frameCounter < 0) {
		m_lastFpsTime = time;
		m_frameCounter = 0;
		m_fps = -1.0f;
	}
	else {
		++m_frameCounter;
		if (time - m_lastFpsTime > 1000) {
			m_fps = (float)(m_frameCounter) / (float)(time - m_lastFpsTime) * 1000.0f;
			m_frameCounter = 0;
			m_lastFpsTime = time;
		}
	}
}

void GraphicsWindow::resizeCallBack(int32 width, int32 height, IWindow * pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	pThis->resize(width, height);
}

void GraphicsWindow::renderCallBack(IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	pThis->render();
}

void GraphicsWindow::updateCallBack(uint32 time, uint32 dt, IWindow *pWindow)
{
	GraphicsWindow *pThis = static_cast<GraphicsWindowUserData*>(pWindow->userData())->pThis;
	pThis->update(time, dt);
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
