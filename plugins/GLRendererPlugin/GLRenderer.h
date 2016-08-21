#ifndef __GLRENDERER__
#define __GLRENDERER__

#include <list>

#include "renderer/IGLRenderer.h"
#include "renderer/IGLResource.h"

namespace FreeWorldEngine {

class GLRenderer : public IGLRenderer {
public:
	GLRenderer();
	~GLRenderer();

	void enable(GLenum cap, bool state = true) const;
	void disable(GLenum cap) const;
	bool isEnabled(GLenum cap) const;

	IGLBufferObject *generateBufferObject() const;
	IGLVertexArrayObject *generateVertexArrayObject() const;
	IGLShader *generateShader(const GLenum shaderType) const;
	IGLShaderProgram *generateShaderProgram() const;

	void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const;
	void getViewport(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const;

	void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const;
	void getClearColor(GLclampf& r, GLclampf& g, GLclampf& b, GLclampf& a) const;

	void setClearDepth(GLclampd value) const;
	void getClearDepth(GLclampd& value) const;

	void clearBuffers(GLbitfield mask) const;
	void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const;

private:
	static const int CACHE_CAPABILITIES_SIZE = 33 + 5;
	static bool m_cacheCapabilities[CACHE_CAPABILITIES_SIZE];

	static std::list<IGLResource*> m_cacheResources;
	static IGLResource *generateResource(const IGLResource::IGLResourceType resourceType, GLenum userData = 0);
	static void destroyResource(IGLResource *pResource);

	static int mapGLCapability(GLenum cap);

}; // class GLRenderer

}

#endif // __GLRENDERER__