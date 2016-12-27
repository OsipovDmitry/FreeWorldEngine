#include "FreeWorldEngine.h"

#include "SceneLoaderPlugin.h"
#include "SceneLoader.h"

namespace FreeWorldEngine {

ISceneLoader *SceneLoaderPlugin::pSceneLoader = 0;

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

	if (!pSceneLoader)
		pSceneLoader = new SceneLoader();

	pCore->setSceneLoader(pSceneLoader);

	return true;
}

void SceneLoaderPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->sceneLoader() == pSceneLoader)
		pCore->setSceneLoader(0);

	delete pSceneLoader;
	pSceneLoader = 0;
}

} // namespace