#ifndef __CONTENTLOADER__
#define __CONTENTLOADER__

#include <string>

#include <IContentLoader.h>
#include <IResourceManager.h>

#include "ContentLoader.h"
#include "Content.h"
#include "DataLoader.h"
#include "Core.h"

namespace FreeWorldEngine {

template <class DataType>
class ContentLoader : public IContentLoader<DataType> {
public:
	ContentLoader(const std::string& resourceManagerName);
	~ContentLoader();

	IContent<DataType> *load(const std::string& filename);
	IContent<DataType> *find(const std::string& name) const;
	void destory(const std::string& name);
	void destory(IContent<DataType> *pContent);

	void registerDataLoader(const std::string& format, DataType *(*pFunc)(const std::string&));
	void unregisterDataLoader(const std::string& format);

private:
	IResourceManager *m_pResourceManager;
	DataLoader<DataType> *m_pDataLoader;

}; // class ContentLoader

template<class DataType>
inline ContentLoader<DataType>::ContentLoader(const std::string & resourceManagerName) :
	m_pResourceManager(coreEngine->createResourceManager(resourceManagerName)),
	m_pDataLoader(new DataLoader<DataType>())
{
}

template<class DataType>
inline ContentLoader<DataType>::~ContentLoader()
{
	delete m_pDataLoader;
	coreEngine->destroyResourceManager(m_pResourceManager);
}

template<class DataType>
inline IContent<DataType>* ContentLoader<DataType>::load(const std::string & filename)
{
	IContent<DataType> *pContent = find(filename);
	if (pContent)
		return pContent;

	DataType *pData = m_pDataLoader->loadData(filename);

	pContent = new Content<DataType>(filename, pData);
	m_pResourceManager->addResource(pContent);
	return pContent;
}

template<class DataType>
inline IContent<DataType>* ContentLoader<DataType>::find(const std::string & name) const
{
	return static_cast<IContent<DataType>*>(m_pResourceManager->findResource(name));
}

template<class DataType>
inline void ContentLoader<DataType>::destory(const std::string & name)
{
	m_pResourceManager->destroyResource(name);
}

template<class DataType>
inline void ContentLoader<DataType>::destory(IContent<DataType>* pContent)
{
	m_pResourceManager->destroyResource(pContent->name());
}

template<class DataType>
inline void ContentLoader<DataType>::registerDataLoader(const std::string & format, DataType *(*pFunc)(const std::string &))
{
	m_pDataLoader->registerDataLoader(format, pFunc);
}

template<class DataType>
inline void ContentLoader<DataType>::unregisterDataLoader(const std::string & format)
{
	m_pDataLoader->unregisterDataLoader(format);
}

} // namespace

#endif // __CONTENTLOADER__