#include <queue>
#include <utility>

#include <3rdparty/assimp/scene.h>
#include <3rdparty/assimp/postprocess.h>
#include <3rdparty/assimp/importerdesc.h>
#include <FreeWorldEngine.h>
#include <math/MeshWrapper.h>

#include "SceneLoaderPlugin.h"

namespace FreeWorldEngine {

Assimp::Importer SceneLoaderPlugin::m_assimpImporter;

SceneLoaderPlugin::SceneLoaderPlugin() :
	m_supportExtensions()
{
}

SceneLoaderPlugin::~SceneLoaderPlugin()
{
}

std::string SceneLoaderPlugin::name() const
{
	return std::string("SceneLoaderPlugin");
}

std::string SceneLoaderPlugin::info() const
{
	return std::string("Assimp Scene Loader Plugin");
}

bool SceneLoaderPlugin::initialize()
{
	ICore *pCore = ICore::instance();
	if (!pCore)
		return false;

	for (uint32 i = 0; i < m_assimpImporter.GetImporterCount(); ++i) {
		const aiImporterDesc *pDesc = m_assimpImporter.GetImporterInfo(i);
		std::string exts(pDesc->mFileExtensions);
		if (exts.empty())
			continue;
		size_t prev = 0, next;
		do {
			next = exts.find(' ', prev);
			m_supportExtensions.push_back(exts.substr(prev, next - prev));
			prev = next + 1;
		} while (next != std::string::npos);
	}

	std::for_each(m_supportExtensions.cbegin(), m_supportExtensions.cend(),
		[pCore](const std::string& s) { pCore->sceneLoader()->registerDataLoader(s, SceneLoaderPlugin::loadScene); });

	return true;
}

void SceneLoaderPlugin::deinitialize()
{
	ICore *pCore = ICore::instance();

	std::for_each(m_supportExtensions.cbegin(), m_supportExtensions.cend(),
		[pCore](const std::string& s) { pCore->sceneLoader()->unregisterDataLoader(s); });

	m_supportExtensions.clear();
}

void assimpColorToUint8Array(const aiColor4D& src, uint8 *pDst)
{
	pDst[0] = (uint8)(src.r * 255.0f + 0.5f);
	pDst[1] = (uint8)(src.g * 255.0f + 0.5f);
	pDst[2] = (uint8)(src.b * 255.0f + 0.5f);
	pDst[3] = (uint8)(src.a * 255.0f + 0.5f);
}

void assimpColorToUint8Array(const aiColor3D& src, uint8 *pDst)
{
	pDst[0] = (uint8)(src.r * 255.0f + 0.5f);
	pDst[1] = (uint8)(src.g * 255.0f + 0.5f);
	pDst[2] = (uint8)(src.b * 255.0f + 0.5f);
}

SceneData *SceneLoaderPlugin::loadScene(const std::string& filename)
{
	const aiScene *pAssimpScene = m_assimpImporter.ReadFile(filename, aiProcess_Triangulate);
	if (!pAssimpScene) {
		ICore::instance()->logger()->printMessage("Failed open file \"" + filename + "\".", ILogger::MessageType_Error);
		return 0;
	}

	SceneData *pScene = new SceneData;
	if (pAssimpScene->HasMeshes())
		for (uint32 meshIdx = 0; meshIdx < pAssimpScene->mNumMeshes; ++meshIdx) {
			aiMesh *pAssimpMesh = pAssimpScene->mMeshes[meshIdx];

			Math::MeshWrapper meshWrapper(new Mesh);
			if (pAssimpMesh->HasFaces()) {
				uint32 numIndices = 0;
				for (uint32 faceIdx = 0; faceIdx < pAssimpMesh->mNumFaces; numIndices += pAssimpMesh->mFaces[faceIdx++].mNumIndices);
				meshWrapper.addIndices(numIndices);
				uint32 *pIndices = meshWrapper.target()->pIndexData;
				for (uint32 faceIdx = 0; faceIdx < pAssimpMesh->mNumFaces; ++faceIdx) {
					const aiFace& face = pAssimpMesh->mFaces[faceIdx];
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
				meshWrapper.setAttributeDeclaration(VertexAttributeType_Binormal, 3, attribOffset + 3);
				attribOffset += 6;
			}
			uint32 texPass = 0;
			for (uint32 i = 0; i < 4; ++i)
				if (pAssimpMesh->HasTextureCoords(i)) {
					VertexAttributeType attribType = (VertexAttributeType)(VertexAttributeType_TexCoord0 + texPass++);
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
					VertexAttributeType attribType = (VertexAttributeType)(VertexAttributeType_TexCoord0 + texPass++);
					for (uint32 i = 0; i < pAssimpMesh->mNumVertices; ++i)
						meshWrapper.setAttributeValue(attribType, i, &(pAssimpMesh->mTextureCoords[slot][i].x));
				}

			switch (pAssimpMesh->mPrimitiveTypes) {
			case aiPrimitiveType_POINT: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Points); break; }
			case aiPrimitiveType_LINE: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Lines); break; }
			case aiPrimitiveType_TRIANGLE: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Triangles); break; }
			default: { meshWrapper.setPrimitiveFormat(PrimitiveFormat_Points); break; }
			}

			SceneData::Mesh *pSceneMesh = new SceneData::Mesh;
			pScene->meshes.push_back(pSceneMesh);

			pSceneMesh->name = pAssimpMesh->mName.C_Str();
			pSceneMesh->materialIndex = pAssimpMesh->mMaterialIndex;
			pSceneMesh->pMeshData = meshWrapper.target();
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

			SceneData::Material *pSceneMaterial = new SceneData::Material;
			pScene->materials.push_back(pSceneMaterial);

			aiString assimpMaterialName;
			if (aiGetMaterialString(pAssimpMaterial, AI_MATKEY_NAME, &assimpMaterialName) != AI_SUCCESS)
				assimpMaterialName = "unnamed";
			pSceneMaterial->name = assimpMaterialName.C_Str();
			pSceneMaterial->pMaterialData = pMaterial;
		}

	if (pAssimpScene->HasLights()) {
		for (uint32 lIdx = 0; lIdx < pAssimpScene->mNumLights; ++lIdx) {
			aiLight *pAssimpLight = pAssimpScene->mLights[lIdx];

			if (pAssimpLight->mType == aiLightSource_UNDEFINED || pAssimpLight->mType == aiLightSource_AMBIENT)
				continue;

			Light *pLight = new Light;

			switch (pAssimpLight->mType) {
			case aiLightSource_DIRECTIONAL: { pLight->type = Light::Type_Directional; break; }
			case aiLightSource_POINT: { pLight->type = Light::Type_Point; break; }
			case aiLightSource_SPOT: { pLight->type = Light::Type_Spot; break; }
			}

			pLight->position = glm::vec3(pAssimpLight->mPosition.x, pAssimpLight->mPosition.y, pAssimpLight->mPosition.z);
			pLight->direction = glm::vec3(pAssimpLight->mDirection.x, pAssimpLight->mDirection.y, pAssimpLight->mDirection.z);
			pLight->attenuationCoefficent = glm::vec3(pAssimpLight->mAttenuationConstant, pAssimpLight->mAttenuationLinear, pAssimpLight->mAttenuationQuadratic);
			pLight->innerCone = pAssimpLight->mAngleInnerCone;
			pLight->outerCone = pAssimpLight->mAngleOuterCone;
			assimpColorToUint8Array(pAssimpLight->mColorAmbient, pLight->ambientColor);
			assimpColorToUint8Array(pAssimpLight->mColorDiffuse, pLight->diffuseColor);
			assimpColorToUint8Array(pAssimpLight->mColorSpecular, pLight->specularColor);

			SceneData::Ligth *pSceneLight = new SceneData::Ligth;
			pScene->lights.push_back(pSceneLight);

			pSceneLight->name = pAssimpLight->mName.C_Str();
			pSceneLight->pLightData = pLight;
		}
	}

	if (pAssimpScene->mRootNode) {
		std::queue<std::pair<aiNode*, Utility::Tree<SceneData::NodeData*>::Node*> > nodesQueue;
		nodesQueue.push(std::make_pair(pAssimpScene->mRootNode, pScene->treeNodes.rootNode()));
		while (!nodesQueue.empty()) {
			aiNode *pAssimpNode = nodesQueue.front().first;
			Utility::Tree<SceneData::NodeData*>::Node *pTreeNode = nodesQueue.front().second;
			nodesQueue.pop();

			SceneData::NodeData *pNode = new SceneData::NodeData;
			pTreeNode->setData(pNode);

			pNode->name = pAssimpNode->mName.C_Str();
			pNode->meshesIndices.resize(pAssimpNode->mNumMeshes);
			std::copy(pAssimpNode->mMeshes, pAssimpNode->mMeshes + pAssimpNode->mNumMeshes, pNode->meshesIndices.begin());
			
			aiVector3D posNode;
			aiQuaternion orientNode;
			pAssimpNode->mTransformation.DecomposeNoScaling(orientNode, posNode);
			pNode->position = glm::vec3(posNode.x, posNode.y, posNode.z);
			pNode->orientation = glm::quat(orientNode.w, orientNode.x, orientNode.y, orientNode.z);

			for (uint32 i = 0; i < pAssimpNode->mNumChildren; ++i) {
				Utility::Tree<SceneData::NodeData*>::Node *p = pTreeNode->addChild();
				nodesQueue.push(std::make_pair(pAssimpNode->mChildren[i], p));
			}
		}
	}

	m_assimpImporter.FreeScene();
	return pScene;
}

} // namespace
