#ifndef __SCENELOADER__
#define __SCENELOADER__

#include <ISceneLoader.h>

namespace FreeWorldEngine {

class IResourceManager;

class SceneLoader : public ISceneLoader {
public:
	SceneLoader();
	~SceneLoader();
	IScene *loadScene(const std::string& filename);
	IScene *findScene(const std::string& name) const;
	void destoryScene(const std::string& name);
	void destoryScene(IScene *pImage);

private:
	IResourceManager *m_pResourceManager;

};

} // namespace

#endif // __SCENELOADER__