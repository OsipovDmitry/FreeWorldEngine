#ifndef __IGPUBUFFERCONTAINER__
#define __IGPUBUFFERCONTAINER__

#include <Types.h>

namespace FreeWorldEngine {

class IGPUBuffer;

class IGPUBufferContainer {
public:

	virtual ~IGPUBufferContainer() {}

	virtual void setVertexAttribute(const IGPUBuffer *pGPUBuffer, uint32 indexAttribute, uint32 numComponents, int32 offsetAttribute, int32 strideAttribute, Type type) const = 0;
	virtual void enableVertexAttribute(uint32 indexAttribute) const = 0;
	virtual void disableVertexAttribute(uint32 indexAttribute) const = 0;

}; // class IGPUBufferContainer

} // namespace

#endif // __IGPUBUFFERCONTAINER__