#include "Window.h"

namespace FreeWorldEngine {

Window::Window(SDL_Window *pWindow, const SDL_GLContext& context) :
	m_window(pWindow),
	m_glContext(context),
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
	if (m_window) {
		SDL_SetWindowPosition(m_window, x, y);
		if (m_funcMoved) m_funcMoved(x, y);
	}
}

void Window::resize(int32 width, int32 height)
{
	if (m_window) {
		SDL_SetWindowSize(m_window, width, height);
		if (m_funcResized)m_funcResized(width, height);
	}
}

void Window::show()
{
	if (m_window) {
		SDL_ShowWindow(m_window);
		if (m_funcShown) m_funcShown();
	}
}

void Window::hide()
{
	if (m_window) {
		SDL_HideWindow(m_window);
		if (m_funcHidden) m_funcHidden();
	}
}

void Window::minimize()
{
	if (m_window) {
		SDL_MinimizeWindow(m_window);
		if (m_funcMinimized) m_funcMinimized();
	}
}

void Window::restore()
{
	if (m_window) {
		SDL_RestoreWindow(m_window);
		if (m_funcRestored) m_funcRestored();
	}
}

void Window::maximize()
{
	if (m_window) {
		SDL_MaximizeWindow(m_window);
		if (m_funcMaximized) m_funcMaximized();
	}
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
	return m_window ? SDL_GetWindowID(m_window) : 0;
}

uint32 Window::width() const
{
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	return w;
}

uint32 Window::height() const
{
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	return h;
}

uint64 Window::frameNumber() const
{
	return m_frameNumber;
}

void Window::setMousePos(const int32 x, const int32 y) const
{
	SDL_WarpMouseInWindow(m_window, x, y);
}

void Window::render() const
{
	m_frameNumber++;
	if (m_funcRender)
		m_funcRender();
	SDL_GL_SwapWindow(m_window);
}

void Window::update(uint32 time, uint32 dt) const
{
	if (m_funcUpdate)
		m_funcUpdate(time, dt);
}

void Window::sendEvent(const SDL_WindowEvent& windowEvent) const
{
	switch (windowEvent.event) {
		case SDL_WINDOWEVENT_SHOWN: {
			if (m_funcShown) m_funcShown();
			break;
		}
		case SDL_WINDOWEVENT_HIDDEN: {
			if (m_funcHidden) m_funcHidden();
			break;
		}
		case SDL_WINDOWEVENT_MOVED: {
			if (m_funcMoved) m_funcMoved(windowEvent.data1, windowEvent.data2);
			break;
		}
		case SDL_WINDOWEVENT_RESIZED: {
			if (m_funcResized) m_funcResized(windowEvent.data1, windowEvent.data2);
			break;
		}
		case SDL_WINDOWEVENT_MINIMIZED: {
			if (m_funcMinimized) m_funcMinimized();
			break;
		}
		case SDL_WINDOWEVENT_RESTORED: {
			if (m_funcRestored) m_funcRestored();
			break;
		}
		case SDL_WINDOWEVENT_MAXIMIZED: {
			if (m_funcMaximized) m_funcMaximized();
			break;
		}
		case SDL_WINDOWEVENT_CLOSE: {
			if (m_funcClose) m_funcClose();
			break;
		}
		default: break;
	}
}

SDL_Window *Window::window() const
{
	return m_window;
}

SDL_GLContext Window::context() const
{
	return m_glContext;
}

} // namespace