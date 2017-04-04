#ifndef __GRAPHICSTEXTURE__
#define __GRAPHICSTEXTURE__

#include <string>

#include <graphics_engine/IGraphicsTexture.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsTexture : public IGraphicsTexture {
public:
	GraphicsTexture(const std::string& name, Renderer::IGPUTexture *pTexture);
	~GraphicsTexture();

	std::string name() const;

	Renderer::IGPUTexture *texture() const;

	void setWrapMode(const WrapMode wrapMode);
	void setFilterMode(const FilterMode filterMode);
	
private:
	std::string m_name;
	Renderer::IGPUTexture *m_pTexture;
	bool m_mipmapIsGen;

}; // class GraphicsTexture

} // namespace
} // namespace


#endif // __GRAPHICSTEXTURE__