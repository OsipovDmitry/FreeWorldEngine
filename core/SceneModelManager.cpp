#include "IResourceManager.h"
#include "ILog.h"

#include "assimp-3.2/include/assimp/scene.h"
#include "assimp-3.2/include/assimp/postprocess.h"
#include "SceneModelManager.h"
#include "SceneModel.h"
#include "Core.h"

namespace FreeWorldEngine {

SceneModelManager::SceneModelManager(IResourceManager *resourceManager) :
	m_resourceManager(resourceManager),
	m_importer()
{
}

SceneModelManager::~SceneModelManager()
{
}

ISceneModel *SceneModelManager::loadSceneModel(/*const std::string&*/const char* filename)
{
	ISceneModel *pSceneModel = getByName(filename);
	if (pSceneModel)
		return pSceneModel;

	const aiScene *pScene = m_importer.ReadFile(filename, aiProcess_Triangulate);
	if (!pScene) {
		coreEngine->log() << "Failed open file \"" << filename << "\"\n"; 
		return 0;
	}

	pSceneModel = new SceneModel(filename, pScene);
	m_resourceManager->addResource(pSceneModel);
	return pSceneModel;
}

ISceneModel *SceneModelManager::getByName(const std::string& name) const
{
	return reinterpret_cast<ISceneModel*>(m_resourceManager->getByName(name));
}

IResourceManager *SceneModelManager::resourceManager() const
{
	return m_resourceManager;
}

} // namespace