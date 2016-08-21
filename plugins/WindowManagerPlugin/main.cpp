#include "WindowManagerPlugin.h"

static FreeWorldEngine::IPlugin *pPlugin = 0;

extern "C" {

__declspec(dllexport) FreeWorldEngine::IPlugin *startLibrary()
{
	if (!pPlugin)
		pPlugin = new FreeWorldEngine::WindowManagerPlugin();
	return pPlugin;
}

__declspec(dllexport) void endLibrary()
{
	delete pPlugin;
	pPlugin = 0;
}

}