#include "GLBufferContainer.h"
#include "GLBuffer.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

namespace Renderer {

GLBufferContainer::GLBufferContainer(const GLuint id) :
	m_id(id)
{
}

GLBufferContainer::~GLBufferContainer()
{
}

void GLBufferContainer::setVertexAttribute(const IGPUBuffer *pGPUBuffer, uint32 indexAttribute, uint32 numComponents, int32 offsetAttribute, int32 strideAttribute, Type type) const
{
	pGLRenderer->bindBufferContainer(this);
	pGLRenderer->bindBuffer(static_cast<const GLBuffer*>(pGPUBuffer), GL_ARRAY_BUFFER);

	if (type == TYPE_FLOAT) {
		glVertexAttribPointer(indexAttribute, numComponents, GL_FLOAT, GL_FALSE, strideAttribute, (const void*)offsetAttribute);
	}
	else {
		GLenum GLtype = GLRenderer::GLType(type);
		if (GLtype)
			glVertexAttribIPointer(indexAttribute, numComponents, GLtype, strideAttribute, (const void*)offsetAttribute);
	}
}

void GLBufferContainer::enableVertexAttribute(uint32 indexAttribute) const
{
	pGLRenderer->bindBufferContainer(this);
	glEnableVertexAttribArray(indexAttribute);
}

void GLBufferContainer::disableVertexAttribute(uint32 indexAttribute) const
{
	pGLRenderer->bindBufferContainer(this);
	glDisableVertexAttribArray(indexAttribute);
}

void GLBufferContainer::setIndexBuffer(const IGPUBuffer *pGPUBuffer) const
{
	pGLRenderer->bindBufferContainer(this);
	pGLRenderer->bindBuffer(static_cast<const GLBuffer*>(pGPUBuffer), GL_ELEMENT_ARRAY_BUFFER);
}

GLuint GLBufferContainer::id() const
{
	return m_id;
}

} // namespace

} // namespace