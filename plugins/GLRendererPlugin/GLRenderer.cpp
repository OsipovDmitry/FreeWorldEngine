#include "GLRenderer.h"
#include "GLBuffer.h"
#include "GLBufferContainer.h"
#include "GLTexture.h"
#include "GLShader.h"

namespace FreeWorldEngine {

GLRenderer *pGLRenderer = 0;

GLRenderer::GLRenderer()
{
	for (uint32 i = 0; i < TEXTURE_UNITS_COUNT; ++i)
		m_cachedTextures[i] = 0;

	m_cachedBufferConatiner = 0;

	for (uint32 i = 0; i < BUFFER_UNITS_COUNT; ++i)
		m_cachedBuffers[i] = 0;

    m_cachedProgram = 0;
}

GLRenderer::~GLRenderer()
{
}

IGPUBuffer *GLRenderer::createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage, void *pData)
{
	GLuint id;
	glGenBuffers(1, &id);

	GLBuffer *pBuffer = new GLBuffer(id);
	
	if (size) {
		bindBuffer(pBuffer, GL_ARRAY_BUFFER);
		glBufferData(GL_ARRAY_BUFFER, size, pData, GLBuffer::toGLUsage(usage));
	}

	return pBuffer;
}

void GLRenderer::destroyBuffer(IGPUBuffer *pBuffer)
{
	GLuint id = static_cast<GLBuffer*>(pBuffer)->id();
	glDeleteBuffers(1, &id);
	delete pBuffer;
}

IGPUBufferContainer *GLRenderer::createBufferContainer()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	GLBufferContainer *pBufferConatiner = new GLBufferContainer(id);
	return pBufferConatiner;
}

void GLRenderer::destroyBufferContainer(IGPUBufferContainer *pBufferContainer)
{
	GLuint id = static_cast<GLBufferContainer*>(pBufferContainer)->id();
	glDeleteVertexArrays(1, &id);
	delete pBufferContainer;
}


IGPUTexture *GLRenderer::createTexture(IGPUTexture::IGPUTextureType type, int *size, TextureFormat format, const void *pData)
{
    GLenum GLtarget = GLTexture::GLtarget(type);
    GLenum GLformat = GLTexture::GLformat(format);
    GLenum GLinternalFormat = GLTexture::GLinternalFormat(format);

    if (!GLtarget || !GLformat || !GLinternalFormat)
        return 0;

    GLuint id;
    glGenTextures(1, &id);

    GLTexture *pTexture = new GLTexture(type, id);
    switch (type) {
    case IGPUTexture::IGPUTextureType_1D: {
        //glTexImage1D(GLtarget, 0, GLinternalFormat, size[0], 0, GLformat, )
    }
    }

    return pTexture;
}

void GLRenderer::destroyTexture(IGPUTexture *pTexture)
{
}

IGPUShader *GLRenderer::createShader(IGPUShader::IGPUShaderType type)
{
    return 0;
}

void GLRenderer::destroyShader(IGPUShader *pShader)
{

}

IGPUProgram *GLRenderer::createProgram()
{
	return 0;
}

void GLRenderer::destroyProgram(IGPUShader *pProgram)
{

}

void GLRenderer::bindBuffer(const GLBuffer *pBuffer, GLenum GLTarget) const
{
	int32 cacheIdx = -1;
	switch (GLTarget) {
	case GL_ARRAY_BUFFER : { cacheIdx = 0; break; }
	case GL_COPY_READ_BUFFER : { cacheIdx = 1; break; }
	case GL_COPY_WRITE_BUFFER : { cacheIdx = 2; break; }
	case GL_ELEMENT_ARRAY_BUFFER: { cacheIdx = 3; break; }
	case GL_PIXEL_PACK_BUFFER: { cacheIdx = 4; break; }
	case GL_PIXEL_UNPACK_BUFFER: { cacheIdx = 5; break; }
	case GL_TEXTURE_BUFFER: { cacheIdx = 6; break; }
	case GL_TRANSFORM_FEEDBACK_BUFFER: { cacheIdx = 7; break; }
	case GL_UNIFORM_BUFFER: { cacheIdx = 8; break; }
	default: break;
	}

	if (cacheIdx >= 0) {
		if (m_cachedBuffers[cacheIdx] == pBuffer)
			return;
		m_cachedBuffers[cacheIdx] = pBuffer;
	}
	glBindBuffer(GLTarget, pBuffer ? pBuffer->id() : 0);
}

void GLRenderer::bindBufferContainer(const GLBufferContainer *pBufferContainer) const
{
	if (m_cachedBufferConatiner == pBufferContainer)
		return;
	m_cachedBufferConatiner = pBufferContainer;
	glBindVertexArray(pBufferContainer ? pBufferContainer->id() : 0);
}

void GLRenderer::bindTexture(const GLTexture *pTexture, uint32 unit) const
{
	if (m_cachedTextures[unit] == pTexture)
		return;
	m_cachedTextures[unit] = pTexture;
	glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GLTexture::GLtarget(pTexture->type()), pTexture->GLid());
}

void GLRenderer::bindProgram(const GLProgram *pProgram) const
{
    if (m_cachedProgram == pProgram)
        return;
    m_cachedProgram = pProgram;
    glUseProgram(pProgram ? pProgram->GLid() : 0);
}

} // namespace
