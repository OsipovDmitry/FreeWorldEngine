#ifndef __IPHYSYCSGEOMETRY__
#define __IPHYSYCSGEOMETRY__

#include <string>

#include "Types.h"
#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

namespace FreeWorldEngine {

class IPhysicsMaterial;

class IPhysicsGeometry {
public:
	enum IPhysicsGeometryType {
		IPhysicsGeometryType_None = 0, // Неопределенная геометрия. Значение 0.

		IPhysicsGeometryType_Sphere, // Далее классы геометрий. 1 <= Значения < IPhysicsGeometryType_NumGeometries.
		IPhysicsGeometryType_Box,
		IPhysicsGeometryType_Capsule,
		IPhysicsGeometryType_Cylinder,
		IPhysicsGeometryType_Convex,
		IPhysicsGeometryType_TriMesh,
		IPhysicsGeometryType_Plane,
		IPhysicsGeometryType_Ray,
		IPhysicsGeometryType_Heightfield,

		IPhysicsGeometryType_NumGeometries, // Значение для разделения простых геометрий и пространств.
		
		IPhysicsGeometryType_SpaceSimple, // Далее классы пространств. Значения > IPhysicsGeometryType_NumGeometries.
		IPhysicsGeometryType_SpaceHash,
		IPhysicsGeometryType_SpaceQuadTree
	};

	virtual ~IPhysicsGeometry() = 0 {}
	virtual uint32 type() const = 0;
	virtual uint32 id() const = 0;
	virtual void setPhysicsMaterial(const std::string& name) = 0; // Только для экземпляров геометрии. Для пространств не используется.
	virtual void setPhysicsMaterial(IPhysicsMaterial *pPhysicsMaterial) = 0; // Только для экземпляров геометрии. Для пространств не используется.
	virtual IPhysicsMaterial *physicsMaterial() const = 0; // Только для экземпляров геометрии. Для пространств не используется.
	virtual void params(float *pParams) const = 0; // Только для экземпляров геометрии. Для пространств не используется.
	virtual void setParams(float* const pParams) = 0; // Только для экземпляров геометрии. Для пространств не используется.
	virtual void setPosition(const glm::vec3& pos) = 0;
	virtual glm::vec3 position() const = 0;
	virtual void setOrientation(const glm::quat& orient) = 0;
	virtual glm::quat orientation() const = 0;
	virtual void aabb(glm::vec3& resultMin, glm::vec3& resultMax) const = 0;

}; // class IPhysicsGeometry

};

#endif // __IPHYSYCSGEOMETRY__