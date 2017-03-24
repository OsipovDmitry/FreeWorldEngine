#ifndef __GRAPHICSWINDOW__
#define __GRAPHICSWINDOW__

#include <graphics_engine/IGraphicsWindow.h>

namespace FreeWorldEngine {

class IWindow;

namespace GraphicsEngine {
	
class GraphicsWindow : public IGraphicsWindow {
public:
	GraphicsWindow(const std::string& name, IWindow *pTargetWindow);
	~GraphicsWindow();

	std::string name() const;

	void setCamera(IGraphicsCamera *pGraphicsCamera);
	const IGraphicsCamera *camera() const;

	void setScene(IGraphicsScene *pGraphicsScene);
	const IGraphicsScene *scene() const;

	float fps() const;
	uint64 frameNumber() const;

	void switchFrustumCulling();
	void switchRenderSpheres();

private:
	std::string m_name;
	IWindow *m_pTargetWindow;
	IGraphicsCamera *m_pCamera;
	IGraphicsScene *m_pScene;

	float m_fps;
	uint64 m_frameNumber;
	uint32 m_lastFpsTime;
	int32 m_frameCounter;

	bool m_frustumCulling, m_renderSpheres;

	static void resizeCallBack(int32 width, int32 height, IWindow *pWindow);
	static void renderCallBack(IWindow *pWindow);
	static void updateCallBack(uint32 time, uint32 dt, IWindow *pWindow);
	static void closeCallBack(IWindow *pWindow);
	static void keyDownCallBack(IWindow::KeyCode keyCode, IWindow *pWindow);

}; // class GraphicsWindow

} // namespace
} // namespace



#endif // __GRAPHICSWINDOW__
