#include <renderer/IGPURenderer.h>
#include <renderer/IGPUBuffer.h>
#include <renderer/IGPUBufferContainer.h>

#include "GraphicsModel.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsModel::GraphicsModel(const std::string& name) :
	m_name(name),
	m_pMaterial(pGraphicsEngine->findMaterial("StandardMaterial")),
	m_pVertexBuffer(pGPURenderer->createBuffer(0)),
	m_pIndexBuffer(pGPURenderer->createBuffer(0)),
	m_pRenderData(new RenderData(pGPURenderer->createBufferContainer(), 0, PrimitiveFormat_Triangles))
{
	m_pRenderData->pBufferContainer->setIndexBuffer(m_pIndexBuffer);
}

GraphicsModel::~GraphicsModel()
{
	pGPURenderer->destroyBufferContainer(m_pRenderData->pBufferContainer);
	delete m_pRenderData;
	pGPURenderer->destroyBuffer(m_pVertexBuffer);
	pGPURenderer->destroyBuffer(m_pIndexBuffer);
}

std::string GraphicsModel::name() const
{
	return m_name;
}

IGraphicsMaterial *GraphicsModel::material() const
{
	return m_pMaterial;
}

void GraphicsModel::setMaterial(IGraphicsMaterial *pMaterial)
{
	m_pMaterial = pMaterial;
}

void GraphicsModel::setMesh(Mesh *pMesh)
{
	const uint64 verticesSize = pMesh->vertexStride * pMesh->numVertices * sizeof(float);
	m_pVertexBuffer->resize(verticesSize);
	void *pVerties = m_pVertexBuffer->map(Renderer::IGPUBuffer::IGPUBufferAccess_WriteOnly);
	if (pMesh)
		memcpy(pVerties, pMesh->pVertexData, verticesSize);
	m_pVertexBuffer->unmap();

	const uint64 indicesSize = pMesh->numIndices * sizeof(uint32);
	m_pIndexBuffer->resize(indicesSize);
	void *pIndices = m_pIndexBuffer->map(Renderer::IGPUBuffer::IGPUBufferAccess_WriteOnly);
	if (pMesh)
		memcpy(pIndices, pMesh->pIndexData, indicesSize);
	m_pIndexBuffer->unmap();

	if (pMesh) {
		for (auto it : pMesh->attributes) {
			m_pRenderData->pBufferContainer->setVertexAttribute(m_pVertexBuffer, it.first, it.second.first, it.second.second * sizeof(float), pMesh->vertexStride * sizeof(float), TYPE_FLOAT);
			m_pRenderData->pBufferContainer->enableVertexAttribute(it.first);
		}
		m_pRenderData->pBufferContainer->setIndexBuffer(m_pIndexBuffer);
		m_pRenderData->primitiveFormat = pMesh->primitiveFormat;
		m_pRenderData->numIndices = pMesh->numIndices;
	}
	else {
		m_pRenderData->primitiveFormat = PrimitiveFormat_Triangles;
		m_pRenderData->numIndices = 0;
	}
}

GraphicsModel::RenderData *GraphicsModel::renderData() const
{
	return m_pRenderData;
}

} // namespace

} // namespace