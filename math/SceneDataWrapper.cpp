#include <list>
#include <iterator>
#include <algorithm>

#include <math/SceneDataWrapper.h>
#include <math/MeshWrapper.h>

namespace FreeWorldEngine {

namespace Math {

SceneDataWrapper::SceneDataWrapper(SceneData * pTargetSceneData) :
	m_pSceneData(pTargetSceneData)
{
}

SceneDataWrapper::~SceneDataWrapper()
{
}

void SceneDataWrapper::setTarget(SceneData * pTargetSceneData)
{
	m_pSceneData = pTargetSceneData;
}

SceneData *SceneDataWrapper::target() const
{
	return m_pSceneData;
}

void SceneDataWrapper::destroyTarget()
{
	delete m_pSceneData;
	m_pSceneData = 0;
}

SceneData::Mesh *SceneDataWrapper::findMesh(const std::string & name) const
{
	auto it = std::find_if(m_pSceneData->meshes.begin(), m_pSceneData->meshes.end(), [&name](SceneData::Mesh *p) { return p->name == name; });
	return (it == m_pSceneData->meshes.end()) ? 0 : *it;
}

SceneData::Material *SceneDataWrapper::findMaterial(const std::string & name) const
{
	auto it = std::find_if(m_pSceneData->materials.begin(), m_pSceneData->materials.end(), [&name](SceneData::Material *p) { return p->name == name; });
	return (it == m_pSceneData->materials.end()) ? 0 : *it;
}

SceneData::Node *SceneDataWrapper::findNode(const std::string & name) const
{
	SceneData::Node *pResultNode = 0;

	std::list<SceneData::Node*> nodes;
	if (m_pSceneData->pRootNode)
		nodes.push_back(m_pSceneData->pRootNode);

	while (!nodes.empty()) {
		SceneData::Node *pNode = nodes.front();
		nodes.erase(nodes.begin());
		if (pNode->name == name) {
			pResultNode = pNode;
			break;
		}
		std::copy(pNode->childNodes.begin(), pNode->childNodes.end(), std::back_inserter(nodes));
	}

	return pResultNode;
}

SceneData *SceneDataWrapper::clone() const
{
	SceneData *pNewScene = new SceneData;

	for (SceneData::MeshList::iterator it = m_pSceneData->meshes.begin(); it != m_pSceneData->meshes.end(); ++it) {
		SceneData::Mesh *pMesh = new SceneData::Mesh;
		pMesh->name = (*it)->name;
		pMesh->materialIndex = (*it)->materialIndex;
		pMesh->pMeshData = MeshWrapper((*it)->pMeshData).clone();
		pNewScene->meshes.push_back(pMesh);
	}

	for (SceneData::MaterialList::iterator it = m_pSceneData->materials.begin(); it != m_pSceneData->materials.end(); ++it) {
		SceneData::Material *pMaterial = new SceneData::Material;
		pMaterial->name = (*it)->name;
		pMaterial->pMaterialData = new Material;
		*(pMaterial->pMaterialData) = *((*it)->pMaterialData);
		pNewScene->materials.push_back(pMaterial);
	}

	if (m_pSceneData->pRootNode) {
		std::list<std::pair<SceneData::Node*, SceneData::Node*> > nodes;
		nodes.push_back(std::make_pair(m_pSceneData->pRootNode, pNewScene->pRootNode = new SceneData::Node));
		while (!nodes.empty()) {
			SceneData::Node *pOldNode = nodes.front().first;
			SceneData::Node *pNewNode = nodes.front().second;
			nodes.erase(nodes.begin());

			pNewNode->name = pOldNode->name;
			pNewNode->meshesIndices = pOldNode->meshesIndices;
			pNewNode->transform = pOldNode->transform;

			pNewNode->childNodes.resize(pOldNode->childNodes.size());
			for (uint32 i = 0; i < pOldNode->childNodes.size(); ++i)
				nodes.push_back(std::make_pair(pOldNode->childNodes[i], pNewNode->childNodes[i] = new SceneData::Node(pNewNode)));
		}
	}

	return pNewScene;
}



} // namespace

} // namespace
