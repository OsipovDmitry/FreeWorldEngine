#ifndef __ILOGGER__
#define __ILOGGER__

#include "ILog.h"

namespace FreeWorldEngine {

class ILogger {
public:
	virtual ~ILogger() = 0 {}

	virtual void addLog(ILog *pLog) = 0;
	virtual ILog *findLog(const std::string& name) const = 0;
	virtual void destroyLog(const std::string& name) = 0;
	virtual void destroyLog(ILog *pLog) = 0;
	virtual void destroyAllLogs() = 0;

	virtual void printMessage(const std::string& message, const ILog::MessageType type = ILog::MessageType_Info) const = 0;

}; // class ILogger

} // namespace

#endif // __ILOGGER__