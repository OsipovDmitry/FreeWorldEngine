#include "GLSampler.h"

namespace FreeWorldEngine {

GLuint GLSampler::m_cache[GLSampler::CACHE_SIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

GLSampler::GLSampler() :
	m_id(0),
	m_unit(0)
{
}

GLSampler::~GLSampler()
{
}

void GLSampler::create()
{
	glGenSamplers(1, &m_id);
}

void GLSampler::clear()
{
	glDeleteSamplers(1, &m_id);
}

GLuint GLSampler::id() const
{
	return m_id;
}

void GLSampler::bind(unsigned int unit) const
{
	m_unit = unit;
	GLSampler::bind(m_id, m_unit);
}

void GLSampler::unbind() const
{
	GLSampler::bind(0, m_unit);
	m_unit = 0;
}

void GLSampler::bind(GLuint id, unsigned int unit)
{
	if (unit >= GLSampler::CACHE_SIZE) {
		glBindSampler(unit, id);
		return;
	}
	if (GLSampler::m_cache[unit] == id)
		return;
	glBindSampler(unit, GLSampler::m_cache[unit] = id);
}

bool GLSampler::isSampler(GLuint id)
{
	return (bool)glIsSampler(id);
}

void GLSampler::setParameter(GLuint id, GLenum pname, GLint param)
{
	glSamplerParameteri(id, pname, param);
}

void GLSampler::setParameter(GLuint id, GLenum pname, GLfloat param)
{
	glSamplerParameterf(id, pname, param);
}

void GLSampler::setParameter(GLuint id, GLenum pname, const GLint *param)
{
	glSamplerParameteriv(id, pname, param);
}

void GLSampler::setParameter(GLuint id, GLenum pname, const GLfloat *param)
{
	glSamplerParameterfv(id, pname, param);
}

void GLSampler::setParameterI(GLuint id, GLenum pname, const GLint *param)
{
	glSamplerParameterIiv(id, pname, param);
}

void GLSampler::setParameterI(GLuint id, GLenum pname, const GLuint *param)
{
	glSamplerParameterIuiv(id, pname, param);
}


} // namespace