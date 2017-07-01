#ifndef __PLUGINMANAGER__
#define __PLUGINMANAGER__

#include <list>

#include <core/IPluginManager.h>

namespace FreeWorldEngine {

namespace Utility {
	class Library;
}

class PluginManager : public IPluginManager {
public:
	typedef void(*StartPluginFunc)();
	typedef void(*EndPluginFunc)();
	typedef IPlugin*(*GetPluginFunc)();

	PluginManager();
	~PluginManager();

	IPlugin *loadPlugin(const std::string& libraryName, const std::string& startPluginFuncName, const std::string& getPluginFuncName, const std::string& endPluginFuncName);
	IPlugin *loadPlugin(void *startPluginFunc, void *getPluginFunc, void *endPluginFunc);
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
		Utility::Library *pLibrary;

		PluginData(StartPluginFunc start = nullptr, GetPluginFunc get = nullptr, EndPluginFunc end = nullptr, Utility::Library *pLib = nullptr) :
			startFunc(start), getFunc(get), endFunc(end), pLibrary(pLib) {}
	};

	std::list<PluginData> m_plugins;

	bool findPluginData(const std::string& pluginName, PluginData& resultData) const;
	IPlugin *loadPlugin(StartPluginFunc pStartFunc, GetPluginFunc pGetFunc, EndPluginFunc pEndFunc, Utility::Library *pLib);
	void unloadPlugin(GetPluginFunc pGetFunc, EndPluginFunc pEndFunc, Utility::Library *pLib);

}; // class PluginManager

} // namespace

#endif // __PLUGINMANAGER__
