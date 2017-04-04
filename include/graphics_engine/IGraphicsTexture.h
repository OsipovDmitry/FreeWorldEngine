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
	enum WrapMode {
		WrapMode_Clamp,
		WrapMode_Repeat,
		WrapMode_MirroredRepeat
	};
	enum FilterMode {
		FilterMode_Off,
		FilterMode_BiLinear,
		FilterMode_TriLinear // Должны быть сгенерированы мипмап уровни предварительно!
	};

	virtual ~IGraphicsTexture() {}

	virtual Renderer::IGPUTexture *texture() const = 0;

	virtual void setWrapMode(const WrapMode wrapMode) = 0;
	virtual void setFilterMode(const FilterMode filterMode) = 0;

}; // class IGraphicsTexture

} // namespace
} // namespace


#endif // __IGRAPHICSTEXTURE__