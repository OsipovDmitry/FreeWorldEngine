#include "GLOcclusionQuery.h"

namespace FreeWorldEngine {

namespace Renderer {

GLOcclusionQuery::GLOcclusionQuery(const GLuint id) :
	m_id(id)
{
}

GLOcclusionQuery::~GLOcclusionQuery()
{
}

void GLOcclusionQuery::begin() const
{
	glBeginQuery(GL_SAMPLES_PASSED, m_id);
}

void GLOcclusionQuery::end() const
{
	glEndQuery(GL_SAMPLES_PASSED);
}

bool GLOcclusionQuery::isReady() const
{
	GLuint res;
	glGetQueryObjectuiv(m_id, GL_QUERY_RESULT_AVAILABLE, &res);
	return res == GL_TRUE;
}

uint32 GLOcclusionQuery::result() const
{
	GLuint res;
	glGetQueryObjectuiv(m_id, GL_QUERY_RESULT, &res);
	return res;
}

GLuint GLOcclusionQuery::GLid() const
{
	return m_id;
}


} // namespace
} // namespace