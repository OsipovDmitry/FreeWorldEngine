set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(INTERFACE_HEADERS
	../include/core/ICore.h
	../include/core/ILibrary.h
	../include/core/ILibraryManager.h
	../include/core/ILog.h
	../include/core/ILogger.h
	../include/core/IPlugin.h
	../include/core/IPluginManager.h
	../include/core/IResource.h
	../include/core/IResourceManager.h
	../include/core/IThread.h
	../include/core/IThreadManager.h
	../include/core/IWindow.h
	../include/core/IWindowManager.h
	../include/core/IContent.h
	../include/core/IContentLoader.h
	../include/core/CoreSettings.h
)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(COMMON_HEADERS
    ../include/FreeWorldEngine.h
	../include/Platform.h
    ../include/Types.h
)

source_group("Common Headers" FILES ${COMMON_HEADERS})

set(HEADERS
    Core.h
	Library.h
	LibraryManager.h
    Logger.h
    PluginManager.h
    ResourceManager.h
    Thread.h
	ThreadManager.h
	DataLoader.h
	ContentLoader.h
	Content.h
)

set(SOURCES
    Core.cpp
	Library.cpp
	LibraryManager.cpp
    Logger.cpp
    PluginManager.cpp
    ResourceManager.cpp
    Thread.cpp
    ThreadManager.cpp
)

add_definitions(-DCORE_EXPORT)

add_library(CoreEngine SHARED ${INTERFACE_HEADERS} ${COMMON_HEADERS} ${HEADERS} ${SOURCES})

target_link_libraries(CoreEngine FWEUtility)
