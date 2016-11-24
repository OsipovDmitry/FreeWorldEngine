#ifndef __IMAGE__
#define __IMAGE__

#include "IImage.h"

namespace FreeWorldEngine {

class Image : public IImage {
public:
    Image(const std::string& name, Raster *pRaster = 0);
	~Image();
	std::string name() const;
    const Raster *raster() const;

private:
	std::string m_name;
    Raster *m_pRaster;

}; // class Image

} // namespace

#endif // IMAGE
