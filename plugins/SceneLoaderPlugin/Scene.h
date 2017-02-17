#ifndef __SCENECONTAINER__
#define __SCENECONTAINER__

#include <IScene.h>

namespace FreeWorldEngine {

class Scene : public IScene {
public:
	Scene(const std::string& name, SceneData *pScene);
	~Scene();

	std::string name() const;
	const SceneData *scene() const;

private:
	std::string m_name;
	SceneData *m_pScene;

}; // class Scene

} // namespace

#endif // __SCENECONTAINER__