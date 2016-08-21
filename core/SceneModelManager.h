#ifndef __SCENEMODELMANAGER__
#define __SCENEMODELMANAGER__

#include "ISceneModelManager.h" 

#include "assimp-3.2/include/assimp/Importer.hpp"

namespace FreeWorldEngine {

class ISceneModel;

class SceneModelManager : public ISceneModelManager{
public:
	SceneModelManager(IResourceManager *resourceManager);
	~SceneModelManager();
	ISceneModel *loadSceneModel(/*const std::string&*/const char* filename);
	ISceneModel *getByName(const std::string& name) const;
	IResourceManager *resourceManager() const;

private:
	IResourceManager *m_resourceManager;
	Assimp::Importer m_importer;

}; // class SceneModelManager

} // namespace

#endif // __SCENEMODELMANAGER__