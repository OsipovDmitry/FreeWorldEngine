#include "GLVertexBufferObject.h"

namespace FreeWorldEngine {

GLuint GLVertexBufferObject::m_cache[10+5] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

GLVertexBufferObject::GLVertexBufferObject(GLuint id) :
	m_id(id)
{
}

GLVertexBufferObject::~GLVertexBufferObject()
{
}

GLuint GLVertexBufferObject::id() const
{
	return m_id;
}

void GLVertexBufferObject::bind(GLenum target) const
{
	bind(target, m_id);
}

void GLVertexBufferObject::unbind(GLenum target) const
{
	bind(target, 0);
}

void GLVertexBufferObject::bind(GLenum target, GLuint id)
{
	int targetCacheId = mapTarget(target);
	if (targetCacheId == -1) {
		glBindBuffer(target, id);
		return;
	}
	if (GLVertexBufferObject::m_cache[targetCacheId] == id)
		return;
	glBindBuffer(target, GLVertexBufferObject::m_cache[targetCacheId] = id);
}

int GLVertexBufferObject::mapTarget(GLenum target)
{
	switch (target) {
		case GL_ARRAY_BUFFER : return 0;
		case GL_COPY_READ_BUFFER : return 1;
		case GL_COPY_WRITE_BUFFER : return 2;
		case GL_DRAW_INDIRECT_BUFFER: return 3;
		case GL_ELEMENT_ARRAY_BUFFER: return 4;
		case GL_PIXEL_PACK_BUFFER: return 5;
		case GL_PIXEL_UNPACK_BUFFER: return 6;
		case GL_TEXTURE_BUFFER: return 7;
		case GL_TRANSFORM_FEEDBACK_BUFFER: return 8;
		case GL_UNIFORM_BUFFER: return 9;
		default: return -1;
	}
}

} // namespace