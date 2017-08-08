#ifndef __WINDOWMANAGERPLUGIN__
#define __WINDOWMANAGERPLUGIN__

#include <vector>

#include <core/IPlugin.h>

namespace FreeWorldEngine {

struct Raster;

class FreeImageLoaderPlugin : public IPlugin {
public:
	FreeImageLoaderPlugin();
	~FreeImageLoaderPlugin();

	std::string name() const;
	std::string info() const;
	bool initialize();
	void deinitialize();

private:
	std::vector<std::string> m_supportExtensions;

	static Raster *loadRaster(const std::string& filename);

}; // class IPlugin

} // namespace

#endif // __WINDOWMANAGERPLUGIN__
