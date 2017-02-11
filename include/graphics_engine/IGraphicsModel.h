#ifndef __IGRAPHICSMODEL__
#define __IGRAPHICSMODEL__

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsMaterial;

class IGraphicsModel : public IGraphicsResource {
public:
	virtual ~IGraphicsModel() {}

	virtual IGraphicsMaterial *material() const = 0;
	virtual void setMaterial(IGraphicsMaterial *pMaterial) = 0;

	virtual void setMesh(Mesh *pMesh) = 0;

}; // class IGraphicsModel

} // namespace

} // namespace

#endif // __IGRAPHICSMODEL__