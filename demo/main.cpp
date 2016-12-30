#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;
using namespace Renderer;
#include <3rdparty/glm/gtc/matrix_transform.hpp>

float vertices[] = {-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					-1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 0.0f};

unsigned int indices[] = {0,1,2,3};

std::string vShader =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"uniform mat4 viewProjMatrix;\n"\
"out vec2 tc;\n"\
"void main(void)\n"\
"{\n"\
"	tc = pos.xy*0.5+vec2(0.5);\n"\
"	gl_Position = viewProjMatrix * vec4 (pos, 1.0);\n"\
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
"out vec4 outColor;\n"\
"void main(void)\n"\
"{\n"\
"	float zMin, zMax;\n"\
"	zMin = zMax = texture(texDepth, gl_FragCoord.xy).r;\n"\
"	for (int x = -1; x <= 1; ++x)\n"\
"		for (int y = -1; y <= 1; ++y) {\n"\
"			float zVal = texture(texDepth, gl_FragCoord.xy+vec2(x,y)).r;\n"\
"			if (zVal < zMin) zMin = zVal;\n"\
"			else if (zVal > zMax) zMax = zVal;\n"\
"		}\n"\
"	outColor = texture(tex, gl_FragCoord.xy);\n"\
"	if (abs(zMax-zMin) > 0.12) outColor = vec4(0,0,0,1);\n"\
"}\n";

IGPUProgram *pProgram;
IGPUBufferContainer *pBCont;
IGPUTexture *pTexture;

IGPUFrameBuffer *pFrameBuffer;
IGPUProgram *pProgramPost;
IGPUBufferContainer *pBContPost;
IGPUTexture *pTexturePost;
IGPUTexture *pDepthTexPost;

void render() {
	int w = getCoreEngine()->mainWindow()->width();
	int h = getCoreEngine()->mainWindow()->height();

	getCoreEngine()->renderer()->setViewport(0,0,w,h);
	getCoreEngine()->renderer()->enableDepthTest();
	getCoreEngine()->renderer()->setFrameBuffer(pFrameBuffer, 1);
	pFrameBuffer->clearDepthBuffer();
	pFrameBuffer->clearColorBuffer(0, 1,0,0,1);

	glm::mat4 vpMatrix;
	static float angle = 0.0f;
	vpMatrix = glm::perspective(45.0f, (float)w/(float)h, 3.0f, 7.0f) * glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.0f)) * glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	angle += 0.01f;
	pProgram->setUniform(pProgram->uniformLocationByName("viewProjMatrix"), vpMatrix);

	getCoreEngine()->renderer()->setTexture(0, pTexture);
	getCoreEngine()->renderer()->renderIndexedGeometry(pProgram, pBCont, PrimitiveFormat_TriangleStrip, TYPE_UNSIGNED_INT_32, 4, 0);
	
	getCoreEngine()->renderer()->setViewport(0,0,w,h);
	getCoreEngine()->renderer()->disableDepthTest();
	getCoreEngine()->renderer()->setFrameBuffer(0);
	getCoreEngine()->renderer()->setTexture(0, pTexturePost);
	getCoreEngine()->renderer()->setTexture(1, pDepthTexPost);
	getCoreEngine()->renderer()->renderIndexedGeometry(pProgramPost, pBContPost, PrimitiveFormat_TriangleStrip, TYPE_UNSIGNED_INT_32, 4, 0);
}

int main() {
	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();

	p->logger()->printMessage("Debug, Hello!", ILogger::MessageType_Debug);

	IScene *pScene = p->sceneLoader()->loadScene("AlexandrShabalin.3DS");

	IWindow *pMainWindow = p->mainWindow();
	if (pMainWindow)
		pMainWindow->setFuncRender(render);

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

	IImage *pImg = p->imageLoader()->loadImage("img.jpg");
	
	uint32 offs[3] = {0,0,0};

	pTexture = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg->raster()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture->setSubData(offs, pImg->raster()->size, TextureFormat::ChannelsCount_3, pImg->raster()->type, pImg->raster()->pData);

	uint32 texsize[2] = {getCoreEngine()->mainWindow()->width(), getCoreEngine()->mainWindow()->height()};
	pTexturePost = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pDepthTexPost = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_Rectangle, texsize, TextureFormat(TextureFormat::PixelFormat_SpecialDepth, TextureFormat::ChannelSize_32, TextureFormat::ChannelsCount_1));

	IGPUBuffer *pVB = p->renderer()->createBuffer(12 * sizeof(float), IGPUBuffer::IGPUBufferUsage_StaticDraw, vertices);
	IGPUBuffer *pFB = p->renderer()->createBuffer(4 * sizeof(int), IGPUBuffer::IGPUBufferUsage_StaticDraw, indices);
	pBCont = p->renderer()->createBufferContainer();
	pBCont->setVertexAttribute(pVB, 0, 3, 0, 0, TYPE_FLOAT);
	pBCont->enableVertexAttribute(0);
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