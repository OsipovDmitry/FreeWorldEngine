#include "FreeWorldEngine.h"

#include "GLRendererPlugin.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

IGLRenderer *GLRendererPlugin::pGLRenderer = 0;

GLRendererPlugin::GLRendererPlugin()
{
}

GLRendererPlugin::~GLRendererPlugin()
{
}

std::string GLRendererPlugin::name() const
{
	return std::string("GLRendererPlugin");
}

std::string GLRendererPlugin::info() const
{
	return std::string("OpenGL Renderer Plugin");
}

bool GLRendererPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	IWindowManager *pWindowManager = pCore->windowManager();
	if (!pWindowManager) {
		//
		return false;
	}

	IWindow *pWindow = pWindowManager->createWindow("", 0, 0, 0);
	if (!pWindow) {
		//
		return false;
	}
	bool GLRendererIsOk = glewInit() == GLEW_OK;
	pCore->windowManager()->destroyWindow(pWindow);
	if (!GLRendererIsOk) {
		//
		return false;
	}

	if (!pGLRenderer)
		pGLRenderer = new GLRenderer();

	pCore->setRenderer(pGLRenderer);
	return true;
}

void GLRendererPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->renderer() == pGLRenderer)
		pCore->setRenderer(0);

	delete pGLRenderer;
	pGLRenderer = 0;
}

} // namespace