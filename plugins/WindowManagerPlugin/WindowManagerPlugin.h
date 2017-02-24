#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class IWindowManager;

class WindowManagerPlugin : public IPlugin {
public:
	WindowManagerPlugin();
	~WindowManagerPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

private:
	static IWindowManager *pWindowManager;

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__