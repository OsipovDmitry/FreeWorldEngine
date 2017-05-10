#include <3rdparty/freeimage/FreeImage.h>

#include <FreeWorldEngine.h>

#include "FreeImageLoaderPlugin.h"

namespace FreeWorldEngine {

FreeImageLoaderPlugin::FreeImageLoaderPlugin() :
	m_supportExtensions()
{
}

FreeImageLoaderPlugin::~FreeImageLoaderPlugin()
{
}

std::string FreeImageLoaderPlugin::name() const
{
	return std::string("FreeImageLoaderPlugin");
}

std::string FreeImageLoaderPlugin::info() const
{
	return std::string("Free Image Manager Plugin");
}

bool FreeImageLoaderPlugin::initialize()
{
	ICore *pCore = getCoreEngine();
	if (!pCore)
		return false;

	FreeImage_Initialise();

	for (int32 extIdx = 0; extIdx < FreeImage_GetFIFCount(); ++extIdx) {
		std::string exts(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)extIdx));
		pCore->logger()->printMessage(exts);
		if (exts.empty())
			continue;
		size_t prev = 0, next;
		do {
			next = exts.find(',', prev);
			m_supportExtensions.push_back(exts.substr(prev, next - prev));
			prev = next + 1;
		} while (next != std::string::npos);
	}

	std::for_each(m_supportExtensions.cbegin(), m_supportExtensions.cend(),
		[pCore](const std::string& s) { pCore->imageLoader()->registerDataLoader(s, FreeImageLoaderPlugin::loadRaster); });

	return true;
}

void FreeImageLoaderPlugin::deinitialize()
{
	ICore *pCore = getCoreEngine();

	std::for_each(m_supportExtensions.cbegin(), m_supportExtensions.cend(),
		[pCore](const std::string& s) { pCore->imageLoader()->unregisterDataLoader(s); });

	m_supportExtensions.clear();

	FreeImage_DeInitialise();
}

Raster *FreeImageLoaderPlugin::loadRaster(const std::string& filename)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(filename.c_str(), 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
	if (fif == FIF_UNKNOWN)
		return nullptr;

	FIBITMAP *dib = nullptr;
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename.c_str());
	if (!dib)
		return nullptr;

	FreeImage_GetImageType(dib);

	return 0;
}

} // namespace