#ifndef __IIMAGEMANAGER__
#define __IIMAGEMANAGER__

//#include "Types.h"
#include <string>

namespace FreeWorldEngine {

class IImage;

class IImageLoader {
public:
	virtual ~IImageLoader() = 0 {}
	virtual IImage *loadImage(const std::string& filename) = 0;
	virtual IImage *findImage(const std::string& name) const = 0;
	virtual void destoryImage(const std::string& name) = 0;
	virtual void destoryImage(IImage *pImage) = 0;

	//typedef Raster*(*LoadRasterFunc)(const std::string& filename);

	//virtual void registerLoadFunc(const std::string& format, LoadRasterFunc func) = 0;

}; // class IImageManager

} // namespace

#endif // __IIMAGEMANAGER__