#include "GLVertexArrayObject.h"

namespace FreeWorldEngine {

GLuint GLVertexArrayObject::m_cache = 0;

GLVertexArrayObject::GLVertexArrayObject(GLuint id) :
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

void GLVertexArrayObject::bind() const
{
	GLVertexArrayObject::bind(m_id);
}

void GLVertexArrayObject::unbind() const
{
	GLVertexArrayObject::bind(0);
}

void GLVertexArrayObject::bind(GLuint id)
{
	if (GLVertexArrayObject::m_cache == id)
		return;
	glBindVertexArray(GLVertexArrayObject::m_cache = id);
}



} // namespace