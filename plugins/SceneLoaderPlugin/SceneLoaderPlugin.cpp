#include "FreeWorldEngine.h"

#include "SceneLoaderPlugin.h"

namespace FreeWorldEngine {


SceneLoaderPlugin::SceneLoaderPlugin()
{
}

SceneLoaderPlugin::~SceneLoaderPlugin()
{
}

std::string SceneLoaderPlugin::name() const
{
	return std::string("SceneLoaderPlugin");
}

std::string SceneLoaderPlugin::info() const
{
	return std::string("Assimp Scene Loader Plugin");
}

bool SceneLoaderPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	return true;
}

void SceneLoaderPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	//if (pCore->windowManager() == pWindowManager)
	//	pCore->setWindowManager(0);

}

} // namespace