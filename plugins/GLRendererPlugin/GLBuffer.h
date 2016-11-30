#ifndef __GLBUFFER__
#define __GLBUFFER__

#include <glew-1.13.0/include/GL/glew.h>

#include <renderer/IGPUBuffer.h>

namespace FreeWorldEngine {

class GLBuffer : public IGPUBuffer {
public:
	GLBuffer(const GLuint id);
	~GLBuffer();

	uint64 size() const;
	void resize(uint64 newSize, IGPUBufferUsage usage = IGPUBufferUsage_OldValue);

	void *map(IGPUBufferAccess access, uint64 offset = 0, uint64 length = 0);
	bool unmap();

	GLuint GLid() const;
	
	static GLenum toGLUsage(IGPUBufferUsage usage);

private:
	GLuint m_id;

}; // class GLBuffer

} // namespace

#endif // __GLBUFFER__