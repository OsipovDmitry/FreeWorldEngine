#include "GLFrameBuffer.h"
#include "GLTexture.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

GLRenderBuffer::GLRenderBuffer(const GLuint id) :
	m_id(id)
{
}

GLRenderBuffer::~GLRenderBuffer()
{
}

GLuint GLRenderBuffer::GLid() const
{
	return m_id;
}

TextureFormat GLRenderBuffer::format() const
{
	GLuint res;
	pGLRenderer->bindRenderBuffer(this);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, (GLint*)&res);
	return GLTexture::fromGLinternalFormat(res);
}

uint32 GLRenderBuffer::width() const
{
	GLuint res;
	pGLRenderer->bindRenderBuffer(this);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, (GLint*)&res);
	return res;
}

uint32 GLRenderBuffer::height() const
{
	GLuint res;
	pGLRenderer->bindRenderBuffer(this);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, (GLint*)&res);
	return res;
}

void GLRenderBuffer::resize(const uint32 width, const uint32 height) 
{
	pGLRenderer->bindRenderBuffer(this);
	GLenum internalFormat;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, (GLint*)&internalFormat);
	pGLRenderer->bindRenderBuffer(0);

	glDeleteRenderbuffers(1, &m_id);
	glGenRenderbuffers(1, &m_id);
	pGLRenderer->bindRenderBuffer(this);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
}

GLFrameBuffer::GLFrameBuffer(const GLuint id) :
	m_id(id)
{
}

GLFrameBuffer::~GLFrameBuffer()
{
}

void GLFrameBuffer::attachColorBuffer(const uint32 slot, const IGPURenderBuffer *pRenderBuffer)
{
	attachRenderBuffer(GL_COLOR_ATTACHMENT0+slot, pRenderBuffer);
}

void GLFrameBuffer::attachColorBuffer(const uint32 slot, const IGPUTexture *pTexture, const uint32 layer)
{
	attachTexture(GL_COLOR_ATTACHMENT0+slot, pTexture, layer);
}

void GLFrameBuffer::attachDepthBuffer(const IGPURenderBuffer *pRenderBuffer)
{
	attachRenderBuffer(GL_DEPTH_ATTACHMENT, pRenderBuffer);
}

void GLFrameBuffer::attachDepthBuffer(const IGPUTexture *pTexture, const uint32 layer)
{
	attachTexture(GL_DEPTH_ATTACHMENT, pTexture, layer);
}

void GLFrameBuffer::clearColorBuffer(const uint32 slot, const float red, const float green, const float blue, const float alpha)
{
	pGLRenderer->bindFrameBuffer(this);
	const GLfloat color[4] = {red, green, blue, alpha};
	glClearBufferfv(GL_COLOR, slot, color);
}

void GLFrameBuffer::clearDepthBuffer(const float depth)
{
	pGLRenderer->bindFrameBuffer(this);
	glClearBufferfv(GL_DEPTH, 0, &depth);
}

GLuint GLFrameBuffer::GLid() const
{
	return m_id;
}

void GLFrameBuffer::attachRenderBuffer(GLenum attachment, const IGPURenderBuffer *pRenderBuffer)
{
	pGLRenderer->bindFrameBuffer(this);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, static_cast<const GLRenderBuffer*>(pRenderBuffer)->GLid());
}

void GLFrameBuffer::attachTexture(GLenum attachment, const IGPUTexture *pTexture, const uint32 layer)
{
	const GLTexture *pGLTexture = static_cast<const GLTexture*>(pTexture);
	IGPUTexture::IGPUTextureType texType = pGLTexture->type();
	GLuint texGLid = pGLTexture->GLid();

	pGLRenderer->bindFrameBuffer(this);
	switch (texType) {
	case IGPUTexture::IGPUTextureType_1D: {
		glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_1D, texGLid, 0);
		break;
	}
	case IGPUTexture::IGPUTextureType_2D: {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texGLid, 0);
		break;
	}
	case IGPUTexture::IGPUTextureType_Rectangle: {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_RECTANGLE, texGLid, 0);
		break;
	}
	case IGPUTexture::IGPUTextureType_CubeMap: {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X+layer, texGLid, 0);
		break;
	}
	case IGPUTexture::IGPUTextureType_3D: {
		glFramebufferTexture3D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_3D, texGLid, 0, layer);
		break;
	}
	case IGPUTexture::IGPUTextureType_1DArray:
	case IGPUTexture::IGPUTextureType_2DArray:{
		glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texGLid, 0, layer);
		break;
	}
	}
}


} // nemasace