#ifndef __IPHYSICSMATERIAL__
#define __IPHYSICSMATERIAL__

#include "IResource.h"

namespace FreeWorldEngine {

class IPhysicsMaterial : public IResource {
public:
	virtual ~IPhysicsMaterial() = 0 {}
	virtual float density() const = 0;
	virtual void setDensity(const float density) = 0;
	virtual float frictionCoefficient() const = 0;
	virtual void setFrictionCoefficient(const float coefficient) = 0;
	virtual float restitutionCoefficient() const = 0;
	virtual void setRestitutionCoefficient(const float coefficient) = 0;

}; // class IPhysicsMaterial

} // namespace

#endif // __IPHYSICSMATERIAL__