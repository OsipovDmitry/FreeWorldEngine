#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;

float vertices[] = {0.0f, 0.0f, -1.0f,
					1.0f, 0.0f, -1.0f,
					0.0f, 1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,};

unsigned int indices[] = {0,1,2,3};

std::string vShader =
"#version 330 core\n"\
"layout(location = 0) in vec3 pos;\n"\
"out vec2 tc;\n"\
"void main(void)\n"\
"{\n"\
"	tc = pos.xy;\n"\
"	gl_Position = vec4 (pos, 1.0);\n"\
"}\n";

std::string fShader =
"#version 330 core\n"\
"uniform sampler2D tex;\n"\
"in vec2 tc;\n"\
"out vec4 outColor;\n"\
"void main(void)\n"\
"{\n"\
"	outColor = texture(tex, tc);\n"\
"}\n";

IGPUProgram *pProgram;
IGPUBufferContainer *pBCont;
IGPUTexture *pTexture;

void render() {
	getCoreEngine()->renderer()->tmp();
	getCoreEngine()->renderer()->setTexture(0, pTexture);
	getCoreEngine()->renderer()->renderIndexedGeometry(pProgram, pBCont, PrimitiveFormat_TriangleStrip, TYPE_UNSIGNED_INT_32, 4, 0);
}

int main() {
	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();

	p->logger()->printMessage("Hello!", ILogger::MessageType_Debug);

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


	IImage *pImg = p->imageLoader()->loadImage("img.jpg");
	
	uint32 offs[3] = {0,0,0};

	pTexture = p->renderer()->createTexture(IGPUTexture::IGPUTextureType_2D, pImg->raster()->size, TextureFormat(TextureFormat::PixelFormat_NormalizeUnsigned, TextureFormat::ChannelSize_8, TextureFormat::ChannelsCount_3));
	pTexture->setSubData(offs, pImg->raster()->size, TextureFormat::ChannelsCount_3, pImg->raster()->type, pImg->raster()->pData);

	IGPUBuffer *pVB = p->renderer()->createBuffer(12 * sizeof(float), IGPUBuffer::IGPUBufferUsage_StaticDraw, vertices);
	IGPUBuffer *pFB = p->renderer()->createBuffer(4 * sizeof(int), IGPUBuffer::IGPUBufferUsage_StaticDraw, indices);

	pBCont = p->renderer()->createBufferContainer();
	pBCont->setVertexAttribute(pVB, 0, 3, 0, 0, TYPE_FLOAT);
	pBCont->enableVertexAttribute(0);
	pBCont->setIndexBuffer(pFB);

	p->windowManager()->mainLoop();
	p->deinitialize();
	destroyCoreEngine();
	return 0;
}