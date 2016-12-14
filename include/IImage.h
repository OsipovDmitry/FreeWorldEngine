#ifndef __IIMAGE__
#define __IIMAGE__

#include "IResource.h"

struct Raster;

namespace FreeWorldEngine {

class IImage : public IResource {
public:
	virtual ~IImage() = 0 {}
    virtual const Raster *raster() const = 0;

}; // class IImage

} // namespace

#endif // __IIMAGE__
