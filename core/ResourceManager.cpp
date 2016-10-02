#include "IResource.h"

#include "ResourceManager.h"

namespace FreeWorldEngine {

class ResourceIteratorPrivate {
public:
	virtual ~ResourceIteratorPrivate() = 0 {}

	virtual ResourceIteratorPrivate *clone() const = 0;

	virtual bool operator !=(const ResourceIteratorPrivate *other) const = 0;
	virtual IResource *operator *() = 0;
	virtual void operator ++() = 0;
	virtual void operator --() = 0;
};

ResourceIterator::ResourceIterator(ResourceIteratorPrivate *pPrivate) :
	m(pPrivate)
{
}

ResourceIterator::~ResourceIterator()
{
	delete m;
}

ResourceIterator::ResourceIterator(const ResourceIterator& other) :
	m(other.m->clone())
{
}

ResourceIterator& ResourceIterator::operator =(const ResourceIterator& other)
{
	m = other.m->clone();
	return *this;
}

bool ResourceIterator::operator !=(const ResourceIterator& other) const
{
	return m->operator!=(other.m);
}

bool ResourceIterator::operator ==(const ResourceIterator& other) const
{
	return !(m->operator!=(other.m));
}

IResource *ResourceIterator::operator *()
{
	return m->operator*();
}

ResourceIterator& ResourceIterator::operator ++()
{
	m->operator++();
	return *this;
}

ResourceIterator& ResourceIterator::operator ++(int)
{
	ResourceIterator save = *this;
	m->operator++();
	return save;
}

ResourceIterator& ResourceIterator::operator --()
{
	m->operator--();
	return *this;
}

ResourceIterator& ResourceIterator::operator --(int)
{
	ResourceIterator save = *this;
	m->operator--();
	return save;
}

class ResourceListIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceListIteratorPrivate(const std::list<IResource*>::iterator& iter) : m_iter(iter) {}
	~ResourceListIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceListIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceListIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return *m_iter; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::list<IResource*>::iterator m_iter;
};

class ResourceListReverseIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceListReverseIteratorPrivate(const std::list<IResource*>::reverse_iterator& iter) : m_iter(iter) {}
	~ResourceListReverseIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceListReverseIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceListReverseIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return *m_iter; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::list<IResource*>::reverse_iterator m_iter;
};

ResourceManagerList::ResourceManagerList(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data(0)
{
}

ResourceManagerList::~ResourceManagerList()
{
	destroyAllResources();
}

std::string ResourceManagerList::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerList::storageType() const
{
	return IResourceManager::StorageType_List;
}

IResource *ResourceManagerList::findResource(const std::string& name) const
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

void ResourceManagerList::destroyResource(const std::string& name)
{
	for (std::list<IResource*>::iterator it = m_data.begin(); it != m_data.end(); ++it) {
		if ((*it)->name() == name) {
			delete *it;
			m_data.erase(it);
			return;
		}
	}
}

void ResourceManagerList::destroyResource(IResource *pResource)
{
	destroyResource(pResource->name());
}

void ResourceManagerList::destroyAllResources()
{
	for (std::list<IResource*>::iterator it = m_data.begin(); it != m_data.end(); ++it)
		delete *it;
	m_data.clear();
}

uint32 ResourceManagerList::size() const
{
	return m_data.size();
}

ResourceIterator ResourceManagerList::begin()
{
	return ResourceIterator(new ResourceListIteratorPrivate(m_data.begin()));
}

ResourceIterator ResourceManagerList::end()
{
	return ResourceIterator(new ResourceListIteratorPrivate(m_data.end()));
}

ResourceIterator ResourceManagerList::rbegin()
{
	return ResourceIterator(new ResourceListReverseIteratorPrivate(m_data.rbegin()));
}

ResourceIterator ResourceManagerList::rend()
{
	return ResourceIterator(new ResourceListReverseIteratorPrivate(m_data.rend()));
}

class ResourceHashIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceHashIteratorPrivate(const std::unordered_map<std::string, IResource*>::iterator& iter) : m_iter(iter) {}
	~ResourceHashIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceHashIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceHashIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return m_iter->second; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::unordered_map<std::string, IResource*>::iterator m_iter;
};

class ResourceHashReverseIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceHashReverseIteratorPrivate(const std::unordered_map<std::string, IResource*>::reverse_iterator& iter) : m_iter(iter) {}
	~ResourceHashReverseIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceHashReverseIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceHashReverseIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return m_iter->second; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::unordered_map<std::string, IResource*>::reverse_iterator m_iter;
};

ResourceManagerHash::ResourceManagerHash(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data(0)
{
}

ResourceManagerHash::~ResourceManagerHash()
{
	destroyAllResources();
}

std::string ResourceManagerHash::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerHash::storageType() const
{
	return IResourceManager::StorageType_Hash;
}

IResource *ResourceManagerHash::findResource(const std::string& name) const
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

void ResourceManagerHash::destroyResource(const std::string& name)
{
	std::unordered_map<std::string, IResource*>::const_iterator i = m_data.find(name);
	if (i != m_data.cend()) {
		delete i->second;
		m_data.erase(i);
	}
}

void ResourceManagerHash::destroyResource(IResource *pResource)
{
	destroyResource(pResource->name());
}

void ResourceManagerHash::destroyAllResources()
{
	for (std::unordered_map<std::string, IResource*>::iterator i = m_data.begin(); i != m_data.end(); ++i)
		delete i->second;
	m_data.clear();
}

uint32 ResourceManagerHash::size() const
{
	return m_data.size();
}

ResourceIterator ResourceManagerHash::begin()
{
	return ResourceIterator(new ResourceHashIteratorPrivate(m_data.begin()));
}

ResourceIterator ResourceManagerHash::end()
{
	return ResourceIterator(new ResourceHashIteratorPrivate(m_data.end()));
}

ResourceIterator ResourceManagerHash::rbegin()
{
	return ResourceIterator(new ResourceHashReverseIteratorPrivate(m_data.rbegin()));
}

ResourceIterator ResourceManagerHash::rend()
{
	return ResourceIterator(new ResourceHashReverseIteratorPrivate(m_data.rend()));
}

class ResourceMapIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceMapIteratorPrivate(const std::map<std::string, IResource*>::iterator& iter) : m_iter(iter) {}
	~ResourceMapIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceMapIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceMapIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return m_iter->second; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::map<std::string, IResource*>::iterator m_iter;
};

class ResourceMapReverseIteratorPrivate : public ResourceIteratorPrivate {
public:
	ResourceMapReverseIteratorPrivate(const std::map<std::string, IResource*>::reverse_iterator& iter) : m_iter(iter) {}
	~ResourceMapReverseIteratorPrivate() {}
	ResourceIteratorPrivate *clone() const { return new ResourceMapReverseIteratorPrivate(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceMapReverseIteratorPrivate*>(other)->m_iter; }
	IResource *operator *() { return m_iter->second; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	std::map<std::string, IResource*>::reverse_iterator m_iter;
};

ResourceManagerMap::ResourceManagerMap(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data()
{
}

ResourceManagerMap::~ResourceManagerMap()
{
	destroyAllResources();
}

std::string ResourceManagerMap::name() const
{
	return m_name;
}

IResourceManager::StorageType ResourceManagerMap::storageType() const
{
	return IResourceManager::StorageType_Map;
}

IResource *ResourceManagerMap::findResource(const std::string& name) const
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

void ResourceManagerMap::destroyResource(const std::string& name)
{
	std::map<std::string, IResource*>::const_iterator i = m_data.find(name);
	if (i != m_data.cend()) {
		delete i->second;
		m_data.erase(i);
	}
}

void ResourceManagerMap::destroyResource(IResource *pResource)
{
	destroyResource(pResource->name());
}

void ResourceManagerMap::destroyAllResources()
{
	for (std::map<std::string, IResource*>::iterator i = m_data.begin(); i != m_data.end(); ++i)
		delete i->second;
	m_data.clear();
}

uint32 ResourceManagerMap::size() const
{
	return m_data.size();
}

ResourceIterator ResourceManagerMap::begin()
{
	return ResourceIterator(new ResourceMapIteratorPrivate(m_data.begin()));
}

ResourceIterator ResourceManagerMap::end()
{
	return ResourceIterator(new ResourceMapIteratorPrivate(m_data.end()));
}

ResourceIterator ResourceManagerMap::rbegin()
{
	return ResourceIterator(new ResourceMapReverseIteratorPrivate(m_data.rbegin()));
}

ResourceIterator ResourceManagerMap::rend()
{
	return ResourceIterator(new ResourceMapReverseIteratorPrivate(m_data.rend()));
}
/*
template<class ContainerType>
DataManager<ContainerType>::DataManager(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data(ContainerType())
{
}

template<class ContainerType>
DataManager<ContainerType>::~DataManager()
{
}

template<class ContainerType>
IDataManager<>::StorageType DataManager<ContainerType>::storageType() const
{
	return StorageType(0);
}

template<class ContainerType>
typename ContainerType::value_type *DataManager<ContainerType>::findResource(const std::string& name) const
{
	return 0;
}

template<class ContainerType>
void DataManager<ContainerType>::addResource(DataType *pResource)
{
}

template<class ContainerType>
void DataManager<ContainerType>::destroyResource(const std::string& name)
{
}

template<class ContainerType>
void DataManager<ContainerType>::destroyResource(DataType *pResource)
{
}

template<class ContainerType>
void DataManager<ContainerType>::destroyAllResources()
{
}

template<class ContainerType>
uint32 DataManager<ContainerType>::size() const
{
	return m_data.size();
}

template<class ContainerType>
ResourceIterator DataManager<ContainerType>::begin()
{
	return ResourceIterator(0);
}

template<class ContainerType>
ResourceIterator DataManager<ContainerType>::end()
{
	return ResourceIterator(0);
}

template<class ContainerType>
ResourceIterator DataManager<ContainerType>::rbegin()
{
	return ResourceIterator(0);
}

template<class ContainerType>
ResourceIterator DataManager<ContainerType>::rend()
{
	return ResourceIterator(0);
}
*/
} // namespace