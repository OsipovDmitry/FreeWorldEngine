#include <3rdparty/glm/gtc/matrix_transform.hpp>

#include "GraphicsCamera.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {
	
GraphicsCamera::GraphicsCamera(const std::string & name) :
	m_name(name),
	m_frustum(),
	m_cacheProjMatrix(),
	m_cacheViewMatrix(),
	m_orientQuat(),
	m_orientAngles(0.0f, 0.0f, 0.0f),
	m_orientMatrix(),
	m_position(),
	m_aspectRatio(1.0f),
	m_zNear(0.5f),
	m_zFar(1000.0f),
	m_fov(45.0f),
	m_halfHeight(1.0f),
	m_projType(ProjectionType_Perspective),
	m_needUpProjMatrix(true),
	m_needUpViewMatrix(true)
{
}

GraphicsCamera::~GraphicsCamera()
{
}

std::string GraphicsCamera::name() const
{
	return m_name;
}

glm::mat4x4 GraphicsCamera::viewMatrix() const
{
	return m_cacheViewMatrix;
}

glm::mat4x4 GraphicsCamera::projectionMatrix() const
{
	return m_cacheProjMatrix;
}

void GraphicsCamera::setViewMatrix(const glm::mat4x4 & value)
{
	m_cacheViewMatrix = value;
	m_needUpViewMatrix = false;
}

void GraphicsCamera::setProjectionMatrix(const glm::mat4x4 & value)
{
	m_cacheProjMatrix = value;
	m_needUpProjMatrix = false;
}

void GraphicsCamera::setAspectRatio(const float aspectRatio)
{
	m_aspectRatio = aspectRatio;
	m_needUpProjMatrix = true;
}

float GraphicsCamera::aspectRatio() const
{
	return m_aspectRatio;
}

void GraphicsCamera::setDepthRange(const float zNear, const float zFar)
{
	m_zNear = zNear;
	m_zFar = zFar;
	m_needUpProjMatrix = true;
}

void GraphicsCamera::depthRange(float & zNear, float & zFar) const
{
	zNear = m_zNear;
	zFar = m_zFar;
}

void GraphicsCamera::setPerspectiveProjection(const float fov)
{
	m_fov = fov;
	m_projType = ProjectionType_Perspective;
	m_needUpProjMatrix = true;
}

void GraphicsCamera::setOrthoProjection(const float halfHeight)
{
	m_halfHeight = halfHeight;
	m_projType = ProjectionType_Ortho;
	m_needUpProjMatrix = true;
}

void GraphicsCamera::setPosition(const glm::vec3 & pos)
{
	m_position = pos;
	m_needUpViewMatrix = true;
}

glm::vec3 GraphicsCamera::position() const
{
	return m_position;
}

void GraphicsCamera::setOrientation(const glm::mat3x3 & rotateMatrix)
{
	m_orientMatrix = rotateMatrix;
	m_orientType = OrientType_Dirs;
	m_needUpViewMatrix = true;
}

void GraphicsCamera::setOrientation(const glm::vec3 & right, const glm::vec3 & up, const glm::vec3 & back)
{
	m_orientMatrix = glm::mat3x3(glm::normalize(right), glm::normalize(up), glm::normalize(back));
	m_orientType = OrientType_Dirs;
	m_needUpViewMatrix = true;
}

void GraphicsCamera::setOrientation(const float yaw, const float pitch, const float roll)
{
	m_orientAngles = glm::vec3(yaw, pitch, roll);
	m_orientType = OrientType_Angles;
	m_needUpViewMatrix = true;
}

void GraphicsCamera::setOrientation(const glm::quat & orient)
{
	m_orientQuat = orient;
	m_orientType = OrientType_Quat;
	m_needUpViewMatrix = true;
}

glm::quat GraphicsCamera::orientation() const
{
	return m_orientQuat;
}

glm::vec3 GraphicsCamera::rightDirection() const
{
	return m_orientMatrix[0];
}

glm::vec3 GraphicsCamera::upDirection() const
{
	return m_orientMatrix[1];
}

glm::vec3 GraphicsCamera::backwardDirection() const
{
	return m_orientMatrix[2];
}

const Math::Frustum & GraphicsCamera::frustum() const
{
	return m_frustum;
}

void GraphicsCamera::update()
{
	if (m_needUpProjMatrix) {
		switch (m_projType) {
		case ProjectionType_Perspective: m_cacheProjMatrix = glm::perspective(m_fov, m_aspectRatio, m_zNear, m_zFar); break;
		case ProjectionType_Ortho: m_cacheProjMatrix = glm::ortho(-m_halfHeight*m_aspectRatio, m_halfHeight*m_aspectRatio, -m_halfHeight, m_halfHeight, m_zNear, m_zFar); break;
		default: break;
		}
	}

	if (m_needUpViewMatrix) {
		switch (m_orientType) {
		case OrientType_Dirs: {
			m_orientQuat = glm::quat(m_orientMatrix);
			m_orientAngles = glm::eulerAngles(m_orientQuat);
			break;
		}
		case OrientType_Quat: {
			m_orientAngles = glm::eulerAngles(m_orientQuat);
			m_orientMatrix = m_orientQuat.operator glm::tmat3x3<float, glm::highp>();
			break;
		}
		case OrientType_Angles: {
			m_orientQuat = glm::quat(m_orientAngles);
			m_orientMatrix = m_orientQuat.operator glm::tmat3x3<float, glm::highp>();
			break;
		}
		default: break;
		}
		m_cacheViewMatrix = glm::translate(glm::mat4x4(m_orientMatrix), -m_position);
	}

	if (m_needUpProjMatrix || m_needUpViewMatrix) {
		glm::mat4 vp = m_cacheProjMatrix * m_cacheViewMatrix;
		// right plane
		m_frustum[0].x = vp[0][3] - vp[0][0];
		m_frustum[0].y = vp[1][3] - vp[1][0];
		m_frustum[0].z = vp[2][3] - vp[2][0];
		m_frustum[0].w = vp[3][3] - vp[3][0];
		// left plane
		m_frustum[1].x = vp[0][3] + vp[0][0];
		m_frustum[1].y = vp[1][3] + vp[1][0];
		m_frustum[1].z = vp[2][3] + vp[2][0];
		m_frustum[1].w = vp[3][3] + vp[3][0];
		// top plane
		m_frustum[2].x = vp[0][3] - vp[0][1];
		m_frustum[2].y = vp[1][3] - vp[1][1];
		m_frustum[2].z = vp[2][3] - vp[2][1];
		m_frustum[2].w = vp[3][3] - vp[3][1];
		// bottom plane
		m_frustum[3].x = vp[0][3] + vp[0][1];
		m_frustum[3].y = vp[1][3] + vp[1][1];
		m_frustum[3].z = vp[2][3] + vp[2][1];
		m_frustum[3].w = vp[3][3] + vp[3][1];
		// far plane
		m_frustum[4].x = vp[0][3] - vp[0][2];
		m_frustum[4].y = vp[1][3] - vp[1][2];
		m_frustum[4].z = vp[2][3] - vp[2][2];
		m_frustum[4].w = vp[3][3] - vp[3][2];
		// near plane
		m_frustum[5].x = vp[0][3] + vp[0][2];
		m_frustum[5].y = vp[1][3] + vp[1][2];
		m_frustum[5].z = vp[2][3] + vp[2][2];
		m_frustum[5].w = vp[3][3] + vp[3][2];
		// normalize
		for (int32 i = 0; i < 6; i++) {
			float len = glm::vec3(m_frustum[i]).length();
			if (len > Math::eps)
				m_frustum[i] /= len;
		}
	}

	m_needUpViewMatrix = false;
	m_needUpProjMatrix = false;
}

} // namespace

} // namespaces