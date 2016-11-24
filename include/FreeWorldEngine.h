#ifndef __FREEWORLDENGINE__
#define __FREEWORLDENGINE__

#include "ICore.h"
#include "ILogger.h"
#include "ILog.h"
#include "IResource.h"
#include "IResourceManager.h"
#include "ILibraryManager.h"
#include "ILibrary.h"
#include "IPluginManager.h"
#include "IPlugin.h"
#include "IThreadManager.h"
#include "IThread.h"
#include "IWindowManager.h"
#include "IWindow.h"
#include "IImageLoader.h"
#include "IImage.h"
#include "renderer/IGPURenderer.h"
#include "CoreSettings.h"

namespace FreeWorldEngine {

CORE_FUNC_DLL bool initCoreEngine(); // ������������� ������
CORE_FUNC_DLL ICore *getCoreEngine(); // �������� ��������� ������
CORE_FUNC_DLL void destroyCoreEngine(); // ����������� ������

}


#endif // __FREEWORLDENGINE__