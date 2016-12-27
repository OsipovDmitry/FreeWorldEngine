#ifndef __IPHYSICSMANAGER__
#define __IPHYSICSMANAGER__

#include "Types.h"
#include "glm/vec3.hpp"

namespace FreeWorldEngine {

class IPhysicsGeometry;
class IPhysicsSpace;
class IPhysicsBody;
class IPhysicsMaterialManager;


class IPhysicsManager {
public:
	virtual ~IPhysicsManager() = 0 {}

	virtual IPhysicsSpace *createSpaceSimple(IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsSpace *createSpaceHash(const int32 minLevel = -3, const int32 maxLevel = 10, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsSpace *createSpaceQuadTree(const glm::vec3& pos, const glm::vec3& size, const int32 depth, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual void destroySpace(IPhysicsSpace *pSpace) = 0;

	virtual IPhysicsGeometry *createGeometrySphere(const float radius, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryBox(const float width, const float height, const float depth, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryCapsule(const float radius, const float length, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryCylinder(const float radius, const float length, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryPlane(const float a, const float b, const float c, const float d, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryRay(const float length, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual IPhysicsGeometry *createGeometryTriMesh(float *pVertices, const uint32 numVertices, const uint32 vertexStride, uint32 *pIndices, const uint32 numIndices, const uint32 triangleStride, IPhysicsSpace *pParentSpace = 0) = 0;
	virtual void destroyGeometry(IPhysicsGeometry *pGeom) = 0;

	virtual IPhysicsBody *createBody() = 0;
	virtual void destroyBody(IPhysicsBody *pBody) = 0;

	virtual void setGravity(const glm::vec3& gravity) = 0;
	virtual glm::vec3 gravity() const = 0;

	virtual void setERP(const float erp) = 0;
	virtual float ERP() const = 0;
	virtual void setCFM(const float cfm) = 0;
	virtual float CFM() const = 0;
	virtual void update(const uint32 dt) = 0;

	virtual void setAutoDisableFlag(const bool flag) = 0;
	virtual bool autoDisableFlag() const = 0;
	virtual void setAutoDisableLinearThreshold(const float threshold) = 0;
	virtual float autoDisableLinearThreshold() const = 0;
	virtual void setAutoDisableAngularThreshold(const float threshold) = 0;
	virtual float autoDisableAngularThreshold() const = 0;
	virtual void setAutoDisableSteps(const uint32 numSteps) = 0;
	virtual uint32 autoDisableSteps() const = 0;
	virtual void setAutoDisableTime(const float time) = 0;
	virtual float autoDisableTime() const = 0;

	virtual IPhysicsMaterialManager *physicsMaterialManager() const = 0;
	virtual IPhysicsSpace *mainSpace() const = 0;

}; // class IPhysicsManager

}; // namespace

#endif // __IPHYSICSMANAGER__