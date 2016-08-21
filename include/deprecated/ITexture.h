#ifndef __ITEXTURE__
#define __ITEXTURE__

#include "glew-1.13.0/include/gl/glew.h"
#include "Settings.h"

namespace FreeWorldEngine {

class ITexture {
public:
	virtual ~ITexture() = 0 {}

	virtual GLuint id() const = 0;
	virtual GLenum target() const = 0;
	virtual void bind(unsigned int unit = 0) const = 0;
	virtual void unbind() const = 0;

	FREEWORLDENGINE_FUNC_DLL static bool isTexture(GLuint id);
	FREEWORLDENGINE_FUNC_DLL static void storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width);
	FREEWORLDENGINE_FUNC_DLL static void storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
	FREEWORLDENGINE_FUNC_DLL static void storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth);
	FREEWORLDENGINE_FUNC_DLL static void storageMultisample(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	FREEWORLDENGINE_FUNC_DLL static void storageMultisample(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	FREEWORLDENGINE_FUNC_DLL static void loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void loadImageMultisample(GLenum target, GLenum samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
	FREEWORLDENGINE_FUNC_DLL static void loadImageMultisample(GLenum target, GLenum samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
	FREEWORLDENGINE_FUNC_DLL static void loadSubImage(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void loadSubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void loadSubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
	FREEWORLDENGINE_FUNC_DLL static void copyImage(GLenum target, GLint level, GLint internalFormat, GLint x, GLint y, GLsizei width);
	FREEWORLDENGINE_FUNC_DLL static void copyImage(GLenum target, GLint level, GLint internalFormat, GLint x, GLint y, GLsizei width, GLsizei height);
	FREEWORLDENGINE_FUNC_DLL static void copySubImage(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
	FREEWORLDENGINE_FUNC_DLL static void copySubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	FREEWORLDENGINE_FUNC_DLL static void copySubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	FREEWORLDENGINE_FUNC_DLL static void getImage(GLenum target, GLint lod, GLenum format, GLenum type, GLvoid* image);
	FREEWORLDENGINE_FUNC_DLL static void pixelStore(GLenum pname, GLint param);
	FREEWORLDENGINE_FUNC_DLL static void pixelStore(GLenum pname, GLfloat param);
	FREEWORLDENGINE_FUNC_DLL static void setParameter(GLenum target, GLenum pname, GLint param);
	FREEWORLDENGINE_FUNC_DLL static void setParameter(GLenum target, GLenum pname, GLfloat param);
	FREEWORLDENGINE_FUNC_DLL static void setParameter(GLenum target, GLenum pname, const GLint *param);
	FREEWORLDENGINE_FUNC_DLL static void setParameter(GLenum target, GLenum pname, const GLfloat *param);
	FREEWORLDENGINE_FUNC_DLL static void setParameterI(GLenum target, GLenum pname, const GLint *param);
	FREEWORLDENGINE_FUNC_DLL static void setParameterI(GLenum target, GLenum pname, const GLuint *param);
	FREEWORLDENGINE_FUNC_DLL static void setBuffer(GLenum target, GLenum internalFormat, GLuint buffer);
	FREEWORLDENGINE_FUNC_DLL static void setBufferRange(GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size);
	FREEWORLDENGINE_FUNC_DLL static void generateMipmap(GLenum target);

}; // class ITexture

} // namespace

#endif // __ITEXTURE__