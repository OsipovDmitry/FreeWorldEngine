#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include <list>
#include <unordered_map>
#include <map>

#include "IResourceManager.h"

namespace FreeWorldEngine {

class ResourceManagerList : public IResourceManager {
public:
	ResourceManagerList(const std::string& resourceManagerName);
	~ResourceManagerList();

	std::string name() const;
	IResourceManager::StorageType storageType() const;

	IResource *getByName(const std::string& name) const;
	void addResource(IResource *pResource);
	void deleteResource(const std::string& name);
	void deleteResource(IResource *pResource);
	void deleteAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::list<IResource*> m_data;

};

class ResourceManagerHash : public IResourceManager {
public:
	ResourceManagerHash(const std::string& resourceManagerName);
	~ResourceManagerHash();

	std::string name() const;
	IResourceManager::StorageType storageType() const;

	IResource *getByName(const std::string& name) const;
	void addResource(IResource *pResource);
	void deleteResource(const std::string& name);
	void deleteResource(IResource *pResource);
	void deleteAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::unordered_map<std::string, IResource*> m_data;

};

class ResourceManagerMap : public IResourceManager {
public:
	ResourceManagerMap(const std::string& resourceManagerName);
	~ResourceManagerMap();

	std::string name() const;
	IResourceManager::StorageType storageType() const;

	IResource *getByName(const std::string& name) const;
	void addResource(IResource *pResource);
	void deleteResource(const std::string& name);
	void deleteResource(IResource *pResource);
	void deleteAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::map<std::string, IResource*> m_data;

};

} // namespace

#endif // __RESOURCEMANAGER__