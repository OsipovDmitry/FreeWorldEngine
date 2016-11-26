#include "Window.h"

namespace FreeWorldEngine {

Window::Window() :
	QGLWidget(),
	m_funcShown(0),
	m_funcHidden(0),
	m_funcMoved(0),
	m_funcResized(0),
	m_funcMinimized(0),
	m_funcRestored(0),
	m_funcMaximized(0),
	m_funcClose(0),
	m_funcRender(0),
	m_funcUpdate(0),
	m_frameNumber(-1)
{
}

Window::~Window()
{
}

void Window::move(int32 x, int32 y)
{
	QGLWidget::move(x, y);
	if (m_funcMoved)
		m_funcMoved(x, y);
}

void Window::resize(int32 width, int32 height)
{
	QGLWidget::resize(width, height);
}

void Window::show()
{
	QGLWidget::show();
	if (m_funcShown)
		m_funcShown();
}

void Window::hide()
{
	QGLWidget::hide();
	if (m_funcHidden)
		m_funcHidden();
}

void Window::minimize()
{
	QGLWidget::showMinimized();
	if (m_funcMinimized)
		m_funcMinimized();
}

void Window::restore()
{
	QGLWidget::showNormal();
	if (m_funcRestored)
		m_funcRestored();
}

void Window::maximize()
{
	QGLWidget::showMaximized();
	if (m_funcMaximized)
		m_funcMaximized();
}

void Window::update(uint32 time, uint32 dt) const
{
	if (m_funcUpdate)
		m_funcUpdate(time, dt);
}

void Window::setFuncShown(void (*func)())
{
	m_funcShown = func;
}

void Window::setFuncHidden(void (*func)())
{
	m_funcHidden = func;
}

void Window::setFuncMoved(void (*func)(int32, int32))
{
	m_funcMoved = func;
}

void Window::setFuncResized(void (*func)(int32, int32))
{
	m_funcResized = func;
}

void Window::setFuncMinimized(void (*func)())
{
	m_funcMinimized = func;
}

void Window::setFuncRestored(void (*func)())
{
	m_funcRestored = func;
}

void Window::setFuncMaximized(void (*func)())
{
	m_funcMaximized = func;
}

void Window::setFuncClose(void (*func)())
{
	m_funcClose = func;
}

void Window::setFuncRender(void (*func)())
{
	m_funcRender = func;
}

void Window::setFuncUpdate(void (*func)(uint32, uint32))
{
	m_funcUpdate = func;
}

uint32 Window::id() const
{
	return 0; // «¿À»œ”’¿!!!!!!!!!!
}

uint32 Window::width() const
{
	return static_cast<uint32>(QGLWidget::width());
}

uint32 Window::height() const
{
	return static_cast<uint32>(QGLWidget::height());
}

uint64 Window::frameNumber() const
{
	return m_frameNumber;
}

void Window::setMousePos(const int32 x, const int32 y) const
{
	return;
}


void Window::initializeGL()
{

}

void Window::resizeGL(int w, int h)
{
	if (m_funcResized)
		m_funcResized(w, h);
}

void Window::paintGL()
{
	m_frameNumber++;
	if (m_funcRender)
		m_funcRender();
}

//void Window::sendEvent(const SDL_WindowEvent& windowEvent) const
//{
//	switch (windowEvent.event) {
//	case SDL_WINDOWEVENT_SHOWN: {
//		if (m_funcShown) m_funcShown();
//		break;
//								}
//	case SDL_WINDOWEVENT_HIDDEN: {
//		if (m_funcHidden) m_funcHidden();
//		break;
//								 }
//	case SDL_WINDOWEVENT_MOVED: {
//		if (m_funcMoved) m_funcMoved(windowEvent.data1, windowEvent.data2);
//		break;
//								}
//	case SDL_WINDOWEVENT_RESIZED: {
//		if (m_funcResized) m_funcResized(windowEvent.data1, windowEvent.data2);
//		break;
//								  }
//	case SDL_WINDOWEVENT_MINIMIZED: {
//		if (m_funcMinimized) m_funcMinimized();
//		break;
//									}
//	case SDL_WINDOWEVENT_RESTORED: {
//		if (m_funcRestored) m_funcRestored();
//		break;
//								   }
//	case SDL_WINDOWEVENT_MAXIMIZED: {
//		if (m_funcMaximized) m_funcMaximized();
//		break;
//									}
//	case SDL_WINDOWEVENT_CLOSE: {
//		if (m_funcClose) m_funcClose();
//		break;
//								}
//	default: break;
//	}
//}

} // namespace