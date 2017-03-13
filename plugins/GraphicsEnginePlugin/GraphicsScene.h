#ifndef __GRAPHICSSCENE__
#define __GRAPHICSSCENE__

#include <vector>

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsSceneNode : public IGraphicsSceneNode {
public:
	GraphicsSceneNode(GraphicsSceneNode *pParentNode);
	~GraphicsSceneNode();

	glm::vec3 position() const;
	void setPosition(const glm::vec3& pos);
	glm::quat orientation();
	void setOrientation(const glm::quat& orient);
	glm::mat4x4 transformation() const;
	glm::mat4x4 worldTransformation() const;

	IGraphicsSceneNode *createChild();
	void destroyChild(IGraphicsSceneNode *pNode);
	uint32 numChildren() const;
	IGraphicsSceneNode *childAt(const uint32 idx) const;
	IGraphicsSceneNode *parentNode() const;

	IGraphicsModel *model() const;
	void setModel(IGraphicsModel *pModel);

private:
	glm::mat4x4 m_cacheTransform, m_cacheWorldlTransform;
	GraphicsSceneNode *m_pParentNode;
	std::vector<GraphicsSceneNode*> m_childNodes;
	glm::vec3 m_position;
	glm::quat m_orientation;
	IGraphicsModel *m_pModel;

	void updateTransform();
	void updateWorldTransform();
};

class GraphicsScene : public IGraphicsScene {
public:
	GraphicsScene(const std::string& name);
	~GraphicsScene();

	std::string name() const;
	
	IGraphicsSceneNode *rootNode() const;

private:
	std::string m_name;
	GraphicsSceneNode *m_pRootNode;

}; // class GraphicsScene

} // namespace

} // namespace

#endif // __GRAPHICSSCENE__