#ifndef __WINDOWMANAGER__
#define __WINDOWMANAGER__

#include <3rdparty/sdl/SDL.h>
#include <IWindowManager.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
}

class Window;

class WindowManager : public IWindowManager {
public:
	WindowManager();
	~WindowManager();
	
	IWindow *createWindow(const std::string& title, const int32 width, const int32 height, const bool fullscreen = false, const bool resizable = true);
	void destroyWindow(IWindow *pWindow);
	void destroyAllWindows();

	void mainLoop();

	bool keyState(const IWindow::KeyCode keyCode) const;
	void mousePos(int32& x, int32& y) const;
	IWindow::MouseButtons mouseButtonsState() const;

	Window *findWindow(Uint32 id) const;

private:
	IResourceManager *m_pResourceManager;
	Utility::AutoNameGenerator *m_pNameGenerator;

}; // class WindowManager

}; // namespace

#endif // __WINDOWMANAGER__