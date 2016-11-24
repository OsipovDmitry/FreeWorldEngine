#include "Image.h"

namespace FreeWorldEngine {

Image::Image(const std::string& name, Raster *pRaster) :
	m_name(name),
    m_pRaster(pRaster ? pRaster : new Raster())
{
}

Image::~Image()
{
    if (m_pRaster)
        delete m_pRaster;
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
