#ifndef __GLRENDERER__
#define __GLRENDERER__

#include <glew-1.13.0/include/GL/glew.h>

#include "renderer/IGPURenderer.h"

namespace FreeWorldEngine {

class GLBuffer;
class GLBufferContainer;
class GLTexture;
class GLRenderBuffer;
class GLFrameBuffer;
class GLProgram;

class GLRenderer : public IGPURenderer {
public:
	GLRenderer();
	~GLRenderer();

	IGPUBuffer *createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage = IGPUBuffer::IGPUBufferUsage_StaticDraw, void *pData = 0);
	void destroyBuffer(IGPUBuffer *pBuffer);

	IGPUBufferContainer *createBufferContainer();
	void destroyBufferContainer(IGPUBufferContainer *pBufferContainer);

    IGPUTexture *createTexture(IGPUTexture::IGPUTextureType type, const uint32 *size, const TextureFormat& internalFormat);
    void destroyTexture(IGPUTexture *pTexture);
	void setTexture(const uint32 slot, const IGPUTexture *pTexture) const;

    IGPUShader *createShader(IGPUShader::IGPUShaderType type);
    void destroyShader(IGPUShader *pShader);

    IGPUProgram *createProgram();
    void destroyProgram(IGPUProgram *pProgram);

	IGPURenderBuffer *createRenderBuffer(const TextureFormat& format, const uint32 width, const uint32 height);
	void destroyRenderBuffer(IGPURenderBuffer *pRenderBuffer);

	IGPUFrameBuffer *createFrameBuffer();
	void destroyFrameBuffer(IGPUFrameBuffer *pFrameBuffer);
	void setFrameBuffer(const IGPUFrameBuffer *pFrameBuffer, const uint32 numDrawBuffers = 1) const;

	void renderGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const uint32 firstVertex, const uint32 numVertices) const;
	void renderIndexedGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const Type indicesType, const uint32 numIndices, const uint32 offset = 0) const;

	void enableDepthTest(const DepthTestFunc func = DepthTestFunc_Less);
	void disableDepthTest();

	void enableBlend(const int32 slot = -1);
	void disableBlend(const int32 slot = -1);
	void setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA);
	void setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA);

	void tmp() const;

	void bindBuffer(const GLBuffer *pBuffer, GLenum GLTarget) const;
	void bindBufferContainer(const GLBufferContainer *pBufferContainer) const;
	void bindTexture(const GLTexture *pTexture, uint32 unit) const;
    void bindProgram(const GLProgram *pProgram) const;
	void bindRenderBuffer(const GLRenderBuffer *pRenderBuffer) const;
	void bindFrameBuffer(const GLFrameBuffer *pFrameBuffer) const;

	static GLenum GLPrimitiveFormat(PrimitiveFormat primitiveFormat);
	static GLenum GLType(Type type);
	static GLenum GLDepthTestFunc(DepthTestFunc func);
	static GLenum GLBlendFunc(BlendFunc func);
	static GLenum GLBlendEquation(BlendEquation func);

private:
	static const uint32 TEXTURE_UNITS_COUNT = 16;
	mutable const GLTexture* m_cachedTextures[TEXTURE_UNITS_COUNT];

	mutable const GLBufferContainer *m_cachedBufferConatiner;

	static const uint32 BUFFER_UNITS_COUNT = 10;
	mutable const GLBuffer* m_cachedBuffers[BUFFER_UNITS_COUNT];

    mutable const GLProgram* m_cachedProgram;

	mutable const GLRenderBuffer *m_cachedRenderBuffer;
	mutable const GLFrameBuffer *m_cachedFrameBuffer;

	mutable bool m_cachedDepthTest;
	mutable DepthTestFunc m_cachedDepthTestFunc;

	static const uint32 BLEND_UNITS_COUNT = 16;
	mutable bool m_cachedBlend[BLEND_UNITS_COUNT];
	mutable BlendFunc m_cachedBlendRGBSrc, m_cachedBlendRGBDst, m_cachedBlendASrc, m_cachedBlendADst;
	mutable BlendEquation m_cachedBlendRGBEquat, m_cachedBlendAEquat;

}; // class GLRenderer

extern GLRenderer *pGLRenderer;

}

#endif // __GLRENDERER__
