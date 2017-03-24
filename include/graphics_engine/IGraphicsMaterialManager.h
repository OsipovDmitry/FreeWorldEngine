#ifndef __IGRAPHICSMATERIALMANAGER__
#define __IGRAPHICSMATERIALMANAGER__

#include <string>

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUProgram;
} // namespace

namespace Utility {
	class File;
} // namespace

namespace GraphicsEngine {

class IGraphicsMaterial;

class IGraphicsMaterialManager {
public:
	virtual ~IGraphicsMaterialManager() {}
	
	virtual IGraphicsMaterial *loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileFragShader, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;
	virtual IGraphicsMaterial *loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileGeomShader, const Utility::File& fileFragShader, const std::string& name = "@utoname", std::string *pLogString = nullptr) = 0;

	virtual IGraphicsMaterial *findMaterial(const std::string& name) const = 0;
	virtual IGraphicsMaterial *createMaterial(Renderer::IGPUProgram *pPrgram, const std::string& name = "@utoname") = 0;

	virtual void destroyMaterial(const std::string& name) = 0;
	virtual void destroyMaterial(IGraphicsMaterial *pMaterial) = 0;

}; // class IGraphicsMaterialManager

} // namespace

} // namespace

#endif // __IGRAPHICSMATERIALMANAGER__