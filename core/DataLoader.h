#ifndef __DATALOADER__
#define __DATALOADER__

#include <string>
#include <map>
#include <algorithm>
#include <iterator>

#include <utility/File.h>

namespace FreeWorldEngine {

template <class DataType>
class DataLoader {
public:
	DataLoader();
	~DataLoader();

	void registerDataLoader(const std::string& format, DataType *(*pFunc)(const std::string&));
	void unregisterDataLoader(const std::string& format);

	DataType *loadData(const std::string& filename) const;

private:
	std::map<std::string, DataType *(*)(const std::string&)> m_loaders;

};

template <class DataType>
inline DataLoader<DataType>::DataLoader() :
	m_loaders()
{
}

template <class DataType>
inline DataLoader<DataType>::~DataLoader()
{
}

template <class DataType>
inline void DataLoader<DataType>::registerDataLoader(const std::string& format, DataType *(*pFunc)(const std::string&))
{
	std::string ext;
	std::transform(format.begin(), format.end(), std::back_inserter(ext), tolower);
	m_loaders[ext] = pFunc;
}

template <class DataType>
inline void DataLoader<DataType>::unregisterDataLoader(const std::string& format)
{
	std::string ext;
	std::transform(format.begin(), format.end(), std::back_inserter(ext), tolower);
	m_loaders.erase(ext);
}

template <class DataType>
inline DataType *DataLoader<DataType>::loadData(const std::string& filename) const
{
	std::string ext = Utility::File(filename).fileExtension();
	std::transform(ext.cbegin(), ext.cend(), ext.begin(), tolower);
	auto loader = m_loaders.find(ext);

	return (loader != m_loaders.cend()) ? loader->second(filename) : 0;
}

} // namespace

#endif // __DATALOADER__
