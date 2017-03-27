#ifndef __RESOURCEMANAGER__
#define __RESOURCEMANAGER__

#include <list>
#include <unordered_map>
#include <map>

#include <IResourceManager.h>

namespace FreeWorldEngine {

template <class Container>
class ResourceManager : public IResourceManager {
public:
	ResourceManager(const std::string& resourceManagerName);
	~ResourceManager();

	std::string name() const;
	IResourceManager::StorageType storageType() const;

	IResource *findResource(const std::string& name) const;
	void addResource(IResource *pResource);
	void destroyResource(const std::string& name);
	void destroyAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();

private:
	std::string m_name;
	Container m_data;

};

IResourceManager *createResourceManager(const std::string& resourceManagerName, const IResourceManager::StorageType storageType);

} // namespace

#endif // __RESOURCEMANAGER__