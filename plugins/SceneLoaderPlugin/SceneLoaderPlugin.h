#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class SceneLoaderPlugin : public IPlugin {
public:
	SceneLoaderPlugin();
	~SceneLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__