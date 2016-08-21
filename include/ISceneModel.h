#ifndef __ISCENEMODEL__
#define __ISCENEMODEL__

#include "IResource.h"
#include "Types.h"

#include "glm/mat4x4.hpp"

namespace FreeWorldEngine {

class ISceneModel : public IResource {
public:
	~ISceneModel() = 0 {}

	virtual uint32 numMeshes() const = 0;
	virtual uint32 numMaterials() const = 0;

	virtual uint32 meshNumVertices(const uint32 meshIndex) const = 0;
	virtual uint32 meshNumIndices(const uint32 meshIndex) const = 0;

	virtual bool meshHasVertices(const uint32 meshIndex) const = 0;
	virtual bool meshHasTangentsAndBinormals(const uint32 meshIndex) const = 0;
	virtual bool meshHasNormals(const uint32 meshIndex) const = 0;
	virtual bool meshHasUV(const uint32 meshIndex, const uint32 uvIndex = 0) const = 0;

	virtual const char *meshName(const uint32 meshIndex) const = 0;
	virtual glm::mat4 meshModelMatrix(const uint32 meshIndex) const = 0;
	virtual uint32 meshMaterialIndex(const uint32 meshIndex) const = 0;

	virtual bool meshBuildVertexBuffer(const uint32 meshIndex, const VertexFormat& vertexFormat, float *ptr) const = 0;
	virtual bool meshBuildIndexBuffer(const uint32 meshIndex, uint32 *ptr) const = 0;

	virtual const char *materialName(const uint32 matIndex) const = 0;
	virtual const char *materialDiffuseTexture(const uint32 matIndex, const uint32 texIndex = 0) const = 0;

}; // class ISceneModel 

} // namespace

#endif // __ISCENEMODEL__