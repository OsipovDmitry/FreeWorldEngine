#ifndef __IGLVERTEXARRAYOBJECT__
#define __IGLVERTEXARRAYOBJECT__

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLVertexArrayObject : public IGLResource {
public:
	virtual ~IGLVertexArrayObject() = 0 {}

	virtual void setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const = 0; // ”становка вершинного атрибута

	virtual void enableAttribArray(GLuint index) const = 0; // –азрешить использовать вершинный атрибут
	virtual void disableAttribArray(GLuint index) const = 0; // «апретить использовать вершинный атрибут

}; // class IGLVertexArrayObject

} // namespace

#endif // __IGLVERTEXARRAYOBJECT__