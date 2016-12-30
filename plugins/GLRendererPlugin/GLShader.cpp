#include <3rdparty/glm/gtc/type_ptr.hpp>

#include <utility/File.h>

#include "GLShader.h"
#include "GLRenderer.h"

namespace FreeWorldEngine {

namespace Renderer {

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
	loadFromData(data.c_str());
}

void GLShader::loadFromFile(const Utility::File &file)
{
	file.open(Utility::File::OpenMode_ReadOnly);
	int64 sz = file.size();
	char *p = new char [sz+1];
	file.readAll(sz, p);
	p[sz] = 0;
	loadFromData(p);
	delete [] p;
}

bool GLShader::compile(std::string *pLogString) const
{
    glCompileShader(m_id);

    GLint res;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &res);

	if (pLogString && (res == GL_FALSE)) {
		char buf[1024];
		glGetShaderInfoLog(m_id, 1024, 0, buf);
		*pLogString = buf;
	}

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

void GLShader::loadFromData(const char *pData)
{
	glShaderSource(m_id, 1, &pData, 0);
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

bool GLProgram::link(std::string *pLogString) const
{
    glLinkProgram(m_id);

    GLint res;
    glGetProgramiv(m_id, GL_LINK_STATUS, &res);

	if (pLogString && (res == GL_FALSE)) {
		char buf[1024];
		glGetProgramInfoLog(m_id, 1024, 0, buf);
		*pLogString = buf;
	}

    return res != GL_FALSE;
}

int32 GLProgram::attributeLocationByName(const std::string &name) const
{
	return glGetAttribLocation(m_id, name.c_str());
}

int32 GLProgram::uniformLocationByName(const std::string &name) const
{
	return glGetUniformLocation(m_id, name.c_str());
}

void GLProgram::setUniform(const int32 location, const float value) const
{
	pGLRenderer->bindProgram(this);
	glUniform1f(location, value);
}

void GLProgram::setUniform(const int32 location, const int32 value) const
{
	pGLRenderer->bindProgram(this);
	glUniform1i(location, value);
}

void GLProgram::setUniform(const int32 location, const uint32 value) const
{
	pGLRenderer->bindProgram(this);
	glUniform1ui(location, value);
}

void GLProgram::setUniform(const int32 location, const glm::vec2& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform2fv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::ivec2& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform2iv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::uvec2& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform2uiv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::vec3& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::ivec3& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform3iv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::uvec3& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform3uiv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::vec4& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::ivec4& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform4iv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::uvec4& value) const
{
	pGLRenderer->bindProgram(this);
	glUniform4uiv(location, 1, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::mat2& value, const bool transpose) const
{
	pGLRenderer->bindProgram(this);
	glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::mat3& value, const bool transpose) const
{
	pGLRenderer->bindProgram(this);
	glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
}

void GLProgram::setUniform(const int32 location, const glm::mat4& value, const bool transpose) const
{
	pGLRenderer->bindProgram(this);
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
}

int32 GLProgram::uniformBlockIndexByName(const std::string& name) const
{
	return glGetUniformBlockIndex(m_id, name.c_str());
}

IGPUProgram::UniformBlockInfo GLProgram::uniformBlockInfo(const int32 index, const bool writeNames) const
{
	IGPUProgram::UniformBlockInfo info;
	GLint len;

	info.index = index;

	glGetActiveUniformBlockiv(m_id, index, GL_UNIFORM_BLOCK_NAME_LENGTH, &len);
	info.name.resize(len+1);
	glGetActiveUniformBlockName(m_id, index, len+1, 0, (GLchar*)info.name.data());

	glGetActiveUniformBlockiv(m_id, index, GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&(info.dataSize));

	glGetActiveUniformBlockiv(m_id, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, (GLint*)&(info.numUniforms));

	info.uniformsIndices.resize(info.numUniforms);
	glGetActiveUniformBlockiv(m_id, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, info.uniformsIndices.data());

	info.uniformsOffsets.resize(info.numUniforms);
	glGetActiveUniformsiv(m_id, info.numUniforms, (GLuint*)info.uniformsIndices.data(), GL_UNIFORM_OFFSET, info.uniformsOffsets.data());

	info.uniformsSizes.resize(info.numUniforms);
	glGetActiveUniformsiv(m_id, info.numUniforms, (GLuint*)info.uniformsIndices.data(), GL_UNIFORM_SIZE, info.uniformsSizes.data());

	if (writeNames) {
		info.uniformsNames.resize(info.numUniforms);
		std::vector<int32> nameLengths(info.numUniforms);
		glGetActiveUniformsiv(m_id, info.numUniforms, (GLuint*)info.uniformsIndices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());

		for (uint32 i = 0; i < info.numUniforms; ++i) {
			info.uniformsNames[i].resize(nameLengths[i]+1);
			glGetActiveUniformName(m_id, info.uniformsIndices[i], nameLengths[i]+1, 0, (GLchar*)info.uniformsNames[i].data());
		}
	}

	return info;
}

void GLProgram::setUniformBlockBindingPoint(const int32 index, const uint32 bindingPoint) const
{
	glUniformBlockBinding(m_id, index, bindingPoint);
}

GLuint GLProgram::GLid() const
{
    return m_id;
}


} // namespace

} // namespace
