#include "IPhysicsMaterial.h"

#include "Core.h"
#include "PhysicsManager.h"
#include "PhysicsSpace.h"
#include "PhysicsGeometry.h"
#include "PhysicsBody.h"
#include "ResourceManager.h"
#include "PhysicsMaterialManager.h"

namespace FreeWorldEngine {

static const std::string g_resourceManagerForPhysicalMaterialsName = "ResourceManagerForPhysicalMaterials";

PhysicsManager::PhysicsManager()
{
	m_world = dWorldCreate();
	m_mainSpace = createSpaceHash();
	m_contactJointGroup = dJointGroupCreate(0);

	m_physicsMaterialManager = new PhysicsMaterialManager(coreEngine->createResourceManager(g_resourceManagerForPhysicalMaterialsName));

	setGravity(glm::vec3(0.0f, -9.8f, 0.0f));

	setERP(0.5f);
	setCFM(1e-5);

	setAutoDisableFlag(true);
	setAutoDisableLinearThreshold(0.03f);
	setAutoDisableAngularThreshold(0.07f);
	setAutoDisableTime(0.0f);
	setAutoDisableSteps(5);

	dWorldSetLinearDamping(m_world, 0.003f);
	dWorldSetAngularDamping(m_world, 0.007f);
	dWorldSetMaxAngularSpeed(m_world, 200.0f);

	dWorldSetContactSurfaceLayer(m_world, 0.001f);
}

PhysicsManager::~PhysicsManager()
{
	delete m_physicsMaterialManager;
	coreEngine->destroyResourceManager(g_resourceManagerForPhysicalMaterialsName);

	dJointGroupDestroy(m_contactJointGroup);
	destroySpace(m_mainSpace);
	dWorldDestroy(m_world);
}

IPhysicsSpace *PhysicsManager::createSpaceSimple(IPhysicsSpace *pParentSpace)
{
	dSpaceID parent = 0;
	if (pParentSpace)
		parent = (dSpaceID)(pParentSpace->id());
	dSpaceID id = dSimpleSpaceCreate(parent);
	IPhysicsSpace *pSpace = new PhysicsSpace(id);
	dGeomSetData((dGeomID)id, pSpace);
	return pSpace;
}

IPhysicsSpace *PhysicsManager::createSpaceHash(const int32 minLevel, const int32 maxLevel, IPhysicsSpace *pParentSpace)
{
	dSpaceID parent = 0;
	if (pParentSpace)
		parent = (dSpaceID)(pParentSpace->id());
	dSpaceID id = dHashSpaceCreate(parent);
	dHashSpaceSetLevels(id, minLevel, maxLevel);
	IPhysicsSpace *pSpace = new PhysicsSpace(id);
	dGeomSetData((dGeomID)id, pSpace);
	return pSpace;
}

IPhysicsSpace *PhysicsManager::createSpaceQuadTree(const glm::vec3& pos, const glm::vec3& size, const int32 depth, IPhysicsSpace *pParentSpace)
{
	dSpaceID parent = 0;
	if (pParentSpace)
		parent = (dSpaceID)(pParentSpace->id());
	dVector3 p = { pos.x, pos.y, pos.z };
	dVector3 s = { size.x, size.y, size.z };
	dSpaceID id = dQuadTreeSpaceCreate(parent, p, s, depth);
	IPhysicsSpace *pSpace = new PhysicsSpace(id);
	dGeomSetData((dGeomID)id, pSpace);
	return pSpace;
}

void PhysicsManager::destroySpace(IPhysicsSpace *pSpace)
{
	if (!pSpace)
		return;
	dSpaceDestroy((dSpaceID)(pSpace->id()));
	delete pSpace;
}

IPhysicsGeometry *PhysicsManager::createGeometrySphere(const float radius, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreateSphere(0, radius);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryBox(const float width, const float height, const float depth, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreateBox(0, width, height, depth);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryCapsule(const float radius, const float length, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreateCapsule(0, radius, length);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryCylinder(const float radius, const float length, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreateCylinder(0, radius, length);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryPlane(const float a, const float b, const float c, const float d, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreatePlane(0, a, b, c, d);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryRay(const float length, IPhysicsSpace *pParentSpace)
{
	dGeomID id = dCreateRay(0, length);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

IPhysicsGeometry *PhysicsManager::createGeometryTriMesh(float *pVertices, const uint32 numVertices, const uint32 vertexStride, uint32 *pIndices, const uint32 numIndices, const uint32 triangleStride, IPhysicsSpace *pParentSpace)
{
	dTriMeshDataID triMeshDataId = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(triMeshDataId, pVertices, vertexStride, numVertices, pIndices, numIndices, triangleStride);

	dGeomID id = dCreateTriMesh(0, triMeshDataId, 0, 0, 0);
	IPhysicsGeometry *pGeom = new PhysicsGeometry(id);
	dGeomSetData(id, pGeom);
	if (pParentSpace)
		pParentSpace->insertPhysicsGeometry(pGeom);
	return pGeom;
}

void PhysicsManager::destroyGeometry(IPhysicsGeometry *pGeom)
{
	if (!pGeom)
		return;
	
	uint32 type = pGeom->type();
	dGeomID id = (dGeomID)(pGeom->id());

	if (type == IPhysicsGeometry::IPhysicsGeometryType_TriMesh) {
		dTriMeshDataID triMeshDataId = dGeomTriMeshGetData(id);
		dGeomTriMeshDataDestroy(triMeshDataId);
	}

	dGeomDestroy(id);
	delete pGeom;
}

IPhysicsBody *PhysicsManager::createBody()
{
	dBodyID id = dBodyCreate(m_world);
	IPhysicsBody *pBody = new PhysicsBody(id);
	dBodySetData(id, pBody);
	return pBody;
}

void PhysicsManager::destroyBody(IPhysicsBody *pBody)
{
	if (!pBody)
		return;
	dBodyDestroy((dBodyID)(pBody->id()));
	delete pBody;
}

IPhysicsMaterialManager *PhysicsManager::physicsMaterialManager() const
{
	return m_physicsMaterialManager;
}

IPhysicsSpace *PhysicsManager::mainSpace() const
{
	return m_mainSpace;
}

void PhysicsManager::setGravity(const glm::vec3& gravity)
{
	dWorldSetGravity(m_world, gravity.x, gravity.y, gravity.z);
}

glm::vec3 PhysicsManager::gravity() const
{
	dVector3 g;
	dWorldGetGravity(m_world, g);
	return glm::vec3(g[0], g[1], g[2]);
}

void PhysicsManager::setERP(const float erp)
{
	dWorldSetERP(m_world, erp);
}

float PhysicsManager::ERP() const
{
	return dWorldGetERP(m_world);
}

void PhysicsManager::setCFM(const float cfm)
{
	dWorldSetCFM(m_world, cfm);
}

float PhysicsManager::CFM() const
{
	return dWorldGetCFM(m_world);
}

void PhysicsManager::update(const uint32 dt)
{
	dSpaceCollide((dSpaceID)(m_mainSpace->id()), this, PhysicsManager::nearCallback);
	dWorldQuickStep(m_world, dt*0.001f);
	dJointGroupEmpty(m_contactJointGroup);
}

void PhysicsManager::setAutoDisableFlag(const bool flag)
{
	dWorldSetAutoDisableFlag(m_world, flag ? 1 : 0);
}

bool PhysicsManager::autoDisableFlag() const
{
	return (bool)dWorldGetAutoDisableFlag(m_world);
}

void PhysicsManager::setAutoDisableLinearThreshold(const float threshold)
{
	dWorldSetAutoDisableLinearThreshold(m_world, threshold);
}

float PhysicsManager::autoDisableLinearThreshold() const
{
	return (bool)dWorldGetAutoDisableLinearThreshold(m_world);
}

void PhysicsManager::setAutoDisableAngularThreshold(const float threshold)
{
	dWorldSetAutoDisableAngularThreshold(m_world, threshold);
}

float PhysicsManager::autoDisableAngularThreshold() const
{
	return (bool)dWorldGetAutoDisableAngularThreshold(m_world);
}

void PhysicsManager::setAutoDisableSteps(const uint32 numSteps)
{
	dWorldSetAutoDisableSteps(m_world, numSteps);
}

uint32 PhysicsManager::autoDisableSteps() const
{
	return dWorldGetAutoDisableSteps(m_world);
}

void PhysicsManager::setAutoDisableTime(const float time)
{
	dWorldSetAutoDisableTime(m_world, time);
}

float PhysicsManager::autoDisableTime() const
{
	return dWorldGetAutoDisableTime(m_world);
}

void PhysicsManager::nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2)) {
		dSpaceCollide2(o1, o2, data, PhysicsManager::nearCallback);
		if (o1 != o2) {
			if (dGeomIsSpace(o1)) dSpaceCollide((dSpaceID)o1, data, PhysicsManager::nearCallback);
			if (dGeomIsSpace(o2)) dSpaceCollide((dSpaceID)o2, data, PhysicsManager::nearCallback);
		}
    }
    else {
		PhysicsManager *pPhysicsManager = (PhysicsManager*)data;
		dWorldID worldID = pPhysicsManager->m_world;
		dJointGroupID contactGroup = pPhysicsManager->m_contactJointGroup;
		IPhysicsMaterial *pMat1 = ((IPhysicsGeometry*)dGeomGetData(o1))->physicsMaterial();
		IPhysicsMaterial *pMat2 = ((IPhysicsGeometry*)dGeomGetData(o1))->physicsMaterial();

		const uint32 maxContacts = 12;
		dContact contacts[maxContacts];
		contacts[0].surface.mode = dContactBounce;
		contacts[0].surface.mu = 0.5f * (pMat1->frictionCoefficient() + pMat2->frictionCoefficient());
		contacts[0].surface.bounce = 0.5f * (pMat1->restitutionCoefficient() + pMat2->restitutionCoefficient());
		contacts[0].surface.bounce_vel = 0.1f;

		for (uint32 contactIdx = 1; contactIdx < maxContacts; ++contactIdx)
			contacts[contactIdx].surface = contacts[0].surface;

		int numContacts = dCollide(o1, o2, maxContacts, &(contacts[0].geom), sizeof(dContact));
		for (int i = 0; i < numContacts; ++i) {
			dJointID contactJoint = dJointCreateContact(worldID, contactGroup, (contacts+i));
			dJointAttach(contactJoint, dGeomGetBody(o1), dGeomGetBody(o2));
		}
	}
}

}; // namespace