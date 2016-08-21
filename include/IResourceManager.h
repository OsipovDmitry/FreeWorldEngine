#ifndef __IRESOURCEMANAGER__
#define __IRESOURCEMANAGER__

#include <string>
#include <list>

#include "IResource.h"

namespace FreeWorldEngine {

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
	virtual std::list<IResource*> listResources() const = 0;

}; // class IResourceManager

} // namespace

#endif // __IRESOURCEMANAGER__