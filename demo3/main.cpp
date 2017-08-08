#include <core/ICore.h>
#include <core/IWindowManager.h>
#include <core/IContentLoader.h>
#include <graphics_engine/IGraphicsEngine.h>
#include <graphics_engine/IGraphicsWindow.h>

int main(int argc, char *argv[])
{
	FreeWorldEngine::ICore *pEngine = FreeWorldEngine::ICore::instance();
	pEngine->initialize();

	FreeWorldEngine::GraphicsEngine::IGraphicsWindow *pRenderWindow = pEngine->graphicsEngine()->createWindow(pEngine->mainWindow());

	FreeWorldEngine::IImageLoader *pLoader = pEngine->imageLoader();
	pLoader->load("./c300.jpg");

	pEngine->windowManager()->mainLoop();
    return 0;
}
