#include <FreeWorldEngine.h>

#include "ShaderManager.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

template <class T>
class GPUResource : public IResource {
public:
	GPUResource(const std::string& name, T *pResource) : m_name(name), m_pResource(pResource) {}
	~GPUResource() {}

	std::string name() const { return m_name; }
	T *resource() const { return m_pResource; }

private:
	std::string m_name;
	T *m_pResource;

}; // class GPUResource

ShaderManager::ShaderManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForShaders"))
{
}

ShaderManager::~ShaderManager()
{
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

Renderer::IGPUShader *ShaderManager::findShader(const std::string& name) const
{
	GPUResource<Renderer::IGPUShader> *pShader = static_cast<GPUResource<Renderer::IGPUShader>*>(m_pResourceManager->findResource(name));
	return (pShader) ? pShader->resource() : 0;
}

Renderer::IGPUShader *ShaderManager::createShader(const std::string& name, Renderer::IGPUShader::IGPUShaderType type)
{
	Renderer::IGPUShader *pShader = findShader(name);
	if (pShader)
		return pShader;

	pShader = pGPURenderer->createShader(type);
	if (!pShader)
		return 0;

	m_pResourceManager->addResource(new GPUResource<Renderer::IGPUShader>(name, pShader));
	return pShader;
}

void ShaderManager::destroyShader(const std::string& name)
{
	GPUResource<Renderer::IGPUShader> *pShader = static_cast<GPUResource<Renderer::IGPUShader>*>(m_pResourceManager->findResource(name));
	pGPURenderer->destroyShader(pShader->resource());

	m_pResourceManager->destroyResource(name);
}

ProgramManager::ProgramManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForPrograms"))
{
}

ProgramManager::~ProgramManager()
{
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

Renderer::IGPUProgram *ProgramManager::findProgram(const std::string& name) const
{
	GPUResource<Renderer::IGPUProgram> *pProgram = static_cast<GPUResource<Renderer::IGPUProgram>*>(m_pResourceManager->findResource(name));
	return (pProgram) ? pProgram->resource() : 0;
}

Renderer::IGPUProgram *ProgramManager::createProgram(const std::string& name)
{
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	pProgram = pGPURenderer->createProgram();
	if (!pProgram)
		return 0;

	m_pResourceManager->addResource(new GPUResource<Renderer::IGPUProgram>(name, pProgram));
	return pProgram;
}

void ProgramManager::destroyProgram(const std::string& name)
{
	GPUResource<Renderer::IGPUProgram> *pProgram = static_cast<GPUResource<Renderer::IGPUProgram>*>(m_pResourceManager->findResource(name));
	pGPURenderer->destroyProgram(pProgram->resource());

	m_pResourceManager->destroyResource(name);
}


} // namespace

} // namespace

