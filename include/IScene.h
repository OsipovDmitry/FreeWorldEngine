#ifndef __ISCENE__
#define __ISCENE__

#include <vector>
#include <glm/mat4x4.hpp>

#include "Types.h"
#include "IResource.h"

namespace FreeWorldEngine {

class IScene : public IResource {
public:

	struct Material {
		std::string name;
	};
	typedef std::vector<IScene::Material> MaterialList;

	struct Mesh {
		std::string name;
		::Mesh *data;
		Material *pMaterial;
	};
	typedef std::vector<IScene::Mesh> MeshList;

	struct Node {
		std::string name;
		std::vector<Mesh*> meshes;
		std::vector<Node*> children;
		Node *pParent;
		glm::mat4x4 transform; 
	};

	virtual ~IScene() = 0 {}

	Node *rootNode() const;
	const MeshList& meshes() const;
	const MaterialList& materials() const;

}; // class IScene

} // namespace

#endif // __ISCENE__