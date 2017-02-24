#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include <IPlugin.h>
#include <Types.h>

namespace FreeWorldEngine {

class ImageLoaderPlugin : public IPlugin {
public:
	ImageLoaderPlugin();
	~ImageLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

private:
	static Raster *loadJPEGFile(const std::string& filename);
	static Raster *loadPNGFile(const std::string& filename);

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__