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

	ResourceIterator begin();
	ResourceIterator end();

private:
	std::string m_name;
	std::list<IResource*> m_data;

	class ResourceListIterator : public ResourceIterator::Interface {
	public:
		ResourceListIterator(const std::list<IResource*>::iterator& iterator);
		~ResourceListIterator();
		ResourceIterator::Interface *clone() const;
		IResource *operator *();

	private:
		std::list<IResource*>::iterator m_iter;
	};
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

	ResourceIterator begin();
	ResourceIterator end();

private:
	std::string m_name;
	std::unordered_map<std::string, IResource*> m_data;
	
	class ResourceHashIterator : public ResourceIterator::Interface {
	public:
		ResourceHashIterator(const std::unordered_map<std::string, IResource*>::iterator& iterator);
		~ResourceHashIterator();
		ResourceIterator::Interface *clone() const;
		IResource *operator *();

	private:
		std::unordered_map<std::string, IResource*>::iterator m_iter;
	};
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

	ResourceIterator begin();
	ResourceIterator end();

private:
	std::string m_name;
	std::map<std::string, IResource*> m_data;

	class ResourceMapIterator : public ResourceIterator::Interface {
	public:
		ResourceMapIterator(const std::map<std::string, IResource*>::iterator& iterator);
		~ResourceMapIterator();
		ResourceIterator::Interface *clone() const;
		IResource *operator *();

	private:
		std::map<std::string, IResource*>::iterator m_iter;
	};
};

} // namespace

#endif // __RESOURCEMANAGER__