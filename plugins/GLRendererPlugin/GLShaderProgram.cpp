#include "renderer/IGLShader.h"
#include "GLShaderProgram.h"

#include "glm/gtc/type_ptr.hpp"

namespace FreeWorldEngine {

GLuint GLShaderProgram::m_cacheBinding = 0;

GLShaderProgram::GLShaderProgram(const GLuint id) :
	m_id(id)
{
}

GLShaderProgram::~GLShaderProgram()
{
}

void GLShaderProgram::attachShader(IGLShader *pShader) const
{
	glAttachShader(m_id, pShader->id());
}

void GLShaderProgram::detachShader(IGLShader *pShader) const
{
	glDetachShader(m_id, pShader->id());
}

void GLShaderProgram::link() const
{
	glLinkProgram(m_id);
}

void GLShaderProgram::getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const
{
	glGetProgramInfoLog(m_id, bufSize, length, infoLog);
}

GLint GLShaderProgram::attributeLocation(const char *variable) const
{
	return glGetUniformLocation(m_id, variable);
}

GLint GLShaderProgram::uniformLocation(const char *variable) const
{
	return glGetAttribLocation(m_id, variable);
}

void GLShaderProgram::setUniform(const GLint loc, const GLint value) const
{
	bind(m_id);
	glUniform1i(loc, value);
}

void GLShaderProgram::setUniform(const GLint loc, const float value) const
{
	bind(m_id);
	glUniform1f(loc, value);
}

void GLShaderProgram::setUniformVector(const GLint loc, const GLint x, const GLint y) const
{
	bind(m_id);
	glUniform2i(loc, x, y);
}

void GLShaderProgram::setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z) const
{
	bind(m_id);
	glUniform3i(loc, x, y, z);
}

void GLShaderProgram::setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z, const GLint w) const
{
	bind(m_id);
	glUniform4i(loc, x, y, z, w);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::ivec2& value) const
{
	bind(m_id);
	glUniform2i(loc, value.x, value.y);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::ivec3& value) const
{
	bind(m_id);
	glUniform3i(loc, value.x, value.y, value.z);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::ivec4& value) const
{
	bind(m_id);
	glUniform4i(loc, value.x, value.y, value.z, value.w);
}

void GLShaderProgram::setUniformVector(const GLint loc, const float x, const float y) const
{
	bind(m_id);
	glUniform2f(loc, x, y);
}

void GLShaderProgram::setUniformVector(const GLint loc, const float x, const float y, const float z) const
{
	bind(m_id);
	glUniform3f(loc, x, y, z);
}

void GLShaderProgram::setUniformVector(const GLint loc, const float x, const float y, const float z, const float w) const
{
	bind(m_id);
	glUniform4f(loc, x, y, z, w);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::vec2& value) const
{
	bind(m_id);
	glUniform2f(loc, value.x, value.y);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::vec3& value) const
{
	bind(m_id);
	glUniform3f(loc, value.x, value.y, value.z);
}

void GLShaderProgram::setUniformVector(const GLint loc, const glm::vec4& value) const
{
	bind(m_id);
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void GLShaderProgram::setUniformMatrix(const GLint loc, const glm::mat2& value, const bool transpose) const
{
	bind(m_id);
	glUniformMatrix2fv(loc, 1, transpose, glm::value_ptr(value));
}

void GLShaderProgram::setUniformMatrix(const GLint loc, const glm::mat3& value, const bool transpose) const
{
	bind(m_id);
	glUniformMatrix3fv(loc, 1, transpose, glm::value_ptr(value));
}

void GLShaderProgram::setUniformMatrix(const GLint loc, const glm::mat4& value, const bool transpose) const
{
	bind(m_id);
	glUniformMatrix4fv(loc, 1, transpose, glm::value_ptr(value));
}

void GLShaderProgram::bind(GLuint id) const
{
	if (GLShaderProgram::m_cacheBinding == id)
		return;
	glUseProgram(GLShaderProgram::m_cacheBinding = id);
}

} // namespace