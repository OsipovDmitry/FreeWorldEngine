#ifndef __THREADMANAGERPLUGIN__
#define __THREADMANAGERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class IThreadManager;

class ThreadManagerPlugin : public IPlugin {
public:
	ThreadManagerPlugin();
	~ThreadManagerPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:
	static IThreadManager *pThreadManager;

}; // class IPlugin

} // namespace

#endif // __THREADMANAGERPLUGIN__