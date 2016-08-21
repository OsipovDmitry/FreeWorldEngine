#ifndef __GLFUNCTIONS__
#define __GLFUNCTIONS__

#include "IShaderProgram.h"
#include "ITexture.h"
#include "IVertexArrayObject.h"
#include "IVertexBufferObject.h"

namespace FreeWorldEngine {

// IShader
bool IShader::isShader(GLuint id) { return (bool)glIsShader(id); }

// IProgram
GLint IProgram::getAttribLocation(GLuint id, const char *name) { return glGetAttribLocation(id, name); }
GLint IProgram::getUniformLocation(GLuint id, const char *name) { return glGetUniformLocation(id, name); }
GLuint IProgram::getUniformBlockIndex(GLuint id, const char *uniformBlockName) { return glGetUniformBlockIndex(id, uniformBlockName); }
void IProgram::setUniform1(GLint location, float v0) { glUniform1f(location, v0); }
void IProgram::setUniform2(GLint location, float v0, float v1) { glUniform2f(location, v0, v1); }
void IProgram::setUniform3(GLint location, float v0, float v1, const float v2) { glUniform3f(location, v0, v1, v2); }
void IProgram::setUniform4(GLint location, float v0, float v1, const float v2, const float v3) { glUniform4f(location, v0, v1, v2, v3); }
void IProgram::setUniform1(GLint location, double v0) { glUniform1d(location, v0); }
void IProgram::setUniform2(GLint location, double v0, double v1) { glUniform2d(location, v0, v1); }
void IProgram::setUniform3(GLint location, double v0, double v1, const double v2) { glUniform3d(location, v0, v1, v2); }
void IProgram::setUniform4(GLint location, double v0, double v1, const double v2, const double v3) { glUniform4d(location, v0, v1, v2, v3); }
void IProgram::setUniform1(GLint location, int v0) { glUniform1i(location, v0); }
void IProgram::setUniform2(GLint location, int v0, int v1) { glUniform2i(location, v0, v1); }
void IProgram::setUniform3(GLint location, int v0, int v1, const int v2) { glUniform3i(location, v0, v1, v2); }
void IProgram::setUniform4(GLint location, int v0, int v1, const int v2, const int v3) { glUniform4i(location, v0, v1, v2, v3); }
void IProgram::setUniform1(GLint location, unsigned int v0) { glUniform1ui(location, v0); }
void IProgram::setUniform2(GLint location, unsigned int v0, unsigned int v1) { glUniform2ui(location, v0, v1); }
void IProgram::setUniform3(GLint location, unsigned int v0, unsigned int v1, const unsigned int v2) { glUniform3ui(location, v0, v1, v2); }
void IProgram::setUniform4(GLint location, unsigned int v0, unsigned int v1, const unsigned int v2, const unsigned int v3) { glUniform4ui(location, v0, v1, v2, v3); }
void IProgram::setUniform1(GLint location, GLsizei count, float *p) { glUniform1fv(location, count, p); }
void IProgram::setUniform2(GLint location, GLsizei count, float *p) { glUniform2fv(location, count, p); }
void IProgram::setUniform3(GLint location, GLsizei count, float *p) { glUniform3fv(location, count, p); }
void IProgram::setUniform4(GLint location, GLsizei count, float *p) { glUniform4fv(location, count, p); }
void IProgram::setUniform1(GLint location, GLsizei count, double *p) { glUniform1dv(location, count, p); }
void IProgram::setUniform2(GLint location, GLsizei count, double *p) { glUniform2dv(location, count, p); }
void IProgram::setUniform3(GLint location, GLsizei count, double *p) { glUniform3dv(location, count, p); }
void IProgram::setUniform4(GLint location, GLsizei count, double *p) { glUniform4dv(location, count, p); }
void IProgram::setUniform1(GLint location, GLsizei count, int *p) { glUniform1iv(location, count, p); }
void IProgram::setUniform2(GLint location, GLsizei count, int *p) { glUniform2iv(location, count, p); }
void IProgram::setUniform3(GLint location, GLsizei count, int *p) { glUniform3iv(location, count, p); }
void IProgram::setUniform4(GLint location, GLsizei count, int *p) { glUniform4iv(location, count, p); }
void IProgram::setUniform1(GLint location, GLsizei count, unsigned int *p) { glUniform1uiv(location, count, p); }
void IProgram::setUniform2(GLint location, GLsizei count, unsigned int *p) { glUniform2uiv(location, count, p); }
void IProgram::setUniform3(GLint location, GLsizei count, unsigned int *p) { glUniform3uiv(location, count, p); }
void IProgram::setUniform4(GLint location, GLsizei count, unsigned int *p) { glUniform4uiv(location, count, p); }
void IProgram::setUniformMatrix2(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p) { glUniformMatrix2fv(location, count, transpose, p); }
void IProgram::setUniformMatrix3(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p) { glUniformMatrix3fv(location, count, transpose, p); }
void IProgram::setUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLfloat *p) { glUniformMatrix4fv(location, count, transpose, p); }
void IProgram::setUniformMatrix2(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p) { glUniformMatrix2dv(location, count, transpose, p); }
void IProgram::setUniformMatrix3(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p) { glUniformMatrix3dv(location, count, transpose, p); }
void IProgram::setUniformMatrix4(GLint location, GLsizei count, GLboolean transpose, const GLdouble *p) { glUniformMatrix4dv(location, count, transpose, p); }
void IProgram::uniformBlockBinding(GLuint id, GLuint uniformBlockIndex, GLuint uniformBlockBinding) { return glUniformBlockBinding(id, uniformBlockIndex, uniformBlockBinding); }
void IProgram::getUniformIndices(GLuint id, GLsizei uniformCount, const char **uniformNames, GLuint *uniformIndices) { glGetUniformIndices(id, uniformCount, uniformNames, uniformIndices); }
bool IProgram::isProgram(GLuint id) { return (bool)glIsProgram(id); }

// IVertexArrayObject
bool IVertexArrayObject::isVertexArray(GLuint id) { return (bool)glIsVertexArray(id); }
void IVertexArrayObject::setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) { glVertexAttribPointer(index, size, type, normalized, stride, pointer); }
void IVertexArrayObject::setAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) { glVertexAttribIPointer(index, size, type, stride, pointer); }
void IVertexArrayObject::setAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) { glVertexAttribLPointer(index, size, type, stride, pointer); }
void IVertexArrayObject::enableAttribArray(GLuint index) { glEnableVertexAttribArray(index); }
void IVertexArrayObject::disableAttribArray(GLuint index) { glDisableVertexAttribArray(index); }

// ITexture
bool ITexture::isTexture(GLuint id) { return (bool)glIsTexture(id); }
void ITexture::storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width) { glTexStorage1D(target, levels, internalFormat, width); }
void ITexture::storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height) { glTexStorage2D(target, levels, internalFormat, width, height); }
void ITexture::storage(GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth) { glTexStorage3D(target, levels, internalFormat, width, height, depth); }
void ITexture::storageMultisample(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) { glTexStorage2DMultisample(target, samples, internalFormat, width, height, fixedsamplelocations); }
void ITexture::storageMultisample(GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) { glTexStorage3DMultisample(target, samples, internalFormat, width, height, depth, fixedsamplelocations); }
void ITexture::loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLenum format, GLenum type, const void *data) { glTexImage1D(target, level, internalFormat, width, 0, format, type, data); }
void ITexture::loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data) { glTexImage2D(target, level, internalFormat, width, height, 0, format, type, data); }
void ITexture::loadImage(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) { glTexImage3D(target, level, internalFormat, width, height, depth, 0, format, type, data); }
void ITexture::loadImageMultisample(GLenum target, GLenum samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations) { glTexImage2DMultisample(target, samples, internalFormat, width, height, fixedsamplelocations); }
void ITexture::loadImageMultisample(GLenum target, GLenum samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations) { glTexImage3DMultisample(target, samples, internalFormat, width, height, depth, fixedsamplelocations); }
void ITexture::loadSubImage(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *data) { glTexSubImage1D(target, level, xoffset, width, format, type, data); }
void ITexture::loadSubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *data) { glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, data); }
void ITexture::loadSubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data) { glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, data); }
void ITexture::copyImage(GLenum target, GLint level, GLint internalFormat, GLint x, GLint y, GLsizei width) { glCopyTexImage1D(target, level, internalFormat, x, y, width, 0); }
void ITexture::copyImage(GLenum target, GLint level, GLint internalFormat, GLint x, GLint y, GLsizei width, GLsizei height) { glCopyTexImage2D(target, level, internalFormat, x, y, width, height, 0); }
void ITexture::copySubImage(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width) { glCopyTexSubImage1D(target, level, xoffset, x, y, width); }
void ITexture::copySubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) { glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height); }
void ITexture::copySubImage(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height) { glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height); }
void ITexture::getImage(GLenum target, GLint lod, GLenum format, GLenum type, GLvoid* image) { glGetTexImage(target, lod, format, type, image); }
void ITexture::pixelStore(GLenum pname, GLint param) { glPixelStorei(pname, param); }
void ITexture::pixelStore(GLenum pname, GLfloat param) { glPixelStoref(pname, param); }
void ITexture::setParameter(GLenum target, GLenum pname, GLint param) { glTexParameteri(target, pname, param); }
void ITexture::setParameter(GLenum target, GLenum pname, GLfloat param) { glTexParameterf(target, pname, param); }
void ITexture::setParameter(GLenum target, GLenum pname, const GLint *param) { glTexParameteriv(target, pname, param); }
void ITexture::setParameter(GLenum target, GLenum pname, const GLfloat *param) { glTexParameterfv(target, pname, param); }
void ITexture::setParameterI(GLenum target, GLenum pname, const GLint *param) { glTexParameterIiv(target, pname, param); }
void ITexture::setParameterI(GLenum target, GLenum pname, const GLuint *param) { glTexParameterIuiv(target, pname, param); }
void ITexture::setBuffer(GLenum target, GLenum internalFormat, GLuint buffer) { glTexBuffer(target, internalFormat, buffer); }
void ITexture::setBufferRange(GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size) { glTexBufferRange(target, internalFormat, buffer, offset, size); }
void ITexture::generateMipmap(GLenum target) { glGenerateMipmap(target); }

// IVertexBufferObject
void IVertexBufferObject::setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage) { glBufferData(target, size, ptr, usage); }
void IVertexBufferObject::setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr) { glBufferSubData(target, offs, size, ptr); }
void IVertexBufferObject::getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr) { glGetBufferSubData(target, offs, size, ptr); }
void IVertexBufferObject::clearData(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data) { glClearBufferData(target, internalformat, format, type, data); }
void IVertexBufferObject::clearSubData(GLenum target, GLenum internalformat, GLintptr offset, GLintptr size, GLenum format, GLenum type, const void *data) { glClearBufferSubData(target, internalformat, offset, size, format, type, data); }
void IVertexBufferObject::copySubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size) { glCopyBufferSubData(readtarget, writetarget, readoffset, writeoffset, size); }
void *IVertexBufferObject::map(GLenum target, GLenum access) { return glMapBuffer(target, access); }
bool IVertexBufferObject::unmap(GLenum target) { return glUnmapBuffer(target) == GL_TRUE; }
void *IVertexBufferObject::mapRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access) { return glMapBufferRange(target, offset, length, access); }
void IVertexBufferObject::flushMappedRange(GLenum target, GLintptr offset, GLsizeiptr length) { glFlushMappedBufferRange(target, offset, length); }
bool IVertexBufferObject::isBuffer(GLuint id) { return (bool)glIsBuffer(id); }
void IVertexBufferObject::bindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) { glBindBufferRange(target, index, buffer, offset, size); }
void IVertexBufferObject::bindBufferBase(GLenum target, GLuint index, GLuint buffer) { glBindBufferBase(target, index, buffer); }

} // namespace

#endif // __GLFUNCTIONS__