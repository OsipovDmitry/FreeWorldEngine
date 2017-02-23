#ifndef __LOGGERPLUGIN__
#define __LOGGERPLUGIN__

#include <list>
#include <IPlugin.h>

namespace FreeWorldEngine {

class ILog;

class LoggerPlugin : public IPlugin {
public:
	LoggerPlugin();
	~LoggerPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

private:
	mutable std::list<ILog*> m_logsList;

}; // class LoggerPlugin

} // namespace

#endif // __LOGGERPLUGIN__