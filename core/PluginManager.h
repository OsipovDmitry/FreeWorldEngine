#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include <list>

#include "IPluginManager.h"

namespace FreeWorldEngine {

class PluginManager : public IPluginManager {
public:
	typedef void(*StartPluginFunc)();
	typedef void(*EndPluginFunc)();
	typedef IPlugin*(*GetPluginFunc)();

	PluginManager();
	~PluginManager();

	IPlugin *loadPlugin(const std::string& libraryName, const std::string& startPluginFuncName, const std::string& getPluginFuncName, const std::string& endPluginFuncName);
	IPlugin *findPlugin(const std::string& pluginName) const;
	void unloadPlugin(IPlugin *pPlugin);
	void unloadPlugin(const std::string& pluginName);

	void loadPlugins(const std::string& pluginsListFileName);
	void unloadPlugins();

private:
	struct PluginData {
		StartPluginFunc startFunc;
		GetPluginFunc getFunc;
		EndPluginFunc endFunc;

		PluginData(StartPluginFunc start = 0, GetPluginFunc get = 0, EndPluginFunc end = 0) : startFunc(start), getFunc(get), endFunc(end) {}
	};

	std::list<PluginData> m_plugins;

	bool findPluginData(const std::string& pluginName, PluginData& resultData) const;
	IPlugin *loadPlugin(StartPluginFunc pStartFunc, GetPluginFunc pGetFunc, EndPluginFunc pEndFunc);
	void unloadPlugin(GetPluginFunc pGetFunc, EndPluginFunc pEndFunc);

}; // class PluginManager

} // namespace

#endif // __PLUGINMANAGER__
