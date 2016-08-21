#include "GLTexture.h"

namespace FreeWorldEngine {

GLuint GLTexture::m_cache[GLTexture::CACHE_SIZE] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

GLTexture::GLTexture(GLenum target, GLuint id) :
	m_id(id),
	m_target(target),
	m_unit(0)
{
	bind(m_target, m_id);
}

GLTexture::~GLTexture()
{
}

GLuint GLTexture::id() const
{
	return m_id;
}

GLenum GLTexture::target() const
{
	return m_target;
}

void GLTexture::bind(unsigned int unit) const
{
	m_unit = unit;
	bind(m_target, m_id, m_unit);
}

void GLTexture::unbind() const
{
	bind(m_target, 0, m_unit);
	m_unit = 0;
}

void GLTexture::bind(GLenum target, GLuint id, unsigned int unit)
{
	if (unit >= CACHE_SIZE) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, id);
		return;
	}
	if (GLTexture::m_cache[unit] == id)
		return;
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, GLTexture::m_cache[unit] = id);
}

} // namespace