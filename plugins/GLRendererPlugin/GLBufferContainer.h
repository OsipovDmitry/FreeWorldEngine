#ifndef __GLBUFFERCONTAINER__
#define __GLBUFFERCONTAINER__

#include <glew-1.13.0/include/GL/glew.h>
#include <renderer/IGPUBufferContainer.h>

namespace FreeWorldEngine {

class GLBufferContainer : public IGPUBufferContainer {
public:
	GLBufferContainer(const GLuint id);
	~GLBufferContainer();

	GLuint id() const;

private:
	GLuint m_id;

}; // class GLVertexArrayObject

} // namespace

#endif // __GLBUFFERCONTAINER__