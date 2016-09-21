#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include <list>
#include "IPluginManager.h"

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
	std::list<IPlugin*> m_pluginsList;
	XMLRoot *m_pXML;

}; // class PluginManager

} // namespace

#endif // __PLUGINMANAGER__
