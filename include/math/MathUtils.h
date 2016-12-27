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

// модуль числа
template <class T>
inline T abs(const T& value) { return (value > 0) ? value : -value; }

// Интерполяция двух значений по коэфициенту.
template <class T1, class T2>
inline T1 interpolate(const T1& v0, const T1& v1, const T2& coef) { return v0*(T2(1)-coef) + v1*coef; }

template <class T1, class T2>
inline void interpolate(const T1& v0, const T1& v1, const T2& coef, T1& result) { result = v0*(T2(1)-coef) + v1*coef; }

// Построить плоскость по трем точкам.
Plane buildPlane(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

// Расстояние от точки до плоскости со знаком.
float distToPlane(const Plane& plane, const glm::vec3& v);

// Классификация полигона относительно плоскости. verts - массив указателей на точки. Полигон может содержать 1,2,3 или более точек.
ClassifyPlane classifyPolygonRelativePlane(const Plane& plane, glm::vec3 **verts, uint32 numVerts, const float eps = 0.0001f);

// Построить BoundBox для меша.
Aabb buldAabb(Mesh *pMesh);

// Разрезать отрезок плоскостью. В resultCoef возвращается коэфициент (0..1) точки пересечения и -1, если пересечения нет.
// Если функция вернула в resultCoef не отрицательно значение, то точку разреза можно получить interpolate(*(verts[0]), *(verts[1]), resultCoef)
void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef, const float eps = 0.0001f);

// Разрезать треугольник плоскостью. В resultCoef[i] возвращаются коэфициенты (0..1) точек разреза ребер треугольника или -1 в случае, если данное ребро резать не нужно.
// Как минимум, в один resultCoef[i] будет возвращено -1, так как плоскость не может одновременно пересекать все три ребра треугольника.
// Если в два из трех resultCoef[i] вернулось -1, то это значит, что плоскость проходит через одно ребро треугольника и противолежащуюю вершину.
// Если в три из трех, то треугольник резать не нужно. Он лежит либо на плоскости, либо по одну сторону от плоскости
void cutTriangle(glm::vec3 **verts, const Plane& plane, float &resultCoef01, float &resultCoef12, float &resultCoef20, const float eps = 0.0001f);

} // namespace
} // namespace

#endif // __MATHUTILS__