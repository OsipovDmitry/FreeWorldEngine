#include <3rdparty/glm/geometric.hpp>

#include <math/MathUtils.h>
#include <math/MeshWrapper.h>
#include <math/MathTypes.h>
#include <Types.h>

namespace FreeWorldEngine {

namespace Math {

Plane buildPlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	glm::vec3 n = glm::normalize(glm::cross(v1-v0, v2-v0));
	return Plane(n, -glm::dot(n, v0));
}

float distToPlane(const Plane& plane, const glm::vec3& v)
{
	return plane.x*v.x + plane.y*v.y + plane.z*v.z + plane.w;
}

ClassifyPlane classifyPolygonRelativePlane(const Plane& plane, glm::vec3 **verts, uint32 numVerts)
{
	bool front = false, back = false;

	for (uint32 i = 0; i < numVerts; ++i) {
		float dist = distToPlane(plane, *(verts[i]));
		
		front = front || (dist > eps);
		back = back || (dist < -eps);

		if (front && back)
			return ClassifyPlane_Intersect;
	}

	if (front)
		return ClassifyPlane_Front;
	if (back)
		return ClassifyPlane_Back;

	return ClassifyPlane_Touch;
}

Aabb buldAabb(Mesh *pMesh)
{
	MeshWrapper mesh(pMesh);

	if (pMesh->numVertices == 0)
		return Aabb(glm::vec3(), glm::vec3());

	float *pVertex = mesh.attributeValue(VertexAttributeType_Position, 0);
	glm::vec3 vMin(pVertex[0], pVertex[1], pVertex[2]), vMax(pVertex[0], pVertex[1], pVertex[2]);

	for (uint32 i = 1; i < pMesh->numVertices; ++i) {
		pVertex = mesh.attributeValue(VertexAttributeType_Position, i);
		
		if (pVertex[0] < vMin.x) vMin.x = pVertex[0];
		else if (pVertex[0] > vMax.x) vMax.x = pVertex[0];

		if (pVertex[1] < vMin.y) vMin.y = pVertex[1];
		else if (pVertex[1] > vMax.y) vMax.y = pVertex[1];

		if (pVertex[2] < vMin.z) vMin.z = pVertex[2];
		else if (pVertex[2] > vMax.z) vMax.z = pVertex[2];
	}

	return Aabb(vMin, vMax);
}

void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef)
{
	resultCoef = -1.0f;

	float dists[2] = { distToPlane(plane, *(verts[0])), distToPlane(plane, *(verts[1])) };

	if ((dists[0] >= -eps && dists[1] >= -eps) ||
		(dists[0] <= eps && dists[1] <= eps)) // обе точки лежат по одну сторону от плоскости или почти по одну, возможно обе на плоскости. Резать не надо.
		return;

	resultCoef = abs(dists[0]) / abs(dists[0] - dists[1]);
}

void cutTriangle(glm::vec3 **verts, const Plane& plane, float &resultCoef01, float &resultCoef12, float &resultCoef20)
{
	resultCoef01 = resultCoef12 = resultCoef20 = -1.0f;

	float dists[3] = { distToPlane(plane, *(verts[0])), distToPlane(plane, *(verts[1])), distToPlane(plane, *(verts[2])) };

	if ((dists[0] >= -eps && dists[1] >= -eps && dists[2] >= -eps) ||
		(dists[0] <= eps && dists[1] <= eps && dists[2] <= eps)) // все точки лежат по одну сторону от плоскости или почти по одну, возможно все на плоскости. Резать не надо.
		return;

	float absDists[3] = { abs(dists[0]), abs(dists[1]), abs(dists[2]) };

	if (absDists[0] < eps) { // Если одна точка лежит на плоскости, то режем противолежащее ребро.
		resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
		return;
	}

	if (absDists[1] < eps) { // Если одна точка лежит на плоскости, то режем противолежащее ребро.
		resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
		return;
	}

	if (absDists[2] < eps) { // Если одна точка лежит на плоскости, то режем противолежащее ребро.
		resultCoef01 = absDists[0] / (absDists[0] + absDists[1]);
		return;
	}

	if (dists[0]*dists[1] < 0.0f) { // точки 0 и 1 лежат по разные стороны
		resultCoef01 = absDists[0] / (absDists[0] + absDists[1]);
		if (dists[0]*dists[2] < 0.0f) // точки 0 и 2 лежат по разные стороны => точка 0 лежит по другую сторону от точек 1 и 2
			resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
		else  // точка 1 лежит по другую сторону от точек 2 и 0
			resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
	} else { // точка 2, лежит по другую сторону от точек 0 и 1
		resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
		resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
	}
}

} // namespace
} // namespace
