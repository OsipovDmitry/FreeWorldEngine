#ifndef __OCTREESCENEOPTIMIZER__
#define __OCTREESCENEOPTIMIZER__

#include "AbstractSceneOptimizer.h"

namespace FreeWorldEngine {

namespace Math {
	struct Aabb;
} // namespace

namespace GraphicsEngine {

struct OctreeNode;

class OctreeSceneOptimizer : public AbstractSceneOptimizer {
public:
	OctreeSceneOptimizer(GraphicsScene *pScene);
	~OctreeSceneOptimizer();

	IGraphicsScene::SceneOptimizerType type() const;

	void updateRenderData(IGraphicsCamera *pCamera, uint64 frameCounter);

	void nodeCreated(GraphicsSceneNode *pNode);
	void nodeDestroyed(GraphicsSceneNode *pNode);
	void nodeAttached(GraphicsSceneNode *pNode);
	void nodeDetached(GraphicsSceneNode *pNode);
	void nodeTransformationChanged(GraphicsSceneNode *pNode);
	void nodeModelChanged(GraphicsSceneNode *pNode);

	void lightCreated(GraphicsLight *pLight);
	void lightDestroyed(GraphicsLight *pLight);
	void lightTransformationChanged(GraphicsLight *pLight);

	OctreeNode *rebuildOctree();
	void reinsertSceneNode(GraphicsSceneNode *pNode);
	OctreeNode *findBestOctreeNode(GraphicsSceneNode *pNode);

	static int32 calcChildIndexFromBox(OctreeNode *pOctreeNode, const Math::Aabb& box);
	static void cutOctreeNode(OctreeNode *pOctreeNode);
	static void pushSceneNodeToOctreeNode(GraphicsSceneNode *pNode, OctreeNode *pOctreeNode);
	static void popSceneNodeFromOctreeNode(GraphicsSceneNode *pNode);
	static std::vector<GraphicsLight*> lightsForSceneNode(GraphicsSceneNode *pNode);

	static const int32 s_maxSceneNodesPerOctreeNode = 16;
	static const int32 s_maxLightsPerSceneNode = 4;

	void detachLight(GraphicsLight *pLight);
	void attachLight(GraphicsLight *pLight);

private:
	OctreeNode *m_pRootOctreeNode;
	bool m_needRebuild;

};


} // namespace
} // namespace


#endif // __OCTREESCENEOPTIMIZER__
