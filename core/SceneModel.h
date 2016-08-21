#ifndef __SCENEMODEL__
#define __SCENEMODEL__

#include <string>

#include "ISceneModel.h"
#include "assimp-3.2/include/assimp/scene.h"

namespace FreeWorldEngine {

struct MeshCache {
	glm::mat4 transform;
};

struct MaterialCache {
	std::string name;
	std::string diffuseMap;
};

class SceneModel : public ISceneModel {
public:
	SceneModel(const std::string& name, const aiScene *scene);
	~SceneModel();
	std::string name() const;

	uint32 numMeshes() const;
	uint32 numMaterials() const;
	
	uint32 meshNumVertices(const uint32 meshIndex) const;
	uint32 meshNumIndices(const uint32 meshIndex) const;

	bool meshHasVertices(const uint32 meshIndex) const;
	bool meshHasTangentsAndBinormals(const uint32 meshIndex) const;
	bool meshHasNormals(const uint32 meshIndex) const;
	bool meshHasUV(const uint32 meshIndex, const uint32 uvIndex = 0) const;

	const char *meshName(const uint32 meshIndex) const;
	glm::mat4 meshModelMatrix(const uint32 meshIndex) const;
	uint32 meshMaterialIndex(const uint32 meshIndex) const;

	bool meshBuildVertexBuffer(const uint32 meshIndex, const VertexFormat& vertexFormat, float *ptr) const;
	bool meshBuildIndexBuffer(const uint32 meshIndex, uint32 *ptr) const;

	const char *materialName(const uint32 matIndex) const;
	const char *materialDiffuseTexture(const uint32 matIndex, const uint32 texIndex = 0) const;

private:
	std::string m_name;
	const aiScene *m_scene;
	MeshCache *m_cachedMeshes;
	MaterialCache *m_cachedMaterials;

}; // class SceneModel 

} // namespace

#endif // __SCENEMODEL__