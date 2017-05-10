#include "GLBuffer.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

namespace Renderer {

GLBuffer::GLBuffer(const GLuint id) :
	m_id(id)
{
}

GLBuffer::~GLBuffer()
{
}

uint64 GLBuffer::size() const
{
	GLint64 res;
	pGLRenderer->bindBuffer(this, GL_ARRAY_BUFFER);
	glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &res);
	return res;
}

void GLBuffer::resize(uint64 newSize, IGPUBufferUsage usage)
{
	pGLRenderer->bindBuffer(this, GL_COPY_READ_BUFFER);
	GLint64 oldSize;
	glGetBufferParameteri64v(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &oldSize);

	if (newSize == oldSize)
		return;

	GLuint newId;
	glGenBuffers(1, &newId);

	if (newSize) {
		GLenum newGLUsage;
		if (usage == IGPUBuffer::IGPUBufferUsage_OldValue)
			glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, (GLint*)&newGLUsage);
		else {
			newGLUsage = GLUsage(usage);
			if (!newGLUsage)
				newGLUsage = GL_STATIC_DRAW;
		}
		GLBuffer *pNewBuffer = new GLBuffer(newId);
		pGLRenderer->bindBuffer(pNewBuffer, GL_COPY_WRITE_BUFFER);
		glBufferData(GL_COPY_WRITE_BUFFER, newSize, 0, newGLUsage);
		uint64 copySize = (oldSize < newSize) ? oldSize : newSize;
		if (copySize)
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, copySize);
		pGLRenderer->bindBuffer(0, GL_COPY_WRITE_BUFFER);
		delete pNewBuffer;
	}
	
	pGLRenderer->bindBuffer(0, GL_COPY_READ_BUFFER);
	glDeleteBuffers(1, &m_id);
	m_id = newId;
}

void *GLBuffer::map(IGPUBufferAccess access, uint64 offset, uint64 length)
{
	pGLRenderer->bindBuffer(this, GL_ARRAY_BUFFER);
	if (length == 0) {
		GLint64 bufferSize;
		glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
		length = bufferSize - offset;
	}
	
	GLbitfield bufferAccess = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
	switch (access) {
	case IGPUBuffer::IGPUBufferAccess_ReadOnly: { bufferAccess = GL_MAP_READ_BIT; break; }
	case IGPUBuffer::IGPUBufferAccess_WriteOnly: { bufferAccess = GL_MAP_WRITE_BIT; break; }
	case IGPUBuffer::IGPUBufferAccess_ReadWrite: { bufferAccess = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT; break; }
	default: break;
	}

	return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, bufferAccess);
}

bool GLBuffer::unmap()
{
	return glUnmapBuffer(GL_ARRAY_BUFFER);
}

GLuint GLBuffer::GLid() const
{
	return m_id;
}

GLenum GLBuffer::GLUsage(IGPUBufferUsage usage)
{
	switch (usage) {
	case IGPUBufferUsage_StreamDraw: return GL_STREAM_DRAW;
	case IGPUBufferUsage_StreamRead: return GL_STREAM_READ;
	case IGPUBufferUsage_StreamCopy: return GL_STREAM_COPY;
	case IGPUBufferUsage_StaticDraw: return GL_STATIC_DRAW;
	case IGPUBufferUsage_StaticRead: return GL_STATIC_READ;
	case IGPUBufferUsage_StaticCopy: return GL_STATIC_COPY;
	case IGPUBufferUsage_DynamicDraw: return GL_DYNAMIC_DRAW;
	case IGPUBufferUsage_DynamicRead: return GL_DYNAMIC_READ;
	case IGPUBufferUsage_DynamicCopy: return GL_DYNAMIC_COPY;
	default: return 0;
	}
}

} // namespace

} // namespace