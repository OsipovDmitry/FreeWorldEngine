#ifndef __GLBUFFER__
#define __GLBUFFER__

#include <3rdparty/glew/glew.h>

#include <renderer/IGPUBuffer.h>

namespace FreeWorldEngine {

namespace Renderer {

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

} // namespace

#endif // __GLBUFFER__