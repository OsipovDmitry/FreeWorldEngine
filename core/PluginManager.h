#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include "IPluginManager.h"
#include "IResourceManager.h"

namespace FreeWorldEngine {

class IPlugin;
class XMLRoot;

class PluginManager : public IPluginManager {
public:
	PluginManager();
	~PluginManager();

	void loadPlugins(const std::string& pluginsListFileName);
	void unloadPlugins();

private:
	XMLRoot *m_pXML;

}; // class PluginManager

} // namespace

#endif // __PLUGINMANAGER__
