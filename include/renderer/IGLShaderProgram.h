#ifndef __IGLSHADERPROGRAM__
#define __IGLSHADERPROGRAM__

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat2x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLShader;

class IGLShaderProgram : public IGLResource {
public:
	virtual ~IGLShaderProgram() = 0 {}

	virtual void attachShader(IGLShader *pShader) const = 0;
	virtual void detachShader(IGLShader *pShader) const = 0;
	virtual void link() const = 0;
	virtual void getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const = 0;

	virtual GLint attributeLocation(const char *variable) const = 0;
	virtual GLint uniformLocation(const char *variable) const = 0;

	virtual void setUniform(const GLint loc, const GLint value) const = 0;
	virtual void setUniform(const GLint loc, const float value) const = 0;

	virtual void setUniformVector(const GLint loc, const GLint x, const GLint y) const = 0;
	virtual void setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z) const = 0;
	virtual void setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z, const GLint w) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::ivec2& value) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::ivec3& value) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::ivec4& value) const = 0;
	virtual void setUniformVector(const GLint loc, const float x, const float y) const = 0;
	virtual void setUniformVector(const GLint loc, const float x, const float y, const float z) const = 0;
	virtual void setUniformVector(const GLint loc, const float x, const float y, const float z, const float w) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::vec2& value) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::vec3& value) const = 0;
	virtual void setUniformVector(const GLint loc, const glm::vec4& value) const = 0;

	virtual void setUniformMatrix(const GLint loc, const glm::mat2& value, const bool transpose = false) const = 0;
	virtual void setUniformMatrix(const GLint loc, const glm::mat3& value, const bool transpose = false) const = 0;
	virtual void setUniformMatrix(const GLint loc, const glm::mat4& value, const bool transpose = false) const = 0;

}; // class IGLShaderProgram

} // namespace

#endif // __IGLSHADERPROGRAM__