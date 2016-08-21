#ifndef __RENDERSYSTEM__
#define __RENDERSYSTEM__

#include "IRenderSystem.h"

namespace FreeWorldEngine {

class GLRenderSystem : public IRenderSystem {
public:
	GLRenderSystem();
	~GLRenderSystem();

	IVertexBufferObject *createVertexBufferObject();
	void destoryVertexBufferObject(IVertexBufferObject *pVBO);
	IVertexArrayObject *createVertexArrayObject();
	void destoryVertexArrayObject(IVertexArrayObject *pVAO);
	ITexture *createTexture(GLenum target);
	void destroyTexture(ITexture *pTexture);
	IShader *createShader(GLenum type);
	void destroyShader(IShader *pShader);
	IProgram *createProgram();
	void destroyProgram(IProgram *pProgram);

	void enable(GLenum cap) const;
	void disable(GLenum cap) const;
	static void enable(GLenum cap, bool value);

	void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const;
	void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const;
	void setClearDepth(GLclampd value) const;
	void clearBuffers(GLbitfield mask) const;
	void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const;

private:

	static const int CACHE_CAPABILITIES_SIZE = 32;
	static bool m_cacheCapabilities[CACHE_CAPABILITIES_SIZE];

	static int mapGLCapability(GLenum cap);

}; // class RenderSystem

} // namespace

#endif // __RENDERSYSTEM__