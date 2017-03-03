#ifndef __IRESOURCEMANAGER__
#define __IRESOURCEMANAGER__

#include <string>
#include <xutility>

#include "IResource.h"
#include "CoreSettings.h"
#include "Types.h"

namespace FreeWorldEngine {

class ResourceIteratorPrivate;
class CORE_FUNC_DLL ResourceIterator : public
	std::_Iterator012<std::bidirectional_iterator_tag,
					  IResource*,
					  ptrdiff_t,
					  IResource**,
					  IResource*&,
					  std::_Iterator_base0> {
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

	virtual IResource *findResource(const std::string& name) const = 0;
	virtual void addResource(IResource *pResource) = 0;
	virtual void destroyResource(const std::string& name) = 0;
	virtual void destroyResource(IResource *pResource) = 0;
	virtual void destroyAllResources() = 0;
	virtual uint32 size() const = 0;

	virtual ResourceIterator begin() = 0;
	virtual ResourceIterator end() = 0;

}; // class IResourceManager

} // namespace

#endif // __IRESOURCEMANAGER__