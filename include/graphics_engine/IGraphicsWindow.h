#ifndef __IGRAPHICSWINDOW__
#define __IGRAPHICSWINDOW__

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsCamera;
class IGraphicsScene;

class IGraphicsWindow : public IGraphicsResource {
public:
	virtual ~IGraphicsWindow() {}

	virtual void setGraphicsCamera(IGraphicsCamera *pGraphicsCamera) = 0;
	virtual const IGraphicsCamera *graphicsCamera() const = 0;

	virtual void setGraphicsScene(IGraphicsScene *pGraphicsScene) = 0;
	virtual const IGraphicsScene *graphicsScene() const = 0;

}; // class IGraphicsWindow

} // namespace
} // namespace



#endif // __IGRAPHICSWINDOW__
