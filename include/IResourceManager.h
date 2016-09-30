#ifndef __IRESOURCEMANAGER__
#define __IRESOURCEMANAGER__

#include <string>

#include "IResource.h"
#include "CoreSettings.h"
#include "Types.h"

namespace FreeWorldEngine {

class ResourceIteratorPrivate;
class CORE_FUNC_DLL ResourceIterator {
public:
	ResourceIterator(ResourceIteratorPrivate *pPrivate);
	~ResourceIterator();
	ResourceIterator(const ResourceIterator& other);
	ResourceIterator& operator =(const ResourceIterator& other);

	bool operator !=(const ResourceIterator& other) const;
	bool operator ==(const ResourceIterator& other) const;
	IResource *operator *();
	ResourceIterator& operator ++();
	ResourceIterator& operator ++(int);
	ResourceIterator& operator --();
	ResourceIterator& operator --(int);

private:
	ResourceIteratorPrivate *m;
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
	virtual uint32 size() const = 0;

	virtual ResourceIterator begin() = 0;
	virtual ResourceIterator end() = 0;

	virtual ResourceIterator rbegin() = 0;
	virtual ResourceIterator rend() = 0;

}; // class IResourceManager

class DataIteratorPrivate;
template<class DataType>
class CORE_FUNC_DLL DataIterator {
public:
	DataIterator(ResourceIteratorPrivate *pPrivate);
	~DataIterator();
	DataIterator(const ResourceIterator& other);
	DataIterator& operator =(const DataIterator& other);

	bool operator !=(const DataIterator& other) const;
	bool operator ==(const DataIterator& other) const;
	DataType *operator *();
	DataIterator& operator ++();
	DataIterator& operator ++(int);
	DataIterator& operator --();
	DataIterator& operator --(int);

private:
	DataIteratorPrivate *m;
};

template<class DataType>
class IDataManager : public IResource {
public:
	enum StorageType {
		StorageType_List = 0,
		StorageType_Hash,
		StorageType_Map
	};

	virtual ~IDataManager() = 0 {}

	virtual StorageType storageType() const = 0;

	virtual DataType *getByName(const std::string& name) const = 0;
	virtual void addResource(DataType *pResource) = 0;
	virtual void deleteResource(const std::string& name) = 0;
	virtual void deleteResource(DataType *pResource) = 0;
	virtual void deleteAllResources() = 0;
	virtual uint32 size() const = 0;

	virtual ResourceIterator begin() = 0;
	virtual ResourceIterator end() = 0;

	virtual ResourceIterator rbegin() = 0;
	virtual ResourceIterator rend() = 0;

}; // class IDataManager

} // namespace

#endif // __IRESOURCEMANAGER__