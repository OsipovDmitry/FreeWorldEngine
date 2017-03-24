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
	m_pMaterial(pGraphicsEngine->materialManager()->findMaterial("StandardMaterial")),
	m_pGPUMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Triangles)),
	m_pBoudSphereGPUMesh(new GPUMesh(nullptr, nullptr, nullptr, 0, PrimitiveFormat_Lines))
{
}

GraphicsModel::~GraphicsModel()
{
	m_pGPUMesh->destroy();
	delete m_pGPUMesh;

	m_pBoudSphereGPUMesh->destroy();
	delete m_pBoudSphereGPUMesh;
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
	
	m_boundSphere = Math::MeshWrapper(pMesh).computeBoundingSphere();
	Mesh sphereMesh;
	Math::MeshWrapper sphereWrapper(&sphereMesh);
	sphereWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	sphereWrapper.setPrimitiveFormat(PrimitiveFormat_Lines);
	sphereWrapper.setVertexStride(3);
	Math::MeshPainter(sphereWrapper).paintSphere(m_boundSphere.w, 8);
	sphereWrapper.translateMesh((float*)&(m_boundSphere.x));

	m_pGPUMesh->setMesh(pMesh);
	m_pBoudSphereGPUMesh->setMesh(&sphereMesh);
}

GPUMesh *GraphicsModel::gpuMesh() const
{
	return m_pGPUMesh;
}

GPUMesh *GraphicsModel::gpuMeshBoundSphere() const
{
	return m_pBoudSphereGPUMesh;
}

const Math::Sphere &GraphicsModel::boundingSphere() const
{
	return m_boundSphere;
}

} // namespace

} // namespace