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

	IResource *findResource(const std::string& name) const;
	void addResource(IResource *pResource);
	void destroyResource(const std::string& name);
	void destroyResource(IResource *pResource);
	void destroyAllResources();
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

	IResource *findResource(const std::string& name) const;
	void addResource(IResource *pResource);
	void destroyResource(const std::string& name);
	void destroyResource(IResource *pResource);
	void destroyAllResources();
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

	IResource *findResource(const std::string& name) const;
	void addResource(IResource *pResource);
	void destroyResource(const std::string& name);
	void destroyResource(IResource *pResource);
	void destroyAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();
	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	std::map<std::string, IResource*> m_data;

};
/*
template<class ContainerType>
class DataManager : public IDataManager<typename ContainerType::value_type> {
public:
	typedef typename ContainerType::value_type DataType

	DataManager(const std::string& resourceManagerName);
	~DataManager();

	IDataManager<DataType>::StorageType storageType() const;

	typename ContainerType::value_type *findResource(const std::string& name) const;
	void addResource(DataType *pResource);
	void destroyResource(const std::string& name);
	void destroyResource(DataType *pResource);
	void destroyAllResources();
	uint32 size() const;

	ResourceIterator begin();
	ResourceIterator end();

	ResourceIterator rbegin();
	ResourceIterator rend();

private:
	std::string m_name;
	ContainerType m_data;

}; // class DataManager
*/
} // namespace

#endif // __RESOURCEMANAGER__