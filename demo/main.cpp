#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;

void render() {
	getCoreEngine()->renderer()->setClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	getCoreEngine()->renderer()->clearBuffers(GL_COLOR_BUFFER_BIT);
}

void task(void *p)
{
	int *pi = (int*)p;
	getCoreEngine()->logger()->printMessage(std::to_string(*pi), ILog::MessageType(rand()%4));
}

int main() {
	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();

	IWindow *pMainWindow = p->mainWindow();
	if (pMainWindow)
		pMainWindow->setFuncRender(render);

	IThread *pThread = getCoreEngine()->threadManager()->createThread("thred0");

	const int N = 100000;
	int values[N];
	for (int i = 0; i < N; ++i) {
		values[i] = i;
		pThread->pushTaskToQueue(task, values+i);
	}
	pThread->start();

	p->windowManager()->mainLoop();
	p->deinitialize();
	destroyCoreEngine();
	return 0;
}