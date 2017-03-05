#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
#include <3rdparty/glm/gtc/matrix_transform.hpp>

float vertices[] = {-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					-1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f};

unsigned int indices[] = {0,1,2,3};

Mesh* pMesh;

std::string vShader =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"layout(location = 1) in vec3 normal;\n"\
"layout(location = 2) in vec2 texcoord;\n"\
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
"	outColor = texture(tex, tc);\n"\
"}\n";

std::string vShaderPost =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"void main(void)\n"\
"{\n"\
"	gl_Position = vec4 (pos, 1.0);\n"\
"}\n";

std::string fShaderPost =
"#version 330 core\n"\
"uniform samplerRect tex;\n"\
"uniform samplerRect texDepth;\n"\
"uniform mat4 invMatrix;"
"out vec4 outColor;\n"\
"void main(void)\n"\
"{\n"\
"	float zMin, zMax;\n"\
"	zMin = zMax = texture(texDepth, gl_FragCoord.xy).r;\n"\
"	for (int x = -2; x <= 2; ++x)\n"\
"		for (int y = -2; y <= 2; ++y) {\n"\
"			float zVal = texture(texDepth, gl_FragCoord.xy+vec2(x,y)).r;\n"\
"			if (zVal < zMin) zMin = zVal;\n"\
"			else if (zVal > zMax) zMax = zVal;\n"\
"		}\n"\
"	outColor = texture(tex, gl_FragCoord.xy);\n"\
"	if (abs(zMax-zMin) > 0.15) outColor = vec4(0,0,0,1);\n"\
"}\n";

IGPUProgram *pProgram;
IGPUBufferContainer *pBCont;
IGPUTexture *pTexture;

IGPUFrameBuffer *pFrameBuffer;
IGPUProgram *pProgramPost;
IGPUBufferContainer *pBContPost;
IGPUTexture *pTexturePost;
IGPUTexture *pDepthTexPost;

void render(IWindow*) {
	int w = getCoreEngine()->mainWindow()->width();
	int h = getCoreEngine()->mainWindow()->height();

	getCoreEngine()->renderer()->setViewport(0,0,w,h);
	getCoreEngine()->renderer()->enableDepthTest();
	getCoreEngine()->renderer()->setFrameBuffer(pFrameBuffer, 1);
	pFrameBuffer->clearDepthBuffer();
	pFrameBuffer->clearColorBuffer(0, 1,0,0,1);

	glm::mat4 vpMatrix;
	static float angle = 0.0f;
	vpMatrix = glm::perspective(45.0f, (float)w/(float)h, 3.0f, 7.0f) *
		glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.0f)) *
		glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(), 1.7f*angle, glm::vec3(1.0f, 0.0f, 0.0f));
	angle += 0.01f;
	pProgram->setUniform(pProgram->uniformLocationByName("viewProjMatrix"), vpMatrix);

	getCoreEngine()->renderer()->setTexture(pTexture, 0);
	getCoreEngine()->renderer()->renderIndexedGeometry(pProgram, pBCont, pMesh->primitiveFormat, TYPE_UNSIGNED_INT_32, pMesh->numIndices, 0);

	getCoreEngine()->renderer()->setViewport(0,0,w,h);
	getCoreEngine()->renderer()->disableDepthTest();
	getCoreEngine()->renderer()->setFrameBuffer(0);
	getCoreEngine()->renderer()->setTexture(pTexturePost, 0);
	getCoreEngine()->renderer()->setTexture(pDepthTexPost, 1);
	getCoreEngine()->renderer()->renderIndexedGeometry(pProgramPost, pBContPost, PrimitiveFormat_TriangleStrip, TYPE_UNSIGNED_INT_32, 4, 0);
}

void resize(int32 w, int32 h, IWindow*) {
	pFrameBuffer->attachColorBuffer(0, (IGPUTexture*)0);
	pFrameBuffer->attachDepthBuffer((IGPUTexture*)0);
	getCoreEngine()->renderer()->destroyTexture(pTexturePost);
	getCoreEngine()->renderer()->destroyTexture(pDepthTexPost);
	getCoreEngine()->renderer()->destroyFrameBuffer(pFrameBuffer);

	const uint32 texsize[2] = { w + (4 - w%4)%4, h + (4 - h%4)%4 };
	pTexturePost = getCoreEngine()->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pDepthTexPost = getCoreEngine()->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_SpecialDepth, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1));
	pFrameBuffer = getCoreEngine()->renderer()->createFrameBuffer();
	pFrameBuffer->attachColorBuffer(0, pTexturePost);
	pFrameBuffer->attachDepthBuffer(pDepthTexPost);
}

#include <utility/Tree.h>
#include <iostream>

int main() {
	Utility::Tree<int> tree(1);
	Utility::TreeNode<int> *c2 = tree.rootNode()->addChild(2);
	Utility::TreeNode<int> *c12 = tree.rootNode()->addChild(12);
	Utility::TreeNode<int> *c13 = tree.rootNode()->addChild(13);
	Utility::TreeNode<int> *c3 = c2->addChild(3);
	Utility::TreeNode<int> *c10 = c2->addChild(10);
	Utility::TreeNode<int> *c4 = c3->addChild(4);
	Utility::TreeNode<int> *c5 = c3->addChild(5);
	Utility::TreeNode<int> *c6 = c3->addChild(6);
	Utility::TreeNode<int> *c7 = c6->addChild(7);
	Utility::TreeNode<int> *c8 = c6->addChild(8);
	Utility::TreeNode<int> *c9 = c8->addChild(9);
	Utility::TreeNode<int> *c11 = c10->addChild(11);
	Utility::TreeNode<int> *c14 = c13->addChild(14);
	Utility::TreeNode<int> *c15 = c13->addChild(15);
	Utility::TreeNode<int> *c16 = c15->addChild(16);
	Utility::TreeNode<int> *c17 = c15->addChild(17);
	Utility::TreeNode<int> *c18 = c17->addChild(18);
	Utility::TreeNode<int> *c19 = c17->addChild(19);
	Utility::TreeNode<int> *c20 = c17->addChild(20);

	for (auto it = tree.beginDepth(); it != tree.endDepth(); it++)
		std::cout << (*it)->data() << " ";

	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();

	p->logger()->printMessage("Debug, Hello!", ILogger::MessageType_Debug);

	IWindow *pMainWindow = p->mainWindow();
	if (pMainWindow) {
		pMainWindow->registerRenderCallBack(render);
		pMainWindow->registerResizeCallBack(resize);
	}

	std::string sLog="";

	IGPUShader *pVSh = p->renderer()->createShader(IGPUShader::IGPUShaderType_Vertex);
	pVSh->loadFromData(vShader);
	if (!pVSh->compile(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	IGPUShader *pFSh = p->renderer()->createShader(IGPUShader::IGPUShaderType_Fragment);
	pFSh->loadFromData(fShader);
	if (!pFSh->compile(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	pProgram = p->renderer()->createProgram();
	pProgram->attachShader(pVSh);
	pProgram->attachShader(pFSh);
	if (!pProgram->link(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	pProgram->setUniform(pProgram->uniformLocationByName("tex"), 0);

	IGPUShader *pVShPost = p->renderer()->createShader(IGPUShader::IGPUShaderType_Vertex);
	pVShPost->loadFromData(vShaderPost);
	if (!pVShPost->compile(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	IGPUShader *pFShPost = p->renderer()->createShader(IGPUShader::IGPUShaderType_Fragment);
	pFShPost->loadFromData(fShaderPost);
	if (!pFShPost->compile(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	pProgramPost = p->renderer()->createProgram();
	pProgramPost->attachShader(pVShPost);
	pProgramPost->attachShader(pFShPost);
	if (!pProgramPost->link(&sLog)) {
		p->logger()->printMessage(sLog, ILogger::MessageType_Error);
	}
	pProgramPost->setUniform(pProgramPost->uniformLocationByName("tex"), 0);
	pProgramPost->setUniform(pProgramPost->uniformLocationByName("texDepth"), 1);

	IImage *pImg = p->imageLoader()->load("box.jpg");

	uint32 offs[3] = {0,0,0};

	pTexture = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg->data()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture->setSubData(offs, pImg->data()->size, TextureFormat::ChannelsCount_3, pImg->data()->type, pImg->data()->pData);

	uint32 texsize[2] = {getCoreEngine()->mainWindow()->width(), getCoreEngine()->mainWindow()->height()};
	pTexturePost = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pDepthTexPost = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_SpecialDepth, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1));

	IScene *pScene = p->sceneLoader()->load("box.FBX");
	pMesh = pScene->data()->meshes[0]->pMeshData;

	IGPUBuffer *pVB = p->renderer()->createBuffer(pMesh->vertexStride * pMesh->numVertices * sizeof(float), IGPUBuffer::IGPUBufferUsage_StaticDraw, pMesh->pVertexData);
	IGPUBuffer *pFB = p->renderer()->createBuffer(pMesh->numIndices * sizeof(int), IGPUBuffer::IGPUBufferUsage_StaticDraw, pMesh->pIndexData);
	pBCont = p->renderer()->createBufferContainer();
	pBCont->setVertexAttribute(pVB, 0, pMesh->attributes[VertexAttributeType_Position].first, pMesh->attributes[VertexAttributeType_Position].second * sizeof(float), pMesh->vertexStride * sizeof(float), TYPE_FLOAT);
	pBCont->enableVertexAttribute(0);
	pBCont->setVertexAttribute(pVB, 1, pMesh->attributes[VertexAttributeType_Normal].first, pMesh->attributes[VertexAttributeType_Normal].second * sizeof(float), pMesh->vertexStride * sizeof(float), TYPE_FLOAT);
	pBCont->enableVertexAttribute(1);
	pBCont->setVertexAttribute(pVB, 2, pMesh->attributes[VertexAttributeType_TexCoord0].first, pMesh->attributes[VertexAttributeType_TexCoord0].second * sizeof(float), pMesh->vertexStride * sizeof(float), TYPE_FLOAT);
	pBCont->enableVertexAttribute(2);
	pBCont->setIndexBuffer(pFB);

	IGPUBuffer *pVBPost = p->renderer()->createBuffer(12 * sizeof(float), IGPUBuffer::IGPUBufferUsage_StaticDraw, vertices);
	IGPUBuffer *pFBPost = p->renderer()->createBuffer(4 * sizeof(int), IGPUBuffer::IGPUBufferUsage_StaticDraw, indices);
	pBContPost = p->renderer()->createBufferContainer();
	pBContPost->setVertexAttribute(pVBPost, 0, 3, 0, 0, TYPE_FLOAT);
	pBContPost->enableVertexAttribute(0);
	pBContPost->setIndexBuffer(pFBPost);

	pFrameBuffer = p->renderer()->createFrameBuffer();
	pFrameBuffer->attachColorBuffer(0, pTexturePost);
	pFrameBuffer->attachDepthBuffer(pDepthTexPost);
	
	p->windowManager()->mainLoop();
	p->deinitialize();
	destroyCoreEngine();
	return 0;
}