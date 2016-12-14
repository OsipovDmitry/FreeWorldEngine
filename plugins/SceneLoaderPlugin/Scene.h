#ifndef __SCENE__
#define __SCENE__

#include <IScene.h>

namespace FreeWorldEngine {

class Scene : public IScene {
public:
	Scene();
	~Scene();

	Node *rootNode() const;
	const MeshList& meshes() const;
	const MaterialList& materials() const;

private:
	MeshList m_meshes;
	MaterialList m_materials;
	Node *m_pRootNode;

}; // class IScene

} // namespace

#endif // __SCENE__