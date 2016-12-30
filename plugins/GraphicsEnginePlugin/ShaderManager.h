#ifndef __SHADERMANAGER__
#define __SHADERMANAGER__

#include <string>

namespace FreeWorldEngine {

class IResourceManager;

namespace Renderer {
	class IGPUShader;
	class IGPUProgram;
} // namespace

namespace GraphicsEngine {

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();

	Renderer::IGPUShader *findShader(const std::string& name) const;
	Renderer::IGPUShader *createShader(const std::string& name, Renderer::IGPUShader::IGPUShaderType type);
	void destroyShader(const std::string& name);

private:
	IResourceManager *m_pResourceManager;

}; // class ShaderManager

class ProgramManager {
public:
	ProgramManager();
	~ProgramManager();

	Renderer::IGPUProgram *findProgram(const std::string& name) const;
	Renderer::IGPUProgram *createProgram(const std::string& name);
	void destroyProgram(const std::string& name);

private:
	IResourceManager *m_pResourceManager;

}; // class ProgramManger

} // namespace

} // namespace

#endif