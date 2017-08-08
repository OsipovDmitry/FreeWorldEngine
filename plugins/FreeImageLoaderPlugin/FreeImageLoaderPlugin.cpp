#include <cstring>

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
	ICore *pCore = ICore::instance();
	if (!pCore)
		return false;

	FreeImage_Initialise();

	for (int32 extIdx = 0; extIdx < FreeImage_GetFIFCount(); ++extIdx) {
		std::string exts(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)extIdx));
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
	ICore *pCore = ICore::instance();

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

	FREE_IMAGE_COLOR_TYPE colorType = FreeImage_GetColorType(dib);
	if (colorType != FIC_RGB && colorType != FIC_RGBALPHA) {
		FreeImage_Unload(dib);
		return false;
	}

	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);
	Type type;
	switch (imageType) {
		case FIT_BITMAP: {
			type = TYPE_UNSIGNED_INT_8; break;
		}
		case FIT_UINT16:
		case FIT_RGB16:
		case FIT_RGBA16: {
			type = TYPE_UNSIGNED_INT_16; break;
		}
		case FIT_INT16: {
			type = TYPE_SIGNED_INT_16; break;
		}
		case FIT_UINT32: {
			type = TYPE_UNSIGNED_INT_32; break;
		}
		case FIT_INT32: {
			type = TYPE_SIGNED_INT_32; break;
		}
		case FIT_FLOAT:
		case FIT_RGBF:
		case FIT_RGBAF: {
			type = TYPE_FLOAT; break;
		}
		default: {
			FreeImage_Unload(dib);
			return false;
		}
	}

	BYTE *pBits = FreeImage_GetBits(dib);
	if (!pBits) {
		FreeImage_Unload(dib);
		return false;
	}

	Raster *pRaster = new Raster;

	pRaster->size[0] = FreeImage_GetWidth(dib);
	pRaster->size[1] = FreeImage_GetHeight(dib);
	pRaster->size[2] = 1;
	pRaster->numComponents = FreeImage_GetLine(dib) / pRaster->size[0] / TypeSize[type];
	pRaster->type = type;
	size_t dataSize = pRaster->size[0] * pRaster->size[1] * pRaster->size[2] * pRaster->numComponents * TypeSize[type];
	pRaster->pData = new uint8[dataSize];
	memcpy(pRaster->pData, pBits, dataSize);

	return pRaster;
}

} // namespace
