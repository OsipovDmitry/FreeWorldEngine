#include "GLShaderProgram.h"

namespace FreeWorldEngine {

GLuint GLProgram::m_cache = 0;

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

GLProgram::GLProgram(GLuint id) :
	m_id(id)
{
}

GLProgram::~GLProgram()
{
}

GLuint GLProgram::id() const
{
	return m_id;
}

void GLProgram::attachShader(GLuint shader) const
{
	glAttachShader(m_id, shader);
}

void GLProgram::detachShader(GLuint shader) const
{
	glDetachShader(m_id, shader);
}

void GLProgram::link() const
{
	glLinkProgram(m_id);
}

void GLProgram::getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const
{
	glGetProgramInfoLog(m_id, bufSize, length, infoLog);
}

void GLProgram::bind() const
{
	bind(m_id);
}
void GLProgram::unbind() const
{
	bind(0);
}

void GLProgram::bind(GLuint id)
{
	if (GLProgram::m_cache == id)
		return;
	glUseProgram(GLProgram::m_cache = id);
}

}// namespace