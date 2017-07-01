#ifndef __TERMINALLOG__
#define __TERMINALLOG__

#include <core/ILog.h>

namespace FreeWorldEngine {

class TerminalLog : public ILog {
public :
	TerminalLog();
	~TerminalLog();

	std::string name() const;

	void printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const;
	void setMessageColor(const ILogger::MessageType type, const std::string& color) {}

}; // class TerminalLog

} // namespace

#endif // __TERMINALLOG__
