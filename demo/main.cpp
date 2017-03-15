#include <QApplication>
#include <QtOpenGL/qgl.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qobject.h>

#include <FreeWorldEngine.h>
using namespace FreeWorldEngine;

void paint(IWindow*) {
	glClearColor((float)rand()/(RAND_MAX-1.0f), (float)rand() / (RAND_MAX - 1.0f), (float)rand() / (RAND_MAX - 1.0f), 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//getCoreEngine()->renderer()->mainFrameBuffer()->clearColorBuffer(0, 0.5f, 0.5f, 1.0f, 1.0f);
	//getCoreEngine()->renderer()->mainFrameBuffer()->clearDepthBuffer(1.0f);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initCoreEngine(argc, argv);
    ICore *pCore = getCoreEngine();
    pCore->initialize();

    IWindow *pWindow = pCore->mainWindow();
	pWindow->registerRenderCallBack(paint);
	QWidget *pWidget = dynamic_cast<QWidget*>(pWindow);

	QPushButton button;
	button.setText("Push");
	button.setCheckable(true);
	button.setChecked(true);
	button.show();
	QObject::connect(&button, SIGNAL(toggled(bool)), pWidget, SLOT(repaint()));

    a.exec();

    pCore->deinitialize();
    destroyCoreEngine();

    return 0;
}
