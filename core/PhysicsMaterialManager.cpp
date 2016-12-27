#include "IResourceManager.h"

#include "PhysicsMaterial.h"
#include "PhysicsMaterialManager.h"

namespace FreeWorldEngine {

PhysicsMaterialManager::PhysicsMaterialManager(IResourceManager *pResourceManager) :
	m_pResourceManager(pResourceManager)
{
	if (!pResourceManager->getByName("StandardPhysicsMaterial"))
		createPhysicsMaterial("StandardPhysicsMaterial");
}

PhysicsMaterialManager::~PhysicsMaterialManager()
{
}

IPhysicsMaterial *PhysicsMaterialManager::createPhysicsMaterial(const std::string& name)
{
	IPhysicsMaterial *pPhysicsMaterial = getByName(name);
	if (pPhysicsMaterial)
		return pPhysicsMaterial;

	pPhysicsMaterial = new PhysicsMaterial(name);
	m_pResourceManager->addResource(pPhysicsMaterial);
	return pPhysicsMaterial;
}

IPhysicsMaterial *PhysicsMaterialManager::getByName(const std::string& name) const
{
	return reinterpret_cast<IPhysicsMaterial*>(m_pResourceManager->getByName(name));
}

IResourceManager *PhysicsMaterialManager::resourceManager() const
{
	return m_pResourceManager;
}


}; // namespace