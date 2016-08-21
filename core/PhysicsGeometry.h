#ifndef __PHYSICSGEOMETRY__
#define __PHYSICSGEOMETRY__

#include "IPhysicsGeometry.h"
#include "ode/ode.h"

namespace FreeWorldEngine {

class PhysicsGeometry : public IPhysicsGeometry {
public:
	PhysicsGeometry(dGeomID id);
	~PhysicsGeometry();
	uint32 type() const;
	uint32 id() const;
	void setPhysicsMaterial(const std::string& name);
	void setPhysicsMaterial(IPhysicsMaterial *pPhysicsMaterial);
	IPhysicsMaterial *physicsMaterial() const;
	void params(float* pParams) const;
	void setParams(float* const pParams);
	void setPosition(const glm::vec3& pos);
	glm::vec3 position() const;
	void setOrientation(const glm::quat& orient);
	glm::quat orientation() const;
	void aabb(glm::vec3& resultMin, glm::vec3& resultMax) const;

private:
	dGeomID m_geom;
	IPhysicsMaterial *m_pPhysicsMaterial;

}; // class PhysicsGeometry

};

#endif // __PHYSICSGEOMETRY__