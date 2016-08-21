#include <algorithm>

#include "GLRenderer.h"
#include "GLBufferObject.h"
#include "GLVertexArrayObject.h"
#include "GLShader.h"
#include "GLShaderProgram.h"

namespace FreeWorldEngine {

bool GLRenderer::m_cacheCapabilities[GLRenderer::CACHE_CAPABILITIES_SIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
std::list<IGLResource*> GLRenderer::m_cacheResources;

GLRenderer::GLRenderer()
{
	m_cacheResources.clear();
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::enable(GLenum cap, bool state) const
{
	int capCacheId = mapGLCapability(cap);
	if (capCacheId < 0) {
		state ? glEnable(cap) : glDisable(cap);
		return;
	}
	if (m_cacheCapabilities[capCacheId] == state)
		return;
	m_cacheCapabilities[capCacheId] = state;
	state ? glEnable(cap) : glDisable(cap);
}

void GLRenderer::disable(GLenum cap) const
{
	enable(cap, false);
}

bool GLRenderer::isEnabled(GLenum cap) const
{
	int capCacheId = mapGLCapability(cap);
	if (capCacheId < 0)
		return (bool)glIsEnabled(cap);
	return m_cacheCapabilities[capCacheId];
}

IGLBufferObject *GLRenderer::generateBufferObject() const
{
	return (IGLBufferObject*)generateResource(IGLResource::IGLResourceType_BufferObject);
}

IGLVertexArrayObject *GLRenderer::generateVertexArrayObject() const
{
	return (IGLVertexArrayObject*)generateResource(IGLResource::IGLResourceType_VertexArrayObject);
}

IGLShader *GLRenderer::generateShader(const GLenum shaderType) const
{
	return (IGLShader*)generateResource(IGLResource::IGLResourceType_Shader, shaderType);
}

IGLShaderProgram *GLRenderer::generateShaderProgram() const
{
	return (IGLShaderProgram*)generateResource(IGLResource::IGLResourceType_ShaderProgram);
}

void GLRenderer::setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const
{
	glViewport(x, y, width, height);
}

void GLRenderer::getViewport(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x = vp[0];
	y = vp[1];
	width = vp[2];
	height= vp[3];
}

void GLRenderer::setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const
{
	glClearColor(r, g, b, a);
}

void GLRenderer::getClearColor(GLclampf& r, GLclampf& g, GLclampf& b, GLclampf& a) const
{
	float cc[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, cc);
	r = cc[0];
	g = cc[1];
	b = cc[2];
	a = cc[3];
}

void GLRenderer::setClearDepth(GLclampd value) const
{
	glClearDepth(value);
}

void GLRenderer::getClearDepth(GLclampd& value) const
{
	glGetDoublev(GL_DEPTH_CLEAR_VALUE, &value);
}

void GLRenderer::clearBuffers(GLbitfield mask) const
{
	glClear(mask);
}

void GLRenderer::drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const
{
	glDrawElements(mode, count, type, indices);
}

IGLResource *GLRenderer::generateResource(const IGLResource::IGLResourceType resourceType, GLenum userData)
{
	IGLResource *pResource = 0;
	GLuint id = 0;
	switch (resourceType) {
	case IGLResource::IGLResourceType_BufferObject: {
		glGenBuffers(1, &id);
		pResource = new GLBufferObject(id);
		break;
													}
	case IGLResource::IGLResourceType_VertexArrayObject: {
		glGenVertexArrays(1, &id);
		pResource = new GLVertexArrayObject(id);
		break;
														 }
	case IGLResource::IGLResourceType_Shader: {
		id = glCreateShader(userData);
		pResource = new GLShader(id);
		break;
											  }
	case IGLResource::IGLResourceType_ShaderProgram:
	case IGLResource::IGLResourceType_Texture:
	case IGLResource::IGLResourceType_Sampler:
	case IGLResource::IGLResourceType_FrameBufferObject:
	case IGLResource::IGLResourceType_RenderBufferObject:

	case IGLResource::IGLResourceType_None:
	default: break;
	}

	if (!pResource)
		return 0;

	m_cacheResources.push_back(pResource);
	return pResource;
}

void GLRenderer::destroyResource(IGLResource *pResource)
{
	GLuint id = pResource->id();
	IGLResource::IGLResourceType type = pResource->type();

	if (!id || (type == IGLResource::IGLResourceType_None))
		return;

	std::list<IGLResource*>::iterator it = std::find(m_cacheResources.begin(), m_cacheResources.end(), pResource);
	if (it == m_cacheResources.end())
		return;
	m_cacheResources.erase(it);

	switch (type) {
	case IGLResource::IGLResourceType_BufferObject: {
		glDeleteBuffers(1, &id);
		break;
													}
	case IGLResource::IGLResourceType_VertexArrayObject: {
		glDeleteVertexArrays(1, &id);
		break;
														 }
	case IGLResource::IGLResourceType_Shader: {
		glDeleteShader(id);
		break;
											  }
	case IGLResource::IGLResourceType_ShaderProgram: {
		glDeleteProgram(id);
		break;
													 }
	case IGLResource::IGLResourceType_Texture: {
		glDeleteTextures(1, &id);
		break;
											   }
	case IGLResource::IGLResourceType_Sampler: {
		glDeleteSamplers(1, &id);
		break;
											   }
	case IGLResource::IGLResourceType_FrameBufferObject: {
		glDeleteFramebuffers(1, &id);
		break;
														 }
	case IGLResource::IGLResourceType_RenderBufferObject: {
		glDeleteRenderbuffers(1, &id);
		break;
														  }
	default: break;
	}

	delete pResource;
}

int GLRenderer::mapGLCapability(GLenum cap)
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