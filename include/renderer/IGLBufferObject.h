#ifndef __IGLVERTEXBUFFEROBJECT__
#define __IGLVERTEXBUFFEROBJECT__

#include "IGLResource.h"

namespace FreeWorldEngine {

class IGLBufferObject : public IGLResource {
public:
	virtual ~IGLBufferObject() = 0 {}

	virtual void setData(GLenum target, GLsizeiptr size, const GLvoid *ptr, GLenum usage) const = 0; // Выделить память и залить данные
	virtual void storage(GLenum target, GLsizeiptr size, GLenum usage) const = 0; // Выделить память

	virtual void setSubData(GLenum target, GLintptr offs, GLsizeiptr size, const GLvoid *ptr) const = 0; // Залить фрагмент данных
	virtual void getSubData(GLenum target, GLintptr offs, GLsizeiptr size, GLvoid *ptr) const = 0; // Прочитать фрагмент данных

	virtual void *map(GLenum target, GLenum access) const = 0; // Отобразить GPU память в память CPU
	virtual bool unmap(GLenum target) const = 0; // Закончить отображение. Используется только в связке с map().

	virtual void copySubData(IGLBufferObject *targetBuffer, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size)	const = 0; // Скопировать фрагмент данного буфера в targetBuffer. Вид копирования (GPU->GPU или GPU->CPU->GPU) зависит от реализации.

}; // class IGLVertexBufferObject

} // namespace

#endif // __IGLVERTEXBUFFEROBJECT__