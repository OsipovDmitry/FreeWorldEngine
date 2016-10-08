#ifndef __LOGGER__
#define __LOGGER__

#include "ILogger.h"
#include "IResourceManager.h"

namespace FreeWorldEngine {

class Logger : public ILogger {
public:
	Logger();
	~Logger();

	ILog *addTextFileLog(const std::string& filename);
	void addLog(ILog *pLog);
	ILog *findLog(const std::string& name) const;
	void destroyLog(const std::string& name);
	void destroyLog(ILog *pLog);
	void destroyAllLogs();

	void printMessage(const std::string& message, const ILog::MessageType type = ILog::MessageType_Info) const;

private:
	IResourceManager *m_pResourceManager;

}; // class ILogger

} // namespace

#endif // __LOGGER__