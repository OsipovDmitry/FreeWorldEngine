#ifndef __IRENDERSYSTEM__
#define __IRENDERSYSTEM__

#include "glew-1.13.0/include/gl/glew.h"

namespace FreeWorldEngine {

class IVertexBufferObject;
class IVertexArrayObject;
class ITexture;
class ISampler;
class IShader;
class IProgram;

class IRenderSystem {
public:
	virtual ~IRenderSystem() = 0 {}

	virtual IVertexBufferObject *createVertexBufferObject() = 0;
	virtual IVertexArrayObject *createVertexArrayObject() = 0;
	virtual ITexture *createTexture(GLenum target) = 0;
	virtual IShader *createShader(GLenum type) = 0;
	virtual IProgram *createProgram() = 0;
	virtual void destoryVertexBufferObject(IVertexBufferObject *pVBO) = 0;
	virtual void destoryVertexArrayObject(IVertexArrayObject *pVAO) = 0;
	virtual void destroyTexture(ITexture *pTexture) = 0;
	virtual void destroyShader(IShader *pShader) = 0;
	virtual void destroyProgram(IProgram *pProgram) = 0;

	virtual void enable(GLenum cap) const = 0;
	virtual void disable(GLenum cap) const = 0;

	virtual void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const = 0;
	virtual void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const = 0;
	virtual void setClearDepth(GLclampd value) const = 0;
	virtual void clearBuffers(GLbitfield mask) const = 0;
	virtual void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const = 0;

}; // class IRenderSystem

} // namespace

#endif // __IRENDERSYSTEM__