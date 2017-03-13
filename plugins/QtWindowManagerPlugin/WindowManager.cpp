#include <QtGui/qapplication.h>

#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>

#include "WindowManager.h"
#include "Window.h"

namespace FreeWorldEngine {

WindowManager::WindowManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForWindowManager", IResourceManager::StorageType_List)),
	m_pNameGenerator(new Utility::AutoNameGenerator("QtWindowName"))
{
	m_isIndependentApp = (m_pQtApp = QApplication::instance()) == nullptr;
	if (m_isIndependentApp) {
		int argc = getCoreEngine()->argc();
		char **argv = getCoreEngine()->argv();
		m_pQtApp = new QApplication(argc, argv);
	}
}

WindowManager::~WindowManager()
{
	destroyAllWindows();
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
	delete m_pNameGenerator;

	if (m_isIndependentApp)
		delete m_pQtApp;
}

IWindow *WindowManager::createWindow(const std::string& title, const int32 width, const int32 height, const bool fullscreen, const bool resizable)
{
	Window *pWindow = new Window(m_pNameGenerator->generateName(), title, width, height, fullscreen, resizable);
	m_pResourceManager->addResource(pWindow);
	return pWindow;
}

void WindowManager::destroyWindow(IWindow *pWindow)
{
	if (!pWindow)
		return;
	Window *p = static_cast<Window*>(pWindow);
	p->QGLWidget::close();
	m_pResourceManager->destroyResource(p);
}

void WindowManager::destroyAllWindows()
{
	while (m_pResourceManager->size())
		destroyWindow(static_cast<IWindow*>(*(m_pResourceManager->begin())));
}

void WindowManager::mainLoop()
{
	if (m_isIndependentApp)
		m_pQtApp->exec();
}

bool WindowManager::keyState(const IWindow::KeyCode keyCode) const
{
	return false;
}

void WindowManager::mousePos(int32& x, int32& y) const
{
	QPoint pos = QCursor::pos();
	x = pos.x();
	y = pos.y();
}

IWindow::MouseButtons WindowManager::mouseButtonsState() const
{
	return Window::qtMouseButtonsStateToMoueButtons(QApplication::mouseButtons());
}

} // namespace