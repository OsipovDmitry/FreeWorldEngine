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

void cutLine(glm::vec3 **verts, const Plane& plane, float &resultCoef)
{
	resultCoef = -1.0f;

	float dists[2] = { distToPlane(plane, *(verts[0])), distToPlane(plane, *(verts[1])) };

	if ((dists[0] >= -eps && dists[1] >= -eps) ||
		(dists[0] <= eps && dists[1] <= eps)) // ��� ����� ����� �� ���� ������� �� ��������� ��� ����� �� ����, �������� ��� �� ���������. ������ �� ����.
		return;

	resultCoef = abs(dists[0]) / abs(dists[0] - dists[1]);
}

void cutTriangle(glm::vec3 **verts, const Plane& plane, float &resultCoef01, float &resultCoef12, float &resultCoef20)
{
	resultCoef01 = resultCoef12 = resultCoef20 = -1.0f;

	float dists[3] = { distToPlane(plane, *(verts[0])), distToPlane(plane, *(verts[1])), distToPlane(plane, *(verts[2])) };

	if ((dists[0] >= -eps && dists[1] >= -eps && dists[2] >= -eps) ||
		(dists[0] <= eps && dists[1] <= eps && dists[2] <= eps)) // ��� ����� ����� �� ���� ������� �� ��������� ��� ����� �� ����, �������� ��� �� ���������. ������ �� ����.
		return;

	float absDists[3] = { abs(dists[0]), abs(dists[1]), abs(dists[2]) };

	if (absDists[0] < eps) { // ���� ���� ����� ����� �� ���������, �� ����� �������������� �����.
		resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
		return;
	}

	if (absDists[1] < eps) { // ���� ���� ����� ����� �� ���������, �� ����� �������������� �����.
		resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
		return;
	}

	if (absDists[2] < eps) { // ���� ���� ����� ����� �� ���������, �� ����� �������������� �����.
		resultCoef01 = absDists[0] / (absDists[0] + absDists[1]);
		return;
	}

	if (dists[0]*dists[1] < 0.0f) { // ����� 0 � 1 ����� �� ������ �������
		resultCoef01 = absDists[0] / (absDists[0] + absDists[1]);
		if (dists[0]*dists[2] < 0.0f) // ����� 0 � 2 ����� �� ������ ������� => ����� 0 ����� �� ������ ������� �� ����� 1 � 2
			resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
		else  // ����� 1 ����� �� ������ ������� �� ����� 2 � 0
			resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
	} else { // ����� 2, ����� �� ������ ������� �� ����� 0 � 1
		resultCoef12 = absDists[1] / (absDists[1] + absDists[2]);
		resultCoef20 = absDists[2] / (absDists[2] + absDists[0]);
	}
}

} // namespace
} // namespace
