#ifndef __GLSHADER__
#define __GLSHADER__

#include <glew-1.13.0/include/GL/glew.h>

#include "renderer/IGPUShader.h"

namespace FreeWorldEngine {

class GLShader : public IGPUShader {
public:
    GLShader(const GLuint id);
    ~GLShader();

    IGPUShaderType type() const;

    void loadFromData(const std::string& data);
    void loadFromFile(const File& file);

    bool compile(std::string *pLogString = 0) const;

    GLuint GLid() const;
    GLenum GLtype() const;
	static GLenum GLtype(const IGPUShaderType type);

private:
    GLuint m_id;

}; // class GLShader


class GLProgram : public IGPUProgram {
public:
    GLProgram(const GLuint id);
    ~GLProgram();

    void attachShader(IGPUShader *pShader);
    void detachShader(IGPUShader *pShader);

    bool link(std::string *pLogString = 0) const;

    int32 attributeLocationByName(const std::string& name) const;
    int32 uniformLocationByName(const std::string& name) const;

	void setUniform(const int32 location, const float value) const;
	void setUniform(const int32 location, const int32 value) const;
	void setUniform(const int32 location, const uint32 value) const;
	void setUniform(const int32 location, const glm::vec2& value) const;
	void setUniform(const int32 location, const glm::ivec2& value) const;
	void setUniform(const int32 location, const glm::uvec2& value) const;
	void setUniform(const int32 location, const glm::vec3& value) const;
	void setUniform(const int32 location, const glm::ivec3& value) const;
	void setUniform(const int32 location, const glm::uvec3& value) const;
	void setUniform(const int32 location, const glm::vec4& value) const;
	void setUniform(const int32 location, const glm::ivec4& value) const;
	void setUniform(const int32 location, const glm::uvec4& value) const;
	void setUniform(const int32 location, const glm::mat2& value, const bool transpose = false) const;
	void setUniform(const int32 location, const glm::mat3& value, const bool transpose = false) const;
	void setUniform(const int32 location, const glm::mat4& value, const bool transpose = false) const;

    GLuint GLid() const;

private:
    GLuint m_id;

}; // class GLProgram

} // namespace

#endif // __GLSHADER__
