#ifndef __IGRAPHICSLIGHT__
#define __IGRAPHICSLIGHT__

#include <3rdparty/glm/vec2.hpp>
#include <3rdparty/glm/vec3.hpp>

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsScene;

class IGraphicsLight : public IGraphicsResource {
public:
	virtual ~IGraphicsLight() {}

	virtual IGraphicsScene *scene() const = 0;

	enum Type {
		Type_None,
		Type_Diectional,
		Type_Point,
		Type_Spot,
		Type_Count
	};

	virtual Type type() const = 0;

	virtual glm::vec3 position() const = 0;
	virtual void setPosition(const glm::vec3& value) = 0;

	virtual glm::vec3 direction() const = 0;
	virtual void setDirection(const glm::vec3& value) = 0;

	virtual glm::vec2 attenuationRadius() const = 0;
	virtual void setAttenuationRadius(const float innerRadius, const float outerRadius) = 0;

	virtual glm::vec3 color() const = 0;
	virtual void setColor(const glm::vec3& value) = 0;

	virtual glm::vec2 spotAngles() const = 0;
	virtual void setSpotAngles(const float innerAngle, const float outerAngle) = 0;

}; // class IGraphicsLight

} // namespace

} // namespace

#endif // __IGRAPHICSLIGHT__
