#ifndef __ILIBRARY__
#define __ILIBRARY__

#include "IResource.h"

namespace FreeWorldEngine {

class ILibrary : public IResource {
public:
	virtual ~ILibrary() = 0 {}

	virtual bool isLoaded() const = 0;
	virtual void *resolve(const std::string& funcName) const = 0;

}; // class ILibrary

} // namespace

#endif // __ILIBRARY__