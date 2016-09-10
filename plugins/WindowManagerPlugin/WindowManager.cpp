#include "WindowManager.h"
#include "Window.h"
#include <algorithm>

namespace FreeWorldEngine {

void sdlKeysymToKeyCode(const SDL_Keysym& sdlKey, IWindowManager::KeyCode& keyCode, uint32& modifiers);
uint32 sdlMouseButtonsStateToMask(Uint32 buttonsState);
IWindowManager::MouseButton sdlMouseButtonToMouseButton(Uint8 sdlMouseButton);
uint32 sdlKeyModifiersToMask(SDL_Keymod mod);
SDL_Scancode keyCodeToSDLScancode(IWindowManager::KeyCode keyCode);

WindowManager::WindowManager() :
	m_windows(),
	m_funcMouseButtonDown(0),
	m_funcMouseButtonUp(0),
	m_funcMouseMotion(0),
	m_funcMouseWheel(0),
	m_funcKeyDown(0),
	m_funcKeyUp(0)
{
}

WindowManager::~WindowManager()
{
	std::list<Window*>::const_iterator i;
	for (i = m_windows.cbegin(); i != m_windows.cend(); ++i) {
		destroyWindow(static_cast<IWindow*>(*i));
	}
	m_windows.clear();
}

IWindow *WindowManager::createWindow(const std::string& title, const int32 width, const int32 height, const uint32 flags)
{
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetSwapInterval(0);

	const bool fullscreen = flags & IWindow::IWindow_Fullscreen;
	const bool show = flags & IWindow::IWindow_Show;
	const bool resizable = flags & IWindow::IWindow_Resizable;

	SDL_Window *pSDLWindow = SDL_CreateWindow(
		title.c_str(),
		fullscreen ? 0 : SDL_WINDOWPOS_CENTERED,
		fullscreen ? 0 : SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | (show ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN) | (resizable ? SDL_WINDOW_RESIZABLE : 0));
	if (!pSDLWindow)
		return 0;

	SDL_GLContext pSDLGLContext = SDL_GL_CreateContext(pSDLWindow);
	if (!pSDLGLContext) {
		SDL_DestroyWindow(pSDLWindow);
		return 0;
	}

	Window *pWindow = new Window(pSDLWindow, pSDLGLContext);
	m_windows.push_back(pWindow);
	return static_cast<IWindow*>(pWindow);
}

void WindowManager::destroyWindow(IWindow *pWindow)
{
	if (!pWindow)
		return;
	uint32 id = pWindow->id();
	if (!id)
		return;
	SDL_Event event;
	event.window.type = SDL_WINDOWEVENT;
	event.window.windowID = id;
	event.window.event = SDL_WINDOWEVENT_CLOSE;
	SDL_PushEvent(&event);
}

IWindow *WindowManager::findWindow(const uint32 id) const
{
	for (std::list<Window*>::const_iterator i = m_windows.cbegin(); i != m_windows.cend(); ++i)
		if ((*i)->id() == id)
			return static_cast<IWindow*>(*i);
	return 0;
}

void WindowManager::mainLoop() {
	bool quit = false;
	static uint32 lastTime = SDL_GetTicks();

	for (std::list<Window*>::const_iterator i = m_windows.cbegin(); i != m_windows.cend(); ++i)
		(*i)->resize((*i)->width(), (*i)->height());

	while (true) {

		const uint32 time = SDL_GetTicks();
		const uint32 dt = time - lastTime;
		lastTime = time;

		std::list<Window*>::const_iterator i;
		for (i = m_windows.cbegin(); i != m_windows.cend(); ++i) {
			(*i)->update(time, dt);
			(*i)->render();
		}

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					quit = true;
					break;
				}
				case SDL_WINDOWEVENT: {
					std::list<Window*>::const_iterator i; // Итератор по окнам. Ищем в него окно с нужным id. 
					for (i = m_windows.cbegin(); i != m_windows.cend(); ++i)
						if ((*i)->id() == event.window.windowID)
							break;
					if (i == m_windows.cend()) // Если не нашли, то событие не нашему окну.
						break; // Выходим.
					(*i)->sendEvent(event.window); // Пересылаем событие окну.
					if (event.window.event == SDL_WINDOWEVENT_CLOSE) { // Если событие было на закрытие окна, то дополнительно уничтожаем его и удаляем из менеджера.
						SDL_Window *pWindow = (*i)->window();
						SDL_GLContext glContext = (*i)->context();
						if (glContext)
							SDL_GL_DeleteContext(glContext);
						if (pWindow)
							SDL_DestroyWindow(pWindow);
						delete *i;
						m_windows.erase(i);

						if (m_windows.empty()) { // Если последний SDL_WINDOWEVENT_CLOSE посылался искусственно, то SDL_QUIT не придет и нужно его симулировать.
							SDL_Event quitEvent;
							quitEvent.type = SDL_QUIT;
							SDL_PushEvent(&quitEvent);
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					if (m_funcMouseButtonDown) {
						MouseButton mouseButton = sdlMouseButtonToMouseButton(event.button.button);
						m_funcMouseButtonDown(event.button.windowID, mouseButton, event.button.clicks, event.button.x, event.button.y);
					}
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					if (m_funcMouseButtonUp) {
						MouseButton mouseButton = sdlMouseButtonToMouseButton(event.button.button);
						m_funcMouseButtonUp(event.button.windowID, mouseButton, event.button.clicks, event.button.x, event.button.y);
					}
					break;
				}
				case SDL_MOUSEMOTION: {
					if (m_funcMouseMotion) {
						uint32 mask = sdlMouseButtonsStateToMask(event.motion.state);
						m_funcMouseMotion(event.motion.windowID, mask, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					}
					break;
				}
				case SDL_MOUSEWHEEL: {
					if (m_funcMouseWheel) {
						m_funcMouseWheel(event.wheel.windowID, event.wheel.x, event.wheel.y);
					}
					break;
				}
				case SDL_KEYDOWN : {
					if (m_funcKeyDown) {
						KeyCode keyCode;
						uint32 modifiers;
						sdlKeysymToKeyCode(event.key.keysym, keyCode, modifiers);
						m_funcKeyDown(event.key.windowID, keyCode, modifiers);
					}
					break;
				}
				case SDL_KEYUP : {
					if (m_funcKeyUp) {
						KeyCode keyCode;
						uint32 modifiers;
						sdlKeysymToKeyCode(event.key.keysym, keyCode, modifiers);
						m_funcKeyUp(event.key.windowID, keyCode, modifiers);
					}
					break;
				}
				default: break;
			}
		}
		if (quit)
			break;
	}
}

void WindowManager::setFuncMouseButtonDown(void (*func)(uint32 windowID, MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY))
{
	m_funcMouseButtonDown = func;
}

void WindowManager::setFuncMouseButtonUp(void (*func)(uint32 windowID, MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY))
{
	m_funcMouseButtonUp = func;
}

void WindowManager::setFuncMouseMotion(void (*func)(uint32 windowID, uint32 buttonsMask, int32 cursorPosX, int32 cursorPosY, int32 relCursorPosX, int32 relCursorPosY))
{
	m_funcMouseMotion = func;
}

void WindowManager::setFuncMouseWheel(void (*func)(uint32 windowID, int32 dirX, int32 dirY))
{
	m_funcMouseWheel = func;
}

void WindowManager::setMousePos(const int32 x, const int32 y, const IWindow *pWindow) const
{
	if (pWindow)
		pWindow->setMousePos(x, y);
	else
		SDL_WarpMouseGlobal(x, y);
}

void WindowManager::mousePos(int32& x, int32& y) const
{
	SDL_GetMouseState(&x, &y);
}

uint32 WindowManager::mouseButtonsState() const
{
	int tmp;
	return sdlMouseButtonsStateToMask(SDL_GetMouseState(&tmp, &tmp));
}

void WindowManager::setFuncKeyDown(void (*func)(uint32 windowId, KeyCode keyCode, uint32 modifiers))	
{
	m_funcKeyDown = func;
}

void WindowManager::setFuncKeyUp(void (*func)(uint32 windowId, KeyCode keyCode, uint32 modifiers))
{
	m_funcKeyUp = func;
}

bool WindowManager::keyState(const KeyCode keyCode) const
{
	return SDL_GetKeyboardState(0)[keyCodeToSDLScancode(keyCode)];
}

uint32 WindowManager::modifiers() const
{
	return sdlKeyModifiersToMask(SDL_GetModState());
}

void sdlKeysymToKeyCode(const SDL_Keysym& sdlKey, IWindowManager::KeyCode& keyCode, uint32& modifiers)
{
	modifiers = sdlKeyModifiersToMask((SDL_Keymod)sdlKey.mod);

	keyCode = IWindowManager::KeyCode_None;
	switch (sdlKey.scancode) {
		case SDL_SCANCODE_UNKNOWN: break;
		case SDL_SCANCODE_0: { keyCode = IWindowManager::KeyCode_0; return; }
		case SDL_SCANCODE_1: { keyCode = IWindowManager::KeyCode_1; return; }
		case SDL_SCANCODE_2: { keyCode = IWindowManager::KeyCode_2; return; }
		case SDL_SCANCODE_3: { keyCode = IWindowManager::KeyCode_3; return; }
		case SDL_SCANCODE_4: { keyCode = IWindowManager::KeyCode_4; return; }
		case SDL_SCANCODE_5: { keyCode = IWindowManager::KeyCode_5; return; }
		case SDL_SCANCODE_6: { keyCode = IWindowManager::KeyCode_6; return; }
		case SDL_SCANCODE_7: { keyCode = IWindowManager::KeyCode_7; return; }
		case SDL_SCANCODE_8: { keyCode = IWindowManager::KeyCode_8; return; }
		case SDL_SCANCODE_9: { keyCode = IWindowManager::KeyCode_9; return; }
		case SDL_SCANCODE_A: { keyCode = IWindowManager::KeyCode_A; return; }
		case SDL_SCANCODE_B: { keyCode = IWindowManager::KeyCode_B; return; }
		case SDL_SCANCODE_C: { keyCode = IWindowManager::KeyCode_C; return; }
		case SDL_SCANCODE_D: { keyCode = IWindowManager::KeyCode_D; return; }
		case SDL_SCANCODE_E: { keyCode = IWindowManager::KeyCode_E; return; }
		case SDL_SCANCODE_F: { keyCode = IWindowManager::KeyCode_F; return; }
		case SDL_SCANCODE_G: { keyCode = IWindowManager::KeyCode_G; return; }
		case SDL_SCANCODE_H: { keyCode = IWindowManager::KeyCode_H; return; }
		case SDL_SCANCODE_I: { keyCode = IWindowManager::KeyCode_I; return; }
		case SDL_SCANCODE_J: { keyCode = IWindowManager::KeyCode_J; return; }
		case SDL_SCANCODE_K: { keyCode = IWindowManager::KeyCode_K; return; }
		case SDL_SCANCODE_L: { keyCode = IWindowManager::KeyCode_L; return; }
		case SDL_SCANCODE_M: { keyCode = IWindowManager::KeyCode_M; return; }
		case SDL_SCANCODE_N: { keyCode = IWindowManager::KeyCode_N; return; }
		case SDL_SCANCODE_O: { keyCode = IWindowManager::KeyCode_O; return; }
		case SDL_SCANCODE_P: { keyCode = IWindowManager::KeyCode_P; return; }
		case SDL_SCANCODE_Q: { keyCode = IWindowManager::KeyCode_Q; return; }
		case SDL_SCANCODE_R: { keyCode = IWindowManager::KeyCode_R; return; }
		case SDL_SCANCODE_S: { keyCode = IWindowManager::KeyCode_S; return; }
		case SDL_SCANCODE_T: { keyCode = IWindowManager::KeyCode_T; return; }
		case SDL_SCANCODE_U: { keyCode = IWindowManager::KeyCode_U; return; }
		case SDL_SCANCODE_V: { keyCode = IWindowManager::KeyCode_V; return; }
		case SDL_SCANCODE_W: { keyCode = IWindowManager::KeyCode_W; return; }
		case SDL_SCANCODE_X: { keyCode = IWindowManager::KeyCode_X; return; }
		case SDL_SCANCODE_Y: { keyCode = IWindowManager::KeyCode_Y; return; }
		case SDL_SCANCODE_Z: { keyCode = IWindowManager::KeyCode_Z; return; }
		case SDL_SCANCODE_F1: { keyCode = IWindowManager::KeyCode_F1; return; }
		case SDL_SCANCODE_F2: { keyCode = IWindowManager::KeyCode_F2; return; }
		case SDL_SCANCODE_F3: { keyCode = IWindowManager::KeyCode_F3; return; }
		case SDL_SCANCODE_F4: { keyCode = IWindowManager::KeyCode_F4; return; }
		case SDL_SCANCODE_F5: { keyCode = IWindowManager::KeyCode_F5; return; }
		case SDL_SCANCODE_F6: { keyCode = IWindowManager::KeyCode_F6; return; }
		case SDL_SCANCODE_F7: { keyCode = IWindowManager::KeyCode_F7; return; }
		case SDL_SCANCODE_F8: { keyCode = IWindowManager::KeyCode_F8; return; }
		case SDL_SCANCODE_F9: { keyCode = IWindowManager::KeyCode_F9; return; }
		case SDL_SCANCODE_F10: { keyCode = IWindowManager::KeyCode_F10; return; }
		case SDL_SCANCODE_F11: { keyCode = IWindowManager::KeyCode_F11; return; }
		case SDL_SCANCODE_F12: { keyCode = IWindowManager::KeyCode_F12; return; }
		case SDL_SCANCODE_LEFT: { keyCode = IWindowManager::KeyCode_Left; return; }
		case SDL_SCANCODE_RIGHT: { keyCode = IWindowManager::KeyCode_Right; return; }
		case SDL_SCANCODE_UP: { keyCode = IWindowManager::KeyCode_Up; return; }
		case SDL_SCANCODE_DOWN: { keyCode = IWindowManager::KeyCode_Down; return; }
		case SDL_SCANCODE_RETURN: { keyCode = IWindowManager::KeyCode_Return; return; }
		case SDL_SCANCODE_ESCAPE: { keyCode = IWindowManager::KeyCode_Escape; return; }
		case SDL_SCANCODE_BACKSPACE: { keyCode = IWindowManager::KeyCode_Backspace; return; }
		case SDL_SCANCODE_TAB: { keyCode = IWindowManager::KeyCode_Tab; return; }
		case SDL_SCANCODE_SPACE: { keyCode = IWindowManager::KeyCode_Spcae; return; }
		case SDL_SCANCODE_MINUS: { keyCode = IWindowManager::KeyCode_Minus; return; }
		case SDL_SCANCODE_EQUALS: { keyCode = IWindowManager::KeyCode_Equals; return; }
		case SDL_SCANCODE_LEFTBRACKET: { keyCode = IWindowManager::KeyCode_LeftBracket; return; }
		case SDL_SCANCODE_RIGHTBRACKET: { keyCode = IWindowManager::KeyCode_RightBracket; return; }
		case SDL_SCANCODE_BACKSLASH: { keyCode = IWindowManager::KeyCode_BackSlash; return; }
		case SDL_SCANCODE_SEMICOLON: { keyCode = IWindowManager::KeyCode_Semicolon; return; }
		case SDL_SCANCODE_APOSTROPHE: { keyCode = IWindowManager::KeyCode_Apostrophe; return; }
		case SDL_SCANCODE_GRAVE: { keyCode = IWindowManager::KeyCode_Grave; return; }
		case SDL_SCANCODE_COMMA: { keyCode = IWindowManager::KeyCode_Comma; return; }
		case SDL_SCANCODE_PERIOD: { keyCode = IWindowManager::KeyCode_Dot; return; }
		case SDL_SCANCODE_SLASH: { keyCode = IWindowManager::KeyCode_Slash; return; }
		case SDL_SCANCODE_CAPSLOCK: { keyCode = IWindowManager::KeyCode_CapsLock; return; }
		case SDL_SCANCODE_PRINTSCREEN: { keyCode = IWindowManager::KeyCode_PrintScreen; return; }
		case SDL_SCANCODE_SCROLLLOCK: { keyCode = IWindowManager::KeyCode_ScrollLock; return; }
		case SDL_SCANCODE_PAUSE: { keyCode = IWindowManager::KeyCode_Pause; return; }
		case SDL_SCANCODE_INSERT: { keyCode = IWindowManager::KeyCode_Insert; return; }
		case SDL_SCANCODE_HOME: { keyCode = IWindowManager::KeyCode_Home; return; }
		case SDL_SCANCODE_PAGEUP: { keyCode = IWindowManager::KeyCode_PageUp; return; }
		case SDL_SCANCODE_DELETE: { keyCode = IWindowManager::KeyCode_Delete; return; }
		case SDL_SCANCODE_END: { keyCode = IWindowManager::KeyCode_End; return; }
		case SDL_SCANCODE_PAGEDOWN: { keyCode = IWindowManager::KeyCode_PageDown; return; }
		case SDL_SCANCODE_NUMLOCKCLEAR: { keyCode = IWindowManager::KeyCode_NumLock; return; }
		case SDL_SCANCODE_KP_DIVIDE: { keyCode = IWindowManager::KeyCode_KeyPadDivide; return; }
		case SDL_SCANCODE_KP_MULTIPLY: { keyCode = IWindowManager::KeyCode_KeyPadMultiply; return; }
		case SDL_SCANCODE_KP_MINUS: { keyCode = IWindowManager::KeyCode_KeyPadMinus; return; }
		case SDL_SCANCODE_KP_PLUS: { keyCode = IWindowManager::KeyCode_KeyPadPlus; return; }
		case SDL_SCANCODE_KP_ENTER: { keyCode = IWindowManager::KeyCode_KeyPadEnter; return; }
		case SDL_SCANCODE_KP_0: { keyCode = IWindowManager::KeyCode_KeyPad0; return; }
		case SDL_SCANCODE_KP_1: { keyCode = IWindowManager::KeyCode_KeyPad1; return; }
		case SDL_SCANCODE_KP_2: { keyCode = IWindowManager::KeyCode_KeyPad2; return; }
		case SDL_SCANCODE_KP_3: { keyCode = IWindowManager::KeyCode_KeyPad3; return; }
		case SDL_SCANCODE_KP_4: { keyCode = IWindowManager::KeyCode_KeyPad4; return; }
		case SDL_SCANCODE_KP_5: { keyCode = IWindowManager::KeyCode_KeyPad5; return; }
		case SDL_SCANCODE_KP_6: { keyCode = IWindowManager::KeyCode_KeyPad6; return; }
		case SDL_SCANCODE_KP_7: { keyCode = IWindowManager::KeyCode_KeyPad7; return; }
		case SDL_SCANCODE_KP_8: { keyCode = IWindowManager::KeyCode_KeyPad8; return; }
		case SDL_SCANCODE_KP_9: { keyCode = IWindowManager::KeyCode_KeyPad9; return; }
		case SDL_SCANCODE_KP_PERIOD: { keyCode = IWindowManager::KeyCode_KeyPadDot; return; }
		case SDL_SCANCODE_LCTRL: { keyCode = IWindowManager::KeyCode_LeftCtrl; return; }
		case SDL_SCANCODE_RCTRL: { keyCode = IWindowManager::KeyCode_RightCtrl; return; }
		case SDL_SCANCODE_LSHIFT: { keyCode = IWindowManager::KeyCode_LeftShift; return; }
		case SDL_SCANCODE_RSHIFT: { keyCode = IWindowManager::KeyCode_RightShift; return; }
		case SDL_SCANCODE_LALT: { keyCode = IWindowManager::KeyCode_LeftAlt; return; }
		case SDL_SCANCODE_RALT: { keyCode = IWindowManager::KeyCode_RightAlt; return; }


		default: break;
	}
	switch (sdlKey.sym) {
		default: break;
	}
}

uint32 sdlMouseButtonsStateToMask(Uint32 buttonsState)
{
	uint32 mask = 0;
	if (buttonsState & SDL_BUTTON_LMASK) mask |= IWindowManager::MouseButton_Left;
	if (buttonsState & SDL_BUTTON_MMASK) mask |= IWindowManager::MouseButton_Middle;
	if (buttonsState & SDL_BUTTON_RMASK) mask |= IWindowManager::MouseButton_Right;
	if (buttonsState & SDL_BUTTON_X1MASK) mask |= IWindowManager::MouseButton_X1;
	if (buttonsState & SDL_BUTTON_X2MASK) mask |= IWindowManager::MouseButton_X2;
	return mask;
}

IWindowManager::MouseButton sdlMouseButtonToMouseButton(Uint8 sdlMouseButton)
{
	if (sdlMouseButton == SDL_BUTTON_LEFT) return IWindowManager::MouseButton_Left;
	else if (sdlMouseButton == SDL_BUTTON_MIDDLE) return IWindowManager::MouseButton_Middle;
	else if (sdlMouseButton == SDL_BUTTON_RIGHT) return IWindowManager::MouseButton_Right;
	else if (sdlMouseButton == SDL_BUTTON_X1) return IWindowManager::MouseButton_X1;
	else if (sdlMouseButton == SDL_BUTTON_X2) return IWindowManager::MouseButton_X2;
	return (IWindowManager::MouseButton)0;
}

uint32 sdlKeyModifiersToMask(SDL_Keymod mod)
{
	uint32 modifiers = 0;
	if (mod & KMOD_LSHIFT) modifiers |= IWindowManager::KeyModifier_LeftShift;
	if (mod & KMOD_RSHIFT) modifiers |= IWindowManager::KeyModifier_RightShift;
	if (mod & KMOD_LCTRL) modifiers |= IWindowManager::KeyModifier_LeftCtrl;
	if (mod & KMOD_RCTRL) modifiers |= IWindowManager::KeyModifier_RightCtrl;
	if (mod & KMOD_LALT) modifiers |= IWindowManager::KeyModifier_LeftAlt;
	if (mod & KMOD_RALT) modifiers |= IWindowManager::KeyModifier_RightAlt;
	return modifiers;
}

SDL_Scancode keyCodeToSDLScancode(IWindowManager::KeyCode keyCode)
{
	switch (keyCode) {
		case IWindowManager::KeyCode_0: return SDL_SCANCODE_0;
		case IWindowManager::KeyCode_1: return SDL_SCANCODE_1;
		case IWindowManager::KeyCode_2: return SDL_SCANCODE_2;
		case IWindowManager::KeyCode_3: return SDL_SCANCODE_3;
		case IWindowManager::KeyCode_4: return SDL_SCANCODE_4;
		case IWindowManager::KeyCode_5: return SDL_SCANCODE_5;
		case IWindowManager::KeyCode_6: return SDL_SCANCODE_6;
		case IWindowManager::KeyCode_7: return SDL_SCANCODE_7;
		case IWindowManager::KeyCode_8: return SDL_SCANCODE_8;
		case IWindowManager::KeyCode_9: return SDL_SCANCODE_9;
		case IWindowManager::KeyCode_A: return SDL_SCANCODE_A;
		case IWindowManager::KeyCode_B: return SDL_SCANCODE_B;
		case IWindowManager::KeyCode_C: return SDL_SCANCODE_C;
		case IWindowManager::KeyCode_D: return SDL_SCANCODE_D;
		case IWindowManager::KeyCode_E: return SDL_SCANCODE_E;
		case IWindowManager::KeyCode_F: return SDL_SCANCODE_F;
		case IWindowManager::KeyCode_G: return SDL_SCANCODE_G;
		case IWindowManager::KeyCode_H: return SDL_SCANCODE_H;
		case IWindowManager::KeyCode_I: return SDL_SCANCODE_I;
		case IWindowManager::KeyCode_J: return SDL_SCANCODE_J;
		case IWindowManager::KeyCode_K: return SDL_SCANCODE_K;
		case IWindowManager::KeyCode_L: return SDL_SCANCODE_L;
		case IWindowManager::KeyCode_M: return SDL_SCANCODE_M;
		case IWindowManager::KeyCode_N: return SDL_SCANCODE_N;
		case IWindowManager::KeyCode_O: return SDL_SCANCODE_O;
		case IWindowManager::KeyCode_P: return SDL_SCANCODE_P;
		case IWindowManager::KeyCode_Q: return SDL_SCANCODE_Q;
		case IWindowManager::KeyCode_R: return SDL_SCANCODE_R;
		case IWindowManager::KeyCode_S: return SDL_SCANCODE_S;
		case IWindowManager::KeyCode_T: return SDL_SCANCODE_T;
		case IWindowManager::KeyCode_U: return SDL_SCANCODE_U;
		case IWindowManager::KeyCode_V: return SDL_SCANCODE_V;
		case IWindowManager::KeyCode_W: return SDL_SCANCODE_W;
		case IWindowManager::KeyCode_X: return SDL_SCANCODE_X;
		case IWindowManager::KeyCode_Y: return SDL_SCANCODE_Y;
		case IWindowManager::KeyCode_Z: return SDL_SCANCODE_Z;
		case IWindowManager::KeyCode_F1: return SDL_SCANCODE_F1;
		case IWindowManager::KeyCode_F2: return SDL_SCANCODE_F2;
		case IWindowManager::KeyCode_F3: return SDL_SCANCODE_F3;
		case IWindowManager::KeyCode_F4: return SDL_SCANCODE_F4;
		case IWindowManager::KeyCode_F5: return SDL_SCANCODE_F5;
		case IWindowManager::KeyCode_F6: return SDL_SCANCODE_F6;
		case IWindowManager::KeyCode_F7: return SDL_SCANCODE_F7;
		case IWindowManager::KeyCode_F8: return SDL_SCANCODE_F8;
		case IWindowManager::KeyCode_F9: return SDL_SCANCODE_F9;
		case IWindowManager::KeyCode_F10: return SDL_SCANCODE_F10;
		case IWindowManager::KeyCode_F11: return SDL_SCANCODE_F11;
		case IWindowManager::KeyCode_F12: return SDL_SCANCODE_F12;
		case IWindowManager::KeyCode_Left: return SDL_SCANCODE_LEFT;
		case IWindowManager::KeyCode_Right: return SDL_SCANCODE_RIGHT;
		case IWindowManager::KeyCode_Up: return SDL_SCANCODE_UP;
		case IWindowManager::KeyCode_Down: return SDL_SCANCODE_DOWN;
		case IWindowManager::KeyCode_Return: return SDL_SCANCODE_RETURN;
		case IWindowManager::KeyCode_Escape: return SDL_SCANCODE_ESCAPE;
		case IWindowManager::KeyCode_Backspace: return SDL_SCANCODE_BACKSPACE;
		case IWindowManager::KeyCode_Tab: return SDL_SCANCODE_TAB;
		case IWindowManager::KeyCode_Spcae: return SDL_SCANCODE_SPACE;
		case IWindowManager::KeyCode_Minus: return SDL_SCANCODE_MINUS;
		case IWindowManager::KeyCode_Equals: return SDL_SCANCODE_EQUALS;
		case IWindowManager::KeyCode_LeftBracket: return SDL_SCANCODE_LEFTBRACKET;
		case IWindowManager::KeyCode_RightBracket: return SDL_SCANCODE_RIGHTBRACKET;
		case IWindowManager::KeyCode_BackSlash: return SDL_SCANCODE_BACKSLASH;
		case IWindowManager::KeyCode_Semicolon: return SDL_SCANCODE_SEMICOLON;
		case IWindowManager::KeyCode_Apostrophe: return SDL_SCANCODE_APOSTROPHE;
		case IWindowManager::KeyCode_Grave: return SDL_SCANCODE_GRAVE;
		case IWindowManager::KeyCode_Comma: return SDL_SCANCODE_COMMA;
		case IWindowManager::KeyCode_Dot: return SDL_SCANCODE_PERIOD;
		case IWindowManager::KeyCode_Slash: return SDL_SCANCODE_SLASH;
		case IWindowManager::KeyCode_CapsLock: return SDL_SCANCODE_CAPSLOCK;
		case IWindowManager::KeyCode_PrintScreen: return SDL_SCANCODE_PRINTSCREEN;
		case IWindowManager::KeyCode_ScrollLock: return SDL_SCANCODE_SCROLLLOCK;
		case IWindowManager::KeyCode_Pause: return SDL_SCANCODE_PAUSE;
		case IWindowManager::KeyCode_Insert: return SDL_SCANCODE_INSERT;
		case IWindowManager::KeyCode_Home: return SDL_SCANCODE_HOME;
		case IWindowManager::KeyCode_PageUp: return SDL_SCANCODE_PAGEUP;
		case IWindowManager::KeyCode_Delete: return SDL_SCANCODE_DELETE;
		case IWindowManager::KeyCode_End: return SDL_SCANCODE_END;
		case IWindowManager::KeyCode_PageDown: return SDL_SCANCODE_PAGEDOWN;
		case IWindowManager::KeyCode_NumLock: return SDL_SCANCODE_NUMLOCKCLEAR;
		case IWindowManager::KeyCode_KeyPadDivide: return SDL_SCANCODE_KP_DIVIDE;
		case IWindowManager::KeyCode_KeyPadMultiply: return SDL_SCANCODE_KP_MULTIPLY;
		case IWindowManager::KeyCode_KeyPadMinus: return SDL_SCANCODE_KP_MINUS;
		case IWindowManager::KeyCode_KeyPadPlus: return SDL_SCANCODE_KP_PLUS;
		case IWindowManager::KeyCode_KeyPadEnter: return SDL_SCANCODE_KP_ENTER;
		case IWindowManager::KeyCode_KeyPad0: return SDL_SCANCODE_KP_0;
		case IWindowManager::KeyCode_KeyPad1: return SDL_SCANCODE_KP_1;
		case IWindowManager::KeyCode_KeyPad2: return SDL_SCANCODE_KP_2;
		case IWindowManager::KeyCode_KeyPad3: return SDL_SCANCODE_KP_3;
		case IWindowManager::KeyCode_KeyPad4: return SDL_SCANCODE_KP_4;
		case IWindowManager::KeyCode_KeyPad5: return SDL_SCANCODE_KP_5;
		case IWindowManager::KeyCode_KeyPad6: return SDL_SCANCODE_KP_6;
		case IWindowManager::KeyCode_KeyPad7: return SDL_SCANCODE_KP_7;
		case IWindowManager::KeyCode_KeyPad8: return SDL_SCANCODE_KP_8;
		case IWindowManager::KeyCode_KeyPad9: return SDL_SCANCODE_KP_9;
		case IWindowManager::KeyCode_KeyPadDot: return SDL_SCANCODE_KP_PERIOD;
		case IWindowManager::KeyCode_LeftCtrl: return SDL_SCANCODE_LCTRL;
		case IWindowManager::KeyCode_RightCtrl: return SDL_SCANCODE_RCTRL;
		case IWindowManager::KeyCode_LeftShift: return SDL_SCANCODE_LSHIFT;
		case IWindowManager::KeyCode_RightShift: return SDL_SCANCODE_RSHIFT;
		case IWindowManager::KeyCode_LeftAlt: return SDL_SCANCODE_LALT;
		case IWindowManager::KeyCode_RightAlt: return SDL_SCANCODE_RALT;
		default: return SDL_SCANCODE_UNKNOWN;
	};
}

} // namespace