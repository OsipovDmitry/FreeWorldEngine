#ifndef __IGRAPHICSTEXTURE__
#define __IGRAPHICSTEXTURE__

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUTexture;
}

namespace GraphicsEngine {

class IGraphicsTexture : public IGraphicsResource {
public:
	virtual ~IGraphicsTexture() {}

	virtual Renderer::IGPUTexture *texture() const = 0;

}; // class IGraphicsTexture

} // namespace
} // namespace


#endif // __IGRAPHICSTEXTURE__