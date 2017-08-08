#include <Platform.h>

#include "SceneLoaderPlugin.h"

namespace {
	FreeWorldEngine::IPlugin *pPlugin = 0;
}

extern "C" {

LIBRARY_EXPORT void startLibrary()
{
	if (!pPlugin)
		pPlugin = new FreeWorldEngine::SceneLoaderPlugin();
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
