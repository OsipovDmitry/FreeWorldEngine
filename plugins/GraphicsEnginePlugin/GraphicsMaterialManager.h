#ifndef __GRAPHICSMATERIALMANAGER__
#define __GRAPHICSMATERIALMANAGER__

#include <graphics_engine/IGraphicsMaterialManager.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class AutoNameGenerator;
} // namespace

namespace GraphicsEngine {

class GraphicsMaterialManager : public IGraphicsMaterialManager {
public:
	GraphicsMaterialManager();
	~GraphicsMaterialManager();

	IGraphicsMaterial *loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileFragShader, const std::string& name, std::string *pLogString);
	IGraphicsMaterial *loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileGeomShader, const Utility::File& fileFragShader, const std::string& name, std::string *pLogString);
	
	IGraphicsMaterial *findMaterial(const std::string& name) const;
	IGraphicsMaterial *createMaterial(Renderer::IGPUProgram *pProgram, const std::string& name = "@utoname");
	
	void destroyMaterial(const std::string& name);
	void destroyMaterial(IGraphicsMaterial *pMaterial);

private:
	IResourceManager *m_pMaterialManager;
	Utility::AutoNameGenerator *m_pMaterialNameGenerator;

}; // class GraphicsMaterialManager

} // namespace
} // namespace

#endif // __GRAPHICSMATERIALMANAGER__
