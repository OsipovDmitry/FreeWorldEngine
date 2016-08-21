#ifndef __GLSHADER__
#define __GLSHADER__

#include "renderer/IGLShader.h"

namespace FreeWorldEngine {

class GLShader : public IGLShader {
public:
	GLShader(GLuint id);
	~GLShader();

	GLuint id() const;
	IGLResource::IGLResourceType type() const;

	void setSource(GLsizei count, const GLchar **string, const GLint *length) const;
	void compile() const;
	void getInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) const;

private:
	GLuint m_id;

}; // class GLShader

} // namespace

#endif // __GLSHADER__