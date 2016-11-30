#ifndef __ISCENE__
#define __ISCENE__

#include "glm/mat4x4.hpp"
#include "Types.h"
#include "IResource.h"

namespace FreeWorldEngine {

/*class IScene : public IResource {
public:
	struct Material {
		std::string name;
	};

	struct Mesh {
		std::string name;
		FreeWorldEngine::Mesh data;
		Material *pMaterial;
	};

	struct Node {
		std::string name;
		std::vector<Mesh*> meshes;
		std::vector<Node*> children;
		Node *pParent;
		glm::mat4x4 transform; 
	};

	virtual ~IScene() = 0 {}

	Node *rootNode() const;
	const std::vector<Mesh*>& meshes() const;
	const std::vector<Materials*>& materials() const;

}; // class IScene
*/
} // namespace

#endif // __ISCENE__