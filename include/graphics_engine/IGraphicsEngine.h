#ifndef __IGRAPHICSENGINE__
#define __IGRAPHICSENGINE__

#include <string>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsModel;
class IGraphicsMaterial;
class IGraphicsScene;

class IGraphicsEngine {
public:
	virtual ~IGraphicsEngine() {}

	virtual IGraphicsModel *findModel(const std::string& name) const = 0;
	virtual IGraphicsModel *createModel(const std::string& name) = 0;
	virtual void destroyModel(const std::string& name) = 0;

	virtual IGraphicsMaterial *findMaterial(const std::string& name) const = 0;
	virtual IGraphicsMaterial *createMaterial(const std::string& name) = 0;
	virtual void destroyMaterial(const std::string& name) = 0;

	virtual IGraphicsScene *findScene(const std::string& name) const = 0;
	virtual IGraphicsScene *createScene(const std::string& name) = 0;
	virtual void destroyScene(const std::string& name) = 0;

}; // class IGraphicsEngine

} // namespace

} // namespace

#endif // __IGRAPHICSENGINE__