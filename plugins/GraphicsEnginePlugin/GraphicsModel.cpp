#include <renderer/IGPURenderer.h>
#include <renderer/IGPUBuffer.h>
#include <renderer/IGPUBufferContainer.h>

#include "GraphicsModel.h"
#include "GraphicsEngine.h"
#include "GraphicsMaterialManager.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsModel::GraphicsModel(const std::string& name) :
	m_name(name),
	m_pMaterial(pGraphicsEngine->materialManager()->findMaterial("StandardMaterial")),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pVertexArray(nullptr),
	m_numIndices(0),
	m_primitiveFormat(PrimitiveFormat_Triangles)
{
}

GraphicsModel::~GraphicsModel()
{
	if (m_pVertexArray)
		pGPURenderer->destroyBufferContainer(m_pVertexArray);
	if (m_pVertexBuffer)
		pGPURenderer->destroyBuffer(m_pVertexBuffer);
	if (m_pIndexBuffer)
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
	const uint64 verticesSize = pMesh ? pMesh->vertexStride * pMesh->numVertices * sizeof(float) : 0;
	const uint64 indicesSize = pMesh ? pMesh->numIndices * sizeof(uint32) : 0;

	if (!m_pVertexBuffer)
		m_pVertexBuffer = pGPURenderer->createBuffer(verticesSize, Renderer::IGPUBuffer::IGPUBufferUsage_StreamDraw, pMesh->pVertexData);
	else {
		m_pVertexBuffer->resize(verticesSize);
		void *pVerties = m_pVertexBuffer->map(Renderer::IGPUBuffer::IGPUBufferAccess_WriteOnly);
		if (pMesh)
			memcpy(pVerties, pMesh->pVertexData, verticesSize);
		m_pVertexBuffer->unmap();
	}

	if (!m_pIndexBuffer)
		m_pIndexBuffer = pGPURenderer->createBuffer(indicesSize, Renderer::IGPUBuffer::IGPUBufferUsage_StreamDraw, pMesh->pIndexData);
	else {
		m_pIndexBuffer->resize(indicesSize);
		void *pIndices = m_pIndexBuffer->map(Renderer::IGPUBuffer::IGPUBufferAccess_WriteOnly);
		if (pMesh)
			memcpy(pIndices, pMesh->pIndexData, indicesSize);
		m_pIndexBuffer->unmap();
	}

	if (m_pVertexArray)
		pGPURenderer->destroyBufferContainer(m_pVertexArray);
	m_pVertexArray = pGPURenderer->createBufferContainer();

	if (pMesh) {
		for (auto it : pMesh->attributes) {
			m_pVertexArray->setVertexAttribute(m_pVertexBuffer, it.first, it.second.first, it.second.second * sizeof(float), pMesh->vertexStride * sizeof(float), TYPE_FLOAT);
			m_pVertexArray->enableVertexAttribute(it.first);
		}
		m_pVertexArray->setIndexBuffer(m_pIndexBuffer);
		m_primitiveFormat = pMesh->primitiveFormat;
		m_numIndices = pMesh->numIndices;
	}
	else {
		m_primitiveFormat = PrimitiveFormat_Triangles;
		m_numIndices = 0;
	}
}

Renderer::IGPUBufferContainer *GraphicsModel::bufferContainer() const
{
	return m_pVertexArray;
}

uint32 GraphicsModel::numIndices() const
{
	return m_numIndices;
}

PrimitiveFormat GraphicsModel::primitiveFormat() const
{
	return m_primitiveFormat;
}

} // namespace

} // namespace