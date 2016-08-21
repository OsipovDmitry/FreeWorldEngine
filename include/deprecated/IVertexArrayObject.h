#ifndef __IVERTEXARRAYOBJECT__
#define __IVERTEXARRAYOBJECT__

#include "glew-1.13.0/include/gl/glew.h"
#include "Settings.h"

namespace FreeWorldEngine {

class IVertexArrayObject {
public:
	virtual ~IVertexArrayObject() = 0 {}

	virtual GLuint id() const = 0;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	FREEWORLDENGINE_FUNC_DLL static bool isVertexArray(GLuint id);
	FREEWORLDENGINE_FUNC_DLL static void setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
	FREEWORLDENGINE_FUNC_DLL static void setAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	FREEWORLDENGINE_FUNC_DLL static void setAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
	FREEWORLDENGINE_FUNC_DLL static void enableAttribArray(GLuint index);
	FREEWORLDENGINE_FUNC_DLL static void disableAttribArray(GLuint index);
	
}; // class IVertexArrayObject

} // namespace

#endif // __IVERTEXARRAYOBJECT__