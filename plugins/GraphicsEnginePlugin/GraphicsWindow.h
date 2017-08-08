#ifndef __GRAPHICSWINDOW__
#define __GRAPHICSWINDOW__

#include <graphics_engine/IGraphicsWindow.h>
#include <graphics_engine/IGraphicsMaterial.h>
#include <Types.h>

namespace FreeWorldEngine {

class IWindow;

namespace GraphicsEngine {

class GraphicsScene;

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
	void switchRenderNodeBoxes();

private:
	std::string m_name;
	IWindow *m_pTargetWindow;
	IGraphicsCamera *m_pCamera;
	GraphicsScene *m_pScene;

	float m_fps;
	uint64 m_frameNumber;
	uint32 m_lastFpsTime;
	int32 m_frameCounter;

	bool m_frustumCulling, m_renderSpheres, m_renderKdNodesBoxes;

	template <class Iter> void renderSolid(Iter itBegin, Iter itEnd);
	template <class Iter> void renderTransparent(Iter itBegin, Iter itEnd);
	template <class Iter> void renderHud(Iter itBegin, Iter itEnd);
	template <class Iter> void renderData(const IGraphicsMaterial::Tag tag, Iter itBegin, Iter itEnd);

	void resize(int32 width, int32 height);
	void render();
	void update(uint32 time, uint32 dt);

	static void resizeCallBack(int32 width, int32 height, IWindow *pWindow);
	static void renderCallBack(IWindow *pWindow);
	static void updateCallBack(uint32 time, uint32 dt, IWindow *pWindow);
	static void closeCallBack(IWindow *pWindow);
	static void keyDownCallBack(IWindow::KeyCode keyCode, IWindow *pWindow);

}; // class GraphicsWindow

} // namespace
} // namespace



#endif // __GRAPHICSWINDOW__
