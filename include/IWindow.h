#ifndef __IWINDOW__
#define __IWINDOW__

#include <bitset>

#include <IResource.h>
#include <Types.h>

namespace FreeWorldEngine {

class IWindow : public IResource {
public:
	enum MouseButton {
		MouseButton_Left,
		MouseButton_Middle,
		MouseButton_Right,
		MouseButton_X1,
		MouseButton_X2,
		MouseButton_Count
	};
	typedef std::bitset<MouseButton_Count> MouseButtons;

	enum KeyCode {
		KeyCode_None = 0,

		KeyCode_0, KeyCode_1, KeyCode_2, KeyCode_3, KeyCode_4,
		KeyCode_5, KeyCode_6, KeyCode_7, KeyCode_8, KeyCode_9,

		KeyCode_A, KeyCode_B, KeyCode_C, KeyCode_D, KeyCode_E,
		KeyCode_F, KeyCode_G, KeyCode_H, KeyCode_I, KeyCode_J,
		KeyCode_K, KeyCode_L, KeyCode_M, KeyCode_N, KeyCode_O,
		KeyCode_P, KeyCode_Q, KeyCode_R, KeyCode_S, KeyCode_T,
		KeyCode_U, KeyCode_V, KeyCode_W, KeyCode_X, KeyCode_Y,
		KeyCode_Z,

		KeyCode_F1, KeyCode_F2, KeyCode_F3, KeyCode_F4, KeyCode_F5,
		KeyCode_F6, KeyCode_F7, KeyCode_F8, KeyCode_F9, KeyCode_F10,
		KeyCode_F11, KeyCode_F12,

		KeyCode_Left, KeyCode_Right, KeyCode_Up, KeyCode_Down,

		KeyCode_Return, // Enter
		KeyCode_Escape, // Esc
		KeyCode_Backspace, // <-
		KeyCode_Tab, // Tab
		KeyCode_Spcae, // Space
		KeyCode_Minus, // -
		KeyCode_Equals, // =
		KeyCode_LeftBracket, // [
		KeyCode_RightBracket, // ]
		KeyCode_BackSlash, // "\"
		KeyCode_Semicolon, // ;
		KeyCode_Apostrophe, // '
		KeyCode_Grave, // `
		KeyCode_Comma, // ,
		KeyCode_Dot, // .
		KeyCode_Slash, // /
		KeyCode_CapsLock, // Caps Lock
		KeyCode_PrintScreen, // PrtSc
		KeyCode_ScrollLock, // ScrLk
		KeyCode_Pause, // PauseSDL_SCANCODE_PAUSE = 72,
		KeyCode_Insert, // Ins
		KeyCode_Home, // Home
		KeyCode_PageUp, // Pg Up
		KeyCode_Delete, // Del
		KeyCode_End, // End
		KeyCode_PageDown, // Pg Down
		KeyCode_NumLock, // Num Lock
		KeyCode_KeyPadDivide,
		KeyCode_KeyPadMultiply,
		KeyCode_KeyPadMinus,
		KeyCode_KeyPadPlus,
		KeyCode_KeyPadEnter,
		KeyCode_KeyPad0,
		KeyCode_KeyPad1,
		KeyCode_KeyPad2,
		KeyCode_KeyPad3,
		KeyCode_KeyPad4,
		KeyCode_KeyPad5,
		KeyCode_KeyPad6,
		KeyCode_KeyPad7,
		KeyCode_KeyPad8,
		KeyCode_KeyPad9,
		KeyCode_KeyPadDot,
		KeyCode_LeftCtrl,
		KeyCode_RightCtrl,
		KeyCode_LeftShift,
		KeyCode_RightShift,
		KeyCode_LeftAlt,
		KeyCode_RightAlt,
	};

	typedef void(*ShowCallBack)(IWindow*);
	typedef void(*HideCallBack)(IWindow*);
	typedef void(*MoveCallBack)(int32 x, int32 y, IWindow*);
	typedef void(*ResizeCallBack)(int32 w, int32 h, IWindow*);
	typedef void(*MinimizeCallBack)(IWindow*);
	typedef void(*RestoreCallBack)(IWindow*);
	typedef void(*MaximizeCallBack)(IWindow*);
	typedef void(*CloseCallBack)(IWindow*);
	typedef void(*RenderCallBack)(IWindow*);
	typedef void(*UpdateCallBack)(uint32 time, uint32 dt, IWindow*);
	typedef void(*MouseDownCallBack)(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY, IWindow*);
	typedef void(*MouseUpCallBack)(MouseButton button, uint32 numClicks, int32 cursorPosX, int32 cursorPosY, IWindow*);
	typedef void(*MouseMotionCallBack)(MouseButtons buttons, int32 cursorPosX, int32 cursorPosY, int32 relCursorPosX, int32 relCursorPosY, IWindow*);
	typedef void(*MouseWheelCallBack)(int32 dirX, int32 dirY, IWindow*);
	typedef void(*KeyDownCallBack)(KeyCode keyCode, IWindow*);
	typedef void(*KeyUpCallBack)(KeyCode keyCode, IWindow*);

	virtual ~IWindow() {}

	virtual void show() = 0;
	virtual void hide() = 0;
	virtual void move(int32 x, int32 y) = 0;
	virtual void resize(int32 width, int32 height) = 0;
	virtual void minimize() = 0;
	virtual void restore() = 0;
	virtual void maximize() = 0;

	virtual void registerShowCallBack(ShowCallBack callback) = 0;
	virtual void registerHideCallBack(HideCallBack callback) = 0;
	virtual void registerMoveCallBack(MoveCallBack callback) = 0;
	virtual void registerResizeCallBack(ResizeCallBack callback) = 0;
	virtual void registerMinimizeCallBack(MinimizeCallBack callback) = 0;
	virtual void registerRestoreCallBack(RestoreCallBack callback) = 0;
	virtual void registerMaximizeCallBack(MaximizeCallBack callback) = 0;
	virtual void registerCloseCallBack(CloseCallBack callback) = 0;
	virtual void registerRenderCallBack(RenderCallBack callback) = 0;
	virtual void registerUpdateCallBack(UpdateCallBack callback) = 0;
	virtual void registerMouseDownCallBack(MouseDownCallBack callback) = 0;
	virtual void registerMouseUpCallBack(MouseUpCallBack callback) = 0;
	virtual void registerMouseMotionCallBack(MouseMotionCallBack callback) = 0;
	virtual void registerMouseWheelCallBack(MouseWheelCallBack callback) = 0;
	virtual void registerKeyDownCallBack(KeyDownCallBack callback) = 0;
	virtual void registerKeyUpCallBack(KeyUpCallBack callback) = 0;

	virtual void unregisterShowCallBack(ShowCallBack callback) = 0;
	virtual void unregisterHideCallBack(HideCallBack callback) = 0;
	virtual void unregisterMoveCallBack(MoveCallBack callback) = 0;
	virtual void unregisterResizeCallBack(ResizeCallBack callback) = 0;
	virtual void unregisterMinimizeCallBack(MinimizeCallBack callback) = 0;
	virtual void unregisterRestoreCallBack(RestoreCallBack callback) = 0;
	virtual void unregisterMaximizeCallBack(MaximizeCallBack callback) = 0;
	virtual void unregisterCloseCallBack(CloseCallBack callback) = 0;
	virtual void unregisterRenderCallBack(RenderCallBack callback) = 0;
	virtual void unregisterUpdateCallBack(UpdateCallBack callback) = 0;
	virtual void unregisterMouseDownCallBack(MouseDownCallBack callback) = 0;
	virtual void unregisterMouseUpCallBack(MouseUpCallBack callback) = 0;
	virtual void unregisterMouseMotionCallBack(MouseMotionCallBack callback) = 0;
	virtual void unregisterMouseWheelCallBack(MouseWheelCallBack callback) = 0;
	virtual void unregisterKeyDownCallBack(KeyDownCallBack callback) = 0;
	virtual void unregisterKeyUpCallBack(KeyUpCallBack callback) = 0;

	virtual int32 width() const = 0;
	virtual int32 height() const = 0;
	virtual void setMousePos(const int32 x, const int32 y) const = 0;

	virtual void setUserData(void *pData) = 0;
	virtual void *userData() const = 0;
};

} // namespace

#endif // __IWINDOW__