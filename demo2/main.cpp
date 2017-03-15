#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
using namespace GraphicsEngine;
#include <3rdparty/glm/gtc/matrix_transform.hpp>

std::string vShader =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"layout(location = 4) in vec2 texcoord;\n"\
"uniform mat4 modelViewProjMatrix;\n"\
"out vec2 tc;\n"\
"void main(void)\n"\
"{\n"\
"	tc = texcoord;\n"\
"	gl_Position = modelViewProjMatrix * vec4(pos, 1.0);\n"\
"}\n";

std::string fShader =
"#version 330 core\n"\
"uniform sampler2D tex;\n"\
"in vec2 tc;\n"\
"layout(location = 0) out vec4 outColor;\n"\
"void main(void)\n"\
"{\n"\
"	outColor = texture(tex, tc);\n"\
"}\n";

int main() {
	initCoreEngine();
	ICore *pCore = getCoreEngine();
	pCore->initialize();

	IImage *pImg = pCore->imageLoader()->load("ASM4102.jpg");
	IScene *pScene = pCore->sceneLoader()->load("ASM4102.3DS");

	IImage *pImg2 = pCore->imageLoader()->load("AGVT150.jpg");
	IScene *pScene2 = pCore->sceneLoader()->load("AGVT150.3DS");

	IGraphicsEngine *pGraphics = pCore->graphicsEngine();

	IGraphicsCamera *pRenderCamera = pGraphics->createCamera();
	pRenderCamera->setPosition(glm::vec3(-0.5, -0.9f, 0.0f));
	pRenderCamera->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(),0.5f,0.0f)));

	uint32 offs[3] = { 0, 0, 0 };
	Renderer::IGPUTexture *pTexture = pCore->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg->data()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture->setSubData(offs, pImg->data()->size, TextureFormat::ChannelsCount_3, pImg->data()->type, pImg->data()->pData);
	pTexture->generateMipMaps();
	pTexture->setMinFilter(IGPUTexture::IGPUTextureMinFilter_LinearMipmapLinear);
	pTexture->setMagFilter(IGPUTexture::IGPUTextureMagFilter_Linear);

	Renderer::IGPUTexture *pTexture2 = pCore->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg2->data()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture2->setSubData(offs, pImg2->data()->size, TextureFormat::ChannelsCount_3, pImg2->data()->type, pImg2->data()->pData);
	pTexture2->generateMipMaps();
	pTexture2->setMinFilter(IGPUTexture::IGPUTextureMinFilter_LinearMipmapLinear);
	pTexture2->setMagFilter(IGPUTexture::IGPUTextureMagFilter_Linear);

	IGPUShader *pVSh = pCore->renderer()->createShader(IGPUShader::IGPUShaderType_Vertex);
	pVSh->loadFromData(vShader);
	if (!pVSh->compile()) {
		pCore->logger()->printMessage("Error!", ILogger::MessageType_Error);
	}
	IGPUShader *pFSh = pCore->renderer()->createShader(IGPUShader::IGPUShaderType_Fragment);
	pFSh->loadFromData(fShader);
	if (!pFSh->compile()) {
		pCore->logger()->printMessage("Error!", ILogger::MessageType_Error);
	}
	IGPUProgram *pProgram = pCore->renderer()->createProgram();
	pProgram->attachShader(pVSh);
	pProgram->attachShader(pFSh);
	if (!pProgram->link()) {
		pCore->logger()->printMessage("Error!", ILogger::MessageType_Error);
	}

	IGraphicsMaterial *pRenderMaterial = pGraphics->materialManager()->createMaterial(pProgram);
	pRenderMaterial->setUniform(pProgram->uniformLocationByName("tex"), pTexture);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);

	IGraphicsMaterial *pRenderMaterial2 = pGraphics->materialManager()->createMaterial(pProgram);
	pRenderMaterial2->setUniform(pProgram->uniformLocationByName("tex"), pTexture2);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);


	IGraphicsScene *pRenderScene = pGraphics->createScene();
	IGraphicsSceneNode *pRootNode = pRenderScene->rootNode();

	for (auto it = pScene->data()->treeNodes.beginDepth(); it != pScene->data()->treeNodes.endDepth(); ++it) {
		SceneData::NodeData *p = (*it)->data();
		for (int i = 0; i < p->meshesIndices.size(); ++i) {
			IGraphicsModel *pRenderModel = pGraphics->createModel();
			pRenderModel->setMesh(pScene->data()->meshes[p->meshesIndices.at(i)]->pMeshData);
			pRenderModel->setMaterial(pRenderMaterial);
			IGraphicsSceneNode *pNode = pRootNode->createChild();
			pNode->setModel(pRenderModel);
			pNode->setPosition(p->position + glm::vec3(0.3f,0,0));
			pNode->setOrientation(p->orientation);
		}
	}

	for (auto it = pScene2->data()->treeNodes.beginDepth(); it != pScene2->data()->treeNodes.endDepth(); ++it) {
		SceneData::NodeData *p = (*it)->data();
		for (int i = 0; i < p->meshesIndices.size(); ++i) {
			IGraphicsModel *pRenderModel = pGraphics->createModel();
			pRenderModel->setMesh(pScene2->data()->meshes[p->meshesIndices.at(i)]->pMeshData);
			pRenderModel->setMaterial(pRenderMaterial2);
			IGraphicsSceneNode *pNode = pRootNode->createChild();
			pNode->setModel(pRenderModel);
			pNode->setPosition(p->position - glm::vec3(0.3f, 0, 0));
			pNode->setOrientation(p->orientation);
		}
	}
	
	//pRootNode->setOrientation(glm::quat(glm::vec3(-0.78f, 0, 0)));

	IGraphicsWindow *pRenderWindow = pCore->graphicsEngine()->createWindow(pCore->mainWindow());
	pRenderWindow->setCamera(pRenderCamera);
	pRenderWindow->setScene(pRenderScene);

	pCore->windowManager()->mainLoop();
	pCore->deinitialize();
	destroyCoreEngine();
	return 0;
}