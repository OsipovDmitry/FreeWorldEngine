#ifndef __GRAPHICSSCENE__
#define __GRAPHICSSCENE__

#include <list>

#include <math/MathTypes.h>

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsModel;
class GraphicsScene;
class GPUMesh;
class KdNode;
class KdTree;

class GraphicsSceneNode : public IGraphicsSceneNode {
public:
	typedef std::list<GraphicsSceneNode*> ChildrenList;

	GraphicsSceneNode(GraphicsScene *pScene);
	~GraphicsSceneNode();

	glm::vec3 position() const;
	void setPosition(const glm::vec3& pos);
	glm::quat orientation();
	void setOrientation(const glm::quat& orient);
	const glm::mat4x4& localTransformation() const;
	const glm::mat4x4& worldTransformation() const;
	const Math::Sphere& worldBoundingSphere() const;
	const Math::Aabb& worldBoundingBox() const;

	IGraphicsSceneNode *parentNode() const;
	IGraphicsScene *scene() const;

	void attachChildNode(IGraphicsSceneNode *pNode);
	void detachChildNode(IGraphicsSceneNode *pNode);
	uint32 numChildNodes() const;
	IGraphicsSceneNode *childNodeAt(const uint32 idx) const;

	IGraphicsModel *model() const;
	void setModel(IGraphicsModel *pModel);

	KdNode *kdNode() const;
	void setKdNode(KdNode *pKdNode);

	void detachFromScene();

private:
	mutable glm::mat4x4 m_cacheWorldlTransform, m_cacheLocalTransform;
	mutable Math::Sphere m_worldSphere;
	mutable Math::Aabb m_worldAabb;

	ChildrenList m_childNodes;
	GraphicsScene *m_pScene;
	GraphicsSceneNode *m_pParentNode;
	GraphicsModel *m_pModel;
	KdNode *m_pKdNode;

	glm::quat m_orientation;
	glm::vec3 m_position;
	mutable bool m_needUpdateTransformation, m_needUpdateSphere;

	void updateTransformationRecursiveDown() const; // пометить матрицы на обновление рекурсивно вниз по дереву
	void recalcTransformation() const;
	void recalcSphere() const;

};

class GraphicsScene : public IGraphicsScene {
public:
	GraphicsScene(const std::string& name);
	~GraphicsScene();

	std::string name() const;
	
	IGraphicsSceneNode *rootNode() const;
	IGraphicsSceneNode *createNode(IGraphicsSceneNode *pParentNode, const glm::vec3& pos = glm::vec3(), const glm::quat& orient = glm::quat(), IGraphicsModel *pModel = nullptr);
	void destroyNode(IGraphicsSceneNode *pNode);

	KdTree *kdTree() const;

private:
	std::string m_name;
	GraphicsSceneNode *m_pRootNode;
	KdTree *m_pTree;

	std::list<GraphicsSceneNode*> m_nodes;

}; // class GraphicsScene

} // namespace

} // namespace

#endif // __GRAPHICSSCENE__