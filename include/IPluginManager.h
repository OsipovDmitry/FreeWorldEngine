#ifndef __IPLUGINMANAGER__
#define __IPLUGINMANAGER__

#include <string>

namespace FreeWorldEngine {

class IPluginManager {
public:
	virtual ~IPluginManager() = 0 {}

	virtual void loadPlugins(const std::string& pluginsListFileName) = 0;
	virtual void unloadPlugins() = 0;


}; // class IPluginManager

} // namespace

#endif // __IPLUGINMANAGER__