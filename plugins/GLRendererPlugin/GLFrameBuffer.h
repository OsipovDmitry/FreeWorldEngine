#ifndef __GLFRAMBUFFER__
#define __GLFRAMBUFFER__

#include <3rdparty/glew/glew.h>

#include <renderer/IGPUFrameBuffer.h>

namespace FreeWorldEngine {

class GLRenderBuffer : public IGPURenderBuffer {
public:
	GLRenderBuffer(const GLuint id);
	~GLRenderBuffer();

	TextureFormat format() const;
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

	void attachDepthBuffer(const IGPURenderBuffer *pRenderBuffer);
	void attachDepthBuffer(const IGPUTexture *pTexture, const uint32 layer = 0);

	void clearColorBuffer(const uint32 slot = 0, const float red = 0.0f, const float green = 0.0f, const float blue = 0.0f, const float alpha = 1.0f);
	void clearDepthBuffer(const float depth = 1.0f);

	GLuint GLid() const;

private:
	GLuint m_id;

	void attachRenderBuffer(GLenum attachment, const IGPURenderBuffer *pRenderBuffer);
	void attachTexture(GLenum attachment, const IGPUTexture *pTexture, const uint32 layer);

}; // class GLFrameBuffer

} // namespace

#endif // __GLFRAMBUFFER__