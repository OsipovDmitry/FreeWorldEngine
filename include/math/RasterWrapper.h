#ifndef __RASTERWRAPPER__
#define __RASTERWRAPPER__

#include "../Types.h"
#include "MathSettings.h"

namespace FreeWorldEngine {

namespace Math {

class MATH_EXPORT RasterWrapper {
public:
	RasterWrapper(Raster *pTargetRaster);
	~RasterWrapper();

	Raster *clone() const;

	void setTarget(Raster *pTargetRaster);
	Raster *target() const;
	void destroyTarget();

	void setPixel(void *pData, const uint32 x = 0, const uint32 y = 0, const uint32 z = 0);
	void *pixel(const uint32 x = 0, const uint32 y = 0, const uint32 z = 0) const;

	void setSize(const uint32 width, const uint32 height = 1, const uint32 depth = 1);
	void size(uint32& width, uint32& height, uint32& depth) const;

	void setNumComponents(const uint32 count);
	uint32 numComponents() const;

	void setDataType(const Type type);
	Type type() const;

	void setDataPointer(void *pData);
	void *dataPointer() const;

private:
	Raster *m_pRaster;

}; // class RasterWrapper

} // namespace

} // namespace

#endif // __RASTERWRAPPER__
