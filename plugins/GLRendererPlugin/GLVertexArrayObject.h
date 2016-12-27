#ifndef __GLVERTEXARRAYOBJECT__
#define __GLVERTEXARRAYOBJECT__

#include "renderer/IGLVertexArrayObject.h"

namespace FreeWorldEngine {

class GLVertexArrayObject : public IGLVertexArrayObject {
public:
	GLVertexArrayObject(const GLuint id);
	~GLVertexArrayObject();

	GLuint id() const;
	IGLResource::IGLResourceType type() const;

	void setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const;

	void enableAttribArray(GLuint index) const;
	void disableAttribArray(GLuint index) const;

private:
	GLuint m_id;

	static GLuint m_cacheBinding;

	static void bind(GLuint id);

}; // class GLVertexArrayObject

} // namespace

#endif // __GLVERTEXARRAYOBJECT__