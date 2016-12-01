#include <iostream>

#include <FreeWorldEngine.h>

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

void TerminalLog::printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const
{
	std::cout << time << " (" << getCoreEngine()->logger()->messageTypeString(type) << "): " << message << std::endl;
}

} // namespace