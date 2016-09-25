#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include <list>
#include <unordered_map>
#include <map>

#include "IResourceManager.h"

namespace FreeWorldEngine {

/*template <class Container>
class ResourceManager : public IResourceManager {
public:
	ResourceManager(const std::string& resourceManagerName);
	~ResourceManager();

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
	Container<IResource*> m_data;
};*/


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

	class ResourceListIterator : public ResourceIterator::Interface {
	public:
		ResourceListIterator(const std::list<IResource*>::iterator& iterator) : m_iter(iterator) {}
		~ResourceListIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceListIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerList::ResourceListIterator& t = *(static_cast<const ResourceManagerList::ResourceListIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return *m_iter; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }

	private:
		std::list<IResource*>::iterator m_iter;
	};
	class ResourceListReverseIterator : public ResourceIterator::Interface {
	public:
		ResourceListReverseIterator(const std::list<IResource*>::reverse_iterator& iterator) : m_iter(iterator) {}
		~ResourceListReverseIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceListReverseIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerList::ResourceListReverseIterator& t = *(static_cast<const ResourceManagerList::ResourceListReverseIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return *m_iter; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }

	private:
		std::list<IResource*>::reverse_iterator m_iter;
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
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::unordered_map<std::string, IResource*> m_data;
	
	class ResourceHashIterator : public ResourceIterator::Interface {
	public:
		ResourceHashIterator(const std::unordered_map<std::string, IResource*>::iterator& iterator) : m_iter(iterator) {}
		~ResourceHashIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceHashIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerHash::ResourceHashIterator& t = *(static_cast<const ResourceManagerHash::ResourceHashIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return (*m_iter).second; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }
	private:
		std::unordered_map<std::string, IResource*>::iterator m_iter;
	};
	class ResourceHashReverseIterator : public ResourceIterator::Interface {
	public:
		ResourceHashReverseIterator(const std::unordered_map<std::string, IResource*>::reverse_iterator& iterator) : m_iter(iterator) {}
		~ResourceHashReverseIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceHashReverseIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerHash::ResourceHashReverseIterator& t = *(static_cast<const ResourceManagerHash::ResourceHashReverseIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return (*m_iter).second; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }
	private:
		std::unordered_map<std::string, IResource*>::reverse_iterator m_iter;
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
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::map<std::string, IResource*> m_data;

	class ResourceMapIterator : public ResourceIterator::Interface {
	public:
		ResourceMapIterator(const std::map<std::string, IResource*>::iterator& iterator) : m_iter(iterator) {}
		~ResourceMapIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceMapIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerMap::ResourceMapIterator& t = *(static_cast<const ResourceManagerMap::ResourceMapIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return (*m_iter).second; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }
	private:
		std::map<std::string, IResource*>::iterator m_iter;
	};
	class ResourceMapReverseIterator : public ResourceIterator::Interface {
	public:
		ResourceMapReverseIterator(const std::map<std::string, IResource*>::reverse_iterator& iterator) : m_iter(iterator) {}
		~ResourceMapReverseIterator() {}
		ResourceIterator::Interface *clone() const { return new ResourceMapReverseIterator(m_iter); }
		bool operator !=(const ResourceIterator::Interface& other) const {
			const ResourceManagerMap::ResourceMapReverseIterator& t = *(static_cast<const ResourceManagerMap::ResourceMapReverseIterator*>(&other));
			return m_iter != t.m_iter;
		}
		IResource *operator *() { return (*m_iter).second; }
		ResourceIterator::Interface *operator ++() { ++m_iter; return this; }
	private:
		std::map<std::string, IResource*>::reverse_iterator m_iter;
	};
};

} // namespace

#endif // __RESOURCEMANAGER__