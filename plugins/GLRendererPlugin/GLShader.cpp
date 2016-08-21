#include "GLShader.h"

namespace FreeWorldEngine {

GLShader::GLShader(GLuint id) :
	m_id(id)
{
}

GLShader::~GLShader()
{
}

GLuint GLShader::id() const
{
	return m_id;
}

IGLResource::IGLResourceType GLShader::type() const
{
	return IGLResource::IGLResourceType_Shader;
}

void GLShader::setSource(GLsizei count, const GLchar **string, const GLint *length) const
{
	glShaderSource(m_id, count, string, length);
}

void GLShader::compile() const
{
	glCompileShader(m_id);
}

void GLShader::getInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) const
{
	glGetShaderInfoLog(m_id, bufSize, length, infoLog);
}

} // namespace