#include <FreeWorldEngine.h>

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
	return std::string("Qt Window Manager Plugin");
}

bool WindowManagerPlugin::initialize()
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	// init libs

	if (!pWindowManager)
		pWindowManager = new WindowManager();

	pCore->setWindowManager(pWindowManager);

	return true;
}

void WindowManagerPlugin::deinitialize()
{
	ICore *pCore = getCoreEngine();

	if (pCore->windowManager() == pWindowManager)
		pCore->setWindowManager(0);

	delete pWindowManager;
	pWindowManager = 0;

	// deinit libs
}

} // namespace