#include "SDL2/include/SDL.h"
#include "FreeWorldEngine.h"

#include "WindowManagerPlugin.h"
#include "WindowManager.h"

namespace FreeWorldEngine {

IWindowManager *WindowManagerPlugin::pWindowManager = 0;

WindowManagerPlugin::WindowManagerPlugin()
{
}

WindowManagerPlugin::~WindowManagerPlugin()
{
}

std::string WindowManagerPlugin::name() const
{
	return std::string("WindowManagerPlugin");
}

std::string WindowManagerPlugin::info() const
{
	return std::string("SDL2 Window Manager Plugin");
}

bool WindowManagerPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (!pWindowManager)
		pWindowManager = new WindowManager();

	pCore->setWindowManager(pWindowManager);

	return true;
}

void WindowManagerPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->windowManager() == pWindowManager)
		pCore->setWindowManager(0);

	delete pWindowManager;
	pWindowManager = 0;

	SDL_Quit();
}

} // namespace