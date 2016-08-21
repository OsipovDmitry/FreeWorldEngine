#ifndef __IGLVERTEXBUFFEROBJECT__
#define __IGLVERTEXBUFFEROBJECT__

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLBufferObject : public IGLResource {
public:
	virtual ~IGLBufferObject() = 0 {}

	virtual void setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage) const = 0; // �������� ������ � ������ ������
	virtual void storage(GLenum target, GLsizeiptr size, GLenum usage) const = 0; // �������� ������

	virtual void setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr) const = 0; // ������ �������� ������
	virtual void getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr) const = 0; // ��������� �������� ������

	virtual void *map(GLenum target, GLenum access) const = 0; // ���������� GPU ������ � ������ CPU
	virtual bool unmap(GLenum target) const = 0; // ��������� �����������. ������������ ������ � ������ � map().

	virtual void copySubData(IGLBufferObject *targetBuffer, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)	const = 0; // ����������� �������� ������� ������ � targetBuffer. ��� ����������� (GPU->GPU ��� GPU->CPU->GPU) ������� �� ����������.

}; // class IGLVertexBufferObject

} // namespace

#endif // __IGLVERTEXBUFFEROBJECT__