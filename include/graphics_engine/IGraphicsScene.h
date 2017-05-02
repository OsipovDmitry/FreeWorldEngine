#ifndef __IGRAPHICSSCENE__
#define __IGRAPHICSSCENE__

#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/mat4x4.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>

#include "IGraphicsResource.h"
#include "Types.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsModel;
class IGraphicsScene;

class IGraphicsSceneNode {
public:
	virtual ~IGraphicsSceneNode() {}

	virtual glm::vec3 position() const = 0;
	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::quat orientation() = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual const glm::mat4x4& localTransformation() const = 0;
	virtual const glm::mat4x4& worldTransformation() const = 0;

	virtual IGraphicsSceneNode *parentNode() const = 0;
	virtual IGraphicsScene *scene() const = 0;

	virtual void attachChildNode(IGraphicsSceneNode *pNode) = 0;
	virtual void detachChildNode(IGraphicsSceneNode *pNode) = 0;
	virtual uint32 numChildNodes() const = 0;
	virtual IGraphicsSceneNode *childNodeAt(const uint32 idx) const = 0;
	
	virtual IGraphicsModel *model() const = 0;
	virtual void setModel(IGraphicsModel *pModel) = 0;
};

class IGraphicsScene : public IGraphicsResource {
public:
	virtual ~IGraphicsScene() {}
	
	virtual IGraphicsSceneNode *rootNode() const = 0;
	virtual IGraphicsSceneNode *createNode(IGraphicsSceneNode *pParentNode, const glm::vec3& pos = glm::vec3(), const glm::quat& orient = glm::quat(), IGraphicsModel *pModel = nullptr) = 0;
	virtual void destroyNode(IGraphicsSceneNode *pNode) = 0;

}; // class IGraphicsModel

} // namespace

} // namespace

#endif // __IGRAPHICSSCENE__