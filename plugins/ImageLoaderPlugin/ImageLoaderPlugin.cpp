#include "FreeWorldEngine.h"

#include "ImageLoaderPlugin.h"
#include "ImageLoader.h"

namespace FreeWorldEngine {

IImageLoader *ImageLoaderPlugin::pImageLoader = 0;

ImageLoaderPlugin::ImageLoaderPlugin()
{
}

ImageLoaderPlugin::~ImageLoaderPlugin()
{
}

std::string ImageLoaderPlugin::name() const
{
	return std::string("ImageLoaderPlugin");
}

std::string ImageLoaderPlugin::info() const
{
	return std::string("Image Manager Plugin");
}

bool ImageLoaderPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	// init libs

	if (!pImageLoader)
		pImageLoader = new ImageLoader();

	pCore->setImageLoader(pImageLoader);

	return true;
}

void ImageLoaderPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->imageLoader() == pImageLoader)
		pCore->setImageLoader(0);

	delete pImageLoader;
	pImageLoader = 0;

	//deinit libs
}

} // namespace