#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include <list>
#include <rapidxml/rapidxml.hpp>
#include "IPluginManager.h"

namespace FreeWorldEngine {

class IPlugin;

class PluginManager : public IPluginManager {
public:
	PluginManager();
	~PluginManager();

	void loadPlugins(const std::string& pluginsListFileName);
	void unloadPlugins();

private:
	std::list<IPlugin*> m_pluginsList;
	char *m_pXmlBuffer;
	rapidxml::xml_document<> m_xmlDoc;

}; // class PluginManager

} // namespace

#endif // __PLUGINMANAGER__