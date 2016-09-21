#include <fstream>

#include "ILibraryManager.h"
#include "ILibrary.h"
#include "IPlugin.h"

#include "Core.h"
#include "PluginManager.h"
#include "XML.h"

namespace FreeWorldEngine {

PluginManager::PluginManager() :
	m_pluginsList(0),
	m_pXML(0)
{
}

PluginManager::~PluginManager()
{
	unloadPlugins();
}

void PluginManager::loadPlugins(const std::string& pluginsListFileName)
{
	unloadPlugins();

	if ((m_pXML = XMLRoot::openFromFile(pluginsListFileName)) == 0) {
		LOG("Could not open file \"" + pluginsListFileName + "\".");
		return;
	}

	if (m_pXML->name() != "plugins_list") {
		LOG("The document \"" + pluginsListFileName + "\" is damaged.");
		XMLRoot::close(m_pXML);
		m_pXML = 0;
		return;
	}

	XMLNode::NodeList nodeList = m_pXML->children();
	for (XMLNode::NodeList::const_iterator it = nodeList.cbegin(); it != nodeList.cend(); ++it) {
		XMLNode *pNode = *it;
		if (pNode->name() != "plugin")
			continue;
		
		std::string libraryName = std::string(pNode->attributeValue("library"));
		std::string startFuncName = std::string(pNode->attributeValue("startFunc"));

		ILibrary *pLibrary = coreEngine->libraryManager()->loadLibrary(libraryName);
		if (!pLibrary) {
			LOG("Unable to load the plugin \"" + libraryName + "\".");
			continue;
		}

		IPlugin *(*pStartFunc)() = (IPlugin*(*)())pLibrary->resolve(startFuncName);
		if (!pStartFunc) {
			LOG("Could not get the function \"" + startFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}
		IPlugin *pPlugin = pStartFunc();
		if (pPlugin) {
			if (!pPlugin->initialize())
				LOG("The initialization function \"" + startFuncName + "\" from \"" + libraryName + "\" plugin returned \"false\".");
			m_pluginsList.push_back(pPlugin);
		}
	}
}

void PluginManager::unloadPlugins()
{
	if (!m_pXML)
		return;

	for (std::list<IPlugin*>::iterator it = m_pluginsList.begin(); it != m_pluginsList.end(); ++it)
		(*it)->deinitialize();
	m_pluginsList.clear();

	if (m_pXML->name() != "plugins_list") {
		//LOG("The document \"" + pluginsListFileName + "\" is damaged.");
		XMLRoot::close(m_pXML);
		m_pXML = 0;
		return;
	}

	XMLNode::NodeList nodeList = m_pXML->children();
	for (XMLNode::NodeList::const_iterator it = nodeList.cbegin(); it != nodeList.cend(); ++it) {
		XMLNode *pNode = *it;
		if (pNode->name() != "plugin")
			continue;

		std::string libraryName = std::string(pNode->attributeValue("library"));
		std::string endFuncName = std::string(pNode->attributeValue("endFunc"));

		ILibrary *pLibrary = coreEngine->libraryManager()->getByName(libraryName);
		if (!pLibrary) {
			LOG("Plugin \"" + libraryName + "\" closed early.");
			continue;
		}

		IPlugin *(*pEndFunc)() = (IPlugin*(*)())pLibrary->resolve(endFuncName);
		if (!pEndFunc) {
			LOG("Could not get the function \"" + endFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}
		pEndFunc();
	}
	
	XMLRoot::close(m_pXML);
	m_pXML = 0;
}

} // namespace