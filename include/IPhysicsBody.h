#ifndef __IPHYSICSBODY__
#define __IPHYSICSBODY__

#include "Types.h"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include <set>

namespace FreeWorldEngine {

class IPhysicsGeometry;

class IPhysicsBody {
public:
	virtual ~IPhysicsBody() = 0 {}
	virtual uint32 id() const = 0;

	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::vec3 position() const = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual glm::quat orientation() const = 0;
	virtual void setLinearVelocity(const glm::vec3& vel) = 0;
	virtual glm::vec3 linearVelocity() const = 0;
	virtual void setAngularVelocity(const glm::vec3& vel) = 0;
	virtual glm::vec3 angularVelocity() const = 0;
	
	virtual void addForce(const glm::vec3 force) = 0;
	virtual void addTorque(const glm::vec3 torque) = 0;
	virtual void addRelativeForce(const glm::vec3 force) = 0;
	virtual void addRelativeTorque(const glm::vec3 torque) = 0;
	virtual void addForceAtPosition(const glm::vec3 force, const glm::vec3& pos) = 0;
	virtual void addForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos) = 0;
	virtual void addRelativeForceAtPosition(const glm::vec3 force, const glm::vec3& pos) = 0;
	virtual void addRelativeForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos) = 0;
	virtual void setForce(const glm::vec3 force) = 0;
	virtual void setTorque(const glm::vec3 force) = 0;
	virtual glm::vec3 force() const = 0;
	virtual glm::vec3 torque() const = 0;

	virtual void enable() = 0;
	virtual void disable() = 0;
	virtual bool isEnabled() const = 0;
	virtual void setAutoDisableFlag(const bool flag) = 0;
	virtual bool autoDisableFlag() const = 0;
	virtual void setAutoDisableLinearThreshold(const float threshold) = 0;
	virtual float autoDisableLinearThreshold() const = 0;
	virtual void setAutoDisableAngularThreshold(const float threshold) = 0;
	virtual float autoDisableAngularThreshold() const = 0;
	virtual void setAutoDisableSteps(const uint32 numSteps) = 0;
	virtual uint32 autoDisableSteps() const = 0;
	virtual void setAutoDisableTime(const float time) = 0;
	virtual float autoDisableTime() const = 0;
	virtual void setAutoDisableDefaults() = 0;

	virtual void setGravityMode(const bool mode) = 0;
	virtual bool gravityMode() const = 0;

	virtual void setKinematicState(const bool state) = 0;
	virtual bool kinematicState() const = 0;

	virtual void addGeometry(IPhysicsGeometry *pGeom) = 0;
	virtual void removeGeometry(IPhysicsGeometry *pGeom) = 0;
	virtual void removeAllGeometries() = 0;
	virtual const std::set<IPhysicsGeometry*>& geometriesList() = 0;

}; // class IPhysicsBody


} // namespace

#endif // __IPHYSICSBODY__