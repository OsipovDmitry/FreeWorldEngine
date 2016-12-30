#include <queue>

#include <Types.h>

#include "Scene.h"

namespace FreeWorldEngine {

SceneContainer::SceneContainer(const std::string& name, Scene *pScene) :
	m_name(name),
	m_pScene(pScene)
{
}

SceneContainer::~SceneContainer()
{
}

std::string SceneContainer::name() const
{
	return m_name;
}

const Scene *SceneContainer::scene() const
{
	return m_pScene;
}

} // namespace