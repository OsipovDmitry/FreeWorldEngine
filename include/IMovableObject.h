#ifndef __IMOVABLEOBJECT__
#define __IMOVABLEOBJECT__

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

namespace FreeWorldEngine {
	
class IMovableObject {
public:
	virtual ~IMovableObject() = 0 {}
	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::vec3 position() const = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual glm::quat orientation() const = 0; 
	virtual glm::mat4 modelMatrix() const = 0; // from local object space to world space

}; // class IMovableObject

} // namespace

#endif // __IMOVABLEOBJECT__