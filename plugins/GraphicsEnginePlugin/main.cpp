#include <Platform.h>

#include "GraphicsEnginePlugin.h"

namespace {
	FreeWorldEngine::IPlugin *pPlugin = 0;
}

extern "C" {

LIBRARY_EXPORT void startLibrary()
{
	if (!pPlugin)
		pPlugin = new FreeWorldEngine::GraphicsEngine::GraphicsEnginePlugin();
}

LIBRARY_EXPORT FreeWorldEngine::IPlugin *getLibrary()
{
	return pPlugin;
}

LIBRARY_EXPORT void endLibrary()
{
	delete pPlugin;
	pPlugin = 0;
}

}
