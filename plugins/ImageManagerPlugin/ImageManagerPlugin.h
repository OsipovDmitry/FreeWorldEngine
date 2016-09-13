#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class IImageManager;

class ImageManagerPlugin : public IPlugin {
public:
	ImageManagerPlugin();
	~ImageManagerPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:
	static IImageManager *pImageManager;

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__