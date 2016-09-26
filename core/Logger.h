#ifndef __LOGGER__
#define __LOGGER__

#include "ILogger.h"
#include "IResourceManager.h"

namespace FreeWorldEngine {

class Logger : public ILogger {
public:
	Logger();
	~Logger();

	void addLog(ILog *pLog);
	ILog *getLogByName(const std::string& name) const;
	void deleteLog(const std::string& name);
	void deleteLog(ILog *pLog);

	void printMessage(const std::string& message, const ILog::MessageType type = ILog::MessageType_Info) const;

private:
	IResourceManager *m_pResourceManager;

}; // class ILogger

} // namespace

#endif // __LOGGER__