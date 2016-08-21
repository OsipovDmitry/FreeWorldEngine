#ifndef __IWINDOWMANAGER__
#define __IWINDOWMANAGER__

#include <string>

#include "Types.h"
#include "IWindow.h"

namespace FreeWorldEngine {

class IWindowManager {
public:
	enum MouseButton {
		MouseButton_Left   = 0x00000001,
		MouseButton_Middle = 0x00000002,
		MouseButton_Right  = 0x00000004,
		MouseButton_X1     = 0x00000008,
		MouseButton_X2     = 0x00000010
	};

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

	enum KeyModifier {
		KeyModifier_LeftShift  = 0x00000001,
		KeyModifier_RightShift = 0x00000002,
		KeyModifier_LeftCtrl   = 0x00000010,
		KeyModifier_RightCtrl  = 0x00000020,
		KeyModifier_LeftAlt    = 0x00000100,
		KeyModifier_RightAlt   = 0x00000200
	};

	virtual ~IWindowManager() = 0 {}
	virtual IWindow *createWindow(const std::string& title, const int32 width, const int32 height, const uint32 flags = IWindow::IWindow_Show) = 0;
	virtual void destroyWindow(IWindow *pWindow) = 0;
	virtual IWindow *findWindow(const uint32 id) const = 0;
	virtual void mainLoop() = 0;

	virtual void setFuncMouseButtonDown(void (*)(uint32, MouseButton, uint32, int32, int32)) = 0;
	virtual void setFuncMouseButtonUp(void (*)(uint32, MouseButton, uint32, int32, int32)) = 0;
	virtual void setFuncMouseMotion(void (*)(uint32, uint32, int32, int32, int32, int32)) = 0;
	virtual void setFuncMouseWheel(void (*)(uint32, int32, int32)) = 0;
	virtual void setMousePos(const int32 x, const int32 y, const IWindow *pWindow = 0) const = 0;
	virtual void mousePos(int32& x, int32& y) const = 0;
	virtual uint32 mouseButtonsState() const = 0;

	virtual void setFuncKeyDown(void (*)(uint32, KeyCode, uint32)) = 0;
	virtual void setFuncKeyUp(void (*)(uint32, KeyCode, uint32)) = 0;
	virtual bool keyState(const KeyCode keyCode) const = 0;
	virtual uint32 modifiers() const = 0;

}; // class IWindowManager


} // namespace

#endif // __IWINDOWMANAGER__