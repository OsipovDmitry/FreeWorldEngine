#include <QtGui/qevent.h>
#include <qtimer.h>
#include <qdatetime.h>

#include "Window.h"

namespace FreeWorldEngine {

Window::Window(const std::string& name, const std::string& title, const int32 width, const int32 height, const bool fullscreen, const bool resizable) :
	QGLWidget(),
	m_name(name),
	m_frameNumber(-1),
	m_lastUpdateTime(QDateTime::currentMSecsSinceEpoch()),
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
	setWindowTitle(QString::fromStdString(title));
	QGLWidget::resize(width, height);

	QTimer *pTimer = new QTimer(this);
	connect(pTimer, SIGNAL(timeout()), this, SLOT(update()));
	pTimer->start(0);
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
	QGLWidget::move(x, y);
}

void Window::resize(int32 width, int32 height)
{
	QGLWidget::resize(width, height); // ֲûחמûוע resizeGL()
}

void Window::show()
{
	QGLWidget::show();
}

void Window::hide()
{
	QGLWidget::hide();
}

void Window::minimize()
{
	QGLWidget::showMinimized();
	std::for_each(m_minimizeCallBacks.cbegin(), m_minimizeCallBacks.cend(), [this](const MinimizeCallBack p) { p(this); });
}

void Window::restore()
{
	QGLWidget::showNormal();
	std::for_each(m_restoreCallBacks.cbegin(), m_restoreCallBacks.cend(), [this](const RestoreCallBack p) { p(this); });
}

void Window::maximize()
{
	QGLWidget::showMaximized();
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
	return QGLWidget::width();
}

int32 Window::height() const
{
	return QGLWidget::height();
}

uint64 Window::frameNumber() const
{
	return m_frameNumber;
}

void Window::setMousePos(const int32 x, const int32 y) const
{
	QCursor::setPos(QGLWidget::mapToGlobal(QPoint(x, y)));
}

void Window::setUserData(void * pData)
{
	m_pUserData = pData;
}

void *Window::userData() const
{
	return m_pUserData;
}

void Window::initializeGL()
{
}

void Window::paintGL()
{
	uint64 time = QDateTime::currentMSecsSinceEpoch();
	uint64 dt = time - m_lastUpdateTime;
	m_lastUpdateTime = time;
	std::for_each(m_updateCallBacks.cbegin(), m_updateCallBacks.cend(), [time, dt, this](UpdateCallBack p) { p(time, dt, this); });

	m_frameNumber++;
	std::for_each(m_renderCallBacks.cbegin(), m_renderCallBacks.cend(), [this](RenderCallBack p) { p(this); });
}

void Window::resizeGL(int width, int height)
{
	std::for_each(m_resizeCallBacks.cbegin(), m_resizeCallBacks.cend(), [width, height, this](const ResizeCallBack p) { p(width, height, this); });
}

void Window::showEvent(QShowEvent * event)
{
	std::for_each(m_showCallBacks.cbegin(), m_showCallBacks.cend(), [this](const ShowCallBack p) { p(this); });
}

void Window::hideEvent(QHideEvent * event)
{
	std::for_each(m_hideCallBacks.cbegin(), m_hideCallBacks.cend(), [this](const HideCallBack p) { p(this); });
}

void Window::moveEvent(QMoveEvent * event)
{
	std::for_each(m_moveCallBacks.cbegin(), m_moveCallBacks.cend(), [event, this](const MoveCallBack p) { p(event->pos().x(), event->pos().y(), this); });
}

void Window::mousePressEvent(QMouseEvent *event)
{
	MouseButton button = qtMouseButtonToMouseButton(event->button());
	int32 x = event->pos().x(), y = event->pos().y();
	std::for_each(m_mouseDownCallBacks.cbegin(), m_mouseDownCallBacks.cend(), [button, x, y, this](const MouseDownCallBack p) { p(button, 1, x, y, this); });
}

void Window::mouseDoubleClickEvent(QMouseEvent * event)
{
	MouseButton button = qtMouseButtonToMouseButton(event->button());
	int32 x = event->pos().x(), y = event->pos().y();
	std::for_each(m_mouseDownCallBacks.cbegin(), m_mouseDownCallBacks.cend(), [button, x, y, this](const MouseDownCallBack p) { p(button, 2, x, y, this); });
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
	MouseButton button = qtMouseButtonToMouseButton(event->button());
	int32 x = event->pos().x(), y = event->pos().y();
	std::for_each(m_mouseUpCallBacks.cbegin(), m_mouseUpCallBacks.cend(), [button, x, y, this](const MouseUpCallBack p) { p(button, 1, x, y, this); });
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
	MouseButtons buttons = qtMouseButtonsStateToMoueButtons(event->buttons());
	int32 x = event->pos().x(), y = event->pos().y();
	std::for_each(m_mouseMotionCallBacks.cbegin(), m_mouseMotionCallBacks.cend(), [buttons, x, y, this](const MouseMotionCallBack p) { p(buttons, x, y, 0, 0, this); });
}

void Window::wheelEvent(QWheelEvent *event)
{
	int32 dir = event->delta();
	std::for_each(m_mouseWheelCallBacks.cbegin(), m_mouseWheelCallBacks.cend(), [dir, this](MouseWheelCallBack p) { p(dir, 0, this); });
}

void Window::keyPressEvent(QKeyEvent * event)
{
	KeyCode keyCode = qtKeysymToKeyCode((Qt::Key)event->key());
	std::for_each(m_keyDownCallBacks.cbegin(), m_keyDownCallBacks.cend(), [keyCode, this](KeyDownCallBack p) { p(keyCode, this); });
}

void Window::keyReleaseEvent(QKeyEvent * event)
{
	KeyCode keyCode = qtKeysymToKeyCode((Qt::Key)event->key());
	std::for_each(m_keyUpCallBacks.cbegin(), m_keyUpCallBacks.cend(), [keyCode, this](KeyUpCallBack p) { p(keyCode, this); });
}

void Window::closeEvent(QCloseEvent * event)
{
	std::for_each(m_closeCallBacks.cbegin(), m_closeCallBacks.cend(), [this](const CloseCallBack p) { p(this); });
}

IWindow::KeyCode Window::qtKeysymToKeyCode(const Qt::Key key)
{
	IWindow::KeyCode keyCode = KeyCode_None;
	switch (key) {
	case Qt::Key_unknown: { keyCode = KeyCode_0; break; }
	case Qt::Key_0: { keyCode = KeyCode_0; break; }
	case Qt::Key_1: { keyCode = KeyCode_1; break; }
	case Qt::Key_2: { keyCode = KeyCode_2; break; }
	case Qt::Key_3: { keyCode = KeyCode_3; break; }
	case Qt::Key_4: { keyCode = KeyCode_4; break; }
	case Qt::Key_5: { keyCode = KeyCode_5; break; }
	case Qt::Key_6: { keyCode = KeyCode_6; break; }
	case Qt::Key_7: { keyCode = KeyCode_7; break; }
	case Qt::Key_8: { keyCode = KeyCode_8; break; }
	case Qt::Key_9: { keyCode = KeyCode_9; break; }
	case Qt::Key_A: { keyCode = KeyCode_A; break; }
	case Qt::Key_B: { keyCode = KeyCode_B; break; }
	case Qt::Key_C: { keyCode = KeyCode_C; break; }
	case Qt::Key_D: { keyCode = KeyCode_D; break; }
	case Qt::Key_E: { keyCode = KeyCode_E; break; }
	case Qt::Key_F: { keyCode = KeyCode_F; break; }
	case Qt::Key_G: { keyCode = KeyCode_G; break; }
	case Qt::Key_H: { keyCode = KeyCode_H; break; }
	case Qt::Key_I: { keyCode = KeyCode_I; break; }
	case Qt::Key_J: { keyCode = KeyCode_J; break; }
	case Qt::Key_K: { keyCode = KeyCode_K; break; }
	case Qt::Key_L: { keyCode = KeyCode_L; break; }
	case Qt::Key_M: { keyCode = KeyCode_M; break; }
	case Qt::Key_N: { keyCode = KeyCode_N; break; }
	case Qt::Key_O: { keyCode = KeyCode_O; break; }
	case Qt::Key_P: { keyCode = KeyCode_P; break; }
	case Qt::Key_Q: { keyCode = KeyCode_Q; break; }
	case Qt::Key_R: { keyCode = KeyCode_R; break; }
	case Qt::Key_S: { keyCode = KeyCode_S; break; }
	case Qt::Key_T: { keyCode = KeyCode_T; break; }
	case Qt::Key_U: { keyCode = KeyCode_U; break; }
	case Qt::Key_V: { keyCode = KeyCode_V; break; }
	case Qt::Key_W: { keyCode = KeyCode_W; break; }
	case Qt::Key_X: { keyCode = KeyCode_X; break; }
	case Qt::Key_Y: { keyCode = KeyCode_Y; break; }
	case Qt::Key_Z: { keyCode = KeyCode_Z; break; }
	case Qt::Key_F1: { keyCode = KeyCode_F1; break; }
	case Qt::Key_F2: { keyCode = KeyCode_F2; break; }
	case Qt::Key_F3: { keyCode = KeyCode_F3; break; }
	case Qt::Key_F4: { keyCode = KeyCode_F4; break; }
	case Qt::Key_F5: { keyCode = KeyCode_F5; break; }
	case Qt::Key_F6: { keyCode = KeyCode_F6; break; }
	case Qt::Key_F7: { keyCode = KeyCode_F7; break; }
	case Qt::Key_F8: { keyCode = KeyCode_F8; break; }
	case Qt::Key_F9: { keyCode = KeyCode_F9; break; }
	case Qt::Key_F10: { keyCode = KeyCode_F10; break; }
	case Qt::Key_F11: { keyCode = KeyCode_F11; break; }
	case Qt::Key_F12: { keyCode = KeyCode_F12; break; }
	case Qt::Key_Left: { keyCode = KeyCode_Left; break; }
	case Qt::Key_Right: { keyCode = KeyCode_Right; break; }
	case Qt::Key_Up: { keyCode = KeyCode_Up; break; }
	case Qt::Key_Down: { keyCode = KeyCode_Down; break; }
	case Qt::Key_Return: { keyCode = KeyCode_Return; break; }
	case Qt::Key_Escape: { keyCode = KeyCode_Escape; break; }
	case Qt::Key_Backspace: { keyCode = KeyCode_Backspace; break; }
	case Qt::Key_Tab: { keyCode = KeyCode_Tab; break; }
	case Qt::Key_Space: { keyCode = KeyCode_Spcae; break; }
	case Qt::Key_Minus: { keyCode = KeyCode_Minus; break; }
	case Qt::Key_Equal: { keyCode = KeyCode_Equals; break; }
	case Qt::Key_BracketLeft: { keyCode = KeyCode_LeftBracket; break; }
	case Qt::Key_BracketRight: { keyCode = KeyCode_RightBracket; break; }
	case Qt::Key_Backslash: { keyCode = KeyCode_BackSlash; break; }
	case Qt::Key_Semicolon: { keyCode = KeyCode_Semicolon; break; }
	case Qt::Key_Apostrophe: { keyCode = KeyCode_Apostrophe; break; }
	case Qt::Key_QuoteLeft: { keyCode = KeyCode_Grave; break; }
	case Qt::Key_Comma: { keyCode = KeyCode_Comma; break; }
	case Qt::Key_Period: { keyCode = KeyCode_Dot; break; }
	case Qt::Key_Slash: { keyCode = KeyCode_Slash; break; }
	case Qt::Key_CapsLock: { keyCode = KeyCode_CapsLock; break; }
	//case SDL_SCANCODE_PRINTSCREEN: { keyCode = KeyCode_PrintScreen; break; }
	case Qt::Key_ScrollLock: { keyCode = KeyCode_ScrollLock; break; }
	case Qt::Key_Pause: { keyCode = KeyCode_Pause; break; }
	case Qt::Key_Insert: { keyCode = KeyCode_Insert; break; }
	case Qt::Key_Home: { keyCode = KeyCode_Home; break; }
	case Qt::Key_PageUp: { keyCode = KeyCode_PageUp; break; }
	case Qt::Key_Delete: { keyCode = KeyCode_Delete; break; }
	case Qt::Key_End: { keyCode = KeyCode_End; break; }
	case Qt::Key_PageDown: { keyCode = KeyCode_PageDown; break; }
	case Qt::Key_NumLock: { keyCode = KeyCode_NumLock; break; }
	//case SDL_SCANCODE_KP_DIVIDE: { keyCode = KeyCode_KeyPadDivide; break; }
	//case SDL_SCANCODE_KP_MULTIPLY: { keyCode = KeyCode_KeyPadMultiply; break; }
	//case SDL_SCANCODE_KP_MINUS: { keyCode = KeyCode_KeyPadMinus; break; }
	//case SDL_SCANCODE_KP_PLUS: { keyCode = KeyCode_KeyPadPlus; break; }
	//case SDL_SCANCODE_KP_ENTER: { keyCode = KeyCode_KeyPadEnter; break; }
	//case SDL_SCANCODE_KP_0: { keyCode = KeyCode_KeyPad0; break; }
	//case SDL_SCANCODE_KP_1: { keyCode = KeyCode_KeyPad1; break; }
	//case SDL_SCANCODE_KP_2: { keyCode = KeyCode_KeyPad2; break; }
	//case SDL_SCANCODE_KP_3: { keyCode = KeyCode_KeyPad3; break; }
	//case SDL_SCANCODE_KP_4: { keyCode = KeyCode_KeyPad4; break; }
	//case SDL_SCANCODE_KP_5: { keyCode = KeyCode_KeyPad5; break; }
	//case SDL_SCANCODE_KP_6: { keyCode = KeyCode_KeyPad6; break; }
	//case SDL_SCANCODE_KP_7: { keyCode = KeyCode_KeyPad7; break; }
	//case SDL_SCANCODE_KP_8: { keyCode = KeyCode_KeyPad8; break; }
	//case SDL_SCANCODE_KP_9: { keyCode = KeyCode_KeyPad9; break; }
	//case SDL_SCANCODE_KP_PERIOD: { keyCode = KeyCode_KeyPadDot; break; }
	case Qt::Key_Control: { keyCode = KeyCode_LeftCtrl; break; }
	//case SDL_SCANCODE_RCTRL: { keyCode = KeyCode_RightCtrl; break; }
	case Qt::Key_Shift: { keyCode = KeyCode_LeftShift; break; }
	//case SDL_SCANCODE_RSHIFT: { keyCode = KeyCode_RightShift; break; }
	case Qt::Key_Alt: { keyCode = KeyCode_LeftAlt; break; }
	//case SDL_SCANCODE_RALT: { keyCode = KeyCode_RightAlt; break; }
	default: break;
	}

	return keyCode;
}

IWindow::MouseButtons Window::qtMouseButtonsStateToMoueButtons(Qt::MouseButtons buttonsState)
{
	IWindow::MouseButtons buttons;
	if (buttonsState.testFlag(Qt::LeftButton)) buttons.set(MouseButton_Left);
	if (buttonsState.testFlag(Qt::MidButton)) buttons.set(MouseButton_Middle);
	if (buttonsState.testFlag(Qt::RightButton)) buttons.set(MouseButton_Right);
	if (buttonsState.testFlag(Qt::XButton1)) buttons.set(MouseButton_X1);
	if (buttonsState.testFlag(Qt::XButton2)) buttons.set(MouseButton_X2);
	return buttons;
}

IWindow::MouseButton Window::qtMouseButtonToMouseButton(Qt::MouseButton qtMouseButton)
{
	MouseButton button = MouseButton_X1;
	switch (qtMouseButton) {
	case Qt::LeftButton: { button = MouseButton_Left; break; }
	case Qt::MidButton: { button = MouseButton_Middle; break; }
	case Qt::RightButton: { button = MouseButton_Right; break; }
	case Qt::XButton1: { button = MouseButton_X1; break; }
	case Qt::XButton2: { button = MouseButton_X2; break; }
	default: break;
	}
	return button;
}

Qt::Key Window::keyCodeToSDLScancode(IWindow::KeyCode keyCode)
{
	switch (keyCode) {
	case KeyCode_0: return Qt::Key_0;
	case KeyCode_1: return Qt::Key_1;
	case KeyCode_2: return Qt::Key_2;
	case KeyCode_3: return Qt::Key_3;
	case KeyCode_4: return Qt::Key_4;
	case KeyCode_5: return Qt::Key_5;
	case KeyCode_6: return Qt::Key_6;
	case KeyCode_7: return Qt::Key_7;
	case KeyCode_8: return Qt::Key_8;
	case KeyCode_9: return Qt::Key_9;
	case KeyCode_A: return Qt::Key_A;
	case KeyCode_B: return Qt::Key_B;
	case KeyCode_C: return Qt::Key_C;
	case KeyCode_D: return Qt::Key_D;
	case KeyCode_E: return Qt::Key_E;
	case KeyCode_F: return Qt::Key_F;
	case KeyCode_G: return Qt::Key_G;
	case KeyCode_H: return Qt::Key_H;
	case KeyCode_I: return Qt::Key_I;
	case KeyCode_J: return Qt::Key_J;
	case KeyCode_K: return Qt::Key_K;
	case KeyCode_L: return Qt::Key_L;
	case KeyCode_M: return Qt::Key_M;
	case KeyCode_N: return Qt::Key_N;
	case KeyCode_O: return Qt::Key_O;
	case KeyCode_P: return Qt::Key_P;
	case KeyCode_Q: return Qt::Key_Q;
	case KeyCode_R: return Qt::Key_R;
	case KeyCode_S: return Qt::Key_S;
	case KeyCode_T: return Qt::Key_T;
	case KeyCode_U: return Qt::Key_U;
	case KeyCode_V: return Qt::Key_V;
	case KeyCode_W: return Qt::Key_W;
	case KeyCode_X: return Qt::Key_X;
	case KeyCode_Y: return Qt::Key_Y;
	case KeyCode_Z: return Qt::Key_Z;
	case KeyCode_F1: return Qt::Key_F1;
	case KeyCode_F2: return Qt::Key_F2;
	case KeyCode_F3: return Qt::Key_F3;
	case KeyCode_F4: return Qt::Key_F4;
	case KeyCode_F5: return Qt::Key_F5;
	case KeyCode_F6: return Qt::Key_F6;
	case KeyCode_F7: return Qt::Key_F7;
	case KeyCode_F8: return Qt::Key_F8;
	case KeyCode_F9: return Qt::Key_F9;
	case KeyCode_F10: return Qt::Key_F10;
	case KeyCode_F11: return Qt::Key_F11;
	case KeyCode_F12: return Qt::Key_F12;
	case KeyCode_Left: return Qt::Key_Left;
	case KeyCode_Right: return Qt::Key_Right;
	case KeyCode_Up: return Qt::Key_Up;
	case KeyCode_Down: return Qt::Key_Down;
	case KeyCode_Return: return Qt::Key_Return;
	case KeyCode_Escape: return Qt::Key_Escape;
	case KeyCode_Backspace: return Qt::Key_Backspace;
	case KeyCode_Tab: return Qt::Key_Tab;
	case KeyCode_Spcae: return Qt::Key_Space;
	case KeyCode_Minus: return Qt::Key_Minus;
	case KeyCode_Equals: return Qt::Key_Equal;
	case KeyCode_LeftBracket: return Qt::Key_BracketLeft;
	case KeyCode_RightBracket: return Qt::Key_BracketRight;
	case KeyCode_BackSlash: return Qt::Key_Backslash;
	case KeyCode_Semicolon: return Qt::Key_Semicolon;
	case KeyCode_Apostrophe: return Qt::Key_Apostrophe;
	case KeyCode_Grave: return Qt::Key_QuoteLeft;
	case KeyCode_Comma: return Qt::Key_Comma;
	case KeyCode_Dot: return Qt::Key_Period;
	case KeyCode_Slash: return Qt::Key_Slash;
	case KeyCode_CapsLock: return Qt::Key_CapsLock;
	//case KeyCode_PrintScreen: return Qt::Key_Print;
	case KeyCode_ScrollLock: return Qt::Key_ScrollLock;
	case KeyCode_Pause: return Qt::Key_Pause;
	case KeyCode_Insert: return Qt::Key_Insert;
	case KeyCode_Home: return Qt::Key_Home;
	case KeyCode_PageUp: return Qt::Key_PageUp;
	case KeyCode_Delete: return Qt::Key_Delete;
	case KeyCode_End: return Qt::Key_End;
	case KeyCode_PageDown: return Qt::Key_PageDown;
	case KeyCode_NumLock: return Qt::Key_NumLock;
	/*case KeyCode_KeyPadDivide: return Qt::Key_KP_DIVIDE;
	case KeyCode_KeyPadMultiply: return Qt::Key_KP_MULTIPLY;
	case KeyCode_KeyPadMinus: return Qt::Key_KP_MINUS;
	case KeyCode_KeyPadPlus: return Qt::Key_KP_PLUS;
	case KeyCode_KeyPadEnter: return Qt::Key_KP_ENTER;
	case KeyCode_KeyPad0: return Qt::Key_KP_0;
	case KeyCode_KeyPad1: return Qt::Key_KP_1;
	case KeyCode_KeyPad2: return Qt::Key_KP_2;
	case KeyCode_KeyPad3: return Qt::Key_KP_3;
	case KeyCode_KeyPad4: return Qt::Key_KP_4;
	case KeyCode_KeyPad5: return Qt::Key_KP_5;
	case KeyCode_KeyPad6: return Qt::Key_KP_6;
	case KeyCode_KeyPad7: return Qt::Key_KP_7;
	case KeyCode_KeyPad8: return Qt::Key_KP_8;
	case KeyCode_KeyPad9: return Qt::Key_KP_9;
	case KeyCode_KeyPadDot: return Qt::Key_KP_PERIOD;*/
	case KeyCode_LeftCtrl: return Qt::Key_Control;
	//case KeyCode_RightCtrl: return Qt::Key_RCTRL;
	case KeyCode_LeftShift: return Qt::Key_Shift;
	//case KeyCode_RightShift: return Qt::Key_RSHIFT;
	case KeyCode_LeftAlt: return Qt::Key_Alt;
	//case KeyCode_RightAlt: return Qt::Key_RALT;
	default: return Qt::Key_unknown;
	};
}

} // namespace