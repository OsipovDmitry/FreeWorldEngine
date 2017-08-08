#ifndef __IPLUGIN__
#define __IPLUGIN__

#include "IResource.h"

namespace FreeWorldEngine {

class IPlugin : public IResource {
public:
	virtual ~IPlugin() {}

	virtual std::string info() const = 0;

	virtual bool initialize() = 0;
	virtual void deinitialize() = 0;

}; // class IPlugin

} // namespace

#endif // __IPLUGIN__
