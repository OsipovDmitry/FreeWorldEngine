#ifndef __GLFRAMBUFFER__
#define __GLFRAMBUFFER__

#include <glew-1.13.0/include/GL/glew.h>

#include <renderer/IGPUFrameBuffer.h>

namespace FreeWorldEngine {

class GLRenderBuffer : public IGPURenderBuffer {
public:
	GLRenderBuffer(const GLuint id);
	~GLRenderBuffer();

	uint32 width() const;
	uint32 height() const;
	void resize(const uint32 width, const uint32 height);

	GLuint GLid() const;

private:
	GLuint m_id;

}; // class GLRnederBuffer

class GLFrameBuffer : public IGPUFrameBuffer {
public:
	GLFrameBuffer(const GLuint id);
	~GLFrameBuffer();

	void attachColorBuffer(const uint32 slot, const IGPURenderBuffer *pRenderBuffer);
	void attachColorBuffer(const uint32 slot, const IGPUTexture *pTexture, const uint32 layer = 0);

	GLuint GLid() const;

private:
	GLuint m_id;

}; // class GLFrameBuffer

} // namespace

#endif // __GLFRAMBUFFER__