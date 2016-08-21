#include "IPhysicsManager.h"
#include "IPhysicsMaterialManager.h"

#include "PhysicsGeometry.h"
#include "Core.h"

namespace FreeWorldEngine {

PhysicsGeometry::PhysicsGeometry(dGeomID id) :
	m_geom(id)
{
	setPhysicsMaterial("StandardPhysicsMaterial");
}

PhysicsGeometry::~PhysicsGeometry()
{
}

uint32 PhysicsGeometry::type() const
{
	int geomClass = dGeomGetClass(m_geom);
	switch (geomClass) {
		case dSphereClass: return IPhysicsGeometryType_Sphere;
		case dBoxClass: return IPhysicsGeometryType_Box;
		case dCapsuleClass: return IPhysicsGeometryType_Capsule;
		case dCylinderClass: return IPhysicsGeometryType_Cylinder;
		case dPlaneClass: return IPhysicsGeometryType_Plane;
		case dRayClass: return IPhysicsGeometryType_Ray;
		case dConvexClass: return IPhysicsGeometryType_Convex;
		case dTriMeshClass: return IPhysicsGeometryType_TriMesh;
		case dHeightfieldClass: return IPhysicsGeometryType_Heightfield;
		default: return IPhysicsGeometryType_None;
	};
	return IPhysicsGeometryType_None;
}

uint32 PhysicsGeometry::id() const
{
	return (uint32)m_geom;
}

void PhysicsGeometry::setPhysicsMaterial(const std::string& name)
{
	m_pPhysicsMaterial = coreEngine->physicsManager()->physicsMaterialManager()->getByName(name);
	if (!m_pPhysicsMaterial)
		m_pPhysicsMaterial = coreEngine->physicsManager()->physicsMaterialManager()->getByName("StandardPhysicsMaterial");
}

void PhysicsGeometry::setPhysicsMaterial(IPhysicsMaterial *pPhysicsMaterial)
{
	if (pPhysicsMaterial)
		m_pPhysicsMaterial = pPhysicsMaterial;
	else
		setPhysicsMaterial("StandardPhysicsMaterial");
}

IPhysicsMaterial *PhysicsGeometry::physicsMaterial() const
{
	return m_pPhysicsMaterial;
}

void PhysicsGeometry::params(float* pParams) const
{
	uint32 t = type();
	switch (t) {
		case IPhysicsGeometryType_Sphere: { 
			pParams[0] = dGeomSphereGetRadius(m_geom);
			break;
		}
		case IPhysicsGeometryType_Box: {
			dGeomBoxGetLengths(m_geom, pParams);
			break;
		}
		case IPhysicsGeometryType_Plane: {
			dGeomPlaneGetParams(m_geom, pParams);
			break;
		}
		case IPhysicsGeometryType_Capsule: {
			dGeomCapsuleGetParams(m_geom, pParams+0, pParams+1);
			break;
		}
		case IPhysicsGeometryType_Cylinder: {
			dGeomCylinderGetParams(m_geom, pParams+0, pParams+1);
			break;
		}
		case IPhysicsGeometryType_Ray: {
			pParams[0] = dGeomRayGetLength(m_geom);
			break;
		}
		case IPhysicsGeometryType_TriMesh: {
			//dTriMeshDataID triMeshDataId = dGeomTriMeshGetTriMeshDataID(m_geom);
			break;
		}
		default: break;
	}
}

void PhysicsGeometry::setParams(float* const pParams)
{
	uint32 t = type();
	switch (t) {
		case IPhysicsGeometryType_Sphere: { 
			dGeomSphereSetRadius(m_geom, pParams[0]);
			break;
		}
		case IPhysicsGeometryType_Box: {
			dGeomBoxSetLengths(m_geom, pParams[0], pParams[1], pParams[2]);
			break;
		}
		case IPhysicsGeometryType_Plane: {
			dGeomPlaneSetParams(m_geom, pParams[0], pParams[1], pParams[2], pParams[3]);
			break;
		}
		case IPhysicsGeometryType_Capsule: {
			dGeomCapsuleSetParams(m_geom, pParams[0], pParams[1]);
			break;
		}
		case IPhysicsGeometryType_Cylinder: {
			dGeomCylinderSetParams(m_geom, pParams[0], pParams[1]);
			break;
		}
		case IPhysicsGeometryType_Ray: {
			dGeomRaySetLength(m_geom, pParams[0]);
			break;
		} 
		default: break;
	}
}

void PhysicsGeometry::setPosition(const glm::vec3& pos)
{
	dGeomSetPosition(m_geom, pos.x, pos.y, pos.z);
}

glm::vec3 PhysicsGeometry::position() const
{
	const dReal *result = dGeomGetPosition(m_geom);
	return glm::vec3(result[0], result[1], result[2]);
}

void PhysicsGeometry::setOrientation(const glm::quat& orient)
{
	dQuaternion q = { orient.w, orient.x, orient.y, orient.z };
	dGeomSetQuaternion(m_geom, q);
}

glm::quat PhysicsGeometry::orientation() const
{
	dQuaternion q;
	dGeomGetQuaternion(m_geom, q);
	return glm::quat(q[0], q[1], q[2], q[3]);
}

void PhysicsGeometry::aabb(glm::vec3& resultMin, glm::vec3& resultMax) const
{
	dReal result[6];
	dGeomGetAABB(m_geom, result);
	resultMin.x = result[0];
	resultMin.y = result[1];
	resultMin.z = result[2];
	resultMax.x = result[3];
	resultMax.y = result[4];
	resultMax.z = result[5];
}


} // namespace