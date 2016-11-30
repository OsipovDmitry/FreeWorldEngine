#ifndef __IGPURENDERER__
#define __IGPURENDERER__

#include "IGPUBuffer.h"
#include "IGPUTexture.h"
#include "IGPUBufferContainer.h"
#include "IGPUShader.h"

namespace FreeWorldEngine {

class IGPURenderer {
public:
	virtual ~IGPURenderer() {}

	virtual IGPUBuffer *createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage = IGPUBuffer::IGPUBufferUsage_StaticDraw, void *pData = 0) = 0;
	virtual void destroyBuffer(IGPUBuffer *pBuffer) = 0;

	virtual IGPUBufferContainer *createBufferContainer() = 0;
	virtual void destroyBufferContainer(IGPUBufferContainer *pBufferContainer) = 0;

    virtual IGPUTexture *createTexture(IGPUTexture::IGPUTextureType type, const uint32 *size, TextureFormat internalFormat) = 0;
    virtual void destroyTexture(IGPUTexture *pTexture) = 0;
	virtual void setTexture(const uint32 slot, const IGPUTexture *pTexture) const = 0;

    virtual IGPUShader *createShader(IGPUShader::IGPUShaderType type) = 0;
    virtual void destroyShader(IGPUShader *pShader) = 0;

    virtual IGPUProgram *createProgram() = 0;
    virtual void destroyProgram(IGPUProgram *pProgram) = 0;

	virtual void renderGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const uint32 firstVertex, const uint32 numVertices) const = 0;
	virtual void renderIndexedGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const Type indicesType, const uint32 numIndices, const uint32 offset = 0) const = 0;

	virtual void tmp() const = 0;

};

} // namespace

#endif
