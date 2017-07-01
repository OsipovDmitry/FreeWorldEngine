#ifndef __IPHYSICSMATERIALMANAGER__
#define __IPHYSICSMATERIALMANAGER__

#include <string>

namespace FreeWorldEngine {

class IPhysicsMaterial;
class IResourceManager;

class IPhysicsMaterialManager {
public:
	virtual ~IPhysicsMaterialManager() = 0 {}
	virtual IPhysicsMaterial *createPhysicsMaterial(const std::string& name) = 0;
	virtual IPhysicsMaterial *getByName(const std::string& name) const = 0;
	virtual IResourceManager *resourceManager() const = 0;

}; // class IPhysicsMaterialManager

} // namespace

#endif // __IPHYSICSMATERIALMANAGER__