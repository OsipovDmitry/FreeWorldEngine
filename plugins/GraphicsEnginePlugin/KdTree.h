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

	inline KdNode *firstChild() const { return m_childNodes[0]; }
	inline KdNode *secondChild() const { return m_childNodes[1]; }
	inline bool isLeaf() const { return m_childNodes[0] == nullptr && m_childNodes[1] == nullptr; }

	const SceneNodesList& sceneNodes() const;

	void insertSceneNode(GraphicsSceneNode *pSceneNode);
	void removeSceneNode(GraphicsSceneNode *pSceneNode);
	
	const Math::Aabb& boundingBox() const;
	GPUMesh *boundingBoxGpuMesh() const;

private:
	static const int32 s_maxSceneNodeInKdNode = 7;

	KdNode *m_pParent, *m_childNodes[2];
	GPUMesh *m_pBoundingBoxGpuMesh;
	SceneNodesList m_sceneNodes;
	Math::Aabb m_boundingBox;

	void rebuild();
	void updateBoundingBox(const Math::Aabb& newChildBox);
	void buildRecursive(const SceneNodesList& sceneNodesList);
	KdNode *findDeepestCoveringNode(const Math::Aabb& boundBox);

	friend class KdTree;
};

class KdTree {
public:
	KdTree();
	~KdTree();

	KdNode *rootNode() const;

	void reinsertSceneNode(GraphicsSceneNode *pSceneNode);
	void removeSceneNode(GraphicsSceneNode *pSceneNode);

private:
	KdNode *m_pRootNode;

};

} // namespace
} // namespace

#endif // __KDTREE__