#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
using namespace GraphicsEngine;
#include <3rdparty/glm/gtc/matrix_transform.hpp>
#include <utility/File.h>
#include <utility/Tree.h>
#include <math/MeshWrapper.h>

ICore *pCore = 0;
IGraphicsWindow *pRenderWindow = 0;
IGraphicsCamera *pRenderCamera = 0;
float cam_lat = 0, cam_long = 0;
glm::vec3 cam_pos(0.0f, 0.0f, 0.0f);
std::string errorLog;

void update(uint32 time, uint32 dt, IWindow*) {
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Left))
		cam_long -= glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Right))
		cam_long += glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Up))
		cam_lat -= glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Down))
		cam_lat += glm::half_pi<float>() * dt * 0.001f;

	const float cam_move_speed = 300.0f;

	if (pCore->windowManager()->keyState(IWindow::KeyCode_W))
		cam_pos -= pRenderCamera->backwardDirection() * (cam_move_speed * dt * 0.001f);
	if (pCore->windowManager()->keyState(IWindow::KeyCode_S))
		cam_pos += pRenderCamera->backwardDirection() * (cam_move_speed * dt * 0.001f);
	if (pCore->windowManager()->keyState(IWindow::KeyCode_A))
		cam_pos -= pRenderCamera->rightDirection() * (cam_move_speed * dt * 0.001f);
	if (pCore->windowManager()->keyState(IWindow::KeyCode_D))
		cam_pos += pRenderCamera->rightDirection() * (cam_move_speed * dt * 0.001f);

	pRenderCamera->setOrientation(glm::mat3x3(glm::rotate(glm::mat4x4(), cam_lat, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4x4(), cam_long, glm::vec3(0.0f, 1.0f, 0.0f))));
	pRenderCamera->setPosition(cam_pos);
}

int main() {
	initCoreEngine();
	pCore = getCoreEngine();
	pCore->initialize();

	pCore->mainWindow()->registerUpdateCallBack(update);

	IImage *pImg = pCore->imageLoader()->load("mi24.jpg");
	IImage *pImg2 = pCore->imageLoader()->load("c300.jpg");

	IScene *pScene = pCore->sceneLoader()->load("mi24.3DS");
	IScene *pScene2 = pCore->sceneLoader()->load("c300.3DS");

	IGraphicsEngine *pGraphics = pCore->graphicsEngine();

	pRenderCamera = pGraphics->createCamera();

	IGraphicsTexture *pTexture = pCore->graphicsEngine()->textureManager()->createTexture2D(pImg->data());
	pTexture->texture()->generateMipMaps();
	pTexture->texture()->setMinFilter(IGPUTexture::IGPUTextureMinFilter_LinearMipmapLinear);
	pTexture->texture()->setMagFilter(IGPUTexture::IGPUTextureMagFilter_Linear);

	IGraphicsTexture *pTexture2 = pCore->graphicsEngine()->textureManager()->createTexture2D(pImg2->data());
	pTexture2->texture()->generateMipMaps();
	pTexture2->texture()->setMinFilter(IGPUTexture::IGPUTextureMinFilter_LinearMipmapLinear);
	pTexture2->texture()->setMagFilter(IGPUTexture::IGPUTextureMagFilter_Linear);

	IGraphicsMaterial *pRenderMaterial = pGraphics->materialManager()->loadMaterial(Utility::File("vertex.glsl"), Utility::File("fragment.glsl"), "@utoname", &errorLog);
	Renderer::IGPUProgram *pProgram = pRenderMaterial->program();
	if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);
	pRenderMaterial->setUniform(pProgram->uniformLocationByName("tex"), pTexture->texture());

	IGraphicsMaterial *pRenderMaterial2 = pGraphics->materialManager()->loadMaterial(Utility::File("vertex.glsl"), Utility::File("fragment.glsl"), "@utoname", &errorLog);
	Renderer::IGPUProgram *pProgram2 = pRenderMaterial2->program();
	if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
	pRenderMaterial2->setAutoUniform(pProgram2->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);
	pRenderMaterial2->setUniform(pProgram2->uniformLocationByName("tex"), pTexture2->texture());

	IGraphicsModel *pRenderModel = pGraphics->createModel();
	pRenderModel->setMesh(pScene->data()->meshes[0]->pMeshData);
	pRenderModel->setMaterial(pRenderMaterial);

	IGraphicsModel *pRenderModel2 = pGraphics->createModel();
	pRenderModel2->setMesh(pScene2->data()->meshes[0]->pMeshData);
	pRenderModel2->setMaterial(pRenderMaterial2);

	IGraphicsScene *pRenderScene = pGraphics->createScene();
	IGraphicsSceneNode *pRootNode = pRenderScene->rootNode();

	const int c_n = 10;
	const float c_coef = 20.0f;
	for (int z = -c_n; z <= c_n; ++z)
		for (int x = -c_n; x <= c_n; ++x) {
			IGraphicsSceneNode *pNode = pRootNode->createChild();
			pNode->setModel((rand()%2) ? pRenderModel : pRenderModel2);
			pNode->setPosition(glm::vec3(x, -1, z) * c_coef);
			pNode->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
		}

	pRenderWindow = pCore->graphicsEngine()->createWindow(pCore->mainWindow());
	pRenderWindow->setCamera(pRenderCamera);
	pRenderWindow->setScene(pRenderScene);

	pCore->windowManager()->mainLoop();
	pCore->deinitialize();
	destroyCoreEngine();
	return 0;
}