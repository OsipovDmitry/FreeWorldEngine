#ifndef __ILOGGER__
#define __ILOGGER__

namespace FreeWorldEngine {

class ILog;

class ILogger {
public:
	enum MessageType {
		MessageType_Info,
		MessageType_Warning,
		MessageType_Error,
		MessageType_Critical,
		MessageType_Debug
	};

	virtual ~ILogger() = 0 {}

	virtual ILog *addTextFileLog(const std::string& filename, const bool rewrite = true) = 0;
	virtual void addLog(ILog *pLog) = 0;
	virtual ILog *findLog(const std::string& name) const = 0;
	virtual void destroyLog(const std::string& name) = 0;
	virtual void destroyLog(ILog *pLog) = 0;
	virtual void destroyAllLogs() = 0;

	virtual void printMessage(const std::string& message, const MessageType type = MessageType_Info) const = 0;
	virtual std::string messageTypeString(const MessageType type) const = 0;

}; // class ILogger

} // namespace

#endif // __ILOGGER__