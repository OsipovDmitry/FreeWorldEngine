#include <renderer/IGPURenderer.h>

#include <math/MeshWrapper.h>
#include <math/MeshPainter.h>

#include "GraphicsModel.h"
#include "GraphicsEngine.h"
#include "GraphicsMaterialManager.h"
#include "GPUMesh.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsModel::GraphicsModel(const std::string& name) :
	m_name(name),
	m_boundSphere(),
	m_aabb(),
	m_pMaterial(pGraphicsEngine->materialManager()->findMaterial("StandardMaterial")),
	m_pGPUMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Triangles)),
	m_pBoudSphereGPUMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines)),
	m_pAabbGPUMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Triangles))
{
}

GraphicsModel::~GraphicsModel()
{
	m_pGPUMesh->destroy();
	delete m_pGPUMesh;

	m_pBoudSphereGPUMesh->destroy();
	delete m_pBoudSphereGPUMesh;

	m_pAabbGPUMesh->destroy();
	delete m_pAabbGPUMesh;
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
	Math::MeshWrapper(pMesh).computeAabbAndBoundingSphere(m_aabb, m_boundSphere);
	
	Mesh sphereMesh;
	Math::MeshWrapper sphereWrapper(&sphereMesh);
	sphereWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	sphereWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	sphereWrapper.setVertexStride(3);
	Math::MeshPainter(sphereWrapper).paintSphere(m_boundSphere.w, 8);
	sphereWrapper.translateMesh((float*)&(m_boundSphere.x));

	Mesh aabbMesh;
	Math::MeshWrapper aabbWrapper(&aabbMesh);
	aabbWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	aabbWrapper.setPrimitiveFormat(PrimitiveFormat_Triangles);
	aabbWrapper.setVertexStride(3);
	glm::vec3 boxPos = 0.5f*(m_aabb.vMin + m_aabb.vMax), boxSize = m_aabb.vMax - m_aabb.vMin;
	Math::MeshPainter(aabbWrapper).paintBox(boxSize.x, boxSize.y, boxSize.z);
	aabbWrapper.translateMesh((float*)&(boxPos.x));

	m_pGPUMesh->setMesh(pMesh);
	m_pBoudSphereGPUMesh->setMesh(&sphereMesh);
	m_pAabbGPUMesh->setMesh(&aabbMesh);
}

GPUMesh *GraphicsModel::gpuMesh() const
{
	return m_pGPUMesh;
}

GPUMesh *GraphicsModel::gpuMeshBoundSphere() const
{
	return m_pBoudSphereGPUMesh;
}

GPUMesh * GraphicsModel::gpuMeshAabb() const
{
	return m_pAabbGPUMesh;
}

const Math::Sphere &GraphicsModel::boundingSphere() const
{
	return m_boundSphere;
}

const Math::Aabb & GraphicsModel::aabb() const
{
	return m_aabb;
}

} // namespace

} // namespace