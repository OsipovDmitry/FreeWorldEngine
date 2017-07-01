#ifndef __IPLUGINMANAGER__
#define __IPLUGINMANAGER__

#include <string>

namespace FreeWorldEngine {

class IPlugin;

class IPluginManager {
public:
	virtual ~IPluginManager() {}

	virtual IPlugin *loadPlugin(const std::string& libraryName, const std::string& startPluginFuncName, const std::string& getPluginFuncName, const std::string& endPluginFuncName) = 0;
	virtual IPlugin *loadPlugin(void *startPluginFunc, void *getPluginFunc, void *endPluginFunc) = 0;
	virtual IPlugin *findPlugin(const std::string& pluginName) const = 0;
	virtual void unloadPlugin(IPlugin *pPlugin) = 0;
	virtual void unloadPlugin(const std::string& pluginName) = 0;

	virtual void loadPlugins(const std::string& pluginsListFileName) = 0;
	virtual void unloadPlugins() = 0;

}; // class IPluginManager

} // namespace

#endif // __IPLUGINMANAGER__
