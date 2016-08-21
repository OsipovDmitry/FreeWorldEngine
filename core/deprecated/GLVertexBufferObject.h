#ifndef __VERTEXBUFFER__
#define __VERTEXBUFFER__

#include "IVertexBufferObject.h"

namespace FreeWorldEngine {

class GLVertexBufferObject : public IVertexBufferObject {
public:
	GLVertexBufferObject(GLuint id);
	~GLVertexBufferObject();

	GLuint id() const;
	void bind(GLenum target) const;
	void unbind(GLenum target) const;

	static void bind(GLenum target, GLuint id);

private:
	GLuint m_id;

	static int mapTarget(GLenum target);
	static GLuint m_cache[10+5];

}; // class VertexBufferObject

} // namespace

#endif // __VERTEXBUFFER__