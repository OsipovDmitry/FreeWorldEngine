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

    bool compile() const;

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

    bool link() const;

    int32 attributeLocationByName(const std::string& name);
    int32 uniformLocationByName(const std::string& name);

    GLuint GLid() const;

private:
    GLuint m_id;

}; // class GLProgram

} // namespace

#endif // __GLSHADER__
