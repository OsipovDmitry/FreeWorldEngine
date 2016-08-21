#ifndef __IVERTEXBUFFER__
#define __IVERTEXBUFFER__

#include "glew-1.13.0/include/gl/glew.h"
#include "Settings.h"

namespace FreeWorldEngine {

class IVertexBufferObject {
public:
	virtual ~IVertexBufferObject() = 0 {}

	virtual GLuint id() const = 0;
	virtual void bind(GLenum target) const = 0;
	virtual void unbind(GLenum target) const = 0;

	FREEWORLDENGINE_FUNC_DLL static void setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage);
	FREEWORLDENGINE_FUNC_DLL static void setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr);
	FREEWORLDENGINE_FUNC_DLL static void getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr);
	FREEWORLDENGINE_FUNC_DLL static void clearData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void clearSubData(GLenum target, GLenum internalformat, GLintptr offset, GLintptr size, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void copySubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
	FREEWORLDENGINE_FUNC_DLL static void *map(GLenum target, GLenum access);
	FREEWORLDENGINE_FUNC_DLL static bool unmap(GLenum target);
	FREEWORLDENGINE_FUNC_DLL static void *mapRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
	FREEWORLDENGINE_FUNC_DLL static void flushMappedRange(GLenum target, GLintptr offset, GLsizeiptr length);
	FREEWORLDENGINE_FUNC_DLL static bool isBuffer(GLuint id);
	FREEWORLDENGINE_FUNC_DLL static void bindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
	FREEWORLDENGINE_FUNC_DLL static void bindBufferBase(GLenum target, GLuint index, GLuint buffer);

}; // class IVertexBufferObject

} // namespace

#endif // __IVERTEXBUFFER__