#ifndef __IGLSHADER__
#define __IGLSHADER__

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLShader : public IGLResource {
public:
	virtual ~IGLShader() = 0 {}

	virtual void setSource(GLsizei count, const GLchar **string, const GLint *length) const = 0;
	virtual void compile() const = 0;
	virtual void getInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, char *infoLog) const = 0;

}; // class IGLShader

} // namespace

#endif // __IGLSHADER__