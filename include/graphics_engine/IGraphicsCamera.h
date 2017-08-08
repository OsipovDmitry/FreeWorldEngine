#ifndef __IGRAPHICSCAMERA__
#define __IGRAPHICSCAMERA__

#include <3rdparty/glm/mat3x3.hpp>
#include <3rdparty/glm/mat4x4.hpp>
#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/gtc/quaternion.hpp>

#include <math/MathTypes.h>

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsCamera: public IGraphicsResource {
public:
	virtual ~IGraphicsCamera() {}

	virtual glm::mat4x4 viewMatrix() const = 0;
	virtual glm::mat4x4 projectionMatrix() const = 0;

	virtual void setViewMatrix(const glm::mat4x4& value) = 0; // Принудительная установка матрицы. Методы position(), orientation(), *Direction() станут невалидными
	virtual void setProjectionMatrix(const glm::mat4x4& value) = 0; // Принудительная установка матрицы. Методы aspectRatio, depthRange станут невалидными

	virtual void setAspectRatio(const float aspectRatio) = 0;
	virtual float aspectRatio() const = 0;

	virtual void setDepthRange(const float zNear, const float zFar) = 0;
	virtual void depthRange(float& zNear, float& zFar) const = 0;

	virtual void setPerspectiveProjection(const float fov) = 0;
	virtual void setOrthoProjection(const float halfHeight) = 0;

	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::vec3 position() const = 0;

	virtual void setOrientation(const glm::mat3x3& rotateMatrix) = 0;
	virtual void setOrientation(const glm::vec3& right, const glm::vec3& up, const glm::vec3& forward) = 0;
	virtual void setOrientation(const float yaw, const float pitch, const float roll) = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual glm::quat orientation() const = 0;
	virtual glm::vec3 rightDirection() const = 0;
	virtual glm::vec3 upDirection() const = 0;
	virtual glm::vec3 backwardDirection() const = 0;

	virtual const Math::Frustum& frustum() const = 0;

}; // class IGraphicsModel

} // namespace

} // namespace

#endif // __IGRAPHICSCAMERA__
