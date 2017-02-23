#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include <vector>

#include <3rdparty/assimp/Importer.hpp>

#include <IPlugin.h>

namespace FreeWorldEngine {

struct SceneData;

class SceneLoaderPlugin : public IPlugin {
public:
	SceneLoaderPlugin();
	~SceneLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

	static SceneData *loadScene(const std::string& filename);

private:
	std::vector<std::string> m_supportExtensions;

	static Assimp::Importer m_assimpImporter;

}; // class SceneLoaderPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__