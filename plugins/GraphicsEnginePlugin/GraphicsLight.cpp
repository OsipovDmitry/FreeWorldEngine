#include <3rdparty/glm/gtc/constants.hpp>

#include <math/MathUtils.h>

#include "GraphicsLight.h"
#include "GraphicsScene.h"
#include "AbstractSceneOptimizer.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsLight::GraphicsLight(GraphicsScene *pScene, Type type, const std::string& name) :
	m_name(name),
	m_type(type),
	m_pScene(pScene),
	m_pOptimizerData(nullptr),
	m_position(),
	m_direction(),
	m_attRadius(10.0f, 12.0f),
	m_color(0.8f, 0.8f, 0.8f),
	m_spotAngles(glm::quarter_pi<float>(), glm::half_pi<float>()),
	m_cosSpotAngles(glm::cos(0.5f * m_spotAngles))
{
	updatePackedData();
	m_pScene->optimizer()->lightCreated(this);
	m_pScene->optimizer()->lightTransformationChanged(this);
}

GraphicsLight::~GraphicsLight()
{
	m_pScene->optimizer()->lightDestroyed(this);
}

std::string GraphicsLight::name() const
{
	return m_name;
}

IGraphicsScene *GraphicsLight::scene() const
{
	return sceneImpl();
}

IGraphicsLight::Type GraphicsLight::type() const
{
	return m_type;
}

glm::vec3 GraphicsLight::position() const
{
	return m_position;
}

void GraphicsLight::setPosition(const glm::vec3& value)
{
	m_position = value;

	m_packedData[0][0] = m_position.x;
	m_packedData[0][1] = m_position.y;
	m_packedData[0][2] = m_position.z;

	m_pScene->optimizer()->lightTransformationChanged(this);
}

glm::vec3 GraphicsLight::direction() const
{
	return m_direction;
}

void GraphicsLight::setDirection(const glm::vec3& value)
{
	m_direction = glm::normalize(value);

	m_packedData[1][0] = m_direction.x;
	m_packedData[1][1] = m_direction.y;
	m_packedData[1][2] = m_direction.z;

	m_pScene->optimizer()->lightTransformationChanged(this);
}

glm::vec2 GraphicsLight::attenuationRadius() const
{
	return m_attRadius;
}

void GraphicsLight::setAttenuationRadius(const float innerRadius, const float outerRadius)
{
	m_attRadius.x = innerRadius;
	m_attRadius.y = outerRadius;

	m_packedData[2][0] = m_attRadius.x;
	m_packedData[2][1] = m_attRadius.y;

	m_pScene->optimizer()->lightTransformationChanged(this);
}

glm::vec3 GraphicsLight::color() const
{
	return m_color;
}

void GraphicsLight::setColor(const glm::vec3& value)
{
	m_color = value;

	m_packedData[3][0] = m_color.x;
	m_packedData[3][1] = m_color.y;
	m_packedData[3][2] = m_color.z;
}

glm::vec2 GraphicsLight::spotAngles() const
{
	return m_spotAngles;
}

void GraphicsLight::setSpotAngles(const float innerAngle, const float outerAngle)
{
	m_spotAngles.x = innerAngle;
	m_spotAngles.y = outerAngle;
	m_cosSpotAngles = glm::cos(0.5f * m_spotAngles);

	m_packedData[2][2] = m_cosSpotAngles.x;
	m_packedData[2][3] = m_cosSpotAngles.y;

	m_pScene->optimizer()->lightTransformationChanged(this);
}

GraphicsScene *GraphicsLight::sceneImpl() const
{
	return m_pScene;
}

float GraphicsLight::powerOfLighting(float distToLight, float cosAngle) const
{
	static const float s_distanceAttenuationLightType[IGraphicsLight::Type_Count] = { 0.0, 0.0, 1.0, 1.0 };
	static const float s_spotAttenuationLightType[IGraphicsLight::Type_Count] = { 0.0, 0.0, 0.0, 1.0 };

	float distAtt = (distToLight - m_attRadius.x) / (m_attRadius.y - m_attRadius.x);
	distAtt = glm::clamp(distAtt, 0.0f, 1.0f);
	distAtt = distAtt * distAtt;
	distAtt = distAtt * s_distanceAttenuationLightType[m_type];
	distAtt = 1.0 - distAtt;

	float spotAtt = (cosAngle - m_cosSpotAngles.y) / (m_cosSpotAngles.x - m_cosSpotAngles.y);
	spotAtt = 1.0 - glm::clamp(spotAtt, 0.0f, 1.0f);
	spotAtt = spotAtt * spotAtt;
	spotAtt = spotAtt * s_spotAttenuationLightType[m_type];
	spotAtt = 1.0 - spotAtt;

	return distAtt * spotAtt;
}

float GraphicsLight::powerOfLighting(const glm::vec3& v) const
{
	glm::vec3 lightToV = v - m_position;
	float d = glm::length(lightToV);
	if (d > Math::eps)
		lightToV /= d;
	return powerOfLighting(d, glm::dot(m_direction, lightToV));
}

bool GraphicsLight::isBoundingBoxLighted(const Math::Aabb& box) const
{
	typedef bool (*LightingOfBoundingBox)(const GraphicsLight*, const Math::Aabb&);
	static LightingOfBoundingBox funcs[IGraphicsLight::Type_Count] = {
		nullptr,
		directionalLightingOfBoundingBox,
		pointLightingOfBoundingBox,
		spotLightingOfBoundingBox
	};

	return funcs[m_type](this, box);
}

const glm::mat4x4&GraphicsLight::packedData() const
{
	return m_packedData;
}

void GraphicsLight::updatePackedData()
{
	m_packedData[0][0] = m_position.x;
	m_packedData[0][1] = m_position.y;
	m_packedData[0][2] = m_position.z;
	m_packedData[0][3];

	m_packedData[1][0] = m_direction.x;
	m_packedData[1][1] = m_direction.y;
	m_packedData[1][2] = m_direction.z;
	m_packedData[1][3];

	m_packedData[2][0] = m_attRadius.x;
	m_packedData[2][1] = m_attRadius.y;

	m_packedData[2][2] = m_cosSpotAngles.x;
	m_packedData[2][3] = m_cosSpotAngles.y;

	m_packedData[3][0] = m_color.x;
	m_packedData[3][1] = m_color.y;
	m_packedData[3][2] = m_color.z;

	m_packedData[3][3] = (float)m_type;
}

void GraphicsLight::setOptimizerData(void *pData)
{
	m_pOptimizerData = pData;
}

void *GraphicsLight::optimizerData() const
{
	return m_pOptimizerData;
}

bool GraphicsLight::directionalLightingOfBoundingBox(const GraphicsLight* pLight, const Math::Aabb& box)
{
	return true;
}

bool GraphicsLight::pointLightingOfBoundingBox(const GraphicsLight* pLight, const Math::Aabb& box)
{
	return Math::distToBoundingBox(box, pLight->position()) < pLight->attenuationRadius()[1];
}

bool GraphicsLight::spotLightingOfBoundingBox(const GraphicsLight* pLight, const Math::Aabb& box)
{
	return Math::distToBoundingBox(box, pLight->position()) < pLight->attenuationRadius()[1];
	// todo: доделать зависимость от углов
}

const glm::mat4x4& GraphicsLight::noneLightPackedData()
{
	static const glm::mat4x4 noneLight(
				glm::vec4(),
				glm::vec4(),
				glm::vec4(),
				glm::vec4(0.0f, 0.0f, 0.0, (float)IGraphicsLight::Type_None)
				);
	return noneLight;
}

} // namespace
} // namespace
