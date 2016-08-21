#ifndef __PHYSICSMATERIALMANAGER__
#define __PHYSICSMATERIALMANAGER__

#include "IPhysicsMaterialManager.h"

namespace FreeWorldEngine {

class PhysicsMaterialManager : public IPhysicsMaterialManager {
public:
	PhysicsMaterialManager(IResourceManager *pResourceManager);
	~PhysicsMaterialManager();
	IPhysicsMaterial *createPhysicsMaterial(const std::string& name);
	IPhysicsMaterial *getByName(const std::string& name) const;
	IResourceManager *resourceManager() const;

private:
	IResourceManager *m_pResourceManager;

};

} // PhysicsMaterialManager

#endif // __PHYSICSMATERIALMANAGER__