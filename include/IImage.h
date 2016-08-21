#ifndef __IIMAGE__
#define __IIMAGE__

#include "IResource.h"
#include "Types.h"

namespace FreeWorldEngine {

class IImage : public IResource {
public:
	virtual ~IImage() = 0 {}
	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual uint32 depth() const = 0;
	virtual uint32 numComponents() const = 0;
	virtual Type type() const = 0;
	virtual void *data() const = 0;
	virtual void setWidth(const uint32 width) = 0;
	virtual void setHeight(const uint32 height) = 0;
	virtual void setDepth(const uint32 depth) = 0;
	virtual void setNumComponents(const uint32 numComponents) = 0;
	virtual void setType(const Type type) = 0;
	virtual void setData(void *data) = 0;
}; // class IImage

} // namespace

#endif // __IIMAGE__