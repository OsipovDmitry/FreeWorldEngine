#include <FreeWorldEngine.h>

#include "SceneLoader.h"

namespace FreeWorldEngine {

SceneLoader::SceneLoader() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForScenes"))
{
}

SceneLoader::~SceneLoader()
{
}

IScene *SceneLoader::loadScene(const std::string& filename)
{
	return 0;
}

IScene *SceneLoader::findScene(const std::string& name) const
{
	return 0;
}

void SceneLoader::destoryScene(const std::string& name)
{
}

void SceneLoader::destoryScene(IScene *pImage)
{
}

} // namespace