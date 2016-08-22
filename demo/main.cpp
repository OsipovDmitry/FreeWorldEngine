#include "FreeWorldEngine.h"
using namespace FreeWorldEngine;

int main() {
	initCoreEngine();
	ICore *p = getCoreEngine();
	p->initialize();
	p->windowManager()->mainLoop();
	p->deinitialize();

	return 0;
}