#ifndef __MATHUTILS__
#define __MATHUTILS__

#include <Types.h>
#include <math/MathTypes.h>

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
inline T abs(const T& value) { return (value > 0) ? value : -value; }

// ������������ ���� �������� �� �����������.
template <class T1, class T2>
inline T1 interpolate(const T1& v0, const T1& v1, const T2& coef) { return v0*(T2(1)-coef) + v1*coef; }

template <class T1, class T2>
inline void interpolate(const T1& v0, const T1& v1, const T2& coef, T1& result) { result = v0*(T2(1)-coef) + v1*coef; }

// ��������� ��������� �� ���� ������.
Plane buildPlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

// ���������� �� ����� �� ��������� �� ������.
float distToPlane(const Plane& plane, const glm::vec3& v);

// ������������� �������� ������������ ���������. verts - ������ ���������� �� �����. ������� ����� ��������� 1,2,3 ��� ����� �����.
ClassifyPlane classifyPolygonRelativePlane(const Plane& plane, glm::vec3 **verts, uint32 numVerts, const float eps = 0.0001f);

// ��������� BoundBox ��� ����.
Aabb buldAabb(Mesh *pMesh);

// ��������� ������� ����������. � resultCoef ������������ ���������� (0..1) ����� ����������� � -1, ���� ����������� ���.
// ���� ������� ������� � resultCoef �� ������������ ��������, �� ����� ������� ����� �������� interpolate(*(verts[0]), *(verts[1]), resultCoef)
void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef, const float eps = 0.0001f);

// ��������� ����������� ����������. � resultCoef[i] ������������ ����������� (0..1) ����� ������� ����� ������������ ��� -1 � ������, ���� ������ ����� ������ �� �����.
// ��� �������, � ���� resultCoef[i] ����� ���������� -1, ��� ��� ��������� �� ����� ������������ ���������� ��� ��� ����� ������������.
// ���� � ��� �� ���� resultCoef[i] ��������� -1, �� ��� ������, ��� ��������� �������� ����� ���� ����� ������������ � ��������������� �������.
// ���� � ��� �� ����, �� ����������� ������ �� �����. �� ����� ���� �� ���������, ���� �� ���� ������� �� ���������
void cutTriangle(glm::vec3 **verts, const Plane& plane, float &resultCoef01, float &resultCoef12, float &resultCoef20, const float eps = 0.0001f);

} // namespace
} // namespace

#endif // __MATHUTILS__