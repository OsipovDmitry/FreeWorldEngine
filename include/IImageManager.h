#ifndef __IIMAGEMANAGER__
#define __IIMAGEMANAGER__

#include <string>

namespace FreeWorldEngine {

class IImage;
class IResourceManager;

class IImageManager {
public:
	virtual ~IImageManager() = 0 {}
	virtual IImage *loadImage(/*const std::string&*/const char* filename) = 0;
	virtual IImage *getByName(const std::string& name) const = 0;
	virtual IResourceManager *resourceManager() const = 0;

}; // class IImageManager

} // namespace

#endif // __IIMAGEMANAGER__