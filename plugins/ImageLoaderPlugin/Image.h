#ifndef __IMAGE__
#define __IMAGE__

#include "IImage.h"

namespace FreeWorldEngine {

class Image : public IImage {
public:
	Image(const std::string& name);
	~Image();
	std::string name() const;
	uint32 width() const;
	uint32 height() const;
	uint32 depth() const;
	uint32 numComponents() const;
	Type type() const;
	void *data() const;
	void setWidth(const uint32 width);
	void setHeight(const uint32 height);
	void setDepth(const uint32 depth);
	void setNumComponents(const uint32 numComponents);
	void setType(const Type type);
	void setData(void *data);

private:
	std::string m_name;
	uint32 m_width, m_height, m_depth;
	uint32 m_numComponents;
	Type m_type;
	void *m_data;

}; // class Image

} // namespace

#endif // IMAGE