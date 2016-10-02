#ifndef __IIMAGEMANAGER__
#define __IIMAGEMANAGER__

#include <string>

namespace FreeWorldEngine {

class IImage;

class IImageManager {
public:
	virtual ~IImageManager() = 0 {}
	virtual IImage *loadImage(const std::string& filename) = 0;
	virtual IImage *findImage(const std::string& name) const = 0;
	virtual void destoryImage(const std::string& name) = 0;
	virtual void destoryImage(IImage *pImage) = 0;

}; // class IImageManager

} // namespace

#endif // __IIMAGEMANAGER__