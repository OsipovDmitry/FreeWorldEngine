#include <3rdparty/glm/gtc/matrix_inverse.hpp>

#include <renderer/IGPURenderer.h>
#include <graphics_engine/IGraphicsWindow.h>
#include <graphics_engine/IGraphicsCamera.h>

#include "GraphicsEngine.h"
#include "GraphicsMaterial.h"
#include "GraphicsLight.h"
#include "AbstractSceneOptimizer.h"

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
	m_uniformBuffers(),
	m_depthCheck(true),
	m_depthWrite(true),
	m_depthFunc(DepthTestFunc_Less),
	m_blendEquationRGB(BlendEquation_Add),
	m_blendEquationA(BlendEquation_Add),
	m_blendSrcRGB(BlendFunc_One),
	m_blendSrcA(BlendFunc_One),
	m_blendDstRGB(BlendFunc_Zero),
	m_blendDstA(BlendFunc_Zero),
	m_cullFaceState(CullFaceState_RenderFrontFaces),
	m_tag(Tag_Solid)
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

void GraphicsMaterial::setDepthCheck(const bool state)
{
	m_depthCheck = state;
}

bool GraphicsMaterial::depthCheck() const
{
	return m_depthCheck;
}

void GraphicsMaterial::setDepthWrite(const bool state)
{
	m_depthWrite = state;
}

bool GraphicsMaterial::depthWrite() const
{
	return m_depthWrite;
}

void GraphicsMaterial::setDepthFunc(const DepthTestFunc func)
{
	m_depthFunc = func;
}

IGraphicsMaterial::DepthTestFunc GraphicsMaterial::depthFunc() const
{
	return m_depthFunc;
}

void GraphicsMaterial::setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA)
{
	m_blendEquationRGB = funcRGB;
	m_blendEquationA = funcA;
}

void GraphicsMaterial::setBlendEquation(const BlendEquation equation)
{
	m_blendEquationRGB = m_blendEquationA = equation;
}

IGraphicsMaterial::BlendEquation GraphicsMaterial::blendEquationRGB() const
{
	return m_blendEquationRGB;
}

IGraphicsMaterial::BlendEquation GraphicsMaterial::blendEquationA() const
{
	return m_blendEquationA;
}

void GraphicsMaterial::setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA)
{
	m_blendSrcRGB = funcSrcRGB;
	m_blendSrcA = funcSrcA;
	m_blendDstRGB = funcDstRGB;
	m_blendDstA = funcDstA;
}

void GraphicsMaterial::setBlendFunc(const BlendFuncState func)
{
	switch (func) {
	case BlendFuncState_Replace: {
		m_blendSrcRGB = m_blendSrcA = BlendFunc_One;
		m_blendDstRGB = m_blendDstA = BlendFunc_Zero;
		break;
	}
	case BlendFuncState_Alpha: {
		m_blendSrcRGB = m_blendSrcA = BlendFunc_SrcAlpha;
		m_blendDstRGB = m_blendDstA = BlendFunc_InvSrcAlpha;
		break;
	}
	case BlendFuncState_Add: {
		m_blendSrcRGB = m_blendSrcA = BlendFunc_SrcAlpha;
		m_blendDstRGB = m_blendDstA = BlendFunc_One;
		break;
	}
	default: break;
	}
}

IGraphicsMaterial::BlendFunc GraphicsMaterial::blendFuncSrcRGB() const
{
	return m_blendSrcRGB;
}

IGraphicsMaterial::BlendFunc GraphicsMaterial::blendFuncSrcA() const
{
	return m_blendSrcA;
}

IGraphicsMaterial::BlendFunc GraphicsMaterial::blendFuncDstRGB() const
{
	return m_blendDstRGB;
}

IGraphicsMaterial::BlendFunc GraphicsMaterial::blendFuncDstA() const
{
	return m_blendDstA;
}

void GraphicsMaterial::setCullFaceState(const CullFaceState state)
{
	m_cullFaceState = state;
}

IGraphicsMaterial::CullFaceState GraphicsMaterial::cullFaceState() const
{
	return m_cullFaceState;
}

void GraphicsMaterial::setTag(const Tag tag)
{
	m_tag = tag;
}

IGraphicsMaterial::Tag GraphicsMaterial::tag() const
{
	return m_tag;
}

void GraphicsMaterial::bind(IGraphicsCamera *pCamera, const ModelRenderData& modelData) const
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
		case AutoUniform_NormalMatrix: { m_pProgram->setUniform(location, glm::inverseTranspose(glm::mat3x3(modelData.modelMatrix))); break; }
		case AutoUniform_ModelMatrix: { m_pProgram->setUniform(location, modelData.modelMatrix); break; }
		case AutoUniform_ViewMatrix: { m_pProgram->setUniform(location, pCamera->viewMatrix()); break; }
		case AutoUniform_ModelViewMatrix: { m_pProgram->setUniform(location, pCamera->viewMatrix()*modelData.modelMatrix); break; }
		case AutoUniform_ProjectionMatrix: { m_pProgram->setUniform(location, pCamera->projectionMatrix()); break; }
		case AutoUniform_ViewProjectionMatrix: { m_pProgram->setUniform(location, pCamera->projectionMatrix()*pCamera->viewMatrix()); break; }
		case AutoUniform_ModelViewProjectionMatrix: { m_pProgram->setUniform(location, pCamera->projectionMatrix()*pCamera->viewMatrix()*modelData.modelMatrix); break; }
		case AutoUniform_CameraPosition: { m_pProgram->setUniform(location, pCamera->position()); break; }
		case AutoUniform_ActiveLightsCount: { m_pProgram->setUniform(location, (int32)modelData.lights.size()); break; }
		case AutoUniform_Light0: { 
				m_pProgram->setUniform(location, (modelData.lights.size() >= 1) ?
					modelData.lights[0]->packedData() :
					GraphicsLight::noneLightPackedData());
				break;
			}
		case AutoUniform_Light1: {
				m_pProgram->setUniform(location, (modelData.lights.size() >= 2) ?
					modelData.lights[1]->packedData() :
					GraphicsLight::noneLightPackedData());
				break;
			}
		case AutoUniform_Light2: {
				m_pProgram->setUniform(location, (modelData.lights.size() >= 3) ?
					modelData.lights[2]->packedData() :
					GraphicsLight::noneLightPackedData());
				break;
			}
		case AutoUniform_Light3: {
				m_pProgram->setUniform(location, (modelData.lights.size() >= 4) ?
					modelData.lights[3]->packedData() :
					GraphicsLight::noneLightPackedData());
				break;
			}
		default: break;
		}
	}

	for (auto it = m_uniformTextures.cbegin(); it != m_uniformTextures.cend(); ++it) {
		pGPURenderer->setTexture(it->first, it->second);
	}

	for (auto it = m_uniformBuffers.cbegin(); it != m_uniformBuffers.cend(); ++it) {
		pGPURenderer->setUniformBuffer(it->first, it->second);
	}

	pGPURenderer->setDepthWriteMask(m_depthWrite);
	if ((!m_depthCheck || m_depthFunc == DepthTestFunc_Never) && !m_depthWrite)
		pGPURenderer->disableDepthTest();
	else {
		Renderer::IGPURenderer::DepthTestFunc func = (Renderer::IGPURenderer::DepthTestFunc)m_depthFunc;
		if (!m_depthCheck)
			func = Renderer::IGPURenderer::DepthTestFunc_Always;
		pGPURenderer->enableDepthTest(func);
	}

	if (!needBlend()) {
		pGPURenderer->disableBlend();
	}
	else {
		pGPURenderer->enableBlend();
		pGPURenderer->setBlendEquation((Renderer::IGPURenderer::BlendEquation)m_blendEquationRGB, (Renderer::IGPURenderer::BlendEquation)m_blendEquationA);
		pGPURenderer->setBlendFunc((Renderer::IGPURenderer::BlendFunc)m_blendSrcRGB,
			(Renderer::IGPURenderer::BlendFunc)m_blendDstRGB,
			(Renderer::IGPURenderer::BlendFunc)m_blendSrcA,
			(Renderer::IGPURenderer::BlendFunc)m_blendDstA);
	}

	pGPURenderer->setCullFaceState((Renderer::IGPURenderer::CullFaceState)m_cullFaceState);
}

bool GraphicsMaterial::needBlend() const
{
	return !((m_blendSrcRGB == BlendFunc_One) && (m_blendSrcA == BlendFunc_One) &&
		(m_blendDstRGB == BlendFunc_Zero) && (m_blendDstA == BlendFunc_Zero) &&
		(m_blendEquationRGB == BlendEquation_Add) && (m_blendEquationA == BlendEquation_Add));
}

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

bool GraphicsMaterial::Comparator::operator()(GraphicsMaterial *p1, GraphicsMaterial *p2) const
{
	Tag tag1 = p1->tag();
	Tag tag2 = p2->tag();

	if (tag1 != tag2)
		return tag1 < tag2;

	return p1->m_pProgram < p2->m_pProgram;
}

} // namespace

} // namespace
