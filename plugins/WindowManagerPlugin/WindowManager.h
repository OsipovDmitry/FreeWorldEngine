#ifndef __WINDOWMANAGER__
#define __WINDOWMANAGER__

#include <list>
#include "IWindowManager.h"

namespace FreeWorldEngine {

class WindowManager : public IWindowManager {
public:
	WindowManager();
	~WindowManager();
	IWindow *createWindow(const std::string& title, const int32 width, const int32 height, const uint32 flags = IWindow::IWindow_Show);
	void destroyWindow(IWindow *pWindow);
	IWindow *findWindow(const uint32 id) const;
	void mainLoop();

	void setFuncMouseButtonDown(void (*func)(uint32 windowID, MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY));
	void setFuncMouseButtonUp(void (*func)(uint32 windowID, MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY));
	void setFuncMouseMotion(void (*func)(uint32 windowID, uint32 buttonsMask, int32 cursorPosX, int32 cursorPosY, int32 relCursorPosX, int32 relCursorPosY));
	void setFuncMouseWheel(void (*func)(uint32 windowID, int32 dirX, int32 dirY));
	void setMousePos(const int32 x, const int32 y, const IWindow *pWindow = 0) const;
	void mousePos(int32& x, int32& y) const;
	uint32 mouseButtonsState() const;

	void setFuncKeyDown(void (*func)(uint32 windowId, KeyCode keyCode, uint32 modifiers));
	void setFuncKeyUp(void (*func)(uint32 windowId, KeyCode keyCode, uint32 modifiers));
	bool keyState(const KeyCode keyCode) const;
	uint32 modifiers() const;

private:
	std::list<IWindow*> m_windows;
	void (*m_funcMouseButtonDown)(uint32, MouseButton, uint32, int32, int32);
	void (*m_funcMouseButtonUp)(uint32, MouseButton, uint32, int32, int32);
	void (*m_funcMouseMotion)(uint32, uint32, int32, int32, int32, int32);
	void (*m_funcMouseWheel)(uint32, int32, int32);
	void (*m_funcKeyDown)(uint32, KeyCode, uint32);
	void (*m_funcKeyUp)(uint32, KeyCode, uint32);

}; // class WindowManager

}; // namespace

#endif // __WINDOWMANAGER__