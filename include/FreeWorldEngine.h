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
#include "IContent.h"
#include "IContentLoader.h"

#include "renderer/IGPURenderer.h"

#include "graphics_engine/IGraphicsEngine.h"
#include "graphics_engine/IGraphicsCamera.h"
#include "graphics_engine/IGraphicsModel.h"
#include "graphics_engine/IGraphicsMaterialManager.h"
#include "graphics_engine/IGraphicsMaterial.h"
#include "graphics_engine/IGraphicsTextureManager.h"
#include "graphics_engine/IGraphicsTexture.h"
#include "graphics_engine/IGraphicsScene.h"
#include "graphics_engine/IGraphicsWindow.h"

#include "Types.h"
#include "CoreSettings.h"

namespace FreeWorldEngine {

CORE_FUNC_DLL bool initCoreEngine(int argc = 0, char **argv = nullptr); // Инициализация движка
CORE_FUNC_DLL ICore *getCoreEngine(); // Поучение синглтона движка
CORE_FUNC_DLL void destroyCoreEngine(); // Уничтожение движка

}


#endif // __FREEWORLDENGINE__