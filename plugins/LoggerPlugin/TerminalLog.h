#ifndef __TERMINALLOG__
#define __TERMINALLOG__

#include <ILog.h>

namespace FreeWorldEngine {

class TerminalLog : public ILog {
public :
	TerminalLog();
	~TerminalLog();

	std::string name() const;
	void printMessage(const MessageType type, const std::string& time, const std::string message) const;

private:
	static std::string messageType(const MessageType type);

}; // class TerminalLog

} // namespace

#endif // __TERMINALLOG__