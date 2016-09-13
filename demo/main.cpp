#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;

void render() {
	getCoreEngine()->renderer()->setClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	getCoreEngine()->renderer()->clearBuffers(GL_COLOR_BUFFER_BIT);
}

int main() {
	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();
	p->mainWindow()->setFuncRender(render);

	p->windowManager()->mainLoop();
	p->deinitialize();

	return 0;
}