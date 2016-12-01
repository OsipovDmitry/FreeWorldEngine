set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin) 

include_directories(../../3rdparty/include)
include_directories(../../include)
link_directories(${LIBRARY_OUTPUT_PATH})

set(HEADERS TerminalLog.h
			HtmlLog.h
            LoggerPlugin.h)

set(INTERFACE_HEADERS
	../../include/ILog.h
    ../../include/ILogger.h)

source_group("Interface Headers" FILES ${INTERFACE_HEADERS})

set(SOURCES TerminalLog.cpp
			HtmlLog.cpp
			LoggerPlugin.cpp
            main.cpp)

add_library(LoggerPlugin SHARED ${HEADERS} ${INTERFACE_HEADERS} ${SOURCES})
	
target_link_libraries(LoggerPlugin CoreEngine)
target_link_libraries(LoggerPlugin FWEUtility)
