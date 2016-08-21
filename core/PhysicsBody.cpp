#include "IPhysicsGeometry.h"
#include "IPhysicsMaterial.h"

#include "PhysicsBody.h"

namespace FreeWorldEngine {

PhysicsBody::PhysicsBody(dBodyID id) :
	m_body(id),
	m_geometries()
{
}

PhysicsBody::~PhysicsBody()
{
}

uint32 PhysicsBody::id() const
{
	return (uint32)m_body;
}

void PhysicsBody::setPosition(const glm::vec3& pos)
{
	dBodySetPosition(m_body, pos.x, pos.y, pos.z);
}

glm::vec3 PhysicsBody::position() const
{
	const dReal *result = dBodyGetPosition(m_body);
	return glm::vec3(result[0], result[1], result[2]);
}

void PhysicsBody::setOrientation(const glm::quat& orient)
{
	dQuaternion q = { orient.w, orient.x, orient.y, orient.z };
	dBodySetQuaternion(m_body, q);
}

glm::quat PhysicsBody::orientation() const
{
	const dReal *result = dBodyGetQuaternion(m_body);
	return glm::quat(result[0], result[1], result[2], result[3]);
}

void PhysicsBody::setLinearVelocity(const glm::vec3& vel)
{
	dBodySetLinearVel(m_body, vel.x, vel.y, vel.z);
}

glm::vec3 PhysicsBody::linearVelocity() const
{
	const dReal *result = dBodyGetLinearVel(m_body);
	return glm::vec3(result[0], result[1], result[2]);
}

void PhysicsBody::setAngularVelocity(const glm::vec3& vel)
{
	dBodySetAngularVel(m_body, vel.x, vel.y, vel.z);
}

glm::vec3 PhysicsBody::angularVelocity() const
{
	const dReal *result = dBodyGetAngularVel(m_body);
	return glm::vec3(result[0], result[1], result[2]);
}

void PhysicsBody::addForce(const glm::vec3 force)
{
	dBodyAddForce(m_body, force.x, force.y, force.z);
}

void PhysicsBody::addTorque(const glm::vec3 torque)
{
	dBodyAddTorque(m_body, torque.x, torque.y, torque.z);
}

void PhysicsBody::addRelativeForce(const glm::vec3 force)
{
	dBodyAddRelForce(m_body, force.x, force.y, force.z);
}

void PhysicsBody::addRelativeTorque(const glm::vec3 torque)
{
	dBodyAddRelTorque(m_body, torque.x, torque.y, torque.z);
}

void PhysicsBody::addForceAtPosition(const glm::vec3 force, const glm::vec3& pos)
{
	dBodyAddForceAtPos(m_body, force.x, force.y, force.z, pos.x, pos.y, pos.z);
}


void PhysicsBody::addForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos)
{
	dBodyAddForceAtRelPos(m_body, force.x, force.y, force.z, pos.x, pos.y, pos.z);
}


void PhysicsBody::addRelativeForceAtPosition(const glm::vec3 force, const glm::vec3& pos)
{
	dBodyAddRelForceAtPos(m_body, force.x, force.y, force.z, pos.x, pos.y, pos.z);
}


void PhysicsBody::addRelativeForceAtRelativePosition(const glm::vec3 force, const glm::vec3& pos)
{
	dBodyAddRelForceAtRelPos(m_body, force.x, force.y, force.z, pos.x, pos.y, pos.z);
}

void PhysicsBody::setForce(const glm::vec3 force)
{
	dBodySetForce(m_body, force.x, force.y, force.z);
}

void PhysicsBody::setTorque(const glm::vec3 force)
{
	dBodySetTorque(m_body, force.x, force.y, force.z);
}

glm::vec3 PhysicsBody::force() const
{
	const dReal *result = dBodyGetForce(m_body);
	return glm::vec3(result[0], result[1], result[2]);
}

glm::vec3 PhysicsBody::torque() const
{
	const dReal *result = dBodyGetTorque(m_body);
	return glm::vec3(result[0], result[1], result[2]);
}

void PhysicsBody::enable()
{
	dBodyEnable(m_body);
}

void PhysicsBody::disable()
{
	dBodyDisable(m_body);
}

bool PhysicsBody::isEnabled() const
{
	return (bool)dBodyIsEnabled(m_body);
}

void PhysicsBody::setAutoDisableFlag(const bool flag)
{
	dBodySetAutoDisableFlag(m_body, flag ? 1 : 0);
}

bool PhysicsBody::autoDisableFlag() const
{
	return (bool)dBodyGetAutoDisableFlag(m_body);
}

void PhysicsBody::setAutoDisableLinearThreshold(const float threshold)
{
	dBodySetAutoDisableLinearThreshold(m_body, threshold);
}

float PhysicsBody::autoDisableLinearThreshold() const
{
	return (bool)dBodyGetAutoDisableLinearThreshold(m_body);
}

void PhysicsBody::setAutoDisableAngularThreshold(const float threshold)
{
	dBodySetAutoDisableAngularThreshold(m_body, threshold);
}

float PhysicsBody::autoDisableAngularThreshold() const
{
	return (bool)dBodyGetAutoDisableAngularThreshold(m_body);
}

void PhysicsBody::setAutoDisableSteps(const uint32 numSteps)
{
	dBodySetAutoDisableSteps(m_body, numSteps);
}

uint32 PhysicsBody::autoDisableSteps() const
{
	return dBodyGetAutoDisableSteps(m_body);
}

void PhysicsBody::setAutoDisableTime(const float time)
{
	dBodySetAutoDisableTime(m_body, time);
}

float PhysicsBody::autoDisableTime() const
{
	return dBodyGetAutoDisableTime(m_body);
}

void PhysicsBody::setAutoDisableDefaults()
{
	dBodySetAutoDisableDefaults(m_body);
}

void PhysicsBody::setGravityMode(const bool mode)
{
	dBodySetGravityMode(m_body, mode ? 1 : 0);
}

bool PhysicsBody::gravityMode() const
{
	return (bool)dBodyGetGravityMode(m_body);
}

void PhysicsBody::setKinematicState(const bool state)
{
	state ? dBodySetDynamic(m_body) : dBodySetKinematic(m_body);
}

bool PhysicsBody::kinematicState() const
{
	return (bool)dBodyIsKinematic(m_body);
}

void PhysicsBody::addGeometry(IPhysicsGeometry *pGeom)
{
	m_geometries.insert(pGeom);
	recalcMass();
}

void PhysicsBody::removeGeometry(IPhysicsGeometry *pGeom)
{
	m_geometries.erase(pGeom);
	dGeomSetBody((dGeomID)(pGeom->id()), 0);
	recalcMass();
}

void PhysicsBody::removeAllGeometries()
{
	m_geometries.clear();
	recalcMass();
}

const std::set<IPhysicsGeometry*>& PhysicsBody::geometriesList()
{
	return m_geometries;
}

void PhysicsBody::recalcMass()
{
	dMass bodyMass, mass;
	m_geometries.empty() ? dMassSetParameters (&bodyMass,1,0,0,0,1,1,1,0,0,0) : dMassSetZero(&bodyMass);

	for (std::set<IPhysicsGeometry*>::const_iterator i = m_geometries.cbegin(); i != m_geometries.cend(); ++i) {
		uint32 type = (*i)->type();
		dGeomID geom = (dGeomID)((*i)->id());
		float params[10];
		(*i)->params(params);
		float density = (*i)->physicsMaterial()->density();
		bool isOk = true;
		switch (type) {
			case IPhysicsGeometry::IPhysicsGeometryType_Sphere: {
				dMassSetSphere(&mass, density, params[0]);
				break;
			}
			case IPhysicsGeometry::IPhysicsGeometryType_Box: {
				dMassSetBox(&mass, density, params[0], params[1], params[2]);
				break;
			}
			case IPhysicsGeometry::IPhysicsGeometryType_Cylinder: {
				dMassSetCylinder(&mass, density, 3, params[0], params[1]);
				break;
			}
			case IPhysicsGeometry::IPhysicsGeometryType_Capsule: {
				dMassSetCapsule(&mass, density, 3, params[0], params[1]);
				break;
			}
			case IPhysicsGeometry::IPhysicsGeometryType_TriMesh: {
				dMassSetTrimesh(&mass, density, geom);
				break;
			}
			default: {
				isOk = false;
				break;
			}		 
		}
		if (!isOk)
			continue;
		
		dMassRotate(&mass, dGeomGetRotation(geom));
		const dReal *pos = dGeomGetPosition(geom);
		dMassTranslate(&mass, pos[0], pos[1], pos[2]);
		dMassAdd(&bodyMass, &mass);
	}

	dMatrix3 identity;
	dRSetIdentity(identity);

	for (std::set<IPhysicsGeometry*>::const_iterator i = m_geometries.cbegin(); i != m_geometries.cend(); ++i) {
		dGeomID geom = (dGeomID)((*i)->id());
		dVector3 pos;
		dMatrix3 rot;
		dGeomCopyPosition(geom, pos);
		dGeomCopyRotation(geom, rot);
		dGeomSetPosition(geom, 0.0f, 0.0f, 0.0f);
		dGeomSetRotation(geom, identity);
		dGeomSetBody(geom, m_body);
		dGeomSetOffsetPosition (geom, pos[0], pos[1], pos[2]);
		dGeomSetOffsetRotation(geom, rot);
	}

	dMassTranslate(&bodyMass, -bodyMass.c[0], -bodyMass.c[1], -bodyMass.c[2]);
	dBodySetMass(m_body, &bodyMass);
}

} // namespace