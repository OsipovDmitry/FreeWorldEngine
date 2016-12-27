#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include <IPlugin.h>

namespace FreeWorldEngine {

class ISceneLoader;

class SceneLoaderPlugin : public IPlugin {
public:
	SceneLoaderPlugin();
	~SceneLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:
	static ISceneLoader *pSceneLoader;

}; // class SceneLoaderPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__