#include <FreeWorldEngine.h>

#include <algorithm>

#include "Window.h"

namespace FreeWorldEngine {

Window::Window(const std::string& name, const std::string& title, const int32 width, const int32 height, const bool fullscreen, const bool resizable) :
	m_name(name),
	m_frameNumber(-1),
	m_pUserData(0),
	m_showCallBacks(),
	m_hideCallBacks(),
	m_moveCallBacks(),
	m_resizeCallBacks(),
	m_minimizeCallBacks(),
	m_restoreCallBacks(),
	m_maximizeCallBacks(),
	m_closeCallBacks(),
	m_renderCallBacks(),
	m_updateCallBacks(),
	m_mouseDownCallBacks(),
	m_mouseUpCallBacks(),
	m_mouseMotionCallBacks(),
	m_mouseWheelCallBacks(),
	m_keyDownCallBacks(),
	m_keyUpCallBacks()
{
	m_window = SDL_CreateWindow(
		title.c_str(),
		fullscreen ? 0 : SDL_WINDOWPOS_CENTERED,
		fullscreen ? 0 : SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_HIDDEN | (resizable ? SDL_WINDOW_RESIZABLE : 0));
	if (!m_window)
		getCoreEngine()->logger()->printMessage("Don't create window.", ILogger::MessageType_Error);

	m_glContext = SDL_GL_CreateContext(m_window);
	if (!m_glContext) {
		if (m_window) {
			SDL_DestroyWindow(m_window);
			m_window = 0;
		}
		getCoreEngine()->logger()->printMessage("Don't create window gl context.", ILogger::MessageType_Error);
	}
}

Window::~Window()
{
}

std::string Window::name() const
{
	return m_name;
}

void Window::move(int32 x, int32 y)
{
	SDL_SetWindowPosition(m_window, x, y);
	std::for_each(m_moveCallBacks.cbegin(), m_moveCallBacks.cend(), [x, y, this](const MoveCallBack p) { p(x, y, this); });
}

void Window::resize(int32 width, int32 height)
{
	SDL_SetWindowSize(m_window, width, height);
	std::for_each(m_resizeCallBacks.cbegin(), m_resizeCallBacks.cend(), [width, height, this](const ResizeCallBack p) { p(width, height, this); });
}

void Window::show()
{
	SDL_ShowWindow(m_window);
	std::for_each(m_showCallBacks.cbegin(), m_showCallBacks.cend(), [this](const ShowCallBack p) { p(this); });
}

void Window::hide()
{
	SDL_HideWindow(m_window);
	std::for_each(m_hideCallBacks.cbegin(), m_hideCallBacks.cend(), [this](const HideCallBack p) { p(this); });
}

void Window::minimize()
{
	SDL_MinimizeWindow(m_window);
	std::for_each(m_minimizeCallBacks.cbegin(), m_minimizeCallBacks.cend(), [this](const MinimizeCallBack p) { p(this); });
}

void Window::restore()
{
	SDL_RestoreWindow(m_window);
	std::for_each(m_restoreCallBacks.cbegin(), m_restoreCallBacks.cend(), [this](const RestoreCallBack p) { p(this); });
}

void Window::maximize()
{
	SDL_MaximizeWindow(m_window);
	std::for_each(m_maximizeCallBacks.cbegin(), m_maximizeCallBacks.cend(), [this](const MaximizeCallBack p) { p(this); });
}

void Window::registerShowCallBack(ShowCallBack callback)
{
	if (std::find(m_showCallBacks.cbegin(), m_showCallBacks.cend(), callback) == m_showCallBacks.cend())
		m_showCallBacks.push_back(callback);
}

void Window::registerHideCallBack(HideCallBack callback)
{
	if (std::find(m_hideCallBacks.cbegin(), m_hideCallBacks.cend(), callback) == m_hideCallBacks.cend())
		m_hideCallBacks.push_back(callback);
}

void Window::registerMoveCallBack(MoveCallBack callback)
{
	if (std::find(m_moveCallBacks.cbegin(), m_moveCallBacks.cend(), callback) == m_moveCallBacks.cend())
		m_moveCallBacks.push_back(callback);
}

void Window::registerResizeCallBack(ResizeCallBack callback)
{
	if (std::find(m_resizeCallBacks.cbegin(), m_resizeCallBacks.cend(), callback) == m_resizeCallBacks.cend())
		m_resizeCallBacks.push_back(callback);
}

void Window::registerMinimizeCallBack(MinimizeCallBack callback)
{
	if (std::find(m_minimizeCallBacks.cbegin(), m_minimizeCallBacks.cend(), callback) == m_minimizeCallBacks.cend())
		m_minimizeCallBacks.push_back(callback);
}

void Window::registerRestoreCallBack(RestoreCallBack callback)
{
	if (std::find(m_restoreCallBacks.cbegin(), m_restoreCallBacks.cend(), callback) == m_restoreCallBacks.cend())
		m_restoreCallBacks.push_back(callback);
}

void Window::registerMaximizeCallBack(MaximizeCallBack callback)
{
	if (std::find(m_maximizeCallBacks.cbegin(), m_maximizeCallBacks.cend(), callback) == m_maximizeCallBacks.cend())
		m_maximizeCallBacks.push_back(callback);
}

void Window::registerCloseCallBack(CloseCallBack callback)
{
	if (std::find(m_closeCallBacks.cbegin(), m_closeCallBacks.cend(), callback) == m_closeCallBacks.cend())
		m_closeCallBacks.push_back(callback);
}

void Window::registerRenderCallBack(RenderCallBack callback)
{
	if (std::find(m_renderCallBacks.cbegin(), m_renderCallBacks.cend(), callback) == m_renderCallBacks.cend())
		m_renderCallBacks.push_back(callback);
}

void Window::registerUpdateCallBack(UpdateCallBack callback)
{
	if (std::find(m_updateCallBacks.cbegin(), m_updateCallBacks.cend(), callback) == m_updateCallBacks.cend())
		m_updateCallBacks.push_back(callback);
}

void Window::registerMouseDownCallBack(MouseDownCallBack callback)
{
	if (std::find(m_mouseDownCallBacks.cbegin(), m_mouseDownCallBacks.cend(), callback) == m_mouseDownCallBacks.cend())
		m_mouseDownCallBacks.push_back(callback);
}

void Window::registerMouseUpCallBack(MouseUpCallBack callback)
{
	if (std::find(m_mouseUpCallBacks.cbegin(), m_mouseUpCallBacks.cend(), callback) == m_mouseUpCallBacks.cend())
		m_mouseUpCallBacks.push_back(callback);
}

void Window::registerMouseMotionCallBack(MouseMotionCallBack callback)
{
	if (std::find(m_mouseMotionCallBacks.cbegin(), m_mouseMotionCallBacks.cend(), callback) == m_mouseMotionCallBacks.cend())
		m_mouseMotionCallBacks.push_back(callback);
}

void Window::registerMouseWheelCallBack(MouseWheelCallBack callback)
{
	if (std::find(m_mouseWheelCallBacks.cbegin(), m_mouseWheelCallBacks.cend(), callback) == m_mouseWheelCallBacks.cend())
		m_mouseWheelCallBacks.push_back(callback);
}

void Window::registerKeyDownCallBack(KeyDownCallBack callback)
{
	if (std::find(m_keyDownCallBacks.cbegin(), m_keyDownCallBacks.cend(), callback) == m_keyDownCallBacks.cend())
		m_keyDownCallBacks.push_back(callback);
}

void Window::registerKeyUpCallBack(KeyUpCallBack callback)
{
	if (std::find(m_keyUpCallBacks.cbegin(), m_keyUpCallBacks.cend(), callback) == m_keyUpCallBacks.cend())
		m_keyUpCallBacks.push_back(callback);
}

void Window::unregisterShowCallBack(ShowCallBack callback)
{
	std::vector<ShowCallBack>::const_iterator it;
	if ((it = std::find(m_showCallBacks.cbegin(), m_showCallBacks.cend(), callback)) != m_showCallBacks.cend())
		m_showCallBacks.erase(it, it+1);
}

void Window::unregisterHideCallBack(HideCallBack callback)
{
	std::vector<HideCallBack>::const_iterator it;
	if ((it = std::find(m_hideCallBacks.cbegin(), m_hideCallBacks.cend(), callback)) != m_hideCallBacks.cend())
		m_hideCallBacks.erase(it, it+1);
}

void Window::unregisterMoveCallBack(MoveCallBack callback)
{
	std::vector<MoveCallBack>::const_iterator it;
	if ((it = std::find(m_moveCallBacks.cbegin(), m_moveCallBacks.cend(), callback)) != m_moveCallBacks.cend())
		m_moveCallBacks.erase(it, it+1);
}

void Window::unregisterResizeCallBack(ResizeCallBack callback)
{
	std::vector<ResizeCallBack>::const_iterator it;
	if ((it = std::find(m_resizeCallBacks.cbegin(), m_resizeCallBacks.cend(), callback)) != m_resizeCallBacks.cend())
		m_resizeCallBacks.erase(it, it+1);
}

void Window::unregisterMinimizeCallBack(MinimizeCallBack callback)
{
	std::vector<MinimizeCallBack>::const_iterator it;
	if ((it = std::find(m_minimizeCallBacks.cbegin(), m_minimizeCallBacks.cend(), callback)) != m_minimizeCallBacks.cend())
		m_minimizeCallBacks.erase(it, it+1);
}

void Window::unregisterRestoreCallBack(RestoreCallBack callback)
{
	std::vector<RestoreCallBack>::const_iterator it;
	if ((it = std::find(m_restoreCallBacks.cbegin(), m_restoreCallBacks.cend(), callback)) != m_restoreCallBacks.cend())
		m_restoreCallBacks.erase(it, it+1);
}

void Window::unregisterMaximizeCallBack(MaximizeCallBack callback)
{
	std::vector<MaximizeCallBack>::const_iterator it;
	if ((it = std::find(m_maximizeCallBacks.cbegin(), m_maximizeCallBacks.cend(), callback)) != m_maximizeCallBacks.cend())
		m_maximizeCallBacks.erase(it, it+1);
}

void Window::unregisterCloseCallBack(CloseCallBack callback)
{
	std::vector<CloseCallBack>::const_iterator it;
	if ((it = std::find(m_closeCallBacks.cbegin(), m_closeCallBacks.cend(), callback)) != m_closeCallBacks.cend())
		m_closeCallBacks.erase(it, it+1);
}

void Window::unregisterRenderCallBack(RenderCallBack callback)
{
	std::vector<RenderCallBack>::const_iterator it;
	if ((it = std::find(m_renderCallBacks.cbegin(), m_renderCallBacks.cend(), callback)) != m_renderCallBacks.cend())
		m_renderCallBacks.erase(it, it+1);
}

void Window::unregisterUpdateCallBack(UpdateCallBack callback)
{
	std::vector<UpdateCallBack>::const_iterator it;
	if ((it = std::find(m_updateCallBacks.cbegin(), m_updateCallBacks.cend(), callback)) != m_updateCallBacks.cend())
		m_updateCallBacks.erase(it, it+1);
}

void Window::unregisterMouseDownCallBack(MouseDownCallBack callback)
{
	std::vector<MouseDownCallBack>::const_iterator it;
	if ((it = std::find(m_mouseDownCallBacks.cbegin(), m_mouseDownCallBacks.cend(), callback)) != m_mouseDownCallBacks.cend())
		m_mouseDownCallBacks.erase(it, it + 1);
}

void Window::unregisterMouseUpCallBack(MouseUpCallBack callback)
{
	std::vector<MouseUpCallBack>::const_iterator it;
	if ((it = std::find(m_mouseUpCallBacks.cbegin(), m_mouseUpCallBacks.cend(), callback)) != m_mouseUpCallBacks.cend())
		m_mouseUpCallBacks.erase(it, it + 1);
}

void Window::unregisterMouseMotionCallBack(MouseMotionCallBack callback)
{
	std::vector<MouseMotionCallBack>::const_iterator it;
	if ((it = std::find(m_mouseMotionCallBacks.cbegin(), m_mouseMotionCallBacks.cend(), callback)) != m_mouseMotionCallBacks.cend())
		m_mouseMotionCallBacks.erase(it, it + 1);
}

void Window::unregisterMouseWheelCallBack(MouseWheelCallBack callback)
{
	std::vector<MouseWheelCallBack>::const_iterator it;
	if ((it = std::find(m_mouseWheelCallBacks.cbegin(), m_mouseWheelCallBacks.cend(), callback)) != m_mouseWheelCallBacks.cend())
		m_mouseWheelCallBacks.erase(it, it + 1);
}

void Window::unregisterKeyDownCallBack(KeyDownCallBack callback)
{
	std::vector<KeyDownCallBack>::const_iterator it;
	if ((it = std::find(m_keyDownCallBacks.cbegin(), m_keyDownCallBacks.cend(), callback)) != m_keyDownCallBacks.cend())
		m_keyDownCallBacks.erase(it, it + 1);
}

void Window::unregisterKeyUpCallBack(KeyUpCallBack callback)
{
	std::vector<KeyUpCallBack>::const_iterator it;
	if ((it = std::find(m_keyUpCallBacks.cbegin(), m_keyUpCallBacks.cend(), callback)) != m_keyUpCallBacks.cend())
		m_keyUpCallBacks.erase(it, it + 1);
}

int32 Window::width() const
{
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	return w;
}

int32 Window::height() const
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

void Window::setUserData(void * pData)
{
	m_pUserData = pData;
}

void *Window::userData() const
{
	return m_pUserData;
}

void Window::render()
{
	m_frameNumber++;
	std::for_each(m_renderCallBacks.cbegin(), m_renderCallBacks.cend(), [this](RenderCallBack p) { p(this); });
	SDL_GL_SwapWindow(m_window);
}

void Window::update(uint32 time, uint32 dt)
{
	std::for_each(m_updateCallBacks.cbegin(), m_updateCallBacks.cend(), [time, dt, this](UpdateCallBack p) { p(time, dt, this); });
}

void Window::mouseButtonDown(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY)
{
	std::for_each(m_mouseDownCallBacks.cbegin(), m_mouseDownCallBacks.cend(), [button, numClicks, cursorPosX, cursorPosY, this](MouseDownCallBack p) { p(button, numClicks, cursorPosX, cursorPosY, this); });
}

void Window::mouseButtonUp(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY)
{
	std::for_each(m_mouseUpCallBacks.cbegin(), m_mouseUpCallBacks.cend(), [button, numClicks, cursorPosX, cursorPosY, this](MouseUpCallBack p) { p(button, numClicks, cursorPosX, cursorPosY, this); });
}

void Window::mouseMotion(MouseButtons buttons, int32 cursorPosX, int32 cursorPosY, int32 relCursorPosX, int32 relCursorPosY)
{
	std::for_each(m_mouseMotionCallBacks.cbegin(), m_mouseMotionCallBacks.cend(), [buttons, cursorPosX, cursorPosY, relCursorPosX, relCursorPosY, this](MouseMotionCallBack p) { p(buttons, cursorPosX, cursorPosY, relCursorPosX, relCursorPosY, this); });
}

void Window::mouseWheel(int32 dirX, int32 dirY)
{
	std::for_each(m_mouseWheelCallBacks.cbegin(), m_mouseWheelCallBacks.cend(), [dirX, dirY, this](MouseWheelCallBack p) { p(dirX, dirY, this); });
}

void Window::keyDown(KeyCode keyCode)
{
	std::for_each(m_keyDownCallBacks.cbegin(), m_keyDownCallBacks.cend(), [keyCode, this](KeyDownCallBack p) { p(keyCode, this); });
}

void Window::keyUp(KeyCode keyCode)
{
	std::for_each(m_keyUpCallBacks.cbegin(), m_keyUpCallBacks.cend(), [keyCode, this](KeyUpCallBack p) { p(keyCode, this); });
}

void Window::sendEvent(const SDL_WindowEvent& windowEvent)
{
	switch (windowEvent.event) {
		case SDL_WINDOWEVENT_SHOWN: {
			std::for_each(m_showCallBacks.cbegin(), m_showCallBacks.cend(), [this](ShowCallBack p) { p(this); });
			break;
		}
		case SDL_WINDOWEVENT_HIDDEN: {
			std::for_each(m_hideCallBacks.cbegin(), m_hideCallBacks.cend(), [this](HideCallBack p) { p(this); });
			break;
		}
		case SDL_WINDOWEVENT_MOVED: {
			std::for_each(m_moveCallBacks.cbegin(), m_moveCallBacks.cend(), [&windowEvent, this](MoveCallBack p) { p(windowEvent.data1, windowEvent.data2, this); });
			break;
		}
		case SDL_WINDOWEVENT_RESIZED: {
			std::for_each(m_resizeCallBacks.cbegin(), m_resizeCallBacks.cend(), [&windowEvent, this](ResizeCallBack p) { p(windowEvent.data1, windowEvent.data2, this); });
			break;
		}
		case SDL_WINDOWEVENT_MINIMIZED: {
			std::for_each(m_minimizeCallBacks.cbegin(), m_minimizeCallBacks.cend(), [this](MinimizeCallBack p) { p(this); });
			break;
		}
		case SDL_WINDOWEVENT_RESTORED: {
			std::for_each(m_restoreCallBacks.cbegin(), m_restoreCallBacks.cend(), [this](RestoreCallBack p) { p(this); });
			break;
		}
		case SDL_WINDOWEVENT_MAXIMIZED: {
			std::for_each(m_maximizeCallBacks.cbegin(), m_maximizeCallBacks.cend(), [this](MaximizeCallBack p) { p(this); });
			break;
		}
		case SDL_WINDOWEVENT_CLOSE: {
			std::for_each(m_closeCallBacks.cbegin(), m_closeCallBacks.cend(), [this](CloseCallBack p) { p(this); });
			break;
		}
		default: break;
	}
}

/*SDL_Window *Window::window() const
{
	return m_window;
}

SDL_GLContext Window::context() const
{
	return m_glContext;
}*/

Uint32 Window::id() const
{
	return SDL_GetWindowID(m_window);
}

IWindow::KeyCode Window::sdlKeysymToKeyCode(const SDL_Keysym& sdlKey)
{
	IWindow::KeyCode keyCode = KeyCode_None;
	switch (sdlKey.scancode) {
	case SDL_SCANCODE_UNKNOWN: break;
	case SDL_SCANCODE_0: { keyCode = KeyCode_0; break; }
	case SDL_SCANCODE_1: { keyCode = KeyCode_1; break; }
	case SDL_SCANCODE_2: { keyCode = KeyCode_2; break; }
	case SDL_SCANCODE_3: { keyCode = KeyCode_3; break; }
	case SDL_SCANCODE_4: { keyCode = KeyCode_4; break; }
	case SDL_SCANCODE_5: { keyCode = KeyCode_5; break; }
	case SDL_SCANCODE_6: { keyCode = KeyCode_6; break; }
	case SDL_SCANCODE_7: { keyCode = KeyCode_7; break; }
	case SDL_SCANCODE_8: { keyCode = KeyCode_8; break; }
	case SDL_SCANCODE_9: { keyCode = KeyCode_9; break; }
	case SDL_SCANCODE_A: { keyCode = KeyCode_A; break; }
	case SDL_SCANCODE_B: { keyCode = KeyCode_B; break; }
	case SDL_SCANCODE_C: { keyCode = KeyCode_C; break; }
	case SDL_SCANCODE_D: { keyCode = KeyCode_D; break; }
	case SDL_SCANCODE_E: { keyCode = KeyCode_E; break; }
	case SDL_SCANCODE_F: { keyCode = KeyCode_F; break; }
	case SDL_SCANCODE_G: { keyCode = KeyCode_G; break; }
	case SDL_SCANCODE_H: { keyCode = KeyCode_H; break; }
	case SDL_SCANCODE_I: { keyCode = KeyCode_I; break; }
	case SDL_SCANCODE_J: { keyCode = KeyCode_J; break; }
	case SDL_SCANCODE_K: { keyCode = KeyCode_K; break; }
	case SDL_SCANCODE_L: { keyCode = KeyCode_L; break; }
	case SDL_SCANCODE_M: { keyCode = KeyCode_M; break; }
	case SDL_SCANCODE_N: { keyCode = KeyCode_N; break; }
	case SDL_SCANCODE_O: { keyCode = KeyCode_O; break; }
	case SDL_SCANCODE_P: { keyCode = KeyCode_P; break; }
	case SDL_SCANCODE_Q: { keyCode = KeyCode_Q; break; }
	case SDL_SCANCODE_R: { keyCode = KeyCode_R; break; }
	case SDL_SCANCODE_S: { keyCode = KeyCode_S; break; }
	case SDL_SCANCODE_T: { keyCode = KeyCode_T; break; }
	case SDL_SCANCODE_U: { keyCode = KeyCode_U; break; }
	case SDL_SCANCODE_V: { keyCode = KeyCode_V; break; }
	case SDL_SCANCODE_W: { keyCode = KeyCode_W; break; }
	case SDL_SCANCODE_X: { keyCode = KeyCode_X; break; }
	case SDL_SCANCODE_Y: { keyCode = KeyCode_Y; break; }
	case SDL_SCANCODE_Z: { keyCode = KeyCode_Z; break; }
	case SDL_SCANCODE_F1: { keyCode = KeyCode_F1; break; }
	case SDL_SCANCODE_F2: { keyCode = KeyCode_F2; break; }
	case SDL_SCANCODE_F3: { keyCode = KeyCode_F3; break; }
	case SDL_SCANCODE_F4: { keyCode = KeyCode_F4; break; }
	case SDL_SCANCODE_F5: { keyCode = KeyCode_F5; break; }
	case SDL_SCANCODE_F6: { keyCode = KeyCode_F6; break; }
	case SDL_SCANCODE_F7: { keyCode = KeyCode_F7; break; }
	case SDL_SCANCODE_F8: { keyCode = KeyCode_F8; break; }
	case SDL_SCANCODE_F9: { keyCode = KeyCode_F9; break; }
	case SDL_SCANCODE_F10: { keyCode = KeyCode_F10; break; }
	case SDL_SCANCODE_F11: { keyCode = KeyCode_F11; break; }
	case SDL_SCANCODE_F12: { keyCode = KeyCode_F12; break; }
	case SDL_SCANCODE_LEFT: { keyCode = KeyCode_Left; break; }
	case SDL_SCANCODE_RIGHT: { keyCode = KeyCode_Right; break; }
	case SDL_SCANCODE_UP: { keyCode = KeyCode_Up; break; }
	case SDL_SCANCODE_DOWN: { keyCode = KeyCode_Down; break; }
	case SDL_SCANCODE_RETURN: { keyCode = KeyCode_Return; break; }
	case SDL_SCANCODE_ESCAPE: { keyCode = KeyCode_Escape; break; }
	case SDL_SCANCODE_BACKSPACE: { keyCode = KeyCode_Backspace; break; }
	case SDL_SCANCODE_TAB: { keyCode = KeyCode_Tab; break; }
	case SDL_SCANCODE_SPACE: { keyCode = KeyCode_Spcae; break; }
	case SDL_SCANCODE_MINUS: { keyCode = KeyCode_Minus; break; }
	case SDL_SCANCODE_EQUALS: { keyCode = KeyCode_Equals; break; }
	case SDL_SCANCODE_LEFTBRACKET: { keyCode = KeyCode_LeftBracket; break; }
	case SDL_SCANCODE_RIGHTBRACKET: { keyCode = KeyCode_RightBracket; break; }
	case SDL_SCANCODE_BACKSLASH: { keyCode = KeyCode_BackSlash; break; }
	case SDL_SCANCODE_SEMICOLON: { keyCode = KeyCode_Semicolon; break; }
	case SDL_SCANCODE_APOSTROPHE: { keyCode = KeyCode_Apostrophe; break; }
	case SDL_SCANCODE_GRAVE: { keyCode = KeyCode_Grave; break; }
	case SDL_SCANCODE_COMMA: { keyCode = KeyCode_Comma; break; }
	case SDL_SCANCODE_PERIOD: { keyCode = KeyCode_Dot; break; }
	case SDL_SCANCODE_SLASH: { keyCode = KeyCode_Slash; break; }
	case SDL_SCANCODE_CAPSLOCK: { keyCode = KeyCode_CapsLock; break; }
	case SDL_SCANCODE_PRINTSCREEN: { keyCode = KeyCode_PrintScreen; break; }
	case SDL_SCANCODE_SCROLLLOCK: { keyCode = KeyCode_ScrollLock; break; }
	case SDL_SCANCODE_PAUSE: { keyCode = KeyCode_Pause; break; }
	case SDL_SCANCODE_INSERT: { keyCode = KeyCode_Insert; break; }
	case SDL_SCANCODE_HOME: { keyCode = KeyCode_Home; break; }
	case SDL_SCANCODE_PAGEUP: { keyCode = KeyCode_PageUp; break; }
	case SDL_SCANCODE_DELETE: { keyCode = KeyCode_Delete; break; }
	case SDL_SCANCODE_END: { keyCode = KeyCode_End; break; }
	case SDL_SCANCODE_PAGEDOWN: { keyCode = KeyCode_PageDown; break; }
	case SDL_SCANCODE_NUMLOCKCLEAR: { keyCode = KeyCode_NumLock; break; }
	case SDL_SCANCODE_KP_DIVIDE: { keyCode = KeyCode_KeyPadDivide; break; }
	case SDL_SCANCODE_KP_MULTIPLY: { keyCode = KeyCode_KeyPadMultiply; break; }
	case SDL_SCANCODE_KP_MINUS: { keyCode = KeyCode_KeyPadMinus; break; }
	case SDL_SCANCODE_KP_PLUS: { keyCode = KeyCode_KeyPadPlus; break; }
	case SDL_SCANCODE_KP_ENTER: { keyCode = KeyCode_KeyPadEnter; break; }
	case SDL_SCANCODE_KP_0: { keyCode = KeyCode_KeyPad0; break; }
	case SDL_SCANCODE_KP_1: { keyCode = KeyCode_KeyPad1; break; }
	case SDL_SCANCODE_KP_2: { keyCode = KeyCode_KeyPad2; break; }
	case SDL_SCANCODE_KP_3: { keyCode = KeyCode_KeyPad3; break; }
	case SDL_SCANCODE_KP_4: { keyCode = KeyCode_KeyPad4; break; }
	case SDL_SCANCODE_KP_5: { keyCode = KeyCode_KeyPad5; break; }
	case SDL_SCANCODE_KP_6: { keyCode = KeyCode_KeyPad6; break; }
	case SDL_SCANCODE_KP_7: { keyCode = KeyCode_KeyPad7; break; }
	case SDL_SCANCODE_KP_8: { keyCode = KeyCode_KeyPad8; break; }
	case SDL_SCANCODE_KP_9: { keyCode = KeyCode_KeyPad9; break; }
	case SDL_SCANCODE_KP_PERIOD: { keyCode = KeyCode_KeyPadDot; break; }
	case SDL_SCANCODE_LCTRL: { keyCode = KeyCode_LeftCtrl; break; }
	case SDL_SCANCODE_RCTRL: { keyCode = KeyCode_RightCtrl; break; }
	case SDL_SCANCODE_LSHIFT: { keyCode = KeyCode_LeftShift; break; }
	case SDL_SCANCODE_RSHIFT: { keyCode = KeyCode_RightShift; break; }
	case SDL_SCANCODE_LALT: { keyCode = KeyCode_LeftAlt; break; }
	case SDL_SCANCODE_RALT: { keyCode = KeyCode_RightAlt; break; }
	default: break;
	}

	switch (sdlKey.sym) {
	default: break;
	}

	return keyCode;
}

IWindow::MouseButtons Window::sdlMouseButtonsStateToMoueButtons(Uint32 buttonsState)
{
	IWindow::MouseButtons buttons;
	if (buttonsState & SDL_BUTTON_LMASK) buttons.set(MouseButton_Left);
	if (buttonsState & SDL_BUTTON_MMASK) buttons.set(MouseButton_Middle);
	if (buttonsState & SDL_BUTTON_RMASK) buttons.set(MouseButton_Right);
	if (buttonsState & SDL_BUTTON_X1MASK) buttons.set(MouseButton_X1);
	if (buttonsState & SDL_BUTTON_X2MASK) buttons.set(MouseButton_X2);
	return buttons;
}

IWindow::MouseButton Window::sdlMouseButtonToMouseButton(Uint8 sdlMouseButton)
{
	MouseButton button = MouseButton_X1;
	switch (sdlMouseButton) {
	case SDL_BUTTON_LEFT: { button = MouseButton_Left; break; }
	case SDL_BUTTON_MIDDLE: { button = MouseButton_Middle; break; }
	case SDL_BUTTON_RIGHT: { button = MouseButton_Right; break; }
	case SDL_BUTTON_X1: { button = MouseButton_X1; break; }
	case SDL_BUTTON_X2: { button = MouseButton_X2; break; }
	default: break;
	}
	return button;
}

SDL_Scancode Window::keyCodeToSDLScancode(IWindow::KeyCode keyCode)
{
	switch (keyCode) {
	case KeyCode_0: return SDL_SCANCODE_0;
	case KeyCode_1: return SDL_SCANCODE_1;
	case KeyCode_2: return SDL_SCANCODE_2;
	case KeyCode_3: return SDL_SCANCODE_3;
	case KeyCode_4: return SDL_SCANCODE_4;
	case KeyCode_5: return SDL_SCANCODE_5;
	case KeyCode_6: return SDL_SCANCODE_6;
	case KeyCode_7: return SDL_SCANCODE_7;
	case KeyCode_8: return SDL_SCANCODE_8;
	case KeyCode_9: return SDL_SCANCODE_9;
	case KeyCode_A: return SDL_SCANCODE_A;
	case KeyCode_B: return SDL_SCANCODE_B;
	case KeyCode_C: return SDL_SCANCODE_C;
	case KeyCode_D: return SDL_SCANCODE_D;
	case KeyCode_E: return SDL_SCANCODE_E;
	case KeyCode_F: return SDL_SCANCODE_F;
	case KeyCode_G: return SDL_SCANCODE_G;
	case KeyCode_H: return SDL_SCANCODE_H;
	case KeyCode_I: return SDL_SCANCODE_I;
	case KeyCode_J: return SDL_SCANCODE_J;
	case KeyCode_K: return SDL_SCANCODE_K;
	case KeyCode_L: return SDL_SCANCODE_L;
	case KeyCode_M: return SDL_SCANCODE_M;
	case KeyCode_N: return SDL_SCANCODE_N;
	case KeyCode_O: return SDL_SCANCODE_O;
	case KeyCode_P: return SDL_SCANCODE_P;
	case KeyCode_Q: return SDL_SCANCODE_Q;
	case KeyCode_R: return SDL_SCANCODE_R;
	case KeyCode_S: return SDL_SCANCODE_S;
	case KeyCode_T: return SDL_SCANCODE_T;
	case KeyCode_U: return SDL_SCANCODE_U;
	case KeyCode_V: return SDL_SCANCODE_V;
	case KeyCode_W: return SDL_SCANCODE_W;
	case KeyCode_X: return SDL_SCANCODE_X;
	case KeyCode_Y: return SDL_SCANCODE_Y;
	case KeyCode_Z: return SDL_SCANCODE_Z;
	case KeyCode_F1: return SDL_SCANCODE_F1;
	case KeyCode_F2: return SDL_SCANCODE_F2;
	case KeyCode_F3: return SDL_SCANCODE_F3;
	case KeyCode_F4: return SDL_SCANCODE_F4;
	case KeyCode_F5: return SDL_SCANCODE_F5;
	case KeyCode_F6: return SDL_SCANCODE_F6;
	case KeyCode_F7: return SDL_SCANCODE_F7;
	case KeyCode_F8: return SDL_SCANCODE_F8;
	case KeyCode_F9: return SDL_SCANCODE_F9;
	case KeyCode_F10: return SDL_SCANCODE_F10;
	case KeyCode_F11: return SDL_SCANCODE_F11;
	case KeyCode_F12: return SDL_SCANCODE_F12;
	case KeyCode_Left: return SDL_SCANCODE_LEFT;
	case KeyCode_Right: return SDL_SCANCODE_RIGHT;
	case KeyCode_Up: return SDL_SCANCODE_UP;
	case KeyCode_Down: return SDL_SCANCODE_DOWN;
	case KeyCode_Return: return SDL_SCANCODE_RETURN;
	case KeyCode_Escape: return SDL_SCANCODE_ESCAPE;
	case KeyCode_Backspace: return SDL_SCANCODE_BACKSPACE;
	case KeyCode_Tab: return SDL_SCANCODE_TAB;
	case KeyCode_Spcae: return SDL_SCANCODE_SPACE;
	case KeyCode_Minus: return SDL_SCANCODE_MINUS;
	case KeyCode_Equals: return SDL_SCANCODE_EQUALS;
	case KeyCode_LeftBracket: return SDL_SCANCODE_LEFTBRACKET;
	case KeyCode_RightBracket: return SDL_SCANCODE_RIGHTBRACKET;
	case KeyCode_BackSlash: return SDL_SCANCODE_BACKSLASH;
	case KeyCode_Semicolon: return SDL_SCANCODE_SEMICOLON;
	case KeyCode_Apostrophe: return SDL_SCANCODE_APOSTROPHE;
	case KeyCode_Grave: return SDL_SCANCODE_GRAVE;
	case KeyCode_Comma: return SDL_SCANCODE_COMMA;
	case KeyCode_Dot: return SDL_SCANCODE_PERIOD;
	case KeyCode_Slash: return SDL_SCANCODE_SLASH;
	case KeyCode_CapsLock: return SDL_SCANCODE_CAPSLOCK;
	case KeyCode_PrintScreen: return SDL_SCANCODE_PRINTSCREEN;
	case KeyCode_ScrollLock: return SDL_SCANCODE_SCROLLLOCK;
	case KeyCode_Pause: return SDL_SCANCODE_PAUSE;
	case KeyCode_Insert: return SDL_SCANCODE_INSERT;
	case KeyCode_Home: return SDL_SCANCODE_HOME;
	case KeyCode_PageUp: return SDL_SCANCODE_PAGEUP;
	case KeyCode_Delete: return SDL_SCANCODE_DELETE;
	case KeyCode_End: return SDL_SCANCODE_END;
	case KeyCode_PageDown: return SDL_SCANCODE_PAGEDOWN;
	case KeyCode_NumLock: return SDL_SCANCODE_NUMLOCKCLEAR;
	case KeyCode_KeyPadDivide: return SDL_SCANCODE_KP_DIVIDE;
	case KeyCode_KeyPadMultiply: return SDL_SCANCODE_KP_MULTIPLY;
	case KeyCode_KeyPadMinus: return SDL_SCANCODE_KP_MINUS;
	case KeyCode_KeyPadPlus: return SDL_SCANCODE_KP_PLUS;
	case KeyCode_KeyPadEnter: return SDL_SCANCODE_KP_ENTER;
	case KeyCode_KeyPad0: return SDL_SCANCODE_KP_0;
	case KeyCode_KeyPad1: return SDL_SCANCODE_KP_1;
	case KeyCode_KeyPad2: return SDL_SCANCODE_KP_2;
	case KeyCode_KeyPad3: return SDL_SCANCODE_KP_3;
	case KeyCode_KeyPad4: return SDL_SCANCODE_KP_4;
	case KeyCode_KeyPad5: return SDL_SCANCODE_KP_5;
	case KeyCode_KeyPad6: return SDL_SCANCODE_KP_6;
	case KeyCode_KeyPad7: return SDL_SCANCODE_KP_7;
	case KeyCode_KeyPad8: return SDL_SCANCODE_KP_8;
	case KeyCode_KeyPad9: return SDL_SCANCODE_KP_9;
	case KeyCode_KeyPadDot: return SDL_SCANCODE_KP_PERIOD;
	case KeyCode_LeftCtrl: return SDL_SCANCODE_LCTRL;
	case KeyCode_RightCtrl: return SDL_SCANCODE_RCTRL;
	case KeyCode_LeftShift: return SDL_SCANCODE_LSHIFT;
	case KeyCode_RightShift: return SDL_SCANCODE_RSHIFT;
	case KeyCode_LeftAlt: return SDL_SCANCODE_LALT;
	case KeyCode_RightAlt: return SDL_SCANCODE_RALT;
	default: return SDL_SCANCODE_UNKNOWN;
	};
}

} // namespace