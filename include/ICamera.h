#ifndef __ICAMERA__
#define __ICAMERA__

#include "IMovableObject.h"

namespace FreeWorldEngine {

class ICamera : public IMovableObject {
public:
	enum ProjectionType {
		ProjectionType_Ortho = 0,
		ProjectionType_Perspective
	};

	virtual ~ICamera() = 0 {}
	
	virtual void setOrientation(const float yaw, const float pitch, const float roll) = 0;
	virtual float yaw() const = 0;
	virtual float pitch() const = 0;
	virtual float roll() const = 0;

	virtual void move(const glm::vec3& delta) = 0;
	virtual void rotate(const glm::vec3& delta) = 0;

	virtual glm::mat4 viewMatrix() const = 0; // from world space to view space
	virtual glm::mat4 projectionMatrix() const = 0; // from view space to clip space

	virtual void setProjectionType(const ProjectionType proj) = 0;
	virtual ProjectionType projectionType() const = 0;

	// for perspective projection
	virtual void setProjectionFieldOfView(const float fov) = 0;
	virtual float projectionFieldOfView() const = 0;
	virtual void setProjectionAspectRatio(const float aspect) = 0;
	virtual float projectionAspectRatio() const = 0;

	// for ortho projection
	virtual void setProjectionLeft(const float value) = 0;
	virtual float projectionLeft() const = 0;
	virtual void setProjectionRight(const float value) = 0;
	virtual float projectionRight() const = 0;
	virtual void setProjectionTop(const float value) = 0;
	virtual float projectionTop() const = 0;
	virtual void setProjectionBottom(const float value) = 0;
	virtual float projectionBottom() const = 0;

	// coomon methods
	virtual void setProjectionZNear(const float value) = 0;
	virtual float projectionZNear() const = 0;
	virtual void setProjectionZFar(const float value) = 0;
	virtual float projectionZFar() const = 0;

}; // class ICamera

} // namespace

#endif // __ICAMERA__