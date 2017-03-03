#include <algorithm>
#include <functional>

#include "IResource.h"

#include "ResourceManager.h"

namespace FreeWorldEngine {

namespace {
	typedef std::list<IResource*> list;
	typedef std::unordered_map<std::string, IResource*> hash;
	typedef std::map<std::string, IResource*> map;
}

class ResourceIteratorPrivate {
public:
	virtual ~ResourceIteratorPrivate() = 0 {}

	virtual ResourceIteratorPrivate *clone() const = 0;

	virtual bool operator !=(const ResourceIteratorPrivate *other) const = 0;
	virtual IResource *operator *() = 0;
	virtual void operator ++() = 0;
	virtual void operator --() = 0;
};

template <class Iter>
class ResourceIteratorPrivateImpl : public ResourceIteratorPrivate {
public:
	ResourceIteratorPrivateImpl(const Iter& iter) : m_iter(iter) {}
	~ResourceIteratorPrivateImpl() {}
	ResourceIteratorPrivate *clone() const { return new ResourceIteratorPrivateImpl<Iter>(m_iter); }
	bool operator !=(const ResourceIteratorPrivate *other) const { return m_iter != static_cast<const ResourceIteratorPrivateImpl<Iter>*>(other)->m_iter; }
	IResource *operator *() { return m_iter->second; }
	void operator ++() { ++m_iter; }
	void operator --() { --m_iter; }
private:
	Iter m_iter;
};

template <>
IResource *ResourceIteratorPrivateImpl<std::list<IResource*>::iterator>::operator *() { return *m_iter; }

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
	delete m;
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

template <class Container>
ResourceManager<Container>::ResourceManager(const std::string& resourceManagerName) :
	m_name(resourceManagerName),
	m_data()
{
}

template <class Container>
ResourceManager<Container>::~ResourceManager()
{
	destroyAllResources();
}

template <class Container>
std::string ResourceManager<Container>::name() const
{
	return m_name;
}

template <class Container>
IResourceManager::StorageType ResourceManager<Container>::storageType() const
{
	return (IResourceManager::StorageType)(-1);
}

template <class Container>
IResource *ResourceManager<Container>::findResource(const std::string& name) const
{
	auto i = m_data.find(name);
	return i == m_data.cend() ? 0 : i->second;
}

template <class Container>
void ResourceManager<Container>::addResource(IResource *pResource)
{
	const std::string name = pResource->name();
	auto i = m_data.find(name);
	if (i != m_data.end()) {
		delete i->second;
		i->second = pResource;
	}
	else
		m_data[name] = pResource;
}

template <class Container>
void ResourceManager<Container>::destroyResource(const std::string& name)
{
	auto i = m_data.find(name);
	if (i != m_data.end()) {
		delete i->second;
		m_data.erase(i);
	}
}

template <class Container>
void ResourceManager<Container>::destroyResource(IResource *pResource)
{
	destroyResource(pResource->name());
}

template <class Container>
void ResourceManager<Container>::destroyAllResources()
{
	std::for_each(m_data.begin(), m_data.end(), [](const std::pair<const std::string, IResource*>& elem) { delete elem.second; });
	m_data.clear();
}

template <class Container>
uint32 ResourceManager<Container>::size() const
{
	return m_data.size();
}

template <class Container>
ResourceIterator ResourceManager<Container>::begin()
{
	return ResourceIterator(new ResourceIteratorPrivateImpl<typename Container::iterator>(m_data.begin()));
}

template <class Container>
ResourceIterator ResourceManager<Container>::end()
{
	return ResourceIterator(new ResourceIteratorPrivateImpl<typename Container::iterator>(m_data.end()));
}

IResourceManager *createResourceManager(const std::string & resourceManagerName, const IResourceManager::StorageType storageType)
{
	IResourceManager *pResourceManager = 0;
	switch (storageType) {
	case IResourceManager::StorageType_List: {
		pResourceManager = new ResourceManager<typename list>(resourceManagerName);
		break;
	}
	case IResourceManager::StorageType_Hash: {
		pResourceManager = new ResourceManager<typename hash>(resourceManagerName);
		break;
	}
	case IResourceManager::StorageType_Map: {
		pResourceManager = new ResourceManager<typename map>(resourceManagerName);
		break;
	}
	default:
		break;
	}
	return pResourceManager;
}

// Частичная специализация методов для list реализации
template <>
IResourceManager::StorageType ResourceManager<typename list>::storageType() const
{
	return IResourceManager::StorageType_List;
}

template <>
IResource *ResourceManager<typename list>::findResource(const std::string& name) const
{
	auto it = std::find_if(m_data.begin(), m_data.end(), [&name](IResource *p) { return p->name() == name; });
	return (it != m_data.end()) ? *it : 0;
}

template <>
void ResourceManager<typename list>::addResource(IResource *pResource)
{
	const std::string name = pResource->name();
	auto it = std::find_if(m_data.begin(), m_data.end(), [&name](IResource *p) { return p->name() == name; });
	if (it != m_data.end()) {
		delete *it;
		*it = pResource;
		return;
	}
	m_data.push_back(pResource);
}

template <>
void ResourceManager<typename list>::destroyResource(const std::string& name)
{
	auto it = std::find_if(m_data.begin(), m_data.end(), [&name](IResource *p) { return p->name() == name; });
	if (it != m_data.end()) {
		delete *it;
		m_data.erase(it);
		return;
	}
}

template <>
void ResourceManager<typename list>::destroyAllResources()
{
	std::for_each(m_data.begin(), m_data.end(), [](IResource *p) { delete p; });
	m_data.clear();
}

// Частичная специализация методов для hash реализации
template <>
IResourceManager::StorageType ResourceManager<typename hash>::storageType() const
{
	return IResourceManager::StorageType_Hash;
}

// Частичная специализация методов для map реализации
template <>
IResourceManager::StorageType ResourceManager<typename map>::storageType() const
{
	return IResourceManager::StorageType_Map;
}

} // namespace