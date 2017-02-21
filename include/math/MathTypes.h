#ifndef __MATHTYPES__
#define __MATHTYPES__

#include <3rdparty/glm/vec3.hpp>
#include <3rdparty/glm/vec4.hpp>
#include <3rdparty/glm/mat4x4.hpp>

#include <Types.h>

namespace FreeWorldEngine {

namespace Math {

const float eps = 0.0001f;

struct Aabb {
	glm::vec3 vMin, vMax;

	Aabb(const glm::vec3& vertexMin, const glm::vec3& vertexMax) : vMin(vertexMin), vMax(vertexMax) {}
};

typedef glm::vec4 Plane;

typedef Plane Frustum[6];

} // namespace
} // namespace

#endif // __MATHTYPES__