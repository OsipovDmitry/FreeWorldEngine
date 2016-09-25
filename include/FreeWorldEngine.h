#ifndef __FREEWORLDENGINE__
#define __FREEWORLDENGINE__

#include "ICore.h"
#include "ILogger.h"
#include "IResource.h"
#include "IResourceManager.h"
#include "IWindowManager.h"
#include "IWindow.h"
#include "IImageManager.h"
#include "IImage.h"
#include "renderer/IGLRenderer.h"
#include "CoreSettings.h"

namespace FreeWorldEngine {

CORE_FUNC_DLL bool initCoreEngine(); // ������������� ������
CORE_FUNC_DLL ICore *getCoreEngine(); // �������� ��������� ������
CORE_FUNC_DLL void destroyCoreEngine(); // ����������� ������

}


#endif // __FREEWORLDENGINE__