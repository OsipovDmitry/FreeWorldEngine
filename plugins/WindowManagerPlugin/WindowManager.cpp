#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>

#include "WindowManager.h"
#include "Window.h"

namespace FreeWorldEngine {

WindowManager::WindowManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForWindowManager", IResourceManager::StorageType_List)),
	m_pNameGenerator(new Utility::AutoNameGenerator("WindowName"))
{
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetSwapInterval(0);
}

WindowManager::~WindowManager()
{
	destroyAllWindows();
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
	delete m_pNameGenerator;
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
	Uint32 id = static_cast<Window*>(pWindow)->id();
	if (!id)
		return;
	SDL_Event event;
	event.window.type = SDL_WINDOWEVENT;
	event.window.windowID = id;
	event.window.event = SDL_WINDOWEVENT_CLOSE;
	SDL_PushEvent(&event);
}

void WindowManager::destroyAllWindows()
{
	while (m_pResourceManager->size())
		destroyWindow(static_cast<IWindow*>(*(m_pResourceManager->begin())));
}

void WindowManager::mainLoop() {
	bool quit = false;
	static uint32 lastTime = SDL_GetTicks();

	std::for_each(m_pResourceManager->begin(), m_pResourceManager->end(), [](IResource *p) {
		IWindow *pp = static_cast<IWindow*>(p);
		pp->resize(pp->width(), pp->height());
	});
		
	while (true) {
		const uint32 time = SDL_GetTicks();
		const uint32 dt = time - lastTime;
		lastTime = time;

		std::for_each(m_pResourceManager->begin(), m_pResourceManager->end(), [time, dt](IResource *p) {
			Window *pp = static_cast<Window*>(p);
			pp->update(time, dt);
			pp->render();
		});

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					quit = true;
					break;
				}
				case SDL_WINDOWEVENT: {
					Window *pWindow = findWindow(event.window.windowID);
					if (!pWindow)
						break;
					pWindow->sendEvent(event.window); // ѕересылаем событие окну.
					if (event.window.event == SDL_WINDOWEVENT_CLOSE) { // ≈сли событие было на закрытие окна, то дополнительно уничтожаем его и удал€ем из менеджера.
						m_pResourceManager->destroyResource(pWindow->name());
						if (m_pResourceManager->size() == 0) { // ≈сли последний SDL_WINDOWEVENT_CLOSE посылалс€ искусственно, то SDL_QUIT не придет и нужно его симулировать.
							SDL_Event quitEvent;
							quitEvent.type = SDL_QUIT;
							SDL_PushEvent(&quitEvent);
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					Window *pWindow = findWindow(event.button.windowID);
					if (pWindow)
						pWindow->mouseButtonDown(Window::sdlMouseButtonToMouseButton(event.button.button), event.button.clicks, event.button.x, event.button.y);
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					Window *pWindow = findWindow(event.button.windowID);
					if (pWindow)
						pWindow->mouseButtonUp(Window::sdlMouseButtonToMouseButton(event.button.button), event.button.clicks, event.button.x, event.button.y);
					break;
				}
				case SDL_MOUSEMOTION: {
					Window *pWindow = findWindow(event.motion.windowID);
					if (pWindow)
						pWindow->mouseMotion(Window::sdlMouseButtonsStateToMoueButtons(event.motion.state), event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;
				}
				case SDL_MOUSEWHEEL: {
					Window *pWindow = findWindow(event.wheel.windowID);
					if (pWindow)
						pWindow->mouseWheel(event.wheel.x, event.wheel.y);
					break;
				}
				case SDL_KEYDOWN : {
					Window *pWindow = findWindow(event.key.windowID);
					if (pWindow)
						pWindow->keyDown(Window::sdlKeysymToKeyCode(event.key.keysym));
					break;
				}
				case SDL_KEYUP : {
					Window *pWindow = findWindow(event.key.windowID);
					if (pWindow)
						pWindow->keyUp(Window::sdlKeysymToKeyCode(event.key.keysym));
					break;
				}
				default: break;
			}
		}
		if (quit)
			break;
	}
}

bool WindowManager::keyState(const IWindow::KeyCode keyCode) const
{
	return SDL_GetKeyboardState(0)[Window::keyCodeToSDLScancode(keyCode)];
}

void WindowManager::mousePos(int32& x, int32& y) const
{
	SDL_GetMouseState(&x, &y);
}

IWindow::MouseButtons WindowManager::mouseButtonsState() const
{
	int tmp;
	return Window::sdlMouseButtonsStateToMoueButtons(SDL_GetMouseState(&tmp, &tmp));
}

Window *WindowManager::findWindow(Uint32 id) const
{
	ResourceIterator i = std::find_if(m_pResourceManager->begin(), m_pResourceManager->end(), [id](IResource *p){ return static_cast<Window*>(p)->id() == id; });
	return (i != m_pResourceManager->end()) ? static_cast<Window*>(*i) : 0;
}

} // namespace