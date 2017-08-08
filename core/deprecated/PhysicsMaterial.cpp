#include "PhysicsMaterial.h"

namespace FreeWorldEngine {

PhysicsMaterial::PhysicsMaterial(const std::string& name) :
	m_name(name),
	m_density(200.0f),
	m_friction((float)_HUGE),
	m_restitution(0.1f)
{
}

PhysicsMaterial::~PhysicsMaterial()
{
}

std::string PhysicsMaterial::name() const
{
	return m_name;
}

float PhysicsMaterial::density() const
{
	return m_density;
}

void PhysicsMaterial::setDensity(const float density)
{
	m_density = density;
}

float PhysicsMaterial::frictionCoefficient() const
{
	return m_friction;
}

void PhysicsMaterial::setFrictionCoefficient(const float coefficient)
{
	m_friction = coefficient;
}

float PhysicsMaterial::restitutionCoefficient() const
{
	return m_restitution;
}

void PhysicsMaterial::setRestitutionCoefficient(const float coefficient)
{
	m_restitution = coefficient;
}


} // namespace