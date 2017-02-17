#ifndef __CONTENT__
#define __CONTENT__

#include <IContent.h>

namespace FreeWorldEngine {

template <class ContentData>
class Content : public IContent {
public:
	Content(const std::string& name, ContentData *pData);
	~Content();

	std::string name() const;
	const ContentData *data() const;

private:
	std::string m_name;
	ContentData *m_pData;

};

} // namespace

#endif //__CONTENT__
