#ifndef __SCENECONTAINER__
#define __SCENECONTAINER__

#include <IScene.h>

namespace FreeWorldEngine {

class SceneContainer : public IScene {
public:
	SceneContainer(const std::string& name, Scene *pScene);
	~SceneContainer();

	std::string name() const;
	const Scene *scene() const;

private:
	std::string m_name;
	Scene *m_pScene;

}; // class SceneContainer

} // namespace

#endif // __SCENECONTAINER__