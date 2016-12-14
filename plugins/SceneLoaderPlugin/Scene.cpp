#include <queue>

#include <math/MeshWrapper.h>

#include "Scene.h"

namespace FreeWorldEngine {

Scene::Scene() :
	m_materials(),
	m_meshes(),
	m_pRootNode(0)
{
}

Scene::~Scene()
{
	for (MeshList::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it) {
		Math::MeshWrapper(it->data).destroyTargetMesh();
	}

	std::queue<IScene::Node*> nodeQueue;
	nodeQueue.push(m_pRootNode);
	while (nodeQueue.size()) {
		IScene::Node *pNode = nodeQueue.front();
		nodeQueue.pop();
		if (!pNode) continue;
		for (std::vector<IScene::Node*>::iterator it = pNode->children.begin(); it != pNode->children.end(); ++it)
			nodeQueue.push(*it);
		delete pNode;
	}
}

IScene::Node *Scene::rootNode() const
{
	return m_pRootNode;
}

const IScene::MeshList& Scene::meshes() const
{
	return m_meshes;
}

const IScene::MaterialList& Scene::materials() const
{
	return m_materials;
}

} // namespace