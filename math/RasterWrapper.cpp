#include <math/RasterWrapper.h>

namespace FreeWorldEngine {

namespace Math {

RasterWrapper::RasterWrapper(Raster *pTargetRaster) :
	m_pRaster(pTargetRaster)
{
}

RasterWrapper::~RasterWrapper()
{
}

void RasterWrapper::setPixel(void *pData, const uint32 x, const uint32 y, const uint32 z)
{
	const uint32 pixelStride = TypeSize[m_pRaster->type] * m_pRaster->numComponents;
	memcpy((int8*)m_pRaster->pData + pixelStride * ((m_pRaster->size[1]*z + y)*m_pRaster->size[0] + x), pData, pixelStride);
}

void *RasterWrapper::pixel(const uint32 x, const uint32 y, const uint32 z) const
{
	const uint32 pixelStride = TypeSize[m_pRaster->type] * m_pRaster->numComponents;
	return (int8*)m_pRaster->pData + pixelStride * ((m_pRaster->size[1]*z + y)*m_pRaster->size[0] + x);
}

void RasterWrapper::setTargetRaster(Raster *pTargetRaster)
{
	m_pRaster = pTargetRaster;
}

void RasterWrapper::destroyTargetRaster()
{
	if (!m_pRaster)
		return;

	delete m_pRaster->pData;
	delete m_pRaster;
	m_pRaster = 0;
}

Raster *RasterWrapper::clone() const
{
	Raster *pNewRaster = new Raster;
	pNewRaster->numComponents = m_pRaster->numComponents;
	pNewRaster->type = m_pRaster->type;
	pNewRaster->size[0] = m_pRaster->size[0];
	pNewRaster->size[1] = m_pRaster->size[1];
	pNewRaster->size[2] = m_pRaster->size[2];
	uint32 dataSize = m_pRaster->size[0] * m_pRaster->size[1] * m_pRaster->size[2] * m_pRaster->numComponents * TypeSize[m_pRaster->type];
	pNewRaster->pData = new int8 [dataSize];
	memcpy(pNewRaster->pData, m_pRaster->pData, dataSize);
	return pNewRaster;
}


} // namespace

} // namespace
