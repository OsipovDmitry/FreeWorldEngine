#include "LibraryManager.h"
#include "Library.h"
#include "Core.h"

namespace FreeWorldEngine {

LibraryManager::LibraryManager() :
	m_pResourceManager(coreEngine->createResourceManager("ResourceManagerForLibraries", IResourceManager::StorageType_List))
{
}

LibraryManager::~LibraryManager()
{
	while (m_pResourceManager->size())
		unloadLibrary(static_cast<ILibrary*>(*(m_pResourceManager->rbegin())));
	coreEngine->destroyResourceManager(m_pResourceManager);
}
	
ILibrary *LibraryManager::loadLibrary(const std::string& filename)
{
	LOG("Load library \""+filename+"\"");
	ILibrary *pLibrary = findLibrary(filename);
	if (pLibrary) {
		LOG("Library \""+filename+"\" is loaded earlier");
		return pLibrary;
	}
	
	pLibrary = new Library(filename);

	if (!pLibrary->isLoaded()) {
		LOG_ERROR("Loading library \"" + filename + "\" failed");
		delete pLibrary;
		return 0;
	}

	LOG("Loading library \"" + filename + "\" is successful");
	m_pResourceManager->addResource(pLibrary);
	return pLibrary;
}

void LibraryManager::unloadLibrary(const std::string& name)
{
	LOG("Unload library \""+name+"\"");
	m_pResourceManager->destroyResource(name);
}

void LibraryManager::unloadLibrary(ILibrary *pLibrary)
{
	unloadLibrary(pLibrary->name());
}

ILibrary *LibraryManager::findLibrary(const std::string& name) const
{
	return reinterpret_cast<ILibrary*>(m_pResourceManager->findResource(name));
}

} // namespace