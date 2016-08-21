#include "GLRenderSystem.h"
#include "GLVertexBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLTexture.h"
#include "GLShaderProgram.h"

namespace FreeWorldEngine {

bool GLRenderSystem::m_cacheCapabilities[GLRenderSystem::CACHE_CAPABILITIES_SIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

GLRenderSystem::GLRenderSystem()
{
}

GLRenderSystem::~GLRenderSystem()
{
}

IVertexBufferObject *GLRenderSystem::createVertexBufferObject()
{
	GLuint id = 0;
	glGenBuffers(1, &id);
	IVertexBufferObject *pVBO = new GLVertexBufferObject(id);
	//
	return pVBO;
}

void GLRenderSystem::destoryVertexBufferObject(IVertexBufferObject *pVBO)
{
	//
	GLuint id = pVBO->id();
	glDeleteBuffers(1, &id);
	delete pVBO;
}

IVertexArrayObject *GLRenderSystem::createVertexArrayObject()
{
	GLuint id = 0;
	glGenVertexArrays(1, &id);
	IVertexArrayObject *pVAO = new GLVertexArrayObject(id);
	//
	return pVAO;
}

void GLRenderSystem::destoryVertexArrayObject(IVertexArrayObject *pVAO)
{
	//
	GLuint id = pVAO->id();
	glDeleteVertexArrays(1, &id);
	delete pVAO;
}

ITexture *GLRenderSystem::createTexture(GLenum target)
{
	GLuint id = 0;
	glGenTextures(1, &id);
	ITexture *pTexture = new GLTexture(target, id);
	//
	return pTexture;
}

void GLRenderSystem::destroyTexture(ITexture *pTexture)
{
	GLuint id = pTexture->id();
	glDeleteTextures(1, &id);
	delete pTexture;
}

IShader *GLRenderSystem::createShader(GLenum type)
{
	GLuint id = glCreateShader(type);
	IShader *pShader = new GLShader(id);
	//
	return pShader;
}

void GLRenderSystem::destroyShader(IShader *pShader)
{
	GLuint id = pShader->id();
	glDeleteShader(id);
	delete pShader;
}

IProgram *GLRenderSystem::createProgram()
{
	GLuint id = glCreateProgram();
	IProgram *pProgram = new GLProgram(id);
	//
	return pProgram;
}

void GLRenderSystem::destroyProgram(IProgram *pProgram)
{
	GLuint id = pProgram->id();
	glDeleteProgram(id);
	delete pProgram;
}

void GLRenderSystem::enable(GLenum cap) const
{
	enable(cap, 1);
}

void GLRenderSystem::disable(GLenum cap) const
{
	enable(cap, 0);
}

void GLRenderSystem::enable(GLenum cap, bool value)
{
	int capCacheId = mapGLCapability(cap);
	if (capCacheId < 0) {
		value ? glEnable(cap) : glDisable(cap);
		return;
	}
	if (m_cacheCapabilities[capCacheId] == value)
		return;
	m_cacheCapabilities[capCacheId] = value;
	value ? glEnable(cap) : glDisable(cap);
}

void GLRenderSystem::setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const
{
	glViewport(x, y, width, height);
}

void GLRenderSystem::setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const
{
	glClearColor(r, g, b, a);
}
void GLRenderSystem::setClearDepth(GLclampd value) const
{
	glClearDepth(value);
}

void GLRenderSystem::clearBuffers(GLbitfield mask) const
{
	glClear(mask);
}

void GLRenderSystem::drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const
{
	glDrawElements(mode, count, type, indices);
}

int GLRenderSystem::mapGLCapability(GLenum cap)
{
	switch (cap) {
		case GL_BLEND: return 0;
		case GL_CLIP_DISTANCE0: return 1;
		case GL_CLIP_DISTANCE1: return 2;
		case GL_CLIP_DISTANCE2: return 3;
		case GL_CLIP_DISTANCE3: return 4;
		case GL_CLIP_DISTANCE4: return 5;
		case GL_CLIP_DISTANCE5: return 6;
		case GL_COLOR_LOGIC_OP: return 7;
		case GL_CULL_FACE: return 8;
		case GL_DEBUG_OUTPUT: return 9;
		case GL_DEBUG_OUTPUT_SYNCHRONOUS: return 10;
		case GL_DEPTH_CLAMP: return 11;
		case GL_DEPTH_TEST: return 12;
		case GL_DITHER: return 13;
		case GL_FRAMEBUFFER_SRGB: return 14;
		case GL_LINE_SMOOTH: return 15;
		case GL_MULTISAMPLE: return 16;
		case GL_POLYGON_OFFSET_FILL: return 17;
		case GL_POLYGON_OFFSET_LINE: return 18;
		case GL_POLYGON_OFFSET_POINT: return 19;
		case GL_POLYGON_SMOOTH: return 20;
		case GL_PRIMITIVE_RESTART: return 21;
		case GL_PRIMITIVE_RESTART_FIXED_INDEX: return 22;
		case GL_RASTERIZER_DISCARD: return 23;
		case GL_SAMPLE_ALPHA_TO_COVERAGE: return 24;
		case GL_SAMPLE_ALPHA_TO_ONE: return 25;
		case GL_SAMPLE_COVERAGE: return 26;
		case GL_SAMPLE_SHADING: return 27;
		case GL_SAMPLE_MASK: return 28;
		case GL_SCISSOR_TEST: return 29;
		case GL_STENCIL_TEST: return 30;
		case GL_TEXTURE_CUBE_MAP_SEAMLESS: return 31;
		case GL_PROGRAM_POINT_SIZE: return 32;
		default: return -1;
	}
}
	
} // namespace