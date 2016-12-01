#ifndef __IGPUFRAMBUFFER__
#define __IGPUFRAMBUFFER__

#include "Types.h"

namespace FreeWorldEngine {

class IGPUTexture;

class IGPURenderBuffer {
public:
	virtual ~IGPURenderBuffer() {}

	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual void resize(const uint32 width, const uint32 height) = 0;

}; // class IGPURnederBuffer

class IGPUFrameBuffer {
public:
	virtual ~IGPUFrameBuffer() {}

	virtual void attachColorBuffer(const uint32 slot, const IGPURenderBuffer *pRenderBuffer) = 0;
	virtual void attachColorBuffer(const uint32 slot, const IGPUTexture *pTexture, const uint32 layer = 0) = 0;

}; // class IGPUFrameBuffer

} // namespace

#endif // __IGPUFRAMBUFFER__