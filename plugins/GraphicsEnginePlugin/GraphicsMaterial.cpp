#include <renderer/IGPUShader.h>
#include <renderer/IGPURenderer.h>

#include "GraphicsEngine.h"
#include "GraphicsMaterial.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsMaterial::GraphicsMaterial(const std::string& name) :
	m_name(name),
	m_pProgram(0),
	m_uniformData()
{
}

GraphicsMaterial::~GraphicsMaterial()
{
	clearUniformData();
}

std::string GraphicsMaterial::name() const
{
	return m_name;
}

Renderer::IGPUProgram *GraphicsMaterial::program() const
{
	return m_pProgram;
}

void GraphicsMaterial::setProgram(Renderer::IGPUProgram *pProgram)
{
	m_pProgram = pProgram;
}

void GraphicsMaterial::setUniform(const int32 location, const float value)
{
	setUniformData(location, UniformType_Vec1f, new float(value));
}

void GraphicsMaterial::setUniform(const int32 location, const int32 value)
{
	setUniformData(location, UniformType_Vec1i, new int32(value));
}

void GraphicsMaterial::setUniform(const int32 location, const uint32 value)
{
	setUniformData(location, UniformType_Vec1ui, new uint32(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::vec2& value)
{
	setUniformData(location, UniformType_Vec2f, new glm::vec2(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::ivec2& value)
{
	setUniformData(location, UniformType_Vec2i, new glm::ivec2(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::uvec2& value)
{
	setUniformData(location, UniformType_Vec2ui, new glm::uvec2(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::vec3& value)
{
	setUniformData(location, UniformType_Vec3f, new glm::vec3(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::ivec3& value)
{
	setUniformData(location, UniformType_Vec3i, new glm::ivec3(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::uvec3& value)
{
	setUniformData(location, UniformType_Vec3ui, new glm::uvec3(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::vec4& value)
{
	setUniformData(location, UniformType_Vec4f, new glm::vec4(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::ivec4& value)
{
	setUniformData(location, UniformType_Vec4i, new glm::ivec4(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::uvec4& value)
{
	setUniformData(location, UniformType_Vec4f, new glm::vec4(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::mat2& value)
{
	setUniformData(location, UniformType_Mat2f, new glm::mat2(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::mat3& value)
{
	setUniformData(location, UniformType_Mat3f, new glm::mat3(value));
}

void GraphicsMaterial::setUniform(const int32 location, const glm::mat4& value)
{
	setUniformData(location, UniformType_Mat4f, new glm::mat4(value));
}

void GraphicsMaterial::setUniform(const int32 location, Renderer::IGPUTexture *pTexture)
{
	setUniformData(location, UniformType_Sampler, new intptr_t((intptr_t)pTexture));
}

void GraphicsMaterial::bind() const
{
	int32 textureSlot = 0;

	for (std::map<int32, std::pair<UniformType, void*> >::const_iterator it = m_uniformData.cbegin(); it != m_uniformData.cend(); ++it) {
		int32 location = it->first;
		std::pair<UniformType, void*> data = it->second;

		switch (data.first) {
		case UniformType_Vec1f: { m_pProgram->setUniform(location, *(const float*)(data.second)); break; }
		case UniformType_Vec1i: { m_pProgram->setUniform(location, *(const int32*)(data.second)); break; }
		case UniformType_Vec1ui: { m_pProgram->setUniform(location, *(const uint32*)(data.second)); break; }
		case UniformType_Vec2f: { m_pProgram->setUniform(location, *(const glm::vec2*)(data.second)); break; }
		case UniformType_Vec2i: { m_pProgram->setUniform(location, *(const glm::ivec2*)(data.second)); break; }
		case UniformType_Vec2ui: { m_pProgram->setUniform(location, *(const glm::uvec2*)(data.second)); break; }
		case UniformType_Vec3f: { m_pProgram->setUniform(location, *(const glm::vec3*)(data.second)); break; }
		case UniformType_Vec3i: { m_pProgram->setUniform(location, *(const glm::ivec3*)(data.second)); break; }
		case UniformType_Vec3ui: { m_pProgram->setUniform(location, *(const glm::uvec3*)(data.second)); break; }
		case UniformType_Vec4f: { m_pProgram->setUniform(location, *(const glm::vec4*)(data.second)); break; }
		case UniformType_Vec4i: { m_pProgram->setUniform(location, *(const glm::ivec4*)(data.second)); break; }
		case UniformType_Vec4ui: { m_pProgram->setUniform(location, *(const glm::uvec4*)(data.second)); break; }
		case UniformType_Mat2f: { m_pProgram->setUniform(location, *(const glm::mat2*)(data.second)); break; }
		case UniformType_Mat3f: { m_pProgram->setUniform(location, *(const glm::mat3*)(data.second)); break; }
		case UniformType_Mat4f: { m_pProgram->setUniform(location, *(const glm::mat4*)(data.second)); break; }
		case UniformType_Sampler: { m_pProgram->setUniform(location, textureSlot); pGPURenderer->setTexture((Renderer::IGPUTexture*)(data.second), textureSlot); textureSlot++; break; }
		default: break;
		}
	}
}

bool GraphicsMaterial::operator <(const GraphicsMaterial& other) const
{
	return m_pProgram < other.m_pProgram;
}

void GraphicsMaterial::setUniformData(const int32 location, const UniformType type, void *pData)
{
	std::map<int32, std::pair<UniformType, void*> >::const_iterator it = m_uniformData.find(location);
	if (it != m_uniformData.cend())
		delete it->second.second;
	m_uniformData[location] = std::make_pair(type, pData);
}

void GraphicsMaterial::clearUniformData()
{
	for (std::map<int32, std::pair<UniformType, void*> >::const_iterator it = m_uniformData.cbegin(); it != m_uniformData.cend(); ++it)
		delete it->second.second;
	m_uniformData.clear();
}

} // namespace

} // namespace