#ifndef __MESHPAINTER__
#define __MESHPAINTER__

#include "../Types.h"
#include "MathSettings.h"

namespace FreeWorldEngine {
namespace Math {

class MeshWrapper;

class MATH_EXPORT MeshPainter {
public:
	MeshPainter(Mesh *pTargetMesh = nullptr);
	MeshPainter(const MeshWrapper& wrapper);
	~MeshPainter();

	// Only targetMesh with primitveFormat are Points or Lines or Triangles!
	void setTarget(Mesh *pTargetMesh);
	Mesh *target() const;

	void paintEllipse(const float radiusX, const float radiusY, const float radiusZ, const uint32 numLongs, const uint32 numLats);
	void paintSphere(const float radius, const uint32 numSegs);
	void paintBox(const float lenX, const float lenY, const float lenZ);
	void paintCube(const float len);
	void paintQuad(const float lenX, const float lenY);

private:
	Mesh *m_pMesh;

}; // class MeshWrapper

} // namespace

} // namespace

#endif // __MESHPAINTER__
