#ifndef __CONTENT__
#define __CONTENT__

#include <IContent.h>

namespace FreeWorldEngine {

template <class DataType>
class Content : public IContent<DataType> {
public:
	Content(const std::string& name, DataType *pData);
	~Content();

	std::string name() const;

	const DataType *data() const;

private:
	std::string m_name;
	DataType *m_pData;

};

template<class DataType>
inline Content<DataType>::Content(const std::string & name, DataType * pData) :
	m_name(name),
	m_pData(pData)
{
}

template<class DataType>
inline Content<DataType>::~Content()
{
	delete  m_pData;
}

template<class DataType>
inline std::string Content<DataType>::name() const
{
	return m_name;
}

template<class DataType>
inline const DataType *Content<DataType>::data() const
{
	return m_pData;
}

} // namespace

#endif