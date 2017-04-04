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
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	if (!pTexture) {
		if (pLogString)
			*pLogString += "Texture is NULL.";
		return nullptr;
	}

	pGraphicsTexture = new GraphicsTexture(resName, pTexture);
	m_pResoureManager->addResource(pGraphicsTexture);
	return pGraphicsTexture;
}

IGraphicsTexture *GraphicsTextureManager::createTexture1D(const Raster *pRaster, const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

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
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::createTexture2D(const Raster *pRaster, const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

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
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture * GraphicsTextureManager::createTexture3D(const Raster *pRaster, const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

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

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_3D, pRaster->size, textureFormat);
	pTexture->setSubData(nullptr, pRaster->size, textureFormat.channelsCount, pRaster->type, pRaster->pData);
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::createTextureCubeMap(const Raster *pRasters[], const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	for (int32 i = 0; i < 6; ++i)
		if (!pRasters[i]) {
			if (pLogString)
				*pLogString += "Raster is NULL.";
			return nullptr;
		}

	for (int32 i = 1; i < 6; ++i)
		if (pRasters[i]->numComponents != pRasters[0]->numComponents ||
			pRasters[i]->type != pRasters[0]->type ||
			pRasters[0]->size[0] != pRasters[0]->size[1] || // тут проверка, что растр квадратный
			pRasters[i]->size[0] != pRasters[0]->size[0] ||
			pRasters[i]->size[1] != pRasters[0]->size[1] ||
			pRasters[i]->size[2] != pRasters[0]->size[2]) {
			if (pLogString)
				*pLogString += "Parameters of rasters do not match.";
			return nullptr;
		}

	Renderer::TextureFormat textureFormat;
	if (!internalTextureFormat(pRasters[0]->numComponents, pRasters[0]->type, textureFormat)) {
		if (pLogString)
			*pLogString += "Could not convert the texture internal format.";
		return nullptr;
	}

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_CubeMap, pRasters[0]->size, textureFormat);
	uint32 offs[3] = { 0, 0, 0 };
	for (int32 i = 0; i < 6; ++i) {
		offs[2] = i;
		pTexture->setSubData(offs, pRasters[i]->size, textureFormat.channelsCount, pRasters[i]->type, pRasters[i]->pData);
	}
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::createTexture1DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	for (int32 i = 0; i < numRasters; ++i)
		if (!pRasters[i]) {
			if (pLogString)
				*pLogString += "Raster is NULL.";
			return nullptr;
		}

	for (int32 i = 1; i < numRasters; ++i)
		if (pRasters[i]->numComponents != pRasters[0]->numComponents ||
			pRasters[i]->type != pRasters[0]->type ||
			pRasters[i]->size[0] != pRasters[0]->size[0] ||
			pRasters[i]->size[1] != pRasters[0]->size[1] ||
			pRasters[i]->size[2] != pRasters[0]->size[2]) {
			if (pLogString)
				*pLogString += "Parameters of rasters do not match.";
			return nullptr;
		}

	Renderer::TextureFormat textureFormat;
	if (!internalTextureFormat(pRasters[0]->numComponents, pRasters[0]->type, textureFormat)) {
		if (pLogString)
			*pLogString += "Could not convert the texture internal format.";
		return nullptr;
	}

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_1DArray, pRasters[0]->size, textureFormat);
	uint32 offs[2] = { 0, 0 };
	for (int32 i = 0; i < numRasters; ++i) {
		offs[1] = i;
		pTexture->setSubData(offs, pRasters[i]->size, textureFormat.channelsCount, pRasters[i]->type, pRasters[i]->pData);
	}
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture *GraphicsTextureManager::createTexture2DArray(const Raster *pRasters[], const uint32 numRasters, const bool generateMipmaps, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	for (int32 i = 0; i < numRasters; ++i)
		if (!pRasters[i]) {
			if (pLogString)
				*pLogString += "Raster is NULL.";
			return nullptr;
		}

	for (int32 i = 1; i < numRasters; ++i)
		if (pRasters[i]->numComponents != pRasters[0]->numComponents ||
			pRasters[i]->type != pRasters[0]->type ||
			pRasters[i]->size[0] != pRasters[0]->size[0] ||
			pRasters[i]->size[1] != pRasters[0]->size[1] ||
			pRasters[i]->size[2] != pRasters[0]->size[2]) {
			if (pLogString)
				*pLogString += "Parameters of rasters do not match.";
			return nullptr;
		}

	Renderer::TextureFormat textureFormat;
	if (!internalTextureFormat(pRasters[0]->numComponents, pRasters[0]->type, textureFormat)) {
		if (pLogString)
			*pLogString += "Could not convert the texture internal format.";
		return nullptr;
	}

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_2DArray, pRasters[0]->size, textureFormat);
	uint32 offs[3] = { 0, 0, 0 };
	for (int32 i = 0; i < numRasters; ++i) {
		offs[2] = i;
		pTexture->setSubData(offs, pRasters[i]->size, textureFormat.channelsCount, pRasters[i]->type, pRasters[i]->pData);
	}
	if (generateMipmaps)
		pTexture->generateMipMaps();

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture * GraphicsTextureManager::createTextureRectangle(const Raster * pRaster, const std::string & name, std::string * pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

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

	Renderer::IGPUTexture *pTexture = pGPURenderer->createTexture(Renderer::IGPUTexture::IGPUTextureType_Rectangle, pRaster->size, textureFormat);
	pTexture->setSubData(nullptr, pRaster->size, textureFormat.channelsCount, pRaster->type, pRaster->pData);

	return createTexture(pTexture, resName, pLogString);
}

IGraphicsTexture * GraphicsTextureManager::createTextureBuffer(Renderer::IGPUBuffer *pBuffer, const std::string& name, std::string *pLogString)
{
	const std::string resName = (name == "@utoname") ? m_pTextureNameGenerator->generateName() : name;
	IGraphicsTexture *pGraphicsTexture = findTexture(resName);
	if (pGraphicsTexture)
		return pGraphicsTexture;

	if (!pBuffer) {
		if (pLogString)
			*pLogString += "Buffer is NULL.";
		return nullptr;
	}

	return nullptr;
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