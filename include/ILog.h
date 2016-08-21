#ifndef __ILOG__
#define __ILOG__

#include <string>

namespace FreeWorldEngine {

class ILog {
public:
	virtual ~ILog() = 0 {}
	virtual void add(const std::string& text) const = 0;
	virtual const ILog& operator <<(const std::string& text) const = 0;

}; // class ILog

} // namespace

#endif // __ILOG__