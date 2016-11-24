#include "GLBufferContainer.h"

namespace FreeWorldEngine {

GLBufferContainer::GLBufferContainer(const GLuint id) :
	m_id(id)
{
}

GLBufferContainer::~GLBufferContainer()
{
}

GLuint GLBufferContainer::id() const
{
	return m_id;
}

} // namespace