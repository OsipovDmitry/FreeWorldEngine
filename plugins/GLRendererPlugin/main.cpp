#include <Platform.h>

#include "GLRendererPlugin.h"

static FreeWorldEngine::IPlugin *pPlugin = 0;

extern "C" {

LIBRARY_EXPORT void startLibrary()
{
	if (!pPlugin)
		pPlugin = new FreeWorldEngine::Renderer::GLRendererPlugin();
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
