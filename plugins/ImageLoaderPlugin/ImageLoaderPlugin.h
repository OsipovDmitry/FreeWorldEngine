#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include "IPlugin.h"

namespace FreeWorldEngine {

class IImageLoader;

class ImageLoaderPlugin : public IPlugin {
public:
	ImageLoaderPlugin();
	~ImageLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize() const;
	void deinitialize() const;

private:
	static ImageLoaderPlugin *pImageLoader;

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__