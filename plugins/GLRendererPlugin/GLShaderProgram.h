#ifndef __GLSHADERPROGRAM__
#define __GLSHADERPROGRAM__

#include "renderer/IGLShaderProgram.h"

namespace FreeWorldEngine {

class GLShaderProgram : public IGLShaderProgram {
public:
	GLShaderProgram(const GLuint id);
	~GLShaderProgram();

	void attachShader(IGLShader *pShader) const;
	void detachShader(IGLShader *pShader) const;
	void link() const;
	void getInfoLog(GLsizei bufSize, GLsizei *length, char *infoLog) const;

	GLint attributeLocation(const char *variable) const;
	GLint uniformLocation(const char *variable) const;

	void setUniform(const GLint loc, const GLint value) const;
	void setUniform(const GLint loc, const float value) const;

	void setUniformVector(const GLint loc, const GLint x, const GLint y) const;
	void setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z) const;
	void setUniformVector(const GLint loc, const GLint x, const GLint y, const GLint z, const GLint w) const;
	void setUniformVector(const GLint loc, const glm::ivec2& value) const;
	void setUniformVector(const GLint loc, const glm::ivec3& value) const;
	void setUniformVector(const GLint loc, const glm::ivec4& value) const;
	void setUniformVector(const GLint loc, const float x, const float y) const;
	void setUniformVector(const GLint loc, const float x, const float y, const float z) const;
	void setUniformVector(const GLint loc, const float x, const float y, const float z, const float w) const;
	void setUniformVector(const GLint loc, const glm::vec2& value) const;
	void setUniformVector(const GLint loc, const glm::vec3& value) const;
	void setUniformVector(const GLint loc, const glm::vec4& value) const;

	void setUniformMatrix(const GLint loc, const glm::mat2& value, const bool transpose = false) const;
	void setUniformMatrix(const GLint loc, const glm::mat3& value, const bool transpose = false) const;
	void setUniformMatrix(const GLint loc, const glm::mat4& value, const bool transpose = false) const;

private:
	GLuint m_id;
	static GLuint m_cacheBinding;

	void bind(GLuint id) const;

}; // class GLShaderProgram

} // namespace

#endif // __GLSHADERPROGRAM__