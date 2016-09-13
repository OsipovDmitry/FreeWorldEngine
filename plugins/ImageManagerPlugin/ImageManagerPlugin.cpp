#include "FreeWorldEngine.h"

#include "ImageManagerPlugin.h"
#include "ImageManager.h"

namespace FreeWorldEngine {

IImageManager *ImageManagerPlugin::pImageManager = 0;

ImageManagerPlugin::ImageManagerPlugin()
{
}

ImageManagerPlugin::~ImageManagerPlugin()
{
}

std::string ImageManagerPlugin::name() const
{
	return std::string("ImageManagerPlugin");
}

std::string ImageManagerPlugin::info() const
{
	return std::string("Image Manager Plugin");
}

bool ImageManagerPlugin::initialize() const
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	// init libs

	if (!pImageManager)
		pImageManager = new ImageManager();

	pCore->setImageManager(pImageManager);

	return true;
}

void ImageManagerPlugin::deinitialize() const
{
	ICore *pCore = getCoreEngine();

	if (pCore->imageManager() == pImageManager)
		pCore->setImageManager(0);

	delete pImageManager;
	pImageManager = 0;

	//deinit libs
}

} // namespace