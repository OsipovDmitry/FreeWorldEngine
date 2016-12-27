#ifndef __GRAPHICSMODEL__
#define __GRAPHICSMODEL__

#include <graphics_engine/IGraphicsModel.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsModel : public IGraphicsModel {
public:
	GraphicsModel(const std::string& name);
	~GraphicsModel();

	std::string name() const;

	IGraphicsMaterial *material() const;
	void setMaterial(IGraphicsMaterial *pMaterial);

private:
	std::string m_name;
	IGraphicsMaterial *m_pMaterial;

}; // class GraphicsModel

} // namespace

} // namespace

#endif // __GRAPHICSMODEL__