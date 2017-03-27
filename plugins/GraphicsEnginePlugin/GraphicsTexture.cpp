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


} // namespace
} // namespace