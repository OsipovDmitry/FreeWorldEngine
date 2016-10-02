#include <fstream>

#include "utility/XML.h"

#include "ILibraryManager.h"
#include "ILibrary.h"
#include "IPlugin.h"

#include "Core.h"
#include "PluginManager.h"

namespace FreeWorldEngine {

PluginManager::PluginManager() :
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
		std::string getFuncName = std::string(pNode->attributeValue("getFunc"));

		ILibrary *pLibrary = coreEngine->libraryManager()->loadLibrary(libraryName);
		if (!pLibrary) {
			LOG("Unable to load the plugin \"" + libraryName + "\".");
			continue;
		}

		void (*pStartFunc)() = (void(*)())pLibrary->resolve(startFuncName);
		IPlugin *(*pGetFunc)() = (IPlugin*(*)())pLibrary->resolve(getFuncName);

		if (!pStartFunc) {
			LOG("Could not get the function \"" + startFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}
		if (!pGetFunc) {
			LOG("Could not get the function \"" + getFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}

		pStartFunc();
		IPlugin *pPlugin = pGetFunc();

		if (pPlugin) {
			LOG("Load plugin \"" + pPlugin->name() + "\"");
			LOG(pPlugin->info());
			if (!pPlugin->initialize())
				LOG("The initialization function \"" + startFuncName + "\" from \"" + libraryName + "\" plugin returned \"false\".");
		}
	}
}

void PluginManager::unloadPlugins()
{
	if (!m_pXML)
		return;

	if (m_pXML->name() != "plugins_list") {
		XMLRoot::close(m_pXML);
		m_pXML = 0;
		return;
	}

	XMLNode::NodeList nodeList = m_pXML->children();
	for (XMLNode::NodeList::const_reverse_iterator it = nodeList.crbegin(); it != nodeList.crend(); ++it) {
		XMLNode *pNode = *it;
		if (pNode->name() != "plugin")
			continue;

		std::string libraryName = std::string(pNode->attributeValue("library"));
		std::string endFuncName = std::string(pNode->attributeValue("endFunc"));
		std::string getFuncName = std::string(pNode->attributeValue("getFunc"));

		ILibrary *pLibrary = coreEngine->libraryManager()->findLibrary(libraryName);
		if (!pLibrary) {
			LOG("Plugin \"" + libraryName + "\" closed early.");
			continue;
		}

		void (*pEndFunc)() = (void(*)())pLibrary->resolve(endFuncName);
		IPlugin *(*pGetFunc)() = (IPlugin*(*)())pLibrary->resolve(getFuncName);

		if (!pEndFunc) {
			LOG("Could not get the function \"" + endFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}
		if (!pGetFunc) {
			LOG("Could not get the function \"" + getFuncName + "\" in \"" + libraryName + "\" plugin.");
			continue;
		}

		IPlugin *pPlugin = pGetFunc();
		if (pPlugin) {
			LOG("Unload plugin \"" + pPlugin->name() + "\"");
			pPlugin->deinitialize();
		}
		pEndFunc();
	}
	
	XMLRoot::close(m_pXML);
	m_pXML = 0;
}

} // namespace