#ifndef __FREEWORLDENGINE__
#define __FREEWORLDENGINE__

#include "ICore.h"
#include "ILog.h"
#include "IResource.h"
#include "IResourceManager.h"
#include "IWindowManager.h"
#include "IWindow.h"
#include "renderer/IGLRenderer.h"
#include "CoreSettings.h"

namespace FreeWorldEngine {

CORE_FUNC_DLL bool initCoreEngine(); // Инициализация движка
CORE_FUNC_DLL ICore *getCoreEngine(); // Поучение синглтона движка
CORE_FUNC_DLL void destroyCoreEngine(); // Уничтожение движка

}


#endif // __FREEWORLDENGINE__