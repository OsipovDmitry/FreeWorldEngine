#ifndef __ILOG__
#define __ILOG__

#include <string>
#include "IResource.h"

namespace FreeWorldEngine {

class ILog : public IResource {
public:
	enum MessageType {
		MessageType_Info,
		MessageType_Warning,
		MessageType_Error,
		MessageType_Critical,
	};

	virtual ~ILog() = 0 {}

	virtual void printMessage(const MessageType type, const std::string& time, const std::string message) const = 0;

}; // class ILog

} // namespace

#endif // __ILOG__