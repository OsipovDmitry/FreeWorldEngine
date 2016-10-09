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
	
	virtual void enable(GLenum cap, bool state = true) const = 0; // включить состояние
	virtual void disable(GLenum cap) const = 0; // выключить состояние
	virtual bool isEnabled(GLenum cap) const = 0; // проверка включенности состояния

	virtual IGLBufferObject *generateBufferObject() const = 0; // выделить буфер
	virtual IGLVertexArrayObject *generateVertexArrayObject() const = 0; // выделить вершинный массив
	virtual IGLShader *generateShader(const GLenum shaderType) const = 0; // выделить шейдер
	virtual IGLShaderProgram *generateShaderProgram() const = 0; // выделить шейдерную программу

	virtual void setViewport(GLint x, GLint y, GLsizei width, GLsizei height) const = 0; // установить параметры вьюпорта
	virtual void getViewport(GLint& x, GLint& y, GLsizei& width, GLsizei& height) const = 0; // получить параметры вьюпорта

	virtual void setClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a) const = 0; // установить начальный цвет буфера кадра
	virtual void getClearColor(GLclampf& r, GLclampf& g, GLclampf& b, GLclampf& a) const = 0; // получить начальный цвет буфера кадра

	virtual void setClearDepth(GLclampd value) const = 0; // установить начальное значение буфера глубины
	virtual void getClearDepth(GLclampd& value) const = 0; // получить начальное значение буфера глубины

	virtual void clearBuffers(GLbitfield mask) const = 0; // очистить буферы
	virtual void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) const = 0; // отрисовка индексированной геометрии

}; // class IGLRenderer

}

#endif // __IGLRENDERER__