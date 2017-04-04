#ifndef __MATHUTILS__
#define __MATHUTILS__

#include "../Types.h"
#include "MathTypes.h"

struct Mesh;

namespace FreeWorldEngine {

namespace Math {

enum ClassifyPlane {
	ClassifyPlane_Touch,
	ClassifyPlane_Front,
	ClassifyPlane_Back,
	ClassifyPlane_Intersect
}; // enum

// ������ �����
template <class T>
inline T abs(const T& value) { return (value > static_cast<T>(0)) ? value : -value; }

// ������������ ���� �������� �� �����������.
template <class T1, class T2>
inline T1 interpolate(const T1& v0, const T1& v1, const T2& coef) {
	return v0*(T2(1)-coef) + v1*coef;
}

template <class T1, class T2>
inline void interpolate(const T1& v0, const T1& v1, const T2& coef, T1& result) {
	result = v0*(T2(1)-coef) + v1*coef;
}

// ��������� ��������� �� ���� ������.
Plane buildPlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

// ���������� �� ����� �� ��������� �� ������.
inline float distToPlane(const Plane& plane, const glm::vec3& v) {
	return plane.x*v.x + plane.y*v.y + plane.z*v.z + plane.w;
}

// ������������� �������� ������������ ���������. verts - ������ ���������� �� �����. ������� ����� ��������� 1,2,3 ��� ����� �����.
ClassifyPlane classifyPolygonRelativePlane(const Plane& plane, glm::vec3 **verts, uint32 numVerts);

// ������������� ����� ������������ ���������.
inline ClassifyPlane classifyRelativePlane(const Plane& plane, const Sphere& sphere) {
	float d = plane.x * sphere.x + plane.y * sphere.y + plane.z * sphere.z + plane.w;
	if (d > sphere.w) return ClassifyPlane_Front;
	else if (d < -sphere.w) return ClassifyPlane_Back;
	else return ClassifyPlane_Intersect;
}

// ������������� AABB ������������ ���������.
inline ClassifyPlane classifyRelativePlane(const Plane& plane, const Aabb& box) {
	glm::vec3 neg_point, pos_point;
	
	if (plane.x < 0.0f) { neg_point.x = box.vMin.x; pos_point.x = box.vMax.x; }
	else { neg_point.x = box.vMax.x; pos_point.x = box.vMin.x; }
	
	if (plane.y < 0.0f) { neg_point.y = box.vMin.y; pos_point.y = box.vMax.y; }
	else { neg_point.y = box.vMax.y; pos_point.y = box.vMin.y; }
	
	if (plane.z < 0.0f) { neg_point.z = box.vMin.z; pos_point.z = box.vMax.z; }
	else { neg_point.z = box.vMax.z; pos_point.z = box.vMin.z; }

	float dist[2] = { distToPlane(plane, neg_point), distToPlane(plane, pos_point) };
	if (dist[0] > 0.0f && dist[1] > 0.0f) return ClassifyPlane_Front;
	else if (dist[0] < 0.0f && dist[1] < 0.0f) return ClassifyPlane_Back;
	else return ClassifyPlane_Intersect;
}

// ��������� ������� ����������. � resultCoef ������������ ���������� (0..1) ����� ����������� � -1, ���� ����������� ���.
// ���� ������� ������� � resultCoef �� ������������ ��������, �� ����� ������� ����� �������� interpolate(*(verts[0]), *(verts[1]), resultCoef)
void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef);

// ��������� ����������� ����������. � resultCoef[i] ������������ ����������� (0..1) ����� ������� ����� ������������ ��� -1 � ������, ���� ������ ����� ������ �� �����.
// ��� �������, � ���� resultCoef[i] ����� ���������� -1, ��� ��� ��������� �� ����� ������������ ���������� ��� ��� ����� ������������.
// ���� � ��� �� ���� resultCoef[i] ��������� -1, �� ��� ������, ��� ��������� �������� ����� ���� ����� ������������ � ��������������� �������.
// ���� � ��� �� ����, �� ����������� ������ �� �����. �� ����� ���� �� ���������, ���� �� ���� ������� �� ���������
void cutTriangle(glm::vec3 **verts, const Plane& plane, float &resultCoef01, float &resultCoef12, float &resultCoef20);


template <class T>
inline bool geomInFrustum(const Frustum& frustum, const T& geom) {
	for (int32 i = 0; i < 6; ++i)
		if (classifyRelativePlane(frustum[i], geom) == ClassifyPlane_Back)
			return false;
	return true;
}


} // namespace
} // namespace

#endif // __MATHUTILS__