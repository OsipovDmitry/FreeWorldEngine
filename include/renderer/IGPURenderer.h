#ifndef __IGPURENDERER__
#define __IGPURENDERER__

#include <math/MathTypes.h>

#include <renderer/IGPUBuffer.h>
#include <renderer/IGPUTexture.h>
#include <renderer/IGPUBufferContainer.h>
#include <renderer/IGPUShader.h>
#include <renderer/IGPUFrameBuffer.h>

namespace FreeWorldEngine {

namespace Renderer {

class IGPURenderer {
public:
	enum DepthTestFunc {
		DepthTestFunc_Never,
		DepthTestFunc_Always,
		DepthTestFunc_Less,
		DepthTestFunc_LessEqual,
		DepthTestFunc_Equal,
		DepthTestFunc_Greater,
		DepthTestFunc_GreaterEqual,
		DepthTestFunc_NotEqual
	};

	enum BlendFunc {
		BlendFunc_Zero,
		BlendFunc_One,
		BlendFunc_SrcColor,
		BlendFunc_InvSrcColor,
		BlendFunc_DstColor,
		BlendFunc_InvDstColor,
		BlendFunc_SrcAlpha,
		BlendFunc_InvSrcAlpha,
		BlendFunc_DstAlpha,
		BlendFunc_InvDstAlpha,
		BlendFunc_ConstColor,
		BlendFunc_InvConstColor,
		BlendFunc_ConstAlpha,
		BlendFunc_InvConstAlpha
	};

	enum BlendEquation {
		BlendEquation_Add,
		BlendEquation_Sub,
		BlendEquation_ReverseSub,
		BlendEquation_Min,
		BlendEquation_Max
	};

	enum CullFaceState {
		CullFaceState_Disabled,
		CullFaceState_RenderFrontFaces,
		CullFaceState_RenderBackFaces
	};

	virtual ~IGPURenderer() {}

	virtual IGPUBuffer *createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage = IGPUBuffer::IGPUBufferUsage_StaticDraw, void *pData = 0) = 0;
	virtual void destroyBuffer(IGPUBuffer *pBuffer) = 0;
	virtual void setUniformBuffer(const IGPUBuffer *pBuffer, const uint32 bindingPoint) const = 0;

	virtual IGPUBufferContainer *createBufferContainer() = 0;
	virtual void destroyBufferContainer(IGPUBufferContainer *pBufferContainer) = 0;

    virtual IGPUTexture *createTexture(IGPUTexture::IGPUTextureType type, const uint32 *size, const TextureFormat& internalFormat) = 0;
    virtual void destroyTexture(IGPUTexture *pTexture) = 0;
	virtual void setTexture(const IGPUTexture *pTexture, const uint32 slot) const = 0;

    virtual IGPUShader *createShader(IGPUShader::IGPUShaderType type) = 0;
    virtual void destroyShader(IGPUShader *pShader) = 0;

    virtual IGPUProgram *createProgram() = 0;
    virtual void destroyProgram(IGPUProgram *pProgram) = 0;

	virtual IGPURenderBuffer *createRenderBuffer(const TextureFormat& format, const uint32 width, const uint32 height) = 0;
	virtual void destroyRenderBuffer(IGPURenderBuffer *pRenderBuffer) = 0;

	virtual IGPUFrameBuffer *createFrameBuffer() = 0;
	virtual void destroyFrameBuffer(IGPUFrameBuffer *pFrameBuffer) = 0;
	virtual void setFrameBuffer(const IGPUFrameBuffer *pFrameBuffer, const uint32 numDrawBuffers = 1) const = 0;
	virtual IGPUFrameBuffer *mainFrameBuffer() const = 0;

	virtual void renderGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const uint32 firstVertex, const uint32 numVertices) const = 0;
	virtual void renderIndexedGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const Type indicesType, const uint32 numIndices, const uint32 offset = 0) const = 0;

	virtual void enableDepthTest(const DepthTestFunc func = DepthTestFunc_Less) = 0;
	virtual void disableDepthTest() = 0;

	virtual void setColorWriteMask(bool red, bool green, bool blue, bool alpha) = 0;
	virtual void setDepthWriteMask(bool depth) = 0;

	// slot == -1 => вкл/выкл во всех слотах
	virtual void enableBlend(const int32 slot = -1) = 0;
	virtual void disableBlend(const int32 slot = -1) = 0;
	virtual void setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA) = 0;
	virtual void setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA) = 0;

	virtual void setViewport(const uint32 x, const uint32 y, const uint32 width, const uint32 height) = 0;
	virtual void viewport(uint32& x, uint32& y, uint32& width, uint32& height) const = 0;

	virtual void setDepthRange(const float near = 0.0f, const float far = 1.0f) = 0;
	virtual void depthRange(float& near, float& far) const = 0;

	virtual void setCullFaceState(CullFaceState state) = 0;
	virtual CullFaceState cullFaceState() const = 0;
};

} // namespace

} // namespace

#endif
