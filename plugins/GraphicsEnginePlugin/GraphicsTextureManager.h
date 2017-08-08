#ifndef __GRAPHICSTEXTUREMANAGER__
#define __GRAPHICSTEXTUREMANAGER__

#include <graphics_engine/IGraphicsTextureManager.h>
#include <Types.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
}

namespace Renderer {
	struct TextureFormat;
}

namespace GraphicsEngine {

class GraphicsTextureManager : public IGraphicsTextureManager  {
public:
	GraphicsTextureManager();
	~GraphicsTextureManager();
	
	IGraphicsTexture *createTexture(Renderer::IGPUTexture *pTexture, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTexture1D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTexture2D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTexture3D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTextureCubeMap(const Raster *pRasters[], const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTexture1DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTexture2DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTextureRectangle(const Raster *pRaster, const std::string& name = "@utoname", std::string *pLogString = nullptr);
	IGraphicsTexture *createTextureBuffer(Renderer::IGPUBuffer *pBuffer, const std::string& name = "@utoname", std::string *pLogString = nullptr);

	IGraphicsTexture *findTexture(const std::string& name) const;

	void destroyTexture(const std::string& name);
	void destroyTexture(IGraphicsTexture *pTexture);

private:
	IResourceManager *m_pResoureManager;
	Utility::AutoNameGenerator *m_pTextureNameGenerator;

	static bool internalTextureFormat(const uint32 channelsCount, const Type type, Renderer::TextureFormat& result);

}; // class GraphicsTextureManager

} // namespace
} // namespace

#endif // __GRAPHICSTEXTUREMANAGER__
