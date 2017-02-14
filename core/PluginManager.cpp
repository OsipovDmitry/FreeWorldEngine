#include "utility/XML.h"

#include "ILibraryManager.h"
#include "ILibrary.h"
#include "IPlugin.h"

#include "Core.h"
#include "PluginManager.h"

namespace FreeWorldEngine {

namespace {
	std::vector<std::string> c_pluginsListGroups( {"log", "window_manager", "renderer", "graphics_engine", "other"} );
}

PluginManager::PluginManager() :
	m_plugins()
{
}

PluginManager::~PluginManager()
{
	unloadPlugins();
}

IPlugin *PluginManager::loadPlugin(const std::string& libraryName, const std::string& startPluginFuncName, const std::string& getPluginFuncName, const std::string& endPluginFuncName)
{
	ILibrary *pLibrary = coreEngine->libraryManager()->loadLibrary(libraryName);
	if (!pLibrary) {
		LOG_ERROR("Could not load plugin from \"" + libraryName + "\".");
		return 0;
	}
	
	StartPluginFunc pStartFunc = (StartPluginFunc)pLibrary->resolve(startPluginFuncName);
	GetPluginFunc pGetFunc = (GetPluginFunc)pLibrary->resolve(getPluginFuncName);
	EndPluginFunc pEndFunc = (EndPluginFunc)pLibrary->resolve(endPluginFuncName);

	if (!pStartFunc)
		LOG_WARNING("Could not get the function \"" + startPluginFuncName + "\" from \"" + libraryName + "\" plugin");
		
	if (!pGetFunc)
		LOG_WARNING("Could not get the function \"" + getPluginFuncName + "\" from \"" + libraryName + "\" plugin");

	if (!pEndFunc)
		LOG_WARNING("Could not get the function \"" + endPluginFuncName + "\" from \"" + libraryName + "\" plugin");

	return loadPlugin(pStartFunc, pGetFunc, pEndFunc);
}

IPlugin *PluginManager::findPlugin(const std::string& pluginName) const
{
	PluginData pluginData;
	if (findPluginData(pluginName, pluginData))
		if (pluginData.getFunc)
			return pluginData.getFunc();
	return 0;
}

void PluginManager::unloadPlugin(IPlugin *pPlugin)
{
	unloadPlugin(pPlugin->name());
}

void PluginManager::unloadPlugin(const std::string& pluginName)
{
	PluginData pluginData;
	if (findPluginData(pluginName, pluginData))
		unloadPlugin(pluginData.getFunc, pluginData.endFunc);
}

void PluginManager::loadPlugins(const std::string& pluginsListFileName)
{
	XMLRoot *m_pXML;
	if ((m_pXML = XMLRoot::openFromFile(pluginsListFileName)) == 0) {
		LOG_ERROR("Could not open file \"" + pluginsListFileName + "\"");
		return;
	}

	if (m_pXML->name() != "plugins_list") {
		LOG_ERROR("The document \"" + pluginsListFileName + "\" is damaged");
		XMLRoot::close(m_pXML);
		return;
	}

	for (int32 groupIdx = 0; groupIdx < c_pluginsListGroups.size(); ++groupIdx) {
		XMLNode *pGroupNode = m_pXML->child(c_pluginsListGroups.at(groupIdx));
		if (!pGroupNode)
			continue;

		XMLNode::NodeList nodeList = pGroupNode->children();
		for (XMLNode::NodeList::const_iterator it = nodeList.cbegin(); it != nodeList.cend(); ++it) {
			XMLNode *pNode = *it;
			if (pNode->name() != "plugin")
				continue;
			std::string libraryName = std::string(pNode->attributeValue("library"));
			std::string startFuncName = std::string(pNode->attributeValue("startFunc"));
			std::string getFuncName = std::string(pNode->attributeValue("getFunc"));
			std::string endFuncName = std::string(pNode->attributeValue("endFunc"));
			loadPlugin(libraryName, startFuncName, getFuncName, endFuncName);
		}
	}

	XMLRoot::close(m_pXML);
}

void PluginManager::unloadPlugins()
{
	while (m_plugins.size()) {
		PluginData& data = m_plugins.back();
		unloadPlugin(data.getFunc, data.endFunc);
	}
}

bool PluginManager::findPluginData(const std::string& pluginName, PluginData& resultData) const
{
	for (std::list<PluginData>::const_iterator it = m_plugins.cbegin(); it != m_plugins.cend(); ++it)
		if (it->getFunc) {
			IPlugin *pPlugin = it->getFunc();
			if (!pPlugin)
				continue;
			if (pPlugin->name() == pluginName) {
				resultData = *it;
				return true;
			}
		}
	return false;
}

IPlugin *PluginManager::loadPlugin(StartPluginFunc pStartFunc, GetPluginFunc pGetFunc, EndPluginFunc pEndFunc)
{
	if (!pStartFunc && !pGetFunc && !pEndFunc)
		return 0;

	if (pStartFunc)
		pStartFunc();

	IPlugin *pPlugin = 0;
	if (pGetFunc)
		pPlugin = pGetFunc();

	if (pPlugin) {
		std::string pluginName = pPlugin->name();
		if (findPlugin(pluginName)) {
			LOG_WARNING("Plugin \""+pluginName+"\" is loaded earlier");
			return pPlugin;
		}
	}

	m_plugins.push_back(PluginData(pStartFunc, pGetFunc, pEndFunc));

	if (pPlugin) {
		LOG("Load plugin \"" + pPlugin->name() + "\"");
		LOG(pPlugin->info());
		if (!pPlugin->initialize())
			LOG_WARNING("The initialization function from \"" + pPlugin->name() + "\" returned \"false\"");
	}

	return pPlugin;
}

void PluginManager::unloadPlugin(GetPluginFunc pGetFunc, EndPluginFunc pEndFunc)
{
	IPlugin *pPlugin = 0;
	if (pGetFunc)
		pPlugin = pGetFunc();

	if (pPlugin) {
		LOG("Unload plugin \"" + pPlugin->name() + "\"");
		pPlugin->deinitialize();

		std::list<PluginData>::iterator it;
		for (it = m_plugins.begin(); it != m_plugins.end(); ++it)
			if (it->getFunc)
				if (it->getFunc() == pPlugin)
					break;

		if (it != m_plugins.end())
			m_plugins.erase(it);
	}

	if (pEndFunc)
		pEndFunc();
}

} // namespace