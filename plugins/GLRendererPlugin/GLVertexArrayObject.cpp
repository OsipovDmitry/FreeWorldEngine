#include "GLVertexArrayObject.h"

namespace FreeWorldEngine {

GLuint GLVertexArrayObject::m_cacheBinding = 0;

GLVertexArrayObject::GLVertexArrayObject(const GLuint id) :
	m_id(id)
{
}

GLVertexArrayObject::~GLVertexArrayObject()
{
}

GLuint GLVertexArrayObject::id() const
{
	return m_id;
}

IGLResource::IGLResourceType GLVertexArrayObject::type() const
{
	return IGLResource::IGLResourceType_VertexArrayObject;
}

void GLVertexArrayObject::setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const
{
	bind(m_id);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void GLVertexArrayObject::enableAttribArray(GLuint index) const
{
	bind(m_id);
	glEnableVertexAttribArray(index);
}

void GLVertexArrayObject::disableAttribArray(GLuint index) const
{
	bind(m_id);
	glDisableVertexAttribArray(index);
}

void GLVertexArrayObject::bind(GLuint id)
{
	if (GLVertexArrayObject::m_cacheBinding == id)
		return;
	glBindVertexArray(GLVertexArrayObject::m_cacheBinding = id);
}



} // namespace