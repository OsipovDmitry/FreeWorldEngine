#include <renderer/IGPUTexture.h>

#include "GraphicsTexture.h"


namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsTexture::GraphicsTexture(const std::string& name, Renderer::IGPUTexture *pTexture) :
	m_name(name),
	m_pTexture(pTexture)
{
}

GraphicsTexture::~GraphicsTexture()
{
}

std::string GraphicsTexture::name() const
{
	return m_name;
}

Renderer::IGPUTexture *GraphicsTexture::texture() const
{
	return m_pTexture;
}

void GraphicsTexture::setWrapMode(const WrapMode wrapMode)
{
	Renderer::IGPUTexture::IGPUTextureWrap wrap = (Renderer::IGPUTexture::IGPUTextureWrap)wrapMode;
	m_pTexture->setWrapS(wrap);
	m_pTexture->setWrapT(wrap);
	m_pTexture->setWrapR(wrap);
}

void GraphicsTexture::setFilterMode(const FilterMode filterMode)
{
	switch (filterMode) {
	case FilterMode_Off: {
		m_pTexture->setMagFilter(Renderer::IGPUTexture::IGPUTextureMagFilter_Nearest);
		m_pTexture->setMinFilter(Renderer::IGPUTexture::IGPUTextureMinFilter_Nearest);
		break;
	}
	case FilterMode_BiLinear: {
		m_pTexture->setMagFilter(Renderer::IGPUTexture::IGPUTextureMagFilter_Linear);
		m_pTexture->setMinFilter(Renderer::IGPUTexture::IGPUTextureMinFilter_Linear);
		break;
	}
	case FilterMode_TriLinear: {
		m_pTexture->setMagFilter(Renderer::IGPUTexture::IGPUTextureMagFilter_Linear);
		m_pTexture->setMinFilter(Renderer::IGPUTexture::IGPUTextureMinFilter_LinearMipmapLinear);
		break;
	}
	}
}

} // namespace
} // namespace