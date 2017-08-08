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
IGraphicsScene *pRenderScene;
IGraphicsWindow *pRenderWindow = 0;
IGraphicsCamera *pRenderCamera = 0;
const int numLights = 4;
IGraphicsLight* lights[numLights];
glm::vec3 lightsPos[numLights];
IGraphicsMaterial *pRenderMaterial = 0, *pRenderMaterial2 = 0;
std::string errorLog;
float cam_lat = 0, cam_long = 0;

IGraphicsSceneNode *pNode01, *pNode02, *pNode011, *pNode012, *pNode013, *pNode014, *pNode021, *pNode022, *pNode023, *pNode024;

float my_random(float a = 0.0f, float b = 1.0f) {
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

	float cam_move_speed = 50.0f;
	if (pCore->windowManager()->keyState(IWindow::KeyCode_LeftShift))
		cam_move_speed = 300.0f;

	glm::vec3 cam_pos = pRenderCamera->position();
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

//	pNode01->setOrientation(glm::quat(glm::vec3(0.0f, 0.0f, -glm::pi<float>()*0.2f*time*0.001f)));
//	pNode02->setOrientation(glm::quat(glm::vec3(0.0f, glm::pi<float>()*0.1f*time*0.001f, 0.0f)));

	//pRenderScene->rootNode()->setOrientation(glm::quat(glm::vec3(0.0f, -glm::pi<float>()*0.02f*time*0.001f, 0.0f)));

	for (int i = 0; i < numLights; ++i) {
		lights[i]->setPosition(lightsPos[i] + glm::vec3(100 * cos(time*0.001f), 0, 100 * sin(time*0.001f)));
	}
}

void keyDown(IWindow::KeyCode keyCode, IWindow*)
{
}

int main() {
	Utility::Time t = Utility::Time::current();
	srand(t.hours * 3600 + t.minutes * 60 + t.seconds);

	pCore = ICore::instance();
	pCore->initialize();

	pCore->mainWindow()->registerUpdateCallBack(update);
	pCore->mainWindow()->registerKeyDownCallBack(keyDown);

	IImage *pImg = pCore->imageLoader()->load("./mi24.jpg");
	IImage *pImg2 = pCore->imageLoader()->load("./c300.jpg");

	IScene *pScene = pCore->sceneLoader()->load("./mi24.3ds");
	IScene *pScene2 = pCore->sceneLoader()->load("./c300.3ds");

	IGraphicsEngine *pGraphics = pCore->graphicsEngine();

	IGraphicsTexture *pTexture = pCore->graphicsEngine()->textureManager()->createTexture2D(pImg->data());
	pTexture->setFilterMode(IGraphicsTexture::FilterMode_TriLinear);

	IGraphicsTexture *pTexture2 = pCore->graphicsEngine()->textureManager()->createTexture2D(pImg2->data());
	pTexture2->setFilterMode(IGraphicsTexture::FilterMode_TriLinear);

	pRenderMaterial = pGraphics->materialManager()->loadMaterial(Utility::File("vertex.glsl"), Utility::File("fragment.glsl"), "@utoname", &errorLog);
	if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
	Renderer::IGPUProgram *pProgram = pRenderMaterial->program();
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("viewProjectionMatrix"), IGraphicsMaterial::AutoUniform_ViewProjectionMatrix);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("modelMatrix"), IGraphicsMaterial::AutoUniform_ModelMatrix);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("normalMatrix"), IGraphicsMaterial::AutoUniform_NormalMatrix);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("cameraPos"), IGraphicsMaterial::AutoUniform_CameraPosition);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("light[0]"), IGraphicsMaterial::AutoUniform_Light0);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("light[1]"), IGraphicsMaterial::AutoUniform_Light1);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("light[2]"), IGraphicsMaterial::AutoUniform_Light2);
	pRenderMaterial->setAutoUniform(pProgram->uniformLocationByName("light[3]"), IGraphicsMaterial::AutoUniform_Light3);
	pRenderMaterial->setUniform(pProgram->uniformLocationByName("tex"), pTexture->texture());

	pRenderMaterial2 = pGraphics->materialManager()->loadMaterial(Utility::File("vertex.glsl"), Utility::File("fragment.glsl"), "@utoname", &errorLog);
	Renderer::IGPUProgram *pProgram2 = pRenderMaterial2->program();
	if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("viewProjectionMatrix"), IGraphicsMaterial::AutoUniform_ViewProjectionMatrix);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("modelMatrix"), IGraphicsMaterial::AutoUniform_ModelMatrix);
	pRenderMaterial2->setAutoUniform(pProgram2->uniformLocationByName("normalMatrix"), IGraphicsMaterial::AutoUniform_NormalMatrix);
	pRenderMaterial2->setAutoUniform(pProgram2->uniformLocationByName("cameraPos"), IGraphicsMaterial::AutoUniform_CameraPosition);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("light[0]"), IGraphicsMaterial::AutoUniform_Light0);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("light[1]"), IGraphicsMaterial::AutoUniform_Light1);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("light[2]"), IGraphicsMaterial::AutoUniform_Light2);
	pRenderMaterial2->setAutoUniform(pProgram->uniformLocationByName("light[3]"), IGraphicsMaterial::AutoUniform_Light3);
	pRenderMaterial2->setUniform(pProgram2->uniformLocationByName("tex"), pTexture2->texture());

	const int32 NUM_COLOR_MATS = 10;
	IGraphicsMaterial *pColorMaterial[NUM_COLOR_MATS];
	for (int i = 0; i < NUM_COLOR_MATS; ++i) {
		pColorMaterial[i] = pGraphics->materialManager()->loadMaterial(Utility::File("color_material_vertex.glsl"), Utility::File("color_material_fragment.glsl"), "@utoname", &errorLog);
		if (!errorLog.empty()) pCore->logger()->printMessage(errorLog, ILogger::MessageType_Error);
		Renderer::IGPUProgram *pProgram = pColorMaterial[i]->program();
		pColorMaterial[i]->setAutoUniform(pProgram->uniformLocationByName("modelViewProjMatrix"), IGraphicsMaterial::AutoUniform_ModelViewProjectionMatrix);
		pColorMaterial[i]->setUniform(pProgram->uniformLocationByName("color"), glm::vec4(my_random(), my_random(), my_random(), 0.6f));
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

//	Math::MeshWrapper quadMesh(new Mesh);
//	quadMesh.setPrimitiveFormat(PrimitiveFormat_Triangles);
//	quadMesh.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
//	quadMesh.setAttributeDeclaration(VertexAttributeType_TexCoord0, 2, 3);
//	quadMesh.setVertexStride(5);
//	Math::MeshPainter(quadMesh).paintQuad(20, 20);

//	IGraphicsModel *pColorModel[NUM_COLOR_MATS];
//	for (int i = 0; i < NUM_COLOR_MATS; ++i) {
//		pColorModel[i] = pGraphics->createModel();
//		pColorModel[i]->setMesh(quadMesh.target());
//		pColorModel[i]->setMaterial(pColorMaterial[i]);
//	}

//	quadMesh.destroyTarget();


	pRenderScene = pGraphics->sceneManager()->createScene("@utoname", IGraphicsScene::SceneOptimizerType_Octree);
	IGraphicsSceneNode *pRootNode = pRenderScene->rootNode();

	pRenderCamera = pGraphics->createCamera();
	pRenderCamera->setPosition(glm::vec3(0,1,0));

	Math::MeshWrapper quadMesh(new Mesh);
	quadMesh.setPrimitiveFormat(PrimitiveFormat_Triangles);
	quadMesh.setAttributeDeclaration(VertexAttributeType_Position, 3, 0);
	quadMesh.setAttributeDeclaration(VertexAttributeType_Tangent, 3, 3);
	quadMesh.setAttributeDeclaration(VertexAttributeType_Binormal, 3, 6);
	quadMesh.setAttributeDeclaration(VertexAttributeType_Normal, 3, 9);
	quadMesh.setAttributeDeclaration(VertexAttributeType_TexCoord0, 2, 12);
	quadMesh.setVertexStride(14);
	Math::MeshPainter(quadMesh).paintQuad(1000, 1000);

	IGraphicsModel *pModel = pGraphics->createModel();
	pModel->setMesh(quadMesh.target());
	pModel->setMaterial(pRenderMaterial);

	quadMesh.destroyTarget();

	IGraphicsSceneNode *pNode = pRenderScene->createNode();
	pNode->setModel(pModel);
	pNode->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
	pNode->setPosition(glm::vec3(0,0,0));
	pRootNode->attachChildNode(pNode);

	/*for (int i = 0; i < 30; ++i) {
		IGraphicsSceneNode *pNode = pRenderScene->createNode();
		pNode->setPosition(glm::vec3(random(-300, 300), random(-300, 300), random(-300, 300)));
		pNode->setModel(pRenderModel);
		pRootNode->attachChildNode(pNode);
	}

	IGraphicsSceneNode *pNode = pRenderScene->createNode();
	pNode->setModel(pRenderModel);
	pNode->setPosition(glm::vec3(20, 0, 0));
	pRootNode->attachChild(pNode);
	*/

	int32 s1 = Utility::DateTime::current().time.seconds;

	const int c_n = 7;
	const float c_coef = 60.0f;
	for (int z = -c_n; z <= c_n; ++z)
		for (int y = -c_n; y <= c_n; ++y) {
			if (y == 0)
				continue;
			for (int x = -c_n; x <= c_n; ++x) {
				IGraphicsSceneNode *pNode = pRenderScene->createNode();
				pNode->setModel((rand() % 2) ? pRenderModel : pRenderModel2);
				pNode->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
				pNode->setPosition(glm::vec3(x + my_random(-0.5f, 0.5f), y + my_random(-0.5f, 0.5f), z + my_random(-0.5f, 0.5f)) * c_coef);
				pRootNode->attachChildNode(pNode);
			}
		}

	for (int i = 0; i < numLights; ++i) {
		lights[i] = pRenderScene->createLight(GraphicsEngine::IGraphicsLight::Type_Point);
		lights[i]->setColor(glm::vec3(my_random(0.5f, 1.0f), my_random(0.5f, 1.0f), my_random(0.5f, 1.0f)));
		lightsPos[i] = glm::vec3(my_random(-350,350), 50, my_random(-350,350));
		lights[i]->setPosition(lightsPos[i]);
		lights[i]->setDirection(glm::vec3(1.0f, -0.5f, -1.0f));
		lights[i]->setAttenuationRadius(100, 150);
		lights[i]->setSpotAngles(0.5f, 0.6f);
	}

//	lightsPos[0] = glm::vec3(-200, 50, -200);
//	lightsPos[1] = glm::vec3(-200, 50, +200);
//	lightsPos[2] = glm::vec3(+200, 50, -200);
//	lightsPos[3] = glm::vec3(+200, 50, +200);

//	const float sz = 200;
//	for (int i = 0; i < 200; ++i) {
//		IGraphicsSceneNode *pNode = pRenderScene->createNode();
//		pNode->setModel((rand() % 2) ? pRenderModel : pRenderModel2);
//		pNode->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//		pNode->setPosition(glm::vec3(my_random(-sz, sz), my_random(-sz, sz), my_random(-sz, sz)));
//		pRootNode->attachChildNode(pNode);
//	}

	
//	for (int z = -c_n; z <= c_n; ++z)
//		for (int x = -c_n; x <= c_n; ++x) {
//			IGraphicsSceneNode *pNode = pRenderScene->createNode();
//			pNode->setModel(pColorModel[rand()%NUM_COLOR_MATS]);
//			pNode->setOrientation(glm::quat(glm::vec3(0.0f, my_random(0.0f, glm::half_pi<float>()), 0.0f)));
//			pNode->setPosition(glm::vec3(x + my_random(-0.2f, 0.2f), 0.0f, z + my_random(-0.2f, 0.2f)) * c_coef);
//			pRootNode->attachChildNode(pNode);
//		}

//	float c_step = 25.0f;
	
//	pRootNode->setModel(pRenderModel);
	
//	for (int i = 0; i < 15; ++i) {
//		IGraphicsSceneNode *pNode = pRenderScene->createNode();
//		pNode->setPosition(glm::vec3(0.0f, i, 0.0f)*c_step);
//		pNode->setModel(pRenderModel);
//		pRootNode->attachChildNode(pNode);
//	}

//	pNode01 = pRenderScene->createNode();
//	pNode01->setPosition(glm::vec3(6.0f, -1.0f, -4.0f)*c_step);
//	pRootNode->attachChildNode(pNode01);
	
//	pNode011 = pRenderScene->createNode();
//	pNode011->setPosition(glm::vec3(1.0f, 0.0f, -1.0f) * c_step);
//	pNode011->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode011->setModel(pRenderModel);
//	pNode01->attachChildNode(pNode011);

//	pNode012 = pRenderScene->createNode();
//	pNode012->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f) * c_step);
//	pNode012->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode012->setModel(pRenderModel);
//	pNode01->attachChildNode(pNode012);

//	pNode013 = pRenderScene->createNode();
//	pNode013->setPosition(glm::vec3(0.0f, 0.0f, 0.5f) * c_step);
//	pNode013->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode013->setModel(pRenderModel);
//	pNode01->attachChildNode(pNode013);

//	pNode014 = pRenderScene->createNode();
//	pNode014->setPosition(glm::vec3(-0.5f, 0.0f, 0.5f) * c_step);
//	pNode014->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode014->setModel(pRenderModel);
//	pNode01->attachChildNode(pNode014);

//	pNode02 = pRenderScene->createNode();
//	pNode02->setPosition(glm::vec3(-4.0, 1.0f, 4.0f)*c_step);
//	pRootNode->attachChildNode(pNode02);

//	pNode021 = pRenderScene->createNode();
//	pNode021->setPosition(glm::vec3(1.0f, 0.0f, -1.0f) * c_step);
//	pNode021->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode021->setModel(pRenderModel2);
//	pNode02->attachChildNode(pNode021);

//	pNode022 = pRenderScene->createNode();
//	pNode022->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f) * c_step);
//	pNode022->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode022->setModel(pRenderModel2);
//	pNode02->attachChildNode(pNode022);

//	pNode023 = pRenderScene->createNode();
//	pNode023->setPosition(glm::vec3(0.0f, 0.0f, 0.5f) * c_step);
//	pNode023->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode023->setModel(pRenderModel2);
//	pNode02->attachChildNode(pNode023);

//	pNode024 = pRenderScene->createNode();
//	pNode024->setPosition(glm::vec3(-0.5f, 0.0f, 0.5f) * c_step);
//	pNode024->setOrientation(glm::quat(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f)));
//	pNode024->setModel(pRenderModel2);
//	pNode02->attachChildNode(pNode024);
		
	s1 = Utility::DateTime::current().time.seconds - s1;
	if (s1 < 0)
		s1 += 60;

	pCore->logger()->printMessage(std::to_string(s1));

	pRenderWindow = pCore->graphicsEngine()->createWindow(pCore->mainWindow());
	pRenderWindow->setCamera(pRenderCamera);
	pRenderWindow->setScene(pRenderScene);

	pCore->windowManager()->mainLoop();
	pCore->deinitialize();
	return 0;
}
