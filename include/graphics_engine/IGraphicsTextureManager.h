#ifndef __IGRAPHICSTEXTUREMANAGER__
#define __IGRAPHICSTEXTUREMANAGER__

#include <string>

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUTexture;
	class IGPUBuffer;
} // namespace

namespace Utility {
	class File;
} // namespace

struct Raster;

namespace GraphicsEngine {

class IGraphicsTexture;

class IGraphicsTextureManager {
public:
	virtual ~IGraphicsTextureManager() {}
	
	virtual IGraphicsTexture *createTexture(Renderer::IGPUTexture *pTexture, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTexture1D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTexture2D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTexture3D(const Raster *pRaster, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTextureCubeMap(const Raster *pRasters[], const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTexture1DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTexture2DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps = true, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTextureRectangle(const Raster *pRaster, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsTexture *createTextureBuffer(Renderer::IGPUBuffer *pBuffer, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;

	virtual IGraphicsTexture *findTexture(const std::string& name) const = 0;
	
	virtual void destroyTexture(const std::string& name) = 0;
	virtual void destroyTexture(IGraphicsTexture *pTexture) = 0;

}; // class IGraphicsTextureManager

} // namespace

} // namespace

#endif // __IGRAPHICSTEXTUREMANAGER__