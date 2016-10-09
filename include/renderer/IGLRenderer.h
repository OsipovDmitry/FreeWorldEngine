#ifndef __IGLRENDERER__
#define __IGLRENDERER__

#include "glew/glew.h"

namespace FreeWorldEngine {

class IGLBufferObject;
class IGLVertexArrayObject;
class IGLShader;
class IGLShaderProgram;

class IGLRenderer {
public:
	virtual ~IGLRenderer() = 0 {}
	
	virtual void enable(GLenum cap, bool state = true) const = 0; // �������� ���������
	virtual void disable(GLenum cap) const = 0; // ��������� ���������
	virtual bool isEnabled(GLenum cap) const = 0; // �������� ������������ ���������

	virtual IGLBufferObject *generateBufferObject() const = 0; // �������� �����
	virtual IGLVertexArrayObject *generateVertexArrayObject() const = 0; // �������� ��������� ������
	virtual IGLShader *generateShader(const GLenum shaderType) const = 0; // �������� ������
	virtual IGLShaderProgram *generateShaderProgram() const = 0; // �������� ��������� ���������

	virtual void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const = 0; // ���������� ��������� ��������
	virtual void getViewport(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const = 0; // �������� ��������� ��������

	virtual void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const = 0; // ���������� ��������� ���� ������ �����
	virtual void getClearColor(GLclampf& r, GLclampf& g, GLclampf& b, GLclampf& a) const = 0; // �������� ��������� ���� ������ �����

	virtual void setClearDepth(GLclampd value) const = 0; // ���������� ��������� �������� ������ �������
	virtual void getClearDepth(GLclampd& value) const = 0; // �������� ��������� �������� ������ �������

	virtual void clearBuffers(GLbitfield mask) const = 0; // �������� ������
	virtual void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const = 0; // ��������� ��������������� ���������

}; // class IGLRenderer

}

#endif // __IGLRENDERER__