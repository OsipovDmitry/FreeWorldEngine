set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../3rdparty/include)
include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACES ../include/ICamera.h
               ../include/ICore.h
               ../include/IImage.h
               ../include/IImageManager.h
               ../include/ILibrary.h
               ../include/ILibraryManager.h
               ../include/ILog.h
               ../include/ILogger.h
               ../include/IMeshBuilder.h
               ../include/IMovableObject.h
               ../include/IPhysicsBody.h
               ../include/IPhysicsGeometry.h
               ../include/IPhysicsManager.h
               ../include/IPhysicsMaterial.h
               ../include/IPhysicsMaterialManager.h
               ../include/IPhysicsSpace.h
               ../include/IPlugin.h
               ../include/IPluginManager.h
               ../include/IResource.h
               ../include/IResourceManager.h
               ../include/ISceneModel.h
               ../include/ISceneModelManager.h
               ../include/IThread.h
               ../include/IThreadManager.h
               ../include/IWindow.h
               ../include/IWindowManager.h)

set(COMMON_HEADERS ../include/CoreSettings.h
                   ../include/FreeWorldEngine.h
                   ../include/Types.h)

set(HEADERS Core.h
            Library.h
            LibraryManager.h
            Logger.h
            TextFileLog.h
            PluginManager.h
            ResourceManager.h)

set(SOURCES Core.cpp
            Library.cpp
            LibraryManager.cpp
            Logger.cpp
            TextFileLog.cpp
            PluginManager.cpp
            ResourceManager.cpp)

add_definitions(-DCORE_EXPORT)

add_library(CoreEngine SHARED ${INTERFACES} ${COMMON_HEADERS} ${HEADERS} ${SOURCES})

target_link_libraries(CoreEngine FWEUtility)
