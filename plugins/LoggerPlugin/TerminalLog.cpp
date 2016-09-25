#include <iostream>

#include "TerminalLog.h"

namespace FreeWorldEngine {

TerminalLog::TerminalLog()
{
}

TerminalLog::~TerminalLog()
{
}

std::string TerminalLog::name() const
{
	return "StandardTermialLog";
}

void TerminalLog::printMessage(const MessageType type, const std::string& time, const std::string message) const
{
	std::cout << time << " (" << messageType(type) << "): " << message << std::endl;
}

std::string TerminalLog::messageType(const MessageType type)
{
	switch (type) {
	case MessageType_Info: return "INFO";
	case MessageType_Warning: return "WARNING";
	case MessageType_Error: return "ERROR";
	case MessageType_Critical: return "CRITICAL";
	default: return "None";
	}
}

} // namespace