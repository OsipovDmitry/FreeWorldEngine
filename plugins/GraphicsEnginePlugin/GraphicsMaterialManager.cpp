#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>
#include <utility/File.h>

#include "GraphicsEngine.h"
#include "GraphicsMaterial.h"
#include "GraphicsMaterialManager.h"
#include "ShaderManager.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsMaterialManager::GraphicsMaterialManager() :
	m_pMaterialManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicsMaterials")),
	m_pMaterialNameGenerator(new Utility::AutoNameGenerator("MaterialName"))
{
}

GraphicsMaterialManager::~GraphicsMaterialManager()
{
	delete m_pMaterialNameGenerator;
	getCoreEngine()->destroyResourceManager(m_pMaterialManager);
}

IGraphicsMaterial *GraphicsMaterialManager::loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileFragShader, const std::string& name, std::string *pLogString)
{
	IGraphicsMaterial *pMaterial = findMaterial(name);
	if (pMaterial)
		return pMaterial;

	Renderer::IGPUProgram *pProgram = pGraphicsEngine->programManager()->loadProgram(fileVertShader, fileFragShader, pLogString);
	if (!pProgram)
		return nullptr;

	return createMaterial(pProgram, name);
}

IGraphicsMaterial *GraphicsMaterialManager::loadMaterial(const Utility::File& fileVertShader, const Utility::File& fileGeomShader, const Utility::File& fileFragShader, const std::string& name, std::string *pLogString)
{
	IGraphicsMaterial *pMaterial = findMaterial(name);
	if (pMaterial)
		return pMaterial;

	Renderer::IGPUProgram *pProgram = pGraphicsEngine->programManager()->loadProgram(fileVertShader, fileGeomShader, fileFragShader, pLogString);
	if (!pProgram)
		return nullptr;

	return createMaterial(pProgram, name);
}

IGraphicsMaterial *GraphicsMaterialManager::findMaterial(const std::string& name) const
{
	return static_cast<IGraphicsMaterial*>(m_pMaterialManager->findResource(name));
}

IGraphicsMaterial * GraphicsMaterialManager::createMaterial(Renderer::IGPUProgram * pPrgram, const std::string & name)
{
	const std::string resName = (name == "@utoname") ? m_pMaterialNameGenerator->generateName() : name;
	IGraphicsMaterial *pMaterial = findMaterial(resName);
	if (pMaterial)
		return pMaterial;

	pMaterial = new GraphicsMaterial(resName, pPrgram);
	m_pMaterialManager->addResource(pMaterial);
	return pMaterial;
}

void GraphicsMaterialManager::destroyMaterial(const std::string & name)
{
	m_pMaterialManager->destroyResource(name);
}

void GraphicsMaterialManager::destroyMaterial(IGraphicsMaterial * pMaterial)
{
	m_pMaterialManager->destroyResource(pMaterial);
}

} // namespace
} // namespace