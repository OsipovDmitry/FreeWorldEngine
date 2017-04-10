#ifndef __KDTREE__
#define __KDTREE__

#include <vector>

#include <math/MathTypes.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GPUMesh;
class GraphicsSceneNode;
class GraphicsModel;

class KdNode {
public:
	typedef std::vector<GraphicsSceneNode*> SceneNodesList;

	KdNode(KdNode *pParent);
	~KdNode();

	void clear();
	void build(const GraphicsSceneNode* pNode);

	KdNode *firstChild() const;
	KdNode *secondChild() const;

	const SceneNodesList& sceneNodes() const;

	const Math::Aabb& localBoundingBox() const;
	Math::Aabb worldBoundingBox(const glm::mat4x4& worldTransformation) const;
	GPUMesh *boundingBoxGpuMesh() const;

private:
	KdNode *m_pParent, *m_childNodes[2];
	GPUMesh *m_pBoundingBoxGpuMesh;
	SceneNodesList m_sceneNodes;
	Math::Aabb m_localBoundingBox;

	void buildRecursive(const SceneNodesList& nodesList, const glm::mat4x4& sceneNodeWorldTransformation, GraphicsModel *pModel);

};

class KdTree {
public:
	KdTree();
	~KdTree();

	void rebuild(const GraphicsSceneNode *pNode);

	KdNode *rootNode() const;

private:
	KdNode *m_pRootNode;

};

} // namespace
} // namespace

#endif // __KDTREE__