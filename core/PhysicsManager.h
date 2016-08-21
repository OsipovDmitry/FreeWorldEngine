#ifndef __PHYSICSMANAGER__
#define __PHYSICSMANAGER__

#include "IPhysicsManager.h"
#include "ode/ode.h"

namespace FreeWorldEngine {

class IResourceManager;

class PhysicsManager : public IPhysicsManager {
public:
	PhysicsManager();
	~PhysicsManager();

	IPhysicsSpace *createSpaceSimple(IPhysicsSpace *pParentSpace = 0);
	IPhysicsSpace *createSpaceHash(const int32 minLevel = -3, const int32 maxLevel = 10, IPhysicsSpace *pParentSpace = 0);
	IPhysicsSpace *createSpaceQuadTree(const glm::vec3& pos, const glm::vec3& size, const int32 depth, IPhysicsSpace *pParentSpace = 0);
	void destroySpace(IPhysicsSpace *pSpace);

	IPhysicsGeometry *createGeometrySphere(const float radius, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryBox(const float width, const float height, const float depth, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryCapsule(const float radius, const float length, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryCylinder(const float radius, const float length, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryPlane(const float a, const float b, const float c, const float d, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryRay(const float length, IPhysicsSpace *pParentSpace = 0);
	IPhysicsGeometry *createGeometryTriMesh(float *pVertices, const uint32 numVertices, const uint32 vertexStride, uint32 *pIndices, const uint32 numIndices, const uint32 triangleStride, IPhysicsSpace *pParentSpace = 0);
	void destroyGeometry(IPhysicsGeometry *pGeom);

	void setERP(const float erp);
	float ERP() const;
	void setCFM(const float cfm);
	float CFM() const;
	void update(const uint32 dt);

	void setAutoDisableFlag(const bool flag);
	bool autoDisableFlag() const;
	void setAutoDisableLinearThreshold(const float threshold);
	float autoDisableLinearThreshold() const;
	void setAutoDisableAngularThreshold(const float threshold);
	float autoDisableAngularThreshold() const;
	void setAutoDisableSteps(const uint32 numSteps);
	uint32 autoDisableSteps() const;
	void setAutoDisableTime(const float time);
	float autoDisableTime() const;

	IPhysicsMaterialManager *physicsMaterialManager() const;
	IPhysicsSpace *mainSpace() const;
	void setGravity(const glm::vec3& gravity);
	glm::vec3 gravity() const;

	IPhysicsBody *createBody();
	void destroyBody(IPhysicsBody *pBody);

private:
	dWorldID m_world;
	dJointGroupID m_contactJointGroup;
	IPhysicsSpace *m_mainSpace;
	IPhysicsMaterialManager *m_physicsMaterialManager;

	static void nearCallback(void *data, dGeomID o1, dGeomID o2);

}; // class PhysicsManager

}; // namespace

#endif // __PHYSICSMANAGER__