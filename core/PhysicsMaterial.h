#ifndef __PHYSICSMATERIAL__
#define __PHYSICSMATERIAL__

#include "IPhysicsMaterial.h"

namespace FreeWorldEngine {

class PhysicsMaterial : public IPhysicsMaterial {
public:
	PhysicsMaterial(const std::string& name);
	~PhysicsMaterial();
	std::string name() const;
	float density() const;
	void setDensity(const float density);
	float frictionCoefficient() const;
	void setFrictionCoefficient(const float coefficient);
	float restitutionCoefficient() const;
	void setRestitutionCoefficient(const float coefficient);

private:
	std::string m_name;
	float m_density;
	float m_friction;
	float m_restitution;

}; // class PhysicsMaterial

} // namespace

#endif // __PHYSICSMATERIAL__