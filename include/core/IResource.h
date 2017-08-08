#ifndef __IRESOURCE__
#define __IRESOURCE__

#include <string>

namespace FreeWorldEngine {

class IResource {
public:
	virtual ~IResource() {}
	virtual std::string name() const = 0;

}; // class IResource

} // namespace

#endif // __IRESOURCE__