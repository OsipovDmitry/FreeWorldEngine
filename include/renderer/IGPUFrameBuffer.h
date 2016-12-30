#ifndef __IGPUFRAMBUFFER__
#define __IGPUFRAMBUFFER__

#include <Types.h>
#include <renderer/IGPUTexture.h>

namespace FreeWorldEngine {

namespace Renderer {

class IGPURenderBuffer {
public:
	virtual ~IGPURenderBuffer() {}

	virtual TextureFormat format() const = 0;
	virtual uint32 width() const = 0;
	virtual uint32 height() const = 0;
	virtual void resize(const uint32 width, const uint32 height) = 0;

}; // class IGPURnederBuffer

class IGPUFrameBuffer {
public:
	virtual ~IGPUFrameBuffer() {}

	virtual void attachColorBuffer(const uint32 slot, const IGPURenderBuffer *pRenderBuffer) = 0;
	virtual void attachColorBuffer(const uint32 slot, const IGPUTexture *pTexture, const uint32 layer = 0) = 0;

	virtual void attachDepthBuffer(const IGPURenderBuffer *pRenderBuffer) = 0;
	virtual void attachDepthBuffer(const IGPUTexture *pTexture, const uint32 layer = 0) = 0;

	virtual void clearColorBuffer(const uint32 slot = 0, const float red = 0.0f, const float green = 0.0f, const float blue = 0.0f, const float alpha = 1.0f) = 0;
	virtual void clearDepthBuffer(const float depth = 1.0f) = 0;

}; // class IGPUFrameBuffer

} // namespace

} // namespace

#endif // __IGPUFRAMBUFFER__