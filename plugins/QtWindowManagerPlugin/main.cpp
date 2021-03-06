#include "WindowManagerPlugin.h"

static FreeWorldEngine::IPlugin *pPlugin = 0;

extern "C" {

__declspec(dllexport) void startLibrary()
{
	if (!pPlugin)
		pPlugin = new FreeWorldEngine::WindowManagerPlugin();
}

__declspec(dllexport) FreeWorldEngine::IPlugin *getLibrary()
{
	return pPlugin;
}

__declspec(dllexport) void endLibrary()
{
	delete pPlugin;
	pPlugin = 0;
}

}