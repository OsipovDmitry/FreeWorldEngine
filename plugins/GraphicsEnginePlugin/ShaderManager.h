#ifndef __SHADERMANAGER__
#define __SHADERMANAGER__

#include <string>

#include <renderer/IGPUShader.h>

namespace FreeWorldEngine {

class IResourceManager;

namespace Utility {
	class File;
} // namespace

namespace GraphicsEngine {

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	Renderer::IGPUShader *findShader(const std::string& name) const;
	Renderer::IGPUShader *loadShaderFromFile(const Utility::File& file, const Renderer::IGPUShader::IGPUShaderType type, std::string *pLog = nullptr);
	Renderer::IGPUShader *loadShaderFromData(const std::string& name, const std::string& data, const Renderer::IGPUShader::IGPUShaderType type, std::string *pLog = nullptr);
	void destroyShader(const std::string& name);
	void destroyAllShaders();

private:
	IResourceManager *m_pResourceManager;

};

class ProgramManager {
public:
	ProgramManager();
	~ProgramManager();

	Renderer::IGPUProgram *findProgram(const std::string& name) const;
	Renderer::IGPUProgram *loadProgram(const std::string& vertShaderName, const std::string& fragShaderName, std::string *pLog = nullptr);
	Renderer::IGPUProgram *loadProgram(const std::string& vertShaderName, const std::string& geomShaderName, const std::string& fragShaderName, std::string *pLog = nullptr);
	Renderer::IGPUProgram *loadProgram(const std::string& name, Renderer::IGPUShader *pVertShader, Renderer::IGPUShader *pGeomShader, Renderer::IGPUShader *pFragShader, std::string *pLog = nullptr);
	Renderer::IGPUProgram *loadProgram(const Utility::File& fileVertShader, const Utility::File& fileFragShader, std::string *pLog = nullptr);
	Renderer::IGPUProgram *loadProgram(const Utility::File& fileVertShader, const Utility::File& fileGeomShader, const Utility::File& fileFragShader, std::string *pLog = nullptr);
	void destroyProgram(const std::string& name);
	void destroyAllPrograms();

private:
	IResourceManager *m_pResourceManager;

};

} // namespace

} // namespace

#endif