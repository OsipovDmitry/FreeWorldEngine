#ifndef __PHYSICSBODY__
#define __PHYSICSBODY__

#include "IPhysicsBody.h"
#include "ode/ode.h"

namespace FreeWorldEngine {

class PhysicsBody : public IPhysicsBody {
public:
	PhysicsBody(dBodyID id);
	~PhysicsBody();
	uint32 id() const;

	void setPosition(const glm::vec3& pos);
	glm::vec3 position() const;
	void setOrientation(const glm::quat& orient);
	glm::quat orientation() const;
	void setLinearVelocity(const glm::vec3& vel);
	glm::vec3 linearVelocity() const;
	void setAngularVelocity(const glm::vec3& vel);
	glm::vec3 angularVelocity() const;
	
	void addForce(const glm::vec3 force);
	void addTorque(const glm::vec3 torque);
	void addRelativeForce(const glm::vec3 force);
	void addRelativeTorque(const glm::vec3 torque);
	void addForceAtPosition(const glm::vec3 force, const glm::vec3& pos);
	void addForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos);
	void addRelativeForceAtPosition(const glm::vec3 force, const glm::vec3& pos);
	void addRelativeForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos);
	void setForce(const glm::vec3 force);
	void setTorque(const glm::vec3 force);
	glm::vec3 force() const;
	glm::vec3 torque() const;

	void enable();
	void disable();
	bool isEnabled() const;
	void setAutoDisableFlag(const bool flag);
	bool autoDisableFlag() const;
	void setAutoDisableLinearThreshold(const float threshold);
	float autoDisableLinearThreshold() const;
	void setAutoDisableAngularThreshold(const float threshold);
	float autoDisableAngularThreshold() const;
	void setAutoDisableSteps(const uint32 numSteps);
	uint32 autoDisableSteps() const;
	void setAutoDisableTime(const float time);
	float autoDisableTime() const;
	void setAutoDisableDefaults();

	void setGravityMode(const bool mode);
	bool gravityMode() const;

	void setKinematicState(const bool state);
	bool kinematicState() const;

	void addGeometry(IPhysicsGeometry *pGeom);
	void removeGeometry(IPhysicsGeometry *pGeom);
	void removeAllGeometries();
	const std::set<IPhysicsGeometry*>& geometriesList();

private:
	dBodyID m_body;
	std::set<IPhysicsGeometry*> m_geometries;

	void recalcMass();

}; // class PhysicsBody


} // namespace

#endif // __PHYSICSBODY__