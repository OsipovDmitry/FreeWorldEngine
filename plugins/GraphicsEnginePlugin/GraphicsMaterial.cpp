#include <renderer/IGPURenderer.h>
#include <graphics_engine/IGraphicsWindow.h>
#include <graphics_engine/IGraphicsCamera.h>


#include "GraphicsEngine.h"
#include "GraphicsMaterial.h"

namespace FreeWorldEngine {

namespace {
	const int32 g_uniformSize[] = {
		sizeof(float),     sizeof(int32),      sizeof(uint32),
		sizeof(glm::vec2), sizeof(glm::ivec2), sizeof(glm::uvec2),
		sizeof(glm::vec3), sizeof(glm::ivec3), sizeof(glm::uvec3), 
		sizeof(glm::vec4), sizeof(glm::ivec4), sizeof(glm::uvec4),
		sizeof(glm::mat2), sizeof(glm::mat3),  sizeof(glm::mat4)
	};
}

namespace GraphicsEngine {

GraphicsMaterial::GraphicsMaterial(const std::string& name, Renderer::IGPUProgram *pProgram) :
	m_name(name),
	m_pProgram(pProgram),
	m_uniformData(),
	m_autoUniformData(),
	m_textureSlotGenerator(0),
	m_uniformTextures(),
	m_uboBindingPointGenerator(0),
	m_uniformBuffers()
{
}

GraphicsMaterial::~GraphicsMaterial()
{
	clearUniforms();
}

std::string GraphicsMaterial::name() const
{
	return m_name;
}

Renderer::IGPUProgram *GraphicsMaterial::program() const
{
	return m_pProgram;
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
	int32 slotId = setUniformTexture(pTexture);
	if (slotId >= 0)
		setUniform(location, slotId);
}

void GraphicsMaterial::setUniform(const int32 index, Renderer::IGPUBuffer *pBuffer)
{
	int32 bindingPoint = setUniformBuffer(pBuffer);
	if (bindingPoint >= 0)
		setUniform(index, bindingPoint);
}

void GraphicsMaterial::setAutoUniform(const int32 location, const AutoUniform value)
{
	m_autoUniformData.insert(std::make_pair(value, location));
}

void GraphicsMaterial::bind(IGraphicsCamera *pCamera, const glm::mat4x4& modelMatrix) const
{
	for (auto it = m_uniformData.cbegin(); it != m_uniformData.cend(); ++it) {
		int32 location = it->first;
		UniformData data = it->second;

		switch (data.type) {
		case UniformType_Vec1f: { m_pProgram->setUniform(location, *(const float*)(data.pData)); break; }
		case UniformType_Vec1i: { m_pProgram->setUniform(location, *(const int32*)(data.pData)); break; }
		case UniformType_Vec1ui: { m_pProgram->setUniform(location, *(const uint32*)(data.pData)); break; }
		case UniformType_Vec2f: { m_pProgram->setUniform(location, *(const glm::vec2*)(data.pData)); break; }
		case UniformType_Vec2i: { m_pProgram->setUniform(location, *(const glm::ivec2*)(data.pData)); break; }
		case UniformType_Vec2ui: { m_pProgram->setUniform(location, *(const glm::uvec2*)(data.pData)); break; }
		case UniformType_Vec3f: { m_pProgram->setUniform(location, *(const glm::vec3*)(data.pData)); break; }
		case UniformType_Vec3i: { m_pProgram->setUniform(location, *(const glm::ivec3*)(data.pData)); break; }
		case UniformType_Vec3ui: { m_pProgram->setUniform(location, *(const glm::uvec3*)(data.pData)); break; }
		case UniformType_Vec4f: { m_pProgram->setUniform(location, *(const glm::vec4*)(data.pData)); break; }
		case UniformType_Vec4i: { m_pProgram->setUniform(location, *(const glm::ivec4*)(data.pData)); break; }
		case UniformType_Vec4ui: { m_pProgram->setUniform(location, *(const glm::uvec4*)(data.pData)); break; }
		case UniformType_Mat2f: { m_pProgram->setUniform(location, *(const glm::mat2*)(data.pData)); break; }
		case UniformType_Mat3f: { m_pProgram->setUniform(location, *(const glm::mat3*)(data.pData)); break; }
		case UniformType_Mat4f: { m_pProgram->setUniform(location, *(const glm::mat4*)(data.pData)); break; }
		default: break;
		}
	}

	for (auto it = m_autoUniformData.cbegin(); it != m_autoUniformData.cend(); ++it) {
		AutoUniform value = it->first;
		int32 location = it->second;

		switch (value) {
		case AutoUniform_ModelMatrix: { m_pProgram->setUniform(location, modelMatrix); break; }
		case AutoUniform_ViewMatrix: { m_pProgram->setUniform(location, pCamera->viewMatrix()); break; }
		case AutoUniform_ModelViewMatrix: { m_pProgram->setUniform(location, pCamera->viewMatrix()*modelMatrix); break; }
		case AutoUniform_ProjectionMatrix: { m_pProgram->setUniform(location, pCamera->projectionMatrix()); break; }
		case AutoUniform_ViewProjectionMatrix: { m_pProgram->setUniform(location, pCamera->viewProjectionMatrix()); break; }
		case AutoUniform_ModelViewProjectionMatrix: { m_pProgram->setUniform(location, pCamera->viewProjectionMatrix()*modelMatrix); break; }
		default: break;
		}
	}

	for (auto it = m_uniformTextures.cbegin(); it != m_uniformTextures.cend(); ++it) {
		pGPURenderer->setTexture(it->first, it->second);
	}

	for (auto it = m_uniformBuffers.cbegin(); it != m_uniformBuffers.cend(); ++it) {
		pGPURenderer->setUniformBuffer(it->first, it->second);
	}
}

/*bool GraphicsMaterial::operator <(const GraphicsMaterial& other) const
{
	return m_pProgram < other.m_pProgram;
}*/

void GraphicsMaterial::setUniformData(const int32 location, const UniformType type, void *pData)
{
	std::map<int32, UniformData>::const_iterator it = m_uniformData.find(location);
	if (it != m_uniformData.cend())
		delete it->second.pData;

	m_uniformData[location] = UniformData(pData, type);
}

int32 GraphicsMaterial::setUniformTexture(Renderer::IGPUTexture *pTexture)
{
	if (m_uniformTextures.count(pTexture))
		return -1;

	int32 slotId = m_textureSlotGenerator.generateId();
	m_uniformTextures[pTexture] = slotId;
	return slotId;
}

int32 GraphicsMaterial::setUniformBuffer(Renderer::IGPUBuffer * pBuffer)
{
	if (m_uniformBuffers.count(pBuffer))
		return -1;

	int32 bindingPoint = m_uboBindingPointGenerator.generateId();
	m_uniformBuffers[pBuffer] = bindingPoint;
	return bindingPoint;
}

void GraphicsMaterial::clearUniforms()
{
	for (auto it = m_uniformData.cbegin(); it != m_uniformData.cend(); ++it)
		delete it->second.pData;
	m_uniformData.clear();
	m_autoUniformData.clear();

	m_uniformTextures.clear();

	m_uniformBuffers.clear();
}

} // namespace

} // namespace