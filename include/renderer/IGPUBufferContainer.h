#ifndef __IGPUBUFFERCONTAINER__
#define __IGPUBUFFERCONTAINER__

#include <Types.h>

namespace FreeWorldEngine {

namespace Renderer {

class IGPUBuffer;

class IGPUBufferContainer {
public:

	virtual ~IGPUBufferContainer() {}

	virtual void setVertexAttribute(const IGPUBuffer *pGPUBuffer, uint32 indexAttribute, uint32 numComponents, int32 offsetAttribute, int32 strideAttribute, Type type) const = 0;
	virtual void enableVertexAttribute(uint32 indexAttribute) const = 0;
	virtual void disableVertexAttribute(uint32 indexAttribute) const = 0;

	virtual void setIndexBuffer(const IGPUBuffer *pGPUBuffer) const = 0;

}; // class IGPUBufferContainer

} // namespace

} // namespace

#endif // __IGPUBUFFERCONTAINER__