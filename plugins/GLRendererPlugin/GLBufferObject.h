#ifndef __GLVERTEXBUFFEROBJECT__
#define __GLVERTEXBUFFEROBJECT__

#include "renderer/IGLBufferObject.h"

namespace FreeWorldEngine {

class GLBufferObject : public IGLBufferObject {
public:
	GLBufferObject(const GLuint id);
	~GLBufferObject();

	GLuint id() const;
	IGLResource::IGLResourceType type() const;

	void setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage) const;
	void storage(GLenum target, GLsizeiptr size, GLenum usage) const;

	void setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr) const;
	void getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr) const;

	void *map(GLenum target, GLenum access) const;
	bool unmap(GLenum target) const;

	void copySubData(IGLBufferObject *targetBuffer, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)	const;

private:
	GLuint m_id;

	static const int CACHE_BINDINGS_SIZE = 10 + 5;
	static GLuint m_cacheBindings[CACHE_BINDINGS_SIZE];
	static int mapTarget(GLenum target);

	static void bind(GLenum target, GLuint id);

}; // class GLVertexBufferObject

} // namespace

#endif // __GLVERTEXBUFFEROBJECT__