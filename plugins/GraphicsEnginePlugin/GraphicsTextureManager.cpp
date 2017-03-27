#include <FreeWorldEngine.h>
#include <utility/AutoNameGenerator.h>
#include <utility/File.h>

#include "GraphicsTextureManager.h"
#include "GraphicsTexture.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsTextureManager::GraphicsTextureManager() :
	m_pResoureManager(getCoreEngine()->createResourceManager("ResourceManagerForGraphicstextures")),
	m_pTextureNameGenerator(new Utility::AutoNameGenerator("TextureName"))
{
}

GraphicsTextureManager::~GraphicsTextureManager()
{
	delete m_pTextureNameGenerator;
	getCoreEngine()->destroyResourceManager(m_pResoureManager);
}
	
IGraphicsTexture *GraphicsTextureManager::createTexture(Renderer::IGPUTexture *pTexture, const std::string& name, std::string *pLogString)
{
	if (!pTexture) {
		if (pLogString)
			*pLogString += "Texture is NULL.";
		return nullptr;
	}

	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	pGraphicsTexture = new GraphicsTexture(resName, pTexture);
	m_pResoureManager->addResource(pGraphicsTexture);
	return pGraphicsTexture;
}

IGraphicsTexture *GraphicsTextureManager::createTexture1D(const Raster *pRaster, const std::string& name, std::string *pLogString)
{
	if (!pRaster) {
		if (pLogString)
			*pLogString += "Raster is NULL.";
		return nullptr;
	}

	Renderer::TextureFormat textureFormat;
	if (!internalTextureFormat(pRaster->numComponents, pRaster->type, textureFormat)) {
		if (pLogString)
			*pLogString += "Could not convert the texture internal format.";
		return nullptr;
	}

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_1D, pRaster->size, textureFormat);
	pTexture->setSubData(nullptr, pRaster->size, textureFormat.channelsCount, pRaster->type, pRaster->pData);

	return createTexture(pTexture, name, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::createTexture2D(const Raster *pRaster, const std::string& name, std::string *pLogString)
{
	if (!pRaster) {
		if (pLogString)
			*pLogString += "Raster is NULL.";
		return nullptr;
	}

	Renderer::TextureFormat textureFormat;
	if (!internalTextureFormat(pRaster->numComponents, pRaster->type, textureFormat)) {
		if (pLogString)
			*pLogString += "Could not convert the texture internal format.";
		return nullptr;
	}

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_2D, pRaster->size, textureFormat);
	pTexture->setSubData(nullptr, pRaster->size, textureFormat.channelsCount, pRaster->type, pRaster->pData);

	return createTexture(pTexture, name, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::findTexture(const std::string& name) const
{
	return static_cast<IGraphicsTexture*>(m_pResoureManager->findResource(name));
}

void GraphicsTextureManager::destroyTexture(const std::string& name)
{
	m_pResoureManager->destroyResource(name);
}

void GraphicsTextureManager::destroyTexture(IGraphicsTexture *pTexture)
{
	m_pResoureManager->destroyResource(pTexture->name());
}

bool GraphicsTextureManager::internalTextureFormat(const uint32 channelsCount, const Type type, Renderer::TextureFormat& result)
{
	switch (channelsCount) {
	case 1: { result.channelsCount = Renderer::TextureFormat::ChannelsCount_1; break; }
	case 2: { result.channelsCount = Renderer::TextureFormat::ChannelsCount_2; break; }
	case 3: { result.channelsCount = Renderer::TextureFormat::ChannelsCount_3; break; }
	case 4: { result.channelsCount = Renderer::TextureFormat::ChannelsCount_4; break; }
	default: return false;
	}

	switch (type) {
	case TYPE_UNSIGNED_INT_8: {
		result.channelSize = Renderer::TextureFormat::ChannelSize_8;
		result.pixelFormat = Renderer::TextureFormat::PixelFormat_NormalizeUnsigned;
		break;
	}
	case TYPE_UNSIGNED_INT_16: {
		result.channelSize = Renderer::TextureFormat::ChannelSize_16;
		result.pixelFormat = Renderer::TextureFormat::PixelFormat_NormalizeUnsigned;
		break;
	}
	case TYPE_UNSIGNED_INT_32: {
		result.channelSize = Renderer::TextureFormat::ChannelSize_32;
		result.pixelFormat = Renderer::TextureFormat::PixelFormat_NormalizeUnsigned;
		break;
	}
	case TYPE_FLOAT: {
		result.channelSize = Renderer::TextureFormat::ChannelSize_32;
		result.pixelFormat = Renderer::TextureFormat::PixelFormat_UnnormalizeFloat;
		break;
	}
	default: return false;
	}

	return true;
}

} // namespace
} // namespace