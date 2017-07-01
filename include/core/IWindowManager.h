#ifndef __IWINDOWMANAGER__
#define __IWINDOWMANAGER__

#include "Types.h"
#include "IWindow.h"

namespace FreeWorldEngine {

class IWindowManager {
public:
	virtual ~IWindowManager() {}

	virtual IWindow *createWindow(const std::string& title, const int32 width, const int32 height, const bool fullscreen = false, const bool resizable = true) = 0;
	virtual void destroyWindow(IWindow *pWindow) = 0;
	virtual void destroyAllWindows() = 0;

	virtual void mainLoop() = 0;

	virtual bool keyState(const IWindow::KeyCode keyCode) const = 0;
	virtual void mousePos(int32& x, int32& y) const = 0;
	virtual IWindow::MouseButtons mouseButtonsState() const = 0;

}; // class IWindowManager


} // namespace

#endif // __IWINDOWMANAGER__