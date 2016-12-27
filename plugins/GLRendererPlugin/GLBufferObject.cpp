#include "GLBufferObject.h"

namespace FreeWorldEngine {

GLuint GLBufferObject::m_cacheBindings[GLBufferObject::CACHE_BINDINGS_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

GLBufferObject::GLBufferObject(const GLuint id) :
	m_id(id)
{
}

GLBufferObject::~GLBufferObject()
{
}

GLuint GLBufferObject::id() const
{
	return m_id;
}

IGLResource::IGLResourceType GLBufferObject::type() const
{
	return IGLResource::IGLResourceType_BufferObject;
}

void GLBufferObject::setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage) const
{
	bind(target, m_id);
	glBufferData(target, size, ptr, usage);
}

void GLBufferObject::storage(GLenum target, GLsizeiptr size, GLenum usage) const
{
	setData(target, size, 0, usage);
}

void GLBufferObject::setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr) const
{
	bind(target, m_id);
	glBufferSubData(target, offs, size, ptr);
}

void GLBufferObject::getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr) const
{
	bind(target, m_id);
	glGetBufferSubData(target, offs, size, ptr);
}

void *GLBufferObject::map(GLenum target, GLenum access) const
{
	bind(target, m_id);
	return (void*)glMapBuffer(target, access);
}

bool GLBufferObject::unmap(GLenum target) const
{
	return (bool)glUnmapBuffer(target);
}

void GLBufferObject::copySubData(IGLBufferObject *targetBuffer, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)	const
{
	bind(GL_COPY_READ_BUFFER, m_id);
	bind(GL_COPY_WRITE_BUFFER, targetBuffer->id());
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readoffset, writeoffset, size);
}

int GLBufferObject::mapTarget(GLenum target)
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

void GLBufferObject::bind(GLenum target, GLuint id)
{
	int targetCacheId = mapTarget(target);
	if (targetCacheId == -1) {
		glBindBuffer(target, id);
		return;
	}
	if (GLBufferObject::m_cacheBindings[targetCacheId] == id)
		return;
	glBindBuffer(target, GLBufferObject::m_cacheBindings[targetCacheId] = id);
}

} // namespace