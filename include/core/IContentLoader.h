#ifndef __ICONTENTLOADER__
#define __ICONTENTLOADER__

#include <string>

namespace FreeWorldEngine {

template <class DataType>
class IContent;

template <class DataType>
class IContentLoader {
public:
	virtual ~IContentLoader() {}
	
	virtual IContent<DataType> *load(const std::string& filename) = 0;
	virtual IContent<DataType> *find(const std::string& name) const = 0;
	virtual void destory(const std::string& name) = 0;
	virtual void destory(IContent<DataType> *pContent) = 0;

	virtual void registerDataLoader(const std::string& format, DataType *(*pFunc)(const std::string&)) = 0;
	virtual void unregisterDataLoader(const std::string& format) = 0;

}; // class IContentLoader

} // namespace

#endif // __ICONTENTLOADER__
