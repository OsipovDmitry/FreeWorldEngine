#include "GraphicsModel.h"
#include "GraphicsEngine.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsModel::GraphicsModel(const std::string& name) :
	m_name(name),
	m_pMaterial(pGraphicsEngine->findMaterial("StandardMaterial"))
{
}

GraphicsModel::~GraphicsModel()
{
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

void GraphicsModel::setMesh(Mesh * pMesh)
{
}

} // namespace

} // namespace