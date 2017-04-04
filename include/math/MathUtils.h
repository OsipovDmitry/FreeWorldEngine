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

// модуль числа
template <class T>
inline T abs(const T& value) { return (value > static_cast<T>(0)) ? value : -value; }

// Интерполяция двух значений по коэфициенту.
template <class T1, class T2>
inline T1 interpolate(const T1& v0, const T1& v1, const T2& coef) {
	return v0*(T2(1)-coef) + v1*coef;
}

template <class T1, class T2>
inline void interpolate(const T1& v0, const T1& v1, const T2& coef, T1& result) {
	result = v0*(T2(1)-coef) + v1*coef;
}

// Построить плоскость по трем точкам.
Plane buildPlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

// Расстояние от точки до плоскости со знаком.
inline float distToPlane(const Plane& plane, const glm::vec3& v) {
	return plane.x*v.x + plane.y*v.y + plane.z*v.z + plane.w;
}

// Классификация полигона относительно плоскости. verts - массив указателей на точки. Полигон может содержать 1,2,3 или более точек.
ClassifyPlane classifyPolygonRelativePlane(const Plane& plane, glm::vec3 **verts, uint32 numVerts);

// Классификация сферы относительно плоскости.
inline ClassifyPlane classifyRelativePlane(const Plane& plane, const Sphere& sphere) {
	float d = plane.x * sphere.x + plane.y * sphere.y + plane.z * sphere.z + plane.w;
	if (d > sphere.w) return ClassifyPlane_Front;
	else if (d < -sphere.w) return ClassifyPlane_Back;
	else return ClassifyPlane_Intersect;
}

// Классификация AABB относительно плоскости.
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

// Разрезать отрезок плоскостью. В resultCoef возвращается коэфициент (0..1) точки пересечения и -1, если пересечения нет.
// Если функция вернула в resultCoef не отрицательно значение, то точку разреза можно получить interpolate(*(verts[0]), *(verts[1]), resultCoef)
void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef);

// Разрезать треугольник плоскостью. В resultCoef[i] возвращаются коэфициенты (0..1) точек разреза ребер треугольника или -1 в случае, если данное ребро резать не нужно.
// Как минимум, в один resultCoef[i] будет возвращено -1, так как плоскость не может одновременно пересекать все три ребра треугольника.
// Если в два из трех resultCoef[i] вернулось -1, то это значит, что плоскость проходит через одно ребро треугольника и противолежащуюю вершину.
// Если в три из трех, то треугольник резать не нужно. Он лежит либо на плоскости, либо по одну сторону от плоскости
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