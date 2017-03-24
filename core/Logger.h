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
	ILog *findLog(const std::string& name) const;
	void destroyLog(const std::string& name);
	void destroyLog(ILog *pLog);
	void destroyAllLogs();

	void printMessage(const std::string& message, const MessageType type = MessageType_Info) const;
	std::string messageTypeString(const MessageType type) const;

private:
	IResourceManager *m_pResourceManager;

}; // class ILogger

} // namespace

#endif // __LOGGER__