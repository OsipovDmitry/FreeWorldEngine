#ifndef __GRAPHICSMATERIAL__
#define __GRAPHICSMATERIAL__

#include <graphics_engine/IGraphicsMaterial.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsMaterial : public IGraphicsMaterial {
public:
	GraphicsMaterial(const std::string& name);
	~GraphicsMaterial();

	std::string name() const;

private:
	std::string m_name;

}; // class GraphicsMaterial

} // namespace

} // namespace

#endif // __GRAPHICSMATERIAL__