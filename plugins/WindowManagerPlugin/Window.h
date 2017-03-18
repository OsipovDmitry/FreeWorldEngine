#ifndef __WINDOW__
#define __WINDOW__

#include <vector>
#include <3rdparty/sdl/SDL.h>
#include <IWindow.h>

namespace FreeWorldEngine {

class Window : public IWindow {
public:	
	Window(const std::string& name, const std::string& title, const int32 width, const int32 height, const bool fullscreen, const bool resizable);
	~Window();

	std::string name() const;

	void show();
	void hide();
	void move(int32 x, int32 y);
	void resize(int32 width, int32 height);
	void minimize();
	void restore();
	void maximize();

	void registerShowCallBack(ShowCallBack callback);
	void registerHideCallBack(HideCallBack callback);
	void registerMoveCallBack(MoveCallBack callback);
	void registerResizeCallBack(ResizeCallBack callback);
	void registerMinimizeCallBack(MinimizeCallBack callback);
	void registerRestoreCallBack(RestoreCallBack callback);
	void registerMaximizeCallBack(MaximizeCallBack callback);
	void registerCloseCallBack(CloseCallBack callback);
	void registerRenderCallBack(RenderCallBack callback);
	void registerUpdateCallBack(UpdateCallBack callback);
	void registerMouseDownCallBack(MouseDownCallBack callback);
	void registerMouseUpCallBack(MouseUpCallBack callback);
	void registerMouseMotionCallBack(MouseMotionCallBack callback);
	void registerMouseWheelCallBack(MouseWheelCallBack callback);
	void registerKeyDownCallBack(KeyDownCallBack callback);
	void registerKeyUpCallBack(KeyUpCallBack callback);

	void unregisterShowCallBack(ShowCallBack callback);
	void unregisterHideCallBack(HideCallBack callback);
	void unregisterMoveCallBack(MoveCallBack callback);
	void unregisterResizeCallBack(ResizeCallBack callback);
	void unregisterMinimizeCallBack(MinimizeCallBack callback);
	void unregisterRestoreCallBack(RestoreCallBack callback);
	void unregisterMaximizeCallBack(MaximizeCallBack callback);
	void unregisterCloseCallBack(CloseCallBack callback);
	void unregisterRenderCallBack(RenderCallBack callback);
	void unregisterUpdateCallBack(UpdateCallBack callback);
	void unregisterMouseDownCallBack(MouseDownCallBack callback);
	void unregisterMouseUpCallBack(MouseUpCallBack callback);
	void unregisterMouseMotionCallBack(MouseMotionCallBack callback);
	void unregisterMouseWheelCallBack(MouseWheelCallBack callback);
	void unregisterKeyDownCallBack(KeyDownCallBack callback);
	void unregisterKeyUpCallBack(KeyUpCallBack callback);

	int32 width() const;
	int32 height() const;
	void setMousePos(const int32 x, const int32 y) const;

	void setUserData(void *pData);
	void *userData() const;

	void render();
	void update(uint32 time, uint32 dt);
	void mouseButtonDown(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY);
	void mouseButtonUp(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY);
	void mouseMotion(MouseButtons buttons, int32 cursorPosX, int32 cursorPosY, int32 relCursorPosX, int32 relCursorPosY);
	void mouseWheel(int32 dirX, int32 dirY);
	void keyDown(KeyCode keyCode);
	void keyUp(KeyCode keyCode);

	void sendEvent(const SDL_WindowEvent& windowEvent);
	/*SDL_Window *window() const;
	SDL_GLContext context() const;*/
	Uint32 id() const;

	static KeyCode sdlKeysymToKeyCode(const SDL_Keysym& sdlKey);
	static MouseButtons sdlMouseButtonsStateToMoueButtons(Uint32 buttonsState);
	static MouseButton sdlMouseButtonToMouseButton(Uint8 sdlMouseButton);
	static SDL_Scancode keyCodeToSDLScancode(KeyCode keyCode);

private:
	std::string m_name;
	SDL_Window *m_window;
	SDL_GLContext m_glContext;
	void *m_pUserData;

	std::vector<ShowCallBack> m_showCallBacks;
	std::vector<HideCallBack> m_hideCallBacks;
	std::vector<MoveCallBack> m_moveCallBacks;
	std::vector<ResizeCallBack> m_resizeCallBacks;
	std::vector<MinimizeCallBack> m_minimizeCallBacks;
	std::vector<RestoreCallBack> m_restoreCallBacks;
	std::vector<MaximizeCallBack> m_maximizeCallBacks;
	std::vector<CloseCallBack> m_closeCallBacks;
	std::vector<RenderCallBack> m_renderCallBacks;
	std::vector<UpdateCallBack> m_updateCallBacks;
	std::vector<MouseDownCallBack> m_mouseDownCallBacks;
	std::vector<MouseUpCallBack> m_mouseUpCallBacks;
	std::vector<MouseMotionCallBack> m_mouseMotionCallBacks;
	std::vector<MouseWheelCallBack> m_mouseWheelCallBacks;
	std::vector<KeyDownCallBack> m_keyDownCallBacks;
	std::vector<KeyUpCallBack> m_keyUpCallBacks;

};

} // namespace

#endif // __WINDOW__