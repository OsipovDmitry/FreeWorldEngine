#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
using namespace GraphicsEngine;
#include <3rdparty/glm/gtc/matrix_transform.hpp>

std::string vShader =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"layout(location = 3) in vec3 normal;\n"\
"layout(location = 4) in vec2 texcoord;\n"\
"uniform mat4 viewProjMatrix;\n"\
"out vec3 n;\n"\
"out vec2 tc;\n"\
"void main(void)\n"\
"{\n"\
"	tc = texcoord;\n"\
"	n = normal;\n"\
"	gl_Position = viewProjMatrix * vec4 (pos, 1.0);\n"\
"}\n";

std::string fShader =
"#version 330 core\n"\
"uniform sampler2D tex;\n"\
"in vec3 n;\n"\
"in vec2 tc;\n"\
"layout(location = 0) out vec4 outColor;\n"\
"void main(void)\n"\
"{\n"\
"	outColor = texture(tex, tc) + vec4(n*0.0001, 0.0);\n"\
"}\n";


IGPUProgram *pProgram;
IGraphicsMaterial *pRenderMaterial;
IGraphicsCamera *pRenderCamera;

void render(IWindow*) {
}

void resize(int32 w, int32 h, IWindow*) {
}

void update(uint32 time, uint32 dt, IWindow*) {
}

int main() {
	initCoreEngine();
	ICore *pCore = getCoreEngine();
	pCore->initialize();
	
	IGraphicsEngine *pGraphics = pCore->graphicsEngine();

	pRenderCamera = pGraphics->createCamera();
	pRenderCamera->setPosition(glm::vec3(0, 0, 1));

	IImage *pImg = pCore->imageLoader()->load("box.jpg");
	uint32 offs[3] = { 0, 0, 0 };
	Renderer::IGPUTexture *pTexture = pCore->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg->data()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture->setSubData(offs, pImg->data()->size, TextureFormat::ChannelsCount_3, pImg->data()->type, pImg->data()->pData);

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
	pProgram = pCore->renderer()->createProgram();
	pProgram->attachShader(pVSh);
	pProgram->attachShader(pFSh);
	if (!pProgram->link()) {
		pCore->logger()->printMessage("Error!", ILogger::MessageType_Error);
	}

	pRenderMaterial = pGraphics->createMaterial(pProgram);
	pRenderMaterial->setUniform(pProgram->uniformLocationByName("tex"), pTexture);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("viewProjMatrix"), IGraphicsMaterial::AutoUniform_ViewProjectionMatrix);

	IGraphicsModel *pRenderModel = pGraphics->createModel();
	IScene *pScene = pCore->sceneLoader()->load("box.FBX");
	pRenderModel->setMesh(pScene->data()->meshes[0]->pMeshData);
	pRenderModel->setMaterial(pRenderMaterial);

	IGraphicsScene *pRenderScene = pGraphics->createScene();
	pRenderScene->rootNode()->setModel(pRenderModel);

	IGraphicsWindow *pRenderWindow = pCore->graphicsEngine()->createWindow(pCore->mainWindow());
	pRenderWindow->setCamera(pRenderCamera);
	pRenderWindow->setScene(pRenderScene);
	pCore->mainWindow()->registerUpdateCallBack(update);
	
	pCore->windowManager()->mainLoop();
	pCore->deinitialize();
	destroyCoreEngine();
	return 0;
}