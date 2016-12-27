#include "PhysicsSpace.h"

namespace FreeWorldEngine {

PhysicsSpace::PhysicsSpace(const dSpaceID id) :
	m_space(id)
{
}

PhysicsSpace::~PhysicsSpace()
{
}

uint32 PhysicsSpace::type() const
{
	int type = dSpaceGetClass(m_space);
	switch (type) {
		case dSimpleSpaceClass: return IPhysicsGeometryType_SpaceSimple;
		case dHashSpaceClass: return IPhysicsGeometryType_SpaceHash;
		case dQuadTreeSpaceClass: return IPhysicsGeometryType_SpaceQuadTree;
		default: return IPhysicsGeometryType_None;
	};
	return IPhysicsGeometryType_None;
}

uint32 PhysicsSpace::id() const
{
	return (uint32)m_space;
}

void PhysicsSpace::aabb(glm::vec3& resultMin, glm::vec3& resultMax) const
{
	dReal result[6];
	dGeomGetAABB((dGeomID)m_space, result);
	resultMin.x = result[0];
	resultMin.y = result[1];
	resultMin.z = result[2];
	resultMax.x = result[3];
	resultMax.y = result[4];
	resultMax.z = result[5];
}

void PhysicsSpace::insertPhysicsGeometry(IPhysicsGeometry *pGeometry)
{
	if (pGeometry)
		dSpaceAdd(m_space, (dGeomID)(pGeometry->id()));
}

void PhysicsSpace::removePhysicsGeometry(IPhysicsGeometry *pGeometry)
{
	if (pGeometry)
		dSpaceRemove(m_space, (dGeomID)(pGeometry->id()));
}

bool PhysicsSpace::queryPhysicsGeometry(IPhysicsGeometry *pGeometry) const
{
	if (!pGeometry)
		return false;
	return (bool)dSpaceQuery(m_space, (dGeomID)(pGeometry->id()));
}

void PhysicsSpace::setCleanup(const bool state)
{
	dSpaceSetCleanup(m_space, state ? 1 : 0);
}

bool PhysicsSpace::isCleanup() const
{
	return (bool)dSpaceGetCleanup(m_space);
}

} // namespace