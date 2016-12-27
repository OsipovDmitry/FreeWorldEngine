#include "GraphicsMaterial.h"

namespace FreeWorldEngine {

namespace GraphicsEngine {

GraphicsMaterial::GraphicsMaterial(const std::string& name) :
	m_name(name)
{
}

GraphicsMaterial::~GraphicsMaterial()
{
}

std::string GraphicsMaterial::name() const
{
	return m_name;
}

} // namespace

} // namespace