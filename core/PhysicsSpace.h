#ifndef __PHYSICSSPACE__
#define __PHYSICSSPACE__

#include "IPhysicsSpace.h"
#include "ode/ode.h"

namespace FreeWorldEngine {

class PhysicsSpace : public IPhysicsSpace {
public:
	PhysicsSpace(const dSpaceID id);
	~PhysicsSpace();
	uint32 type() const;
	uint32 id() const;
	void setPhysicsMaterial(const std::string&) {}
	void setPhysicsMaterial(IPhysicsMaterial*) {}
	IPhysicsMaterial *physicsMaterial() const { return 0; }
	void params(float* pPparams) const {}
	void setParams(float* const pPparams) {}
	void setPosition(const glm::vec3& pos) {}
	glm::vec3 position() const { return glm::vec3(); }
	void setOrientation(const glm::quat& orient) {}
	glm::quat orientation() const { return glm::quat(); }
	void aabb(glm::vec3& resultMin, glm::vec3& resultMax) const;

	void insertPhysicsGeometry(IPhysicsGeometry *pGeometry);
	void removePhysicsGeometry(IPhysicsGeometry *pGeometry);
	bool queryPhysicsGeometry(IPhysicsGeometry *pGeometry) const;
	void setCleanup(const bool state);
	bool isCleanup() const;
	
private:
	dSpaceID m_space;

}; // class PhysicsSpace

} // namespace

#endif // __PHYSICSSPACE__