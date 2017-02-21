#ifndef __IGRAPHICSMATERIAL__
#define __IGRAPHICSMATERIAL__

#include <3rdparty/glm/vec2.hpp>
#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/vec4.hpp>
#include <3rdparty/glm/mat2x2.hpp>
#include <3rdparty/glm/mat3x3.hpp>
#include <3rdparty/glm/mat4x4.hpp>

#include <Types.h>

#include "IGraphicsResource.h"

namespace FreeWorldEngine {

namespace Renderer {
	class IGPUTexture;
	class IGPUBuffer;
	class IGPUProgram;
} // namespace

namespace GraphicsEngine {

class IGraphicsMaterial : public IGraphicsResource {
public:
	virtual ~IGraphicsMaterial() {}

	virtual Renderer::IGPUProgram *program() const = 0;
	virtual void setProgram(Renderer::IGPUProgram *pProgram) = 0;

	virtual void setUniform(const int32 location, const float value) = 0;
	virtual void setUniform(const int32 location, const int32 value) = 0;
	virtual void setUniform(const int32 location, const uint32 value) = 0;
	virtual void setUniform(const int32 location, const glm::vec2& value) = 0;
	virtual void setUniform(const int32 location, const glm::ivec2& value) = 0;
	virtual void setUniform(const int32 location, const glm::uvec2& value) = 0;
	virtual void setUniform(const int32 location, const glm::vec3& value) = 0;
	virtual void setUniform(const int32 location, const glm::ivec3& value) = 0;
	virtual void setUniform(const int32 location, const glm::uvec3& value) = 0;
	virtual void setUniform(const int32 location, const glm::vec4& value) = 0;
	virtual void setUniform(const int32 location, const glm::ivec4& value) = 0;
	virtual void setUniform(const int32 location, const glm::uvec4& value) = 0;
	virtual void setUniform(const int32 location, const glm::mat2& value) = 0;
	virtual void setUniform(const int32 location, const glm::mat3& value) = 0;
	virtual void setUniform(const int32 location, const glm::mat4& value) = 0;
	virtual void setUniform(const int32 location, Renderer::IGPUTexture *pTexture) = 0;
	virtual void setUniform(const int32 index, Renderer::IGPUBuffer *pBuffer) = 0; // UBO

}; // class IGraphicsMaterial

} // namespace

} // namespace

#endif // __IGRAPHICSMATERIAL__