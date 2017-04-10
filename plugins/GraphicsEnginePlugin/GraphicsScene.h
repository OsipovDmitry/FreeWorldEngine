#ifndef __GRAPHICSSCENE__
#define __GRAPHICSSCENE__

#include <vector>

#include <math/MathTypes.h>

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsModel;
class GPUMesh;
class KdTree;

class GraphicsSceneNode : public IGraphicsSceneNode {
public:
	typedef std::vector<GraphicsSceneNode*> ChildrenList;

	GraphicsSceneNode(GraphicsSceneNode *pParentNode);
	~GraphicsSceneNode();

	IGraphicsSceneNode *clone(IGraphicsSceneNode *pParent) const;

	glm::vec3 position() const;
	void setPosition(const glm::vec3& pos);
	glm::quat orientation();
	void setOrientation(const glm::quat& orient);
	const glm::mat4x4& localTransformation() const;
	const glm::mat4x4& worldTransformation() const;

	IGraphicsSceneNode *createChild();
	void destroyChild(IGraphicsSceneNode *pNode);
	uint32 numChildren() const;
	IGraphicsSceneNode *childAt(const uint32 idx) const;
	IGraphicsSceneNode *parentNode() const;

	IGraphicsModel *model() const;
	void setModel(IGraphicsModel *pModel);

	KdTree *kdTree() const;

private:
	mutable glm::mat4x4 m_cacheWorldlTransform, m_cacheLocalTransform;

	ChildrenList m_childNodes;
	GraphicsSceneNode *m_pParentNode;
	GraphicsModel *m_pModel;
	KdTree *m_pTree;

	glm::quat m_orientation;
	glm::vec3 m_position;
	mutable bool m_needUpdateTransformation, m_needUpdateBoundingBox;

	void updateTransformationRecursiveDown() const; // пометить матрицы на обновление рекурсивно вниз по дереву
	void recalcTransformation() const;
	
	void updateBoundingBoxRecursiveUp() const; // пометить баунд боксы на обновление рекурсивно вверх по дереву
	void updateBoundingBoxRecursiveDown() const; // пометить баунд боксы на обновление рекурсивно вниз по дереву 
	void recalcBoundingBox() const;

	friend class KdNode;
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