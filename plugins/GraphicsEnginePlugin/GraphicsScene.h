#ifndef __GRAPHICSSCENE__
#define __GRAPHICSSCENE__

#include <graphics_engine/IGraphicsScene.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsScene : public IGraphicsScene {
public:
	GraphicsScene(const std::string& name);
	~GraphicsScene();

	std::string name() const;
	
	Node *rootNode() const;

private:
	std::string m_name;
	Node *m_pRootNode;

}; // class GraphicsScene

} // namespace

} // namespace

#endif // __GRAPHICSSCENE__