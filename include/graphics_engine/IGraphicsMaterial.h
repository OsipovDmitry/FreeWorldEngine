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
	enum AutoUniform {
		AutoUniform_NormalMatrix,
		AutoUniform_ModelMatrix,
		AutoUniform_ViewMatrix,
		AutoUniform_ProjectionMatrix,
		AutoUniform_ModelViewMatrix,
		AutoUniform_ViewProjectionMatrix,
		AutoUniform_ModelViewProjectionMatrix,
		AutoUniform_CameraPosition,
		AutoUniform_Light0,
		AutoUniform_Light1,
		AutoUniform_Light2,
		AutoUniform_Light3,
		AutoUniform_ActiveLightsCount,
	};

	enum DepthTestFunc {
		DepthTestFunc_Never,
		DepthTestFunc_Always,
		DepthTestFunc_Less,
		DepthTestFunc_LessEqual,
		DepthTestFunc_Equal,
		DepthTestFunc_Greater,
		DepthTestFunc_GreaterEqual,
		DepthTestFunc_NotEqual
	};

	enum BlendFunc {
		BlendFunc_Zero,
		BlendFunc_One,
		BlendFunc_SrcColor,
		BlendFunc_InvSrcColor,
		BlendFunc_DstColor,
		BlendFunc_InvDstColor,
		BlendFunc_SrcAlpha,
		BlendFunc_InvSrcAlpha,
		BlendFunc_DstAlpha,
		BlendFunc_InvDstAlpha,
		BlendFunc_ConstColor,
		BlendFunc_InvConstColor,
		BlendFunc_ConstAlpha,
		BlendFunc_InvConstAlpha,
	};

	enum BlendFuncState {
		BlendFuncState_Replace,
		BlendFuncState_Alpha,
		BlendFuncState_Add,
	};

	enum BlendEquation {
		BlendEquation_Add,
		BlendEquation_Sub,
		BlendEquation_ReverseSub,
		BlendEquation_Min,
		BlendEquation_Max
	};

	enum CullFaceState {
		CullFaceState_Disabled,
		CullFaceState_RenderFrontFaces,
		CullFaceState_RenderBackFaces
	};

	enum Tag {
		Tag_Solid,
		Tag_Transparent,
		Tag_Hud,
		Tag_Count
	};

	virtual ~IGraphicsMaterial() {}

	virtual Renderer::IGPUProgram *program() const = 0;

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
	virtual void setUniform(const int32 index,    Renderer::IGPUBuffer *pBuffer) = 0; // UBO

	virtual void setAutoUniform(const int32 location, const AutoUniform value) = 0;

	virtual void setDepthCheck(const bool state) = 0;
	virtual bool depthCheck() const = 0;

	virtual void setDepthWrite(const bool state) = 0;
	virtual bool depthWrite() const = 0;

	virtual void setDepthFunc(const DepthTestFunc func) = 0;
	virtual DepthTestFunc depthFunc() const = 0;

	virtual void setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA) = 0;
	virtual void setBlendEquation(const BlendEquation func) = 0;
	virtual BlendEquation blendEquationRGB() const = 0;
	virtual BlendEquation blendEquationA() const = 0;

	virtual void setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA) = 0;
	virtual void setBlendFunc(const BlendFuncState func) = 0;
	virtual BlendFunc blendFuncSrcRGB() const = 0;
	virtual BlendFunc blendFuncSrcA() const = 0;
	virtual BlendFunc blendFuncDstRGB() const = 0;
	virtual BlendFunc blendFuncDstA() const = 0;

	virtual void setCullFaceState(const CullFaceState state) = 0;
	virtual CullFaceState cullFaceState() const = 0;

	virtual void setTag(const Tag tag) = 0;
	virtual Tag tag() const = 0;

}; // class IGraphicsMaterial

} // namespace

} // namespace

#endif // __IGRAPHICSMATERIAL__
