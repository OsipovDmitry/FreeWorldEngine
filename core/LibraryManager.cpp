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
	std::list<IResource*> librariesList = m_pResourceManager->listResources();
	for (std::list<IResource*>::iterator it = librariesList.begin(); it != librariesList.end(); ++it)
		unloadLibrary((*it)->name());
	m_pResourceManager->deleteAllResources();
	coreEngine->destroyResourceManager(m_pResourceManager);
}
	
ILibrary *LibraryManager::loadLibrary(const std::string& filename)
{
	LOG("Load library \""+filename+"\"");
	ILibrary *pLibrary = getByName(filename);
	if (pLibrary) {
		LOG("Library \""+filename+"\" is loaded earlier.");
		return pLibrary;
	}
	
	pLibrary = new Library(filename);

	if (!pLibrary->isLoaded()) {
		LOG("Loading library \"" + filename + "\" failed.");
		delete pLibrary;
		return 0;
	}

	LOG("Loading library \"" + filename + "\" is successful.");
	m_pResourceManager->addResource(pLibrary);
	return pLibrary;
}

void LibraryManager::unloadLibrary(const std::string& name)
{
	LOG("Unload library \""+name+"\"");
	m_pResourceManager->deleteResource(name);
}

void LibraryManager::unloadLibrary(ILibrary *pLibrary)
{
	LOG("Unload library \""+pLibrary->name()+"\"");
	m_pResourceManager->deleteResource(pLibrary);
}

ILibrary *LibraryManager::getByName(const std::string& name) const
{
	return reinterpret_cast<ILibrary*>(m_pResourceManager->getByName(name));
}

} // namespace