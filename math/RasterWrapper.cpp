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

Raster *RasterWrapper::targetRaster() const
{
	return m_pRaster;
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

void RasterWrapper::setSize(const uint32 width, const uint32 height, const uint32 depth)
{
	m_pRaster->size[0] = width;
	m_pRaster->size[1] = height;
	m_pRaster->size[2] = depth;
}

void RasterWrapper::size(uint32& width, uint32& height, uint32& depth) const
{
	width = m_pRaster->size[0];
	height = m_pRaster->size[1];
	depth = m_pRaster->size[2];
}

void RasterWrapper::setNumComponents(const uint32 count)
{
	m_pRaster->numComponents = count;
}

uint32 RasterWrapper::numComponents() const
{
	return m_pRaster->numComponents;
}

void RasterWrapper::setDataType(const Type type)
{
	m_pRaster->type = type;
}

Type RasterWrapper::type() const
{
	return m_pRaster->type;
}

void RasterWrapper::setDataPointer(void *pData)
{
	m_pRaster->pData = pData;
}

void *RasterWrapper::dataPointer() const
{
	return m_pRaster->pData;
}

} // namespace

} // namespace
