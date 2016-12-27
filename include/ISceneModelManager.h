#ifndef __ISCENEMODELMANAGER__
#define __ISCENEMODELMANAGER__

namespace FreeWorldEngine {

class ISceneModel;
class IResourceManager;

class ISceneModelManager {
public:
	virtual ~ISceneModelManager() = 0 {}
	virtual ISceneModel *loadSceneModel(/*const std::string&*/const char* filename) = 0;
	virtual ISceneModel *getByName(const std::string& name) const = 0;
	virtual IResourceManager *resourceManager() const = 0;
}; // class ISceneModelManager

} // namespace

#endif // __ISCENEMODELMANAGER__