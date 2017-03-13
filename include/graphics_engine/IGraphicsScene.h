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

class IGraphicsSceneNode {
public:
	virtual ~IGraphicsSceneNode() {}

	virtual glm::vec3 position() const = 0;
	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::quat orientation() = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual glm::mat4x4 transformation() const = 0;
	virtual glm::mat4x4 worldTransformation() const = 0;

	virtual IGraphicsSceneNode *createChild() = 0;
	virtual void destroyChild(IGraphicsSceneNode *pNode) = 0;
	virtual uint32 numChildren() const = 0;
	virtual IGraphicsSceneNode *childAt(const uint32 idx) const = 0;
	virtual IGraphicsSceneNode *parentNode() const = 0;

	virtual IGraphicsModel *model() const = 0;
	virtual void setModel(IGraphicsModel *pModel) = 0;
};

class IGraphicsScene : public IGraphicsResource {
public:
	virtual ~IGraphicsScene() {}
	
	virtual IGraphicsSceneNode *rootNode() const = 0;

}; // class IGraphicsModel

} // namespace

} // namespace

#endif // __IGRAPHICSSCENE__