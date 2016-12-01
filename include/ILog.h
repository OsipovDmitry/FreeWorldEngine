#ifndef __ILOG__
#define __ILOG__

#include <string>
#include "ILogger.h"
#include "IResource.h"

namespace FreeWorldEngine {

class ILog : public IResource {
public:
	virtual ~ILog() {}

	virtual void printMessage(const ILogger::MessageType type, const std::string& time, const std::string message) const = 0;
	virtual void setMessageColor(const ILogger::MessageType type, const std::string& color) = 0;

}; // class ILog

} // namespace

#endif // __ILOG__