#ifndef __GRAPHICSMATERIAL__
#define __GRAPHICSMATERIAL__

#include <graphics_engine/IGraphicsMaterial.h>

namespace FreeWorldEngine {

namespace GraphicsEngine {

class GraphicsMaterial : public IGraphicsMaterial {
public:
	GraphicsMaterial(const std::string& name);
	~GraphicsMaterial();

	std::string name() const;

	Renderer::IGPUProgram *program() const;
	void setProgram(Renderer::IGPUProgram *pProgram);

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

	void bind() const;
	bool operator <(const GraphicsMaterial& other) const;

private:
	std::string m_name;
	Renderer::IGPUProgram *m_pProgram;

	enum UniformType {
		UniformType_Vec1f, UniformType_Vec1i, UniformType_Vec1ui,
		UniformType_Vec2f, UniformType_Vec2i, UniformType_Vec2ui, 
		UniformType_Vec3f, UniformType_Vec3i, UniformType_Vec3ui, 
		UniformType_Vec4f, UniformType_Vec4i, UniformType_Vec4ui,
		UniformType_Mat2f, UniformType_Mat3f, UniformType_Mat4f,
		UniformType_Sampler,
	};
	std::map<int32, std::pair<UniformType, void*> > m_uniformData;

	void setUniformData(const int32 location, const UniformType type, void *pData);
	void clearUniformData();

}; // class GraphicsMaterial

} // namespace

} // namespace

#endif // __GRAPHICSMATERIAL__