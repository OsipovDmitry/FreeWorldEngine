#ifndef __CAMERA__
#define __CAMERA__

#include "ICamera.h"

namespace FreeWorldEngine {

class Camera : public ICamera {
public:
	Camera();
	~Camera();
	
	void setPosition(const glm::vec3& pos);
	glm::vec3 position() const;
	void setOrientation(const glm::quat& orient);
	glm::quat orientation() const;
	glm::mat4 modelMatrix() const;

	void setOrientation(const float yaw, const float pitch, const float roll);
	float yaw() const;
	float pitch() const;
	float roll() const;

	void move(const glm::vec3& delta);
	void rotate(const glm::vec3& delta);

	glm::mat4 viewMatrix() const;
	glm::mat4 projectionMatrix() const;

	void setProjectionType(const ProjectionType proj);
	ProjectionType projectionType() const;

	void setProjectionFieldOfView(const float fov);
	float projectionFieldOfView() const;
	void setProjectionAspectRatio(const float aspect);
	float projectionAspectRatio() const;

	void setProjectionLeft(const float value);
	float projectionLeft() const;
	void setProjectionRight(const float value);
	float projectionRight() const;
	void setProjectionTop(const float value);
	float projectionTop() const;
	void setProjectionBottom(const float value);
	float projectionBottom() const;

	void setProjectionZNear(const float value);
	float projectionZNear() const;
	void setProjectionZFar(const float value);
	float projectionZFar() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_angles;
	
	struct {
		ProjectionType m_type;
		float m_fov; 
		float m_aspect;
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
		float m_zNear;
		float m_zFar;
	} m_projection;

}; // class Camera

} // namespace

#endif // __CAMERA__