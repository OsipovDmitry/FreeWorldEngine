#ifndef __IMESHBUILDER__
#define __IMESHBUILDER__

#include "Types.h"

namespace FreeWorldEngine {

class IMeshBuilder {
public:
	virtual ~IMeshBuilder() = 0 {}

	virtual void setTargetMesh(Mesh *pMesh) = 0;
	virtual void buildSphere(float radius, uint32 numSegments) const = 0;

	virtual void addVertices(uint32 numVertices) const = 0;
	virtual void addIndices(uint32 numIndices) const = 0;
};

} // namespace

#endif // __IMESHBUILDER__