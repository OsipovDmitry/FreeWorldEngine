#ifndef __GLSHADERPROGRAM__
#define __GLSHADERPROGRAM__

#include "IShaderProgram.h"

namespace FreeWorldEngine {

class GLShader : public IShader {
public:
	GLShader(GLuint id);
	~GLShader();

	GLuint id() const;
	void setSource(GLsizei count, const GLchar **string, const GLint *length) const;
	void compile() const;
	void getInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) const;

private:
	GLuint m_id;

}; // class GLShader


class GLProgram : public IProgram {
public:
	GLProgram(GLuint id);
	~GLProgram();

	GLuint id() const;
	void attachShader(GLuint shader) const;
	void detachShader(GLuint shader) const;
	void link() const;
	void getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const;
	void bind() const;
	void unbind() const;

	static void bind(GLuint id);

private:
	GLuint m_id;
	static GLuint m_cache;

}; // class GLProgram

} // namespace

#endif // __GLSHADERPROGRAM__