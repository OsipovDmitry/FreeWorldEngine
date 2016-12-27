#include "SceneModel.h"
#include "Common.h"

#include <queue>

namespace FreeWorldEngine {

SceneModel::SceneModel(const std::string& name, const aiScene *scene) :
	m_name(name),
	m_scene(scene)
{
	if (m_scene->HasMeshes()) {
		m_cachedMeshes = new MeshCache [m_scene->mNumMeshes];
		std::queue<std::pair<aiNode*, aiMatrix4x4>> nodes;
		nodes.push(std::pair<aiNode*, aiMatrix4x4>(m_scene->mRootNode, aiMatrix4x4()));
		while (!nodes.empty()) {
			std::pair<aiNode*, aiMatrix4x4> data = nodes.front();
			nodes.pop();
			aiNode *pNode = data.first;
			aiMatrix4x4 m = data.second * pNode->mTransformation; // current transformation
			for (uint32 i = 0; i < pNode->mNumMeshes; ++i)
				m_cachedMeshes[pNode->mMeshes[i]].transform = glm::mat4(m.a1, m.b1, m.c1, m.d1,
																		m.a2, m.b2, m.c2, m.d2,
																		m.a3, m.b3, m.c3, m.d3,
																		m.a4, m.b4, m.c4, m.d4);
			for (uint32 i = 0; i < pNode->mNumChildren; ++i)
				nodes.push(std::pair<aiNode*, aiMatrix4x4>(pNode->mChildren[i], m));
		}
	}
	else
		m_cachedMeshes = 0;

	if (m_scene->HasMaterials()) {
		m_cachedMaterials = new MaterialCache [m_scene->mNumMaterials];
		for (uint32 i = 0; i < m_scene->mNumMaterials; ++i) {
			aiString str;
			m_cachedMaterials[i].diffuseMap = (m_scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS) ? str.C_Str() : "";
		}
	}
	else
		m_cachedMaterials = 0;
}

SceneModel::~SceneModel()
{
	if (m_cachedMeshes)
		delete [] m_cachedMeshes;
	if (m_cachedMaterials)
		delete [] m_cachedMaterials;
}

std::string SceneModel::name() const
{
	return m_name;
}

uint32 SceneModel::numMeshes() const
{
	return m_scene->mNumMeshes;
}

uint32 SceneModel::numMaterials() const
{
	return m_scene->mNumMaterials;
}

uint32 SceneModel::meshNumVertices(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->mNumVertices;
}

uint32 SceneModel::meshNumIndices(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->mNumFaces * 3;
}

bool SceneModel::meshHasVertices(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->HasPositions();
}

bool SceneModel::meshHasTangentsAndBinormals(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->HasTangentsAndBitangents();
}

bool SceneModel::meshHasNormals(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->HasNormals();
}

bool SceneModel::meshHasUV(const uint32 meshIndex, const uint32 uvIndex) const
{
	return m_scene->mMeshes[meshIndex]->HasTextureCoords(uvIndex);
}

const char *SceneModel::meshName(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->mName.C_Str();
}

glm::mat4 SceneModel::meshModelMatrix(const uint32 meshIndex) const
{
	return m_cachedMeshes[meshIndex].transform;
}

uint32 SceneModel::meshMaterialIndex(const uint32 meshIndex) const
{
	return m_scene->mMeshes[meshIndex]->mMaterialIndex;
}

bool SceneModel::meshBuildVertexBuffer(const uint32 meshIndex, const VertexFormat& vertexFormat, float *ptr) const
{
	uint32 vSize = calcVertexSize(vertexFormat);
	if (!vSize)
		return false;
	
	aiMesh *pMesh = m_scene->mMeshes[meshIndex];
	memset(ptr, 0, vSize*pMesh->mNumVertices*sizeof(float));

	if (pMesh->HasPositions() && vertexFormat.positionSize >= 3) {
		aiVector3D *pVerts = pMesh->mVertices;
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.positionStride, pVerts + i, 3 * sizeof(float)); 
	}

	if (pMesh->HasTangentsAndBitangents() && vertexFormat.tangentSize >= 3) {
		aiVector3D *pTangents = pMesh->mTangents;
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.tangentStride, pTangents + i, 3 * sizeof(float)); 
	}

	if (pMesh->HasTangentsAndBitangents() && vertexFormat.binormalSize >= 3) {
		aiVector3D *pBinormal = pMesh->mBitangents;
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.binormalStride, pBinormal + i, 3 * sizeof(float)); 
	}

	if (pMesh->HasNormals() && vertexFormat.normalSize >= 3) {
		aiVector3D *pNormal = pMesh->mNormals;
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.normalStride, pNormal + i, 3 * sizeof(float)); 
	}

	if (pMesh->HasTextureCoords(0) && vertexFormat.texCoord0Size > 0 && vertexFormat.texCoord0Size <= 3) {
		aiVector3D *pUV = pMesh->mTextureCoords[0];
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.texCoord0Stride, pUV + i, vertexFormat.texCoord0Size * sizeof(float)); 
	}

	if (pMesh->HasTextureCoords(1) && vertexFormat.texCoord1Size > 0 && vertexFormat.texCoord1Size <= 3) {
		aiVector3D *pUV = pMesh->mTextureCoords[1];
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.texCoord1Stride, pUV + i, vertexFormat.texCoord1Size * sizeof(float)); 
	}

	if (pMesh->HasTextureCoords(2) && vertexFormat.texCoord2Size > 0 && vertexFormat.texCoord2Size <= 3) {
		aiVector3D *pUV = pMesh->mTextureCoords[2];
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.texCoord2Stride, pUV + i, vertexFormat.texCoord2Size * sizeof(float)); 
	}

	if (pMesh->HasTextureCoords(3) && vertexFormat.texCoord3Size > 0 && vertexFormat.texCoord3Size <= 3) {
		aiVector3D *pUV = pMesh->mTextureCoords[3];
		for (uint32 i = 0; i < pMesh->mNumVertices; ++i)
			memcpy(ptr + i*vSize + vertexFormat.texCoord3Stride, pUV + i, vertexFormat.texCoord3Size * sizeof(float)); 
	}

	return true;
}

bool SceneModel::meshBuildIndexBuffer(const uint32 meshIndex, uint32 *ptr) const
{
	aiMesh *pMesh = m_scene->mMeshes[meshIndex];
	if (!pMesh->HasFaces())
		return false;

	for (uint32 i = 0; i < pMesh->mNumFaces; ++i)
		memcpy(ptr + i*3, pMesh->mFaces[i].mIndices, 3*sizeof(uint32));

	return true;
}

const char *SceneModel::materialName(const uint32 matIndex) const
{
	return m_cachedMaterials[matIndex].name.c_str();
}

const char *SceneModel::materialDiffuseTexture(const uint32 matIndex, const uint32 texIndex) const
{
	return m_cachedMaterials[matIndex].diffuseMap.c_str();
}

} // namespace