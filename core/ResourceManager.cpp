#include "IResource.h"

#include "ResourceManager.h"

namespace FreeWorldEngine {

ResourceManagerList::ResourceManagerList(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data(0)
{
}

ResourceManagerList::~ResourceManagerList()
{
	deleteAllResources();
}

std::string ResourceManagerList::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerList::storageType() const
{
	return IResourceManager::StorageType_List;
}

IResource *ResourceManagerList::getByName(const std::string& name) const
{
	for (std::list<IResource*>::const_iterator it = m_data.cbegin(); it != m_data.cend(); ++it)
		if ((*it)->name() == name)
			return *it;
	return 0;
}

void ResourceManagerList::addResource(IResource *pResource)
{
	const std::string name = pResource->name();
	for (std::list<IResource*>::iterator it = m_data.begin(); it != m_data.end(); ++it) {
		if ((*it)->name() == name) {
			delete *it;
			*it = pResource;
			return;
		}
	}
	m_data.push_back(pResource);
}

void ResourceManagerList::deleteResource(const std::string& name)
{
	for (std::list<IResource*>::iterator it = m_data.begin(); it != m_data.end(); ++it) {
		if ((*it)->name() == name) {
			delete *it;
			m_data.erase(it);
			return;
		}
	}
}

void ResourceManagerList::deleteResource(IResource *pResource)
{
	deleteResource(pResource->name());
}

void ResourceManagerList::deleteAllResources()
{
	for (std::list<IResource*>::iterator it = m_data.begin(); it != m_data.end(); ++it)
		delete *it;
	m_data.clear();
}

std::list<IResource*> ResourceManagerList::listResources() const
{
	return m_data;
}

ResourceManagerHash::ResourceManagerHash(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data(0)
{
}

ResourceManagerHash::~ResourceManagerHash()
{
	deleteAllResources();
}

std::string ResourceManagerHash::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerHash::storageType() const
{
	return IResourceManager::StorageType_Hash;
}

IResource *ResourceManagerHash::getByName(const std::string& name) const
{
	std::unordered_map<std::string, IResource*>::const_iterator i = m_data.find(name);
	return i == m_data.cend() ? 0 : i->second;
}

void ResourceManagerHash::addResource(IResource *pResource)
{
	const std::string name = pResource->name();
	std::unordered_map<std::string, IResource*>::iterator i = m_data.find(name);
	if (i != m_data.end()) {
		delete i->second;
		i->second = pResource;
	}
	else
		m_data[name] = pResource;
}

void ResourceManagerHash::deleteResource(const std::string& name)
{
	std::unordered_map<std::string, IResource*>::const_iterator i = m_data.find(name);
	if (i != m_data.cend()) {
		delete i->second;
		m_data.erase(i);
	}
}

void ResourceManagerHash::deleteResource(IResource *pResource)
{
	deleteResource(pResource->name());
}

void ResourceManagerHash::deleteAllResources()
{
	for (std::unordered_map<std::string, IResource*>::iterator i = m_data.begin(); i != m_data.end(); ++i)
		delete i->second;
	m_data.clear();
}

std::list<IResource*> ResourceManagerHash::listResources() const
{
	std::list<IResource*> listResources(0);
	for (std::unordered_map<std::string, IResource*>::const_iterator it = m_data.cbegin(); it != m_data.cend(); ++it)
		listResources.push_back(it->second);
	return listResources;
}

ResourceManagerMap::ResourceManagerMap(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data()
{
}

ResourceManagerMap::~ResourceManagerMap()
{
	deleteAllResources();
}

std::string ResourceManagerMap::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerMap::storageType() const
{
	return IResourceManager::StorageType_Map;
}

IResource *ResourceManagerMap::getByName(const std::string& name) const
{
	std::map<std::string, IResource*>::const_iterator i = m_data.find(name);
	return i == m_data.cend() ? 0 : i->second;
}

void ResourceManagerMap::addResource(IResource *pResource)
{
	const std::string name = pResource->name();
	std::map<std::string, IResource*>::iterator i = m_data.find(name);
	if (i != m_data.end()) {
		delete i->second;
		i->second = pResource;
	}
	else
		m_data[name] = pResource;
}

void ResourceManagerMap::deleteResource(const std::string& name)
{
	std::map<std::string, IResource*>::const_iterator i = m_data.find(name);
	if (i != m_data.cend()) {
		delete i->second;
		m_data.erase(i);
	}
}

void ResourceManagerMap::deleteResource(IResource *pResource)
{
	deleteResource(pResource->name());
}

void ResourceManagerMap::deleteAllResources()
{
	for (std::map<std::string, IResource*>::iterator i = m_data.begin(); i != m_data.end(); ++i)
		delete i->second;
	m_data.clear();
}

std::list<IResource*> ResourceManagerMap::listResources() const
{
	std::list<IResource*> listResources(0);
	for (std::map<std::string, IResource*>::const_iterator it = m_data.cbegin(); it != m_data.cend(); ++it)
		listResources.push_back(it->second);
	return listResources;
}

} // namespace