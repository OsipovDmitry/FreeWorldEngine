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
	GLuint id = static_cast<GLBuffer*>(pBuffer)->GLid();
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


IGPUTexture *GLRenderer::createTexture(IGPUTexture::IGPUTextureType type, const uint32 *size, TextureFormat internalFormat)
{
    GLenum GLtarget = GLTexture::GLtarget(type);
    GLenum GLinternalFormat = GLTexture::GLinternalFormat(internalFormat);

    if (!GLtarget || !GLinternalFormat)
        return 0;

    GLuint id;
    glGenTextures(1, &id);

    GLTexture *pTexture = new GLTexture(type, id);
	bindTexture(pTexture, 0);

	glTexParameteri(GLtarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GLtarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (type) {

	case IGPUTexture::IGPUTextureType_2DArray:
	case IGPUTexture::IGPUTextureType_3D: {
		glTexImage3D(GLtarget, 0, GLinternalFormat, size[0], size[1], size[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
	}
	
	case IGPUTexture::IGPUTextureType_1DArray:
	case IGPUTexture::IGPUTextureType_2D:
	case IGPUTexture::IGPUTextureType_Rectangle: {
		glTexImage2D(GLtarget, 0, GLinternalFormat, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
	}

	case IGPUTexture::IGPUTextureType_CubeMap: {
		for (GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++target)
			glTexImage2D(target, 0, GLinternalFormat, size[0], size[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
	}

    case IGPUTexture::IGPUTextureType_1D: {
        glTexImage1D(GLtarget, 0, GLinternalFormat, size[0], 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
    }
	
    }

    return pTexture;
}

void GLRenderer::destroyTexture(IGPUTexture *pTexture)
{
	GLTexture *pGLTexture = static_cast<GLTexture*>(pTexture);
	GLuint id = pGLTexture->GLid();
	glDeleteTextures(1, &id);
	delete pTexture;
}

void GLRenderer::setTexture(const uint32 slot, const IGPUTexture *pTexture) const
{
	bindTexture(static_cast<const GLTexture*>(pTexture), slot);
}

IGPUShader *GLRenderer::createShader(IGPUShader::IGPUShaderType type)
{
	GLenum GLtype = GLShader::GLtype(type);
	if (!GLtype)
		return 0;

	GLuint id = glCreateShader(GLtype);
	if (!id)
		return 0;

	return new GLShader(id);
}

void GLRenderer::destroyShader(IGPUShader *pShader)
{
	GLShader *pGLShader = static_cast<GLShader*>(pShader);
	glDeleteShader(pGLShader->GLid());
	delete pShader;
}

IGPUProgram *GLRenderer::createProgram()
{
	GLuint id = glCreateProgram();
	if (!id)
		return 0;

	return new GLProgram(id);
}

void GLRenderer::destroyProgram(IGPUProgram *pProgram)
{
	GLProgram *pGLProgram = static_cast<GLProgram*>(pProgram);
	glDeleteProgram(pGLProgram->GLid());
	delete pProgram;
}

void GLRenderer::renderGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const uint32 firstVertex, const uint32 numVertices) const
{
	GLenum mode = GLPrimitiveFormat(primitiveFormat);
	if (mode) {
		bindProgram(static_cast<const GLProgram*>(pProgram));
		bindBufferContainer(static_cast<const GLBufferContainer*>(pBufferContainer));
		glDrawArrays(mode, firstVertex, numVertices);
	}
}

void GLRenderer::renderIndexedGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const Type indicesType, const uint32 numIndices, const uint32 offset) const
{
	GLenum mode = GLPrimitiveFormat(primitiveFormat);
	GLenum type = GLType(indicesType);
	if (mode && type) {
		bindProgram(static_cast<const GLProgram*>(pProgram));
		bindBufferContainer(static_cast<const GLBufferContainer*>(pBufferContainer));
		glDrawElements(mode, numIndices, type, (const void*)offset);
	}
}

void GLRenderer::tmp() const
{
	glViewport(0,0,500,500);
	glClearColor(1,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	glBindBuffer(GLTarget, pBuffer ? pBuffer->GLid() : 0);
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

GLenum GLRenderer::GLPrimitiveFormat(PrimitiveFormat primitiveFormat)
{
	switch (primitiveFormat) {
	case PrimitiveFormat_Points: return GL_POINTS;
	case PrimitiveFormat_Lines: return GL_LINES;
	case PrimitiveFormat_LineStrip: return GL_LINE_STRIP;
	case PrimitiveFormat_LineLoop: return GL_LINE_LOOP;
	case PrimitiveFormat_Trangles: return GL_TRIANGLES;
	case PrimitiveFormat_TriangleStrip: return GL_TRIANGLE_STRIP;
	case PrimitiveFormat_TrangleFan: return GL_TRIANGLE_FAN;
	}
	return 0;
}

GLenum GLRenderer::GLType(Type type)
{
	switch (type) {
	case TYPE_SIGNED_INT_8: return GL_BYTE;
	case TYPE_SIGNED_INT_16: return GL_SHORT;
	case TYPE_SIGNED_INT_32: return GL_INT;
	case TYPE_UNSIGNED_INT_8: return GL_UNSIGNED_BYTE;
	case TYPE_UNSIGNED_INT_16: return GL_UNSIGNED_SHORT;
	case TYPE_UNSIGNED_INT_32: return GL_UNSIGNED_INT;
	case TYPE_FLOAT: return GL_FLOAT;
	}
	return 0;
}

} // namespace
