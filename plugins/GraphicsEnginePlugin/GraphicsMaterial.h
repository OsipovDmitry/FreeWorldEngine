#ifndef __GRAPHICSMATERIAL__
#define __GRAPHICSMATERIAL__

#include <utility/KeyGenerator.h>
#include <graphics_engine/IGraphicsMaterial.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class IGraphicsCamera;

class GraphicsMaterial : public IGraphicsMaterial {
public:
	GraphicsMaterial(const std::string& name, Renderer::IGPUProgram *pProgram);
	~GraphicsMaterial();

	std::string name() const;

	Renderer::IGPUProgram *program() const;

	void setUniform(const int32 location, const float value);
	void setUniform(const int32 location, const int32 value);
	void setUniform(const int32 location, const uint32 value);
	void setUniform(const int32 location, const glm::vec2& value);
	void setUniform(const int32 location, const glm::ivec2& value);
	void setUniform(const int32 location, const glm::uvec2& value);
	void setUniform(const int32 location, const glm::vec3& value);
	void setUniform(const int32 location, const glm::ivec3& value);
	void setUniform(const int32 location, const glm::uvec3& value);
	void setUniform(const int32 location, const glm::vec4& value);
	void setUniform(const int32 location, const glm::ivec4& value);
	void setUniform(const int32 location, const glm::uvec4& value);
	void setUniform(const int32 location, const glm::mat2& value);
	void setUniform(const int32 location, const glm::mat3& value);
	void setUniform(const int32 location, const glm::mat4& value);
	void setUniform(const int32 location, Renderer::IGPUTexture *pTexture);
	void setUniform(const int32 index, Renderer::IGPUBuffer *pBuffer);

	void setAutoUniform(const int32 location, const AutoUniform value);

	void setDepthCheck(const bool state);
	bool depthCheck() const;

	void setDepthWrite(const bool state);
	bool depthWrite() const;

	void setDepthFunc(const DepthTestFunc func);
	DepthTestFunc depthFunc() const;

	void setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA);
	void setBlendEquation(const BlendEquation func);
	BlendEquation blendEquationRGB() const;
	BlendEquation blendEquationA() const;

	void setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA);
	void setBlendFunc(const BlendFuncState func);
	BlendFunc blendFuncSrcRGB() const;
	BlendFunc blendFuncSrcA() const;
	BlendFunc blendFuncDstRGB() const;
	BlendFunc blendFuncDstA() const;

	bool isTransparent() const;

	void bind(IGraphicsCamera *pCamera, const glm::mat4x4& modelMatrix) const;

	class Comparator {
	public:
		bool operator ()(GraphicsMaterial *p1, GraphicsMaterial *p2);
	};

private:
	std::string m_name;
	Renderer::IGPUProgram *m_pProgram;

	enum UniformType {
		UniformType_Vec1f, UniformType_Vec1i, UniformType_Vec1ui,
		UniformType_Vec2f, UniformType_Vec2i, UniformType_Vec2ui, 
		UniformType_Vec3f, UniformType_Vec3i, UniformType_Vec3ui, 
		UniformType_Vec4f, UniformType_Vec4i, UniformType_Vec4ui,
		UniformType_Mat2f, UniformType_Mat3f, UniformType_Mat4f,
	};
	struct UniformData {
		void *pData;
		UniformType type;

		UniformData() : pData(nullptr) {}
		UniformData(void *p, UniformType t) : pData(p), type(t) {}
	};
	std::map<int32, UniformData> m_uniformData;
	std::multimap<AutoUniform, int32> m_autoUniformData;

	Utility::KeyGenerator m_textureSlotGenerator;
	std::map<Renderer::IGPUTexture*, int32> m_uniformTextures;

	Utility::KeyGenerator m_uboBindingPointGenerator;
	std::map<Renderer::IGPUBuffer*, int32> m_uniformBuffers;

	bool m_depthCheck, m_depthWrite;
	DepthTestFunc m_depthFunc;

	BlendEquation m_blendEquationRGB, m_blendEquationA;
	BlendFunc m_blendSrcRGB, m_blendSrcA, m_blendDstRGB, m_blendDstA;

	void setUniformData(const int32 location, const UniformType type, void *pData);
	int32 setUniformTexture(Renderer::IGPUTexture *pTexture);
	int32 setUniformBuffer(Renderer::IGPUBuffer *pBuffer);
	void clearUniforms();

}; // class GraphicsMaterial

} // namespace

} // namespace

#endif // __GRAPHICSMATERIAL__