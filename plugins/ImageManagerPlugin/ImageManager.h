#ifndef __IMAGEMANAGER__
#define __IMAGEMANAGER__

#include <IImageManager.h>
#include <Types.h>

namespace FreeWorldEngine {

class IResourceManager;

class ImageManager : public IImageManager {
public:
	ImageManager();
	~ImageManager();
	IImage *loadImage(const std::string& filename);
	IImage *getByName(const std::string& name) const;

private:
	IResourceManager *m_pResourceManager;

	static std::string fileExtension(const std::string& filename);
	static bool loadJPEGFile(const std::string& filename, uint32& width, uint32& height, uint32& depth, uint32& numComponents, Type& type, void *&data);
	static bool loadPNGFile(const std::string& filename, uint32& width, uint32& height, uint32& depth, uint32& numComponents, Type& type, void *&data);
};

} // namespace

#endif // __IMAGEMANAGER__