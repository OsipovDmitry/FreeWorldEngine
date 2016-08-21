#include <fstream>

#include "ILibraryManager.h"
#include "ILibrary.h"
#include "IPlugin.h"

#include "Core.h"
#include "PluginManager.h"

namespace FreeWorldEngine {

PluginManager::PluginManager() :
	m_pluginsList(0),
	m_pXmlBuffer(0),
	m_xmlDoc()
{
}

PluginManager::~PluginManager()
{
	unloadPlugins();
}

void PluginManager::loadPlugins(const std::string& pluginsListFileName)
{
	unloadPlugins();

	std::ifstream file(pluginsListFileName, std::ios::binary);
	if (!file.is_open()) {
		//
		return;
	}
	std::filebuf* pFileBuffer = file.rdbuf();
	std::size_t size = pFileBuffer->pubseekoff(0, file.end, file.in);
	pFileBuffer->pubseekpos(0, file.in);
	m_pXmlBuffer = new char[size+1];
	pFileBuffer->sgetn(m_pXmlBuffer, size);
	m_pXmlBuffer[size] = '\0';
	file.close();

	m_xmlDoc.parse<0>(m_pXmlBuffer);
	rapidxml::xml_node<> *pNode = m_xmlDoc.first_node();
	if (std::string(pNode->name()) != "plugins_list") {
		unloadPlugins();
		return;
	}

	pNode = pNode->first_node("plugin");
	while (pNode) {
		std::string libraryName = std::string(pNode->first_attribute("library")->value());
		std::string startFuncName = std::string(pNode->first_attribute("startFunc")->value());

		ILibrary *pLibrary = coreEngine->libraryManager()->loadLibrary(libraryName);
		if (!pLibrary) {
			//
			pNode = pNode->next_sibling("plugin");
			continue;
		}
		if (!pLibrary->isLoaded()) {
			//
			pNode = pNode->next_sibling("plugin");
			continue;
		}
		IPlugin *(*pStartFunc)() = (IPlugin*(*)())pLibrary->resolve(startFuncName);
		if (!pStartFunc) {
			//
			pNode = pNode->next_sibling("plugin");
			continue;
		}
		IPlugin *pPlugin = pStartFunc();
		if (pPlugin) {
			if (!pPlugin->initialize()) {
			//
			}
			m_pluginsList.push_back(pPlugin);
		}

		pNode = pNode->next_sibling("plugin");
	}
}

void PluginManager::unloadPlugins()
{
	for (std::list<IPlugin*>::iterator it = m_pluginsList.begin(); it != m_pluginsList.end(); ++it)
		(*it)->deinitialize();
	m_pluginsList.clear();

	rapidxml::xml_node<> *pNode = m_xmlDoc.first_node();
	if (!pNode || std::string(pNode->name()) != "plugins_list") {
		//
		return;
	}
	pNode = pNode->first_node("plugin");

	while (pNode) {
		std::string libraryName = std::string(pNode->first_attribute("library")->value());
		std::string endFuncName = std::string(pNode->first_attribute("endFunc")->value());

		ILibrary *pLibrary = coreEngine->libraryManager()->getByName(libraryName);
		if (!pLibrary) {
			//
			continue;
		}
		if (!pLibrary->isLoaded()) {
			//
			continue;
		}
		IPlugin *(*pEndFunc)() = (IPlugin*(*)())pLibrary->resolve(endFuncName);
		if (!pEndFunc) {
			//
			continue;
		}
		pEndFunc();

		pNode = pNode->next_sibling("plugin");
	}
	
	m_xmlDoc.clear();
	delete [] m_pXmlBuffer;
	m_pXmlBuffer = 0;
}

} // namespace