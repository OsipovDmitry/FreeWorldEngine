#ifndef __GRAPHICSLIGHT__
#define __GRAPHICSLIGHT__

#include <list>

#include <3rdparty/glm/mat4x4.hpp>

#include <graphics_engine/IGraphicsLight.h>

namespace FreeWorldEngine {

namespace Math {
	struct Aabb;
}

namespace GraphicsEngine {

class GraphicsScene;
class GraphicsSceneNode;

class GraphicsLight : public IGraphicsLight {
public:

	GraphicsLight(GraphicsScene *pScene, Type type, const std::string& name);
	~GraphicsLight();

	std::string name() const;

	IGraphicsScene *scene() const ;

	Type type() const;

	glm::vec3 position() const;
	void setPosition(const glm::vec3& value);

	glm::vec3 direction() const;
	void setDirection(const glm::vec3& value);

	glm::vec2 attenuationRadius() const;
	void setAttenuationRadius(const float innerRadius, const float outerRadius);

	glm::vec3 color() const;
	void setColor(const glm::vec3& value);

	glm::vec2 spotAngles() const;
	void setSpotAngles(const float innerAngle, const float outerAngle);

	void setOptimizerData(void *pData);
	void *optimizerData() const;

	GraphicsScene *sceneImpl() const;
	float powerOfLighting(float distToLight, float cosAngle) const;
	float powerOfLighting(const glm::vec3& v) const;
	bool isBoundingBoxLighted(const Math::Aabb& box) const;
	const glm::mat4x4& packedData() const;

	void updatePackedData();

	static bool directionalLightingOfBoundingBox(const GraphicsLight *pLight, const Math::Aabb& box);
	static bool pointLightingOfBoundingBox(const GraphicsLight *pLight, const Math::Aabb& box);
	static bool spotLightingOfBoundingBox(const GraphicsLight *pLight, const Math::Aabb& box);

	static const glm::mat4x4& noneLightPackedData();

private:
	glm::mat4x4 m_packedData;
	std::string m_name;
	GraphicsScene *m_pScene;
	void *m_pOptimizerData;
	glm::vec3 m_position, m_direction, m_color;
	glm::vec2 m_spotAngles, m_cosSpotAngles, m_attRadius;
	Type m_type;

}; // class GraphicsLight

} // namespace
} // namespace

#endif // __GRAPHICSLIGHT__
