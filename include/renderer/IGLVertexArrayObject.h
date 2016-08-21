#ifndef __IGLVERTEXARRAYOBJECT__
#define __IGLVERTEXARRAYOBJECT__

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLVertexArrayObject : public IGLResource {
public:
	virtual ~IGLVertexArrayObject() = 0 {}

	virtual void setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) const = 0; // ��������� ���������� ��������

	virtual void enableAttribArray(GLuint index) const = 0; // ��������� ������������ ��������� �������
	virtual void disableAttribArray(GLuint index) const = 0; // ��������� ������������ ��������� �������

}; // class IGLVertexArrayObject

} // namespace

#endif // __IGLVERTEXARRAYOBJECT__