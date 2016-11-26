#ifndef __WINDOW__
#define __WINDOW__

#include <QtOpenGL/QGLWidget>

#include <IWindow.h>

namespace FreeWorldEngine {

class Window : public QGLWidget, public IWindow {
	Q_OBJECT

public:
	Window();
	~Window();
	void show();
	void hide();
	void move(int32 x, int32 y);
	void resize(int32 width, int32 height);
	void minimize();
	void restore();
	void maximize();

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

	void update(uint32 time, uint32 dt) const;
	// void sendEvent(const SDL_WindowEvent& windowEvent) const;

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
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