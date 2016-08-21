#ifndef __WINDOW__
#define __WINDOW__

#include "IWindow.h"

#include "SDL2/include/SDL.h"

namespace FreeWorldEngine {

class Window : public IWindow {
public:
	Window(SDL_Window *pWindow, const SDL_GLContext& context);
	~Window();
	void show() const;
	void hide() const;
	void move(int32 x, int32 y) const;
	void resize(int32 width, int32 height) const;
	void minimize() const;
	void restore() const;
	void maximize() const;
	void render() const;
	void update(uint32 time, uint32 dt) const;

	void setFuncShown(void (*func)());
	void setFuncHidden(void (*func)());
	void setFuncMoved(void (*func)(int32, int32));
	void setFuncResized(void (*func)(int32, int32));
	void setFuncMinimized(void (*func)());
	void setFuncRestored(void (*func)());
	void setFuncMaximized(void (*func)());
	void setFuncClose(void (*func)());
	void setFuncRender(void (*func)());
	void setFuncUpdate(void (*func)(uint32, uint32));

	uint32 id() const;
	uint32 width() const;
	uint32 height() const;
	uint64 frameNumber() const;
	void setMousePos(const int32 x, const int32 y) const;

	void sendEvent(const SDL_WindowEvent& windowEvent) const;

private:
	SDL_Window *m_window;
	SDL_GLContext m_glContext;
	mutable uint64 m_frameNumber;

	void (*m_funcShown)();
	void (*m_funcHidden)();
	void (*m_funcMoved)(int32, int32);
	void (*m_funcResized)(int32, int32);
	void (*m_funcMinimized)();
	void (*m_funcRestored)();
	void (*m_funcMaximized)();
	void (*m_funcClose)();
	void (*m_funcRender)();
	void (*m_funcUpdate)(uint32, uint32);

};

} // namespace

#endif // __WINDOW__