#ifndef __IGRAPHICSSCENE__
#define __IGRAPHICSSCENE__

#include <list>
#include <algorithm>

#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsModel;

class IGraphicsScene : public IGraphicsResource {
public:
	class Node {
	public:
		Node(Node *pParentNode) : m_pParentNode(pParentNode), m_childNodes(), m_position(), m_orientation(), m_pModel(0) {}
		~Node() {
			for (std::list<Node*>::iterator it = m_childNodes.begin(); it != m_childNodes.end(); ++it)
				delete *it;
		}

		glm::vec3 position() const { return m_position; }
		void setPosition(const glm::vec3& pos) { m_position = pos; }
		glm::quat orientation() const { return m_orientation; }
		void setPosition(const glm::quat& orient) { m_orientation = orient; }
		
		Node *createChild() {
			Node *pChild = new Node(this);
			m_childNodes.push_back(pChild);
			return pChild;
		}
		void destroyChild(Node *pNode) {
			std::list<Node*>::iterator it = std::find(m_childNodes.begin(), m_childNodes.end(), pNode);
			if (it != m_childNodes.end()) { m_childNodes.erase(it); delete *it; }
		}
		Node *parentNode() const { return m_pParentNode; }

		IGraphicsModel *model() const { return m_pModel; }
		void setModel(IGraphicsModel *pModel) { m_pModel = pModel; }

	private:
		Node *m_pParentNode;
		std::list<Node*> m_childNodes;
		glm::vec3 m_position;
		glm::quat m_orientation;
		IGraphicsModel *m_pModel;
	};


	virtual ~IGraphicsScene() {}
	
	virtual Node *rootNode() const = 0; 

}; // class IGraphicsModel

} // namespace

} // namespace

#endif // __IGRAPHICSSCENE__