#ifndef __RASTERWRAPPER__
#define __RASTERWRAPPER__

#include <Types.h>
#include <math/MathSettings.h>

namespace FreeWorldEngine {

namespace Math {

class MATH_DLL RasterWrapper {
public:
	RasterWrapper(Raster *pTargetRaster);
	~RasterWrapper();

	void setPixel(void *pData, const uint32 x = 0, const uint32 y = 0, const uint32 z = 0);
	void *pixel(const uint32 x = 0, const uint32 y = 0, const uint32 z = 0) const;

	void setTargetRaster(Raster *pTargetRaster);
	void destroyTargetRaster();

	Raster *clone() const;

private:
	Raster *m_pRaster;

}; // class RasterWrapper

} // namespace

} // namespace

#endif // __RASTERWRAPPER__