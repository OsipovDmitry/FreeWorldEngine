#include <FreeWorldEngine.h>
#include <utility/File.h>

#include "ShaderManager.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class ShaderResource : public IResource {
public:
	ShaderResource(const std::string& name, Renderer::IGPUShader *pShader) :
		m_name(name), m_pShader(pShader)
	{}

	std::string name() const { return m_name; }
	Renderer::IGPUShader *shader() const { return m_pShader; }

private:
	std::string m_name;
	Renderer::IGPUShader *m_pShader;

};

class ProgramResource : public IResource {
public:
	ProgramResource(const std::string& name, Renderer::IGPUProgram *pProgram) :
		m_name(name), m_pProgram(pProgram)
	{}

	std::string name() const { return m_name; }
	Renderer::IGPUProgram *program() const { return m_pProgram; }

private:
	std::string m_name;
	Renderer::IGPUProgram *m_pProgram;

};

ShaderManager::ShaderManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForShaderManager"))
{
}

ShaderManager::~ShaderManager()
{
	destroyAllShaders();
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

Renderer::IGPUShader * ShaderManager::findShader(const std::string& name) const
{
	ShaderResource *pResource = static_cast<ShaderResource*>(m_pResourceManager->findResource(name));
	return pResource ? pResource->shader() : nullptr;
}

Renderer::IGPUShader * ShaderManager::loadShaderFromFile(const Utility::File &file, const Renderer::IGPUShader::IGPUShaderType type, std::string *pLog)
{
	bool isOpened = file.isOpened();
	if (!isOpened)
		if (!file.open(Utility::File::OpenMode_ReadOnly)) {
			if (pLog)
				*pLog += "Could not open file \"" + file.filename() + "\".";
			return nullptr;
		}

	int64 len = file.size();
	char *buf = new char[len + 1];
	file.readAll(len, buf);
	buf[len] = '\0';

	if (!isOpened)
		file.close();

	Renderer::IGPUShader *pShader = loadShaderFromData(file.filename(), std::string(buf), type, pLog);
	delete [] buf;
	return pShader;
}

Renderer::IGPUShader * ShaderManager::loadShaderFromData(const std::string& name, const std::string& data, const Renderer::IGPUShader::IGPUShaderType type, std::string *pLog)
{
	Renderer::IGPUShader *pShader = findShader(name);
	if (pShader)
		return pShader;

	pShader = pGPURenderer->createShader(type);
	if (!pShader) {
		if (pLog)
			*pLog += "Could not create shader \"" + name + "\".";
		return nullptr;
	}

	pShader->loadFromData(data);
	if (!pShader->compile(pLog)) {
		pGPURenderer->destroyShader(pShader);
		return nullptr;
	}

	m_pResourceManager->addResource(new ShaderResource(name, pShader));
	return pShader;
}

void ShaderManager::destroyShader(const std::string &name)
{
	ShaderResource *pResource = static_cast<ShaderResource*>(m_pResourceManager->findResource(name));
	if (!pResource)
		return;
	pGPURenderer->destroyShader(pResource->shader());
	m_pResourceManager->destroyResource(pResource);
}

void ShaderManager::destroyAllShaders()
{
	while (m_pResourceManager->size())
		destroyShader((*(m_pResourceManager->begin()))->name());
}


ProgramManager::ProgramManager() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForProgramManager"))
{
}

ProgramManager::~ProgramManager()
{
	destroyAllPrograms();
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

Renderer::IGPUProgram * ProgramManager::findProgram(const std::string& name) const
{
	ProgramResource *pResource = static_cast<ProgramResource*>(m_pResourceManager->findResource(name));
	return pResource ? pResource->program() : nullptr;
}

Renderer::IGPUProgram * ProgramManager::loadProgram(const std::string & vertShaderName, const std::string & fragShaderName, std::string * pLog)
{
	std::string name = programName(vertShaderName, "", fragShaderName);
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	ShaderManager *pShaderManager = pGraphicsEngine->shaderManager();
	Renderer::IGPUShader *pVertShader = pShaderManager->findShader(vertShaderName);
	Renderer::IGPUShader *pFragShader = pShaderManager->findShader(fragShaderName);

	return loadProgram(name, pVertShader, nullptr, pFragShader, pLog);
}

Renderer::IGPUProgram * ProgramManager::loadProgram(const std::string& vertShaderName, const std::string& geomShaderName, const std::string& fragShaderName, std::string *pLog)
{
	std::string name = programName(vertShaderName, geomShaderName, fragShaderName);
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	ShaderManager *pShaderManager = pGraphicsEngine->shaderManager();
	Renderer::IGPUShader *pVertShader = pShaderManager->findShader(vertShaderName);
	Renderer::IGPUShader *pGeomShader = pShaderManager->findShader(geomShaderName);
	Renderer::IGPUShader *pFragShader = pShaderManager->findShader(fragShaderName);

	return loadProgram(name, pVertShader, pGeomShader, pFragShader, pLog);
}

Renderer::IGPUProgram * ProgramManager::loadProgram(const std::string& name, Renderer::IGPUShader * pVertShader, Renderer::IGPUShader * pGeomShader, Renderer::IGPUShader * pFragShader, std::string * pLog)
{
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	pProgram = pGPURenderer->createProgram();
	if (!pProgram) {
		if (pLog)
			*pLog = "";
		return nullptr;
	}

	if (pVertShader)
		pProgram->attachShader(pVertShader);
	if (pGeomShader)
		pProgram->attachShader(pGeomShader);
	if (pFragShader)
		pProgram->attachShader(pFragShader);

	if (!pProgram->link(pLog)) {
		pGPURenderer->destroyProgram(pProgram);
		return nullptr;
	}

	m_pResourceManager->addResource(new ProgramResource(name, pProgram));
	return pProgram;
}

Renderer::IGPUProgram * ProgramManager::loadProgram(const Utility::File& fileVertShader, const Utility::File& fileFragShader, std::string * pLog)
{
	std::string name = programName(fileVertShader.filename(), "", fileFragShader.filename());
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	ShaderManager *pShaderManager = pGraphicsEngine->shaderManager();
	Renderer::IGPUShader *pVertShader = pShaderManager->loadShaderFromFile(fileVertShader, Renderer::IGPUShader::IGPUShaderType_Vertex, pLog);
	Renderer::IGPUShader *pFragShader = pShaderManager->loadShaderFromFile(fileFragShader, Renderer::IGPUShader::IGPUShaderType_Fragment, pLog);

	return loadProgram(name, pVertShader, nullptr, pFragShader, pLog);
}

Renderer::IGPUProgram * ProgramManager::loadProgram(const Utility::File& fileVertShader, const Utility::File& fileGeomShader, const Utility::File& fileFragShader, std::string * pLog)
{
	std::string name = programName(fileVertShader.filename(), fileGeomShader.filename(), fileFragShader.filename());
	Renderer::IGPUProgram *pProgram = findProgram(name);
	if (pProgram)
		return pProgram;

	ShaderManager *pShaderManager = pGraphicsEngine->shaderManager();
	Renderer::IGPUShader *pVertShader = pShaderManager->loadShaderFromFile(fileVertShader, Renderer::IGPUShader::IGPUShaderType_Vertex, pLog);
	Renderer::IGPUShader *pGeomShader = pShaderManager->loadShaderFromFile(fileGeomShader, Renderer::IGPUShader::IGPUShaderType_Geometry, pLog);
	Renderer::IGPUShader *pFragShader = pShaderManager->loadShaderFromFile(fileFragShader, Renderer::IGPUShader::IGPUShaderType_Fragment, pLog);

	return loadProgram(name, pVertShader, pGeomShader, pFragShader, pLog);
}

void ProgramManager::destroyProgram(const std::string& name)
{
	ProgramResource *pResource = static_cast<ProgramResource*>(m_pResourceManager->findResource(name));
	if (!pResource)
		return;

	pGPURenderer->destroyProgram(pResource->program());
	m_pResourceManager->destroyResource(pResource);
}

void ProgramManager::destroyAllPrograms()
{
	while (m_pResourceManager->size())
		destroyProgram((*(m_pResourceManager->begin()))->name());
}

std::string ProgramManager::programName(const std::string & vertShaderName, const std::string & geomShaderName, const std::string & fragShaderName)
{
	std::string name = vertShaderName;
	if (!geomShaderName.empty())
		name += "|" + geomShaderName;
	name += "|" + fragShaderName;
	return name;
}

} // namespace

} // namespace

