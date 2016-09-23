#ifndef __IRESOURCEMANAGER__
#define __IRESOURCEMANAGER__

#include <string>
#include <list>

#include "IResource.h"

namespace FreeWorldEngine {

class ResourceIterator {
public:
	class Interface {
	public:
		virtual ~Interface() = 0 {}
		virtual Interface *clone() const = 0;
		virtual IResource *operator *() = 0;
	};

	ResourceIterator(Interface *pIter) : m_pIter(pIter) {}
	ResourceIterator(const ResourceIterator& other) : m_pIter(other.m_pIter->clone()) {}
	~ResourceIterator() { delete m_pIter; }
	ResourceIterator& operator =(const ResourceIterator& other) { delete m_pIter; m_pIter = other.m_pIter->clone(); }

	IResource *operator *() { return m_pIter->operator*(); }

private:
	Interface *m_pIter;
};

class IResourceManager : public IResource {
public:
	enum StorageType {
		StorageType_List = 0,
		StorageType_Hash,
		StorageType_Map
	};

	virtual ~IResourceManager() = 0 {}

	virtual StorageType storageType() const = 0;

	virtual IResource *getByName(const std::string& name) const = 0;
	virtual void addResource(IResource *pResource) = 0;
	virtual void deleteResource(const std::string& name) = 0;
	virtual void deleteResource(IResource *pResource) = 0;
	virtual void deleteAllResources() = 0;

	virtual ResourceIterator begin() = 0;
	virtual ResourceIterator end() = 0;

}; // class IResourceManager

} // namespace

#endif // __IRESOURCEMANAGER__