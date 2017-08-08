#ifndef __GRAPHICSSCENE__
#define __GRAPHICSSCENE__

#include <vector>
#include <map>

#include <math/MathTypes.h>

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
} // namespace

namespace GraphicsEngine {

class GraphicsModel;
class GraphicsScene;
class GraphicsLight;
class GPUMesh;
class AbstractSceneOptimizer;

class GraphicsSceneNode : public IGraphicsSceneNode {
public:
	typedef std::vector<GraphicsSceneNode*> ChildrenList;

	GraphicsSceneNode(GraphicsScene *pScene);
	~GraphicsSceneNode();

	glm::vec3 position() const;
	void setPosition(const glm::vec3& pos);
	glm::quat orientation();
	void setOrientation(const glm::quat& orient);
	const glm::mat4x4& localTransformation() const;
	const glm::mat4x4& worldTransformation() const;

	IGraphicsSceneNode *parentNode() const;
	IGraphicsScene *scene() const;

	void attachChildNode(IGraphicsSceneNode *pNode);
	void detachChildNode(IGraphicsSceneNode *pNode);
	uint32 numChildNodes() const;
	IGraphicsSceneNode *childNodeAt(const uint32 idx) const;

	IGraphicsModel *model() const;
	void setModel(IGraphicsModel *pModel);

	const Math::Sphere& worldBoundingSphere() const;

	GraphicsModel *modelImpl() const;
	GraphicsScene *sceneImpl() const;
	GraphicsSceneNode *parentSceneNodeImpl() const;
	const ChildrenList& chilNodesImpl() const;

	void setOptimizerData(void *pData);
	void *optimizerData() const;

private:
	mutable glm::mat4x4 m_cacheWorldlTransform, m_cacheLocalTransform;
	mutable Math::Sphere m_worldSphere;

	ChildrenList m_childNodes;
	GraphicsScene *m_pScene;
	GraphicsSceneNode *m_pParentNode;
	GraphicsModel *m_pModel;

	void *m_pOptimizerData;

	glm::quat m_orientation;
	glm::vec3 m_position;
	mutable bool m_needUpdateTransformation, m_needUpdateBoundingSphere;

	void updateTransformationRecursiveDown();
	void updateBoundingSpheresRecursiveDown() const;
	void recalcTransformation() const;
	void recalcBoundingSphere() const;
};

class GraphicsScene : public IGraphicsScene {
public:
	GraphicsScene(const std::string& name, IGraphicsScene::SceneOptimizerType optimizerType);
	~GraphicsScene();

	std::string name() const;
	
	IGraphicsSceneNode *rootNode() const;
	IGraphicsSceneNode *createNode();
	void destroyNode(IGraphicsSceneNode *pNode);

	IGraphicsLight *findLight(const std::string& name) const;
	IGraphicsLight *createLight(IGraphicsLight::Type type, const std::string& name = "@utoname");
	void destroyLight(const std::string& name);
	void destroyLight(IGraphicsLight *pLight);

	SceneOptimizerType optimizerType() const;

	AbstractSceneOptimizer *optimizer() const;

	GraphicsSceneNode *rootNodeImpl() const;
	IResourceManager *lightsImpl() const;

private:
	std::string m_name;
	GraphicsSceneNode *m_pRootNode;
	AbstractSceneOptimizer *m_pOptimizer;
	IResourceManager *m_pLightManager;
	Utility::AutoNameGenerator *m_pLightNameGenerator;

	std::list<GraphicsSceneNode*> m_nodes;

}; // class GraphicsScene

} // namespace

} // namespace

#endif // __GRAPHICSSCENE__
