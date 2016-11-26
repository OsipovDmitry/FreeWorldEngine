#include "GLShader.h"

namespace FreeWorldEngine {

GLShader::GLShader(const GLuint id) :
	m_id(id)
{
}

GLShader::~GLShader()
{
}

IGPUShader::IGPUShaderType GLShader::type() const
{
    GLenum val = GLtype();

    switch (val) {
	case GL_VERTEX_SHADER: return IGPUShader::IGPUShaderType_Vertex;
    case GL_GEOMETRY_SHADER: return IGPUShader::IGPUShaderType_Geometry;
    case GL_FRAGMENT_SHADER: return IGPUShader::IGPUShaderType_Fragment;
    }

    return IGPUShader::IGPUShaderType_None;
}

void GLShader::loadFromData(const std::string &data)
{
	const char *p = data.c_str();
	glShaderSource(m_id, 1, &p, 0);
}

void GLShader::loadFromFile(const File &file)
{

}

bool GLShader::compile() const
{
    glCompileShader(m_id);

    GLint res;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &res);
    return res != GL_FALSE;
}

GLuint GLShader::GLid() const
{
    return m_id;
}

GLenum GLShader::GLtype() const
{
    GLint val;
    glGetShaderiv(m_id, GL_SHADER_TYPE, &val);
    return (GLenum)val;
}

GLenum GLShader::GLtype(const IGPUShaderType type)
{
	switch (type) {
	case IGPUShader::IGPUShaderType_Vertex: return GL_VERTEX_SHADER;
	case IGPUShader::IGPUShaderType_Geometry: return GL_GEOMETRY_SHADER;
	case IGPUShader::IGPUShaderType_Fragment: return GL_FRAGMENT_SHADER;
	}
	return 0;
}

GLProgram::GLProgram(const GLuint id) :
    m_id(id)
{
}

GLProgram::~GLProgram()
{
}

void GLProgram::attachShader(IGPUShader *pShader)
{
    GLShader *pGLShader = static_cast<GLShader*>(pShader);
    glAttachShader(m_id, pGLShader->GLid());
}

void GLProgram::detachShader(IGPUShader *pShader)
{
    GLShader *pGLShader = static_cast<GLShader*>(pShader);
    glDetachShader(m_id, pGLShader->GLid());
}

bool GLProgram::link() const
{
    glLinkProgram(m_id);

    GLint res;
    glGetProgramiv(m_id, GL_LINK_STATUS, &res);
    return res != GL_FALSE;
}

int32 GLProgram::attributeLocationByName(const std::string &name)
{
	return -1;
}

int32 GLProgram::uniformLocationByName(const std::string &name)
{
	return -1;
}

GLuint GLProgram::GLid() const
{
    return m_id;
}


} // namespace
