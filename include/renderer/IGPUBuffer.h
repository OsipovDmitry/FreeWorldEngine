#ifndef __IGPUBUFFER__
#define __IGPUBUFFER__

#include "Types.h"

namespace FreeWorldEngine {

class IGPUBuffer {
public:
	virtual ~IGPUBuffer() {}

	enum IGPUBufferUsage {
		IGPUBufferUsage_StreamDraw,
		IGPUBufferUsage_StreamRead,
		IGPUBufferUsage_StreamCopy,
		IGPUBufferUsage_StaticDraw,
		IGPUBufferUsage_StaticRead,
		IGPUBufferUsage_StaticCopy,
		IGPUBufferUsage_DynamicDraw,
		IGPUBufferUsage_DynamicRead,
		IGPUBufferUsage_DynamicCopy,
		IGPUBufferUsage_OldValue,
	};
	virtual uint64 size() const = 0;
	virtual void resize(uint64 newSize, IGPUBufferUsage usage = IGPUBufferUsage_OldValue) = 0;

	enum IGPUBufferAccess {
		IGPUBufferAccess_ReadOnly,
		IGPUBufferAccess_WriteOnly,
		IGPUBufferAccess_ReadWrite,
	};
	virtual void *map(IGPUBufferAccess access, uint64 offset = 0, uint64 length = 0) = 0;
	virtual bool unmap() = 0;

}; // class IGPUBuffer

} // namespace

#endif // __IGPUBUFFER__