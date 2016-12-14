#ifndef __IPLUGIN__
#define __IPLUGIN__

#include "IResource.h"

namespace FreeWorldEngine {

class IPlugin : public IResource {
public:
	virtual ~IPlugin() = 0 {}

	virtual std::string info() const = 0;

	virtual bool initialize() const = 0;
	virtual void deinitialize() const = 0;

}; // class IPlugin

} // namespace

#endif // __IPLUGIN__