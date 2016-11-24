#ifndef __GLRENDERER__
#define __GLRENDERER__

#include <glew-1.13.0/include/GL/glew.h>

#include "renderer/IGPURenderer.h"

namespace FreeWorldEngine {

class GLBuffer;
class GLBufferContainer;
class GLTexture;
class GLProgram;

class GLRenderer : public IGPURenderer {
public:
	GLRenderer();
	~GLRenderer();

	IGPUBuffer *createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage = IGPUBuffer::IGPUBufferUsage_StaticDraw, void *pData = 0);
	void destroyBuffer(IGPUBuffer *pBuffer);

	IGPUBufferContainer *createBufferContainer();
	void destroyBufferContainer(IGPUBufferContainer *pBufferContainer);

    IGPUTexture *createTexture(IGPUTexture::IGPUTextureType type, int *size, TextureFormat format, const void *pData = 0);
    void destroyTexture(IGPUTexture *pTexture);

    IGPUShader *createShader(IGPUShader::IGPUShaderType type);
    void destroyShader(IGPUShader *pShader);

    IGPUProgram *createProgram();
    void destroyProgram(IGPUShader *pProgram);

	void bindBuffer(const GLBuffer *pBuffer, GLenum GLTarget) const;
	void bindBufferContainer(const GLBufferContainer *pBufferContainer) const;
	void bindTexture(const GLTexture *pTexture, uint32 unit) const;
    void bindProgram(const GLProgram *pProgram) const;

private:
	static const uint32 TEXTURE_UNITS_COUNT = 16;
	mutable const GLTexture* m_cachedTextures[TEXTURE_UNITS_COUNT];

	mutable const GLBufferContainer *m_cachedBufferConatiner;

	static const uint32 BUFFER_UNITS_COUNT = 10;
	mutable const GLBuffer* m_cachedBuffers[BUFFER_UNITS_COUNT];

    mutable const GLProgram* m_cachedProgram;

}; // class GLRenderer

extern GLRenderer *pGLRenderer;

}

#endif // __GLRENDERER__
