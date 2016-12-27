#include "glm/gtc/matrix_transform.hpp"

#include "Camera.h"

namespace FreeWorldEngine {

Camera::Camera() :
	m_position(0.0f, 0.0f, 0.0f),
	m_angles(0.0f, 0.0f, 0.0f)
{
	m_projection.m_type = ICamera::ProjectionType_Perspective;
	m_projection.m_fov = glm::quarter_pi<float>();
	m_projection.m_aspect = 1.0f;
	m_projection.m_left = -1.0f;
	m_projection.m_right = 1.0f;
	m_projection.m_bottom = -1.0f;
	m_projection.m_top = 1.0f;
	m_projection.m_zNear = 0.1f;
	m_projection.m_zFar = 10000.0f;
}

Camera::~Camera()
{
}
	
void Camera::setPosition(const glm::vec3& pos)
{
	m_position = pos;
}

glm::vec3 Camera::position() const
{
	return m_position;
}

void Camera::setOrientation(const glm::quat& orient)
{
	m_angles = glm::eulerAngles(orient);
}

glm::quat Camera::orientation() const
{
	return glm::quat(m_angles);
}

glm::mat4 Camera::modelMatrix() const
{
	return glm::transpose(viewMatrix());
}

void Camera::setOrientation(const float yaw, const float pitch, const float roll)
{
	m_angles.x = yaw;
	m_angles.y = pitch;
	m_angles.z = roll;
}

float Camera::yaw() const
{
	return m_angles.x;
}

float Camera::pitch() const
{
	return m_angles.y;
}

float Camera::roll() const
{
	return m_angles.z;
}

void Camera::move(const glm::vec3& delta)
{
	m_position += glm::mat3(modelMatrix()) * delta;
}

void Camera::rotate(const glm::vec3& delta)
{
	m_angles += delta;
}

glm::mat4 Camera::viewMatrix() const
{
	return glm::rotate(glm::mat4(), m_angles.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
		glm::rotate(glm::mat4(), m_angles.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), m_angles.z, glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::translate(glm::mat4(), -m_position);
}

glm::mat4 Camera::projectionMatrix() const
{
	if (m_projection.m_type == ICamera::ProjectionType_Ortho)
		return glm::ortho(m_projection.m_left, m_projection.m_right, m_projection.m_bottom, m_projection.m_top, m_projection.m_zNear, m_projection.m_zFar);
	else if (m_projection.m_type == ICamera::ProjectionType_Perspective)
		return glm::perspective(m_projection.m_fov, m_projection.m_aspect, m_projection.m_zNear, m_projection.m_zFar);
	else
		return glm::mat4();
}

void Camera::setProjectionType(const ProjectionType proj)
{
	m_projection.m_type = proj;
}

ICamera::ProjectionType Camera::projectionType() const
{
	return m_projection.m_type;
}

void Camera::setProjectionFieldOfView(const float fov)
{
	m_projection.m_fov = fov;
}

float Camera::projectionFieldOfView() const
{
	return m_projection.m_fov;
}

void Camera::setProjectionAspectRatio(const float aspect)
{
	m_projection.m_aspect = aspect;
}

float Camera::projectionAspectRatio() const
{
	return m_projection.m_aspect;
}

void Camera::setProjectionLeft(const float value)
{
	m_projection.m_left = value;
}

float Camera::projectionLeft() const
{
	return m_projection.m_left;
}

void Camera::setProjectionRight(const float value)
{
	m_projection.m_right = value;
}

float Camera::projectionRight() const
{
	return m_projection.m_right;
}

void Camera::setProjectionTop(const float value)
{
	m_projection.m_top = value;
}

float Camera::projectionTop() const
{
	return m_projection.m_top;
}

void Camera::setProjectionBottom(const float value)
{
	m_projection.m_bottom = value;
}

float Camera::projectionBottom() const
{
	return m_projection.m_bottom;
}

void Camera::setProjectionZNear(const float value)
{
	m_projection.m_zNear = value;
}

float Camera::projectionZNear() const
{
	return m_projection.m_zNear;
}

void Camera::setProjectionZFar(const float value)
{
	m_projection.m_zFar = value;
}

float Camera::projectionZFar() const
{
	return m_projection.m_zFar;
}


} // namespace