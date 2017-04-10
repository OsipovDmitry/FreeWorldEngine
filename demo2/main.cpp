#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
using namespace GraphicsEngine;
#include <3rdparty/glm/gtc/matrix_transform.hpp>
#include <utility/File.h>
#include <utility/DateTime.h>
#include <math/MeshPainter.h>
#include <math/MeshWrapper.h>

ICore *pCore = 0;
IGraphicsWindow *pRenderWindow = 0;
IGraphicsCamera *pRenderCamera = 0;
float cam_lat = 0, cam_long = 0;
glm::vec3 cam_pos(0.0f, 0.0f, 0.0f);
std::string errorLog;

IGraphicsSceneNode *pNode01, *pNode02, *pNode011, *pNode012, *pNode013, *pNode021, *pNode022, *pNode023;

float random(float a = 0.0f, float b = 1.0f) {
	return a + (b - a) * (float)rand() / (float)(RAND_MAX-1);
}

void update(uint32 time, uint32 dt, IWindow*) {
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Left))
		cam_long -= glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Right))
		cam_long += glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Up))
		cam_lat -= glm::half_pi<float>() * dt * 0.001f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_Down))
		cam_lat += glm::half_pi<float>() * dt * 0.001f;

	float cam_move_speed = 150.0f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_LeftShift))
		cam_move_speed = 300.0f;

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

	//pNode01->setOrientation(glm::quat(glm::vec3(0.0f, 0.0f, -glm::pi<float>()*0.2f*time*0.001f)));
	//pNode02->setOrientation(glm::quat(glm::vec3(0.0f, glm::pi<float>()*0.1f*time*0.001f, 0.0f)));
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
	pTexture->setFilterMode(IGraphicsTexture::FilterMode_TriLinear);

	IGraphicsTexture *pTexture2 = pCore->graphicsEngine()->textureManager()->createTexture2D(pImg2->data());
	pTexture2->setFilterMode(IGraphicsTexture::FilterMode_TriLinear);

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

	const int32 NUM_COLOR_MATS = 10;
	IGraphicsMaterial *pColorMaterial[NUM_COLOR_MATS];
	for (int i = 0; i < NUM_COLOR_MATS; ++i) {
		pColorMaterial[i] = pGraphics->materialManager()->loadMaterial(Utility::File("color_material_vertex.glsl"), Utility::File("color_material_fragment.glsl"), "@utoname", &errorLog);
		if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
		Renderer::IGPUProgram *pProgram = pColorMaterial[i]->program();
		pColorMaterial[i]->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);
		pColorMaterial[i]->setUniform(pProgram->uniformLocationByName("color"), glm::vec4(random(), random(), random(), 0.6f));
		pColorMaterial[i]->setBlendFunc(IGraphicsMaterial::BlendFuncState_Alpha);
		pColorMaterial[i]->setCullFaceState(IGraphicsMaterial::CullFaceState_Disabled);
		pColorMaterial[i]->setDepthWrite(false);
		pColorMaterial[i]->setTag(IGraphicsMaterial::Tag_Transparent);
	}

	IGraphicsModel *pRenderModel = pGraphics->createModel();
	pRenderModel->setMesh(pScene->data()->meshes[0]->pMeshData);
	pRenderModel->setMaterial(pRenderMaterial);

	IGraphicsModel *pRenderModel2 = pGraphics->createModel();
	pRenderModel2->setMesh(pScene2->data()->meshes[0]->pMeshData);
	pRenderModel2->setMaterial(pRenderMaterial2);

	Math::MeshWrapper quadMesh(new Mesh);
	quadMesh.setPrimitiveFormat(PrimitiveFormat_Triangles);
	quadMesh.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	quadMesh.setAttributeDeclaration(VertexAttributeType_TexCoord0, 2, 3);
	quadMesh.setVertexStride(5);
	Math::MeshPainter(quadMesh).paintQuad(20, 20);

	IGraphicsModel *pColorModel[NUM_COLOR_MATS];
	for (int i = 0; i < NUM_COLOR_MATS; ++i) {
		pColorModel[i] = pGraphics->createModel();
		pColorModel[i]->setMesh(quadMesh.target());
		pColorModel[i]->setMaterial(pColorMaterial[i]);
	}

	quadMesh.destroyTarget();


	IGraphicsScene *pRenderScene = pGraphics->sceneManager()->createScene();
	IGraphicsSceneNode *pRootNode = pRenderScene->rootNode();

	int32 s1 = Utility::DateTime::current().time.seconds;

	const int c_n = 7;
	const float c_coef = 60.0f;
	for (int z = -c_n; z <= c_n; ++z)
		for (int y = -c_n; y <= c_n; ++y) {
			if (y == 0)
				continue;
			for (int x = -c_n; x <= c_n; ++x) {
				IGraphicsSceneNode *pNode = pRootNode->createChild();
				pNode->setModel((rand() % 2) ? pRenderModel : pRenderModel2);
				pNode->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
				pNode->setPosition((glm::vec3(x, y, z) + glm::vec3(0.4, -0.3, 0.7)) * c_coef);
			}
		}

	/*
	for (int z = -c_n; z <= c_n; ++z)
		for (int x = -c_n; x <= c_n; ++x) {
			IGraphicsSceneNode *pNode = pRootNode->createChild();
			pNode->setModel(pColorModel[rand()%NUM_COLOR_MATS]);
			pNode->setOrientation(glm::quat(glm::vec3(0.0f, random(0.0f, glm::pi<float>()), 0.0f)));
			pNode->setPosition(glm::vec3(x + random(-0.2f, 0.2f), 0.0f, z + random(-0.2f, 0.2f)) * c_coef);
		}
	*/

	/*float c_step = 25.0f;
	pNode01 = pRootNode->createChild();
	pNode01->setPosition(glm::vec3(6.0f, -1.0f, -4.0f)*c_step);

	pNode02 = pRootNode->createChild();
	pNode02->setPosition(glm::vec3(-4.0, 1.0f, 4.0f)*c_step);

	pNode011 = pNode01->createChild();
	pNode011->setPosition(glm::vec3(1.0f, 0.0f, -1.0f) * c_step);
	pNode011->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode011->setModel(pRenderModel);

	pNode012 = pNode01->createChild();
	pNode012->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f) * c_step);
	pNode012->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode012->setModel(pRenderModel);

	pNode013 = pNode01->createChild();
	pNode013->setPosition(glm::vec3(0.0f, 0.0f, 0.5f) * c_step);
	pNode013->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode013->setModel(pRenderModel);

	pNode021 = pNode02->createChild();
	pNode021->setPosition(glm::vec3(1.0f, 0.0f, -1.0f) * c_step);
	pNode021->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode021->setModel(pRenderModel2);

	pNode022 = pNode02->createChild();
	pNode022->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f) * c_step);
	pNode022->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode022->setModel(pRenderModel2);

	pNode023 = pNode02->createChild();
	pNode023->setPosition(glm::vec3(0.0f, 0.0f, 0.5f) * c_step);
	pNode023->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode023->setModel(pRenderModel2);*/
		
	s1 = Utility::DateTime::current().time.seconds - s1;
	if (s1 < 0)
		s1 += 60;

	pCore->logger()->printMessage(std::to_string(s1));

	pRenderWindow = pCore->graphicsEngine()->createWindow(pCore->mainWindow());
	pRenderWindow->setCamera(pRenderCamera);
	pRenderWindow->setScene(pRenderScene);

	pCore->windowManager()->mainLoop();
	pCore->deinitialize();
	destroyCoreEngine();
	return 0;
}