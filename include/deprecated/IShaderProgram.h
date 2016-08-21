#ifndef __ISHADERPROGRAM__
#define __ISHADERPROGRAM__

#include "glew-1.13.0/include/gl/glew.h"
#include "Settings.h"

namespace FreeWorldEngine {

class IShader {
public:
	virtual ~IShader() = 0 {}

	virtual GLuint id() const = 0;
	virtual void setSource(GLsizei count, const GLchar **string, const GLint *length) const = 0;
	virtual void compile() const = 0;
	virtual void getInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) const = 0;

	FREEWORLDENGINE_FUNC_DLL bool isShader(GLuint id);

}; // class IShader

class IProgram {
public:
	virtual ~IProgram() = 0 {}

	virtual GLuint id() const = 0;
	virtual void attachShader(GLuint shader) const = 0;
	virtual void detachShader(GLuint shader) const = 0;
	virtual void link() const = 0;
	virtual void getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const = 0;
	virtual void bind() const = 0;
	virtual void unbind() const = 0;

	FREEWORLDENGINE_FUNC_DLL static GLint getAttribLocation(GLuint id, const char *name);
	FREEWORLDENGINE_FUNC_DLL static GLint getUniformLocation(GLuint id, const char *name);
	FREEWORLDENGINE_FUNC_DLL static GLuint getUniformBlockIndex(GLuint id, const char *uniformBlockName);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, float v0);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, float v0, float v1);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, float v0, float v1, const float v2);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, float v0, float v1, const float v2, const float v3);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, double v0);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, double v0, double v1);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, double v0, double v1, const double v2);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, double v0, double v1, const double v2, const double v3);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, int v0);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, int v0, int v1);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, int v0, int v1, const int v2);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, int v0, int v1, const int v2, const int v3);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, unsigned int v0);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, unsigned int v0, unsigned int v1);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, unsigned int v0, unsigned int v1, const unsigned int v2);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, unsigned int v0, unsigned int v1, const unsigned int v2, const unsigned int v3);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, GLsizei count, float *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, GLsizei count, float *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, GLsizei count, float *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, GLsizei count, float *p);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, GLsizei count, double *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, GLsizei count, double *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, GLsizei count, double *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, GLsizei count, double *p);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, GLsizei count, int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, GLsizei count, int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, GLsizei count, int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, GLsizei count, int *p);

	FREEWORLDENGINE_FUNC_DLL static void setUniform1(GLint location, GLsizei count, unsigned int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform2(GLint location, GLsizei count, unsigned int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform3(GLint location, GLsizei count, unsigned int *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniform4(GLint location, GLsizei count, unsigned int *p);

	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix2(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix3(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p);

	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix2(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix3(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p);
	FREEWORLDENGINE_FUNC_DLL static void setUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p);

	FREEWORLDENGINE_FUNC_DLL static void uniformBlockBinding(GLuint id, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	FREEWORLDENGINE_FUNC_DLL static void getUniformIndices(GLuint id, GLsizei uniformCount, const char **uniformNames, GLuint *uniformIndices);

	FREEWORLDENGINE_FUNC_DLL static bool isProgram(GLuint id);

}; // class IProgram

} // namespace

#endif // __ISHADERPROGRAM__