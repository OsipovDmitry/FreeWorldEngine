#ifndef __GLBUFFERCONTAINER__
#define __GLBUFFERCONTAINER__

#include <glew-1.13.0/include/GL/glew.h>
#include <renderer/IGPUBufferContainer.h>

namespace FreeWorldEngine {

class GLBufferContainer : public IGPUBufferContainer {
public:
	GLBufferContainer(const GLuint id);
	~GLBufferContainer();

	void setVertexAttribute(const IGPUBuffer *pGPUBuffer, uint32 indexAttribute, uint32 numComponents, int32 offsetAttribute, int32 strideAttribute, Type type) const;
	void enableVertexAttribute(uint32 indexAttribute) const;
	void disableVertexAttribute(uint32 indexAttribute) const;

	GLuint id() const;

private:
	GLuint m_id;

}; // class GLVertexArrayObject

} // namespace

#endif // __GLBUFFERCONTAINER__