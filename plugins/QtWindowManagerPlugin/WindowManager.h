#ifndef __WINDOWMANAGER__
#define __WINDOWMANAGER__

#include "IWindowManager.h"

class QCoreApplication;

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
}

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
private:
	IResourceManager *m_pResourceManager;
	Utility::AutoNameGenerator *m_pNameGenerator;

	QCoreApplication *m_pQtApp;
	bool m_isIndependentApp;

}; // class WindowManager

}; // namespace

#endif // __WINDOWMANAGER__