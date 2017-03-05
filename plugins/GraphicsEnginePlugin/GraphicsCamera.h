#ifndef __GRAPHICSCAMERA__
#define __GRAPHICSCAMERA__

#include <graphics_engine/IGraphicsCamera.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsCamera: public IGraphicsCamera {
public:
	GraphicsCamera(const std::string& name);
	~GraphicsCamera();

	std::string name() const;

	glm::mat4x4 viewMatrix() const;
	glm::mat4x4 projectionMatrix() const;

	void setViewMatrix(const glm::mat4x4& value);
	void setProjectionMatrix(const glm::mat4x4& value);

	void setAspectRatio(const float aspectRatio);
	float aspectRatio() const;

	void setDepthRange(const float zNear, const float zFar);
	void depthRange(float& zNear, float& zFar) const;

	void setPerspectiveProjection(const float fov);
	void setOrthoProjection(const float halfHeight);

	void setPosition(const glm::vec3& pos);
	glm::vec3 position() const;

	void setOrientation(const glm::mat3x3& rotateMatrix);
	void setOrientation(const glm::vec3& right, const glm::vec3& up, const glm::vec3& back);
	void setOrientation(const float yaw, const float pitch, const float roll);
	void setOrientation(const glm::quat& orient);
	glm::quat orientation() const;
	glm::vec3 rightDirection() const;
	glm::vec3 upDirection() const;
	glm::vec3 backwardDirection() const;

	const Math::Frustum& frustum() const;

	void update();

private:
	std::string m_name;

	Math::Frustum m_frustum;

	glm::mat4x4 m_cacheViewMatrix, m_cacheProjMatrix;
	glm::quat m_orientQuat;
	glm::mat3x3 m_orientMatrix;
	glm::vec3 m_orientAngles;
	glm::vec3 m_position;
	
	float m_aspectRatio, m_zNear, m_zFar;
	float m_fov, m_halfHeight;

	enum ProjectionType { ProjectionType_Perspective, ProjectionType_Ortho };
	ProjectionType m_projType;

	enum OrientType { OrientType_Quat, OrientType_Dirs, OrientType_Angles };
	OrientType m_orientType;

	bool m_needUpViewMatrix, m_needUpProjMatrix;

}; // class GraphicsModel

} // namespace

} // namespace

#endif // __GRAPHICSCAMERA__