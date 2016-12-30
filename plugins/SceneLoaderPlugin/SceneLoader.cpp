#include <queue>
#include <utility>

#include <3rdparty/assimp/scene.h>
#include <3rdparty/assimp/postprocess.h>

#include <FreeWorldEngine.h>
#include <math/MeshWrapper.h>

#include "SceneLoader.h"
#include "Scene.h"

namespace FreeWorldEngine {

void assimpColorToUint8Array(const aiColor4D& src, uint8 *pDst) {
	pDst[0] = (uint8)(src.r * 255.0f + 0.5f);
	pDst[1] = (uint8)(src.g * 255.0f + 0.5f);
	pDst[2] = (uint8)(src.b * 255.0f + 0.5f);
	pDst[3] = (uint8)(src.a * 255.0f + 0.5f);
}

SceneLoader::SceneLoader() :
	m_pResourceManager(getCoreEngine()->createResourceManager("ResourceManagerForScenes")),
	m_assimpImporter()
{
}

SceneLoader::~SceneLoader()
{
	getCoreEngine()->destroyResourceManager(m_pResourceManager);
}

IScene *SceneLoader::loadScene(const std::string& filename)
{
	IScene *pResScene = findScene(filename);
	if (pResScene)
		return pResScene;
	
	const aiScene *pAssimpScene = m_assimpImporter.ReadFile(filename, aiProcess_Triangulate);
	if (!pAssimpScene) {
		getCoreEngine()->logger()->printMessage("Failed open file \"" + filename + "\".", ILogger::MessageType_Error); 
		return 0;
	}

	Scene *pScene = new Scene;
	if (pAssimpScene->HasMeshes())
		for (uint32 meshIdx = 0; meshIdx < pAssimpScene->mNumMeshes; ++meshIdx) {
			aiMesh *pAssimpMesh = pAssimpScene->mMeshes[meshIdx];

			Math::MeshWrapper meshWrapper(new Mesh);
			if (pAssimpMesh->HasFaces()) {
				uint32 numIndices = 0;
				for (uint32 faceIdx = 0; faceIdx < pAssimpMesh->mNumFaces; numIndices += pAssimpMesh->mFaces[faceIdx++].mNumIndices) ;
				meshWrapper.addIndices(numIndices);
				uint32 *pIndices = meshWrapper.targetMesh()->pIndexData;
				for (uint32 faceIdx = 0; faceIdx < pAssimpMesh->mNumFaces; ++faceIdx) {
					const aiFace& face = pAssimpMesh->mFaces[faceIdx++];
					memcpy(pIndices, face.mIndices, face.mNumIndices*sizeof(face.mIndices[0]));
					pIndices += face.mNumIndices;
				}
			}
			
			uint16 vertexStride = 0;
			vertexStride += pAssimpMesh->HasPositions() ? 3 : 0;
			vertexStride += pAssimpMesh->HasNormals() ? 3 : 0;
			vertexStride += pAssimpMesh->HasTangentsAndBitangents() ? 6 : 0;
			for (uint32 i = 0; i < 4; ++i)
				vertexStride += pAssimpMesh->mNumUVComponents[i];
			meshWrapper.setVertexStride(vertexStride);

			uint32 attribOffset = 0;
			if (pAssimpMesh->HasPositions()) {
				meshWrapper.setAttributeDeclaration(VertexAttributeType_Position, 3, attribOffset);
				attribOffset += 3;
			}
			if (pAssimpMesh->HasNormals()) {
				meshWrapper.setAttributeDeclaration(VertexAttributeType_Normal, 3, attribOffset);
				attribOffset += 3;
			}
			if (pAssimpMesh->HasTangentsAndBitangents()) {
				meshWrapper.setAttributeDeclaration(VertexAttributeType_Tangent, 3, attribOffset);
				meshWrapper.setAttributeDeclaration(VertexAttributeType_Binormal, 3, attribOffset+3);
				attribOffset += 6;
			}
			uint32 texPass = 0;
			for (uint32 i = 0; i < 4; ++i)
				if (pAssimpMesh->HasTextureCoords(i)) {
					VertexAttributeType attribType = (VertexAttributeType)(VertexAttributeType_TexCoord0+texPass++);
					meshWrapper.setAttributeDeclaration(attribType, pAssimpMesh->mNumUVComponents[i], attribOffset);
					attribOffset += pAssimpMesh->mNumUVComponents[i];
				}

			meshWrapper.addVertices(pAssimpMesh->mNumVertices);
			if (pAssimpMesh->HasPositions())
				for (uint32 i = 0; i < pAssimpMesh->mNumVertices; ++i)
					meshWrapper.setAttributeValue(VertexAttributeType_Position, i, &(pAssimpMesh->mVertices[i].x));
			if (pAssimpMesh->HasNormals())
				for (uint32 i = 0; i < pAssimpMesh->mNumVertices; ++i)
					meshWrapper.setAttributeValue(VertexAttributeType_Normal, i, &(pAssimpMesh->mNormals[i].x));
			if (pAssimpMesh->HasTangentsAndBitangents())
				for (uint32 i = 0; i < pAssimpMesh->mNumVertices; ++i) {
					meshWrapper.setAttributeValue(VertexAttributeType_Tangent, i, &(pAssimpMesh->mTangents[i].x));
					meshWrapper.setAttributeValue(VertexAttributeType_Binormal, i, &(pAssimpMesh->mBitangents[i].x));
				}
			texPass = 0;
			for (int slot = 0; slot < 4; ++slot)
				if (pAssimpMesh->HasTextureCoords(slot)) {
					VertexAttributeType attribType = (VertexAttributeType)(VertexAttributeType_TexCoord0+texPass++);
					for (uint32 i = 0; i < pAssimpMesh->mNumVertices; ++i)
						meshWrapper.setAttributeValue(attribType, i, &(pAssimpMesh->mTextureCoords[slot][i].x));
				}

			switch (pAssimpMesh->mPrimitiveTypes) {
			case aiPrimitiveType_POINT: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Points); break; }
			case aiPrimitiveType_LINE: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Lines); break; }
			case aiPrimitiveType_TRIANGLE: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Trangles); break; }
			default: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Points); break; }
			}

			Scene::Mesh *pSceneMesh = new Scene::Mesh;
			pScene->meshes.push_back(pSceneMesh);

			pSceneMesh->name = pAssimpMesh->mName.C_Str();
			pSceneMesh->materialIndex = pAssimpMesh->mMaterialIndex;
			pSceneMesh->pMeshData = meshWrapper.targetMesh();
		}

	if (pAssimpScene->HasMaterials())
		for (uint32 matIdx = 0; matIdx < pAssimpScene->mNumMaterials; ++matIdx) {
			aiMaterial *pAssimpMaterial = pAssimpScene->mMaterials[matIdx];
			Material *pMaterial = new Material;
			aiColor4D assimpColor;

			if (aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_AMBIENT, &assimpColor) != AI_SUCCESS)
				assimpColor = aiColor4D(0.2f, 0.2f, 0.2f, 1.0f);
			assimpColorToUint8Array(assimpColor, pMaterial->ambientColor);

			if (aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_DIFFUSE, &assimpColor) != AI_SUCCESS)
				assimpColor = aiColor4D(0.8f, 0.8f, 0.8f, 1.0f);
			assimpColorToUint8Array(assimpColor, pMaterial->diffuseColor);

			if (aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_SPECULAR, &assimpColor) != AI_SUCCESS)
				assimpColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
			assimpColorToUint8Array(assimpColor, pMaterial->specularColor);

			if (aiGetMaterialColor(pAssimpMaterial, AI_MATKEY_COLOR_EMISSIVE, &assimpColor) != AI_SUCCESS)
				assimpColor = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
			assimpColorToUint8Array(assimpColor, pMaterial->emissionColor);

			float shininess, strength;
			uint32 max = 1;
			aiReturn ret1 = aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_SHININESS, &shininess, &max);
			max = 1;
			aiReturn ret2 = aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
			pMaterial->shininess = ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS)) ? shininess * strength : 0.0f;

			float opacity;
			max = 1;
			pMaterial->opacity = (aiGetMaterialFloatArray(pAssimpMaterial, AI_MATKEY_SHININESS, &opacity, &max) == AI_SUCCESS) ? opacity : 1.0f;

			int two_sided;
			max = 1;
			pMaterial->isTwoSided = (aiGetMaterialIntegerArray(pAssimpMaterial, AI_MATKEY_TWOSIDED, &two_sided, &max) == AI_SUCCESS) ? two_sided != 0 : false;

			aiString texPath;
			pMaterial->ambientMap = (pAssimpMaterial->GetTexture(aiTextureType_AMBIENT, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->diffuseMap = (pAssimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->specularMap = (pAssimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->emissionMap = (pAssimpMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->heightMap = (pAssimpMaterial->GetTexture(aiTextureType_HEIGHT, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->normalMap = (pAssimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->shininessMap = (pAssimpMaterial->GetTexture(aiTextureType_SHININESS, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";
			pMaterial->opacityMap = (pAssimpMaterial->GetTexture(aiTextureType_OPACITY, 0, &texPath) == AI_SUCCESS) ? texPath.C_Str() : "";

			Scene::Material *pSceneMaterial = new Scene::Material;
			pScene->materials.push_back(pSceneMaterial);

			aiString assimpMaterialName;
			if (aiGetMaterialString(pAssimpMaterial, AI_MATKEY_NAME, &assimpMaterialName) != AI_SUCCESS)
				assimpMaterialName = "unnamed";
			pSceneMaterial->name = assimpMaterialName.C_Str();
			pSceneMaterial->pMaterialData = pMaterial;
		}

	if (pAssimpScene->mRootNode) {
		std::queue<std::pair<aiNode*, Scene::Node*> > nodesQueue;
		nodesQueue.push(std::make_pair(pAssimpScene->mRootNode, pScene->pRootNode = new Scene::Node));
		while (!nodesQueue.empty()) {
			aiNode *pAssimpNode = nodesQueue.front().first;
			Scene::Node *pNode = nodesQueue.front().second;
			nodesQueue.pop();

			pNode->name = pAssimpNode->mName.C_Str();
			pNode->meshesIndices.resize(pAssimpNode->mNumMeshes);
			for (uint32 i = 0; i < pAssimpNode->mNumMeshes; ++i)
				pNode->meshesIndices[i] = pAssimpNode->mMeshes[i];
			aiMatrix4x4& m = pAssimpNode->mTransformation;
			pNode->transform = glm::mat4(m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4);

			pNode->childNodes.resize(pAssimpNode->mNumChildren);
			for (uint32 i = 0; i < pAssimpNode->mNumChildren; ++i)
				nodesQueue.push(std::make_pair(pAssimpNode->mChildren[i], pNode->childNodes[i] = new Scene::Node(pNode)));
		}
	}

	m_assimpImporter.FreeScene();
	pResScene = new SceneContainer(filename, pScene);
	m_pResourceManager->addResource(pResScene);
	return pResScene;
}

IScene *SceneLoader::findScene(const std::string& name) const
{
	return static_cast<IScene*>(m_pResourceManager->findResource(name));
}

void SceneLoader::destoryScene(const std::string& name)
{
	m_pResourceManager->destroyResource(name);
}

void SceneLoader::destoryScene(IScene *pScene)
{
	m_pResourceManager->destroyResource(pScene);
}

} // namespace