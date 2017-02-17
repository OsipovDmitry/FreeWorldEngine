#include <queue>

#include <Types.h>

#include "Scene.h"

namespace FreeWorldEngine {

Scene::Scene(const std::string& name, SceneData *pScene) :
	m_name(name),
	m_pScene(pScene ? pScene : new SceneData)
{
}

Scene::~Scene()
{
}

std::string Scene::name() const
{
	return m_name;
}

const SceneData *Scene::scene() const
{
	return m_pScene;
}

} // namespace