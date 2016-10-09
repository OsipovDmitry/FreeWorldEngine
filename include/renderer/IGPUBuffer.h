#ifndef __IGPUBUFFER__
#define __IGPUBUFFER__

#include "Types.h"

namespace FreeWorldEngine {

class IGPUBuffer {
public:
	virtual ~IGPUBuffer() {}

	virtual uint32 size() const = 0;
	virtual void resize(const uint32 newSize) = 0;

	virtual uint32 capacity() const = 0;
	virtual void reserved(const uint32 reservedSize) = 0;

}; // class IGPUBuffer

} // namespace

#endif // __IGPUBUFFER__