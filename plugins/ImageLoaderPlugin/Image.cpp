#include <Types.h>
#include <math/RasterWrapper.h>

#include "Image.h"

namespace FreeWorldEngine {

Image::Image(const std::string& name, Raster *pRaster) :
	m_name(name),
    m_pRaster(pRaster ? pRaster : new Raster)
{
}

Image::~Image()
{
	Math::RasterWrapper(m_pRaster).destroyTargetRaster();
}

std::string Image::name() const
{
	return m_name;
}

const Raster *Image::raster() const
{
    return m_pRaster;
}

} // namespace
