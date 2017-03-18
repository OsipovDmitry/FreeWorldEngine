#ifndef __WINDOW__
#define __WINDOW__

#include <QtOpenGL/QGLWidget>

#include <IWindow.h>

namespace FreeWorldEngine {

class Window : public QGLWidget, public IWindow {
	Q_OBJECT

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

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void closeEvent(QCloseEvent *event);
	void moveEvent(QMoveEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

public:
	static KeyCode qtKeysymToKeyCode(const Qt::Key key);
	static MouseButtons qtMouseButtonsStateToMoueButtons(Qt::MouseButtons buttonsState);
	static MouseButton qtMouseButtonToMouseButton(Qt::MouseButton qtMouseButton);
	static Qt::Key keyCodeToSDLScancode(KeyCode keyCode);

private:
	std::string m_name;
	uint64 m_lastUpdateTime;
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