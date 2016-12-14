#ifndef __MATHTYPES__
#define __MATHTYPES__

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace FreeWorldEngine {

namespace Math {

struct Aabb {
	glm::vec3 vMin, vMax;

	Aabb(const glm::vec3& vertexMin, const glm::vec3& vertexMax) : vMin(vertexMin), vMax(vertexMax) {}
};

typedef glm::vec4 Plane;

} // namespace
} // namespace

#endif // __MATHTYPES__